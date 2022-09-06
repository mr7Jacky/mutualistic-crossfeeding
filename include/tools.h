#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

int mkDir(const char *path);

//const double PI = 3.141592653589793238462643383279502884197;

const double PI = 3.14159;

#ifdef NDEBUG
inline void MyAssert(bool b, const char* msg){}

#else
inline void MyAssert(bool b, const char* msg)
{
	if (!b)
	{
		printf("%s\n", msg);
		fflush(stdout);
		assert(false);
		exit(-1);
	}
}
#endif

template<typename T>
struct Coord2D
{
	Coord2D(){}
	Coord2D(T _x, T _y): x(_x), y(_y) {}

	T x;
	T y;
};

typedef Coord2D<double> DoubleCoord2D;
typedef Coord2D<int> IntCoord2D;

inline double max(double n1, double n2)
{
	return n1>n2? n1: n2;
}

inline int max(int n1, int n2)
{
	return n1>n2? n1: n2;
}

inline double min(double n1, double n2)
{
	return n1<n2? n1: n2;
}

struct data
{
    data()
    {
        cellType = 0;
        nutrientB = 0;
        k1 = 0;
        k2 = 0;
        k3 = 0;
        k4 = 0;
        k5 = 0;
        k6 = 0;
        k7 = 0;
        k0 = 0;
#ifdef MUTUALISM
        nutrientA = 0;
        k8 = 0;
        k9 = 0;
        k10 = 0;
#endif
    }
    int cellType;
    int nutrientB;
    double k1; // cell1 divide
    double k2; // cell2 divide
    double k3; // cell1 diffuse
    double k4; // cell2 diffuse
    double k5; // cell1 excrete nutrient B
    double k6; // cell2 intake nutrient B
    double k7; // nutrient B Diffuse
#ifdef MUTUALISM
    int nutrientA;
    double k8; // cell 1 take up nutrient A
    double k9; // cell 2 excrete nutrient A
    double k10; // nutrient A Diffuse
#endif
    double k0; // sum_i(k_i)
};

struct param
{
    double maxGrowthRate1;
    double maxGrowthRate2;
    double excretionRateB;
    double diffusion1;
    double diffusion2;
    double diffusionB;
    double yieldB;
    double monodConstantB;
#ifdef MUTUALISM
    double excretionRateA;
    double yieldA;
    double diffusionA;
    double monodConstantA;
#endif
    
#ifdef TOXICITY
    double toxicityB;
#ifdef MUTUALISM
    double toxicityA;
#endif
#endif
    
    // time
    double t_max;
    
    // non-physical constants
    int BoxX;
    int BoxY;
    double BoxLength;
    
    // directory name
    char DirName[500];
};

int GetFileLen(FILE* myFile);

char* GetNextString(char*& buffer);

param ReadParameters(char* fname, char* outDir);

#endif /* TOOLS_H_ */

