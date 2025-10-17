#ifndef _ME_COM_LCFG_H_
#define _ME_COM_LCFG_H_

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

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

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern void (*const ME_Com_Tx_CbkFunction_Array[ME_COM_TX_PDU_NUMBER])(void);

extern void (*const ME_Com_Tx_CalloutFunction_Array[ME_COM_TX_PDU_NUMBER])(void);

extern void (*const ME_Com_Rx_TimeOutFunction_Array[ME_COM_RX_PDU_NUMBER])(void);

extern Std_ReturnType (*const ME_Com_Rx_CbkFunction_Array[ME_COM_RX_PDU_NUMBER])(void);

#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* _ME_COM_LCFG_H_ */
