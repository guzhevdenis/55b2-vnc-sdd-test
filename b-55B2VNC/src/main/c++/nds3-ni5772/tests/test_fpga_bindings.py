"""Binding tests use synthetic C API headers, never hardware identifiers."""

import importlib.util
import pathlib
import subprocess
import sys
import tempfile
import unittest


SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "tools" / "generate_fpga_bindings.py"
SPEC = importlib.util.spec_from_file_location("generate_fpga_bindings", SCRIPT)
bindings = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(bindings)
PREFIX = "NiFpga_SYNTHETIC_TEST_ONLY"


def fixture():
    rows = [
        "/* Synthetic test fixture: these are NOT hardware resource IDs. */",
        '#define {}_Bitfile "synthetic-test-only.lvbitx"'.format(PREFIX),
        'static const char* const {}_Signature = "0123456789abcdef0123456789ABCDEF";'.format(PREFIX),
        "typedef enum {",
    ]
    fifo_id = 0
    for index, (name, kind) in enumerate(bindings.REQUIRED_RESOURCES):
        exported_kind = kind.replace("Fifo", "TargetToHostFifo")
        if kind.startswith("Fifo"):
            resource_id = fifo_id
            fifo_id += 1
        else:
            resource_id = 0x10000 + index * 8
        rows.append("    {}_{}_{} = 0x{:X},".format(PREFIX, exported_kind, name, resource_id))
    rows.append("} SyntheticTestResources;")
    return "\n".join(rows)


class BindingTests(unittest.TestCase):
    def test_all_documented_resources(self):
        prefix, result = bindings.parse_header(fixture())
        self.assertEqual(prefix, PREFIX)
        self.assertEqual(len(result), 56)
        self.assertEqual([(name, kind) for name, kind, _ in result], list(bindings.REQUIRED_RESOURCES))
        self.assertEqual(sum(kind.startswith("Fifo") for _, kind, _ in result), 10)

    def test_underscores_and_holdoff_capitalization(self):
        source = fixture()
        for name, _ in bindings.REQUIRED_RESOURCES:
            source = source.replace("_" + name + " =", "_" + name.replace("_", "") + " =")
        source = source.replace("ScopeHoldOffTicks", "ScopeHoldoffTicks")
        _, result = bindings.parse_header(source)
        self.assertEqual(len(result), len(bindings.REQUIRED_RESOURCES))

    def test_comments_do_not_supply_resources(self):
        source = fixture().replace(
            PREFIX + "_ControlBool_Start_Ch0 = 0x10020,",
            "// " + PREFIX + "_ControlBool_Start_Ch0 = 0x10020,",
        )
        with self.assertRaisesRegex(bindings.BindingError, "missing Start_Ch0"):
            bindings.parse_header(source)

    def test_wrong_type_rejected(self):
        source = fixture().replace("_ControlI32_Threshold_Ch0", "_ControlI16_Threshold_Ch0")
        with self.assertRaisesRegex(bindings.BindingError, "Threshold_Ch0 requires ControlI32"):
            bindings.parse_header(source)

    def test_actual_build_numeric_widths_are_preserved(self):
        source = fixture()
        for name, alternatives in bindings.EXPORTED_TYPE_ALTERNATIVES.items():
            original = dict(bindings.REQUIRED_RESOURCES)[name]
            source = source.replace("_{}_{}".format(original, name), "_{}_{}".format(alternatives[0], name))
        _, result = bindings.parse_header(source)
        actual = {name: kind for name, kind, _ in result}
        for name, alternatives in bindings.EXPORTED_TYPE_ALTERNATIVES.items():
            self.assertEqual(actual[name], alternatives[0])

    def test_alternative_types_do_not_allow_other_narrowed_controls(self):
        source = fixture().replace("_ControlI32_PeakMin_Ch0", "_ControlI16_PeakMin_Ch0")
        with self.assertRaisesRegex(bindings.BindingError, "PeakMin_Ch0 requires ControlI32"):
            bindings.parse_header(source)

    def test_wrong_fifo_direction_rejected(self):
        source = fixture().replace("_TargetToHostFifoI64_PulseRecords_ToHost_Ch0", "_HostToTargetFifoI64_PulseRecords_ToHost_Ch0")
        with self.assertRaisesRegex(bindings.BindingError, "exported HostToTargetFifoI64"):
            bindings.parse_header(source)

    def test_duplicate_normalized_name_rejected(self):
        source = fixture() + "\n{} = 123,".format(PREFIX + "_ControlBool_StartCh0")
        with self.assertRaisesRegex(bindings.BindingError, "ambiguous Start_Ch0"):
            bindings.parse_header(source)

    def test_duplicate_fifo_id_rejected(self):
        source = fixture().replace("_TargetToHostFifoI64_PulseRecords_ToHost_Ch0 = 0x1", "_TargetToHostFifoI64_PulseRecords_ToHost_Ch0 = 0x0")
        with self.assertRaisesRegex(bindings.BindingError, "FIFO identifier 0 is shared"):
            bindings.parse_header(source)

    def test_invalid_signature_rejected(self):
        for signature in ("", "1234", "g" * 32):
            source = fixture().replace("0123456789abcdef0123456789ABCDEF", signature)
            with self.assertRaisesRegex(bindings.BindingError, "32 hexadecimal"):
                bindings.parse_header(source)

    def test_multiple_signatures_rejected(self):
        source = fixture() + '\nstatic const char* NiFpga_Another_Signature = "' + "0" * 32 + '";'
        with self.assertRaisesRegex(bindings.BindingError, "exactly one"):
            bindings.parse_header(source)

    def test_mismatched_bitfile_prefix_rejected(self):
        source = fixture().replace(PREFIX + "_Bitfile", "NiFpga_Other_Bitfile")
        with self.assertRaisesRegex(bindings.BindingError, "matching"):
            bindings.parse_header(source)

    def test_identifier_expression_rejected(self):
        source = fixture().replace("_ControlBool_Start_Ch0 = 0x10020", "_ControlBool_Start_Ch0 = base + 4")
        with self.assertRaisesRegex(bindings.BindingError, "unsupported non-literal"):
            bindings.parse_header(source)

    def test_identifier_overflow_rejected(self):
        source = fixture().replace("_ControlBool_Start_Ch0 = 0x10020", "_ControlBool_Start_Ch0 = 0x100000000")
        with self.assertRaisesRegex(bindings.BindingError, "exceeds uint32_t"):
            bindings.parse_header(source)

    def test_generated_header_uses_original_symbols(self):
        with tempfile.TemporaryDirectory() as directory:
            source = pathlib.Path(directory) / "with spaces" / "actual.h"
            source.parent.mkdir()
            source.write_text(fixture(), encoding="utf-8")
            output = pathlib.Path(directory) / "output" / "DiamondGenerated.h"
            bindings.generate(source, output)
            rendered = output.read_text(encoding="utf-8")
            self.assertIn('#include "../with spaces/actual.h"', rendered)
            self.assertIn("return " + PREFIX + "_Signature;", rendered)
            self.assertIn("return " + PREFIX + "_Bitfile;", rendered)
            self.assertIn("Kind::FifoI64, static_cast<std::uint32_t>(" + PREFIX + "_TargetToHostFifoI64_PulseRecords_ToHost_Ch0)", rendered)
            self.assertNotIn("0x10020", rendered)

    def test_cli_failure_produces_no_output(self):
        with tempfile.TemporaryDirectory() as directory:
            source = pathlib.Path(directory) / "wrong.h"
            source.write_text("// missing FPGA export", encoding="utf-8")
            output = pathlib.Path(directory) / "DiamondGenerated.h"
            result = subprocess.run([sys.executable, str(SCRIPT), "--header", str(source), "--output", str(output)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            self.assertEqual(result.returncode, 1)
            self.assertIn("FPGA binding generation failed", result.stderr)
            self.assertFalse(output.exists())

    def test_bitfile_signature_mismatch_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            source = pathlib.Path(directory) / "synthetic.h"
            source.write_text(fixture(), encoding="utf-8")
            bitfile = pathlib.Path(directory) / "synthetic.lvbitx"
            bitfile.write_text("<Bitfile><SignatureRegister>" + "0" * 32 + "</SignatureRegister></Bitfile>", encoding="utf-8")
            output = pathlib.Path(directory) / "DiamondGenerated.h"
            with self.assertRaisesRegex(bindings.BindingError, "does not match"):
                bindings.generate(source, output, bitfile)
            self.assertFalse(output.exists())

    def test_real_export_matches_supplied_bitfile(self):
        project = SCRIPT.parents[1]
        source = project / "include" / "NiFpga_DiamondDetectorDAQ_FPGA_Top.h"
        bitfile = project / "bitfile" / "NiFpga_DiamondDetectorDAQ_FPGA_Top.lvbitx"
        self.assertTrue(source.is_file())
        self.assertTrue(bitfile.is_file())
        with tempfile.TemporaryDirectory() as directory:
            output = pathlib.Path(directory) / "DiamondGenerated.h"
            bindings.generate(source, output, bitfile)
            rendered = output.read_text(encoding="utf-8")
            self.assertIn('"IntegralMin_Ch1", Kind::ControlI16', rendered)
            self.assertIn('"PeakShift_Ch0", Kind::ControlI32', rendered)


if __name__ == "__main__":
    unittest.main()
