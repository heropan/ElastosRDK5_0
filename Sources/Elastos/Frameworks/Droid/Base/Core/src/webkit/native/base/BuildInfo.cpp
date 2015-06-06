
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

const String BuildInfo::TAG("BuildInfo");
const Int32 BuildInfo::MAX_FINGERPRINT_LENGTH;

/**
 * BuildInfo is a static utility class and therefore shouldn't be
 * instantiated.
 */
BuildInfo::BuildInfo()
{
}

//@CalledByNative
String BuildInfo::GetDevice()
{
    return IBuild::DEVICE;
}

//@CalledByNative
String BuildInfo::GetBrand()
{
    return IBuild::BRAND;
}

//@CalledByNative
String BuildInfo::GetAndroidBuildId()
{
    return IBuild::ID;
}

/**
 * @return The build fingerprint for the current Android install.  The value is truncated to a
 *         128 characters as this is used for crash and UMA reporting, which should avoid huge
 *         strings.
 */
//@CalledByNative
String BuildInfo::GetAndroidBuildFingerprint()
{
    String str(IBuild::FINGERPRINT);
    Int32 length = str.GetLength();
    Int32 minLen = Math::Min(length, MAX_FINGERPRINT_LENGTH);
    return str.Substring(0, minLen, MAX_FINGERPRINT_LENGTH);
}

//@CalledByNative
String BuildInfo::GetDeviceModel()
{
    return Build::MODEL;
}

//@CalledByNative
String BuildInfo::GetPackageVersionCode(
    /* [in] */ IContext* context)
{
    String msg("versionCode not available.");
    //try {
        AutoPtr<IPackageManager> pm;
        context->GetPackageManager((IPackageManager**)&pm);
        String pkn;
        context->GetPackageName(&pkn);
        AutoPtr<IPackageInfo> pi;
        pm->GetPackageInfo(pkn, 0, (IPackageInfo**)&pi);
        msg = "";
        Int32 versionCode;
        pi->GetVersionCode(&versionCode);
        if (versionCode > 0) {
            msg = Integer.toString(pi.versionCode);
        }
    //} catch (NameNotFoundException e) {
    //    Log.d(TAG, msg);
    //}
    return msg;

}

//@CalledByNative
String BuildInfo::GetPackageVersionName(
    /* [in] */ IContext* context)
{
    String msg("versionName not available");
    //try {
        AutoPtr<IPackageManager> pm;
        context->GetPackageManager((IPackageManager**)&pm);
        String pkn;
        context->GetPackageName(&pkn);
        AutoPtr<IPackageInfo> pi;
        pm->GetPackageInfo(pkn, 0, (IPackageInfo**)&pi);
        pi->GetVersionName(&msg);
    //} catch (NameNotFoundException e) {
    //    Log.d(TAG, msg);
    //}
    return msg;
}

//@CalledByNative
String BuildInfo::GetPackageLabel(
    /* [in] */ IContext* context)
{
    String strRet("");
    //try {
        AutoPtr<IPackageManager> packageManager;
        context->GetPackageManager((IPackageManager**)&packageManager);
        String pkn;
        context->GetPackageName(&pkn);
        AutoPtr<IApplicationInfo> appInfo;
        packageManager->GetApplicationInfo(pkn, IPackageManager::GET_META_DATA, (IApplicationInfo**)&appInfo);
        AutoPtr<ICharSequence> label;
        packageManager->GetApplicationLabel(appInfo, (ICharSequence**)&label);
        if (label != NULL) {
            label->ToString(&strRet);
        }
        return  strRet;
    //} catch (NameNotFoundException e) {
    //    return "";
    //}
}

//@CalledByNative
String BuildInfo::GetPackageName(
    /* [in] */ IContext* context)
{
    String packageName("");
    if (context != NULL) {
        context->GetPackageName(&packageName);
    }

    return packageName;
}

//@CalledByNative
String BuildInfo::GetBuildType()
{
    return IBuild::TYPE;
}

//@CalledByNative
Int32 BuildInfo::GetSdkInt()
{
    return Build.VERSION.SDK_INT;
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
