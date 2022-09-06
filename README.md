# instruction 
#### Compile options:
1. Compile the code for Commensalism in release mode
    ```bash
    g++ *.cpp -O3
    ```
2. Compile the code for Commensalism in debug mode
    ```bash
    g++ *.cpp -D DEBUG
    ```
3. Compile the code for Syntrophy in release mode
    ```bash
    g++ *.cpp -O3 -D TOXICITY
    ```
4. Compile the code for Syntrophy in debug mode
    ```bash
    g++ *.cpp -D DEBUG -D TOXICITY
    ```
1. Compile the code for Mutualism in release mode
    ```bash
    g++ *.cpp -O3 -D TOXICITY -D MUTUALISM
    ```
2. Compile the code for Mutualism in debug mode
    ```bash
    g++ *.cpp -D DEBUG -D TOXICITY -D MUTUALISM
    ```
3. Compile the code for Mutualism with Toxicity in release mode
    ```bash
    g++ *.cpp -O3 -D MUTUALISM -D TOXICITY
    ```
4. Compile the code for Mutualism with Toxicity in debug mode
    ```bash
    g++ *.cpp -D DEBUG -D TOXICITY -D MUTUALISM
    ```

## Usage
#### To run the code:
```bash
./a.out in1.txt 100
```
* `in1.txt` is the name of the input file, 100 the number of files you would like the program to output.

* All the parameters are included in the input file, and can be changed.

* The name of the input file can also be changed.

