/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

/* HAL Includes */
#include "ButtonLED_HAL.h"
#include "Timer32_HAL.h"
#include "LCD_display_HAL.h"

// TODO: add the uart HAL include statement here  to include the files in building the project
#include "UART_HAL.h"
/* Other Includes */
#include "edit.h"

//
extern HWTimer_t timer0, timer1;

// TODO: feel free to add
void init_all(edit_t* edit);
void init_edit(edit_t* edit);
void setB(edit_t *edit);
void setM(edit_t *edit);
void write(edit_t *edit);
void formatter(edit_t *edit);
void backColor(edit_t *edit);
void fontColor (edit_t *edit);
void font_S(edit_t *edit);
void setFont (edit_t *edit);
void setFontCm (edit_t *edit);
void setFontCmss (edit_t *edit);
void setFontCmtt (edit_t *edit);
void font_T(edit_t *edit) ;
void movement (edit_t *edit);



int main(void)
{
    edit_t edit;
    init_edit(&edit);
    init_all(&edit);

    // Launchpad left button
    button_t L_S1;
    initButton(&L_S1, GPIO_PORT_P1, GPIO_PIN1, &timer0);


    // Launchpad right button
    button_t L_S2;
    initButton(&L_S2, GPIO_PORT_P1, GPIO_PIN4, &timer0);

    // TODO: Make sure you initialize the state machines of your buttons like the example above or simply use your own solution from HW4

    while(1)
    {
        if(ButtonPushed(&L_S2))
               setB(&edit); //if bottom button is pressed this will cycle the baud rate

        if(ButtonPushed(&L_S1))
               setM(&edit); //if top button is pressed this will cycle the mode


               write(&edit); //edit writer for edit mode
               formatter(&edit); //formatter for format mode
    }
}

//Baud rate color based on the moment of the initial Baud rate
void setB(edit_t *edit)
{
    if (edit->curr_R == baud9600)
    {
        edit->curr_R = baud19200;

        turnOff_BoosterpackLED_red();
        turnOn_BoosterpackLED_green();
        turnOff_BoosterpackLED_blue();
    }
    if (edit->curr_R == baud19200)
    {
        edit->curr_R = baud38400;

        turnOff_BoosterpackLED_red();
        turnOff_BoosterpackLED_green();
        turnOn_BoosterpackLED_blue();
    }
    if (edit->curr_R == baud38400)
    {
        edit->curr_R = baud57600;

        turnOn_BoosterpackLED_red();
        turnOn_BoosterpackLED_green();
        turnOn_BoosterpackLED_blue();
    }
    if (edit->curr_R == baud57600)
    {
        edit->curr_R = baud9600;

        turnOn_BoosterpackLED_red();
        turnOff_BoosterpackLED_green();
        turnOff_BoosterpackLED_blue();
    }
    InitializeUart(edit);
}

//this function will set the next mode depending on the current mode
void setM(edit_t *edit)
{
    if (edit->curr_M == test)
    {
        edit->curr_M= format;
    }
    if (edit->curr_M == format)
    {
           edit->curr_M = test;
           edit->tem = true;
    }
}

//This function displays the UART
void write(edit_t *edit)
{
    if(edit->curr_M == test)
    {
        Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
        Graphics_drawLineV(g_sGraphicInfo, Graphics_getStringWidth(g_sGraphicInfo, edit->edit, edit->len),0, Graphics_getStringBaseline(g_sGraphicInfo));
        if(UARTHasChar(EUSCI_A0_BASE)) {
            char in = UART_receiveData(EUSCI_A0_BASE);

            if(in == '\b')
            {
                if(edit->len >ZERO)
                {
                    edit->edit[edit->len-1] = '\0';
                    edit->len--;
                    lcd_draw_string(edit);
                }
            }
            else  {
            edit->edit[edit->len] = in;
            edit->len++;
            lcd_draw_string(edit);
            if (edit->len >= DIM)
            {
                int i;

                for(i = ZERO; i<edit->len;i++)
                {
                    edit->edit[i] = '\0';
                }
                edit->len = ZERO;
            }

            UART_transmitData(EUSCI_A0_BASE,in);
            }

        }
    }
    if(edit->curr_M == format) {
        if(edit->tem)
            lcd_draw_string(edit);
        edit->tem = false;

    }

}
//This funciton helps format the the diplay
void formatter(edit_t *edit)
{
    if(edit->curr_M == format)
    {
        Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;

        if(UARTHasChar(EUSCI_A0_BASE)) {
            char in = UART_receiveData(EUSCI_A0_BASE);
            UART_transmitData(EUSCI_A0_BASE,in);

            if(in == 'b')
            {
                backColor(edit);
            }
            if(in == 'c')
            {
                fontColor(edit);
            }
            if(in == 's')
            {
                font_S(edit);
            }
            if(in == 't')
            {
                font_T(edit);
            }
            if(in == 'r')
            {
                Graphics_setFont(g_sGraphicInfo, &g_sFontCmss12);
                Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_BLACK);
                Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_WHITE);
                edit->position_X = 0;
                edit->position_Y = 0;
                edit->font_S = 12;
                edit->font_T = cmss;
                lcd_draw_string(edit);
            }
            if(in == 'm')
            {
                movement(edit);
                lcd_draw_string(edit);

            }
        }
    }
}
//This helps move the text position
void movement (edit_t *edit) {
    Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
    char first = UART_receiveData(EUSCI_A0_BASE);
    UART_transmitData(EUSCI_A0_BASE,first);

    char second = UART_receiveData(EUSCI_A0_BASE);
    UART_transmitData(EUSCI_A0_BASE,second);

    int f = first - '0';
    int s = second - '0';
    int pixels = 10* f + s;

    char in = UART_receiveData(EUSCI_A0_BASE);
    UART_transmitData(EUSCI_A0_BASE,in);

    if(in == 'd') {
        edit->position_Y = edit->position_Y + pixels;
    }
    if(in == 'u') {
        edit->position_Y = edit->position_Y - pixels;
    }
    if(in == 'l') {
        edit->position_X = edit->position_X - pixels;
    }
    if(in == 'r') {
        edit->position_X = edit->position_X + pixels;
    }

}
//This function helps change the font type
void font_T(edit_t *edit)
{
    char in = UART_receiveData(EUSCI_A0_BASE);
    UART_transmitData(EUSCI_A0_BASE,in);
        if(in == 'c')
            edit->font_T = cm;
        if(in == 's')
            edit->font_T = cmss;
        if(in == 't')
            edit->font_T = cmtt;
        setFont(edit);
        lcd_draw_string(edit);

}

//This function helps change the size type
void font_S (edit_t *edit) {
    char in = UART_receiveData(EUSCI_A0_BASE);
    UART_transmitData(EUSCI_A0_BASE,in);
        if(in == 'b') {
            if (edit->font_S < 43) {
                edit->font_S ++;
                edit->font_S ++;
                setFont (edit);
                lcd_draw_string(edit);

            }
        }

        if(in == 's') {
            if (edit->font_S > 13) {
            edit->font_S --;
            edit->font_S --;
            setFont (edit);
            lcd_draw_string(edit);

            }
        }

}

//This function helps to set the right font
void setFont (edit_t *edit)
{

        if (edit->font_T == cm) {
            setFontCm (edit);
        }
        if (edit->font_T == cmss) {
            setFontCmss (edit);
        }
        if (edit->font_T == cmtt) {
            setFontCmtt (edit);
        }



}

//This function set the font to the specific number
void setFontCm (edit_t * edit)
{
    Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
    if(edit->font_S == 12)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm12);

    if(edit->font_S == 14)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm14);

    if(edit->font_S == 16)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm16);

    if(edit->font_S == 18)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm18);

    if(edit->font_S == 20)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm20);

    if(edit->font_S == 22)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm22);

    if(edit->font_S == 24)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm24);

    if(edit->font_S == 26)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm26);

    if(edit->font_S == 28)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm28);

    if(edit->font_S == 30)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm30);

    if(edit->font_S == 32)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm32);

    if(edit->font_S == 34)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm34);

    if(edit->font_S == 36)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm36);

    if(edit->font_S == 38)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm38);

    if(edit->font_S == 40)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm40);

    if(edit->font_S == 42)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm42);

    if(edit->font_S == 44)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCm44);

}
//This function set the font to the specific number
void setFontCmss (edit_t * edit)
{
    Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
    if(edit->font_S == 12)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss12);
    if(edit->font_S == 14)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss14);
    if(edit->font_S == 16)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss16);
    if(edit->font_S == 18)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss18);
    if(edit->font_S == 20)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss20);
    if(edit->font_S == 22)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss22);
    if(edit->font_S == 24)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss24);
    if(edit->font_S == 26)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss26);
    if(edit->font_S == 28)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss28);
    if(edit->font_S == 30)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss30);
    if(edit->font_S == 32)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss32);
    if(edit->font_S == 34)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss34);
    if(edit->font_S == 36)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss36);
    if(edit->font_S == 38)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss38);
    if(edit->font_S == 40)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss40);
    if(edit->font_S == 42)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss42);
    if(edit->font_S == 44)
        Graphics_setFont(g_sGraphicInfo, &g_sFontCmss44);
}

//This sets the r
void setFontCmtt (edit_t * edit)
{
    Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
        if(edit->font_S == 12)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt12);
        if(edit->font_S == 14)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt14);
        if(edit->font_S == 16)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt16);
        if(edit->font_S == 18)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt18);
        if(edit->font_S == 20)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt20);
        if(edit->font_S == 22)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt22);
        if(edit->font_S == 24)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt24);
        if(edit->font_S == 26)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt26);
        if(edit->font_S == 28)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt28);
        if(edit->font_S == 30)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt30);
        if(edit->font_S == 32)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt32);
        if(edit->font_S == 34)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt34);
        if(edit->font_S == 36)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt36);
        if(edit->font_S == 38)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt38);
        if(edit->font_S == 40)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt40);
        if(edit->font_S == 42)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt42);
        if(edit->font_S == 44)
            Graphics_setFont(g_sGraphicInfo, &g_sFontCmtt44);
}


//This sets the back ground color
void backColor (edit_t *edit)
{
        Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
        char in = UART_receiveData(EUSCI_A0_BASE);
        UART_transmitData(EUSCI_A0_BASE,in);

        if(in == 'r') {
            Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_RED);
            lcd_draw_string(edit);
        }
        else if(in == 'l') {
            Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_BLACK);
            lcd_draw_string(edit);
        }
        else if(in == 'g') {
            Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_GREEN);
            lcd_draw_string(edit);
        }
        else if(in == 'y') {
              Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_YELLOW);
              lcd_draw_string(edit);
          }
        else if(in == 'b') {
              Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_BLUE);
              lcd_draw_string(edit);
          }
        else if(in == 'm') {
              Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_MAGENTA);
              lcd_draw_string(edit);
          }
        else if(in == 'c') {
              Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_CYAN);
              lcd_draw_string(edit);
          }
        else if(in == 'w') {
              Graphics_setBackgroundColor(g_sGraphicInfo, GRAPHICS_COLOR_WHITE);
              lcd_draw_string(edit);
          }
}

//This help set the font color
void fontColor (edit_t *edit)
{
        Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
        char in = UART_receiveData(EUSCI_A0_BASE);
        UART_transmitData(EUSCI_A0_BASE,in);

        if(in == 'r')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_RED);
            lcd_draw_string(edit);
        }
        else if(in == 'l')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_BLACK);
            lcd_draw_string(edit);
        }
        else if(in == 'g')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_GREEN);
            lcd_draw_string(edit);
        }
        else if(in == 'y')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_YELLOW);
              lcd_draw_string(edit);
          }
        else if(in == 'b')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_BLUE);
              lcd_draw_string(edit);
          }
        else if(in == 'm')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_MAGENTA);
              lcd_draw_string(edit);
          }
        else if(in == 'c')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_CYAN);
              lcd_draw_string(edit);
          }
        else if(in == 'w')
        {
            Graphics_setForegroundColor(g_sGraphicInfo, GRAPHICS_COLOR_WHITE);
              lcd_draw_string(edit);
          }
}

//Initalize the Leds
void init_leds()
{
    initialize_BoosterpackLED_red();
    initialize_BoosterpackLED_green();
    initialize_BoosterpackLED_blue();
    turnOn_BoosterpackLED_red();
    turnOff_BoosterpackLED_green();
    turnOff_BoosterpackLED_blue();

}

//Intinitialize the buttons
void init_buttons()
{
    initialize_BoosterpackTopButton();
    initialize_BoosterpackBottomButton();

}

//This function initializes the timers
void init_timers()
{
    initHWTimer0();
    initHWTimer1();
}

void init_all(edit_t* edit)
{
    WDT_A_hold(WDT_A_BASE);

    init_leds();
    init_buttons();
    init_timers();
    init_graphics(edit);
    //InitializeUart(edit);
}

//This function helps initalize the string to display
void init_edit(edit_t* edit)
{
    edit->curr_M = test;

    edit->position_X = 0;
    edit->position_Y = 0;
    edit->font_S = 12;
    edit->len = 0;
    edit-> font_T = cmss;
    edit-> curr_R = baud9600;
}

















