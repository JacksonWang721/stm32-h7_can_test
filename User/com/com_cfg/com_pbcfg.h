#ifndef _ME_COM_PBCFG_H_
#define _ME_COM_PBCFG_H_

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

#ifdef _SUB_EQ4_SIL
#define extern
#endif

#include "me_com_types.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define ME_COM_TX_PDU_NUMBER                             1
#define ME_COM_RX_PDU_NUMBER                             21

#define COM_CORE_CAR_SENSOR_TX                         0

#define COM_CORE_APPLICATION_MESSAGE_PROTOCOL_RX       0
#define COM_CORE_BOOT_DIAGNOSTICS_MESSAGE_PROTOCOL_RX  1
#define COM_CORE_CALIBRATION_DYNAMIC_PROTOCOL_RX       2
#define COM_CORE_CALIBRATION_SPC_PROTOCOL_RX           3
#define COM_CORE_CALIBRATION_STATIC_PROTOCOL_RX        4
#define COM_CORE_COMMON_PROTOCOL_RX                    5
#define COM_CORE_DS_TRAFFIC_SIGNS_PROTOCOL_RX          6
#define COM_CORE_FAILSAFE_PROTOCOL_RX                  7
#define COM_CORE_FCF_CV_DYN_PROTOCOL_RX                8
#define COM_CORE_FCF_VD_DYN_PROTOCOL_RX                9
#define COM_CORE_FCF_VRU_DYN_PROTOCOL_RX               10
#define COM_CORE_HIGH_LOW_BEAM_PROTOCOL_RX             11
#define COM_CORE_INTELLIGENT_SPEED_ASSIST_PROTOCOL_RX  12
#define COM_CORE_LANES_ADJACENT_PROTOCOL_RX            13
#define COM_CORE_LANES_APPLICATIONS_PROTOCOL_RX        14
#define COM_CORE_LANES_HOST_PROTOCOL_RX                15
#define COM_CORE_LANES_ROAD_EDGE_PROTOCOL_RX           16
#define COM_CORE_LATERAL_DEPARTURE_WARNING_PROTOCOL_RX 17
#define COM_CORE_MTFV_PROTOCOL_RX                      18
#define COM_CORE_OBJECTS_PROTOCOL_RX                   19
#define COM_CORE_SAFETY_DIAGNOSTICS_PROTOCOL_RX        20

/* com_rx_mainfunction_v based on 20ms, for the frst timeout should be 180ms ( 27 * 6 = 162) */
#define ME_COM_RX_PDU_TIMEOUT_FIRST_THRESHOLD 18

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern const COM_CFG_ST m_com_pduCfg_st;

extern CORE_CAR_SENSOR m_com_txForeCoreCarsensorBuffer_st;

extern CORE_APPLICATION_MESSAGE_PROTOCOL       m_com_rxBackCoreApplicationMessageprotocolBuffer_st;

#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* _ME_COM_PBCFG_H_ */
