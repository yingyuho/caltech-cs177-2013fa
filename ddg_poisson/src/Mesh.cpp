#include <map>
#include <fstream>
#include "Mesh.h"
#include "MeshIO.h"
#include "DenseMatrix.h"

using namespace std;

namespace DDG
{
   void Mesh :: indexVertices( void )
   // assigns a unique integer to the "index" member of each vertex in the range 0, ..., |V|-1
   {
      for ( int i = 0; i < vertices.size(); i++ ) {
         vertices[i].index = i;
      }
   }

   void Mesh :: buildLaplacian( void )
   // build the cotan-Laplace matrix L
   // L(0-form) = 2-form
   {
      // index again anyway for safety c.f. buildFlowOperator(double)
      indexVertices();
      
      const int V = vertices.size();
      // initialize a VxV matrix with zeroes
      L = SparseMatrix(V,V); L.zero();

      // loop through all half-edges
      for ( std::vector<HalfEdge>::const_iterator it = halfedges.begin(); \
         it != halfedges.end(); it++ ) {
         double half_cot = it->cotan() / 2.;
         int i = it->vertex->index;
         int j = it->next->vertex->index;
         L(i,i) -= half_cot;
         L(j,j) -= half_cot;
         L(i,j) += half_cot;
         L(j,i) += half_cot;
      }
   }

   void Mesh :: solveScalarPoissonProblem( void )
   // solve L phi = rho where rho is a scalar density on vertices
   // determined by the value of Vertex::rho; the solution phi is
   // copied back to the vertex attribute Vertex::phi
   {
      buildLaplacian();
      
      const int V = vertices.size();
      // rho is actually density integrated over dual areas of vertices
      // so it consists of 2-forms
      DenseMatrix rho_mat(V,1); rho_mat.zero();
      // phi is 0-form scalar potential
      DenseMatrix phi_mat(V,1); phi_mat.zero();

      // load masses into matrix
      for ( std::vector<Vertex>::const_iterator it = vertices.begin(); \
         it != vertices.end(); it++ ) {
         rho_mat(it->index,0) = it->rho;
      }

      solve( L, phi_mat, rho_mat );

      // unload potential from matrix
      for ( std::vector<Vertex>::iterator it = vertices.begin(); \
         it != vertices.end(); it++ ) {
         it->phi = phi_mat(it->index,0);// / it->dualArea();
      }
   }
   
   void Mesh :: buildFlowOperator( double h )
   // build the matrix A = I - hL where h is the time step and L is the Laplacian
   // A(0-form) = 0-form so take the dual of the result of cotan-Laplacian
   {
      // index again because mesh is reloaded by Viewer::mComputeFlow()
      indexVertices();

      const int V = vertices.size();

      // initialize a VxV matrix with zeroes
      A = SparseMatrix(V,V); A.zero();

      // fill the diagonal with 1
      for ( int i = 0; i < V; i++ )
         A(i,i) = 1.;

      // loop through all half-edges to add (-h)L
      for ( std::vector<HalfEdge>::const_iterator it = halfedges.begin(); \
         it != halfedges.end(); it++ ) {
         double half_cot = it->cotan() / 2.;
         int i = it->vertex->index;
         int j = it->next->vertex->index;
         // 1 / dualArea() converts 2-forms to 0-forms
         A(i,i) -= (-h) * half_cot / vertices[i].dualArea();
         A(j,j) -= (-h) * half_cot / vertices[j].dualArea();
         A(i,j) += (-h) * half_cot / vertices[i].dualArea();
         A(j,i) += (-h) * half_cot / vertices[j].dualArea();
      }
   }

   void Mesh :: computeImplicitMeanCurvatureFlow( double h )
   {
      buildFlowOperator(h);

      const int V = vertices.size();
      DenseMatrix f_0(V,3); f_0.zero();
      DenseMatrix f_h(V,3); f_h.zero();

      // load current coordinates
      for ( std::vector<Vertex>::const_iterator it = vertices.begin(); \
         it != vertices.end(); it++ ) {
         f_0(it->index,0) = it->position[0];
         f_0(it->index,1) = it->position[1];
         f_0(it->index,2) = it->position[2];
      }

      solve( A, f_h, f_0 );

      // unload new coordinates
      for ( std::vector<Vertex>::iterator it = vertices.begin(); \
         it != vertices.end(); it++ ) {
         it->position[0] = f_h(it->index,0);
         it->position[1] = f_h(it->index,1);
         it->position[2] = f_h(it->index,2);
      }
   }

   Mesh :: Mesh( void )
   {}
   
   Mesh :: Mesh( const Mesh& mesh )
   {
      *this = mesh;
   }
   
   class  HalfEdgeIterCompare { public: bool operator()( const  HalfEdgeIter& i, const  HalfEdgeIter& j ) const { return &*i < &*j; } };
   class HalfEdgeCIterCompare { public: bool operator()( const HalfEdgeCIter& i, const HalfEdgeCIter& j ) const { return &*i < &*j; } };
   class    VertexIterCompare { public: bool operator()( const    VertexIter& i, const    VertexIter& j ) const { return &*i < &*j; } };
   class   VertexCIterCompare { public: bool operator()( const   VertexCIter& i, const   VertexCIter& j ) const { return &*i < &*j; } };
   class      FaceIterCompare { public: bool operator()( const      FaceIter& i, const      FaceIter& j ) const { return &*i < &*j; } };
   class     FaceCIterCompare { public: bool operator()( const     FaceCIter& i, const     FaceCIter& j ) const { return &*i < &*j; } };
   class      EdgeIterCompare { public: bool operator()( const      EdgeIter& i, const      EdgeIter& j ) const { return &*i < &*j; } };
   class     EdgeCIterCompare { public: bool operator()( const     EdgeCIter& i, const     EdgeCIter& j ) const { return &*i < &*j; } };
   
   const Mesh& Mesh :: operator=( const Mesh& mesh )
   {
      map< HalfEdgeCIter, HalfEdgeIter, HalfEdgeCIterCompare > halfedgeOldToNew;
      map<   VertexCIter,   VertexIter,   VertexCIterCompare >   vertexOldToNew;
      map<     EdgeCIter,     EdgeIter,     EdgeCIterCompare >     edgeOldToNew;
      map<     FaceCIter,     FaceIter,     FaceCIterCompare >     faceOldToNew;
   
      // copy geometry from the original mesh and create a
      // map from pointers in the original mesh to
      // those in the new mesh
      halfedges.clear(); for( HalfEdgeCIter he = mesh.halfedges.begin(); he != mesh.halfedges.end(); he++ ) halfedgeOldToNew[ he ] = halfedges.insert( halfedges.end(), *he );
       vertices.clear(); for(   VertexCIter  v =  mesh.vertices.begin();  v !=  mesh.vertices.end();  v++ )   vertexOldToNew[ v  ] =  vertices.insert(  vertices.end(), *v  );
          edges.clear(); for(     EdgeCIter  e =     mesh.edges.begin();  e !=     mesh.edges.end();  e++ )     edgeOldToNew[ e  ] =     edges.insert(     edges.end(), *e  );
          faces.clear(); for(     FaceCIter  f =     mesh.faces.begin();  f !=     mesh.faces.end();  f++ )     faceOldToNew[ f  ] =     faces.insert(     faces.end(), *f  );
   
      // "search and replace" old pointers with new ones
      for( HalfEdgeIter he = halfedges.begin(); he != halfedges.end(); he++ )
      {
         he->next   = halfedgeOldToNew[ he->next   ];
         he->flip   = halfedgeOldToNew[ he->flip   ];
         he->vertex =   vertexOldToNew[ he->vertex ];
         he->edge   =     edgeOldToNew[ he->edge   ];
         he->face   =     faceOldToNew[ he->face   ];
      }
   
      for( VertexIter v = vertices.begin(); v != vertices.end(); v++ ) v->he = halfedgeOldToNew[ v->he ];
      for(   EdgeIter e =    edges.begin(); e !=    edges.end(); e++ ) e->he = halfedgeOldToNew[ e->he ];
      for(   FaceIter f =    faces.begin(); f !=    faces.end(); f++ ) f->he = halfedgeOldToNew[ f->he ];
   
      return *this;
   }

   int Mesh::read( const string& filename )
   {
      inputFilename = filename;
      ifstream in( filename.c_str() );

      if( !in.is_open() )
      {
         cerr << "Error reading from mesh file " << filename << endl;
         return 1;
      }

      int rval;
      if( !( rval = MeshIO::read( in, *this )))
      {
         normalize();
      }
      return rval;
   }

   int Mesh::write( const string& filename ) const
   // reads a mesh from a Wavefront OBJ file; return value is nonzero
   // only if there was an error
   {
      ofstream out( filename.c_str() );

      if( !out.is_open() )
      {
         cerr << "Error writing to mesh file " << filename << endl;
         return 1;
      }

      MeshIO::write( out, *this );

      return 0;
   }

   bool Mesh::reload( void )
   {
      return read( inputFilename );
   }

   void Mesh::normalize( void )
   {
      // compute center of mass
      Vector c( 0., 0., 0. );
      for( VertexCIter v = vertices.begin(); v != vertices.end(); v++ )
      {
         c += v->position;
      }
      c /= (double) vertices.size();

      // translate to origin
      for( VertexIter v = vertices.begin(); v != vertices.end(); v++ )
      {
         v->position -= c;
      }

      // rescale such that the mesh sits inside the unit ball
      double rMax = 0.;
      for( VertexCIter v = vertices.begin(); v != vertices.end(); v++ )
      {
         rMax = max( rMax, v->position.norm() );
      }
      for( VertexIter v = vertices.begin(); v != vertices.end(); v++ )
      {
         v->position /= rMax;
      }
   }
}

