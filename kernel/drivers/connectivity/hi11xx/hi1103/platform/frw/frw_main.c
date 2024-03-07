

/* ͷ�ļ����� */
#include "frw_main.h"
#include "frw_task.h"
#include "frw_event_main.h"
#include "frw_event_deploy.h"
#include "frw_ext_if.h"
#include "frw_timer.h"

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#include "oal_hcc_host_if.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_MAIN_C

/* ȫ�ֱ������� */
frw_init_enum_uint16 en_wlan_driver_init_state_etc = FRW_INIT_STATE_BUTT;
OAL_STATIC frw_event_sub_table_item_stru frw_timeout_event_sub_table[FRW_TIMEOUT_SUB_TYPE_BUTT];

/*
  3 ����ʵ��
 */
oal_uint32 frw_event_fsm_register_etc(oal_void)
{
    frw_timeout_event_sub_table[FRW_TIMEOUT_TIMER_EVENT].p_func = frw_timer_timeout_proc_etc;
    frw_event_table_register_etc(FRW_EVENT_TYPE_TIMEOUT, FRW_EVENT_PIPELINE_STAGE_0, frw_timeout_event_sub_table);

    return OAL_SUCC;
}

oal_int32 frw_main_init_etc(oal_void)
{
    oal_uint32 ul_ret;

    frw_set_init_state_etc(FRW_INIT_STATE_START);

    /* �¼�����ģ���ʼ�� */
    ul_ret = frw_event_init_etc();
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_main_init_etc:: frw_event_init_etc return err code: %d}", ul_ret);
        return -OAL_EFAIL;
    }

    frw_task_init_etc();

#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    /* �¼��˼䲿��ģ���ʼ�� */
    ul_ret = frw_event_deploy_init_etc();
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_main_init_etc:: frw_event_deploy_init_etc return err code: %d}", ul_ret);
        return -OAL_EFAIL;
    }
#endif

    frw_timer_init_etc(FRW_TIMER_DEFAULT_TIME, frw_timer_timeout_proc_event_etc, 0);

    frw_event_fsm_register_etc();

    /* �����ɹ��������ӡ ����״̬ʼ�շ���� */
    frw_set_init_state_etc(FRW_INIT_STATE_FRW_SUCC);

    return OAL_SUCC;
}

oal_void frw_main_exit_etc(oal_void)
{
    /* ж�ض�ʱ�� */
    frw_timer_exit_etc();

    /* ж���¼�����ģ�� */
    frw_event_exit_etc();

    frw_task_exit_etc();

#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    /* �¼��˼䲿��ģ��ж�� */
    frw_event_deploy_exit_etc();
#endif

    /* ж�سɹ�������״̬λ */
    frw_set_init_state_etc(FRW_INIT_STATE_START);

    /* ж�سɹ��������ӡ */

    return;
}

/*
 * �� �� ��  : frw_set_init_state_etc
 * ��������  : ���ó�ʼ��״̬
 * �������  : ��ʼ��״̬
 */
oal_void frw_set_init_state_etc(frw_init_enum_uint16 en_init_state)
{
    if (en_init_state >= FRW_INIT_STATE_BUTT) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_set_init_state_etc:: en_init_state >= FRW_INIT_STATE_BUTT: %d}",
                         en_init_state);
        return;
    }

    en_wlan_driver_init_state_etc = en_init_state;

    return;
}

/*
 * �� �� ��  : frw_get_init_state_etc
 * ��������  : ��ȡ��ʼ��״̬
 */
frw_init_enum_uint16 frw_get_init_state_etc(oal_void)
{
    return en_wlan_driver_init_state_etc;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(frw_main_init_etc);
oal_module_symbol(frw_main_exit_etc);

oal_module_symbol(frw_set_init_state_etc);
oal_module_symbol(frw_get_init_state_etc);