
#include "CAttributedString.h"

namespace Elastos {
namespace Text {

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator)
{
    return AttributedString::Init(iterator);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::Init(iterator, start, end);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes)
{
    return AttributedString::Init(iterator, start, end, attributes);
}

ECode CAttributedString::constructor(
    /* [in] */ const String& value)
{
    return AttributedString::Init(value);
}

// ECode CAttributedString::constructor(
//     /* [in] */ const String& value,
//     /* [in] */ IObjectMap * attributes)
// {
//     return AttributedString::Init(value,attributes);
// }

ECode CAttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value)
{
    return AttributedString::AddAttribute(attribute, value);
}

ECode CAttributedString::AddAttributeEx(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::AddAttributeEx(attribute, value, start, end);
}

ECode CAttributedString::GetIterator(
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIterator(iterator);
}

ECode CAttributedString::GetIteratorEx(
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIteratorEx(attributes, iterator);
}

ECode CAttributedString::GetIteratorEx2(
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIteratorEx2(attributes, start, end, iterator);
}

// ECode CAttributedString::AddAttributes(
//     /* [in] */ IObjectMap * attributes,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return NOERROR;
// }

} // namespace Text
} // namespace Elastos
