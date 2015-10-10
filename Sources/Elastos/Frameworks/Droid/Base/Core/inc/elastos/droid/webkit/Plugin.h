
#ifndef __ELASTOS_DROID_WEBKIT_PLUGIN_H__
#define __ELASTOS_DROID_WEBKIT_PLUGIN_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Represents a plugin (Java equivalent of the PluginPackageAndroid
 * C++ class in libs/WebKitLib/WebKit/WebCore/plugins/android/)
 *
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
class Plugin : public ElRefBase
{
public:
    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI_(String) GetName();

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI_(String) GetPath();

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI_(String) GetFileName();

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI_(String) GetDescription();

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI SetName(
        /* [in] */ const String& name);

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI SetPath(
        /* [in] */ const String& path);

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI SetFileName(
        /* [in] */ const String& fileName);

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI SetDescription(
        /* [in] */ const String& description);

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    virtual CARAPI SetClickHandler(
        /* [in] */ IPreferencesClickHandler* handler);

   /**
    * Invokes the click handler for this plugin.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    virtual CARAPI DispatchClickEvent(
        /* [in] */ IContext* context);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_PLUGIN_H__
