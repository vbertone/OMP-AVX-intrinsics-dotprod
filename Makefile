# Choose compiler

#use GNU compiler collection
# COMPILER=GCC_
#alternatively from the shell
#export COMPILER=GCC_
#or
#make COMPILER=GCC_

#use Intel compiler
COMPILER=ICC_

#use PGI compiler
# COMPILER=PGI_

# Define source files that must be compuled
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
PROGRAM	= main.${COMPILER}

#uncomment the next to lines for debugging and detailed performance analysis
CXXFLAGS += -g
LINKFLAGS += -g
#do not use -pg with PGI compilers

ifndef COMPILER
  COMPILER=  GCC_
endif

# Include compiler settings file
include ${COMPILER}default.mk
