#ifndef _ME_COM_H_
#define _ME_COM_H_

#ifdef  __cplusplus
extern "C"
{
#endif  /* defined(__cplusplus) */

#include "me_com_types.h"
#include "me_com_pbcfg.h"
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern uint8 m_com_allRxTimeoutCnt_au8[ME_COM_RX_PDU_NUMBER];

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern void com_init_v(void);
extern void com_tx_mainfunction_v(void);
extern void com_rx_mainfunction_v(void);
extern void com_deinit_v(void);

extern void com_enable_all_transmit_v(void);
extern void com_disable_all_transmit_v(void);
extern void com_enable_transmit_v(uint8 p_txPduSN_u8);
extern void com_trigger_transmit_v(uint8 p_txPduSN_u8);
extern void com_disable_transmit_v(uint8 p_txPduSN_u8);
extern boolean com_get_event_state_v(uint8 p_txPduSN_u8);

extern void com_enable_all_receive_v(void);
extern void com_disable_all_receive_v(void);
extern void com_enable_receive_v(uint8 p_rxPduSN_u8);
extern void com_disable_receive_v(uint8 p_rxPduSN_u8);

extern com_RX_PDU_FORE_STATE_ENM com_get_rx_frame_state_v(uint8 p_rxPduSN_u8);
extern void com_clear_rx_frame_state_v(uint8 p_rxPduSN_u8);

extern Std_ReturnType com_provide_rx_buffer_enm(uint8 * * p_rxBuffer_ppu8, uint32 p_length_u32, uint8 p_msgId_u8);
extern Std_ReturnType com_rx_indication_enm(uint8 IsRxSuccess, uint8 p_msgId_u8);
extern Std_ReturnType com_tx_callback_enm(uint8 p_msgId_u8);

extern void com_get_rx_timeout_status_v(uint8 * p_rxTimeoutStatus_pu8);
extern Std_ReturnType com_provide_sbs_buffer_enm(uint8 * * p_rxBuffer_ppu8, uint32 p_length_u32);
extern void com_clear_sbs_buffer_v(void);
#ifdef  __cplusplus
}
#endif  /* defined(__cplusplus) */
#endif /* _ME_COM_H_ */

