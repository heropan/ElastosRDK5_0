
#ifndef __CPLUGIN_H__
#define __CPLUGIN_H__

#include "_CPlugin.h"

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
CarClass(CPlugin)
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
        : public ElRefBase
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

    private:
        AutoPtr<IAlertDialog> mDialog;
        CPlugin* mOwner;
    };

public:
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
    CARAPI ToString(
        /* [out] */ String* name);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetFileName(
        /* [out] */ String* fileName);

    CARAPI GetDescription(
        /* [out] */ String* desc);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI SetPath(
        /* [in] */ const String& path);

    CARAPI SetFileName(
        /* [in] */ const String& fileName);

    CARAPI SetDescription(
        /* [in] */ const String& description);

    CARAPI SetClickHandler(
        /* [in] */ IPreferencesClickHandler* handler);

    CARAPI DispatchClickEvent(
        /* [in] */ IContext* context);

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

#endif // __CPLUGIN_H__
