#ifndef __USB_DEV_DRIVER_H__
#define __USB_DEV_DRIVER_H__
#include <stdint.h>
#include <stdbool.h>
#include "usb_spec20.h"

/** \addtogroup USB_DEV_Driver
  * \brief USB device driver.
  * @{
  */

/**
 * \defgroup USB_Dev_Driver_Usage_Chapter_Vendor How to Use Vendor Callback
 * @{
 *
 * \details The vendor callback will notify the information to the upper application.
 *
 * \par Example
 * \code
 *      #include "usb_dev_driver.h"
 *
 *      int demo_get_vendor(uint16_t request_cmd, void *buf, uint16_t len)
 *      {
 *          ...
 *      }
 *
 *      int demo_set_vendor(uint16_t request_cmd, void *buf, uint16_t len)
 *      {
 *          ...
 *      }
 *
 *      T_USB_DEV_VENDOR_CBS demo_cbs =
 *      {
 *          .get = demo_get_vendor,
 *          .set = demo_set_vendor,
 *      };
 *      usb_dev_driver_vendor_cbs_register(&demo_cbs);

 *\endcode
 * @}
 */

/** @defgroup USB_DEV_Driver_Exported_Types USB Device Driver Exported Types
  * @{
  */

/**
 * usb_dev_driver.h
 *
 * \brief   USB device-related strings.
 *
 * \param id The string ID defined in the device descriptor.
 * \param s  The actual string that matches with \ref id.
 *
 */
typedef struct _string
{
    uint8_t          id;
    char             *s;
} T_STRING;

/**
 * usb_dev_driver.h
 *
 * \brief   USB device-related string table.
 *
 * \param language The language id used by strings.
 * \param strings  The strings refer to \ref T_STRING.
 *
 */
typedef struct _string_tab
{
    uint16_t language;
    T_STRING *strings;
} T_STRING_TAB;

/** End of group USB_DEV_Driver_Exported_Types
  * @}
  */

/** @defgroup USB_DEV_Driver_Exported_Functions USB Device Driver Exported Functions
  * @{
  */

/**
 *
 * \brief   USB device speed callback.
 *
 */
typedef bool (*USB_SPD_CB)(uint8_t);

/**
 * usb_dev_driver.h
 *
 * \brief   register USB speed callback.
 *
 * \param USB_SPD_CB speed callbac \ref USB_SPD_CB.
 *
 */
void usb_spd_cb_register(USB_SPD_CB cb);

/**
 * usb_dev_driver.h
 *
 * \brief  Register the USB device descriptor.
 *
 * \param desc The device descriptor.
 *
 */
void usb_dev_driver_dev_desc_register(T_USB_DEVICE_DESC *desc);
/**
 * usb_dev_driver.h
 *
 * \brief  Register the USB string descriptor.
 *
 * \param string_tbl The string descriptor table.
 *
 */
void usb_dev_driver_string_desc_register(T_STRING_TAB *string_tbl[]);
/**
 * usb_dev_driver.h
 *
 * \brief  Register the USB configuration descriptor.
 *
 * \param desc The configuration descriptor.
 *
 */
void usb_dev_driver_cfg_desc_register(T_USB_CONFIG_DESC *desc);
/**
 * usb_dev_driver.h
 *
 * \brief  Unregister the USB device descriptor.
 *
 */
void usb_dev_driver_dev_desc_unregister(void);
/**
 * usb_dev_driver.h
 *
 * \brief  Unregister the USB string descriptor.
 * \param string_tbl The string descriptor table.
 *
 */
void usb_dev_driver_string_desc_unregister(T_STRING_TAB *string_tbl[]);
/**
 * usb_dev_driver.h
 *
 * \brief  Unregister the USB configuration descriptor.
 * \param desc The configuration descriptor.
 *
 */
void usb_dev_driver_cfg_desc_unregister(T_USB_CONFIG_DESC *desc);

/** @} */ /* End of group USB_DEV_Driver_Exported_Functions */
/** @}*/
/** End of USB_DEV_Driver
*/
#endif
