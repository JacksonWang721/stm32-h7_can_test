#ifndef _ME_COM_TYPES_H_
#define _ME_COM_TYPES_H_

#include "protocol_common_types.h"
#include "me_com_protocol_struct.h"

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef _SUB_EQ4_SIL
#define COM_READ_SPI_MSG_MAX_LEN              620U
#endif

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* TYPEDEFS DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
typedef enum tagMecomState
{
    COM_INIT = 0,
    COM_UNINIT = 1
}COM_STATE_ENM;

typedef enum tagMeComPduState
{
    COM_PDU_UNABLE = 0,
    COM_PDU_ENABLE = 1
}COM_PDU_STATE_ENM;

typedef enum tagMeComTxPduState 
{
    COM_TX_PDU_IDLE = 0,
    COM_TX_PDU_PENDING = 1
}COM_TX_PDU_STATE_ENM;

typedef enum tagMeComRxPduBackState
{
    COM_RX_PDU_BACK_IDLE = 0,
    COM_RX_PDU_BACK_BUSY = 1
}COM_RX_PDU_BACK_STATE_ENM;

typedef enum tagMeComRxPduForeState
{
    COM_RX_PDU_FORE_NEVER_UPDATE = 0,
    COM_RX_PDU_FORE_UPDATE_EVER = 1,
    COM_RX_PDU_FORE_HAVE_UPDATE = 3
}COM_RX_PDU_FORE_STATE_ENM;

typedef struct tagMeComTxPduStatus
{
    COM_PDU_STATE_ENM       pduState_enm;
    COM_TX_PDU_STATE_ENM    txPduState_enm;
    sint16                    txCycleCnt_s16;
    boolean                   txEvent_b;
}COM_TX_PDU_STATUS_ST, * PTR_COM_TX_PDU_STATUS_ST;

typedef struct tagMeComRxPduStatus
{
    COM_PDU_STATE_ENM           pduState_enm;
    COM_RX_PDU_BACK_STATE_ENM   rxPduBackBufState_enm;
    COM_RX_PDU_FORE_STATE_ENM   rxPduForeBufState_enm;
    sint16						  rxTimeOutCnt_s16;
}COM_RX_PDU_STATUS_ST, * PTR_COM_RX_PDU_STATUS_ST;

typedef struct tagMeComTxPduCfg
{
    uint8      msgID_u8;
    sint16     txMsgCycle_s16;
    uint32     msgDataLen_u32;
    uint32     msgStructLen_u32;
    uint8*     msgBuffer_pu8;
}COM_TX_PDU_CFG_ST, * PTR_COM_TX_PDU_CFG_ST;

typedef struct tagMeComRxPduCfg
{
    uint8      msgID_u8;
    sint16     rxMsgTimeOut_s16;
    uint32     msgDataLen_u32;
    uint32     msgStructLen_u32;
    uint8*     msgBackBuffer_pu8;
    uint8*     msgForeBuffer_pu8;
}COM_RX_PDU_CFG_ST, * PTR_COM_RX_PDU_CFG_ST;

typedef struct tagMeComCfg
{
    const COM_TX_PDU_CFG_ST   * txPduCfg_pst;
    PTR_COM_TX_PDU_STATUS_ST    txPduStatus_st; 

    const COM_RX_PDU_CFG_ST   * rxPduCfg_pst;
    PTR_COM_RX_PDU_STATUS_ST    rxPduStatus_st;
}COM_CFG_ST, * PTR_COM_CFG_ST;

#ifdef _SUB_EQ4_SIL
typedef struct tagSpiMsgBuf

{
    uint32 spiDataBlockIndicator_u32;
    uint32 spiTimeStampInMs_u32;
    uint32 spiMsgBuffer_au32[COM_READ_SPI_MSG_MAX_LEN];
}COM_SPI_MSG_ST;
#endif
#endif /* _ME_COM_TYPES_H_ */

