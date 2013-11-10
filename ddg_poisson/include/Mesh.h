// -----------------------------------------------------------------------------
// libDDG -- Mesh.h
// -----------------------------------------------------------------------------
//
// Mesh represents a polygonal surface mesh using the halfedge data structure.
// It is essentially a large collection of disjoint vertices, edges, and faces
// that are ``glued together'' by halfedges which encode connectivity (see
// the documentation for an illustration).  By construction, the halfedge data
// structure cannot represent nonorientable surfaces or meshes with nonmanifold
// edges.
//
// Mesh elements are referenced using iterators -- common usage of these
// iterators is to either traverse an entire vector of mesh elements:
//
//    // visit all vertices
//    for( VertexIter i = vertices.begin(); i != vertices.end(); i++ )
//    {
//       //...
//    }
//
// or to perform a local traversal over the neighborhood of some mesh element:
//
//    // visit both halfedges of edge e
//    HalfEdgeIter he = e->he;
//    do
//    {
//       // ...
//
//       he = he->flip;
//    }
//    while( he != e->he );
//
// (See Types.h for an explicit definition of iterator types.)
//
// Meshes with boundary are handled by creating an additional face for each
// boundary loop (the method Face::isBoundary() determines whether a given
// face is a boundary loop).  Isolated vertices (i.e., vertiecs not contained
// in any edge or face) reference a dummy halfedge and can be checked via
// the method Vertex::isIsolated().
//

#ifndef DDG_MESH_H
#define DDG_MESH_H

#include <vector>
#include <string>

#include "HalfEdge.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "SparseMatrix.h"

namespace DDG
{
   class Mesh
   {
      public:
         Mesh( void );
         // constructs an empty mesh

         Mesh( const Mesh& mesh );
         // constructs a copy of mesh

         const Mesh& operator=( const Mesh& mesh );
         // copies mesh

         int read( const std::string& filename );
         // reads a mesh from a Wavefront OBJ file; return value is nonzero
         // only if there was an error

         int write( const std::string& filename ) const;
         // writes a mesh to a Wavefront OBJ file; return value is nonzero
         // only if there was an error

         bool reload( void );
         // reloads a mesh from disk using the most recent input filename
         
         void normalize( void );
         // centers around the origin and rescales to have unit radius

         void indexVertices( void );
         // assigns a unique integer to the "index" member of each vertex in the range 0, ..., |V|-1
         
         void buildLaplacian( void );
         // build the cotan-Laplace matrix L

         void solveScalarPoissonProblem( void );
         // solve L phi = rho where rho is a scalar density on vertices
         // determined by the value of Vertex::rho; the solution phi is
         // copied back to the vertex attribute Vertex::phi

         void buildFlowOperator( double h );
         // build the matrix A = I - hL where h is the time step and L is the Laplacian

         void computeImplicitMeanCurvatureFlow( double h );
         // integrate mean curvature flow for time t using a single implicit step

         std::vector<HalfEdge> halfedges;
         std::vector<Vertex>   vertices;
         std::vector<Edge>     edges;
         std::vector<Face>     faces;
         // storage for mesh elements

         SparseMatrix L;
         // cotan-Laplace matrix

         SparseMatrix A;
         // matrix used to compute implicit mean curvature flow

      protected:
         std::string inputFilename;
   };
}

#endif

