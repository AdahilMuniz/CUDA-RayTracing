APP := raytracing

SRC_DIR := src
OBJ_DIR := obj

SRC_CU := $(wildcard $(SRC_DIR)/*.cu) 
OBJ_CU := $(SRC_CU:$(SRC_DIR)/%.cu=$(OBJ_DIR)/%.o)

SRC_CPP := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_CPP := $(SRC_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#SRC += $(SRC_CPP)
#OBJ += $(OBJ_CPP)

NC := nvcc
CPP := g++

INCLUDE := -Iinclude
CUFLAGS := -dc

.PHONY: all clean

all: $(APP)

#$(APP): $(OBJ_CU) $(OBJ_CPP)
#	$(NC) -lSDL2 $^ -o $@

$(APP): $(OBJ_CU) $(OBJ_CPP)
	$(NC) -lcudart -lSDL2 $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cu | $(OBJ_DIR)
	$(NC) -lSDL2 $(INCLUDE) $(CUFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CPP) $(INCLUDE) -lSDL2 -c $< -o $@

$(OBJ_DIR):
	mkdir $@

run:
	./$(APP)

clean:
	rm -rf $(OBJ_DIR) $(APP) result.ppm