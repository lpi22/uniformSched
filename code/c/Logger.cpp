#include <sstream>
#include <ctime>

#include "../include/Logger.h"
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#include <cstring>
#endif



const int MAXSIZE = 524288; // Maximum size in Bytes 1024 * 512
//const int MAXSIZE = 500; // Maximum size in Bytes 1024 * 512

Logger::Logger
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
(
    char* path,
    bool withMakespan,
    char* sysArgv1
)
{
    if (sysArgv1 == NULL)
        sysArgv1 = (char*)"Wrong_Input";
    this->openLogFile(path, withMakespan, sysArgv1);
    this->writeHeader();
}


void Logger::openLogFile
//-----------------------------------------------------------------------------
// first time opening a log file
//-----------------------------------------------------------------------------
(
    char* path,
    bool withMakespan,
    char* sysArgv1
)
{
    char msString[4] = "";

    // get process id for unique filename
    #ifdef _WIN32
        int pid = GetCurrentProcessId();
        if (withMakespan)
            strcpy_s(msString, MAKESPANEXTENSION); // filename for makespan input
        // filename
        m_fileName = "C:\\Users\\LPI\\Forschung\\2024_UniformSched\\Implementierung\\uniformsched\\code" + SLASH + "output" + sysArgv1 + msString + "_" + std::to_string(pid) + LOGNAMEEXTENSION;

    #else
        int pid = getpid();
        if (withMakespan)
            strcpy(msString, MAKESPANEXTENSION); // filename for makespan input
        // filename
        m_fileName = getenv("WORK") + SLASH + "code" + SLASH + "output" + sysArgv1 + msString + "_" + std::to_string(pid) + LOGNAMEEXTENSION;
    #endif


    m_logFile.open(m_fileName, OUTFILE::out);
    if (!m_logFile.is_open())
    {
        std::cerr << "Error opening logfile: " << m_fileName << std::endl;
    }
} 

Logger::~Logger
//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
(
)
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void Logger::log
//-----------------------------------------------------------------------------
// writing in log file
//-----------------------------------------------------------------------------
(
    const STRING& data,
    char* logType
)
{
    if (m_logFile.is_open())
    {
        // check if file is too big
        if (m_logFile.tellp() > MAXSIZE)
        {
            newLogFile();
        }

        m_logFile << this->dateAndTimeNow("-%Y%m%d:%H%M%S:") << logType << ":\t" << data << std::endl;
    }
}

void Logger::log
//-----------------------------------------------------------------------------
// writing in log file
//-----------------------------------------------------------------------------
(
    const STRING& data,
    double makespan,
    char* logType
)
{
    if (m_logFile.is_open())
    {
        // check if file is too big
        if (m_logFile.tellp() > MAXSIZE)
        {
            newLogFile();
        }

        m_logFile << this->dateAndTimeNow("-%Y%m%d:%H%M%S:") << logType << ":\t" << data << makespan << std::endl;
    }
}

void Logger::log
//-----------------------------------------------------------------------------
// writing config matrix in log file
//-----------------------------------------------------------------------------
(
    const STRING& data,
    STRING& filename,
    MATRIX& cm,
    char* logType
)
{
    if (m_logFile.is_open())
    {
        // check if file is too big
        if (m_logFile.tellp() > MAXSIZE)
        {
            newLogFile();
        }

        m_logFile << "=========================================================================================================" << std::endl;
        m_logFile << this->dateAndTimeNow("-%Y%m%d:%H%M%S:") << logType << ":\t" << data << filename << std::endl;

        if (cm.size() == 4)
        {
            m_logFile << "\t\t\t\t\t\t# jobs: \t\t";
            for (const int& value : cm[0])
                m_logFile << value << " ";
            m_logFile << std::endl;
            m_logFile << "\t\t\t\t\t\tjob sizes: \t\t";
            for (const int& value : cm[1])
                m_logFile << value << " ";
            m_logFile << std::endl;
            m_logFile << "\t\t\t\t\t\t# machines: \t\t";
            for (const int& value : cm[2])
                m_logFile << value << " ";
            m_logFile << std::endl;
            m_logFile << "\t\t\t\t\t\tspeed values: \t\t";
            for (const int& value : cm[3])
                m_logFile << value << " ";
            m_logFile << std::endl;
        }
        else
            this->log("wrong size of input matrix (Logger::log)", LogTypeError);
        m_logFile << "=========================================================================================================" << std::endl;
    }
}

void Logger::log
//-----------------------------------------------------------------------------
// writing elapsted time in log file
//-----------------------------------------------------------------------------
(
    const STRING& data,
    long long elapsedTime,
    const STRING& unit,
    char* logType
)
{
    if (m_logFile.is_open())
    {
        // check if file is too big
        if (m_logFile.tellp() > MAXSIZE)
        {
            newLogFile();
        }

        m_logFile << this->dateAndTimeNow("-%Y%m%d:%H%M%S:") << logType << ":\t" << data + std::to_string(elapsedTime) + unit << std::endl;
    }
}

char* Logger::dateAndTimeNow
//-----------------------------------------------------------------------------
// returns timestamp in the given format
//-----------------------------------------------------------------------------
(
    const char* format
)
{
    static char dateTimeString[32];

#ifdef _WIN32
    time_t currentTime = time(0L);
    struct tm currentDate;
    localtime_s(&currentDate, &currentTime);
    strftime(dateTimeString, sizeof(dateTimeString), format, &currentDate);
#else /* ========================== */
    time_t rawtime;
    struct tm* p_info;
    time(&rawtime);
    p_info = localtime(&rawtime);
    strftime(dateTimeString, sizeof(dateTimeString), format, p_info);
#endif /* ========================== */
    return dateTimeString;
}

void Logger::newLogFile
//-----------------------------------------------------------------------------
// if old log file is too big, create a new one
//-----------------------------------------------------------------------------
(
)
{
    // close old file
    m_logFile << "=========================================================================================================" << std::endl;
    m_logFile << this->dateAndTimeNow("  %Y%m%d") << "\t\tto be continued" << std::endl;
    m_logFile << "=========================================================================================================" << std::endl;
    m_logFile.close();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // get timestamp for new filename
    char* timestamp = this->dateAndTimeNow("%H%M%S");
    std::string newFilename = m_fileName + "." + timestamp;

    // rename old file
    if (std::rename(m_fileName.c_str(), newFilename.c_str()) != 0)
    {
        std::cerr << "Error renaming logfile: " << m_fileName << std::endl;
    }

    // reopen log file
    m_logFile.open(m_fileName, OUTFILE::out);
    if (m_logFile.is_open())
    {
        this->writeHeader(newFilename.c_str());
    }
    else
    {
        std::cerr << "Error opening new logfile: " << m_fileName << std::endl;
    }
}

void Logger::writeHeader
//-----------------------------------------------------------------------------
// write a header to the log file
//-----------------------------------------------------------------------------
(
    const char* oldFileName
)
{
    if (m_logFile.is_open())
    {
        // get current date and time
        m_logFile << "=========================================================================================================" << std::endl;
        m_logFile << this->dateAndTimeNow("  %Y%m%d") << "\t\t\t\tMy mastersthesis calculations" << std::endl;
        if (oldFileName != NULL)
            m_logFile << this->dateAndTimeNow("  %Y%m%d") << "\tContinuation of the logfile: " << oldFileName << std::endl;
        m_logFile << "=========================================================================================================" << std::endl;
    }
}

