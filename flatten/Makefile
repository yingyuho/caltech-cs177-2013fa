##########################################################################################

# Specify library locations here (add or remove "#" marks to comment/uncomment lines for your platform)

# Mac OS X
DDG_INCLUDE_PATH      = -I/usr/local/include -I/usr/local/opt/metis4/include
DDG_LIBRARY_PATH      = -L/usr/local/lib -L/usr/local/opt/metis4/lib
DDG_BLAS_LIBS         = -framework Accelerate
DDG_SUITESPARSE_LIBS  = -ltbb -lspqr -lumfpack -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -lm -lsuitesparseconfig
DDG_OPENGL_LIBS       = -framework OpenGL -framework GLUT

# # Linux
# DDG_INCLUDE_PATH      =
# DDG_LIBRARY_PATH      =
# DDG_BLAS_LIBS         = -llapack -lblas -lgfortran
# DDG_SUITESPARSE_LIBS  = -lspqr -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -lm
# DDG_OPENGL_LIBS       = -lglut -lGL -lGLU -lX11

# # Windows / Cygwin
# DDG_INCLUDE_PATH      = -I/usr/include/opengl -I/usr/include/suitesparse
# DDG_LIBRARY_PATH      = -L/usr/lib/w32api -L/usr/lib/suitesparse
# DDG_BLAS_LIBS         = -llapack -lblas
# DDG_SUITESPARSE_LIBS  = -lspqr -lcholmod -lcolamd -lccolamd -lcamd -lamd -lm
# DDG_OPENGL_LIBS       = -lglut32 -lglu32 -lopengl32

########################################################################################

TARGET = flatten
CC = g++
LD = g++
CFLAGS = -O0 -Wall -Werror -Wno-error=deprecated-declarations -Wno-error=constant-logical-operand -ansi -pedantic  $(DDG_INCLUDE_PATH) -I./include -I./src
LFLAGS = -O0 -Wall -Werror -pedantic $(DDG_LIBRARY_PATH)
LIBS = $(DDG_OPENGL_LIBS) $(DDG_SUITESPARSE_LIBS) $(DDG_BLAS_LIBS)

########################################################################################
## !! Do not edit below this line

HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) -o $(TARGET) $(LFLAGS) $(LIBS)

obj/%.o: src/%.cpp ${HEADERS}
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f $(TARGET).exe
