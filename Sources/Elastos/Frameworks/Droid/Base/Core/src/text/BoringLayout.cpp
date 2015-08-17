
#include "text/TextDirectionHeuristics.h"
#include "text/CBoringLayoutMetrics.h"
#include "text/BoringLayout.h"
#include "text/CBoringLayout.h"
#include "text/TextUtils.h"
#include "text/TextLine.h"
#include <elastos/core/Math.h>

using Elastos::Core::CString;
using Elastos::Core::ECLSID_CStringWrapper;
using Elastos::Droid::Text::Style::EIID_IParagraphStyle;

namespace Elastos {
namespace Droid {
namespace Text {

const Char32 BoringLayout::FIRST_RIGHT_TO_LEFT = 0x0590;//'\u0590';

PInterface BoringLayout::Probe(
    /* [in] */ REIID riid)
{
    assert(0);
    return NOERROR;
}

AutoPtr<IBoringLayout> BoringLayout::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
{
    AutoPtr<IBoringLayout> layout;
    ASSERT_SUCCEEDED(CBoringLayout::New(source, paint, outerwidth, align,
        spacingmult, spacingadd, metrics, includepad, (IBoringLayout**)&layout));

    return layout;
}

AutoPtr<IBoringLayout> BoringLayout::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    AutoPtr<IBoringLayout> layout;
    ASSERT_SUCCEEDED(CBoringLayout::New(source, paint, outerwidth, align,
        spacingmult, spacingadd, metrics, includepad, ellipsize,
        ellipsizedWidth, (IBoringLayout**)&layout));

    return layout;
}

/**
 * Returns a BoringLayout for the specified text, potentially reusing
 * this one if it is already suitable.  The caller must make sure that
 * no one is still using this Layout.
 */
ECode BoringLayout::ReplaceOrMake(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [out] */ IBoringLayout** layout)
{
    assert(layout != NULL);

    ReplaceWith(source, paint, outerwidth, align, spacingmult,
        spacingadd);

    mEllipsizedWidth = outerwidth;
    mEllipsizedStart = 0;
    mEllipsizedCount = 0;

    Init(source, paint, outerwidth, align, spacingmult, spacingadd,
        metrics, includepad, TRUE);
    *layout = THIS_PROBE(IBoringLayout);
    REFCOUNT_ADD(*layout);
    return NOERROR;
}

/**
 * Returns a BoringLayout for the specified text, potentially reusing
 * this one if it is already suitable.  The caller must make sure that
 * no one is still using this Layout.
 */
CARAPI BoringLayout::ReplaceOrMake(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth,
    /* [out] */ IBoringLayout** layout)
{
    assert(layout != NULL);

    Boolean trust = FALSE;

    if (ellipsize == TextUtilsTruncateAt_NONE || ellipsize == TextUtilsTruncateAt_MARQUEE) {
        ReplaceWith(source, paint, outerwidth, align, spacingmult,
                    spacingadd);

        mEllipsizedWidth = outerwidth;
        mEllipsizedStart = 0;
        mEllipsizedCount = 0;
        trust = TRUE;
    } else {
       ReplaceWith(TextUtils::Ellipsize(source, paint, ellipsizedWidth,
                                      ellipsize, TRUE, (ITextUtilsEllipsizeCallback*)this->Probe(EIID_ITextUtilsEllipsizeCallback)),
                   paint, outerwidth, align, spacingmult,
                   spacingadd);

        mEllipsizedWidth = ellipsizedWidth;
        trust = FALSE;
    }

    Init(GetText(), paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, trust);

    *layout = THIS_PROBE(IBoringLayout);
    REFCOUNT_ADD(*layout);
    return NOERROR;
}

BoringLayout::BoringLayout()
     : mTopPadding(0)
     , mBottomPadding(0)
     , mMax(0)
     , mEllipsizedWidth(0)
     , mEllipsizedStart(0)
     , mEllipsizedCount(0)
{}

BoringLayout::BoringLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
    : Layout(source, paint, outerwidth, align, spacingmult, spacingadd)
    , mTopPadding(0)
    , mBottomPadding(0)
    , mMax(0)
    , mEllipsizedWidth(outerwidth)
    , mEllipsizedStart(0)
    , mEllipsizedCount(0)
{
    Init(source, paint, outerwidth, align, spacingmult, spacingadd,
        metrics, includepad, TRUE);
}

BoringLayout::BoringLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
    : mTopPadding(0)
    , mBottomPadding(0)
    , mMax(0)
    , mEllipsizedWidth(0)
    , mEllipsizedStart(0)
    , mEllipsizedCount(0)
{
    /*
     * It is silly to have to call super() and then replaceWith(),
     * but we can't use "this" for the callback until the call to
     * super() finishes.
     */
    Layout::Init(source, paint, outerwidth, align, spacingmult, spacingadd);

    Boolean trust = FALSE;

    if (ellipsize == TextUtilsTruncateAt_NONE || ellipsize == TextUtilsTruncateAt_MARQUEE) {
        mEllipsizedWidth = outerwidth;
        mEllipsizedStart = 0;
        mEllipsizedCount = 0;
        trust = TRUE;
    } else {
        assert(0);
        ReplaceWith(TextUtils::Ellipsize(source, paint, ellipsizedWidth,
                     ellipsize, TRUE, (ITextUtilsEllipsizeCallback*)this->Probe(EIID_ITextUtilsEllipsizeCallback)),
                   paint, outerwidth, align, spacingmult,
                   spacingadd);

        mEllipsizedWidth = ellipsizedWidth;
        trust = FALSE;
    }

    Init(GetText(), paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, trust);
}

void BoringLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ Boolean trustWidth)
{
    assert(source != NULL);
    Int32 spacing;

    mDirect = NULL;

    IObject* obj = IObject::Probe(source);
    if (obj != NULL) {
        ClassID clsid;
        obj->GetClassID(&clsid);

        if (clsid == ECLSID_CStringWrapper && align == Elastos::Droid::Text::ALIGN_NORMAL) {
            source->ToString(&mDirect);
        }
    }

    mPaint = paint;

    Int32 mtbottom, mttop, mtdescent, mtascent, mtwidth;
    metrics->GetBottom(&mtbottom);
    metrics->GetTop(&mttop);
    metrics->GetDescent(&mtdescent);
    metrics->GetAscent(&mtascent);
    metrics->GetWidth(&mtwidth);
    if (includepad) {
        spacing = mtbottom - mttop;
    } else {
        spacing = mtdescent - mtascent;
    }

    if (spacingmult != 1 || spacingadd != 0) {
        spacing = (Int32)(spacing * spacingmult + spacingadd + 0.5f);
    }

    mBottom = spacing;

    if (includepad) {
        mDesc = spacing + mttop;
    } else {
        mDesc = spacing + mtascent;
    }

    if (trustWidth) {
        mMax = mtwidth;
    } else {
        /*
         * If we have ellipsized, we have to actually calculate the
         * width because the width that was passed in was for the
         * full text, not the ellipsized form.
         */
        AutoPtr<TextLine> line = TextLine::Obtain();
        Int32 length = 0;
        source->GetLength(&length);
        line->Set(paint, source, 0, length, ILayout::DIR_LEFT_TO_RIGHT,
                Layout::DIRS_ALL_LEFT_TO_RIGHT, FALSE, NULL);
        mMax = (Int32) Elastos::Core::Math::Ceil(line->Metrics(NULL));
        TextLine::Recycle(line);
    }

    if (includepad) {
        mTopPadding = mttop - mtascent;
        mBottomPadding = mtbottom - mtdescent;
    }
}

ECode BoringLayout::_Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
{
    Layout::Init(source, paint, outerwidth, align, spacingmult, spacingadd);
    mEllipsizedWidth = outerwidth;
    mEllipsizedStart = 0;
    mEllipsizedCount = 0;
    Init(source, paint, outerwidth, align, spacingmult, spacingadd,
        metrics, includepad, TRUE);

    return NOERROR;
}

ECode BoringLayout::_Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    /*
     * It is silly to have to call super() and then replaceWith(),
     * but we can't use "this" for the callback until the call to
     * super() finishes.
     */
    Layout::Init(source, paint, outerwidth, align, spacingmult, spacingadd);

    Boolean trust = FALSE;

    if (ellipsize == TextUtilsTruncateAt_NONE || ellipsize == TextUtilsTruncateAt_MARQUEE) {
        mEllipsizedWidth = outerwidth;
        mEllipsizedStart = 0;
        mEllipsizedCount = 0;
        trust = TRUE;
    } else {
        ReplaceWith(TextUtils::Ellipsize(source, paint, ellipsizedWidth,
                     ellipsize, TRUE, (ITextUtilsEllipsizeCallback*)this->Probe(EIID_ITextUtilsEllipsizeCallback)),
                   paint, outerwidth, align, spacingmult,
                   spacingadd);

        mEllipsizedWidth = ellipsizedWidth;
        trust = FALSE;
    }

    Init(GetText(), paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, trust);

    return NOERROR;
}

/**
 * Returns null if not boring; the width, ascent, and descent if boring.
 */
AutoPtr<IBoringLayoutMetrics> BoringLayout::IsBoring(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint)
{
    return IsBoring(text, paint, TextDirectionHeuristics::FIRSTSTRONG_LTR, NULL);
}

/**
 * Returns null if not boring; the width, ascent, and descent if boring.
 * @hide
 */
AutoPtr<IBoringLayoutMetrics> BoringLayout::IsBoring(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextDirectionHeuristic* textDir)
{
    return IsBoring(text, paint, textDir, NULL);
}

/**
 * Returns null if not boring; the width, ascent, and descent in the
 * provided Metrics object (or a new one if the provided one was null)
 * if boring.
 */
AutoPtr<IBoringLayoutMetrics> BoringLayout::IsBoring(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ IBoringLayoutMetrics* metrics)
{
    return IsBoring(text, paint, TextDirectionHeuristics::FIRSTSTRONG_LTR, metrics);
}

/**
 * Returns null if not boring; the width, ascent, and descent in the
 * provided Metrics object (or a new one if the provided one was null)
 * if boring.
 * @hide
 */
AutoPtr<IBoringLayoutMetrics> BoringLayout::IsBoring(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ IBoringLayoutMetrics* metrics)
{
    assert(text != NULL);
    //char[] temp = TextUtils.obtain(500);
    AutoPtr< ArrayOf<Char32> > temp = TextUtils::Obtain(4 * 500);
    Int32 length;
    text->GetLength(&length);
    Boolean boring = TRUE;

    for (Int32 i = 0; i < length; i += 500) {
        Int32 j = i + 500;

        if (j > length)
            j = length;

        TextUtils::GetChars(text, i, j, temp, 0);

        Int32 n = j - i;

        for (Int32 a = 0; a < n; a++) {
            Char32 c = (*temp)[a];

            if (c == '\n' || c == '\t' || c >= FIRST_RIGHT_TO_LEFT) {
                boring = FALSE;
                goto outer;
            }
        }

        Boolean bFlag = FALSE;
        if (textDir != NULL && (textDir->IsRtl(temp, 0, n, &bFlag), bFlag)) {
           boring = FALSE;
           goto outer;
        }
    }

outer:
    TextUtils::Recycle(temp);

    if (boring && ISpanned::Probe(text) != NULL) {
        ISpanned* sp = ISpanned::Probe(text);
        AutoPtr<ArrayOf<IInterface*> > styles;
        sp->GetSpans(0, length, EIID_IParagraphStyle, (ArrayOf<IInterface*>**)&styles);
        if (styles && styles->GetLength() > 0) {
            boring = FALSE;
        }
    }

    if (boring) {
        AutoPtr<IBoringLayoutMetrics> fm = metrics;
        if (fm == NULL) {
            CBoringLayoutMetrics::New((IBoringLayoutMetrics**)&fm);
        }
        AutoPtr<TextLine> line = TextLine::Obtain();
        assert(line);
        line->Set(paint, text, 0, length, ILayout::DIR_LEFT_TO_RIGHT,
                Layout::DIRS_ALL_LEFT_TO_RIGHT, FALSE, NULL);
        Int32 wid = (Int32) Elastos::Core::Math::Ceil(line->Metrics(fm));
        assert(fm != NULL);
        fm->SetWidth(wid);
        TextLine::Recycle(line);
        return fm;
    } else {
        return NULL;
    }
}

//@Override
Int32 BoringLayout::GetHeight()
{
    return mBottom;
}

//@Override
Int32 BoringLayout::GetLineCount()
{
    return 1;
}

//@Override
Int32 BoringLayout::GetLineTop(
    /* [in] */ Int32 line)
{
    if (line == 0)
        return 0;
    else
        return mBottom;
}

//@Override
Int32 BoringLayout::GetLineDescent(
    /* [in] */ Int32 line)
{
    return mDesc;
}

//@Override
Int32 BoringLayout::GetLineStart(
    /* [in] */ Int32 line)
{
    if (line == 0) {
        return 0;
    } else {
        Int32 len;
        GetText()->GetLength(&len);
        return len;
    }
}

//@Override
Int32 BoringLayout::GetParagraphDirection(
    /* [in] */ Int32 line)
{
    return ILayout::DIR_LEFT_TO_RIGHT;
}

//@Override
Boolean BoringLayout::GetLineContainsTab(
    /* [in] */ Int32 line)
{
    return FALSE;
}

//@Override
Float BoringLayout::GetLineMax(
    /* [in] */ Int32 line)
{
    return mMax;
}

//@Override
AutoPtr<ILayoutDirections> BoringLayout::GetLineDirections(
    /* [in] */ Int32 line)
{
    return Layout::DIRS_ALL_LEFT_TO_RIGHT;
}

//@Override
Int32 BoringLayout::GetTopPadding()
{
    return mTopPadding;
}

//@Override
Int32 BoringLayout::GetBottomPadding()
{
    return mBottomPadding;
}

//@Override
Int32 BoringLayout::GetEllipsisCount(
    /* [in] */ Int32 line)
{
    return mEllipsizedCount;
}

//@Override
Int32 BoringLayout::GetEllipsisStart(
    /* [in] */ Int32 line)
{
    return mEllipsizedStart;
}

//@Override
Int32 BoringLayout::GetEllipsizedWidth()
{
    return mEllipsizedWidth;
}

// Override draw so it will be faster.
//@Override
ECode BoringLayout::Draw(
    /* [in] */ ICanvas* c,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightpaint,
    /* [in] */ Int32 cursorOffset)
{
    assert(c != NULL);

    if (!mDirect.IsNull() && highlight == NULL) {
        c->DrawText(mDirect, 0, mBottom - mDesc, mPaint);
    } else {
        Layout::Draw(c, highlight, highlightpaint, cursorOffset);
    }

    return NOERROR;
}

/**
 * Callback for the ellipsizer to report what region it ellipsized.
 */
ECode BoringLayout::Ellipsized(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    mEllipsizedStart = start;
    mEllipsizedCount = end - start;

    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
