/*
 * This example program solves a 1-D elasticity problem by applying
 * the finite element method. The calculated displacements in the
 * nodes are saved to the output file "disp.out".
 */
#include <fstream>
#include <iostream>

#include "Params.h"
#include "solve.h"

using namespace std;

//-----------------------------------------------------------------------
//   readParams()
//-----------------------------------------------------------------------
// Reads the model parameters from an input file.

bool readParams ( Params& params )
{
  ifstream  input ( "elastic.dat" );
  string    dummy;

  input >> params.nelm;
  getline ( input, dummy );

  input >> params.force;
  getline ( input, dummy );

  input >> params.tract;
  getline ( input, dummy );

  input >> params.area;
  getline ( input, dummy );

  input >> params.young;
  getline ( input, dummy );

  input >> params.length;
  getline ( input, dummy );

  if ( ! input )
  {
    cerr << "Invalid input.\n";
    return false;
  }

  if ( params.nelm <= 0 )
  {
    cerr << "Invalid number of elements: "
              << params.nelm << ".\n";
    return false;
  }

  params.nnod = params.nelm + 1;

  if ( params.area <= 0.0 )
  {
    cerr << "Invalid area: "
              << params.area << ".\n";
    return false;
  }

  if ( params.young <= 0.0 )
  {
    cerr << "Invalid Young's modulus: "
              << params.young << ".\n";
    return false;
  }

  if ( params.length <= 0.0 )
  {
    cerr << "Invalid length: "
              << params.length << ".\n";
    return false;
  }

  return true;
}

// Creates the finite element mesh. To be precise, this function
// determines the node coordinates and the element connectivities
// (which element is connected to which nodes).

void makeMesh
  ( double*                 coords,  // node coordinates
    int*                    elcon,   // element connectivities
    const Params&           params ) // global parameters
{
  double  dx = params.length / params.nelm;

  for ( int inod = 0; inod < params.nnod; inod++ )
  {
    coords[inod] = inod * dx;
  }

  for ( int ielm = 0; ielm < params.nelm; ielm++ )
  {
    elcon[0 + ielm * 2] = ielm + 0;  // first node
    elcon[1 + ielm * 2] = ielm + 1;  // second node
  }
}

// Assembles the global stiffness matrix and the right-hand side vector.

void assemble
  ( double*                 stiffmat,  // stiffness matrix
    double*                 rhs,       // right-hand side vector
    const double*           coords,    // node coordinates
    const int*              elcon,     // element connectivities
    const Params&           params )   // global parameters
{
  double  elmmat[2][2];
  double  elmvec[2];
  int     elmnod[2];
  double  coeff;
  double  dx;

  // Set the global stiffness matrix and the right-hand side
  // vector to zero first.

  for ( int i = 0; i < (params.nnod * params.nnod); i++ )
  {
    stiffmat[i] = 0.0;
  }

  for ( int i = 0; i < params.nnod; i++ )
  {
    rhs[i] = 0.0;
  }

  // Main loop over the elements.

  for ( int ielm = 0; ielm < params.nelm; ielm++ )
  {
    // Store the two element nodes in a local array.

    elmnod[0] = elcon[0 + ielm * 2];
    elmnod[1] = elcon[1 + ielm * 2];
    dx        = coords[elmnod[1]] - coords[elmnod[0]];

    // Compute the element stiffness matrix.

    coeff        =  params.young * params.area / dx;
    elmmat[0][0] =  coeff;
    elmmat[0][1] = -coeff;
    elmmat[1][0] = -coeff;
    elmmat[1][1] =  coeff;

    // Add the element stiffness matrix to the global stiffness matrix.

    for ( int i = 0; i < 2; i++ )
    {
      for ( int j = 0; j < 2; j++ )
      {
        int  ii = elmnod[i];
        int  jj = elmnod[j];
        stiffmat[ii + jj * params.nnod] += elmmat[i][j];
      }
    }

    // Compute the element force vector that incorporates the
    // traction along the bar.

    coeff     = 0.5 * dx * params.tract;
    elmvec[0] = coeff;
    elmvec[1] = coeff;

    // Add the element force vector to the global right-hand side vector.

    for ( int i = 0; i < 2; i++ )
    {
      int  ii = elmnod[i];
      rhs[ii] += elmvec[i];
    }
  }

  // Add the force acting on the right end of the bar to the
  // right-hand side vector.

  rhs[params.nnod - 1] += params.force;

  // to avoid singulaties of stiffness matrix, the zero displacement in the first node by adding
  // a large value to the first diagonal entry in global stiffness matrix.

  stiffmat[0] += 1.0e10;
}

// Saves the displacement in the nodes to an output file.

void saveDisp
  ( const double*           disp,     // node displacements
    const double*           coords,   // node coordinates
    const Params&           params )  // global parameters
{
  ofstream  fout ( "disp.out" );

  fout << "# Column 1: node number.\n";
  fout << "# Column 2: node coordinate [m].\n";
  fout << "# Column 3: node displacement [m].\n\n";

  for ( int inod = 0; inod < params.nnod; inod++ )
  {
    fout << inod << ' ' << coords[inod] << " ";
    fout         << ' ' << disp[inod] << endl;
  }
}

// Calculate the stress in each element and then save into an output file.

void saveStress
  ( const double*           disp,      // node displacements
    const double*           coords,    // node coordinates
    const int*              elcon,     // element connectivities
    const Params&           params )
{
  ofstream  fout ( "stress.out" );

  double         elmdisp[2];
  int            elmnod [2];
  double         center;
  double         strain;
  double         stress;
  double         dx;

  fout << "# Column 1: element number.\n";
  fout << "# Column 2: element center coordinate [m].\n";
  fout << "# Column 3: element stress [Pa].\n\n";

  for ( int ielm = 0; ielm < params.nelm; ielm++ )
  {
    // Store the two element nodes in a local array.

    elmnod[0] = elcon[0 + ielm * 2];
    elmnod[1] = elcon[1 + ielm * 2];

    // Compute the length and center point of the element.

    dx     =  coords[elmnod[1]] - coords[elmnod[0]];
    center = (coords[elmnod[1]] + coords[elmnod[0]]) / 2.0;

    // Get the displacements in the nodes of this element.

    elmdisp[0] = disp[elmnod[0]];
    elmdisp[1] = disp[elmnod[1]];

    // Compute the strain and stress in this element.

    strain = (elmdisp[1] - elmdisp[0]) / dx;
    stress = params.young * strain;

    // Save the data to the output file.
    fout << ielm << ' ' << center;
    fout         << ' ' << stress << endl;
  }
}

//My friend, this is where the main program starts.

int main ()
{
  double*  stiffmat = nullptr;  // global stiffness matrix
  double*  coords   = nullptr;  // node coordinates
  int*     elcon    = nullptr;  // element connectivities
  double*  disp     = nullptr;  // displacements
  double*  rhs      = nullptr;  // right-hand side vector
  Params   params;              // global parameters
  int   check = false;

  cout << endl;
  //cout << "Reading input parameters ...\n";

  if ( ! readParams( params ) )
  {
    return 1;
  }

  if (check == true){
    cout << "\n";
    cout << "  nodes    : " << params.nnod   << endl;
    cout << "  elements : " << params.nelm   << endl;
    cout << "  force    : " << params.force  << endl;
    cout << "  traction : " << params.tract  << endl;
    cout << "  area     : " << params.area   << endl;
    cout << "  young    : " << params.young  << endl;
    cout << "  length   : " << params.length << endl;
    cout << endl;
  }

  //cout << "Allocating arrays ...\n";

  stiffmat = new double [params.nnod * params.nnod];
  coords   = new double [params.nnod];
  elcon    = new int    [params.nelm * 2];
  disp     = new double [params.nnod];
  rhs      = new double [params.nnod];

  //cout << "Creating the finite element mesh ...\n";

  makeMesh ( coords, elcon, params );

  //cout << "Assembling the system of equations ...\n";

  assemble ( stiffmat, rhs, coords, elcon, params );

  //cout << "Solving the system of equations ...\n";

  if ( ! solve( stiffmat, disp, rhs, params.nnod ) )
  {
    cout << "*** Singular stiffness matrix ***\n";
    return 1;
  }

  //cout << "Saving the results ...\n";

  saveDisp ( disp, coords, params );

  saveStress ( disp, coords, elcon, params );
  
  //cout << "Deallocating arrays ...\n";

  delete [] stiffmat;
  delete [] coords;
  delete [] elcon;
  delete [] disp;
  delete [] rhs;

  //Make sure that the arrays are not used anymore by accident.

  stiffmat = nullptr;
  coords   = nullptr;
  elcon    = nullptr;
  disp     = nullptr;
  rhs      = nullptr;

  cout << "Done and success ...\n\n";

  return 0;
}
