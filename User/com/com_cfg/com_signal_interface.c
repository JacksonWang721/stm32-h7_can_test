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

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DEFINE                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

uint8 com_get_core_application_message_protocol_app_zero_byte_u8(void)
{
    return m_com_rxForeCoreApplicationMessageprotocolBuffer_st.sMsg.APP_Zero_Byte;
}
uint8 com_get_core_application_message_protocol_application_message_version_u8(void)
{
    return m_com_rxForeCoreApplicationMessageprotocolBuffer_st.sMsg.Application_Message_Version;
}


void com_set_core_car_sensor_cin_zero_byte_v(uint8 t_val_u8)
{
    m_com_txForeCoreCarsensorBuffer_st.sMsg.CIN_Zero_Byte = t_val_u8;
}
void com_set_core_car_sensor_reserved_1_v(uint32 t_val_u32)
{
    m_com_txForeCoreCarsensorBuffer_st.sMsg.Reserved_1 = t_val_u32;
}
