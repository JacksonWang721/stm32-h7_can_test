/*
* Copyright (c) 2020, imotion.ai
* All Rights Reserved.
* Author: mosc
* File: can_if.h
* Description:
* History:
* 2020-08-06, original
* 2020-08-06, these only used for CAN_FD
*/

#ifndef _CAN_IF_H_
#define _CAN_IF_H_

#ifdef  __cplusplus
extern "C"
{
#endif  /* defined(__cplusplus) */

#include "can_if_cfg.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* TYPEDEF                                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/
typedef enum
{
    COM = 0;
    DIAG = 1;
}CANIF_FRAMETYPE_ENM;

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
extern void can_if_init(void);
extern void can_if_mainfunction(void);
extern Std_ReturnType can_if_transmit(void);
extern boolean can_if_get_transmit_status(uint8 uCanIfTxMOIndex);
extern boolean can_if_cancel_transmit(uint8 uCanIfTxMOIndex);
extern void can_status_check(void);
#ifdef  __cplusplus
}
#endif  /* defined(__cplusplus) */
#endif /* _CAN_IF_H_ */

