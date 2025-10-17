/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include <string.h>

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
/* INLINE DEFINE                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

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
static void com_tx_pdu_init_v(void);
static void com_rx_pdu_init_v(void);
static void com_tx_cycle_calc_v(uint8 p_txPduSN_u8);
static void com_tx_msg_transmit_v(uint8 p_txPduSN_u8);
static void com_rx_timeout_monitor_v(uint8 p_rxPduSN_u8);
static void com_rx_copy_back_2_fore_v(uint8 p_rxPduSN_u8);
static Std_ReturnType com_search_tx_pdu_serial_number_enm(uint8 p_txPduId_u8, uint8 * p_txPduSN_pu8);
static Std_ReturnType com_search_rx_pdu_serial_number_enm(uint8 p_rxPduId_u8, uint8 * p_rxPduSN_pu8);

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_init_v                                                */
/* FUNCTION   : com_init_v                                                */
/* CREATE     : mingyu.wang                                                 */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_init(void)
{
    /* the reset of the Tx PDU */
    com_tx_pdu_init();

    /* the reset of the Rx PDU */
    com_rx_pdu_init();

    com_extention_init();
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_tx_pdu_init                                           */
/* FUNCTION   : com_tx_pdu_init                                           */
/* CREATE     : mingyu.wang                                                 */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_tx_pdu_init(void)
{
    uint8 t_txPduSN_u8 = 0U;

    for(t_txPduSN_u8 = 0U; t_txPduSN_u8 < ME_COM_TX_PDU_NUMBER ; t_txPduSN_u8++)
    {
        /* reset Tx status */
        m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].pduState_enm = COM_PDU_UNABLE;
        m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].txPduState_enm = COM_TX_PDU_PENDING;
        m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].txCycleCnt_s16 = 0U;
        m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].txEvent_b = FALSE;
        /* clear Tx PDU */
        memset(m_com_pduCfg_st.txPduCfg_pst[t_txPduSN_u8].msgBuffer_pu8, 0U, m_com_pduCfg_st.txPduCfg_pst[t_txPduSN_u8].msgStructLen_u32);
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_rx_pdu_init                                           */
/* FUNCTION   : com_rx_pdu_init                                           */
/* CREATE     : mingyu.wang                                                 */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_rx_pdu_init(void)
{
    uint8 t_rxPduSN_u8 = 0U;

    for(t_rxPduSN_u8 = 0U; t_rxPduSN_u8 < ME_COM_RX_PDU_NUMBER ; t_rxPduSN_u8++)
    {
        /* reset Rx status */
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].pduState_enm = COM_PDU_UNABLE;
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm = COM_RX_PDU_BACK_IDLE;
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduForeBufState_enm = COM_RX_PDU_FORE_NEVER_UPDATE;
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxTimeOutCnt_s16 = ME_COM_RX_PDU_TIMEOUT_FIRST_THRESHOLD;
        /* clear Rx PDU */
        memset(m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgBackBuffer_pu8, 0U, m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgStructLen_u32);
        memset(m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgForeBuffer_pu8, 0U, m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgStructLen_u32);
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_tx_mainfunction                                       */
/* FUNCTION   : com_tx_mainfunction                                       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     : uint8 [0-255]                                   */
/*--------------------------------------------------------------------------*/
void com_tx_mainfunction(void)
{
    uint8 t_txPduSN_u8 = 0U;

    for(t_txPduSN_u8 = 0U; t_txPduSN_u8 < ME_COM_TX_PDU_NUMBER; t_txPduSN_u8++)
    {
        /* check if Ipdu is active */
        if(COM_PDU_ENABLE == m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].pduState_enm)
        {
            com_tx_cycle_calc_v(t_txPduSN_u8);
            com_tx_msg_transmit_v(t_txPduSN_u8);
        }
        else
        {
            /* do nothing; */
        }
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_tx_cycle_calc_v                                      */
/* FUNCTION   : com_tx_cycle_calc_v                                      */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_tx_cycle_calc_v(uint8 p_txPduSN_u8)
{
    sint16 t_cycleCnt_s16 = m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txCycleCnt_s16;
    sint16 t_msgTxCycle_s16 = m_com_pduCfg_st.txPduCfg_pst[p_txPduSN_u8].txMsgCycle_s16;

    t_cycleCnt_s16++;

    if(t_cycleCnt_s16 >= t_msgTxCycle_s16)
    {
        m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txPduState_enm = COM_TX_PDU_PENDING;
    }
    else
    {
        /* do nothing; */
    }
    m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txCycleCnt_s16 = t_cycleCnt_s16;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_tx_msg_transmit_v                                              */
/* FUNCTION   : com_tx_msg_transmit_v                                              */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_tx_msg_transmit_v(uint8 p_txPduSN_u8)
{
    uint8  t_msgId_u8 = m_com_pduCfg_st.txPduCfg_pst[p_txPduSN_u8].msgID_u8;
    uint32 t_msgLen_u32  = m_com_pduCfg_st.txPduCfg_pst[p_txPduSN_u8].msgDataLen_u32;
    uint8* t_sduDataPtr_pu8 = m_com_pduCfg_st.txPduCfg_pst[p_txPduSN_u8].msgBuffer_pu8;

    if(COM_TX_PDU_PENDING == m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txPduState_enm)
    {
        /* call the tx callout functions ,do CRC and others */
        (ME_Com_Tx_CalloutFunction_Array[p_txPduSN_u8])();

        if(E_OK == mepdur_data_write(t_sduDataPtr_pu8, t_msgLen_u32, t_msgId_u8))
        {
            m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txPduState_enm = COM_TX_PDU_IDLE;
            m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txCycleCnt_s16 = 0U;
            /* used for event message */
            if(m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txEvent_b == TRUE)
            {
                m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].pduState_enm = COM_PDU_UNABLE;
                m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txEvent_b = FALSE;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txPduState_enm = COM_TX_PDU_PENDING;
        }
    }
    else
    {
        /* do nothing */
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_tx_callback_enm                                                */
/* FUNCTION   : com_tx_callback_enm                                                */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     : if mepdur_data_write is succeed, call this functions        */
/*--------------------------------------------------------------------------*/
Std_ReturnType com_tx_callback(uint8 p_msgId_u8)
{
    Std_ReturnType t_retVal_enm = E_NOT_OK;
    uint8 t_txPduSN_u8 = 0U;

    t_retVal_enm = com_search_tx_pdu_serial_number_enm(p_msgId_u8, &t_txPduSN_u8);

    if(E_OK == t_retVal_enm)
    {
        /* call the tx indication functions */
        (ME_Com_Tx_CbkFunction_Array[t_txPduSN_u8])();
    }
    else
    {
        /* do nothing */
    }
    return t_retVal_enm;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_search_tx_pdu_serial_number_enm                            */
/* FUNCTION   : com_search_tx_pdu_serial_number_enm                            */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType com_search_tx_pdu_serial_number(uint8 p_txMsgId_u8, uint8 * p_txPduSN_pu8)
{
    Std_ReturnType t_retVal_enm = E_NOT_OK;
    uint8 t_txPduSN_u8 = 0U;

    for(t_txPduSN_u8 = 0U; t_txPduSN_u8 < ME_COM_TX_PDU_NUMBER; t_txPduSN_u8++)
    {
        if(p_txMsgId_u8 == (m_com_pduCfg_st.txPduCfg_pst[t_txPduSN_u8].msgID_u8))
        {
            * p_txPduSN_pu8 = t_txPduSN_u8;
            t_retVal_enm = E_OK;
            break;
        }
        else
        {
            /* do nothing */
        }
    }
    return t_retVal_enm;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_rx_mainfunction_v                                       */
/* FUNCTION   : com_rx_mainfunction_v                                       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_rx_mainfunction_v(void)
{
    uint8 t_rxPduSN_u8 = 0U;
    boolean t_rxTimeoutFlag_u8 = FALSE;

    for(t_rxPduSN_u8 = 0U; t_rxPduSN_u8 < ME_COM_RX_PDU_NUMBER; t_rxPduSN_u8++)
    {
        /* check if Ipdu is active, and buffer is no employ */
        if((COM_PDU_ENABLE == m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].pduState_enm)
            &&(COM_RX_PDU_BACK_IDLE == m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm))
        {
            /* check if the period is out of the allowed, this is used for timeout. */
            com_rx_timeout_monitor_v(t_rxPduSN_u8);
        }
        else
        {
            /* do nothing */
        }
        
        if(m_com_allRxTimeoutCnt_au8[t_rxPduSN_u8] != 0) /*check if recover from rx_timeout*/
        {
            t_rxTimeoutFlag_u8 = TRUE;
        }
        else
        {
            /*do nothing*/
        }
    }

    if(t_rxTimeoutFlag_u8 == FALSE)
    {
        com_report_event_pass_EQAppRxMessageTimeOut_v();
        com_report_event_pass_EQKeyRxMessageTimeOut_v();
        com_report_event_pass_EQOtherRxMessageTimeOut_v();
        com_report_event_pass_EQRxMessageTimeOutAction_v();
    }
    else
    {
        /*do nothing*/
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_rx_timeout_monitor_v                            */
/* FUNCTION   : com_rx_timeout_monitor_v                            */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_rx_timeout_monitor_v(uint8 p_rxPduSN_u8)
{
    sint16 t_timeOutCnt_s16 = m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxTimeOutCnt_s16;
    sint16 t_rxMsgTimeOut_s16 = m_com_pduCfg_st.rxPduCfg_pst[p_rxPduSN_u8].rxMsgTimeOut_s16;

    t_timeOutCnt_s16--;

    if(t_timeOutCnt_s16 <= 0)
    {
        t_timeOutCnt_s16 = t_rxMsgTimeOut_s16;
        (ME_Com_Rx_TimeOutFunction_Array[p_rxPduSN_u8])();
    }
    else
    {
        /* do nothing */
    }
    m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxTimeOutCnt_s16 = t_timeOutCnt_s16;
}


/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_provide_rx_buffer_enm                                     */
/* FUNCTION   : com_provide_rx_buffer_enm                                     */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     : This is called by the lower layer to get the receive buffer */
/*--------------------------------------------------------------------------*/
Std_ReturnType com_provide_rx_buffer_enm(uint8 * * p_rxBuffer_ppu8, uint32 p_length_u32, uint8 p_msgId_u8)
{
    Std_ReturnType t_retVal_enm = E_NOT_OK;
    Std_ReturnType t_Indbc_enm = E_NOT_OK;
    uint8 t_rxPduSN_u8 = 0U;

    t_Indbc_enm = com_search_rx_pdu_serial_number_enm(p_msgId_u8, &t_rxPduSN_u8);

    if(E_OK == t_Indbc_enm)//in dbc
    {
        if(COM_RX_PDU_BACK_IDLE == m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm)//empty buffer
        {
            if(p_length_u32 <= m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgDataLen_u32)
            {
                * p_rxBuffer_ppu8 = m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgBackBuffer_pu8;
                m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm = COM_RX_PDU_BACK_BUSY;
            }
            else
            {
            #ifndef COM_DATA_CUT_OFF
                * p_rxBuffer_ppu8 = NULL;
            #else
                m_com_msgLenCutOffCnt_u32++;
                m_com_msgLenCutOffAppId_u32 = p_msgId_u8;
                m_com_msgLenCutOffLen_u32 = p_length_u32;
                m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm = COM_RX_PDU_BACK_BUSY;
                * p_rxBuffer_ppu8 = m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgBackBuffer_pu8;
                p_length_u32 = m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgDataLen_u32;
                //* p_length_u32 = m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].msgDataLen_u32;
            #endif
                com_report_event_pass_McuErr_v();
            }
            t_retVal_enm = E_OK;
        }
        else//SHOULD'T BE HERE
        {
            t_retVal_enm = E_NOT_OK;//SHOULD WAIT MESSAGE FOR CONSUMED FROM MSC 
        }

    }
    else //not in dbc
    {
        t_retVal_enm = E_NOT_OK;//METP WILL IGNORE THIS FRAME ,SHOULD NOT WAIT FOR AVAIABLE FRAME
    }
    
    return t_retVal_enm;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_rx_indication_enm                                         */
/* FUNCTION   : com_rx_indication_enm                                         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     : This is called by the lower layer to ensure message (PDU)   */
/*              integrity.                                                  */
/*--------------------------------------------------------------------------*/
Std_ReturnType com_rx_indication_enm(uint8 IsRxSuccess, uint8 p_msgId_u8)
{
    Std_ReturnType t_retVal_enm = E_NOT_OK;
    uint8 t_rxPduSN_u8 = 0U;

    t_retVal_enm = com_search_rx_pdu_serial_number_enm(p_msgId_u8, &t_rxPduSN_u8);

    if(E_OK == t_retVal_enm)
    {
        if((PROTOCOL_FLAG_TRUE == IsRxSuccess)
            && (COM_RX_PDU_BACK_BUSY == m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm))
        {
            com_rx_copy_back_2_fore_v(t_rxPduSN_u8);
            m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm = COM_RX_PDU_BACK_IDLE;
            m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxTimeOutCnt_s16 = m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].rxMsgTimeOut_s16;
            if(m_com_allRxTimeoutCnt_au8[t_rxPduSN_u8] < RX_TIMEOUT_MAX_COUNT)
            {
                m_com_allRxTimeoutCnt_au8[t_rxPduSN_u8] = 0U;             /*msg receive successfully and clear the rx_timeout count*/
            }
            else
            {
                //do nothing
            }
        }
        else
        {
            m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxPduBackBufState_enm = COM_RX_PDU_BACK_IDLE;
            m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxTimeOutCnt_s16 = m_com_pduCfg_st.rxPduCfg_pst[t_rxPduSN_u8].rxMsgTimeOut_s16;
            if(m_com_allRxTimeoutCnt_au8[t_rxPduSN_u8] < RX_TIMEOUT_MAX_COUNT)
            {
                m_com_allRxTimeoutCnt_au8[t_rxPduSN_u8] = 0U;             /*msg receive successfully and clear the rx_timeout count*/
            }
            else
            {
                //do nothing
            }
        }
    }
    else
    {
        /* do nothing */
    }

    return t_retVal_enm;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_rx_copy_back_2_fore_v                         */
/* FUNCTION   : com_rx_copy_back_2_fore_v                         */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
static void com_rx_copy_back_2_fore_v(uint8 p_rxPduSN_u8)
{
    uint32 t_msgLen_u32 = m_com_pduCfg_st.rxPduCfg_pst[p_rxPduSN_u8].msgDataLen_u32;
    uint8* t_srcBuffer_pu8 = m_com_pduCfg_st.rxPduCfg_pst[p_rxPduSN_u8].msgBackBuffer_pu8;
    uint8* t_destBuffer_pu8 = m_com_pduCfg_st.rxPduCfg_pst[p_rxPduSN_u8].msgForeBuffer_pu8;

    memcpy(t_destBuffer_pu8, t_srcBuffer_pu8, t_msgLen_u32);
    if(E_OK == (ME_Com_Rx_CbkFunction_Array[p_rxPduSN_u8])())
    {
        m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxPduForeBufState_enm = COM_RX_PDU_FORE_HAVE_UPDATE;
    }
    else
    {
        /* CRC /rollingcounter error ,do not set update flag */
    }
    /* clear background */
    memset(t_srcBuffer_pu8, 0U, t_msgLen_u32);
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_search_rx_pdu_serial_number_enm                       */
/* FUNCTION   : com_search_rx_pdu_serial_number_enm                       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType com_search_rx_pdu_serial_number_enm(uint8 p_rxPduId_u8, uint8 * p_rxPduSN_pu8)
{
    Std_ReturnType t_retVal_enm = E_NOT_OK;
    uint8 t_rxAppSN_u8 = 0U;

    for(t_rxAppSN_u8 = 0U; t_rxAppSN_u8 < ME_COM_RX_PDU_NUMBER; t_rxAppSN_u8++)
    {
        if(p_rxPduId_u8 == (m_com_pduCfg_st.rxPduCfg_pst[t_rxAppSN_u8].msgID_u8))
        {
            * p_rxPduSN_pu8  = t_rxAppSN_u8;
            t_retVal_enm = E_OK;
            break;
        }
        else
        {
            /* do nothing */
        }
    }
    return t_retVal_enm;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_deinit_v                                                */
/* FUNCTION   : com_deinit_v                                                */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_deinit_v(void)
{
    /* the reset of the Tx PDU */
    com_tx_pdu_init_v();

    /* the reset of the Rx PDU */
    com_rx_pdu_init_v();

    com_extention_init_v();
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_enable_all_transmit_v                                   */
/* FUNCTION   : com_enable_all_transmit_v                                   */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_enable_all_transmit_v(void)
{
    uint8 t_txPduSN_u8 = 0U;

    /* ME COM TX IPDUs handling */
    for(t_txPduSN_u8 = 0U; t_txPduSN_u8 < ME_COM_TX_PDU_NUMBER; t_txPduSN_u8++)
    {
        m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].pduState_enm = COM_PDU_ENABLE;
    }
    com_extention_init_v();
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_disable_all_transmit_v                                  */
/* FUNCTION   : com_disable_all_transmit_v                                  */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_disable_all_transmit_v(void)
{
    uint8 t_txPduSN_u8 = 0U;

    /* ME COM TX IPDUs handling */
    for(t_txPduSN_u8 = 0U; t_txPduSN_u8 < ME_COM_TX_PDU_NUMBER; t_txPduSN_u8++)
    {
        m_com_pduCfg_st.txPduStatus_st[t_txPduSN_u8].pduState_enm = COM_PDU_UNABLE;
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_enable_transmit_v                                       */
/* FUNCTION   : com_enable_transmit_v                                       */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_enable_transmit_v(uint8 p_txPduSN_u8)
{
    m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].pduState_enm = COM_PDU_ENABLE;
    com_extention_init_v();
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_trigger_transmit_v                                      */
/* FUNCTION   : com_trigger_transmit_v                                      */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_trigger_transmit_v(uint8 p_txPduSN_u8)
{
    m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].pduState_enm = COM_PDU_ENABLE;
    m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txEvent_b = TRUE;
}

boolean com_get_event_state_v(uint8 p_txPduSN_u8)
{
    return m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].txEvent_b;
}
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_disable_transmit_v                                      */
/* FUNCTION   : com_disable_transmit_v                                      */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_disable_transmit_v(uint8 p_txPduSN_u8)
{
    m_com_pduCfg_st.txPduStatus_st[p_txPduSN_u8].pduState_enm = COM_PDU_UNABLE;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_enable_all_receive_v                            */
/* FUNCTION   : com_enable_all_receive_v                            */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_enable_all_receive_v(void)
{
    uint8 t_rxPduSN_u8 = 0U;

    /* ME COM RX IPDUs handling */
    for(t_rxPduSN_u8 = 0U; t_rxPduSN_u8 < ME_COM_RX_PDU_NUMBER; t_rxPduSN_u8++)
    {
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].pduState_enm = COM_PDU_ENABLE;
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxTimeOutCnt_s16 = ME_COM_RX_PDU_TIMEOUT_FIRST_THRESHOLD;
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_disable_all_receive_v                           */
/* FUNCTION   : com_disable_all_receive_v                           */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_disable_all_receive_v(void)
{
    uint8 t_rxPduSN_u8 = 0U;

    /* ME COM RX IPDUs handling */
    for(t_rxPduSN_u8 = 0U; t_rxPduSN_u8 < ME_COM_RX_PDU_NUMBER; t_rxPduSN_u8++)
    {
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].pduState_enm = COM_PDU_UNABLE;
        m_com_pduCfg_st.rxPduStatus_st[t_rxPduSN_u8].rxTimeOutCnt_s16 = ME_COM_RX_PDU_TIMEOUT_FIRST_THRESHOLD;
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_enable_receive_v                                */
/* FUNCTION   : com_enable_receive_v                                */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_enable_receive_v(uint8 p_rxPduSN_u8)
{
    m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].pduState_enm = COM_PDU_ENABLE;
    m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxTimeOutCnt_s16 = m_com_pduCfg_st.rxPduCfg_pst[p_rxPduSN_u8].rxMsgTimeOut_s16;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_disable_receive_v                               */
/* FUNCTION   : com_disable_receive_v                               */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_disable_receive_v(uint8 p_rxPduSN_u8)
{
    m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].pduState_enm = COM_PDU_UNABLE;
    m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxTimeOutCnt_s16 = m_com_pduCfg_st.rxPduCfg_pst[p_rxPduSN_u8].rxMsgTimeOut_s16;
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_get_rx_frame_state_v                                   */
/* FUNCTION   : com_get_rx_frame_state_v                                   */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :  0x00 never update, 0x01 update ever, 0x03 have update      */
/*--------------------------------------------------------------------------*/
COM_RX_PDU_FORE_STATE_ENM com_get_rx_frame_state_v(uint8 p_rxPduSN_u8)
{
    COM_RX_PDU_FORE_STATE_ENM uRetStatus = m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxPduForeBufState_enm;

    return uRetStatus;
}
/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_clear_rx_frame_state_v                                 */
/* FUNCTION   : com_clear_rx_frame_state_v                                 */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_clear_rx_frame_state_v(uint8 p_rxPduSN_u8)
{
    COM_RX_PDU_FORE_STATE_ENM uRetStatus = m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxPduForeBufState_enm;

    if(COM_RX_PDU_FORE_NEVER_UPDATE != uRetStatus)
    {
        (m_com_pduCfg_st.rxPduStatus_st[p_rxPduSN_u8].rxPduForeBufState_enm) = COM_RX_PDU_FORE_UPDATE_EVER;
    }
    else
    {
        /* do nothing */
    }
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_clear_rx_frame_state_v                                */
/* FUNCTION   : com_clear_rx_frame_state_v                                */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_get_rx_timeout_status_v(uint8 * p_rxTimeoutStatus_pu8)
{
    memcpy(p_rxTimeoutStatus_pu8, m_com_allRxTimeoutCnt_au8, ME_COM_RX_PDU_NUMBER);
}

/*--------------------------------------------------------------------------*/
/* @(p)                                                                     */
/* NAME       : com_provide_sbs_buffer_enm                                */
/* FUNCTION   : com_provide_sbs_buffer_enm                                */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
Std_ReturnType com_provide_sbs_buffer_enm(uint8 * * p_rxBuffer_ppu8, uint32 p_length_u32)
{
    Std_ReturnType uRetVal = E_NOT_OK;

    if((p_rxBuffer_ppu8 != NULL) && (p_length_u32 < m_com_pduCfg_st.rxPduCfg_pst[COM_CORE_OBJECTS_PROTOCOL_RX].msgStructLen_u32))
    {
        *p_rxBuffer_ppu8 = m_com_pduCfg_st.rxPduCfg_pst[COM_CORE_OBJECTS_PROTOCOL_RX].msgBackBuffer_pu8;
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
/* NAME       : com_clear_sbs_buffer_v                                    */
/* FUNCTION   : com_clear_sbs_buffer_v                                    */
/* RETURN     : none                                                        */
/* ARGUMENT   : none                                                        */
/* CREATE     : mosc                                                        */
/* UPDATE     :                                                             */
/* REMARK     :                                                             */
/*--------------------------------------------------------------------------*/
void com_clear_sbs_buffer_v(void)
{
    memset(m_com_pduCfg_st.rxPduCfg_pst[COM_CORE_OBJECTS_PROTOCOL_RX].msgBackBuffer_pu8, 0u, m_com_pduCfg_st.rxPduCfg_pst[COM_CORE_OBJECTS_PROTOCOL_RX].msgStructLen_u32);
}
