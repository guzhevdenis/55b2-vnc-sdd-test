#!/bin/bash
# Setting Device node on and children nodes on
#Device ON
caput TEST-TEST-HWCF:SDN-0-StateMachine-setState ON
sleep 1
#Device RUNNING
caput TEST-TEST-HWCF:SDN-0-StateMachine-setState RUNNING

read -p "Press [Enter] key to stop the application"

#Stopping Device
caput TEST-TEST-HWCF:SDN-0-StateMachine-setState ON
sleep 1
#Powering Off Device
caput TEST-TEST-HWCF:SDN-0-StateMachine-setState OFF

