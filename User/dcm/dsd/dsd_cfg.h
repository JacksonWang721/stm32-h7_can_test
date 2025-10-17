#ifndef _DSD_CFG_H_
#define _DSD_CFG_H_

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define SERVICE_TABLE_MAX                   14U

typedef struct tagServiceTableItem
{
    uint8 uSID;
    uint8 (* ptr_srv_func)(void);
    PROTOCAL_FLAG_TYPE IsfuncAddrAllow;
    SESSION_STATE enmSessionLv;
    SECURITY_LEVEL enmSecurityLv;
    SESSION_STATE (* ptr_get_session)(void);
    SECURITY_LEVEL (* ptr_get_security_level)(void);
    
}SERVICE_TABLE_ITEM, * LP_SERVICE_TABLE_ITEM;

typedef uint8 (* PTR_SRV_FUNC)(void);
typedef SESSION_STATE (* PTR_GET_SESSION)(void);
typedef SECURITY_LEVEL (* PTR_GET_SECURITY_LEVEL)(void);

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern SERVICE_TABLE_ITEM m_sServiceTable[SERVICE_TABLE_MAX];

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/


#endif /* _DSD_CFG_H_ */

