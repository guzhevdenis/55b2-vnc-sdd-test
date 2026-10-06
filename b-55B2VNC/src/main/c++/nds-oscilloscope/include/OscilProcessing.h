#ifndef NDS_OSCILLOSCOPE
#define NDS_OSCILLOSCOPE

#include <iostream>
#include <mutex>
#include <NiFpga.h>
#include <nds3/nds.h>
#include "./../include/NiFpga_NI5772_Oscilloscope_FPGA_Top.h"
#include <string>
#include <vector>
#include <cstddef>
#include <cstdio>
#include <atomic>

class OscilProcessing
{
    public:
    //Constructor
    OscilProcessing(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t&);
    //Destructor
    ~OscilProcessing();

    NiFpga_Session session = 0;
    NiFpga_Status status = -1;
    NiFpga_Status FIFO_Ch0_status = -1;
    NiFpga_Status FIFO_Ch1_status = -1;

    size_t FIFO_Ch0_DataAcquired = 0;
    size_t FIFO_Ch1_DataAcquired = 0;

    //Allocation method
    static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
    //Deallocation method
    static void deallocateDevice(void* deviceName);
    //getDevice function 
    static OscilProcessing* getInstance(const std::string& deviceName);

    private:
    int depthMultiplier = 391;
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

    /*Data Acquisition node*/
    nds::DataAcquisition<std::vector<std::int32_t>> m_DataAcquisition_Ch0; //Channel 0
    nds::DataAcquisition<std::vector<std::int32_t>> m_DataAcquisition_Ch1; //Channel 1
    
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

    //Methods to control DataAcquisition Node state machine for channel 1.
    void switchOn_DataAcquisition_Ch1();    //called to switch on the DataAcquisition node
    void switchOff_DataAcquisition_Ch1();   //called to switch off the DataAcquisition node
    void start_DataAcquisition_Ch1();       //called to start the DataAcquisition node
    void stop_DataAcquisition_Ch1();        //called to stop the DataAcquisition node
    void recover_DataAcquisition_Ch1();     //called to recover the DataAcquisition node from a failure
    bool allow_DataAcquisition_Change_Ch1(  //called to verify if a state change is allowed
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
    
    //DataAcquisition setters for channel 1
    void PV_DataAcquisition_Gain_Writer_Ch1(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Offset_Writer_Ch1(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Bandwidth_Writer_Ch1(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Resolution_Writer_Ch1(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Impedance_Writer_Ch1(const timespec& timestamp, const double& value);
    void PV_DataAcquisition_Coupling_Writer_Ch1(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SignalRefType_Writer_Ch1(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_Ground_Writer_Ch1(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_DMAEnable_Writer_Ch1(const timespec& timestamp, const std::int32_t& value);
    void PV_DataAcquisition_SamplingRate_Writer_Ch1(const timespec& timestamp, const double& value);

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

    // //DataAcquisition initializersfor channel 1
    void PV_DataAcquisition_Gain_Initializer_Ch1(timespec* timestamp, double* value);
    void PV_DataAcquisition_Offset_Initializer_Ch1(timespec* timestamp, double* value);
    void PV_DataAcquisition_Bandwidth_Initializer_Ch1(timespec* timestamp, double* value);
    void PV_DataAcquisition_Resolution_Initializer_Ch1(timespec* timestamp, double* value);
    void PV_DataAcquisition_Impedance_Initializer_Ch1(timespec* timestamp, double* value);
    void PV_DataAcquisition_Coupling_Initializer_Ch1(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SignalRefType_Initializer_Ch1(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_Ground_Initializer_Ch1(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_DMAEnable_Initializer_Ch1(timespec* timestamp, int32_t* value);
    void PV_DataAcquisition_SamplingRate_Initializer_Ch1(timespec* timestamp, double* value);

    //Function that continuously acquires FIFO data from channel 0. It is launched by start_DataAcquisition() in a separate thread
    void FIFO_Ch0_DataAcquisition_thread_body();
    //Function that continuously acquires FIFO data from channel 1. It is launched by start_DataAcquisition() in a separate thread
    void FIFO_Ch1_DataAcquisition_thread_body();

    //A thread that runs DataAcquisition_thread_body() for channel 0
    nds::Thread m_FIFO_Ch0_DataAcquisition_Thread;
    //A thread that runs DataAcquisition_thread_body() for channel 1
    nds::Thread m_FIFO_Ch1_DataAcquisition_Thread;
    
    //A boolean flag that stop the DataAcquisition loop in FIFO_Ch0_DataAcquisition_thread_body() when true.
    std::atomic<bool> m_bStop_FIFO_Ch0_DataAcquisition;
    //A boolean flag that stop the DataAcquisition loop in FIFO_Ch1_DataAcquisition_thread_body() when true.
    std::atomic<bool> m_bStop_FIFO_Ch1_DataAcquisition;

    //FPGA functions
    //NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type Cluster_Ch0;
    //Bool Indicators
    void Status_Reader(const timespec *timestamp, std::int32_t *pIn);
    void DAQ_On_Reader(const timespec* timestamp, std::int32_t *pIn);
    void IOModuleInitializationDone_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Rec_Ch0_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Rec_Ch1_Reader(const timespec* timestamp, std::int32_t *pIn);

    //Functions for I16 indicators
    void Elements_Ch0_Reader(const timespec* timestamp, std::int32_t *pIn);
    void Elements_Ch1_Reader(const timespec* timestamp, std::int32_t *pIn);
    void SizeElements_Ch0_Reader(const timespec *timestamp, std::int32_t *pIn);
    void SizeElements_Ch1_Reader(const timespec *timestamp, std::int32_t *pIn);

    //Controls for bool
    void Reseter_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void DAQ_En_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void Channel_0_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void FSA_On_Ch0_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void Channel_1_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void FSA_On_Ch1_Writer(const timespec& timestamp, const std::int32_t& pOut);

    //Controls for I16
    void Reference_Ch0_Writer(const timespec& timestamp, const std::int32_t& pOut); //ex-Baseline
    void Reference_Ch1_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void delta_Ch0_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void delta_Ch1_Writer(const timespec& timestamp, const std::int32_t& pOut);
    void Difference_Ch0_Writer(const timespec& timestamp, const std::int32_t& pOut); //ex-noise
    void Difference_Ch1_Writer(const timespec& timestamp, const std::int32_t& pOut);

    size_t ToHostFIFO_Ch0_depth = 0;
    size_t ToHostFIFO_Ch1_depth = 0;

};

#endif