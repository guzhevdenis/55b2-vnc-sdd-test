program VNCPSOS

#define STM_NUM 5

short safeCm;					assign	safeCm					to "D1-I2:SAFE-CMD";			monitor safeCm;
short absentCm;					assign	absentCm				to "D1-I2:ABSENT-CMD";			monitor absentCm;
short powerOffCm;				assign	powerOffCm				to "D1-I2:POWEROFF-CMD";		monitor powerOffCm;
short powerOnCm;				assign	powerOnCm				to "D1-I2:POWERON-CMD";			monitor powerOnCm;	
short IC_readyCm;				assign	IC_readyCm				to "D1-I2:INCREADY-CMD";		monitor IC_readyCm;
short configCm;					assign	configCm				to "D1-I2:CONFIGURATION-CMD";	monitor configCm;
short i55B2ReadyCm;				assign	i55B2ReadyCm			to "D1-I2:55B2_READY-CMD";		monitor i55B2ReadyCm;
short localCm;					assign	localCm					to "D1-I2:LOCAL-CMD";			monitor localCm;
short preparCm;					assign	preparCm				to "D1-I2:PREPARING-CMD";		monitor preparCm;
short readyCm;					assign	readyCm					to "D1-I2:READY-CMD";			monitor readyCm;
short measureCm;				assign	measureCm				to "D1-I2:RUNNING-CMD";			monitor measureCm;
short postCheckCm;				assign	postCheckCm				to "D1-I2:POST_CHECK-CMD";		monitor postCheckCm;
short abortCm;					assign	abortCm					to "D1-I2:ABORT-CMD";			monitor abortCm;
short faultCm;					assign	faultCm					to "D1-I2:FAULT-CMD";			monitor faultCm;

short stateId;					assign	stateId					to "D1-I2:STATE";
short psosState;				assign	psosState				to "D1-I2:STATE-PSOS";			monitor psosState;

short safeSt;					assign	safeSt					to "D1-I2:SAFE-STATUS";
short absentSt;					assign	absentSt 				to "D1-I2:ABSENT-STATUS"; 
short powerOffSt;				assign	powerOffSt 				to "D1-I2:POWEROFF-STATUS";
short powerOnSt;				assign	powerOnSt 				to "D1-I2:POWERON-STATUS";	
short IC_readySt;				assign	IC_readySt 				to "D1-I2:INCREADY-STATUS";
short configSt;					assign	configSt 				to "D1-I2:CONFIGURATION-STATUS";
short i55B2ReadySt;				assign	i55B2ReadySt 			to "D1-I2:55B2_READY-STATUS";
short localSt;					assign	localSt 				to "D1-I2:LOCAL-STATUS";
short preparSt;					assign	preparSt 				to "D1-I2:PREPARING-STATUS";
short readySt;					assign	readySt 				to "D1-I2:READY-STATUS"; 
short measureSt;				assign	measureSt 				to "D1-I2:RUNNING-STATUS";
short postCheckSt;				assign	postCheckSt 			to "D1-I2:POST_CHECK-STATUS";
short abortSt;					assign	abortSt 				to "D1-I2:ABORT-STATUS";
short faultSt;					assign	faultSt 				to "D1-I2:FAULT-STATUS";

short devicesSTMs[STM_NUM];		assign devicesSTMs				to {"D1-I2-HWCF:B-7-StateMachine-setState", "D1-I2-HWCF:SDN-0-StateMachine-setState",
																		"D1-I2-HWCF:DAN-1-StateMachine-setState", "D1-I2-HWCF:B-8-StateMachine-setState",
																		"D1-I2-HWCF:DAN-Ni5763-StateMachine-setState"};	monitor devicesSTMs;

short devicesGetState[STM_NUM];	assign devicesGetState			to {"D1-I2-HWCF:B-7-StateMachine-getState", "D1-I2-HWCF:SDN-0-StateMachine-getState",
																	"D1-I2-HWCF:DAN-1-StateMachine-getState", "D1-I2-HWCF:B-8-StateMachine-getState",
																	"D1-I2-HWCF:DAN-Ni5763-StateMachine-getState"};	monitor devicesGetState;


short masterSetState;			assign	masterSetState			to "D1-I2-MasterStateMachine-setState";				monitor masterSetState;
short masterGetState;			assign	masterGetState			to "D1-I2-MasterStateMachine-getState";				monitor	masterGetState;

short PlantSystemPresent;		assign PlantSystemPresent		to "D1-I2-B2A0-OPR-AUT:CD-PS-PP-0000-CDREPNNPVL";	monitor PlantSystemPresent;
short PlantSystemPowerOff;		assign PlantSystemPowerOff		to "D1-I2-B2A0-OPR-AUT:CD-PS-PO-0000-CDREPNNPVL";	monitor PlantSystemPowerOff;
short PlantSystemPowerON;		assign PlantSystemPowerON		to "D1-I2-B2A0-OPR-AUT:CD-PS-AA-0000-CDREPNNPVL";	monitor PlantSystemPowerON;
short Network;					assign Network					to "D1-I2-B2A0-OPR-AUT:CD-CN-CN-0000-CDREPNNPVL";	monitor	Network;
short IC_OK;					assign IC_OK					to "D1-I2-B2A0-OPR-AUT:CD-PS-IC-0000-CDREPNNPVL";	monitor IC_OK;
short LocalRequest;				assign LocalRequest				to "D1-I2-B2A0-OPR-AUT:CD-CO-LR-0000-CDREPNNPVL";	monitor LocalRequest;
short Calibration;				assign Calibration				to "D1-I2-B2A0-OPR-AUT:CD-CO-CA-0000-CDREPNNPVL";	monitor Calibration;
short PlantSystemEquipment;		assign PlantSystemEquipment		to "D1-I2-B2A0-OPR-AUT:CD-PS-ER-0000-CDREPNNPVL";	monitor PlantSystemEquipment;
short PlantSystemConfig;		assign PlantSystemConfig		to "D1-I2-B2A0-OPR-AUT:CD-PS-CD-0000-CDREPNNPVL";	monitor PlantSystemConfig;
short PulseReceive;				assign PulseReceive				to "D1-I2-B2A0-OPR-AUT:CD-CO-CP-0000-CDREPNNPVL";	monitor PulseReceive;
short SystemHealth;				assign SystemHealth				to "D1-I2-B2A0-OPR-AUT:CD-PS-SH-0000-CDREPNNPVL";	monitor SystemHealth;
short SpecificConfig;			assign SpecificConfig			to "D1-I2-B2A0-OPR-AUT:CD-CO-SC-0000-CDREPNNPVL";	monitor SpecificConfig;
short StartMeasurement;			assign StartMeasurement			to "D1-I2-B2A0-OPR-AUT:CD-CO-SM-0000-CDREPNNPVL";	monitor StartMeasurement;
short Termination;				assign Termination				to "D1-I2-B2A0-OPR-AUT:CD-CO-TP-0000-CDREPNNPVL";	monitor Termination;
short AbortRequest;				assign AbortRequest				to "D1-I2-B2A0-OPR-AUT:CD-CO-AE-0000-CDREPNNPVL";	monitor AbortRequest;

short prevState = 1;
short counter = 0;

evflag stmCommand;



ss VNC_state {
	state SYNCHRONIZE {
		entry {
			pvPut(stateId);
			safeCm=safeSt=(stateId==0);					pvPut(safeCm);			pvPut(safeSt);
			powerOffCm=powerOffSt=(stateId==2); 		pvPut(powerOffSt);		pvPut(powerOffCm);
			absentCm=absentSt=(stateId==1); 			pvPut(absentSt);		pvPut(absentCm);
			powerOnCm=powerOnSt=stateId==3; 			pvPut(powerOnSt);		pvPut(powerOnCm);
			IC_readyCm=IC_readySt=stateId==4;			pvPut(IC_readySt);		pvPut(IC_readyCm);
			configCm=configSt=stateId==5; 				pvPut(configSt);		pvPut(configCm);
			localCm=localSt=stateId==6;					pvPut(localSt);			pvPut(localCm);
			i55B2ReadyCm=i55B2ReadySt=stateId==7;		pvPut(i55B2ReadySt);	pvPut(i55B2ReadyCm);
			preparCm=preparSt=stateId==8; 				pvPut(preparSt);		pvPut(preparCm);
			readyCm=readySt=stateId==9; 				pvPut(readySt);			pvPut(readyCm);
			measureCm=measureSt=stateId==10;			pvPut(measureSt);		pvPut(measureCm);
			postCheckCm=postCheckSt=stateId==11; 		pvPut(postCheckSt);		pvPut(postCheckCm);
			faultCm=faultSt=stateId==12;				pvPut(faultSt);			pvPut(faultCm);
			abortCm=abortSt=stateId==13; 				pvPut(abortSt);			pvPut(abortCm);
		}
		when (pvPutComplete(stateId)		&&	pvPutComplete(powerOnCm)	&&
			  pvPutComplete(IC_readyCm)		&&	pvPutComplete(configCm)		&&
			  pvPutComplete(absentCm)		&&	pvPutComplete(abortCm)		&&
			  pvPutComplete(i55B2ReadyCm)	&&	pvPutComplete(preparCm)		&&
			  pvPutComplete(readyCm)		&&	pvPutComplete(measureCm)	&&
			  pvPutComplete(localCm)		&&	pvPutComplete(postCheckCm)	&&
			  pvPutComplete(powerOffCm)		&&	pvPutComplete(faultCm)		&&
			  pvPutComplete(safeCm)) {	} state SELECTSTATE
	}
	state SELECTSTATE {
		when (stateId==1)  { } state ABSENT
		when (stateId==0)  { } state SAFE
		when (stateId==2)  { } state POWEROFF
		when (stateId==3)  { } state POWERON
		when (stateId==4)  { } state IC_READY
		when (stateId==5)  { } state CONFIGURATION
		when (stateId==6)  { } state LOCAL
		when (stateId==7)  { } state I55B2_READY
		when (stateId==8)  { } state PREPARING
		when (stateId==9)  { } state READY
		when (stateId==10) { } state MEASUREMENT
		when (stateId==11) { } state POST_CHECK
		when (stateId==12) { } state FAULT
		when (stateId==13) { } state ABORT
	}
	//------------------------------------------------------------------------------------
	state SAFE {
		when(PlantSystemPresent && PlantSystemPowerOff && powerOffCm) {stateId=2;} state SYNCHRONIZE //go to power off
	}
	//------------------------------------------------------------------------------------
	state POWEROFF {
		when(PlantSystemPowerON && powerOnCm) {stateId=3;} state SYNCHRONIZE //go to poweron
		when(absentCm) {stateId=1;} state SYNCHRONIZE//go to absent
	}
	//------------------------------------------------------------------------------------
	state ABSENT {
		when(powerOffCm) {stateId=2;} state SYNCHRONIZE//go to poweroff
	}
	//------------------------------------------------------------------------------------
	state POWERON {
		when(PlantSystemPresent && PlantSystemPowerOff && powerOffCm) {stateId=2;} state SYNCHRONIZE//go ro poweroff
		when(Network && IC_OK && IC_readyCm) {stateId=4;} state SYNCHRONIZE//go to ic_ready

	}
	//------------------------------------------------------------------------------------
	state IC_READY {
		when(IC_OK && configCm) {stateId=5;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE//go to fault
	}
	//------------------------------------------------------------------------------------
	state CONFIGURATION {
		when(LocalRequest && localCm) {stateId=6;} state SYNCHRONIZE
		when(Calibration && PlantSystemEquipment && PlantSystemConfig && i55B2ReadyCm) {stateId=7;} state SYNCHRONIZE
		when(PlantSystemPresent && PlantSystemPowerOff && powerOffCm) {stateId=2;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE//go to fault
	}
	//------------------------------------------------------------------------------------
	state I55B2_READY {
		when(PulseReceive && SystemHealth && preparCm) {stateId=8;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE//go to fault

	}
	//------------------------------------------------------------------------------------
	state PREPARING {
		when(SpecificConfig && readyCm) {stateId=9;} state SYNCHRONIZE
		when(AbortRequest && abortCm) {stateId=13;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE//go to fault
	}
	//------------------------------------------------------------------------------------
	state READY {
		when(StartMeasurement && measureCm) {stateId=10;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE
		when(AbortRequest && abortCm) {stateId=13;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state MEASUREMENT {
		when(Termination && postCheckCm) {stateId=11;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE
		when(AbortRequest && abortCm) {stateId=13;} state SYNCHRONIZE

	}
	//------------------------------------------------------------------------------------
	state POST_CHECK {
		when(PulseReceive && SystemHealth && preparCm) {stateId=8;} state SYNCHRONIZE
		when(faultCm) {stateId=12;} state SYNCHRONIZE
		when(AbortRequest && abortCm) {stateId=13;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state FAULT {
		when(LocalRequest && localCm){stateId=6;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------    
	state ABORT {
		when(IC_OK && configCm) {stateId=5;} state SYNCHRONIZE
	}
	//------------------------------------------------------------------------------------
	state LOCAL {
		when(IC_OK && configCm) {stateId=5;} state SYNCHRONIZE
		when(Calibration && PlantSystemEquipment && PlantSystemConfig && i55B2ReadyCm) {stateId=7;} state SYNCHRONIZE
	}
	
}

ss PSOS_STATE {
	state SELECT_PSOS_STATE {
		when(psosState == 0) { //go to power off for safe
			psosState = 2;
			pvPut(psosState);
		} state SELECT_PSOS_STATE
		when(psosState == 1 && powerOffCm) { //go to absent
			absentCm = 1;
			pvPut(absentCm);
		} state SELECT_PSOS_STATE
		when(psosState == 2 && (safeCm || powerOnCm || configCm || absentCm)) { //go to power off
			powerOffCm = 1;
			pvPut(powerOffCm);
		} state SELECT_PSOS_STATE
		when(psosState == 3 && powerOffCm) { //go to power on
			PlantSystemPowerON = 1;
			PlantSystemPowerOff = 0;
			powerOnCm = 1;
			pvPut(PlantSystemPowerON);
			pvPut(PlantSystemPowerOff);
			pvPut(powerOnCm);
		} state SELECT_PSOS_STATE
		when(psosState == 4 && powerOnCm) { //go to I&C Ready
			IC_readyCm = 1;
			pvPut(IC_readyCm);
		} state SELECT_PSOS_STATE
		when(psosState == 5 && (powerOffCm || IC_readyCm || localCm || abortCm)) { //go to Configuration
			configCm = 1;
			pvPut(configCm);
		} state SELECT_PSOS_STATE
		when(psosState == 6 && (configCm || faultCm)) { //go to Local
			localCm = 1;
			LocalRequest = 1;
			pvPut(localCm);
			pvPut(LocalRequest);
		} state SELECT_PSOS_STATE
		when(psosState == 7 && (configCm || localCm)) { //go to 55B2 Ready
			i55B2ReadyCm = 1;
			Calibration = 1;
			PlantSystemEquipment = 1;
			PlantSystemConfig = 1;
			pvPut(i55B2ReadyCm);
			pvPut(Calibration);
			pvPut(PlantSystemEquipment);
			pvPut(PlantSystemConfig);
		} state SELECT_PSOS_STATE
		when(psosState == 8 && (i55B2ReadyCm || postCheckCm)) { //go to Prepare for measurement
			preparCm = 1;
			PulseReceive = 1;
			SystemHealth = 1;
			pvPut(preparCm);
			pvPut(PulseReceive);
			pvPut(SystemHealth);
		} state SELECT_PSOS_STATE
		when(psosState == 9 && preparCm) { //go to Ready for measurement
			readyCm = 1;
			SpecificConfig = 1;
			pvPut(readyCm);
			pvPut(SpecificConfig);
		} state SELECT_PSOS_STATE
		when(psosState == 10 && readyCm) { //go to Measurement Running
			measureCm = 1;
			pvPut(measureCm);
		} state SELECT_PSOS_STATE
		when(psosState == 11 && measureCm) { //go to Post Check
			postCheckCm = 1;
			pvPut(postCheckCm);
		} state SELECT_PSOS_STATE
		when(psosState == 13 && (preparCm || readyCm || measureCm || postCheckCm)) { //go to Abort
			abortCm = 1;
			AbortRequest = 1;
			pvPut(abortCm);
			pvPut(AbortRequest);
		} state SELECT_PSOS_STATE
	}
}

//prevState = 1 - SWITCH OFF
//prevState = 2 - SWITCHING OFF
//prevState = 3 - SWITCHING ON
//prevState = 4 - SWITCH ON
//prevState = 5 - STOPPING
//prevState = 6 - STARTING
//prevState = 7 - RUNNING

ss VNC_STATE_SWITCH {
	state STATE_SWITCH {
		when(powerOffCm && prevState == 4) 	{  } state SWITCHING_OFF	//when PSOS switching to Power Off
		when(IC_readyCm && prevState == 1) 	{  } state SWITCHING_ON		//when PSOS switching to I&C Ready 
		when(postCheckCm && prevState == 7) {  } state STOPPING			//when PSOS switching to Post Check
		when(measureCm && prevState == 4) 	{  } state STARTING			//when PSOS switching to Measurement
		when(devicesGetState[0] == 8 || devicesGetState[1] == 8 || 		//when error occurred
			 devicesGetState[2] == 8 || devicesGetState[3] == 8 || 
			 devicesGetState[4] == 8 && !faultCm) {
				faultCm = 1;
			} state STATE_SWITCH
	}
	state SWITCH_OFF {
		when(devicesGetState[0] == 1 && devicesGetState[1] == 1 && 
			 devicesGetState[2] == 1 && devicesGetState[3] == 1 && 
			 devicesGetState[4] == 1 && prevState == 2) { //PSOS will be Switched Off when all NDS drivers have been successfully switched off
				PlantSystemPowerOff = 1;
				PlantSystemPresent = 1;
				pvPut(PlantSystemPowerOff);
				pvPut(PlantSystemPresent);
				prevState = 1;
		} state STATE_SWITCH
	}
	state SWITCHING_OFF {
		when(prevState == 4 && devicesSTMs[0] == 4 && devicesSTMs[1] == 4 && devicesSTMs[2] == 4 && devicesSTMs[3] == 4 && devicesSTMs[4] == 4) {
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 1;
				pvPut(devicesSTMs[counter]); //send Switch Off states to NDS drivers
			}
			prevState = 2;
		} state SWITCH_OFF
	}
	state SWITCHING_ON {
		when(devicesSTMs[0] != 4 && devicesSTMs[1] != 4 && devicesSTMs[2] != 4 && devicesSTMs[3] != 4 && devicesSTMs[4] != 4) {
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 4;
				pvPut(devicesSTMs[counter]); //send Switch On states to NDS drivers
			}
			prevState = 3;
		} state SWITCH_ON
	}
	state SWITCH_ON {
		when(devicesGetState[0] == 4 && devicesGetState[1] == 4 && 
			 devicesGetState[2] == 4 && devicesGetState[3] == 4 && 
			 devicesGetState[4] == 4 && prevState == 3) { //PSOS will be switched to I&C Ready when all NDS drivers have been successfully switched on
				Network = 1;
				IC_OK = 1;
				pvPut(Network);
				pvPut(IC_OK);
				prevState = 4;
		} state STATE_SWITCH
		when(devicesGetState[0] == 4 && devicesGetState[1] == 4 && 
			 devicesGetState[2] == 4 && devicesGetState[3] == 4 && 
			 devicesGetState[4] == 4 && prevState == 5) { //PSOS will be switched to Post Check when all NDS drivers have been successfully switched on
				Termination = 1;
				pvPut(Termination);
				prevState = 4;
		} state STATE_SWITCH
		when(devicesGetState[0] == 8 || devicesGetState[1] == 8 || 
			 devicesGetState[2] == 8 || devicesGetState[3] == 8 || 
			 devicesGetState[4] == 8) {
				faultCm = 1;
		} state STATE_SWITCH
	}
	state STOPPING {
		when(devicesSTMs[0] != 4 && devicesSTMs[1] != 4 && devicesSTMs[2] != 4 && devicesSTMs[3] != 4 && devicesSTMs[4] != 4) {
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 4;
				pvPut(devicesSTMs[counter]);
			}
			prevState = 5;
		} state SWITCH_ON
	}
	state STARTING {
		when(devicesSTMs[0] == 4 && devicesSTMs[1] == 4 && devicesSTMs[2] == 4 && devicesSTMs[3] == 4 && devicesSTMs[4] == 4) {
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 7;
				pvPut(devicesSTMs[counter]); //send Running states to NDS drivers
			}
			prevState = 6;
		} state RUNNING
	}
	state RUNNING {
		when(devicesGetState[0] == 7 && devicesGetState[1] == 7 && 
			 devicesGetState[2] == 7 && devicesGetState[3] == 7 && 
			 devicesGetState[4] == 7 && prevState == 6) { //PSOS will be switched to Measurement when all NDS drivers have been successfully switched to Running
				StartMeasurement = 1;
				pvPut(StartMeasurement);
				prevState = 7;
		} state STATE_SWITCH
	}
}