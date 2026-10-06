import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os
import numpy as np
from time import sleep


class NPD0007(TestNdsNisync):
    def test_NPD0007(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)                                                                                             
        except OSError:                                                                                       
                pass  
                                                                                                    
        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0007: Tests FTEs generation and timestamping them on the same terminal.")
        print("+---------------------------------------------------------------------")    
        print("|")
        print("Requirement(s):")
        print("-SYNC-SRS-F-59")
        print("-SYNC-SRS-F-60")
        print("-SYNC-SRS-F-61")
        print("-SYNC-SRS-F-62")
        print("-SYNC-SRS-F-63")
        print("-SYNC-SRS-F-64")
        print("-SYNC-SRS-F-66")
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
        self.FTEandTS()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        print("+---------------------------------------------------------------------")
        print("|")
        self.StateMachinesON()
        self.StateMachinesOFF()
        self.printTestResults(self.tsList,timestamps=True)
        
    def FTEandTS(self):         
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

        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)                                                                                      
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                 
        pvList= [
                ['Timing-STM-setState',            RUNNING ,"","","",""],                                                                                             
                ['Timing-STM-getState',               None ,"","","",""],                                     
                ['Timing-SyncStatus',                 None ,"","","",""],
                ['Timing-Time'  ,                     None ,"","","",""], 
            ]                                                                                                 
        '''Reformat'''                                                                                        
        for i in range(len(pvList)) :                                                                         
                a = pvList[i][0].strip()
                pvList[i][0] = self.prefix+a    
                if(pvList[i][0].find('Time')==-1):                                                                               
                    epics.camonitor(pvList[i][0], None, self.monitCallback)                                                           
        '''Call test function'''                                                                                                  
        pvList=self.checkPVlist(pvList)                                                                                           
        '''Assert results   PVNAME[0],     TEST?[1],       EXPECTED[2]'''                                                         
        assertList=[
                ['Timing-STM-SetState',                True,int(7)  ],                                                                                                              
                ['Timing-STM-getState',                True,int(7)  ],                                                             
                ['Timing-SyncStatus',                  True,int(2)  ], 
                ['Timing-Time' ,                      False,int(1)  ],                                                         
            ]                                                                                                                     
        self.assertPVList(pvList,assertList)
        
        print("Wait for sync")
        timeout = 0
        syncStatus = epics.caget(self.prefix+'Timing-SyncStatus')
        while syncStatus != int(2) and timeout < 20:
            sleep(1)
            print('.')
            syncStatus = epics.caget(self.prefix+'Timing-SyncStatus')
            timeout += 1
                                                                                                                                                                                                                                                                                   
        print("+---------------------------------------------------------------------")                                                                                                                          
        print("Checkpoint 1. Configuring the FTE and Timestamping nodes for PFI2 terminal. Press enter to continue.\n")                                                      
        raw_input("+---------------------------------------------------------------------")

        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [  
                ['TR2-TS-DecimationType',   0 ,"","","",""],
                ['TR2-TS-Decimation',           0 ,"","","",""],
                ['TR2-TS-Edge',                      "ANY","","","",""],
                ['TR2-TS-Edge_RBV',              None,"","","",""],
                ['TR2-FTE-Enable',                 "ENABLE" ,"","","",""],
                ['TR2-FTE-ModeSet',              "inmLVL" ,"","","",""],
                ['TR2-FTE-LevelSet',              "LOW" ,"","","",""], 
                ['TR2-FTE-Set',                       "ON" ,"","","",""],
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
                ['TR2-TS-DecimationType',   False,''],
                ['TR2-TS-Decimation',          False,''],
                ['TR2-TS-Edge',                    False,''],
                ['TR2-TS-Edge_RBV',            True,int(2)],
                ['TR2-FTE-Enable',                False,''],
                ['TR2-FTE-ModeSet',             False,''],
                ['TR2-FTE-LevelSet',             False,''],
                ['TR2-FTE-Set',                     False,''],
            ]
        pvList=self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        '''separately register a callback function for timestamps'''
        epics.camonitor(self.prefix+'TR2-TS-Timestamps', None, self.monitTSCallback)
        synced=epics.pv.get_pv(self.prefix+'Timing-SyncStatus',connect=True)._getarg('value')
        self.assertEqual(synced, int(2),"Device is not synced to continue test.SyncStatus="+str(synced))
        
        print("+---------------------------------------------------------------------")
        print("Checkpoint 2.FTE and Timestamps configured. Next, programming FTEs on PFI2. Press enter to continue") 
        raw_input("+---------------------------------------------------------------------")

        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''  
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)                                   
        timenow= 20 + int(epics.pv.get_pv(self.prefix+'Timing-Time')._getarg('value')[0])
        pvList= [
                ['TR2-FTE-ModeSet ',                   "PULSE" ,"","","",""],
                ['TR2-FTE-StartTimeSet',            np.array([timenow+4,int(0)]),"","","",""],
                ['TR2-FTE-StopTimeSet',            np.array([timenow+5,int(0)]),"","","",""],
                ['TR2-FTE-Set',                            "ON" ,"","","",""],
                ['TR2-FTE-StartTimeSet',            np.array([timenow+6,int(0)]),"","","",""],
                ['TR2-FTE-StopTimeSet',            np.array([timenow+8,int(0)]),"","","",""],
                ['TR2-FTE-Set',                            "ON" ,"","","",""],
                ['TR2-FTE-TerminalPending',       int(1),"","","",""],
                ['TR2-FTE-PendingValue',           None,"","","",""],
                ['TR2-FTE-SetCode',                    None ,"","","",""],
                ['TR2-FTE-SetStatus',                  None ,"","","",""],                                                                                    
                ['TR2-STM-setState',                  RUNNING ,"","","",""],
                ['TR2-STM-getState',                  None ,"","","",""],
            ]                                                                                                                     
        '''Reformat'''                                                                                                            
        for i in range(len(pvList)) :                                                                                             
                a = pvList[i][0].strip()
                pvList[i][0] = self.prefix+a
                if(pvList[i][0].find('Time')==-1):                    
                    epics.camonitor(pvList[i][0], None, self.monitCallback)     
        '''Call test function'''           
        pvList=self.checkPVlist(pvList)
        '''Assert results   PVNAME[0],     TEST?[1],       EXPECTED[2]'''
        assertList=[
                ['TR2-FTE-ModeSet ',                 False,''  ],
                ['TR2-FTE-StartTimeSet',           False, ''  ],
                ['TR2-FTE-StopTimeSet',            False,''  ],
                ['TR2-FTE-Set',                           False,''  ],
                ['TR2-FTE-StartTimeSet',           False, ''  ],
                ['TR2-FTE-StopTimeSet',            False,''  ],
                ['TR2-FTE-Set',                           False,''  ],
                ['TR2-FTE-TerminalPending',      False,''  ],
                ['TR2-FTE-PendingValue',          True,int(4) ],
                ['TR2-FTE-SetCode',                   True,int(0) ],
                ['TR2-FTE-SetStatus',                 True,"OK"],
                ['TR2-FTE-setState',                 False,''],
                ['TR2-FTE-getState',                 True,RUNNING],
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
          

        pvList=[] 
        
        
        print("+---------------------------------------------------------------------")
        print("Waiting for timestamps...")
        sleep(20)

        
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
                    ['Timing-STM-getState       ',        None,"","","",""],
                    ['Timing-STM-setState       ',        ON,"","","",""], 
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
                    ['Timing-STM-getState       ',        True,ON  ],   
                    ['Timing-STM-setState       ',        True,ON  ],
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
                    ['TR2-STM-setState          ',        ON ,"","","",""],
                    ['TR2-STM-getState          ',        None,"","","",""],
                    ['TR2-STM-setState          ',        OFF ,"","","",""],
                    ['TR2-STM-getState          ',        None,"","","",""],
                    ['Timing-STM-setState       ',         OFF,"","","",""],
                    ['Timing-STM-getState       ',        None,"","","",""],
                    ['Timing-STM-setState       ',         OFF,"","","",""],
                    ['STM-getGlobalState        ',        None,"","","",""],
                    ['STM-getState              ',        None,"","","",""],
                    ['STM-setState              ',         OFF,"","","",""],
                ]
            '''Reformat'''
            for i in range(len(pvList)) :
                    a = pvList[i][0].strip()
                    pvList[i][0] = self.prefix+a
                    epics.camonitor(pvList[i][0], None, self.monitCallback)     
            '''Call test function, wait for device to switch off'''
            sleep(1)           
            pvList=self.checkPVlist(pvList)
            '''Assert results   PVNAME[0],     TEST?[1],       EXPECTED[2]'''
            assertList=[
                    ['TR2-STM-setState'          ,       False,ON   ],
                    ['TR2-STM-getState          ',       False,OFF  ],
                    ['TR2-STM-setState'          ,       False,OFF  ],
                    ['TR2-STM-getState          ',        True,OFF  ],
                    ['Timing-STM-setState       ',       False,OFF  ],   
                    ['Timing-STM-getState       ',        True,OFF  ],
                    ['Timing-STM-setState       ',        True,OFF  ],
                    ['STM-getGlobalState        ',        True,OFF  ],
                    ['STM-getState              ',        True,OFF  ],
                    ['STM-setState              ',        True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      

if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
