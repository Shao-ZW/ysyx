TOPNAME = top
VERILATOR = verilator
VERILATOR_CFLAGS += --build -cc --exe -x-assign fast --trace #-Wall

WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build
INC_PATH := $(WORK_DIR)/include $(INC_PATH)

OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
VCD_FILE = $(BUILD_DIR)/wave.vcd

$(shell mkdir -p $(BUILD_DIR))

VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.cc" -or -name "*.cpp")
INCLUDES = $(addprefix -I, $(INC_PATH))
CFLAGS  := -O2 $(INCLUDES) $(CFLAGS)

override ARGS ?= --log=$(BUILD_DIR)/npc-log.txt

# Command to execute NPC
IMG ?=
NPC_SIM := $(BIN) $(ARGS) $(IMG)

$(BIN): $(VSRCS) $(CSRCS)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
	-CFLAGS $(CFLAGS)\
	--top-module $(TOPNAME) $^ \
	--Mdir $(OBJ_DIR) -o $(abspath $(BIN))

sim: $(BIN)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	$(NPC_SIM)

vcd: $(BIN)
	$(BIN)
	gtkwave $(VCD_FILE)

clean:
	rm -rf $(BUILD_DIR)

include ../Makefile

.PHONY: sim vcd clean