#include "UART_HAL.h"
#include "ButtonLED_HAL.h"
#include "edit.h"
//  uart
const eUSCI_UART_Config uartConfig9600 =
{
         EUSCI_A_UART_CLOCKSOURCE_SMCLK,
         312,
         8,
         0,
         EUSCI_A_UART_NO_PARITY,
         EUSCI_A_UART_LSB_FIRST,
         EUSCI_A_UART_ONE_STOP_BIT,
         EUSCI_A_UART_MODE,
         EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };
const eUSCI_UART_Config uartConfig19200 =
    {
         EUSCI_A_UART_CLOCKSOURCE_SMCLK,
         156,
         4,
         0,
         EUSCI_A_UART_NO_PARITY,
         EUSCI_A_UART_LSB_FIRST,
         EUSCI_A_UART_ONE_STOP_BIT,
         EUSCI_A_UART_MODE,
         EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };
const eUSCI_UART_Config uartConfig38400 =
    {
         EUSCI_A_UART_CLOCKSOURCE_SMCLK,
         78,
         2,
         0,
         EUSCI_A_UART_NO_PARITY,
         EUSCI_A_UART_LSB_FIRST,
         EUSCI_A_UART_ONE_STOP_BIT,
         EUSCI_A_UART_MODE,
         EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };
const eUSCI_UART_Config uartConfig57600 =
    {
         EUSCI_A_UART_CLOCKSOURCE_SMCLK,
         52,
         1,
         0x25,
         EUSCI_A_UART_NO_PARITY,
         EUSCI_A_UART_LSB_FIRST,
         EUSCI_A_UART_ONE_STOP_BIT,
         EUSCI_A_UART_MODE,
         EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };


void InitializeUart(edit_t* edit)
{
    if (edit->curr_R == baud9600)
    {
        InitUART(EUSCI_A0_BASE, &uartConfig9600, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
    }
    else if (edit->curr_R == baud19200)
    {
        InitUART(EUSCI_A0_BASE, &uartConfig19200, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
    }
    else if (edit->curr_R == baud38400)
    {
        InitUART(EUSCI_A0_BASE, &uartConfig38400, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
    }
    else if (edit->curr_R == baud57600)
    {
        InitUART(EUSCI_A0_BASE, &uartConfig57600, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
    }

}

//Initialize the UART
void InitUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p, uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
    UART_initModule(moduleInstance, uartConfig_p);
    UART_enableModule(moduleInstance);
    GPIO_setAsPeripheralModuleFunctionInputPin(selectedPort, selectedPins, GPIO_PRIMARY_MODULE_FUNCTION);
}

//Update the UART
void UpdateUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p)
{
    UART_initModule(moduleInstance, uartConfig_p);
    UART_enableModule(moduleInstance);
}

//Check the UART
bool UARTHasChar(uint32_t moduleInstance)
{
    if (UART_getInterruptStatus(moduleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
        return true;
    return false;
}
//Return the char
uint8_t UARTGetChar(uint32_t moduleInstance)
{
    return UART_receiveData(moduleInstance);

}
//Test if the data can be sent to UART
bool UARTCanSend(uint32_t moduleInstance)
{
    if (UART_getInterruptStatus(moduleInstance, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
        return true;
    else
        return false;
}
//sets the char
void UARTPutChar(uint32_t moduleInstance, uint8_t tChar)
{
    UART_transmitData(moduleInstance, tChar);
}

