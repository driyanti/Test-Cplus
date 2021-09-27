#include <cmath>
#include <cassert>
#include "solve.h"

//-----------------------------------------------------------------------
//   solve the system of equations using GE 
//-----------------------------------------------------------------------

using namespace std;

bool solve
  ( const double*           mat,
    double*                 sol,
    const double*           rhs,
    int                     dim )
{
  assert ( dim >= 0 );

  const double  EPS   = 1.0e-10;

  double*       fact   = nullptr;
  double*       rvec   = nullptr;
  bool          result = false;

  double        s, t;
  int           i, j, k;

  // Handle the trivial case.

  if ( dim == 0 )
  {
    return true;
  }

  // Allocate memory for the local arrays.

  try
  {
    fact = new double[dim * dim];
    rvec = new double[dim];
  }
  catch ( ... )
  {
    delete [] fact;
    delete [] rvec;

    throw;
  }

  // Copy the matrix and right-hand side vector. Also scale
  // each row with the largest matrix entry on that row.
  // Note that the matrix "fact" is stored row wise for
  // better performance.

  for ( i = 0; i < dim; i++ )
  {
    s = 0.0;

    for ( j = 0; j < dim; j++ )
    {
      t = fabs ( mat[i + j * dim] );

      if ( t > s )
      {
        s = t;
      }
    }

    if ( s <= 0.0 )
    {
      // Singular matrix, then exit.
      goto done;
    }

    s       = 1.0 / s;
    rvec[i] = s * rhs[i];

    for ( j = 0; j < dim; j++ )
    {
      fact[j + i * dim] = s * mat[i + j * dim];
    }
  }

  // Perform Gauss elimination.

  for ( int i = 0; i < dim; i++ )
  {
    // Find the row with the largest pivot.

    s = fabs ( fact[i + i * dim] );
    j = i;

    for ( k = i + 1; k < dim; k++ )
    {
      t = fabs ( fact[i + k * dim] );

      if ( t > s )
      {
        s = t;
        j = k;
      }
    }

    k = j;

    // Check for a singular matrix.

    if ( s <= EPS )
    {
      goto done;
    }

    if ( k != i )
    {
      // Swap row i and row k.

      t       = rvec[i];
      rvec[i] = rvec[k];
      rvec[k] = t;

      for ( j = i; j < dim; j++ )
      {
        t                 = fact[j + i * dim];
        fact[j + i * dim] = fact[j + k * dim];
        fact[j + k * dim] = t;
      }
    }

    // Subtract the current row from the rows below it.

    s = 1.0 / fact[i + i * dim];

    for ( k = i + 1; k < dim; k++ )
    {
      t                 = s * fact[i + k * dim];
      rvec[k]          -= t * rvec[i];
      fact[i + k * dim] = 0.0;

      for ( j = i + 1; j < dim; j++ )
      {
        fact[j + k * dim] -= t * fact[j + i * dim];
      }
    }
  }

  // The matrix is now an upper triangular matrix. Calculate the
  // solution, starting at the last row.

  for ( i = dim - 1; i >= 0; i-- )
  {
    t = rvec[i];

    for ( j = i + 1; j < dim; j++ )
    {
      t -= sol[j] * fact[j + i * dim];
    }

    sol[i] = t / fact[i + i * dim];
  }

  result = true;

done:

  delete [] fact;
  delete [] rvec;

  return result;
}
