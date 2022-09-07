#ifndef _LATTICE_H_
#define _LATTICE_H_

#include "Array.h"
#include <string>
using namespace std;
struct eventInfo
{
    eventInfo() { coord.x = 0; coord.y = 0; rate = 0.0; };
    eventInfo(const eventInfo &e){coord.x = e.coord.x; coord.y = e.coord.y; rate = e.rate;};
    void operator=(const eventInfo &e){coord.x = e.coord.x; coord.y = e.coord.y; rate = e.rate;};
    IntCoord2D coord;
    double rate;
};

class lattice
{
public:
    // Initialization and destory
    lattice(char* fname, char* outDir);
    ~lattice();

    // Initialization of Colony
    void putInitialCells(int initialSeparation);
    void putInitialCellsRandom(int numCells, int sideLen);
    void putInitialCellsSideBySide(int numberOfInitialCellsPerRow);
    void putInitialCellsWithMatrix(string path);

    // Calculation
    void calculateEventRate();
    eventInfo findCoordOfEvent();
    int updateEvent(eventInfo info);
    void updateWeight(int i, int j, double dk);
#ifdef MUTUALISM
    void updateNutrientA(int i, int j, int oldNutrientA, int newNutrientA);
#endif
    void updateNutrientB(int i, int j, int oldNutrientB, int newNutrientB);
    void divideCell(int i, int j, int type);
    void diffuseCell(int i, int j, int type);
    double timeIncrease(double t);

    // Main function
    void simulation(int numOutput);

    // Data I/O
    void saveData(string path);
    void readData(string path);
    void outputAllInfo(string path);
    void writeToFile(int OutputID);

    // Statistics
    int countCells(int& CellNum1, int& CellNum2);
#ifdef DEBUG
    int checkWeight(int i, int j, const char errorMessage[100]);
    int checkWeightBeforeChange(int i, int j, double dk, const char errorMessage[100]);
#endif
private:
    param P;
    Array2D<datax>* D;
    Array2D<double>* CDF; // Cumulative Distribution Function for events
    Array2D<double>* Del;
    double totalEventWeight;
    int minX;
    int maxX;
    int minY;
    int maxY;
    int searchLevels;
};

#endif
