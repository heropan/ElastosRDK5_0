
#ifndef __ELASTOS_TEXT_CBIDI_H__
#define __ELASTOS_TEXT_CBIDI_H__

#include "_Elastos_Text_CBidi.h"
#include "Bidi.h"

namespace Elastos {
namespace Text {

CarClass(CBidi) , public Bidi
{
public:
    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* paragraph);

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32>* pTexts,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* pEmbeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ const String& paragraph,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ Int64 pBidi);

    CARAPI BaseIsLeftToRight(
        /* [out] */ Boolean* baseLevel);

    CARAPI CreateLineBidi(
        /* [in] */ Int32 lineStart,
        /* [in] */ Int32 lineLimit,
        /* [out] */ IBidi** object);

    CARAPI GetBaseLevel(
        /* [out] */ Int32* baseLevel);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetLevelAt(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* level);

    CARAPI GetRunCount(
        /* [out] */ Int32* runCount);

    CARAPI GetRunLevel(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runLevel);

    CARAPI GetRunLimit(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runLimit);

    CARAPI GetRunStart(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runStart);

    CARAPI IsLeftToRight(
        /* [out] */ Boolean* isLefttoRight);

    CARAPI IsMixed(
        /* [out] */ Boolean* isMixed);

    CARAPI IsRightToLeft(
        /* [out] */ Boolean* isRightToLeft);
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CBIDI_H__
