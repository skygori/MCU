/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G12 V2.02.00.02 [11 Feb 2014]
* Device(s)    : R5F102AA
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for Serial module.
* Creation Date: 4/10/2015
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t * gp_csi20_rx_address;         /* csi20 receive buffer address */
extern volatile uint16_t  g_csi20_rx_length;           /* csi20 receive data length */
extern volatile uint16_t  g_csi20_rx_count;            /* csi20 receive data count */
extern volatile uint8_t * gp_csi20_tx_address;         /* csi20 send buffer address */
extern volatile uint16_t  g_csi20_send_length;         /* csi20 send data length */
extern volatile uint16_t  g_csi20_tx_count;            /* csi20 send data count */
/* Start user code for global. Do not edit comment generated here */
extern volatile MD_STATUS g_Uart0TxEnd;		/* UART0 transmission end */
extern volatile MD_STATUS g_CSI20TxEnd;		/* CSI20 transmission end */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_interrupt_receive(void)
{
    uint8_t rx_data;
    uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
  	g_Uart0TxEnd = 1U;		/* Set transmission end flag */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_interrupt
* Description  : This function is INTCSI20 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_csi20_interrupt(void)
{
    uint8_t err_type;
    uint8_t sio_dummy;

    err_type = (uint8_t)(SSR10 & _0001_SAU_OVERRUN_ERROR);
    SIR10 = (uint16_t)err_type;

    if (1U == err_type)
    {
        r_csi20_callback_error(err_type);    /* overrun error occurs */
    }
    else
    {
        if (g_csi20_tx_count > 0U)
        {
            if (0U != gp_csi20_rx_address)
            {
                *gp_csi20_rx_address = SIO20;
                gp_csi20_rx_address++;
            }
            else
            {
                sio_dummy = SIO20;
            }

            if (0U != gp_csi20_tx_address)
            {
                SIO20 = *gp_csi20_tx_address;
                gp_csi20_tx_address++;
            }
            else
            {
                SIO20 = 0xFFU;
            }

            g_csi20_tx_count--;
        }
        else 
        {
            if (0U == g_csi20_tx_count)
            {
                if (0U != gp_csi20_rx_address)
                {
                    *gp_csi20_rx_address = SIO20;
                }
                else
                {
                    sio_dummy = SIO20;
                }
            }

            r_csi20_callback_sendend();    /* complete send */
            r_csi20_callback_receiveend();    /* complete receive */
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_csi20_callback_receiveend
* Description  : This function is a callback function when CSI20 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi20_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_callback_error
* Description  : This function is a callback function when CSI20 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_csi20_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_callback_sendend
* Description  : This function is a callback function when CSI20 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi20_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_CSI20TxEnd = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
