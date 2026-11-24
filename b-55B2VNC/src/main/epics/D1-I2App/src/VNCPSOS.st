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
	state SAFE {
		entry {
			safeSt = 1;
			stateId = 0;
			pvPut(safeSt);
			pvPut(stateId);
		}
		when(PlantSystemPresent && PlantSystemPowerOff) {
			powerOffSt = 1;
			safeSt = 0;
			psosState = 2;
			stateId = 2;
			pvPut(powerOffSt);
			pvPut(safeSt);
			pvPut(psosState);
			pvPut(stateId);
		} state POWEROFF //go to power off
	}
	//------------------------------------------------------------------------------------
	state POWEROFF {
		when(PlantSystemPowerON && psosState == 3) {
			PlantSystemPowerOff = 0;
			powerOnSt = 1;
			powerOffSt = 0;
			stateId = 3;
			pvPut(PlantSystemPowerOff);
			pvPut(powerOnSt);
			pvPut(powerOffSt);
			pvPut(stateId);
		} state POWERON //go to poweron
		when(psosState == 1 ) {
			absentSt = 1;
			powerOffSt = 0;
			stateId = 1;
			pvPut(absentSt);
			pvPut(powerOffSt);
			pvPut(stateId);
		} state ABSENT //go to absent
	}
	//------------------------------------------------------------------------------------
	state ABSENT {
		when(psosState == 2) {
			powerOffSt = 1;
			absentSt = 0;
			stateId = 2;
			pvPut(powerOffSt);
			pvPut(absentSt);
			pvPut(stateId);
		} state POWEROFF //go to poweroff
	}
	//------------------------------------------------------------------------------------
	state POWERON {
		when(psosState == 2) {
			powerOffSt = 1;
			powerOnSt = 0;
			stateId = 2;
			pvPut(powerOffSt);
			pvPut(powerOnSt);
			pvPut(stateId);
		} state POWEROFF //go ro poweroff
		when(Network && IC_OK && psosState == 4) {
			IC_readySt = 1;
			powerOnSt = 0;
			stateId = 4;
			pvPut(IC_readySt);
			pvPut(powerOnSt);
			pvPut(stateId);
		} state IC_READY //go to ic_ready

	}
	//------------------------------------------------------------------------------------
	state IC_READY {
		when(IC_OK && psosState == 5) {
			configSt = 1;
			IC_readySt = 0;
			stateId = 5;
			pvPut(configSt);
			pvPut(IC_readySt);
			pvPut(stateId);
		} state CONFIGURATION //go to configuration
		when(faultCm) {
			faultSt = 1;
			IC_readySt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(IC_readySt);
			pvPut(stateId);
		} state FAULT //go to fault
	}
	//------------------------------------------------------------------------------------
	state CONFIGURATION {
		when(LocalRequest && psosState == 6) {
			localSt = 1;
			configSt = 0;
			stateId = 6;
			pvPut(localSt);
			pvPut(configSt);
			pvPut(stateId);
		} state LOCAL //go to local
		when(Calibration && PlantSystemEquipment && PlantSystemConfig && psosState == 7) {
			i55B2ReadySt = 1;
			configSt = 0;
			stateId = 7;
			pvPut(i55B2ReadySt);
			pvPut(configSt);
			pvPut(stateId);
		} state I55B2_READY //go to i55b2_ready
		when(PlantSystemPresent && PlantSystemPowerOff && psosState == 2) {
			powerOffSt = 1;
			configSt = 0;
			stateId = 2;
			pvPut(powerOffSt);
			pvPut(configSt);
			pvPut(stateId);
		} state POWEROFF //go to poweroff
		when(faultCm) {
			faultSt = 1;
			configSt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(configSt);
			pvPut(stateId);
		} state FAULT //go to fault
	}
	//------------------------------------------------------------------------------------
	state I55B2_READY {
		when(PulseReceive && SystemHealth && psosState == 8) {
			preparSt = 1;
			i55B2ReadySt = 0;
			stateId = 8;
			pvPut(preparSt);
			pvPut(i55B2ReadySt);
			pvPut(stateId);
		} state PREPARING //go to preparing
		when(faultCm) {
			faultSt = 1;
			i55B2ReadySt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(i55B2ReadySt);
			pvPut(stateId);
		} state FAULT //go to fault

	}
	//------------------------------------------------------------------------------------
	state PREPARING {
		when(SpecificConfig && psosState == 9) {
			readySt = 1;
			preparSt = 0;	
			stateId = 9;
			pvPut(readySt);
			pvPut(preparSt);
			pvPut(stateId);
		} state READY //go to ready
		when(AbortRequest && psosState == 13) {
			abortSt = 1;
			preparSt = 0;
			stateId = 13;
			pvPut(abortSt);
			pvPut(preparSt);
			pvPut(stateId);
		} state ABORT //go to abort
		when(faultCm) {
			faultSt = 1;
			preparSt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(preparSt);
			pvPut(stateId);
		} state FAULT //go to fault
	}
	//------------------------------------------------------------------------------------
	state READY {
		when(StartMeasurement && psosState == 10) {
			measureSt = 1;
			readySt = 0;
			stateId = 10;
			pvPut(measureSt);
			pvPut(readySt);
			pvPut(stateId);
		} state MEASUREMENT //go to measurement
		when(AbortRequest && psosState == 13) {
			abortSt = 1;
			readySt = 0;
			stateId = 13;
			pvPut(abortSt);
			pvPut(readySt);
			pvPut(stateId);
		} state ABORT //go to abort
		when(faultCm) {
			faultSt = 1;
			readySt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(readySt);
			pvPut(stateId);
		} state FAULT //go to fault
	}
	//------------------------------------------------------------------------------------
	state MEASUREMENT {
		when(Termination && psosState == 11) {
			postCheckSt = 1;
			measureSt = 0;
			stateId = 11;
			pvPut(postCheckSt);
			pvPut(measureSt);
			pvPut(stateId);
		} state POST_CHECK //go to post check
		when(AbortRequest && psosState == 13) {
			abortSt = 1;
			measureSt = 0;
			stateId = 13;
			pvPut(abortSt);
			pvPut(measureSt);
			pvPut(stateId);
		} state ABORT //go to abort
		when(faultCm) {
			faultSt = 1;
			measureSt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(measureSt);
			pvPut(stateId);
		} state FAULT //go to fault

	}
	//------------------------------------------------------------------------------------
	state POST_CHECK {
		when(PulseReceive && SystemHealth && psosState == 8) {
			preparSt = 1;
			postCheckSt = 0;
			stateId = 8;
			pvPut(preparSt);
			pvPut(postCheckSt);
			pvPut(stateId);
		} state PREPARING //go to preparing
		when(AbortRequest && psosState == 13) {
			abortSt = 1;
			postCheckSt = 0;
			stateId = 13;
			pvPut(abortSt);
			pvPut(postCheckSt);
			pvPut(stateId);
		} state ABORT //go to abort
		when(faultCm) {
			faultSt = 1;
			postCheckSt = 0;
			stateId = 12;
			pvPut(faultSt);
			pvPut(postCheckSt);
			pvPut(stateId);
		} state FAULT //go to fault
	}
	//------------------------------------------------------------------------------------
	state FAULT {
		entry {
			psosState = 12;
			pvPut(psosState);
		}
		when(LocalRequest && psosState == 6){
			localSt = 1;
			faultSt = 0;	
			stateId = 6;
			pvPut(localSt);
			pvPut(faultSt);
			pvPut(stateId);
		} state LOCAL //go to local
	}
	//------------------------------------------------------------------------------------    
	state ABORT {
		when(IC_OK && psosState == 5) {
			AbortRequest = 0;
			configSt = 1;
			abortSt = 0;
			stateId = 5;
			pvPut(AbortRequest);
			pvPut(configSt);
			pvPut(abortSt);
			pvPut(stateId);
		} state CONFIGURATION //go to configuration
	}
	//------------------------------------------------------------------------------------
	state LOCAL {
		entry {
			faultCm = 0;
			faultSt = 0;
			pvPut(faultCm);
			pvPut(faultSt);
		}
		when(IC_OK && psosState == 5) {
			configSt = 1;
			localSt = 0;
			stateId = 5;
			pvPut(configSt);
			pvPut(localSt);
			pvPut(stateId);
		} state CONFIGURATION //go to configuration
		when(Calibration && PlantSystemEquipment && PlantSystemConfig && psosState == 7) {
			i55B2ReadySt = 1;
			localSt = 0;
			stateId = 7;
			pvPut(i55B2ReadySt);
			pvPut(localSt);
			pvPut(stateId);
		} state I55B2_READY //go to i55b2_ready
	}
	
}

ss VNC_STATE_SWITCH {
	state SWITCH_OFF { //when PSOS switching to Power Off
		when(devicesGetState[0] == 1 && devicesGetState[1] == 1 && 
			 devicesGetState[2] == 1 && devicesGetState[3] == 1 && 
			 devicesGetState[4] == 1 && psosState == 2) { //PSOS will be Switched Off when all NDS drivers have been successfully switched off
				PlantSystemPowerOff = 1;				   
				PlantSystemPresent = 1;
				pvPut(PlantSystemPowerOff);
				pvPut(PlantSystemPresent);
		} state SWITCHING_ON_OR_STOPPING
		when(devicesGetState[0] == 8 || devicesGetState[1] == 8 || 
			 devicesGetState[2] == 8 || devicesGetState[3] == 8 || 
			 devicesGetState[4] == 8) { //when error occurred
				faultCm = 1;
		} state FAULT
	}
	state SWITCHING_ON_OR_STOPPING {
		when(psosState == 4 || psosState == 11) { //when PSOS switching to I&C Ready or to Post Check
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 4; 
				pvPut(devicesSTMs[counter]); //send Switch On states to NDS drivers
			}
		} state SWITCH_ON
	}
	state SWITCH_ON {
		when(devicesGetState[0] == 4 && devicesGetState[1] == 4 && 
			 devicesGetState[2] == 4 && devicesGetState[3] == 4 && 
			 devicesGetState[4] == 4 && psosState == 4) { //PSOS will be switched to I&C Ready when all NDS drivers have been successfully switched on
				Network = 1;
				IC_OK = 1;
				pvPut(Network);
				pvPut(IC_OK);
		} state STARTING_OR_SWITCHING_OFF
		when(devicesGetState[0] == 4 && devicesGetState[1] == 4 && 
			 devicesGetState[2] == 4 && devicesGetState[3] == 4 && 
			 devicesGetState[4] == 4 && psosState == 11) { //PSOS will be switched to Post Check when all NDS drivers have been successfully switched on
				Termination = 1;
				pvPut(Termination);
		} state STARTING_OR_SWITCHING_OFF
		when(devicesGetState[0] == 8 || devicesGetState[1] == 8 || 
			 devicesGetState[2] == 8 || devicesGetState[3] == 8 || 
			 devicesGetState[4] == 8) {
				faultCm = 1;
		} state FAULT
	}
	state STARTING_OR_SWITCHING_OFF { 
		when(psosState == 10) { //when PSOS switching to Measurement
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 7;
				pvPut(devicesSTMs[counter]); //send Running states to NDS drivers
			}
		} state RUNNING
		when(psosState == 2) { //when PSOS switching to Power Off
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 1;
				pvPut(devicesSTMs[counter]); //send Switch Off states to NDS drivers
			}
		} state SWITCH_OFF
	}
	state RUNNING {
		when(devicesGetState[0] == 7 && devicesGetState[1] == 7 && 
			 devicesGetState[2] == 7 && devicesGetState[3] == 7 && 
			 devicesGetState[4] == 7) { //PSOS will be switched to Measurement when all NDS drivers have been successfully switched to Running
				StartMeasurement = 1;
				pvPut(StartMeasurement);
		} state SWITCHING_ON_OR_STOPPING
		when(devicesGetState[0] == 8 || devicesGetState[1] == 8 || 
			 devicesGetState[2] == 8 || devicesGetState[3] == 8 || 
			 devicesGetState[4] == 8) {
				faultCm = 1;
		} state FAULT
	}
	state FAULT {
		when(LocalRequest) { 
			for(counter = 0; counter < STM_NUM; counter++) {
				devicesSTMs[counter] = 4; 
				pvPut(devicesSTMs[counter]); //send Switch On states to NDS drivers
			}
		} state SWITCHING_ON_OR_STOPPING
	}
}