#include "./../include/OscilProcessing.h"

//the number of elements to read from host-FIFO
#define SIZE 800000

//general type of mutex for working with threads
static std::map<std::string, OscilProcessing*> m_DeviceMap;
static std::mutex m_lockDeviceMap;

//constructor with all parameters
OscilProcessing::OscilProcessing(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters) : m_name(deviceName) {
    std::lock_guard<std::mutex> lock(m_lockDeviceMap);
    if (m_DeviceMap.find(deviceName) != m_DeviceMap.end()) {
        throw std::logic_error("Device with the same name already allocated. This should not happen");
        m_DeviceMap[deviceName] = this;
    }

    //Creation of a port-node to communicate with control system
    nds::Port rootNode(deviceName);

    //adding a state machine node
    m_Device_stateMachine = rootNode.addChild(nds::StateMachine(
            true, std::bind(&OscilProcessing::switchOn_Device, this),
            std::bind(&OscilProcessing::switchOff_Device, this),
            std::bind(&OscilProcessing::start_Device, this),
            std::bind(&OscilProcessing::stop_Device, this),
            std::bind(&OscilProcessing::recover_Device, this),
            std::bind(&OscilProcessing::allow_Device_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    ));

    /*The parameter used to follow the parent node transitions. None - the node
    won't follow it's parent node transition.*/
    nds::autoEnable_t autoEnable = nds::autoEnable_t::running;

    //FPGA bool indicators
    nds::PVBaseIn DAQ_On_m =
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "DAQ_On",
            std::bind(&OscilProcessing::DAQ_On_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));
    nds::PVBaseIn IOModuleInitializationDone_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "IOModuleInitializationDone",
            std::bind(&OscilProcessing::IOModuleInitializationDone_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Rec_Ch0_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Rec_Ch0",
            std::bind(&OscilProcessing::Rec_Ch0_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Rec_Ch1_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Rec_Ch1",
            std::bind(&OscilProcessing::Rec_Ch1_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //FPGA I16 indicators
    nds::PVBaseIn Elements_Ch0_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Elements_Ch0",
            std::bind(&OscilProcessing::Elements_Ch0_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn Elements_Ch1_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Elements_Ch1",
            std::bind(&OscilProcessing::Elements_Ch1_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn SizeElement_Ch0_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "SizeElements_Ch0",
            std::bind(&OscilProcessing::SizeElements_Ch0_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseIn SizeElement_Ch1_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "SizeElements_Ch1",
            std::bind(&OscilProcessing::SizeElements_Ch1_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //FPGA bool controls
    nds::PVBaseOut DAQ_En_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "DAQ_En",
            std::bind(&OscilProcessing::DAQ_En_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));
    nds::PVBaseOut Channel0_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Channel_0",
            std::bind(&OscilProcessing::Channel_0_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut FSA_On_Ch0_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "FSA_On_Ch0",
            std::bind(&OscilProcessing::FSA_On_Ch0_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut Channel1_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Channel_1",
            std::bind(&OscilProcessing::Channel_1_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));
                
    nds::PVBaseOut FSA_On_Ch1_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "FSA_On_Ch1",
            std::bind(&OscilProcessing::FSA_On_Ch1_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    //FPGA I16 controls
    nds::PVBaseOut Reference_Ch0_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Reference_Ch0",
            std::bind(&OscilProcessing::Reference_Ch0_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut Reference_Ch1_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Reference_Ch1",
            std::bind(&OscilProcessing::Reference_Ch1_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut delta_Ch0_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Delta_Ch0",
            std::bind(&OscilProcessing::delta_Ch0_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));
    
    nds::PVBaseOut delta_Ch1_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Delta_Ch1",
            std::bind(&OscilProcessing::delta_Ch1_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut Difference_Ch0_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Difference_Ch0",
            std::bind(&OscilProcessing::Difference_Ch0_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    nds::PVBaseOut Difference_Ch1_m = 
        rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Difference_Ch1",
            std::bind(&OscilProcessing::Difference_Ch1_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

    // Debug and Not FPGA Variables
    nds::PVBaseIn Status_m = 
        rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
            "Status", std::bind(&OscilProcessing::Status_Reader, this,
                            std::placeholders::_1, std::placeholders::_2)));
    nds::PVBaseOut Reseter_m = rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
            "Reseter", std::bind(&OscilProcessing::Reseter_Writer, this,
                            std::placeholders::_1, std::placeholders::_2)));

    m_DataAcquisition_Ch0 =
        rootNode.addChild(nds::DataAcquisition<std::vector<std::int32_t>>(
            "DataAcquisitionNode_Ch0", SIZE,
            std::bind(&OscilProcessing::switchOn_DataAcquisition_Ch0, this),
            std::bind(&OscilProcessing::switchOff_DataAcquisition_Ch0, this),
            std::bind(&OscilProcessing::start_DataAcquisition_Ch0, this),
            std::bind(&OscilProcessing::stop_DataAcquisition_Ch0, this),
            std::bind(&OscilProcessing::recover_DataAcquisition_Ch0, this),
            std::bind(&OscilProcessing::allow_DataAcquisition_Change_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3),
            std::bind(&OscilProcessing::PV_DataAcquisition_Gain_Writer_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Offset_Writer_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Bandwidth_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Resolution_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Impedance_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Coupling_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_SignalRefType_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Ground_Writer_Ch0, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_DMAEnable_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_SamplingRate_Writer_Ch0,
                      this, std::placeholders::_1, std::placeholders::_2),
            autoEnable));

    m_DataAcquisition_Ch1 =
        rootNode.addChild(nds::DataAcquisition<std::vector<std::int32_t>>(
            "DataAcquisitionNode_Ch1", SIZE,
            std::bind(&OscilProcessing::switchOn_DataAcquisition_Ch1, this),
            std::bind(&OscilProcessing::switchOff_DataAcquisition_Ch1, this),
            std::bind(&OscilProcessing::start_DataAcquisition_Ch1, this),
            std::bind(&OscilProcessing::stop_DataAcquisition_Ch1, this),
            std::bind(&OscilProcessing::recover_DataAcquisition_Ch1, this),
            std::bind(&OscilProcessing::allow_DataAcquisition_Change_Ch1, this,
                      std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3),
            std::bind(&OscilProcessing::PV_DataAcquisition_Gain_Writer_Ch1, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Offset_Writer_Ch1, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Bandwidth_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Resolution_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Impedance_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Coupling_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_SignalRefType_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_Ground_Writer_Ch1, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_DMAEnable_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OscilProcessing::PV_DataAcquisition_SamplingRate_Writer_Ch1,
                      this, std::placeholders::_1, std::placeholders::_2),
            autoEnable));
    
    /*initialization of port*/
    rootNode.initialize(this, factory);
}

/*Destructor. It closes the FPGA session, releases the fifo elements, releases
the name of the device.*/
OscilProcessing::~OscilProcessing() {
    std::lock_guard<std::mutex> lock(m_lockDeviceMap);
    m_DeviceMap.erase(m_name);
}

//find device function
OscilProcessing* OscilProcessing::getInstance(const std::string& deviceName) {
    std::lock_guard<std::mutex> lock(m_lockDeviceMap);

    std::map<std::string, OscilProcessing*>::const_iterator findDevice = m_DeviceMap.find(deviceName);
    if (findDevice == m_DeviceMap.end()) {
        return 0;
    }
    return findDevice->second;
}
//Allocation function
void* OscilProcessing::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters) {
    return new OscilProcessing(factory, deviceName, parameters);
}

//Deallocation function
void OscilProcessing::deallocateDevice(void* deviceName) {
    m_DeviceMap.erase(((OscilProcessing*)deviceName)->m_name);
    delete (OscilProcessing*)deviceName;
}

/*Delegate function to read proccess variables
Functions for Reading variables from FPGA*/
void OscilProcessing::Status_Reader(const timespec* timestamp,
                                    std::int32_t* pIn) {
  *pIn = int32_t(status);
}
void OscilProcessing::Reseter_Writer(const timespec& timestamp,
                                     const std::int32_t& pOut) {
  if (pOut == 1) {
    NiFpga_Reset(session);
    NiFpga_Run(session, 0);
  }
}

//Bool
void OscilProcessing::DAQ_On_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_DAQ_On,
            (NiFpga_Bool*)(pIn)
        );
}
void OscilProcessing::IOModuleInitializationDone_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_IO_ModuleInitialization_Done,
            (NiFpga_Bool*)(pIn)
        );
}

void OscilProcessing::Rec_Ch0_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_Rec_0,
            (NiFpga_Bool*)(pIn)
        );
}

void OscilProcessing::Rec_Ch1_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        status = NiFpga_ReadBool(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_Rec_1,
            (NiFpga_Bool*)(pIn)
        );
}

//I16
void OscilProcessing::Elements_Ch0_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        std::int16_t tmp = 0;
        status = NiFpga_ReadI16(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16_Elements_0, &tmp
        );
        *pIn = tmp;
}

void OscilProcessing::Elements_Ch1_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        std::int16_t tmp = 0;
        status = NiFpga_ReadI16(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16_Elements_1, &tmp
        );
        *pIn = tmp;
}

void OscilProcessing::SizeElements_Ch0_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        std::int16_t tmp = 0;
        status = NiFpga_ReadI16(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16Size_Elements_0, &tmp
        );
        *pIn = tmp;
}

void OscilProcessing::SizeElements_Ch1_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
        std::int16_t tmp = 0;
        status = NiFpga_ReadI16(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16Size_Elements_1, &tmp
        );
        *pIn = tmp;
}

//Functions for control variables FPGA
//Bool
void OscilProcessing::DAQ_En_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        status = NiFpga_WriteBool(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlBool_DAQ_En,
            (NiFpga_Bool)pOut);
}

void OscilProcessing::Channel_0_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {

        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type Cluster_Ch0; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(buffer, &Cluster_Ch0); //unpacking cluster

        Cluster_Ch0.Ch_On = (NiFpga_Bool)pOut; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(buffer, &Cluster_Ch0); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::FSA_On_Ch0_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type Cluster_Ch0; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(buffer, &Cluster_Ch0); //unpacking cluster

        Cluster_Ch0.FSA_On = (NiFpga_Bool)pOut; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(buffer, &Cluster_Ch0); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::Channel_1_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type Cluster_Ch1; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(buffer, &Cluster_Ch1); //unpacking cluster

        Cluster_Ch1.Ch_On = (NiFpga_Bool)pOut; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(buffer, &Cluster_Ch1); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::FSA_On_Ch1_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type Cluster_Ch1; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(buffer, &Cluster_Ch1); //unpacking cluster

        Cluster_Ch1.FSA_On = (NiFpga_Bool)pOut; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(buffer, &Cluster_Ch1); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );
}

//I16
void OscilProcessing::Reference_Ch0_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type Cluster_Ch0; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(buffer, &Cluster_Ch0); //unpacking cluster

        Cluster_Ch0.reference = tmp; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(buffer, &Cluster_Ch0); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::Reference_Ch1_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type Cluster_Ch1; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(buffer, &Cluster_Ch1); //unpacking cluster

        Cluster_Ch1.reference = tmp; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(buffer, &Cluster_Ch1); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::delta_Ch0_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type Cluster_Ch0; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(buffer, &Cluster_Ch0); //unpacking cluster

        Cluster_Ch0.delta = tmp; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(buffer, &Cluster_Ch0); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::delta_Ch1_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type Cluster_Ch1; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(buffer, &Cluster_Ch1); //unpacking cluster

        Cluster_Ch1.delta = tmp; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(buffer, &Cluster_Ch1); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::Difference_Ch0_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type Cluster_Ch0; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(buffer, &Cluster_Ch0); //unpacking cluster

        Cluster_Ch0.difference = tmp; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(buffer, &Cluster_Ch0); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource,
            buffer, sizeof(buffer)
        );
}

void OscilProcessing::Difference_Ch1_Writer(
    const timespec& timestamp, const std::int32_t& pOut) {
        std::int16_t tmp = pOut;
        uint8_t buffer[NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackedSizeInBytes]; //bites buffer
        
        //read current FPGA values
        status = NiFpga_ReadArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type Cluster_Ch1; //creating struct

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(buffer, &Cluster_Ch1); //unpacking cluster

        Cluster_Ch1.difference = tmp; //changing the value

        NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(buffer, &Cluster_Ch1); //packing cluster

        //writing new values to FPGA
        status = NiFpga_WriteArrayU8(
            session, NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource,
            buffer, sizeof(buffer)
        );
}

NDS_DEFINE_DRIVER(OscilProcessing, OscilProcessing)