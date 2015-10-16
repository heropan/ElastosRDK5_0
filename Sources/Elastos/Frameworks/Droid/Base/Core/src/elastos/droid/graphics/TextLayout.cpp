#define LOG_TAG "TextLayout"

#include "elastos/droid/graphics/TextLayout.h"
#include "elastos/droid/graphics/TextLayoutCache.h"

// #include <android_runtime/AndroidRuntime.h>

#include <skia/core/SkTemplates.h>
#include <unicode/ubidi.h>
#include <unicode/ushape.h>
#include <utils/Log.h>

namespace android {

// Returns true if we might need layout.  If bidiFlags force LTR, assume no layout, if
// bidiFlags indicate there probably is RTL, assume we do, otherwise scan the text
// looking for a character >= the first RTL character in unicode and assume we do if
// we find one.
bool TextLayout::needsLayout(const Char16* text, Int32 len, Int32 bidiFlags)
{
    if (bidiFlags == kBidi_Force_LTR) {
        return false;
    }
    if ((bidiFlags == kBidi_RTL) || (bidiFlags == kBidi_Default_RTL) ||
            bidiFlags == kBidi_Force_RTL) {
        return true;
    }
    for (Int32 i = 0; i < len; ++i) {
        if (text[i] >= UNICODE_FIRST_RTL_CHAR) {
            return true;
        }
    }
    return false;
}

// Draws or gets the path of a paragraph of text on a single line, running bidi and shaping.
// This will draw if canvas is not null, otherwise path must be non-null and it will create
// a path representing the text that would have been drawn.
void TextLayout::handleText(SkPaint *paint, const Char16* text, Int32 len,
                            Int32 bidiFlags, Float x, Float y, SkPath *path)
{
    sp<TextLayoutValue> value = TextLayoutEngine::getInstance().getValue(paint,
            text, 0, len, len, bidiFlags);
    if (value == NULL) {
        return ;
    }
    SkScalar x_ = SkFloatToScalar(x);
    SkScalar y_ = SkFloatToScalar(y);
    // Beware: this needs Glyph encoding (already done on the Paint constructor)
    paint->getTextPath(value->getGlyphs(), value->getGlyphsCount() * 2, x_, y_, path);
}

void TextLayout::getTextRunAdvances(SkPaint* paint, const Char16* chars, Int32 start,
                                    Int32 count, Int32 contextCount, Int32 dirFlags,
                                    Float* resultAdvances, Float* resultTotalAdvance)
{
    sp<TextLayoutValue> value = TextLayoutEngine::getInstance().getValue(paint,
            chars, start, count, contextCount, dirFlags);
    if (value == NULL) {
        return ;
    }
    if (resultAdvances) {
        memcpy(resultAdvances, value->getAdvances(), value->getAdvancesCount() * sizeof(Float));
    }
    if (resultTotalAdvance) {
        *resultTotalAdvance = value->getTotalAdvance();
    }
}

void TextLayout::getTextRunAdvancesICU(SkPaint* paint, const Char16* chars, Int32 start,
                                    Int32 count, Int32 contextCount, Int32 dirFlags,
                                    Float* resultAdvances, Float& resultTotalAdvance)
{
    // Compute advances and return them
    computeAdvancesWithICU(paint, chars, start, count, contextCount, dirFlags,
            resultAdvances, &resultTotalAdvance);
}

void TextLayout::getTextPath(SkPaint *paint, const Char16 *text, Int32 len,
                             Int32 bidiFlags, Float x, Float y, SkPath *path)
{
    handleText(paint, text, len, bidiFlags, x, y, path);
}


void TextLayout::drawTextOnPath(SkPaint* paint, const Char16* text, Int32 count,
                                Int32 bidiFlags, Float hOffset, Float vOffset,
                                SkPath* path, SkCanvas* canvas)
{

    SkScalar h_ = SkFloatToScalar(hOffset);
    SkScalar v_ = SkFloatToScalar(vOffset);

    sp<TextLayoutValue> value = TextLayoutEngine::getInstance().getValue(paint,
            text, 0, count, count, bidiFlags);
    if (value == NULL) {
        return;
    }

    // Beware: this needs Glyph encoding (already done on the Paint constructor)
    canvas->drawTextOnPathHV(value->getGlyphs(), value->getGlyphsCount() * 2, *path, h_, v_, *paint);
}

void TextLayout::computeAdvancesWithICU(SkPaint* paint, const UChar* chars,
        size_t start, size_t count, size_t contextCount, Int32 dirFlags,
        Float* outAdvances, Float* outTotalAdvance)
{
    SkAutoSTMalloc<CHAR_BUFFER_SIZE, Char16> tempBuffer(contextCount);
    Char16* buffer = tempBuffer.get();
    SkScalar* scalarArray = (SkScalar*)outAdvances;

    // this is where we'd call harfbuzz
    // for now we just use ushape.c
    size_t widths;
    const Char16* text;
    if (dirFlags & 0x1) { // rtl, call arabic shaping in case
        UErrorCode status = U_ZERO_ERROR;
        // Use fixed length since we need to keep start and count valid
        u_shapeArabic(chars, contextCount, buffer, contextCount,
                U_SHAPE_LENGTH_FIXED_SPACES_NEAR |
                U_SHAPE_TEXT_DIRECTION_LOGICAL | U_SHAPE_LETTERS_SHAPE |
                U_SHAPE_X_LAMALEF_SUB_ALTERNATE, &status);
        // we shouldn't fail unless there's an out of memory condition,
        // in which case we're hosed anyway
        for (Int32 i = start, e = i + count; i < e; ++i) {
            if (buffer[i] == UNICODE_NOT_A_CHAR) {
                buffer[i] = UNICODE_ZWSP; // zero-width-space for skia
            }
        }
        text = buffer + start;
        widths = paint->getTextWidths(text, count << 1, scalarArray);
    } else {
        text = chars + start;
        widths = paint->getTextWidths(text, count << 1, scalarArray);
    }

    Float totalAdvance = 0;
    if (widths < count) {
#if DEBUG_ADVANCES
    ALOGD("ICU -- count=%d", widths);
#endif
        // Skia operates on code points, not code units, so surrogate pairs return only
        // one value. Expand the result so we have one value per UTF-16 code unit.

        // Note, skia's getTextWidth gets confused if it encounters a surrogate pair,
        // leaving the remaining widths zero.  Not nice.
        for (size_t i = 0, p = 0; i < widths; ++i) {
            totalAdvance += outAdvances[p++] = SkScalarToFloat(scalarArray[i]);
            if (p < count &&
                    text[p] >= UNICODE_FIRST_LOW_SURROGATE &&
                    text[p] < UNICODE_FIRST_PRIVATE_USE &&
                    text[p-1] >= UNICODE_FIRST_HIGH_SURROGATE &&
                    text[p-1] < UNICODE_FIRST_LOW_SURROGATE) {
                outAdvances[p++] = 0;
            }
#if DEBUG_ADVANCES
            ALOGD("icu-adv = %f - total = %f", outAdvances[i], totalAdvance);
#endif
        }
    } else {
#if DEBUG_ADVANCES
    ALOGD("ICU -- count=%d", count);
#endif
        for (size_t i = 0; i < count; i++) {
            totalAdvance += outAdvances[i] = SkScalarToFloat(scalarArray[i]);
#if DEBUG_ADVANCES
            ALOGD("icu-adv = %f - total = %f", outAdvances[i], totalAdvance);
#endif
        }
    }
    *outTotalAdvance = totalAdvance;
}

}
