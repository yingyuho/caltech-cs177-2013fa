#include <algorithm>
#include <cassert>
#include <cmath>

#include <SuiteSparseQR.hpp>

#include "SparseMatrix.h"
#include "DenseMatrix.h"
#include "LinearContext.h"

using namespace std;

namespace DDG
{
   extern LinearContext context;

   SparseMatrix :: SparseMatrix( int m_, int n_, int xtype_ )
   : m( m_ ),
     n( n_ ),
     xtype( xtype_ ),
     stype( 0 ),
     A( NULL )
   {}

   SparseMatrix :: ~SparseMatrix( void )
   {
      if( A )
      {
         cholmod_l_free_sparse( &A, context );
      }
   }

   void SparseMatrix :: resize( int m_, int n_ )
   {
      m = m_;
      n = n_;

      data.clear();
   }

   int SparseMatrix :: nRows( void ) const
   {
      return m;
   }

   int SparseMatrix :: nColumns( void ) const
   {
      return n;
   }

   int SparseMatrix :: length( void ) const
   {
      return max( m, n );
   }

   void SparseMatrix :: zero( double rVal, double iVal )
   {
      EntryValue val( rVal, iVal );

      for( EntryMap::iterator i = data.begin(); i != data.end(); i++ )
      {
         i->second = val;
      }
   }

   void SparseMatrix :: transpose( void )
   {
      EntryMap transposed;

      for( const_iterator e = data.begin(); e != data.end(); e++ )
      {
         int i = e->first.first;
         int j = e->first.second;
         transposed[ EntryIndex( j, i ) ] = e->second;
      }

      data = transposed;
      swap( m, n );
   }

   void SparseMatrix :: horzcat( const SparseMatrix& A, const SparseMatrix& B )
   {
      assert( A.m == B.m );
      assert( A.xtype == B.xtype );

      data.clear();
      m = A.m;
      n = A.n + B.n;
      xtype = A.xtype;

      for( const_iterator e = A.begin(); e != A.end(); e++ )
      {
         int i = e->first.second;
         int j = e->first.first;
         data[ EntryIndex( j, i ) ] = e->second;
      }

      for( const_iterator e = B.begin(); e != B.end(); e++ )
      {
         int i = e->first.second;
         int j = e->first.first;
         data[ EntryIndex( j+A.n, i ) ] = e->second;
      }
   }

   void SparseMatrix :: vertcat( const SparseMatrix& A, const SparseMatrix& B )
   {
      assert( A.n == B.n );
      assert( A.xtype == B.xtype );

      data.clear();
      m = A.m + B.m;
      n = A.n;
      xtype = A.xtype;

      for( const_iterator e = A.begin(); e != A.end(); e++ )
      {
         int i = e->first.second;
         int j = e->first.first;
         data[ EntryIndex( j, i ) ] = e->second;
      }

      for( const_iterator e = B.begin(); e != B.end(); e++ )
      {
         int i = e->first.second;
         int j = e->first.first;
         data[ EntryIndex( j, i+A.m ) ] = e->second;
      }
   }

   cholmod_sparse* SparseMatrix :: operator*( void )
   {
      if( A )
      {
         cholmod_l_free_sparse( &A, context );
         A = NULL;
      }

      int nzmax = data.size();
      int sorted = true;
      int packed = true;
      A = cholmod_l_allocate_sparse( m, n, nzmax, sorted, packed, stype, xtype, context );

      // build compressed matrix (note that EntryMap stores entries in column-major order)
      double* pr = (double*) A->x;
      double* pi = (double*) A->z;
      UF_long* ir = (UF_long*) A->i;
      UF_long* jc = (UF_long*) A->p;
      int i = 0;
      int j = -1;
      for( EntryMap::const_iterator e = data.begin(); e != data.end(); e++ )
      {
         int c = e->first.first;
         if( c != j )
         {
            for( int k = j+1; k <= c; k++ )
            {
               jc[k] = i;
            }
            j = c;
         }

         ir[i] = e->first.second;
         pr[i] = e->second.first;
         if( xtype == Complex )
         {
            pi[i] = e->second.second;
         }
         i++;
      }
      for( int k = j+1; k <= n; k++ )
      {
         jc[k] = i;
      }

      return A;
   }

   double& SparseMatrix :: operator()( int row, int col )       { return retrieveEntry( row, col, Real    ); }
   double  SparseMatrix :: operator()( int row, int col ) const { return retrieveEntry( row, col, Real    ); }
   double& SparseMatrix ::          r( int row, int col )       { return retrieveEntry( row, col, Real    ); }
   double  SparseMatrix ::          r( int row, int col ) const { return retrieveEntry( row, col, Real    ); }
   double& SparseMatrix ::          i( int row, int col )       { return retrieveEntry( row, col, Complex ); }
   double  SparseMatrix ::          i( int row, int col ) const { return retrieveEntry( row, col, Complex ); }

   SparseMatrix::iterator SparseMatrix :: begin( void )
   {
      return data.begin();
   }

   SparseMatrix::const_iterator SparseMatrix :: begin( void ) const
   {
      return data.begin();
   }

   SparseMatrix::iterator SparseMatrix :: end( void )
   {
      return data.end();
   }

   SparseMatrix::const_iterator SparseMatrix :: end( void ) const
   {
      return data.end();
   }

   double& SparseMatrix :: retrieveEntry( int row, int col, int c )
   {
      EntryIndex index( col, row );

      EntryMap::const_iterator entry = data.find( index );
      if( entry == data.end())
      {
         data[ index ] = EntryValue( 0., 0. );
      }

      if( c == Real ) return data[ index ].first;
      else            return data[ index ].second;
   }

   double SparseMatrix :: retrieveEntry( int row, int col, int c ) const
   {
      EntryIndex index( col, row );

      EntryMap::const_iterator entry = data.find( index );

      if( entry == data.end())
      {
         return 0;
      }

      if( c == Real ) return entry->second.first;
      else            return entry->second.second;
   }

   void solve( SparseMatrix& A,
                DenseMatrix& x,
                DenseMatrix& b )
   {
      x = SuiteSparseQR<double>( *A, *b, context );
   }
}

