# BM 25 generator fiel

IDIR =include 
CXX=g++ 
CC=gcc
CXXFLAGS= -I$(IDIR) -I./lib  -std=c++17 -fpermissive -fopenmp -lstdc++fs -w -O3  #-D_GLIBCXX_PARALLEL

SRCDIR=src
TARGETDIR=bin

ODIR=build
TARGET=bin/bm
LDIR=-L../lib

LIBS=-lm 


_DEPS=  bm.h
	
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = bm.o main.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRCDIR)/%.cpp 
	@mkdir -p $(ODIR)
	@mkdir -p $(TARGETDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
$(ODIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(ODIR)
	@mkdir -p $(TARGETDIR)
	$(CC) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJ)
	@echo "\n\n\n Linking ...." 
	g++ -o $@ $^ $(CXXFLAGS) $(LIBS) 


.PHONY: clean

clean:
	@echo " Cleaning ....";
	rm -rf $(ODIR) bin/fp