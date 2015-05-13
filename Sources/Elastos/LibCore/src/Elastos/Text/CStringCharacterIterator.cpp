
#include "CStringCharacterIterator.h"

namespace Elastos {
namespace Text {

ECode CStringCharacterIterator::SetText(
    /* [in] */ const String& value)
{
    return StringCharacterIterator::SetText(value);
}

ECode CStringCharacterIterator::GetBeginIndex(
    /* [out] */ Int32* index)
{
    return StringCharacterIterator::GetBeginIndex(index);
}

ECode CStringCharacterIterator::Clone(
    /* [out] */ ICharacterIterator** copy)
{
    return StringCharacterIterator::Clone(copy);
}

ECode CStringCharacterIterator::Current(
    /* [out] */ Char32* character)
{
    return StringCharacterIterator::Current(character);
}

ECode CStringCharacterIterator::GetEndIndex(
    /* [out] */ Int32* index)
{
    return StringCharacterIterator::GetEndIndex(index);
}

ECode CStringCharacterIterator::First(
    /* [out] */ Char32* character)
{
    return StringCharacterIterator::First(character);
}

ECode CStringCharacterIterator::GetIndex(
    /* [out] */ Int32* index)
{
    return StringCharacterIterator::GetIndex(index);
}

ECode CStringCharacterIterator::Last(
    /* [out] */ Char32* character)
{
    return StringCharacterIterator::Last(character);
}

ECode CStringCharacterIterator::Next(
    /* [out] */ Char32* character)
{
    return StringCharacterIterator::Next(character);
}

ECode CStringCharacterIterator::Previous(
    /* [out] */ Char32* character)
{
    return StringCharacterIterator::Previous(character);
}

ECode CStringCharacterIterator::SetIndex(
    /* [in] */ Int32 location,
    /* [out] */ Char32* character)
{
    return StringCharacterIterator::SetIndex(location,character);
}

ECode CStringCharacterIterator::constructor(
    /* [in] */ const String& value)
{
    return StringCharacterIterator::Init(value);
}

ECode CStringCharacterIterator::constructor(
    /* [in] */ const String& value,
    /* [in] */ Int32 location)
{
    return StringCharacterIterator::Init(value,location);
}

ECode CStringCharacterIterator::constructor(
    /* [in] */ const String& value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 location)
{
    return StringCharacterIterator::Init(value,start,end,location);
}

} // namespace Text
} // namespace Elastos
