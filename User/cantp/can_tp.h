#ifndef _CAN_TP_H_
#define _CAN_TP_H_

#ifdef  __cplusplus
extern "C"
{
#endif  /* defined(__cplusplus) */


/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define PCI_TYPE_SINGLE_FRAME               0x00
#define PCI_TYPE_FIRST_FRAME                0x01
#define PCI_TYPE_CONSECUTIVE_FRAME          0x02
#define PCI_TYPE_FLOW_CONTROL               0x03

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
typedef enum tagCanTpStatus
{
    SINGLEFRAME_STATUS_RX = 0,
    MULTIFRAMEFRAME_STATUS_RX,
    SINGLEFRAME_STATUS_TX,
    MULTIFRAMEFRAME_STATUS_TX,
    IDLE,
} CANTP_STATUS;

typedef enum tagFrameType
{
    SINGLE_FRAME_TYPE = 0,
    FIRST_FRAME_TYPE,
    CONSECUTIVE_FRAME_TYPE,
    FLOW_CONTROL_TYPE,
    IDLE_TYPE,
} FRAME_TYPE;

extern CANTP_STATUS m_enmCanTpStatus;
extern FRAME_TYPE m_enmFrameType;
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern void can_tp_init(void);

extern Std_ReturnType can_tp_onrecv_data(const uint8 * pData, uint8 uDataLen, PROTOCAL_FLAG_TYPE IsPhyAddr);

extern Std_ReturnType can_tp_transmit(const uint8 * pData, uint16 uDataLen);

extern void can_tp_final(void);

extern void can_tp_monitor(void);

#ifdef  __cplusplus
}
#endif  /* defined(__cplusplus) */

#endif /* _CAN_TP_H_ */
