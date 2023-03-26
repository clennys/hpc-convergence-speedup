CXX		= mpicxx
# CXXFLAGS= -pedantic-errors -Wall -Wextra
LDFLAGS	=
BUILD	= ./build
OBJ_DIR	= $(BUILD)/objects
APP_DIR	= $(BUILD)/apps
TARGET	= main
INCLUDE	= -Iinclude/
SRC		= \
	$(wildcard src/*.cc) \
	$(wildcard src/inputs/*.cc) \
	$(wildcard src/helpers/*.cc) \
	$(wildcard src/iterative_solvers/*.cc) \


OBJECTS	= $(SRC:%.cc=$(OBJ_DIR)/%.o)
DEPENDENCIES \
	:= $(OBJECTS:.o=.d)
MPI	:= \ /usr/include/openmpi-x86_64/mpi.h

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES) $(MPI)

.PHONY: all build clean debug release info optimized

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

optimized: CXXFLAGS += -O3
optimized: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

bear:
	@bear -- make

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
