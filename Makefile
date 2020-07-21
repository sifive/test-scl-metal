# Copyright 2019 SiFive, Inc #
# SPDX-License-Identifier: Apache-2.0 #

PROGRAM ?= test-scl-metal

# ----------------------------------------------------------------------
# Common def
# ----------------------------------------------------------------------
override CURRENT_DIR := $(patsubst %/,%, $(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
override BUILD_DIRECTORY = $(CURRENT_DIR)/$(CONFIGURATION)/build
override SOURCE_DIR = $(CURRENT_DIR)

override SOURCE_DIRS := $(SOURCE_DIR)
override SOURCE_DIRS += $(SOURCE_DIR)/tests/scl/sha
override SOURCE_DIRS += $(SOURCE_DIR)/tests/scl/aes
override SOURCE_DIRS += $(SOURCE_DIR)/tests/api
override SOURCE_DIRS += $(SOURCE_DIR)/tests/api/bignumbers/software
override SOURCE_DIRS += $(SOURCE_DIR)/tests/api/sha/software
override SOURCE_DIRS += $(SOURCE_DIR)/tests/api/sha/hardware
override SOURCE_DIRS += $(SOURCE_DIR)/tests/api/aes/hardware
override SOURCE_DIRS += $(SOURCE_DIR)/tests/test_runners


override C_SOURCES = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.c))
					
override ASM_SOURCES = $(wildcard *.S)
override OBJS :=	$(subst $(SOURCE_DIR),$(BUILD_DIRECTORY),$(C_SOURCES:.c=.o)) \
					$(subst $(SOURCE_DIR),$(BUILD_DIRECTORY),$(ASM_SOURCES:.S=.o))

# ----------------------------------------------------------------------
# Add custom flags for libscl 
# ----------------------------------------------------------------------
SCL_SOURCE_PATH ?= ../../scl-metal
SCL_DIR = $(abspath $(SCL_SOURCE_PATH))
include $(SCL_DIR)/scripts/scl.mk

TEST_FLAGS_SCL := $(foreach dir,$(SCL_INCLUDES),-I $(dir))
override CFLAGS += $(foreach dir,$(SCL_INCLUDES),-I $(dir))

override LDLIBS += -lscl
override LDFLAGS += -L$(join $(abspath  $(BUILD_DIRECTORY)),/scl/lib)


# ----------------------------------------------------------------------
# Add variable for HCA
# ----------------------------------------------------------------------
export HCA_VERSION ?= 0.5

# ----------------------------------------------------------------------
# Add custom flags for test
# ----------------------------------------------------------------------
TEST_FW_METAL_DIR = $(abspath $(CURRENT_DIR)/../../test-framework-metal)
BUILD_DIRECTORY_TEST_FW = $(join $(abspath  $(BUILD_DIRECTORY)),/test-framework-metal)

# Unity
UNITY_DIR = $(join $(TEST_FW_METAL_DIR),/Unity)
BUILD_DIRECTORY_UNITY = $(join $(BUILD_DIRECTORY_TEST_FW),/Unity)

UNITY_INCLUDES = 	$(UNITY_DIR)/src \
					$(UNITY_DIR)/extras/fixture/src

override UNITY_SOURCE_DIRS = 	$(UNITY_DIR)/src \
								$(UNITY_DIR)/extras/fixture/src

override UNITY_C_SOURCES = $(foreach dir,$(UNITY_SOURCE_DIRS),$(wildcard $(dir)/*.c))

override OBJS += $(subst $(UNITY_DIR),$(BUILD_DIRECTORY_UNITY),$(UNITY_C_SOURCES:.c=.o))

override CFLAGS += $(foreach dir,$(UNITY_INCLUDES),-I $(dir))
override CFLAGS += -DUNITY_FIXTURE_NO_EXTRAS

# ----------------------------------------------------------------------
# Update LDLIBS
# ----------------------------------------------------------------------
FILTER_PATTERN = -Wl,--end-group
override LDLIBS := $(filter-out $(FILTER_PATTERN),$(LDLIBS)) -Wl,--end-group

# ----------------------------------------------------------------------
# Common def
# ----------------------------------------------------------------------
override INCLUDE_DIRS := 	$(CURRENT_DIR) \
							$(CURRENT_DIR)/tests/sha

override CFLAGS += $(foreach dir,$(INCLUDE_DIRS),-I $(dir))
override CFLAGS += -Wall -Wextra -fstack-protector-all

# override CFLAGS += -I $(CURRENT_DIR)
override ASFLAGS = $(CFLAGS)

override LDFLAGS  += -Wl,--defsym,__stack_size=0xC00

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

$(BUILD_DIRECTORY_UNITY)/%.o: $(UNITY_DIR)/%.c
	$(HIDE) mkdir -p $(dir $@)
	$(HIDE) $(CC) -c -o $@ $(CFLAGS) $(XCFLAGS) $<
	
$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIR)/%.c
	$(HIDE) mkdir -p $(dir $@)
	$(HIDE) $(CC) -c -o $@ $(CFLAGS) $(XCFLAGS) $<

$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIR)/%.S
	$(HIDE) mkdir -p $(dir $@)
	$(HIDE) $(CC) -c -o $@ $(ASFLAGS) $<

libscl.a: 
	make -f Makefile -C $(SCL_DIR) \
	BUILD_DIR=$(join $(abspath  $(BUILD_DIRECTORY)),/scl) \
	libscl.a \
	VERBOSE=$(VERBOSE)
	
$(PROGRAM): \
	libscl.a \
	$(OBJS)
	$(HIDE) $(CC) $(CFLAGS) $(XCFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

.PHONY : check-format
check-format:
	clang-format -i $(C_SOURCES)

clean::
	rm -rf $(BUILD_DIRECTORY)
	rm -f $(PROGRAM) $(PROGRAM).hex
