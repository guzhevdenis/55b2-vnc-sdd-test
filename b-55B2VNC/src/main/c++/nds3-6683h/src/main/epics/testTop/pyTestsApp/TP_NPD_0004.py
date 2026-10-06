import unittest
from NisyncUTFixture import TestNdsNisync
import NisyncUTFixture
import epics
import os
from time import sleep


class NPD0004(TestNdsNisync):   
    def test_NPD0004(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)                                                                                             
        except OSError:                                                                                       
                pass                                                                                          
        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0004:Routes signal from one terminal to other terminals.                 ")
        print("          The source is the board clock. To check routing event timestamping  ")
        print("          is used.Event time stamping can be enabled or disabled.             ")
        print("          When board clock generates high frequency clock                     ")
        print("          event time stamping decimation is used.                             ")
        print("+---------------------------------------------------------------------")    
        print("|")
        print("Requirement(s):")
        print("-SYNC-SRS-F-51")
        print("-SYNC-SRS-F-52")
        print("-SYNC-SRS-F-53")
        print("-SYNC-SRS-F-57")
        print("-SYNC-SRS-F-72")
        print("-SYNC-SRS-F-73")
        print("-SYNC-SRS-F-74")
        print("-SYNC-SRS-F-75")
        print("-SYNC-SRS-F-76")
        print("-SYNC-SRS-F-77")
        print("-SYNC-SRS-F-78")
        print("-SYNC-SRS-F-79")
        raw_input("Test ready. Press enter to Start")
        
        '''Track checkpoints to follow test plan'''
        self.timestamps()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        self.StateMachinesOFF()
        self.printTestResults(self.tsList,timestamps=True,decimation=10) 
    
    def timestamps(self):                                                                                       
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
        raw_input("Checkpoint 1.Connect oscilloscope to PFI2 terminal. Press enter to continue.")  
        print("+---------------------------------------------------------------------")                                                    
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [                                                     
                ['Routing-STM-setState',          int(4) ,"","","",""],
                ['Routing-TermSrc',              int(36) ,"","","",""],
                ['Routing-TermDst',               int(2) ,"","","",""],
                ['Routing-TermSet',                 "ON" ,"","","",""],                                                                                                     
                ['Routing-TermDstRead',           int(2) ,"","","",""],                                                      
                ['Routing-TermSetCode',             None ,"","","",""],                                                      
                ['Routing-TermSetStatus',           None ,"","","",""],
                ['Routing-TermSrcRead',          int(36) ,"","","",""],
                ['Routing-TermInvertRead',        int(2) ,"","","",""],
                ['Routing-STM-getState',            None ,"","","",""],                                     
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
                ['Routing-STM-setState',            False,int(4) ],
                ['Routing-TermSrc',                 False,int(36) ],
                ['Routing-TermDst',                 False,int(2) ],
                ['Routing-TermSet',                 False,int(1) ],                                                                                            
                ['Routing-TermDstRead',              True,int(2) ],                                             
                ['Routing-TermSetCode',              True,int(0) ],                                                
                ['Routing-TermSetStatus',           False,"OK"    ],
                ['Routing-TermSrcRead',              True,int(36) ],    
                ['Routing-TermInvertRead',           True,int(0) ], 
                ['Routing-STM-getState',             True,int(4) ],                                        
            ]
        
        pvList=self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
       
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 2.Enabling Timestamping on PFI2. Check timestamps on output file. Press enter to continue.")
        print("+---------------------------------------------------------------------") 
        '''separately register a callback function for timestamps'''
        epics.camonitor(self.prefix+'TR2-TS-Timestamps', None, self.monitTSCallback)
        
    
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['TR2-TS-Decimation',           int(1000000),"","","",""],
                ['TR2-TS-DecimationType',                "HW" ,"","","",""],
                ['TR2-STM-setState',                 ON ,"","","",""],
                ['TR2-TS-Edge',                      "FALLING" ,"","","",""],
                ['TR2-TS-MaxTimestamps',              None ,"","","",""],            
                ['TR2-TS-Edge_RBV',                   None ,"","","",""],
                ['TR2-STM-setState',            RUNNING ,"","","",""],
                ['Routing-TermDstRead',             int(2) ,"","","",""],                                                      
                ['Routing-TermSetCode',               None ,"","","",""],                                                      
                ['Routing-TermSetStatus',             None ,"","","",""],
                ['Routing-TermSrcRead',             int(2) ,"","","",""],
                ['Routing-TermInvertRead',          int(2) ,"","","",""],
                ['Routing-STM-getState',              None ,"","","",""],
                ['TR2-STM-getState',               None ,"","","",""],                                            
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
                ['TR2-TS-Decimation',                False,int(7)  ],
                ['TR2-TS-DecimationType',              False,int(7)  ],
                ['TR2-STM-setState',             False,int(7)  ],
                ['TR2-TS-Edge',                     False,int(1)  ],
                ['TR2-TS-MaxTimestamps',            True,int(1024)],   
                ['TR2-TS-Edge_RBV',                  True,int(1)  ],
                ['TR2-STM-setState',             False,int(4)  ],
                ['Routing-TermDstRead',              True,int(2)  ],                                                      
                ['Routing-TermSetCode',              True,int(0)  ],                                                    
                ['Routing-TermSetStatus',            True,"OK"    ],
                ['Routing-TermSrcRead',              True,int(36) ],
                ['Routing-TermInvertRead',           True,int(0)  ],
                ['Routing-STM-getState',             True,int(4)  ],  
                ['TR2-STM-getState',              True,int(7)  ],  
            ]
        
        pvList=self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 3.Disable Timestamp read routing configuration. Press enter to continue.")
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['TR2-STM-setState',               ON ,"","","",""],
                ['TR2-TS-Edge_RBV',                 None ,"","","",""],
                ['Routing-TermDstRead',           int(2) ,"","","",""],                                                      
                ['Routing-TermSetCode',             None ,"","","",""],                                                      
                ['Routing-TermSetStatus',           None ,"","","",""],
                ['Routing-TermSrcRead',             None ,"","","",""],
                ['Routing-TermInvertRead',          None ,"","","",""],
                ['Routing-STM-getState',            None ,"","","",""],
                ['TR2-STM-getState',             None ,"","","",""],
                ['TR2-TS-Timestamps',               None ,"","","",""],                                               
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
                ['TR2-STM-setState',           False,int(7)  ],
                ['TR2-TS-Edge_RBV',                True,int(1)  ],
                ['Routing-TermDstRead',            True,int(2)  ],
                ['Routing-TermSetCode',            True,int(0)  ],
                ['Routing-TermSetStatus',          True,"OK"    ],
                ['Routing-TermSrcRead',            True,int(36) ],
                ['Routing-TermInvertRead',         True,int(0)  ],
                ['Routing-STM-getState',           True,int(4)  ], 
                ['TR2-STM-getState',            True,int(4)  ],
                ['TR2-TS-Timestamps',             False,int(1)  ],                                                               
            ]
        
        pvList=self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]

        
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
                    ['STM-getState',                None,"","","",""],
                    ['STM-setState',                  ON,"","","",""],
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
                    ['STM-getState',                    True,ON  ],   
                    ['STM-setState',                    True,ON  ],
                ]
            self.assertPVList(pvList,assertList)  
            pvList=[]

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
                    ['TR2-STM-setState',              ON ,"","","",""],
                    ['TR2-STM-setState',             OFF ,"","","",""],
                    ['Routing-STM-setState',            OFF ,"","","",""],
                    ['STM-getState',           None,"","","",""],
                    ['STM-setState',            OFF,"","","",""],
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
                    ['TR2-STM-setState',               False,ON  ],
                    ['TR2-STM-setState',               True,OFF  ],
                    ['Routing-STM-setState',              True,OFF  ],
                    ['STM-getState',             True,OFF  ],
                    ['STM-setState',             True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      
            pvList=[]
            
if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
