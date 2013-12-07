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
         double initial_area = mesh.area();

	 // TODO: create matrix for Lc
	 // TODO: build energy
	 // buildEnergy( mesh, Lc );
	 // TODO: compute the solution; 
	 // smallestEigPositiveDefinite(Lc, star0, x);
	 // TODO: then assign the solution
         // assignSolution(x, mesh); // see below
         
         // rescale mesh for display convenience
         double scale = std::sqrt( initial_area / mesh.area() );
         normalizeMesh(scale, mesh);
      }
      
   protected:
      void buildEnergy(const Mesh& mesh, SparseMatrix<Complex>& A) const
      {
	// TODO
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
