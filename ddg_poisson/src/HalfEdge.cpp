#include "HalfEdge.h"
#include "Mesh.h"

namespace DDG
{
   double HalfEdge::cotan( void ) const
   // returns the cotangent of the angle opposite this half edge
   {
      HalfEdgeCIter he = this->next->next;
      
      // reference point
      const Vector& p = he->vertex->position;

      he = he->next;
      Vector v = he->vertex->position - p;
      he = he->next;
      Vector u = he->vertex->position - p;

      return dot( u, v ) / cross( u, v ).norm();
   }
}

