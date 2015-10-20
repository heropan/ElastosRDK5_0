
#include "CWifiP2pDeviceList.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {

ECode CWifiP2pDeviceList::constructor()
{
    return NOERROR;
}

ECode CWifiP2pDeviceList::constructor(
    /* [in] */ IWifiP2pDeviceList* source)
{
    if (source != NULL) {
        AutoPtr<ArrayOf<IWifiP2pDevice*> > array;
        FAIL_RETURN(source->GetDeviceList((ArrayOf<IWifiP2pDevice*>**)&array));

        if (array != NULL) {
            String temp;
            for (Int32 i = 0; i < array->GetLength(); ++i) {
                FAIL_RETURN((*array)[i]->GetDeviceAddress(&temp));
                mDevices[temp] = (*array)[i];
            }
        }
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::constructor(
    /* [in] */ ArrayOf<IWifiP2pDevice*>* devices)
{
    if (devices != NULL) {
        String temp;
        for (Int32 i = 0; i < devices->GetLength(); ++i) {
            FAIL_RETURN((*devices)[i]->GetDeviceAddress(&temp));
            if (!temp.IsNull()) {
                mDevices[temp] = (*devices)[i];
            }
        }
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::Clear(
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = FALSE;

    if (mDevices.IsEmpty()) {
        return NOERROR;
    }

    mDevices.Clear();
    *ret = TRUE;
    return NOERROR;
}

ECode CWifiP2pDeviceList::Update(
    /* [in] */ IWifiP2pDevice* device)
{
    if (device == NULL) return NOERROR;

Slogger::D("CWifiP2pDeviceList", "========================Update Start==========================");
    String address, temp;
    FAIL_RETURN(device->GetDeviceAddress(&address));
    if (address.IsNull()) return NOERROR;

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it = mDevices.Find(address);
    if (it != mDevices.End()) {
        AutoPtr<IWifiP2pDevice> d = it->mSecond;

        FAIL_RETURN(device->GetDeviceName(&temp));
        FAIL_RETURN(d->SetDeviceName(temp));

        FAIL_RETURN(device->GetPrimaryDeviceType(&temp));
        FAIL_RETURN(d->SetPrimaryDeviceType(temp));

        Int32 nTemp;
        FAIL_RETURN(device->GetWpsConfigMethodsSupported(&nTemp));
        FAIL_RETURN(d->SetWpsConfigMethodsSupported(nTemp));

        FAIL_RETURN(device->GetDeviceCapability(&nTemp));
        FAIL_RETURN(d->SetDeviceCapability(nTemp));

        FAIL_RETURN(device->GetGroupCapability(&nTemp));
        FAIL_RETURN(d->SetGroupCapability(nTemp));

        AutoPtr<IWifiP2pWfdInfo> wfdInfo;
        FAIL_RETURN(device->GetWfdInfo((IWifiP2pWfdInfo**)&wfdInfo));
        FAIL_RETURN(d->SetWfdInfo(wfdInfo));

        return NOERROR;
    }

    Slogger::D("CWifiP2pDeviceList", "========================Update Done==========================");
    //Not found, add a new one
    mDevices[address] = device;

    return NOERROR;
}

ECode CWifiP2pDeviceList::UpdateGroupCapability(
    /* [in] */ const String& deviceAddress,
    /* [in] */ Int32 groupCapab)
{
    if (deviceAddress.IsNullOrEmpty()) return NOERROR;

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it = mDevices.Find(deviceAddress);
    if (it != mDevices.End()) {
        AutoPtr<IWifiP2pDevice> d = it->mSecond;
        FAIL_RETURN(d->SetGroupCapability(groupCapab));
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::UpdateStatus(
    /* [in] */ const String& deviceAddress,
    /* [in] */ Int32 status)
{
    if (deviceAddress.IsNullOrEmpty()) return NOERROR;

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it = mDevices.Find(deviceAddress);
    if (it != mDevices.End()) {
        AutoPtr<IWifiP2pDevice> d = it->mSecond;
        FAIL_RETURN(d->SetStatus(status));
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::Get(
    /* [in] */ const String& deviceAddress,
    /* [out] */ IWifiP2pDevice** device)
{
    VALIDATE_NOT_NULL(device);
    *device = NULL;

    if (deviceAddress.IsNull()) {
        return NOERROR;
    }

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it = mDevices.Find(deviceAddress);
    if (it != mDevices.End()) {
        *device = it->mSecond;
        REFCOUNT_ADD(*device);
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::Remove(
    /* [in] */ IWifiP2pDevice* device,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = FALSE;

    if (device == NULL) {
        return NOERROR;
    }

    String temp;
    FAIL_RETURN(device->GetDeviceAddress(&temp));
    if (temp.IsNull()) {
        return NOERROR;
    }

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it = mDevices.Find(temp);
    if (it != mDevices.End()) {
        mDevices.Erase(it);
        *ret = TRUE;
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::Remove(
    /* [in] */ IWifiP2pDeviceList* list,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = FALSE;

    AutoPtr<ArrayOf<IWifiP2pDevice*> > array;
    FAIL_RETURN(list->GetDeviceList((ArrayOf<IWifiP2pDevice*>**)&array));

    if (array != NULL) {
        Boolean temp;
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            Remove((*array)[i], &temp);
            if (temp) *ret = TRUE;
        }
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::GetDeviceList(
    /* [out, callee] */ ArrayOf<IWifiP2pDevice*>** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<ArrayOf<IWifiP2pDevice*> > array = ArrayOf<IWifiP2pDevice*>::Alloc(
        mDevices.GetSize());

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it;
    Int32 i;
    for (it = mDevices.Begin(), i = 0; it != mDevices.End(); ++it, ++i) {
        AutoPtr<IWifiP2pDevice> item = it->mSecond;
        array->Set(i, item);
    }

    *list = array;
    REFCOUNT_ADD(*list);
    return NOERROR;
}

ECode CWifiP2pDeviceList::IsGroupOwner(
    /* [in] */ const String& deviceAddress,
    /* [out] */ Boolean* isGroupOwner)
{
    VALIDATE_NOT_NULL(isGroupOwner);
    *isGroupOwner = FALSE;

    if (!deviceAddress.IsNull()) {
        HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it = mDevices.Find(deviceAddress);
        if (it != mDevices.End()) {
            AutoPtr<IWifiP2pDevice> device = it->mSecond;
            return device->IsGroupOwner(isGroupOwner);
        }
    }

    return NOERROR;
}

ECode CWifiP2pDeviceList::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);
    *string = String("");

    StringBuilder sb("");

    AutoPtr<ArrayOf<IWifiP2pDevice*> > array;
    FAIL_RETURN(GetDeviceList((ArrayOf<IWifiP2pDevice*>**)&array));
    if (array != NULL) {
        String temp;
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            FAIL_RETURN((*array)[i]->ToString(&temp));
            sb += "\n ";
            sb += temp;
        }
    }

    *string = sb.ToString();
    return NOERROR;
}

ECode CWifiP2pDeviceList::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    Int32 size;
    source->ReadInt32(&size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IWifiP2pDevice> device;
        source->ReadInterfacePtr((Handle32*)&device);
        Update(device);
    }
    return NOERROR;
}

ECode CWifiP2pDeviceList::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteInt32(mDevices.GetSize()));

    HashMap<String, AutoPtr<IWifiP2pDevice> >::Iterator it;
    for (it = mDevices.Begin(); it != mDevices.End(); ++it) {
        AutoPtr<IWifiP2pDevice> item = it->mSecond;
        FAIL_RETURN(dest->WriteInterfacePtr(item.Get()));
    }

    return NOERROR;
}


}
}
}
}
