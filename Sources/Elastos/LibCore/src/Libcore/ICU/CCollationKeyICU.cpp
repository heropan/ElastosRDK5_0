
#include "CCollationKeyICU.h"

namespace Libcore {
namespace ICU {

PInterface CCollationKeyICU::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_CollationKeyICU) {
        return reinterpret_cast<PInterface>((CollationKeyICU*)this);
    }
    return _CCollationKeyICU::Probe(riid);
}

ECode CCollationKeyICU::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * value)
{
    return CollationKeyICU::Equals(object,value);
}

ECode CCollationKeyICU::GetHashCode(
    /* [out] */ Int32 * value)
{
    return CollationKeyICU::GetHashCode(value);
}

ECode CCollationKeyICU::CompareTo(
    /* [in] */ IInterface* value,
    /* [out] */ Int32* result)
{
    return CollationKeyICU::CompareTo(value,result);
}

ECode CCollationKeyICU::GetSourceString(
    /* [out] */ String* sourceString)
{
    return CollationKeyICU::GetSourceString(sourceString);
}

ECode CCollationKeyICU::ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** array)
{
    return CollationKeyICU::ToByteArray(array);
}

ECode CCollationKeyICU::constructor(
    /* [in] */ const String& source,
    /* [in] */ const ArrayOf<Byte>& bytes)
{
    return CollationKeyICU::Init(source,bytes);
}

} // namespace ICU
} // namespace Libcore
