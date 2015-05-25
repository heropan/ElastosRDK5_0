
#include "CBidi.h"

namespace Elastos {
namespace Text {

ECode CBidi::constructor(
    /* [in] */ IAttributedCharacterIterator * pParagraph)
{
    return Bidi::Init(pParagraph);
}

ECode CBidi::constructor(
    /* [in] */ ArrayOf<Char32>* texts,
    /* [in] */ Int32 textStart,
    /* [in] */ ArrayOf<Byte>* embeddings,
    /* [in] */ Int32 embStart,
    /* [in] */ Int32 paragraphLength,
    /* [in] */ Int32 flags)
{
    return Bidi::Init(texts, textStart, embeddings, embStart, paragraphLength, flags);
}

ECode CBidi::constructor(
    /* [in] */ const String& paragraph,
    /* [in] */ Int32 flags)
{
    return Bidi::Init(paragraph, flags);
}

ECode CBidi::constructor(
    /* [in] */ Int64 pBidi)
{
    return Bidi::Init(pBidi);
}

ECode CBidi::BaseIsLeftToRight(
    /* [out] */ Boolean* baseLevel)
{
    return Bidi::BaseIsLeftToRight(baseLevel);
}

ECode CBidi::CreateLineBidi(
    /* [in] */ Int32 lineStart,
    /* [in] */ Int32 lineLimit,
    /* [out] */ IBidi** object)
{
    return Bidi::CreateLineBidi(lineStart, lineLimit, object);
}

ECode CBidi::GetBaseLevel(
    /* [out] */ Int32* baseLevel)
{
    return Bidi::GetBaseLevel(baseLevel);
}

ECode CBidi::GetLength(
    /* [out] */ Int32* length)
{
    return Bidi::GetLength(length);
}

ECode CBidi::GetLevelAt(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* level)
{
    return Bidi::GetLevelAt(offset, level);
}

ECode CBidi::GetRunCount(
    /* [out] */ Int32* runCount)
{
    return Bidi::GetRunCount(runCount);
}

ECode CBidi::GetRunLevel(
    /* [in] */ Int32 run,
    /* [out] */ Int32* runLevel)
{
    return Bidi::GetRunLevel(run, runLevel);
}

ECode CBidi::GetRunLimit(
    /* [in] */ Int32 run,
    /* [out] */ Int32* runLimit)
{
    return Bidi::GetRunLimit(run, runLimit);
}

ECode CBidi::GetRunStart(
    /* [in] */ Int32 run,
    /* [out] */ Int32* runStart)
{
    return Bidi::GetRunStart(run, runStart);
}

ECode CBidi::IsLeftToRight(
    /* [out] */ Boolean* isLefttoRight)
{
    return Bidi::IsLeftToRight(isLefttoRight);
}

ECode CBidi::IsMixed(
    /* [out] */ Boolean* isMixed)
{
    return Bidi::IsMixed(isMixed);
}

ECode CBidi::IsRightToLeft(
    /* [out] */ Boolean* isRightToLeft)
{
    return Bidi::IsRightToLeft(isRightToLeft);
}

} // namespace Text
} // namespace Elastos