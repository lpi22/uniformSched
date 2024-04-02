#pragma once

#include "../include/MyVector.h"
#include "../include/Defines.h"

using namespace std;

class MyVector;


class ConfigMatrix
{
private:
	MATRIX			m_configData;
	UINT			m_numConfigCols;
	UINT			m_numConfigRows;
	MATRIX			construct(MyVector* numJobs, MyVector* processingTimes, MyVector* numMachines, MyVector* speedValues, double makespan);
	void			generateVectors(MATRIX& matrix, VECTOR& currentVector, UINT rowIndex, double makespan,
									MyVector* processingTimes, UINT speed, MATRIX& resultVectors, UINT& num_configs_j);
	MATRIX			transpose(MATRIX* matrix);
	
public:
	ConfigMatrix(MyVector* numJobs, MyVector* processingTimes, MyVector* numMachines, MyVector* speeds, double makespan);
	~ConfigMatrix();
	int				getNumRows();
	int				getNumCols();
	int				get(int row, int col);
	void			set(int row, int col, int value);
	int				getMaxAbsValue();
};

inline int ConfigMatrix::getNumRows() { return m_numConfigRows; }
inline int ConfigMatrix::getNumCols() { return m_numConfigCols; }