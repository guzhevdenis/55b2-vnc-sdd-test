import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os

class FirmwareTest(TestNdsNisync):
    def test_Firmware(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)
        except OSError:
                pass
        UNKNOWN = int(0)
        OFF = int(1)
        SWITCH_OFF = int(2)
        INITIALIZING = int(3)
        ON = int(4)
        STOPPING = int(5)
        STARTING = int(6)
        RUNNING = int(7)
        FAULT = int(8)
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''
        pvList= [
                ['STM-setState',               ON ,"","","",""],
                ['Firm-STM-setState',          ON ,"","","",""],
                ['Firm-DriverVersion',                None ,"","","",""],
                ['Firm-HWRevision',                   None ,"","","",""],
                ['Firm-Model',                        None ,"","","",""],
                ['Firm-SerialNumber',                 None ,"","","",""],
                ['Firm-Version',                      None ,"","","",""],
                ['Firm-STM-setState',          OFF,"","","",""],
                ['STM-setState',               OFF,"","","",""],
            ]
        '''Reformat'''
        for i in range(len(pvList)) :
                a = pvList[i][0].strip()
                pvList[i][0] = self.prefix+a
                epics.camonitor(pvList[i][0], None, self.monitCallback)     
        '''Call test function'''           
        pvList=self.checkPVlist(pvList)
        '''Assert results   PVNAME[0],     TEST?[1],       EXPECTED[2]'''
        assertList=[
                ['STM-setState',             True,int(1)         ],
                ['Firm-STM-setState',        True,int(1)         ],
                ['Firm-DriverVersion',                True,"2.1.3"        ],
                ['Firm-HWRevision',                   True,"336730400"    ],
                ['Firm-Model',                        True,"PXI-6683H"    ],
                ['Firm-SerialNumber',                 True,"25907896"     ],
                ['Firm-Version',                      True,"placeholder"  ],
                ['Firm-STM-setState',        True,int(1)         ],
                ['STM-setState',             True,int(1)         ],
            ]

        self.assertPVList(pvList,assertList)  
        '''Call print function'''
        self.printTestResults(pvList)

