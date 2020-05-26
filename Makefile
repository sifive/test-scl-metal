# Copyright 2019 SiFive, Inc #
# SPDX-License-Identifier: Apache-2.0 #

PROGRAM ?= test-scl-metal

# ----------------------------------------------------------------------
# Common def
# ----------------------------------------------------------------------
override CURRENT_DIR := $(patsubst %/,%, $(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
override BUILD_DIRECTORY = $(CURRENT_DIR)/$(CONFIGURATION)/build
override SOURCE_DIR = $(CURRENT_DIR)

override C_SOURCES = $(wildcard *.c)
override ASM_SOURCES = $(wildcard *.S)
override OBJS := $(C_SOURCES:%.c=$(BUILD_DIRECTORY)/%.o) \
				 $(ASM_SOURCES:%.S=$(BUILD_DIRECTORY)/%.o)

override CFLAGS += -I $(CURRENT_DIR)
override ASFLAGS = $(CFLAGS)

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
# Add custom flags for libcmocka 
# ----------------------------------------------------------------------
CMOCKA_METAL_DIR = $(abspath $(CURRENT_DIR)/../../cmocka-metal)

SCL_INCLUDES = $(CMOCKA_METAL_DIR)/include
override CFLAGS += $(foreach dir,$(SCL_INCLUDES),-I $(dir))

override LDLIBS += -lcmocka
override LDFLAGS += -L$(join $(abspath  $(BUILD_DIRECTORY)),/cmocka/lib)

# ----------------------------------------------------------------------
# Update LDLIBS
# ----------------------------------------------------------------------
FILTER_PATTERN = -Wl,--end-group
override LDLIBS := $(filter-out $(FILTER_PATTERN),$(LDLIBS)) -Wl,--end-group

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

$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIR)/%.c
	$(info TEST_FLAGS_SCL:$(TEST_FLAGS_SCL))
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

libcmocka.a: 
	mkdir -p $(join $(abspath  $(BUILD_DIRECTORY)),/cmocka)
	cd $(join $(abspath  $(BUILD_DIRECTORY)),/cmocka) && \
		cmake $(CMOCKA_METAL_DIR) -DWITH_STATIC_LIB=true && \
		make
	mkdir -p $(join $(abspath  $(BUILD_DIRECTORY)),/cmocka/lib)
	cp $(join $(abspath  $(BUILD_DIRECTORY)),/cmocka/src/libcmocka-static.a) \
		 $(join $(abspath  $(BUILD_DIRECTORY)),/cmocka/lib/libcmocka.a)

$(PROGRAM): \
	libscl.a \
	libcmocka.a \
	$(OBJS)
	$(CC) $(CFLAGS) $(XCFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	@echo

clean::
	rm -rf $(BUILD_DIRECTORY)
	rm -f $(PROGRAM) $(PROGRAM).hex
