/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include "me_com_lcfg.h"
#include "me_com_extention.h"

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

void (*const ME_Com_Tx_CbkFunction_Array[ME_COM_TX_PDU_NUMBER])(void) = {

    &do_core_car_sensor_callback,
};
void (*const ME_Com_Tx_CalloutFunction_Array[ME_COM_TX_PDU_NUMBER])(void) = {

    &do_core_car_sensor_callout,
};

Std_ReturnType (*const ME_Com_Rx_CbkFunction_Array[ME_COM_RX_PDU_NUMBER])(void) = {

    &do_core_application_message_protocol_callback,
    &do_core_boot_diagnostics_message_protocol_callback,
    &do_core_calibration_dynamic_protocol_callback,
    &do_core_calibration_spc_protocol_callback,
    &do_core_calibration_static_protocol_callback,
    &do_core_common_protocol_callback,
    &do_core_ds_traffic_signs_protocol_callback,
    &do_core_failsafe_protocol_callback,
    &do_core_fcf_cv_dyn_protocol_callback,
    &do_core_fcf_vd_dyn_protocol_callback,
    &do_core_fcf_vru_dyn_protocol_callback,
    &do_core_high_low_beam_protocol_callback,
    &do_core_intelligent_speed_assist_protocol_callback,
    &do_core_lanes_adjacent_protocol_callback,
    &do_core_lanes_applications_protocol_callback,
    &do_core_lanes_host_protocol_callback,
    &do_core_lanes_road_edge_protocol_callback,
    &do_core_lateral_departure_warning_protocol_callback,
    &do_core_mtfv_protocol_callback,
    &do_core_objects_protocol_callback,
    &do_core_safety_diagnostics_protocol_callback,
};

void (*const ME_Com_Rx_TimeOutFunction_Array[ME_COM_RX_PDU_NUMBER])(void) = {

    &do_core_application_message_protocol_timeout,
    &do_core_boot_diagnostics_message_protocol_timeout,
    &do_core_calibration_dynamic_protocol_timeout,
    &do_core_calibration_spc_protocol_timeout,
    &do_core_calibration_static_protocol_timeout,
    &do_core_common_protocol_timeout,
    &do_core_ds_traffic_signs_protocol_timeout,
    &do_core_failsafe_protocol_timeout,
    &do_core_fcf_cv_dyn_protocol_timeout,
    &do_core_fcf_vd_dyn_protocol_timeout,
    &do_core_fcf_vru_dyn_protocol_timeout,
    &do_core_high_low_beam_protocol_timeout,
    &do_core_intelligent_speed_assist_protocol_timeout,
    &do_core_lanes_adjacent_protocol_timeout,
    &do_core_lanes_applications_protocol_timeout,
    &do_core_lanes_host_protocol_timeout,
    &do_core_lanes_road_edge_protocol_timeout,
    &do_core_lateral_departure_warning_protocol_timeout,
    &do_core_mtfv_protocol_timeout,
    &do_core_objects_protocol_timeout,
    &do_core_safety_diagnostics_protocol_timeout,
};
