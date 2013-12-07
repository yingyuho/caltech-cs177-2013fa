#######################################################################################################

# Specify library locations here (add or remove "#" marks to comment/uncomment lines for your platform)

# Mac OS X
DDG_INCLUDE_PATH      = -I/usr/local/opt/metis4/include
DDG_LIBRARY_PATH      = -L/usr/local/opt/metis4/lib
DDG_BLAS_LIBS         = -framework Accelerate
DDG_SUITESPARSE_LIBS  = -ltbb -lspqr -lmetis -lcholmod -lcolamd -lccolamd -lcamd -lamd -lm -lsuitesparseconfig
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

#######################################################################################################

TARGET = ddg
CC = g++
LD = g++
CFLAGS = -O3 -Wall -ansi -pedantic  $(DDG_INCLUDE_PATH) -I./include
LFLAGS = -O3 -Wall -Werror -pedantic $(DDG_LIBRARY_PATH)
LIBS = $(DDG_OPENGL_LIBS) $(DDG_SUITESPARSE_LIBS) $(DDG_BLAS_LIBS)
OBJS = obj/Camera.o obj/DenseMatrix.o obj/Edge.o obj/Face.o obj/HalfEdge.o obj/LinearContext.o obj/LinearEquation.o obj/LinearPolynomial.o obj/LinearSystem.o obj/Mesh.o obj/MeshIO.o obj/Quaternion.o obj/SparseMatrix.o obj/Variable.o obj/Vector.o obj/Vertex.o obj/Viewer.o obj/main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

obj/Camera.o: src/Camera.cpp include/Camera.h include/Quaternion.h include/Vector.h
	$(CC) $(CFLAGS) -c src/Camera.cpp -o obj/Camera.o

obj/DenseMatrix.o: src/DenseMatrix.cpp include/DenseMatrix.h include/Types.h include/LinearContext.h
	$(CC) $(CFLAGS) -c src/DenseMatrix.cpp -o obj/DenseMatrix.o

obj/Edge.o: src/Edge.cpp include/Edge.h include/Types.h include/Mesh.h include/HalfEdge.h include/Vector.h include/Vertex.h include/Edge.h include/Face.h
	$(CC) $(CFLAGS) -c src/Edge.cpp -o obj/Edge.o

obj/Face.o: src/Face.cpp include/Face.h include/Types.h include/Mesh.h include/HalfEdge.h include/Vector.h include/Vertex.h include/Edge.h include/Face.h include/Vector.h
	$(CC) $(CFLAGS) -c src/Face.cpp -o obj/Face.o

obj/HalfEdge.o: src/HalfEdge.cpp include/HalfEdge.h include/Vector.h include/Types.h include/Mesh.h include/HalfEdge.h include/Vertex.h include/Edge.h include/Face.h
	$(CC) $(CFLAGS) -c src/HalfEdge.cpp -o obj/HalfEdge.o

obj/LinearContext.o: src/LinearContext.cpp include/LinearContext.h
	$(CC) $(CFLAGS) -c src/LinearContext.cpp -o obj/LinearContext.o

obj/LinearEquation.o: src/LinearEquation.cpp include/LinearEquation.h include/LinearPolynomial.h include/Variable.h
	$(CC) $(CFLAGS) -c src/LinearEquation.cpp -o obj/LinearEquation.o

obj/LinearPolynomial.o: src/LinearPolynomial.cpp include/LinearPolynomial.h include/Variable.h include/Types.h
	$(CC) $(CFLAGS) -c src/LinearPolynomial.cpp -o obj/LinearPolynomial.o

obj/LinearSystem.o: src/LinearSystem.cpp include/LinearSystem.h include/LinearEquation.h include/LinearPolynomial.h include/Variable.h include/SparseMatrix.h include/Types.h include/DenseMatrix.h include/LinearContext.h include/SparseMatrix.h include/Types.h
	$(CC) $(CFLAGS) -c src/LinearSystem.cpp -o obj/LinearSystem.o

obj/Mesh.o: src/Mesh.cpp include/Mesh.h include/HalfEdge.h include/Vector.h include/Types.h include/Vertex.h include/Edge.h include/Face.h include/MeshIO.h
	$(CC) $(CFLAGS) -c src/Mesh.cpp -o obj/Mesh.o

obj/MeshIO.o: src/MeshIO.cpp include/MeshIO.h include/Mesh.h include/HalfEdge.h include/Vector.h include/Types.h include/Vertex.h include/Edge.h include/Face.h
	$(CC) $(CFLAGS) -c src/MeshIO.cpp -o obj/MeshIO.o

obj/Quaternion.o: src/Quaternion.cpp include/Quaternion.h include/Vector.h
	$(CC) $(CFLAGS) -c src/Quaternion.cpp -o obj/Quaternion.o

obj/SparseMatrix.o: src/SparseMatrix.cpp include/SparseMatrix.h include/Types.h include/DenseMatrix.h include/LinearContext.h
	$(CC) $(CFLAGS) -c src/SparseMatrix.cpp -o obj/SparseMatrix.o

obj/Variable.o: src/Variable.cpp include/Variable.h
	$(CC) $(CFLAGS) -c src/Variable.cpp -o obj/Variable.o

obj/Vector.o: src/Vector.cpp include/Vector.h
	$(CC) $(CFLAGS) -c src/Vector.cpp -o obj/Vector.o

obj/Vertex.o: src/Vertex.cpp include/Vertex.h include/Vector.h include/Types.h include/Mesh.h include/HalfEdge.h include/Vertex.h include/Edge.h include/Face.h include/HalfEdge.h
	$(CC) $(CFLAGS) -c src/Vertex.cpp -o obj/Vertex.o

obj/Viewer.o: src/Viewer.cpp include/Viewer.h include/Mesh.h include/HalfEdge.h include/Vector.h include/Types.h include/Vertex.h include/Edge.h include/Face.h include/Camera.h include/Quaternion.h
	$(CC) $(CFLAGS) -c src/Viewer.cpp -o obj/Viewer.o

obj/main.o: src/main.cpp include/Viewer.h include/Mesh.h include/HalfEdge.h include/Vector.h include/Types.h include/Vertex.h include/Edge.h include/Face.h include/Camera.h include/Quaternion.h
	$(CC) $(CFLAGS) -c src/main.cpp -o obj/main.o

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(TARGET).exe


