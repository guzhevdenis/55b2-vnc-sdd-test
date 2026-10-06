import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os
from time import sleep


class NPD0001(TestNdsNisync):
    def test_NPD0001(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)
        except OSError:
                pass
        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0001:Reads board synchronization status and ")
        print("          verifies sync, not-sync state changes. ")
        print("+---------------------------------------------------------------------")    
        print("|")
        print("Requirement(s):")
        print("-SYNC-SRS-F-20")
        print("-SYNC-SRS-F-28")
        raw_input("Test ready. Press enter to Start")
        
        '''Track checkpoints to follow test plan'''
        self.syncStatus()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        self.StateMachinesOFF()
        
    def syncStatus(self):
        epics.ca.use_initial_context()
        print("Wait for sync...")
        sleep(15) # Wait for sync
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''
        pvList= [
                ['Timing-STM-getState',               None ,"","","",""],
                ['Timing-SyncStatus',                 None ,"","","",""],
                ['Timing-SecsLastSync',               None ,"","","",""],
                ['Timing-Time',                       None ,"","","",""],
                ['Timing-HTime',                      None ,"","","",""],
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
                ['Timing-STM-getState',          True,int(7)  ],
                ['Timing-SyncStatus',           True,int(2)  ],
                ['Timing-SecsLastSync',         False,""  ],
                ['Timing-Time',                 False,""  ],
                ['Timing-HTime',                False,""  ],
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        print("+---------------------------------------------------------------------")
        raw_input("|Checkpoint 1. Disconnect cable and Press enter to continue.")
        print("+---------------------------------------------------------------------")
        print("Wait for de-sync...")
        sleep(5) # Wait for de-sync
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)
        pvList=self.checkPVlist(pvList)
        assertList=[
                ['Timing-STM-getState',               True,int(7)  ],
                ['Timing-SyncStatus',                 True,int(3)  ],
                ['Timing-SecsLastSync',         False,""  ],
                ['Timing-Time',                      False,""  ],   
                ['Timing-HTime',                     False,""  ],   
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        print("+---------------------------------------------------------------------")
        raw_input("|Checkpoint 2. Connect cable and  Press enter to continue.")
        print("+---------------------------------------------------------------------")
        print("Wait for sync...")
        sleep(30) # Wait for sync
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)
        pvList=self.checkPVlist(pvList)
        assertList=[
                ['Timing-STM-getState',                True,int(7)  ],
                ['Timing-SyncStatus',                  True,int(2)   ],
                ['Timing-SecsLastSync',         False,""  ],
                ['Timing-Time',                       False,"" ],   
                ['Timing-HTime',                      False,""  ],   
            ]
        self.assertPVList(pvList,assertList)            
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
                    ['STM-getState      ',       None,"","","",""],
                    ['STM-setState      ',       "ON","","","",""],
                    ['Timing-STM-setState       ',        "ON","","","",""],
                    ['Firm-STM-getState         ',        None,"","","",""],
                    ['Firm-STM-setState         ',        "ON","","","",""],
                    ['Timing-STM-setState       ',   "RUNNING","","","",""],
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
                    ['STM-getState              ',        True,ON       ],   
                    ['STM-setState              ',        True,ON       ],    
                    ['Timing-STM-setState       ',                False,ON       ],   
                    ['Firm-STM-getState         ',                 True,ON       ],   
                    ['Firm-STM-setState         ',                 True,ON       ],         
                    ['Timing-STM-setState  ',                      True,RUNNING  ],
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
                    ['Timing-STM-setState',             "ON","","","",""],
                    ['Firm-STM-getState',              None,"","","",""],
                    ['Firm-STM-setState',              "OFF","","","",""],
                    ['Timing-STM-getState',            None,"","","",""],
                    ['Timing-STM-setState',             "OFF","","","",""],
                    ['STM-getState',          None,"","","",""],
                    ['STM-setState',           "OFF","","","",""],
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
                    ['Timing-STM-setState',              False,ON   ],
                    ['Firm-STM-getState         ',        True,OFF  ],
                    ['Firm-STM-setState         ',        True,OFF  ],
                    ['Timing-STM-getState       ',        True,OFF  ],
                    ['Timing-STM-setState       ',        True,OFF  ],
                    ['STM-getState     ',        True,OFF  ],
                    ['STM-setState     ',        True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      

if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
