import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os



class NPD0000(TestNdsNisync):
    def test_NPD0000(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)
        except OSError:
                pass

        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0000:Test verifies drivers ability to read status of the firmware,") 
        print("          the serial number, board model, device driver version,"       )
        print("          and the reset function."                                      )                                     
        print("+---------------------------------------------------------------------")
        print("|")
        print("Requirement(s):")
        print("-SYNC-SRS-F-01")
        print("-SYNC-SRS-F-07")
        print("-SYNC-SRS-F-08")
        print("-SYNC-SRS-F-09")
        print("-SYNC-SRS-F-10")
        
        raw_input("Test ready. Press enter to Start")
        
        '''Track checkpoints to follow test plan'''
        self.Firmware()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        self.StateMachinesOFF()
        
    def Firmware(self):
        epics.ca.use_initial_context()
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''
        pvList= [
                ['Firm-STM-setState',                  "ON","","","",""],
                ['Firm-STM-setState',                 "RUNNING","","","",""],
                ['Firm-STM-getState',                 None ,"","","",""],
                ['Firm-DriverVersion',                None ,"","","",""],
                ['Firm-HWRevision',                   None ,"","","",""],
                ['Firm-Model',                        None ,"","","",""],
                ['Firm-SerialNumber',                 None ,"","","",""],
                ['Firm-Status',                       None ,"","","",""], 
                ['STM-getState',             None ,"","","",""],                   
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
                ['Firm-STM-setState',                 False,int(7)         ],
                ['Firm-STM-setState',                 True,int(7)         ],
                ['Firm-STM-getState',                 True,int(7)     ],
                ['Firm-DriverVersion',               False,"2.1.3"    ],
                ['Firm-HWRevision',                  False,"336730400"],
                ['Firm-Model',                        True,"PXI-6683H"],
                ['Firm-SerialNumber',                False,"25907896" ],
                ['Firm-Status',                       True,int(0) ],
                ['STM-getState',             True,int(4)     ],
            ]

        pvList=self.assertPVList(pvList,assertList)
                    
        '''Call print function'''
        self.printTestResults(pvList)
        
    def StateMachinesON(self):
            UNKNOWN = int(0)                                                                                      
            OFF = int(1)                                                                                          
            SWITCH_OFF = int(2)                                                                                   
            INITIALIZING = int(3)                                                                                 
            ON = int(4)                                                                                           
            STOPPING = int(5)                                                                                     
            STARTING = int(6)                                                                                     
            RUNNING = int(7)                                                                                      
            FAULT = int(8) 
            epics.ca.use_initial_context()
            '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''
            pvList= [
                    ['STM-setState',             "ON","","","",""],
                    ['STM-getState',             None,"","","",""],
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
                    ['STM-setState              ',        True,ON  ], 
                    ['STM-getState              ',        True,ON  ], 
                ]
            self.assertPVList(pvList,assertList)  
            

    def StateMachinesOFF(self):
            epics.ca.use_initial_context()
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
                    ['Firm-STM-setState',              "ON","","","",""],
                    ['Firm-STM-setState',             "OFF","","","",""],
                    ['STM-setState',         "OFF","","","",""],
                    ['STM-getState',          None,"","","",""],
                    ['Firm-STM-getState',              None,"","","",""],
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
                    ['Firm-STM-setState',              False,OFF  ],
                    ['Firm-STM-setState',              True,OFF  ],
                    ['STM-setState',          True,OFF  ],
                    ['STM-getState',          True,OFF  ],
                    ['Firm-STM-getState',              True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      


if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
