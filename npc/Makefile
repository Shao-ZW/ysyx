TOPNAME = top
VERILATOR = verilator
VERILATOR_CFLAGS += --build -cc --exe -x-assign fast --trace -Wall

BUILD_DIR = $(abspath ./build)
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
VCD_FILE = $(BUILD_DIR)/wave.vcd

$(shell mkdir -p $(BUILD_DIR))

VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.cc" -or -name "*.cpp")

override ARGS ?= --log=$(BUILD_DIR)/npc-log.txt

# Command to execute NPC
IMG ?=
NPC_SIM := $(BIN) $(ARGS) $(IMG)

$(BIN): $(VSRCS) $(CSRCS)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
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