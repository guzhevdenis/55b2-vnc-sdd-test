import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os
import numpy as np
import time


class NPD0005(TestNdsNisync):
    def test_NPD0005(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)                                                                                             
        except OSError:                                                                                       
                pass                                                                                         
        '''Start'''
        self.StateMachinesON()
        print("+---------------------------------------------------------------------")
        print("|TEST0005:Test verifies terminal connection and signal inversion.     ")
        print("|         Routing backplane clocks, syncing signal output and FTE clock generation.")
        print("+---------------------------------------------------------------------")    
        print("|")
        print("Requirement(s):")
        print("-SYNC-SRS-F-55")
        print("-SYNC-SRS-F-56")
        print("-SYNC-SRS-F-57")
        print("-SYNC-SRS-F-67")
        print("-SYNC-SRS-F-68")
        print("-SYNC-SRS-F-69")
        print("-SYNC-SRS-F-70")
        print("-SYNC-SRS-F-71")
        raw_input("Test ready. Press enter to Start")
        
        '''Track checkpoints to follow test plan'''
        self.routeInvClk()
        
        '''End'''
        print("+---------------------------------------------------------------------")
        raw_input("Test complete. Press enter to switch off nds-nisync driver.")
        self.StateMachinesOFF()
        
    def routeInvClk(self):                                                                                       
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
        raw_input("Checkpoint 1. Connect oscilloscope to PFI2 terminal. Press enter to continue.")
        print("+---------------------------------------------------------------------")                                                    
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['Timing-STM-setState',               "RUNNING","","","",""],
                ['Routing-STM-getState',                  None ,"","","",""],                                                         
                ['Routing-STM-setState',                int(4) ,"","","",""],
                ['Routing-TermSrc',                    int(36) ,"","","",""],
                ['Routing-TermDst',                     int(2) ,"","","",""],
                ['Routing-TermSet',                       "ON" ,"","","",""],
                ['Routing-TermSetCode',                   None ,"","","",""],
                ['Routing-TermSetStatus',                 None ,"","","",""],                                                                                          
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
                ['Timing-STM-setState',                True,RUNNING ], 
                ['Routing-STM-getState',               True,int(4)  ],                                                         
                ['Routing-STM-setState',               True,int(4)  ],
                ['Routing-TermSrc',                   False,int(36) ],
                ['Routing-TermDst',                   False,int(2)  ], 
                ['Routing-TermSet',                   False,int(1)  ],
                ['Routing-TermSetCode',                True,int(0)    ],                                                         
                ['Routing-TermSetStatus',              True,"OK"    ],                                                        
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 2. Connect oscilloscope to PFI0 terminal. Press enter to continue.") 
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['Routing-STM-getState',                  None ,"","","",""],                                                         
                ['Routing-TermSrc',                    int(36) ,"","","",""],
                ['Routing-TermDst',                     int(0) ,"","","",""],
                ['Routing-TermSet',                       "ON" ,"","","",""],
                ['Routing-TermSetCode',                   None ,"","","",""],
                ['Routing-TermSetStatus',                 None ,"","","",""],                                                                           
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
                ['Routing-STM-getState',               True,int(4)  ],                                                         
                ['Routing-TermSrc',                   False,int(1)  ],
                ['Routing-TermDst',                   False,int(5)  ], 
                ['Routing-TermSet',                   False,int(1)  ],
                ['Routing-TermSetCode',                True,int(0)    ],                                                         
                ['Routing-TermSetStatus',              True,"OK"  ],                                                        
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 3.Inverting PFI0 terminal. Press enter to continue.") 
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['Routing-STM-getState',                  None ,"","","",""],                                                         
                ['Routing-TermInvertSet',                 "ON" ,"","","",""],
                ['Routing-TermSet',                       "ON" ,"","","",""],
                ['Routing-TermDstRead',                 int(0) ,"","","",""],
                ['Routing-TermSyncRead',                  None ,"","","",""],
                ['Routing-TermSrcRead',                   None ,"","","",""],
                ['Routing-TermInvertRead',                None ,"","","",""],
                                                                                                          
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
                ['Routing-STM-getState',               True,int(4)  ],                                                         
                ['Routing-TermInvertSet',             False,int(1)  ],
                ['Routing-TermSet',                   False,int(1)  ],
                ['Routing-TermDstRead',               False,int(0)  ],
                ['Routing-TermSyncRead',               True,int(0)  ],
                ['Routing-TermSrcRead',                True,int(36) ],
                ['Routing-TermInvertRead',             True,int(1)  ],
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 4. Disconnect the PFI0 connector. Connect Oscilloscope to CLK_OUT.") 
        print("Routing Backplane clk to CLK_OUT. Press enter to continue.")
        print("+---------------------------------------------------------------------")
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['Routing-STM-getState',                  None ,"","","",""],
                ['Routing-TermSrc',                    int(12) ,"","","",""],
                ['Routing-TermDst',                     int(2) ,"","","",""],
                ['Routing-TermInvertSet',                "OFF" ,"","","",""],                                                         
                ['Routing-TermSyncSet',                  "OFF" ,"","","",""],
                ['Routing-TermSet',                       "ON" ,"","","",""],
                ['Routing-TermDstRead',                 int(2) ,"","","",""],
                ['Routing-TermSyncRead',                  None ,"","","",""],
                ['Routing-TermSrcRead',                   None ,"","","",""],
                ['Routing-TermInvertRead',                None ,"","","",""],
                ['Routing-ClkSrc',                      int(2) ,"","","",""],  #backplane to clk out                                                                                                       
                ['Routing-ClkDst',                      int(5) ,"","","",""],                                                        
                ['Routing-ClkSet',                        "ON" ,"","","",""],                                                         
                ['Routing-ClkSetCode',                    None ,"","","",""],
                ['Routing-ClkSetStatus',                  None ,"","","",""],
                ['Routing-ClkDstRead',                  int(5) ,"","","",""],                                                         
                ['Routing-ClkSrcRead',                    None ,"","","",""],
                                                                                       
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
                ['Routing-STM-getState',             True,int(4)  ],        
                ['Routing-TermSrc',                 False,int(1)  ],
                ['Routing-TermDst',                 False,int(1)  ],
                ['Routing-TermInvertSet',           False,int(1)  ],
                ['Routing-TermSyncSet',             False,int(1)  ],
                ['Routing-TermSet',                 False,int(1)  ],
                ['Routing-TermDstRead',              True,int(2)  ],
                ['Routing-TermSyncRead',             True,int(0)  ],
                ['Routing-TermSrcRead',              True,int(12)  ],
                ['Routing-TermInvertRead',           True,int(0)  ],
                ['Routing-ClkSrc',                  False,int(1)  ],
                ['Routing-ClkDst',                  False,int(1)  ],
                ['Routing-ClkSet',                  False,int(1)  ],
                ['Routing-ClkSetCode',               True,int(0)  ],
                ['Routing-ClkSetStatus',             True,"OK"  ],
                ['Routing-ClkDstRead',               True,int(5)  ],
                ['Routing-ClkSrcRead',               True,int(2)  ],                
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 5. Scheduling FTE clock on PXI_TRIG1, which is routed to FPI2. NOT SYNCED. Press enter to continue.")
        print("+---------------------------------------------------------------------") 
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]''' 
        maxtime=2147483647  #end of EPICS time
        epics.caput(self.prefix+'Timing-Time.PROC', int(1), True)                                   
        timenow=int(epics.pv.get_pv(self.prefix+'Timing-Time')._getarg('value')[0])+30                                   
        pvList= [
                ['TR12-STM-setState',                             ON ,"","","",""],
                ['TR12-FTE-Enable ',                             "ENABLE" ,"","","",""],
                ['TR12-FTE-Enable_RBV ',                         "ENABLE" ,"","","",""],
                ['TR12-FTE-StartTimeSet',      np.array([timenow,int(0)]) ,"","","",""],
                ['TR12-FTE-StopTimeSet',       np.array([maxtime,maxtime]) ,"","","",""],
                ['TR12-FTE-LevelSet ',                             "LOW" ,"","","",""],
                ['TR12-FTE-ModeSet',                               "CLK" ,"","","",""],
                ['TR12-FTE-DutyCycleSet',                      int(50) ,"","","",""], 
                ['TR12-FTE-PeriodNsecSet',                  int(1000000) ,"","","",""], 
                ['TR12-FTE-Set',                                    int(1) ,"","","",""],
                ['TR12-FTE-SetCode',                                None ,"","","",""],
                ['TR12-FTE-SetStatus',                              None ,"","","",""], 
                ['TR12-STM-getState',                           None ,"","","",""],
                ['Timing-Time',                                None ,"","","",""],                                    
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
                ['TR12-STM-setState',              False,int(4)  ],   
                ['TR12-FTE-Enable',              False,"ENABLE"  ],
                ['TR12-FTE-Enable_RBV',             True,int(1)  ],     
                ['TR12-FTE-StartTimeSet',          False,int(1)  ],
                ['TR12-FTE-StopTimeSet',           False,int(1)  ],
                ['TR12-FTE-LevelSet ',             False,int(1)  ],
                ['TR12-FTE-ModeSet',               False,int(1)  ],
                ['TR12-FTE-DutyCycleSet',          False,int(1)  ],
                ['TR12-FTE-PeriodNsecSet',         False,int(1)  ],
                ['TR12-FTE-Set',                   False,int(1)  ],
                ['TR12-FTE-SetCode',                True,int(0)  ],
                ['TR12-FTE-SetStatus',              True,"OK"  ],
                ['TR12-STM-getState',           True,int(4)  ],
                ['Timing-Time',               False,int(1)  ],

            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 6. Wait up to 1 min for clock to apear in PFI2.Sync-ing PFI2 to backplane Rising Edges... Press enter to continue.")
        print("+---------------------------------------------------------------------") 
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['Routing-TermSyncSet',           "ON" ,"","","",""],
                ['Routing-TermSet',               "ON" ,"","","",""],
                ['Routing-TermDstRead',         int(2) ,"","","",""],   
                ['Routing-TermSyncRead',          None ,"","","",""],
                ['Routing-TermSrcRead',           None ,"","","",""],
                ['Routing-TermInvertRead',        None ,"","","",""],                                                                               
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
                ['Routing-TermSyncSet',                False,int(1)  ],
                ['Routing-TermSet',                    False,int(1)  ],  
                ['Routing-TermDstRead',                 True,int(2)  ],
                ['Routing-TermSyncRead',                True,int(1)  ],
                ['Routing-TermSrcRead',                 True,int(12)  ],
                ['Routing-TermInvertRead',              True,int(0)  ],
            ]
        self.assertPVList(pvList,assertList)            
        '''Call print function'''
        self.printTestResults(pvList)
        pvList=[]
        
        print("+---------------------------------------------------------------------")
        raw_input("Checkpoint 7. To disable clock generation. Press enter to continue.")
        print("+---------------------------------------------------------------------") 
        '''TEST PVLIST FIELDS: PVNAME[0], PVPUT[1],PVGET[2],initTS[3],lastTS[4],Result[5]'''                                      
        pvList= [
                ['TR12-FTE-ModeSuppress',                          "CLK" ,"","","",""],
                ['TR12-FTE-Suppress',                               "ON" ,"","","",""],
                ['TR12-FTE-SuppressCode',                           None ,"","","",""],
                ['TR12-FTE-SuppressStatus',                         None ,"","","",""],                                                                              
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
                ['TR12-FTE-ModeSuppress',               False,int(1)  ],  
                ['TR12-FTE-Suppress',                   False,int(1)  ],
                ['TR12-FTE-SuppressCode',                True,int(0)  ],
                ['TR12-FTE-SuppressStatus',              True,"OK"  ],
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
                    ['STM-getState',           None,"","","",""],
                    ['STM-setState',             ON,"","","",""],
                    ['Timing-STM-setState',             "ON","","","",""],

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
                    ['STM-getState',                True,ON  ],   
                    ['STM-setState',                True,ON  ], 
                    ['Timing-STM-setState',                  True,ON  ],     
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
                    ['Routing-STM-setState',           "OFF","","","",""],
                    ['Timing-STM-setState',            "OFF","","","",""],
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
                    ['Timing-STM-setState',                       True,OFF  ],
                    ['Routing-STM-setState',                      True,OFF  ],
                    ['Timing-STM-setState',                       True,OFF  ],
                    ['STM-getState',                     True,OFF  ],
                    ['STM-setState',                     True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      

if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
