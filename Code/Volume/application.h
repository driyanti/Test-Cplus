#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "unit.h"
#include "horizon.h"

#include <iostream>

#define PROGNAME             ("volume")
#define VERSION              ("0.0.1")

#define ERR_NO_ERROR         (0x00)
#define ERR_ANY_ERROR        (0xFF)
#define ERR_INV_OPTION       (0x01)
#define ERR_HELP_MENU        (0x10)
#define ERR_VERSION          (0x11)
#define ERR_ABOUT            (0x12)
#define ERR_NO_DX            (0x20)
#define ERR_NO_DY            (0x21)
#define ERR_NO_NX            (0x22)
#define ERR_NO_NY            (0x23)
#define ERR_NO_TOPHORIZON    (0x24)
#define ERR_NO_BASEHORIZON   (0x25)
#define ERR_NO_FLUIDCONTACT  (0x26)
#define ERR_NO_UNIT          (0x27)

#define MAX_STRLEN           (1024)

class Application
{
public:
    Application( int argc, char *argv[] );

    int checkInput();
    int getStatus();
    void printErrorMessage();
    void showHelp();

    int exec(); //! main routine

private:
    std::string inputFileTopHorizon_;
    std::string inputFileBaseHorizon_;

    int nx_;
    int ny_;

    double dx_;
    double dy_;

    double fluidContact_;

    Unit::unit_t unit_;

    bool flagHasBaseHorizon_;

    int status_;
};

#endif  // APPLICATION_H_
