#include "CAttributes.h"
#include "cmdef.h"
#include "CHashMap.h"
#include "CStringWrapper.h"
#include "CName.h"

using Elastos::Utility::CHashMap;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Utility {
namespace Jar {

ECode CAttributes::GetValue(
    /* [in] */ IName* name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    AutoPtr<IInterface> valueInInterface;
    mMap->Get(name, (IInterface**)&valueInInterface);
    AutoPtr<ICharSequence> cs = ICharSequence::Probe(valueInInterface);
    if (cs) {
        return cs->ToString(value);
    }
    *value = String();
    return NOERROR;
}

ECode CAttributes::GetValueEx(
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    AutoPtr<IName> iname;
    FAIL_RETURN(CName::New(name, (IName**)&iname))
    return GetValue(iname, value);
}

ECode CAttributes::PutValue(
    /* [in] */ const String& name,
    /* [in] */ const String& val,
    /* [out] */ String* oldVal)
{
    AutoPtr<IName> iname;
    FAIL_RETURN(CName::New(name, (IName**)&iname))
    AutoPtr<ICharSequence> cs;
    FAIL_RETURN(CStringWrapper::New(val, (ICharSequence**)&cs))

    if (oldVal) {
        AutoPtr<IInterface> outface;
        mMap->Put(iname, cs, (IInterface**)&outface);
        ICharSequence* csRet = ICharSequence::Probe(outface);
        if (csRet) {
            csRet->ToString(oldVal);
        }
        else {
            *oldVal = NULL;
        }
    }
    else {
        mMap->Put(iname, cs, NULL);
    }
    return NOERROR;
}

ECode CAttributes::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IAttributes> clone;
    FAIL_RETURN(CAttributes::New((IAttributes*)IAttributes::Probe(this),
        (IAttributes**)&clone))
    *object = clone;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CAttributes::Clear()
{
    mMap = NULL;
    return NOERROR;
}

ECode CAttributes::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mMap->ContainsKey(key, result);
}

ECode CAttributes::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return mMap->ContainsValue(value, result);
}

ECode CAttributes::EntrySet(
    /* [out] */ ISet** entries)
{
    return mMap->EntrySet(entries);
}

ECode CAttributes::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if(object == this->Probe(EIID_IInterface)) {
        *result = TRUE;
        return NOERROR;
    }

    IAttributes* othterObj = IAttributes::Probe(object);
    if (othterObj) {
        return mMap->Equals(((CAttributes*)othterObj)->mMap, result);
    }
    return NOERROR;
}

ECode CAttributes::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mMap->Get(key, value);
}

ECode CAttributes::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mMap->GetHashCode(hashCode);
}

ECode CAttributes::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 size = 0;
    return (mMap->GetSize(&size), size) == 0;
}

ECode CAttributes::KeySet(
    /* [out] */ ISet** keySet)
{
    return mMap->KeySet(keySet);
}

ECode CAttributes::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return mMap->Put(key, value, oldValue);
}

ECode CAttributes::PutAll(
    /* [in] */ IMap* attrib)
{
    if (attrib == NULL || !IAttributes::Probe(attrib)) {
        return E_CLASS_NOT_FOUND_EXCEPTION;
    }

    return mMap->PutAll(attrib);
}

ECode CAttributes::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mMap->Remove(key, value);
}

ECode CAttributes::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mMap->GetSize(size);
}

ECode CAttributes::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)
    return mMap->Values(value);
}

ECode CAttributes::constructor()
{
    return CHashMap::New((IMap**)&mMap);
}

ECode CAttributes::constructor(
    /* [in] */ IAttributes* attributes)
{
    AutoPtr<ICloneable> res = ICloneable::Probe(((CAttributes*)attributes)->mMap);
    AutoPtr<IInterface> outface;
    ECode ec = res->Clone((IInterface**)&outface);
    mMap = IMap::Probe(outface);
    return ec;
}

ECode CAttributes::constructor(
    /* [in] */ Int32 size)
{
    return CHashMap::New(size, (IMap**)&mMap);
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos
