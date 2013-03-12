GCC        = g++ 
CLANG      = clang++
CC         = $(GCC)
CFLAGS     = -c -Wall -std=c++11
LDFLAGS    =

OBJDIR     = ../obj
EXEDIR     = ../bin
EXE        = main

SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
INCLUDES   = $(wildcard *.hpp)
EXECUTABLE = $(addprefix $(EXEDIR)/, $(EXE))


#$(info $(OBJECTS))
#$(info $(INCLUDES))

all : $(SOURCES) $(EXECUTABLE) $(INCLUDES)

$(EXECUTABLE): $(OBJECTS) $(INCLUDES) $(EXEDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cpp $(INCLUDES) $(OBJDIR)
		$(CC) $(CFLAGS) $< -o $@

$(OBJDIR) :
	mkdir $@

$(EXEDIR) :
	mkdir $@


clean:
	rm -f $(OBJECTS) $(EXEDIR)/core $(EXECUTABLE)


realclean:
	rm -f $(OBJECTS) $(EXEDIR)/core $(EXECUTABLE)
	rmdir $(OBJDIR)
	rmdir $(EXEDIR)

