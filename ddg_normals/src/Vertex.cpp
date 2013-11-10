#include <vector>
#include <cmath>
using namespace std;

#include "Vertex.h"
#include "Mesh.h"
#include "HalfEdge.h"
#include "CyclicArray.h"

namespace DDG
{
   Vector Vertex::normalEquallyWeighted( void ) const
   // returns unit vertex normal using equal weights
   {
      Vector N(0.,0.,0.);

      HalfEdgeIter he = this->he;
      do {
         N += he->face->normal();
         
         // go to a half-edge of the next adjacent face
         he = he->flip->next;
      }
      while( he != this->he );

      return N.unit();
   }

   Vector Vertex::normalAreaWeighted( void ) const
   // returns unit vertex normal using face area weights
   {
      Vector N(0.,0.,0.);

      HalfEdgeIter he = this->he;
      do {
         FaceIter f = he->face;
         Vector area;
         HalfEdgeIter he2 = f->he;
         do {
            const Vector& vec1 = he2->vertex->position;
            const Vector& vec2 = he2->next->vertex->position;

            area += cross( vec2+vec1, vec2-vec1 );

            // go to the next half-edge of the same face
            he2 = he2->next;
         }
         while( he2 != f->he );
          
         N += area;
         
         // go to a half-edge of the next adjacent face
         he = he->flip->next;
      }
      while( he != this->he );

      return N.unit();
   }

   Vector Vertex::normalAngleWeighted( void ) const
   // returns unit vertex normal using tip angle weights
   {
      Vector N(0.,0.,0.);
      HalfEdgeIter heBegin, he;
      FaceIter face;

      const Vector& p_i = this->position;
      Vector vec1, vec2;

      he = this->he->flip;
      vec1 = (he->vertex->position - p_i).unit();
      face = he->face;
      he = heBegin = he->next->flip;
      do
      {
         vec2 = (he->vertex->position - p_i).unit();
         N += face->normal() * atan2( cross(vec1,vec2).norm(), -dot(vec1,vec2) );
         vec1 = vec2;
         face = he->face;

         he = he->next->flip;
      }
      while( he != heBegin );

      return N.unit();
   }

   Vector Vertex::normalMeanCurvature( void ) const
   // returns unit mean curvature normal
   {
      Vector N(0.,0.,0.);

      const Vector& p_i = this->position;
      HalfEdgeIter heBegin, he;
      int vertexDeg = 0;
      int j;

      // count vertex degree
      he = heBegin = this->he;
      do {
         vertexDeg++;
         he = he->flip->next;
      }
      while( he != heBegin );

      // get vectors from p_i to adjacent p_j's
      CyclicArray<Vector> p_j(vertexDeg);
      he = heBegin = this->he->flip;
      j = 0;
      do {
         p_j[j] = he->vertex->position - p_i;
         he = he->next->flip;
         j++;
      }
      while( he != heBegin );

      // compute gradient of area using cot formula
      for ( j = 0; j < vertexDeg; j++ ) {
         double cot_a = \
            dot( p_j[j-1]-p_j[j], p_j[j-1] ) / cross( p_j[j], p_j[j-1] ).norm();
         double cot_b = \
            dot( p_j[j+1]-p_j[j], p_j[j+1] ) / cross( p_j[j], p_j[j+1] ).norm();

         N -= (cot_a + cot_b) * p_j[j];
      }

      return N.unit();
   }

   Vector Vertex::normalSphereInscribed( void ) const
   // returns unit sphere-inscribed normal
   {
      Vector N(0.,0.,0.);
      HalfEdgeIter heBegin, he;

      Vector p_i = this->position;
      Vector vec1, vec2;

      he = this->he->flip;
      vec1 = he->vertex->position - p_i;
      he = heBegin = he->next->flip;
      do
      {
         vec2 = (he->vertex->position - p_i);
         N -= ( cross(vec1,vec2) ) / ( vec1.norm2() * vec2.norm2() );
         vec1 = vec2;

         he = he->next->flip;
      }
      while( he != heBegin );

      return N.unit();
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

