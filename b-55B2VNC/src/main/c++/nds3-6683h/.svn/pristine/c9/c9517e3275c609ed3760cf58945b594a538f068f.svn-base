

#!/usr/bin/env python
'''Controls the test IOC'''
import subprocess
import atexit
import time
import os
import ioctests
from threading import Thread
from time import sleep
from _dbus_bindings import String
from numpy import append
import stat

class iocControl:
    iocProcess = None
    # for debugging: DEVNULL = None
    stopping_flag = 1
    iocBoot_path = ""


    def threaded_function(self):
        '''Thread to read from the pipe'''
        while  self.iocProcess is not None and self.iocProcess.poll() is None:
            if(self.stopping_flag == 0):
                break
            try:    
                output = self.iocProcess.stdout.readline()
                print('* \033[94m' +output.strip()+'\033[0m')
            except:
                print("Exception: iocControl: exception in the thread function used to read ioc output")
                break
    

    def startIOC(self, arglist=None):
        '''Starts the test IOC'''
        relative_path_to_iocBoot_App = '../../iocBoot/iocndsNisync/'
        childEnviron = os.environ.copy()
    
        iocCommand = [ioctests.iocExecutable]
    
        path=os.path.dirname(os.path.realpath(relative_path_to_iocBoot_App))
    
        self.iocBoot_path = path
    
        os.chdir(path)
	       
        '''opening a pipe using subprocess'''
        self.iocProcess = subprocess.Popen(iocCommand, 
                                           env=childEnviron,
                                           stdin=subprocess.PIPE, 
                                           stdout=subprocess.PIPE, 
                                           stderr=subprocess.STDOUT,
                                           cwd=path)
         
        
        time.sleep(.5)
         
        self.thread = Thread(target = self.threaded_function)
        self.thread.start()
        
    
        atexit.register(self.stop)



    def iocCommandCompose(self,appName,libFile,subsFile,DeviceName,DriverName):
        
        TOP_PATH = 'epicsEnvSet("TOP","'+self.iocBoot_path+'/..")'
       
        ENV_EXAMPLE_LIB_PATH= 'epicsEnvSet("EXAMPLE_LIB_PATH", "/opt/codac/lib")'

        EPICS_DB_INCLUDE_PATH = 'epicsEnvSet("EPICS_DB_INCLUDE_PATH",".:$(TOP)/../db:$(EPICS_BASE)/db:$(TOP)/../ndsNisyncApp/Db")'

        STREAM_PROTOCOL_PATH = 'epicsEnvSet("STREAM_PROTOCOL_PATH","$(TOP)/db:$(EPICS_BASE)/db")'

        EPICS_BASE_PATH = 'epicsEnvSet("EPICS_BASE","/opt/codac/epics")'

        iocDb= '${TOP}/'+appName+'App/Db'
        
        iocCmd = TOP_PATH +'\n' \
                + EPICS_BASE_PATH +'\n' \
                + STREAM_PROTOCOL_PATH+'\n' \
                + ENV_EXAMPLE_LIB_PATH+'\n' \
                + EPICS_DB_INCLUDE_PATH+'\n' \
                +'cd .. \n'\
                + 'dbLoadDatabase dbd/'+appName+'.dbd\n'\
                + appName+'_registerRecordDeviceDriver pdbbase\n'\
                + 'ndsLoadDriver $(EXAMPLE_LIB_PATH)/'+libFile+'\n'\
                + 'ndsCreateDevice '+DriverName+' '+DeviceName+'\n'\
                + 'cd "'+iocDb+'"\n'\
                +'dbLoadTemplate('+subsFile+')\n'\
                +'dbLoadTemplate(nisyncDevice6683H-terminal.substitutions)\n'\
                + 'iocInit\n' \
                +'dbl\n'
        return iocCmd
    def iocFromSTfile(self,filename=None):
        if filename == '':
           filename='/st6683H.cmd'
        '''Starts the selected IOC'''
        relative_path_to_iocBoot_App = '../../iocBoot/iocndsNisync/'
        childEnviron = os.environ.copy()
        path=os.path.dirname(os.path.realpath(relative_path_to_iocBoot_App))
        self.iocBoot_path = path
        os.chdir(path)
        iocCommand =path+'/iocndsNisync'+str(filename)
        print(iocCommand)
        os.chmod(iocCommand, 00707)
        '''opening a pipe using subprocess'''
        self.iocProcess = subprocess.Popen(iocCommand, 
                                           env=childEnviron,
                                           stdin=subprocess.PIPE, 
                                           stdout=subprocess.PIPE, 
                                           stderr=subprocess.STDOUT,
                                           cwd=path+'/iocndsNisync')
         
        
        time.sleep(.5)
        atexit.register(self.stop)
        try:
            self.thread = Thread(target = self.threaded_function)
            self.thread.start()
        except OSError, e:
            print(e)
            print(OSError.message)
            self.stop
  

    def stop(self):
        '''Stops the test IOC'''
        
        if self.iocProcess:
            '''self.iocProcess.stdin.close()'''
           
            self.iocProcess.stdin.write('exit\n')
            self.stopping_flag=0
            sleep(1)
            self.iocProcess = None
            '''Stops the thread'''
            self.thread.join()
        


if __name__ == "__main__":
    ioctests.setup()
    print("Running the test IOC in verbose mode for {0} seconds".format(ioctests.iocRunDuration))
    ioctests.verbose = True
    test_iocControl = iocControl()
    test_iocControl.startIOC()
    time.sleep(ioctests.iocRunDuration)
    print("Stopping")
    test_iocControl.stop()
