#include <vector>
using namespace std;

#include "Vertex.h"
#include "Mesh.h"
#include "HalfEdge.h"

namespace DDG
{
   Vector Vertex::normalEquallyWeighted( void ) const
   // returns unit vertex normal using equal weights
   {
      Vector N;

      // TODO

      return N;
   }

   Vector Vertex::normalAreaWeighted( void ) const
   // returns unit vertex normal using face area weights
   {
      Vector N;

      // TODO

      return N;
   }

   Vector Vertex::normalAngleWeighted( void ) const
   // returns unit vertex normal using tip angle weights
   {
      Vector N;

      // TODO

      return N;
   }

   Vector Vertex::normalMeanCurvature( void ) const
   // returns unit mean curvature normal
   {
      Vector N;

      // TODO

      return N;
   }

   Vector Vertex::normalSphereInscribed( void ) const
   // returns unit sphere-inscribed normal
   {
      Vector N;

      // TODO

      return N;
   }

   Vector Vertex::normal( NormalScheme scheme ) const
   // returns the vertex normal
   {
      Vector N;

      // compute the normal direction based on the selected scheme
      switch( scheme )
      {
         case nsEquallyWeighted:
            N = normalEquallyWeighted();
            break;
         case nsAreaWeighted:
            N = normalAreaWeighted();
            break;
         case nsAngleWeighted:
            N = normalAngleWeighted();
            break;
         case nsMeanCurvature:
            N = normalMeanCurvature();
            break;
         case nsSphereInscribed:
            N = normalSphereInscribed();
            break;
         default:
            break;
      }

      return N;
   }

   vector<HalfEdge> isolated; // all isolated vertices point to isolated.begin()

   bool Vertex::isIsolated( void ) const
   // returns true if the vertex is not contained in any face or edge; false otherwise
   {
      return he == isolated.begin();
   }
}

