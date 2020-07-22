 /*
  * edit.h
  *
  * Created on: Oct 29, 2019
  *    Author: Pranav
  */

#ifndef EDIT_H_
#define EDIT_H_

#define ZERO 0
#define DIM 10

//DriverLib Include

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <string.h>

typedef enum {baud9600, baud19200, baud38400, baud57600} UARTBaudRate_t;
typedef enum {test, format} mode;
typedef enum {cm, cmss, cmtt} font;

typedef struct
{
    char edit[10];

    mode curr_M;

    char font_C;
    char back_C;

    int32_t position_X;
    int32_t position_Y;

    uint32_t font_S;

    int len;

    font font_T;
    UARTBaudRate_t curr_R;

    Graphics_Context g_sGraphicInfo;

    bool tem;


} edit_t;

#endif


