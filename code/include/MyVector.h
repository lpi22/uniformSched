#pragma once

#include "../include/Defines.h"

using namespace std;

class MyVector
{
private:
	UINT			m_size;
	VECTOR			m_data;
	void			merge(MyVector* s, UINT left, UINT middle, UINT right);
	int				getGCD(int a, int b);

public:
	MyVector(UINT numElems);
	MyVector(VECTOR input);
	~MyVector();
	int				get(UINT index);
	void 			set(UINT, int);
	UINT			getSize();
	int				infNorm();
	MyVector		vecSum(MyVector& vec);
	MyVector		vecDiff(MyVector& vec);
	bool			equals(MyVector& vec);
	void			sortTwoVec(MyVector* s);
	void			mergeSortTwoVec(MyVector* s, UINT left, UINT right);
	int				getLCM();
};

inline UINT MyVector::getSize() { return m_size; }