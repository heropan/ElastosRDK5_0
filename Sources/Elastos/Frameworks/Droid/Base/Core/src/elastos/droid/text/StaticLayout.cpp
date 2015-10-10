#include "elastos/droid/text/StaticLayout.h"
#include <elastos/core/Math.h>
#include <elastos/core/Character.h>
#include "elastos/droid/utility/ArrayUtils.h"
#include "graphics/CPaintFontMetricsInt.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/text/TextDirectionHeuristics.h"
#include "elastos/droid/text/AndroidBidi.h"

#include <elastos/utility/logging/Slogger.h>
#include <unistd.h>
using Elastos::Utility::Logging::Slogger;

using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::Character;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Graphics::CPaintFontMetricsInt;
using Elastos::Droid::Text::Style::EIID_ILineHeightSpan;
using Elastos::Droid::Text::Style::EIID_ILeadingMarginSpan;
using Elastos::Droid::Text::Style::EIID_ILeadingMarginSpan2;
using Elastos::Droid::Text::Style::ILineHeightSpanWithDensity;
using Elastos::Droid::Text::Style::ILeadingMarginSpan;
using Elastos::Droid::Text::Style::ILeadingMarginSpan2;
using Elastos::Droid::Text::Style::EIID_IMetricAffectingSpan;
using Elastos::Droid::Text::Style::IMetricAffectingSpan;
using Elastos::Droid::Text::Style::EIID_ITabStopSpan;

namespace Elastos {
namespace Droid {
namespace Text {

const String StaticLayout::TAG("StaticLayout");
const Int32 StaticLayout::COLUMNS_NORMAL;
const Int32 StaticLayout::COLUMNS_ELLIPSIZE;
const Int32 StaticLayout::START;
const Int32 StaticLayout::DIR;
const Int32 StaticLayout::TAB;
const Int32 StaticLayout::TOP;
const Int32 StaticLayout::DESCENT;
const Int32 StaticLayout::ELLIPSIS_START;
const Int32 StaticLayout::ELLIPSIS_COUNT;

const Int32 StaticLayout::START_MASK;
const Int32 StaticLayout::DIR_SHIFT;
const Int32 StaticLayout::TAB_MASK;

const Int32 StaticLayout::TAB_INCREMENT;

const Char32 StaticLayout::CHAR_NEW_LINE;
const Char32 StaticLayout::CHAR_TAB;
const Char32 StaticLayout::CHAR_SPACE;
const Char32 StaticLayout::CHAR_ZWSP;

const Double StaticLayout::EXTRA_ROUNDING;

const Int32 StaticLayout::CHAR_FIRST_HIGH_SURROGATE;
const Int32 StaticLayout::CHAR_LAST_LOW_SURROGATE;

StaticLayout::StaticLayout()
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
}

StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    assert(source != NULL);
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    Int32 length = 0;
    source->GetLength(&length);
    Init(source, 0, length, paint, width, align,
             spacingmult, spacingadd, includepad);
}

/**
 * @hide
 */
StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    assert(source != NULL);
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    Int32 length = 0;
    source->GetLength(&length);
    Init(source, 0, length, paint, width, align, textDir,
                spacingmult, spacingadd, includepad);
}

StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    Init(source, bufstart, bufend, paint, outerwidth, align,
             spacingmult, spacingadd, includepad,
             TextUtilsTruncateAt_NONE, 0);
}

/**
 * @hide
 */
StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    Init(source, bufstart, bufend, paint, outerwidth, align, textDir,
                spacingmult, spacingadd, includepad,
                TextUtilsTruncateAt_NONE, 0, Elastos::Core::Math::INT32_MAX_VALUE);
}

StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    Init(source, bufstart, bufend, paint, outerwidth, align,
                TextDirectionHeuristics::FIRSTSTRONG_LTR,
                spacingmult, spacingadd, includepad, ellipsize,
                ellipsizedWidth, Elastos::Core::Math::INT32_MAX_VALUE);
}

/**
 * @hide
 */
StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth,
    /* [in] */ Int32 maxLines)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    assert(source != NULL);

    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    AutoPtr<ICharSequence> tempCS;
    if (source->Probe(EIID_ISpanned)) {
        assert(0 && "TODO");
//        tempCS = (ICharSequence*)(new SpannedEllipsizer(source));
    } else {
        tempCS = (ICharSequence*)(new Ellipsizer(source));
    }

    AutoPtr<ICharSequence> initCS;
    if (ellipsize == TextUtilsTruncateAt_NONE) {
        initCS = source;
    } else {
        initCS = tempCS;
    }

    Layout::Init(initCS,
          paint, outerwidth, align, textDir, spacingmult, spacingadd);

    /*
     * This is annoying, but we can't refer to the layout until
     * superclass construction is finished, and the superclass
     * constructor wants the reference to the display text.
     *
     * This will break if the superclass constructor ever actually
     * cares about the content instead of just holding the reference.
     */
    if (ellipsize != TextUtilsTruncateAt_NONE) {
        AutoPtr<ICharSequence> charSeq = GetText();
        AutoPtr<Ellipsizer> e = (Ellipsizer*)(charSeq->Probe(EIID_Ellipsizer));
        assert(e != NULL);

        e->mLayout = this;
        e->mWidth = ellipsizedWidth;
        e->mMethod = ellipsize;
        mEllipsizedWidth = ellipsizedWidth;

        mColumns = COLUMNS_ELLIPSIZE;
    } else {
        mColumns = COLUMNS_NORMAL;
        mEllipsizedWidth = outerwidth;
    }

    mLineDirections = ArrayUtils::NewUnpaddedArray(2 * mColumns);
    mLines = ArrayOf<Int32>::Alloc(mLineDirections->GetLength);
    mMaximumVisibleLineCount = maxLines;

    mMeasured = MeasuredText::Obtain();

    Generate(source, bufstart, bufend, paint, outerwidth, textDir, spacingmult,
             spacingadd, includepad, includepad, ellipsizedWidth,
             ellipsize);

    mMeasured = MeasuredText::Recycle(mMeasured);
    mFontMetricsInt = NULL;
}

/* package */
StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* text)
    : mLineCount(0)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mColumns(0)
    , mEllipsizedWidth(0)
    , mMaximumVisibleLineCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&mFontMetricsInt);
    Layout::Init(text, NULL, 0, ALIGN_NONE, 0.0f, 0.0f);

    mColumns = COLUMNS_ELLIPSIZE;
    mLineDirections = ArrayUtils::NewUnpaddedArray(2 * mColumns);
    mLines = ArrayOf<Int32>::Alloc(mLineDirections->GetLength);
    // FIXME This is never recycled
    mMeasured = MeasuredText::Obtain();
}

/* package */
void StaticLayout::Generate(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufStart,
    /* [in] */ Int32 bufEnd,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerWidth,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ Boolean trackpad,
    /* [in] */ Float ellipsizedWidth,
    /* [in] */ TextUtilsTruncateAt ellipsize)
{
    AutoPtr< ArrayOf<Int32> > breakOpp;
    AutoPtr<ILocale> locale;
    paint->GetTextLocale((ILocale**)&locale);
    String localeLanguageTag;
    locale->ToLanguageTag(&localeLanguageTag);

    mLineCount = 0;

    Int32 v = 0;
    Boolean needMultiply = (spacingmult != 1 || spacingadd != 0);

    AutoPtr<IPaintFontMetricsInt> fm = mFontMetricsInt;
    AutoPtr< ArrayOf<Int32> > chooseHtv;

    AutoPtr<MeasuredText> measured = mMeasured;
    AutoPtr<ISpanned> spanned;
    if (ISpanned::Probe(source))
        spanned = ISpanned::Probe(source);

    Int32 paraEnd;
    for (Int32 paraStart = bufStart; paraStart <= bufEnd; paraStart = paraEnd) {
        paraEnd = TextUtils::IndexOf(source, CHAR_NEW_LINE, paraStart, bufEnd);
        if (paraEnd < 0)
            paraEnd = bufEnd;
        else
            paraEnd++;

        Int32 firstWidthLineLimit = mLineCount + 1;
        Int32 firstWidth = outerWidth;
        Int32 restWidth = outerWidth;

        AutoPtr< ArrayOf<IInterface*> > chooseHt; // ILineHeightSpan

        if (spanned != NULL) {
            AutoPtr< ArrayOf<IInterface*> > sp = GetParagraphSpans(spanned, paraStart, paraEnd,
                    EIID_ILeadingMarginSpan);
            Int32 tempWidth;
            for (Int32 i = 0; i < sp->GetLength(); i++) {
                AutoPtr<ILeadingMarginSpan> lms = (ILeadingMarginSpan*)((*sp)[i]->Probe(EIID_ILeadingMarginSpan));
                lms->GetLeadingMargin(TRUE, &tempWidth);
                firstWidth -= tempWidth;
                lms->GetLeadingMargin(FALSE, &tempWidth);
                restWidth -= tempWidth;

                // LeadingMarginSpan2 is odd.  The count affects all
                // leading margin spans, not just this particular one
                if (lms->Probe(EIID_ILeadingMarginSpan2)) {
                    AutoPtr<ILeadingMarginSpan2> lms2 = (ILeadingMarginSpan2*)(lms->Probe(EIID_ILeadingMarginSpan2));
                    Int32 temp = 0;
                    spanned->GetSpanStart(lms2, &temp);
                    Int32 lmsFirstLine = GetLineForOffset(temp);
                    lms2->GetLeadingMarginLineCount(&temp);
                    firstWidthLineLimit = Elastos::Core::Math::Max(firstWidthLineLimit, lmsFirstLine + temp);
                }
            }

            chooseHt = GetParagraphSpans(spanned, paraStart, paraEnd, EIID_ILineHeightSpan);

            Int32 length = chooseHt->GetLength();
            if (length != 0) {
                if (chooseHtv == NULL ||
                    chooseHtv->GetLength() < length) {
                    chooseHtv = ArrayUtils::NewUnpaddedIntArray(chooseHt->GetLength());
                }

                for (Int32 i = 0; i < length; i++) {
                    AutoPtr<ILineHeightSpan> lhs = (ILineHeightSpan*)((*chooseHt)[i]->Probe(EIID_ILineHeightSpan));
                    Int32 o = 0;
                    spanned->GetSpanStart(lhs, &o);

                    if (o < paraStart) {
                        // starts in this layout, before the
                        // current paragraph

                        (*chooseHtv)[i] = GetLineTop(GetLineForOffset(o));
                    } else {
                        // starts in this paragraph

                        (*chooseHtv)[i] = v;
                    }
                }
            }
        }

        measured->SetPara(source, paraStart, paraEnd, textDir);

        AutoPtr< ArrayOf<Char32> > chs = measured->mChars;
        AutoPtr< ArrayOf<Float> > widths = measured->mWidths;
        AutoPtr< ArrayOf<Byte> > chdirs = measured->mLevels;
        Int32 dir = measured->mDir;
        Boolean easy = measured->mEasy;

        breakOpp = nLineBreakOpportunities(localeLanguageTag, chs, paraEnd - paraStart, breakOpp);
        int breakOppIndex = 0;

        Int32 width = firstWidth;

        Float w = 0;
        // here is the offset of the starting character of the line we are currently measuring
        Int32 here = paraStart;

        // ok is a character offset located after a word separator (space, tab, number...) where
        // we would prefer to cut the current line. Equals to here when no such break was found.
        Int32 ok = paraStart;
        Float okWidth = w;
        Int32 okAscent = 0, okDescent = 0, okTop = 0, okBottom = 0;

        // fit is a character offset such that the [here, fit[ range fits in the allowed width.
        // We will cut the line there if no ok position is found.
        Int32 fit = paraStart;
        Float fitWidth = w;
        Int32 fitAscent = 0, fitDescent = 0, fitTop = 0, fitBottom = 0;
        // same as fitWidth but not including any trailing whitespace
        Float fitWidthGraphing = w;

        Boolean hasTabOrEmoji = FALSE;
        Boolean hasTab = FALSE;
        AutoPtr<TabStops> tabStops;

        for (Int32 spanStart = paraStart, spanEnd; spanStart < paraEnd; spanStart = spanEnd) {
            if (spanned == NULL) {
                spanEnd = paraEnd;
                Int32 spanLen = spanEnd - spanStart;
                measured->AddStyleRun(paint, spanLen, fm);
            }
            else {
                spanned->NextSpanTransition(spanStart, paraEnd,
                        EIID_IMetricAffectingSpan, &spanEnd);
                Int32 spanLen = spanEnd - spanStart;
                AutoPtr< ArrayOf<IInterface*> > spans; //IMetricAffectingSpan
                spanned->GetSpans(spanStart, spanEnd, EIID_IMetricAffectingSpan,
                           (ArrayOf<IInterface*>**)&spans);
                spans = TextUtils::RemoveEmptySpans(spans, spanned, EIID_IMetricAffectingSpan);
                measured->AddStyleRun(paint, spans, spanLen, fm);
            }

            Int32 fmTop;
            Int32 fmBottom;
            Int32 fmAscent;
            Int32 fmDescent;
            assert(fm != NULL);
            fm->GetTop(&fmTop);
            fm->GetBottom(&fmBottom);
            fm->GetAscent(&fmAscent);
            fm->GetDescent(&fmDescent);

            for (Int32 j = spanStart; j < spanEnd; j++) {
                Char32 c = (*chs)[j - paraStart];

                if (c == CHAR_NEW_LINE) {
                    // intentionally left empty
                }
                else if (c == CHAR_TAB) {
                    if (hasTab == FALSE) {
                        hasTab = TRUE;
                        hasTabOrEmoji = TRUE;
                        if (spanned != NULL) {
                            // First tab this para, check for tabstops
                            AutoPtr< ArrayOf<IInterface*> > spans = GetParagraphSpans(spanned, paraStart,
                                    paraEnd, EIID_ITabStopSpan);//ITabStopSpan*
                            if (spans->GetLength() > 0) {
                                tabStops = new TabStops(TAB_INCREMENT, spans);
                            }
                        }
                    }
                    if (tabStops != NULL) {
                        w = tabStops->NextTab(w);
                    } else {
                        w = tabStops->NextDefaultStop(w, TAB_INCREMENT);
                    }
                }
                else if (c >= CHAR_FIRST_HIGH_SURROGATE && c <= CHAR_LAST_LOW_SURROGATE
                        && j + 1 < spanEnd) {
                    assert(0 && "TODO");
//                    Int32 emoji = Character.codePointAt(chs, j - paraStart);
//
//                    if (emoji >= MIN_EMOJI && emoji <= MAX_EMOJI) {
//                        AutoPtr<IBitmap> bm = EMOJI_FACTORY->GetBitmapFromAndroidPua(emoji);
//
//                        if (bm != NULL) {
//                            AutoPtr<IPaint> whichPaint;
//
//                            if (spanned == NULL) {
//                                whichPaint = paint;
//                            } else {
//                                whichPaint = mWorkPaint;
//                            }
//
//                            Int32 width, height, ascent;
//                            bm->GetWidth(&width);
//                            whichPaint->Ascent(&ascent);
//                            bm->GetHeight(&height);
//                            Float wid = width * -ascent / height;
//
//                            w += wid;
//                            hasTabOrEmoji = TRUE;
//                            j++;
//                        } else {
//                            w += widths[j - paraStart];
//                        }
//                    } else {
//                        w += widths[j - paraStart];
//                    }
                } else {
                    w += (*widths)[j - paraStart];
                }

                Boolean isSpaceOrTab = c == CHAR_SPACE || c == CHAR_TAB || c == CHAR_ZWSP;

                if (w <= width || isSpaceOrTab) {
                    fitWidth = w;
                    fit = j + 1;

                    if (fmTop < fitTop)
                        fitTop = fmTop;
                    if (fmAscent < fitAscent)
                        fitAscent = fmAscent;
                    if (fmDescent > fitDescent)
                        fitDescent = fmDescent;
                    if (fmBottom > fitBottom)
                        fitBottom = fmBottom;

                    while ((*breakOpp)[breakOppIndex] != -1
                            && (*breakOpp)[breakOppIndex] < j - paraStart + 1) {
                        breakOppIndex++;
                    }
                    Boolean isLineBreak = breakOppIndex < breakOpp->GetLength() &&
                            (*breakOpp)[breakOppIndex] == j - paraStart + 1;

                    if (isLineBreak) {
                        okWidth = fitWidthGraphing;
                        ok = j + 1;

                        if (fitTop < okTop)
                            okTop = fitTop;
                        if (fitAscent < okAscent)
                            okAscent = fitAscent;
                        if (fitDescent > okDescent)
                            okDescent = fitDescent;
                        if (fitBottom > okBottom)
                            okBottom = fitBottom;
                    }
                } else {
                    Boolean moreChars;
                    Int32 endPos;
                    Int32 above, below, top, bottom;
                    Float currentTextWidth;

                    if (ok != here) {
                        endPos = ok;
                        above = okAscent;
                        below = okDescent;
                        top = okTop;
                        bottom = okBottom;
                        currentTextWidth = okWidth;
                        moreChars = (j + 1 < spanEnd);
                    } else if (fit != here) {
                        endPos = fit;
                        above = fitAscent;
                        below = fitDescent;
                        top = fitTop;
                        bottom = fitBottom;
                        currentTextWidth = fitWidth;
                        moreChars = (j + 1 < spanEnd);
                    } else {
                        // must make progress, so take next character
                        endPos = here + 1;
                        // but to deal properly with clusters
                        // take all zero width characters following that
                        while (endPos < spanEnd && (*widths)[endPos - paraStart] == 0) {
                            endPos++;
                        }
                        above = fmAscent;
                        below = fmDescent;
                        top = fmTop;
                        bottom = fmBottom;
                        currentTextWidth = (*widths)[here - paraStart];
                        moreChars = (endPos < spanEnd);
                    }

                    v = Out(source, here, endPos,
                            above, below, top, bottom,
                            v, spacingmult, spacingadd, chooseHt, chooseHtv, fm, hasTabOrEmoji,
                            needMultiply, chdirs, dir, easy, bufEnd, includepad, trackpad,
                            chs, widths, paraStart, ellipsize, ellipsizedWidth,
                            currentTextWidth, paint, moreChars);

                    here = endPos;
                    j = here - 1; // restart j-span loop from here, compensating for the j++
                    ok = fit = here;
                    w = 0;
                    fitWidthGraphing = w;
                    fitAscent = fitDescent = fitTop = fitBottom = 0;
                    okAscent = okDescent = okTop = okBottom = 0;

                    if (--firstWidthLineLimit <= 0) {
                        width = restWidth;
                    }

                    if (here < spanStart) {
                        // The text was cut before the beginning of the current span range.
                        // Exit the span loop, and get spanStart to start over from here.
                        measured->SetPos(here);
                        spanEnd = here;
                        break;
                    }

                    if (mLineCount >= mMaximumVisibleLineCount) {
                        return;
                    }
                }
            }
        }

        if (paraEnd != here && mLineCount < mMaximumVisibleLineCount) {
            if ((fitTop | fitBottom | fitDescent | fitAscent) == 0) {
                Int32 spacing;
                paint->GetFontMetricsInt(fm, &spacing);

                fm->GetTop(&fitTop);
                fm->GetBottom(&fitBottom);
                fm->GetAscent(&fitAscent);
                fm->GetDescent(&fitDescent);
            }

            // Log.e("text", "output rest " + here + " to " + end);

            v = Out(source,
                    here, paraEnd, fitAscent, fitDescent,
                    fitTop, fitBottom,
                    v,
                    spacingmult, spacingadd, chooseHt,
                    chooseHtv, fm, hasTabOrEmoji,
                    needMultiply, chdirs, dir, easy, bufEnd,
                    includepad, trackpad, chs,
                    widths, paraStart, ellipsize,
                    ellipsizedWidth, w, paint, paraEnd != bufEnd);
        }

        paraStart = paraEnd;

        if (paraEnd == bufEnd)
            break;
    }

    Char32 charAt = 0;
    source->GetCharAt(bufEnd - 1, &charAt);
    if ((bufEnd == bufStart || charAt == CHAR_NEW_LINE) &&
            mLineCount < mMaximumVisibleLineCount) {
        // Log.e("text", "output last " + bufEnd);

        Int32 spacing;
        paint->GetFontMetricsInt(fm, &spacing);

        Int32 ascent, descent, top, bottom;
        fm->GetAscent(&ascent);
        fm->GetDescent(&descent);
        fm->GetTop(&top);
        fm->GetBottom(&bottom);

        v = Out(source,
                bufEnd, bufEnd, ascent, descent,
                top, bottom,
                v,
                spacingmult, spacingadd, NULL,
                NULL, fm, FALSE,
                needMultiply, measured->mLevels, measured->mDir, measured->mEasy, bufEnd,
                includepad, trackpad, NULL,
                NULL, bufStart, ellipsize,
                ellipsizedWidth, 0, paint, FALSE);
    }
}

// Override the base class so we can directly access our members,
// rather than relying on member functions.
// The logic mirrors that of Layout.getLineForVertical
// FIXME: It may be faster to do a linear search for layouts without many lines.
//@Override
Int32 StaticLayout::GetLineForVertical(
    /* [in] */ Int32 vertical)
{
    Int32 high = mLineCount;
    Int32 low = -1;
    Int32 guess;
    AutoPtr< ArrayOf<Int32> > lines = mLines;
    while (high - low > 1) {
        guess = (high + low) >> 1;
        if ((*lines)[mColumns * guess + TOP] > vertical){
            high = guess;
        } else {
            low = guess;
        }
    }
    if (low < 0) {
        return 0;
    } else {
        return low;
    }
}

//@Override
Int32 StaticLayout::GetLineCount()
{
    return mLineCount;
}

//@Override
Int32 StaticLayout::GetLineTop(
    /* [in] */ Int32 line)
{
    assert(mLines != NULL);
    Int32 top = (*mLines)[mColumns * line + TOP];
    if (mMaximumVisibleLineCount > 0 && line >= mMaximumVisibleLineCount &&
            line != mLineCount) {
        top += GetBottomPadding();
    }
    return top;
}

//@Override
Int32 StaticLayout::GetLineDescent(
    /* [in] */ Int32 line)
{
    Int32 descent = (*mLines)[mColumns * line + DESCENT];
    if (mMaximumVisibleLineCount > 0 && line >= mMaximumVisibleLineCount - 1 && // -1 intended
            line != mLineCount) {
        descent += GetBottomPadding();
    }
    return descent;
}

//@Override
Int32 StaticLayout::GetLineStart(
    /* [in] */ Int32 line)
{
    return (*mLines)[mColumns * line + START] & START_MASK;
}

//@Override
Int32 StaticLayout::GetParagraphDirection(
    /* [in] */ Int32 line)
{
    return (*mLines)[mColumns * line + DIR] >> DIR_SHIFT;
}

//@Override
Boolean StaticLayout::GetLineContainsTab(
    /* [in] */ Int32 line)
{
    return ((*mLines)[mColumns * line + TAB] & TAB_MASK) != 0;
}

//@Override
AutoPtr<ILayoutDirections> StaticLayout::GetLineDirections(
    /* [in] */ Int32 line)
{
    assert(line >= 0);
    AutoPtr<ILayoutDirections> ld;
    if (mLineDirections != NULL && line >= 0 && line < mLineDirections->GetLength()) {
        ld = (*mLineDirections)[line];
    }
    return ld;
}

//@Override
Int32 StaticLayout::GetTopPadding()
{
    return mTopPadding;
}

//@Override
Int32 StaticLayout::GetBottomPadding()
{
    return mBottomPadding;
}

//@Override
Int32 StaticLayout::GetEllipsisCount(
    /* [in] */ Int32 line)
{
    if (mColumns < COLUMNS_ELLIPSIZE) {
        return 0;
    }

    return (*mLines)[mColumns * line + ELLIPSIS_COUNT];
}

//@Override
Int32 StaticLayout::GetEllipsisStart(
    /* [in] */ Int32 line)
{
    if (mColumns < COLUMNS_ELLIPSIZE) {
        return 0;
    }

    return (*mLines)[mColumns * line + ELLIPSIS_START];
}

//@Override
Int32 StaticLayout::GetEllipsizedWidth()
{
    return mEllipsizedWidth;
}

void StaticLayout::Prepare()
{
    mMeasured = MeasuredText::Obtain();
}

void StaticLayout::Finish()
{
    mMeasured = MeasuredText::Recycle(mMeasured);
}

ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    assert(source != NULL);
    Int32 length = 0;
    source->GetLength(&length);
    return Init(source, 0, length, paint, width, align,
             spacingmult, spacingadd, includepad);
}

/**
 * @hide
 */
ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    assert(source != NULL);
    Int32 length = 0;
    source->GetLength(&length);
    return Init(source, 0, length, paint, width, align, textDir,
                spacingmult, spacingadd, includepad);
}

ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return Init(source, bufstart, bufend, paint, outerwidth, align,
             spacingmult, spacingadd, includepad,
             TextUtilsTruncateAt_NONE, 0);
}

/**
 * @hide
 */
ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return Init(source, bufstart, bufend, paint, outerwidth, align, textDir,
                spacingmult, spacingadd, includepad,
                TextUtilsTruncateAt_NONE, 0, Elastos::Core::Math::INT32_MAX_VALUE);
}

ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return Init(source, bufstart, bufend, paint, outerwidth, align,
                TextDirectionHeuristics::FIRSTSTRONG_LTR,
                spacingmult, spacingadd, includepad, ellipsize,
                ellipsizedWidth, Elastos::Core::Math::INT32_MAX_VALUE);
}

/**
 * @hide
 */
ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth,
    /* [in] */ Int32 maxLines)
{
    assert(source != NULL);
    AutoPtr<ICharSequence> tempCS;
    if (source->Probe(EIID_ISpanned)) {
        tempCS = (ICharSequence*)(new SpannedEllipsizer(source))->Probe(EIID_ICharSequence);
    } else {
        tempCS = (ICharSequence*)(new Ellipsizer(source));
    }
    AutoPtr<ICharSequence> initCS;
    if (ellipsize == TextUtilsTruncateAt_NONE) {
        initCS = source;
    } else {
        initCS = tempCS;
    }
    Layout::Init(initCS,
          paint, outerwidth, align, textDir, spacingmult, spacingadd);

    /*
     * This is annoying, but we can't refer to the layout until
     * superclass construction is finished, and the superclass
     * constructor wants the reference to the display text.
     *
     * This will break if the superclass constructor ever actually
     * cares about the content instead of just holding the reference.
     */
    if (ellipsize != TextUtilsTruncateAt_NONE) {
        AutoPtr<Ellipsizer> e = (Ellipsizer*)GetText().Get();
        e->mLayout = this;
        e->mWidth = ellipsizedWidth;
        e->mMethod = ellipsize;
        mEllipsizedWidth = ellipsizedWidth;

        mColumns = COLUMNS_ELLIPSIZE;
    } else {
        mColumns = COLUMNS_NORMAL;
        mEllipsizedWidth = outerwidth;
    }
    mLines = ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    mLineDirections = ArrayOf<ILayoutDirections*>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    mMaximumVisibleLineCount = maxLines;

    mMeasured = MeasuredText::Obtain();

    Generate(source, bufstart, bufend, paint, outerwidth, textDir, spacingmult,
             spacingadd, includepad, includepad, ellipsizedWidth,
             ellipsize);

    mMeasured = MeasuredText::Recycle(mMeasured);
    mFontMetricsInt = NULL;
    return NOERROR;
}

/* package */
ECode StaticLayout::Init(
    /* [in] */ ICharSequence* text)
{
    Layout::Init(text, NULL, 0, ALIGN_NONE, 0.0f, 0.0f);

    mColumns = COLUMNS_ELLIPSIZE;
    mLines = ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    mLineDirections = ArrayOf<ILayoutDirections*>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    // FIXME This is never recycled
    mMeasured = MeasuredText::Obtain();
    return NOERROR;
}

Int32 StaticLayout::Out(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 above,
    /* [in] */ Int32 below,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 v,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ ArrayOf<IInterface*>* chooseHt,//ILineHeightSpan
    /* [in] */ ArrayOf<Int32>* chooseHtv,
    /* [in] */ IPaintFontMetricsInt* fm,
    /* [in] */ Boolean hasTabOrEmoji,
    /* [in] */ Boolean needMultiply,
    /* [in] */ ArrayOf<Byte>* chdirs,
    /* [in] */ Int32 dir,
    /* [in] */ Boolean easy,
    /* [in] */ Int32 bufEnd,
    /* [in] */ Boolean includePad,
    /* [in] */ Boolean trackPad,
    /* [in] */ ArrayOf<Char32>* chs,
    /* [in] */ ArrayOf<Float>* widths,
    /* [in] */ Int32 widthStart,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Float ellipsisWidth,
    /* [in] */ Float textWidth,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Boolean moreChars)
{
    Int32 j = mLineCount;
    Int32 off = j * mColumns;
    Int32 want = off + mColumns + TOP;
    AutoPtr< ArrayOf<Int32> > lines = mLines;

    if (want >= lines->GetLength()) {
        AutoPtr< ArrayOf<ILayoutDirections*> > grow2 = ArrayUtils::NewUnpaddedArray(GrowingArrayUtils::GrowSize(want));
        grow2->Copy(mLineDirections);
        mLineDirections = grow2;

        AutoPtr< ArrayOf<Int32> > grow = ArrayOf<Int32>::Alloc(grow2->GetLength());
        grow->Copy(lines);
        mLines = grow;
        lines = grow;
    }

    if (chooseHt != NULL) {
        fm->SetAscent(above);
        fm->SetDescent(below);
        fm->SetTop(top);
        fm->SetBottom(bottom);

        for (Int32 i = 0; i < chooseHt->GetLength(); i++) {
            ILineHeightSpan* obj = (ILineHeightSpan*)((*chooseHt)[i]->Probe(EIID_ILineHeightSpan));
            assert(obj != NULL);
            ILineHeightSpanWithDensity* lhsd = ILineHeightSpanWithDensity::Probe(obj);
            if (lhsd) {
                lhsd->ChooseHeight(text, start, end, (*chooseHtv)[i], v, fm, paint);
            } else {
                obj->ChooseHeight(text, start, end, (*chooseHtv)[i], v, fm);
            }
        }

        fm->GetAscent(&above);
        fm->GetDescent(&below);
        fm->GetTop(&top);
        fm->GetBottom(&bottom);
    }

    Boolean firstLine = (j == 0);
    Boolean currentLineIsTheLastVisibleOne = (j + 1 == mMaximumVisibleLineCount);
    Boolean lastLine = currentLineIsTheLastVisibleOne || (end == bufEnd);
    if (firstLine) {
        if (trackPad) {
            mTopPadding = top - above;
        }

        if (includePad) {
            above = top;
        }
    }

    Int32 extra;

    if (lastLine) {
        if (trackPad) {
            mBottomPadding = bottom - below;
        }

        if (includePad) {
            below = bottom;
        }
    }

    if (needMultiply && !lastLine) {
        Double ex = (below - above) * (spacingmult - 1) + spacingadd;
        if (ex >= 0) {
            extra = (Int32)(ex + EXTRA_ROUNDING);
        } else {
            extra = -(Int32)(-ex + EXTRA_ROUNDING);
        }
    } else {
        extra = 0;
    }

    (*lines)[off + START] = start;
    (*lines)[off + TOP] = v;
    (*lines)[off + DESCENT] = below + extra;

    v += (below - above) + extra;
    (*lines)[off + mColumns + START] = end;
    (*lines)[off + mColumns + TOP] = v;

    if (hasTabOrEmoji)
        (*lines)[off + TAB] |= TAB_MASK;

    (*lines)[off + DIR] |= dir << DIR_SHIFT;

    // easy means all chars < the first RTL, so no emoji, no nothing
    // XXX a run with no text or all spaces is easy but might be an empty
    // RTL paragraph.  Make sure easy is false if this is the case.
    AutoPtr<ILayoutDirections> linedirs;
    if (easy) {
        linedirs = DIRS_ALL_LEFT_TO_RIGHT;
    } else {
        linedirs = AndroidBidi::Directions(dir, chdirs, start - widthStart, chs,
                start - widthStart, end - start);
    }
    mLineDirections->Set(j, linedirs);

    if (ellipsize != TextUtilsTruncateAt_NONE) {
        // If there is only one line, then do any type of ellipsis except when it is MARQUEE
        // if there are multiple lines, just allow END ellipsis on the last line
        Boolean forceEllipsis = moreChars && (mLineCount + 1 == mMaximumVisibleLineCount);

        Boolean doEllipsis =
                    (((mMaximumVisibleLineCount == 1 && moreChars) || (firstLine && !moreChars)) &&
                            ellipsize != TextUtilsTruncateAt_MARQUEE) ||
                    (!firstLine && (currentLineIsTheLastVisibleOne || !moreChars) &&
                            ellipsize == TextUtilsTruncateAt_END);
        if (doEllipsis) {
            CalculateEllipsis(start, end, widths, widthStart,
                    ellipsisWidth, ellipsize, j,
                    textWidth, paint, forceEllipsis);
        }
    }

    mLineCount++;
    return v;
}

void StaticLayout::CalculateEllipsis(
    /* [in] */ Int32 lineStart,
    /* [in] */ Int32 lineEnd,
    /* [in] */ ArrayOf<Float>* widths,
    /* [in] */ Int32 widthStart,
    /* [in] */ Float avail,
    /* [in] */ TextUtilsTruncateAt where,
    /* [in] */ Int32 line,
    /* [in] */ Float textWidth,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Boolean forceEllipsis)
{
    assert(paint != NULL);

    if (textWidth <= avail && !forceEllipsis) {
        // Everything fits!
        (*mLines)[mColumns * line + ELLIPSIS_START] = 0;
        (*mLines)[mColumns * line + ELLIPSIS_COUNT] = 0;
        return;
    }

    Float ellipsisWidth = 0.0f;
    AutoPtr< ArrayOf<Char32> > text = ArrayOf<Char32>::Alloc(1);
    (*text)[0] = (where == TextUtilsTruncateAt_END_SMALL) ?
            ELLIPSIS_TWO_DOTS[0] : ELLIPSIS_NORMAL[0];
    paint->MeasureText(*text, 0, 1, &ellipsisWidth);
    Int32 ellipsisStart = 0;
    Int32 ellipsisCount = 0;
    Int32 len = lineEnd - lineStart;

    // We only support start ellipsis on a single line
    if (where == TextUtilsTruncateAt_START) {
        if (mMaximumVisibleLineCount == 1) {
            Float sum = 0;
            Int32 i;

            for (i = len; i >= 0; i--) {
                assert(widths != NULL);
                Float w = (*widths)[i - 1 + lineStart - widthStart];

                if (w + sum + ellipsisWidth > avail) {
                    break;
                }

                sum += w;
            }

            ellipsisStart = 0;
            ellipsisCount = i;
        } else {
//            if (Log.isLoggable(TAG, Log.WARN)) {
//                Log.w(TAG, "Start Ellipsis only supported with one line");
//            }
        }
    } else if (where == TextUtilsTruncateAt_END || where == TextUtilsTruncateAt_MARQUEE ||
            where == TextUtilsTruncateAt_END_SMALL) {
        Float sum = 0;
        Int32 i;

        for (i = 0; i < len; i++) {
            Float w = (*widths)[i + lineStart - widthStart];

            if (w + sum + ellipsisWidth > avail) {
                break;
            }

            sum += w;
        }

        ellipsisStart = i;
        ellipsisCount = len - i;
        if (forceEllipsis && ellipsisCount == 0 && len > 0) {
            ellipsisStart = len - 1;
            ellipsisCount = 1;
        }
    } else {
        // where = TextUtilsTruncateAt_MIDDLE We only support middle ellipsis on a single line
        if (mMaximumVisibleLineCount == 1) {
            Float lsum = 0, rsum = 0;
            Int32 left = 0, right = len;

            Float ravail = (avail - ellipsisWidth) / 2;
            for (right = len; right >= 0; right--) {
                assert(widths != NULL);
                Float w = (*widths)[right - 1 + lineStart - widthStart];

                if (w + rsum > ravail) {
                    break;
                }

                rsum += w;
            }

            Float lavail = avail - ellipsisWidth - rsum;
            for (left = 0; left < right; left++) {
                Float w = (*widths)[left + lineStart - widthStart];

                if (w + lsum > lavail) {
                    break;
                }

                lsum += w;
            }

            ellipsisStart = left;
            ellipsisCount = right - left;
        } else {
//            if (Log.isLoggable(TAG, Log.WARN)) {
//                Log.w(TAG, "Middle Ellipsis only supported with one line");
//            }
        }
    }

    (*mLines)[mColumns * line + ELLIPSIS_START] = ellipsisStart;
    (*mLines)[mColumns * line + ELLIPSIS_COUNT] = ellipsisCount;
}

AutoPtr<ArrayOf<Int32> > StaticLayout::nLineBreakOpportunities(
    /* [in] */ const String& locale,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 length,
    /* [in] */ ArrayOf<Int32>* recycle)
{

}


} // namespace Text
} // namepsace Droid
} // namespace Elastos
