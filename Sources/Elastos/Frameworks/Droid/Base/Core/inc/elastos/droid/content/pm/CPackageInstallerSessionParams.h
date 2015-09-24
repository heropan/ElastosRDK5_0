
#ifndef __ELASTOS_DROID_CONTENT_PM_CPACKAGE_INSTALLER_SESSION_PARAMS_H__
#define __ELASTOS_DROID_CONTENT_PM_CPACKAGE_INSTALLER_SESSION_PARAMS_H__

#include "_Elastos_Droid_Content_Pm_CPackageInstallerSessionParams.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Net::IUri;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CPackageInstallerSessionParams)
    , public Object
    , public IPackageInstallerSessionParams
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CPackageInstallerSessionParams();

    virtual ~CPackageInstallerSessionParams();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 mode);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Provide value of {@link PackageInfo#installLocation}, which may be used
     * to determine where the app will be staged. Defaults to
     * {@link PackageInfo#INSTALL_LOCATION_INTERNAL_ONLY}.
     */
    CARAPI SetInstallLocation(
        /* [in] */ Int32 installLocation);

    /**
     * Optionally indicate the total size (in bytes) of all APKs that will be
     * delivered in this session. The system may use this to ensure enough disk
     * space exists before proceeding, or to estimate container size for
     * installations living on external storage.
     *
     * @see PackageInfo#INSTALL_LOCATION_AUTO
     * @see PackageInfo#INSTALL_LOCATION_PREFER_EXTERNAL
     */
    CARAPI SetSize(
        /* [in] */ Int64 installLocation);

    /**
     * Optionally set the package name of the app being installed. It's strongly
     * recommended that you provide this value when known, so that observers can
     * communicate installing apps to users.
     * <p>
     * If the APKs staged in the session aren't consistent with this package
     * name, the install will fail. Regardless of this value, all APKs in the
     * app must have the same package name.
     */
    CARAPI SetAppPackageName(
        /* [in] */ const String& appPackageName);

    /**
     * Optionally set an icon representing the app being installed. This should
     * be roughly {@link ActivityManager#getLauncherLargeIconSize()} in both
     * dimensions.
     */
    CARAPI SetAppIcon(
        /* [in] */ IBitmap* appIcon);

    /**
     * Optionally set a label representing the app being installed.
     */
    CARAPI SetAppLabel(
        /* [in] */ ICharSequence* appLabel);

    /**
     * Optionally set the URI where this package was downloaded from. Used for
     * verification purposes.
     *
     * @see Intent#EXTRA_ORIGINATING_URI
     */
    CARAPI SetOriginatingUri(
        /* [in] */ IUri* originatingUri);

    /**
     * Optionally set the URI that referred you to install this package. Used
     * for verification purposes.
     *
     * @see Intent#EXTRA_REFERRER
     */
    CARAPI SetReferrerUri(
        /* [in] */ IUri* referrerUri);

    /** {@hide} */
    CARAPI SetInstallFlagsInternal();

    /** {@hide} */
    CARAPI SetInstallFlagsExternal();

public:
    /** {@hide} */
    Int32 mMode;// = MODE_INVALID;
    /** {@hide} */
    Int32 mInstallFlags;
    /** {@hide} */
    Int32 mInstallLocation;// = PackageInfo.INSTALL_LOCATION_INTERNAL_ONLY;
    /** {@hide} */
    Int64 mSizeBytes;// = -1;
    /** {@hide} */
    String mAppPackageName;
    /** {@hide} */
    AutoPtr<IBitmap> mAppIcon;
    /** {@hide} */
    String mAppLabel;
    /** {@hide} */
    Int64 mAppIconLastModified;// = -1;
    /** {@hide} */
    AutoPtr<IUri> mOriginatingUri;
    /** {@hide} */
    AutoPtr<IUri> mReferrerUri;
    /** {@hide} */
    String mAbiOverride;

};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CPACKAGE_INSTALLER_SESSION_PARAMS_H__
