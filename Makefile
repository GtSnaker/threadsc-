
CFLAGS= -I./src -g -O3 -std=c++11 

SOURCE += main.cpp
SOURCE += mainCreadorMatrices.cpp
SOURCE += GeneradorMatrices.cpp

OBJ=$(join $(addsuffix obj/, $(dir $(SOURCE))), $(notdir $(SOURCE:.cpp=.o)))

EXE=generar multiplicar

SRCDIR=src

CXX=g++

all: obj/ $(OBJ) $(EXE)

multiplicar:
	$(CXX) obj/main.o obj/GeneradorMatrices.o $(CFLAGS) -o $@

generar:
	$(CXX) obj/mainCreadorMatrices.o obj/GeneradorMatrices.o $(CFLAGS) -o $@


obj/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@


# RUN_PARAMS=10 make run
run:
	./generar $(RUN_PARAMS)
	./multiplicar $(RUN_PARAMS)

obj/:
	mkdir obj

clean:
	@$(foreach exe, $(EXE), if [ -a $(exe) ]; then rm $(exe); fi;)
	if [ -a obj/ ]; then rm -rf obj/; fi;
	
