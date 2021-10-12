//============================================================================
// Name        : volume.cpp
// Author      : MW/CDR
// Version     : 001
// Copyright   : Your copyright notice
// Description : Calculate the volume
//============================================================================

#include "application.h"

int main(int argc, char * argv[])
{
    // register arguments

    Application app(argc, argv);

    if (int status = app.getStatus() != ERR_NO_ERROR)
    {
        app.printErrorMessage();
        return status;
    }

    // check input

    if (int status = app.checkInput() != ERR_NO_ERROR)
    {
        return status;
    }

    // execute the main program

    return app.exec();
}

