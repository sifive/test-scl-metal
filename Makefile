# Copyright 2019 SiFive, Inc #
# SPDX-License-Identifier: Apache-2.0 #

PROGRAM ?= test-scl-metal

# ----------------------------------------------------------------------
# Common def
# ----------------------------------------------------------------------
override CURRENT_DIR := $(patsubst %/,%, $(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
override BUILD_DIRECTORY = $(CURRENT_DIR)/$(CONFIGURATION)/build

# ----------------------------------------------------------------------
# Add custom flags for libscl 
# ----------------------------------------------------------------------
SCL_SOURCE_PATH ?= ../../scl-metal
SCL_DIR = $(abspath $(SCL_SOURCE_PATH))
SCL_TEST_DIR = $(join $(abspath  $(SCL_DIR)),/tests)

# ----------------------------------------------------------------------
# Macro
# ----------------------------------------------------------------------
ifeq ($(VERBOSE),TRUE)
	HIDE := 
else
	HIDE := @
endif

# ----------------------------------------------------------------------
# Build rules
# ----------------------------------------------------------------------
	
$(PROGRAM):
	make -f Makefile -C $(SCL_TEST_DIR) $(PROGRAM) BUILD_DIRECTORY=$(BUILD_DIRECTORY)
	mv $(BUILD_DIRECTORY)/$(PROGRAM) $(CURRENT_DIR)/
	mv $(BUILD_DIRECTORY)/$(PROGRAM).map $(CURRENT_DIR)/

clean:
	rm -rf $(BUILD_DIRECTORY)
	rm -f $(PROGRAM) $(PROGRAM).hex
