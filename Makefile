APP := raytracing

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.cu)
OBJ := $(SRC:$(SRC_DIR)/%.cu=$(OBJ_DIR)/%.o)

NC := nvcc

CPPFLAGS := -Iinclude
CFLAGS   := -dc

.PHONY: all clean

all: $(APP)

$(APP): $(OBJ)
	$(NC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cu | $(OBJ_DIR)
	$(NC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

run:
	./$(APP)

clean:
	rm -rf $(OBJ) $(APP) result.ppm