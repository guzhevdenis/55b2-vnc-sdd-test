import unittest
from NisyncUTFixture import TestNdsNisync
import epics
import os

class RoutingTest(TestNdsNisync):
        def test_Routing(self):
            pvList= self.Routing()
            '''Call print function'''
            self.printTestResults(pvList)
            
            
        def Routing(self):
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
                    ['STM-setState',           ON ,"","","",""],
                    ['Routing-STM-setState',            ON,"","","",""],
                    ['Routing-ClkDst',                  None,"","","",""],
                    ['Routing-ClkDstRead',              None,"","","",""],
                    ['Routing-ClkSet',                  None,"","","",""],  
                    ['Routing-ClkSetCode',              None,"","","",""],
                    ['Routing-ClkSetStatus',            None,"","","",""],
                    ['Routing-ClkSrc',                  None,"","","",""],  
                    ['Routing-ClkSrcRead',              None,"","","",""],
                    ['Routing-STM-getGlobalState',      None,"","","",""],
                    ['Routing-STM-getState',            None,"","","",""],  
                    ['Routing-TermDst',                 None,"","","",""],
                    ['Routing-TermDstRead',             None,"","","",""],  
                    ['Routing-TermInvertRead',          None,"","","",""],
                    ['Routing-TermInvertSet',           None,"","","",""],
                    ['Routing-TermSet',                 None,"","","",""],  
                    ['Routing-TermSetCode',             None,"","","",""],
                    ['Routing-TermSetStatus',           None,"","","",""],
                    ['Routing-TermSrc',                 None,"","","",""],  
                    ['Routing-TermSrcRead',             None,"","","",""],
                    ['Routing-TermSyncRead',            None,"","","",""],
                    ['Routing-TermSyncSet',             None,"","","",""],  
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

        