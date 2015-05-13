
#include "net/CInterfaceConfiguration.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Net {

const String CInterfaceConfiguration::FLAG_UP("up");
const String CInterfaceConfiguration::FLAG_DOWN("down");

ECode CInterfaceConfiguration::constructor()
{
    return NOERROR;
}

ECode CInterfaceConfiguration::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    StringBuilder builder;
    builder.AppendCStr("mHwAddr=");
    builder.AppendString(mHwAddr);
    builder.AppendCStr(" mAddr=");
    String mAddrS;
    mAddr->ToString(&mAddrS);
    builder.AppendString(mAddrS);
    builder.AppendCStr(" mFlags=");
    AutoPtr< ArrayOf<String> > flags;
    GetFlags((ArrayOf<String>**)&flags);
    for(Int32 i = 0; i < flags->GetLength() ; i++) {
        builder.AppendString((*flags)[i]);
        builder.AppendCStr(", ");
    }
    *result = builder.ToString();
    return NOERROR;
}

ECode CInterfaceConfiguration::GetFlags(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr< ArrayOf<String> > resArray = ArrayOf<String>::Alloc(mFlags.GetSize());
    HashSet<String>::Iterator iter = mFlags.Begin();
    for (Int32 i = 0; iter != mFlags.End(); ++iter) {
        (*resArray)[i++] = *iter;
    }
    *result = resArray;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CInterfaceConfiguration::HasFlag(
    /* [in] */ const String& flag,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(ValidateFlag(flag));
    HashSet<String>::Iterator iter = mFlags.Find(flag);
    *result = iter != mFlags.End();
    return NOERROR;
}

ECode CInterfaceConfiguration::ClearFlag(
    /* [in] */ const String& flag)
{
    FAIL_RETURN(ValidateFlag(flag));
    mFlags.Erase(flag);
    return NOERROR;
}

ECode CInterfaceConfiguration::SetFlag(
    /* [in] */ const String& flag)
{
    FAIL_RETURN(ValidateFlag(flag));
    mFlags.Insert(flag);
    return NOERROR;
}

ECode CInterfaceConfiguration::SetInterfaceUp()
{
    mFlags.Erase(FLAG_DOWN);
    mFlags.Insert(FLAG_UP);
    return NOERROR;
}

ECode CInterfaceConfiguration::SetInterfaceDown()
{
    mFlags.Erase(FLAG_UP);
    mFlags.Insert(FLAG_DOWN);
    return NOERROR;
}

ECode CInterfaceConfiguration::GetLinkAddress(
    /* [out] */ ILinkAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAddr;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CInterfaceConfiguration::SetLinkAddress(
    /* [in] */ ILinkAddress* addr)
{
    mAddr = addr;
    return NOERROR;
}

ECode CInterfaceConfiguration::GetHardwareAddress(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHwAddr;
    return NOERROR;
}

ECode CInterfaceConfiguration::SetHardwareAddress(
    /* [in] */ const String& hwAddr)
{
    mHwAddr = hwAddr;
    return NOERROR;
}

ECode CInterfaceConfiguration::IsActive(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    Boolean hasFlag;
    if (HasFlag(FLAG_UP, &hasFlag), hasFlag) {
        AutoPtr<IInetAddress> addr;
        mAddr->GetAddress((IInetAddress**)&addr);
        AutoPtr< ArrayOf<Byte> > byteArray;
        addr->GetAddress((ArrayOf<Byte>**)&byteArray);
        for (Int32 i = 0; i < byteArray->GetLength(); i++) {
            Byte b = (*byteArray)[i];
            if (b != 0) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    // } catch (NullPointerException e) {
        // *result = FALSE;
    // }
    *result = FALSE;
    return NOERROR;
}

ECode CInterfaceConfiguration::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    src->ReadString(&mHwAddr);
    Byte bv;
    src->ReadByte(&bv);
    if (bv == 1) {
        AutoPtr<IInterface> obj;
        src->ReadInterfacePtr((Handle32*)&obj);
        mAddr = ILinkAddress::Probe(obj);
    }
    Int32 size;
    src->ReadInt32(&size);
    for (Int32 i = 0; i < size; i++) {
        String flag;
        src->ReadString(&flag);
        mFlags.Insert(flag);
    }
    return NOERROR;
}

ECode CInterfaceConfiguration::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mHwAddr);
    if (mAddr != NULL) {
        dest->WriteByte(1);
        dest->WriteInterfacePtr(mAddr.Get());
    }
    else {
        dest->WriteByte(0);
    }
    dest->WriteInt32(mFlags.GetSize());
    HashSet<String>::Iterator iter;
    for (iter = mFlags.Begin(); iter != mFlags.End(); ++iter) {
        dest->WriteString(*iter);
    }
    return NOERROR;
}

ECode CInterfaceConfiguration::ValidateFlag(
        /* [in] */ const String& flag)
{
    if (flag.IndexOf(' ') >= 0) {
        // throw new IllegalArgumentException("flag contains space: " + flag);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
