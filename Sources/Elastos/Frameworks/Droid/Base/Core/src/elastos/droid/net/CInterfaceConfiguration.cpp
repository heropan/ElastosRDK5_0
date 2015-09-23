
#include "elastos/droid/net/CInterfaceConfiguration.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_OBJECT_IMPL(CInterfaceConfiguration)

CAR_INTERFACE_IMPL_2(CInterfaceConfiguration, Object, IParcelable, IInterfaceConfiguration)

const String CInterfaceConfiguration::FLAG_UP("up");
const String CInterfaceConfiguration::FLAG_DOWN("down");

CInterfaceConfiguration::CInterfaceConfiguration()
{
    AutoPtr<ISets> tmp;
#if 0 // TODO: Wait for CSets, ISets
    CSets::AcquireSingleton((ISets**)&tmp);
    tmp->NewHashSet((IHashSet**)&mFlags);
#else
    assert(0);
#endif
}

ECode CInterfaceConfiguration::constructor()
{
    return NOERROR;
}

ECode CInterfaceConfiguration::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
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
#endif
}

ECode CInterfaceConfiguration::GetFlags(
    /* [out] */ IIterable** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    VALIDATE_NOT_NULL(result);

    AutoPtr< ArrayOf<String> > resArray = ArrayOf<String>::Alloc(mFlags.GetSize());
    HashSet<String>::Iterator iter = mFlags.Begin();
    for (Int32 i = 0; iter != mFlags.End(); ++iter) {
        (*resArray)[i++] = *iter;
    }
    *result = resArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::HasFlag(
    /* [in] */ const String& flag,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(ValidateFlag(flag));
    HashSet<String>::Iterator iter = mFlags.Find(flag);
    *result = iter != mFlags.End();
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::ClearFlag(
    /* [in] */ const String& flag)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    FAIL_RETURN(ValidateFlag(flag));
    mFlags.Erase(flag);
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::SetFlag(
    /* [in] */ const String& flag)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    FAIL_RETURN(ValidateFlag(flag));
    mFlags.Insert(flag);
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::SetInterfaceUp()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    mFlags.Erase(FLAG_DOWN);
    mFlags.Insert(FLAG_UP);
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::SetInterfaceDown()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    mFlags.Erase(FLAG_UP);
    mFlags.Insert(FLAG_DOWN);
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::GetLinkAddress(
    /* [out] */ ILinkAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mAddr;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::SetLinkAddress(
    /* [in] */ ILinkAddress* addr)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    mAddr = addr;
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::GetHardwareAddress(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mHwAddr;
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::SetHardwareAddress(
    /* [in] */ const String& hwAddr)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    mHwAddr = hwAddr;
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::IsActive(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
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
#endif
}

ECode CInterfaceConfiguration::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
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
#endif
}

ECode CInterfaceConfiguration::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
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
#endif
}

ECode CInterfaceConfiguration::ValidateFlag(
        /* [in] */ const String& flag)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Previous translated. Need check.
    if (flag.IndexOf(' ') >= 0) {
        // throw new IllegalArgumentException("flag contains space: " + flag);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
#endif
}

ECode CInterfaceConfiguration::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
