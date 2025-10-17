/*--------------------------------------------------------------------------*/
/*                                                                          */
/*INCLUDE FILE                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "std_type.h"
#include "dsl.h"
#include "dsd_cfg.h"

/*supported uds services*/
#include "uds_session_control.h"
#include "uds_ecu_reset.h"
#include "uds_security_access.h"
#include "uds_read_data_by_identifier.h"
#include "uds_write_data_by_identifier.h"
#include "uds_routine_control.h"
#include "uds_request_download.h"
#include "uds_transfer_data.h"
#include "uds_request_transfer_exit.h"
#include "uds_control_dtc_setting.h"
#include "uds_communication_control.h"
#include "eq_request_download.h"
#include "eq_transfer_data.h"
#include "eq_request_transfer_exit.h"

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
SERVICE_TABLE_ITEM m_sServiceTable[SERVICE_TABLE_MAX] = 
{
    {
        0x10,
        &uds_session_control,
        PROTOCAL_FLAG_TRUE,
        DEFAULT_SESSION,
        SECURITY_LV_NONE,
        NULL,
        NULL,
    },
    {
        0x11,
        &uds_ecu_reset,
        PROTOCAL_FLAG_TRUE,
        DEFAULT_SESSION,
        SECURITY_LV_NONE,
        NULL,
        NULL,
    },
    {
        0x27,
        &uds_security_access,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_NONE,
        NULL,
        NULL,
    },
    {
        0x22,
        &uds_read_data_by_identifier,
        PROTOCAL_FLAG_TRUE,
        DEFAULT_SESSION,
        SECURITY_LV_NONE,
        NULL,
        NULL,
    },
    {
        0x2E,
        &uds_write_data_by_identifier,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
    {
        0x31,
        &uds_routine_control,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        &uds_routine_control_get_session,
        &uds_routine_control_get_security_level,
    },
    {
        0x34,
        &uds_request_download,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
    {
        0x36,
        &uds_transfer_data,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
    {
        0x37,
        &uds_request_transfer_exit,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
    {
        0x85,
        &uds_control_dtc_setting,
        PROTOCAL_FLAG_TRUE,
        EXTENDED_DIAGNOSTIC_SESSION,
        SECURITY_LV_NONE,
        NULL,
        NULL,
    },
    {
        0x28,
        &uds_communication_control,
        PROTOCAL_FLAG_TRUE,
        EXTENDED_DIAGNOSTIC_SESSION,
        SECURITY_LV_NONE,
        NULL,
        NULL,
    },
    {
        0x80,
        &eq_request_download,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
    {
        0x81,
        &eq_transfer_data,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
    {
        0x82,
        &eq_request_transfer_exit,
        PROTOCAL_FLAG_FALSE,
        PROGRAM_SESSION,
        SECURITY_LV_2,
        NULL,
        NULL,
    },
};

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

    
