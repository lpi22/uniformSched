#include <iostream>
#include <tuple>
#include <vector>
//#include "ilcplex\cplex.h"
//#include "ilcplex\ilocplex.h"

#include "../include/DynTable.h"
#include "../include/Logger.h"

extern Logger* globalLogger;    // for logging




DynTable::DynTable
//-----------------------------------------------------------------------------
// constructor;
//-----------------------------------------------------------------------------
(
    ConfigMatrix* cm,
    MyVector& numMachines,
    int jobBound,
    MyVector* numJobs
)
{
    // split the dynamic table into two parts:
    // the base table consists of jobBound^numJobType columns and numMachineType rows; 
    //      for each possible job vector and machine type, the feasibility of the subproblem with identical machines is stored
    // the inductive table consists of jobBound^numJobType columns and numMachineType rows;
    //      for each possible job vetor and machine type j we store the feasibility of the subproblem with machines of types 1..j

    tie(m_baseTable, m_inductiveTable) = dynProg(cm, numMachines, jobBound, numJobs);
}

DynTable::~DynTable
//-----------------------------------------------------------------------------
// destructor;
//-----------------------------------------------------------------------------
(
)
{
    // nothing to do
}

MYVEC_MATRIX DynTable::getFeasibleJobs
//-----------------------------------------------------------------------------
// Determine set of feasible job vectors N
//-----------------------------------------------------------------------------
(
    int numMachineTypes,
    int jobBound,
    int numJobTypes
)
{
    // define dimensions
    int dim = (int)pow(jobBound, numJobTypes);
    int tableHeight = m_inductiveTable.size();
    // store feasible job vectors in result matrix
    MYVEC_MATRIX result;

    // if there is one machine type, check the base table for each job vector
    if (numMachineTypes == 1)
    {
        for (int i = 0; i < dim; i++)
        {
            if (m_baseTable[0][i])
            {
                // add feasible job vector to result matrix
                MyVector vec = getVector(i, numJobTypes, jobBound);
                result.push_back(vec);
            }
        }
    }
    else // if there is more than one machine type, check the inductive table for each job vector
    {
        for (int i = 0; i < dim; i++)
        {
            if (m_inductiveTable[numMachineTypes - 2][i])
            {
                // add feasible job vector to result matrix
                MyVector vec = getVector(i, numJobTypes, jobBound);
                result.push_back(vec);
            }
        }
    }
    return result;
}


int DynTable::getIndex
//-----------------------------------------------------------------------------
// Compute index of the flattened vector n
//-----------------------------------------------------------------------------
(
    MyVector& vec,
    int jobBound
)
{
    // define dimensions
    int numJobTypes = vec.getSize();

    // calculate index
    int index = 0;
    for (int i = 0; i < numJobTypes; i++)
    {
        int val = vec.get(i);
        // check if dimension i of the vector lies in the box
        if (val >= 0 && val <= jobBound)
        {
            index += val * (int)pow(jobBound, numJobTypes - i - 1);
        }
        else
        {
            return -1;
        }
    }
    return index;
}

MyVector DynTable::getVector
//-----------------------------------------------------------------------------
// Compute vector n from a given index
//-----------------------------------------------------------------------------
(
    int index,
    int numJobTypes,
    int jobBound
)
{
    MyVector result(numJobTypes);
    int range, val;
    int remainder = index;
    for (int j = 0; j < numJobTypes; j++)
    {
        // determine next value in vector and remainder of the index
        range = (int)pow(jobBound, numJobTypes - j - 1);
        val = (int)floor(remainder / range);
        result.set(j, val);
        remainder = remainder % range;
    }
    return result;
}


bool DynTable::isSolutionValid
//-----------------------------------------------------------------------------
// Check if a combination of x values satisfies the constraints
//-----------------------------------------------------------------------------
(
    MATRIX& A,
    MyVector& b,
    VECTOR& machinesToConfigs
)
{
    // define dimensions
    UINT numVariables = (UINT)A[0].size();
    UINT numMachines = (UINT)machinesToConfigs.size();
    // determine solution vector x
    VECTOR x(numVariables, 0);
    for (UINT i = 0; i < numMachines; i++)
    {
        int index = machinesToConfigs[i];
        x[index]++;
    }

    // Check the Ax = b constraint
    UINT numJobTypes = (UINT) A.size();
    for (UINT i = 0; i < numJobTypes; i++) 
    {
        int constraintValue = 0;
        for (UINT j = 0; j < numVariables; j++)
        {
            constraintValue += A[i][j] * x[j];
        }
        if (constraintValue != b.get(i))
        {
            return false;
        }
    }
    return true;
}


bool DynTable::solvePC_max
//-----------------------------------------------------------------------------
// Solve the P||C_max instance via brute force
//-----------------------------------------------------------------------------
(
    MATRIX& cm,
    UINT numIdenticalMachines,
    MyVector& numJobs
)
{
    // define dimensions
    UINT numConfigs = (UINT)cm[0].size();
    // set the first possible solution
    VECTOR machinesToConfigs(numIdenticalMachines, 0);
    // Iterate through all possible combinations of assignments of configurations to machines
    while (true) 
    {
        if (isSolutionValid(cm, numJobs, machinesToConfigs)) 
        {
            // stop if we find a valid solution
            return true;
        }
        // Increment the combination of machinesToConfigs
        UINT index = 0;
        while (index < numIdenticalMachines) 
        {
            machinesToConfigs[index]++;
            if ((UINT)machinesToConfigs[index] < numConfigs) 
            {
                break;
            }
            else {
                machinesToConfigs[index] = 0;
                index++;
            }
        }
        // If all combinations are exhausted, exit the loop
        if (index == numIdenticalMachines) 
        {
            break;
        }
    }
    // No valid solution found
    return false;
}


BOOL_MATRIX DynTable::indStep
//-----------------------------------------------------------------------------
// Determines the result of the dynamic program requireing less storage space 
//-----------------------------------------------------------------------------
(
    BOOL_MATRIX& baseRes,
    int dim,
    MyVector& numMachines,
    MyVector* numJobs
)
{
    // define dimensions
    int numMachineTypes = numMachines.getSize();
    int numJobTypes = (*numJobs).getSize();
    BOOL_MATRIX indRes(numMachineTypes - 1, BOOL_VECTOR(dim));

    for (int j = 0; j < numMachineTypes - 1; j++)
    {
        for (int i = 0; i < dim; i++)
        {
            if (j == 0) // look up combinations in the base table
            {
                bool feasibility = false;
                for (int k = 0; k <= i; k++)
                {
                    // if there exists a feasible combination, we can stop
                    if (baseRes[0][k] && baseRes[j + 1][i - k])
                    {
                        feasibility = true;
                        break;
                    }
                }
                indRes[j][i] = feasibility;
            }
            else // look up combinations in previously filled rows of the inductive table
            {
                bool feasibility = false;
                for (int k = 0; k < i; k++)
                {
                    // if there exists a feasible combination, we can stop
                    if (indRes[j - 1][k] && baseRes[j + 1][i - k])
                    {
                        feasibility = true;
                        break;
                    }
                }
                indRes[j][i] = feasibility;
            }
        }
    }
    return indRes;
}


BOOL_MATRIX DynTable::optimizedIndStep
//-----------------------------------------------------------------------------
// Determines the result of the dynamic program requireing less storage space 
//-----------------------------------------------------------------------------
(
    BOOL_MATRIX& baseRes,
    int dim,
    MyVector& numMachines,
    MyVector* numJobs
)
{
    // define dimensions
    int numMachineTypes = numMachines.getSize();
    int numJobTypes = (*numJobs).getSize();
    BOOL_MATRIX indRes(2, BOOL_VECTOR(dim));

    for (int j = 0; j < numMachineTypes - 1; j++)
    {
        for (int i = 0; i < dim; i++)
        {
            if (j == 0) // look up combinations in the base table
            {
                bool feasibility = false;
                for (int k = 0; k <= i; k++)
                {
                    // if there exists a feasible combination, we can stop
                    if (baseRes[0][k] && baseRes[j + 1][i - k])
                    {
                        feasibility = true;
                        break;
                    }
                }
                indRes[0][i] = feasibility;
            }
            else // look up combinations in previously filled row of the inductive table
            {
                bool feasibility = false;
                for (int k = 0; k < i; k++)
                {
                    // if there exists a feasible combination, we can stop
                    if (indRes[0][k] && baseRes[j + 1][i - k])
                    {
                        feasibility = true;
                        break;
                    }
                }
                indRes[1][i] = feasibility;
            }
        }
        indRes[0] = indRes[1]; // push current machine set to previous machine set
    }
    return indRes;
}

/*
bool DynTable::solvePC_maxCPLEX
//-----------------------------------------------------------------------------
// Solve the P||C_max instance using CPLEX
//-----------------------------------------------------------------------------
(
    ConfigMatrix* A,
    MyVector& b
)
{
    bool result = false;
    int numVars = A->getNumCols();
    int numRows = A->getNumRows();

    // Model Definition
    IloEnv myenv; // environment object
    IloModel mymodel(myenv); // model object
    IloNumVarArray x(myenv, numVars, 0, IloInfinity); // array of numVars variables x
    myenv.setOut(myenv.getNullStream());

    // Constraint Generation
    for (int i = 0; i < numRows; i++)
    {
        IloExpr myexpr(myenv); //  empty expression
        for (int j = 0; j < numVars; j++)
        {
            myexpr += A->get(i,j) * x[j];
        }
        mymodel.add(myexpr == b.get(i)); // i-th constraint
        myexpr.end(); // clear memory
    }

    // Model Solution
    IloCplex mycplex(myenv);
    mycplex.extract(mymodel);
    mycplex.solve();

    if (mycplex.isPrimalFeasible())
    {
        result = true;
    }

    // Closing the Model
    mycplex.clear();
    myenv.end();

    return result;
}
*/

tuple<BOOL_MATRIX, BOOL_MATRIX> DynTable::dynProg
//-----------------------------------------------------------------------------
// Compute the dynamic table for given A, mu, nu, d
//-----------------------------------------------------------------------------
(
    ConfigMatrix* cm,
    MyVector& numMachines,
    int jobBound,
    MyVector* numJobs
)
{
    // define dimensions
    int numMachineTypes = numMachines.getSize();
    int numJobTypes = (*numJobs).getSize();
    int dim = (int)pow(jobBound, numJobTypes);
    //int Delta = cm->getMaxAbsValue();
  

    // initialize base table
    BOOL_MATRIX baseRes(numMachineTypes, BOOL_VECTOR(dim));

    // fill base table
    for (int i = 0; i < dim; i++)
    {
        MyVector currentJobs = getVector(i, numJobTypes, jobBound);

        // use CPLEX

        /*
        for (int j = 0; j < tau; j++)
        {
            // determine b
            int len = tau + d;
            MyVector b(len);
            for (int k = 0; k < d; k++)
            {
                b.set(k, n.get(k));
            }
            for (int k = d; k < len; k++)
            {
                if (k == d + j)
                {
                    b.set(k, mu.get(j));
                }
            }


            baseRes[j][i] = solvePC_maxCPLEX(A, b);
        }
        */


        // use brute force to determine the feasibility of the subproblem with identical machines
        int col = 0;

        // reduce A and only keep configurations for a certain machine type
        for (int row = 0; row < numMachineTypes; row++)
        {
            MATRIX reducedA(numJobTypes, VECTOR(0));
            // get all configurations of a single machine type
            bool singleType = true;
            while (singleType && col < cm->getNumCols())
            {
                if (cm->get(row + numJobTypes, col) == 1)
                {
                    for (int r = 0; r < numJobTypes; r++)
                    {
                        int value = cm->get(r, col);
                        reducedA[r].push_back(value);
                    }
                    col++;
                }
                else
                {
                    singleType = false;
                }
            }

            // if there are configruations of the vertain machine type, solve the P||C_max instance of that type
            if (reducedA[0].size() > 0)
            {
                baseRes[row][i] = this->solvePC_max(reducedA, (UINT)numMachines.get(row), currentJobs);
            }
        }
    }

    //BOOL_MATRIX indRes = optimizedIndStep(baseRes, dim, numMachines, numJobs);
    BOOL_MATRIX indRes = optimizedIndStep(baseRes, dim, numMachines, numJobs);
    

    //fill inductive table
    for (int j = 0; j < numMachineTypes - 1; j++)
    {
        for (int i = 0; i < dim; i++)
        {
            if (j == 0) // look up combinations in the base table
            {
                bool feasibility = false;
                for (int k = 0; k <= i; k++)
                {
                    // if there exists a feasible combination, we can stop
                    if (baseRes[0][k] && baseRes[j + 1][i - k])
                    {
                        feasibility = true;
                        break;
                    }
                }
                indRes[j][i] = feasibility;
            }
            else // look up combinations in previously filled rows of the inductive table
            {
                bool feasibility = false;
                for (int k = 0; k < i; k++)
                {
                    // if there exists a feasible combination, we can stop
                    if (indRes[j - 1][k] && baseRes[j + 1][i - k])
                    {
                        feasibility = true;
                        break;
                    }
                }
                indRes[j][i] = feasibility;
            }
        }
    }
    // return both, the base table and the indutive table
    return make_tuple(baseRes, indRes);
}
