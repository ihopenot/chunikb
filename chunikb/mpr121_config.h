#ifndef _MPR121_CONFIG_H
#define _MPR121_CONFIG_H

// Configuration values

#define MPR121_FFI_6	0
#define MPR121_FFI_10	1
#define MPR121_FFI_18	2
#define MPR121_FFI_34	3

#define MPR121_CDT_INHERIT	0
#define MPR121_CDT_500NS	1
#define MPR121_CDT_1US		2
#define MPR121_CDT_2US		3
#define MPR121_CDT_4US		4
#define MPR121_CDT_8US		5
#define MPR121_CDT_16US		6
#define MPR121_CDT_32US		7

#define MPR121_SFI_4	0
#define MPR121_SFI_6	0
#define MPR121_SFI_10	0
#define MPR121_SFI_18	0

#define MPR121_ESI_1MS		0
#define MPR121_ESI_2MS		1
#define MPR121_ESI_4MS		2
#define MPR121_ESI_8MS		3
#define MPR121_ESI_16MS		4
#define MPR121_ESI_32MS		5
#define MPR121_ESI_64MS		6
#define MPR121_ESI_128MS	7

#define MPR121_CL_NONE	0
#define MPR121_CL_LOCK	1
#define MPR121_CL_INIT1	2
#define MPR121_CL_INIT2	3

#define MPR121_ELEPROX_0	0
#define MPR121_ELEPROX_2	1
#define MPR121_ELEPROX_4	2
#define MPR121_ELEPROX_12	3

#define MPR121_AFES_6	0
#define MPR121_AFES_10	1
#define MPR121_AFES_18	2
#define MPR121_AFES_34	3

#define MPR121_RETRY_0	0
#define MPR121_RETRY_2	1
#define MPR121_RETRY_4	2
#define MPR121_RETRY_8	3

// Device register map

#define MPR121_TOUCHSTATUS_L 0x00
#define MPR121_TOUCHSTATUS_H 0x01
#define MPR121_OOR_L 0x02
#define MPR121_OOR_R 0x03
#define MPR121_FILTDATA(ch) (0x04 + 2*(ch))
#define MPR121_BASELINE(ch) (0x1E + (ch))
#define MPR121_MHDR 0x2B
#define MPR121_NHDR 0x2C
#define MPR121_NCLR 0x2D
#define MPR121_FDLR 0x2E
#define MPR121_MHDF 0x2F
#define MPR121_NHDF 0x30
#define MPR121_NCLF 0x31
#define MPR121_FDLF 0x32
#define MPR121_NHDT 0x33
#define MPR121_NCLT 0x34
#define MPR121_FDLT 0x35
#define MPR121_MHDPROXR 0x36
#define MPR121_HHDPROXR 0x37
#define MPR121_NCLPROXR 0x38
#define MPR121_FDLPROXR 0x39
#define MPR121_MHDPROXF 0x3A
#define MPR121_NHDPROXF 0x3B
#define MPR121_NCLPROXF 0x3C
#define MPR121_FDLPROXF 0x3D
#define MPR121_NHDPROXT 0x3E
#define MPR121_NCLPROXT 0x3F
#define MPR121_FDLPROXT 0x40
#define MPR121_TOUCHTH(ch) (0x41 + 2*(ch))
#define MPR121_RELEASETH(ch) (0x42 + 2*(ch))
#define MPR121_DEBOUNCE 0x5B
#define MPR121_AFE1 0x5C
#define MPR121_AFE2 0x5D
#define MPR121_ECR 0x5E
#define MPR121_CDC(ch) (0x5F + (ch))
#define MPR121_CDT(ch) (0x6C + ((ch)>>1))
#define MPR121_CTL0 0x73
#define MPR121_CTL1 0x74
#define MPR121_DAT 0x75
#define MPR121_DIR 0x76
#define MPR121_EN 0x77
#define MPR121_SET 0x78
#define MPR121_CLR 0x79
#define MPR121_TOG 0x7A
#define MPR121_AUTOCONF0 0x7B
#define MPR121_AUTOCONF1 0x7C
#define MPR121_USL 0x7D
#define MPR121_LSL 0x7E
#define MPR121_TL 0x7F
#define MPR121_SRST 0x80

#define MPR121_I2CADDR_DEFAULT 0x5A

// Thresholds
#define MPR121CONF_TTH  6///< 触发阈值
#define MPR121CONF_RTH  6///< 释放阈值
// De-bounce
#define MPR121CONF_DR 3 ///< 松开样本采集数
#define MPR121CONF_DT 2 ///< 按下样本采集数
// AFE Configurations
#define MPR121CONF_FFI  MPR121_FFI_6 ///< 一层过滤采样数
#define MPR121CONF_CDC  16 ///< 充放电电流
#define MPR121CONF_CDT  MPR121_CDT_500NS ///< 充放电时间
#define MPR121CONF_SFI  MPR121_SFI_4 ///< 二层过滤
#define MPR121CONF_ESI  MPR121_ESI_1MS ///< 二层过滤采样间隔时间
// ECR Configurations
#define MPR121CONF_CL   MPR121_CL_INIT2 ///< 启用基线动态校准
#define MPR121CONF_ELEPROX  MPR121_ELEPROX_0  ///< 接近检测电极，禁用
#define MPR121CONF_ELE    12 ///< touch detection electrodes
// 基线动态校准
#define MPR121CONF_MHDR 1 ///< 上升最大变化值
#define MPR121CONF_NHDR 1 ///< 上升幅度
#define MPR121CONF_NCLR 0xe ///< 上升修正样本个数
#define MPR121CONF_FDLR 0 ///< 修正前等待样本个数
#define MPR121CONF_MHDF 1 ///< 下降最大变化值
#define MPR121CONF_NHDF 5 ///< 下降幅度
#define MPR121CONF_NCLF 0xe  ///< 下降修正样本个数
#define MPR121CONF_FDLF 0 ///< 修正前等待样本个数
#define MPR121CONF_NHDT 0 ///< Touched Noise Half Delta
#define MPR121CONF_NCLT 0 ///< Touched Noise Count Limit
#define MPR121CONF_FDLT 0 ///< Touched Filter Delay Count Limit
// 自动初始化
#define MPR121CONF_USL    200 ///< 上限，MPR121_UPLIMIT，((Vdd - 0.7)/Vdd) * 256，200
#define MPR121CONF_LSL    130 ///< 下限，MPR121_LOWLIMIT，UPLIMIT * 0.65，130
#define MPR121CONF_TL   180 ///< 目标，MPR121_TARGETLIMIT，UPLIMIT * 0.9，180

// #define MPR121CONF_USL    156 ///< 上限，MPR121_UPLIMIT，((Vdd - 0.7)/Vdd) * 256
// #define MPR121CONF_LSL    101 ///< 下限，MPR121_LOWLIMIT，UPLIMIT * 0.65
// #define MPR121CONF_TL   140 ///< 目标，MPR121_TARGETLIMIT，UPLIMIT * 0.9

// #define MPR121CONF_USL    166 ///< 上限，MPR121_UPLIMIT，((Vdd - 0.7)/Vdd) * 256，200
// #define MPR121CONF_LSL    108 ///< 下限，MPR121_LOWLIMIT，UPLIMIT * 0.65，130
// #define MPR121CONF_TL   149 ///< 目标，MPR121_TARGETLIMIT，UPLIMIT * 0.9，180

#define MPR121CONF_AFES   MPR121CONF_FFI  ///< 必须设置为FFI
#define MPR121CONF_RETRY  MPR121_RETRY_0  ///< 重试次数
#define MPR121CONF_BVA    MPR121CONF_CL ///< 必须设置为CL
#define MPR121CONF_ARE    1 ///< 开启自动重配置
#define MPR121CONF_ACE    1 ///< 开启自动配置
#define MPR121CONF_SCTS   1 ///< 跳过充电时间
//中断用，全部关闭
#define MPR121CONF_ACFIE  0 ///< Auto-configuration fail interrupt enable
#define MPR121CONF_ARFIE  0 ///< Auto-reconfiguration fail interrupt enable
#define MPR121CONF_OORIE  0 ///< Out-of-range interrupt enable

#endif
