#!/bin/bash
# Setting Device node on and children nodes on
#Device ON
caput TEST-FCT0-HWCF:B-0-STM-setState ON
sleep 1
#Routing terminals
caput TEST-FCT0-HWCF:B-0-Routing-TermSrc 15
caput TEST-FCT0-HWCF:B-0-Routing-TermDst 1
caput -S TEST-FCT0-HWCF:B-0-Routing-TermSet ON
sleep 1

#Configuring a 1Hz clock on Terminal 15
caput -S TEST-FCT0-HWCF:B-0-TR15-FTE-Enable ENABLE
caput TEST-FCT0-HWCF:B-0-TR15-FTE-ModeSet CLK
caput TEST-FCT0-HWCF:B-0-TR15-FTE-PeriodNsecSet 1000000000
caput TEST-FCT0-HWCF:B-0-TR15-FTE-DutyCycleSet 500000000
caput -S TEST-FCT0-HWCF:B-0-TR15-FTE-Set ON

sleep 1
caput TEST-FCT0-HWCF:B-0-STM-setState RUNNING
sleep 1

read -p "Press [Enter] key to stop the application"

caput TEST-FCT0-HWCF:B-0-STM-setState ON
sleep 1

caput TEST-FCT0-HWCF:B-0-STM-setState OFF
sleep 1

