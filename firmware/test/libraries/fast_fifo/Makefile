# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

#We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANF = rm -rf
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

DEBUG ?= 0
BUILD_DIR=./.build
DEV_ROOT=../../..
UNITY_ROOT=$(DEV_ROOT)/test/Unity

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
#CFLAGS += -Wno-misleading-indentation
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
endif

TARGET_BASE=fast_fifo_test

TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

SRC_FILES=\
	fast_fifo_test.c \
	test_runners/fast_fifo_test_runner.c \
	$(UNITY_ROOT)/src/unity.c \
	$(DEV_ROOT)/libraries/fast_fifo/fifo.c

INC_DIRS=\
	-Isrc \
	-I$(UNITY_ROOT)/src \
	-I$(DEV_ROOT)/libraries/fast_fifo

SYMBOLS=

all: clean runner build run

build: $(SRC_FILES)
	@echo "BUILD TESTS"
	@mkdir -p $(BUILD_DIR)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(BUILD_DIR)/$(TARGET)
	@echo "" 

runner: fast_fifo_test.c
	@echo "GENERATE TEST RUNNERS"
	@mkdir -p test_runners
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb fast_fifo_test.c  test_runners/fast_fifo_test_runner.c
	@echo "" 

clean:
	@echo "CLEAN TESTS"
	$(CLEANF) $(BUILD_DIR)
	@echo "" 

run:
	@echo "RUN TESTS"
	$(BUILD_DIR)/$(TARGET)

ci: CFLAGS += -Werror
ci: default
