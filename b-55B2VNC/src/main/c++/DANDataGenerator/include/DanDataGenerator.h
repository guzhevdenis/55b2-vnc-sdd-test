#include <nds3/nds.h>
#include <mutex>
class DanDataGenerator
{
    public:
    DanDataGenerator(nds::Factory &factory,
                 const std::string &deviceName,
                const nds::namedParameters_t &parameters);

                /*Data Acquisition node*/

            private:
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


                //Function that continuously acquires data. It is launched by
                //start_DataAcquisition() in a separate thread.
            void DataAcquisition_thread_body();

                //A thread that runs DataAcquisition_thread_body().
            nds::Thread m_DataAcquisition_Thread;

            //A boolean flag that stop the DataAcquisition loop in
            //DataAcquisition_thread_body() when true.
            volatile bool m_bStop_DataAcquisition;
            bool m_acquisitionThreadRunning;

            static timespec wallClockTimestamp();

};