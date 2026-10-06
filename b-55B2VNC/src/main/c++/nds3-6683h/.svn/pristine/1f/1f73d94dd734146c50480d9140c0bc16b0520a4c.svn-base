##!/usr/bin/env python
import sys
import os
import unittest
import epics
from epics import ca
import subprocess
from time import sleep, localtime
import iocControl
import ioctests
import numpy as np
from __builtin__ import iter
from gtk._gtk import TREE_MODEL_LIST_ONLY
from _elementtree import TreeBuilder
import NisyncPVlist as NPV
from UserList import UserList
from numpy import alen, iterable
import select

def handle_messages(text):
    print(text)
    pass



class TestNdsNisync(unittest.TestCase): #unittest.TestCase
    '''Testing basic load and initialization of NDS driver in EPICS'''
    ''' 1-setup
    2-test***
    3-tearDown
    '''
    prefix =""
    ioc= None
    pvFilePath  = ""
    pvList=[]                                 # self.pvList will be used as the test data holder
    chkList=[]
    tsList = []
    lastCB=''
    monitFlag = 0
    printFlag = 0
    checkpointN=1

    def setUp(self):                     
            '''Other test utilities'''
            self.pvFilePath  = os.getcwd()+'/pvList.txt'
            self.resultFilePath  = os.getcwd()+'/'+self._testMethodName +'Results.log'       

            CBS1 = 'TEST'
            CBS2 = 'FCT0'
            BOARDTYPE = 'B'
            BOARDTYPEIDX='0'
            startFile=''
               
            epics.ca.initialize_libca()
            epics.ca.replace_printf_handler(handle_messages)
            self.ioc = iocControl.iocControl()
            self.ioc.iocFromSTfile(startFile)
            sleep(1)
            self.ioc.iocProcess.stdin.write('dbl >'+ self.pvFilePath +'\n')
            self.ioc.iocProcess.stdin.flush()
            sleep(1)
            print("+---------------------------------------------------------------------")
            print("|                    CHECKING TESTS")
            print("+---------------------------------------------------------------------")    
            print("|")
            '''delete previous result, prepare prefix name'''
            with open(self.pvFilePath) as m_file :
                self.pvList = m_file.readlines()
            self.prefix = CBS1+'-'+CBS2+'-HWCF:'+BOARDTYPE+'-'+BOARDTYPEIDX+'-'
            self.userList=NPV.getPVList()
            '''------------------------check if userList constains pvList--------------'''    
            for i in range(len(self.userList)):
                self.chkList.insert(i,self.prefix+str(self.userList[i].strip()))
                #print(i, self.chkList[i]) #debug
            for i in range(len(self.pvList)):
                self.pvList[i]=self.pvList[i].strip()
                #print (i,self.pvList[i]) #debug
            chkSet=set(self.pvList).difference(set(self.chkList))
            if len(chkSet) != 0:
                i=0
                while(chkSet!= set()):
                    print (i, chkSet.pop(),'Warning! PV not in test cases!')
                    i=i+1
            print("| Finished.")  
            print("+---------------------------------------------------------------------")    
            print("|")    
            self.pvList=[]
    def tearDown(self):
        '''return to test path'''
        os.chdir(os.path.dirname(self.pvFilePath))
        epics.ca.finalize_libca()
        os.remove(self.pvFilePath)
        if self.ioc is not None:
            self.ioc.stop()       
     
    def printTestResults(self,pvlist,timestamps=None, decimation=1):
        '''Print Results'''
        print('Logging test to '+self.resultFilePath)
        if timestamps:
            with open(self.resultFilePath,mode="at+") as res_file :
                bkpStd=sys.stdout
                sys.stdout=res_file
                print("+-----------TIMESTAMPS--------------------------------->")
                print("{0:<45},{1:<45},{2}").format("PVname","HWTimestamp","EPICSTimestamp")
                for i in range(len(pvlist)) :
                    if(i%decimation==0):
                        print("{0:<45},{1:<15},{2}").format(
                                pvlist[i][0],
                                pvlist[i][1],
                                pvlist[i][2],
                                ) 
                sys.stdout=bkpStd    
                    
        else:
            col0=max([len(c) for c in [row[0] for row in pvlist]])
            col1=max([len(str(c)) for c in [row[1] for row in pvlist]])+4
            col2=max([len(str(c)) for c in [row[2] for row in pvlist]])
            col3=max([len(str(c)) for c in [row[3] for row in pvlist]])
            col4=max([len(str(c)) for c in [row[4] for row in pvlist]])+1
            col5=int(10)
            fmtPVstr=''.join(["{0:<"+str(col0)+"},{1:<"+str(col1)+"},{2:<" +str(col2)+"},{3:^"+str(col3)+"},{4:^"+str(col4)+"},{5:<"+str(col5)+"}"])
            with open(self.resultFilePath,mode="at+") as res_file :
                bkpStd=sys.stdout
                sys.stdout=res_file
                print("+-----------CHECKPOINT#"+str(self.checkpointN)+"----------------------------------")
                print("|")
                print(fmtPVstr.format("PVname","PVtry","PVread","initTS","lastTS","|Result"))
                for i in range(len(pvlist)) :
                    print(fmtPVstr.format(
                            pvlist[i][0],
                            pvlist[i][1],
                            pvlist[i][2],
                            pvlist[i][3],
                            pvlist[i][4],
                            pvlist[i][5]
                            ) )
                sys.stdout=bkpStd
            self.checkpointN=self.checkpointN+1

    def monitCallback(self,pvname,*args,**kwds):
        #epics.ca.use_initial_context()
        if pvname != 0 :
            if self.lastCB != pvname :
                #print(pvname,kwds['type'],kwds['count']) debug
                if ("char" in kwds['type'])&(kwds['count'] >1) :
                    print("{0:<45},{1:<15},{2}").format(pvname,''.join([chr(c) for c in kwds['value']]),epics.pv.fmt_time(kwds['timestamp']))
                else:
                    print("{0:<45},{1:<15},{2}").format(pvname,kwds['value'],epics.pv.fmt_time(kwds['timestamp']))
            self.monitFlag = 1
        else :
            print('error callback pvname')
        self.lastCB=pvname
    '''Another monitor callback to manage timestamps, unmanaged errors if called in parallel to monitCallback'''    
    def monitTSCallback(self,pvname,*args,**kwds):
        #epics.ca.use_initial_context()
        if ("Timestamps" in pvname) :
            self.tsList.append([pvname,kwds['value'],epics.pv.fmt_time(kwds['timestamp'])])
            print("\033[92m"+"{0:<45},{1:<15},{2}"+"\033[0m").format(pvname,kwds['value'],epics.pv.fmt_time(kwds['timestamp']))
            self.monitFlag = 1
        else :
            print('error callback pvname')  

    def checkPVlist(self,pvlist):
        #epics.ca.use_initial_context()
        ''' append new timestamp to "initTS[3] column"'''
        for i in range(len(pvlist)) :
                pvlist[i][3]=epics.pv.fmt_time(epics.pv.get_pv(pvlist[i][0].strip(),
                                                 connect=True)._getarg('timestamp'))
                #Remove comment to debug: print "{0:>3},{1:<60},{2:12}".format(i,   pvlist[i][0],epics.pv.fmt_time(pvlist[i][3])) 
                epics.ca.poll()
        '''CAPUT values, to "PVREAD[2] column"'''       
        for i in range(len(pvlist)) : 
            if (type(pvlist[i][1]).__name__!= 'NoneType'): 
                print('---caput---->  ' + str(pvlist[i]))
                epics.caput(pvlist[i][0], pvlist[i][1] , wait=True, timeout=5)
                sleep(0.3)
                epics.ca.poll(1,1)
                if self.monitFlag == 0:
                    print('Timeout: '+ str(pvlist[i][0]))
                self.monitFlag = 0
                print("+---------------------------------------------------------------------")
        '''CAGET new values, append new timestamp to "lastTS[4] column"'''
        for i in range(len(pvlist)) :
            pvget=epics.pv.get_pv(pvlist[i][0].strip(),connect=True)
            pvlist[i][4]=epics.pv.fmt_time(pvget._getarg('timestamp'))
            if ("char" in pvget._getarg('type'))&(pvget._getarg('count')>1):
                pvlist[i][2]=''.join([chr(c) for c in pvget._getarg('value')])
            else:
                pvlist[i][2]=pvget._getarg('value')
            epics.ca.poll()
        # This is the code for debugging: getlist=[row[0] for row in pvlist]
        #                                 readbacks=epics.caget_many(getlist)
        #                                 for i in range(len(pvlist)) :
        #                                       pvlist[i][2]=readbacks[i]
        return pvlist   
    def assertPVList(self,pvlist,assertlist):
        for i in range(len(pvlist)) :
            if(assertlist[i][1]):
                pvlist[i][5]=("|PASSED" if ((assertlist[i][2])==(pvlist[i][2]))else "|FAILED")
                FAILMSG=pvlist[i][0]+"[FAILED]"+"\n"+"Got:"+str(pvlist[i][2])+"\n"+"Expected:"+str(assertlist[i][2])+"\n"
                                     
                try:
                    self.assertEqual(pvlist[i][2], assertlist[i][2],pvlist[i][0]+"[FAILED]")
                except:
                    self.printTestResults(pvlist)
                    self.fail(FAILMSG)
        
            else:
                pvlist[i][5]="|N/A"
        return pvlist
    def waitInput(self,msg="Press enter to continue.."):
        print(msg)
        read_inp=[sys.stdin]
        ready = None
        while not ready:
            ready = select.select(read_inp,[],[],0)[0]
            if not ready:
                sleep(0.5)
            else:
                line = sys.stdin.readline()
       
if __name__ == '__main__':
    unittest.main()

