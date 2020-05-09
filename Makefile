# Name of the project
PROJ_NAME?=main

#Output Image:
OUT_IMG?=result

# Compiler
CPP=g++

# Include Directory
INC_DIR=./include/

# Source Directory
SRC_DIR=./src/


# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -O3        \
         -Wall      \


all: $(PROJ_NAME)
 
$(PROJ_NAME):
	$(CPP) -o $@ $(SRC_DIR)*.cpp -I$(INC_DIR)

run:
	./$(PROJ_NAME) >> $(OUT_IMG).ppm

clean:
	rm -rf *.o $(PROJ_NAME) *~