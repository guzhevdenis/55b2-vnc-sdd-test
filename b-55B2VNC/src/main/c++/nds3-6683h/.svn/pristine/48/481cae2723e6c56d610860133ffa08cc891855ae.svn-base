import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os



class NPD0002(TestNdsNisync):
    def test_NPD0002(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)                                                                                             
        except OSError:                                                                                       
                pass                                                                                          
        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0002:Test verifies the possibility of routing board clock to ")
        print("          external connector. And other Routing configurations. ")
        print("          Physical connection is verified by oscilloscope. ")
        print("+---------------------------------------------------------------------")    
        print("|")
        print("Requirement(s):")
        print("-SYNC-SRS-F-30")
        print("-SYNC-SRS-F-31")
        print("-SYNC-SRS-F-32")
        raw_input("Test ready. Press enter to Start")
        '''Track checkpoints to follow test plan'''
        self.routeClk()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        self.StateMachinesOFF()
        
    def routeClk(self):                                                                                       
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
        print("+---------------------------------------------------------------------")                                                                           
        raw_input("Checkpoint 1.Connect oscilloscope to CLK OUT terminal. Press enter to continue.")
        print("+---------------------------------------------------------------------")                                                      
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [                                                         
                ['Routing-STM-setState',            int(4) ,"","","",""],
                ['Routing-ClkSrc',                  int(1) ,"","","",""],                                                                                                         
                ['Routing-ClkDst',                  int(5) ,"","","",""],                                                         
                ['Routing-ClkSet',                    "ON" ,"","","",""],                                                         
                ['Routing-ClkSetCode',                None ,"","","",""],
                ['Routing-ClkSetStatus',              None ,"","","",""],
                ['Routing-ClkDstRead',              int(5) ,"","","",""],                                                         
                ['Routing-ClkSrcRead',              int(1) ,"","","",""],
                ['Routing-STM-getState',              None ,"","","",""],                                
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
                ['Routing-STM-setState',           True,int(4)  ],
                ['Routing-ClkSrc',                 True,int(1)  ],
                ['Routing-ClkDst',                 True,int(5)  ], 
                ['Routing-ClkSet',                 True,int(1)  ],
                ['Routing-ClkSetCode',            True,int(0)   ],
                ['Routing-ClkSetStatus',          True,"OK"     ],                                                         
                ['Routing-ClkDstRead',             True,int(5)  ],                                                        
                ['Routing-ClkSrcRead',             True,int(1)  ],
                ['Routing-STM-getState',           True,int(4)  ],                              
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        
        
    
    
    def StateMachinesON(self):
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
                    ['STM-getState              ',        None,"","","",""],
                    ['STM-setState              ',        ON,"","","",""],
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
                    ['STM-getState              ',        True,ON  ],   
                    ['STM-setState              ',        True,ON  ],
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
                    ['Routing-STM-setState',                       OFF ,"","","",""],
                    ['STM-getState              ',        None,"","","",""],
                    ['STM-setState              ',        OFF,"","","",""],
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

                    ['Routing-STM-setState',                       True,OFF  ],
                    ['STM-getState              ',        True,OFF  ],
                    ['STM-setState              ',        True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      

if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)