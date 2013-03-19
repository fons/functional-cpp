GCC          = g++ 
CLANG        = clang++
CC           = $(GCC)
CFLAGS       = -c -Wall -std=c++11
LDFLAGS      =

OBJDIR       = ../obj
EXEDIR       = ../bin
EXE          = main

SOURCES      = $(wildcard *.cpp)
INCLUDES     = $(wildcard *.hpp)

OBJECTS      = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
EXECUTABLE   = $(addprefix $(EXEDIR)/, $(EXE))
MKTARGETDIR  = @mkdir -p $(@D)

#$(info $(OBJECTS))
#$(info $(INCLUDES))

all : $(SOURCES) $(EXECUTABLE) $(INCLUDES)

$(EXECUTABLE): $(OBJECTS) $(INCLUDES) 
		$(MKTARGETDIR)
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cpp $(INCLUDES) 
		$(MKTARGETDIR)
		$(CC) $(CFLAGS) $< -o $@

#$(OBJDIR) :
#	mkdir $@

#$(EXEDIR) :
#	mkdir $@


clean:
	rm -f $(OBJECTS) $(EXEDIR)/core $(EXECUTABLE)

realclean:
	rm -f $(OBJECTS) $(EXEDIR)/core $(EXECUTABLE)
	rmdir $(OBJDIR)
	rmdir $(EXEDIR)

