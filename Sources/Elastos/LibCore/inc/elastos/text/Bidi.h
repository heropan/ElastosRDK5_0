#ifndef __ELASTOS_TEXT_BIDI_H__
#define __ELASTOS_TEXT_BIDI_H__

#include <elastos/core/Object.h>

using Elastos::Text::IAttributedCharacterIterator;;

namespace Elastos {
namespace Text {

class Bidi
    : public Object
    , public IBidi
{
public:
    class BidiRun : public Object {
    public:
        BidiRun(
            /* [in] */ Int32 start,
            /* [in] */ Int32 limit,
            /* [in] */ Int32 level);

        CARAPI_(Int32) GetLevel();

        CARAPI_(Int32) GetLimit();

        CARAPI_(Int32) GetStart();

    private:
        Int32 mStart;
        Int32 mLimit;
        Int32 mLevel;
    };

public:
    CAR_INTERFACE_DECL()

    Bidi();

    virtual ~Bidi();

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* paragraph);

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ const String& paragraph,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ Int64 pBidi);

    CARAPI BaseIsLeftToRight(
        /* [out] */ Boolean* isLeftToRight);

    CARAPI CreateLineBidi(
        /* [in] */ Int32 lineStart,
        /* [in] */ Int32 lineLimit,
        /* [out] */ IBidi** lineBidi);

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

    static CARAPI ReorderVisually(
        /* [in] */ ArrayOf<Byte>* levels,
        /* [in] */ Int32 levelStart,
        /* [in] */ ArrayOf<IInterface*>* objects,
        /* [in] */ Int32 objectStart,
        /* [in] */ Int32 count);

    static CARAPI RequiresBidi(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [out] */ Boolean * result);

private:

    static CARAPI CreateUBiDi(
        /* [in] */ ArrayOf<Char32>* texts,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags,
        /* [out] */ Int64* uBiDi);

    CARAPI_(void) ReadBidiInfo(
        /* [in] */ Int64 pBidi);

    CARAPI CreateEmptyLineBidi(
        /* [in] */ Int64 parent,
        /* [out] */ IBidi** lineBidi);

    static CARAPI_(Int64) Ubidi_open();

    static CARAPI_(void) Ubidi_close(
        /* [in] */ Int64 pBiDi);

    static CARAPI Ubidi_setPara(
        /* [in] */ Int64 pBiDi,
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 length,
        /* [in] */ Int32 paraLevel,
        /* [in] */ ArrayOf<Byte>* embeddingLevels);

    static CARAPI Ubidi_setLine(
        /* [in] */ Int64 pParaBiDi,
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [out] */ Int64* line);

    static CARAPI_(Int32) Ubidi_getDirection(
        /* [in] */ Int64 pBiDi);

    static CARAPI_(Int32) Ubidi_getLength(
        /* [in] */ Int64 pBiDi);

    static CARAPI_(Byte) Ubidi_getParaLevel(
        /* [in] */ Int64 pBiDi);

    static CARAPI Ubidi_getLevels(
        /* [in] */ Int64 pBiDi,
        /* [out] */ ArrayOf<Byte>** levels);

    static CARAPI Ubidi_countRuns(
        /* [in] */ Int64 pBiDi,
        /* [out] */ Int32* count);

    static CARAPI Ubidi_getRuns(
        /* [in] */ Int64 pBidi,
        /* [out, callee] */ ArrayOf<BidiRun*>** runs);

public:

    static CARAPI_(AutoPtr<ArrayOf<Int32> >) Ubidi_reorderVisual(
        /* [in] */ ArrayOf<Byte>* levelBytes,
        /* [in] */ Int32 length);

private:
    Int32 mBaseLevel;

    Int32 mLength;

    AutoPtr<ArrayOf<Byte> > mOffsetLevel;

    AutoPtr<ArrayOf<BidiRun*> > mRuns;

    Int32 mDirection;

    Boolean mUnidirectional;

    // const static Int32 UBIDI_LEVEL_OVERRIDE;
    const static Int32 UBiDiDirection_UBIDI_LTR;
    const static Int32 UBiDiDirection_UBIDI_RTL;
    const static Int32 UBiDiDirection_UBIDI_MIXED;
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_BIDI_H__
