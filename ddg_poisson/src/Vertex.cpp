#include <vector>
using namespace std;

#include "Vertex.h"
#include "Mesh.h"
#include "HalfEdge.h"

namespace DDG
{
   double Vertex::dualArea( void ) const
   // returns the area of the barycentric dual cell associated with this vertex
   {
      double three_area = 0.;
      HalfEdgeCIter he, heBegin;

      he = heBegin = this->he;
      do {
         three_area += he->face->area();
         he = he->flip->next;
      } while ( he != heBegin );

      return three_area / 3.;
   }

   Vector Vertex::normal( void ) const
   // returns the vertex normal
   {
      Vector N;

      HalfEdgeCIter h = he;
      do
      {
         N += h->face->normal();
         h = h->flip->next;
      }
      while( h != he );

      return N.unit();
   }

   vector<HalfEdge> isolated; // all isolated vertices point to isolated.begin()

   bool Vertex::isIsolated( void ) const
   // returns true if the vertex is not contained in any face or edge; false otherwise
   {
      return he == isolated.begin();
   }
}

