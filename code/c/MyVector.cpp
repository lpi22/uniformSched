#include <iostream>
#include <vector>

#include "../include/MyVector.h"
#include "../include/Logger.h"

extern Logger* globalLogger;    // for logging


MyVector::MyVector
//-----------------------------------------------------------------------------
// constructor;
//-----------------------------------------------------------------------------
(
    UINT numElems
)
{
    m_size = numElems;
    m_data = VECTOR(m_size, 0);
}

MyVector::MyVector
//-----------------------------------------------------------------------------
// constructor init given;
//-----------------------------------------------------------------------------
(
    VECTOR input
)
{
    m_size = (UINT) input.size();
    m_data = input;
}

MyVector::~MyVector
//-----------------------------------------------------------------------------
// destructor;
//-----------------------------------------------------------------------------
(
)
{
    // nothing to do
}



int MyVector::get
//-----------------------------------------------------------------------------
// getter for the Element in Vector[row][col]
//-----------------------------------------------------------------------------
(
    UINT index
)
{
    if (index < 0 || index >= this->m_size) 
    {
        globalLogger->log("index out of bounds (MyVector::get)", LogTypeError);
        cout << "index out of bounds (MyVector::get)" << endl;
        return 0;
    }
    return m_data[index];
}

void MyVector::set
//-----------------------------------------------------------------------------
// setter for the Element in Vector[index]
//-----------------------------------------------------------------------------
(
    UINT index,
    int value
)
{
    if (index >= 0 && index < this->m_size) 
    {
        m_data[index] = value;
    }
    else 
    {
        globalLogger->log("index out of bounds (MyVector::set)", LogTypeError);
        cout << "index out of bounds (MyVector::set)" << endl;
        // do nothing
    }
}


int MyVector::infNorm
//-----------------------------------------------------------------------------
// calculate the infinity norm
//-----------------------------------------------------------------------------
(
)
{
    int maxVal = 0;
    for (UINT i = 0; i < this->m_size; i++)
    {
        int absVal = std::abs(this->get(i));
        if (absVal > maxVal)
        {
            maxVal = absVal;
        }
    }
    return maxVal;
}

MyVector MyVector::vecSum
//-----------------------------------------------------------------------------
// Return sum of this and the given vector
//-----------------------------------------------------------------------------
(
    MyVector& vec
)
{
    MyVector res(m_size);
    if (m_size != vec.getSize())
    {
        globalLogger->log("Incompatible vector sizes (MyVector::vecSum)", LogTypeError);
        std::cout << "Incompatible vector sizes (MyVector::vecSum)" << endl;
    }
    else
    {
        for (UINT i = 0; i < this->m_size; i++)
        {
            res.set(i, this->get(i) + vec.get(i));
        }
    }
    return res.m_data;
}


MyVector MyVector::vecDiff
//-----------------------------------------------------------------------------
// Return difference between this and the given vector
//-----------------------------------------------------------------------------
(
    MyVector& vec
)
{
    MyVector res(this->m_size);
    if (this->m_size != vec.getSize())
    {
        globalLogger->log("Incompatible vector sizes (MyVector::vecDiff)", LogTypeError);
        std::cout << "Incompatible vector sizes (MyVector::vecDiff)" << endl;
    }
    else
    {
        for (UINT i = 0; i < this->m_size; i++)
        {
            res.set(i, this->get(i) - vec.get(i));
        }
    }
    return res.m_data;
}

bool MyVector::equals
//-----------------------------------------------------------------------------
// Returns true if this vector equals the given one; false otherwise
//-----------------------------------------------------------------------------
(
    MyVector& vec
)
{
    if (this->m_size != vec.getSize())
    {
        globalLogger->log("Incompatible vector sizes (MyVector::equals)", LogTypeError);
        std::cout << "Incompatible vector sizes (MyVector::equals)" << endl;
        return false;
    }
    else
    {
        for (UINT i = 0; i < this->m_size; i++)
        {
            if (this->get(i) != vec.get(i))
            {
                return false;
            }
        }
    }
    return true;
}


void MyVector::sortTwoVec
//-----------------------------------------------------------------------------
// sorts this vector and adapts the given one 
//-----------------------------------------------------------------------------
(
    MyVector* s
)
{
    if (this->m_size != s->getSize())
    {
        globalLogger->log("Incompatible vector sizes (MyVector::sortTwoVec)", LogTypeError);
        std::cout << "Incompatible vector sizes (MyVector::sortTwoVec)" << endl;
    }
    else
    {
        int tmpM, tmpS;
        for (UINT i = 0; i < this->m_size; i++)
        {
            for (UINT j = i + 1; j < this->m_size; j++)
            {
                if (this->get(i) > this->get(j))
                {
                    tmpM = this->get(i);
                    this->set(i, this->get(j));
                    this->set(j, tmpM);
                    tmpS = s->get(i);
                    s->set(i, s->get(j));
                    s->set(j, tmpS);
                }
            }
        }
    }
}

void MyVector::merge
//-----------------------------------------------------------------------------
// merges two sorted arrays
//-----------------------------------------------------------------------------
(
    MyVector* s, 
    UINT left,
    UINT middle,
    UINT right
) 
{
    //define lenths of arrays
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // create temporary arrays L and R and Ls and Rs
    MyVector L(n1);
    MyVector R(n2);
    MyVector Ls(n1);
    MyVector Rs(n2);

    // copy data to temporary arrays L and R
    for (int i = 0; i < n1; i++) 
    {
        L.set(i, this->get(left + i));
        Ls.set(i, s->get(left + i));
    }

    for (int i = 0; i < n2; i++) 
    {
        R.set(i, this->get(middle + i + 1));
        Rs.set(i, s->get(middle + i + 1));
    }

    int i = 0;
    int j = 0;
    int k = left;

    // Merge the two subarrays back into the original array
    while (i < n1 && j < n2) {
        if (L.get(i) >= R.get(j)) 
        {
            this->set(k, L.get(i));
            s->set(k, Ls.get(i));
            i++;
        }
        else {
            this->set(k, R.get(j));
            s->set(k, Rs.get(j));
            j++;
        }
        k++;
    }

    // copy all remaining elements of L and R
    while (i < n1) {
        this->set(k, L.get(i));
        s->set(k, Ls.get(i));
        i++;
        k++;
    }

    while (j < n2) {
        this->set(k, R.get(j));
        s->set(k, Rs.get(j));
        j++;
        k++;
    }
}


void MyVector::mergeSortTwoVec
//-----------------------------------------------------------------------------
// sorts this vector and adapts the given one via merge sort
//-----------------------------------------------------------------------------
(
    MyVector* s, 
    UINT left, 
    UINT right
) 
{
    if (left < right) {
        // find the middle point
        int middle = left + (right - left) / 2;

        // recursively sort the first and second halves
        this->mergeSortTwoVec(s, left, middle);
        this->mergeSortTwoVec(s, middle + 1, right);

        // merge the sorted halves
        this->merge(s, left, middle, right);
    }
}

int MyVector::getGCD
//-----------------------------------------------------------------------------
// calculates the greatest common divisor of the two input numbers
//-----------------------------------------------------------------------------
(
    int a, 
    int b
)
{
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int MyVector::getLCM
//-----------------------------------------------------------------------------
// calculates the least common multiple of all elements in the vector
//-----------------------------------------------------------------------------
(
)
{
    // check if there are elements in the vector
    if (this->m_size == 0) {
        globalLogger->log("LCM is undefined for empty vector (MyVector::getLCM)", LogTypeError);
        cout << "LCM is undefined for empty vector (MyVector::getLCM)" << endl;
        return 0; 
    }
    
    // initialize lcm with the first value
    int lcm = this->get(0);
    // determine lcm pairwise
    for (UINT i = 1; i < this->getSize(); i++) {
        int num = this->get(i);
        int gcd = this->getGCD(lcm, num);
        lcm = (lcm * num) / gcd;
    }

    return lcm;
}