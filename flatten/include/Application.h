/*
 * Spectral Conformal Parameterization
 * Patrick Mullen, Yiying Tong, Pierre Alliez, Mathieu Desbrun.
 * Symposium of Geometry Processing, 2008.
 */

#ifndef DDG_APPLICATION_H
#define DDG_APPLICATION_H

#include "Mesh.h"
#include "Complex.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "DiscreteExteriorCalculus.h"

namespace DDG
{
   class Application
   {
   public:
      void run(Mesh& mesh)
      {
         if (mesh.boundaries.empty())
         {
            std::cout << "Mesh has no boundary" << std::endl;
            return;
         }
         // double initial_area = mesh.area();
         // std::cout << "area: " << initial_area << std::endl;

         // create matrix for Lc
         const int V = mesh.vertices.size();
         SparseMatrix<Complex> Lc(V,V);

         // build energy
         buildEnergy(mesh, Lc);

         // compute the solution;
         DenseMatrix<Complex> x(V,1);
         x.randomize();

         SparseMatrix<Complex> star0;
         HodgeStar0Form<Complex>::build(mesh, star0);

         Lc += Complex(1E-8) * star0;

         smallestEigPositiveDefinite<Complex>(Lc, star0, x);

         // then assign the solution
         assignSolution(x, mesh);
         
         // rescale mesh for display convenience
         double scale = std::sqrt( mesh.area() / mesh.area2D() );
         normalizeMesh(scale, mesh);
      }
      
   protected:
      void buildEnergy(const Mesh& mesh, SparseMatrix<Complex>& A) const
      {
         const int V = mesh.vertices.size();
         // initialize a sparse VxV complex matrix
         A = SparseMatrix<Complex>(V,V);

         // loop through all half-edges to get Delta/2 part
         for ( std::vector<HalfEdge>::const_iterator it = mesh.halfedges.cbegin(); \
         it != mesh.halfedges.cend(); it++ ) {
            if ( it->onBoundary ) continue;

            Complex half_half_cot = Complex(it->cotan() / 4.);
            int i = it->vertex->index;
            int j = it->next->vertex->index;
            A(i,i) += half_half_cot;
            A(j,j) += half_half_cot;
            A(i,j) -= half_half_cot;
            A(j,i) -= half_half_cot;
         }

         /*SparseMatrix<Complex> d0, star1;

         ExteriorDerivative0Form<Complex>::build( mesh, d0 );
         HodgeStar1Form<Complex>::build( mesh, star1 );
         A = d0.transpose() * star1 * d0;*/

         // for each boundary face
         for ( std::vector<Face>::const_iterator it = mesh.boundaries.cbegin(); \
         it != mesh.boundaries.cend(); it++ ) {
            // loop through all half-edges to get A(z) part
            HalfEdgeIter he = it->he;
            do {
               int i = he->vertex->index;
               int j = he->next->vertex->index;
               A(i,j) -= Complex(0.,1/4.);
               A(j,i) += Complex(0.,1/4.);
               he = he->next;
            } while( he != it->he );
         }
      }
      
      void assignSolution(const DenseMatrix<Complex>& x, Mesh& mesh)
      {
         for( VertexIter v = mesh.vertices.begin();
             v != mesh.vertices.end();
             v ++ )
         {
            int i = v->index;
            const Complex& z = x(i,0);
            v->texture.x = z.re;
            v->texture.y = z.im;
            v->texture.z = 0.0;
         }
      }
      
      void normalizeMesh(const double scale, Mesh& mesh)
      {
         Vector avg;
         for( VertexIter v = mesh.vertices.begin();
             v != mesh.vertices.end();
             v ++ )
         {
            avg += v->texture;
         }
         avg /= mesh.vertices.size();
         
         for( VertexIter v = mesh.vertices.begin();
             v != mesh.vertices.end();
             v ++ )
         {
            v->texture = scale*(v->texture - avg);
         }
      }
   };
}

#endif
