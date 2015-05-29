
#include "CAttributedCharacterIteratorAttribute.h"

namespace Elastos {
namespace Text {

ECode CAttributedCharacterIteratorAttribute::constructor(
    /* [in] */ const String& name)
{
    return AttributedCharacterIteratorAttribute::Init(name);
}

ECode CAttributedCharacterIteratorAttribute::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return _CAttributedCharacterIteratorAttribute::GetClassID(clsid);
}

ECode CAttributedCharacterIteratorAttribute::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return AttributedCharacterIteratorAttribute::GetName(name);
}

PInterface CAttributedCharacterIteratorAttribute::Probe(
    /* [in] */ REIID riid)
{
    return _CAttributedCharacterIteratorAttribute::Probe(riid);
}

ECode CAttributedCharacterIteratorAttribute::Equals(
    /* [in] */ IInterface * obj,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    return AttributedCharacterIteratorAttribute::Equals(obj,value);
}

ECode CAttributedCharacterIteratorAttribute::GetHashCode(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);
    return AttributedCharacterIteratorAttribute::GetHashCode(value);
}

ECode CAttributedCharacterIteratorAttribute::ReadResolve(
    /* [out] */ IInterface** resolve)
{
    VALIDATE_NOT_NULL(resolve);
    return AttributedCharacterIteratorAttribute::ReadResolve(resolve);
}

ECode CAttributedCharacterIteratorAttribute::ToString(
    /* [out] */ String * str)
{
    VALIDATE_NOT_NULL(str);
    return AttributedCharacterIteratorAttribute::ToString(str);
}


} // namespace Text
} // namespace Elastos

