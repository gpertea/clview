# Useful directories
# the path to Geo's C++ utility library source code
GCD := ../gclib
SAMPREFIX = /ccb/sw
SAMINCDIR := ${SAMPREFIX}/include
#we'll include htslib/*.h files
SAMLIBDIR := ${SAMPREFIX}/lib

#this must be the path to FOX install prefix directory 
FOXPREFIX = /ccb/sw
FOXINCDIR := ${FOXPREFIX}/include/fox-1.7
FOXLIBDIR := ${FOXPREFIX}/lib

# Directories to search for header files
INCDIRS := -I. -I${GCD} -I${SAMINCDIR} -I${FOXINCDIR}

SYSTYPE :=     $(shell uname)

# C compiler

BASEOPTS  = -Wall ${INCDIRS} -D_FILE_OFFSET_BITS=64 \
-D_LARGEFILE_SOURCE -D_REENTRANT -fno-strict-aliasing

# C++ compiler
CXX      := g++

ifeq ($(findstring debug,$(MAKECMDGOALS)),)
  CXXFLAGS = -O2 -Wall -DNDEBUG -D_NDEBUG $(BASEOPTS)
  LDFLAGS = -L${SAMLIBDIR} -L${FOXLIBDIR}
else
  CXXFLAGS = -g -Wall -DDEBUG -D_DEBUG $(BASEOPTS)
  LDFLAGS = -g -L${SAMLIBDIR} -L${FOXLIBDIR}
endif

# C/C++ linker
LINKER    := g++
ifeq ($(findstring static,$(MAKECMDGOALS)),)
LIBS := -lhts -lFOX-1.7 -lm -lXext -lX11 -lXi -lXrender -lXfixes -lfontconfig -lXrandr -lXcursor -lpthread -lpng -lXft -ljpeg -lrt
else
LIBS :=  -Wl,-Bstatic -lhts -lFOX-1.7 -Wl,-Bdynamic \
 -lm -lXext -lX11 -lXi -lXrender -lXfixes -lfontconfig -lXrandr -lXcursor -lpthread -lpng -lXft -ljpeg -lrt
endif
%.o : %.c
	${CXX} ${CXXFLAGS} -c $< -o $@

%.o : %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

%.o : %.C
	${CXX} ${CXXFLAGS} -c $< -o $@

%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

%.o : %.cxx
	${CXX} ${CXXFLAGS} -c $< -o $@

OBJS = ./appmain.o ./mdichild.o ${GCD}/GBase.o ${GCD}/gdna.o ${GCD}/GapAssem.o LayoutParser.o AceParser.o SamParser.o mainwin.o FXClView.o

.PHONY : all
all:   clview
debug: all
static: all
clview: ${OBJS}
	${LINKER} ${LDFLAGS} -o $@ ${filter-out %.a %.so, $^} ${LIBS}

# target for removing all object files

.PHONY : clean
clean::
	@${RM} clview core* *.exe ${OBJS}



