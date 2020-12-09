// -*- C++ -*-
/*!
 * @file
 * @brief
 * @date
 * @author
 *
 */

#include "LatencySender.h"

using DAQMW::FatalType::DATAPATH_DISCONNECTED;
using DAQMW::FatalType::OUTPORT_ERROR;
using DAQMW::FatalType::USER_DEFINED_ERROR1;
using DAQMW::FatalType::USER_DEFINED_ERROR2;

// Module specification
// Change following items to suit your component's spec.
static const char* latencysender_spec[] =
{
    "implementation_id", "LatencySender",
    "type_name",         "LatencySender",
    "description",       "LatencySender component",
    "version",           "1.0",
    "vendor",            "Kazuo Nakayoshi, KEK",
    "category",          "example",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
};

LatencySender::LatencySender(RTC::Manager* manager)
    : DAQMW::DaqComponentBase(manager),
      m_OutPort("latencysender_out", m_out_data),
      // m_sock(0),
      m_bufsize_kb(0),
      m_bufsize_b(0),
      m_out_status(BUF_SUCCESS),

      m_debug(false)
{
    // Registration: InPort/OutPort/Service

    // Set OutPort buffers
    registerOutPort("latencysender_out", m_OutPort);

    init_command_port();
    init_state_table();
    set_comp_name("LATENCYSENDER");
}

LatencySender::~LatencySender()
{
}

RTC::ReturnCode_t LatencySender::onInitialize()
{
    if (m_debug) {
        std::cerr << "LatencySender::onInitialize()" << std::endl;
    }

    return RTC::RTC_OK;
}

RTC::ReturnCode_t LatencySender::onExecute(RTC::UniqueId ec_id)
{
    daq_do();

    return RTC::RTC_OK;
}

int LatencySender::daq_dummy()
{
    return 0;
}

int LatencySender::daq_configure()
{
    std::cerr << "*** LatencySender::configure" << std::endl;

    ::NVList* paramList;
    paramList = m_daq_service0.getCompParams();
    parse_params(paramList);

    m_data = new unsigned char [m_bufsize_b];
    memset(m_data, 'X', m_bufsize_b);

    return 0;
}

int LatencySender::parse_params(::NVList* list)
{
    //bool srcAddrSpecified = false;
    //bool srcPortSpecified = false;
    bool bufsize_kbSpecified = false;

    std::cerr << "param list length:" << (*list).length() << std::endl;

    int len = (*list).length();
    for (int i = 0; i < len; i+=2) {
        std::string sname  = (std::string)(*list)[i].value;
        std::string svalue = (std::string)(*list)[i+1].value;

        std::cerr << "sname: " << sname << "  ";
        std::cerr << "value: " << svalue << std::endl;

        if ( sname == "bufsize_kb" ) {
            bufsize_kbSpecified = true;
            if (m_debug) {
                std::cerr << "source port: " << svalue << std::endl;
            }
            char* offset;
            m_bufsize_kb = (int)strtol(svalue.c_str(), &offset, 10);
            m_bufsize_b  = 1024*m_bufsize_kb;
        }

    }
    if (!bufsize_kbSpecified) {
        std::cerr << "### ERROR:bufsize_kb not specified\n";
        fatal_error_report(USER_DEFINED_ERROR2, "NO BUFSIZE_KB");
    }

    return 0;
}

int LatencySender::daq_unconfigure()
{
    std::cerr << "*** LatencySender::unconfigure" << std::endl;

    delete [] m_data;
    return 0;
}

int LatencySender::daq_start()
{
    std::cerr << "*** LatencySender::start" << std::endl;

    m_out_status = BUF_SUCCESS;

    // Check data port connections
    bool outport_conn = check_dataPort_connections( m_OutPort );
    if (!outport_conn) {
        std::cerr << "### NO Connection" << std::endl;
        fatal_error_report(DATAPATH_DISCONNECTED);
    }

    return 0;
}

int LatencySender::daq_stop()
{
    std::cerr << "*** LatencySender::stop" << std::endl;

    return 0;
}

int LatencySender::daq_pause()
{
    std::cerr << "*** LatencySender::pause" << std::endl;

    return 0;
}

int LatencySender::daq_resume()
{
    std::cerr << "*** LatencySender::resume" << std::endl;

    return 0;
}

int LatencySender::set_data(unsigned int data_byte_size)
{
    unsigned char header[8];
    unsigned char footer[8];

    set_header(&header[0], data_byte_size);
    set_footer(&footer[0]);

    ///set OutPort buffer length
    m_out_data.data.length(data_byte_size + HEADER_BYTE_SIZE + FOOTER_BYTE_SIZE);
    memcpy(&(m_out_data.data[0]), &header[0], HEADER_BYTE_SIZE);
    memcpy(&(m_out_data.data[HEADER_BYTE_SIZE]), &m_data[0], data_byte_size);
    memcpy(&(m_out_data.data[HEADER_BYTE_SIZE + data_byte_size]), &footer[0],
           FOOTER_BYTE_SIZE);

    return 0;
}

int LatencySender::write_OutPort()
{
    ////////////////// send data from OutPort  //////////////////
    bool ret = m_OutPort.write();

    //////////////////// check write status /////////////////////
    if (ret == false) {  // TIMEOUT or FATAL
        m_out_status  = check_outPort_status(m_OutPort);
        if (m_out_status == BUF_FATAL) {   // Fatal error
            fatal_error_report(OUTPORT_ERROR);
        }
        if (m_out_status == BUF_TIMEOUT) { // Timeout
            return -1;
        }
    }
    else {
        m_out_status = BUF_SUCCESS; // successfully done
    }

    return 0;
}

int LatencySender::daq_run()
{
    if (m_debug) {
        std::cerr << "*** LatencySender::run" << std::endl;
    }

    if (check_trans_lock()) {  // check if stop command has come
        set_trans_unlock();    // transit to CONFIGURED state
        return 0;
    }

    if (m_out_status == BUF_SUCCESS) {   // previous OutPort.write() successfully done
        unsigned long *long_p;
        long_p = (unsigned long *)&m_data[0];
        *long_p = m_bufsize_b;
        clock_gettime(CLOCK_MONOTONIC, (struct timespec *)&m_data[sizeof(unsigned long)]);

        set_data(m_bufsize_b); // set data to OutPort Buffer
    }

    if (write_OutPort() < 0) {
        ;     // Timeout. do nothing.
    }
    else {    // OutPort write successfully done
        inc_sequence_num();                     // increase sequence num.
        inc_total_data_size(m_bufsize_b);  // increase total data byte size
    }

    return 0;
}

extern "C"
{
    void LatencySenderInit(RTC::Manager* manager)
    {
        RTC::Properties profile(latencysender_spec);
        manager->registerFactory(profile,
                    RTC::Create<LatencySender>,
                    RTC::Delete<LatencySender>);
    }
};
