//============================================================================
// Name        : application.cpp
// Author      : MW/CDR
// Version     : 001
// Copyright   : Your copyright notice
// Description : 
//============================================================================

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "application.h"
#include "unit.h"
#include "horizon.h"

/****/

Application::Application ( int argc, char *argv[] )
{
	//! initialize

	inputFileTopHorizon_  = "<no-file>";
	inputFileBaseHorizon_ = "<no-file>";

	dx_ = -1.;
	dy_ = -1.;

	nx_ = 0;
	ny_ = 0;

	fluidContact_ = 0.;

	unit_ = Unit::nounit;

	flagHasBaseHorizon_ = false;

	//! if no arguments, show to help menu

	if (argc == 1)
	{
		status_ = ERR_HELP_MENU;
		return;
	}

	//! parse arguments

    for (int i = 1; i < argc; i++)
    {
        if (*(argv[i]) == '-')
        {
            if (strcmp(argv[i], "-tophorizon") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_TOPHORIZON;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_TOPHORIZON;
                        return;
                    }

                    inputFileTopHorizon_ = argv[i + 1];
                    i++;
                }
            }
            else if (strcmp(argv[i], "-basehorizon") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_BASEHORIZON;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_BASEHORIZON;
                        return;
                    }

                    inputFileBaseHorizon_ = argv[i + 1];

                    flagHasBaseHorizon_ = true;

                    i++;
                }
            }
            else if (strcmp(argv[i], "-dx") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_DX;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_DX;
                        return;
                    }

                    dx_ = atof(argv[i + 1]);
                    i++;
                }
            }
            else if (strcmp(argv[i], "-dy") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_DY;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_DY;
                        return;
                    }

                    dy_ = atof(argv[i + 1]);
                    i++;
                }
            }
            else if (strcmp(argv[i], "-nx") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_NX;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_NX;
                        return;
                    }

                    nx_ = atoi(argv[i + 1]);
                    i++;
                }
            }
            else if (strcmp(argv[i], "-ny") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_NY;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_NY;
                        return;
                    }

                    ny_ = atoi(argv[i + 1]);
                    i++;
                }
            }
            else if (strcmp(argv[i], "-fluidcontact") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_FLUIDCONTACT;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_FLUIDCONTACT;
                        return;
                    }

                    fluidContact_ = atof(argv[i + 1]);
                    i++;
                }
            }
            else if (strcmp(argv[i], "-unit") == 0)
            {
                if (i == argc - 1)
                {
                    status_ = ERR_NO_UNIT;
                    return;
                }
                else
                {
                    if (*(argv[i + 1]) == '-')
                    {
                        status_ = ERR_NO_UNIT;
                        return;
                    }

                    if (strncmp(argv[i + 1], "feet", 4) == 0)
                    {
                        unit_ = Unit::feet;
                    }
                    else if (strncmp(argv[i + 1], "meter", 5) == 0)
                    {
                        unit_ = Unit::meter;
                    }
                    else
                    {
                        unit_ = Unit::nounit;
                    }

                    i++;
                }
            }
            else if (strcmp(argv[i], "--version") == 0)
            {
                status_ = ERR_VERSION;
                return;
            }
            else if (strcmp(argv[i], "--about") == 0)
            {
                status_ = ERR_ABOUT;
                return;
            }
            else if (strcmp(argv[i], "--help") == 0)
            {
                status_ = ERR_HELP_MENU;
                return;
            }
            else
            {
                status_ = ERR_INV_OPTION;
                return;
            }
        }
        else
        {
            status_ = ERR_INV_OPTION;
            return;
        }
    }

    status_ = ERR_NO_ERROR;

} //! constructor

/***/

int Application::checkInput()
{
    int status = ERR_NO_ERROR;

    //! check unit

    if (unit_ == Unit::nounit)
	{
        fprintf(stderr, "error: unknown unit\n");
        status = ERR_ANY_ERROR;
	}

    //! check cell spacing

    if ( (dx_ <= 0.) || (dy_ <= 0.) )
    {
        fprintf(stderr, "error: missing input for the cell spacing or its value is negative or zero\n");
        status = ERR_ANY_ERROR;
    }

    //! check number of points

    if ( (nx_ < 2) || (ny_ < 2) )
    {
        fprintf(stderr, "error: missing input for the number of point or the number is less than two points in each direction\n");
        status = ERR_ANY_ERROR;
    }

    //! check file top horizon

    if (inputFileTopHorizon_ == "<no-file>")
    {
        fprintf(stderr, "error: missing input file for top horizon\n");
        status = ERR_ANY_ERROR;
    }

    return status;

} //! checkInput

/**
 *
 */

void Application::printErrorMessage()
{
    switch (status_)
    {
    case ERR_INV_OPTION:
        fprintf(stderr, "%s: error: unknown option\n", PROGNAME);
        fprintf(stderr, "    for information type '%s --help'\n", PROGNAME);
        break;

    case ERR_HELP_MENU:
        showHelp();
        break;

    case ERR_VERSION:
    	fprintf(stdout, "%s\n", VERSION);
        break;

    case ERR_ABOUT:
    	fprintf(stdout, "%s: version %s\n", PROGNAME, VERSION);
        fprintf(stdout, "(C) Copyright 2018\n");
        break;

    default:
        fprintf(stderr, "%s: unknown error\n", PROGNAME);
        showHelp();
        break;
    }
}

/**
 *
 */

void Application::showHelp()
{
    std::cout << " Usage: volume -dx <double> ... " << std::endl;
    std::cout << " volume - compute the volume between top and base horizons above fluid contact " << std::endl;
    std::cout << " "                                << std::endl;
    std::cout << " IO Parameters:"                  << std::endl;
    std::cout << "   -dx <double> ................ Cell spacing in x direction" << std::endl;
    std::cout << "   -dy <double> ................ Cell spacing in y direction" << std::endl;
    std::cout << "   -nx <integer> ............... Number of points in x direction" << std::endl;
    std::cout << "   -ny <integer> ............... Number of points in y direction" << std::endl;
    std::cout << "   -tophorizon <file> .......... Input file for top horizon" << std::endl;
    std::cout << "   -basehorizon <file> ......... Input file for base horizon (optional)" << std::endl;
    std::cout << "   -fluidcontact <double> ...... Fluid contact" << std::endl;
    std::cout << "   -unit <string> .............. Unit: feet or meter" << std::endl;
    std::cout << " "                               << std::endl;
    std::cout << " Options:"                       << std::endl;
    std::cout << "   --help ...................... Display this help and exit" << std::endl;
    std::cout << "   --version ................... Display the version of the program" << std::endl;
    std::cout << "   --about ..................... Display author information" << std::endl;
    std::cout << "As an example please run this: ./volume -unit feet -nx 16 -ny 26 -dx 200 -dy 200 -fluidcontact 9842.519685 -tophorizon top.txt"<< std::endl;
}

/**
 *
 */

int Application::getStatus()
{
    return status_;
}

/**
 * MAIN PROGRAM
 */

int Application::exec()
{
    double volume;

    //! compute volume

    if (flagHasBaseHorizon_)
    {
        //! allocate the horizon container

        Horizon::Array2D top (nx_, ny_, dx_, dy_, unit_);
        Horizon::Array2D base(nx_, ny_, dx_, dy_, unit_);

        //! read data from file

        if (int status = Horizon::readHorizonFromFile(&top, inputFileTopHorizon_.c_str()) != 0)
        {
            return status;
        }

        if (int status = Horizon::readHorizonFromFile(&base, inputFileBaseHorizon_.c_str()) != 0)
        {
            return status;
        }

        if (int status = Horizon::checkValidityOfTopAndBaseHorizon(&top, &base) != 0)
        {
            return status;
        }

        //!

        double volumeTopHorizon  = Horizon::computeVolumeAboveFluidContact(&top, fluidContact_);
        double volumeBaseHorizon = Horizon::computeVolumeAboveFluidContact(&base, fluidContact_);

        volume = volumeTopHorizon - volumeBaseHorizon;
    }
    else
    {
        //! allocate the horizon container

        Horizon::Array2D top(nx_, ny_, dx_, dy_, unit_);

        //! read data from file

        if (int status = Horizon::readHorizonFromFile(&top, inputFileTopHorizon_.c_str()) != 0)
        {
            return status;
        }

        //!

        double volumeTopHorizon = Horizon::computeVolumeAboveFluidContact(&top, fluidContact_);

        volume = volumeTopHorizon;
    }

    //! show result with different unit

    if (unit_ == Unit::feet)
    {
        double volume_f = volume;
        double volume_m = volume * FEET2METER * FEET2METER * FEET2METER;
        double volume_b = volume * CFEET2BARREL;

        printf("The volume :\n");
        printf("%.0f cubic feet\n", volume_f);
        printf("%.0f cubic meter\n", volume_m);
        printf("%.0f barrels\n", volume_b);
    }
    else
    {
        double volume_m = volume;
        double volume_f = volume / FEET2METER / FEET2METER / FEET2METER;
        double volume_b = volume * CMETER2BARREL;

        printf("The volume is :\n");
        printf("%0e0f cubic meter\n", volume_m);
        printf("%.0f cubic feet\n", volume_f);
        printf("%.0f barrels\n", volume_b);
    }

    //!

    return 0;
}



