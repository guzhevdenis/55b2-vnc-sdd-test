program VNCPSOS

short safeCm;		assign		safeCm to "D1-I2:SAFE-CMD";		monitor safeCm;
short absentCm;		assign 	     	absentCm to "D1-I2:ABSENT-CMD"; 	monitor absentCm;
short powerOffCm;	assign	        powerOffCm to "D1-I2:POWEROFF-CMD";	monitor powerOffCm;
short powerOnCm;	assign	        powerOnCm to "D1-I2:POWERON-CMD";	monitor powerOnCm;	
short IC_readyCm;	assign	     	IC_readyCm to "D1-I2:INCREADY-CMD";	monitor IC_readyCm;
short configCm;		assign 	       	configCm to "D1-I2:CONFIGURATION-CMD";	monitor configCm;
short i55B2ReadyCm;	assign 	   	i55B2ReadyCm to "D1-I2:55B2_READY-CMD";	monitor i55B2ReadyCm;
short localCm;		assign		localCm to "D1-I2:LOCAL-CMD";		monitor localCm;
short preparCm;		assign 	  	preparCm to "D1-I2:PREPARING-CMD";	monitor preparCm;
short readyCm;		assign 	        readyCm to "D1-I2:READY-CMD"; 		monitor readyCm;
short measureCm;	assign          measureCm to "D1-I2:RUNNING-CMD";	monitor measureCm;
short postCheckCm;	assign        	postCheckCm to "D1-I2:POST_CHECK-CMD";	monitor postCheckCm;
short abortCm;		assign 	        abortCm to "D1-I2:ABORT-CMD";		monitor abortCm;
short faultCm;		assign		faultCm to "D1-I2:FAULT-CMD";		monitor faultCm;

short stateId;		assign 	        stateId to "D1-I2:STATE";

short safeSt;		assign		safeSt to "D1-I2:SAFE-STATUS";
short absentSt;		assign 	     	absentSt to "D1-I2:ABSENT-STATUS"; 
short powerOffSt;	assign	        powerOffSt to "D1-I2:POWEROFF-STATUS";
short powerOnSt;	assign	        powerOnSt to "D1-I2:POWERON-STATUS";	
short IC_readySt;	assign	     	IC_readySt to "D1-I2:INCREADY-STATUS";
short configSt;		assign 	       	configSt to "D1-I2:CONFIGURATION-STATUS";
short i55B2ReadySt;	assign 	   	i55B2ReadySt to "D1-I2:55B2_READY-STATUS";
short localSt;		assign		localSt to "D1-I2:LOCAL-STATUS";
short preparSt;		assign 	  	preparSt to "D1-I2:PREPARING-STATUS";
short readySt;		assign 	        readySt to "D1-I2:READY-STATUS"; 
short measureSt;	assign          measureSt to "D1-I2:RUNNING-STATUS";
short postCheckSt;	assign        	postCheckSt to "D1-I2:POST_CHECK-STATUS";
short abortSt;		assign 	        abortSt to "D1-I2:ABORT-STATUS";
short faultSt;		assign		faultSt to "D1-I2:FAULT-STATUS";



ss VNC_state {
	state SYNCHRONIZE {
		entry {
			pvPut(stateId);
			safeCm=safeSt=(stateId == 0);							pvPut(safeCm); pvPut(safeSt);
			powerOffCm=powerOffSt=(stateId==2); 						pvPut(powerOffSt); pvPut(powerOffCm);
			absentCm=absentSt=(stateId==1); 						pvPut(absentSt); pvPut(absentCm);
			powerOnCm=powerOnSt=stateId==3; 						pvPut(powerOnSt); pvPut(powerOnCm);
			IC_readyCm=IC_readySt=stateId==4;						pvPut(IC_readySt); pvPut(IC_readyCm);
			configCm=configSt=stateId==5; 							pvPut(configSt); pvPut(configCm);
			localCm=localSt=stateId==6;							pvPut(localSt); pvPut(localCm);
			i55B2ReadyCm=i55B2ReadySt=stateId==7;						pvPut(i55B2ReadySt); pvPut(i55B2ReadyCm);
			preparCm=preparSt=stateId==8; 							pvPut(preparSt); pvPut(preparCm);
			readyCm=readySt=stateId==9; 							pvPut(readySt); pvPut(readyCm);
			measureCm=measureSt=stateId==10;						pvPut(measureSt); pvPut(measureCm);
			postCheckCm=postCheckSt=stateId==11; 						pvPut(postCheckSt); pvPut(postCheckCm);
			faultCm=faultSt=stateId==12;							pvPut(faultSt); pvPut(faultCm);
			abortCm=abortSt=stateId==13; 							pvPut(abortSt); pvPut(abortCm);
		}
		when (pvPutComplete(stateId)&&pvPutComplete(powerOnCm)&&
			  pvPutComplete(IC_readyCm)&&pvPutComplete(configCm)&&
			  pvPutComplete(absentCm)&&pvPutComplete(abortCm)&&
			  pvPutComplete(i55B2ReadyCm)&&pvPutComplete(preparCm)&&
			  pvPutComplete(readyCm)&&pvPutComplete(measureCm)&&
			  pvPutComplete(localCm)&&pvPutComplete(postCheckCm)&&
			  pvPutComplete(powerOffCm)&&pvPutComplete(faultCm)&&
                        pvPutComplete(safeCm)){
		} state SELECTSTATE
	}
	state SELECTSTATE {
		when (stateId==1)  {		} state ABSENT
		when (stateId==0)  {		} state SAFE
		when (stateId==2)  {		} state POWEROFF
		when (stateId==3)  {		} state POWERON
		when (stateId==4)  {		} state IC_READY
		when (stateId==5)  {		} state CONFIGURATION
		when (stateId==6)  {		} state LOCAL
		when (stateId==7)  {		} state I55B2_READY
		when (stateId==8)  {		} state PREPARING
		when (stateId==9)  {		} state READY
		when (stateId==10) {		} state MEASUREMENT
		when (stateId==11) {		} state POST_CHECK
		when (stateId==12) {		} state FAULT
		when (stateId==13) {		} state ABORT
	}
	//------------------------------------------------------------------------------------
	state SAFE {
		when (powerOffCm){stateId=2;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state POWEROFF {
		when (powerOnCm) {stateId=3;} state SYNCHRONIZE //go to poweron
		when (absentCm) {stateId=1;} state SYNCHRONIZE//go to absent
	}
	//------------------------------------------------------------------------------------
	state ABSENT {
		when (powerOffCm) {stateId=2;} state SYNCHRONIZE//go to poweroff

	}
	//------------------------------------------------------------------------------------
	state POWERON {
		when (powerOffCm) {stateId=2;} state SYNCHRONIZE//go ro poweroff
		when (IC_readyCm) {stateId=4;} state SYNCHRONIZE//go to ic_ready

	}
	//------------------------------------------------------------------------------------
	state IC_READY {
		when (configCm) {stateId=5;} state SYNCHRONIZE
		when (faultCm) {stateId=12;} state SYNCHRONIZE//go to fault
	}
	//------------------------------------------------------------------------------------
	state CONFIGURATION {
		when (localCm) {stateId=6;} state SYNCHRONIZE
		when (i55B2ReadyCm) {stateId=7;} state SYNCHRONIZE
		when (powerOffCm) {stateId=2;} state SYNCHRONIZE
		when (faultCm) {stateId=12;} state SYNCHRONIZE//go to fault
	}
	//------------------------------------------------------------------------------------
	state I55B2_READY {
		when (preparCm) {stateId=8;} state SYNCHRONIZE
		when (faultCm) {stateId=12;} state SYNCHRONIZE//go to fault

	}
	//------------------------------------------------------------------------------------
	state PREPARING {
		when (readyCm) {stateId=9;} state SYNCHRONIZE
		when (abortCm) {stateId=13;} state SYNCHRONIZE
		when (faultCm) {stateId=12;} state SYNCHRONIZE//go to fault
	}
	//------------------------------------------------------------------------------------
	state READY {
		when (measureCm) {stateId=10;} state SYNCHRONIZE
		when (faultCm) {stateId=12;} state SYNCHRONIZE
		when (abortCm) {stateId=13;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state MEASUREMENT {
		when (postCheckCm) {stateId=11;} state SYNCHRONIZE
		when (faultCm) {stateId=12;} state SYNCHRONIZE

	}
	//------------------------------------------------------------------------------------
	state POST_CHECK {
		when (faultCm) {stateId=12;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state FAULT {
		when (localCm){stateId=6;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state ABORT {
		when (configCm) {stateId=5;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state LOCAL {
		when (configCm) {stateId=5;} state SYNCHRONIZE
		when (i55B2ReadyCm) {stateId=7;} state SYNCHRONIZE
	}
	
	
	
}

