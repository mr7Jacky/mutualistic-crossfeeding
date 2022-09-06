
# Cross-feeding

#### Article

#### DOI

#### Files
* [bin/](./mutualistic-crossfeeding/bin)
  * [commensalism](./mutualistic-crossfeeding/bin/commensalism)
  * [mutualism](./mutualistic-crossfeeding/bin/mutualism)
  * [mutualism-tox](./mutualistic-crossfeeding/bin/mutualism-tox)
  * [syntrophy](./mutualistic-crossfeeding/bin/syntrophy)
* [include/](./mutualistic-crossfeeding/include)
  * [Array.h](./mutualistic-crossfeeding/include/Array.h)
  * [ClockIt.h](./mutualistic-crossfeeding/include/ClockIt.h)
  * [Lattice.h](./mutualistic-crossfeeding/include/Lattice.h)
  * [tools.h](./mutualistic-crossfeeding/include/tools.h)
* [params/](./mutualistic-crossfeeding/params)
  * [default_param.txt](./mutualistic-crossfeeding/params/default_param.txt)
* [src/](./mutualistic-crossfeeding/src)
  * [Lattice.cpp](./mutualistic-crossfeeding/src/Lattice.cpp)
  * [main.cpp](./mutualistic-crossfeeding/src/main.cpp)
  * [tools.cpp](./mutualistic-crossfeeding/src/tools.cpp)
* [LICENSE](./mutualistic-crossfeeding/LICENSE)
* [Makefile](./mutualistic-crossfeeding/Makefile)
* [README.md](./mutualistic-crossfeeding/README.md)

#### Usage
1. Compiling the simulator:
```bash
make <model-type>
```
There are four types of model types offered:
```
1. commensalism
2. syntrophy
3. mutualism
4. mutualism-tox
```
To access the debug mode add `-d` at the end of each type, as following:
```
1. commensalism-d
2. syntrophy-d
3. mutualism-d
4. mutualism-tox-d
```
Note that all compiled simulators are output to the subfolder `bin/` with the same name as the `<model-type>`, access [here](./bin/)

2. To start the simulation, use the following shell command:
```bash
./bin/<model-type> <parameter> <number-output-files> <output-directory>
```
Parameter description:
| Parameter Name          | Description                                           |
| :---------------------- | :---------------------------------------------------- |
| `<model-type>`          | Type of the model used                                |
| `<parameter>`           | Directory to parameter files                          |
| `<number-output-files>` | Number of output files, having the same time interval |ß
| `<output-directory>`    | Directory stores all output files                     |

3. To access the output files, go to the `<output-directory>` specified in previous step. Note that:
   1. All output files are three $n\times n$ matrices representing lattices of the colony and nutrient profiles.
      1. Colony are integers: 0 represents empty, 1 and 2 represent two types of species
      2. Nutrient are floating numbers representing the amount of nutrient produced in each lattice.
   2. Output files are in the format of `.txt` file, which can be easily accessed using common programming language.
