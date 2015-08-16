# Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE =
endif

TARGET_NAME=Spi
TEST_TARGET_NAME=Timer0_ATtiny861

### Directory structure ###
# List all production code folders
ROOT_DIR=../..
SRC_DIRS=$(ROOT_DIR)/src $(ROOT_DIR)/mockHw/avr
INC_DIRS=$(ROOT_DIR)/inc $(ROOT_DIR)/mockHw
LIB_DIRS=
#Static library names without lib prefix and .a suffix
LIB_LIST=
OBJ_DIR=$(ROOT_DIR)/obj
TARGET_DIR=$(ROOT_DIR)/build

# List all user unit test folders
#TEST_DIR is assumed to be a subdirectory of the root test directory
TEST_ROOT_DIR=$(ROOT_DIR)/test
TEST_DIR=.
TEST_SRC_DIRS=$(TEST_DIR)/src
TEST_INC_DIR=$(TEST_DIR)/inc
TEST_LIB_DIRS=$(TEST_DIR)/lib
#Static library names without lib prefix and .a suffix
TEST_LIB_LIST=
TEST_TARGET_DIR=$(TEST_DIR)/build
PRODUCTION_LIB_DIR=$(TEST_ROOT_DIR)/build
TEST_OBJ_DIR=$(TEST_DIR)/obj


# CppUTest test harness source code
CPPUTEST_LIB_LIST=CppUTest CppUTestExt
OSTYPE:=$(shell uname -o)
ifeq ("$(OSTYPE)","Cygwin")
# Cygwin's linker can't seem to find CppUTest libraries even though they're in the PATH...
CPPUTEST_LIB_DIR=/usr/local/lib
endif

### Compiler tools ###
C_COMPILER=gcc
C_LINKER=gcc
ARCHIVER=ar
CPP_COMPILER=g++
CPP_LINKER=g++


# Do the real work
include $(ROOT_DIR)/test/MakefileWorker.make
