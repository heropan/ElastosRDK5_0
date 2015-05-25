
#include "CMessageFormatField.h"

namespace Elastos {
namespace Text {

TEXTATTRIBUITEDCHARACTERITERATORATTRIBUTE_METHODS_IMPL(CMessageFormatField, AttributedCharacterIteratorAttribute)

ECode CMessageFormatField::constructor(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return AttributedCharacterIteratorAttribute::Init(name);
}

ECode CMessageFormatField::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return _CMessageFormatField::GetClassID(clsid);
}

PInterface CMessageFormatField::Probe(
    /* [in] */ REIID riid)
{
    return _CMessageFormatField::Probe(riid);
}

} // namespace Text
} // namespace Elastos
