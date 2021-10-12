#ifndef HORIZON_H_
#define HORIZON_H_

#include "unit.h"

namespace Horizon
{

class Array2D
{
public:
    Array2D(int M, int N, double dx, double dy, Unit::unit_t unit);
    ~Array2D();

    int getM();
    int getN();

    void setDx(double dx);
    double getDx();

    void setDy(double dy);
    double getDy();

    Unit::unit_t getUnit();

    void setValueAt(int row, int col, double value);
    double getValueAt(int row, int col);

private:
    int M_;
    int N_;

    double dx_;
    double dy_;

    double *data_;

    Unit::unit_t unit_;
};

//!

int readHorizonFromFile(Array2D *horizon, const char *filename);

double computeVolumeAboveFluidContact(Array2D *horizon, double waterContact);

int checkValidityOfTopAndBaseHorizon(Array2D *top, Array2D *base);

};


#endif  // Horizon_H_
