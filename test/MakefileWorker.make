# For basic info on this makefile, run
# make help
#
# This makefile should auto-detect all source code in the given directory structure,
# which is set up in makefile.
# It contains compiler options, flags, functions for detecting source code and libraries,
# and rules for compiling and linking source code, and running unit tests.
#
# When executing test code, all production code is compiled into a library.
# Test files can then override production files using gcc's link-time substitution.
#
# Support for MCU-specific code may not yet be supported.
# Dat hard to do.

ifndef DEBUG
	DEBUG=Y
endif

### Generate target name ###
TARGET=$(TARGET_DIR)/$(TARGET_NAME)



### Generate and set flags ###
##User-entered flags
# Production code
COMPILER_FLAGS=-Wall
INCLUDE_FLAGS=
LINKER_FLAGS=
LINKER_FLAGS=

#Flags for user's unit tests written under CppUTest framework
TEST_COMPILER_FLAGS=
TEST_INCLUDE_FLAGS=
TEST_LINKER_FLAGS=

#Flags for CppUTest framework's source code
#(not the user's unit tests; the test framework itself)
CPPUTEST_LINKER_FLAGS=


##Auto-generated flags
# Production code
COMPILER_FLAGS+=-c -MMD -MP
ifeq ($(DEBUG),Y)
	COMPILER_FLAGS+=-g
endif
INCLUDE_FLAGS=$(addprefix -I,$(INC_DIRS))
LINKER_FLAGS=$(addprefix -L,$(LIB_DIRS))
LINKER_FLAGS+=$(addprefix -l,$(LIB_LIST))

#Flags for user's unit tests written under CppUTest framework
ifeq ($DEBUG,Y)
	TEST_COMPILER_FLAGS+=-g
endif
TEST_INCLUDE_FLAGS+=$(addprefix -I,$(TEST_INC_DIR))
#Link to any other libraries utilized by user tests
TEST_LINKER_FLAGS+=$(addprefix -L,$(TEST_LIB_DIR))
TEST_LINKER_FLAGS+=$(addprefix -l,$(TEST_LIB_LIST))
#Link to production source code library is included as a prerequisite in rule for building TEST_TARGET
# TEST_LINKER_FLAGS+=$(addprefix -L,$(TEST_TARGET_DIR))
# TEST_LINKER_FLAGS+=$(addprefix -l,$(TARGET_NAME))

#Flags for CppUTest framework's source code
#(not the user's unit tests; the test framework itself)
CPPUTEST_LINKER_FLAGS+=$(addprefix -l,$(CPPUTEST_LIB_LIST))
ifeq ("$(OSTYPE)","Cygwin")
CPPUTEST_LINKER_FLAGS+=$(addprefix -L,$(CPPUTEST_LIB_DIR))
endif

#Flags for archive tool
ifdef SILENCE
	ARCHIVER_FLAGS=rcs
else
	ARCHIVER_FLAGS=rcvs
endif



### Auto-detect source code and generate object files ###
# Production source code
SRC=$(call get_src_from_dir_list,$(SRC_DIRS))
CLEAN_SRC=$(call clean_path,$(SRC))
SRC_OBJ=$(addprefix $(OBJ_DIR)/,$(call src_to_o,$(CLEAN_SRC)))
SRC_DEP=$(addprefix $(OBJ_DIR)/,$(call src_to_d,$(CLEAN_SRC)))
INC=$(call get_inc_from_dir_list,$(INC_DIRS))
LIBS=$(addprefix lib,$(addsuffix .a,$(LIB_LIST)))

# Test code using CppUTest test harness
# User unit tests
TEST_TARGET=$(TEST_TARGET_DIR)/$(TEST_TARGET_NAME)_test
#Production code is compiled into a library
PRODUCTION_LIB=$(PRODUCTION_LIB_DIR)/$(addsuffix .a,$(addprefix lib,$(TARGET_NAME)))

TEST_SRC=$(call get_src_from_dir_list,$(TEST_SRC_DIRS))
CLEAN_TEST_SRC=$(call clean_path,$(TEST_SRC))
TEST_OBJ=$(addprefix $(TEST_OBJ_DIR)/,$(call src_to_o,$(CLEAN_TEST_SRC)))
TEST_DEP=$(addprefix $(TEST_OBJ_DIR)/,$(call src_to_d,$(CLEAN_TEST_SRC)))
TEST_INC=$(call get_inc_from_dir,$(TEST_INC_DIR))
TEST_LIBS=$(addprefix lib,$(addsuffix .a,$(TEST_LIB_LIST)))

# CppUTest test harness source code
CPPUTEST_LIBS=$(addprefix lib,$(addsuffix .a,$(CPPUTEST_LIB_LIST)))

DEP_FILES=$(SRC_DEP) $(TEST_DEP)

### Helper functions ###
get_subdirs = $(patsubst %/,%,$(sort $(dir $(wildcard $1*/))))
get_src_from_dir = $(wildcard $1/*.c) $(wildcard $1/*.cpp)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
get_inc_from_dir = $(wildcard $1/*.h)
get_inc_from_dir_list = $(foreach dir, $1, $(call get_inc_from_dir,$(dir)))
remove_dotdot=$(patsubst ../%,%,$1)
remove_dot=$(patsubst ./%,%,$1)
#Hahaha, need to loop this for as many subdirectories as we're going to support
#There must be a better way ;)
clean_path=$(call remove_dot,$(call remove_dotdot,$(call remove_dotdot,$(call remove_dotdot,$1))))
#nest calls so we don't get a repetition of .c and .cpp files
src_to=$(patsubst %.c,%$1,$(patsubst %.cpp,%$1,$2))
src_to_o=$(call src_to,.o,$1)
src_to_d=$(call src_to,.d,$1)



#"test" echo; used for checking makefile parameters
ECHO=@echo -e
techo=$(ECHO) "${BoldPurple}  $1:${NoColor}"; echo $2; echo;

### Makefile targets ###
.PHONY: all rebuild run compile clean cleanp
.PHONY: test rtest cleant
.PHONY: dirlist filelist flags vars colortest help


all: test

rebuild: clean all

### Production code rules ###
run: $(TARGET)
	echo $(TARGET)
	$(ECHO) "\n${BoldYellow}Executing $(notdir $<)...${NoColor}"
	$(ECHO) "${DarkGray}Production${NoColor}"
	$(ECHO)
	@$(SILENCE)$(TARGET)
	$(ECHO) "\n\n${Green}...Execution finished!${NoColor}\n"

compile: $(TARGET)

$(TARGET): $(SRC_OBJ) $(MCU_OBJ)
	$(ECHO) "\n${Yellow}Linking $(notdir $@)...${NoColor}"
	$(ECHO) "${DarkGray}Production${NoColor}"
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(C_LINKER) $^ -o $@ $(LINKER_FLAGS)
	$(ECHO) "${Green}...Executable created!\n${NoColor}"

$(OBJ_DIR)/%.o: $(ROOT_DIR)/%.c
	$(ECHO) "\n${Yellow}Compiling $(notdir $<)...${NoColor}"
	$(ECHO) "${DarkGray}Production${NoColor}"
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(C_COMPILER) $(COMPILER_FLAGS) $< $(INCLUDE_FLAGS) $(MCU_INCLUDE_FLAGS) -o $@

clean:
	$(ECHO) "${Yellow}Cleaning project...${NoColor}"
	$(SILENCE)rm -rf $(TARGET_DIR)
	$(SILENCE)rm -rf $(OBJ_DIR)
	$(SILENCE)rm -rf $(PRODUCTION_LIB_DIR)
	$(SILENCE)rm -rf $(TEST_OBJ_DIR)
	$(SILENCE)rm -rf $(TEST_TARGET_DIR)
	$(ECHO) "${Green}...Clean finished!${NoColor}\n"


### Test code rules ###
test: $(TEST_TARGET)
	$(ECHO) "\n${BoldRed}Executing $(notdir $<)...${BoldBlue}"
	$(ECHO)
	$(SILENCE)$(TEST_TARGET)
	$(ECHO) "\n${BoldGreen}...Tests executed!${NoColor}\n"

rtest: clean test

# Be SURE to link the test objects before the source code library!!
# This is what enables link-time substitution
$(TEST_TARGET): $(TEST_OBJ) $(PRODUCTION_LIB)
	$(ECHO) "\n${Yellow}Linking $(notdir $@)...${NoColor}"
	$(ECHO) "${DarkGray}test${NoColor}"
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(CPP_LINKER) -o $@ $^ $(LINKER_FLAGS) $(TEST_LINKER_FLAGS) $(CPPUTEST_LINKER_FLAGS)

#Target source code library is placed in the test folder because the production build doesn't use it
$(PRODUCTION_LIB): $(SRC_OBJ)
	$(ECHO) "\n${Yellow}Archiving all production code into $(notdir $@)... ${NoColor}"
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $^

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(ECHO) "\n${Yellow}Compiling $(notdir $<)...${NoColor}"
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(C_COMPILER) $(COMPILER_FLAGS) $< -o $@ $(INCLUDE_FLAGS) $(TEST_INCLUDE_FLAGS)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(ECHO) "\n${Yellow}Compiling $(notdir $<)...${NoColor}"
	$(SILENCE)mkdir -p $(dir $@)
	$(ECHO) "${DarkGray}test${NoColor}"
	$(SILENCE)$(CPP_COMPILER) $(COMPILER_FLAGS) $< -o $@ $(INCLUDE_FLAGS) $(TEST_INCLUDE_FLAGS)

# MAKECMDGOALS is a special variable that is set by Make
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEP_FILES)
endif


### Targets for debugging this makefile ###
dirlist:
	$(ECHO) "\n${BoldCyan}Production code build results:"
	$(call techo,TARGET_DIR,$(TARGET_DIR))
	$(call techo,OBJ_DIR,$(OBJ_DIR))

	$(ECHO) "\n${BoldCyan}Production code:${NoColor}"
	$(call techo,SRC_DIRS,$(SRC_DIRS))
	$(call techo,INC_DIRS,$(INC_DIRS))
	$(call techo,LIB_DIRS,$(LIB_DIRS))

	$(ECHO) "\n${BoldCyan}Test code:${NoColor}"
	$(call techo,TEST_SRC_DIRS,$(TEST_SRC_DIRS))
	$(call techo,TEST_INC_DIR,$(TEST_INC_DIR))
	$(call techo,TEST_LIB_DIRS,$(TEST_LIB_DIRS))

	$(ECHO) "\n${BoldCyan}Test code build results:"
	$(call techo,TEST_OBJ_DIR,$(TEST_OBJ_DIR))
	$(call techo,TEST_TARGET_DIR,$(TEST_TARGET_DIR))
	$(call techo,PRODUCTION_LIB_DIR,$(PRODUCTION_LIB_DIR))

	$(ECHO) "\n${BoldCyan}CppUTest code:${NoColor}"
	$(ECHO) "${BoldRed}Learn how to detect if CppUTest is installed!"

filelist:
	$(ECHO) "\n${BoldCyan}Directory of MakefileWorker.make:${NoColor}"
	$(ECHO) "$(shell pwd)\n"

	$(call techo,TARGET,$(TARGET))

	$(ECHO) "\n${BoldCyan}All Dependencies:${NoColor}"
	$(call techo,DEP_FILES,$(DEP_FILES))

	$(ECHO) "\n${BoldCyan}Production code:${NoColor}"
	$(call techo,SRC,$(SRC))
	$(call techo,SRC_OBJ,$(SRC_OBJ))
	$(call techo,SRC_DEP,$(SRC_DEP))
	$(call techo,INC,$(INC))
	$(call techo,LIBS,$(LIBS))

	$(ECHO) "\n${BoldCyan}Test code:${NoColor}"
	$(call techo,PRODUCTION_LIB,$(PRODUCTION_LIB))
	$(call techo,TEST_TARGET,$(TEST_TARGET))
	$(call techo,TEST_SRC,$(TEST_SRC))
	$(call techo,TEST_OBJ,$(TEST_OBJ))
	$(call techo,TEST_DEP,$(TEST_DEP))
	$(call techo,TEST_INC,$(TEST_INC))
	$(call techo,TEST_LIBS,$(TEST_LIBS))

	$(ECHO) "\n${BoldCyan}CppUTest code:${NoColor}"
	$(call techo,CPPUTEST_LIBS,$(CPPUTEST_LIBS))

flags:
	$(ECHO) "\n${BoldCyan}Compiler and Linker${NoColor}"
	$(call techo,C_COMPILER,$(C_COMPILER))
	$(call techo,CPP_COMPILER,$(CPP_COMPILER))
	$(call techo,ARCHIVER,$(ARCHIVER))
	$(call techo,C_LINKER,$(C_LINKER))
	$(call techo,CPP_LINKER,$(CPP_LINKER))

	$(ECHO) "\n${BoldCyan}Production code${NoColor}"
	$(call techo,COMPILER_FLAGS,$(COMPILER_FLAGS))
	$(call techo,INCLUDE_FLAGS,$(INCLUDE_FLAGS))
	$(call techo,LINKER_FLAGS,$(LINKER_FLAGS))

	$(ECHO) "\n${BoldCyan}Test code${NoColor}"
	$(call techo,TEST_COMPILER_FLAGS,$(TEST_COMPILER_FLAGS))
	$(call techo,TEST_INCLUDE_FLAGS,$(TEST_INCLUDE_FLAGS))
	$(call techo,ARCHIVER_FLAGS,$(ARCHIVER_FLAGS))
	$(call techo,TEST_LINKER_FLAGS,$(TEST_LINKER_FLAGS))

	$(ECHO) "\n${BoldCyan}CppUTest code${NoColor}"
	$(call techo,CPPUTEST_LINKER_FLAGS,$(CPPUTEST_LINKER_FLAGS))

vars:
	$(ECHO) "\n${BoldCyan}Makefile variables${NoColor}"
	$(call techo,TEST_DIR,$(TEST_DIR))

help:
	$(ECHO) "${BoldCyan}Production code options:${NoColor}"
	$(ECHO) "all:\t\tCompile all updated production code"
	$(ECHO) "rebuild:\tClean and rebuild all production code"
	$(ECHO) "run:\t\tRun all production code, compiling if necessary"
	$(ECHO) "clean:\t\tClean all production (and test) code"
	$(ECHO)
	$(ECHO) "${BoldCyan}Test code options:${NoColor}"
	$(ECHO) "test:\t\tCompile all updated test code and run all tests"
	$(ECHO) "rtest:\tClean and recompile all test code, run all tests"
	$(ECHO) "clean:\t\tClean all test and production code"
	$(ECHO)
	$(ECHO) "${BoldCyan}Makefile debugging options:${NoColor}"
	$(ECHO) "dirlist:\tList all directories detected and used by the project"
	$(ECHO) "filelist:\tList all files detected and used by the project"
	$(ECHO) "flags:\t\tList all flags"
	$(ECHO) "colortest:\tEcho text in every color"
	$(ECHO) "help:\t\tThis"

colortest:
	$(ECHO) "${Blue}Blue${NC}\t\tdefault"
	$(ECHO) "${BoldBlue}BoldBlue${NC}\tdefault"
	$(ECHO) "${Gray}Gray${NC}\t\tdefault"
	$(ECHO) "${DarkGray}DarkGray${NC}\tdefault"
	$(ECHO) "${Green}Green${NC}\t\tdefault"
	$(ECHO) "${BoldGreen}BoldGreen${NC}\tdefault"
	$(ECHO) "${Cyan}Cyan${NC}\t\tdefault"
	$(ECHO) "${BoldCyan}BoldCyan${NC}\tdefault"
	$(ECHO) "${Red}Red${NC}\t\tdefault"
	$(ECHO) "${BoldRed}BoldRed${NC}\t\tdefault"
	$(ECHO) "${Purple}Purple${NC}\t\tdefault"
	$(ECHO) "${BoldPurple}BoldPurple${NC}\tdefault"
	$(ECHO) "${Yellow}Yellow${NC}\t\tdefault"
	$(ECHO) "${BoldYellow}BoldYellow${NC}\tdefault"
	$(ECHO) "${BoldWhite}BoldWhite${NC}\tdefault"
	$(ECHO) "${NoColor}NoColor${NC}\t\tdefault"

### Color codes ###
Blue       =\033[0;34m
BoldBlue   =\033[1;34m
Gray       =\033[0;37m
DarkGray   =\033[1;30m
Green      =\033[0;32m
BoldGreen  =\033[1;32m
Cyan       =\033[0;36m
BoldCyan   =\033[1;36m
Red        =\033[0;31m
BoldRed    =\033[1;31m
Purple     =\033[0;35m
BoldPurple =\033[1;35m
Yellow     =\033[0;33m
BoldYellow =\033[1;33m
BoldWhite  =\033[1;37m
NoColor    =\033[0;0m
NC         =\033[0;0m


### Documentation ###
# $@	the name of the target
# $<	the name of the first prerequisite
# $^	the names of all prerequisites separated by a space

# (look into these more)
# -MMD  Output a list of dependencies to a .d file
# -MP   Something fancy about creating a phony target for .h files that are moved/removed

#ar [-]p [mod] <archive> <members ...>
#p:
#r  Add files into archive with replacement
#[mod]:
#v  Verbose mode
#c  Create the archive if it does not already exist.
#s  Add an index to the archive. Helps with linking. Equivalent to ranlib [?].
#[Use nm-s to display this index]

# $(call <function>,<param>,<param>,...)
# make sure you put a comma directly after <function>. If you don't it won't work.

# $(foreach <varname>,<list>,<body>)

# $(wildcard <pattern ...>)
# is replaced with a space-separated list of names that match the pattern.

# $(patsubst <pattern>,<replacement>,<text_to_search>)

# $(addprefix <prefix>,<names_to_edit>)

# $(notdir <names ...>)
# extract the non-directory part of each filename

# mkdir -p
# make parent directories as needed

#Order from LPCXpresso
#{COMMAND} ${FLAGS} ${OUTPUT_FLAG} ${OUTPUT_PREFIX}${OUTPUT} ${INPUTS}

# get_subdirs
# Be sure to include a trailing '/' when calling get_subdirs!
# wildcard $1*/        returns all files and folders within the path $1
# dir                  limits this result to only directories
# sort                 sorts this list
# patsubst %/,%,<list>  removes an unneeded trailing '/'
