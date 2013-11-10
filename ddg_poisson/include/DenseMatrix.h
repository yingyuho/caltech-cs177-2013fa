// -----------------------------------------------------------------------------
// libDDG -- DenseMatrix.h
// -----------------------------------------------------------------------------
//
// DenseMatrix represents an m by n (real or complex) matrix where every
// entry -- including zero-valued entries -- is stored explicitly.  This
// class is most commonly used to represent dense vectors in sparse linear
// systems (i.e., the right hand side and the solution vector).
//
// A real or complex matrix is allocated via
//
//    DenseMatrix A( m, n );
//    DenseMatrix A( m, n, Complex );
//
// Matrix elements are then accessed using parenthesis, e.g.,
//
//    A(i,j) = 1;
//    A(i,j) += 2;
//    a = A(i,j);
//
// etc.
//
// DenseMatrix is interoperable with the SuiteSparse numerical linear algebra
// library.  In particular, dereferencing a DenseMatrix returns a cholmod_dense*
// which can be used by routines in SuiteSparse.  For basic operations, however,
// you should not need to access this pointer explicitly -- see the solve()
// method in SparseMatrix.h.
// 

#ifndef DDG_DENSEMATRIX_H
#define DDG_DENSEMATRIX_H

#include <cholmod.h>
#include "Types.h"

namespace DDG
{
   class DenseMatrix
   {
      public:
         DenseMatrix( int m = 0, int n = 0, int xtype = Real );
         // initialize an mxn matrix of doubles
         // xtype is either DDG::Real or DDG::Complex
         
         DenseMatrix( const DenseMatrix& A );
         // copy constructor

         const DenseMatrix& operator=( const DenseMatrix& A );
         // copies A
         
         ~DenseMatrix( void );
         // destructor

         int nRows( void ) const;
         // returns the number of rows

         int nColumns( void ) const;
         // returns the number of columns

         int length( void ) const;
         // returns the size of the largest dimension

         void zero( double rVal = 0., double iVal = 0. );
         // sets all elements to rVal+iVal*i

         double norm( void );
         // returns the maximum magnitude of any entry

         double& operator()( int row, int col );
         double  operator()( int row, int col ) const;
         double& r( int row, int col );
         double  r( int row, int col ) const;
         // access real part of element (row,col)
         // note: uses 0-based indexing

         double& i( int row, int col );
         double  i( int row, int col ) const;
         // access imaginary part of element (row,col)
         // note: uses 0-based indexing
         
         double& operator()( int index );
         double  operator()( int index ) const;
         // access real part of element ind of a vector
         // note: uses 0-based indexing
         
         double& r( int index );
         double  r( int index ) const;
         // access real part of element ind of a vector
         // note: uses 0-based indexing
         
         double& i( int index );
         double  i( int index ) const;
         // access imaginary part of element ind of a vector
         // note: uses 0-based indexing

         cholmod_dense* operator*( void );
         // returns pointer to underlying cholmod_dense data structure

         const DenseMatrix& operator=( cholmod_dense* A );
         // gets pointer to A; will deallocate A upon destruction

      protected:
         void initializeFromCopy( void );

         int m, n;
         int xtype;
         cholmod_dense* data;
         double* rData;
         double* iData;
   };
}

#endif
