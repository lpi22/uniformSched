#pragma once
typedef unsigned int		UINT;

#define BOOL_MATRIX			std::vector<std::vector<bool>>
#define BOOL_VECTOR			std::vector<bool>
#define MYVEC_MATRIX		std::vector<MyVector>
#define INFILE				std::ifstream
#define OUTFILE				std::ofstream
#define STRING				std::string
#define INSTRING			std::istringstream
#define VECTOR				std::vector<UINT>
#define MATRIX				std::vector<std::vector<UINT>>
#define LogTypeInfo			(char*)"INFO"
#define LogTypeWarning		(char*)"WARNING"
#define LogTypeError		(char*)"ERROR"
#define LogTypeTime			(char*)"TIME"
#define LOGNAME				(char*)"Log."
#define LOGNAMEEXTENSION	(char*)".log"
#define MAKESPANEXTENSION	(char*)".m"

#ifdef _WIN32
#define SLASH (STRING)"\\"
#else
#define SLASH (STRING)"/"
#endif