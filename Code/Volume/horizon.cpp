//============================================================================
// Name        : horizon.cpp
// Author      : MW/CDR
// Version     : 001
// Copyright   :
// Description :
//============================================================================

#include "unit.h"
#include "horizon.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

namespace Horizon {

#define NUM_CELL_SLICE        (1000)

/****/

Array2D::Array2D ( int M, int N, double dx, double dy, Unit::unit_t unit )
{
    data_ = new double [M * N];

    M_    = M;
    N_    = N;

    dx_   = dx;
    dy_   = dy;

    unit_ = unit;
}

/****/

Array2D::~Array2D()
{
    M_ = 0;
    N_ = 0;

    delete[] data_;
}

/****/

int Array2D::getM()
{
    return M_;
}

/****/

int Array2D::getN()
{
    return N_;
}

//Declare the sampling in x-direction

void Array2D::setDx ( double dx )
{
	dx_ = dx;
}

//Get the value of dx

double Array2D::getDx()
{
	return dx_;
}

//Declare the sampling in y-direction

void Array2D::setDy ( double dy )
{
	dy_ = dy;
}

//Get the value of dx

double Array2D::getDy()
{
	return dy_;
}

//Get the unit of the volume

Unit::unit_t Array2D::getUnit()
{
	return unit_;
}

//Set the values of the matrix

void Array2D::setValueAt ( int row, int col, double val )
{
    data_[row + M_ * col] = val;
}

/**
 *
 */

double Array2D::getValueAt ( int row, int col )
{
    return data_[row + M_ * col];
}

/*read data from file*/

int readHorizonFromFile(Array2D *horizon, const char *filename)
{
    double buffer;
    int status = 0;

    std::fstream inputFile(filename, std::ios_base::in);

    if (inputFile.is_open()) {
        int M = horizon->getM();
        int N = horizon->getN();

        int row = 0;
        int col = 0;

        while ( (inputFile >> buffer) && (col < N) )
        {
            horizon->setValueAt(row, col, buffer);

            row++;

            if (row == M)
            {
                row = 0;
                col++;
            }

        }

        //! check if input array dimension is the same as in the file

        if ( ((inputFile.eof()) && (col < N)) || ((!inputFile.eof()) && (col == N)) )
        {
            std::cout << "error: array dimension is not the same as in the file";
            status = 1;
        }

        //! close file

        inputFile.close();
    }
    else
    {
        std::cout << "error: unable to open or file does not exist";
        status = 1;
    }

    return status;

} //! readHorizonFromFile

//Compute the volume above the fluid contact

double computeVolumeAboveFluidContact ( Array2D * horizon, double fluidContact )
{
    double  volume;
    double  volume_t;

    double *height;
    double  height_t;
    double  height_1;
    double  height_2;
    double  height_a;    // ! height at point A
    double  height_b;    // ! height at point B
    double  height_c;    // ! height at point C
    double  height_d;    // ! height at point D

    double  h__x, tx;
    double  h__y, ty;

    int     point_type;

    int     M = horizon->getM();
    int     N = horizon->getN();

    double  d__x = horizon->getDx();
    double  d__y = horizon->getDy();

    //! compute the distance of the horizon to the fluid contact
    //! and check if a point is above or below the fluid contact

    height = new double [M * N];

    for (int col = 0; col < N; col++)
    {
        for (int row = 0; row < M; row++)
        {
            height[row + M * col]  = horizon->getValueAt(row, col) - fluidContact;
            height[row + M * col] *= -1;
        }
    }

    //! compute volume cell-per-cell
    //!
    //!  D  ---  C
    //!    |   |
    //!  A  ---  B

    volume = 0.;

    for (int col = 0; col < N-1; col++)
    {
        for (int row = 0; row < M-1; row++)
        {
            height_a = height[(row  ) + M * (col  )];
            height_b = height[(row+1) + M * (col  )];
            height_c = height[(row+1) + M * (col+1)];
            height_d = height[(row  ) + M * (col+1)];

            point_type = 0;
            if (height_a >= 0.) point_type += 1;
            if (height_b >= 0.) point_type += 1;
            if (height_c >= 0.) point_type += 1;
            if (height_d >= 0.) point_type += 1;

            switch (point_type)
            {
            case 4:
                //! all points are above the fluid contact
                //! use the bilinear interpolation

                height_t = .25 * (height_a + height_b + height_c + height_d);

                volume  += d__x * d__y * height_t;

                break;

            case 0:
                //! all points are below the fluid contact
                break;

            default:
                //! at the transisition cell, we use approximation

                h__x = d__x / NUM_CELL_SLICE;
                h__y = d__y / NUM_CELL_SLICE;

                volume_t = 0.;

                for (int iy = 0; iy < NUM_CELL_SLICE; iy++)
                {
                    for (int ix = 0; ix < NUM_CELL_SLICE; ix++)
                    {
                        tx = (ix + 0.5) / NUM_CELL_SLICE;
                        ty = (iy + 0.5) / NUM_CELL_SLICE;

                        height_1 = (1-tx) * height_a + (tx) * height_b;
                        height_2 = (1-tx) * height_d + (tx) * height_c;

                        height_t = (1-ty) * height_1 + (ty) * height_2;

                        if (height_t > 0.)
                        {
                            volume_t += h__x * h__y * height_t;
                        }
                    }
                }

                volume += volume_t;

                break;
            }
        }
    }

    //!

    delete[] height;

    //!

    return volume;

} //End computeVolumeAboveFluidContact

//Check whether the volume of top or base horizon between the fluid contact

int checkValidityOfTopAndBaseHorizon ( Array2D *top, Array2D *base )
{
    if (top->getM() != base->getM())
    {
        fprintf(stderr, "error: NX of top and base horizons are not equal \n");
        return 1;
    }

    if (top->getN() != base->getN())
    {
        fprintf(stderr, "error: NY of top and base horizons are not equal \n");
        return 1;
    }

    if (top->getDx() != base->getDx())
    {
        fprintf(stderr, "error: DX of top and base horizons are not equal \n");
        return 1;
    }

    if (top->getDy() != base->getDy())
    {
        fprintf(stderr, "error: DY of top and base horizons are not equal \n");
        return 1;
    }

    if (top->getUnit() != base->getUnit())
    {
        fprintf(stderr, "error: Unit of top and base horizons are not equal \n");
        return 1;
    }

    int status = 0;

    for (int col = 0; col < top->getN(); col++)
    {
        for (int row = 0; row < top->getM(); row++)
        {
            if (top->getValueAt(row,col) > base->getValueAt(row,col))
            {
                status = 1;
            }
        }
    }

    if (status != 0)
    {
        fprintf(stderr, "error: some part of top horizon gets below the base horizon\n");
        return 1;
    }

    return status;
}

} //! namespace Horizon
