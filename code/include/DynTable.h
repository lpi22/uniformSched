#pragma once

#include "../include/MyVector.h"
#include "../include/ConfigMatrix.h"
#include "../include/Defines.h"

class MyVector;
class ConfigMatrix;


class DynTable
{
private:
	BOOL_MATRIX							m_baseTable;
	BOOL_MATRIX							m_inductiveTable;
	tuple<BOOL_MATRIX, BOOL_MATRIX>		dynProg(ConfigMatrix* cm, MyVector& numMachines, int jobBound, MyVector* numJobs);
	int									getIndex(MyVector& vec, int jobBound);
	MyVector							getVector(int index, int numJobTypes, int jobBound);
	bool								solvePC_max(MATRIX& cm, UINT numIdenticalMachines, MyVector& numJobs);
	//bool								solvePC_maxCPLEX(ConfigMatrix* A, MyVector& b);
	BOOL_MATRIX							indStep(BOOL_MATRIX& baseRes, int dim, MyVector& numMachines, MyVector* numJobs);
	BOOL_MATRIX							optimizedIndStep(BOOL_MATRIX& baseRes, int dim, MyVector& numMachines, MyVector* numJobs);
	bool								isSolutionValid(MATRIX& A, MyVector& b, VECTOR& machinesToConfigs);

public:
	DynTable(ConfigMatrix* cm, MyVector& numMachines, int jobBound, MyVector* numJobs);
	~DynTable();
	MYVEC_MATRIX						getFeasibleJobs(int numMachineTypes, int jobBound, int numJobTypes);
};