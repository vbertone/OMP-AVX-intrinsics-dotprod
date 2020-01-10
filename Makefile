# Choose compiler

#use GNU compiler collection
# COMPILER = GCC_
#use Intel compiler
COMPILER = ICC_
#alternatively from the shell
#export COMPILER=GCC_
#or
#make COMPILER=GCC_

# Define source files that must be compuled
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
PROGRAM	= main.${COMPILER}

CXXFLAGS  += -g
LINKFLAGS += -g

ifndef COMPILER
  COMPILER = GCC_
endif

# Include compiler settings file
include ${COMPILER}default.mk
