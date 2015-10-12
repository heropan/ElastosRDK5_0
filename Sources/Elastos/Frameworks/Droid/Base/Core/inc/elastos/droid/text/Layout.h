#ifndef __ELASTOS_DROID_TEXT_LAYOUT_H__
#define __ELASTOS_DROID_TEXT_LAYOUT_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/SpanSet.h"

using Elastos::Core::ICharSequence;
//using Elastos::Droid::Emoji::IEmojiFactory;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPath;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Text::Style::IParagraphStyle;
using Elastos::Droid::Text::Style::ILineBackgroundSpan;


namespace Elastos {
namespace Droid {
namespace Text {

/**
 * A base class that manages text layout in visual elements on
 * the screen.
 * <p>For text that will be edited, use a {@link DynamicLayout},
 * which will be updated as the text changes.
 * For text that will not change, use a {@link StaticLayout}.
 */
class Layout
    : public Object
    , public ILayout
{
public:
    /**
     * @hide
     */
    /* package */
    class TabStops
        : public Object
    {
    public:
        static CARAPI_(Float) NextDefaultStop(
            /* [in] */ Float h,
            /* [in] */ Int32 inc);

        TabStops(
            /* [in] */ Int32 increment,
            /* [in] */ ArrayOf<IInterface*>* spans);

        virtual CARAPI_(void) Reset(
            /* [in] */ Int32 increment,
            /* [in] */ ArrayOf<IInterface*>* spans);

        virtual CARAPI_(Float) NextTab(
            /* [in] */ Float h);

    private:
        AutoPtr< ArrayOf<Int32> > mStops;
        Int32 mNumStops;
        Int32 mIncrement;
    };

    /* package */
    class Ellipsizer
        : public Object
        , public IGetChars
    {
    public:
        CAR_INTERFACE_DECL()

        Ellipsizer(
            /* [in] */ ICharSequence* s);

        virtual CARAPI GetCharAt(
            /* [in] */ Int32 off,
            /* [out] */ Char32* ch);

        virtual CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ ArrayOf<Char32>* dest,
            /* [in] */ Int32 destoff);

        virtual CARAPI GetLength(
            /* [in] */ Int32* length);

        virtual CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** subcsq);

        virtual CARAPI ToString(
            /* [out] */ String* str);

    public:
        /* package */ AutoPtr<ICharSequence> mText;
        /* package */ Layout* mLayout;
        /* package */ Int32 mWidth;
        /* package */ TextUtilsTruncateAt mMethod;
    };

    /* package */
    class SpannedEllipsizer
        : public Ellipsizer
        , public ISpanned
    {
    public:
        CAR_INTERFACE_DECL()

        SpannedEllipsizer(
            /* [in] */ ICharSequence* display);

        virtual CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ ArrayOf<Char32>* dest,
            /* [in] */ Int32 destoff);

        virtual CARAPI GetLength(
            /* [in] */ Int32* length);

        virtual CARAPI GetCharAt(
            /* [in] */ Int32 off,
            /* [out] */ Char32* ch);

        virtual CARAPI ToString(
            /* [out] */ String* str);

        virtual CARAPI GetSpans(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ const InterfaceID& type,
            /* [out, callee] */ ArrayOf<IInterface*>** objs);

        virtual CARAPI GetSpanStart(
            /* [in] */ IInterface* tag,
            /* [out] */ Int32* start);

        virtual CARAPI GetSpanEnd(
            /* [in] */ IInterface* tag,
            /* [out] */ Int32* end);

        virtual CARAPI GetSpanFlags(
            /* [in] */ IInterface* tag,
            /* [out] */ Int32* flags);

        virtual CARAPI NextSpanTransition(
            /* [in] */ Int32 start,
            /* [in] */ Int32 limit,
            /* [in] */ const InterfaceID& type,
            /* [out] */ Int32* offset);

        virtual CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** subcsq);

    private:
        AutoPtr<ISpanned> mSpanned;
    };

public:
    CAR_INTERFACE_DECL()

    Layout();

    virtual ~Layout();

    /**
     * Return how wide a layout must be in order to display the
     * specified text with one line per paragraph.
     */
    static CARAPI_(Float) GetDesiredWidth(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint);

    /**
     * Return how wide a layout must be in order to display the
     * specified text slice with one line per paragraph.
     */
    static CARAPI_(Float) GetDesiredWidth(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ITextPaint* paint);

    /**
     * Replace constructor properties of this Layout with new ones.  Be careful.
     */
    virtual CARAPI ReplaceWith(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd);

    /**
     * Draw this Layout on the specified Canvas.
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* c);

    /**
     * Draw this Layout on the specified canvas, with the highlight path drawn
     * between the background and the text.
     *
     * @param canvas the canvas
     * @param highlight the path of the highlight or cursor; can be null
     * @param highlightPaint the paint for the highlight
     * @param cursorOffsetVertical the amount to temporarily translate the
     *        canvas while rendering the highlight
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightPaint,
        /* [in] */ Int32 cursorOffsetVertical);

    /**
     * @hide
     */
    virtual CARAPI DrawText(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 firstLine,
        /* [in] */ Int32 lastLine);

    /**
     * @hide
     */
    virtual CARAPI DrawBackground(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightPaint,
        /* [in] */ Int32 cursorOffsetVertical,
        /* [in] */ Int32 firstLine,
        /* [in] */ Int32 lastLine);

    /**
     * @param canvas
     * @return The range of lines that need to be drawn, possibly empty.
     * @hide
     */
    virtual CARAPI_(Int64) GetLineRangeForDraw(
        /* [in] */ ICanvas* canvas);

    /**
     * Return the text that is displayed by this Layout.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetText();

    /**
     * Return the base Paint properties for this layout.
     * Do NOT change the paint, which may result in funny
     * drawing for this layout.
     */
    virtual CARAPI_(AutoPtr<ITextPaint>) GetPaint();

    /**
     * Return the width of this layout.
     */
    virtual CARAPI_(Int32) GetWidth();

    /**
     * Return the width to which this Layout is ellipsizing, or
     * {@link #getWidth} if it is not doing anything special.
     */
    virtual CARAPI_(Int32) GetEllipsizedWidth();

    /**
     * Increase the width of this layout to the specified width.
     * Be careful to use this only when you know it is appropriate&mdash;
     * it does not cause the text to reflow to use the full new width.
     */
    virtual CARAPI IncreaseWidthTo(
        /* [in] */ Int32 wid);

    /**
     * Return the total height of this layout.
     */
    virtual CARAPI_(Int32) GetHeight();

    /**
     * Return the base alignment of this layout.
     */
    virtual CARAPI_(LayoutAlignment) GetAlignment();

    /**
     * Return what the text height is multiplied by to get the line height.
     */
    virtual CARAPI_(Float) GetSpacingMultiplier();

    /**
     * Return the number of units of leading that are added to each line.
     */
    virtual CARAPI_(Float) GetSpacingAdd();

    /**
     * Return the heuristic used to determine paragraph text direction.
     * @hide
     */
    virtual CARAPI_(AutoPtr<ITextDirectionHeuristic>) GetTextDirectionHeuristic();

    /**
     * Return the number of lines of text in this layout.
     */
    virtual CARAPI_(Int32) GetLineCount() = 0;

    /**
     * Return the baseline for the specified line (0&hellip;getLineCount() - 1)
     * If bounds is not null, return the top, left, right, bottom extents
     * of the specified line in it.
     * @param line which line to examine (0..getLineCount() - 1)
     * @param bounds Optional. If not null, it returns the extent of the line
     * @return the Y-coordinate of the baseline
     */
    virtual CARAPI_(Int32) GetLineBounds(
        /* [in] */ Int32 line,
        /* [in] */ IRect* bounds);

    /**
     * Return the vertical position of the top of the specified line
     * (0&hellip;getLineCount()).
     * If the specified line is equal to the line count, returns the
     * bottom of the last line.
     */
    virtual CARAPI_(Int32) GetLineTop(
        /* [in] */ Int32 line) = 0;

    /**
     * Return the descent of the specified line(0&hellip;getLineCount() - 1).
     */
    virtual CARAPI_(Int32) GetLineDescent(
        /* [in] */ Int32 line) = 0;

    /**
     * Return the text offset of the beginning of the specified line (
     * 0&hellip;getLineCount()). If the specified line is equal to the line
     * count, returns the length of the text.
     */
    virtual CARAPI_(Int32) GetLineStart(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the primary directionality of the paragraph containing the
     * specified line, either 1 for left-to-right lines, or -1 for right-to-left
     * lines (see {@link #DIR_LEFT_TO_RIGHT}, {@link #DIR_RIGHT_TO_LEFT}).
     */
    virtual CARAPI_(Int32) GetParagraphDirection(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns whether the specified line contains one or more
     * characters that need to be handled specially, like tabs
     * or emoji.
     */
    virtual CARAPI_(Boolean) GetLineContainsTab(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the directional run information for the specified line.
     * The array alternates counts of characters in left-to-right
     * and right-to-left segments of the line.
     *
     * <p>NOTE: this is inadequate to support bidirectional text, and will change.
     */
    virtual CARAPI_(AutoPtr<ILayoutDirections>) GetLineDirections(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the (negative) number of extra pixels of ascent padding in the
     * top line of the Layout.
     */
    virtual CARAPI_(Int32) GetTopPadding() = 0;

    /**
     * Returns the number of extra pixels of descent padding in the
     * bottom line of the Layout.
     */
    virtual CARAPI_(Int32) GetBottomPadding() = 0;

    /**
     * Returns true if the character at offset and the preceding character
     * are at different run levels (and thus there's a split caret).
     * @param offset the offset
     * @return true if at a level boundary
     * @hide
     */
    virtual CARAPI_(Boolean) IsLevelBoundary(
        /* [in] */ Int32 offset);

    /**
     * Returns true if the character at offset is right to left (RTL).
     * @param offset the offset
     * @return true if the character is RTL, false if it is LTR
     */
    virtual CARAPI_(Boolean) IsRtlCharAt(
        /* [in] */ Int32 offset);

    /**
     * Get the primary horizontal position for the specified text offset.
     * This is the location where a new character would be inserted in
     * the paragraph's primary direction.
     */
    virtual CARAPI_(Float) GetPrimaryHorizontal(
        /* [in] */ Int32 offset);

    /**
     * Get the primary horizontal position for the specified text offset, but
     * optionally clamp it so that it doesn't exceed the width of the layout.
     * @hide
     */
    CARAPI_(Float) GetPrimaryHorizontal(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean clamped);

    /**
     * Get the secondary horizontal position for the specified text offset.
     * This is the location where a new character would be inserted in
     * the direction other than the paragraph's primary direction.
     */
    virtual CARAPI_(Float) GetSecondaryHorizontal(
        /* [in] */ Int32 offset);

    /**
     * Get the secondary horizontal position for the specified text offset, but
     * optionally clamp it so that it doesn't exceed the width of the layout.
     * @hide
     */
    CARAPI_(Float) GetSecondaryHorizontal(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean clamped);

    /**
     * Get the leftmost position that should be exposed for horizontal
     * scrolling on the specified line.
     */
    virtual CARAPI_(Float) GetLineLeft(
        /* [in] */ Int32 line);

    /**
     * Get the rightmost position that should be exposed for horizontal
     * scrolling on the specified line.
     */
    virtual CARAPI_(Float) GetLineRight(
        /* [in] */ Int32 line);

    /**
     * Gets the unsigned horizontal extent of the specified line, including
     * leading margin indent, but excluding trailing whitespace.
     */
    virtual CARAPI_(Float) GetLineMax(
        /* [in] */ Int32 line);

    /**
     * Gets the unsigned horizontal extent of the specified line, including
     * leading margin indent and trailing whitespace.
     */
    virtual CARAPI_(Float) GetLineWidth(
        /* [in] */ Int32 line);

    /**
     * Get the line number corresponding to the specified vertical position.
     * If you ask for a position above 0, you get 0; if you ask for a position
     * below the bottom of the text, you get the last line.
     */
    // FIXME: It may be faster to do a linear search for layouts without many lines.
    virtual CARAPI_(Int32) GetLineForVertical(
        /* [in] */ Int32 vertical);

    /**
     * Get the line number on which the specified text offset appears.
     * If you ask for a position before 0, you get 0; if you ask for a position
     * beyond the end of the text, you get the last line.
     */
    virtual CARAPI_(Int32) GetLineForOffset(
        /* [in] */ Int32 offset);

    /**
     * Get the character offset on the specified line whose position is
     * closest to the specified horizontal position.
     */
    virtual CARAPI_(Int32) GetOffsetForHorizontal(
        /* [in] */ Int32 line,
        /* [in] */ Float horiz);

    /**
     * Return the text offset after the last character on the specified line.
     */
    virtual CARAPI_(Int32) GetLineEnd(
        /* [in] */ Int32 line);

    /**
     * Return the text offset after the last visible character (so whitespace
     * is not counted) on the specified line.
     */
    virtual CARAPI_(Int32) GetLineVisibleEnd(
        /* [in] */ Int32 line);

    /**
     * Return the vertical position of the bottom of the specified line.
     */
    virtual CARAPI_(Int32) GetLineBottom(
        /* [in] */ Int32 line);

    /**
     * Return the vertical position of the baseline of the specified line.
     */
    virtual CARAPI_(Int32) GetLineBaseline(
        /* [in] */ Int32 line);

    /**
     * Get the ascent of the text on the specified line.
     * The return value is negative to match the Paint.ascent() convention.
     */
    virtual CARAPI_(Int32) GetLineAscent(
        /* [in] */ Int32 line);

    virtual CARAPI_(Int32) GetOffsetToLeftOf(
        /* [in] */ Int32 offset);

    virtual CARAPI_(Int32) GetOffsetToRightOf(
        /* [in] */ Int32 offset);

    /**
     * Determine whether we should clamp cursor position. Currently it's
     * only robust for left-aligned displays.
     * @hide
     */
    virtual CARAPI_(Boolean) ShouldClampCursor(
        /* [in] */ Int32 line);

    /**
     * Fills in the specified Path with a representation of a cursor
     * at the specified offset.  This will often be a vertical line
     * but can be multiple discontinuous lines in text with multiple
     * directionalities.
     */
    virtual CARAPI GetCursorPath(
        /* [in] */ Int32 point,
        /* [in] */ IPath* dest,
        /* [in] */ ICharSequence* editingBuffer);

    /**
     * Fills in the specified Path with a representation of a highlight
     * between the specified offsets.  This will often be a rectangle
     * or a potentially discontinuous set of rectangles.  If the start
     * and end are the same, the returned path is empty.
     */
    virtual CARAPI GetSelectionPath(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPath* dest);

    /**
     * Get the alignment of the specified paragraph, taking into account
     * markup attached to it.
     */
    virtual CARAPI_(LayoutAlignment) GetParagraphAlignment(
        /* [in] */ Int32 line);

    /**
     * Get the left edge of the specified paragraph, inset by left margins.
     */
    virtual CARAPI_(Int32) GetParagraphLeft(
        /* [in] */ Int32 line);

    /**
     * Get the right edge of the specified paragraph, inset by right margins.
     */
    virtual CARAPI_(Int32) GetParagraphRight(
        /* [in] */ Int32 line);

    /* package */
    static CARAPI_(Float) MeasurePara(
        /* [in] */ ITextPaint* paint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Returns the position of the next tab stop after h on the line.
     *
     * @param text the text
     * @param start start of the line
     * @param end limit of the line
     * @param h the current horizontal offset
     * @param tabs the tabs, can be null.  If it is null, any tabs in effect
     * on the line will be used.  If there are no tabs, a default offset
     * will be used to compute the tab stop.
     * @return the offset of the next tab stop.
     */
    static CARAPI_(Float) NextTab(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float h,
        /* [in] */ ArrayOf<IInterface*>* tabs);

     /**
     * Returns the same as <code>text.getSpans()</code>, except where
     * <code>start</code> and <code>end</code> are the same and are not
     * at the very beginning of the text, in which case an empty array
     * is returned instead.
     * <p>
     * This is needed because of the special case that <code>getSpans()</code>
     * on an empty range returns the spans adjacent to that range, which is
     * primarily for the sake of <code>TextWatchers</code> so they will get
     * notifications when text goes from empty to non-empty.  But it also
     * has the unfortunate side effect that if the text ends with an empty
     * paragraph, that paragraph accidentally picks up the styles of the
     * preceding paragraph (even though those styles will not be picked up
     * by new text that is inserted into the empty paragraph).
     * <p>
     * The reason it just checks whether <code>start</code> and <code>end</code>
     * is the same is that the only time a line can contain 0 characters
     * is if it is the final paragraph of the Layout; otherwise any line will
     * contain at least one printing or newline character.  The reason for the
     * additional check if <code>start</code> is greater than 0 is that
     * if the empty paragraph is the entire content of the buffer, paragraph
     * styles that are already applied to the buffer will apply to text that
     * is inserted into it.
     */
    static CARAPI_(AutoPtr< ArrayOf<IInterface*> >) GetParagraphSpans(
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const InterfaceID& type);

    /**
     * Return the offset of the first character to be ellipsized away,
     * relative to the start of the line.  (So 0 if the beginning of the
     * line is ellipsized, not getLineStart().)
     */
    virtual CARAPI_(Int32) GetEllipsisStart(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the number of characters to be ellipsized away, or 0 if
     * no ellipsis is to take place.
     */
    virtual CARAPI_(Int32) GetEllipsisCount(
        /* [in] */ Int32 line) = 0;

protected:
    /**
     * Subclasses of Layout use this constructor to set the display text,
     * width, and other standard properties.
     * @param text the text to render
     * @param paint the default paint for the layout.  Styles can override
     * various attributes of the paint.
     * @param width the wrapping width for the text.
     * @param align whether to left, right, or center the text.  Styles can
     * override the alignment.
     * @param spacingMult factor by which to scale the font size to get the
     * default line spacing
     * @param spacingAdd amount to add to the default line spacing
     */
    Layout(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingMult,
        /* [in] */ Float spacingAdd);

    /**
     * Subclasses of Layout use this constructor to set the display text,
     * width, and other standard properties.
     * @param text the text to render
     * @param paint the default paint for the layout.  Styles can override
     * various attributes of the paint.
     * @param width the wrapping width for the text.
     * @param align whether to left, right, or center the text.  Styles can
     * override the alignment.
     * @param spacingMult factor by which to scale the font size to get the
     * default line spacing
     * @param spacingAdd amount to add to the default line spacing
     *
     * @hide
     */
    Layout(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ ITextDirectionHeuristic* textDir,
        /* [in] */ Float spacingMult,
        /* [in] */ Float spacingAdd);

    virtual CARAPI Init(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingMult,
        /* [in] */ Float spacingAdd);

    virtual CARAPI Init(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ ITextDirectionHeuristic* textDir,
        /* [in] */ Float spacingMult,
        /* [in] */ Float spacingAdd);

    virtual CARAPI_(Boolean) IsSpanned();

private:
    /**
     * Return the start position of the line, given the left and right bounds
     * of the margins.
     *
     * @param line the line index
     * @param left the left bounds (0, or leading margin if ltr para)
     * @param right the right bounds (width, minus leading margin if rtl para)
     * @return the start position of the line (to right of line if rtl para)
     */
    CARAPI_(Int32) GetLineStartPos(
        /* [in] */ Int32 line,
        /* [in] */ Int32 left,
        /* [in] */ Int32 right);

    CARAPI_(Boolean) PrimaryIsTrailingPrevious(
        /* [in] */ Int32 offset);

    CARAPI_(Float) GetHorizontal(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean trailing,
        /* [in] */ Boolean clamped);

    CARAPI_(Float) GetHorizontal(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean trailing,
        /* [in] */ Int32 line,
        /* [in] */ Boolean clamped);

    /**
     * Like {@link #getLineExtent(int,TabStops,boolean)} but determines the
     * tab stops instead of using the ones passed in.
     * @param line the index of the line
     * @param full whether to include trailing whitespace
     * @return the extent of the line
     */
    CARAPI_(Float) GetLineExtent(
        /* [in] */ Int32 line,
        /* [in] */ Boolean full);

    /**
     * Returns the signed horizontal extent of the specified line, excluding
     * leading margin.  If full is false, excludes trailing whitespace.
     * @param line the index of the line
     * @param tabStops the tab stops, can be null if we know they're not used.
     * @param full whether to include trailing whitespace
     * @return the extent of the text on this line
     */
    CARAPI_(Float) GetLineExtent(
        /* [in] */ Int32 line,
        /* [in] */ TabStops* tabStops,
        /* [in] */ Boolean full);

    CARAPI_(Int32) GetLineVisibleEnd(
        /* [in] */ Int32 line,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Int32) GetOffsetToLeftRightOf(
        /* [in] */ Int32 caret,
        /* [in] */ Boolean toLeft);

    CARAPI_(Int32) GetOffsetAtStartOf(
        /* [in] */ Int32 offset);

    CARAPI_(void) AddSelection(
        /* [in] */ Int32 line,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom,
        /* [in] */ IPath* dest);

    /**
     * Returns the effective leading margin (unsigned) for this line,
     * taking into account LeadingMarginSpan and LeadingMarginSpan2.
     * @param line the line index
     * @return the leading margin of this line
     */
    CARAPI_(Int32) GetParagraphLeadingMargin(
        /* [in] */ Int32 line);

    CARAPI_(Char32) GetEllipsisChar(
        /* [in] */ TextUtilsTruncateAt method);

    CARAPI_(void) Ellipsize(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 line,
        /* [in] */ ArrayOf<Char32>* dest,
        /* [in] */ Int32 destoff,
        /* [in] */ TextUtilsTruncateAt method);

public:
//    /* package */ static AutoPtr<IEmojiFactory> EMOJI_FACTORY;
    /* package */ static Int32 MIN_EMOJI, MAX_EMOJI;

    /* package */ static AutoPtr<ILayoutDirections> DIRS_ALL_LEFT_TO_RIGHT;
    /* package */ static AutoPtr<ILayoutDirections> DIRS_ALL_RIGHT_TO_LEFT;

    /* package */ static const Char32 ELLIPSIS_NORMAL[];//{ '\u2026' }; // this is "..."
    /* package */ static const Char32 ELLIPSIS_TWO_DOTS[];//{ '\u2025' }; // this is ".."

    /* package */ AutoPtr<ITextPaint> mWorkPaint;

private:
    static AutoPtr< ArrayOf<IParagraphStyle*> > NO_PARA_SPANS;
    static const Int32 TAB_INCREMENT = 20;
    static AutoPtr<IRect> sTempRect;
    static Object sTempRectLock;

    AutoPtr<ICharSequence> mText;
    AutoPtr<ITextPaint> mPaint;

    Int32 mWidth;
    LayoutAlignment mAlignment;
    Float mSpacingMult;
    Float mSpacingAdd;

    Boolean mSpannedText;
    AutoPtr<ITextDirectionHeuristic> mTextDir;
    AutoPtr< SpanSet<ILineBackgroundSpan> > mLineBackgroundSpans;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_LAYOUT_H__
