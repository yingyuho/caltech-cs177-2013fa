// -----------------------------------------------------------------------------
// libDDG -- SparseMatrix.h
// -----------------------------------------------------------------------------
//
// SparseMatrix represents an m by n (real or complex) matrix where only
// nonzero entries are stored explicitly.  This class is most commonly
// used to represent the linear term in sparse linear systems (i.e., the matrix
// part).
//
// A real or complex matrix is allocated via
//
//    SparseMatrix A( m, n );
//    SparseMatrix A( m, n, Complex );
//
// Matrix elements are then accessed using parenthesis, e.g.,
//
//    A(i,j) = 1;
//    A(i,j) += 2;
//    a = A(i,j);
//
// etc.
//
// SparseMatrix is interoperable with the SuiteSparse numerical linear algebra
// library.  In particular, dereferencing a SparseMatrix returns a
// cholmod_sparse* which can be used by routines in SuiteSparse.  For basic
// operations, however, you should not need to access this pointer explicitly --
// see the solve() method below.
//
// Internally SparseMatrix stores nonzero entries in a heap data structure; the
// amortized cost of insertion is therefore no worse than the sorting cost of
// putting the matrix in compressed-column order.
// 

#ifndef DDG_SPARSE_MATRIX_H
#define DDG_SPARSE_MATRIX_H

#include <cholmod.h>
#include <map>
#include "Types.h"

namespace DDG
{
   class SparseMatrix
   {
      public:
         SparseMatrix( int m = 0, int n = 0, int xtype = Real );
         // initialize an mxn matrix of doubles
         // xtype is either DDG::Real or DDG::Complex

         ~SparseMatrix( void );
         // destructor

         void resize( int m, int n );
         // clears and resizes to mxn matrix
         
         cholmod_sparse* operator*( void );
         // dereference operator gets pointer to underlying cholmod_sparse data structure

         int nRows( void ) const;
         // returns the number of rows

         int nColumns( void ) const;
         // returns the number of columns

         int length( void ) const;
         // returns the size of the largest dimension

         void zero( double rVal = 0., double iVal = 0. );
         // sets all nonzero elements to rVal+iVal*i
         
         void transpose( void );
         // replaces this matrix with its transpose

         void horzcat( const SparseMatrix& A, const SparseMatrix& B );
         // replaces the current matrix with [ A, B ]

         void vertcat( const SparseMatrix& A, const SparseMatrix& B );
         // replaces the current matrix with [ A; B ]

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
         
         typedef std::pair<int,int> EntryIndex;
         // convenience type for an entry index; note that we store column THEN
         // row, which makes it easier to build compressed column format

         typedef std::pair<double,double> EntryValue;
         // convenience type for a complex entry value (real,imaginary)
         
         typedef std::map<EntryIndex,EntryValue> EntryMap;
         typedef EntryMap::iterator       iterator;
         typedef EntryMap::const_iterator const_iterator;
         // convenience type for storing and accessing entries

               iterator begin( void );
         const_iterator begin( void ) const;
               iterator   end( void );
         const_iterator   end( void ) const;
         // return iterators to first and last nonzero entries

      protected:
         int m, n;
         int xtype, stype;
         cholmod_sparse* A;
         EntryMap data;

         double& retrieveEntry( int row, int col, int c );
         double  retrieveEntry( int row, int col, int c ) const;
   };

   void solve( SparseMatrix& A, DenseMatrix& x, DenseMatrix& b );
   // solves the sparse linear system  Ax = b
}

#endif
