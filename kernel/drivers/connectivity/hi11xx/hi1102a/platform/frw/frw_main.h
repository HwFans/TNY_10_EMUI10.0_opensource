

#ifndef __FRW_MAIN_H__
#define __FRW_MAIN_H__

/* 其他头文件包含 */
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_MAIN_H

/* 宏定义 */
#define FRW_ERR_LOG(_uc_vap_id, _puc_string)
#define FRW_ERR_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define FRW_ERR_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define FRW_ERR_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define FRW_ERR_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define FRW_ERR_VAR(_uc_vap_id, _c_fmt, ...)

#define FRW_WARNING_LOG(_uc_vap_id, _puc_string)
#define FRW_WARNING_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define FRW_WARNING_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define FRW_WARNING_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define FRW_WARNING_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define FRW_WARNING_VAR(_uc_vap_id, _c_fmt, ...)

#define FRW_INFO_LOG(_uc_vap_id, _puc_string)
#define FRW_INFO_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define FRW_INFO_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define FRW_INFO_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define FRW_INFO_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define FRW_INFO_VAR(_uc_vap_id, _c_fmt, ...)

#ifdef _PRE_DEBUG_MODE
#define FRW_EVENT_INTERNAL(_puc_macaddr, _uc_vap_id, en_event_type, output_data, data_len)  \
                    oam_event_report(_puc_macaddr, _uc_vap_id, OAM_MODULE_ID_FRW, en_event_type, output_data, data_len)
#else
#define FRW_EVENT_INTERNAL(_puc_macaddr, _uc_vap_id, en_event_type, output_data, data_len)
#endif

#define FRW_TIMER_DEFAULT_TIME 10

/* 枚举定义 */
typedef enum {
    FRW_TIMEOUT_TIMER_EVENT, /* 以FRW_TIMER_DEFAULT_TIME为周期的定时器事件子类型 */

    FRW_TIMEOUT_SUB_TYPE_BUTT
} hmac_timeout_event_sub_type_enum;
typedef oal_uint8 hmac_timeout_event_sub_type_enum_uint8;

/* 函数声明 */
extern oal_int32 frw_main_init(oal_void);
extern oal_void frw_main_exit(oal_void);
extern oal_void frw_set_init_state(frw_init_enum_uint16 en_init_state);
extern frw_init_enum_uint16 frw_get_init_state(oal_void);

#endif /* end of frw_main */
