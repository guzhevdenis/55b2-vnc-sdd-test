#include "./../include/Ni5763Main.h"
#include <iostream>

//the number of elements to read from host-FIFO
#define SIZE 800000
#define THRES_COEFF 32767.499778164026501829540582614
#define CALIBR_COEFF 281475268175561.75426646137737108

//general type of mutex for working with threads
static std::map<std::string, Ni5763Main*> m_DeviceMap;
static std::mutex m_lockDeviceMap;

//constructor with all parameters
Ni5763Main::Ni5763Main(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters) : m_name(deviceName) {
    std::lock_guard<std::mutex> lock(m_lockDeviceMap);
    if (m_DeviceMap.find(deviceName) != m_DeviceMap.end()) {
        throw std::logic_error("Device with the same name already allocated. This should not happen");
        m_DeviceMap[deviceName] = this;
    }

    //Creation of a port-node to communicate with control system
    nds::Port rootNode(deviceName);

    //adding a state machine node
    m_Device_stateMachine = rootNode.addChild(nds::StateMachine(
            true, std::bind(&Ni5763Main::switchOn_Device, this),
            std::bind(&Ni5763Main::switchOff_Device, this),
            std::bind(&Ni5763Main::start_Device, this),
            std::bind(&Ni5763Main::stop_Device, this),
            std::bind(&Ni5763Main::recover_Device, this),
            std::bind(&Ni5763Main::allow_Device_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    ));

    /*The parameter used to follow the parent node transitions. None - the node
    won't follow it's parent node transition.*/
    nds::autoEnable_t autoEnable = nds::autoEnable_t::running;

    //FPGA bool indicators
    nds::PVBaseIn DAQ_On_m =
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "DAQ_On",
            std::bind(&Ni5763Main::DAQ_On_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Input_Valid_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Input_Valid",
            std::bind(&Ni5763Main::Input_Valid_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn ReadyForInput_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "ReadyForInput",
            std::bind(&Ni5763Main::ReadyForInput_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //Controls for bool
    nds::PVBaseOut EnableDAQ_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "EnableDAQ",
            std::bind(&Ni5763Main::EnableDAQ_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type
    nds::PVBaseIn EEPROMPowerEnabled_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "EEPROMPowerEnabled",
            std::bind(&Ni5763Main::EEPROMPowerEnabled_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Expected_ID_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int64_t>(
            "Expected_ID",
            std::bind(&Ni5763Main::Expected_ID_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Inserted_ID_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int64_t>(
            "Inserted_ID",
            std::bind(&Ni5763Main::Inserted_ID_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn IOEnabled_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "IOEnabled",
            std::bind(&Ni5763Main::IOEnabled_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn PowerEnabled_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "PowerEnabled",
            std::bind(&Ni5763Main::PowerEnabled_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn PowerGood_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "PowerGood",
            std::bind(&Ni5763Main::PowerGood_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Present_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Present",
            std::bind(&Ni5763Main::Present_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn InitializationDone_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "InitializationDone",
            std::bind(&Ni5763Main::InitializationDone_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type
    nds::PVBaseIn Clk100PLL_Unlocked_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Clk100PLL_Unlocked",
            std::bind(&Ni5763Main::Clk100PLL_Unlocked_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Power3_3V_m = 
        rootNode.addChild(nds::PVDelegateIn<double>(
            "Power3_3V",
            std::bind(&Ni5763Main::Power3_3V_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Power12V_m = 
        rootNode.addChild(nds::PVDelegateIn<double>(
            "Power12V",
            std::bind(&Ni5763Main::Power12V_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Temperature_degC_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Temperature_degC",
            std::bind(&Ni5763Main::Temperature_degC_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn RawDataFIFO_Depth_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "RawDataFIFO_Depth",
            std::bind(&Ni5763Main::RawDataFIFO_Depth_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn FluctCountRate_Hz_FIFO_Depth_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "FluctCountRate_Hz_FIFO_Depth",
            std::bind(&Ni5763Main::FluctCountRate_Hz_FIFO_Depth_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn PulseChainFIFO_Depth_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "PulseChainFIFO_Depth",
            std::bind(&Ni5763Main::PulseChainFIFO_Depth_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //Config FIFO depth
    nds::PVBaseOut ConfigFIFO_Depth_m =
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "ConfigFIFO_Depth",
            std::bind(&Ni5763Main::ConfigFIFO_Depth_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));
    
    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type
    nds::PVBaseOut FirstExpCoeff_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "FirstExpCoeff",
            std::bind(&Ni5763Main::FirstExpCoeff_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut SecondExpCoeff_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "SecondExpCoeff",
            std::bind(&Ni5763Main::SecondExpCoeff_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut Type_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Type",
            std::bind(&Ni5763Main::Type_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));
    
    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type                
    nds::PVBaseOut CalibrationCoefficient_m = 
        rootNode.addChild(nds::PVDelegateOut<double>(
            "CalibrationCoefficient",
            std::bind(&Ni5763Main::CalibrationCoefficient_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type
                    
    nds::PVBaseOut Threshold_m = 
        rootNode.addChild(nds::PVDelegateOut<double>(
            "Threshold",
            std::bind(&Ni5763Main::Threshold_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut SinglePulseMax_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "SinglePulseMax",
            std::bind(&Ni5763Main::SinglePulseMax_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut SinglePulseMin_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "SinglePulseMin",
            std::bind(&Ni5763Main::SinglePulseMin_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut TotalPulseMin_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "TotalPulseMin",
            std::bind(&Ni5763Main::TotalPulseMin_Writer, this,
                    std::placeholders::_1, std::placeholders::_2))); 

    FluctCountRate_Hz_m =
        rootNode.addChild(nds::PVVariableIn<double>(
            "FluctCountRate_Hz"));

    SinglePules_m =
        rootNode.addChild(nds::PVVariableIn<std::int64_t>(
            "SinglePules"));

    SingleRate_m =
        rootNode.addChild(nds::PVVariableIn<double>(
            "SingleRate"));

    TotalPulses_m =
        rootNode.addChild(nds::PVVariableIn<std::int64_t>(
            "TotalPulses"));

    TotalRate_m =
        rootNode.addChild(nds::PVVariableIn<double>(
            "TotalRate"));

    SingleRateData_m =
        rootNode.addChild(nds::PVVariableIn<std::vector<double>>(
            "SingleRateData"));

    TotalRateData_m =
        rootNode.addChild(nds::PVVariableIn<std::vector<double>>(
            "TotalRateData"));

    nds::PVBaseIn Status_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Status", std::bind(&Ni5763Main::Status_Reader, this,
                            std::placeholders::_1, std::placeholders::_2)));
    nds::PVBaseOut Reseter_m = rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Reseter", std::bind(&Ni5763Main::Reseter_Writer, this,
                            std::placeholders::_1, std::placeholders::_2)));

    m_DataAcquisition_Ch0 =
        rootNode.addChild(nds::DataAcquisition<std::vector<std::int32_t>>(
            "DataAcquisitionNode_FissCh", SIZE,
            std::bind(&Ni5763Main::switchOn_DataAcquisition_Ch0, this),
            std::bind(&Ni5763Main::switchOff_DataAcquisition_Ch0, this),
            std::bind(&Ni5763Main::start_DataAcquisition_Ch0, this),
            std::bind(&Ni5763Main::stop_DataAcquisition_Ch0, this),
            std::bind(&Ni5763Main::recover_DataAcquisition_Ch0, this),
            std::bind(&Ni5763Main::allow_DataAcquisition_Change_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3),
            std::bind(&Ni5763Main::PV_DataAcquisition_Gain_Writer_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_Offset_Writer_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_Bandwidth_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_Resolution_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_Impedance_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_Coupling_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_SignalRefType_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_Ground_Writer_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_DMAEnable_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5763Main::PV_DataAcquisition_SamplingRate_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            autoEnable));

    /*initialization of port*/
    rootNode.initialize(this, factory);
}

/*Destructor. It closes the FPGA session, releases the fifo elements, releases
the name of the device.*/
Ni5763Main::~Ni5763Main() {
    std::lock_guard<std::mutex> lock(m_lockDeviceMap);
    m_DeviceMap.erase(m_name);
}

//find device function
Ni5763Main* Ni5763Main::getInstance(const std::string& deviceName) {
    std::lock_guard<std::mutex> lock(m_lockDeviceMap);

    std::map<std::string, Ni5763Main*>::const_iterator findDevice = m_DeviceMap.find(deviceName);
    if (findDevice == m_DeviceMap.end()) {
        return 0;
    }
    return findDevice->second;
}
//Allocation function
void* Ni5763Main::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters) {
    return new Ni5763Main(factory, deviceName, parameters);
}

//Deallocation function
void Ni5763Main::deallocateDevice(void* deviceName) {
    m_DeviceMap.erase(((Ni5763Main*)deviceName)->m_name);
    delete (Ni5763Main*)deviceName;
}

/*Delegate function to read proccess variables
Functions for Reading variables from FPGA*/
//Bool
void Ni5763Main::Status_Reader(const timespec* timestamp,
                                    std::int32_t* pIn) {
  *pIn = int32_t(status);
}
void Ni5763Main::Reseter_Writer(const timespec& timestamp,
                                     const std::int32_t& pOut) {
  if (pOut == 1) {
    NiFpga_Reset(session);
    NiFpga_Run(session, 0);
  }
}

void Ni5763Main::DAQ_On_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool_DAQ_On,
            (NiFpga_Bool*)(pIn)
        );
}

void Ni5763Main::Input_Valid_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool_Input_Valid,
            (NiFpga_Bool*)(pIn)
        );
}

void Ni5763Main::ReadyForInput_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool_ReadyforInput,
            (NiFpga_Bool*)(pIn)
        );
}

//Functions for control variables FPGA
//Bool
void Ni5763Main::EnableDAQ_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        status = NiFpga_WriteBool(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlBool_EnableDAQ,
            (NiFpga_Bool)pOut);
}

//Indicator functions
//Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type
void Ni5763Main::EEPROMPowerEnabled_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)ADC_Status_Cluster.EEPROMPowerEnabled; //taking the value
}

void Ni5763Main::Expected_ID_Reader(
    const timespec* timestamp, std::int64_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int64_t)ADC_Status_Cluster.Expected_ID; //taking the value
}

void Ni5763Main::Inserted_ID_Reader(
    const timespec* timestamp, std::int64_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int64_t)ADC_Status_Cluster.Inserted_ID; //taking the value
}

void Ni5763Main::IOEnabled_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)ADC_Status_Cluster.IOEnabled; //taking the value
}

void Ni5763Main::PowerEnabled_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)ADC_Status_Cluster.PowerEnabled; //taking the value
}

void Ni5763Main::PowerGood_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)ADC_Status_Cluster.PowerGood; //taking the value
}

void Ni5763Main::Present_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)ADC_Status_Cluster.Present; //taking the value
}

void Ni5763Main::InitializationDone_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type ADC_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(buffer, &ADC_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)ADC_Status_Cluster.InitializationDone; //taking the value
}

//Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type
void Ni5763Main::Clk100PLL_Unlocked_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type Dev_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_UnpackCluster(buffer, &Dev_Status_Cluster); //unpacking cluster

        *pIn = (std::int32_t)Dev_Status_Cluster.Clk100PLL_Unlocked; //taking the value
}

void Ni5763Main::Power3_3V_Reader(
    const timespec* timestamp, double *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type Dev_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_UnpackCluster(buffer, &Dev_Status_Cluster); //unpacking cluster

        *pIn = static_cast<double>(Dev_Status_Cluster.Power3_3V) * 0.01; //taking the value and calculating power in W
}

void Ni5763Main::Power12V_Reader(
    const timespec* timestamp, double *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type Dev_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_UnpackCluster(buffer, &Dev_Status_Cluster); //unpacking cluster

        *pIn = static_cast<double>(Dev_Status_Cluster.Power12V) * 0.01;; //taking the value and calculating power in W
}

void Ni5763Main::Temperature_degC_Reader(
    const timespec* timestamp, std::int32_t *pIn) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type Dev_Status_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_UnpackCluster(buffer, &Dev_Status_Cluster); //unpacking cluster
        
        std::int8_t temp = Dev_Status_Cluster.Temperature_degC;
        *pIn = temp; //taking the value
}

//Actual FIFO depth
void Ni5763Main::RawDataFIFO_Depth_Reader(
    const timespec* timestamp, std::int32_t *pIn) {
        *pIn = RawData_depth;
}

void Ni5763Main::FluctCountRate_Hz_FIFO_Depth_Reader(
    const timespec* timestamp, std::int32_t *pIn) {
        *pIn = FluctCountRate_Hz_depth;
}

void Ni5763Main::PulseChainFIFO_Depth_Reader(
    const timespec* timestamp, std::int32_t *pIn) {
        *pIn = PulseChain_depth;
}

//Configure FIFO depth
void Ni5763Main::ConfigFIFO_Depth_Writer(
    const timespec& timestamp, const std::int32_t& pOut) { 
        if(pOut != 0) {
            ConfigFIFO_Depth = pOut;
        }
        else {
            ConfigFIFO_Depth = 10230;
        }
}

//Control functions
//Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type
void Ni5763Main::FirstExpCoeff_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type Filter_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_UnpackCluster(buffer, &Filter_Ch0_Cluster); //unpacking cluster

        Filter_Ch0_Cluster.FirstExpCoeff = tmp; //changing the value

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackCluster(buffer, &Filter_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

void Ni5763Main::SecondExpCoeff_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type Filter_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_UnpackCluster(buffer, &Filter_Ch0_Cluster); //unpacking cluster

        Filter_Ch0_Cluster.SecondExpCoeff = tmp; //changing the value

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackCluster(buffer, &Filter_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

void Ni5763Main::Type_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        if(pOut == 0 || pOut == 1 || pOut == 2 || pOut == 3) {
            std::uint16_t tmp = pOut;
            uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackedSizeInBytes]; //bites buffer
            
            //read current FPGA values
            status = NiFpga_ReadArrayU8(
                session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource,
                buffer, sizeof(buffer)
            );

            NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type Filter_Ch0_Cluster; //creating struct

            NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_UnpackCluster(buffer, &Filter_Ch0_Cluster); //unpacking cluster

            Filter_Ch0_Cluster.Type = tmp; //changing the value

            NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackCluster(buffer, &Filter_Ch0_Cluster); //packing cluster

            //writing new values to FPGA
            status = NiFpga_WriteArrayU8(
                session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource,
                buffer, sizeof(buffer)
            );
        }
}

//Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type

void Ni5763Main::CalibrationCoefficient_Writer(
    const timespec& timestamp, const double& pOut) {

        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type FluctChainCfg_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_UnpackCluster(buffer, &FluctChainCfg_Ch0_Cluster); //unpacking cluster

        //FluctChainCfg_Ch0_Cluster.CalibrationCoefficient = (pOut / 3.55271E-15); //changing the value
        FluctChainCfg_Ch0_Cluster.CalibrationCoefficient = pOut * CALIBR_COEFF;

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_PackCluster(buffer, &FluctChainCfg_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

//Functions for NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type
void Ni5763Main::Threshold_Writer(
    const timespec& timestamp, const double& pOut) {
        //std::int16_t tmp = (pOut / 30.518044E-6);
        std::int16_t tmp = pOut * THRES_COEFF;
        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type PulseChainCfg_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_UnpackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //unpacking cluster

        PulseChainCfg_Ch0_Cluster.Threshold = tmp; //changing the value

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

void Ni5763Main::SinglePulseMax_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::uint16_t tmp = static_cast<std::uint16_t>(std::round(pOut * 0.8));
        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type PulseChainCfg_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_UnpackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //unpacking cluster

        PulseChainCfg_Ch0_Cluster.SinglePulseMax = tmp; //changing the value

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

void Ni5763Main::SinglePulseMin_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::uint16_t tmp = static_cast<std::uint16_t>(std::round(pOut * 0.8));
        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type PulseChainCfg_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_UnpackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //unpacking cluster

        PulseChainCfg_Ch0_Cluster.SinglePulseMin = tmp; //changing the value

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

void Ni5763Main::TotalPulseMin_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::uint16_t tmp = static_cast<std::uint16_t>(std::round(pOut * 0.8));
        uint8_t buffer[NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type PulseChainCfg_Ch0_Cluster; //creating struct

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_UnpackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //unpacking cluster

        PulseChainCfg_Ch0_Cluster.TotalPulseMin = tmp; //changing the value

        NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackCluster(buffer, &PulseChainCfg_Ch0_Cluster); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource,
            buffer, sizeof(buffer)
        );
}

NDS_DEFINE_DRIVER(Ni5763, Ni5763Main)