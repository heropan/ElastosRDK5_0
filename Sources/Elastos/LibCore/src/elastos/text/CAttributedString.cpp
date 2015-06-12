
#include "CAttributedString.h"

namespace Elastos {
namespace Text {

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator)
{
    return AttributedString::constructor(iterator);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::constructor(iterator, start, end);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes)
{
    return AttributedString::constructor(iterator, start, end, attributes);
}

ECode CAttributedString::constructor(
    /* [in] */ const String& value)
{
    return AttributedString::constructor(value);
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

ECode CAttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::AddAttribute(attribute, value, start, end);
}

ECode CAttributedString::GetIterator(
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIterator(iterator);
}

ECode CAttributedString::GetIterator(
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIterator(attributes, iterator);
}

ECode CAttributedString::GetIterator(
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIterator(attributes, start, end, iterator);
}

ECode CAttributedString::AddAttributes(
    /* [in] */ IMap * attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(0 && "TODO");
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
