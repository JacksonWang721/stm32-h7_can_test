/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "std_type.h"
#include "dsl.h"
#include "dsd.h"
#include "dsd_cfg.h"
#include "dsp.h"
#include "voltage_monitor.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define SERVICE_ID_POS          0U

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
PROTOCAL_FLAG_TYPE m_uRespInFilling = PROTOCAL_FLAG_FALSE;

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
int dsd_get_sid_index(uint8 uSID);
SESSION_STATE dsd_get_service_required_session(uint8 uSidIndex);
SECURITY_LEVEL dsd_get_service_required_security_lv(uint8 uSidIndex);

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_init                                                    */
/* FUNCTION   : This is the initialization function of the dsd module       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_init(void)
{
    m_uRespInFilling = PROTOCAL_FLAG_FALSE;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsd_query_addressing_supportness                            */
/* FUNCTION   : dsd query addressing supportness                            */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType dsd_query_addressing_supportness(uint8 uSupposeSid, PROTOCAL_FLAG_TYPE IsPhyAddr)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    
    int nSidIndex = dsd_get_sid_index(uSupposeSid);
    
    if(nSidIndex >= 0)
    {
        if(IsPhyAddr || m_sServiceTable[nSidIndex].IsfuncAddrAllow == PROTOCAL_FLAG_TRUE)
        {
            uRetVal = E_OK;
        }
        else
        {
            uRetVal = E_NOT_OK;
        }
    }
    else
    {
        uRetVal = E_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsd_data_indication_functionality                           */
/* FUNCTION   : dsd data indication functionality                           */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_data_indication_functionality(LP_MSG_CONTEXT pMsgContext)
{
    
    /* this traversing query has already been done in dsd_query_addressing_supportness, 
    but the table is small , so I sacrifice the speed for a more simple code */
    int nSidIndex = dsd_get_sid_index(pMsgContext->pReqDataBuffer[SERVICE_ID_POS]); 
    /* is sid OK valid? */
    if(nSidIndex >= 0)
    {
        /* session level OK? */
        SESSION_STATE enmCurrentSession = dsl_get_session_level();
        SESSION_STATE enmNeededSession = dsd_get_service_required_session((uint8)nSidIndex);
        
        if((enmNeededSession == DEFAULT_SESSION) || (enmCurrentSession == enmNeededSession))
        {
            SECURITY_LEVEL enmCurrentSecurityLv = dsl_get_security_level();
            SECURITY_LEVEL enmNeededSecurityLv = dsd_get_service_required_security_lv((uint8)nSidIndex);
            
            if((enmNeededSecurityLv == SECURITY_LV_NONE) || (enmCurrentSecurityLv == enmNeededSecurityLv))
            {
                /* is serive idle? */
                if(dsp_is_service_idle() == PROTOCAL_FLAG_TRUE)
                {
                    dsp_load_service_ptr(m_sServiceTable[nSidIndex].ptr_srv_func);
                    dsp_set_service_busy();
                }
                else/*nrc: 0x21 NRC_BUSY_REPEAT_REQUEST*/
                {
                    dsd_set_negative_response(pMsgContext, NRC_BUSY_REPEAT_REQUEST);
                    dsd_process_done();
                }
            }
            else/*nrc: 0x33 NRC_SECURITY_ACCESS_DENIED*/
            {
                dsd_set_negative_response(pMsgContext, NRC_SECURITY_ACCESS_DENIED);
                dsd_process_done();
            }
        }
        else/*nrc: 0x7F NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION*/
        {
            if(isfunaddr == TRUE)
            {
                dcom_sMsgContext.uResDataLen = 0u;
            }
            else
            {
                dsd_set_negative_response(pMsgContext, NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION);
            }
            dsd_process_done();
        }
    }
    else/*nrc: 0x11 NRC_SERVICE_NOT_SUPPORTED*/
    {
        if(isfunaddr == TRUE)
        {
            dcom_sMsgContext.uResDataLen = 0u;
        }
        else
        {
            dsd_set_negative_response(pMsgContext, NRC_SERVICE_NOT_SUPPORTED);
        }
        dsd_process_done();
    }

}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsd_set_negative_response                                   */
/* FUNCTION   : dsd set negative response                                   */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_set_negative_response(LP_MSG_CONTEXT pMsgContext, uint8 uNRC)
{
    /*locked before tansmit*/
    dsd_resp_edit_lock();
    /*load SIDNR 0x7F*/
    pMsgContext->pResDataBuffer[0] = SIDNR;
    pMsgContext->uResDataLen++;
    /*load SIDRQ*/
    pMsgContext->pResDataBuffer[1] = pMsgContext->pReqDataBuffer[0];
    pMsgContext->uResDataLen++;
    /*load NRC*/
    pMsgContext->pResDataBuffer[2] = uNRC;
    pMsgContext->uResDataLen++;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_process_done                                            */
/* FUNCTION   : This is the finalization function of the dsd module         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_process_done(void)
{
    if(dcom_sMsgContext.uResDataLen > 0)
    {
        dsl_response_transmit();
    }
    else
    {
        dsl_no_tx_comfirmation();
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_process_cancel_reponse                                  */
/* FUNCTION   : This is the finalization function of the dsd module         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_process_cancel_reponse(void)
{
    dcom_sMsgContext.uResDataLen = 0;
    m_uRespInFilling = PROTOCAL_FLAG_FALSE;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_process_pending                                         */
/* FUNCTION   : dsd_process_pending                                         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_process_pending(void)
{
    if(m_uRespInFilling == PROTOCAL_FLAG_FALSE)
    {
        dsd_set_negative_response(&dcom_sMsgContext, NRC_RESPONSE_PENDING);
        
        dsl_pending_transmit();
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_resp_edit_lock                                          */
/* FUNCTION   : dsd_resp_edit_lock                                          */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_resp_edit_lock(void)
{
    m_uRespInFilling = PROTOCAL_FLAG_TRUE;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_resp_edit_unlock                                        */
/* FUNCTION   : dsd_resp_edit_unlock                                        */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_resp_edit_unlock(void)
{
    m_uRespInFilling = PROTOCAL_FLAG_FALSE;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsd_final                                                   */
/* FUNCTION   : This is the finalization function of the dsd module         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsd_final(void)
{
    m_uRespInFilling = PROTOCAL_FLAG_FALSE;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsd_get_sid_index                                           */
/* FUNCTION   : dsd_get_sid_index                                           */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
int dsd_get_sid_index(uint8 uSID)
{
    int nIdx = 0;
    
    for(nIdx = 0; nIdx < SERVICE_TABLE_MAX; nIdx++)
    {
        if(m_sServiceTable[nIdx].uSID == uSID)
        {
            break;
        }
        else
        {
            /* do nothing */
        }
    }
    
    if(nIdx == SERVICE_TABLE_MAX)
    {
        nIdx = -1;
    }
    else
    {
        /* do nothing */
    }
    
    return nIdx;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsd_get_service_required_session                            */
/* FUNCTION   : dsp get service required session                            */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
SESSION_STATE dsd_get_service_required_session(uint8 uSidIndex)
{
    return ((m_sServiceTable[uSidIndex].ptr_get_session == NULL) ? m_sServiceTable[uSidIndex].enmSessionLv : m_sServiceTable[uSidIndex].ptr_get_session());
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsd_get_service_required_security_lv                        */
/* FUNCTION   : dsd_get_service_required_security_lv                        */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
SECURITY_LEVEL dsd_get_service_required_security_lv(uint8 uSidIndex)
{
    return ((m_sServiceTable[uSidIndex].ptr_get_security_level == NULL) ? m_sServiceTable[uSidIndex].enmSecurityLv : m_sServiceTable[uSidIndex].ptr_get_security_level());
}

 