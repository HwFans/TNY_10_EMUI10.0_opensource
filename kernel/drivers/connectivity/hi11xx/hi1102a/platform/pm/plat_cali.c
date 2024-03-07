

/* ͷ�ļ����� */
#define HISI_NVRAM_SUPPORT

#include "plat_cali.h"
#include "plat_firmware.h"
#include "plat_debug.h"
#include "securec.h"
#include "hisi_ini.h"

/* �궨�� */
#define RF_CALI_DATA_BUF_LEN (sizeof(oal_cali_param_stru))

/* ȫ�ֱ������� */
/* ����У׼���ݵ�buf */
oal_uint8 *pucCaliDataBuf = NULL;
oal_uint8 g_uc_netdev_is_open = OAL_FALSE;

/* add for hi1102a bfgx */
oal_uint8 *pucBfgxCaliDataBuf_hi1102a = NULL;

/* ���岻�ܳ���BFGX_BT_CUST_INI_SIZE/4 (128) */
bfgx_ini_cmd bfgx_ini_config_cmd_hi1102a[BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                   0x0505 },
    { "bt_edrpow_offset",              0 },
    { "bt_blepow_offset",              0 },
    { "bt_fem_control",                0 },
    { "bt_cali_swtich_all",            0 },
    { "bt_ant_num_bt",                 0 },
    { "bt_frame_end_detection_switch", 0 },
    { "bt_reserved1",                  0 },
    { "bt_reserved2",                  0 },
    { "bt_reserved3",                  0 },
    { "bt_reserved4",                  0 },
    { "bt_reserved5",                  0 },
    { "bt_reserved6",                  0 },
    { "bt_reserved7",                  0 },
    { "bt_reserved8",                  0 },
    { "bt_reserved9",                  0 },
    { "bt_reserved10",                 0 },

    { NULL, 0 }
};

/* ���岻�ܳ���BFGX_BT_CUST_INI_SIZE/4 (128) */
int32 bfgx_cust_ini_data_hi1102a[BFGX_BT_CUST_INI_SIZE / 4] = {0};

/*
 * �� �� ��  : get_cali_count
 * ��������  : ����У׼�Ĵ������״ο���У׼ʱΪ0���Ժ����
 * �������  : uint32 *count:���ú�������У׼�����ĵ�ַ
 * �������  : count:�Կ����������Ѿ�У׼�Ĵ���
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
oal_int32 get_cali_count(oal_uint32 *count)
{
    oal_cali_param_stru *pst_cali_data = NULL;
    oal_uint16 cali_count;
    oal_uint32 cali_parm;

    if (count == NULL) {
        PS_PRINT_ERR("count is NULL\n");
        return -EFAIL;
    }

    if (pucCaliDataBuf == NULL) {
        PS_PRINT_ERR("pucCaliDataBuf is NULL\n");
        return -EFAIL;
    }

    pst_cali_data = (oal_cali_param_stru *)pucCaliDataBuf;
    cali_count = pst_cali_data->st_cali_update_info.ul_cali_time;
    cali_parm = *(oal_uint32 *)&(pst_cali_data->st_cali_update_info);

    PS_PRINT_WARNING("cali count is [%d], cali update info is [%d]\n", cali_count, cali_parm);

    *count = cali_parm;

    return SUCC;
}

/*
 * �� �� ��  : get_bfgx_cali_data
 * ��������  : ���ر���bfgxУ׼���ݵ��ڴ��׵�ַ�Լ�����
 * �������  : uint8  *buf:���ú�������bfgxУ׼���ݵ��׵�ַ
 *             uint32 *len:���ú�������bfgxУ׼�����ڴ泤�ȵĵ�ַ
 *             uint32 buf_len:buf�ĳ���
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 get_bfgx_cali_data(oal_uint8 *buf, oal_uint32 *len, oal_uint32 buf_len)
{
    oal_cali_param_stru *pst_cali_data = NULL;
    oal_uint32 bfgx_cali_data_len;
    oal_int32 ret;

    PS_PRINT_INFO("%s\n", __func__);

    if (unlikely(buf == NULL)) {
        PS_PRINT_ERR("buf is NULL\n");
        return -EFAIL;
    }

    if (unlikely(len == NULL)) {
        PS_PRINT_ERR("len is NULL\n");
        return -EFAIL;
    }

    if (unlikely(pucBfgxCaliDataBuf_hi1102a == NULL)) {
        PS_PRINT_ERR("pucBfgxCaliDataBuf_hi1102a is NULL\n");
        return -EFAIL;
    }

    if (pucCaliDataBuf == NULL) {
        PS_PRINT_ERR("pucCaliDataBuf is NULL\n");
        return -EFAIL;
    }

    bfgx_cali_data_len = sizeof(oal_bfgn_cali_param_stru);
    if (bfgx_cali_data_len > BFGX_BT_CALI_DATA_SIZE) {
        PS_PRINT_ERR("bfgx data buffer[%d] is smaller than struct size[%d]\n",
                     BFGX_BT_CALI_DATA_SIZE, bfgx_cali_data_len);
        return -EFAIL;
    }

    pst_cali_data = (oal_cali_param_stru *)pucCaliDataBuf;
    ret = memcpy_s(pucBfgxCaliDataBuf_hi1102a, BFGX_CALI_DATA_BUF_LEN,
                   (oal_uint8 *)&(pst_cali_data->st_bfgn_cali_data), bfgx_cali_data_len);
    if (ret != EOK) {
        PS_PRINT_ERR("get_bfgx_cali_data: memcpy_s failed,ret = %d\n", ret);
        return -EFAIL;
    }

#ifdef HISI_NVRAM_SUPPORT
    if (bfgx_nv_data_init() != OAL_SUCC) {
        PS_PRINT_ERR("bfgx nv data init fail!\n");
    }
#endif

    bfgx_cali_data_len = sizeof(bfgx_cali_data_stru);
    ret = memcpy_s(buf, buf_len, pucBfgxCaliDataBuf_hi1102a, bfgx_cali_data_len);
    if (ret != EOK) {
        PS_PRINT_ERR("cali buf len[%d] is smaller than struct size[%d] ret=%d\n", buf_len, bfgx_cali_data_len, ret);
        return -EFAIL;
    }
    *len = bfgx_cali_data_len;

    return SUCC;
}

/*
 * �� �� ��  : get_cali_data_buf_addr
 * ��������  : ���ر���У׼���ݵ��ڴ��ַ
 * �� �� ֵ  : g_pucCaliDataBuf�ĵ�ַ��Ҳ������NULL
 */
void *get_cali_data_buf_addr(void)
{
    return pucCaliDataBuf;
}

/*
 * �� �� ��  : bfgx_get_cust_ini_data_buf
 * ��������  : ���ر���bfgx ini���ƻ����ݵ��ڴ��ַ
 * �������  : pul_len : bfgx ini���ƻ�����buf�ĳ���
 * �� �� ֵ  : bfgx ini����buf�ĵ�ַ��Ҳ������NULL
 */
void *bfgx_get_cust_ini_data_buf(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (pucBfgxCaliDataBuf_hi1102a == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (bfgx_cali_data_stru *)pucBfgxCaliDataBuf_hi1102a;

    *pul_len = sizeof(pst_bfgx_cali_data_buf->auc_bt_cust_ini_data);

    PS_PRINT_INFO("bfgx cust ini buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_bt_cust_ini_data;
}

/*
 * �� �� ��  : bfgx_get_nv_data_buf
 * ��������  : ���ر���bfgx nv���ݵ��ڴ��ַ
 * �������  : nv buf�ĳ���
 * �� �� ֵ  : bfgx nv����buf�ĵ�ַ��Ҳ������NULL
 */
void *bfgx_get_nv_data_buf(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (pucBfgxCaliDataBuf_hi1102a == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (bfgx_cali_data_stru *)pucBfgxCaliDataBuf_hi1102a;

    *pul_len = sizeof(pst_bfgx_cali_data_buf->auc_nv_data);

    PS_PRINT_INFO("bfgx nv buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_nv_data;
}

EXPORT_SYMBOL(get_cali_data_buf_addr);
EXPORT_SYMBOL(g_uc_netdev_is_open);

/*
 * �� �� ��  : plat_bfgx_cali_data_test
 * ��������  : test
 */
void plat_bfgx_cali_data_test(void)
{
    oal_cali_param_stru *pst_cali_data = NULL;
    oal_uint32 *p_test = NULL;
    oal_uint32 count;
    oal_uint32 i;

    pst_cali_data = (oal_cali_param_stru *)get_cali_data_buf_addr();
    if (pst_cali_data == NULL) {
        PS_PRINT_ERR("get_cali_data_buf_addr failed\n");
        return;
    }

    p_test = (oal_uint32 *)&(pst_cali_data->st_bfgn_cali_data);
    count = sizeof(oal_bfgn_cali_param_stru) / sizeof(oal_uint32);

    for (i = 0; i < count; i++) {
        p_test[i] = i;
    }

    return;
}

/*
 * �� �� ��  : cali_data_buf_malloc
 * ��������  : ���䱣��У׼���ݵ��ڴ�
 * �� �� ֵ  : 0��ʾ����ɹ���-1��ʾ����ʧ��
 */
oal_int32 cali_data_buf_malloc(void)
{
    pucCaliDataBuf = OS_KZALLOC_GFP(RF_CALI_DATA_BUF_LEN);
    if (pucCaliDataBuf == NULL) {
        PS_PRINT_ERR("malloc for pucCaliDataBuf fail\n");
        return -EFAIL;
    }

    pucBfgxCaliDataBuf_hi1102a = (oal_uint8 *)OS_KZALLOC_GFP(BFGX_CALI_DATA_BUF_LEN);
    if (pucBfgxCaliDataBuf_hi1102a == NULL) {
        OS_MEM_KFREE(pucCaliDataBuf);
        pucCaliDataBuf = NULL;
        PS_PRINT_ERR("malloc for pucBfgxCaliDataBuf_hi1102a fail\n");
        return -EFAIL;
    }

    return SUCC;
}

/*
 * �� �� ��  : cali_data_buf_free
 * ��������  : �ͷű���У׼���ݵ��ڴ�
 */
void cali_data_buf_free(void)
{
    if (pucCaliDataBuf != NULL) {
        OS_MEM_KFREE(pucCaliDataBuf);
    }
    pucCaliDataBuf = NULL;

    if (pucBfgxCaliDataBuf_hi1102a != NULL) {
        OS_MEM_KFREE(pucBfgxCaliDataBuf_hi1102a);
    }
    pucBfgxCaliDataBuf_hi1102a = NULL;
}

/*
 * �� �� ��  : bfgx_cust_ini_init
 * ��������  : btУ׼���ƻ����ʼ��
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 bfgx_cust_ini_init(void)
{
    int32 i;
    int32 l_ret = INI_FAILED;
    int32 l_cfg_value;
    int32 l_ori_val;
    int8 *pst_buf = NULL;
    uint32 ul_len;

    for (i = 0; i < BFGX_CFG_INI_BUTT; i++) {
        l_ori_val = bfgx_ini_config_cmd_hi1102a[i].init_value;

        /* ��ȡini������ֵ */
        l_ret = get_cust_conf_int32(INI_MODU_DEV_BT, bfgx_ini_config_cmd_hi1102a[i].name, &l_cfg_value);
        if (l_ret == INI_FAILED) {
            bfgx_cust_ini_data_hi1102a[i] = l_ori_val;
            PS_PRINT_WARNING("bfgx read ini file failed cfg_id[%d],default value[%d]!", i, l_ori_val);
            continue;
        }

        bfgx_cust_ini_data_hi1102a[i] = l_cfg_value;

        PS_PRINT_INFO("bfgx ini init [id:%d] [%s] changed from [%d]to[%d]",
                      i, bfgx_ini_config_cmd_hi1102a[i].name, l_ori_val, l_cfg_value);
    }

    pst_buf = bfgx_get_cust_ini_data_buf(&ul_len);
    if (pst_buf == NULL) {
        PS_PRINT_ERR("get cust ini buf fail!");
        return INI_FAILED;
    }

    l_ret = memcpy_s(pst_buf, ul_len, bfgx_cust_ini_data_hi1102a, ul_len);
    if (l_ret != EOK) {
        PS_PRINT_ERR("bfgx_cust_ini_init: memcpy_s failed, ret=%d\n!", l_ret);
        return INI_FAILED;
    }

    return INI_SUCC;
}

#ifdef HISI_NVRAM_SUPPORT
/*
 * �� �� ��  : bfgx_nv_data_init
 * ��������  : bt У׼NV��ȡ
 */
oal_int32 bfgx_nv_data_init(void)
{
    int32 l_ret;
    int8 *pst_buf = NULL;
    uint32 ul_len;

    oal_uint8 bt_cal_nvram_tmp[OAL_BT_NVRAM_DATA_LENGTH];

    l_ret = read_conf_from_nvram(bt_cal_nvram_tmp, OAL_BT_NVRAM_DATA_LENGTH,
                                 OAL_BT_NVRAM_NUMBER, OAL_BT_NVRAM_NAME);
    if (l_ret != INI_SUCC) {
        PS_PRINT_ERR("bfgx_nv_data_init::BT read NV error!");
        // last byte of NV ram is used to mark if NV ram is failed to read.
        bt_cal_nvram_tmp[OAL_BT_NVRAM_DATA_LENGTH - 1] = OAL_TRUE;
    }
    else {
        // last byte of NV ram is used to mark if NV ram is failed to read.
        bt_cal_nvram_tmp[OAL_BT_NVRAM_DATA_LENGTH - 1] = OAL_FALSE;
    }

    pst_buf = bfgx_get_nv_data_buf(&ul_len);
    if (pst_buf == NULL) {
        PS_PRINT_ERR("get bfgx nv buf fail!");
        return INI_FAILED;
    }

    l_ret = memcpy_s(pst_buf, ul_len, bt_cal_nvram_tmp, OAL_BT_NVRAM_DATA_LENGTH);
    if (l_ret != EOK) {
        PS_PRINT_ERR("bfgx_nv_data_init: memcpy_s failed, ret=%d\n!", l_ret);
        return INI_FAILED;
    }
    PS_PRINT_INFO("bfgx_nv_data_init SUCCESS");
    return INI_SUCC;
}
#endif

/*
 * �� �� ��  : bfgx_customize_init
 * ��������  : bfgx���ƻ����ʼ������ȡini�����ļ�����ȡnv����
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 bfgx_customize_init(void)
{
    int32 ret;

    /* �������ڱ���У׼���ݵ�buffer */
    ret = cali_data_buf_malloc();
    if (ret != OAL_SUCC) {
        PS_PRINT_ERR("alloc cali data buf fail\n");
        return INI_FAILED;
    }

    ret = bfgx_cust_ini_init();
    if (ret != OAL_SUCC) {
        PS_PRINT_ERR("bfgx ini init fail!\n");
        cali_data_buf_free();
        return INI_FAILED;
    }

#ifdef HISI_NVRAM_SUPPORT
    ret = bfgx_nv_data_init();
    if (ret != OAL_SUCC) {
        PS_PRINT_ERR("bfgx nv data init fail!\n");
        cali_data_buf_free();
        return INI_FAILED;
    }
#endif

    return INI_SUCC;
}
