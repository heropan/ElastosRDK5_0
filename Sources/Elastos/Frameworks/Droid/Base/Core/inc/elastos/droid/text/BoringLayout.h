#ifndef __ELASTOS_DROID_TEXT_BORINGLAYOUT_H__
#define __ELASTOS_DROID_TEXT_BORINGLAYOUT_H__

#include "elastos/droid/text/Layout.h"
#include "elastos/droid/text/CTextPaint.h"

namespace Elastos {
namespace Droid {
namespace Text {

/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
 * A BoringLayout is a very simple Layout implementation for text that
 * fits on a single line and is all left-to-right characters.
 * You will probably never want to make one of these yourself;
 * if you do, be sure to call {@link #isBoring} first to make sure
 * the text meets the criteria.
 * <p>This class is used by widgets to control text layout. You should not need
 * to use this class directly unless you are implementing your own widget
 * or custom display object, in which case
 * you are encouraged to use a Layout instead of calling
 * {@link android.graphics.Canvas#drawText(java.lang.CharSequence, int, int, float, float, android.graphics.Paint)
 *  Canvas.drawText()} directly.</p>
 */
class BoringLayout : public Layout
{
public:
    BoringLayout();

    BoringLayout(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    BoringLayout(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

public:
    static CARAPI_(AutoPtr<IBoringLayout>) Make(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    static CARAPI_(AutoPtr<IBoringLayout>) Make(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    /**
     * Returns a BoringLayout for the specified text, potentially reusing
     * this one if it is already suitable.  The caller must make sure that
     * no one is still using this Layout.
     */
    CARAPI ReplaceOrMake(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [out] */ IBoringLayout** layout);

    /**
     * Returns a BoringLayout for the specified text, potentially reusing
     * this one if it is already suitable.  The caller must make sure that
     * no one is still using this Layout.
     */
    CARAPI ReplaceOrMake(
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
        /* [out] */ IBoringLayout** layout);

    /* package */
    CARAPI_(void) Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ Boolean trustWidth);

    /**
     * Returns null if not boring; the width, ascent, and descent if boring.
     */
    static CARAPI_(AutoPtr<IBoringLayoutMetrics>) IsBoring(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint);

    /**
     * Returns null if not boring; the width, ascent, and descent if boring.
     * @hide
     */
    static CARAPI_(AutoPtr<IBoringLayoutMetrics>) IsBoring(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextDirectionHeuristic* textDir);

    /**
     * Returns null if not boring; the width, ascent, and descent in the
     * provided Metrics object (or a new one if the provided one was null)
     * if boring.
     */
    static CARAPI_(AutoPtr<IBoringLayoutMetrics>) IsBoring(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ IBoringLayoutMetrics* metrics);

    /**
     * Returns null if not boring; the width, ascent, and descent in the
     * provided Metrics object (or a new one if the provided one was null)
     * if boring.
     * @hide
     */
    static CARAPI_(AutoPtr<IBoringLayoutMetrics>) IsBoring(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextDirectionHeuristic* textDir,
        /* [in] */ IBoringLayoutMetrics* metrics);

    //@Override
    CARAPI_(Int32) GetHeight();

    //@Override
    CARAPI_(Int32) GetLineCount();

    //@Override
    CARAPI_(Int32) GetLineTop(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetLineDescent(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetLineStart(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetParagraphDirection(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Boolean) GetLineContainsTab(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Float) GetLineMax(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(AutoPtr<ILayoutDirections>) GetLineDirections(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetTopPadding();

    //@Override
    CARAPI_(Int32) GetBottomPadding();

    //@Override
    CARAPI_(Int32) GetEllipsisCount(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetEllipsisStart(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetEllipsizedWidth();

    // Override draw so it will be faster.
    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* c,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightpaint,
        /* [in] */ Int32 cursorOffset);

    /**
     * Callback for the ellipsizer to report what region it ellipsized.
     */
    CARAPI Ellipsized(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

public:
    /* package */ Int32 mBottom, mDesc;   // for Direct

protected:
    CARAPI _Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    CARAPI _Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

private:
    static const Char32 FIRST_RIGHT_TO_LEFT;// = '\u0590';

    String mDirect;
    AutoPtr<IPaint> mPaint;

    Int32 mTopPadding, mBottomPadding;
    Float mMax;
    Int32 mEllipsizedWidth, mEllipsizedStart, mEllipsizedCount;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_BORINGLAYOUT_H__
