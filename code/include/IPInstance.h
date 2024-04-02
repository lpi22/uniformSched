#pragma once

#include "../include/MyVector.h"
#include "../include/DynTable.h"
#include "../include/ConfigMatrix.h"
#include "../include/Defines.h"

class MyVector;
class ConfigMatrix;
class DynTable;

class IPInstance
{
private:
	ConfigMatrix*	m_cm;
	MyVector*		m_numJobs;
	MyVector*		m_numMachines;
	void			getIterations(int maxSupp, MyVector* numIters);
	void			getNumMachinesSubProb(int maxSupp, MyVector& numIters, int iteration, MyVector* tildeM);
	void			getUsedJobs(MYVEC_MATRIX jobSet, MyVector* usedJobs);
	bool			isInBox(MyVector& numJobsi, int iterations, int current_iter, int boxSize_D);
	bool			nIsInFinalSet(MYVEC_MATRIX& setNi);
	int				getBoxBound(MyVector& usedJobs, int numIters, int currentIter);

public:
	IPInstance(ConfigMatrix* cm, MyVector* numJobs, MyVector* numMachines);
	~IPInstance();
	bool			solve();
};