#include <iostream>
#include <vector>

#include "../include/IPInstance.h"
#include "../include/Logger.h"

extern Logger* globalLogger;    // for logging


IPInstance::IPInstance
//-----------------------------------------------------------------------------
// constructor;
//-----------------------------------------------------------------------------
(
    ConfigMatrix* confMatrix,
    MyVector* jobVec,
    MyVector* machineVec
)
{
    m_cm = confMatrix;
    m_numJobs = jobVec;
    m_numMachines = machineVec;
}


IPInstance::~IPInstance
//-----------------------------------------------------------------------------
// destructor;
//-----------------------------------------------------------------------------
(
)
{
    // do nothing
}


void IPInstance::getIterations
//-----------------------------------------------------------------------------
// returns a vector of the number of iterations for each machine type
//-----------------------------------------------------------------------------
(
    int maxSupp,
    MyVector* numIters
)
{
    // define dimension
    int numMachineTypes = m_numMachines->getSize();

    // iterate over the machine types
    for (int j = 0; j < numMachineTypes; j++)
    {
        // determine the number of iterations for the current type
        double mS = (double)maxSupp; // cast maxSupp to double for division

        double val = std::log2( (m_numMachines->get(j) + mS) / (2 * mS + 1));

        int value = 1;
        // check if val is integer
        if (std::ceil(val) == std::floor(val))
        {
            value = max(value, (int)val + 2);
        } 
        else
        {
            value = max(value, (int)std::ceil(val) + 1);
        }
        numIters->set(j, value);
    }
}




void IPInstance::getNumMachinesSubProb
//-----------------------------------------------------------------------------
// determines the number of machines of the subproblem in iteration i
//-----------------------------------------------------------------------------
(
    int maxSupp,
    MyVector& numIters,
    int iteration,
    MyVector* numMachinesSubProb
)
{
    // define dimension
    int numMachineTypes = m_numMachines->getSize();

    // iterate over the machine types
    for (int j = 0; j < numMachineTypes; j++)
    {
        // determine the iteration where the current machine type is introduced
        int startingIter = numIters.get(0) - numIters.get(j) + 1;

        if (startingIter <= iteration) // machine type has been introduced
        {
            // set number of machines of the sub problem = \lfloor\frac{m_j}{2^{\calI-i}} - \maxSupp \sum_{k=i}^{\calI-1} \frac{1}{2^{\calI-k}}\rfloor 
            double sum = 0;
            for (int k = iteration; k <= numIters.get(0) - 1; k++)
            {
                sum += 1 / std::pow(2, numIters.get(0) - k);
            }
            int value = (int)std::floor(m_numMachines->get(j) / std::pow(2, numIters.get(0) - iteration) - maxSupp * sum);

            numMachinesSubProb->set(j, value);
        }
        else // machine type is not introduced yet
        {
            numMachinesSubProb->set(j, 0);
        }
    }
}

void IPInstance::getUsedJobs
//-----------------------------------------------------------------------------
// determines the minimum number of jobs that has been used in an iteration
//-----------------------------------------------------------------------------
(
    MYVEC_MATRIX jobSet,
    MyVector* usedJobs
)
{
    // define dimension
    int numJobTypes = m_numJobs->getSize();
    
    for (int j = 0; j < numJobTypes; j++)
    {
        usedJobs->set(j, jobSet[0].get(j));
    }
    
    for (int i = 1; i < jobSet.size(); i++)
    {
        for (int j = 0; j < numJobTypes; j++)
        {
            if (jobSet[i].get(j) < usedJobs->get(j))
            {
                usedJobs->set(j, jobSet[i].get(j));
            }
        }
    }
}


bool IPInstance::isInBox
//-----------------------------------------------------------------------------
// determines wheather a given point lies in the box around n/2^(I-i)
//-----------------------------------------------------------------------------
(
    MyVector& n_i,
    int iterations,
    int currentIter,
    int boxSize
)
{
    // define dimension
    int numJobTypes = m_numJobs->getSize();

    // determine the new vector depending on the iteration's center
    MyVector newVec(numJobTypes);
    for (int i = 0; i < numJobTypes; i++)
    {
        int val = (int)std::floor(m_numJobs->get(i) / pow(2, iterations - currentIter) - n_i.get(i));
        newVec.set(i, val);
    }

    // check wheather the new vector lies in the box or not
    return newVec.infNorm() <= boxSize;
}



bool IPInstance::nIsInFinalSet
//-----------------------------------------------------------------------------
// determines wheather the point n is in the set final set of jobvectors
//-----------------------------------------------------------------------------
(
    MYVEC_MATRIX& currentJobSet
)
{
    // iterate over the set of jobvectors
    for (UINT i = 0; i < currentJobSet.size(); i++)
    {
        // if the final jobvector is found, we are done :)
        if (m_numJobs->equals(currentJobSet[i]))
        {
            return true;
        }
    }
    // final jobvector has not been found
    return false;
}


int IPInstance::getBoxBound
(
    MyVector& usedJobs, 
    int numIters, 
    int currentIter
)
{
    // define dimensions
    int numJobTypes = m_numJobs->getSize();
    
    // determine maximum bound
    int maxBound = 0;
    for (int i = 0; i < numJobTypes; i++)
    {
        int currentBound = (int) std::floor(((double) m_numJobs->get(i) / std::pow(2, numIters - currentIter)) - usedJobs.get(i));
        maxBound = std::max(maxBound, currentBound);
    }

    return maxBound;
}


bool IPInstance::solve
//-----------------------------------------------------------------------------
// solves the IP-instance and returns its feasibility
//-----------------------------------------------------------------------------
(
)
{
    // determine dimensions
    int numMachineTypes = m_numMachines->getSize();
    int numJobTypes = m_numJobs->getSize();

    // define variables
    int Delta = m_cm->getMaxAbsValue();
    cout << "Delta = " << Delta << endl;
    int maxSupp = (int)std::floor(2.0 * (numJobTypes +1) * std::log2(4 * (numJobTypes+1) * Delta));
    cout << "maxSupp = " << maxSupp << endl;

    // determine the numbers of iterations for each machine type
    MyVector numIters(numMachineTypes);
    this->getIterations(maxSupp, &numIters);

    globalLogger->log("The number of iterations I is ", numIters.get(0), LogTypeInfo);
    cout << "I = " << numIters.get(0) << endl;

    // determine box size of the subproblem
    int boxSizeD = numMachineTypes * maxSupp * Delta;
    globalLogger->log("The box size D is ", boxSizeD, LogTypeInfo);
    cout << "D = " << boxSizeD << endl;

    
    // first iteration
    MyVector usedJobs(VECTOR(numJobTypes, 0));
    int boxBound = getBoxBound(usedJobs, numIters.get(0), 1);
    globalLogger->log("The bound for the box is ", boxBound, LogTypeInfo);
    cout << "box bound = " << boxBound << endl;

    int boxSize = std::min(boxBound, boxSizeD) + 1; // + 1 to include 0

    //determine number of machines for the first subproblem
    MyVector numMachinesFirstSubProb(numMachineTypes);
    this->getNumMachinesSubProb(maxSupp, numIters, 1, &numMachinesFirstSubProb);

    // sets of feasible jobs before and after the duplication and of the subproblem
    MYVEC_MATRIX jobSet, doubledJobSet, jobSetSubProb;

    // construct the dynamic table of the first iteration
    DynTable dtFirst(m_cm, numMachinesFirstSubProb, boxSize, m_numJobs);

    // define the first set of feasible jobs directly from the dynamic table
    jobSet = dtFirst.getFeasibleJobs(numMachineTypes, boxSize, numJobTypes);


    // iterations 2 .. I
    for (int i = 2; i <= numIters.get(0); i++)
    {
        // update3 box bound
        this->getUsedJobs(jobSet, &usedJobs);
        boxBound = getBoxBound(usedJobs, numIters.get(0), i);
        boxSize = std::min(boxBound, boxSizeD) + 1; // + 1 to include 0
        cout << "new box bound = " << boxBound << endl;

        // double the feasible jobs of the prior iteration
        for (UINT j = 0; j < jobSet.size(); j++)
        {
            doubledJobSet.push_back(jobSet[j].vecSum(jobSet[j]));
        }

        // delete jobs of the prior iteration from the set of feasible jobs
        jobSet.clear();

        //determine number of machines for the current subproblem
        MyVector numMachinesSubProb(numMachineTypes);
        this->getNumMachinesSubProb(maxSupp, numIters, i, &numMachinesSubProb);

        // construct the dynamic table of the current iteration
        DynTable dt(m_cm, numMachinesSubProb, 3 * boxSize, m_numJobs);
        
        // define the set of feasible jobs of the current subproblem
        jobSetSubProb = dt.getFeasibleJobs(numMachineTypes, boxSize, numJobTypes);

        // generate combinations of doubled jobs of the previous iteration and the jobs of the subproblem
        for (UINT j = 0; j < doubledJobSet.size(); j++)
        {
            for (UINT k = 0; k < jobSetSubProb.size(); k++)
            {
                MyVector jobVector = doubledJobSet[j].vecSum(jobSetSubProb[k]);
                
                // check wheather the new jobvector lies in the box of the current iteration
                if (isInBox(jobVector, numIters.get(0), i, boxSize))
                {
                    jobSet.push_back(jobVector);
                }
            }
        }

        // delete jobs from the auxiliary vectors
        doubledJobSet.clear();
        jobSetSubProb.clear();
    }

    // check wheather the given job vector is in the set of feasible jobs
    return nIsInFinalSet(jobSet);
}