/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "std_type.h"
#include "dsl.h"
#include "can_tp.h"
#include "dsd.h"
#include "dsd_cfg.h"
#include "sw_reset.h"
#include "dsp.h"
#include "boot_mgr.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define DCOM_RX_BUFFER_SIZE         4096
#define DCOM_TX_BUFFER_SIZE         4096

/* S3 timeout, 5s */
#define S3_TIMING_MAX               500
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
uint8 m_uRxBuffer[DCOM_RX_BUFFER_SIZE];
uint8 m_uTxBuffer[DCOM_TX_BUFFER_SIZE];

MSG_CONTEXT dcom_sMsgContext = 
{
    m_uRxBuffer,
    0,
    m_uTxBuffer,
    0
};

SESSION_STATE m_enmSessionState = DEFAULT_SESSION;

SECURITY_LEVEL m_enmSecurityLv = SECURITY_LV_NONE;

uint16 m_uS3MaxCount = 0;

PROTOCAL_FLAG_TYPE m_uNewReqHasArrived = PROTOCAL_FLAG_FALSE;

PROTOCAL_FLAG_TYPE m_uSeverPending = PROTOCAL_FLAG_FALSE;

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* INLINE DEFINE                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
inline void dsl_rst_s3timer(void)
{
    m_uS3MaxCount = 0;
}
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_init                                                    */
/* FUNCTION   : This is the initialization function of the dsl module       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_init(void)
{
    dcom_sMsgContext.pReqDataBuffer = m_uRxBuffer;
    dcom_sMsgContext.uReqDataLen = 0;
    dcom_sMsgContext.pResDataBuffer = m_uTxBuffer;
    dcom_sMsgContext.uResDataLen = 0;
    
    m_enmSessionState = DEFAULT_SESSION;
    m_enmSecurityLv = SECURITY_LV_NONE;
    
    m_uS3MaxCount = 0;
    
    m_uNewReqHasArrived = PROTOCAL_FLAG_FALSE;
    
    m_uSeverPending = PROTOCAL_FLAG_FALSE;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_frame_rx_can_prehandle                                  */
/* FUNCTION   : indicates a new frame has been recvived                     */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType dsl_frame_rx_prehandle(uint8 uFrameType, uint8 const * pFrameData, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    uint8 uDataFirstByte;
    uint8 uDataSecondByte;
    uint8 uDataLength;
    uint8 uResponeLength;
    uint8 testerpresent_buf[3];
    
    if(uFrameType == PCI_TYPE_SINGLE_FRAME)
    {
        uDataFirstByte = * (pFrameData + 1u);
        uDataSecondByte = * (pFrameData + 2u);
    
        if(uDataFirstByte == TESTER_PRESENT)
        {
            uDataLength = *pFrameData;
            if(uDataLength == 2u)
            {
                if(uDataSecondByte == 0x00u)
                {
                    dsl_rst_s3timer();
                    testerpresent_buf[0] = TESTER_PRESENT + 0x40u;
                    testerpresent_buf[1] = 0x00u;
                    uResponeLength = 2u;
                }
                else if(uDataSecondByte == 0x80u)
                {
                    dsl_rst_s3timer();
                    uResponeLength = 0u;
                }
                else
                {
                    if(isfunaddr == TRUE)
                    {
                        uResponeLength = 0u;
                    }
                    else
                    {
                        testerpresent_buf[0] = 0x7Fu;
                        testerpresent_buf[1] = 0x3Eu;
                        testerpresent_buf[2] = 0x12u;
                        uResponeLength = 3u;
                    }
                }
            }
            else
            {
                testerpresent_buf[0] = 0x7Fu;
                testerpresent_buf[1] = 0x3Eu;
                testerpresent_buf[2] = 0x13u;
                uResponeLength = 3u;
            }
            can_tp_transmit(testerpresent_buf, uResponeLength);
            
            uRetVal = E_NOT_OK;
        }
        else
        {
            uRetVal = dsd_query_addressing_supportness(uDataFirstByte, IsPhyAddr);
        }
    }
    else if(uFrameType == PCI_TYPE_FIRST_FRAME)
    {
        uDataSecondByte = * (pFrameData + 2u);
        uRetVal = dsd_query_addressing_supportness(uDataSecondByte, IsPhyAddr);
    }
    else
    {
        /* fc & cf, E_OK means this ecu is in session */
        if(dcom_sMsgContext.uReqDataLen == 0)
        {
            uRetVal = E_NOT_OK;
        }
        else
        {
            uRetVal = E_OK;
        }
    }

    return uRetVal;
}


/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsl_provide_rx_buffer                                       */
/* FUNCTION   : dsl provide rx buffer                                       */
/* RETURN     : int, return 1 if rx buffer is availble                      */
/* ARGUMENT   : [i]pRxBufferPtr - uint8 * *                                 */
/*              [i]uLen - uint16                                            */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType dsl_provide_rx_buffer(uint8 * * pRxBufferPtr, uint16 uLen)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    
    if((pRxBufferPtr != NULL) && (uLen <= DCOM_RX_BUFFER_SIZE))
    {
        if(dcom_sMsgContext.uReqDataLen == 0)
        {
            * pRxBufferPtr = dcom_sMsgContext.pReqDataBuffer;
            
            dcom_sMsgContext.uReqDataLen = uLen;
            
            uRetVal = E_OK;
        }
        else
        {
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
/* NAME       : dsl_rx_indication                                           */
/* FUNCTION   : indicates a new request has been recvived                   */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_rx_indication(PROTOCAL_FLAG_TYPE IsRxSuccess)
{
    if(IsRxSuccess == PROTOCAL_FLAG_TRUE)
    {
        m_uNewReqHasArrived = PROTOCAL_FLAG_TRUE;
    }
    else
    {
        if((m_uNewReqHasArrived == PROTOCAL_FLAG_FALSE) 
            && (dsp_is_service_idle() == PROTOCAL_FLAG_TRUE))
        {
            /*package corrupt during request receiving, discard received data*/
            dcom_sMsgContext.uReqDataLen = 0;
            dcom_sMsgContext.uResDataLen = 0;
        }
        else
        {
            /* illegal data received during server handling, do nothing */
        }
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_pending_transmit                                        */
/* FUNCTION   : This is the tx of the dsl module                            */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_pending_transmit(void)
{
    m_uSeverPending = PROTOCAL_FLAG_TRUE;
    
    can_tp_transmit(dcom_sMsgContext.pResDataBuffer, dcom_sMsgContext.uResDataLen);
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_response_transmit                                       */
/* FUNCTION   : This is the tx of the dsl module                            */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_response_transmit(void)
{
    m_uSeverPending = PROTOCAL_FLAG_FALSE;
    
    can_tp_transmit(dcom_sMsgContext.pResDataBuffer, dcom_sMsgContext.uResDataLen);
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_tx_comfirmation                                         */
/* FUNCTION   : indicates the Tx bufferd data has been send successfully    */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_tx_comfirmation(void)
{
    dcom_sMsgContext.uResDataLen = 0;
    dsd_resp_edit_unlock();
    
    if(m_uSeverPending == PROTOCAL_FLAG_FALSE)
    {
        dcom_sMsgContext.uReqDataLen = 0;
    }
    else
    {
        /* do nothing */
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_no_tx_comfirmation                                      */
/* FUNCTION   : indicates the session has done without transmit             */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_no_tx_comfirmation(void)
{
    dcom_sMsgContext.uResDataLen = 0;
    dsd_resp_edit_unlock();
    
    if(m_uSeverPending == PROTOCAL_FLAG_FALSE)
    {
        dcom_sMsgContext.uReqDataLen = 0;
    }
    else
    {
        /* do nothing */
    }
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsl_get_session_level                                       */
/* FUNCTION   : dsl get session level                                       */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
SESSION_STATE dsl_get_session_level(void)
{   
    return m_enmSessionState;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsl_set_session_level                                       */
/* FUNCTION   : dsl set session level                                       */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_set_session_level(SESSION_STATE enmSession)
{
    m_enmSessionState = enmSession;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsl_get_security_level                                      */
/* FUNCTION   : dsl_get_security_level                                      */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
SECURITY_LEVEL dsl_get_security_level(void)
{
    return m_enmSecurityLv;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsl_set_security_level                                      */
/* FUNCTION   : dsl_set_security_level                                      */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_set_security_level(SECURITY_LEVEL enmSecurityLv)
{
    m_enmSecurityLv = enmSecurityLv;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dcm_proc                                                    */
/* FUNCTION   : This is the proc of the dsl module                          */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dcm_proc(void)
{
    /*handle request*/
    if(m_uNewReqHasArrived == PROTOCAL_FLAG_TRUE)
    {
        dsd_data_indication_functionality(&dcom_sMsgContext);
        dsl_rst_s3timer();
        m_uNewReqHasArrived = PROTOCAL_FLAG_FALSE;
    }
    else
    {
        /* do nothing */
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_monitor_proc                                            */
/* FUNCTION   : This is the proc of the dsl timing monitoring module        */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_monitor_proc(void)
{
    /*handle S3 timing*/
    if(m_enmSessionState == DEFAULT_SESSION)
    {
        m_uS3MaxCount = 0;
    }
    else
    {
        m_uS3MaxCount++;
    }
    
    if(m_uS3MaxCount >= S3_TIMING_MAX)
    {
        m_enmSessionState = DEFAULT_SESSION;
        m_enmSecurityLv = SECURITY_LV_NONE;
        m_uS3MaxCount = 0;
        
        sw_reset();
        
    }
    else
    {
        /* do nothing */
    }
    
    /* monitoring for session level back to default, like vehspd... */
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsl_final                                                   */
/* FUNCTION   : This is the finalization function of the dsl module         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsl_final(void)
{
    dcom_sMsgContext.pReqDataBuffer = m_uRxBuffer;
    dcom_sMsgContext.uReqDataLen = 0;
    dcom_sMsgContext.pResDataBuffer = m_uTxBuffer;
    dcom_sMsgContext.uResDataLen = 0;
    
    m_enmSessionState = DEFAULT_SESSION;
    m_enmSecurityLv = SECURITY_LV_NONE;
    
    m_uS3MaxCount = 0;
    
    m_uNewReqHasArrived = PROTOCAL_FLAG_FALSE;
    
    m_uSeverPending = PROTOCAL_FLAG_FALSE;
}


    
    
