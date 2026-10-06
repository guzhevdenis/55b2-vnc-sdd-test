import unittest
from TP_NPD_0000 import NPD0000
#code for debugging: from test_nisync_Firmware import FirmwareTest
#    unittest.TestLoader().loadTestsFromTestCase(TestNdsNisync)
NisyncUTSuite = unittest.TestSuite()
NisyncUTSuite.addTest(NPD0000('test_NPD0000'))
#NisyncUTSuite.addTest(FirmwareTest('test_Firmware'))
unittest.TextTestRunner(verbosity=2).run(NisyncUTSuite)
