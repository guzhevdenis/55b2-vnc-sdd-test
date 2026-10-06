#ifndef NDS_NI5763
#define NDS_NI5763

#include <iostream>
#include <mutex>
#include <NiFpga.h>
#include <nds3/nds.h>
#include "./../include/NiFpga_Fission_Chamber_DAQ_FPGA_Top.h"
#include <string>
#include <vector>
#include <cstddef>
#include <cstdio>
#include <atomic>
#include <cmath>

class Ni5763Main
{
    public:
    //Constructor
    Ni5763Main(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t&);
    //Destructor
    ~Ni5763Main();

    NiFpga_Session session = 0;
    NiFpga_Status status = -1;
    NiFpga_Status FIFO_Ch0_status = -1;

    size_t RawData_DataAcquired = 0;
    size_t FluctCountRate_Hz_DataAcquired = 0;
    size_t PulseChain_DataAcquired = 0;

    //Allocation method
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    //Deallocation method
    static void deallocateDevice(void* deviceName);
    //getDevice function 
    static Ni5763Main* getInstance(const std::string& deviceName);

    private:
    int ConfigFIFO_Depth = 10230;

    //Device name
    std::string m_name;

    //State Machine
    nds::StateMachine m_Device_stateMachine;

    //Methods to control Device state machine
    void switchOn_Device();     //called to switch on the Device
    void switchOff_Device();    //called to switch off the Device
    void start_Device();        //called to start the Device
    void stop_Device();         //called to stop the Device
    void recover_Device();      //called to recover the Device from a failure
    bool allow_Device_Change(   //called to verify if a state change is allowed
        const nds::state_t, 
        const nds::state_t, 
        const nds::state_t
    );

    // NDS Variables In
    nds::PVBaseIn FluctCountRate_Hz_m;
    nds::PVBaseIn SinglePules_m;
    nds::PVBaseIn SingleRate_m;
    nds::PVBaseIn TotalPulses_m;
    nds::PVBaseIn TotalRate_m;
    nds::PVBaseIn SingleRateData_m;
    nds::PVBaseIn TotalRateData_m;

    /*Data Acquisition node*/
    nds::DataAcquisition<std::vector<std::int32_t>> m_DataAcquisition_Ch0; //Channel 0
    
    //Methods to control DataAcquisition Node state machine for channel 0.
    void switchOn_DataAcquisition_Ch0();    //called to switch on the DataAcquisition node
    void switchOff_DataAcquisition_Ch0();   //called to switch off the DataAcquisition node
    void start_DataAcquisition_Ch0();       //called to start the DataAcquisition node
    void stop_DataAcquisition_Ch0();        //called to stop the DataAcquisition node
    void recover_DataAcquisition_Ch0();     //called to recover the DataAcquisition node from a failure
    bool allow_DataAcquisition_Change_Ch0(  //called to verify if a state change is allowed
        const nds::state_t,
        const nds::state_t,
        const nds::state_t
    );

    //DataAcquisition setters for channel 0
    void PV_DataAcquisition_Gain_Writer_Ch0(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Offset_Writer_Ch0(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Bandwidth_Writer_Ch0(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Resolution_Writer_Ch0(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Impedance_Writer_Ch0(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Coupling_Writer_Ch0(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SignalRefType_Writer_Ch0(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_Ground_Writer_Ch0(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_DMAEnable_Writer_Ch0(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SamplingRate_Writer_Ch0(const timespec& timestamp, const double& value);

    //DataAcquisition initializersfor channel 0
    void PV_DataAcquisition_Gain_Initializer_Ch0(timespec* timestamp, double* value);
    void PV_DataAcquisition_Offset_Initializer_Ch0(timespec* timestamp, double* value);
    void PV_DataAcquisition_Bandwidth_Initializer_Ch0(timespec* timestamp, double* value);
    void PV_DataAcquisition_Resolution_Initializer_Ch0(timespec* timestamp, double* value);
    void PV_DataAcquisition_Impedance_Initializer_Ch0(timespec* timestamp, double* value);
    void PV_DataAcquisition_Coupling_Initializer_Ch0(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SignalRefType_Initializer_Ch0(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_Ground_Initializer_Ch0(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_DMAEnable_Initializer_Ch0(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SamplingRate_Initializer_Ch0(timespec* timestamp, double* value);


    //Function that continuously acquires FIFO data from channel 0. It is launched by start_DataAcquisition() in a separate thread
    void DataAcquisition_Ch0_thread_body();
    void RawDataAcquisition_Ch0_thread_body();

    //A thread that runs DataAcquisition_thread_body() for channel 0
    nds::Thread m_DataAcquisition_Ch0_Thread;
    nds::Thread m_RawDataAcquisition_Ch0_Thread;
    
    //A boolean flag that stop the DataAcquisition loop in FIFO_Ch0_DataAcquisition_thread_body() when true.
    std::atomic<bool> m_bStop_DataAcquisition_Ch0;
    std::atomic<bool> m_bStop_RawDataAcquisition_Ch0;

    //FPGA functions
    //Bool Indicators
    void Status_Reader(const timespec *timestamp, std::int32_t *pIn);
    void Reseter_Writer(const timespec& timestamp, const std::int32_t& pOut);

    void DAQ_On_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Input_Valid_Reader(const timespec* timestamp, std::int32_t *pIn);
    void ReadyForInput_Reader(const timespec* timestamp, std::int32_t *pIn);

    //Controls for bool
    void EnableDAQ_Writer(const timespec& timestamp, const std::int32_t& pOut);

    //Indicator functions
    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type
    void EEPROMPowerEnabled_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Expected_ID_Reader(const timespec* timestamp, std::int64_t *pIn);
    void Inserted_ID_Reader(const timespec* timestamp, std::int64_t *pIn);
    void IOEnabled_Reader(const timespec* timestampm, std::int32_t *pIn);
    void PowerEnabled_Reader(const timespec* timestamp, std::int32_t *pIn);
    void PowerGood_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Present_Reader(const timespec* timestamp, std::int32_t *pIn);
    void InitializationDone_Reader(const timespec* timestamp, std::int32_t *pIn);

    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type
    void Clk100PLL_Unlocked_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Power3_3V_Reader(const timespec* timestamp, double *pIn);
    void Power12V_Reader(const timespec* timespec, double *pIn);
    void Temperature_degC_Reader(const timespec* timestamp, std::int32_t *pIn);

    //Control functions
    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type
    void FirstExpCoeff_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void SecondExpCoeff_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void Type_Writer(const timespec& timoestamp, const std::int32_t& pOut);

    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type
    void CalibrationCoefficient_Writer(const timespec& timestamp, const double& pOut);

    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type
    void Threshold_Writer(const timespec& timestamp, const double& pOut);
    void SinglePulseMax_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void SinglePulseMin_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void TotalPulseMin_Writer(const timespec& timestamp, const std::int32_t& pOut);

    void ConfigFIFO_Depth_Writer(const timespec& timestamp, const std::int32_t& pOut);

    void RawDataFIFO_Depth_Reader(const timespec* timestamp, std::int32_t *pIn);
    void FluctCountRate_Hz_FIFO_Depth_Reader(const timespec* timestamp, std::int32_t *pIn);
    void PulseChainFIFO_Depth_Reader(const timespec* timestamp, std::int32_t *pIn);

    size_t RawData_depth = 0;
    size_t FluctCountRate_Hz_depth = 0;
    size_t PulseChain_depth = 0;

    double FluctCountRate_Hz_Average = 0.0;

    double count_average(std::vector<std::int64_t>& data_buffer, size_t size);
};

#endif