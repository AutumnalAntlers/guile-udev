#include <libguile.h>
#include <libudev.h>

#include "udev-device-type.h"

/**
 * Get a string device property from an SCM device object.
 */
static SCM _get_string_property(SCM device,
                                const char* (*func)(struct udev_device *))
#define FUNC_NAME "_get_string_property"
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     const char* result = func(udd->udev_device);
     return scm_from_locale_string(result);
}
#undef FUNC_NAME


SCM_DEFINE(gudev_device_get_action,
           "udev-device-get-action", 1, 0, 0,
           (SCM device),
           "Get action.")
#define FUNC_NAME s_gudev_device_get_action
{
    return _get_string_property(device, udev_device_get_action);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_sysname,
           "udev-device-get-sysname", 1, 0, 0,
           (SCM device),
           "Get sysname.")
#define FUNC_NAME s_gudev_device_get_sysname
{
    return _get_string_property(device, udev_device_get_sysname);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devpath,
           "udev-device-get-devpath", 1, 0, 0,
           (SCM device),
           "Get devpath.")
#define FUNC_NAME s_gudev_device_get_devpath
{
    return _get_string_property(device, udev_device_get_devpath);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devnode,
           "udev-device-get-devnode", 1, 0, 0,
           (SCM device),
           "Get devnode.")
#define FUNC_NAME s_gudev_device_get_devnode
{
    return _get_string_property(device, udev_device_get_devnode);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_devtype,
           "udev-device-get-devtype", 1, 0, 0,
           (SCM device),
           "Get the device type.")
#define FUNC_NAME s_gudev_device_get_devtype
{
    return _get_string_property(device, udev_device_get_devtype);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_driver,
           "udev-device-get-driver", 1, 0, 0,
           (SCM device),
           "Get the device driver.")
#define FUNC_NAME s_gudev_device_get_driver
{
     return _get_string_property(device, udev_device_get_driver);
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_udev,
           "udev-device-get-udev", 1, 0, 0,
           (SCM device),
           "Get the parent udev object.")
#define FUNC_NAME s_gudev_device_get_udev
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     return udd->udev;
}
#undef FUNC_NAME

SCM_DEFINE(gudev_device_get_property_value,
           "udev-device-get-property-value", 2, 0, 0,
           (SCM device, SCM property),
           "Get the value of given PROPERTY for the DEVICE.")
#define FUNC_NAME s_gudev_device_get_property
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     char* c_property = NULL;

     scm_dynwind_begin(0);
     c_property = scm_to_locale_string(property);
     scm_dynwind_free(c_property);

     const char* value = udev_device_get_property_value(udd->udev_device,
                                                        c_property);

     scm_dynwind_end();

     return scm_from_locale_string(value);
}
#undef FUNC_NAME

/**
 * Convert an Udev list to a Scheme alist.
 */
SCM _scm_alist_from_udev_list(struct udev_list_entry* entry)
{
     SCM alist = scm_make_list(scm_from_int(0), SCM_UNDEFINED);
     for (; entry != NULL; entry = udev_list_entry_get_next(entry)) {
          const char* name  = udev_list_entry_get_name(entry);
          const char* value = udev_list_entry_get_value(entry);

          alist = scm_acons(scm_from_locale_string(name),
                            scm_from_locale_string(value),
                            alist);
     }
     return alist;
}

SCM_DEFINE(gudev_device_get_links,
           "udev-device-get-links", 1, 0, 0,
           (SCM device),
           "Get device links.")
#define FUNC_NAME s_gudev_device_get_links
{
     struct udev_device_data* udd = _scm_to_udev_device_data(device);
     struct udev_list_entry* entry
          = udev_device_get_devlinks_list_entry(udd->udev_device);
     return _scm_alist_from_udev_list(entry);
}
#undef FUNC_NAME


void init_udev_device_func()
{
#include "udev-device-func.x"
}
