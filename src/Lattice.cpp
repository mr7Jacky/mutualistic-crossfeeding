#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Lattice.h"

using namespace std;

lattice::lattice(char* fname, char* outDir)
{
    srand (time(NULL));
    P = ReadParameters(fname, outDir);
    D = new Array2D<data>(P.BoxX,P.BoxY);
    CDF = new Array2D<double>(P.BoxX,P.BoxY);
    Del = new Array2D<double>(P.BoxX,P.BoxY);
    //putInitialCells(2);
    putInitialCellsRandom(100,20);
    //putInitialCellsSideBySide(50);
    //putInitialCellsWithMatrix("30.txt");
    //readData("fields_data.lattice");
    searchLevels = ((int) log(P.BoxX)/log(2)) -1;
};

lattice::lattice() {};

lattice::~lattice()
{
    delete D;
    delete CDF;
    delete Del;
};

/*
 * Save the Array2D
 */
void lattice::saveData(string path)
{
    ofstream file;
	file.open(path,ios::out);
	file.write(reinterpret_cast<char*>(&P),sizeof(P));
    file.write(reinterpret_cast<char*>(D),sizeof(D));
    file.write(reinterpret_cast<char*>(CDF),sizeof(CDF));
    file.write(reinterpret_cast<char*>(Del),sizeof(Del));
	file.close();
}

/*
 * Read from the saved data
 */
void lattice::readData(string path)
{
    ifstream file;
	file.open(path, ios::in);
	file.read(reinterpret_cast<char*>(&P),sizeof(P));
    file.read(reinterpret_cast<char*>(D),sizeof(D));
    file.read(reinterpret_cast<char*>(CDF),sizeof(CDF));
    file.read(reinterpret_cast<char*>(Del),sizeof(Del));
    file.close();
}

void lattice::outputAllInfo(string path)
{
    FILE* out;
#ifdef _WIN32
    fopen_s(&out, path, "w"); 
#else
    out = fopen(path.c_str(), "w");
#endif
    D->OutputAll(out);
    CDF->Append(out);
    Del->Append(out);
    fclose(out);
};

/*
 * Default Initialization
 */
void lattice::putInitialCells(int initialSeparation)
{
    int type, i1, j1, i2, j2;
    type = rand()%2+1;
    i1 = rand()%initialSeparation-initialSeparation/2;
    j1 = rand()%initialSeparation-initialSeparation/2;
    do {
        i2 = rand()%initialSeparation-initialSeparation/2;
        j2 = rand()%initialSeparation-initialSeparation/2;
    } while (i2 == i1 && j2 ==j1);
    D[0](P.BoxX/2+i1, P.BoxY/2+j1).cellType = type;
    D[0](P.BoxX/2+i2, P.BoxY/2+j2).cellType = 3-type;
    minX = min(P.BoxX/2+i1, P.BoxX/2+i2);
    maxX = max(P.BoxX/2+i1, P.BoxX/2+i2);
    minY = min(P.BoxY/2+j1, P.BoxY/2+j2);
    maxY = max(P.BoxY/2+j1, P.BoxY/2+j2);
    //printf("minX = %d, minY = %d, maxX = %d, maxY = %d\n i1 = %d, j1 = %d, i2 = %d, j2 = %d\n", minX, minY, maxX, maxY, i1, j1, i2, j2);
};

/*
 * Put given number of cells randomly
 */
void lattice::putInitialCellsRandom(int numCells, int sideLen)
{   
    while (sideLen * sideLen < numCells) {
        sideLen *= 2;
    }
    int x,y;
    minX = P.BoxX;
    maxX = 0;
    minY = P.BoxY;
    maxY = 0;
    int mid = P.BoxX / 2;
    for(int i = 0; i < numCells; i++) {
        srand(time(0));
        while (true) {
            x = rand() % (sideLen + 1) + (mid - sideLen / 2);
            y = rand() % (sideLen + 1) + (mid - sideLen / 2);
            if (D[0](x, y).cellType == 0) {
                D[0](x, y).cellType = 1;
                if (x < minX) minX = x;
                if (y < minY) minY = y;
                if (x > maxX) maxX = x;
                if (y > maxY) maxY = y;
                break;
            }
        }
    }
    for(int i = 0; i < numCells; i++) {
        srand(time(0));
        while (true) {
            x = rand() % (sideLen + 1) + (mid - sideLen / 2);
            y = rand() % (sideLen + 1) + (mid - sideLen / 2);
            if (D[0](x, y).cellType == 0) {
                D[0](x, y).cellType = 2;
                if (x < minX) minX = x;
                if (y < minY) minY = y;
                if (x > maxX) maxX = x;
                if (y > maxY) maxY = y;
                break;
            }
        }
    }
    printf("minX = %d, minY = %d, maxX = %d, maxY = %d\n", minX, minY, maxX, maxY);
};

/*
 * side-by-side stripes of two cells
 */

void lattice::putInitialCellsSideBySide(int numberOfInitialCellsPerRow)
{
    int mid = (int) numberOfInitialCellsPerRow / 2;
    for (int i = 0; i < mid + 1; i++) {
        D[0](P.BoxX/2+1, P.BoxY/2+i).cellType = 1;
        D[0](P.BoxX/2+1, P.BoxY/2-i).cellType = 1;
        D[0](P.BoxX/2, P.BoxY/2+i).cellType = 2;
        D[0](P.BoxX/2, P.BoxY/2-i).cellType = 2;
    }
    minX = P.BoxX/2;
    maxX = P.BoxX/2 + 1;
    minY = P.BoxY/2 - mid;
    maxY = P.BoxY/2 + mid;
    if (numberOfInitialCellsPerRow % 2 == 0) {
        D[0](P.BoxX/2, P.BoxY/2+mid).cellType = 0;
        D[0](P.BoxX/2+1, P.BoxY/2+mid).cellType = 0;
        maxY = P.BoxY/2 + mid - 1;
    }
    //printf("minX = %d, minY = %d, maxX = %d, maxY = %d\n", minX, minY, maxX, maxY);
};

/*
 * Initialize with a given matrix
 */
void lattice::putInitialCellsWithMatrix(string path)
{
    ifstream file;
    file.open(path);
    if (!file.is_open()) return;
    string word;
    int type, row, col, count, value;
    type = row = col = count = 0;
    minX = P.BoxX;
    minY = P.BoxY;
    maxX = maxY = 0;
    while (file >> word)
    {
        // Calculate the row and col in the D
        row = count / 1500;
        col = count % 1500;
        // Only read the first matrix from file
        count ++;
        if (row >= 1500) {
            if (row >= 3000) {
                value = stoi(word,nullptr,10);
                D[0](col, row - 3000).nutrientA = value;
                continue;
            } else {
                value = stoi(word,nullptr,10);
                D[0](col, row - 1500).nutrientB = value;
                continue;
            }
        }
        // Get the type of cell
        type = stoi(word,nullptr,10);
        // If no cell read the next cell
        if (type == 0) {
            continue;
        }
        // add the cell to the matrix
        D[0](col, row).cellType = type;
        // calulate the min and max of X and Y
        if (row < minX) minX = row;
        if (col < minY) minY = col;
        if (row > maxX) maxX = row;
        if (col > maxY) maxY = col;
    }
    //printf("minX = %d, minY = %d, maxX = %d, maxY = %d\n", minX, minY, maxX, maxY);
};


void lattice::calculateEventRate()
{
    int i,j;
    totalEventWeight = 0;
    for (i=0; i<P.BoxX; i++)
    {
        for (j=0; j<P.BoxY; j++)
        {
            D[0](i,j).k7 = P.diffusionB*D[0](i,j).nutrientB/P.BoxLength/P.BoxLength;
            if (D[0](i,j).cellType==1)
            {
                double lam1 = P.maxGrowthRate1;
#ifdef TOXICITY
                lam1 = lam1/(1+D[0](i,j).nutrientB/P.toxicityB);
#endif
#ifdef MUTUALISM
                lam1 = lam1*D[0](i,j).nutrientA/(D[0](i,j).nutrientA+P.monodConstantA);
#endif
                D[0](i,j).k1 = lam1;
                D[0](i,j).k2 = P.excretionRateB;
                D[0](i,j).k3 = P.diffusion1/P.BoxLength/P.BoxLength;
                D[0](i,j).k4 = 0;
                D[0](i,j).k5 = 0;
                D[0](i,j).k6 = 0;
#ifdef MUTUALISM
                D[0](i,j).k8 = lam1/P.yieldA;
                D[0](i,j).k9 = 0;
                D[0](i,j).k10 = P.diffusionA*D[0](i,j).nutrientA/P.BoxLength/P.BoxLength;
                D[0](i,j).k0 = D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3+D[0](i,j).k7+D[0](i,j).k8+D[0](i,j).k10;
#else
                D[0](i,j).k0 = D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3+D[0](i,j).k7;
#endif
            }
            else if (D[0](i,j).cellType==2)
            {
                double lam2 = P.maxGrowthRate2*D[0](i,j).nutrientB/(P.monodConstantB+D[0](i,j).nutrientB);
#if defined(TOXICITY) && defined(MUTUALISM)
                lam2 = lam2/(1+D[0](i,j).nutrientA/P.toxicityA);
#endif
                D[0](i,j).k1 = 0;
                D[0](i,j).k2 = 0;
                D[0](i,j).k3 = 0;
                D[0](i,j).k4 = lam2;
                D[0](i,j).k5 = lam2/P.yieldB;
                D[0](i,j).k6 = P.diffusion2/P.BoxLength/P.BoxLength;
                
#ifdef MUTUALISM
                D[0](i,j).k8 = 0;
                D[0](i,j).k9 = P.excretionRateA;
                D[0](i,j).k10 = P.diffusionA*D[0](i,j).nutrientA/P.BoxLength/P.BoxLength;
                D[0](i,j).k0 = D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6+D[0](i,j).k7+D[0](i,j).k9+D[0](i,j).k10;
#else
                D[0](i,j).k0 = D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6+D[0](i,j).k7;
#endif
            }
            else
            {
                D[0](i,j).k1 = 0;
                D[0](i,j).k2 = 0;
                D[0](i,j).k3 = 0;
                D[0](i,j).k4 = 0;
                D[0](i,j).k5 = 0;
                D[0](i,j).k6 = 0;
#ifdef MUTUALISM
                D[0](i,j).k8 = 0;
                D[0](i,j).k9 = 0;
                D[0](i,j).k10 = P.diffusionA*D[0](i,j).nutrientA/P.BoxLength/P.BoxLength;
                D[0](i,j).k0 = D[0](i,j).k7+D[0](i,j).k10;
#else
                D[0](i,j).k0 = D[0](i,j).k7;
#endif
            }
            totalEventWeight += D[0](i,j).k0;
        }
    }
    int a, b, l;
    for (i=0; i<P.BoxX; i++)
    {
        for (j=0; j<P.BoxY; j++)
            CDF[0](i,j) = D[0](i,j).k0;
        for (j=0; j<P.BoxY-1; j++)
        {
            a = 0;
            b = P.BoxY-1;
            CDF[0](i,b) += D[0](i,j).k0;
            for (l=0; l<searchLevels; l++)
            {
                if ((a+b)/2>=j)
                {
                    b = (a+b)/2;
                    if (j==b)
                        break;
                    CDF[0](i,b) += D[0](i,j).k0;
                }
                else
                    a = (a+b)/2+1;
            }
        }
    }
    for (i=0; i<P.BoxX-1; i++)
        Del[0](i,P.BoxY-1) = CDF[0](i,P.BoxY-1);
    for (i=0; i<P.BoxX-1; i++)
    {
        a = 0;
        b = P.BoxX-1;
        Del[0](b,P.BoxY-1) += CDF[0](i,P.BoxY-1);
        for (l=0; l<searchLevels; l++)
        {
            if ((a+b)/2>=i)
            {
                b = (a+b)/2;
                if (i==b)
                    break;
                Del[0](b,P.BoxY-1) += CDF[0](i,P.BoxY-1);
            }
            else
                a = (a+b)/2+1;
        }
    }
#ifdef DEBUG
    for (i=0; i<P.BoxX; i++)
    {
        for (j=0; j<P.BoxY; j++)
        {
            checkWeight(i,j,"Error within calculateEventRate\n");
        }
    }
#endif
};

eventInfo lattice::findCoordOfEvent()
{
    eventInfo info;
    info.rate = ((double) rand()+1) / ((double) RAND_MAX+1) * totalEventWeight;
    double kk = info.rate;
    int i, j, l;
    int a = 0;
    int b = P.BoxX-1;
    for (l=0; l<searchLevels; l++)
    {
        if (info.rate<=Del[0]((b+a)/2,P.BoxY-1))
            b = (b+a)/2;
        else
        {
            info.rate -= Del[0]((b+a)/2,P.BoxY-1);
            a = (b+a)/2+1;
        }
    }
    info.coord.x = b;
    for (i=a; i<b; i++)
    {
        if (info.rate<=CDF[0](i,P.BoxY-1))
        {
            info.coord.x = i;
            break;
        }
        else
            info.rate-=CDF[0](i,P.BoxY-1);
    }
    
    a = 0;
    b = P.BoxY-1;
    for (l=0; l<searchLevels; l++)
    {
        if (info.rate<=CDF[0](info.coord.x,(b+a)/2))
            b = (b+a)/2;
        else
        {
            info.rate -= CDF[0](info.coord.x,(b+a)/2);
            a = (b+a)/2+1;
        }
    }
    info.coord.y = b;
    for (j=a; j<b; j++)
    {
        if (info.rate<=CDF[0](info.coord.x,j))
        {
            info.coord.y = j;
            break;
        }
        else
            info.rate-=CDF[0](info.coord.x,j);
    }
#ifdef DEBUG
    if (D[0](info.coord.x,info.coord.y).k0==0)
    {
        printf("Found a wrong event: %d %d %g %g\n", info.coord.x,info.coord.y, D[0](info.coord.x,info.coord.y).k0, CDF[0](info.coord.x,info.coord.y));
        assert(0);
    }
#endif
    return info;
};

void lattice::updateWeight(int i, int j, double dk)
{
    D[0](i,j).k0 += dk;
    CDF[0](i,j) += dk;
    int a = 0;
    int b = P.BoxY-1;
    CDF[0](i,b) += dk;
    int l;
    for (l=0; l<searchLevels; l++)
    {
        if ((a+b)/2>=j)
        {
            b = (a+b)/2;
            if (j==b)
                break;
            CDF[0](i,b) += dk;
        }
        else
            a = (a+b)/2+1;
    }
    a = 0;
    b = P.BoxX-1;
    Del[0](i,P.BoxY-1) += dk;
    Del[0](b,P.BoxY-1) += dk;
    for (l=0; l<searchLevels; l++)
    {
        if ((a+b)/2>=i)
        {
            b = (a+b)/2;
            if (i==b)
                break;
            Del[0](b,P.BoxY-1) += dk;
        }
        else
            a = (a+b)/2+1;
    }
    totalEventWeight += dk;
#ifdef DEBUG
    checkWeight(i,j,"Error within updateWeight\n");
#endif
};

void lattice::updateNutrientB(int i, int j, int oldNutrientB, int newNutrientB)
{
#ifdef DEBUG
    assert(oldNutrientB>=0);
    assert(newNutrientB>=0);
    checkWeight(i,j,"Error within updateNutrientB pre\n");
    
    double k0=D[0](i,j).k0;
    double k1=D[0](i,j).k1;
    double k2=D[0](i,j).k2;
    double k3=D[0](i,j).k3;
    double k4=D[0](i,j).k4;
    double k5=D[0](i,j).k5;
    double k6=D[0](i,j).k6;
    double k7=D[0](i,j).k7;
#ifdef MUTUALISM
    double k8=D[0](i,j).k8;
    double k9=D[0](i,j).k9;
    double k10=D[0](i,j).k10;
#endif
#endif
    D[0](i,j).nutrientB = newNutrientB;
    double dk = P.diffusionB*(newNutrientB-oldNutrientB)/P.BoxLength/P.BoxLength;
    D[0](i,j).k7 = P.diffusionB*newNutrientB/P.BoxLength/P.BoxLength;
    if (D[0](i,j).cellType==1)
    {
        double lam1 = P.maxGrowthRate1;
        double dlam1 = lam1;
#ifdef TOXICITY
        dlam1 = dlam1/(1+newNutrientB/P.toxicityB)-dlam1/(1+oldNutrientB/P.toxicityB);
        lam1 = lam1/(1+newNutrientB/P.toxicityB);
#else
        dlam1 = dlam1*0;
#endif
#ifdef MUTUALISM
        lam1 = lam1*D[0](i,j).nutrientA/(D[0](i,j).nutrientA+P.monodConstantA);
        dlam1 = dlam1*D[0](i,j).nutrientA/(D[0](i,j).nutrientA+P.monodConstantA);
        dk += dlam1*(1.0+1.0/P.yieldA);
        D[0](i,j).k8 = lam1/P.yieldA;
#else
        dk += dlam1;
#endif
        D[0](i,j).k1 = lam1;
    }
    else if (D[0](i,j).cellType==2)
    {
        double lam = P.maxGrowthRate2*newNutrientB/(P.monodConstantB+newNutrientB);
        double lamOld = P.maxGrowthRate2*oldNutrientB/(P.monodConstantB+oldNutrientB);
#if defined(TOXICITY) && defined(MUTUALISM)
        lam = lam/(1+D[0](i,j).nutrientA/P.toxicityA);
        lamOld = lamOld/(1+D[0](i,j).nutrientA/P.toxicityA);
#endif
        dk += (lam-lamOld)*(1.0+1.0/P.yieldB);
        D[0](i,j).k4 = lam;
        D[0](i,j).k5 = lam/P.yieldB;
    }
#ifdef DEBUG
    checkWeightBeforeChange(i,j,dk,"Error in line 329 Lattice.cpp before updateWeight\n");
#endif
    updateWeight(i, j, dk);
    
#ifdef DEBUG
    checkWeight(i,j,"Error within updateNutrientB post\n");
#endif
};

#ifdef MUTUALISM
void lattice::updateNutrientA(int i, int j, int oldNutrientA, int newNutrientA)
{
#ifdef DEBUG
    assert(oldNutrientA>=0);
    assert(newNutrientA>=0);
    checkWeight(i,j,"Error within updateNutrientA pre\n");
#endif
    D[0](i,j).nutrientA = newNutrientA;
    double dk = 0;
#ifdef DEBUG
    double k8 = D[0](i,j).k8;
    double k10 =D[0](i,j).k10;
    double k1 = D[0](i,j).k1;
    double k4 = D[0](i,j).k4;
    double k5 = D[0](i,j).k5;
#endif
    if (D[0](i,j).cellType==1)
    {
        double lam1 = P.maxGrowthRate1;
        double dlam1 = lam1;
#ifdef TOXICITY
        dlam1 = dlam1/(1+D[0](i,j).nutrientB/P.toxicityB);
        lam1 = lam1/(1+D[0](i,j).nutrientB/P.toxicityB);
#endif
        lam1 = lam1*newNutrientA/(newNutrientA+P.monodConstantA);
        D[0](i,j).k1 = lam1;
        dlam1 = dlam1*(newNutrientA/(newNutrientA+P.monodConstantA)-oldNutrientA/(oldNutrientA+P.monodConstantA));
        dk += dlam1*(1.0+1.0/P.yieldA);
        D[0](i,j).k8 = lam1/P.yieldA;
        dk += P.diffusionA*(newNutrientA-oldNutrientA)/P.BoxLength/P.BoxLength;
        D[0](i,j).k10 = P.diffusionA*newNutrientA/P.BoxLength/P.BoxLength;
#ifdef DEBUG
        assert(fabs(D[0](i,j).k1+D[0](i,j).k8+D[0](i,j).k10-k1-k8-k10-dk)<1e-10);
#endif
    }
    else if (D[0](i,j).cellType==2)
    {
        double lam = P.maxGrowthRate2*D[0](i,j).nutrientB/(P.monodConstantB+D[0](i,j).nutrientB);
        double lamOld = lam;
#ifdef TOXICITY
        lam = lam/(1+newNutrientA/P.toxicityA);
        lamOld = lamOld/(1+oldNutrientA/P.toxicityA);
        dk += (lam-lamOld)*(1.0+1.0/P.yieldB);
        D[0](i,j).k4 = lam;
        D[0](i,j).k5 = lam/P.yieldB;
#endif
        dk += P.diffusionA*(newNutrientA-oldNutrientA)/P.BoxLength/P.BoxLength;
        D[0](i,j).k10 = P.diffusionA*newNutrientA/P.BoxLength/P.BoxLength;
#ifdef DEBUG
        assert(fabs(D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k10-k4-k5-k10-dk)<1e-10);
#endif
    }
    else
    {
        dk += P.diffusionA*(newNutrientA-oldNutrientA)/P.BoxLength/P.BoxLength;
        D[0](i,j).k10 = P.diffusionA*newNutrientA/P.BoxLength/P.BoxLength;
#ifdef DEBUG
        assert(dk == D[0](i,j).k10-k10);
#endif
    }
#ifdef DEBUG
    checkWeightBeforeChange(i,j,dk,"Error in line 380 Lattice.cpp before updateWeight\n");
#endif
    updateWeight(i, j, dk);
#ifdef DEBUG
    checkWeight(i,j,"Error within updateNutrientA post\n");
#endif
};
#endif

int lattice::updateEvent(eventInfo info)
{
    int i=info.coord.x;
    int j=info.coord.y;
    if (D[0](i,j).cellType==0)
    {
        int indX [5] = {(i-1+ P.BoxX)%P.BoxX, (i+1)%P.BoxX, i, i};
        int indY [5] = {j, j, (j-1+ P.BoxY)%P.BoxY, (j+1)%P.BoxY};
        int r = rand()%4;
#ifdef MUTUALISM
        if (info.rate <= D[0](i,j).k7)
        {
            updateNutrientB(i, j, D[0](i,j).nutrientB, D[0](i,j).nutrientB-1);
            updateNutrientB(indX[r],indY[r], D[0](indX[r],indY[r]).nutrientB, D[0](indX[r],indY[r]).nutrientB+1);
        }
        else
        {
#ifdef DEBUG
            assert(D[0](i,j).nutrientA-1>=0);
#endif
            updateNutrientA(i, j, D[0](i,j).nutrientA, D[0](i,j).nutrientA-1);
            updateNutrientA(indX[r],indY[r], D[0](indX[r],indY[r]).nutrientA, D[0](indX[r],indY[r]).nutrientA+1);
        }
#else
        updateNutrientB(i, j, D[0](i,j).nutrientB, D[0](i,j).nutrientB-1);
        updateNutrientB(indX[r],indY[r], D[0](indX[r],indY[r]).nutrientB, D[0](indX[r],indY[r]).nutrientB+1);
#endif
        return 0;
    }
    else if (info.rate <= D[0](i,j).k1)
    {
        divideCell(i,j,1);
        return 1;
    }
    else
    {
        info.rate -= D[0](i,j).k1;
        if (info.rate <= D[0](i,j).k2)
        {
            updateNutrientB(i, j, D[0](i,j).nutrientB, D[0](i,j).nutrientB+1);
            return 2;
        }
        else
        {
            info.rate -= D[0](i,j).k2;
            if (info.rate <= D[0](i,j).k3)
            {
                diffuseCell(i,j,1);
                return 3;
            }
            else
            {
                info.rate -= D[0](i,j).k3;
                if (info.rate <= D[0](i,j).k4)
                {
                    divideCell(i,j,2);
                    return 4;
                }
                else
                {
                    info.rate -= D[0](i,j).k4;
                    if (info.rate <= D[0](i,j).k5)
                    {
                        updateNutrientB(i, j, D[0](i,j).nutrientB, D[0](i,j).nutrientB-1);
                        return 5;
                    }
                    else
                    {
                        info.rate -= D[0](i,j).k5;
                        if (info.rate <= D[0](i,j).k6)
                        {
                            diffuseCell(i,j,2);
                            return 6;
                        }
                        else
                        {
                            info.rate -= D[0](i,j).k6;
                            if (info.rate <= D[0](i,j).k7)
                            {
                                int indX [5] = {(i-1)%P.BoxX, (i+1)%P.BoxX, i, i};
                                int indY [5] = {j, j, (j-1)%P.BoxY, (j+1)%P.BoxY};
                                int r = rand()%4;
                                updateNutrientB(i, j, D[0](i,j).nutrientB, D[0](i,j).nutrientB-1);
                                updateNutrientB(indX[r],indY[r], D[0](indX[r],indY[r]).nutrientB, D[0](indX[r],indY[r]).nutrientB+1);
                                i = indX[r]; j = indY[r];
                                return 7;
                            }
                            else
                            {
#ifdef MUTUALISM
                                info.rate -= D[0](i,j).k7;
                                if (info.rate <= D[0](i,j).k8)
                                {
#ifdef DEBUG
                                    assert(D[0](i,j).nutrientA-1>=0);
#endif
                                    updateNutrientA(i, j, D[0](i,j).nutrientA, D[0](i,j).nutrientA-1);
                                    return 8;
                                }
                                else
                                {
                                    info.rate -= D[0](i,j).k8;
                                    if (info.rate <= D[0](i,j).k9)
                                    {
                                        updateNutrientA(i, j, D[0](i,j).nutrientA, D[0](i,j).nutrientA+1);
                                        return 9;
                                    }
                                    else
                                    {
                                        info.rate -= D[0](i,j).k9;
                                        if (info.rate <= D[0](i,j).k10)
                                        {
                                            int indX [5] = {(i-1)%P.BoxX, (i+1)%P.BoxX, i, i};
                                            int indY [5] = {j, j, (j-1)%P.BoxY, (j+1)%P.BoxY};
                                            int r = rand()%4;
#ifdef DEBUG
                                            assert(D[0](i,j).nutrientA-1>=0);
#endif
                                            updateNutrientA(i, j, D[0](i,j).nutrientA, D[0](i,j).nutrientA-1);
                                            updateNutrientA(indX[r],indY[r], D[0](indX[r],indY[r]).nutrientA, D[0](indX[r],indY[r]).nutrientA+1);
                                            i = indX[r]; j = indY[r];
                                            return 10;
                                        }
                                        else
                                        {
#ifdef DEBUG
                                            printf("1 error exit from updateEvent\n");
                                            assert(0);
#endif
                                        }
                                    }
                                }
#else
#ifdef DEBUG
                                printf("2 error exit from updateEvent\n");
                                assert(0);
#endif
#endif
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
};

void lattice::divideCell(int i, int j, int type)
{
    int possibNbrs = 0;
    int indX[4];
    int indY[4];
    if (D[0](i-1,j).cellType == 0)
    {
        indX[possibNbrs] = (i-1)%P.BoxX;
        indY[possibNbrs] = j;
        possibNbrs++;
    }
    if (D[0](i+1,j).cellType == 0)
    {
        indX[possibNbrs] = (i+1)%P.BoxX;
        indY[possibNbrs] = j;
        possibNbrs++;
    }
    if (D[0](i,j-1).cellType == 0)
    {
        indX[possibNbrs] = i;
        indY[possibNbrs] = (j-1)%P.BoxY;
        possibNbrs++;
    }
    if (D[0](i,j+1).cellType == 0)
    {
        indX[possibNbrs] = i;
        indY[possibNbrs] = (j+1)%P.BoxY;
        possibNbrs++;
    }
    if (possibNbrs>=1)
    {
        int r = rand()%possibNbrs;
#ifdef DEBUG
        checkWeight(i,j,"Error within divideCell mother cell pre\n");
        checkWeight(indX[r],indY[r],"Error within divideCell daughter cell pre\n");
#endif
        D[0](indX[r],indY[r]).cellType = type;
        if (type==1)
        {
            double lam1 = P.maxGrowthRate1;
#ifdef TOXICITY
            lam1 = lam1/(1+D[0](indX[r],indY[r]).nutrientB/P.toxicityB);
#endif
#ifdef MUTUALISM
            lam1 = lam1*D[0](indX[r],indY[r]).nutrientA/(D[0](indX[r],indY[r]).nutrientA+P.monodConstantA);
#endif
            D[0](indX[r],indY[r]).k1 = lam1;
            D[0](indX[r],indY[r]).k2 = P.excretionRateB;
            D[0](indX[r],indY[r]).k3 = P.diffusion1/P.BoxLength/P.BoxLength;
            double dk = 0;
#ifdef MUTUALISM
            D[0](indX[r],indY[r]).k8 = lam1/P.yieldA;
            dk += D[0](indX[r],indY[r]).k8;
#endif
            dk += D[0](indX[r],indY[r]).k1+D[0](indX[r],indY[r]).k2+D[0](indX[r],indY[r]).k3;
#ifdef DEBUG
            checkWeightBeforeChange(indX[r],indY[r],dk,"Error in line 573 Lattice.cpp before updateWeight\n");
#endif
            updateWeight(indX[r],indY[r], dk);
        }
        else if (type==2)
        {
            double dk = 0;
#if defined(TOXICITY) && defined(MUTUALISM)
            D[0](indX[r],indY[r]).k4 = P.maxGrowthRate2*D[0](indX[r],indY[r]).nutrientB/(P.monodConstantB+D[0](indX[r],indY[r]).nutrientB)/(1+D[0](indX[r],indY[r]).nutrientA/P.toxicityA);
#else
            D[0](indX[r],indY[r]).k4 = P.maxGrowthRate2*D[0](indX[r],indY[r]).nutrientB/(P.monodConstantB+D[0](indX[r],indY[r]).nutrientB);
#endif
            D[0](indX[r],indY[r]).k5 = D[0](indX[r],indY[r]).k4/P.yieldB;
            D[0](indX[r],indY[r]).k6 = P.diffusion2/P.BoxLength/P.BoxLength;
#ifdef MUTUALISM
            D[0](indX[r],indY[r]).k9 = P.excretionRateA;
            dk += D[0](indX[r],indY[r]).k9;
#endif
            dk += D[0](indX[r],indY[r]).k4+D[0](indX[r],indY[r]).k5+D[0](indX[r],indY[r]).k6;
#ifdef DEBUG
            checkWeightBeforeChange(indX[r],indY[r],dk,"Error in line 592 Lattice.cpp before updateWeight\n");
#endif
            updateWeight(indX[r],indY[r], dk);
        }
        minX = min(minX,indX[r]);
        maxX = max(maxX,indX[r]);
        minY = min(minY,indY[r]);
        maxY = max(minY,indY[r]);
#ifdef DEBUG
        checkWeight(i,j,"Error within divideCell mother cell post\n");
        checkWeight(indX[r],indY[r],"Error within divideCell daughter cell post\n");
#endif
    }
};

void lattice::diffuseCell(int i, int j, int type)
{
    int possibNbrs = 0;
    int indX[4];
    int indY[4];
    if (D[0](i-1,j).cellType == 0)
    {
        indX[possibNbrs] = (i-1)%P.BoxX;
        indY[possibNbrs] = j;
        possibNbrs++;
    }
    if (D[0](i+1,j).cellType == 0)
    {
        indX[possibNbrs] = (i+1)%P.BoxX;
        indY[possibNbrs] = j;
        possibNbrs++;
    }
    if (D[0](i,j-1).cellType == 0)
    {
        indX[possibNbrs] = i;
        indY[possibNbrs] = (j-1)%P.BoxY;
        possibNbrs++;
    }
    if (D[0](i,j+1).cellType == 0)
    {
        indX[possibNbrs] = i;
        indY[possibNbrs] = (j+1)%P.BoxY;
        possibNbrs++;
    }
    if (possibNbrs>=1)
    {
        int r = rand()%possibNbrs;
#ifdef DEBUG
        checkWeight(i,j,"Error within diffuseCell prev pos pre\n");
        checkWeight(indX[r],indY[r],"Error within diffuseCell next pos pre\n");
#endif
        D[0](indX[r],indY[r]).cellType = type;
        D[0](i,j).cellType = 0;
        if (type==1)
        {
            double dk = D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3;
#ifdef MUTUALISM
            dk += D[0](i,j).k8;
#endif
            D[0](i,j).k1 = 0;
            D[0](i,j).k2 = 0;
            D[0](i,j).k3 = 0;
#ifdef MUTUALISM
            D[0](i,j).k8 = 0;
#endif
#ifdef DEBUG
            checkWeightBeforeChange(i,j,-dk,"Error in line 703 Lattice.cpp before updateWeight\n");
#endif
            updateWeight(i, j, -dk);
            double lam = P.maxGrowthRate1;
#ifdef TOXICITY
            lam = lam/(1+D[0](indX[r],indY[r]).nutrientB/P.toxicityB);
#endif
#ifdef MUTUALISM
            lam = lam*D[0](indX[r],indY[r]).nutrientA/(P.monodConstantA+D[0](indX[r],indY[r]).nutrientA);
#endif
            D[0](indX[r],indY[r]).k1 += lam;
            D[0](indX[r],indY[r]).k2 += P.excretionRateB;
            D[0](indX[r],indY[r]).k3 += P.diffusion1/P.BoxLength/P.BoxLength;
#ifdef MUTUALISM
            D[0](indX[r],indY[r]).k8 += lam/P.yieldA;
#endif
            dk = D[0](indX[r],indY[r]).k1+D[0](indX[r],indY[r]).k2+D[0](indX[r],indY[r]).k3;
#ifdef MUTUALISM
            dk += D[0](indX[r],indY[r]).k8;
#endif
#ifdef DEBUG
            checkWeightBeforeChange(indX[r],indY[r],dk,"Error in line 724 Lattice.cpp before updateWeight\n");
#endif
            updateWeight(indX[r],indY[r], dk);
        }
        else if (type==2)
        {
            double dk = D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6;
#ifdef MUTUALISM
            dk += D[0](i,j).k9;
#endif
            D[0](i,j).k4 = 0;
            D[0](i,j).k5 = 0;
            D[0](i,j).k6 = 0;
#ifdef MUTUALISM
            D[0](i,j).k9 = 0;
#endif
#ifdef DEBUG
            checkWeightBeforeChange(i,j,-dk,"Error in line 676 Lattice.cpp before updateWeight\n");
#endif
            updateWeight(i, j, -dk);
#if defined(MUTUALISM) && defined(TOXICITY)
            double lam = P.maxGrowthRate2*D[0](indX[r],indY[r]).nutrientB/(P.monodConstantB+D[0](indX[r],indY[r]).nutrientB)/(1+D[0](indX[r],indY[r]).nutrientA/P.toxicityA);
#else
            double lam = P.maxGrowthRate2*D[0](indX[r],indY[r]).nutrientB/(P.monodConstantB+D[0](indX[r],indY[r]).nutrientB);
#endif
            D[0](indX[r],indY[r]).k4 = lam;
            D[0](indX[r],indY[r]).k5 = D[0](indX[r],indY[r]).k4/P.yieldB;
            D[0](indX[r],indY[r]).k6 = P.diffusion2/P.BoxLength/P.BoxLength;
#ifdef MUTUALISM
            D[0](indX[r],indY[r]).k9 = P.excretionRateA;
#endif
            dk = D[0](indX[r],indY[r]).k4+D[0](indX[r],indY[r]).k5+D[0](indX[r],indY[r]).k6;
#ifdef MUTUALISM
            dk += D[0](indX[r],indY[r]).k9;
#endif
#ifdef DEBUG
            checkWeightBeforeChange(indX[r],indY[r],dk,"Error in line 701 Lattice.cpp before updateWeight\n");
#endif
            updateWeight(indX[r],indY[r], dk);
        }
        minX = min(minX,indX[r]);
        maxX = max(maxX,indX[r]);
        minY = min(minY,indY[r]);
        maxY = max(minY,indY[r]);
#ifdef DEBUG
        checkWeight(i,j,"Error within diffuseCell prev pos post\n");
        checkWeight(indX[r],indY[r],"Error within diffuseCell next pos post\n");
#endif
    }
};

double lattice::timeIncrease(double t)
{
    double r2 = ((double) rand()+1) / ((double) RAND_MAX+1);
    double dt = -log(r2)/totalEventWeight; //R2 RANDPM NUMBER IT CAN EQUAL TO ZERO? 
    return t+dt;
};

void lattice::simulation(int numOutput)
{
    double t = 0;
    int outputFlag = 1;
    int outputID = 0;
    long numEvents[11] = {0,0,0,0,0,0,0,0,0,0,0};
    long totalNumEvents = 0;
    double dtOutput = P.t_max/numOutput;
    double timeOutput = 0;
    calculateEventRate();
    // printf("Consol output records the following: Output step; time; num events; total weight; CellType1 Num; CellType2 Num; Total Cell Num\n");
    // int cell1Prev = 0;
    // int cell2Prev = 0;
    do {
        eventInfo info = findCoordOfEvent();
        numEvents[updateEvent(info)]++;
        totalNumEvents++;
        t = timeIncrease(t);
        if (t>timeOutput)
        {
            outputFlag = 1;
            timeOutput += dtOutput;
        }
        if (outputFlag)
        {
			writeToFile(outputID);
            // int cell1Num, cell2Num, totalCellNum;
            // countCells(cell1Num, cell2Num);
            // if (cell1Prev == cell1Num || cell2Prev == cell2Num){
            //     cout << "stop growing" << endl;
            //     break;
            // } else {
            // cell1Prev = cell1Num;
            // cell2Prev = cell2Num;
            // }
            
//            printf("%d %.4e %ld %ld %.4e %d %d %d\n", outputID, t, totalNumEvents, LONG_MAX, totalEventWeight, cell1Num, cell2Num, totalCellNum);
//            printf("%d %.4e %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %.4e %d %d %d\n", outputID, t, numEvents[0],numEvents[1],numEvents[2],numEvents[3],numEvents[4],numEvents[5],numEvents[6],numEvents[7],numEvents[8],numEvents[9],numEvents[10], totalEventWeight, cell1Num, cell2Num, totalCellNum);//CELLS INFO
            cout << "Working on simulation " << outputID << endl;
            outputID++;
            outputFlag = 0;
        }
        if (minX<=1 || minY<=1 || maxX>=P.BoxX-2 || maxY>=P.BoxY-2 )
        {
            cout << "Colony reaches domain boundary" << endl;
            break;
        }
    } while (t<P.t_max);
};

void lattice::writeToFile(int OutputID)
{
    char fileName[500];
    FILE* out;
#ifdef _WIN32
    strcpy_s(fileName, P.DirName); 
#else
    strcpy(fileName, P.DirName);
#endif
    mkDir(fileName);
#ifdef _WIN32
   sprintf_s(fileName,"%s/%d.txt",P.DirName,OutputID);
   fopen_s(&out, fileName, "w"); 
#else
   sprintf(fileName, "%s/%d.txt", P.DirName, OutputID);
   out = fopen(fileName, "w");
#endif
    
    D->Output(out);
//    CDF->Append(out);
//    Del->Append(out);
    fclose(out);
};

int lattice::countCells(int& CellNum1, int& CellNum2)
{
    CellNum1 = 0;
    CellNum2 = 0;
    int totalCellNum = 0;
    for (int i=0; i<P.BoxX; i++)
    {
        for (int j=0; j<P.BoxY; j++)
        {
            if (D[0](i,j).cellType==1)
            {
                CellNum1++;
                totalCellNum++;
            }
            else if (D[0](i,j).cellType==2)
            {
                CellNum2++;
                totalCellNum++;
            }
        }
    }
    return totalCellNum;
};

#ifdef DEBUG
int lattice::checkWeight(int i, int j, const char errorMessage[100])
{
#ifdef MUTUALISM
    if (fabs(D[0](i,j).k0 - (D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3+D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6+D[0](i,j).k7+D[0](i,j).k8+D[0](i,j).k9+D[0](i,j).k10))>1e-10)
    {
        printf("%s\n",errorMessage);
        assert(0);
    }
#else
    if (fabs(D[0](i,j).k0 - (D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3+D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6+D[0](i,j).k7))>1e-10)
    {
        printf("%s\n",errorMessage);
        assert(0);
    }
#endif
    return 0;
};

int lattice::checkWeightBeforeChange(int i, int j, double dk, const char errorMessage[100])
{
#ifdef MUTUALISM
    if (fabs(D[0](i,j).k0+dk - (D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3+D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6+D[0](i,j).k7+D[0](i,j).k8+D[0](i,j).k9+D[0](i,j).k10))>1e-10)
    {
        printf("%s\n",errorMessage);
        assert(0);
    }
#else
    if (fabs(D[0](i,j).k0+dk - (D[0](i,j).k1+D[0](i,j).k2+D[0](i,j).k3+D[0](i,j).k4+D[0](i,j).k5+D[0](i,j).k6+D[0](i,j).k7))>1e-10)
    {
        printf("%s\n",errorMessage);
        return 2;
        assert(0);
    }
#endif
    return 0;
};

#endif

extern lattice L;
