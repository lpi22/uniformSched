#include <iostream>
#include <vector>

#include "../include/ConfigMatrix.h"
#include "../include/Logger.h"

extern Logger* globalLogger;    // for logging



ConfigMatrix::ConfigMatrix
//-----------------------------------------------------------------------------
// constructor;
//-----------------------------------------------------------------------------
(
    MyVector* numJobs,
    MyVector* processingTimes,
    MyVector* numMachines,
    MyVector* speedValues,
    double makespan
)
{   
    m_configData = this->construct(numJobs, processingTimes, numMachines, speedValues, makespan);
    m_numConfigRows = (UINT) m_configData.size();
    m_numConfigCols = (UINT) m_configData[0].size();
    for (UINT i = 0; i < m_numConfigRows; i++)
    {
        for (UINT j = 0; j < m_numConfigCols; j++)
        {
            cout << m_configData[i][j] << " ";
        }
        cout << endl;
    }
    globalLogger->log("Number of columns in the configuration matrix: ", m_numConfigCols, LogTypeInfo);
    //cout << "config marix columns = " << m_numConfigCols << endl;
}

ConfigMatrix::~ConfigMatrix
//-----------------------------------------------------------------------------
// destructor;
//-----------------------------------------------------------------------------
(
)
{
    // nothing to do
}

int ConfigMatrix::get
//-----------------------------------------------------------------------------
// getter for the Element in Matrix[row][col]
//-----------------------------------------------------------------------------
(
    int row,
    int col
)
{
    if (row < 0 || row >= this->getNumRows() || col < 0 || col >= this->getNumCols()) 
    {
        globalLogger->log("index out of bounds (ConfigMatrix::get)", LogTypeError);
        //cout << "index out of bounds (ConfigMatrix::get)" << endl;        
        return 0;
    }
    return m_configData[row][col];
}


void ConfigMatrix::set
//-----------------------------------------------------------------------------
// setter for the Element in Matrix[row][col]
//-----------------------------------------------------------------------------
(
    int row,
    int col,
    int value
)
{
    if (row >= 0 && row < this->getNumRows() && col >= 0 && col < this->getNumCols()) 
    {
        m_configData[row][col] = value;
    }
    else 
    {
        globalLogger->log("index out of bounds (ConfigMatrix::set)", LogTypeError);
        //cout << "index out of bounds (ConfigMatrix::set)" << endl;
        // do nothing
    }
}


int ConfigMatrix::getMaxAbsValue
//-----------------------------------------------------------------------------
// calculates the maximum absolut value of a given matrix
//-----------------------------------------------------------------------------
(
)
{
    int maxAbsVal = 0;
    for (int i = 0; i < this->getNumRows(); i++)
    {
        for (int j = 0; j < this->getNumCols(); j++)
        {
            int val = this->get(i, j);
            if (std::abs(val) > maxAbsVal)
            {
                maxAbsVal = val;
            }
        }
    }
    return maxAbsVal;
}


void ConfigMatrix::generateVectors
//-----------------------------------------------------------------------------
// determine all feasible configurations
//-----------------------------------------------------------------------------
(
    MATRIX& matrix,
    VECTOR& currentVector,
    UINT rowIndex,
    double makespan,
    MyVector* processingTimes,
    UINT speed,
    MATRIX& resultVectors,
    UINT& numConfigs
)
{
    if (rowIndex == matrix.size()) {
        // add vector to result if total load <= makespan
        double load = 0;
        for (UINT i = 0; i < matrix.size(); i++)
        {
            load += currentVector[i] * (processingTimes->get(i) / (double)speed);
        }
        if (load <= makespan) {
            resultVectors.push_back(currentVector);
            numConfigs += 1;
        }
        return;
    }

    // use depth-first search to generate all possible configurations
    for (unsigned int i = 0; i < matrix[rowIndex].size(); i++) {
        currentVector.push_back(matrix[rowIndex][i]);
        generateVectors(matrix, currentVector, rowIndex + 1, makespan, processingTimes, speed, resultVectors, numConfigs);
        currentVector.pop_back();
    }
}


MATRIX ConfigMatrix::transpose
//-----------------------------------------------------------------------------
// tranposes the given matrix
//-----------------------------------------------------------------------------
(
    MATRIX* matrix
)
{
    // define dimensions
    UINT rows = (UINT) matrix->size();
    UINT cols = (UINT) (*matrix)[0].size();

    // iterate over all enties in the given matrix and save them in the correct positions of the result
    MATRIX result(cols, VECTOR(rows));
    for (UINT i = 0; i < rows; i++)
    {
        for (UINT j = 0; j < cols; j++)
        {
            result[j][i] = (*matrix)[i][j];
        }
    }
    return result;
}



MATRIX ConfigMatrix::construct
//-----------------------------------------------------------------------------
// constructs the configuration matrix of an instance and makespan
//-----------------------------------------------------------------------------
(
    MyVector* numJobs,
    MyVector* processingTimes,
    MyVector* numMachines,
    MyVector* speedValues,
    double makespan
)
{
    // define dimensions
    int numJobTypes = numJobs->getSize();
    int numMachineTypes = numMachines->getSize();

    // iterate over all machine types
    MATRIX configList;
    VECTOR num_configs(numMachineTypes);
    for (int j = 0; j < numMachineTypes; j++)
    {
        // for each job type determine the possible values in a configuration
        MATRIX m_table;
        for (int i = 0; i < numJobTypes; i++)
        {
            // get maximum
            int max_val = std::min(numJobs->get(i), (int) std::floor(makespan / (processingTimes->get(i) / (double)speedValues->get(j))));
            VECTOR arr(max_val + 1);
            for (int k = 0; k <= max_val; k++)
            {
                arr[k] = k;
            }
            // array with possible values
            m_table.push_back(arr);
        }
        // determine all feasible configurations
        VECTOR currentVector;
        generateVectors(m_table, currentVector, 0, makespan, processingTimes, speedValues->get(j), configList, num_configs[j]);
    }

    // transpose configurations
    MATRIX result = this->transpose(&configList);

    // add the lower part of the configuration matrix
    UINT totalNumConfigs = (UINT) result[0].size();
    int currentPos = 0;
    for (int j = 0; j < numMachineTypes; j++)
    {
        VECTOR vec(totalNumConfigs);
        result.push_back(vec);

        // for each configuration add a 1 at the row of the corresponding machine type
        for (UINT i = 0; i < num_configs[j]; i++)
        {
            result[result.size() - 1][currentPos] = 1;
            currentPos += 1;
        }
    }

    return result;
}

