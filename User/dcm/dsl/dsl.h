#ifndef _DSL_H_
#define _DSL_H_

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
typedef struct tagMsgContext
{
    uint8 * pReqDataBuffer;
    uint16 uReqDataLen;
    
    uint8 * pResDataBuffer;
    uint16 uResDataLen;
    
}MSG_CONTEXT, * LP_MSG_CONTEXT;

typedef enum tagSessionState
{
    DEFAULT_SESSION = 1,
    PROGRAM_SESSION = 2,
    EXTENDED_DIAGNOSTIC_SESSION = 3,
    
}SESSION_STATE, * LP_SESSION_STATE;

typedef enum tagSecurityLevel
{
    SECURITY_LV_NONE = 0,
    SECURITY_LV_1 = 1,
    SECURITY_LV_2 = 2,
    
}SECURITY_LEVEL, * LP_SECURITY_LEVEL;

#define TESTER_PRESENT          0x3E
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern MSG_CONTEXT dcom_sMsgContext;
extern SESSION_STATE m_enmSessionState;
extern boolean isfunaddr;

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern void dsl_init(void);

extern Std_ReturnType dsl_frame_rx_prehandle(uint8 uFrameType, uint8 const * pFrameData, PROTOCAL_FLAG_TYPE IsPhyAddr);

extern Std_ReturnType dsl_provide_rx_buffer(uint8 * * pRxBufferPtr, uint16 uLen);
extern void dsl_rx_indication(PROTOCAL_FLAG_TYPE IsRxSuccess);

extern void dsl_pending_transmit(void);
extern void dsl_response_transmit(void);
extern void dsl_tx_comfirmation(void);
extern void dsl_no_tx_comfirmation(void);

extern SESSION_STATE dsl_get_session_level(void);
extern void dsl_set_session_level(SESSION_STATE enmSession);

extern SECURITY_LEVEL dsl_get_security_level(void);
extern void dsl_set_security_level(SECURITY_LEVEL enmSecurityLv);

extern void dcm_proc(void);
extern void dsl_monitor_proc(void);

extern void dsl_final(void);

#endif /* _DSL_H_ */

