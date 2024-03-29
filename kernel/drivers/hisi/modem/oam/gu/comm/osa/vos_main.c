/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*****************************************************************************/
/*                                                                           */
/*                Copyright 1999 - 2003, Huawei Tech. Co., Ltd.              */
/*                           ALL RIGHTS RESERVED                             */
/*                                                                           */
/* FileName: vos_main.c                                                      */
/*                                                                           */
/* Author: Yang Xiangqian                                                    */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* Date: 2006-10                                                             */
/*                                                                           */
/* Description: implement root function                                      */
/*                                                                           */
/* Others:                                                                   */
/*                                                                           */
/* History:                                                                  */
/* 1. Date:                                                                  */
/*    Author:                                                                */
/*    Modification: Create this file                                         */
/*                                                                           */
/* 2. Date: 2006-10                                                          */
/*    Author: Xu Cheng                                                       */
/*    Modification: Standardize code                                         */
/*                                                                           */
/* 3. Date: 2007-03-10                                                       */
/*    Author: Xu Cheng                                                       */
/*    Modification: A32D07254                                                */
/*                                                                           */
/*****************************************************************************/

#include "v_typdef.h"
#include "v_root.h"
#include "vos_config.h"
#include "v_IO.h"
#include "v_blkMem.h"
#include "v_queue.h"
#include "v_sem.h"
#include "v_timer.h"
#include "vos_Id.h"
#include "v_int.h"
#include "NVIM_Interface.h"
#include "v_private.h"
#include "mdrv.h"
#include "pam_tag.h"

/* LINUX 不支持 */
#if (VOS_VXWORKS== VOS_OS_VER)
#include "stdio.h"
#endif



/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_VOS_MAIN_C
#define    THIS_MODU           mod_pam_osa

typedef struct
{
    VOS_UINT32 ulStartUpStage;
    VOS_UINT32 ulStep;
    VOS_UINT16 usFidInitStep;
    VOS_UINT16 usFidInitId;
    VOS_UINT16 usPidInitStep;
    VOS_UINT16 usPidInitId;
}VOS_START_ERR_STEP_STRU;

extern VOS_UINT32       g_ulOmFidInit ;
extern VOS_UINT32       g_ulOmPidInit ;
extern VOS_UINT16       g_usFidInitStep;
extern VOS_UINT16       g_usFidInitId;
extern VOS_UINT16       g_usPidInitStep;
extern VOS_UINT16       g_usPidInitId;

extern VOS_UINT32 VOS_MsgInit(VOS_VOID);

extern VOS_VOID OM_RecordMemInit(VOS_VOID);

VOS_UINT32 vos_StartUpStage = 0x00010000;
VOS_UINT32 g_ulVosStartStep = 0;

HTIMER  g_VosProtectInitTimer = VOS_NULL_PTR;

VOS_UINT32 *g_pulOsaLogTmp    = VOS_NULL_PTR;

VOS_VOID V_LogInit(VOS_VOID)
{
#if (VOS_RTOSCK == VOS_OS_VER)
    VOS_UINT32                          ulRecordAddr;

    /* 初始化定位信息 */
    ulRecordAddr = (VOS_UINT32)VOS_EXCH_MEM_MALLOC;

    if (VOS_NULL_PTR == ulRecordAddr)
    {
        return;
    }

    /* COMM在PID初始化流程会用到部分内容，使用最后的16个UINT32作为记录 */
    g_pulOsaLogTmp  = (VOS_UINT32 *)(ulRecordAddr+(VOS_DUMP_MEM_TOTAL_SIZE-16*sizeof(VOS_UINT32)));

    if ( VOS_NULL_PTR == VOS_MemSet_s((VOS_VOID *)g_pulOsaLogTmp, 16*sizeof(VOS_UINT32), 0x5A, 16*sizeof(VOS_UINT32)) )
    {
        mdrv_om_system_error(VOS_REBOOT_MEMSET_MEM, 0, (VOS_INT)((THIS_FILE_ID << 16) | __LINE__), 0, 0);
    }
#endif
    return;
}

VOS_VOID V_LogRecord(VOS_UINT32 ulIndex, VOS_UINT32 ulValue)
{
#if (VOS_RTOSCK == VOS_OS_VER)
    if (VOS_NULL_PTR == g_pulOsaLogTmp)
    {
        return;
    }

    if (ulIndex >= 16)
    {
        return;
    }

    g_pulOsaLogTmp[ulIndex] = ulValue;
#endif
    return;
}

/*****************************************************************************
 Function   : root
 Description: main function
 Input      : void
 Return     : void
 Other      :
 *****************************************************************************/
MODULE_EXPORTED VOS_VOID root( VOS_VOID)
{
    mdrv_err("<root> VOS_Startup Begin !\n");

#if (VOS_WIN32 == VOS_OS_VER)
    VOS_SplInit();
#endif

    /* 2016.03.14:底软接口修改，先调用register函数申请内存，后面使用get field函数获取内存地址 */
    (VOS_VOID)mdrv_om_register_field(DUMP_SAVE_MOD_OSA_MEM, "OAM", VOS_NULL_PTR, VOS_NULL_PTR, VOS_DUMP_MEM_ALL_SIZE, 0);

    V_LogInit();

    if ( VOS_OK != VOS_Startup( VOS_STARTUP_INIT_DOPRA_SOFEWARE_RESOURCE ) )
    {
        mdrv_err("<root> VOS_Startup Phase 0: Error.\n");
    }

    if ( VOS_OK != VOS_Startup( VOS_STARTUP_SET_TIME_INTERRUPT ) )
    {
        mdrv_err("<root> VOS_Startup Phase 1: Error.\n");
    }

    if ( VOS_OK != VOS_Startup( VOS_STARTUP_CREATE_TICK_TASK ) )
    {
        mdrv_err("<root> VOS_Startup Phase2: Error.\n");
    }

    if( VOS_OK != VOS_Startup( VOS_STARTUP_CREATE_ROOT_TASK ) )
    {
        mdrv_err("<root> VOS_Startup Phase 3: Error\n");
    }

    if ( VOS_OK != VOS_Startup( VOS_STARTUP_SUSPEND_MAIN_TASK ) )
    {
        mdrv_err("<root> VOS_Startup Phase 4: Error\n");
    }

    mdrv_err("<root> VOS_Startup End !\n");

    return;
}

/*****************************************************************************
 Function   : VOS_ProtectInit
 Description: reboot if OSA can't init
 Calls      :
 Called By  : root
 Input      : None
 Return     : None
 Other      :
 *****************************************************************************/
VOS_VOID VOS_ProtectInit(VOS_UINT32 ulParam1, VOS_UINT32 ulParam2)
{
    VOS_START_ERR_STEP_STRU stStep;

    stStep.ulStartUpStage = vos_StartUpStage;
    stStep.ulStep         = g_ulVosStartStep;
    stStep.usFidInitStep  = g_usFidInitStep;
    stStep.usFidInitId    = g_usFidInitId;
    stStep.usPidInitStep  = g_usPidInitStep;
    stStep.usPidInitId    = g_usPidInitId;


    VOS_ProtectionReboot(OSA_EXPIRE_ERROR, 0, 0, (VOS_CHAR *)&stStep, sizeof(VOS_START_ERR_STEP_STRU) );
}


VOS_UINT32 VOS_Startup( enum VOS_STARTUP_PHASE ph )
{
    VOS_UINT32      ulReturnValue;
    VOS_UINT32      ulStartUpFailStage = 0;

    switch(ph)
    {
        case VOS_STARTUP_INIT_DOPRA_SOFEWARE_RESOURCE :
            vos_StartUpStage    = 0x00010000;
            V_LogRecord(0, 0x00010000);

            if ( VOS_OK != VOS_MemInit() )
            {
                ulStartUpFailStage |= 0x0001;

                break;
            }

            VOS_SemCtrlBlkInit();

            VOS_QueueCtrlBlkInit();

            VOS_TaskCtrlBlkInit();

            if ( VOS_OK != VOS_TimerCtrlBlkInit() )
            {
                ulStartUpFailStage |= 0x0010;
            }

            OM_RecordMemInit();

            if ( VOS_OK != RTC_TimerCtrlBlkInit() )
            {
                ulStartUpFailStage |= 0x0100;
            }

            if ( VOS_OK != VOS_PidCtrlBlkInit() )
            {
                ulStartUpFailStage |= 0x0200;
            }

            if ( VOS_OK != VOS_FidCtrlBlkInit() )
            {
                ulStartUpFailStage |= 0x0400;
            }

            if ( VOS_OK != CreateFidsQueque() )
            {
                ulStartUpFailStage |= 0x0800;
            }
            break;

        case VOS_STARTUP_SET_TIME_INTERRUPT:
            vos_StartUpStage = 0x00020000;
            V_LogRecord(0, 0x00020000);
            break;

        case VOS_STARTUP_CREATE_TICK_TASK:
            vos_StartUpStage = 0x00040000;
            V_LogRecord(0, 0x00040000);

            /* create soft timer task */
            if ( VOS_OK != VOS_TimerTaskCreat() )
            {
                ulStartUpFailStage |= 0x0001;
            }

            if ( VOS_OK != RTC_TimerTaskCreat() )
            {
                ulStartUpFailStage |= 0x0002;
            }

            break;

        case VOS_STARTUP_CREATE_ROOT_TASK:
            vos_StartUpStage = 0x00080000;
            V_LogRecord(0, 0x00080000);

            g_ulVosStartStep = 0x0000;
            V_LogRecord(1, 0x0000);

            ulReturnValue = VOS_MsgInit();

            if (VOS_OK != ulReturnValue)
            {
                ulStartUpFailStage |= ulReturnValue;
            }

            g_ulVosStartStep = 0x0008;
            V_LogRecord(1, 0x0008);

            if ( VOS_OK != VOS_FidsInit() )
            {
                ulStartUpFailStage |= 0x0008;
            }

            g_ulVosStartStep = 0x0020;
            V_LogRecord(1, 0x0020);

            /* create FID task & selftask task */
            if ( VOS_OK != CreateFidsTask() )
            {
                ulStartUpFailStage |= 0x0020;
            }

            g_ulVosStartStep = 0x0100;
            V_LogRecord(1, 0x0100);

            if ( VOS_OK != VOS_PidsInit() )
            {
                ulStartUpFailStage |= 0x0100;
            }
            break;

        case VOS_STARTUP_SUSPEND_MAIN_TASK:
            vos_StartUpStage = 0x00100000;
            V_LogRecord(0, 0x00100000);

            g_ulVosStartStep = 0x0002;
            V_LogRecord(1, 0x0002);

            /* Resume FID task & selftask task */
            if ( VOS_OK != VOS_ResumeFidsTask() )
            {
                ulStartUpFailStage |= 0x0002;
            }

            g_ulVosStartStep = 0x0008;
            V_LogRecord(1, 0x0008);

            /* stop protect timer */

 #if ((OSA_CPU_CCPU == VOS_OSA_CPU) || (OSA_CPU_NRCPU == VOS_OSA_CPU))
            /* OSA初始化完成，需要调用DRV函数通知DRV OSA启动完成 */
            if ( VOS_OK != mdrv_sysboot_ok() )
            {
                ulStartUpFailStage |= 0x0008;
            }
#endif
            break;

        default:
            break;
    }

    /* calculate return value */
    if( 0 != ulStartUpFailStage )
    {
        ulReturnValue = vos_StartUpStage;
        ulReturnValue |= ulStartUpFailStage;
        mdrv_err("<VOS_Startupstartup> return value=%x.\n",ulReturnValue);

        /* reboot */
        VOS_ProtectionReboot(OSA_INIT_ERROR, (VOS_INT)ulReturnValue,
            (VOS_INT)g_ulOmPidInit, (VOS_CHAR *)&g_ulOmFidInit, sizeof(VOS_UINT32));

        return(ulReturnValue);
    }
    else
    {
        return(VOS_OK);
    }
}

#if (VOS_LINUX == VOS_OS_VER)

/*****************************************************************************
 Function   : VOS_ModuleInit
 Description:
 Input      :
 Output     :
 Return     :
 *****************************************************************************/
extern VOS_INT APP_VCOM_Init(VOS_VOID);

VOS_INT VOS_ModuleInit(VOS_VOID)
{
#if (FEATURE_OFF == FEATURE_DELAY_MODEM_INIT)
    APP_VCOM_Init();
#endif

    root();

    return 0;
}

#if (FEATURE_OFF == FEATURE_DELAY_MODEM_INIT)
/*****************************************************************************
 Function   : VOS_ModuleExit
 Description:
 Input      :
 Output     :
 Return     :
 *****************************************************************************/
static VOS_VOID VOS_ModuleExit(VOS_VOID)
{
    return;
}
#endif

/*when flash less on, VOS_ModuleInit should be called by bsp drv.*/
#if (FEATURE_OFF == FEATURE_DELAY_MODEM_INIT)
module_init(VOS_ModuleInit);

module_exit(VOS_ModuleExit);


MODULE_AUTHOR("x51137");

MODULE_DESCRIPTION("Hisilicon OSA");

MODULE_LICENSE("GPL");
#endif


#endif



