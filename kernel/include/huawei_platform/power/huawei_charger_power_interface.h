/*
 * huawei_charger_power_interface.h
 *
 * interface for power module
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _HUAWEI_CHARGER_POWER_INTERFACE_H_
#define _HUAWEI_CHARGER_POWER_INTERFACE_H_

#define POWER_IF_RD_BUF_SIZE  64
#define POWER_IF_WR_BUF_SIZE  256

enum power_if_sysfs_type {
	POWER_IF_SYSFS_BEGIN = 0,
	POWER_IF_SYSFS_ENABLE_CHARGER = POWER_IF_SYSFS_BEGIN,
	POWER_IF_SYSFS_VBUS_IIN_LIMIT,
	POWER_IF_SYSFS_BATT_ICHG_LIMIT,
	POWER_IF_SYSFS_BATT_ICHG_RATIO,
	POWER_IF_SYSFS_BATT_VTERM_DEC,
	POWER_IF_SYSFS_RT_TEST_TIME,
	POWER_IF_SYSFS_RT_TEST_RESULT,
	POWER_IF_SYSFS_HOTA_IIN_LIMIT,
	POWER_IF_SYSFS_STARTUP_IIN_LIMIT,
	POWER_IF_SYSFS_END,

};

enum power_if_op_user {
	POWER_IF_OP_USER_BEGIN = 0,
	POWER_IF_OP_USER_DEFAULT = POWER_IF_OP_USER_BEGIN, /* for default */
	POWER_IF_OP_USER_RC, /* for rc file */
	POWER_IF_OP_USER_HIDL, /* for hidl interface */
	POWER_IF_OP_USER_HEALTHD, /* for healthd daemon */
	POWER_IF_OP_USER_LIMIT_CURRENT, /* for limit_current native */
	POWER_IF_OP_USER_CHARGE_MONITOR, /* for charge_monitor native */
	POWER_IF_OP_USER_ATCMD, /* for atcmd native */
	POWER_IF_OP_USER_THERMAL, /* for thermal daemon */
	POWER_IF_OP_USER_AI, /* for ai */
	POWER_IF_OP_USER_RUNNING, /* for running apk */
	POWER_IF_OP_USER_FWK, /* for framework */
	POWER_IF_OP_USER_APP, /* for app */
	POWER_IF_OP_USER_SHELL, /* for shell command */
	POWER_IF_OP_USER_KERNEL, /* for kernel space */
	POWER_IF_OP_USER_BSOH, /* for bsoh daemon */
	POWER_IF_OP_USER_END,
};

enum power_if_op_type {
	POWER_IF_OP_TYPE_BEGIN = 0,
	POWER_IF_OP_TYPE_DCP = POWER_IF_OP_TYPE_BEGIN, /* standard */
	POWER_IF_OP_TYPE_OTG, /* otg */
	POWER_IF_OP_TYPE_HVC, /* hvc */
	POWER_IF_OP_TYPE_PD, /* pd */
	POWER_IF_OP_TYPE_LVC, /* lvc */
	POWER_IF_OP_TYPE_SC, /* sc */
	POWER_IF_OP_TYPE_WL, /* wireless */
	POWER_IF_OP_TYPE_WL_SC, /* wireless sc */
	POWER_IF_OP_TYPE_ALL, /* for all type */
	POWER_IF_OP_TYPE_END,
};

enum power_if_error_code {
	POWER_IF_ERRCODE_INVAID_OP = -22,
	POWER_IF_ERRCODE_FAIL = -1,
	POWER_IF_ERRCODE_PASS = 0,
};

/* power interface operator */
struct power_if_ops {
	const char *type_name;
	int (*set_enable_charger)(unsigned int value);
	int (*get_enable_charger)(unsigned int *value);
	int (*set_iin_limit)(unsigned int value);
	int (*get_iin_limit)(unsigned int *value);
	int (*set_ichg_limit)(unsigned int value);
	int (*get_ichg_limit)(unsigned int *value);
	int (*set_ichg_ratio)(unsigned int value);
	int (*get_ichg_ratio)(unsigned int *value);
	int (*set_vterm_dec)(unsigned int value);
	int (*get_vterm_dec)(unsigned int *value);
	int (*get_rt_test_time)(unsigned int *value);
	int (*get_rt_test_result)(unsigned int *value);
	int (*get_hota_iin_limit)(unsigned int *value);
	int (*get_startup_iin_limit)(unsigned int *value);
};

/* power interface info */
struct power_if_device_info {
	struct device *dev;
	unsigned int total_ops;
	struct power_if_ops *ops[POWER_IF_OP_TYPE_END];
};

int power_if_ops_register(struct power_if_ops *ops);
int power_if_kernel_sysfs_get(unsigned int type, unsigned int sysfs_type,
	unsigned int *value);
int power_if_kernel_sysfs_set(unsigned int type, unsigned int sysfs_type,
	unsigned int value);

#endif /* _HUAWEI_CHARGER_POWER_INTERFACE_H_ */
