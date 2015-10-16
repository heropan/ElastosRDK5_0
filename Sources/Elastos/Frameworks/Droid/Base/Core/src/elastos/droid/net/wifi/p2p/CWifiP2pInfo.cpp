
#include "CWifiP2pInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

ECode CWifiP2pInfo::GetGroupFormed(
    /* [out] */ Boolean* formed)
{
    VALIDATE_NOT_NULL(formed);

    *formed = mGroupFormed;

    return NOERROR;
}

ECode CWifiP2pInfo::SetGroupFormed(
    /* [in] */ Boolean formed)
{
    mGroupFormed = formed;

    return NOERROR;
}

ECode CWifiP2pInfo::IsGroupOwner(
    /* [out] */ Boolean* isGroupOwner)
{
    VALIDATE_NOT_NULL(isGroupOwner);

    *isGroupOwner = mIsGroupOwner;

    return NOERROR;
}

ECode CWifiP2pInfo::SetIsGroupOwner(
    /* [in] */ Boolean isGroupOwner)
{
    mIsGroupOwner = isGroupOwner;

    return NOERROR;
}

ECode CWifiP2pInfo::GetGroupOwnerAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    *address = mGroupOwnerAddress;
    REFCOUNT_ADD(*address);

    return NOERROR;
}

ECode CWifiP2pInfo::SetGroupOwnerAddress(
    /* [in] */ IInetAddress* address)
{
    mGroupOwnerAddress = address;

    return NOERROR;
}

ECode CWifiP2pInfo::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    StringBuilder sb;
    sb += "groupFormed: ";
    sb += mGroupFormed;
    sb += " isGroupOwner: ";
    sb += mIsGroupOwner;
    sb += " groupOwnerAddress: ";
    if (mGroupOwnerAddress) {
        String temp;
        mGroupOwnerAddress->ToString(&temp);
        sb += temp;
    }
    else {
        sb += " NULL";
    }

    *string = sb.ToString();
    return NOERROR;
}

ECode CWifiP2pInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    Byte b;
    source->ReadByte(&b);
    mGroupFormed = (b == 1);
    source->ReadByte(&b);
    mIsGroupOwner = (b == 1);

    mGroupOwnerAddress = NULL;
    source->ReadByte(&b);
    if (b == 1) {
        AutoPtr<ArrayOf<Byte> > bytes;
        source->ReadArrayOf((Handle32*)&bytes); //in.createByteArray()

        AutoPtr<IInetAddressHelper> helper;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
        helper->GetByAddress(bytes, (IInetAddress**)&mGroupOwnerAddress);
    }

    return NOERROR;
}

ECode CWifiP2pInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteByte(mGroupFormed ? (Byte)1 : (Byte)0));
    FAIL_RETURN(dest->WriteByte(mIsGroupOwner ? (Byte)1 : (Byte)0));

    if (mGroupOwnerAddress != NULL) {
        FAIL_RETURN(dest->WriteByte((Byte)1));
        AutoPtr<ArrayOf<Byte> > bytes;
        mGroupOwnerAddress->GetAddress((ArrayOf<Byte> **)&bytes);
        FAIL_RETURN(dest->WriteArrayOf((Handle32)bytes.Get()));
    }
    else {
        FAIL_RETURN(dest->WriteByte((Byte)0));
    }

    return NOERROR;
}

ECode CWifiP2pInfo::constructor()
{
    return NOERROR;
}

ECode CWifiP2pInfo::constructor(
    /* [in] */ IWifiP2pInfo* source)
{
    if (source != NULL) {
        FAIL_RETURN(source->GetGroupFormed(&mGroupFormed));
        FAIL_RETURN(source->IsGroupOwner(&mIsGroupOwner));
        FAIL_RETURN(source->GetGroupOwnerAddress(
            (IInetAddress**)&mGroupOwnerAddress));
    }

    return NOERROR;
}

}
}
}
}
}
