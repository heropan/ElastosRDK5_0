
#include "elastos/droid/text/Layout.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/text/TextLine.h"
#include "elastos/droid/text/CTextPaint.h"
#include "elastos/droid/text/CSpannableString.h"
#include "elastos/droid/text/TextDirectionHeuristics.h"
#include "elastos/droid/text/CLayoutDirections.h"
#include "elastos/droid/text/MeasuredText.h"
#include "elastos/droid/text/AndroidBidi.h"
#include "elastos/droid/text/method/TextKeyListener.h"
#include "elastos/droid/text/method/CTextKeyListener.h"
//#include "emoji/CEmojiFactoryHelper.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/internal/utility/ArrayUtils.h"
#include "elastos/droid/internal/utility/GrowingArrayUtils.h"

#include <elastos/core/Math.h>

using Elastos::Core::CString;
using Elastos::Core::EIID_ICharSequence;
//using Elastos::Droid::Emoji::IEmojiFactoryHelper;
using Elastos::Droid::Graphics::CRect;
//using Elastos::Droid::Graphics::PathDirection;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Text::MeasuredText;
using Elastos::Droid::Text::AndroidBidi;
using Elastos::Droid::Text::Style::EIID_IParagraphStyle;
using Elastos::Droid::Text::Style::EIID_IAlignmentSpan;
using Elastos::Droid::Text::Style::EIID_ILeadingMarginSpan;
using Elastos::Droid::Text::Style::EIID_ILeadingMarginSpan2;
using Elastos::Droid::Text::Style::EIID_ITabStopSpan;
using Elastos::Droid::Text::Style::EIID_ILineBackgroundSpan;
using Elastos::Droid::Text::Style::EIID_IReplacementSpan;
using Elastos::Droid::Text::Style::ITabStopSpan;
using Elastos::Droid::Text::Style::IAlignmentSpan;
using Elastos::Droid::Text::Style::ILeadingMarginSpan;
using Elastos::Droid::Text::Style::ILeadingMarginSpan2;
using Elastos::Droid::Text::Method::TextKeyListener;
using Elastos::Droid::Text::Method::CTextKeyListener;
using Elastos::Droid::Text::Method::MetaKeyKeyListener;
using Elastos::Droid::Text::Method::IMetaKeyKeyListener;
using Elastos::Droid::Text::MeasuredText;
using Elastos::Droid::Text::AndroidBidi;

// {d219ea75-7801-4f4c-b079-588c9cacb88d}
extern "C" const InterfaceID EIID_Ellipsizer =
        { 0xd219ea75, 0x7801, 0x4f4c, { 0xb0, 0x79, 0x58, 0x8c, 0x9c, 0xac, 0xb8, 0x8d } };

namespace Elastos {
namespace Droid {
namespace Text {

Int32 Layout::MIN_EMOJI;
Int32 Layout::MAX_EMOJI;
Mutex Layout::sTempRectLock;


const Int32 Layout::TAB_INCREMENT;
const Char32 Layout::ELLIPSIS_NORMAL[] = { 0x2026 }; // this is "..."
const Char32 Layout::ELLIPSIS_TWO_DOTS[] = { 0x2025 }; // this is ".."

////////////////////////////////////////////////////////////////////////////////
//          Layout::Ellipsizer
////////////////////////////////////////////////////////////////////////////////

Layout::Ellipsizer::Ellipsizer(
    /* [in] */ ICharSequence* s)
    : mText(s)
    , mLayout(NULL)
    , mWidth(0)
    , mMethod(TextUtilsTruncateAt_NONE)
{
}

ECode Layout::Ellipsizer::GetCharAt(
    /* [in] */ Int32 off,
    /* [out] */ Char32* ch)
{
    VALIDATE_NOT_NULL(ch);

    AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(1);
    GetChars(off, off + 1, buf, 0);
    *ch = (*buf)[0];

    TextUtils::Recycle(buf);
    return NOERROR;
}

ECode Layout::Ellipsizer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff)
{
    Int32 line1 = mLayout->GetLineForOffset(start);
    Int32 line2 = mLayout->GetLineForOffset(end);

    TextUtils::GetChars(mText, start, end, dest, destoff);

    for (Int32 i = line1; i <= line2; i++) {
        mLayout->Ellipsize(start, end, i, dest, destoff, mMethod);
    }

    return NOERROR;
}

ECode Layout::Ellipsizer::GetLength(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);

    return mText->GetLength(len);
}

ECode Layout::Ellipsizer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** subcsq)
{
    VALIDATE_NOT_NULL(subcsq);
    *subcsq = NULL;

    AutoPtr< ArrayOf<Char32> > s = ArrayOf<Char32>::Alloc(end - start);
    GetChars(start, end, s, 0);
    String str = String(*s);
    FAIL_RETURN(CString::New(str, subcsq))

    return NOERROR;
}

ECode Layout::Ellipsizer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    Int32 len;
    GetLength(&len);
    AutoPtr< ArrayOf<Char32> > s = ArrayOf<Char32>::Alloc(len);
    GetChars(0, len, s, 0);
    *str = String(*s);
    return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
//          Layout::TabStops
////////////////////////////////////////////////////////////////////////////////

Layout::TabStops::TabStops(
    /* [in] */ Int32 increment,
    /* [in] */ ArrayOf<IInterface*>* spans)
    : mNumStops(0)
    , mIncrement(0)
{
    Reset(increment, spans);
}

void Layout::TabStops::Reset(
    /* [in] */ Int32 increment,
    /* [in] */ ArrayOf<IInterface*>* spans)
{
    mIncrement = increment;

    Int32 ns = 0;
    if (spans != NULL) {
        AutoPtr< ArrayOf<Int32> > stops = mStops;

        Int32 length = spans->GetLength();
        for (Int32 i = 0; i < length; i++) {
            if ((*spans)[i] == NULL) {
                continue;
            }

            ITabStopSpan* tss = (ITabStopSpan*)((*spans)[i]->Probe(EIID_ITabStopSpan));
            if (tss) {
                if (stops == NULL) {
                    stops = ArrayOf<Int32>::Alloc(10);
                }
                else if (ns == stops->GetLength()) {
                    AutoPtr< ArrayOf<Int32> > nstops = ArrayOf<Int32>::Alloc(ns * 2);
                    for (Int32 j = 0; j < ns; ++j) {
                        (*nstops)[j] = (*stops)[j];
                    }
                    stops = nstops;
                }

                Int32 temp = 0;
                tss->GetTabStop(&temp);
                (*stops)[ns++] = temp;
            }
        }

        if (ns > 1) {
            ArrayUtils::Sort(stops, 0, ns);
        }

        if (stops != mStops) {
            mStops = stops;
        }
    }

    mNumStops = ns;
}

Float Layout::TabStops::NextTab(
    /* [in] */ Float h)
{
    Int32 ns = mNumStops;
    if (ns > 0) {
        AutoPtr< ArrayOf<Int32> > stops = mStops;
        for (Int32 i = 0; i < ns; ++i) {
            Int32 stop = (*stops)[i];
            if (stop > h) {
                return stop;
            }
        }
    }

    return NextDefaultStop(h, mIncrement);
}

Float Layout::TabStops::NextDefaultStop(
    /* [in] */ Float h,
    /* [in] */ Int32 inc)
{
    return ((Int32) ((h + inc) / inc)) * inc;
}

////////////////////////////////////////////////////////////////////////////////
//          Layout::SpannedEllipsizer
////////////////////////////////////////////////////////////////////////////////


Layout::SpannedEllipsizer::SpannedEllipsizer(
    /* [in] */ ICharSequence* display)
    : Ellipsizer(display)
{
    mSpanned = ISpanned::Probe(display);
}

ECode Layout::SpannedEllipsizer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff)
{
    return Ellipsizer::GetChars(start, end, dest, destoff);
}

ECode Layout::SpannedEllipsizer::GetLength(
    /* [out] */ Int32* len)
{
    return Ellipsizer::GetLength(len);
}

ECode Layout::SpannedEllipsizer::GetCharAt(
    /* [in] */ Int32 off,
    /* [out] */ Char32* ch)
{
    return Ellipsizer::GetCharAt(off, ch);
}

//ECode Layout::SpannedEllipsizer::ToString(
//    /* [in] */ Int32 off,
//    /* [out] */ Char32* ch)
//{
//    return Ellipsizer::ToString(off, ch);
//}

ECode Layout::SpannedEllipsizer::ToString(
    /* [out] */ String* str)
{
    return Ellipsizer::ToString(str);
}

ECode Layout::SpannedEllipsizer::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    return mSpanned->GetSpans(start, end, type, objs);
}

ECode Layout::SpannedEllipsizer::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    return mSpanned->GetSpanStart(tag, start);
}

ECode Layout::SpannedEllipsizer::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    return mSpanned->GetSpanEnd(tag, end);
}

ECode Layout::SpannedEllipsizer::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    return mSpanned->GetSpanFlags(tag, flags);
}

ECode Layout::SpannedEllipsizer::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    return mSpanned->NextSpanTransition(start, limit, type, offset);
}

ECode Layout::SpannedEllipsizer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** subcsq)
{
    VALIDATE_NOT_NULL(subcsq);
    *subcsq = NULL;

    AutoPtr< ArrayOf<Char32> > s = TextUtils::Obtain(end - start);
    GetChars(start, end, s, 0);

    AutoPtr<ICharSequence> seq;
    String str = String(*s, 0, s->GetLength());
    FAIL_RETURN(CString::New(str, (ICharSequence**)&seq))

    AutoPtr<ISpannableString> ss;
    FAIL_RETURN(CSpannableString::New(seq, (ISpannableString**)&ss));
    TextUtils::CopySpansFrom(mSpanned, start, end, EIID_IInterface, ss, 0);
    *subcsq = ICharSequence::Probe(ss);
    REFCOUNT_ADD(*subcsq);
    return NOERROR;
}

static AutoPtr< ArrayOf<IParagraphStyle*> > InitNO_PARA_SPANS()
{
    AutoPtr< ArrayOf<IParagraphStyle*> > tmp = ArrayOf<IParagraphStyle*>::Alloc(0);
    return tmp;
}

static AutoPtr<CRect> InitsTempRect()
{
    AutoPtr<CRect> tmp;
    CRect::NewByFriend((CRect**)&tmp);
    return tmp;
}

static AutoPtr<ILayoutDirections> InitDIRS_ALL_LEFT_TO_RIGHT()
{
    AutoPtr< ArrayOf<Int32> > val = ArrayOf<Int32>::Alloc(2);
    (*val)[0] = 0;
    (*val)[1] = ILayout::RUN_LENGTH_MASK;

    AutoPtr<CLayoutDirections> obj;
    CLayoutDirections::NewByFriend(val, (CLayoutDirections**)&obj);
    AutoPtr<ILayoutDirections> temp = (ILayoutDirections*)obj.Get();
    return temp;
}

static AutoPtr<ILayoutDirections> InitDIRS_ALL_RIGHT_TO_LEFT()
{
    AutoPtr< ArrayOf<Int32> > val = ArrayOf<Int32>::Alloc(2);
    (*val)[0] = 0;
    (*val)[1] = ILayout::RUN_LENGTH_MASK | ILayout::RUN_RTL_FLAG;

    AutoPtr<CLayoutDirections> obj;
    CLayoutDirections::NewByFriend(val, (CLayoutDirections**)&obj);
    AutoPtr<ILayoutDirections> temp = (ILayoutDirections*)obj.Get();
    return temp;
}

/*
static AutoPtr<IEmojiFactory> InitEMOJI()
{
    AutoPtr<IEmojiFactoryHelper> efh;
    CEmojiFactoryHelper::AcquireSingle((IEmojiFactoryHelper**)&efh);
    AutoPtr<IEmojiFactory> ef;
    efh->NewAvailableInstance((IEmojiFactory**)&ef);
    if (ef != NULL) {
        ef->GetMinimumAndroidPua(&(Layout::MIN_EMOJI));
        ef->GetMaximumAndroidPua(&(Layout::MAX_EMOJI));
    }
    else {
        Layout::MIN_EMOJI = -1;
        Layout::MAX_EMOJI = -1;
    }

    return ef;
}
*/

AutoPtr< ArrayOf<IParagraphStyle*> > Layout::NO_PARA_SPANS = InitNO_PARA_SPANS();
//AutoPtr<IEmojiFactory> Layout::EMOJI_FACTORY = InitEMOJI();
AutoPtr<IRect> Layout::sTempRect = InitsTempRect();
AutoPtr<ILayoutDirections> Layout::DIRS_ALL_LEFT_TO_RIGHT = InitDIRS_ALL_LEFT_TO_RIGHT();
AutoPtr<ILayoutDirections> Layout::DIRS_ALL_RIGHT_TO_LEFT = InitDIRS_ALL_RIGHT_TO_LEFT();

CAR_INTERFACE_IMPL(Layout, Object, ILayout)

Layout::Layout()
{
}

Layout::~Layout()
{
}

/**
 * Return how wide a layout must be in order to display the
 * specified text with one line per paragraph.
 */
Float Layout::GetDesiredWidth(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint)
{
    assert(source != NULL);
    Int32 len = 0;
    source->GetLength(&len);
    return GetDesiredWidth(source, 0, len, paint);
}

/**
 * Return how wide a layout must be in order to display the
 * specified text slice with one line per paragraph.
 */
Float Layout::GetDesiredWidth(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ITextPaint* paint)
{
    Float need = 0;

    Int32 next;
    for (Int32 i = start; i <= end; i = next) {
        next = TextUtils::IndexOf(source, '\n', i, end);

        if (next < 0)
            next = end;

        // note, omits trailing paragraph char
        Float w = MeasurePara(paint, source, i, next);

        if (w > need)
            need = w;

        next++;
    }

    return need;
}

Layout::Layout(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingMult,
    /* [in] */ Float spacingAdd)
{
    Init(text, paint, width, align, TextDirectionHeuristics::FIRSTSTRONG_LTR,
            spacingMult, spacingAdd);
}

Layout::Layout(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingMult,
    /* [in] */ Float spacingAdd)
    : mText(text)
    , mPaint(paint)
    , mWidth(width)
    , mAlignment(align)
    , mSpacingMult(spacingMult)
    , mSpacingAdd(spacingAdd)
    , mTextDir(textDir)
{
//    if (width < 0)
//        throw new IllegalArgumentException("Layout: " + width + " < 0");
    assert(width > 0);

    // Ensure paint doesn't have baselineShift set.
    // While normally we don't modify the paint the user passed in,
    // we were already doing this in Styled.drawUniformRun with both
    // baselineShift and bgColor.  We probably should reevaluate bgColor.
    if (paint != NULL) {
        paint->SetBgColor(0);
        paint->SetBaselineShift(0);
    }

    CTextPaint::New((ITextPaint**)&mWorkPaint);
    mSpannedText = text != NULL && text->Probe(EIID_ISpanned) != NULL;
}

ECode Layout::Init(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingMult,
    /* [in] */ Float spacingAdd)
{
    return Init(text, paint, width, align, TextDirectionHeuristics::FIRSTSTRONG_LTR,
            spacingMult, spacingAdd);
}

ECode Layout::Init(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingMult,
    /* [in] */ Float spacingAdd)
{
//    if (width < 0)
//        throw new IllegalArgumentException("Layout: " + width + " < 0");
    assert(width >= 0);

    mText = text;
    mPaint = paint;
    mWidth = width;
    mAlignment = align;
    mSpacingMult = spacingMult;
    mSpacingAdd = spacingAdd;
    mTextDir = textDir;

    // Ensure paint doesn't have baselineShift set.
    // While normally we don't modify the paint the user passed in,
    // we were already doing this in Styled.drawUniformRun with both
    // baselineShift and bgColor.  We probably should reevaluate bgColor.
    if (paint != NULL) {
        paint->SetBgColor(0);
        paint->SetBaselineShift(0);
    }

    CTextPaint::New((ITextPaint**)&mWorkPaint);
    mSpannedText = text != NULL && text->Probe(EIID_ISpanned) != NULL;
    return NOERROR;
}

/**
 * Replace constructor properties of this Layout with new ones.  Be careful.
 */
ECode Layout::ReplaceWith(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd)
{
//    if (width < 0) {
//        throw new IllegalArgumentException("Layout: " + width + " < 0");
//    }
    assert(width >= 0 && "Layout width < 0");

    mText = text;
    mPaint = paint;
    mWidth = width;
    mAlignment = align;
    mSpacingMult = spacingmult;
    mSpacingAdd = spacingadd;
    mSpannedText = text != NULL && text->Probe(EIID_ISpanned) != NULL;

    return NOERROR;
}

/**
 * Draw this Layout on the specified Canvas.
 */
ECode Layout::Draw(
    /* [in] */ ICanvas* c)
{
    return Draw(c, NULL, NULL, 0);
}

ECode Layout::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightPaint,
    /* [in] */ Int32 cursorOffsetVertical)
{
    Int64 lineRange = GetLineRangeForDraw(canvas);
    Int32 firstLine = TextUtils::UnpackRangeStartFromInt64(lineRange);
    Int32 lastLine = TextUtils::UnpackRangeEndFromInt64(lineRange);
    if (lastLine < 0) return NOERROR;

    DrawBackground(canvas, highlight, highlightPaint, cursorOffsetVertical,
            firstLine, lastLine);
    return DrawText(canvas, firstLine, lastLine);
}

/**
 * @hide
 */
ECode Layout::DrawText(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 firstLine,
    /* [in] */ Int32 lastLine)
{
    Int32 previousLineBottom = GetLineTop(firstLine);
    Int32 previousLineEnd = GetLineStart(firstLine);
    AutoPtr< ArrayOf<IInterface*> > spans = (ArrayOf<IInterface*>*)(NO_PARA_SPANS.Get());
    Int32 spanEnd = 0;
    AutoPtr<ITextPaint> paint = mPaint;
    AutoPtr<ICharSequence> buf = mText;

    LayoutAlignment paraAlign = mAlignment;
    AutoPtr<TabStops> tabStops;
    Boolean tabStopsIsInitialized = FALSE;

    AutoPtr<TextLine> tl = TextLine::Obtain();

    // Draw the lines, one at a time.
    // The baseline is the top of the following line minus the current line's descent.
    for (Int32 i = firstLine; i <= lastLine; i++) {
        Int32 start = previousLineEnd;
        previousLineEnd = GetLineStart(i + 1);
        Int32 end = GetLineVisibleEnd(i, start, previousLineEnd);

        Int32 ltop = previousLineBottom;
        Int32 lbottom = GetLineTop(i+1);
        previousLineBottom = lbottom;
        Int32 lbaseline = lbottom - GetLineDescent(i);

        Int32 dir = GetParagraphDirection(i);
        Int32 left = 0;
        Int32 right = mWidth;

        if (mSpannedText) {
            AutoPtr<ISpanned> sp = ISpanned::Probe(buf);
            Int32 textLength = 0;
            buf->GetLength(&textLength);
            Char32 ch;
            Boolean isFirstParaLine = (start == 0 || (buf->GetCharAt(start - 1, &ch), ch) == '\n');

            // New batch of paragraph styles, collect into spans array.
            // Compute the alignment, last alignment style wins.
            // Reset tabStops, we'll rebuild if we encounter a line with
            // tabs.
            // We expect paragraph spans to be relatively infrequent, use
            // spanEnd so that we can check less frequently.  Since
            // paragraph styles ought to apply to entire paragraphs, we can
            // just collect the ones present at the start of the paragraph.
            // If spanEnd is before the end of the paragraph, that's not
            // our problem.
            if (start >= spanEnd && (i == firstLine || isFirstParaLine)) {
                sp->NextSpanTransition(start, textLength, EIID_IParagraphStyle, &spanEnd);
                spans = GetParagraphSpans(sp, start, spanEnd, EIID_IParagraphStyle);

                paraAlign = mAlignment;
                for (Int32 n = spans->GetLength() - 1; n >= 0; n--) {
                    if ((*spans)[n] != NULL && (*spans)[n]->Probe(EIID_IAlignmentSpan)) {
                        IAlignmentSpan* as = (IAlignmentSpan*)((*spans)[n]->Probe(EIID_IAlignmentSpan));
                        as->GetAlignment(&paraAlign);
                        break;
                    }
                }

                tabStopsIsInitialized = FALSE;
            }

            // Draw all leading margin spans.  Adjust left or right according
            // to the paragraph direction of the line.
            Int32 length = spans.Get()->GetLength();
            // for (Int32 n = 0; n < length; n++) {
            //     if ((*spans)[n] != NULL && (*spans)[n]->Probe(EIID_ILeadingMarginSpan)) {
            //         AutoPtr<ILeadingMarginSpan> margin = (ILeadingMarginSpan*)((*spans)[n]->Probe(EIID_ILeadingMarginSpan));
            //         Boolean useFirstLineMargin = isFirstParaLine;
            //         if (margin != NULL && margin->Probe(EIID_ILeadingMarginSpan2)) {
            //             ILeadingMarginSpan2* margin2 = (ILeadingMarginSpan2*)(margin->Probe(EIID_ILeadingMarginSpan2));
            //             Int32 count;
            //             margin2->GetLeadingMarginLineCount(&count);
            //             Int32 tmp;
            //             sp->GetSpanStart(margin, &tmp);
            //             Int32 startLine = GetLineForOffset(tmp);
            //             useFirstLineMargin = i < startLine + count;
            //         }

            Boolean useFirstLineMargin = isFirstParaLine;
            for (Int32 n = 0; n < length; n++) {
                ILeadingMarginSpan2* lms2 = ILeadingMarginSpan2::Probe((*spans)[n]);
                if (lms2 != NULL) {
                    Int32 count, tmp;
                    lms2->GetLeadingMarginLineCount(&count);
                    sp->GetSpanStart(margin, &tmp);
                    Int32 startLine = GetLineForOffset(tmp);
                    // if there is more than one LeadingMarginSpan2, use
                    // the count that is greatest
                    if (i < startLine + count) {
                        useFirstLineMargin = true;
                        break;
                    }
                }
            }
            for (Int32 n = 0; n < length; n++) {
                ILeadingMarginSpan* margin = ILeadingMarginSpan::Probe((*spans)[n]);
                if (margin != NULL) {
                    if (dir == ILayout::DIR_RIGHT_TO_LEFT) {
                        margin->DrawLeadingMargin(canvas, paint, right, dir, ltop,
                                                 lbaseline, lbottom, buf,
                                                 start, end, isFirstParaLine, THIS_PROBE(ILayout));

                        Int32 tmp;
                        margin->GetLeadingMargin(useFirstLineMargin, &tmp);
                        right -= tmp;
                    }
                    else {
                        margin->DrawLeadingMargin(canvas, paint, left, dir, ltop,
                                                 lbaseline, lbottom, buf,
                                                 start, end, isFirstParaLine, THIS_PROBE(ILayout));
                        Int32 tmp;
                        margin->GetLeadingMargin(useFirstLineMargin, &tmp);
                        left += tmp;
                    }
                }
            }
        }

        Boolean hasTabOrEmoji = GetLineContainsTab(i);
        // Can't tell if we have tabs for sure, currently
        if (hasTabOrEmoji && !tabStopsIsInitialized) {
            if (tabStops == NULL) {
                tabStops = new TabStops(TAB_INCREMENT, (ArrayOf<IInterface*>*)(spans.Get()));
            } else {
                tabStops->Reset(TAB_INCREMENT, (ArrayOf<IInterface*>*)(spans.Get()));
            }
            tabStopsIsInitialized = TRUE;
        }

        // Determine whether the line aligns to normal, opposite, or center.
        LayoutAlignment align = paraAlign;
        if (align == ALIGN_LEFT) {
            align = (dir == ILayout::DIR_LEFT_TO_RIGHT) ?
                ALIGN_NORMAL : ALIGN_OPPOSITE;
        } else if (align == ALIGN_RIGHT) {
            align = (dir == ILayout::DIR_LEFT_TO_RIGHT) ?
                ALIGN_OPPOSITE : ALIGN_NORMAL;
        }

        Int32 x;
        if (align == ALIGN_NORMAL) {
            if (dir == ILayout::DIR_LEFT_TO_RIGHT) {
                x = left;
            } else {
                x = right;
            }
        } else {
            Int32 max = (Int32)GetLineExtent(i, tabStops, FALSE);
            if (align == ALIGN_OPPOSITE) {
                if (dir == ILayout::DIR_LEFT_TO_RIGHT) {
                    x = right - max;
                } else {
                    x = left - max;
                }
            } else { // Alignment.ALIGN_CENTER
                max = max & ~1;
                x = (right + left - max) >> 1;
            }
        }

        AutoPtr<ILayoutDirections> directions = GetLineDirections(i);
        if (directions == DIRS_ALL_LEFT_TO_RIGHT && !mSpannedText && !hasTabOrEmoji) {
            // XXX: assumes there's nothing additional to be done
            canvas->DrawText(buf, start, end, x, lbaseline, paint);
        } else {
            tl->Set(paint, buf, start, end, dir, directions, hasTabOrEmoji, tabStops);
            tl->Draw(canvas, x, ltop, lbaseline, lbottom);
        }
    }
    TextLine::Recycle(tl);

    return NOERROR;
}

/**
 * @hide
 */
ECode Layout::DrawBackground(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightPaint,
    /* [in] */ Int32 cursorOffsetVertical,
    /* [in] */ Int32 firstLine,
    /* [in] */ Int32 lastLine)
{
    // First, draw LineBackgroundSpans.
    // LineBackgroundSpans know nothing about the alignment, margins, or
    // direction of the layout or line.  XXX: Should they?
    // They are evaluated at each line.
    if (mSpannedText) {
        if (mLineBackgroundSpans == NULL) {
            mLineBackgroundSpans = new SpanSet<ILineBackgroundSpan>(EIID_ILineBackgroundSpan);
        }

        AutoPtr<ISpanned> buffer = ISpanned::Probe(mText);
        Int32 textLength;
        buffer->GetLength(&textLength);
        mLineBackgroundSpans->Init(buffer, 0, textLength);

        Int32 numberOfSpans = mLineBackgroundSpans->mNumberOfSpans;
        if (numberOfSpans > 0) {
            Int32 previousLineBottom = GetLineTop(firstLine);
            Int32 previousLineEnd = GetLineStart(firstLine);
            AutoPtr< ArrayOf<IInterface*> > spans = (ArrayOf<IInterface*>*)(NO_PARA_SPANS.Get());
            Int32 spansLength = 0;
            AutoPtr<ITextPaint> paint = mPaint;
            Int32 spanEnd = 0;
            Int32 width = mWidth;
            for (Int32 i = firstLine; i <= lastLine; i++) {
                Int32 start = previousLineEnd;
                Int32 end = GetLineStart(i + 1);
                previousLineEnd = end;

                Int32 ltop = previousLineBottom;
                Int32 lbottom = GetLineTop(i + 1);
                previousLineBottom = lbottom;
                Int32 lbaseline = lbottom - GetLineDescent(i);

                if (start >= spanEnd) {
                    // These should be infrequent, so we'll use this so that
                    // we don't have to check as often.
                    spanEnd = mLineBackgroundSpans->GetNextTransition(start, textLength);
                    // All LineBackgroundSpans on a line contribute to its background.
                    spansLength = 0;
                    // Duplication of the logic of getParagraphSpans
                    if (start != end || start == 0) {
                        // Equivalent to a getSpans(start, end), but filling the 'spans' local
                        // array instead to reduce memory allocation
                        Int32 numberOfSpans = mLineBackgroundSpans->mNumberOfSpans;
                        AutoPtr< ArrayOf<Int32> > spanStarts = mLineBackgroundSpans->mSpanStarts;
                        AutoPtr< ArrayOf<Int32> > spanEnds = mLineBackgroundSpans->mSpanEnds;
                        AutoPtr< ArrayOf<ILineBackgroundSpan*> > lbSpans = mLineBackgroundSpans->mSpans;
                        for (Int32 j = 0; j < numberOfSpans; j++) {
                            // equal test is valid since both intervals are not empty by
                            // construction
                            if ((*spanStarts)[j] >= end || (*spanEnds)[j] <= start)
                                continue;

                            spans = GrowingArrayUtils::Append(
                                    spans, spansLength, TO_IINTERFACE((*lbSpans)[i]));
                            spansLength++;
                        }
                    }
                }

                for (Int32 n = 0; n < spansLength; n++) {
                    AutoPtr<ILineBackgroundSpan> lineBackgroundSpan
                        = ILineBackgroundSpan::Probe((*spans)[n]);
                    if (lineBackgroundSpan)
                        lineBackgroundSpan->DrawBackground(canvas, paint, 0, width,
                                ltop, lbaseline, lbottom,
                                buffer, start, end, i);
                }
            }
        }

        mLineBackgroundSpans->Recycle();
    }

    // There can be a highlight even without spans if we are drawing
    // a non-spanned transformation of a spanned editing buffer.
    if (highlight != NULL) {
        if (cursorOffsetVertical != 0) canvas->Translate(0, cursorOffsetVertical);
        canvas->DrawPath(highlight, highlightPaint);
        if (cursorOffsetVertical != 0) canvas->Translate(0, -cursorOffsetVertical);
    }

    return NOERROR;
}

/**
 * @param canvas
 * @return The range of lines that need to be drawn, possibly empty.
 * @hide
 */
Int64 Layout::GetLineRangeForDraw(
    /* [in] */ ICanvas* canvas)
{
    assert(canvas != NULL);

    Int32 dtop, dbottom;

    {
        AutoLock lock(sTempRectLock);
        Boolean flag = FALSE;
        canvas->GetClipBounds(sTempRect, &flag);
        if (!flag) {
            // Negative range end used as a special flag
            return TextUtils::PackRangeInInt64(0, -1);
        }

        sTempRect->GetTop(&dtop);
        sTempRect->GetBottom(&dbottom);
    }

    Int32 top = Elastos::Core::Math::Max(dtop, 0);
    Int32 bottom = Elastos::Core::Math::Min(GetLineTop(GetLineCount()), dbottom);

    if (top >= bottom) return TextUtils::PackRangeInInt64(0, -1);
    return TextUtils::PackRangeInInt64(GetLineForVertical(top), GetLineForVertical(bottom));
}

/**
 * Return the start position of the line, given the left and right bounds
 * of the margins.
 *
 * @param line the line index
 * @param left the left bounds (0, or leading margin if ltr para)
 * @param right the right bounds (width, minus leading margin if rtl para)
 * @return the start position of the line (to right of line if rtl para)
 */
Int32 Layout::GetLineStartPos(
    /* [in] */ Int32 line,
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    // Adjust the point at which to start rendering depending on the
    // alignment of the paragraph.
    LayoutAlignment align = GetParagraphAlignment(line);
    Int32 dir = GetParagraphDirection(line);

    if (align == ALIGN_LEFT) {
        align = (dir == ILayout::DIR_LEFT_TO_RIGHT) ? ALIGN_NORMAL : ALIGN_OPPOSITE;
    } else if (align == ALIGN_RIGHT) {
        align = (dir == ILayout::DIR_LEFT_TO_RIGHT) ? ALIGN_OPPOSITE : ALIGN_NORMAL;
    }

    Int32 x;
    if (align == ALIGN_NORMAL) {
        if (dir == ILayout::DIR_LEFT_TO_RIGHT) {
            x = left;
        } else {
            x = right;
        }
    } else {
        AutoPtr<TabStops> tabStops;
        if (mSpannedText && GetLineContainsTab(line)) {
            AutoPtr<ISpanned> spanned = ISpanned::Probe(mText);
            Int32 start = GetLineStart(line);
            Int32 length;
            Int32 spanEnd;
            spanned->GetLength(&length);
            spanned->NextSpanTransition(start, length, EIID_ITabStopSpan, &spanEnd);
            AutoPtr< ArrayOf<IInterface*> > tabSpans = GetParagraphSpans(
                    spanned, start, spanEnd, EIID_ITabStopSpan);
            if (tabSpans->GetLength() > 0) {
                tabStops = new TabStops(TAB_INCREMENT, tabSpans);
            }
        }

        Int32 max = (Int32)GetLineExtent(line, tabStops, FALSE);
        if (align == ALIGN_OPPOSITE) {
            if (dir == ILayout::DIR_LEFT_TO_RIGHT) {
                x = right - max;
            } else {
                // max is negative here
                x = left - max;
            }
        } else { // Alignment.ALIGN_CENTER
            max = max & ~1;
            x = (left + right - max) >> 1;
        }
    }

    return x;
}

/**
 * Return the text that is displayed by this Layout.
 */
AutoPtr<ICharSequence> Layout::GetText()
{
    return mText;
}

/**
 * Return the base Paint properties for this layout.
 * Do NOT change the paint, which may result in funny
 * drawing for this layout.
 */
AutoPtr<ITextPaint> Layout::GetPaint()
{
    return mPaint;
}

/**
 * Return the width of this layout.
 */
Int32 Layout::GetWidth()
{
    return mWidth;
}

/**
 * Return the width to which this Layout is ellipsizing, or
 * {@link #getWidth} if it is not doing anything special.
 */
Int32 Layout::GetEllipsizedWidth()
{
    return mWidth;
}

/**
 * Increase the width of this layout to the specified width.
 * Be careful to use this only when you know it is appropriate&mdash;
 * it does not cause the text to reflow to use the full new width.
 */
ECode Layout::IncreaseWidthTo(
    /* [in] */ Int32 wid)
{
//    if (wid < mWidth) {
//        throw new RuntimeException("attempted to reduce Layout width");
//    }
    assert(wid >= mWidth);

    mWidth = wid;

    return NOERROR;
}

/**
 * Return the total height of this layout.
 */
Int32 Layout::GetHeight()
{
    return GetLineTop(GetLineCount());
}

/**
 * Return the base alignment of this layout.
 */
LayoutAlignment Layout::GetAlignment()
{
    return mAlignment;
}

/**
 * Return what the text height is multiplied by to get the line height.
 */
Float Layout::GetSpacingMultiplier()
{
    return mSpacingMult;
}

/**
 * Return the number of units of leading that are added to each line.
 */
Float Layout::GetSpacingAdd()
{
    return mSpacingAdd;
}

/**
 * Return the heuristic used to determine paragraph text direction.
 * @hide
 */
AutoPtr<ITextDirectionHeuristic> Layout::GetTextDirectionHeuristic()
{
    return mTextDir;
}

/**
 * Return the baseline for the specified line (0&hellip;getLineCount() - 1)
 * If bounds is not null, return the top, left, right, bottom extents
 * of the specified line in it.
 * @param line which line to examine (0..getLineCount() - 1)
 * @param bounds Optional. If not null, it returns the extent of the line
 * @return the Y-coordinate of the baseline
 */
Int32 Layout::GetLineBounds(
    /* [in] */ Int32 line,
    /* [in] */ IRect* bounds)
{
    if (bounds != NULL) {
        bounds->SetLeft(0);
        bounds->SetTop(GetLineTop(line));
        bounds->SetRight(mWidth);   // ???
        bounds->SetBottom(GetLineTop(line + 1));
    }

    return GetLineBaseline(line);
}

/**
 * Returns true if the character at offset and the preceding character
 * are at different run levels (and thus there's a split caret).
 * @param offset the offset
 * @return true if at a level boundary
 * @hide
 */
Boolean Layout::IsLevelBoundary(
    /* [in] */ Int32 offset)
{
    Int32 line = GetLineForOffset(offset);
    AutoPtr<ILayoutDirections> dirs = GetLineDirections(line);
    if (dirs == DIRS_ALL_LEFT_TO_RIGHT || dirs == DIRS_ALL_RIGHT_TO_LEFT) {
        return FALSE;
    }

    AutoPtr< ArrayOf<Int32> > runs = NULL;
    dirs->GetDirections((ArrayOf<Int32>**)&runs);
    Int32 lineStart = GetLineStart(line);
    Int32 lineEnd = GetLineEnd(line);
    if (offset == lineStart || offset == lineEnd) {
        Int32 paraLevel = GetParagraphDirection(line) == 1 ? 0 : 1;
        Int32 runIndex = offset == lineStart ? 0 : runs.Get()->GetLength() - 2;
        return (((*(runs.Get()))[runIndex + 1] /*>>>*/>> ILayout::RUN_LEVEL_SHIFT) & ILayout::RUN_LEVEL_MASK) != paraLevel;
    }

    offset -= lineStart;
    for (Int32 i = 0; i < runs.Get()->GetLength(); i += 2) {
        if (offset == (*(runs.Get()))[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Returns true if the character at offset is right to left (RTL).
 * @param offset the offset
 * @return true if the character is RTL, false if it is LTR
 */
Boolean Layout::IsRtlCharAt(
    /* [in] */ Int32 offset)
{
    Int32 line = GetLineForOffset(offset);
    AutoPtr<ILayoutDirections> dirs = GetLineDirections(line);
    if (dirs == DIRS_ALL_LEFT_TO_RIGHT) {
        return FALSE;
    }

    if (dirs == DIRS_ALL_RIGHT_TO_LEFT) {
        return  TRUE;
    }

    AutoPtr< ArrayOf<Int32> >runs;
    dirs->GetDirections((ArrayOf<Int32>**)&runs);
    Int32 lineStart = GetLineStart(line);
    for (Int32 i = 0; i < runs.Get()->GetLength(); i += 2) {
        Int32 start = lineStart + ((*(runs.Get()))[i] & ILayout::RUN_LENGTH_MASK);
        Int32 limit = start + ((*runs)[i+1] & RUN_LENGTH_MASK);
        if (offset >= start && offset < limit) {
            Int32 level = ((*runs)[i+1] /*>>>*/>> ILayout::RUN_LEVEL_SHIFT) & ILayout::RUN_LEVEL_MASK;
            return ((level & 1) != 0);
        }
    }
    // Should happen only if the offset is "out of bounds"
    return FALSE;
}

Boolean Layout::PrimaryIsTrailingPrevious(
    /* [in] */ Int32 offset)
{
    Int32 line = GetLineForOffset(offset);
    Int32 lineStart = GetLineStart(line);
    Int32 lineEnd = GetLineEnd(line);
    AutoPtr< ArrayOf<Int32> > runs;
    GetLineDirections(line)->GetDirections((ArrayOf<Int32>**)&runs);

    Int32 levelAt = -1;
    for (Int32 i = 0; i < runs.Get()->GetLength(); i += 2) {
        Int32 start = lineStart + (*(runs.Get()))[i];
        Int32 limit = start + ((*(runs.Get()))[i+1] & ILayout::RUN_LENGTH_MASK);
        if (limit > lineEnd) {
            limit = lineEnd;
        }

        if (offset >= start && offset < limit) {
            if (offset > start) {
                // Previous character is at same level, so don't use trailing.
                return FALSE;
            }

            levelAt = ((*(runs.Get()))[i+1] /*>>>*/>> ILayout::RUN_LEVEL_SHIFT) & ILayout::RUN_LEVEL_MASK;
            break;
        }
    }

    if (levelAt == -1) {
        // Offset was limit of line.
        levelAt = GetParagraphDirection(line) == 1 ? 0 : 1;
    }

    // At level boundary, check previous level.
    Int32 levelBefore = -1;
    if (offset == lineStart) {
        levelBefore = GetParagraphDirection(line) == 1 ? 0 : 1;
    } else {
        offset -= 1;
        for (Int32 i = 0; i < runs.Get()->GetLength(); i += 2) {
            Int32 start = lineStart + (*(runs.Get()))[i];
            Int32 limit = start + ((*(runs.Get()))[i+1] & ILayout::RUN_LENGTH_MASK);
            if (limit > lineEnd) {
                limit = lineEnd;
            }
            if (offset >= start && offset < limit) {
                levelBefore = ((*(runs.Get()))[i+1] /*>>>*/>> ILayout::RUN_LEVEL_SHIFT) & ILayout::RUN_LEVEL_MASK;
                break;
            }
        }
    }

    return levelBefore < levelAt;
}

/**
 * Get the primary horizontal position for the specified text offset.
 * This is the location where a new character would be inserted in
 * the paragraph's primary direction.
 */
Float Layout::GetPrimaryHorizontal(
    /* [in] */ Int32 offset)
{
    return GetPrimaryHorizontal(offset, FALSE);
}

Float Layout::GetPrimaryHorizontal(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean clamped)
{
    Boolean trailing = PrimaryIsTrailingPrevious(offset);
    return GetHorizontal(offset, trailing, clamped);
}

/**
 * Get the secondary horizontal position for the specified text offset.
 * This is the location where a new character would be inserted in
 * the direction other than the paragraph's primary direction.
 */
Float Layout::GetSecondaryHorizontal(
    /* [in] */ Int32 offset)
{
    return GetSecondaryHorizontal(offset, FALSE);
}

Float Layout::GetSecondaryHorizontal(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean clamped)
{
    Boolean trailing = PrimaryIsTrailingPrevious(offset);
    return GetHorizontal(offset, !trailing, clamped);
}

Float Layout::GetHorizontal(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean trailing,
    /* [in] */ Boolean clamped)
{
    Int32 line = GetLineForOffset(offset);

    return GetHorizontal(offset, trailing, line, clamped);
}

Float Layout::GetHorizontal(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean trailing,
    /* [in] */ Int32 line,
    /* [in] */ Boolean clamped)
{
    Int32 start = GetLineStart(line);
    Int32 end = GetLineEnd(line);
    Int32 dir = GetParagraphDirection(line);
    Boolean hasTabOrEmoji = GetLineContainsTab(line);
    AutoPtr<ILayoutDirections> directions = GetLineDirections(line);

    AutoPtr<TabStops> tabStops;
    if (hasTabOrEmoji && ISpanned::Probe(mText)) {
        // Just checking this line should be good enough, tabs should be
        // consistent across all lines in a paragraph.
        AutoPtr<ISpanned> spanned = ISpanned::Probe(mText);
        AutoPtr< ArrayOf<IInterface*> > tabs = GetParagraphSpans(
                spanned, start, end, EIID_ITabStopSpan);
        if (tabs->GetLength() > 0) {
            tabStops = new TabStops(TAB_INCREMENT, tabs); // XXX should reuse
        }
    }

    AutoPtr<TextLine> tl = TextLine::Obtain();
    tl->Set(mPaint, mText, start, end, dir, directions, hasTabOrEmoji, tabStops);
    Float wid = tl->Measure(offset - start, trailing, NULL);
    TextLine::Recycle(tl);

    if (clamped && wid > mWidth) {
        wid = mWidth;
    }

    Int32 left = GetParagraphLeft(line);
    Int32 right = GetParagraphRight(line);

    return GetLineStartPos(line, left, right) + wid;
}

/**
 * Get the leftmost position that should be exposed for horizontal
 * scrolling on the specified line.
 */
Float Layout::GetLineLeft(
    /* [in] */ Int32 line)
{
    Int32 dir = GetParagraphDirection(line);
    LayoutAlignment align = GetParagraphAlignment(line);

    if (align == ALIGN_LEFT) {
        return 0;
    } else if (align == ALIGN_NORMAL) {
        if (dir == ILayout::DIR_RIGHT_TO_LEFT)
            return GetParagraphRight(line) - GetLineMax(line);
        else
            return 0;
    } else if (align == ALIGN_RIGHT) {
        return mWidth - GetLineMax(line);
    } else if (align == ALIGN_OPPOSITE) {
        if (dir == ILayout::DIR_RIGHT_TO_LEFT)
            return 0;
        else
            return mWidth - GetLineMax(line);
    } else { /* align == Alignment.ALIGN_CENTER */
        Int32 left = GetParagraphLeft(line);
        Int32 right = GetParagraphRight(line);
        Int32 max = ((Int32) GetLineMax(line)) & ~1;

        return left + ((right - left) - max) / 2;
    }
}

/**
 * Get the rightmost position that should be exposed for horizontal
 * scrolling on the specified line.
 */
Float Layout::GetLineRight(
    /* [in] */ Int32 line)
{
    Int32 dir = GetParagraphDirection(line);
    LayoutAlignment align = GetParagraphAlignment(line);

    if (align == ALIGN_LEFT) {
        return GetParagraphLeft(line) + GetLineMax(line);
    } else if (align == ALIGN_NORMAL) {
        if (dir == ILayout::DIR_RIGHT_TO_LEFT)
            return mWidth;
        else
            return GetParagraphLeft(line) + GetLineMax(line);
    } else if (align == ALIGN_RIGHT) {
        return mWidth;
    } else if (align == ALIGN_OPPOSITE) {
        if (dir == ILayout::DIR_RIGHT_TO_LEFT)
            return GetLineMax(line);
        else
            return mWidth;
    } else { /* align == Alignment.ALIGN_CENTER */
        Int32 left = GetParagraphLeft(line);
        Int32 right = GetParagraphRight(line);
        Int32 max = ((Int32) GetLineMax(line)) & ~1;

        return right - ((right - left) - max) / 2;
    }
}

/**
 * Gets the unsigned horizontal extent of the specified line, including
 * leading margin indent, but excluding trailing whitespace.
 */
Float Layout::GetLineMax(
    /* [in] */ Int32 line)
{
    Float margin = GetParagraphLeadingMargin(line);
    Float signedExtent = GetLineExtent(line, FALSE);
    return margin + (signedExtent >= 0 ? signedExtent : -signedExtent);
}

/**
 * Gets the unsigned horizontal extent of the specified line, including
 * leading margin indent and trailing whitespace.
 */
Float Layout::GetLineWidth(
    /* [in] */ Int32 line)
{
    Float margin = GetParagraphLeadingMargin(line);
    Float signedExtent = GetLineExtent(line, TRUE);
    return margin + (signedExtent >= 0 ? signedExtent : -signedExtent);
}

/**
 * Like {@link #getLineExtent(int,TabStops,boolean)} but determines the
 * tab stops instead of using the ones passed in.
 * @param line the index of the line
 * @param full whether to include trailing whitespace
 * @return the extent of the line
 */
Float Layout::GetLineExtent(
    /* [in] */ Int32 line,
    /* [in] */ Boolean full)
{
    Int32 start = GetLineStart(line);
    Int32 end = full ? GetLineEnd(line) : GetLineVisibleEnd(line);

    Boolean hasTabsOrEmoji = GetLineContainsTab(line);
    AutoPtr<TabStops> tabStops;
    if (hasTabsOrEmoji && mText != NULL && mText->Probe(EIID_ISpanned)) {
        // Just checking this line should be good enough, tabs should be
        // consistent across all lines in a paragraph.
        AutoPtr<ISpanned> spanned = ISpanned::Probe(mText);
        AutoPtr< ArrayOf<IInterface*> > tabs = GetParagraphSpans(
                spanned, start, end, EIID_ITabStopSpan);
        if (tabs->GetLength() > 0) {
            tabStops = new TabStops(TAB_INCREMENT, tabs); // XXX should reuse
        }
    }

    AutoPtr<ILayoutDirections> directions = GetLineDirections(line);
    // Returned directions can actually be null
    if (directions == NULL) {
        return /*0f*/0.0;
    }

    Int32 dir = GetParagraphDirection(line);

    AutoPtr<TextLine> tl = TextLine::Obtain();
    tl->Set(mPaint, mText, start, end, dir, directions, hasTabsOrEmoji, tabStops);
    Float width = tl->Metrics(NULL);
    TextLine::Recycle(tl);

    return width;
}

/**
 * Returns the signed horizontal extent of the specified line, excluding
 * leading margin.  If full is false, excludes trailing whitespace.
 * @param line the index of the line
 * @param tabStops the tab stops, can be null if we know they're not used.
 * @param full whether to include trailing whitespace
 * @return the extent of the text on this line
 */
Float Layout::GetLineExtent(
    /* [in] */ Int32 line,
    /* [in] */ TabStops* tabStops,
    /* [in] */ Boolean full)
{
    Int32 start = GetLineStart(line);
    Int32 end = full ? GetLineEnd(line) : GetLineVisibleEnd(line);
    Boolean hasTabsOrEmoji = GetLineContainsTab(line);
    AutoPtr<ILayoutDirections> directions = GetLineDirections(line);
    Int32 dir = GetParagraphDirection(line);

    AutoPtr<TextLine> tl = TextLine::Obtain();
    tl->Set(mPaint, mText, start, end, dir, directions, hasTabsOrEmoji, tabStops);
    Float width = tl->Metrics(NULL);
    TextLine::Recycle(tl);

    return width;
}

/**
 * Get the line number corresponding to the specified vertical position.
 * If you ask for a position above 0, you get 0; if you ask for a position
 * below the bottom of the text, you get the last line.
 */
// FIXME: It may be faster to do a linear search for layouts without many lines.
Int32 Layout::GetLineForVertical(
    /* [in] */ Int32 vertical)
{
    Int32 high = GetLineCount(), low = -1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if (GetLineTop(guess) > vertical)
            high = guess;
        else
            low = guess;
    }

    if (low < 0)
        return 0;
    else
        return low;
}

/**
 * Get the line number on which the specified text offset appears.
 * If you ask for a position before 0, you get 0; if you ask for a position
 * beyond the end of the text, you get the last line.
 */
Int32 Layout::GetLineForOffset(
    /* [in] */ Int32 offset)
{
    Int32 high = GetLineCount(), low = -1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if (GetLineStart(guess) > offset)
            high = guess;
        else
            low = guess;
    }

    if (low < 0)
        return 0;
    else
        return low;
}

/**
 * Get the character offset on the specified line whose position is
 * closest to the specified horizontal position.
 */
Int32 Layout::GetOffsetForHorizontal(
    /* [in] */ Int32 line,
    /* [in] */ Float horiz)
{
    Int32 max = GetLineEnd(line) - 1;
    Int32 min = GetLineStart(line);
    AutoPtr<ILayoutDirections> dirs = GetLineDirections(line);

    if (line == GetLineCount() - 1)
        max++;

    Int32 best = min;
    Float bestdist = Elastos::Core::Math::Abs(GetPrimaryHorizontal(best) - horiz);

    AutoPtr< ArrayOf<Int32> > directions;
    dirs->GetDirections((ArrayOf<Int32>**)&directions);
    for (Int32 i = 0; i < directions->GetLength(); i += 2) {
        Int32 here = min + (*directions)[i];
        Int32 there = here + ((*directions)[i+1] & ILayout::RUN_LENGTH_MASK);
        Int32 swap = ((*directions)[i+1] & ILayout::RUN_RTL_FLAG) != 0 ? -1 : 1;

        if (there > max)
            there = max;
        Int32 high = there - 1 + 1, low = here + 1 - 1, guess;

        while (high - low > 1) {
            guess = (high + low) / 2;
            Int32 adguess = GetOffsetAtStartOf(guess);

            if (GetPrimaryHorizontal(adguess) * swap >= horiz * swap)
                high = guess;
            else
                low = guess;
        }

        if (low < here + 1)
            low = here + 1;

        if (low < there) {
            low = GetOffsetAtStartOf(low);

            Float dist = Elastos::Core::Math::Abs(GetPrimaryHorizontal(low) - horiz);

            Int32 aft = TextUtils::GetOffsetAfter(mText, low);
            if (aft < there) {
                Float other = Elastos::Core::Math::Abs(GetPrimaryHorizontal(aft) - horiz);

                if (other < dist) {
                    dist = other;
                    low = aft;
                }
            }

            if (dist < bestdist) {
                bestdist = dist;
                best = low;
            }
        }

        Float dist = Elastos::Core::Math::Abs(GetPrimaryHorizontal(here) - horiz);

        if (dist < bestdist) {
            bestdist = dist;
            best = here;
        }
    }

    Float dist = Elastos::Core::Math::Abs(GetPrimaryHorizontal(max) - horiz);

    if (dist <= bestdist) {
        bestdist = dist;
        best = max;
    }

    return best;
}

/**
 * Return the text offset after the last character on the specified line.
 */
Int32 Layout::GetLineEnd(
    /* [in] */ Int32 line)
{
    return GetLineStart(line + 1);
}

/**
 * Return the text offset after the last visible character (so whitespace
 * is not counted) on the specified line.
 */
Int32 Layout::GetLineVisibleEnd(
    /* [in] */ Int32 line)
{
    return GetLineVisibleEnd(line, GetLineStart(line), GetLineStart(line+1));
}

Int32 Layout::GetLineVisibleEnd(
    /* [in] */ Int32 line,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ICharSequence> text = mText;
    Char32 ch;
    if (line == GetLineCount() - 1) {
        return end;
    }

    for (; end > start; end--) {
        text->GetCharAt(end - 1, &ch);

        if (ch == '\n') {
            return end - 1;
        }

        if (ch != ' ' && ch != '\t') {
            break;
        }

    }

    return end;
}

/**
 * Return the vertical position of the bottom of the specified line.
 */
Int32 Layout::GetLineBottom(
    /* [in] */ Int32 line)
{
    return GetLineTop(line + 1);
}

/**
 * Return the vertical position of the baseline of the specified line.
 */
Int32 Layout::GetLineBaseline(
    /* [in] */ Int32 line)
{
    // getLineTop(line+1) == getLineTop(line)
    return GetLineTop(line+1) - GetLineDescent(line);
}

/**
 * Get the ascent of the text on the specified line.
 * The return value is negative to match the Paint.ascent() convention.
 */
Int32 Layout::GetLineAscent(
    /* [in] */ Int32 line)
{
    // getLineTop(line+1) - getLineDescent(line) == getLineBaseLine(line)
    return GetLineTop(line) - (GetLineTop(line+1) - GetLineDescent(line));
}

Int32 Layout::GetOffsetToLeftOf(
    /* [in] */ Int32 offset)
{
    return GetOffsetToLeftRightOf(offset, TRUE);
}

Int32 Layout::GetOffsetToRightOf(
    /* [in] */ Int32 offset)
{
    return GetOffsetToLeftRightOf(offset, FALSE);
}

Int32 Layout::GetOffsetToLeftRightOf(
    /* [in] */ Int32 caret,
    /* [in] */ Boolean toLeft)
{
    Int32 line = GetLineForOffset(caret);
    Int32 lineStart = GetLineStart(line);
    Int32 lineEnd = GetLineEnd(line);
    Int32 lineDir = GetParagraphDirection(line);

    Boolean lineChanged = FALSE;
    Boolean advance = toLeft == (lineDir == ILayout::DIR_RIGHT_TO_LEFT);
    // if walking off line, look at the line we're headed to
    if (advance) {
        if (caret == lineEnd) {
            if (line < GetLineCount() - 1) {
                lineChanged = TRUE;
                ++line;
            } else {
                return caret; // at very end, don't move
            }
        }
    } else {
        if (caret == lineStart) {
            if (line > 0) {
                lineChanged = TRUE;
                --line;
            } else {
                return caret; // at very start, don't move
            }
        }
    }

    if (lineChanged) {
        lineStart = GetLineStart(line);
        lineEnd = GetLineEnd(line);
        Int32 newDir = GetParagraphDirection(line);
        if (newDir != lineDir) {
            // unusual case.  we want to walk onto the line, but it runs
            // in a different direction than this one, so we fake movement
            // in the opposite direction.
            toLeft = !toLeft;
            lineDir = newDir;
        }
    }

    AutoPtr<ILayoutDirections> directions = GetLineDirections(line);

    AutoPtr<TextLine> tl = TextLine::Obtain();
    // XXX: we don't care about tabs
    tl->Set(mPaint, mText, lineStart, lineEnd, lineDir, directions, FALSE, NULL);
    caret = lineStart + tl->GetOffsetToLeftRightOf(caret - lineStart, toLeft);
    tl = TextLine::Recycle(tl);

    return caret;
}

Int32 Layout::GetOffsetAtStartOf(
    /* [in] */ Int32 offset)
{
    // XXX this probably should skip local reorderings and
    // zero-width characters, look at callers
    if (offset == 0)
        return 0;

    AutoPtr<ICharSequence> text = mText;
    Char32 c;
    text->GetCharAt(offset, &c);

    if (c >= 0xDC00/*'\uDC00'*/ && c <= 0xDFFF/*'\uDFFF'*/) {
        Char32 c1;
        text->GetCharAt(offset - 1, &c1);

        if (c1 >= 0xD800/*'\uD800'*/ && c1 <= 0xDBFF/*'\uDBFF'*/)
           offset -= 1;
    }

    if (mSpannedText) {
        AutoPtr<ISpanned> spanned = (ISpanned*)(text->Probe(EIID_ISpanned));

        AutoPtr< ArrayOf<IInterface*> > spans;
        spanned->GetSpans(offset, offset, EIID_IReplacementSpan, (ArrayOf<IInterface*>**)&spans);

        Int32 start;
        Int32 end;
        for (Int32 i = 0; i < spans->GetLength(); i++) {
            spanned->GetSpanStart((*spans)[i], &start);
            spanned->GetSpanEnd((*spans)[i], &end);

            if (start < offset && end > offset)
                offset = start;
        }
    }

    return offset;
}

Boolean Layout::ShouldClampCursor(
        /* [in] */ Int32 line)
{
    // Only clamp cursor position in left-aligned displays.
    switch (GetParagraphAlignment(line)) {
        case LayoutAlignment_ALIGN_LEFT:
            return TRUE;
        case LayoutAlignment_ALIGN_NORMAL:
            return GetParagraphDirection(line) > 0;
        default:
            return FALSE;
    }
}

/**
 * Fills in the specified Path with a representation of a cursor
 * at the specified offset.  This will often be a vertical line
 * but can be multiple discontinuous lines in text with multiple
 * directionalities.
 */
ECode Layout::GetCursorPath(
    /* [in] */ Int32 point,
    /* [in] */ IPath* dest,
    /* [in] */ ICharSequence* editingBuffer)
{
    dest->Reset();

    Int32 line = GetLineForOffset(point);
    Int32 top = GetLineTop(line);
    Int32 bottom = GetLineTop(line+1);

    Boolean clamped = ShouldClampCursor(line);
    Float h1 = GetPrimaryHorizontal(point, clamped) - 0.5f;
    Float h2 = IsLevelBoundary(point) ? GetSecondaryHorizontal(point, clamped) - 0.5f : h1;

    Int32 caps = CTextKeyListener::GetMetaState(editingBuffer, IMetaKeyKeyListener::META_SHIFT_ON) |
            CTextKeyListener::GetMetaState(editingBuffer, IMetaKeyKeyListener::META_SELECTING);
    Int32 fn = CTextKeyListener::GetMetaState(editingBuffer, IMetaKeyKeyListener::META_ALT_ON);
    Int32 dist = 0;

    if (caps != 0 || fn != 0) {
        dist = (bottom - top) >> 2;

        if (fn != 0)
            top += dist;
        if (caps != 0)
            bottom -= dist;
    }

    if (h1 < 0.5f)
        h1 = 0.5f;
    if (h2 < 0.5f)
        h2 = 0.5f;

    if (Elastos::Core::Math::Compare(h1, h2) == 0) {
        dest->MoveTo(h1, top);
        dest->LineTo(h1, bottom);
    } else {
        dest->MoveTo(h1, top);
        dest->LineTo(h1, (top + bottom) >> 1);

        dest->MoveTo(h2, (top + bottom) >> 1);
        dest->LineTo(h2, bottom);
    }

    if (caps == 2) {
        dest->MoveTo(h2, bottom);
        dest->LineTo(h2 - dist, bottom + dist);
        dest->LineTo(h2, bottom);
        dest->LineTo(h2 + dist, bottom + dist);
    } else if (caps == 1) {
        dest->MoveTo(h2, bottom);
        dest->LineTo(h2 - dist, bottom + dist);

        dest->MoveTo(h2 - dist, bottom + dist - 0.5f);
        dest->LineTo(h2 + dist, bottom + dist - 0.5f);

        dest->MoveTo(h2 + dist, bottom + dist);
        dest->LineTo(h2, bottom);
    }

    if (fn == 2) {
        dest->MoveTo(h1, top);
        dest->LineTo(h1 - dist, top - dist);
        dest->LineTo(h1, top);
        dest->LineTo(h1 + dist, top - dist);
    } else if (fn == 1) {
        dest->MoveTo(h1, top);
        dest->LineTo(h1 - dist, top - dist);

        dest->MoveTo(h1 - dist, top - dist + 0.5f);
        dest->LineTo(h1 + dist, top - dist + 0.5f);

        dest->MoveTo(h1 + dist, top - dist);
        dest->LineTo(h1, top);
    }

    return NOERROR;
}

void Layout::AddSelection(
    /* [in] */ Int32 line,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom,
    /* [in] */ IPath* dest)
{
    Int32 linestart = GetLineStart(line);
    Int32 lineend = GetLineEnd(line);
    AutoPtr<ILayoutDirections> dirs = GetLineDirections(line);

    Char32 ch;
    if (lineend > linestart && (mText->GetCharAt(lineend - 1, &ch), ch) == '\n')
        lineend--;

    AutoPtr< ArrayOf<Int32> > directions = NULL;
    dirs->GetDirections((ArrayOf<Int32>**)&directions);
    for (Int32 i = 0; i < directions->GetLength(); i += 2) {
        Int32 here = linestart + (*directions)[i];
        Int32 there = here + ((*directions)[i+1] & ILayout::RUN_LENGTH_MASK);

        if (there > lineend)
            there = lineend;

        if (start <= there && end >= here) {
            Int32 st = Elastos::Core::Math::Max(start, here);
            Int32 en = Elastos::Core::Math::Min(end, there);

            if (st != en) {
                Float h1 = GetHorizontal(st, FALSE, line, FALSE /* not clamped */);
                Float h2 = GetHorizontal(en, TRUE, line, FALSE /* not clamped */);

                Float left = Elastos::Core::Math::Min(h1, h2);
                Float right = Elastos::Core::Math::Max(h1, h2);

                dest->AddRect(left, top, right, bottom, Elastos::Droid::Graphics::PathDirection_CW);
            }
        }
    }
}

/**
 * Fills in the specified Path with a representation of a highlight
 * between the specified offsets.  This will often be a rectangle
 * or a potentially discontinuous set of rectangles.  If the start
 * and end are the same, the returned path is empty.
 */
ECode Layout::GetSelectionPath(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPath* dest)
{
    dest->Reset();

    if (start == end)
        return NOERROR;

    if (end < start) {
        Int32 temp = end;
        end = start;
        start = temp;
    }

    Int32 startline = GetLineForOffset(start);
    Int32 endline = GetLineForOffset(end);

    Int32 top = GetLineTop(startline);
    Int32 bottom = GetLineBottom(endline);

    if (startline == endline) {
        AddSelection(startline, start, end, top, bottom, dest);
    } else {
        Float width = mWidth;

        AddSelection(startline, start, GetLineEnd(startline),
                     top, GetLineBottom(startline), dest);

        if (GetParagraphDirection(startline) == ILayout::DIR_RIGHT_TO_LEFT)
            dest->AddRect(GetLineLeft(startline), top,
                          0, GetLineBottom(startline), Elastos::Droid::Graphics::PathDirection_CW);
        else
            dest->AddRect(GetLineRight(startline), top,
                          width, GetLineBottom(startline), Elastos::Droid::Graphics::PathDirection_CW);

        for (Int32 i = startline + 1; i < endline; i++) {
            top = GetLineTop(i);
            bottom = GetLineBottom(i);
            dest->AddRect(0, top, width, bottom, Elastos::Droid::Graphics::PathDirection_CW);
        }

        top = GetLineTop(endline);
        bottom = GetLineBottom(endline);

        AddSelection(endline, GetLineStart(endline), end,
                     top, bottom, dest);

        if (GetParagraphDirection(endline) == ILayout::DIR_RIGHT_TO_LEFT)
            dest->AddRect(width, top, GetLineRight(endline), bottom, Elastos::Droid::Graphics::PathDirection_CW);
        else
            dest->AddRect(0, top, GetLineLeft(endline), bottom, Elastos::Droid::Graphics::PathDirection_CW);
    }

    return NOERROR;
}

/**
 * Get the alignment of the specified paragraph, taking into account
 * markup attached to it.
 */
LayoutAlignment Layout::GetParagraphAlignment(
    /* [in] */ Int32 line)
{
    LayoutAlignment align = mAlignment;

    if (mSpannedText) {
        AutoPtr<ISpanned> spanned = ISpanned::Probe(mText);
        if (spanned) {
            AutoPtr< ArrayOf<IInterface*> > spans = GetParagraphSpans(
                    spanned, GetLineStart(line), GetLineEnd(line), EIID_IAlignmentSpan);
            Int32 spanLength = spans->GetLength();
            if (spanLength > 0) {
                IAlignmentSpan* as = IAlignmentSpan::Probe((*spans)[spanLength-1]);
                as->GetAlignment(&align);
            }
        }
    }

    return align;
}

/**
 * Get the left edge of the specified paragraph, inset by left margins.
 */
Int32 Layout::GetParagraphLeft(
    /* [in] */ Int32 line)
{
    Int32 left = 0;
    Int32 dir = GetParagraphDirection(line);
    if (dir == ILayout::DIR_RIGHT_TO_LEFT || !mSpannedText) {
        return left; // leading margin has no impact, or no styles
    }

    return GetParagraphLeadingMargin(line);
}

/**
 * Get the right edge of the specified paragraph, inset by right margins.
 */
Int32 Layout::GetParagraphRight(
    /* [in] */ Int32 line)
{
    Int32 right = mWidth;
    Int32 dir = GetParagraphDirection(line);
    if (dir == ILayout::DIR_LEFT_TO_RIGHT || !mSpannedText) {
        return right; // leading margin has no impact, or no styles
    }

    return right - GetParagraphLeadingMargin(line);
}

/**
 * Returns the effective leading margin (unsigned) for this line,
 * taking into account LeadingMarginSpan and LeadingMarginSpan2.
 * @param line the line index
 * @return the leading margin of this line
 */
Int32 Layout::GetParagraphLeadingMargin(
    /* [in] */ Int32 line)
{
    if (!mSpannedText) {
        return 0;
    }

    AutoPtr<ISpanned> spanned = ISpanned::Probe(mText);

    Int32 lineStart = GetLineStart(line);
    Int32 lineEnd = GetLineEnd(line);
    Int32 spanEnd;
    spanned->NextSpanTransition(lineStart, lineEnd, EIID_ILeadingMarginSpan, &spanEnd);
    AutoPtr< ArrayOf<IInterface*> > spans = GetParagraphSpans(spanned, lineStart, spanEnd,
                                            EIID_ILeadingMarginSpan);
    if (spans->GetLength() == 0) {
        return 0; // no leading margin span;
    }

    Int32 margin = 0;

    Char32 ch;
    Boolean isFirstParaLine = lineStart == 0 || (spanned->GetCharAt(lineStart - 1, &ch), ch) == '\n';
    Boolean useFirstLineMargin = isFirstParaLine;

    Int32 length = spans->GetLength();
    for (Int32 i = 0; i < length; i++) {
        ILeadingMarginSpan2* lms2 = ILeadingMarginSpan2::Probe((*spans)[i]);
        if (lms2 != NULL) {
            Int32 spStart, spanLine, count;
            spanned->GetSpanStart((*spans)[i], &spStart);
            spanLine = GetLineForOffset(spStart);
            lms2->GetLeadingMarginLineCount(&count);
            // if there is more than one LeadingMarginSpan2, use the count that is greatest
            useFirstLineMargin |= line < spanLine + count;
        }
    }
    for (Int32 i = 0; i < length; i++) {
        ILeadingMarginSpan* span = ILeadingMarginSpan::Probe((*spans)[i]);

        Int32 tmp = 0;
        span->GetLeadingMargin(useFirstLineMargin, &tmp);
        margin += tmp;
    }

    return /*margin*/0;
}

/* package */
Float Layout::MeasurePara(
    /* [in] */ ITextPaint* paint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<MeasuredText> mt = MeasuredText::Obtain();
    AutoPtr<TextLine> tl = TextLine::Obtain();
    //try {
        mt->SetPara(text, start, end, TextDirectionHeuristics::LTR);
        AutoPtr<ILayoutDirections> directions;
        Int32 dir;
        if (mt->mEasy) {
            directions = DIRS_ALL_LEFT_TO_RIGHT;
            dir = ILayout::DIR_LEFT_TO_RIGHT;
        } else {
            directions = AndroidBidi::Directions(mt->mDir, mt->mLevels,
                0, mt->mChars, 0, mt->mLen);
            dir = mt->mDir;
        }

        AutoPtr< ArrayOf<Char32> > chars = mt->mChars;
        Int32 len = mt->mLen;
        Boolean hasTabs = FALSE;
        AutoPtr<TabStops> tabStops;

        // leading margins should be taken into account when measuring a paragraph
        Int32 margin = 0;
        ISpanned* spanned = ISpanned::Probe(text);
        if (spanned != NULL) {
            AutoPtr< ArrayOf<IInterface*> > spans = GetParagraphSpans(spanned, start, end, EIID_ILeadingMarginSpan);
            if (spans != NULL) {
                for (Int32 i = 0; i < spans->GetLength(); ++i) {
                    ILeadingMarginSpan* lms = ILeadingMarginSpan::Probe((*spans)[i]);
                    Int32 tmp = 0;
                    lms->GetLeadingMargin(TRUE, &tmp);
                    margin += tmp;
                }
            }
        }

        for (Int32 i = 0; i < len; ++i) {
            if ((*chars)[i] == '\t') {
                hasTabs = TRUE;
                if (text != NULL && text->Probe(EIID_ISpanned)) {
                    AutoPtr<ISpanned> spanned = (ISpanned*)(text->Probe(EIID_ISpanned));
                    Int32 spanEnd;
                    spanned->NextSpanTransition(start, end,
                            EIID_ITabStopSpan, &spanEnd);
                    AutoPtr< ArrayOf<IInterface*> > spans = GetParagraphSpans(spanned, start, spanEnd,
                            EIID_ITabStopSpan);
                    if (spans->GetLength() > 0) {
                        tabStops = new TabStops(TAB_INCREMENT, spans);
                    }
                }
                break;
            }
        }

        tl->Set(paint, text, start, end, dir, directions, hasTabs, tabStops);
        Float result =  margin + tl->Metrics(NULL);
    //} finally {
        TextLine::Recycle(tl);
        MeasuredText::Recycle(mt);
    //}

    return result;
}

Float Layout::NextTab(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float h,
    /* [in] */ ArrayOf<IInterface*>* _tabs)
{
    Float nh = Elastos::Core::Math::FLOAT_MAX_VALUE;
    Boolean alltabs = FALSE;

    AutoPtr< ArrayOf<IInterface*> > tabs = _tabs;
    if (text != NULL && text->Probe(EIID_ISpanned)) {
        if (tabs == NULL) {
            AutoPtr<ISpanned> spanned = (ISpanned*)(text->Probe(EIID_ISpanned));
            tabs = GetParagraphSpans(spanned, start, end, EIID_ITabStopSpan);
            alltabs = TRUE;
        }

        Int32 length = tabs->GetLength();
        for (Int32 i = 0; i < length; i++) {
            if (!alltabs) {
                if ((*tabs)[i] == NULL || (*tabs)[i]->Probe(EIID_ITabStopSpan) == NULL)
                    continue;
            }

            Int32 where;
            ITabStopSpan* tss = (ITabStopSpan*)((*tabs)[i]->Probe(EIID_ITabStopSpan));
            tss->GetTabStop(&where);

            if (where < nh && where > h)
                nh = where;
        }

        if (nh != Elastos::Core::Math::FLOAT_MAX_VALUE)
            return nh;
    }

    return ((Int32) ((h + TAB_INCREMENT) / TAB_INCREMENT)) * TAB_INCREMENT;
}

Boolean Layout::IsSpanned()
{
    return mSpannedText;
}

AutoPtr< ArrayOf<IInterface*> > Layout::GetParagraphSpans(
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type)
{
    AutoPtr< ArrayOf<IInterface*> > tmp;
    if (start == end && start > 0) {
        tmp = ArrayOf<IInterface*>::Alloc(0);
        return tmp;
//        return ArrayUtils::EmptyArray(type);
    }

    text->GetSpans(start, end, type, (ArrayOf<IInterface*>**)&tmp);
    return tmp;
}

Char32 Layout::GetEllipsisChar(
    /* [in] */ TextUtilsTruncateAt method)
{
    return (method == TextUtilsTruncateAt_END_SMALL) ?
            ELLIPSIS_TWO_DOTS[0] :
            ELLIPSIS_NORMAL[0];
}

void Layout::Ellipsize(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 line,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff,
    /* [in] */ TextUtilsTruncateAt method)
{
    Int32 ellipsisCount = GetEllipsisCount(line);

    if (ellipsisCount == 0) {
        return;
    }

    Int32 ellipsisStart = GetEllipsisStart(line);
    Int32 linestart = GetLineStart(line);

    for (Int32 i = ellipsisStart; i < ellipsisStart + ellipsisCount; i++) {
        Char32 c;

        if (i == ellipsisStart) {
            c = GetEllipsisChar(method); // ellipsis
        }
        else {
            c = 0xFEFF;//'\uFEFF'; // 0-width space
        }

        Int32 a = i + linestart;

        if (a >= start && a < end) {
            (*dest)[destoff + a - start] = c;
        }
    }
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
