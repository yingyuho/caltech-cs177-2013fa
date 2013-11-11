#include "Face.h"
#include "Mesh.h"
#include "Vector.h"

namespace DDG
{
   double Face::area( void ) const
   {
      Vector four_area(0.,0.,0.);
      HalfEdgeCIter he, heBegin;

      he = heBegin = this->he;
      do {
         const Vector& vec1 = he->vertex->position;
         const Vector& vec2 = he->next->vertex->position;

         // 4 N_V = \int 2f \wedge df
         four_area += cross( vec2+vec1, vec2-vec1 );

         // go to the next half-edge of the same face
         he = he->next;
      }
      while( he != heBegin );

      return four_area.norm() / 4.;
   }

   Vector Face::normal( void ) const
   {
      Vector p0 = he->vertex->position;
      Vector p1 = he->next->vertex->position;
      Vector p2 = he->next->next->vertex->position;

      return cross( p1-p0, p2-p0 ).unit();
   }

   bool Face::isBoundary( void ) const
   {
      return he->onBoundary;
   }
}

