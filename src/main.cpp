#include <cstdlib>
#include <signal.h>
#include <iostream>
//#include <unistd.h>

#include "Lattice.h"

#include "Array.h"

//
// #####################################
// ######## instruction ################
// #####################################
//
// Compile options:
// 1. Compile the code for Commensalism in release mode
//    g++ *.cpp -O3
// 2. Compile the code for Commensalism in debug mode
//    g++ *.cpp -D DEBUG
// 3. Compile the code for Syntrophy in release mode
//    g++ *.cpp -O3 -D TOXICITY
// 4. Compile the code for Syntrophy in debug mode
//    g++ *.cpp -D DEBUG -D TOXICITY
// 5. Compile the code for Mutualism in release mode
//    g++ *.cpp -O3 -D TOXICITY -D MUTUALISM
// 6. Compile the code for Mutualism in debug mode
//    g++ *.cpp -D DEBUG -D TOXICITY -D MUTUALISM
// 7. Compile the code for Mutualism with Toxicity in release mode
//    g++ *.cpp -O3 -D MUTUALISM -D TOXICITY
// 8. Compile the code for Mutualism with Toxicity in debug mode
//    g++ *.cpp -D DEBUG -D TOXICITY -D MUTUALISM
//
// #####################################
//
// To run the code:
//    ./a.out in1.txt 100 outDir
// in1.txt is the name of the input file, 100 the number of files you would like the program to output.
// All the parameters are included in the input file, and can be changed.
// The name of the input file can also be changed.
//

lattice * L;

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_handler(int signum) {
    string path = "fields_data.lattice";
    cout << "Exiting the program and saving the data to " << path << endl;
    L->saveData(path);

    // Terminate program
    exit(signum);
}

int main(int argc, char* argv[])
{
    if(argc < 4) {
        printf("Usage:\n[relations executable] [parameter file] [number rounds] [output directory]\n");
        exit(-1);
    }
#if defined(MUTUALISM) && defined(TOXICITY)
    printf("Mutualism with toxicity is selected.\n");
#elif defined(MUTUALISM)
    printf("Mutualism without toxicity is selected.\n");
#elif defined(TOXICITY)
    printf("Syntrophy is selected.\n");
#else
    printf("Commensalism is selected.\n");
#endif
    //signal(SIGINT, signal_handler);
    L = new lattice(argv[1],argv[3]);
    L->simulation(atoi(argv[2]));
    delete L;
    return 0;
}
