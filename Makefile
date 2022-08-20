# Makefile
# Tobit Flatscher - github.com/2b-t (2020)

# Compiler settings: ICC or GCC (alternatively: 'export COMPILER=ICC' in console)
COMPILER = GCC

# Define sub-directories to be included in compilation
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
INCLUDES = $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
PROGRAM	 = main.$(COMPILER)

# Compiler flags
WARNINGS  = -Wall -pedantic -Wextra -Weffc++ -Woverloaded-virtual -Wfloat-equal -Wshadow -Wredundant-decls -Winline -fmax-errors=1
CXXFLAGS += -std=c++20 -O3 -flto -funroll-all-loops -finline-functions -mavx2 -mavx512f -march=cascadelake -I/usr/local/include/ -DNDEBUG
LDFLAGS  += -O3 -flto -I/usr/local/include/

# Compiler settings for specific compiler
ifeq ($(COMPILER),ICC)
        # Intel compiler ICC
	CXX       = icpc
	LD        = icpc
	CXXFLAGS += -fopenmp
	LDFLAGS  += -qopenmp
	COMPILER  = ICC
else
        # Gnu compiler GCC
	CXX       = /usr/local/Cellar/gcc/12.1.0/bin/gcc-12
	LD        = /usr/local/Cellar/gcc/12.1.0/bin/g++-12
	CXXFLAGS += -fopenmp
	LDFLAGS  += -lgomp
	COMPILER  = GCC
endif

# Make commands
default: $(BINDIR)/$(PROGRAM)

$(BINDIR)/$(PROGRAM): $(OBJECTS)
	@mkdir -p $(@D)
	$(LD)  $(OBJECTS)  $(LDFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	@rm -f $(BINDIR)/$(PROGRAM) $(OBJECTS)

run: clean $(BINDIR)/$(PROGRAM)
	./$(BINDIR)/$(PROGRAM)

doc:
	doxygen Doxyfile

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<

info:
	@echo "Compiler version: "
	$(CXX) --version
	@echo "Compiler flags: "
	@echo $(CXXFLAGS)
	@echo "Linker flags: "
	@echo $(LDFLAGS)
