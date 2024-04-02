#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "Defines.h"

class Logger {
private:
    STRING  m_fileName;
    OUTFILE m_logFile;

    void  newLogFile();
    void  openLogFile(char* path, bool withMakespan, char* sysArgv1);
    char* dateAndTimeNow(const char* format);
    void  writeHeader(const char* oldFileName = NULL);


public:
    Logger(char* path, bool withMakespan = false, char* sysArgv1 = NULL);
    ~Logger();
    void log(const STRING& message, char* logType = LogTypeInfo);
    void log(const STRING& message, double makespan, char* logType = LogTypeInfo);
    void log(const STRING& message, STRING& filename, MATRIX& cm, char* logType = LogTypeInfo);
    void log(const STRING& data, long long elapsed, const STRING& unit, char* logType = LogTypeInfo);
};
