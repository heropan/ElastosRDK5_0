
#ifndef __ELASTOS_DROID_WEBKIT_PLUGIN_H__
#define __ELASTOS_DROID_WEBKIT_PLUGIN_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;

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
class Plugin
    : public Object
    , public IPlugin
{
private:
   /**
    * Default click handler. The plugins should implement their own.
    *
    * @hide
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    class DefaultClickHandler
        : public Object
        , public IPreferencesClickHandler
        , public IDialogInterfaceOnClickListener
    {
    public:
        DefaultClickHandler(
            /* [in] */ CPlugin* owner)
            : mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        CARAPI HandleClickEvent(
            /* [in] */ IContext* context);

        /**
         * @hide
         * @deprecated This interface was intended to be used by Gears. Since Gears was
         * deprecated, so is this class.
         */
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        AutoPtr<IAlertDialog> mDialog;
        CPlugin* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    Plugin(
        /* [in] */ const String& name,
        /* [in] */ const String& path,
        /* [in] */ const String& fileName,
        /* [in] */ const String& description);

    /**
     * @hide
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& path,
        /* [in] */ const String& fileName,
        /* [in] */ const String& description);

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

    CARAPI ToString(
        /* [out] */ String* info);

protected:
    Plugin();

private:
    String mName;
    String mPath;
    String mFileName;
    String mDescription;
    AutoPtr<IPreferencesClickHandler> mHandler;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_PLUGIN_H__
