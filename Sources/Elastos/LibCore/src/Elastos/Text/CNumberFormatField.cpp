
#include "CNumberFormatField.h"

namespace Elastos {
namespace Text {

TEXTATTRIBUITEDCHARACTERITERATORATTRIBUTE_METHODS_IMPL(CNumberFormatField,AttributedCharacterIteratorAttribute)

ECode CNumberFormatField::constructor(
    /* [in] */ const String& fieldName)
{
    return AttributedCharacterIteratorAttribute::Init(fieldName);
}

PInterface CNumberFormatField::Probe(
    /* [in] */ REIID riid)
{
    return _CNumberFormatField::Probe(riid);
}

ECode CNumberFormatField::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return _CNumberFormatField::GetClassID(clsid);
}

} // namespace Text
} // namespace Elastos
