#ifndef PARAMS_H
#define PARAMS_H

//-----------------------------------------------------------------------
//   class Params
//-----------------------------------------------------------------------

class Params
{
 public:

  // Number of elements and number of nodes.

  int     nelm    = 0;
  int     nnod    = 0;

  // Input parameters:
  //
  //   force  : force [N] applied to the right end of the bar;
  //   tract  : traction [N/m] applied along the bar;
  //   area   : cross-sectional area [m^2];
  //   young  : Young's modulus [Pa];
  //   length : length of the bar [m].

  double  force  = 0.0;
  double  tract  = 0.0;
  double  area   = 0.0;
  double  young  = 0.0;
  double  length = 0.0;

};

#endif
