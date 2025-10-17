#ifndef _ME_COM_EXTENTION_H_
#define _ME_COM_EXTENTION_H_

#include "protocol_common_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */
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
extern void com_extention_init_v(void);

extern void do_core_car_sensor_callout(void);

extern void do_core_car_sensor_callback(void);

extern void do_core_application_message_protocol_timeout(void);
extern void do_core_boot_diagnostics_message_protocol_timeout(void);
extern void do_core_calibration_dynamic_protocol_timeout(void);
extern void do_core_calibration_spc_protocol_timeout(void);
extern void do_core_calibration_static_protocol_timeout(void);
extern void do_core_common_protocol_timeout(void);
extern void do_core_ds_traffic_signs_protocol_timeout(void);
extern void do_core_failsafe_protocol_timeout(void);
extern void do_core_fcf_cv_dyn_protocol_timeout(void);
extern void do_core_fcf_vd_dyn_protocol_timeout(void);
extern void do_core_fcf_vru_dyn_protocol_timeout(void);
extern void do_core_high_low_beam_protocol_timeout(void);
extern void do_core_intelligent_speed_assist_protocol_timeout(void);
extern void do_core_lanes_adjacent_protocol_timeout(void);
extern void do_core_lanes_applications_protocol_timeout(void);
extern void do_core_lanes_host_protocol_timeout(void);
extern void do_core_lanes_road_edge_protocol_timeout(void);
extern void do_core_lateral_departure_warning_protocol_timeout(void);
extern void do_core_mtfv_protocol_timeout(void);
extern void do_core_objects_protocol_timeout(void);
extern void do_core_safety_diagnostics_protocol_timeout(void);

extern Std_ReturnType do_core_application_message_protocol_callback(void);
extern Std_ReturnType do_core_boot_diagnostics_message_protocol_callback(void);
extern Std_ReturnType do_core_calibration_dynamic_protocol_callback(void);
extern Std_ReturnType do_core_calibration_spc_protocol_callback(void);
extern Std_ReturnType do_core_calibration_static_protocol_callback(void);
extern Std_ReturnType do_core_common_protocol_callback(void);
extern Std_ReturnType do_core_ds_traffic_signs_protocol_callback(void);
extern Std_ReturnType do_core_failsafe_protocol_callback(void);
extern Std_ReturnType do_core_fcf_cv_dyn_protocol_callback(void);
extern Std_ReturnType do_core_fcf_vd_dyn_protocol_callback(void);
extern Std_ReturnType do_core_fcf_vru_dyn_protocol_callback(void);
extern Std_ReturnType do_core_high_low_beam_protocol_callback(void);
extern Std_ReturnType do_core_intelligent_speed_assist_protocol_callback(void);
extern Std_ReturnType do_core_lanes_adjacent_protocol_callback(void);
extern Std_ReturnType do_core_lanes_applications_protocol_callback(void);
extern Std_ReturnType do_core_lanes_host_protocol_callback(void);
extern Std_ReturnType do_core_lanes_road_edge_protocol_callback(void);
extern Std_ReturnType do_core_lateral_departure_warning_protocol_callback(void);
extern Std_ReturnType do_core_mtfv_protocol_callback(void);
extern Std_ReturnType do_core_objects_protocol_callback(void);
extern Std_ReturnType do_core_safety_diagnostics_protocol_callback(void);

#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* _ME_COM_LINK_H_ */
