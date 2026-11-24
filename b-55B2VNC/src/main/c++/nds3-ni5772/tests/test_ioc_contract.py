"""Offline integration checks: exported resources, DB loads and every OPI PV.

These do not replace compiling against the installed NDS/NI libraries or IOC tests.
"""
from pathlib import Path
import re
import sys
import unittest
import xml.etree.ElementTree as ET

DRIVER = Path(__file__).resolve().parents[1]
MAIN = DRIVER.parents[1]
DB = MAIN / 'epics/PCF0CoreApp/Db'
BOOT = MAIN / 'epics/iocBoot/iocD1-I2-PCF0CORE'
BOY = MAIN / 'boy/mimics'
sys.path.insert(0, str(DRIVER / 'tools'))
import generate_fpga_bindings as binding

def expand(text, macros):
    return re.sub(r'\$\((\w+)(?:=([^)]*))?\)',
                  lambda m: str(macros.get(m[1], m[2] if m[2] is not None else m[0])), text)

def records(text):
    return re.findall(r'record\s*\(\s*\w+\s*,\s*"([^"]+)"\s*\)', text)

def substitutions(path):
    text = path.read_text()
    result = []
    for part in re.split(r'\bfile\s+"', text)[1:]:
        filename, body = part.split('"', 1)
        groups = re.findall(r'\{([^{}]*)\}', body)
        names = [x.strip() for x in groups[0].split(',') if x.strip()]
        for row in groups[1:]:
            values = re.findall(r'"([^"]*)"', row)
            if not values: continue
            if len(values) != len(names): raise AssertionError('Bad substitution row: ' + row)
            result.append(expand((path.parent / filename).read_text(), dict(zip(names, values))))
    return '\n'.join(result)

class IOCContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.text = '\n'.join(substitutions(DB / p) for p in [
            'DiamondDetectorDAQ.substitutions', 'nisyncDevice6683H-basic.substitutions',
            'nisyncDevice6683H-terminal.substitutions'])
        cls.pvs = set(records(cls.text))

    def test_two_boards_and_four_channels_without_duplicate_names(self):
        names = records(self.text)
        self.assertEqual(len(names), len(set(names)))
        self.assertNotIn('$(', self.text)
        for board in [0, 2]:
            for channel in [0, 1]:
                self.assertIn('D1-I2-HWCF:B-{}-ScopeData_Ch{}'.format(board, channel), self.pvs)
        self.assertIn('D1-I2-HWCF:B-1-STM-setState', self.pvs)

    def test_all_boot_databases_are_installed(self):
        cmd = (BOOT / 'dbToLoad.cmd').read_text()
        install = (DB / 'Makefile').read_text() + (MAIN / 'epics/D1-I2App/Db/Makefile').read_text()
        for name in re.findall(r'dbLoad(?:Records|Template)\("([^"]+)"', cmd):
            self.assertTrue((DB / name).exists() or (MAIN / 'epics/D1-I2App/Db' / name).exists(), name)
            self.assertIn(name, install)

    def test_legacy_driver_and_dan_are_not_loaded(self):
        boot = (BOOT / 'userPreDriverConf.cmd').read_text()
        self.assertEqual(boot.count('ndsCreateDevice(Ni5772Digitizer'), 2)
        self.assertIn('ndsCreateDevice(nisyncDevice', boot)
        self.assertNotIn('Ni5763', boot)
        self.assertNotIn('nds subscribe', boot)
        for path in [BOOT / 'dbToLoad.cmd', DB / 'Makefile']:
            self.assertNotIn('Ni5763', path.read_text())
        self.assertEqual(boot.count('INIT=OFF'), 2)

    def test_templates_match_driver_resources_and_types(self):
        _, resources = binding.parse_header((DRIVER / 'include/NiFpga_DiamondDetectorDAQ_FPGA_Top.h').read_text())
        expected = {'StateMachine-setState', 'StateMachine-getState', 'StateMachine-getGlobalState',
                    'Status', 'Acquiring', 'WorkerFault', 'DeviceConfig'}
        for name, kind, _ in resources:
            if kind.startswith('Control'): expected.update([name, name + '_RBV'])
            elif kind.startswith('Indicator'):
                expected.add(name)
                if kind == 'IndicatorU64': expected.add(name + '_Display')
        for ch in [0, 1]:
            for name in ['MeasuredRate', 'CorrectedRate', 'CorrectedRate2', 'DeadFraction', 'PileUpFraction',
                         'RawData', 'ScopeData', 'PulseRecords', 'RateRecords', 'SpectrumHeader', 'ScopeHeader',
                         'PeakSpectrum', 'IntegralSpectrum', 'RateGateGaps', 'SpectrumGateGaps',
                         'SpectrumBadBlocks', 'SpectrumBadWords', 'ScopeBadWords']:
                expected.add(name + '_Ch' + str(ch))
        loaded = {n[len('D1-I2-HWCF:B-0-'):] for n in self.pvs if n.startswith('D1-I2-HWCF:B-0-')}
        self.assertEqual(loaded, expected)
        for n, kind, _ in resources:
            if not kind.startswith('Control'): continue
            pattern = r'record\((\w+), "D1-I2-HWCF:B-0-' + re.escape(n) + r'"\)\s*\{([^}]+)\}'
            match = re.search(pattern, self.text)
            self.assertIsNotNone(match, n)
            self.assertIn('asynFloat64' if kind == 'ControlU32' else 'asynInt32', match[2])
            if kind == 'ControlI16' and 'Min_' in n:
                self.assertIn('"-32768"', match[2]); self.assertIn('"32767"', match[2])

    def test_template_copies_are_synchronized(self):
        for name in ['DiamondDetectorDAQ.template', 'DiamondDetectorDAQ-channel.template']:
            self.assertEqual((DB / name).read_bytes(), (DRIVER / 'db' / name).read_bytes())

    def test_all_opi_pvs_resolve_and_open_actions_exist(self):
        visited = set()
        def walk(path, macros):
            tree = ET.parse(path).getroot()
            defaults = {n.tag: n.text or '' for n in tree.findall('./macros/*') if n.tag != 'include_parent_macros'}
            defaults.update(macros)
            used_macros = set(re.findall(r'\$\((\w+)\)', ET.tostring(tree, encoding='unicode')))
            key = (str(path), tuple(sorted((k, v) for k, v in defaults.items() if k in used_macros)))
            if key in visited: return
            visited.add(key)
            ids = [w.findtext('wuid') for w in tree.iter('widget')]
            self.assertEqual(len(ids), len(set(ids)), path.name)
            for node in tree.iter():
                if node.tag == 'pv_name' or re.match(r'trace_\d+_[xy]_pv$', node.tag):
                    if not node.text: continue
                    pv = expand(node.text, defaults)
                    self.assertIn(pv, self.pvs, '{}: {}'.format(path.name, pv))
            for action in tree.findall('.//action[@type="OPEN_DISPLAY"]'):
                target = path.parent / action.findtext('path')
                self.assertTrue(target.exists(), str(target))
                inherited = dict(defaults)
                inherited.update({n.tag: expand(n.text or '', defaults) for n in action.findall('./macros/*')
                                  if n.tag != 'include_parent_macros'})
                walk(target, inherited)
        walk(BOY / 'PCF0CORE-DiamondDetectorDAQ.opi', {})
        self.assertEqual(len(visited), 7) # overview + two board views + four channel views

    def test_hmi_widgets_fit_display(self):
        for filename in ['PCF0CORE-DiamondDetectorDAQ.opi', 'DiamondDetectorDAQ-Board.opi', 'DiamondDetectorDAQ-Channel.opi']:
            root = ET.parse(BOY / filename).getroot()
            width, height = int(root.findtext('width')), int(root.findtext('height'))
            for w in root.findall('widget'):
                self.assertLessEqual(int(w.findtext('x')) + int(w.findtext('width')), width, filename)
                self.assertLessEqual(int(w.findtext('y')) + int(w.findtext('height')), height, filename)

    def test_delegate_readbacks_are_polled_and_streams_interrupt_scanned(self):
        for record in re.findall(r'record\(\w+, "D1-I2-HWCF:B-0-([^"]+)"\)\s*\{([^}]+)\}', self.text):
            name, body = record
            if name.endswith('_RBV') or name in ['Status', 'Acquiring', 'WorkerFault', 'DeviceConfig']:
                self.assertIn('field(SCAN, "1 second")', body, name)
            if name.startswith(('RawData_', 'ScopeData_', 'PeakSpectrum_', 'IntegralSpectrum_')):
                self.assertIn('field(SCAN, "I/O Intr")', body, name)

if __name__ == '__main__':
    unittest.main()
