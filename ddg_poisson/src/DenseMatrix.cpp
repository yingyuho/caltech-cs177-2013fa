#include <algorithm>
#include <cassert>
#include <cmath>
#include "DenseMatrix.h"
#include "LinearContext.h"

using namespace std;

namespace DDG
{
   extern LinearContext context;

   DenseMatrix :: DenseMatrix( int m_, int n_, int xtype_ )
   // initialize an mxn matrix of doubles
   // xtype is either DDG::Real or DDG::Complex
   : m( m_ ),
     n( n_ ),
     xtype( xtype_ ),
     data( NULL ),
     rData( NULL ),
     iData( NULL )
   {
      int d = m; // leading dimension
      data = cholmod_l_allocate_dense( m, n, d, xtype, context );
      rData = (double*) data->x;
      if( xtype == Complex )
      {
         iData = (double*) data->z;
      }
   }

   DenseMatrix :: DenseMatrix( const DenseMatrix& A )
   // copy constructor
   : data( NULL ),
     rData( NULL ),
     iData( NULL )
   {
      *this = A;
   }

   DenseMatrix :: ~DenseMatrix( void )
   // destructor
   {
      if( data ) cholmod_l_free_dense( &data, context );
   }

   cholmod_dense* DenseMatrix :: operator*( void )
   {
      return data;
   }

   void DenseMatrix :: initializeFromCopy( void )
   {
      assert( data->dtype == CHOLMOD_DOUBLE );

      xtype = data->xtype;
      m = data->nrow;
      n = data->ncol;
      rData = (double*) data->x;
      if( data->xtype == Complex )
      {
         iData = (double*) data->z;
      }
   }

   const DenseMatrix& DenseMatrix :: operator=( const DenseMatrix& A )
   // copies A
   {
      if( data ) { cholmod_l_free_dense( &data, context ); data = NULL; }

      data = cholmod_l_copy_dense( A.data, context );

      initializeFromCopy();

      return *this;
   }

   const DenseMatrix& DenseMatrix :: operator=( cholmod_dense* A )
   // gets pointer to A; will deallocate A upon destruction
   {
      if( data ) { cholmod_l_free_dense( &data, context ); data = NULL; }

      data = A;

      initializeFromCopy();

      return *this;
   }

   int DenseMatrix :: nRows( void ) const
   // returns the number of rows
   {
      return m;
   }

   int DenseMatrix :: nColumns( void ) const
   // returns the number of columns
   {
      return n;
   }

   int DenseMatrix :: length( void ) const
   // returns the size of the largest dimension
   {
      return max( m, n );
   }

   void DenseMatrix :: zero( double rVal, double iVal )
   // sets all elements to rVal+iVal*i
   {
      if( rData )
      for( int i = 0; i < m*n; i++ )
      {
         rData[i] = rVal;
      }

      if( iData )
      for( int i = 0; i < m*n; i++ )
      {
         iData[i] = iVal;
      }
   }

   double DenseMatrix :: norm( void )
   // returns the maximum magnitude of any entry
   {
      return cholmod_l_norm_dense( **this, 0, context );
   }

   double& DenseMatrix :: operator()( int row, int col )       { return rData[ row + m*col ]; }
   double  DenseMatrix :: operator()( int row, int col ) const { return rData[ row + m*col ]; }
   double& DenseMatrix ::          r( int row, int col )       { return rData[ row + m*col ]; }
   double  DenseMatrix ::          r( int row, int col ) const { return rData[ row + m*col ]; }
   double& DenseMatrix ::          i( int row, int col )       { return iData[ row + m*col ]; }
   double  DenseMatrix ::          i( int row, int col ) const { return iData[ row + m*col ]; }
   double& DenseMatrix :: operator()( int index )              { return rData[ index ]; }
   double  DenseMatrix :: operator()( int index ) const        { return rData[ index ]; }
   double& DenseMatrix ::          r( int index )              { return rData[ index ]; }
   double  DenseMatrix ::          r( int index ) const        { return rData[ index ]; }
   double& DenseMatrix ::          i( int index )              { return iData[ index ]; }
   double  DenseMatrix ::          i( int index ) const        { return iData[ index ]; }
}

