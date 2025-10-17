/*
* Copyright (c) 2020, imotion.ai
* All Rights Reserved.
* Author: mosc
* File: can_if.c
* Description:
* History:
* 2020-08-06, original
* 2020-08-06, these only used for CAN_FD
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include "std_type.h"
#include "can_tp.h"
#include "can_if.h"
#include "can_fifo.h"
#include "com.h"
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void can_if_receive_indication();
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_if_init                                                 */
/* FUNCTION   : can_if_init                                                 */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     : support can fd , and use fifo for simple                    */
/*--------------------------------------------------------------------------*/
void can_if_init(void)
{

}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_if_mainfunction                                         */
/* FUNCTION   : can_if_mainfunction                                         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_if_mainfunction(void)
{
    uint32 t_id_u32;
    uint8 t_data_pu8[64];
    uint8 t_len_u8;

    while(canfd_data_read(&t_id_u32, t_data_pu8, &t_len_u8) == E_OK)
    {
        if(t_id_u32 == UDS_PHY_ADDR) || (t_id_u32 == UDS_FUNC_ADDR)
        {
            can_tp_onrecv_data(t_data_pu8, t_len_u8, PROTOCAL_FLAG_PHY);
        }
        else
        {
            com_provide_rx_buffer_enm(t_data_pu8, t_len_u8, t_id_u32);
        }
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_if_receive                                              */
/* FUNCTION   : can_if_receive                                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
#define UDS_PHY_ADDR 0x7DF
#define UDS_FUNC_ADDR 0x7E0
void can_if_receive_indication(uint32 p_id_u32, uint8 * p_data_pu8, uint8 p_len_u8)
{
    if(p_id_u32 == UDS_PHY_ADDR) || (p_id_u32 == UDS_FUNC_ADDR)
    {
        canfd_rx_data_fifo_insert_tail(p_id_u32, p_data_pu8, p_len_u8);
    }
    else
    {
        canfd_rx_data_fifo_insert_tail(p_id_u32, p_data_pu8, p_len_u8);
    }
}


/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_if_com_transmit                                         */
/* FUNCTION   : can_if_com_transmit                                         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     : only used for CAN channel 3                                 */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_if_transmit()
{

}

void can_status_check()
{

}

boolean can_if_get_transmit_status()
{

}

boolean can_if_cancel_transmit()
{

}
