import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os


class NPD0003(TestNdsNisync):
    def test_NPD0003(self):
        epics.ca.use_initial_context()
        try:
                os.remove(self.resultFilePath)                                                                                             
        except OSError:                                                                                       
                pass                                                                                          
        '''Start'''
        self.StateMachinesON()
        raw_input("Test ready. Press enter to Start")
        
        print("Not applicable to 6683H.")
        
        '''End'''
        raw_input("Test complete. Press enter to switch off nds-nisync device.")
        self.StateMachinesOFF()
        
                                                                                      

    
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
                    ['STM-getGlobalState        ',        None,"","","",""],
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
                    ['STM-getGlobalState        ',        True,OFF  ],   
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
                    ['STM-getGlobalState        ',        None,"","","",""],
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
                    ['STM-getGlobalState        ',        True,OFF  ],
                    ['STM-getState              ',        True,OFF  ],
                    ['STM-setState              ',        True,OFF  ],
                ]
            self.assertPVList(pvList,assertList)      

if __name__ == '__main__':
    unittest.main()
    #unittest.TextTestRunner().run(NPD0000)
