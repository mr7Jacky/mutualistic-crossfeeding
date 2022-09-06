# Compile options:
# 1. Compile the code for Commensalism in release mode
#    g++ *.cpp -O3
# 2. Compile the code for Commensalism in debug mode
#    g++ *.cpp -D DEBUG
# 3. Compile the code for Syntrophy in release mode
#    g++ *.cpp -O3 -D TOXICITY
# 4. Compile the code for Syntrophy in debug mode
#    g++ *.cpp -D DEBUG -D TOXICITY
# 5. Compile the code for Mutualism in release mode
#    g++ *.cpp -O3 -D TOXICITY -D MUTUALISM
# 6. Compile the code for Mutualism in debug mode
#    g++ *.cpp -D DEBUG -D TOXICITY -D MUTUALISM #do we include tox?
# 7. Compile the code for Mutualism with Toxicity in release mode
#    g++ *.cpp -O3 -D MUTUALISM -D TOXICITY
# 8. Compile the code for Mutualism with Toxicity in debug mode
#    g++ *.cpp -D DEBUG -D TOXICITY -D MUTUALISM

G = g++ -I ./include

SRC = ./src
BIN = ./bin
OBJ = ./obj
INC = ./include

vpath %.h ./include
vpath %.cpp ./src

DEBUG = -D DEBUG
TOX = -D TOXICITY
MUTUAL = -D MUTUALISM

C = $(SRC)/*.cpp
LIB = $(INC)/Array.h $(INC)/ClockIt.h $(INC)/Lattice.h $(INC)/tools.h

EXEC = commensalism syntrophy mutualism mutualism-tox
EXECD = commensalism-d syntrophy-d mutualism-d mutualism-tox-d

all: $(EXEC)
all-d: $(EXECD)

commensalism: $(LIB) $(C)
	$(G) $(C) -O3 -o $(BIN)/commensalism
commensalism-d: $(LIB) $(C)
	$(G) $(C) $(DEBUG) -o $(BIN)/commensalism

syntrophy: $(LIB) $(C)
	$(G) $(C) -O3 $(TOX) -o $(BIN)/syntrophy
syntrophy-d: $(LIB) $(C)
	$(G) $(C) $(TOX) $(DEBUG) -o $(BIN)/syntrophy

mutualism: $(LIB) $(C)
	$(G) $(C) -O3 $(MUTUAL) -o $(BIN)/mutualism
mutualism-d: $(LIB) $(O) $(C)
	$(G) $(C) $(MUTUAL) $(DEBUG) -o $(BIN)/mutualism

mutualism-tox: $(LIB) $(C)
	$(G) $(C) -O3 $(MUTUAL) $(TOX) -o $(BIN)/mutualism-tox
mutualism-tox-d: $(LIB) $(C)
	$(G) $(C) $(TOX) $(MUTUAL) $(DEBUG) -o $(BIN)/mutualism-tox

