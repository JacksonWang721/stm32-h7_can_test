/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include<string.h>

#include <Cpu/Std/IfxCpu.h>

#include "canfd_fifo.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*Rx Data FIFO-----------------------------*/
/*-----------------------------------------*/
#define FRAME_DATA_FIXED_LEN        64U
typedef struct tagRxDataFifoItem
{
    FRAME_ID_TYPE FrameId;
    FRAME_DATA_TYPE FrameData[FRAME_DATA_FIXED_LEN];
    FRAME_LEN_TYPE DataLen;

}RX_DATA_FIFO_ITEM, * PTR_RX_DATA_FIFO_ITEM;

#define RX_DATA_FIFO_DEPTH          64U
typedef struct tagRxDataFifo
{
    RX_DATA_FIFO_ITEM sFrameDataBuff[RX_DATA_FIFO_DEPTH];
    volatile RX_DATA_FIFO_PARA_TYPE FIFOHead;
    volatile RX_DATA_FIFO_PARA_TYPE FIFOTail;
    volatile RX_DATA_FIFO_PARA_TYPE FIFOCount;

}RX_DATA_FIFO, * PTR_RX_DATA_FIFO;

#define ENTER_CANFD_CRITICAL_SECTION()       boolean IsrEn = IfxCpu_disableInterrupts()
#define EXIT_CANFD_CRITICAL_SECTION()        IfxCpu_restoreInterrupts(IsrEn)
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static RX_DATA_FIFO m_sRxComDataFifo;
static RX_DATA_FIFO m_sRxDcmDataFifo;
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* INLINE DEFINE                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static Std_ReturnType canfd_rx_data_fifo_readout_head(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pData, FRAME_LEN_TYPE * pLen);
static Std_ReturnType canfd_rx_data_fifo_read_head(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pData, FRAME_LEN_TYPE * pLen);

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_data_read                                              */
/* FUNCTION   : canfd_data_read                                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     : The user must set the data buffer big enough.               */
/*--------------------------------------------------------------------------*/
Std_ReturnType canfd_data_read(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pDest, FRAME_LEN_TYPE * pLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;

    ENTER_CANFD_CRITICAL_SECTION();

    uRetVal = canfd_rx_data_fifo_readout_head(pId, pDest, pLen);

    EXIT_CANFD_CRITICAL_SECTION();

    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_data_read_no_step                                              */
/* FUNCTION   : canfd_data_read_no_step                                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     : The user must set the data buffer big enough.               */
/*--------------------------------------------------------------------------*/
Std_ReturnType canfd_data_read_no_step(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pDest, FRAME_LEN_TYPE * pLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;

    ENTER_CANFD_CRITICAL_SECTION();

    uRetVal = canfd_rx_data_fifo_read_head(pId, pDest, pLen);

    EXIT_CANFD_CRITICAL_SECTION();

    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_rx_data_fifo_init                                      */
/* FUNCTION   : canfd_rx_data_fifo_init                                      */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void canfd_rx_data_fifo_init(void)
{
    memset(&m_sRxComDataFifo, 0, sizeof(RX_DATA_FIFO));
    memset(&m_sRxDcmDataFifo, 0, sizeof(RX_DATA_FIFO));
}

PTR_RX_DATA_FIFO canfd_get_fifo_type(FRAME_ID_TYPE Id)
{
    PTR_RX_DATA_FIFO p_RxDataFifo_pst = NULL;
    if((Id == UDS_PHY_ADDR) || (Id == UDS_FUNC_ADDR))
    {
        p_RxDataFifo_pst = &m_sRxComDataFifo;
    }
    else
    {
        p_RxDataFifo_pst = &m_sRxDcmDataFifo;
    }

    return p_RxDataFifo_pst;
}
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_rx_data_fifo_insert_tail                               */
/* FUNCTION   : canfd_rx_data_fifo_insert_tail                               */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType canfd_rx_data_fifo_insert_tail(FRAME_ID_TYPE Id, FRAME_DATA_TYPE const * pData, FRAME_LEN_TYPE uLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    PTR_RX_DATA_FIFO p_RxDataFifo_pst = canfd_get_fifo_type(Id);

    if((pData != NULL) && (uLen <= FRAME_DATA_FIXED_LEN))
    {
        memcpy(p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOTail].FrameData, pData, uLen);
        p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOTail].FrameId = Id;
        p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOTail].DataLen = uLen;

        if(p_RxDataFifo_pst->FIFOTail < RX_DATA_FIFO_DEPTH - 1)
        {
            p_RxDataFifo_pst->FIFOTail++;
        }
        else
        {
            p_RxDataFifo_pst->FIFOTail= 0;
        }

        p_RxDataFifo_pst->FIFOCount = (p_RxDataFifo_pst->FIFOCount < RX_DATA_FIFO_DEPTH) ? p_RxDataFifo_pst->FIFOCount + 1 : RX_DATA_FIFO_DEPTH;

        uRetVal = E_OK;
    }
    else
    {
        uRetVal = E_NOT_OK;
    }

    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_rx_data_fifo_readout_head                              */
/* FUNCTION   : canfd_rx_data_fifo_readout_head                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
static Std_ReturnType canfd_rx_data_fifo_readout_head(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pData, FRAME_LEN_TYPE * pLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    PTR_RX_DATA_FIFO p_RxDataFifo_pst = canfd_get_fifo_type(Id);

    if((p_RxDataFifo_pst->FIFOCount > 0) && (pData != NULL) && (pLen != NULL))
    {
        memcpy(pData, p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].FrameData, p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].DataLen);
        * pId = p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].FrameId;
        * pLen = p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].DataLen;

        if(p_RxDataFifo_pst->FIFOHead < RX_DATA_FIFO_DEPTH - 1)
        {
            p_RxDataFifo_pst->FIFOHead++;
        }
        else
        {
            p_RxDataFifo_pst->FIFOHead = 0;
        }

        p_RxDataFifo_pst->FIFOCount--;

        uRetVal = E_OK;
    }
    else
    {
        uRetVal = E_NOT_OK;
    }

    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_rx_data_fifo_read_head                              */
/* FUNCTION   : canfd_rx_data_fifo_read_head                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
static Std_ReturnType canfd_rx_data_fifo_read_head(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pData, FRAME_LEN_TYPE * pLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    PTR_RX_DATA_FIFO p_RxDataFifo_pst = canfd_get_fifo_type(Id);
    
    if((p_RxDataFifo_pst->FIFOCount > 0) && (pData != NULL) && (pLen != NULL))
    {
        memcpy(pData, p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].FrameData, p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].DataLen);
        * pId = p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].FrameId;
        * pLen = p_RxDataFifo_pst->sFrameDataBuff[p_RxDataFifo_pst->FIFOHead].DataLen;

        uRetVal = E_OK;
    }
    else
    {
        uRetVal = E_NOT_OK;
    }

    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : canfd_rx_data_fifo_step_head                              */
/* FUNCTION   : canfd_rx_data_fifo_step_head                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType canfd_rx_data_fifo_step_head(void)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    PTR_RX_DATA_FIFO p_RxDataFifo_pst = canfd_get_fifo_type(Id);
    
    if(p_RxDataFifo_pst->FIFOCount > 0)
    {
        if(p_RxDataFifo_pst->FIFOHead < RX_DATA_FIFO_DEPTH - 1)
        {
            p_RxDataFifo_pst->FIFOHead++;
        }
        else
        {
            p_RxDataFifo_pst->FIFOHead = 0;
        }

        p_RxDataFifo_pst->FIFOCount--;

        uRetVal = E_OK;
    }
    else
    {
        uRetVal = E_NOT_OK;
    }

    return uRetVal;
}

