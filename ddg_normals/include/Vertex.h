// -----------------------------------------------------------------------------
// libDDG -- Vertex.h
// -----------------------------------------------------------------------------
//
// Vertex stores attributes associated with a mesh edge.  The iterator he
// points to its "outgoing" halfedge.  (See the documentation for a more
// in-depth discussion of the halfedge data structure.)
// 

#ifndef DDG_VERTEX_H
#define DDG_VERTEX_H

#include "Vector.h"
#include "Types.h"

namespace DDG
{
   enum NormalScheme
   {
      nsEquallyWeighted,
      nsAreaWeighted,
      nsAngleWeighted,
      nsMeanCurvature,
      nsSphereInscribed
   };
   // used to enumerate the possible methods for computing vertex normals

   class Vertex
   {
      public:
         HalfEdgeIter he;
         // points to the "outgoing" halfedge

         Vector position;
         // location of vertex in Euclidean 3-space

         Vector normal( NormalScheme scheme = nsEquallyWeighted ) const;
         // returns unit vertex normal using the specified scheme

         bool isIsolated( void ) const;
         // returns true if the vertex is not contained in any face or edge; false otherwise

      protected:
         Vector normalEquallyWeighted( void ) const;
         // returns unit vertex normal using equal weights
         
         Vector normalAreaWeighted( void ) const;
         // returns unit vertex normal using face area weights
         
         Vector normalAngleWeighted( void ) const;
         // returns unit vertex normal using tip angle weights

         Vector normalMeanCurvature( void ) const;
         // returns unit mean curvature normal
         
         Vector normalSphereInscribed( void ) const;
         // returns unit sphere-inscribed normal
   };
}

#endif

