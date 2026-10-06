import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os
import numpy as np
from time import sleep


class NPD0006(TestNdsNisync):
    def test_NPD0006(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)                                                                                             
        except OSError:                                                                                       
                pass  
                                                                                                    
        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0006:Test the generation and removing of future time events,      ")
        print("|         Checks the correct order of FTEs generated.")
        print("|         Tests pulse generation.")
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
        raw_input("Test ready. Press enter to Start")
        
        '''Track checkpoints to follow test plan'''
        self.fteTSPulse()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        self.StateMachinesON()
        self.StateMachinesOFF()
        self.printTestResults(self.tsList,timestamps=True)
        
    def fteTSPulse(self):         
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
        print("Wait for sync...")
        sleep(20)
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
        print("+---------------------------------------------------------------------")                                                                                                                          
        raw_input("Checkpoint 1. Connect oscilloscope to PFI2 terminal. Set time scale to 5 seconds. Press enter to continue.")                                                      
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['TR15-STM-setState',                             ON ,"","","",""],
                ['TR15-STM-getState',                             ON ,"","","",""],
                ['TR15-FTE-Enable ',                             "ENABLE" ,"","","",""],
                ['TR15-FTE-Enable_RBV ',                         "ENABLE" ,"","","",""],
                ['TR15-FTE-LevelSet ',                             "LOW" ,"","","",""],
                ['TR15-FTE-ModeSet',                            "inmLVL" ,"","","",""],
                ['TR15-FTE-Set',                                    "ON" ,"","","",""],
                ['TR15-FTE-SetCode',                                None ,"","","",""],
                ['TR15-FTE-SetStatus',                              None ,"","","",""],
                ['Routing-STM-getState',                       None ,"","","",""],                                                         
                ['Routing-STM-setState',                     int(4) ,"","","",""],
                ['Routing-TermSrc',                         int(15) ,"","","",""],
                ['Routing-TermDst',                          int(2) ,"","","",""],
                ['Routing-TermSyncSet',                       "OFF" ,"","","",""],
                ['Routing-TermInvertSet',                     "OFF" ,"","","",""],
                ['Routing-TermSet',                            "ON" ,"","","",""],
                ['Routing-TermDstRead',                      int(2) ,"","","",""],
                ['Routing-TermSetCode',                        None ,"","","",""],
                ['Routing-TermSetStatus',                      None ,"","","",""],
                ['Routing-TermSrcRead',                        None ,"","","",""],                                                       
                ['Routing-TermInvertRead',                     None ,"","","",""],  
                ['TR2-STM-setState',                          ON ,"","","",""],
                ['TR2-STM-getState',                        None ,"","","",""],
                ['TR2-TS-Edge',                               "ANY" ,"","","",""],
                ['TR2-TS-DecimationType',                  "SW" ,"","","",""],
                ['TR2-STM-setState',                     RUNNING ,"","","",""],
                ['TR2-STM-getState',                        None ,"","","",""], 
                         
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
                ['TR15-STM-setState',               False,int(4 )  ],
                ['TR15-STM-getState',                True,int(4 )  ],
                ['TR15-FTE-Enable',               False,"ENABLE"  ],
                ['TR15-FTE-Enable_RBV',               True,int(1)  ],
                ['TR15-FTE-LevelSet ',                  False,int(0 )  ],
                ['TR15-FTE-ModeSet',                    False,int(3 )  ],
                ['TR15-FTE-Set',                        False,int(1 )  ],
                ['TR15-FTE-SetCode',                     True,int(0 )  ],
                ['TR15-FTE-SetStatus',                   True,"OK"     ],
                ['Routing-STM-getState',            True,int(4 )  ],                                                         
                ['Routing-STM-setState',        False,int(4 )  ],
                ['Routing-TermSrc',             False,int(15)  ],
                ['Routing-TermDst',             False,int(2 )  ],
                ['Routing-TermSyncSet',         False,int(0 )  ],
                ['Routing-TermInvertSet',       False,int(0 )  ],
                ['Routing-TermSet',             False,int(1 )  ],
                ['Routing-TermDstRead',         False,int(2 )  ],
                ['Routing-TermSetCode',        True,int(0 )    ],                                                         
                ['Routing-TermSetStatus',        True,"OK"     ],                                                        
                ['Routing-TermSrcRead',          True,int(15)  ],
                ['Routing-TermInvertRead',       True,int(0 )  ],
                ['TR2-STM-setState',            False,int(7 )  ],
                ['TR2-STM-getState',            False,int(7 )  ],
                ['TR2-TS-Edge',                 False,int(2 )  ],
                ['TR2-TS-DecimationType',       False,int(0 )  ],
                ['TR2-STM-setState',            False,int(7 )  ],
                ['TR2-STM-getState',             True,int(7 )  ],  
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
        raw_input("Checkpoint 2.Terminals Routed. Next, enabling Timestamps on PFI2. Press enter to continue") 
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''  
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)                                   
        timenow= 20 + int(epics.pv.get_pv(self.prefix+'Timing-Time')._getarg('value')[0])
        pvList= [
                ['TR15-STM-getState',                                 None ,"","","",""],
                ['TR15-FTE-LevelSet ',                                   "LOW" ,"","","",""],
                ['Timing-Time'  ,                                     None,"","","",""],
                ['Timing-TimeSec'  ,                                  None,"","","",""],
                ['TR15-FTE-StartTimeSet',            np.array([timenow,int(0)]),"","","",""],
                ['TR15-FTE-StopTimeSet',           np.array([timenow+1,int(0)]),"","","",""],
                ['TR15-FTE-ModeSet',                                   "PULSE" ,"","","",""],
                ['TR15-FTE-Set',                                          "ON" ,"","","",""],
                ['TR15-FTE-SetCode',                                      None ,"","","",""],
                ['TR15-FTE-SetStatus',                                    None ,"","","",""],                                                                                    
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
                ['TR15-STM-getState',                   True,int(4)  ],
                ['TR15-FTE-LevelSet ',                     False,int(4)  ],
                ['Timing-Time'  ,                     False,int(4)  ],
                ['Timing-TimeSec'  ,                  False,int(4)  ],
                ['TR15-FTE-StartTimeSet',                  False,int(4)  ],
                ['TR15-FTE-StopTimeSet',                   False,int(4)  ],
                ['TR15-FTE-ModeSet',                       False,int(4)  ],
                ['TR15-FTE-Set',                           False,int(4)  ],
                ['TR15-FTE-SetCode',                        True,int(0)  ],
                ['TR15-FTE-SetStatus',                      True,"OK"  ],
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        
        print("+---------------------------------------------------------------------")
        print("Wait for timestamps...")
        sleep(15)
        print("Checkpoint 3.Generating 2 pulses. Reading FTE pending status.")
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)                                     
        timenow=int(epics.pv.get_pv(self.prefix+'Timing-Time')._getarg('value')[0] +120 )
        pvList= [
                ['TR15-STM-getState',                                 None ,"","","",""],
                ['TR15-FTE-LevelSet ',                                   "LOW" ,"","","",""],
                ['Timing-Time'  ,                                     None,"","","",""],
                ['Timing-TimeSec'  ,                                  None,"","","",""],
                ['TR15-FTE-StartTimeSet',            np.array([timenow,int(0)]),"","","",""],
                ['TR15-FTE-StopTimeSet',           np.array([timenow+1,int(0)]),"","","",""],
                ['TR15-FTE-ModeSet',                                   "PULSE" ,"","","",""],
                ['TR15-FTE-Set',                                          "ON" ,"","","",""],
                ['TR15-FTE-StartTimeSet',         np.array([timenow+10,int(0)]),"","","",""],
                ['TR15-FTE-StopTimeSet',          np.array([timenow+11,int(0)]),"","","",""],
                ['TR15-FTE-ModeSet',                                   "PULSE" ,"","","",""],
                ['TR15-FTE-Set',                                          "ON" ,"","","",""],
                ['TR15-FTE-SetCode',                                      None ,"","","",""],
                ['TR15-FTE-SetStatus',                                    None ,"","","",""],
                ['TR15-FTE-TerminalPending',                           int(1) ,"","","",""],
                ['TR15-FTE-PendingCode',                                  None ,"","","",""],
                ['TR15-FTE-PendingStatus',                                None ,"","","",""],
                ['TR15-FTE-PendingValue',                                 None ,"","","",""],
                ['TR15-FTE-Maximum',                                      None ,"","","",""],                                            
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
                ['TR15-STM-getState',               True,int(4)  ],
                ['TR15-FTE-LevelSet ',                 False,int(4)  ],
                ['Timing-Time'  ,                 False,int(4)  ],
                ['Timing-TimeSec'  ,              False,int(4)  ],
                ['TR15-FTE-StartTimeSet',              False,int(4)  ],
                ['TR15-FTE-StopTimeSet',               False,int(4)  ],
                ['TR15-FTE-ModeSet',                   False,int(4)  ],
                ['TR15-FTE-Set',                       False,int(4)  ],
                ['TR15-FTE-StartTimeSet',              False,int(4)  ],
                ['TR15-FTE-StopTimeSet',               False,int(4)  ],
                ['TR15-FTE-ModeSet',                   False,int(4)  ],
                ['TR15-FTE-Set',                       False,int(4)  ],
                ['TR15-FTE-SetCode',                    True,int(0)  ],
                ['TR15-FTE-SetStatus',                  True,"OK"  ],
                ['TR15-FTE-TerminalPending',           False,int(1)  ],
                ['TR15-FTE-PendingCode',                True,int(0)  ],
                ['TR15-FTE-PendingStatus',              True,"OK"  ],
                ['TR15-FTE-PendingValue',               True,int(4)  ],
                ['TR15-FTE-Maximum',                    True,int(1023)  ],
                 ]       
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        
        print("+---------------------------------------------------------------------")
        print("Checkpoint 4.Suppressing(Aborting) ALL FTEs before they happen.")
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]''' 
        pvList= [
                ['TR15-FTE-AllSuppress',                  "ON" ,"","","",""],
                ['TR15-FTE-Suppress',                     "ON" ,"","","",""],
                ['TR15-FTE-TerminalPending',           int(1) ,"","","",""],
                ['TR15-FTE-PendingCode',                  None ,"","","",""],
                ['TR15-FTE-PendingStatus',                None ,"","","",""],
                ['TR15-FTE-PendingValue',                 None ,"","","",""],
                ['TR15-FTE-AllSuppress',                  "OFF" ,"","","",""],   
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
                   ['TR15-FTE-AllSuppress',                    False,int(4)  ],
                   ['TR15-FTE-Suppress',                       False,int(4)  ],
                   ['TR15-FTE-TerminalPending',                 True,int(1) ],
                   ['TR15-FTE-PendingCode',                     True,int(0)  ],
                   ['TR15-FTE-PendingStatus',                   True,"OK"    ],
                   ['TR15-FTE-PendingValue',                    True,int(0)  ],
                   ['TR15-FTE-AllSuppress',                    False,int(4)  ],
                
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]

        print("+---------------------------------------------------------------------")
        print("Checkpoint 5.Suppressing(Aborting) ONE FTEs before they happen.")
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]''' 
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True) 
        timenow=0                                    
        timenow=int(epics.pv.get_pv(self.prefix+'Timing-Time')._getarg('value')[0])+60
        pvList= [
                ['TR15-STM-getState',                                 None ,"","","",""],
                ['TR15-FTE-LevelSet ',                                   "LOW" ,"","","",""],
                ['Timing-Time'  ,                                     None,"","","",""],
                ['Timing-TimeSec'  ,                                  None,"","","",""],
                ['TR15-FTE-StartTimeSet',            np.array([timenow,int(0)]),"","","",""],
                ['TR15-FTE-StopTimeSet',           np.array([timenow+1,int(0)]),"","","",""],
                ['TR15-FTE-ModeSet',                                   "PULSE" ,"","","",""],
                ['TR15-FTE-Set',                                          "ON" ,"","","",""],
                ['TR15-FTE-StartTimeSet',         np.array([timenow+10,int(0)]),"","","",""],
                ['TR15-FTE-StopTimeSet',          np.array([timenow+11,int(0)]),"","","",""],
                ['TR15-FTE-ModeSet',                                   "PULSE" ,"","","",""],
                ['TR15-FTE-Set',                                          "ON" ,"","","",""],
                ['TR15-FTE-SetCode',                                      None ,"","","",""],
                ['TR15-FTE-SetStatus',                                    None ,"","","",""],
                ['TR15-FTE-StartTimeSuppress',      np.array([timenow,int(0)]) ,"","","",""],
                ['TR15-FTE-ModeSuppress',                                "FTE" ,"","","",""],
                ['TR15-FTE-Suppress',                                     "ON" ,"","","",""],
                ['TR15-FTE-StartTimeSuppress',    np.array([timenow+1,int(0)]) ,"","","",""],
                ['TR15-FTE-Suppress',                                     "ON" ,"","","",""],
                ['TR15-FTE-SuppressCode',                                 None ,"","","",""],
                ['TR15-FTE-SuppressStatus',                               None ,"","","",""],   
                ['TR15-FTE-TerminalPending',                           int(1) ,"","","",""],
                ['TR15-FTE-PendingCode',                                  None ,"","","",""],
                ['TR15-FTE-PendingStatus',                                None ,"","","",""],
                ['TR15-FTE-PendingValue',                                 None ,"","","",""],
                ['Timing-Time'  ,                                    None ,"","","",""],                                            
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
                ['TR15-STM-getState',                  True,int(4)  ],
                ['TR15-FTE-LevelSet ',                    False,int(4)  ],
                ['Timing-Time'  ,                    False,int(4)  ],
                ['Timing-TimeSec'  ,                 False,int(4)  ],
                ['TR15-FTE-StartTimeSet',                 False,int(4)  ],
                ['TR15-FTE-StopTimeSet',                  False,int(4)  ],
                ['TR15-FTE-ModeSet',                      False,int(4)  ],
                ['TR15-FTE-Set',                          False,int(4)  ],
                ['TR15-FTE-StartTimeSet',                 False,int(4)  ],
                ['TR15-FTE-StopTimeSet',                  False,int(4)  ],
                ['TR15-FTE-ModeSet',                      False,int(4)  ],
                ['TR15-FTE-Set',                          False,int(4)  ],
                ['TR15-FTE-SetCode',                       True,int(0)  ],
                ['TR15-FTE-SetStatus',                     True,"OK"  ],
                ['TR15-FTE-StartTimeSuppress',            False,int(1)  ],
                ['TR15-FTE-ModeSuppress',                 False,int(1)  ],
                ['TR15-FTE-Suppress',                     False,int(1)  ],
                ['TR15-FTE-StartTimeSuppress',            False,int(1)  ],
                ['TR15-FTE-Suppress',                     False,int(1)  ],
                ['TR15-FTE-SuppressCode',                  True,int(0)  ],
                ['TR15-FTE-SuppressStatus',                True,"OK"  ],
                ['TR15-FTE-TerminalPending',              False,int(1)  ],
                ['TR15-FTE-PendingCode',                   True,int(0)  ],
                ['TR15-FTE-PendingStatus',                 True,"OK"  ],
                ['TR15-FTE-PendingValue',                  True,int(2)  ],
                ['Timing-Time'  ,                    False,int(1)  ],
                

                 ]       
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        print("+---------------------------------------------------------------------")
        print("Wait a minute for timestamp...")
        sleep(60)
        
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
                    ['TR15-STM-setState         ',        ON ,"","","",""],
                    ['TR15-STM-getState         ',        None,"","","",""],
                    ['TR2-STM-setState          ',        ON ,"","","",""],
                    ['TR2-STM-getState          ',        None,"","","",""],
                    ['TR15-STM-setState         ',        OFF ,"","","",""],
                    ['TR15-STM-getState         ',        None,"","","",""],
                    ['TR2-STM-setState          ',        OFF ,"","","",""],
                    ['TR2-STM-getState          ',        None,"","","",""],
                    ['Timing-STM-setState       ',         OFF,"","","",""],
                    ['Timing-STM-getState       ',        None,"","","",""],
                    ['Timing-STM-setState       ',         OFF,"","","",""],
                    ['Routing-STM-setState',               OFF,"","","",""],
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
                    ['TR15-STM-setState'         ,       False,ON   ],
                    ['TR15-STM-getState         ',       False,OFF  ],
                    ['TR2-STM-setState'          ,       False,ON   ],
                    ['TR2-STM-getState          ',       False,OFF  ],
                    ['TR15-STM-setState'         ,       False,OFF  ],
                    ['TR15-STM-getState         ',        True,OFF  ],
                    ['TR2-STM-setState'          ,       False,OFF  ],
                    ['TR2-STM-getState          ',        True,OFF  ],
                    ['Timing-STM-setState       ',       False,OFF  ],   
                    ['Timing-STM-getState       ',        True,OFF  ],
                    ['Timing-STM-setState       ',        True,OFF  ],
                    ['Routing-STM-setState',              True,OFF  ],
                    ['STM-getGlobalState        ',        True,OFF  ],
                    ['STM-getState              ',        True,OFF  ],
                    ['STM-setState              ',        True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      

if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
