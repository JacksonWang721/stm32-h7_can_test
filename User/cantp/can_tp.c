/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "std_type.h"
#include "can_tp.h"
#include "event.h"
#include "dsl.h"
#include "can_if.h"
#include "can_if_cfg.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define TX_MAX_LENGTH                       4096U
#define CAN_FRAME_LEN                       8U
#define SIGNAL_FRAME_DATA_LENGTH            7U
#define FIRST_FRAME_DATA_LENGTH             6U
#define CONSECUTIVE_FRAME_DATA_LENGTH       7U
#define SF_DATA_START_BYTE                  1U
#define FF_DATA_START_BYTE                  2U
#define CF_DATA_START_BYTE                  1U
#define FC_DATA_START_BYTE                  3U
#define SN_MAX                              0x0F
#define EMPTY_BYTE_PADDING                  0xAA

#define BS_BRING_IT_ON                      0x00
#define ST_MIN_BRING_IT_ON                  0x00
#define ST_MIN_LITTLE_FAST                  0x02
#define ST_MIN_REASONABLE                   0x05

#define SF_FF_DL_LENGTH                     7U
#define SF_MIN_DL_LENGTH                    1U
/*FS values of FC frame*/
#define CTS             0
#define WT              1
#define OVFLW           2

/*STmin scaler unit, cos event proc in 2ms scheduler*/
#define EVENT_SCALER_UNIT       2

/*Parameter of diagnostic network layer in 2ms task*/
#define N_As   12
#define N_Ar   12
#define N_Bs   40   
#define N_Cr   75
typedef union tagCanTpNPCI
{
    struct
    {
        unsigned char                       :4;
        unsigned char   uN_PCItype          :4;

        unsigned char   u_KeepSession_L     :8;
        unsigned char   u_KeepSession_H     :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;

    }sF;    
        
    struct
    {
        unsigned char   uSF_DL              :4;
        unsigned char   uN_PCItype          :4;
        
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;

    }sSF;
        
    struct
    {
        unsigned char   uFF_DL_H            :4;
        unsigned char   uN_PCItype          :4;
        
        unsigned char   uFF_DL_L            :8;
        
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;

    }sFF;       
        
    struct
    {
        unsigned char   uSN                 :4;
        unsigned char   uN_PCItype          :4;
        
        unsigned char                       :8;         
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;

    }sCF;
        
    struct
    {
        unsigned char   uFS                 :4;
        unsigned char   uN_PCItype          :4;
        
        unsigned char   uBS                 :8;
        
        unsigned char   uSTmin              :8;
        
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;
        unsigned char                       :8;

    }sFC;
    
    uint32 ulFrame[2];  /* for 4-byte alignment */
    
}CAN_TP_FRAME, * LP_CAN_TP_FRAME;

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
uint8 * m_pRxDataPtr = NULL;
uint16 m_uRxTotalLen = 0;

const uint8 * m_pTxDataPtr = NULL;
uint16 m_uTxTotalLen = 0;

uint8 m_bySN = 0xFF;

PROTOCAL_FLAG_TYPE m_uCantpTranfer = PROTOCAL_FLAG_FALSE;

uint16 m_uCrMaxCount = 0;
uint16 m_uBsMaxCount = 0;
uint16 m_uBs2MaxCount = 0;
uint16 m_uAsMaxCount = 0;
PROTOCAL_FLAG_TYPE m_uN_CrTO_Monitor = PROTOCAL_FLAG_FALSE;
PROTOCAL_FLAG_TYPE m_uN_BsTO_Monitor = PROTOCAL_FLAG_FALSE;
PROTOCAL_FLAG_TYPE m_uN_Bs2TO_Monitor = PROTOCAL_FLAG_FALSE;
PROTOCAL_FLAG_TYPE m_uN_AsTO_Monitor = PROTOCAL_FLAG_FALSE;

CANTP_STATUS m_enmCanTpStatus = IDLE;
FRAME_TYPE m_enmFrameType = IDLE_TYPE;

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_onrecv_sf(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr);
Std_ReturnType can_tp_onrecv_fc(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr);
Std_ReturnType can_tp_onrecv_ff(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr);
Std_ReturnType can_tp_onrecv_cf(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr);

Std_ReturnType can_tp_send_sf(const uint8 * pDataBuffer,uint16 uDataLen);
Std_ReturnType can_tp_send_ff(const uint8 * pDataBuffer, uint16 uDataLen);
void can_tp_send_cf(void);
Std_ReturnType can_tp_send_fc(uint8 byFS);
void can_tp_cf_block_finish_callback(void);
Std_ReturnType can_tp_do_send_data(const uint8 * pDataBuffer, uint16 uDataLen);

void can_tp_set_N_Cr_status(PROTOCAL_FLAG_TYPE IsStart);
void can_tp_set_N_Bs_status(PROTOCAL_FLAG_TYPE IsStart);
void can_tp_set_N_Bs2_status(PROTOCAL_FLAG_TYPE IsStart);
void can_tp_set_N_As_status(PROTOCAL_FLAG_TYPE IsStart);
void can_tp_N_Cr_timeout_monitor(void);
void can_tp_N_Bs_timeout_monitor(void);
void can_tp_N_Bs2_timeout_monitor(void);
void can_tp_N_As_timeout_monitor(void);
CANTP_STATUS can_tp_get_TP_status(void);
void can_tp_set_TP_status(CANTP_STATUS tpstatus);
FRAME_TYPE can_tp_get_frame_type(void);
void can_tp_set_frame_type(FRAME_TYPE frametype);
void can_tp_clear_data(void);
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_init                                                 */
/* FUNCTION   : This is the initialization function of the CAN TP module    */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_init(void)
{
    m_pRxDataPtr = NULL;
    m_uRxTotalLen = 0;
    m_pTxDataPtr = NULL;
    m_uTxTotalLen = 0;
    m_bySN = 0xFF;
    m_enmCanTpStatus = IDLE;
    m_enmFrameType = IDLE_TYPE;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_onrecv_data                                          */
/* FUNCTION   : analize received data by N_PCI.                             */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_onrecv_data(const uint8 * pData, uint8 uDataLen, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    CANTP_STATUS CanTpStatus;
    uint8 KeepSessionL = 0;
    uint8 KeepSessionH = 0;
    
    if((pData != NULL) && (uDataLen == sizeof(CAN_TP_FRAME)))
    {
        LP_CAN_TP_FRAME pTPParm = (LP_CAN_TP_FRAME)pData;
        CanTpStatus = can_tp_get_TP_status();

        switch(pTPParm->sF.uN_PCItype)
        {
            case PCI_TYPE_SINGLE_FRAME:
            {
                KeepSessionL = pTPParm->sF.u_KeepSession_L;
                KeepSessionH = pTPParm->sF.u_KeepSession_H;
                if(KeepSessionL == 0x3E)
                {
                    //do nothing
                }
                else
                {
                    if(CanTpStatus == MULTIFRAMEFRAME_STATUS_RX)
                    {
                        can_tp_clear_data();
                        can_tp_set_TP_status(IDLE);
                    }
                    else
                    {}
                }
                can_tp_set_frame_type(SINGLE_FRAME_TYPE);
                uRetVal = can_tp_onrecv_sf(pTPParm, IsPhyAddr);
                
                break;
            }
            case PCI_TYPE_FLOW_CONTROL:
            {
                if((CanTpStatus == MULTIFRAMEFRAME_STATUS_RX) || (isfunaddr == TRUE))
                {
                    //do nothing
                }
                else
                {
                    if(can_tp_get_frame_type() == FLOW_CONTROL_TYPE)
                    {
                        //do nothing
                    }
                    else
                    {
                        can_tp_set_frame_type(FLOW_CONTROL_TYPE);
                        uRetVal = can_tp_onrecv_fc(pTPParm, IsPhyAddr);
                    }
                }

                break;
            }
            case PCI_TYPE_FIRST_FRAME:
            {
                if(CanTpStatus == MULTIFRAMEFRAME_STATUS_RX)
                {
                    can_tp_clear_data();
                    can_tp_set_TP_status(IDLE);
                }
                else
                {}
                can_tp_set_frame_type(FIRST_FRAME_TYPE);
                uRetVal = can_tp_onrecv_ff(pTPParm, IsPhyAddr);
                
                break;
            }
            case PCI_TYPE_CONSECUTIVE_FRAME:
            {
                if(CanTpStatus == MULTIFRAMEFRAME_STATUS_TX)
                {
                    //do nothing
                }
                else
                {
                    can_tp_set_frame_type(CONSECUTIVE_FRAME_TYPE);
                    uRetVal = can_tp_onrecv_cf(pTPParm, IsPhyAddr);
                }
                
                break;
            }
            default:
            {
                can_tp_set_frame_type(IDLE_TYPE);
                break;
            }
        }
    }
    else
    {
        uRetVal = E_NOT_OK;
    }

    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_onrecv_sf                                            */
/* FUNCTION   : Process UDS physical data SingleFrame                       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_onrecv_sf(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    
    if(dsl_frame_rx_prehandle(PCI_TYPE_SINGLE_FRAME, (uint8 *)pTPParm, IsPhyAddr) == E_OK)
    {
        uint8 * pRxBufferRequested = NULL;
        
        if(dsl_provide_rx_buffer(&pRxBufferRequested, pTPParm->sSF.uSF_DL) == E_OK)
        {
            if((pTPParm->sSF.uSF_DL <= SF_FF_DL_LENGTH) && (pTPParm->sSF.uSF_DL >= SF_MIN_DL_LENGTH))
            {
                memcpy(pRxBufferRequested, &((uint8 *)pTPParm)[SF_DATA_START_BYTE], pTPParm->sSF.uSF_DL);
                dsl_rx_indication(PROTOCAL_FLAG_TRUE);
            
                uRetVal = E_OK;
            }
            else
            {
                dsl_rx_indication(PROTOCAL_FLAG_FALSE);
                uRetVal = E_NOT_OK;
            }
        }
        else
        {
            dsl_rx_indication(PROTOCAL_FLAG_FALSE);
            uRetVal = E_NOT_OK;
        }
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_onrecv_fc                                            */
/* FUNCTION   : Process UDS physical data FlowControl                       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_onrecv_fc(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_OK; /* default E_OK */
    uint8 Timer = 0;
    if(dsl_frame_rx_prehandle(PCI_TYPE_FLOW_CONTROL, (uint8 *)pTPParm, IsPhyAddr) == E_OK)
    {
        can_tp_set_N_Bs_status(PROTOCAL_FLAG_FALSE);

        switch(pTPParm->sFC.uFS)
        {
            case CTS:
            {
                if( m_uTxTotalLen > 0 )
                {
                    /*1-7 bytes need 1 frame*/
                    uint16 uCfTotalFramesNeeded = (m_uTxTotalLen - 1) / CONSECUTIVE_FRAME_DATA_LENGTH + 1;
                    
                    Timer = pTPParm->sFC.uSTmin;
                    if(Timer >= 0x7F)
                    {
                       if((Timer >= 0xF1) && (Timer <= 0xF9))
                       {
                           Timer = 0;
                       }
                       else
                       {    
                           /*should 0x7F*/
                           Timer = 0x8C;
                       }
                    }
                    else
                    {
                        Timer = pTPParm->sFC.uSTmin;
                    }
                    uint8 uSTmin = (Timer / EVENT_SCALER_UNIT) * EVENT_SCALER_UNIT;
                    
                    if(pTPParm->sFC.uBS == 0)
                    {       
                        event_start(&can_tp_send_cf, uCfTotalFramesNeeded, uSTmin, &can_tp_cf_block_finish_callback);
                    }
                    else
                    {                               
                        if(uCfTotalFramesNeeded <= pTPParm->sFC.uBS)
                        {       
                            event_start(&can_tp_send_cf, uCfTotalFramesNeeded, uSTmin, &can_tp_cf_block_finish_callback);
                        }
                        else
                        {
                            event_start(&can_tp_send_cf, pTPParm->sFC.uBS, uSTmin, &can_tp_cf_block_finish_callback);
                        }
                    }
                }
                
                break;
            }
            case WT:
            {
                can_tp_set_frame_type(IDLE_TYPE);
                break;
            }
            case OVFLW:
            {
                /*abort transmission*/
                m_pTxDataPtr = NULL;
                m_uTxTotalLen = 0;
                m_bySN = 0xFF;
                dsl_tx_comfirmation();
                
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    {
        /* maybe the fc is for other ecu, do nothing */
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_onrecv_ff                                            */
/* FUNCTION   : Process UDS physical data FirstFrame                        */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_onrecv_ff(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    
    uint8 * pRxBufferRequested = NULL;
    
    uint16 uTotalBytes = pTPParm->sFF.uFF_DL_H;
    uTotalBytes <<= 8;
    uTotalBytes += pTPParm->sFF.uFF_DL_L;

    if(dsl_frame_rx_prehandle(PCI_TYPE_FIRST_FRAME, (uint8 *)pTPParm, IsPhyAddr) == E_OK)
    {
        if(dsl_provide_rx_buffer(&pRxBufferRequested, uTotalBytes) == E_OK)
        {
            if(uTotalBytes > SF_FF_DL_LENGTH)
            {
                memcpy(pRxBufferRequested, &((uint8 *)pTPParm)[FF_DATA_START_BYTE], FIRST_FRAME_DATA_LENGTH);
            
                m_pRxDataPtr = pRxBufferRequested + FIRST_FRAME_DATA_LENGTH;
                m_uRxTotalLen = uTotalBytes - FIRST_FRAME_DATA_LENGTH;
                m_bySN = 1;
                
                uRetVal = can_tp_send_fc(CTS);
                /*only CTS need N_Cr monitor*/
                can_tp_set_N_Cr_status(PROTOCAL_FLAG_TRUE);
                
                can_tp_set_TP_status(MULTIFRAMEFRAME_STATUS_RX);
            }
            else
            {
                can_tp_clear_data();
                uRetVal = E_NOT_OK;
            }
        }
        else
        {
            if(m_enmCanTpStatus == MULTIFRAMEFRAME_STATUS_TX)
            {
                dsl_rx_indication(PROTOCAL_FLAG_FALSE);
                
                m_pRxDataPtr = NULL;
                m_uRxTotalLen = 0;
            }
            else
            {
                can_tp_send_fc(OVFLW);
                dsl_rx_indication(PROTOCAL_FLAG_FALSE);
                
                m_pRxDataPtr = NULL;
                m_uRxTotalLen = 0;
                m_bySN = 0xFF;
            }

            
            uRetVal = E_NOT_OK;
        }
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_onrecv_cf                                            */
/* FUNCTION   : Process UDS physical data ConsecutiveFrame                  */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_onrecv_cf(const LP_CAN_TP_FRAME pTPParm, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    uint8 uSN = pTPParm->sCF.uSN;
    
    if(dsl_frame_rx_prehandle(PCI_TYPE_CONSECUTIVE_FRAME, (uint8 *)pTPParm, IsPhyAddr) == E_OK)
    {
        if(m_bySN == uSN)
        {
            if(m_pRxDataPtr != NULL)
            {
                uint8 * pRxBufferRequested = m_pRxDataPtr;
                uint8 uLen = (m_uRxTotalLen > CONSECUTIVE_FRAME_DATA_LENGTH) ? CONSECUTIVE_FRAME_DATA_LENGTH : m_uRxTotalLen;
                
                memcpy(pRxBufferRequested, &((uint8 *)pTPParm)[CF_DATA_START_BYTE], uLen);
                
                m_pRxDataPtr = pRxBufferRequested + uLen;
                m_uRxTotalLen -= uLen;
                m_bySN = (m_bySN + 1 > SN_MAX) ? 0 : (m_bySN + 1); 
                
                if(m_uRxTotalLen == 0)
                {
                    dsl_rx_indication(PROTOCAL_FLAG_TRUE);
                    
                    m_pRxDataPtr = NULL;
                    m_uRxTotalLen = 0;
                    m_bySN = 0xFF;

                    can_tp_set_N_Cr_status(PROTOCAL_FLAG_FALSE);
                    can_tp_set_TP_status(IDLE);
                }
                else
                {
                    can_tp_set_N_Cr_status(PROTOCAL_FLAG_TRUE);
                }
                
                uRetVal = E_OK;
            }
            else
            {
                /* this will happen when this ecu is in session and transmitting cf, 
                the tester sends cf by mistaken, and the SN fits by chance*/
                uRetVal = E_NOT_OK;
            }
        }
        else
        {
            dsl_rx_indication(PROTOCAL_FLAG_FALSE);
            
            m_pRxDataPtr = NULL;
            m_uRxTotalLen = 0;
            m_bySN = 0xFF;
            
            dsl_tx_comfirmation();  //clear dcom's rx/tx buffer
            
            uRetVal = E_NOT_OK;
        }
    }
    else
    {
        /* maybe the cf is for other ecu, do nothing */
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_transmit                                             */
/* FUNCTION   : Send the can tp data                                        */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     : Make sure data in buffer will not modify                    */
/*            : until dsl_tx_comfirmation is called                         */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_transmit(const uint8 * pData, uint16 uDataLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;

    if((pData != NULL) && (uDataLen > 0))
    {
        if(uDataLen < CAN_FRAME_LEN)
        {
            uRetVal = can_tp_send_sf(pData, uDataLen);
            dsl_tx_comfirmation();
        }
        else if(uDataLen < TX_MAX_LENGTH)
        {
            uRetVal = can_tp_send_ff(pData, uDataLen);
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_send_sf                                              */
/* FUNCTION   : Send the can tp single frame                                */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_send_sf(const uint8 * pDataBuffer, uint16 uDataLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    uint8 uBytes = (uDataLen < CAN_FRAME_LEN) ? uDataLen : (CAN_FRAME_LEN - 1);

    if(pDataBuffer != NULL)
    {
        CAN_TP_FRAME sTPFrame;
    
        memset(&sTPFrame, 0x00, sizeof(CAN_TP_FRAME));
        
        sTPFrame.sSF.uN_PCItype = PCI_TYPE_SINGLE_FRAME;
        sTPFrame.sSF.uSF_DL = uBytes;

        memset(&((uint8 *)(&sTPFrame))[SF_DATA_START_BYTE], EMPTY_BYTE_PADDING, SIGNAL_FRAME_DATA_LENGTH);
        memcpy(&((uint8 *)(&sTPFrame))[SF_DATA_START_BYTE], pDataBuffer, uBytes);
        
        uRetVal = can_tp_do_send_data(&sTPFrame, CAN_FRAME_LEN);
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_send_ff                                              */
/* FUNCTION   : Send the can tp first frame                                 */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_send_ff(const uint8 * pDataBuffer, uint16 uDataLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    uint16 uBytes = (uDataLen < TX_MAX_LENGTH) ? uDataLen : (TX_MAX_LENGTH - 1);

    if(pDataBuffer != NULL && uBytes >= FIRST_FRAME_DATA_LENGTH)
    {
        CAN_TP_FRAME sTPFrame;
    
        memset(&sTPFrame, 0x00, sizeof(CAN_TP_FRAME));
        
        sTPFrame.sFF.uN_PCItype = PCI_TYPE_FIRST_FRAME;
        sTPFrame.sFF.uFF_DL_H = (uint8)((uBytes >> 8) & 0x0F);
        sTPFrame.sFF.uFF_DL_L = (uint8)(uBytes & 0xFF);
        
        memcpy(&((uint8 *)(&sTPFrame))[FF_DATA_START_BYTE], pDataBuffer, FIRST_FRAME_DATA_LENGTH);
        
        uRetVal = can_tp_do_send_data(&sTPFrame, CAN_FRAME_LEN);
        can_tp_set_N_As_status(PROTOCAL_FLAG_TRUE);
        can_tp_set_N_Bs_status(PROTOCAL_FLAG_TRUE);
        can_tp_set_TP_status(MULTIFRAMEFRAME_STATUS_TX);
        
        m_pTxDataPtr = pDataBuffer + FIRST_FRAME_DATA_LENGTH;
        m_uTxTotalLen = uBytes - FIRST_FRAME_DATA_LENGTH;
        m_bySN = 1;     /* SN of FF is 0 */

        
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_send_cf                                              */
/* FUNCTION   : Send the can tp consecutive frame                           */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_send_cf(void)
{
    uint8 uSendBytes = 0;
    CAN_TP_FRAME sTPFrame;

    if(m_pTxDataPtr != NULL)
    {
        memset(&sTPFrame, 0x00, sizeof(CAN_TP_FRAME));
        
        sTPFrame.sCF.uN_PCItype = PCI_TYPE_CONSECUTIVE_FRAME;
        sTPFrame.sCF.uSN = m_bySN;
        
        if(m_uTxTotalLen > CONSECUTIVE_FRAME_DATA_LENGTH)
        {
            uSendBytes = CONSECUTIVE_FRAME_DATA_LENGTH;
        }
        else
        {
            uSendBytes = m_uTxTotalLen;
            memset(&((uint8 *)(&sTPFrame))[CF_DATA_START_BYTE], EMPTY_BYTE_PADDING, CONSECUTIVE_FRAME_DATA_LENGTH);
        }
        memcpy(&((uint8 *)(&sTPFrame))[CF_DATA_START_BYTE], m_pTxDataPtr, uSendBytes);
        
        can_tp_do_send_data(&sTPFrame, CAN_FRAME_LEN);
        can_tp_set_N_As_status(PROTOCAL_FLAG_TRUE);
        
        m_pTxDataPtr = m_pTxDataPtr + uSendBytes;
        m_uTxTotalLen = m_uTxTotalLen - uSendBytes;
        m_bySN = (m_bySN + 1 > SN_MAX) ? 0 : (m_bySN + 1);
    }
    else
    {
        /* do nothing */
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_send_fc                                              */
/* FUNCTION   : Send the can tp flow control frame                          */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_send_fc(uint8 byFS)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    CAN_TP_FRAME sTPFrame;

    memset(&sTPFrame, EMPTY_BYTE_PADDING, sizeof(CAN_TP_FRAME));
    
    sTPFrame.sFC.uN_PCItype = PCI_TYPE_FLOW_CONTROL;
    sTPFrame.sFC.uFS = byFS;
    sTPFrame.sFC.uBS = BS_BRING_IT_ON;
    sTPFrame.sFC.uSTmin = ST_MIN_BRING_IT_ON;
    
    uRetVal = can_tp_do_send_data(&sTPFrame, CAN_FRAME_LEN);
    can_tp_set_N_Bs2_status(PROTOCAL_FLAG_TRUE);
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_cf_block_finish_callback                             */
/* FUNCTION   : Send the can tp consecutive frame                           */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_cf_block_finish_callback(void)
{
    if(m_uTxTotalLen == 0)
    {
        m_pTxDataPtr = NULL;
        m_bySN = 0xFF;

        can_tp_set_TP_status(IDLE);
        dsl_tx_comfirmation();
    }
    else
    {
        /* do nothing */
    }
    can_tp_set_frame_type(IDLE_TYPE);
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_do_send_data                                         */
/* FUNCTION   : Send the can tp's frame                                     */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType can_tp_do_send_data(const uint8 * pDataBuffer, uint16 uDataLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    
    if((pDataBuffer != NULL) && (uDataLen <= CAN_FRAME_LEN))
    {
        uRetVal = can_if_dcm_transmit(CAN_IF_DIAG_RESP_FCM_TX,pDataBuffer,uDataLen);
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}


/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_final                                                */
/* FUNCTION   : This is the final function of the can tp module.            */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_final(void)
{
    m_pRxDataPtr = NULL;
    m_uRxTotalLen = 0;
    m_pTxDataPtr = NULL;
    m_uTxTotalLen = 0;
    m_bySN = 0xFF;
}


/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_monitor                                              */
/* FUNCTION   : indicates the Tx bufferd data has been send successfully    */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_monitor(void)
{
    can_tp_N_As_timeout_monitor();
    can_tp_N_Bs_timeout_monitor();
    can_tp_N_Bs2_timeout_monitor();
    can_tp_N_Cr_timeout_monitor();
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_N_As_timeout_monitor                                 */
/* FUNCTION   : This is the TO check function of the can tp module.         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_N_As_timeout_monitor(void)
{
    /*get bus heavy/bus off...*/
    boolean uRetVal = can_if_get_transmit_status(CAN_IF_DIAG_RESP_FCM_TX);
    if((m_uN_AsTO_Monitor == PROTOCAL_FLAG_TRUE) && (uRetVal == TRUE))
    {
        m_uAsMaxCount++;
        if(m_uAsMaxCount >= N_As)
        {
            can_tp_clear_data();
            /*clear register*/
            (void)can_if_cancel_transmit(CAN_IF_DIAG_RESP_FCM_TX);
            m_uN_AsTO_Monitor = PROTOCAL_FLAG_FALSE;
            
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        m_uN_AsTO_Monitor = PROTOCAL_FLAG_FALSE;
        m_uAsMaxCount = 0;
    }

}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_N_Bs_timeout_monitor                                 */
/* FUNCTION   : This is the TO check function of the can tp module.         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_N_Bs_timeout_monitor(void)
{
    if(m_uN_BsTO_Monitor == PROTOCAL_FLAG_TRUE)
    {
        m_uBsMaxCount++;
        if(m_uBsMaxCount >= N_Bs)
        {
            can_tp_clear_data();
            m_uN_BsTO_Monitor = PROTOCAL_FLAG_FALSE;
            
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        //do nothing
    }

}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_N_Bs_timeout_monitor                                 */
/* FUNCTION   : This is the TO check function of the can tp module.         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_N_Bs2_timeout_monitor(void)
{
    boolean uRetVal = can_if_get_transmit_status(CAN_IF_DIAG_RESP_FCM_TX);
    if((m_uN_Bs2TO_Monitor == PROTOCAL_FLAG_TRUE) && (uRetVal == TRUE))
    {
        m_uBs2MaxCount++;
        if(m_uBs2MaxCount >= N_Bs)
        {
            can_tp_clear_data();
            (void)can_if_cancel_transmit(CAN_IF_DIAG_RESP_FCM_TX);
            m_uN_Bs2TO_Monitor = PROTOCAL_FLAG_FALSE;
            
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        m_uN_Bs2TO_Monitor = PROTOCAL_FLAG_FALSE;
        m_uBs2MaxCount = 0;
    }

}


/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : can_tp_N_Cr_timeout_monitor                                 */
/* FUNCTION   : This is the TO check function of the can tp module.         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_N_Cr_timeout_monitor(void)
{
    if(m_uN_CrTO_Monitor == PROTOCAL_FLAG_TRUE)
    {
        m_uCrMaxCount++;
        if(m_uCrMaxCount >= N_Cr)
        {
            can_tp_clear_data();
            m_uN_CrTO_Monitor = PROTOCAL_FLAG_FALSE;
            
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        //do nothing
    }
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_set_N_As_status                                      */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_set_N_As_status(PROTOCAL_FLAG_TYPE IsStart)
{
    /*Timeout monitor start*/
    if(IsStart == PROTOCAL_FLAG_TRUE)
    {
        m_uN_AsTO_Monitor = PROTOCAL_FLAG_TRUE; 
    }
    else
    {
        m_uN_AsTO_Monitor = PROTOCAL_FLAG_FALSE;
    }
    m_uAsMaxCount = 0;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_set_N_Bs_status                                      */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_set_N_Bs_status(PROTOCAL_FLAG_TYPE IsStart)
{
    /*Timeout monitor start*/
    if(IsStart == PROTOCAL_FLAG_TRUE)
    {
        m_uN_BsTO_Monitor = PROTOCAL_FLAG_TRUE; 
    }
    else
    {
        m_uN_BsTO_Monitor = PROTOCAL_FLAG_FALSE;
    }
    m_uBsMaxCount = 0;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_set_N_Bs_status                                      */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_set_N_Bs2_status(PROTOCAL_FLAG_TYPE IsStart)
{
    /*Timeout monitor start*/
    if(IsStart == PROTOCAL_FLAG_TRUE)
    {
        m_uN_Bs2TO_Monitor = PROTOCAL_FLAG_TRUE; 
    }
    else
    {
        m_uN_Bs2TO_Monitor = PROTOCAL_FLAG_FALSE;
    }
    m_uBs2MaxCount = 0;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_set_N_Cr_status                                      */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_set_N_Cr_status(PROTOCAL_FLAG_TYPE IsStart)
{
    /*Timeout monitor start*/
    if(IsStart == PROTOCAL_FLAG_TRUE)
    {
        m_uN_CrTO_Monitor = PROTOCAL_FLAG_TRUE;
        
    }
    else
    {
        m_uN_CrTO_Monitor = PROTOCAL_FLAG_FALSE;
    }
    m_uCrMaxCount = 0;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       :  can_tp_set_TP_status                                       */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_set_TP_status(CANTP_STATUS tpstatus)
{
    m_enmCanTpStatus = tpstatus;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_get_TP_status                                        */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
CANTP_STATUS can_tp_get_TP_status(void)
{
    return m_enmCanTpStatus;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       :  can_tp_set_frame_type                                      */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_set_frame_type(FRAME_TYPE frametype)
{
    m_enmFrameType = frametype;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_get_frame_type                                       */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
FRAME_TYPE can_tp_get_frame_type(void)
{
    return m_enmFrameType;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : can_tp_clear_data                                           */
/* FUNCTION   :                                                             */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : WangMY                                                      */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void can_tp_clear_data(void)
{
    dsl_rx_indication(PROTOCAL_FLAG_FALSE);
    m_pRxDataPtr = NULL;
    m_uRxTotalLen = 0;
    m_pTxDataPtr = NULL;
    m_uTxTotalLen = 0;
    m_bySN = 0xFF;
    dsl_tx_comfirmation();  //clear dcom's rx/tx buffer
}

