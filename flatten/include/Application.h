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
      void run(Mesh& mesh);
      
   protected:
      void buildEnergy(const Mesh& mesh, SparseMatrix<Complex>& A) const;
      void assignSolution(const DenseMatrix<Complex>& x, Mesh& mesh);
      void normalizeMesh(const double scale, Mesh& mesh);

   };
}

#endif
