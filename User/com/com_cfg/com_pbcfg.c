/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include "me_com_pbcfg.h"

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
CORE_CAR_SENSOR m_com_txForeCoreCarsensorBuffer_st;

CORE_APPLICATION_MESSAGE_PROTOCOL       m_com_rxBackCoreApplicationMessageprotocolBuffer_st;
CORE_APPLICATION_MESSAGE_PROTOCOL       m_com_rxForeCoreApplicationMessageprotocolBuffer_st;
CORE_BOOT_DIAGNOSTICS_MESSAGE_PROTOCOL  m_com_rxBackCoreBootDiagnosticsMessageprotocolBuffer_st;
CORE_BOOT_DIAGNOSTICS_MESSAGE_PROTOCOL  m_com_rxForeCoreBootDiagnosticsMessageprotocolBuffer_st;
CORE_CALIBRATION_DYNAMIC_PROTOCOL       m_com_rxBackCoreCalibrationDynamicprotocolBuffer_st;
CORE_CALIBRATION_DYNAMIC_PROTOCOL       m_com_rxForeCoreCalibrationDynamicprotocolBuffer_st;
CORE_CALIBRATION_SPC_PROTOCOL           m_com_rxBackCoreCalibrationSPCprotocolBuffer_st;
CORE_CALIBRATION_SPC_PROTOCOL           m_com_rxForeCoreCalibrationSPCprotocolBuffer_st;
CORE_CALIBRATION_STATIC_PROTOCOL        m_com_rxBackCoreCalibrationStaticprotocolBuffer_st;
CORE_CALIBRATION_STATIC_PROTOCOL        m_com_rxForeCoreCalibrationStaticprotocolBuffer_st;
CORE_COMMON_PROTOCOL                    m_com_rxBackCoreCommonprotocolBuffer_st;
CORE_COMMON_PROTOCOL                    m_com_rxForeCoreCommonprotocolBuffer_st;
CORE_DS_TRAFFIC_SIGNS_PROTOCOL          m_com_rxBackCoreDSTrafficSignsprotocolBuffer_st;
CORE_DS_TRAFFIC_SIGNS_PROTOCOL          m_com_rxForeCoreDSTrafficSignsprotocolBuffer_st;
CORE_FAILSAFE_PROTOCOL                  m_com_rxBackCoreFailsafeprotocolBuffer_st;
CORE_FAILSAFE_PROTOCOL                  m_com_rxForeCoreFailsafeprotocolBuffer_st;
CORE_FCF_CV_DYN_PROTOCOL                m_com_rxBackCoreFCFCVDYNprotocolBuffer_st;
CORE_FCF_CV_DYN_PROTOCOL                m_com_rxForeCoreFCFCVDYNprotocolBuffer_st;
CORE_FCF_VD_DYN_PROTOCOL                m_com_rxBackCoreFCFVDDYNprotocolBuffer_st;
CORE_FCF_VD_DYN_PROTOCOL                m_com_rxForeCoreFCFVDDYNprotocolBuffer_st;
CORE_FCF_VRU_DYN_PROTOCOL               m_com_rxBackCoreFCFVRUDYNprotocolBuffer_st;
CORE_FCF_VRU_DYN_PROTOCOL               m_com_rxForeCoreFCFVRUDYNprotocolBuffer_st;
CORE_HIGH_LOW_BEAM_PROTOCOL             m_com_rxBackCoreHighLowBeamprotocolBuffer_st;
CORE_HIGH_LOW_BEAM_PROTOCOL             m_com_rxForeCoreHighLowBeamprotocolBuffer_st;
CORE_INTELLIGENT_SPEED_ASSIST_PROTOCOL  m_com_rxBackCoreIntelligentSpeedAssistprotocolBuffer_st;
CORE_INTELLIGENT_SPEED_ASSIST_PROTOCOL  m_com_rxForeCoreIntelligentSpeedAssistprotocolBuffer_st;
CORE_LANES_ADJACENT_PROTOCOL            m_com_rxBackCoreLanesAdjacentprotocolBuffer_st;
CORE_LANES_ADJACENT_PROTOCOL            m_com_rxForeCoreLanesAdjacentprotocolBuffer_st;
CORE_LANES_APPLICATIONS_PROTOCOL        m_com_rxBackCoreLanesApplicationsprotocolBuffer_st;
CORE_LANES_APPLICATIONS_PROTOCOL        m_com_rxForeCoreLanesApplicationsprotocolBuffer_st;
CORE_LANES_HOST_PROTOCOL                m_com_rxBackCoreLanesHostprotocolBuffer_st;
CORE_LANES_HOST_PROTOCOL                m_com_rxForeCoreLanesHostprotocolBuffer_st;
CORE_LANES_ROAD_EDGE_PROTOCOL           m_com_rxBackCoreLanesRoadEdgeprotocolBuffer_st;
CORE_LANES_ROAD_EDGE_PROTOCOL           m_com_rxForeCoreLanesRoadEdgeprotocolBuffer_st;
CORE_LATERAL_DEPARTURE_WARNING_PROTOCOL m_com_rxBackCoreLateralDepartureWarningprotocolBuffer_st;
CORE_LATERAL_DEPARTURE_WARNING_PROTOCOL m_com_rxForeCoreLateralDepartureWarningprotocolBuffer_st;
CORE_MTFV_PROTOCOL                      m_com_rxBackCoreMTFVprotocolBuffer_st;
CORE_MTFV_PROTOCOL                      m_com_rxForeCoreMTFVprotocolBuffer_st;
CORE_OBJECTS_PROTOCOL                   m_com_rxBackCoreObjectsprotocolBuffer_st;
CORE_OBJECTS_PROTOCOL                   m_com_rxForeCoreObjectsprotocolBuffer_st;
CORE_SAFETY_DIAGNOSTICS_PROTOCOL        m_com_rxBackCoreSafetyDiagnosticsprotocolBuffer_st;
CORE_SAFETY_DIAGNOSTICS_PROTOCOL        m_com_rxForeCoreSafetyDiagnosticsprotocolBuffer_st;

/* for Tx PDU status */
COM_TX_PDU_STATUS_ST m_com_txPduStatus_ast[ME_COM_TX_PDU_NUMBER];

/* for Rx PDU status */
COM_RX_PDU_STATUS_ST m_com_rxPduStatus_ast[ME_COM_RX_PDU_NUMBER];
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* INLINE DEFINE                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

const COM_TX_PDU_CFG_ST m_com_txPduCfg_ast[ME_COM_TX_PDU_NUMBER] = {

    /* Tx IPdu Index 0 : Core_Car_sensor */
    {
        /* msgID_u8             */ 0xA2,
        /* txMsgCycle_s16       */ 0,
        /* msgDataLen_u32       */ 136,
        /* msgStructLen_u32     */ 136,
        /* msgBuffer_pu8        */ (uint8*)(&m_com_txForeCoreCarsensorBuffer_st),
    },
};

const COM_RX_PDU_CFG_ST m_com_rxPduCfg_ast[ME_COM_RX_PDU_NUMBER] = {

    /* Rx IPdu Index 0 : Core_Application_Message_protocol */
    {
        /* msgID_u8             */ 0x41,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 116,
        /* msgStructLen_u32     */ 120,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreApplicationMessageprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreApplicationMessageprotocolBuffer_st),
    },
    /* Rx IPdu Index 1 : Core_Boot_Diagnostics_Message_protocol */
    {
        /* msgID_u8             */ 0x10,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 72,
        /* msgStructLen_u32     */ 76,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreBootDiagnosticsMessageprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreBootDiagnosticsMessageprotocolBuffer_st),
    },
    /* Rx IPdu Index 2 : Core_Calibration_Dynamic_protocol */
    {
        /* msgID_u8             */ 0x63,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 64,
        /* msgStructLen_u32     */ 68,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreCalibrationDynamicprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreCalibrationDynamicprotocolBuffer_st),
    },
    /* Rx IPdu Index 3 : Core_Calibration_SPC_protocol */
    {
        /* msgID_u8             */ 0x65,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 24,
        /* msgStructLen_u32     */ 28,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreCalibrationSPCprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreCalibrationSPCprotocolBuffer_st),
    },
    /* Rx IPdu Index 4 : Core_Calibration_Static_protocol */
    {
        /* msgID_u8             */ 0x64,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 32,
        /* msgStructLen_u32     */ 36,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreCalibrationStaticprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreCalibrationStaticprotocolBuffer_st),
    },
    /* Rx IPdu Index 5 : Core_Common_protocol */
    {
        /* msgID_u8             */ 0x52,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 72,
        /* msgStructLen_u32     */ 76,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreCommonprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreCommonprotocolBuffer_st),
    },
    /* Rx IPdu Index 6 : Core_DS_Traffic_Signs_protocol */
    {
        /* msgID_u8             */ 0x5A,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 816,
        /* msgStructLen_u32     */ 820,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreDSTrafficSignsprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreDSTrafficSignsprotocolBuffer_st),
    },
    /* Rx IPdu Index 7 : Core_Failsafe_protocol */
    {
        /* msgID_u8             */ 0x71,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 28,
        /* msgStructLen_u32     */ 32,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreFailsafeprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreFailsafeprotocolBuffer_st),
    },
    /* Rx IPdu Index 8 : Core_FCF_CV_DYN_protocol */
    {
        /* msgID_u8             */ 0xE0,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 196,
        /* msgStructLen_u32     */ 200,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreFCFCVDYNprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreFCFCVDYNprotocolBuffer_st),
    },
    /* Rx IPdu Index 9 : Core_FCF_VD_DYN_protocol */
    {
        /* msgID_u8             */ 0xD8,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 184,
        /* msgStructLen_u32     */ 188,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreFCFVDDYNprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreFCFVDDYNprotocolBuffer_st),
    },
    /* Rx IPdu Index 10 : Core_FCF_VRU_DYN_protocol */
    {
        /* msgID_u8             */ 0xD9,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 140,
        /* msgStructLen_u32     */ 144,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreFCFVRUDYNprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreFCFVRUDYNprotocolBuffer_st),
    },
    /* Rx IPdu Index 11 : Core_High_Low_Beam_protocol */
    {
        /* msgID_u8             */ 0x7C,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 16,
        /* msgStructLen_u32     */ 20,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreHighLowBeamprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreHighLowBeamprotocolBuffer_st),
    },
    /* Rx IPdu Index 12 : Core_Intelligent_Speed_Assist_protocol */
    {
        /* msgID_u8             */ 0x4C,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 8,
        /* msgStructLen_u32     */ 12,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreIntelligentSpeedAssistprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreIntelligentSpeedAssistprotocolBuffer_st),
    },
    /* Rx IPdu Index 13 : Core_Lanes_Adjacent_protocol */
    {
        /* msgID_u8             */ 0x8C,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 172,
        /* msgStructLen_u32     */ 176,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreLanesAdjacentprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreLanesAdjacentprotocolBuffer_st),
    },
    /* Rx IPdu Index 14 : Core_Lanes_Applications_protocol */
    {
        /* msgID_u8             */ 0x8D,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 84,
        /* msgStructLen_u32     */ 88,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreLanesApplicationsprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreLanesApplicationsprotocolBuffer_st),
    },
    /* Rx IPdu Index 15 : Core_Lanes_Host_protocol */
    {
        /* msgID_u8             */ 0x8E,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 136,
        /* msgStructLen_u32     */ 140,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreLanesHostprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreLanesHostprotocolBuffer_st),
    },
    /* Rx IPdu Index 16 : Core_Lanes_Road_Edge_protocol */
    {
        /* msgID_u8             */ 0x8A,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 92,
        /* msgStructLen_u32     */ 96,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreLanesRoadEdgeprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreLanesRoadEdgeprotocolBuffer_st),
    },
    /* Rx IPdu Index 17 : Core_Lateral_Departure_Warning_protocol */
    {
        /* msgID_u8             */ 0x8B,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 92,
        /* msgStructLen_u32     */ 96,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreLateralDepartureWarningprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreLateralDepartureWarningprotocolBuffer_st),
    },
    /* Rx IPdu Index 18 : Core_MTFV_protocol */
    {
        /* msgID_u8             */ 0xDF,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 272,
        /* msgStructLen_u32     */ 276,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreMTFVprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreMTFVprotocolBuffer_st),
    },
    /* Rx IPdu Index 19 : Core_Objects_protocol */
    {
        /* msgID_u8             */ 0x9A,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 2420,
        /* msgStructLen_u32     */ 2424,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreObjectsprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreObjectsprotocolBuffer_st),
    },
    /* Rx IPdu Index 20 : Core_Safety_Diagnostics_protocol */
    {
        /* msgID_u8             */ 0x74,
        /* rxMsgTimeOut_s16     */ 15,
        /* msgDataLen_u32       */ 24,
        /* msgStructLen_u32     */ 28,
        /* msgBackBuffer_pu8    */ (uint8*)(&m_com_rxBackCoreSafetyDiagnosticsprotocolBuffer_st),
        /* msgForeBuffer_pu8    */ (uint8*)(&m_com_rxForeCoreSafetyDiagnosticsprotocolBuffer_st),
    },
};

const COM_CFG_ST m_com_pduCfg_st = {
    /* txPduCfg_pst        */ m_com_txPduCfg_ast,
    /* txPduStatus_st      */ m_com_txPduStatus_ast,

    /* rxPduCfg_pst        */ m_com_rxPduCfg_ast,
    /* rxPduStatus_st      */ m_com_rxPduStatus_ast,
};
