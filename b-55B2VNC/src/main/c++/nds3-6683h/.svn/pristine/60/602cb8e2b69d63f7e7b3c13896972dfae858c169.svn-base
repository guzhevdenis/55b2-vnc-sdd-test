import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os

class StateMachineTest(TestNdsNisync):
        def test_StateMachines(self):
            pvList= self.StateMachinesON()
            '''Call print function'''
            self.printTestResults(pvList)
            '''TEST OFF STATE'''
            pvList= self.StateMachinesOFF()
            self.printTestResults(pvList)
        
        def StateMachinesON(self):
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
                    ['Firm-STM-getGlobalState   ',        None,"","","",""],
                    ['Firm-STM-getState         ',        None,"","","",""],
                    ['Firm-STM-setState         ',        ON,"","","",""],
                    ['HQ-STM-getGlobalState     ',        None,"","","",""],
                    ['HQ-STM-getState           ',        None,"","","",""],
                    ['HQ-STM-setState           ',        ON,"","","",""],
                    ['STM-getGlobalState        ',        None,"","","",""],
                    ['STM-getState              ',        None,"","","",""],
                    ['STM-setState              ',        ON,"","","",""],
                    ['TR0-STM-getGlobalState ',        None,"","","",""],
                    ['TR0-STM-getState       ',        None,"","","",""],
                    ['TR0-STM-setState       ',        ON,"","","",""],
                    ['TR1-STM-getGlobalState ',        None,"","","",""],
                    ['TR1-STM-getState       ',        None,"","","",""],
                    ['TR1-STM-setState       ',        ON,"","","",""],
                    ['TR11-STM-getGlobalState',        None,"","","",""],
                    ['TR11-STM-getState      ',        None,"","","",""],
                    ['TR11-STM-setState      ',        ON,"","","",""],
                    ['TR12-STM-getGlobalState',        None,"","","",""],
                    ['TR12-STM-getState      ',        None,"","","",""],
                    ['TR12-STM-setState      ',        ON,"","","",""],
                    ['TR13-STM-getGlobalState',        None,"","","",""],
                    ['TR13-STM-getState      ',        None,"","","",""],
                    ['TR13-STM-setState      ',        ON,"","","",""],
                    ['TR14-STM-getGlobalState',        None,"","","",""],
                    ['TR14-STM-getState      ',        None,"","","",""],
                    ['TR14-STM-setState      ',        ON,"","","",""],
                    ['TR15-STM-getGlobalState',        None,"","","",""],
                    ['TR15-STM-getState      ',        None,"","","",""],
                    ['TR15-STM-setState      ',        ON,"","","",""],
                    ['TR16-STM-getGlobalState',        None,"","","",""],
                    ['TR16-STM-getState      ',        None,"","","",""],
                    ['TR16-STM-setState      ',        ON,"","","",""],
                    ['TR17-STM-getGlobalState',        None,"","","",""],
                    ['TR17-STM-getState      ',        None,"","","",""],
                    ['TR17-STM-setState      ',        ON,"","","",""],
                    ['TR18-STM-getGlobalState',        None,"","","",""],
                    ['TR18-STM-getState      ',        None,"","","",""],
                    ['TR18-STM-setState      ',        ON,"","","",""],
                    ['TR2-STM-getGlobalState ',        None,"","","",""],
                    ['TR2-STM-getState       ',        None,"","","",""],
                    ['TR2-STM-setState       ',        ON,"","","",""],
                    ['Timing-STM-getGlobalState ',        None,"","","",""],
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
                    ['Firm-STM-getGlobalState   ',        True,OFF  ],   
                    ['Firm-STM-getState         ',        True,ON  ],   
                    ['Firm-STM-setState         ',        True,ON  ],   
                    ['HQ-STM-getGlobalState     ',        True,OFF  ],   
                    ['HQ-STM-getState           ',        True,ON  ],   
                    ['HQ-STM-setState           ',        True,ON  ],   
                    ['STM-getGlobalState        ',        True,OFF  ],   
                    ['STM-getState              ',        True,ON  ],   
                    ['STM-setState              ',        True,ON  ],   
                    ['TR0-STM-getGlobalState ',        True,OFF  ],   
                    ['TR0-STM-getState       ',        True,ON  ],   
                    ['TR0-STM-setState       ',        True,ON  ],   
                    ['TR1-STM-getGlobalState ',        True,OFF  ],   
                    ['TR1-STM-getState       ',        True,ON  ],   
                    ['TR1-STM-setState       ',        True,ON  ],   
                    ['TR11-STM-getGlobalState',        True,OFF  ],   
                    ['TR11-STM-getState      ',        True,ON  ],   
                    ['TR11-STM-setState      ',        True,ON  ],   
                    ['TR12-STM-getGlobalState',        True,OFF],   
                    ['TR12-STM-getState      ',        True,ON  ],   
                    ['TR12-STM-setState      ',        True,ON  ],   
                    ['TR13-STM-getGlobalState',        True,OFF],   
                    ['TR13-STM-getState      ',        True,ON  ],   
                    ['TR13-STM-setState      ',        True,ON  ],   
                    ['TR14-STM-getGlobalState',        True,OFF  ],   
                    ['TR14-STM-getState      ',        True,ON  ],   
                    ['TR14-STM-setState      ',        True,ON  ],   
                    ['TR15-STM-getGlobalState',        True,OFF  ],   
                    ['TR15-STM-getState      ',        True,ON  ],   
                    ['TR15-STM-setState      ',        True,ON  ],   
                    ['TR16-STM-getGlobalState',        True,OFF  ],   
                    ['TR16-STM-getState      ',        True,ON  ],   
                    ['TR16-STM-setState      ',        True,ON  ],   
                    ['TR17-STM-getGlobalState',        True,OFF  ],   
                    ['TR17-STM-getState      ',        True,ON  ],   
                    ['TR17-STM-setState      ',        True,ON  ],   
                    ['TR18-STM-getGlobalState',        True,OFF  ],   
                    ['TR18-STM-getState      ',        True,ON  ],   
                    ['TR18-STM-setState      ',        True,ON  ],   
                    ['TR2-STM-getGlobalState ',        True,OFF  ],   
                    ['TR2-STM-getState       ',        True,ON  ],   
                    ['TR2-STM-setState       ',        True,ON  ],   
                    ['Timing-STM-getGlobalState ',        True,OFF  ],   
                    ['Timing-STM-getState       ',        True,ON  ],   
                    ['Timing-STM-setState       ',        True,ON  ],   
                ]
            for i in range(len(pvList)) :
                if(assertList[i][1]):
                    pvList[i][5]=("|PASSED" if assertList[i][2]==pvList[i][2] else "|FAILED")
                    try:
                        self.assertEqual(pvList[i][2], assertList[i][2],pvList[i][0]+"[FAILED]")
                    except:
                        self.printTestResults(pvList)
                        self.fail(pvList[i][0]+"[FAILED]")
            
                else:
                    pvList[i][5]="|N/A"
            return pvList            
            #raw_input("Press enter to switch off STM")

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
                    ['Firm-STM-getGlobalState   ',        None,"","","",""],
                    ['Firm-STM-getState         ',        None,"","","",""],
                    ['Firm-STM-setState         ',        OFF,"","","",""],
                    ['HQ-STM-getGlobalState     ',        None,"","","",""],
                    ['HQ-STM-getState           ',        None,"","","",""],
                    ['HQ-STM-setState           ',        OFF,"","","",""],
                    ['STM-getGlobalState        ',        None,"","","",""],
                    ['STM-getState              ',        None,"","","",""],
                    ['STM-setState              ',        OFF,"","","",""],
                    ['TR0-STM-getGlobalState ',        None,"","","",""],
                    ['TR0-STM-getState       ',        None,"","","",""],
                    ['TR0-STM-setState       ',        OFF,"","","",""],
                    ['TR1-STM-getGlobalState ',        None,"","","",""],
                    ['TR1-STM-getState       ',        None,"","","",""],
                    ['TR1-STM-setState       ',        OFF,"","","",""],
                    ['TR11-STM-getGlobalState',        None,"","","",""],
                    ['TR11-STM-getState      ',        None,"","","",""],
                    ['TR11-STM-setState      ',        OFF,"","","",""],
                    ['TR12-STM-getGlobalState',        None,"","","",""],
                    ['TR12-STM-getState      ',        None,"","","",""],
                    ['TR12-STM-setState      ',        OFF,"","","",""],
                    ['TR13-STM-getGlobalState',        None,"","","",""],
                    ['TR13-STM-getState      ',        None,"","","",""],
                    ['TR13-STM-setState      ',        OFF,"","","",""],
                    ['TR14-STM-getGlobalState',        None,"","","",""],
                    ['TR14-STM-getState      ',        None,"","","",""],
                    ['TR14-STM-setState      ',        OFF,"","","",""],
                    ['TR15-STM-getGlobalState',        None,"","","",""],
                    ['TR15-STM-getState      ',        None,"","","",""],
                    ['TR15-STM-setState      ',        OFF,"","","",""],
                    ['TR16-STM-getGlobalState',        None,"","","",""],
                    ['TR16-STM-getState      ',        None,"","","",""],
                    ['TR16-STM-setState      ',        OFF,"","","",""],
                    ['TR17-STM-getGlobalState',        None,"","","",""],
                    ['TR17-STM-getState      ',        None,"","","",""],
                    ['TR17-STM-setState      ',        OFF,"","","",""],
                    ['TR18-STM-getGlobalState',        None,"","","",""],
                    ['TR18-STM-getState      ',        None,"","","",""],
                    ['TR18-STM-setState      ',        OFF,"","","",""],
                    ['TR2-STM-getGlobalState ',        None,"","","",""],
                    ['TR2-STM-getState       ',        None,"","","",""],
                    ['TR2-STM-setState       ',        OFF,"","","",""],
                    ['Timing-STM-getGlobalState ',        None,"","","",""],
                    ['Timing-STM-getState       ',        None,"","","",""],
                    ['Timing-STM-setState       ',        OFF,"","","",""],
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
                    ['Firm-STM-getGlobalState   ',        True,OFF  ],
                    ['Firm-STM-getState         ',        True,OFF  ],
                    ['Firm-STM-setState         ',        True,OFF  ],
                    ['HQ-STM-getGlobalState     ',        True,OFF  ],
                    ['HQ-STM-getState           ',        True,OFF  ],
                    ['HQ-STM-setState           ',        True,OFF  ],
                    ['STM-getGlobalState        ',        True,OFF  ],
                    ['STM-getState              ',        True,OFF  ],
                    ['STM-setState              ',        True,OFF  ],
                    ['TR0-STM-getGlobalState ',        True,OFF  ],
                    ['TR0-STM-getState       ',        True,OFF  ],
                    ['TR0-STM-setState       ',        True,OFF  ],
                    ['TR1-STM-getGlobalState ',        True,OFF  ],
                    ['TR1-STM-getState       ',        True,OFF  ],
                    ['TR1-STM-setState       ',        True,OFF  ],
                    ['TR11-STM-getGlobalState',        True,OFF  ],
                    ['TR11-STM-getState      ',        True,OFF  ],
                    ['TR11-STM-setState      ',        True,OFF  ],
                    ['TR12-STM-getGlobalState',        True,OFF  ],
                    ['TR12-STM-getState      ',        True,OFF  ],
                    ['TR12-STM-setState      ',        True,OFF  ],
                    ['TR13-STM-getGlobalState',        True,OFF  ],
                    ['TR13-STM-getState      ',        True,OFF  ],
                    ['TR13-STM-setState      ',        True,OFF  ],
                    ['TR14-STM-getGlobalState',        True,OFF  ],
                    ['TR14-STM-getState      ',        True,OFF  ],
                    ['TR14-STM-setState      ',        True,OFF  ],
                    ['TR15-STM-getGlobalState',        True,OFF  ],
                    ['TR15-STM-getState      ',        True,OFF  ],
                    ['TR15-STM-setState      ',        True,OFF  ],
                    ['TR16-STM-getGlobalState',        True,OFF  ],
                    ['TR16-STM-getState      ',        True,OFF  ],
                    ['TR16-STM-setState      ',        True,OFF  ],
                    ['TR17-STM-getGlobalState',        True,OFF  ],
                    ['TR17-STM-getState      ',        True,OFF  ],
                    ['TR17-STM-setState      ',        True,OFF  ],
                    ['TR18-STM-getGlobalState',        True,OFF  ],
                    ['TR18-STM-getState      ',        True,OFF  ],
                    ['TR18-STM-setState      ',        True,OFF  ],
                    ['TR2-STM-getGlobalState ',        True,OFF  ],
                    ['TR2-STM-getState       ',        True,OFF  ],
                    ['TR2-STM-setState       ',        True,OFF  ],
                    ['Timing-STM-getGlobalState ',        True,OFF  ],
                    ['Timing-STM-getState       ',        True,OFF  ],
                    ['Timing-STM-setState       ',        True,OFF  ],
                ]
            for i in range(len(pvList)) :
                if(assertList[i][1]):
                    pvList[i][5]=("|PASSED" if assertList[i][2]==pvList[i][2] else "|FAILED")
                    try:
                        self.assertEqual(pvList[i][2], assertList[i][2],pvList[i][0]+"[FAILED]")
                    except:
                        self.printTestResults(pvList)
                        self.fail(pvList[i][0]+"[FAILED]")
            
                else:
                    pvList[i][5]="|N/A"
            return pvList             