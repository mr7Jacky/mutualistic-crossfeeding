#include "tools.h"
#include <string.h>

int mkDir(const char *path)
{
#ifdef _WIN32
    return ::_mkdir(path);
#elif __unix__
    return ::mkdir(path, 0755);
#elif _POSIX_C_SOURCE
    return ::mkdir(path);
#else
    return ::mkdir(path, 0755);
#endif
};



int GetFileLen(FILE* myFile)
{
    fseek (myFile, 0, SEEK_END);
    int size = ftell(myFile);
    fseek(myFile, 0, SEEK_SET);
    return size;
};

char* GetNextString(char*& buffer)
{
    char* out = buffer;
    if (!*buffer) return NULL; // return on empty string
    while(! (*buffer == 0x0A || *buffer == 0x0D || *buffer == 0x00) ) // 0x0A and 0x0D
        buffer++; // skip forward until we find the start of the next line (10/13/0)
    if (*buffer) *buffer++ = 0; // if we ended on 10/13 end the string and move to the next char
    if(*buffer == 0x0A) buffer++;  // on windows skip the 10 after the 13
    
    return out;
};

param ReadParameters(char* fname, char* outDir)
{
    
#ifdef _WIN32
    FILE* FID;
    fopen_s(&FID,fname, "r");
#else
    FILE* FID = fopen(fname,"r");
    fopen(fname, "r");
#endif
    if (FID == NULL) {
        fprintf(stderr, "Can't open parameter file.\n");
        exit(1);
    }
    char* data_string;
    char var_name[100];
    char var_value[100];
    
    int fileLen = GetFileLen(FID);
    char* buffer = (char*) malloc(fileLen+1);
    fread(buffer, fileLen, 1, FID);
    buffer[fileLen] = 0;
    
    param p;
    
#ifdef _WIN32
    strcpy_s(p.DirName, outDir);
#else
    strcpy(p.DirName, outDir);
#endif  
    mkDir(p.DirName);

    while((data_string = GetNextString(buffer)))
    {

#ifdef _WIN32
        sscanf_s(data_string, "%s %s", var_name, (unsigned)_countof(var_name), var_value, (unsigned)_countof(var_value));

#else
        sscanf(data_string, "%s %s", var_name, var_value);

#endif
        
        if (strcmp(var_name,"maxGrowthRate1")==0)
            p.maxGrowthRate1 = atof(var_value);
        else if (strcmp(var_name,"maxGrowthRate2")==0)
            p.maxGrowthRate2 = atof(var_value);
        else if (strcmp(var_name,"excretionRateB")==0)
            p.excretionRateB = atof(var_value);
        else if (strcmp(var_name,"diffusion1")==0)
            p.diffusion1 = atof(var_value);
        else if (strcmp(var_name,"diffusion2")==0)
            p.diffusion2 = atof(var_value);
        else if (strcmp(var_name,"diffusionB")==0)
            p.diffusionB = atof(var_value);
        else if (strcmp(var_name,"yieldB")==0)
            p.yieldB = atof(var_value);
        else if (strcmp(var_name,"monodConstantB")==0)
            p.monodConstantB = atof(var_value);
        else if (strcmp(var_name,"t_max")==0)
            p.t_max = atof(var_value);
        else if (strcmp(var_name,"BoxLength")==0)
            p.BoxLength = atof(var_value);
        else if (strcmp(var_name,"BoxX")==0)
            p.BoxX = atoi(var_value);
        else if (strcmp(var_name,"BoxY")==0)
            p.BoxY = atoi(var_value);
        else if  (strcmp(var_name,"excretionRateA")==0)
        {
#ifdef MUTUALISM
            p.excretionRateA = atof(var_value);
#endif
        }
        else if (strcmp(var_name,"yieldA")==0)
        {
#ifdef MUTUALISM
            p.yieldA = atof(var_value);
#endif
        }
        else if (strcmp(var_name,"diffusionA")==0)
        {
#ifdef MUTUALISM
            p.diffusionA = atof(var_value);
#endif
        }
        else if (strcmp(var_name,"monodConstantA")==0)
        {
#ifdef MUTUALISM
            p.monodConstantA = atof(var_value);
#endif
        }
        else if (strcmp(var_name,"toxicityB")==0)
        {
#ifdef TOXICITY
            p.toxicityB = atof(var_value);
#endif
        }
        else if (strcmp(var_name,"toxicityA")==0)
        {
#if defined(MUTUALISM) && defined(TOXICITY)
            p.toxicityA = atof(var_value);
#endif
        }
        else
        {
            printf("Unknown parameter: %s \n", var_name);
            /*fflush(stdout);
             assert(false);
             exit(-1);*/
        }
    }
    fclose(FID);
    
    return p;
};
