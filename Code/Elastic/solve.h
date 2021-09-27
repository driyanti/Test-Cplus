#ifndef SOLVE_H
#define SOLVE_H

//-----------------------------------------------------------------------
//   Solve function
//-----------------------------------------------------------------------

bool solve
  ( const double*           mat,    // matrix
    double*                 sol,    // solution vector
    const double*           rhs,    // righ-hand side vector
    int                     dim );  // matrix size

#endif
