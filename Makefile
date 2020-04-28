# Makefile
# Tobit Flatscher - github.com/2b-t (2020)

# Compiler settings (alternatively: export COMPILER=)
COMPILER = GCC

# compiler settings
ifeq ($(COMPILER),ICC)
	# Intel compiler ICC
	CXX        = icpc
	LINKER     = icpc
	CXXFLAGS  += -qopenmp
	LINKFLAGS += -qopenmp
	COMPILER   = ICC
else
	# Gnu Compiler GCC
	CXX        = g++
	LINKER     = g++
	CXXFLAGS  += -fopenmp
	LINKFLAGS += -lgomp
	COMPILER   = GCC
endif

# Define source files to be compiled
SOURCES  = $(wildcard *.cpp)
INCLUDES = $(wildcard *.hpp)
OBJECTS  = $(SOURCES:.cpp=.o)
PROGRAM	 = main.$(COMPILER)

# compiler flags
WARNINGS   = -Wall -pedantic -Wextra -Weffc++ -Woverloaded-virtual  -Wfloat-equal -Wshadow -Wredundant-decls -Winline -fmax-errors=1
CXXFLAGS  += -O3 -std=c++17 -funroll-all-loops -finline-functions -flto -mavx2 -march=native -DNDEBUG
LINKFLAGS += -O3

# Make commands
default: $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(LINKER)  $^  $(LINKFLAGS) -o $@

clean:
	@rm -f $(PROGRAM) $(OBJECTS)

run: clean $(PROGRAM)
	./$(PROGRAM)

doc:
	doxygen Doxyfile

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<

info:
	@echo "Show optimization flags"
	$(CXX) --version
	@echo $(CXXFLAGS)
