# Useful directories
# the path to Geo's C++ utility library source code
GCD := ../gclib

## adjust as needed: path to FOX install prefix directory 
FOXPREFIX = ${HOME}/sw


FOXINCDIR := ${FOXPREFIX}/include/fox-1.7
FOXLIBDIR := ${FOXPREFIX}/lib

# Directories to search for header files
INCDIRS := -I. -I${GCD} -I${FOXINCDIR} \
 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

SYSTYPE :=     $(shell uname)

# C compiler

BASEOPTS  = -Wall ${INCDIRS} -D_FILE_OFFSET_BITS=64 \
-D_LARGEFILE_SOURCE -D_REENTRANT -fno-strict-aliasing

# C++ compiler
CXX      := g++

ifeq ($(findstring debug,$(MAKECMDGOALS)),)
  CXXFLAGS = -O2 -Wall -DNDEBUG -D_NDEBUG $(BASEOPTS)
  LDFLAGS = -L${FOXLIBDIR}
else
  CXXFLAGS = -g -Wall -DDEBUG -D_DEBUG $(BASEOPTS)
  LDFLAGS = -g -L${FOXLIBDIR}
endif
FOXPKG := ${FOXPREFIX}/lib/pkgconfig/fox17.pc
X_BASE_LIBS := $(shell pkg-config --variable=X_BASE_LIBS ${FOXPKG})
LIBS := $(shell pkg-config --variable=LIBS ${FOXPKG})

# C/C++ linker
LINKER    := g++
ifeq ($(findstring static,$(MAKECMDGOALS)),)  
   LIBS := -lFOX-1.7 ${X_BASE_LIBS} ${LIBS}
else
LIBS :=  -Wl,-Bstatic -lFOX-1.7 -Wl,-Bdynamic ${X_BASE_LIBS} ${LIBS}
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

OBJS = ./appmain.o ./mdichild.o ${GCD}/GBase.o LayoutParser.o AceParser.o ./mainwin.o ./FXClView.o

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



