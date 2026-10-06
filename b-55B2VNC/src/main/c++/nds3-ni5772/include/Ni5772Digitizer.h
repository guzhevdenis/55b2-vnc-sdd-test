/**
 * @file Ni5772Digitizer.h
 *
 * @author Denis Guzhev
 * @date 22 Oct 2024
 * Labview version program 1.5
 */

#ifndef VNC_H_
#define VNC_H_

#include <iostream>
#include <mutex>
#include <unistd.h>
#include<functional>
#include <fstream>
#include <nds3/nds.h>
#include <vector>
#include <string>
#include <cstddef>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <bits/stdc++.h>
#include "./../include/NiFpga_exp_test_FIC.h"
#include "Ni5772Digitizer.h"
#include "NiFpga.h"
#include "SignalProcessing.h"



        class Ni5772Digitizer

        {
            public: 
            //constructor
            Ni5772Digitizer(nds::Factory& factory,
                   const std::string& deviceName,
                   const nds::namedParameters_t&);
             //destructor
            ~Ni5772Digitizer();
    
            //LabVIEW FPGA session and status parameters 
            NiFpga_Session session = 0; 
            NiFpga_Status status = -1;

            //Allocation method
            static void* allocateDevice(nds::Factory& factory, const
            std::string& deviceName, const nds::namedParameters_t&
            parameters);
            //getDevice function 
            Ni5772Digitizer* getInstance (const std::string& deviceName);
            //Deallocation method
            static void deallocateDevice(void* deviceName);


            private:
            //Device name
            std::string m_name; 

                /*State Machine node*/
            nds::StateMachine m_Device_stateMachine;

            //Methods to control StateMachine Node
            void switchOn_Device(); //Called to switch on the Device
            void switchOff_Device(); //Called to switch off the Device
            void start_Device(); //Called to start the Device
            void stop_Device(); //Called to stop the Device
            void recover_Device(); //Called to recover the Device from a failure
            bool allow_Device_Change(const nds::state_t, const nds::state_t, const nds::state_t); //Called to verify if a state change is allowed
                                  
            // NDS Variables In
            nds::PVBaseIn MaximumAmplitude_m;
            // NDS Variables Out
            nds::PVBaseOut Histogram_Bin_Step_m; 

            /*Data Acquisition node*/
            nds::DataAcquisition<std::vector<std::int16_t>> m_DataAcquisition;
        
            //Methods to control DataAcquisition Node state machine.
            void switchOn_DataAcquisition();  //Called to switch on the DataAcquisition node.
            void switchOff_DataAcquisition(); //Called to switch off the DataAcquisition node.
            void start_DataAcquisition();     //Called to start the DataAcquisition node.
            void stop_DataAcquisition();      //Called to stop the DataAcquisition node.
            void recover_DataAcquisition();   //Called to recover the DataAcquisition node from a failure.
            bool allow_DataAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t); //Called to verify if a state change is allowed

            //DataAcquisition setters
            void PV_DataAcquisition_Gain_Writer(const timespec& timestamp, const double& value);
            void PV_DataAcquisition_Offset_Writer(const timespec& timestamp, const double& value);
            void PV_DataAcquisition_Bandwidth_Writer(const timespec& timestamp, const double& value);
            void PV_DataAcquisition_Resolution_Writer(const timespec& timestamp, const double& value);
            void PV_DataAcquisition_Impedance_Writer(const timespec& timestamp, const double& value);
            void PV_DataAcquisition_Coupling_Writer(const timespec& timestamp, const std::int32_t& value);
            void PV_DataAcquisition_SignalRefType_Writer(const timespec& timestamp, const std::int32_t& value);
            void PV_DataAcquisition_Ground_Writer(const timespec& timestamp, const std::int32_t& value);
            void PV_DataAcquisition_DMAEnable_Writer(const timespec& timestamp, const std::int32_t& value);
            void PV_DataAcquisition_SamplingRate_Writer(const timespec& timestamp, const double& value);

                /**
                 * DataAcquisition initializers
                 */
            void PV_DataAcquisition_Gain_Initializer(timespec* timestamp, double* value);
            void PV_DataAcquisition_Offset_Initializer(timespec* timestamp, double* value);
            void PV_DataAcquisition_Bandwidth_Initializer(timespec* timestamp, double* value);
            void PV_DataAcquisition_Resolution_Initializer(timespec* timestamp, double* value);
            void PV_DataAcquisition_Impedance_Initializer(timespec* timestamp, double* value);
            void PV_DataAcquisition_Coupling_Initializer(timespec* timestamp, int32_t* value);
            void PV_DataAcquisition_SignalRefType_Initializer(timespec* timestamp, int32_t* value);
            void PV_DataAcquisition_Ground_Initializer(timespec* timestamp, int32_t* value);
            void PV_DataAcquisition_DMAEnable_Initializer(timespec* timestamp, int32_t* value);
            void PV_DataAcquisition_SamplingRate_Initializer(timespec* timestamp, double* value);


            //Function that continuously acquires processed FIFO dada. It is launched by
            //start_DataAcquisition() in a separate thread.
            void DataAcquisition_thread_body();
            //A thread that runs DataAcquisition_thread_body().
            nds::Thread m_DataAcquisition_Thread;
            
            //Function that continuously acquires Raw FIFO dada. It is launched by
            //start_DataAcquisition() in a separate thread.
            void RawDataAcquisition_thread_body();

            //A thread that runs RawDataAcquisition thread body
            nds::Thread m_RawDataAcquisition_Thread;

            //A boolean flag that stop the DataAcquisition loop in
            //DataAcquisition_thread_body() when true.
            volatile bool m_bStop_DataAcquisition;


            //A boolean flag that stop the RawDataAcquisition loop in
            //RawDataAcquisition_thread_body() when true.
            volatile bool m_bStop_RawDataAcquisition;

            /*FPGA functions*/

            void Status_Reader(const timespec *timestamp, std::int32_t *pIn);

            //Bool Indicators
            void IndicatorBoolKey_Reader(const timespec *timestamp, std::int32_t *pIn);
            void Clock100PLLUnlocked_Reader(const timespec* timestamp, std::int32_t *pIn);
            void EEPROMPowerEnabled_Reader(const timespec* timestamp, std::int32_t *pIn);
            void IOModuleIOEnabled_Reader(const timespec* timestamp, std::int32_t *pIn);
            void IOModuleInitializationDone_Reader(const timespec* timestamp, std::int32_t *pIn);
            void IOModulePowerEnabled_Reader(const timespec* timestamp, std::int32_t *pIn);
            void IOModulePowerGood_Reader(const timespec* timestamp, std::int32_t *pIn);
            void IOModulePresent_Reader(const timespec* timestamp, std::int32_t *pIn);

             /*Function for I8 indicators*/
            void DeviceTemperature_Reader(const timespec* timestamp, std::int32_t *pIn);
            /*Function for I8 indicators*/
           // void DeviceTemperature_Reader(const timespec* timestamp, std::int32_t *pIn);
            //I16
            void ResultAfterSub_Reader(const timespec* timestamp, std::int32_t *pIn);
            //I32
            void Counter1_Reader(const timespec* timestamp, std::int32_t *pIn);
            void Counter2_Reader(const timespec* timestamp, std::int32_t *pIn);
            void Counter3_Reader(const timespec* timestamp, std::int32_t *pIn);
            void CounterTemp_Reader(const timespec* timestamp, std::int32_t *pIn);
;

            /*Indicators for U64*/
            void SinglePulsesCountRate_Reader(const timespec* timestamp, std::int32_t *pIn);
            void SinglePulsesRegistered_Reader(const timespec* timestamp, std::int32_t *pIn);
            void TotalCountRate_Reader(const timespec* timestamp, std::int32_t *pIn);
            void TotalPulsesRegistered_Reader(const timespec* timestamp, std::int32_t *pIn);

            /*Controls for bool*/
            void ThresholdValid_Writer(const timespec& timestamp, const std::int32_t& pOut);
            void ControlKey_Writer(const timespec& timestamp, const std::int32_t& pOut);
            void Reseter_Writer(const timespec& timestamp, const std::int32_t &pOut);

            
            /*Controls for I16*/
            void Threshold_Writer(const timespec& timestamp, const std::int32_t& pOut);
            void Alpha16BitFirst_Writer(const timespec& timestamp, const std::int32_t& pOut);
            void Alpha16BitSecond_Writer(const timespec& timestamp, const std::int32_t& pOut);

            /*Controls for I64*/
            void CalibrationCoefficient_Writer(const timespec& timestamp, const double& pOut);

            /*Controls for U16*/
            //void ChannelSelect_Writer(const timespec& timestamp, const std::int32_t& pOut);
            void FilterType_Writer(const timespec& timestamp, const std::int32_t& pOut);

            /*Controls for U32*/
            void MaxTicksPerPulse_Writer(const timespec& timestamp, const std::int64_t& pOut);
            void MinTicksPerPulse_Writer(const timespec& timestamp, const std::int64_t& pOut);
            void MinTicksPerPulse2_Writer(const timespec& timestamp, const std::int64_t& pOut);

           //Proxy methods
           void NeutronProfile_Reader(const timespec* timestamp, std::vector<std::int32_t>* pIn); 
           void CountRateAverage_Reader(const timespec* timestamp, double* pIn); 
           void MSV_Reader(const timespec* timestamp, double* pIn);

         //values copied from previous working version

            size_t PulsePeaksFIFO_depth = 0;
            size_t RawDataFIFO_depth = 0;
            size_t CountRateFluctuationFIFO_depth = 0;
            size_t MSVADCFIFO_depth = 0;
            size_t stage_depth =  0;
            size_t Correctedsignal_depth = 0;
            size_t TarToHost_depth = 0;
            size_t MSV_depth = 0;



            std::int64_t CountRateFluctuationAverage = 0;

            double CountRate_Average = 0.0;
            double MSV = 0.0;
            double MaximumAmplitude = 0.0;
            std::int64_t MSVValue = 0;

            };


#endif
