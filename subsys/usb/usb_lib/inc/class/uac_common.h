/**
 * @copyright Copyright (C) 2023 Realtek Semiconductor Corporation.
 *
 * @file uac_common.h
 * @version 1.0
 * @brief common definitions for both uac1.0 & uac2.0
 *
 * @note:
 */
#ifndef __UAC_COMMON_H__
#define __UAC_COMMON_H__

#define UAC_SUBCLASS_UNDEFINED          0x00
#define UAC_SUBCLASS_AUDIOCONTROL       0x01
#define UAC_SUBCLASS_AUDIOSTREAMING     0x02
#define UAC_SUBCLASS_MIDISTREAMING      0x03

#define UAC_DT_CS_UNDEFINE                0x20
#define UAC_DT_CS_DEVICE                  0x21
#define UAC_DT_CS_CONFIGURATION           0x22
#define UAC_DT_CS_STRING                  0x23
#define UAC_DT_CS_INTERFACE               0x24
#define UAC_DT_CS_ENDPOINT                0x25

#endif
