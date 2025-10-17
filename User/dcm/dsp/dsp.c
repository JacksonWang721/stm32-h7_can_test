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

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
typedef enum tagServiceState
{
    SERVICE_STATE_IDLE = 0,
    SERVICE_STATE_BUSY = 1,
    
}SERVICE_STATE, * LP_SERVICE_STATE;

/* P2 timeout, 50ms */
#define P2_TIMING_MAX                   4U
/* P2_star timeout, 2s */
#define P2_STAR_TIMING_MAX              150U

#define SERVICE_PROCESS_TIMEOUT         50000U

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
PTR_SRV_FUNC m_pfnSrvFunc = NULL;

SERVICE_STATE m_enmServiceState = SERVICE_STATE_IDLE;

uint16 m_uP2MaxCount = 0;

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
/* NAME       : dsp_init                                                    */
/* FUNCTION   : This is the initialization function of the dsp module       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsp_init(void)
{
    m_pfnSrvFunc = NULL;
    m_enmServiceState = SERVICE_STATE_IDLE;
    m_uP2MaxCount = 0;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsp_is_service_idle                                         */
/* FUNCTION   : dsp is service idle                                         */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
PROTOCAL_FLAG_TYPE dsp_is_service_idle(void)
{
    return ((m_enmServiceState == SERVICE_STATE_IDLE) ? PROTOCAL_FLAG_TRUE : PROTOCAL_FLAG_FALSE);
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsp_get_sid_index                                           */
/* FUNCTION   : dsp get sid index                                           */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType dsp_load_service_ptr(PTR_SRV_FUNC ptr_srv_func)
{
    Std_ReturnType uRetVal = E_NOT_OK;
    
    if(ptr_srv_func != NULL)
    {
        m_pfnSrvFunc = ptr_srv_func;
        
        uRetVal = E_OK;
    }
    else
    {
        uRetVal = E_NOT_OK;
    }
    
    return uRetVal;
}

/*--------------------------------------------------------------------------*/
/* @(f)                                                                     */
/* NAME       : dsp_set_service_busy                                        */
/* FUNCTION   : dsp set service busy                                        */
/* RETURN     :                                                             */
/* ARGUMENT   :                                                             */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsp_set_service_busy(void)
{
    m_enmServiceState = SERVICE_STATE_BUSY;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsp_service_proc                                            */
/* FUNCTION   : This is the proc function of the dsp module                 */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsp_service_proc(void)
{
    if((m_pfnSrvFunc != NULL) && (m_enmServiceState == SERVICE_STATE_BUSY))
    {
        if(m_pfnSrvFunc() == E_OK)
        {
            m_pfnSrvFunc = NULL;
            m_enmServiceState = SERVICE_STATE_IDLE;
            dsd_process_done();
            m_uP2MaxCount = 0;
        }
        else
        {
            /* move to dsp_service_monitor_proc */
        }
    }
    else
    {
        m_uP2MaxCount = 0;
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsp_service_monitor_proc                                    */
/* FUNCTION   : This is the monitoring proc function of the dsp module      */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsp_service_monitor_proc(void)
{
    if((m_pfnSrvFunc != NULL) && (m_enmServiceState == SERVICE_STATE_BUSY))
    {
        m_uP2MaxCount++;
        
        if(m_uP2MaxCount == P2_TIMING_MAX)
        {
            dsd_process_pending();
        }
        else if(m_uP2MaxCount > P2_TIMING_MAX)
        {
            if(m_uP2MaxCount % P2_STAR_TIMING_MAX == 0)
            {
                dsd_process_pending();
            }
            else
            {
                /* do nothing */
            }
            
            /* something's wrong */
            if(m_uP2MaxCount > SERVICE_PROCESS_TIMEOUT)
            {
                m_pfnSrvFunc = NULL;
                m_enmServiceState = SERVICE_STATE_IDLE;
                dsd_process_cancel_reponse();
                m_uP2MaxCount = 0;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* do nothing... */
        }
    }
    else
    {
        m_uP2MaxCount = 0;
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsp_direct_process_pending                                  */
/* FUNCTION   : invoke this func if you fear the process delay in routine   */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsp_direct_process_pending(void)
{
    m_uP2MaxCount = P2_TIMING_MAX;
    
    dsd_process_pending();
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : dsp_final                                                   */
/* FUNCTION   : This is the initialization function of the dsp module       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : yian                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void dsp_final(void)
{
    m_pfnSrvFunc = NULL;
    m_enmServiceState = SERVICE_STATE_IDLE;
    m_uP2MaxCount = 0;
}

