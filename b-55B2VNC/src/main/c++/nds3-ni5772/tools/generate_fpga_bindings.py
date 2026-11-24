#!/usr/bin/env python3
"""Validate the real DiamondDetectorDAQ C API header and generate C++ bindings.

Usage: generate_fpga_bindings.py --header NiFpga_<build>.h --output <output>.h
No hardware addresses, FIFO identifiers or signatures are supplied by this tool.
They must all be exported together from the actual FPGA build.
"""

import argparse
import os
import pathlib
import re
import sys
import xml.etree.ElementTree as ET


GLOBAL_RESOURCES = (
    ("TriggerReset", "ControlBool"),
    ("ClearLossCounters", "ControlBool"),
    ("GateTicks", "ControlU32"),
    ("ScopeHoldOffTicks", "ControlU32"),
)
CHANNEL_RESOURCES = (
    ("Start", "ControlBool"),
    ("BaselineHold", "ControlBool"),
    ("ScopeEnable", "ControlBool"),
    ("RawEnable", "ControlBool"),
    ("Threshold", "ControlI32"),
    ("PeakMin", "ControlI32"),
    ("IntegralMin", "ControlI32"),
    ("TailTicks", "ControlU16"),
    ("ScopeRecordTicks", "ControlU16"),
    ("MaxLength", "ControlU32"),
    ("PeakShift", "ControlU8"),
    ("IntegralShift", "ControlU8"),
    ("ScopePreTicks", "ControlU8"),
    ("Run", "IndicatorBool"),
    ("Baseline", "IndicatorI32"),
    ("RawLost", "IndicatorU64"),
    ("PulseLost", "IndicatorU64"),
    ("RateLost", "IndicatorU64"),
    ("SpecLost", "IndicatorU64"),
    ("ScopeLost", "IndicatorU64"),
    ("SpecOverrun", "IndicatorU32"),
    ("RawData_ToHost", "FifoI16"),
    ("PulseRecords_ToHost", "FifoI64"),
    ("RateRecords_ToHost", "FifoU64"),
    ("SpectrumBlocks_ToHost", "FifoU32"),
    ("ScopeRecords_ToHost", "FifoU64"),
)
REQUIRED_RESOURCES = GLOBAL_RESOURCES + tuple(
    ("{}_Ch{}".format(name, channel), kind)
    for channel in range(2)
    for name, kind in CHANNEL_RESOURCES
)

# The supplied 06B443BD4564E81B9835D621C0EEF203 build differs from the
# document's front-panel type table. Preserve its real access widths. These
# specific alternatives also allow a subsequent corrected export to use the
# types in the document. The runtime validates shift values and I16 bounds.
EXPORTED_TYPE_ALTERNATIVES = {
    "PeakMin_Ch1": ("ControlI16",),
    "IntegralMin_Ch1": ("ControlI16",),
    "PeakShift_Ch0": ("ControlI32",),
    "IntegralShift_Ch0": ("ControlI32",),
    "PeakShift_Ch1": ("ControlI16",),
    "IntegralShift_Ch1": ("ControlI16",),
}


class BindingError(ValueError):
    """The supplied header cannot describe the documented firmware safely."""


def normalized(name):
    """NI generator versions can remove underscores in front-panel names."""
    return re.sub(r"[^a-z0-9]", "", name.lower())


def without_comments(source):
    # Keep string literals intact (including a possible // in a bitfile path).
    tokens = r'"(?:\\.|[^"\\])*"|/\*[\s\S]*?\*/|//[^\r\n]*'
    return re.sub(tokens, lambda m: m.group(0) if m.group(0).startswith('"') else " ", source)


def parse_header(source):
    """Return prefix, bitfile symbol, signature symbol and validated bindings."""
    source = without_comments(source)
    signatures = re.findall(
        r'\b(NiFpga_\w+)_Signature\s*=\s*"([^"\r\n]*)"\s*;', source
    )
    if len(signatures) != 1:
        raise BindingError("Expected exactly one exported NiFpga_*_Signature")
    prefix, signature = signatures[0]
    if not re.fullmatch(r"[0-9a-fA-F]{32}", signature):
        raise BindingError("The exported FPGA signature must contain exactly 32 hexadecimal digits")
    bitfile_pattern = r'^\s*#\s*define\s+' + re.escape(prefix) + r'_Bitfile\s+"([^"\r\n]+)"\s*$'
    bitfiles = re.findall(bitfile_pattern, source, re.MULTILINE)
    if len(bitfiles) != 1 or not bitfiles[0].lower().endswith(".lvbitx"):
        raise BindingError("Expected one matching {}_Bitfile ending in .lvbitx".format(prefix))

    # The C API generator emits literal resource addresses/IDs in enum entries.
    # Restrict matches to the same compilation prefix as its signature.
    entry_pattern = (
        r'\b(' + re.escape(prefix)
        + r'_(Control|Indicator|TargetToHostFifo|HostToTargetFifo)'
        + r'(Bool|I8|U8|I16|U16|I32|U32|I64|U64|Sgl|Dbl)_([A-Za-z0-9_]+))'
        + r'\s*=\s*([^,}\r\n]+)'
    )
    entries = {}
    for match in re.finditer(entry_pattern, source):
        symbol, direction, data_type, name, literal = match.groups()
        kind = ("Fifo" if direction == "TargetToHostFifo" else direction) + data_type
        entries.setdefault(normalized(name), []).append((symbol, kind, literal.strip()))

    bindings = []
    problems = []
    fifo_ids = {}
    for name, expected_kind in REQUIRED_RESOURCES:
        candidates = entries.get(normalized(name), [])
        if not candidates:
            problems.append("missing {} ({})".format(name, expected_kind))
            continue
        if len(candidates) != 1:
            problems.append("ambiguous {}: {}".format(name, ", ".join(c[0] for c in candidates)))
            continue
        symbol, actual_kind, literal = candidates[0]
        if actual_kind not in (expected_kind,) + EXPORTED_TYPE_ALTERNATIVES.get(name, ()):
            problems.append("{} requires {}, exported {}".format(name, expected_kind, actual_kind))
            continue
        integer = re.fullmatch(r"(0[xX][0-9a-fA-F]+|[0-9]+)[uUlL]*", literal)
        if not integer:
            problems.append("{} has unsupported non-literal identifier {}".format(name, literal))
            continue
        value = integer.group(1)
        resource_id = int(value, 16 if value.lower().startswith("0x") else 10)
        if resource_id > 0xFFFFFFFF:
            problems.append("{} identifier exceeds uint32_t".format(name))
            continue
        if expected_kind.startswith("Fifo"):
            if resource_id in fifo_ids:
                problems.append("FIFO identifier {} is shared by {} and {}".format(
                    resource_id, fifo_ids[resource_id], name))
            fifo_ids[resource_id] = name
        bindings.append((name, actual_kind, symbol))
    if problems:
        raise BindingError("Header does not match DiamondDetectorDAQ:\n  " + "\n  ".join(problems))
    return prefix, bindings


def render_header(header_path, prefix, bindings, output_dir=None):
    include_path = pathlib.Path(header_path).resolve().as_posix()
    if output_dir is not None:
        include_path = os.path.relpath(str(pathlib.Path(header_path).resolve()),
                                     str(pathlib.Path(output_dir).resolve())).replace('\\', '/')
    if any(character in include_path for character in ('"', "\r", "\n")):
        raise BindingError("Header path contains a character not supported by #include")
    lines = [
        "// Generated from the actual NI FPGA C API header. Do not edit.",
        "#ifndef DIAMOND_GENERATED_H",
        "#define DIAMOND_GENERATED_H",
        '#include "DiamondResources.h"',
        '#include "{}"'.format(include_path),
        "#include <vector>",
        "",
        "namespace diamond {",
        "inline const char* bitfileName() { return " + prefix + "_Bitfile; }",
        "inline const char* signature() { return " + prefix + "_Signature; }",
        "inline std::vector<Resource> resources() {",
        "    return {",
    ]
    lines.extend('        {{"{}", Kind::{}, static_cast<std::uint32_t>({})}},'.format(
        name, kind, symbol) for name, kind, symbol in bindings)
    lines.extend(["    };", "}", "} // namespace diamond", "", "#endif", ""])
    return "\n".join(lines)


def verify_bitfile(source, bitfile_path):
    signature = re.search(
        r'\bNiFpga_\w+_Signature\s*=\s*"([0-9a-fA-F]{32})"\s*;', without_comments(source)
    ).group(1)
    try:
        actual = ET.parse(bitfile_path).getroot().findtext("SignatureRegister")
    except ET.ParseError as error:
        raise BindingError("Invalid bitfile XML: {}".format(error)) from error
    if actual is None or actual.strip().lower() != signature.lower():
        raise BindingError("Bitfile SignatureRegister does not match the exported header signature")


def generate(header_path, output_path, bitfile_path=None):
    header_path = pathlib.Path(header_path)
    source = header_path.read_text(encoding="utf-8-sig")
    prefix, bindings = parse_header(source)
    if bitfile_path is not None:
        verify_bitfile(source, bitfile_path)
    output_path = pathlib.Path(output_path)
    rendered = render_header(header_path, prefix, bindings, output_path.parent)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    # Validation is complete before replacing any previously generated binding.
    with output_path.open("w", encoding="utf-8", newline="\n") as output:
        output.write(rendered)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--header", required=True, type=pathlib.Path)
    parser.add_argument("--output", required=True, type=pathlib.Path)
    parser.add_argument("--bitfile", type=pathlib.Path, help="also check the matching .lvbitx signature")
    args = parser.parse_args()
    try:
        generate(args.header, args.output, args.bitfile)
    except (BindingError, OSError, UnicodeError) as error:
        parser.exit(1, "FPGA binding generation failed: {}\n".format(error))
    return 0


if __name__ == "__main__":
    sys.exit(main())
