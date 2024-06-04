-include $(NPC_HOME)/../Makefile
include $(NPC_HOME)/scripts/build.mk

# override ARGS ?= --log=$(BUILD_DIR)/npc-log.txt
# override ARGS += $(ARGS_DIFF)

# Command to execute NPC
IMG ?=
NPC_SIM := $(BIN) $(ARGS) $(IMG)

sim: $(BIN)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	$(NPC_SIM)

vcd: $(BIN)
	$(BIN)
	gtkwave $(VCD_FILE)

clean:
	rm -rf $(BUILD_DIR)
	
.PHONY: sim vcd clean
