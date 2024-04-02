#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/ConfigMatrix.h"
#include "../include/MyVector.h"
#include "../include/DynTable.h"
#include "../include/IPInstance.h"
#include "../include/Defines.h"
#include "../include/Logger.h"

#ifdef _WIN32
#include <direct.h>         // for _getcwd()
#include <windows.h>        // for MAX_PATH
#define GETCWD _getcwd
#else
#include <filesystem>
#include <unistd.h>
#define GETCWD getcwd
#define MAX_PATH 4096
#endif

using namespace std;
using std::cout;

Logger* globalLogger;


MATRIX readFile
(
    std::string inputFileName
)
{
    // Matrix to save input vectors n, p, m, s
    MATRIX inputVectors = { {}, {}, {}, {} };

    INFILE inputFile(inputFileName);

    if (!inputFile.is_open())
    {
        cout << "Error reading input file: " << inputFileName << std::endl;
        return inputVectors;
    }

    // Read machine data from file and save values in matrix
    STRING line;

    int numMachines = 0;
    if (std::getline(inputFile, line))
    {
        INSTRING(line) >> numMachines;
    }

    while (numMachines > 0 && std::getline(inputFile, line))
    {
        INSTRING iss(line);
        STRING token;

        // split line at ","
        if (std::getline(iss, token, ','))
        {
            int machines;
            INSTRING(token) >> machines;
            inputVectors[2].push_back(machines);
            numMachines -= machines;
        }
        if (std::getline(iss, token, ','))
        {
            int speed;
            INSTRING(token) >> speed;
            inputVectors[3].push_back(speed);
        }
    }

    // Read job data from file and save values in matrix
    int numJobs = 0;
    if (std::getline(inputFile, line))
    {
        INSTRING(line) >> numJobs;
    }


    while (numJobs > 0 && std::getline(inputFile, line))
    {
        INSTRING iss(line);
        STRING token;

        // split line at ","
        if (std::getline(iss, token, ','))
        {
            int jobs;
            INSTRING(token) >> jobs;
            inputVectors[0].push_back(jobs);
            numJobs -= jobs;
        }
        if (std::getline(iss, token, ','))
        {
            int proTime;
            INSTRING(token) >> proTime;
            inputVectors[1].push_back(proTime);
        }

    }

    inputFile.close();

    return inputVectors;
}



double binarySearchForOptMakespan
(
    MyVector* numJobs,
    MyVector* jobSizes,
    MyVector* numMachines,
    MyVector* speeds
)
{
    // get dimensions
    UINT numMachineTypes = numMachines->getSize();
    UINT numJobTypes = numJobs->getSize();

    // begin time measurement
    auto totalStart = chrono::steady_clock::now();
    auto stop = chrono::steady_clock::now();
    auto main_ms = chrono::duration_cast<chrono::milliseconds>(stop - totalStart);

    // determine starting search interval

    // determine average completion time
    double sumProcessingTime = 0;
    int pmax = 1;
    for (UINT i = 0; i < numJobTypes; i++)
    {
        sumProcessingTime += numJobs->get(i) * jobSizes->get(i);
        pmax = max(pmax, jobSizes->get(i));
    }
    double sumSpeeds = 0;
    for (UINT j = 0; j < numMachineTypes; j++)
    {
        sumSpeeds += numMachines->get(j) * speeds->get(j);
    }
    double avg = sumProcessingTime / sumSpeeds;



    //determine least common multiple of all speed values
    int lcmSpeeds = speeds->getLCM();

    // upper bound = avg + pmax
    UINT scaledUb = (UINT)lcmSpeeds * (avg + pmax);
    // lower bound = avg
    UINT scaledLb = (UINT)std::floor(lcmSpeeds * avg);


    while (scaledUb != scaledLb)
    {
        // calculate middle of the search interval and the corresponding makespan
        UINT scaledMiddle = (UINT)std::floor((scaledUb + scaledLb) / 2);
        double makespan = scaledMiddle / (double)lcmSpeeds;

        globalLogger->log("The guessed makespan is ", makespan, LogTypeInfo);
        //cout << "guessed makespan = " << makespan << endl;

        // determine configuration matrix A
        ConfigMatrix A(numJobs, jobSizes, numMachines, speeds, makespan);

        // generate IP-instance
        IPInstance inst(&A, numJobs, numMachines);

        auto start = chrono::steady_clock::now();
        // determine feasibility of the IP-instance
        bool feasible = inst.solve();
        auto stop = chrono::steady_clock::now();
        main_ms = main_ms + chrono::duration_cast<chrono::milliseconds>(stop - start);

        // update search interval
        if (feasible)
        {
            scaledUb = scaledMiddle;
        }
        else
        {
            scaledLb = scaledMiddle + 1;
        }
    }

    // end time measurement and save used time
    auto totalStop = chrono::steady_clock::now();
    auto total_ms = chrono::duration_cast<chrono::milliseconds>(totalStop - totalStart);
    auto preprocessing_ms = chrono::duration_cast<chrono::milliseconds>(total_ms - main_ms);

    globalLogger->log("This test took ", total_ms.count(), "ms", LogTypeTime);
    globalLogger->log("The preprocessing of this test took ", preprocessing_ms.count(), "ms", LogTypeTime);
    globalLogger->log("The iterations of this test took ", main_ms.count(), "ms", LogTypeTime);
    //cout << "elapsed time = " << ms.count() << "ms" << endl;
    // makespan = ub = lb
    return scaledUb / (double)lcmSpeeds;
}

int main(int argc, char* argv[])
{
    // determine file path
    char path[MAX_PATH];
    GETCWD(path, MAX_PATH);


    // look for inputdata
    if (argc < 2)
    {
        globalLogger = new Logger(path);
        globalLogger->log("Usage: Programname inputfilename", LogTypeError);
        cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        delete globalLogger;
        return 1;
    }

    // create Logger and LogFile
    globalLogger = new Logger(path, argc == 3, argv[1]);


    // look for inputfile-name
    STRING filename = argv[1];
    STRING inputFileName;
    // get process id for unique filename
    #ifdef _WIN32
        //inputFileName = path + SLASH + "mastersthesis" + SLASH + "code" + SLASH + "instances_uniform" + filename;
    inputFileName = "C:\\Users\\LPI\\Forschung\\2024_UniformSched\\Implementierung\\uniformsched\\code\\instances_uniform" + filename;
    #else
        inputFileName = std::getenv("WORK") + SLASH + "code" + SLASH + "instances_uniform" + filename;
    #endif

    cout << inputFileName << endl;

    MATRIX inputVectors = readFile(inputFileName);
    if (inputVectors[0].size() == 0)
    {
        globalLogger->log("Error: No input data.", LogTypeError);
        //cout << "Error: No input data." << endl;
        return 1;
    }

    double makespan = 0;
    // look for makespan as argument
    if (argc == 3)
    {
        makespan = atoi(argv[2]);
        globalLogger->log("The given makespan is ", makespan, LogTypeInfo);
    }

    MyVector numJobs(inputVectors[0]);
    MyVector processingTimes(inputVectors[1]);
    MyVector numMachines(inputVectors[2]);
    MyVector speedValues(inputVectors[3]);

    globalLogger->log("These are the input values of this test from file ", filename, inputVectors, LogTypeInfo);


    // sort values in m in non-increasing order
    numMachines.mergeSortTwoVec(&speedValues, 0, numMachines.getSize() - 1);



    if (makespan != 0)
    {
        // start time measurement
        auto start = chrono::steady_clock::now();

        // determine configuration matrix A
        ConfigMatrix A(&numJobs, &processingTimes, &numMachines, &speedValues, makespan);


        // generate IP-instance
        IPInstance inst(&A, &numJobs, &numMachines);

        // time after preprocessing
        auto preprocessingTime = chrono::steady_clock::now();

        // determine feasibility of IP-instance
        bool feasible = inst.solve();

        // stop time measurement and save used time
        auto stop = chrono::steady_clock::now();
        auto total_ms = chrono::duration_cast<chrono::milliseconds>(stop - start);
        auto preprocessing_ms = chrono::duration_cast<chrono::milliseconds>(preprocessingTime - start);
        auto main_ms = chrono::duration_cast<chrono::milliseconds>(stop - preprocessingTime);

        // print result
        if (feasible)
        {
            globalLogger->log("This test took ", total_ms.count(), "ms", LogTypeTime);
            globalLogger->log("The preprocessing of this test took ", preprocessing_ms.count(), "ms", LogTypeTime);
            globalLogger->log("The iterations of this test took ", main_ms.count(), "ms", LogTypeTime);
            globalLogger->log("Instance is feasible", LogTypeInfo);
            cout << "Instance is feasible" << endl;
            cout << "Elapsed time: " << total_ms.count() << "ms" << endl;
        }
        else
        {
            globalLogger->log("This test took ", total_ms.count(), "ms", LogTypeTime);
            globalLogger->log("The preprocessing of this test took ", preprocessing_ms.count(), "ms", LogTypeTime);
            globalLogger->log("The iterations of this test took ", main_ms.count(), "ms", LogTypeTime);
            globalLogger->log("Instance is not feasible", LogTypeInfo);
            cout << "Instance is not feasible" << endl;
            cout << "Elapsed time: " << total_ms.count() << "ms" << endl;
        }
    }
    else
    {
        double opt = binarySearchForOptMakespan(&numJobs, &processingTimes, &numMachines, &speedValues);
        globalLogger->log("The makespan is ", opt, LogTypeInfo);
        cout << "Makespan = " << opt << endl;
    }

    delete globalLogger;

    return 0;
}
