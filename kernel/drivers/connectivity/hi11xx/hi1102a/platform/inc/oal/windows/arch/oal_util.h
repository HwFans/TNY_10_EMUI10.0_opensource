

#ifndef __OAL_WINDOWS_UTIL_H__
#define __OAL_WINDOWS_UTIL_H__

/* 其他头文件包含 */
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "hsimu_reg.h"
#include <sys/stat.h>

/* 宏定义 */
/* 32字节序大小端转换 */
#define OAL_SWAP_BYTEORDER_32(_val)  \
    ((((_val)&0x000000FF) << 24) +   \
        (((_val)&0x0000FF00) << 8) + \
        (((_val)&0x00FF0000) >> 8) + \
        (((_val)&0xFF000000) >> 24))

#define OAL_CONST const
/* 获取CORE ID */
#define OAL_GET_CORE_ID() (g_cpuid_register[0])

typedef LONG oal_bitops;

typedef FILE oal_file_stru;
typedef long oal_file_pos;

typedef struct _stat oal_file_stat_stru;

#define OAL_FILE_FAIL OAL_PTR_NULL

#define HEX_DUMP_GROUP_SIZE 32

/* 同linux内核定义 */
#define MIN_NICE            (-20)

#define OAL_LIKELY(_expr)   _expr
#define OAL_UNLIKELY(_expr) _expr

#if defined(_PRE_PC_LINT)
#define OAL_FUNC_NAME "pc_lint"
#else
#define OAL_FUNC_NAME __FUNCTION__
#endif
#define OAL_RET_ADDR OAL_PTR_NULL

/* 将几个字符串按照指定格式合成一个字符串 */
#define OAL_SPRINTF sprintf_s

#ifndef snprintf_s
#define snprintf_s _snprintf_s
#endif

/* 内存读屏障 */
#define OAL_RMB()

/* 内存写屏障 */
#define OAL_WMB()

/* 内存屏障 */
#define OAL_MB()

#define OAL_OFFSET_OF(TYPE, MEMBER) ((unsigned long)&((TYPE *)0)->MEMBER)

#define __OAL_DECLARE_PACKED

#define KERN_EMERG
#define KERN_ALERT
#define KERN_CRIT
#define KERN_ERR
#define KERN_WARNING
#define KERN_NOTICE
#define KERN_INFO
#define KERN_DEBUG

#define KERN_DEFAULT
#define KERN_CONT

#define OAL_IO_PRINT printf

#define OAL_BUG_ON(_con)

#define OAL_WARN_ON(condition) (condition)

#define OAL_VSPRINTF vsprintf_s

/* 虚拟地址转物理地址 */
#define OAL_VIRT_TO_PHY_ADDR(_virt_addr) ((oal_uint32)(_virt_addr))
#define OAL_DSCR_VIRT_TO_PHY(_virt_addr) ((oal_uint32)(_virt_addr))

/* 物理地址转虚拟地址 */
#define OAL_PHY_TO_VIRT_ADDR(_phy_addr) ((oal_uint32 *)(_phy_addr))
#define OAL_DSCR_PHY_TO_VIRT(_phy_addr) ((oal_uint32 *)(_phy_addr))

#define OAL_CFG_FILE_PATH ("C:\\1151_cfg.ini")

#ifndef current
#define current 0
#endif

#define OAL_STRLEN  strlen
#define OAL_MEMCMP  memcmp
#define OAL_STRSTR  strstr
#define OAL_STRCMP  strcmp
#define OAL_STRNCMP strncmp

/* 检查size1不大于size2, 其中size1/2可以使用sizeof(a) */
#define SIZE_OF_SIZE1_NOT_LARGER_THAN_SIZE2_BY_NAME(name, size1, size2)
/* 检查结构体的大小是否不大于特定值 */
#define SIZE_OF_TYPE_NOT_LARGER_THAN_DETECT(type, size)

/* STRUCT定义 */
typedef struct object {
    char *name;
} oal_kobject;

/* OTHERS定义 */
#define OAL_ROUND_DOWN(value, boundary) ((value) & (~((boundary)-1)))
#define OAL_ROUND_UP(value, boundary)   ((((value)-1) | ((boundary)-1)) + 1)

/* us time cost sub */
#define declare_time_cost_stru(name)

#define time_cost_var_start(name)
#define time_cost_var_end(name)
#define time_cost_var_sub(name) 0

#define oal_get_time_cost_start(name)
#define oal_get_time_cost_end(name)
#define oal_calc_time_cost_sub(name)

/*
 * 函 数 名  : oal_bit_atomic_test_and_set
 * 功能描述  : 对某个位进行置1操作，并返回该位置的旧值。
 * 输入参数  : nr: 需要设置的位
 *             p_addr需要置位的变量地址
 * 返 回 值  : 返回原来bit位的值
 */
OAL_INLINE oal_bitops oal_bit_atomic_test_and_set(oal_bitops nr, OAL_VOLATILE oal_bitops *p_addr)
{
    return InterlockedBitTestAndSet(p_addr, nr);
}

/*
 * 函 数 名  : oal_bit_atomic_clear
 * 功能描述  : 封装各个操作系统平台下对某个位进行清0操作。
 * 输入参数  : nr: 需要清零的位
 *             p_addr需要清零的变量地址
 */
OAL_INLINE oal_void oal_bit_atomic_clear(oal_bitops nr, OAL_VOLATILE oal_bitops *p_addr)
{
    InterlockedBitTestAndReset(p_addr, nr);
}

/*
 * 函 数 名  : oal_byteorder_host_to_net_uint16
 * 功能描述  : 将16位本地字节序转换为网络字节序
 * 输入参数  : us_byte: 需要字节序转换的变量
 * 返 回 值  : 转换好的变量
 */
OAL_INLINE oal_uint16 oal_byteorder_host_to_net_uint16(oal_uint16 us_byte)
{
    us_byte = ((us_byte & 0x00FF) << 8) + ((us_byte & 0xFF00) >> 8);

    return us_byte;
}

/*
 * 函 数 名  : oal_byteorder_net_to_host_uint16
 * 功能描述  : 将16位本地字节序转换为网络字节序
 * 输入参数  : us_byte: 需要字节序转换的变量
 * 返 回 值  : 转换好的变量
 */
OAL_INLINE oal_uint16 oal_byteorder_net_to_host_uint16(oal_uint16 us_byte)
{
    us_byte = ((us_byte & 0x00FF) << 8) + ((us_byte & 0xFF00) >> 8);

    return us_byte;
}

/*
 * 函 数 名  : oal_byteorder_host_to_net_uint32
 * 功能描述  : 将32位本地字节序转换为网络字节序
 * 输入参数  : us_byte: 需要字节序转换的变量
 * 返 回 值  : 转换好的变量
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_byteorder_host_to_net_uint32(oal_uint32 ul_byte)
{
    ul_byte = OAL_SWAP_BYTEORDER_32(ul_byte);

    return ul_byte;
}

/*
 * 函 数 名  : oal_byteorder_net_to_host_uint32
 * 功能描述  : 将32位本地字节序转换为网络字节序
 * 输入参数  : us_byte: 需要字节序转换的变量
 * 返 回 值  : 转换好的变量
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_byteorder_net_to_host_uint32(oal_uint32 ul_byte)
{
    ul_byte = OAL_SWAP_BYTEORDER_32(ul_byte);

    return ul_byte;
}

/*
 * 函 数 名  : oal_file_open_rw
 * 功能描述  : 以读写方式打开指定文件，没有则创建
 * 返 回 值  : 成功返回打开的文件，失败返回FAIL
 */
OAL_INLINE oal_file_stru *oal_file_open_rw(const oal_int8 *pc_path)
{
    oal_file_stru *rlt;

    fopen_s(&rlt, pc_path, "w+");

    if (rlt == OAL_PTR_NULL) {
        return OAL_FILE_FAIL;
    }

    return rlt;
}

/*
 * 函 数 名  : oal_file_open_append
 * 功能描述  : 以在尾部加入方式打开指定文件没有则创建
 * 返 回 值  : 成功返回打开的文件，失败返回FAIL
 */
OAL_INLINE oal_file_stru *oal_file_open_append(const oal_int8 *pc_path)
{
    oal_file_stru *rlt;

    fopen_s(&rlt, pc_path, "a");

    if (rlt == OAL_PTR_NULL) {
        return OAL_FILE_FAIL;
    }

    return rlt;
}

/*
 * 函 数 名  : oal_file_open_readonly
 * 功能描述  : 已只读方式打开指定文件，注意确定文件一定存在。
 * 返 回 值  : 成功返回打开的文件，失败返回FAIL
 */
OAL_INLINE oal_file_stru *oal_file_open_readonly(const oal_int8 *pc_path)
{
    oal_file_stru *rlt;

    fopen_s(&rlt, pc_path, "r");

    if (rlt == OAL_PTR_NULL) {
        return OAL_FILE_FAIL;
    }

    return rlt;
}

/*
 * 函 数 名  : oal_file_write
 * 功能描述  : 写文件
 * 输入参数  : file:需要进行写操作的文件
 *             pc_string: 要写进文件字符串的地址
 *             ul_length: 字符串大小
 * 返 回 值  : 成功返回文件，失败返回FAIL
 */
OAL_INLINE oal_file_stru *oal_file_write(oal_file_stru *file, oal_int8 *pc_string, oal_uint32 ul_length)
{
    oal_int32 l_rlt;

    l_rlt = fprintf(file, "%s", pc_string);

    if (l_rlt < 0) {
        return OAL_FILE_FAIL;
    }

    return file;
}

/*
 * 函 数 名  : oal_file_close
 * 功能描述  : 关闭指定的文件
 * 输入参数  : file: 需要关闭的文件
 * 返 回 值  : 成功返回0，失败返回-1
 */
OAL_INLINE oal_int32 oal_file_close(oal_file_stru *file)
{
    oal_int32 l_rlt;

    l_rlt = fclose(file);

    if (l_rlt == EOF) {
        return -1;
    }

    return 0;
}

/*
 * 函 数 名  : oal_file_read
 * 功能描述  : 读取不超过ul_count-1个字符到buf中，读入最后一个字符串，在buf最后加上'\0'
 */
OAL_STATIC OAL_INLINE oal_int32 oal_file_read(oal_file_stru *file,
                                              oal_int8 *puc_buf,
                                              oal_uint32 ul_count)
{
    return (oal_int32)fread(puc_buf, 1, ul_count, file);
}

OAL_STATIC OAL_INLINE oal_int32 oal_file_read_ext(oal_file_stru *file,
                                                  oal_file_pos pos,
                                                  oal_int8 *pc_buf,
                                                  oal_uint32 ul_count)
{
    fseek(file, pos, SEEK_SET);
    return (oal_int32)fread(pc_buf, 1, ul_count, file);
}

/*
 * 函 数 名  : oal_file_size
 * 功能描述  : 计算文件大小
 */
OAL_INLINE oal_int32 oal_file_size(oal_uint32 *pul_file_size)
{
    oal_file_stat_stru st_file_stat;
    oal_int32 l_ret;

    l_ret = _stat(OAL_CFG_FILE_PATH, &st_file_stat);
    if (l_ret != OAL_SUCC) {
        return l_ret;
    }

    *pul_file_size = (oal_uint32)st_file_stat.st_size;

    return OAL_SUCC;
}

/*
 * 函 数 名  : oal_atoi
 * 功能描述  : 字符串类型转换成整形
 * 输入参数  : c_string: 字符串地址
 */
OAL_INLINE oal_int32 oal_atoi(const oal_int8 *c_string)
{
    return atoi(c_string);
}

/*
 * 函 数 名  : oal_itoa
 * 功能描述  : 整形转字符串
 */
OAL_INLINE oal_void oal_itoa(oal_int32 l_val, oal_int8 *c_string, oal_uint8 uc_strlen)
{
    _itoa_s(l_val, c_string, uc_strlen, 10); /* 将字符串转成10进制整形 */
}

OAL_STATIC OAL_INLINE oal_int8 *oal_strtok(oal_int8 *pc_token, OAL_CONST oal_int8 *pc_delemit, oal_int8 **ppc_context)
{
    return strtok_s(pc_token, pc_delemit, ppc_context);
}

OAL_STATIC OAL_INLINE oal_int oal_strtol(OAL_CONST oal_int8 *pc_nptr, oal_int8 **ppc_endptr, oal_int32 l_base)
{
    return strtol(pc_nptr, ppc_endptr, l_base);
}

/*
 * 函 数 名  : oal_udelay
 * 功能描述  : 微秒级延迟函数
 */
OAL_STATIC OAL_INLINE oal_void oal_udelay(oal_uint u_loops)
{
}

/*
 * 函 数 名  : oal_mdelay
 * 功能描述  : 毫秒级延迟函数
 */
OAL_STATIC OAL_INLINE oal_void oal_mdelay(oal_uint u_loops)
{
}

/*
 * 函 数 名  : oal_kallsyms_lookup_name
 * 功能描述  : 根据全局变量名字查找全局变量地址
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_kallsyms_lookup_name(OAL_CONST oal_uint8 *uc_var_name)
{
    return 0;
}

/*
 * 函 数 名  : oal_dump_stack
 * 功能描述  : 打印函数调用栈
 */
OAL_STATIC OAL_INLINE oal_void oal_dump_stack(oal_void)
{
    /* win32下, do nothing */
}

OAL_STATIC OAL_INLINE oal_void oal_msleep(oal_uint32 ul_usecs)
{
}

OAL_STATIC OAL_INLINE oal_void oal_usleep_range(oal_ulong min_us, oal_ulong max_us)
{
}

OAL_STATIC OAL_INLINE errno_t oal_strncpy(oal_int8 *pc_dest, OAL_CONST oal_int8 *pc_src, oal_uint32 ul_len)
{
    return strcpy_s((oal_int8 *)pc_dest, ul_len, pc_src);
}

OAL_STATIC OAL_INLINE oal_void oal_random_ether_addr(oal_uint8 *addr)
{
    addr[0] = 0x22;
    addr[1] = 0x22;
    addr[2] = 0x21;
    addr[3] = 0x22;
    addr[4] = 0x23;
    addr[5] = 0x22;
}

OAL_STATIC OAL_INLINE oal_void oal_print_hex_dump(oal_uint8 *addr, oal_int32 len,
                                                  oal_int32 groupsize, oal_int8 *pre_str)
{
    OAL_REFERENCE(groupsize);
    OAL_REFERENCE(addr);
    OAL_REFERENCE(len);
    OAL_REFERENCE(pre_str);
}

/*
 * 函 数 名  : oal_div_u64
 * 功能描述  : linux uint64 除法
 */
OAL_STATIC OAL_INLINE oal_uint64 oal_div_u64(oal_uint64 dividend, oal_uint32 divisor)
{
    return (dividend / divisor);
}

/*
 * 函 数 名  : oal_div_s64
 * 功能描述  : linux int64 除法
 */
OAL_STATIC OAL_INLINE oal_int64 oal_div_s64(oal_int64 dividend, oal_int32 divisor)
{
    return (dividend / divisor);
}

#endif /* end of oal_util.h */
