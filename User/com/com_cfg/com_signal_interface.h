#ifndef _ME_COM_SIGNAL_INTERFACE_H_
#define _ME_COM_SIGNAL_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

#include "protocol_common_types.h"
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

extern uint8  com_get_core_application_message_protocol_app_zero_byte_u8(void);
extern uint8  com_get_core_application_message_protocol_application_message_version_u8(void);

extern void com_set_core_car_sensor_cin_zero_byte_v(uint8 t_val_u8);
extern void com_set_core_car_sensor_reserved_1_v(uint32 t_val_u32);


#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* _ME_COM_SIGNAL_INTERFACE_H_ */
