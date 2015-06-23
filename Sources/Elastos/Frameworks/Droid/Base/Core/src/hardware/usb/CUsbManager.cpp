
#include "hardware/usb/CUsbManager.h"
#include "hardware/usb/CUsbDeviceConnection.h"
#include <elastos/core/Character.h>
#include "os/CSystemProperties.h"

using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

const String CUsbManager::TAG("UsbManager");

ECode CUsbManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIUsbManager* service)
{
    mContext = context;
    mService = service;

    return NOERROR;
}

ECode CUsbManager::GetDeviceList(
    /* [out] */ IObjectStringMap** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<IBundle> bundle;
    FAIL_RETURN(mService->GetDeviceList((IBundle**)&bundle));

    AutoPtr<IObjectContainer> keySet;
    FAIL_RETURN(bundle->KeySet((IObjectContainer**)&keySet));

    if (keySet == NULL) {
        *list = NULL;
        return NOERROR;
    }

    AutoPtr<IObjectEnumerator> iter;
    FAIL_RETURN(keySet->GetObjectEnumerator((IObjectEnumerator**)&iter));
    Boolean hasNext;

    CObjectStringMap::New(list);
    while (iter->MoveNext(&hasNext), hasNext) {
        AutoPtr<ICharSequence> chars;
        FAIL_RETURN(iter->Current((IInterface**)&chars));

        String name;
        chars->ToString(&name);

        AutoPtr<IUsbDevice> dev;
        FAIL_RETURN(bundle->Get(name, (IInterface**)&dev));
        (*list)->Put(name, dev);
    }

    return NOERROR;
}

ECode CUsbManager::OpenDevice(
    /* [in] */ IUsbDevice* device,
    /* [out] */ IUsbDeviceConnection** connection)
{
    VALIDATE_NOT_NULL(connection);

    String deviceName;
    FAIL_RETURN(device->GetDeviceName(&deviceName));

    AutoPtr<IParcelFileDescriptor> pfd;
    FAIL_RETURN(mService->OpenDevice(deviceName, (IParcelFileDescriptor**)&pfd));

    if (pfd == NULL) {
        *connection = NULL;
        return NOERROR;
    }

    AutoPtr<IUsbDeviceConnection> tempConn;
    CUsbDeviceConnection::New(device, (IUsbDeviceConnection**)&tempConn);

    Boolean result;
    FAIL_RETURN(tempConn->Open(deviceName, pfd, &result));

    if (result == FALSE) {
        *connection = NULL;
        return NOERROR;
    }

    *connection = tempConn;
    REFCOUNT_ADD(*connection);
    return NOERROR;
}

ECode CUsbManager::GetAccessoryList(
    /* [out, callee] */ ArrayOf<IUsbAccessory*>** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<IUsbAccessory> accessory;
    FAIL_RETURN(mService->GetCurrentAccessory((IUsbAccessory**)&accessory));

    if (accessory == NULL) {
        *list = NULL;
        return NOERROR;
    }

    AutoPtr<ArrayOf<IUsbAccessory*> > tempList = ArrayOf<IUsbAccessory*>::Alloc(1);

    tempList->Set(0, accessory);
    *list = tempList;

    REFCOUNT_ADD(*list);
    return NOERROR;
}

ECode CUsbManager::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    FAIL_RETURN(mService->OpenAccessory(accessory, descriptor));
    return NOERROR;
}

ECode CUsbManager::HasDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(mService->HasDevicePermission(device, result));
    return NOERROR;
}

ECode CUsbManager::HasAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(mService->HasAccessoryPermission(accessory, result));
    return NOERROR;
}

ECode CUsbManager::RequestDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ IPendingIntent* pi)
{
    String name;
    FAIL_RETURN(mContext->GetPackageName(&name));
    FAIL_RETURN(mService->RequestDevicePermission(device, name, pi));
    return NOERROR;
}

ECode CUsbManager::RequestAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ IPendingIntent* pi)
{
    String name;
    FAIL_RETURN(mContext->GetPackageName(&name));
    FAIL_RETURN(mService->RequestAccessoryPermission(accessory, name, pi));
    return NOERROR;
}

ECode CUsbManager::IsFunctionEnabled(
    /* [in] */ const String& function,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    String property("sys.usb.config");
    *result = PropertyContainsFunction(property, function);
    return NOERROR;
}

ECode CUsbManager::GetDefaultFunction(
    /* [out] */ String* function)
{
    VALIDATE_NOT_NULL(function);
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    String str;
    sp->Get(String("persist.sys.usb.config"), String(""), &str);

    int commaIndex = str.IndexOf(',');

    if (commaIndex > 0) {
        *function = str.Substring(0, commaIndex);
        return NOERROR;
    }

    *function = str;
    return NOERROR;
}

ECode CUsbManager::SetCurrentFunction(
    /* [in] */ const String& function,
    /* [in] */ Boolean makeDefault)
{
    FAIL_RETURN(mService->SetCurrentFunction(function, makeDefault));
    return NOERROR;
}

ECode CUsbManager::SetMassStorageBackingFile(
    /* [in] */ const String& path)
{
    FAIL_RETURN(mService->SetMassStorageBackingFile(path));
    return NOERROR;
}

Boolean CUsbManager::PropertyContainsFunction(
    /* [in] */ const String& property,
    /* [in] */ const String& function)
{
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    String functions;
    sp->Get(property, String(""), &functions);
    Int32 index = functions.IndexOf(function);

    if (index < 0) {
        return FALSE;
    }

    Char32 c1 = functions.GetChar(index - 1);
    if (index > 0 && c1 != ',') {
        return FALSE;
    }

    Int32 charAfter = index + function.GetLength();
    Char32 c2 = functions.GetChar(charAfter);
    if (charAfter < (Int32)functions.GetLength() && c2 != ',') {
        return FALSE;
    }

    return TRUE;
}

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
