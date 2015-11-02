#include "CLegacyFlashlightHack.h"
#include "Manifest.h"

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::EIID_IHardwareService;
using Elastos::Droid::Content::Pm::IPackageManager;

namespace Elastos {
namespace Droid {
namespace Server {

const String CLegacyFlashlightHack::FLASHLIGHT_FILE("/sys/class/leds/spotlight/brightness");

ECode CLegacyFlashlightHack::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode CLegacyFlashlightHack::GetFlashlightEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);
    //try {
    AutoPtr<IFileInputStream> fis;
    CFileInputStream::New(FLASHLIGHT_FILE, (IFileInputStream**)&fis);

    Int32 result;
    if (FAILED(fis->Read(&result))) {
        *isEnabled = FALSE;
        return NOERROR;
    }
    fis->Close();
    *isEnabled = (result != '0');
    return NOERROR;
    //} catch (Exception e) {
    //    return false;
    //}
}

ECode CLegacyFlashlightHack::SetFlashlightEnabled(
    /* [in] */ Boolean on)
{
    String permission1 = Elastos::Droid::Manifest::permission::FLASHLIGHT;
    Int32 result1;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(permission1, &result1));
    String permission2 = Elastos::Droid::Manifest::permission::HARDWARE_TEST;
    Int32 result2;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(permission2, &result2));
    if (result1 != IPackageManager::PERMISSION_GRANTED &&
            result2 != IPackageManager::PERMISSION_GRANTED) {
        //throw new SecurityException("Requires FLASHLIGHT or HARDWARE_TEST permission");
        return E_SECURITY_EXCEPTION;
    }
    //try {
    AutoPtr<IFileOutputStream> fos;
    CFileOutputStream::New(FLASHLIGHT_FILE, (IFileOutputStream**)&fos);

    AutoPtr<ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(2);
    (*bytes)[0] = (Byte)(on ? '1' : '0');
    (*bytes)[1] = '\n';
    fos->WriteBytes(*bytes);
    fos->Close();
    //} catch (Exception e) {
        // fail silently
    //}
    return NOERROR;
}

ECode CLegacyFlashlightHack::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

}
}
}