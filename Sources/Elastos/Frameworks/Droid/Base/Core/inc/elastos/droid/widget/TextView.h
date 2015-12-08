#ifndef __ELASTOS_DROID_WIDGET_TEXTVIEW_H__
#define __ELASTOS_DROID_WIDGET_TEXTVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Runnable.h"
#include "elastos/droid/view/View.h"

using Elastos::Droid::Content::IUndoManager;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Graphics::IPath;
using Elastos::Droid::Text::ITextPaint;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::ISpanned;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::IGetChars;
using Elastos::Droid::Text::ILayout;
using Elastos::Droid::Text::IBoringLayout;
using Elastos::Droid::Text::IGraphicsOperations;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Text::ISpanWatcher;
using Elastos::Droid::Text::IEditableFactory;
using Elastos::Droid::Text::ISpannableFactory;
using Elastos::Droid::Text::LayoutAlignment;
using Elastos::Droid::Text::TextUtilsTruncateAt;
using Elastos::Droid::Text::ITextDirectionHeuristic;
using Elastos::Droid::Text::IBoringLayoutMetrics;
using Elastos::Droid::Text::Method::IKeyListener;
using Elastos::Droid::Text::Method::IMovementMethod;
using Elastos::Droid::Text::Method::ITransformationMethod;
using Elastos::Droid::Text::Method::IWordIterator;
using Elastos::Droid::Text::Style::IURLSpan;
using Elastos::Droid::View::View;
using Elastos::Droid::View::IDragEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IChoreographer;
using Elastos::Droid::View::IFrameCallback;
using Elastos::Droid::View::IOnPreDrawListener;
using Elastos::Droid::View::ITextSegmentIterator;
using Elastos::Droid::View::InputMethod::IExtractedTextRequest;
using Elastos::Droid::View::InputMethod::IExtractedText;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::InputMethod::ICorrectionInfo;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Widget {

class TextView;

//==============================================================================
//              UpdateTextServicesLocaleRunnable
//==============================================================================
class UpdateTextServicesLocaleRunnable
    : public Runnable
{
public:
    UpdateTextServicesLocaleRunnable(
        /* [in] */ TextView* textView);

    virtual ECode Run();

protected:
    AutoPtr<IWeakReference> mTextView;
};

//==============================================================================
//              EditorRunnable
//==============================================================================
class CompressTextRunnable
    : public Runnable
{
public:
    CompressTextRunnable(
        /* [in] */ TextView* textView);

    virtual ECode Run();

protected:
    AutoPtr<IWeakReference> mTextView;
};

//==============================================================================
//          Drawables
//==============================================================================
class Drawables
    : public Object
{
public:
    Drawables();

    Drawables(
        /* [in] */ IContext* context);

    CARAPI ResolveWithLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    CARAPI SetErrorDrawable(
        /* [in] */ IDrawable* dr,
        /* [in] */ ITextView*tv);

private:
    CARAPI_(void) UpdateDrawablesLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    CARAPI_(void) ApplyErrorDrawableIfNeeded(
        /* [in] */ Int32 layoutDirection);

public:
    AutoPtr<IRect> mCompoundRect;

    AutoPtr<IDrawable> mDrawableTop;
    AutoPtr<IDrawable> mDrawableBottom;
    AutoPtr<IDrawable> mDrawableLeft;
    AutoPtr<IDrawable> mDrawableRight;
    AutoPtr<IDrawable> mDrawableStart;
    AutoPtr<IDrawable> mDrawableEnd;
    AutoPtr<IDrawable> mDrawableError;
    AutoPtr<IDrawable> mDrawableTemp;

    AutoPtr<IDrawable> mDrawableLeftInitial;
    AutoPtr<IDrawable> mDrawableRightInitial;

    Boolean mIsRtlCompatibilityMode;
    Boolean mOverride;

    Int32 mDrawableSizeTop;
    Int32 mDrawableSizeBottom;
    Int32 mDrawableSizeLeft;
    Int32 mDrawableSizeRight;
    Int32 mDrawableSizeStart;
    Int32 mDrawableSizeEnd;
    Int32 mDrawableSizeError;
    Int32 mDrawableSizeTemp;

    Int32 mDrawableWidthTop;
    Int32 mDrawableWidthBottom;
    Int32 mDrawableHeightLeft;
    Int32 mDrawableHeightRight;
    Int32 mDrawableHeightStart;
    Int32 mDrawableHeightEnd;
    Int32 mDrawableHeightError;
    Int32 mDrawableHeightTemp;

    Int32 mDrawablePadding;
    Int32 mDrawableSaved;

    static const Int32 DRAWABLE_NONE = -1;
    static const Int32 DRAWABLE_RIGHT = 0;
    static const Int32 DRAWABLE_LEFT = 1;
};

//==============================================================================
//          CharWrapper
//==============================================================================
class CharWrapper
    : public Object
    , public ICharSequence
    , public IGetChars
    , public IGraphicsOperations
{
public:
    CAR_INTERFACE_DECL()

    CharWrapper(
        /* [in] */ const ArrayOf<Char32>* chars,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len);

    CARAPI_(void) Set(
        /* [in] */ const ArrayOf<Char32>* chars,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len);

    CARAPI GetLength(
        /* [out] */ Int32* len);

    CARAPI GetCharAt(
        /* [in] */ Int32 off,
        /* [out] */ Char32* c);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Char32>* dest,
        /* [in] */ Int32 destoff);

    CARAPI DrawText(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* p);

    CARAPI DrawTextRun(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Boolean isRtl,
        /* [in] */ IPaint* p);

    CARAPI MeasureText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaint* p,
        /* [out] */ Float* width);

    CARAPI GetTextWidths(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Float>** widths,
        /* [in] */ IPaint* p,
        /* [out] */ Int32* count);

    CARAPI GetTextRunAdvances(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Boolean isRtl,
        /* [out] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [in] */ IPaint* p,
        /* [out] */ Float* advance);

    CARAPI GetTextRunCursor(
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [in] */ IPaint* p,
        /* [out] */ Int32* cursor);

private:
    friend class TextView;

    AutoPtr<ArrayOf<Char32> > mChars;
    Int32 mStart;
    Int32 mLength;
};

//==============================================================================
//          Marquee
//==============================================================================
class Marquee
    : public Object
{
private:
    class TickCallback
        : public Object
        , public IFrameCallback
    {
    public:
        TickCallback(
            /* [in] */ Marquee* host);

        CAR_INTERFACE_DECL()

        CARAPI DoFrame(
            /* [in] */ Int64 frameTimeNanos);
    private:
        Marquee* mHost;
    };

    class StartCallback
        : public Object
        , public IFrameCallback
    {
    public:
        StartCallback(
            /* [in] */ Marquee* host);

        CAR_INTERFACE_DECL()

        CARAPI DoFrame(
            /* [in] */ Int64 frameTimeNanos);
    private:
        Marquee* mHost;
    };

    class RestartCallback
        : public Object
        , public IFrameCallback
    {
    public:
        RestartCallback(
            /* [in] */ Marquee* host);

        CAR_INTERFACE_DECL()

        CARAPI DoFrame(
            /* [in] */ Int64 frameTimeNanos);
    private:
        Marquee* mHost;
    };

public:
    Marquee(
        /* [in] */ ITextView* v);

    CARAPI_(void) Tick();

    CARAPI_(void) Stop();

    CARAPI_(void) Start(
        /* [in] */ Int32 repeatLimit);

    CARAPI_(Float) GetGhostOffset();

    CARAPI_(Float) GetScroll();

    CARAPI_(Float) GetMaxFadeScroll();

    CARAPI_(Boolean) ShouldDrawLeftFade();

    CARAPI_(Boolean) ShouldDrawGhost();

    CARAPI_(Boolean) IsRunning();

    CARAPI_(Boolean) IsStopped();

protected:
    CARAPI_(void) ResetScroll();

private:
    friend class TextView;

    // TODO: Add an option to configure this
    static const Float MARQUEE_DELTA_MAX = 0.07f;
    static const Int32 MARQUEE_DELAY = 1200;
    static const Int32 MARQUEE_RESTART_DELAY = 1200;
    static const Int32 MARQUEE_DP_PER_SECOND = 30;

    static const Byte MARQUEE_STOPPED = 0x0;
    static const Byte MARQUEE_STARTING = 0x1;
    static const Byte MARQUEE_RUNNING = 0x2;

    AutoPtr<IWeakReference> mView;  //TextView
    AutoPtr<IChoreographer> mChoreographer;
    AutoPtr<IFrameCallback> mTickCallback;
    AutoPtr<IFrameCallback> mStartCallback;
    AutoPtr<IFrameCallback> mRestartCallback;

    Byte mStatus;// = MARQUEE_STOPPED;
    Float mPixelsPerSecond;
    Float mMaxScroll;
    Float mMaxFadeScroll;
    Float mGhostStart;
    Float mGhostOffset;
    Float mFadeStop;
    Int32 mRepeatLimit;

    Float mScroll;
    Int64 mLastAnimationMs;
};

//==============================================================================
//          ChangeWatcher
//==============================================================================
class ChangeWatcher
    : public Object
    , public ITextWatcher
    , public ISpanWatcher
{
public:
    CAR_INTERFACE_DECL()

    ChangeWatcher(
        /* [in] */ TextView* host);

    ~ChangeWatcher();

    CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* buffer,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* buffer,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI AfterTextChanged(
        /* [in] */ IEditable* buffer);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* buf,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 s,
        /* [in] */ Int32 e,
        /* [in] */ Int32 st,
        /* [in] */ Int32 en);

    CARAPI OnSpanAdded(
        /* [in] */ ISpannable* buf,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 s,
        /* [in] */ Int32 e);

    CARAPI OnSpanRemoved(
        /* [in] */ ISpannable* buf,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 s,
        /* [in] */ Int32 e);

private:
    AutoPtr<ICharSequence> mBeforeText;
    TextView* mHost;
};

//==============================================================================
//          TextView
//==============================================================================

class UserDictionaryListener;

class TextView
    : public Elastos::Droid::View::View
    , public ITextView
    , public IOnPreDrawListener
{
public:
    TextView();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    static CARAPI_(void) InitStatic();

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) OnPreDraw();

    CARAPI OnPreDraw(
        /* [out] */ Boolean* res);

    /**
     * Sets the typeface and style in which the text should be displayed,
     * and turns on the fake bold and italic bits in the Paint if the
     * Typeface that you provided does not have all the bits in the
     * style that you specified.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    virtual CARAPI SetTypeface(
        /* [in] */ ITypeface* tf,
        /* [in] */ Int32 style);

    /**
     * Return the text the TextView is displaying. If setText() was called with
     * an argument of BufferType.SPANNABLE or BufferType.EDITABLE, you can cast
     * the return value from this method to Spannable or Editable, respectively.
     *
     * Note: The content of the return value should not be modified. If you want
     * a modifiable one, you should make your own copy first.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetText();

    CARAPI GetText(
        /* [out] */ ICharSequence** text);

    /**
     * Returns the length, in characters, of the text managed by this TextView
     */
    virtual CARAPI_(Int32) GetLength();

    CARAPI GetLength(
        /* [out] */ Int32* length);

    /**
     * Return the text the TextView is displaying as an Editable object.  If
     * the text is not editable, null is returned.
     *
     * @see #getText
     */
    virtual CARAPI_(AutoPtr<IEditable>) GetEditableText();

    CARAPI GetEditableText(
        /* [out] */ IEditable** editable);

    /**
     * @return the height of one standard line in pixels.  Note that markup
     * within the text can cause individual lines to be taller or shorter
     * than this height, and the layout may contain additional first-
     * or last-line padding.
     */
    virtual CARAPI_(Int32) GetLineHeight();

    CARAPI GetLineHeight(
        /* [out] */ Int32* height);

    /**
     * @return the Layout that is currently being used to display the text.
     * This can be null if the text or width has recently changes.
     */
    CARAPI_(AutoPtr<ILayout>) GetLayout();

    CARAPI GetLayout(
        /* [out] */ ILayout** layout);

    /**
     * @return the current key listener for this TextView.
     * This will frequently be null for non-EditText TextViews.
     */
    CARAPI_(AutoPtr<IKeyListener>) GetKeyListener();

    CARAPI GetKeyListener(
        /* [out] */ IKeyListener** listener);

    /**
     * Sets the key listener to be used with this TextView.  This can be null
     * to disallow user input.  Note that this method has significant and
     * subtle interactions with soft keyboards and other input method:
     * see {@link KeyListener#getInputType() KeyListener.getContentType()}
     * for important details.  Calling this method will replace the current
     * content type of the text view with the content type returned by the
     * key listener.
     * <p>
     * Be warned that if you want a TextView with a key listener or movement
     * method not to be focusable, or if you want a TextView without a
     * key listener or movement method to be focusable, you must call
     * {@link #setFocusable} again after calling this to get the focusability
     * back the way you want it.
     *
     * @attr ref android.R.styleable#TextView_numeric
     * @attr ref android.R.styleable#TextView_digits
     * @attr ref android.R.styleable#TextView_phoneNumber
     * @attr ref android.R.styleable#TextView_inputMethod
     * @attr ref android.R.styleable#TextView_capitalize
     * @attr ref android.R.styleable#TextView_autoText
     */
    virtual CARAPI SetKeyListener(
        /* [in] */ IKeyListener* input);

    /**
     * @return the movement method being used for this TextView.
     * This will frequently be null for non-EditText TextViews.
     */
    CARAPI_(AutoPtr<IMovementMethod>) GetMovementMethod();

    CARAPI GetMovementMethod(
        /* [out] */ IMovementMethod** movement);

    /**
     * Sets the movement method (arrow key handler) to be used for
     * this TextView.  This can be null to disallow using the arrow keys
     * to move the cursor or scroll the view.
     * <p>
     * Be warned that if you want a TextView with a key listener or movement
     * method not to be focusable, or if you want a TextView without a
     * key listener or movement method to be focusable, you must call
     * {@link #setFocusable} again after calling this to get the focusability
     * back the way you want it.
     */
    virtual CARAPI SetMovementMethod(
        /* [in] */ IMovementMethod* movement);

    /**
     * @return the current transformation method for this TextView.
     * This will frequently be null except for single-line and password
     * fields.
     */
    virtual CARAPI_(AutoPtr<ITransformationMethod>) GetTransformationMethod();

    CARAPI GetTransformationMethod(
        /* [out] */ ITransformationMethod** method);

    /**
     * Sets the transformation that is applied to the text that this
     * TextView is displaying.
     *
     * @attr ref android.R.styleable#TextView_password
     * @attr ref android.R.styleable#TextView_singleLine
     */
    virtual CARAPI SetTransformationMethod(
        /* [in] */ ITransformationMethod* method);

    /**
     * Returns the top padding of the view, plus space for the top
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingTop();

    CARAPI GetCompoundPaddingTop(
        /* [out] */ Int32* top);

    /**
     * Returns the bottom padding of the view, plus space for the bottom
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingBottom();

    CARAPI GetCompoundPaddingBottom(
        /* [out] */ Int32* bottom);

    /**
     * Returns the left padding of the view, plus space for the left
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingLeft();

    CARAPI GetCompoundPaddingLeft(
        /* [out] */ Int32* left);

    /**
     * Returns the right padding of the view, plus space for the right
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingRight();

    CARAPI GetCompoundPaddingRight(
        /* [out] */ Int32* right);

    /**
     * Returns the start padding of the view, plus space for the start
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingStart();

    CARAPI GetCompoundPaddingStart(
        /* [out] */ Int32* start);

    /**
     * Returns the end padding of the view, plus space for the end
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetCompoundPaddingEnd();

    CARAPI GetCompoundPaddingEnd(
        /* [out] */ Int32* end);

    /**
     * Returns the extended top padding of the view, including both the
     * top Drawable if any and any extra space to keep more than maxLines
     * of text from showing.  It is only valid to call this after measuring.
     */
    virtual CARAPI_(Int32) GetExtendedPaddingTop();

    CARAPI GetExtendedPaddingTop(
        /* [out] */ Int32* top);

    /**
     * Returns the extended bottom padding of the view, including both the
     * bottom Drawable if any and any extra space to keep more than maxLines
     * of text from showing.  It is only valid to call this after measuring.
     */
    virtual CARAPI_(Int32) GetExtendedPaddingBottom();

    CARAPI GetExtendedPaddingBottom(
        /* [out] */ Int32* bottom);

    /**
     * Returns the total left padding of the view, including the left
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingLeft();

    CARAPI GetTotalPaddingLeft(
        /* [out] */ Int32* left);

    /**
     * Returns the total right padding of the view, including the right
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingRight();

    CARAPI GetTotalPaddingRight(
        /* [out] */ Int32* right);

    /**
     * Returns the total start padding of the view, including the start
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingStart();

    CARAPI GetTotalPaddingStart(
        /* [out] */ Int32* start);

    /**
     * Returns the total end padding of the view, including the end
     * Drawable if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingEnd();

    CARAPI GetTotalPaddingEnd(
        /* [out] */ Int32* end);

    /**
     * Returns the total top padding of the view, including the top
     * Drawable if any, the extra space to keep more than maxLines
     * from showing, and the vertical offset for gravity, if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingTop();

    CARAPI GetTotalPaddingTop(
        /* [out] */ Int32* top);

    /**
     * Returns the total bottom padding of the view, including the bottom
     * Drawable if any, the extra space to keep more than maxLines
     * from showing, and the vertical offset for gravity, if any.
     */
    virtual CARAPI_(Int32) GetTotalPaddingBottom();

    CARAPI GetTotalPaddingBottom(
        /* [out] */ Int32* bottom);

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use null if you do not
     * want a Drawable there.  The Drawables must already have had
     * {@link Drawable#setBounds} called.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawables(
        /* [in] */ IDrawable* left,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* right,
        /* [in] */ IDrawable* bottom);

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use 0 if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @param left Resource identifier of the left Drawable.
     * @param top Resource identifier of the top Drawable.
     * @param right Resource identifier of the right Drawable.
     * @param bottom Resource identifier of the bottom Drawable.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesWithIntrinsicBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Sets the Drawables (if any) to appear to the left of, above,
     * to the right of, and below the text.  Use null if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @attr ref android.R.styleable#TextView_drawableLeft
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableRight
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesWithIntrinsicBounds(
        /* [in] */ IDrawable* left,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* right,
        /* [in] */ IDrawable* bottom);

    /**
     * Sets the Drawables (if any) to appear to the start of, above,
     * to the end of, and below the text.  Use null if you do not
     * want a Drawable there.  The Drawables must already have had
     * {@link Drawable#setBounds} called.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesRelative(
        /* [in] */ IDrawable* start,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* end,
        /* [in] */ IDrawable* bottom);

    /**
     * Sets the Drawables (if any) to appear to the start of, above,
     * to the end of, and below the text.  Use 0 if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @param start Resource identifier of the start Drawable.
     * @param top Resource identifier of the top Drawable.
     * @param end Resource identifier of the end Drawable.
     * @param bottom Resource identifier of the bottom Drawable.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesRelativeWithIntrinsicBounds(
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom);

    /**
     * Sets the Drawables (if any) to appear to the start of, above,
     * to the end of, and below the text.  Use null if you do not
     * want a Drawable there. The Drawables' bounds will be set to
     * their intrinsic bounds.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI SetCompoundDrawablesRelativeWithIntrinsicBounds(
        /* [in] */ IDrawable* start,
        /* [in] */ IDrawable* top,
        /* [in] */ IDrawable* end,
        /* [in] */ IDrawable* bottom);

    /**
     * Returns drawables for the left, top, right, and bottom borders.
     */
    virtual CARAPI_(AutoPtr<ArrayOf<IDrawable*> >) GetCompoundDrawables();

    CARAPI GetCompoundDrawables(
        /* [out, callee] */ ArrayOf<IDrawable*>** drawables);

    /**
     * Returns drawables for the start, top, end, and bottom borders.
     *
     * @attr ref android.R.styleable#TextView_drawableStart
     * @attr ref android.R.styleable#TextView_drawableTop
     * @attr ref android.R.styleable#TextView_drawableEnd
     * @attr ref android.R.styleable#TextView_drawableBottom
     */
    virtual CARAPI_(AutoPtr<ArrayOf<IDrawable*> >) GetCompoundDrawablesRelative();

    CARAPI GetCompoundDrawablesRelative(
        /* [out, callee] */ ArrayOf<IDrawable*>** drawables);

    /**
     * Sets the size of the padding between the compound drawables and
     * the text.
     *
     * @attr ref android.R.styleable#TextView_drawablePadding
     */
    virtual CARAPI SetCompoundDrawablePadding(
        /* [in] */ Int32 pad);

    /**
     * Returns the padding between the compound drawables and the text.
     */
    virtual CARAPI_(Int32) GetCompoundDrawablePadding();

    CARAPI GetCompoundDrawablePadding(
        /* [out] */ Int32* pad);

    /**
     * Gets the autolink mask of the text.  See {@link
     * android.text.util.Linkify#ALL Linkify.ALL} and peers for
     * possible values.
     *
     * @attr ref android.R.styleable#TextView_autoLink
     */
    virtual CARAPI_(Int32) GetAutoLinkMask();

    CARAPI GetAutoLinkMask(
        /* [out] */ Int32* mask);

    /**
     * Sets the text color, size, style, hint color, and highlight color
     * from the specified TextAppearance resource.
     */
    virtual CARAPI SetTextAppearance(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resid);

    /**
     * Get the default {@link Locale} of the text in this TextView.
     * @return the default {@link Locale} of the text in this TextView.
     */
    virtual CARAPI_(AutoPtr<ILocale>) GetTextLocale();

    CARAPI GetTextLocale(
        /* [out] */ ILocale** locale);

    /**
     * Set the default {@link Locale} of the text in this TextView to the given value. This value
     * is used to choose appropriate typefaces for ambiguous characters. Typically used for CJK
     * locales to disambiguate Hanzi/Kanji/Hanja characters.
     *
     * @param locale the {@link Locale} for drawing text, must not be null.
     *
     * @see Paint#setTextLocale
     */
    virtual CARAPI SetTextLocale(
        /* [in] */ ILocale* locale);


    /**
     * @return the size (in pixels) of the default text size in this TextView.
     */
    virtual CARAPI_(Float) GetTextSize();

    CARAPI GetTextSize(
        /* [out] */ Float* size);

    virtual CARAPI_(Float) GetScaledTextSize();

    virtual CARAPI_(Int32) GetTypefaceStyle();

    /**
     * Set the default text size to the given value, interpreted as "scaled
     * pixel" units.  This size is adjusted based on the current density and
     * user font size preference.
     *
     * @param size The scaled pixel size.
     *
     * @attr ref android.R.styleable#TextView_textSize
     */
    virtual CARAPI SetTextSize(
        /* [in] */ Float size);

    /**
     * Set the default text size to a given unit and value.  See {@link
     * TypedValue} for the possible dimension units.
     *
     * @param unit The desired dimension unit.
     * @param size The desired size in the given units.
     *
     * @attr ref android.R.styleable#TextView_textSize
     */
    virtual CARAPI SetTextSize(
        /* [in] */ Int32 unit,
        /* [in] */ Float size);

    /**
     * @return the extent by which text is currently being stretched
     * horizontally.  This will usually be 1.
     */
    virtual CARAPI_(Float) GetTextScaleX();

    CARAPI GetTextScaleX(
        /* [out] */ Float* size);

    /**
     * Sets the extent by which text should be stretched horizontally.
     *
     * @attr ref android.R.styleable#TextView_textScaleX
     */
    virtual CARAPI SetTextScaleX(
        /* [in] */ Float size);

    /**
     * Sets the typeface and style in which the text should be displayed.
     * Note that not all Typeface families actually have bold and italic
     * variants, so you may need to use
     * {@link #setTypeface(Typeface, Int32)} to get the appearance
     * that you actually want.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    virtual CARAPI SetTypeface(
        /* [in] */ ITypeface* tf);

    /**
     * @return the current typeface and style in which the text is being
     * displayed.
     */
    virtual CARAPI_(AutoPtr<ITypeface>) GetTypeface();

    CARAPI GetTypeface(
        /* [out] */ ITypeface** face);

    virtual CARAPI SetElegantTextHeight(
        /* [in] */ Boolean elegant);

    virtual CARAPI_(Float) GetLetterSpacing();

    CARAPI GetLetterSpacing(
        /* [out] */ Float* spacing);

    virtual CARAPI SetLetterSpacing(
        /* [in] */ Float letterSpacing);

    virtual CARAPI_(String) GetFontFeatureSettings();

    CARAPI GetFontFeatureSettings(
        /* [out] */ String* settings);

    virtual CARAPI SetFontFeatureSettings(
        /* [in] */ const String& fontFeatureSettings);

    /**
     * Sets the text color for all the states (normal, selected,
     * focused) to be this color.
     *
     * @attr ref android.R.styleable#TextView_textColor
     */
    virtual CARAPI SetTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the text color.
     *
     * @attr ref android.R.styleable#TextView_textColor
     */
    virtual CARAPI SetTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * Return the set of text colors.
     *
     * @return Returns the set of text colors.
     */
    virtual CARAPI_(AutoPtr<IColorStateList>) GetTextColors();

    CARAPI GetTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * <p>Return the current color selected for normal text.</p>
     *
     * @return Returns the current text color.
     */
    virtual CARAPI_(Int32) GetCurrentTextColor();

    CARAPI GetCurrentTextColor(
        /* [out] */ Int32* colors);

    /**
     * Sets the color used to display the selection highlight.
     *
     * @attr ref android.R.styleable#TextView_textColorHighlight
     */
    virtual CARAPI SetHighlightColor(
        /* [in] */ Int32 color);

    /**
     * @return the color used to display the selection highlight
     *
     * @see #setHighlightColor(Int32)
     *
     * @attr ref android.R.styleable#TextView_textColorHighlight
     */
    virtual CARAPI_(Int32) GetHighlightColor();

    CARAPI GetHighlightColor(
        /* [out] */ Int32* color);

    /**
     * Sets whether the soft input method will be made visible when this
     * TextView gets focused. The default is true.
     * @hide
     */
    virtual CARAPI SetShowSoftInputOnFocus(
        /* [in] */ Boolean show);

    /**
     * Returns whether the soft input method will be made visible when this
     * TextView gets focused. The default is true.
     * @hide
     */
    virtual CARAPI_(Boolean) GetShowSoftInputOnFocus();

    CARAPI GetShowSoftInputOnFocus(
        /* [out] */ Boolean* show);

    /**
     * Gives the text a shadow of the specified radius and color, the specified
     * distance from its normal position.
     *
     * @attr ref android.R.styleable#TextView_shadowColor
     * @attr ref android.R.styleable#TextView_shadowDx
     * @attr ref android.R.styleable#TextView_shadowDy
     * @attr ref android.R.styleable#TextView_shadowRadius
     */
    virtual CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Int32 color);

    /**
     * Gets the radius of the shadow layer.
     *
     * @return the radius of the shadow layer. If 0, the shadow layer is not visible
     *
     * @see #setShadowLayer(Float, Float, Float, Int32)
     *
     * @attr ref android.R.styleable#TextView_shadowRadius
     */
    virtual CARAPI_(Float) GetShadowRadius();

    CARAPI GetShadowRadius(
        /* [out] */ Float* radius);

    /**
     * @return the horizontal offset of the shadow layer
     *
     * @see #setShadowLayer(Float, Float, Float, Int32)
     *
     * @attr ref android.R.styleable#TextView_shadowDx
     */
    virtual CARAPI_(Float) GetShadowDx();

    CARAPI GetShadowDx(
        /* [out] */ Float* dx);

    /**
     * @return the vertical offset of the shadow layer
     *
     * @see #setShadowLayer(Float, Float, Float, Int32)
     *
     * @attr ref android.R.styleable#TextView_shadowDy
     */
    virtual CARAPI_(Float) GetShadowDy();

    CARAPI GetShadowDy(
        /* [out] */ Float* dy);

    /**
     * @return the color of the shadow layer
     *
     * @see #setShadowLayer(Float, Float, Float, Int32)
     *
     * @attr ref android.R.styleable#TextView_shadowColor
     */
    virtual CARAPI_(Int32) GetShadowColor();

    CARAPI GetShadowColor(
        /* [out] */ Int32* color);

    /**
     * @return the base paint used for the text.  Please use this only to
     * consult the Paint's properties and not to change them.
     */
    virtual CARAPI_(AutoPtr<ITextPaint>) GetPaint();

    CARAPI GetPaint(
        /* [out] */ ITextPaint** paint);

    /**
     * Sets the autolink mask of the text.  See {@link
     * android.text.util.Linkify#ALL Linkify.ALL} and peers for
     * possible values.
     *
     * @attr ref android.R.styleable#TextView_autoLink
     */
    virtual CARAPI SetAutoLinkMask(
        /* [in] */ Int32 mask);

    /**
     * Sets whether the movement method will automatically be set to
     * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
     * set to nonzero and links are detected in {@link #setText}.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_linksClickable
     */
    virtual CARAPI SetLinksClickable(
        /* [in] */ Boolean whether);

    /**
     * Returns whether the movement method will automatically be set to
     * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
     * set to nonzero and links are detected in {@link #setText}.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_linksClickable
     */
    virtual CARAPI_(Boolean) GetLinksClickable();

    CARAPI GetLinksClickable(
        /* [out] */ Boolean* whether);


    /**
     * Returns the list of URLSpans attached to the text
     * (by {@link Linkify} or otherwise) if any.  You can call
     * {@link URLSpan#getURL} on them to find where they link to
     * or use {@link Spanned#getSpanStart} and {@link Spanned#getSpanEnd}
     * to find the region of the text they are attached to.
     */
    virtual CARAPI_(AutoPtr< ArrayOf<IURLSpan*> >) GetUrls();

    CARAPI GetUrls(
        /* [out, callee] */ ArrayOf<IURLSpan*>** urls);

    /**
     * Sets the color of the hint text.
     *
     * @attr ref android.R.styleable#TextView_textColorHint
     */
    virtual CARAPI SetHintTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the color of the hint text.
     *
     * @attr ref android.R.styleable#TextView_textColorHint
     */
    virtual CARAPI SetHintTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * <p>Return the color used to paint the hint text.</p>
     *
     * @return Returns the list of hint text colors.
     */
    virtual CARAPI_(AutoPtr<IColorStateList>) GetHintTextColors();

    CARAPI GetHintTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * <p>Return the current color selected to paint the hint text.</p>
     *
     * @return Returns the current hint text color.
     */
    virtual CARAPI_(Int32) GetCurrentHintTextColor();

    CARAPI GetCurrentHintTextColor(
        /* [out] */ Int32* color);

    /**
     * Sets the color of links in the text.
     *
     * @attr ref android.R.styleable#TextView_textColorLink
     */
    virtual CARAPI SetLinkTextColor(
        /* [in] */ Int32 color);

    /**
     * Sets the color of links in the text.
     *
     * @attr ref android.R.styleable#TextView_textColorLink
     */
    virtual CARAPI SetLinkTextColor(
        /* [in] */ IColorStateList* colors);

    /**
     * <p>Returns the color used to paint links in the text.</p>
     *
     * @return Returns the list of link text colors.
     */
    virtual CARAPI_(AutoPtr<IColorStateList>) GetLinkTextColors();

    CARAPI GetLinkTextColors(
        /* [out] */ IColorStateList** colors);

    /**
     * Sets the horizontal alignment of the text and the
     * vertical gravity that will be used when there is extra space
     * in the TextView beyond what is required for the text itself.
     *
     * @see android.view.Gravity
     * @attr ref android.R.styleable#TextView_gravity
     */
    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Returns the horizontal and vertical alignment of this TextView.
     *
     * @see android.view.Gravity
     * @attr ref android.R.styleable#TextView_gravity
     */
    virtual CARAPI_(Int32) GetGravity();

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    /**
     * @return the flags on the Paint being used to display the text.
     * @see Paint#getFlags
     */
    virtual CARAPI_(Int32) GetPaintFlags();

    CARAPI GetPaintFlags(
        /* [out] */ Int32* flags);

    /**
     * Sets flags on the Paint being used to display the text and
     * reflows the text if they are different from the old flags.
     * @see Paint#setFlags
     */
    virtual CARAPI SetPaintFlags(
        /* [in] */ Int32 flags);

    /**
     * Sets whether the text should be allowed to be wider than the
     * View is.  If false, it will be wrapped to the width of the View.
     *
     * @attr ref android.R.styleable#TextView_scrollHorizontally
     */
    virtual CARAPI SetHorizontallyScrolling(
        /* [in] */ Boolean whether);

    /**
     * Returns whether the text is allowed to be wider than the View is.
     * If false, the text will be wrapped to the width of the View.
     *
     * @attr ref android.R.styleable#TextView_scrollHorizontally
     * @hide
     */
    virtual CARAPI_(Boolean) GetHorizontallyScrolling();

    CARAPI GetHorizontallyScrolling(
        /* [out] */ Boolean* whether);


    /**
     * Makes the TextView at least this many lines tall
     *
     * @attr ref android.R.styleable#TextView_minLines
     */
    virtual CARAPI SetMinLines(
        /* [in] */ Int32 minlines);

    /**
     * @return the minimum number of lines displayed in this TextView, or -1 if the minimum
     * height was set in pixels instead using {@link #setMinHeight(Int32) or #setHeight(Int32)}.
     *
     * @see #setMinLines(Int32)
     *
     * @attr ref android.R.styleable#TextView_minLines
     */
    virtual CARAPI_(Int32) GetMinLines();

    CARAPI GetMinLines(
        /* [out] */ Int32* minlines);

    /**
     * Makes the TextView at least this many pixels tall
     *
     * @attr ref android.R.styleable#TextView_minHeight
     */
    virtual CARAPI SetMinHeight(
        /* [in] */ Int32 minHeight);

    /**
     * @return the minimum height of this TextView expressed in pixels, or -1 if the minimum
     * height was set in number of lines instead using {@link #setMinLines(Int32) or #setLines(Int32)}.
     *
     * @see #setMinHeight(Int32)
     *
     * @attr ref android.R.styleable#TextView_minHeight
     */
    virtual CARAPI_(Int32) GetMinHeight();

    CARAPI GetMinHeight(
        /* [out] */ Int32* minHeight);

    /**
     * Makes the TextView at most this many lines tall
     *
     * @attr ref android.R.styleable#TextView_maxLines
     */
    virtual CARAPI SetMaxLines(
        /* [in] */ Int32 maxlines);

    /**
     * @return the maximum number of lines displayed in this TextView, or -1 if the maximum
     * height was set in pixels instead using {@link #setMaxHeight(Int32) or #setHeight(Int32)}.
     *
     * @see #setMaxLines(Int32)
     *
     * @attr ref android.R.styleable#TextView_maxLines
     */
    virtual CARAPI_(Int32) GetMaxLines();

    CARAPI GetMaxLines(
        /* [out] */ Int32* maxlines);

    /**
     * Makes the TextView at most this many pixels tall
     *
     * @attr ref android.R.styleable#TextView_maxHeight
     */
    virtual CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    /**
     * @return the maximum height of this TextView expressed in pixels, or -1 if the maximum
     * height was set in number of lines instead using {@link #setMaxLines(Int32) or #setLines(Int32)}.
     *
     * @see #setMaxHeight(Int32)
     *
     * @attr ref android.R.styleable#TextView_maxHeight
     */
    virtual CARAPI_(Int32) GetMaxHeight();

    CARAPI GetMaxHeight(
        /* [out] */ Int32* maxHeight);

    /**
     * Makes the TextView exactly this many lines tall
     *
     * @attr ref android.R.styleable#TextView_lines
     */
    virtual CARAPI SetLines(
        /* [in] */ Int32 lines);

    /**
     * Makes the TextView exactly this many pixels tall.
     * You could do the same thing by specifying this number in the
     * LayoutParams.
     *
     * @attr ref android.R.styleable#TextView_height
     */
    virtual CARAPI SetHeight(
        /* [in] */ Int32 pixels);

    /**
     * Makes the TextView at least this many ems wide
     *
     * @attr ref android.R.styleable#TextView_minEms
     */
    virtual CARAPI SetMinEms(
        /* [in] */ Int32 minems);

    /**
     * @return the minimum width of the TextView, expressed in ems or -1 if the minimum width
     * was set in pixels instead (using {@link #setMinWidth(Int32)} or {@link #setWidth(Int32)}).
     *
     * @see #setMinEms(Int32)
     * @see #setEms(Int32)
     *
     * @attr ref android.R.styleable#TextView_minEms
     */
    virtual CARAPI_(Int32) GetMinEms();

    CARAPI GetMinEms(
        /* [out] */ Int32* minems);

    /**
     * Makes the TextView at least this many pixels wide
     *
     * @attr ref android.R.styleable#TextView_minWidth
     */
    virtual CARAPI SetMinWidth(
        /* [in] */ Int32 minpixels);

    /**
     * @return the minimum width of the TextView, in pixels or -1 if the minimum width
     * was set in ems instead (using {@link #setMinEms(Int32)} or {@link #setEms(Int32)}).
     *
     * @see #setMinWidth(Int32)
     * @see #setWidth(Int32)
     *
     * @attr ref android.R.styleable#TextView_minWidth
     */
    virtual CARAPI_(Int32) GetMinWidth();

    CARAPI GetMinWidth(
        /* [out] */ Int32* minpixels);

    /**
     * Makes the TextView at most this many ems wide
     *
     * @attr ref android.R.styleable#TextView_maxEms
     */
    virtual CARAPI SetMaxEms(
        /* [in] */ Int32 maxems);

    /**
     * @return the maximum width of the TextView, expressed in ems or -1 if the maximum width
     * was set in pixels instead (using {@link #setMaxWidth(Int32)} or {@link #setWidth(Int32)}).
     *
     * @see #setMaxEms(Int32)
     * @see #setEms(Int32)
     *
     * @attr ref android.R.styleable#TextView_maxEms
     */
    virtual CARAPI_(Int32) GetMaxEms();

    /**
     * Makes the TextView at most this many pixels wide
     *
     * @attr ref android.R.styleable#TextView_maxWidth
     */
    virtual CARAPI SetMaxWidth(
        /* [in] */ Int32 maxpixels);

    /**
     * @return the maximum width of the TextView, in pixels or -1 if the maximum width
     * was set in ems instead (using {@link #setMaxEms(Int32)} or {@link #setEms(Int32)}).
     *
     * @see #setMaxWidth(Int32)
     * @see #setWidth(Int32)
     *
     * @attr ref android.R.styleable#TextView_maxWidth
     */
    virtual CARAPI_(Int32) GetMaxWidth();

    CARAPI GetMaxWidth(
        /* [out] */ Int32* maxpixels);

    /**
     * Makes the TextView exactly this many ems wide
     *
     * @attr ref android.R.styleable#TextView_ems
     */
    virtual CARAPI SetEms(
        /* [in] */ Int32 ems);

    /**
     * Makes the TextView exactly this many pixels wide.
     * You could do the same thing by specifying this number in the
     * LayoutParams.
     *
     * @attr ref android.R.styleable#TextView_width
     */
    virtual CARAPI SetWidth(
        /* [in] */ Int32 pixels);

    /**
     * Sets line spacing for this TextView.  Each line will have its height
     * multiplied by <code>mult</code> and have <code>add</code> added to it.
     *
     * @attr ref android.R.styleable#TextView_lineSpacingExtra
     * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
     */
    virtual CARAPI SetLineSpacing(
        /* [in] */ Float add,
        /* [in] */ Float mult);

    /**
     * Gets the line spacing multiplier
     *
     * @return the value by which each line's height is multiplied to get its actual height.
     *
     * @see #setLineSpacing(Float, Float)
     * @see #getLineSpacingExtra()
     *
     * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
     */
    virtual CARAPI_(Float) GetLineSpacingMultiplier();

    CARAPI GetLineSpacingMultiplier(
        /* [out] */ Float* multiplier);

    /**
     * Gets the line spacing extra space
     *
     * @return the extra space that is added to the height of each lines of this TextView.
     *
     * @see #setLineSpacing(Float, Float)
     * @see #getLineSpacingMultiplier()
     *
     * @attr ref android.R.styleable#TextView_lineSpacingExtra
     */
    virtual CARAPI_(Float) GetLineSpacingExtra();

    CARAPI GetLineSpacingExtra(
        /* [out] */ Float* extra);

    /**
     * Convenience method: Append the specified text to the TextView's
     * display buffer, upgrading it to BufferType.EDITABLE if it was
     * not already editable.
     */
    virtual CARAPI Append(
        /* [in] */ ICharSequence* text);

    /**
     * Convenience method: Append the specified text slice to the TextView's
     * display buffer, upgrading it to BufferType.EDITABLE if it was
     * not already editable.
     */
    virtual CARAPI Append(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Control whether this text view saves its entire text contents when
     * freezing to an icicle, in addition to dynamic state such as cursor
     * position.  By default this is false, not saving the text.  Set to true
     * if the text in the text view is not being saved somewhere else in
     * persistent storage (such as in a content provider) so that if the
     * view is later thawed the user will not lose their data.
     *
     * @param freezesText Controls whether a frozen icicle should include the
     * entire text data: true to include it, false to not.
     *
     * @attr ref android.R.styleable#TextView_freezesText
     */
    virtual CARAPI SetFreezesText(
        /* [in] */ Boolean freezesText);

    /**
     * Return whether this text view is including its entire text contents
     * in frozen icicles.
     *
     * @return Returns true if text is included, false if it isn't.
     *
     * @see #setFreezesText
     */
    virtual CARAPI_(Boolean) GetFreezesText();

    CARAPI GetFreezesText(
        /* [out] */ Boolean* text);

    /**
     * Sets the Factory used to create new Editables.
     */
    virtual CARAPI SetEditableFactory(
        /* [in] */ IEditableFactory* factory);

    /**
     * Sets the Factory used to create new Spannables.
     */
    virtual CARAPI SetSpannableFactory(
        /* [in] */ ISpannableFactory* factory);

    /**
     * Sets the string value of the TextView. TextView <em>does not</em> accept
     * HTML-like formatting, which you can do with text strings in XML resource files.
     * To style your strings, attach android.text.style.* objects to a
     * {@link android.text.SpannableString SpannableString}, or see the
     * <a href="{@docRoot}guide/topics/resources/available-resources.html#stringresources">
     * Available Resource Types</a> documentation for an example of setting
     * formatted text in the XML resource file.
     *
     * @attr ref android.R.styleable#TextView_text
     */
    virtual CARAPI SetText(
        /* [in] */ ICharSequence* text);

    /**
     * Like {@link #setText(CharSequence)},
     * except that the cursor position (if any) is retained in the new text.
     *
     * @param text The new text to place in the text view.
     *
     * @see #setText(CharSequence)
     */
    virtual CARAPI SetTextKeepState(
        /* [in] */ ICharSequence* text);

    /**
     * Sets the text that this TextView is to display (see
     * {@link #setText(CharSequence)}) and also sets whether it is stored
     * in a styleable/spannable buffer and whether it is editable.
     *
     * @attr ref android.R.styleable#TextView_text
     * @attr ref android.R.styleable#TextView_bufferType
     */
    virtual CARAPI SetText(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    /**
     * Sets the TextView to display the specified slice of the specified
     * char array.  You must promise that you will not change the contents
     * of the array except for right before another call to setText(),
     * since the TextView has no way to know that the text
     * has changed and that it needs to invalidate and re-layout.
     */
    virtual CARAPI SetText(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len);

    /**
     * Like {@link #setText(CharSequence, android.widget.TextView.BufferType)},
     * except that the cursor position (if any) is retained in the new text.
     *
     * @see #setText(CharSequence, android.widget.TextView.BufferType)
     */
    virtual CARAPI SetTextKeepState(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    virtual CARAPI SetText(
        /* [in] */ Int32 resid);

    virtual CARAPI SetText(
        /* [in] */ Int32 resid,
        /* [in] */ BufferType type);

    /**
     * Sets the text to be displayed when the text of the TextView is empty.
     * Null means to use the normal empty text. The hint does not currently
     * participate in determining the size of the view.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    virtual CARAPI SetHint(
        /* [in] */ ICharSequence* hint);

    /**
     * Sets the text to be displayed when the text of the TextView is empty,
     * from a resource.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    virtual CARAPI SetHint(
        /* [in] */ Int32 resid);

    /**
     * Returns the hint that is displayed when the text of the TextView
     * is empty.
     *
     * @attr ref android.R.styleable#TextView_hint
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetHint();

    CARAPI GetHint(
        /* [out] */ ICharSequence** hint);

    /**
     * Set the type of the content with a constant as defined for
     * {@link EditorInfo#inputType}.  This will take care of changing
     * the key listener, by calling {@link #setKeyListener(KeyListener)}, to
     * match the given content type.  If the given content type is
     * {@link EditorInfo#TYPE_NULL} then a soft keyboard will
     * not be displayed for this text view.
     *
     * @see #getInputType()
     * @see #setRawInputType(Int32)
     * @see android.text.InputType
     * @attr ref android.R.styleable#TextView_inputType
     */
    virtual CARAPI SetInputType(
        /* [in] */ Int32 type);

    /**
     * Directly change the content type integer of the text view, without
     * modifying any other state.
     * @see #setInputType(Int32)
     * @see android.text.InputType
     * @attr ref android.R.styleable#TextView_inputType
     */
    virtual CARAPI SetRawInputType(
        /* [in] */ Int32 type);

    /**
     * Get the type of the content.
     *
     * @see #setInputType(Int32)
     * @see android.text.InputType
     */
    virtual CARAPI_(Int32) GetInputType();

    CARAPI GetInputType(
        /* [out] */ Int32* type);

    /**
     * Change the editor type integer associated with the text view, which
     * will be reported to an IME with {@link EditorInfo#imeOptions} when it
     * has focus.
     * @see #getImeOptions
     * @see android.view.inputmethod.EditorInfo
     * @attr ref android.R.styleable#TextView_imeOptions
     */
    virtual CARAPI SetImeOptions(
        /* [in] */ Int32 imeOptions);

    /**
     * Get the type of the IME editor.
     *
     * @see #setImeOptions(Int32)
     * @see android.view.inputmethod.EditorInfo
     */
    virtual CARAPI_(Int32) GetImeOptions();

    CARAPI GetImeOptions(
        /* [out] */ Int32* options);

    /**
     * Change the custom IME action associated with the text view, which
     * will be reported to an IME with {@link EditorInfo#actionLabel}
     * and {@link EditorInfo#actionId} when it has focus.
     * @see #getImeActionLabel
     * @see #getImeActionId
     * @see android.view.inputmethod.EditorInfo
     * @attr ref android.R.styleable#TextView_imeActionLabel
     * @attr ref android.R.styleable#TextView_imeActionId
     */
    virtual CARAPI SetImeActionLabel(
        /* [in] */ ICharSequence* label,
        /* [in] */ Int32 actionId);

    /**
     * Get the IME action label previous set with {@link #setImeActionLabel}.
     *
     * @see #setImeActionLabel
     * @see android.view.inputmethod.EditorInfo
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetImeActionLabel();

    CARAPI GetImeActionLabel(
        /* [out] */ ICharSequence** label);

    /**
     * Get the IME action ID previous set with {@link #setImeActionLabel}.
     *
     * @see #setImeActionLabel
     * @see android.view.inputmethod.EditorInfo
     */
    virtual CARAPI_(Int32) GetImeActionId();

    CARAPI GetImeActionId(
        /* [out] */ Int32* id);

    /**
     * Set a special listener to be called when an action is performed
     * on the text view.  This will be called when the enter key is pressed,
     * or when an action supplied to the IME is selected by the user.  Setting
     * this means that the normal hard key event will not insert a newline
     * into the text view, even if it is multi-line; holding down the ALT
     * modifier will, however, allow the user to insert a newline character.
     */
    virtual CARAPI SetOnEditorActionListener(
        /* [in] */ IOnEditorActionListener* l);

    /**
     * Called when an attached input method calls
     * {@link InputConnection#performEditorAction(Int32)
     * InputConnection.performEditorAction()}
     * for this text view.  The default implementation will call your action
     * listener supplied to {@link #setOnEditorActionListener}, or perform
     * a standard operation for {@link EditorInfo#IME_ACTION_NEXT
     * EditorInfo.IME_ACTION_NEXT} or {@link EditorInfo#IME_ACTION_DONE
     * EditorInfo.IME_ACTION_DONE}.
     *
     * <p>For backwards compatibility, if no IME options have been set and the
     * text view would not normally advance focus on enter, then
     * the NEXT and DONE actions received here will be turned into an enter
     * key down/up pair to go through the normal key handling.
     *
     * @param actionCode The code of the action being performed.
     *
     * @see #setOnEditorActionListener
     */
    virtual CARAPI OnEditorAction(
        /* [in] */ Int32 actionCode);

    /**
     * Set the private content type of the text, which is the
     * {@link EditorInfo#privateImeOptions EditorInfo.privateImeOptions}
     * field that will be filled in when creating an input connection.
     *
     * @see #getPrivateImeOptions()
     * @see EditorInfo#privateImeOptions
     * @attr ref android.R.styleable#TextView_privateImeOptions
     */
    virtual CARAPI SetPrivateImeOptions(
        /* [in] */ const String& type);

    /**
     * Get the private type of the content.
     *
     * @see #setPrivateImeOptions(String)
     * @see EditorInfo#privateImeOptions
     */
    virtual CARAPI_(String) GetPrivateImeOptions();

    CARAPI GetPrivateImeOptions(
        /* [out] */ String* options);

    /**
     * Set the extra input data of the text, which is the
     * {@link EditorInfo#extras TextBoxAttribute.extras}
     * Bundle that will be filled in when creating an input connection.  The
     * given integer is the resource ID of an XML resource holding an
     * {@link android.R.styleable#InputExtras &lt;input-extras&gt;} XML tree.
     *
     * @see #getInputExtras(Boolean)
     * @see EditorInfo#extras
     * @attr ref android.R.styleable#TextView_editorExtras
     */
    virtual CARAPI SetInputExtras(
        /* [in] */ Int32 xmlResId);

    /**
     * Retrieve the input extras currently associated with the text view, which
     * can be viewed as well as modified.
     *
     * @param create If true, the extras will be created if they don't already
     * exist.  Otherwise, null will be returned if none have been created.
     * @see #setInputExtras(Int32)
     * @see EditorInfo#extras
     * @attr ref android.R.styleable#TextView_editorExtras
     */
    virtual CARAPI_(AutoPtr<IBundle>) GetInputExtras(
        /* [in] */ Boolean create);

    CARAPI GetInputExtras(
        /* [in] */ Boolean create,
        /* [out] */ IBundle** bundle);

    /**
     * Returns the error message that was set to be displayed with
     * {@link #setError}, or <code>null</code> if no error was set
     * or if it the error was cleared by the widget after user input.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetError();

    CARAPI GetError(
        /* [out] */ ICharSequence** error);

    /**
     * Sets the right-hand compound drawable of the TextView to the "error"
     * icon and sets an error message that will be displayed in a popup when
     * the TextView has focus.  The icon and error message will be reset to
     * null when any key events cause changes to the TextView's text.  If the
     * <code>error</code> is <code>null</code>, the error message and icon
     * will be cleared.
     */
    virtual CARAPI SetError(
        /* [in] */ ICharSequence* error);

    /**
     * Sets the right-hand compound drawable of the TextView to the specified
     * icon and sets an error message that will be displayed in a popup when
     * the TextView has focus.  The icon and error message will be reset to
     * null when any key events cause changes to the TextView's text.  The
     * drawable must already have had {@link Drawable#setBounds} set on it.
     * If the <code>error</code> is <code>null</code>, the error message will
     * be cleared (and you should provide a <code>null</code> icon as well).
     */
    virtual CARAPI SetError(
        /* [in] */ ICharSequence* error,
        /* [in] */ IDrawable* icon);

    /**
     * Sets the list of input filters that will be used if the buffer is
     * Editable.  Has no effect otherwise.
     *
     * @attr ref android.R.styleable#TextView_maxLength
     */
    virtual CARAPI SetFilters(
        /* [in] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>* filters);

    /**
     * Returns the current list of input filters.
     */
    virtual CARAPI_(AutoPtr<ArrayOf<Elastos::Droid::Text::IInputFilter*> >) GetFilters();

    CARAPI GetFilters(
        /* [out, callee] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>** filters);

    /**
     * When a TextView is used to display a useful piece of information to the user (such as a
     * contact's address), it should be made selectable, so that the user can select and copy this
     * content.
     *
     * Use {@link #setTextIsSelectable(boolean)} or the
     * {@link android.R.styleable#TextView_textIsSelectable} XML attribute to make this TextView
     * selectable (text is not selectable by default).
     *
     * Note that this method simply returns the state of this flag. Although this flag has to be set
     * in order to select text in non-editable TextView, the content of an {@link EditText} can
     * always be selected, independently of the value of this flag.
     *
     * @return True if the text displayed in this TextView can be selected by the user.
     *
     * @attr ref android.R.styleable#TextView_textIsSelectable
     */
    virtual CARAPI_(Boolean) IsTextSelectable();

    CARAPI IsTextSelectable(
        /* [out] */ Boolean* selectable);

    /**
     * Sets whether or not (default) the content of this view is selectable by the user.
     *
     * Note that this methods affect the {@link #setFocusable(boolean)},
     * {@link #setFocusableInTouchMode(boolean)} {@link #setClickable(boolean)} and
     * {@link #setLongClickable(boolean)} states and you may want to restore these if they were
     * customized.
     *
     * See {@link #isTextSelectable} for details.
     *
     * @param selectable Whether or not the content of this TextView should be selectable.
     */
    virtual CARAPI SetTextIsSelectable(
        /* [in] */ Boolean selectable);

    /**
     * Return the number of lines of text, or 0 if the internal Layout has not
     * been built.
     */
    virtual CARAPI_(Int32) GetLineCount();

    CARAPI GetLineCount(
        /* [out] */ Int32* count);

    /**
     * Return the baseline for the specified line (0...getLineCount() - 1)
     * If bounds is not null, return the top, left, right, bottom extents
     * of the specified line in it. If the internal Layout has not been built,
     * return 0 and set bounds to (0, 0, 0, 0)
     * @param line which line to examine (0..getLineCount() - 1)
     * @param bounds Optional. If not null, it returns the extent of the line
     * @return the Y-coordinate of the baseline
     */
    virtual CARAPI_(Int32) GetLineBounds(
        /* [in] */ Int32 line,
        /* [in] */ IRect* bounds);

    CARAPI GetLineBounds(
        /* [in] */ Int32 line,
        /* [in] */ IRect* bounds,
        /* [out] */ Int32* y);

    /**
     * Resets the mErrorWasChanged flag, so that future calls to {@link #setError(CharSequence)}
     * can be recorded.
     * @hide
     */
    virtual CARAPI ResetErrorChangedFlag();

    /**
     * @hide
     */
    virtual CARAPI HideErrorIfUnchanged();

    /**
     * If this TextView contains editable content, extract a portion of it
     * based on the information in <var>request</var> in to <var>outText</var>.
     * @return Returns true if the text was successfully extracted, else false.
     */
    virtual CARAPI_(Boolean) ExtractText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ IExtractedText* outText);

    CARAPI ExtractText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ IExtractedText* outText,
        /* [out] */ Boolean* result);

    /**
     * Apply to this text view the given extracted text, as previously
     * returned by {@link #extractText(ExtractedTextRequest, ExtractedText)}.
     */
    virtual CARAPI SetExtractedText(
        /* [in] */ IExtractedText* text);

    /**
     * @hide
     */
    virtual CARAPI SetExtracting(
        /* [in] */ IExtractedTextRequest* req);

    /**
     * Called by the framework in response to a text completion from
     * the current input method, provided by it calling
     * {@link InputConnection#commitCompletion
     * InputConnection.commitCompletion()}.  The default implementation does
     * nothing; text views that are supporting auto-completion should override
     * this to do their desired behavior.
     *
     * @param text The auto complete text the user has selected.
     */
    virtual CARAPI OnCommitCompletion(
        /* [in] */ ICompletionInfo* text);

    /**
     * Called by the framework in response to a text auto-correction (such as fixing a typo using a
     * a dictionnary) from the current input method, provided by it calling
     * {@link InputConnection#commitCorrection} InputConnection.commitCorrection()}. The default
     * implementation flashes the background of the corrected word to provide feedback to the user.
     *
     * @param info The auto correct info about the text that was corrected.
     */
    virtual CARAPI OnCommitCorrection(
        /* [in] */ ICorrectionInfo* info);

    virtual CARAPI BeginBatchEdit();

    virtual CARAPI EndBatchEdit();

    /**
     * Called by the framework in response to a request to begin a batch
     * of edit operations through a call to link {@link #beginBatchEdit()}.
     */
    virtual CARAPI OnBeginBatchEdit();

    /**
     * Called by the framework in response to a request to end a batch
     * of edit operations through a call to link {@link #endBatchEdit}.
     */
    virtual CARAPI OnEndBatchEdit();

    /**
     * Called by the framework in response to a private command from the
     * current method, provided by it calling
     * {@link InputConnection#performPrivateCommand
     * InputConnection.performPrivateCommand()}.
     *
     * @param action The action name of the command.
     * @param data Any additional data for the command.  This may be null.
     * @return Return true if you handled the command, else false.
     */
    virtual CARAPI_(Boolean) OnPrivateIMECommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI OnPrivateIMECommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data,
        /* [out] */ Boolean* result);

    /**
     * Set whether the TextView includes extra top and bottom padding to make
     * room for accents that go above the normal ascent and descent.
     * The default is true.
     *
     * @attr ref android.R.styleable#TextView_includeFontPadding
     */
    virtual CARAPI SetIncludeFontPadding(
        /* [in] */ Boolean includepad);

    /**
     * Gets whether the TextView includes extra top and bottom padding to make
     * room for accents that go above the normal ascent and descent.
     *
     * @see #setIncludeFontPadding(boolean)
     *
     * @attr ref android.R.styleable#TextView_includeFontPadding
     */
    virtual CARAPI_(Boolean) GetIncludeFontPadding();

    CARAPI GetIncludeFontPadding(
        /* [out] */ Boolean* padding);

    /**
     * Move the point, specified by the offset, into the view if it is needed.
     * This has to be called after layout. Returns true if anything changed.
     */
    virtual CARAPI_(Boolean) BringPointIntoView(
        /* [in] */ Int32 offset);

    CARAPI BringPointIntoView(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* result);

    /**
     * Move the cursor, if needed, so that it is at an offset that is visible
     * to the user.  This will not move the cursor if it represents more than
     * one character (a selection range).  This will only work if the
     * TextView contains spannable text; otherwise it will do nothing.
     *
     * @return True if the cursor was actually moved, false otherwise.
     */
    virtual CARAPI_(Boolean) MoveCursorToVisibleOffset();

    CARAPI MoveCursorToVisibleOffset(
        /* [out] */ Boolean* result);

    /**
     * Convenience for {@link Selection#getSelectionStart}.
     */
    virtual CARAPI_(Int32) GetSelectionStart();

    CARAPI GetSelectionStart(
        /* [out] */ Int32* start);

    /**
     * Convenience for {@link Selection#getSelectionEnd}.
     */
    virtual CARAPI_(Int32) GetSelectionEnd();

    CARAPI GetSelectionEnd(
        /* [out] */ Int32* end);

    /**
     * Return true iff there is a selection inside this text view.
     */
    virtual CARAPI_(Boolean) HasSelection();

    CARAPI HasSelection(
        /* [out] */ Boolean* result);

    /**
     * Sets the properties of this field (lines, horizontally scrolling,
     * transformation method) to be for a single-line input.
     *
     * @attr ref android.R.styleable#TextView_singleLine
     */
    virtual CARAPI SetSingleLine();

    /**
     * Sets the properties of this field to transform input to ALL CAPS
     * display. This may use a "small caps" formatting if available.
     * This setting will be ignored if this field is editable or selectable.
     *
     * This call replaces the current transformation method. Disabling this
     * will not necessarily restore the previous behavior from before this
     * was enabled.
     *
     * @see #setTransformationMethod(TransformationMethod)
     * @attr ref android.R.styleable#TextView_textAllCaps
     */
    virtual CARAPI SetAllCaps(
        /* [in] */ Boolean allCaps);

    /**
     * If true, sets the properties of this field (lines, horizontally
     * scrolling, transformation method) to be for a single-line input;
     * if false, restores these to the default conditions.
     * Note that calling this with false restores default conditions,
     * not necessarily those that were in effect prior to calling
     * it with true.
     *
     * @attr ref android.R.styleable#TextView_singleLine
     */
    virtual CARAPI SetSingleLine(
        /* [in] */ Boolean singleLine);

    /**
     * Causes words in the text that are longer than the view is wide
     * to be ellipsized instead of broken in the middle.  You may also
     * want to {@link #setSingleLine} or {@link #setHorizontallyScrolling}
     * to constrain the text to a single line.  Use <code>null</code>
     * to turn off ellipsizing.
     *
     * @attr ref android.R.styleable#TextView_ellipsize
     */
    virtual CARAPI SetEllipsize(
        /* [in] */ TextUtilsTruncateAt where);

    /**
     * Sets how many times to repeat the marquee animation. Only applied if the
     * TextView has marquee enabled. Set to -1 to repeat indefinitely.
     *
     * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
     */
    virtual CARAPI SetMarqueeRepeatLimit(
        /* [in] */ Int32 marqueeLimit);

    /**
     * Gets the number of times the marquee animation is repeated. Only meaningful if the
     * TextView has marquee enabled.
     *
     * @return the number of times the marquee animation is repeated. -1 if the animation
     * repeats indefinitely
     *
     * @see #setMarqueeRepeatLimit(Int32)
     *
     * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
     */
    virtual CARAPI_(Int32) GetMarqueeRepeatLimit();

    CARAPI GetMarqueeRepeatLimit(
        /* [out] */ Int32* marqueeLimit);

    /**
     * Returns where, if anywhere, words that are longer than the view
     * is wide should be ellipsized.
     */
    virtual CARAPI_(TextUtilsTruncateAt) GetEllipsize();

    CARAPI GetEllipsize(
        /* [out] */ TextUtilsTruncateAt* size);

    /**
     * Set the TextView so that when it takes focus, all the text is
     * selected.
     *
     * @attr ref android.R.styleable#TextView_selectAllOnFocus
     */
    virtual CARAPI SetSelectAllOnFocus(
        /* [in] */ Boolean selectAllOnFocus);

    /**
     * Set whether the cursor is visible.  The default is true.
     *
     * @attr ref android.R.styleable#TextView_cursorVisible
     */
    virtual CARAPI SetCursorVisible(
        /* [in] */ Boolean visible);

    /**
     * @return whether or not the cursor is visible (assuming this TextView is editable)
     *
     * @see #setCursorVisible(boolean)
     *
     * @attr ref android.R.styleable#TextView_cursorVisible
     */
    virtual CARAPI_(Boolean) IsCursorVisible();

    CARAPI IsCursorVisible(
        /* [out] */ Boolean* visible);

    /**
     * Called when a context menu option for the text view is selected.  Currently
     * this will be one of: {@link android.R.id#selectAll},
     * {@link android.R.id#startSelectingText},
     * {@link android.R.id#cut}, {@link android.R.id#copy},
     * {@link android.R.id#paste}, {@link android.R.id#copyUrl},
     * or {@link android.R.id#switchInputMethod}.
     */
    virtual CARAPI_(Boolean) OnTextContextMenuItem(
        /* [in] */ Int32 id);

    CARAPI OnTextContextMenuItem(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* result);

    /**
     * Adds a TextWatcher to the list of those whose methods are called
     * whenever this TextView's text changes.
     * <p>
     * In 1.0, the {@link TextWatcher#afterTextChanged} method was erroneously
     * not called after {@link #setText} calls.  Now, doing {@link #setText}
     * if there are any text changed listeners forces the buffer type to
     * Editable if it would not otherwise be and does call this method.
     */
    virtual CARAPI AddTextChangedListener(
        /* [in] */ ITextWatcher* watcher);


    /**
     * Removes the specified TextWatcher from the list of those whose
     * methods are called
     * whenever this TextView's text changes.
     */
    virtual CARAPI RemoveTextChangedListener(
        /* [in] */ ITextWatcher* watcher);

    /**
     * Use {@link BaseInputConnection#removeComposingSpans
     * BaseInputConnection.removeComposingSpans()} to remove any IME composing
     * state from this text view.
     */
    virtual CARAPI ClearComposingText();

    /**
     * Returns true, only while processing a touch gesture, if the initial
     * touch down event caused focus to move to the text view and as a result
     * its selection changed.  Only valid while processing the touch gesture
     * of interest.
     */
    virtual CARAPI_(Boolean) DidTouchFocusSelect();

    CARAPI DidTouchFocusSelect(
        /* [out] */ Boolean* result);

    virtual CARAPI SetScroller(
        /* [in] */ IScroller* s);

    /**
     * This is a temporary method. Future versions may support multi-locale text.
     * Caveat: This method may not return the latest text services locale, but this should be
     * acceptable and it's more important to make this method asynchronous.
     *
     * @return The locale that should be used for a word iterator and a spell checker
     * in this TextView, based on the current spell checker settings,
     * the current IME's locale, or the system default locale.
     * @hide
     */
    // TODO: Support multi-locale
    // TODO: Update the text services locale immediately after the keyboard locale is switched
    // by catching intent of keyboard switch event
    virtual CARAPI_(AutoPtr<ILocale>) GetTextServicesLocale();

    CARAPI GetTextServicesLocale(
        /* [out] */ ILocale** locale);

    /**
     * This method is used by the ArrowKeyMovementMethod to jump from one word to the other.
     * Made available to achieve a consistent behavior.
     * @hide
     */
    virtual CARAPI_(AutoPtr<IWordIterator>) GetWordIterator();

    CARAPI GetWordIterator(
        /* [out] */ IWordIterator** iterator);

    /**
     * Gets the text reported for accessibility purposes.
     *
     * @return The accessibility text.
     *
     * @hide
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextForAccessibility();

    CARAPI GetTextForAccessibility(
        /* [out] */ ICharSequence** text);

    /**
     * Returns whether this text view is a current input method target.  The
     * default implementation just checks with {@link InputMethodManager}.
     */
    virtual CARAPI_(Boolean) IsInputMethodTarget();

    CARAPI IsInputMethodTarget(
        /* [out] */ Boolean* result);

    /**
     * Return whether or not suggestions are enabled on this TextView. The suggestions are generated
     * by the IME or by the spell checker as the user types. This is done by adding
     * {@link SuggestionSpan}s to the text.
     *
     * When suggestions are enabled (default), this list of suggestions will be displayed when the
     * user asks for them on these parts of the text. This value depends on the inputType of this
     * TextView.
     *
     * The class of the input type must be {@link InputType#TYPE_CLASS_TEXT}.
     *
     * In addition, the type variation must be one of
     * {@link InputType#TYPE_TEXT_VARIATION_NORMAL},
     * {@link InputType#TYPE_TEXT_VARIATION_EMAIL_SUBJECT},
     * {@link InputType#TYPE_TEXT_VARIATION_LONG_MESSAGE},
     * {@link InputType#TYPE_TEXT_VARIATION_SHORT_MESSAGE} or
     * {@link InputType#TYPE_TEXT_VARIATION_WEB_EDIT_TEXT}.
     *
     * And finally, the {@link InputType#TYPE_TEXT_FLAG_NO_SUGGESTIONS} flag must <i>not</i> be set.
     *
     * @return true if the suggestions popup window is enabled, based on the inputType.
     */
    virtual CARAPI_(Boolean) IsSuggestionsEnabled();

    CARAPI IsSuggestionsEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * If provided, this ActionMode.Callback will be used to create the ActionMode when text
     * selection is initiated in this View.
     *
     * The standard implementation populates the menu with a subset of Select All, Cut, Copy and
     * Paste actions, depending on what this View supports.
     *
     * A custom implementation can add new entries in the default menu in its
     * {@link android.view.ActionMode.Callback#onPrepareActionMode(ActionMode, Menu)} method. The
     * default actions can also be removed from the menu using {@link Menu#removeItem(Int32)} and
     * passing {@link android.R.id#selectAll}, {@link android.R.id#cut}, {@link android.R.id#copy}
     * or {@link android.R.id#paste} ids as parameters.
     *
     * Returning false from
     * {@link android.view.ActionMode.Callback#onCreateActionMode(ActionMode, Menu)} will prevent
     * the action mode from being started.
     *
     * Action click events should be handled by the custom implementation of
     * {@link android.view.ActionMode.Callback#onActionItemClicked(ActionMode, MenuItem)}.
     *
     * Note that text selection mode is not started when a TextView receives focus and the
     * {@link android.R.attr#selectAllOnFocus} flag has been set. The content is highlighted in
     * that case, to allow for quick replacement.
     */
    virtual CARAPI SetCustomSelectionActionModeCallback(
        /* [in] */ IActionModeCallback* actionModeCallback);

    /**
     * Retrieves the value set in {@link #setCustomSelectionActionModeCallback}. Default is null.
     *
     * @return The current custom selection callback.
     */
    virtual CARAPI_(AutoPtr<IActionModeCallback>) GetCustomSelectionActionModeCallback();

    CARAPI GetCustomSelectionActionModeCallback(
        /* [out] */ IActionModeCallback** actionModeCallback);

    /**
     * Get the character offset closest to the specified absolute position. A typical use case is to
     * pass the result of {@link MotionEvent#getX()} and {@link MotionEvent#getY()} to this method.
     *
     * @param x The horizontal absolute position of a point on screen
     * @param y The vertical absolute position of a point on screen
     * @return the character offset for the character whose position is closest to the specified
     *  position. Returns -1 if there is no layout.
     */
    virtual CARAPI_(Int32) GetOffsetForPosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI GetOffsetForPosition(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Int32* offset);

    virtual CARAPI_(Int32) GetHorizontalOffsetForDrawables();

    CARAPI GetHorizontalOffsetForDrawables(
        /* [out] */ Int32* offset);

    virtual CARAPI_(AutoPtr<ILocale>) GetSpellCheckerLocale();

    virtual CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

public: /* override */

    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI SetPaddingRelative(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    virtual CARAPI OnScreenStateChanged(
        /* [in] */ Int32 screenState);

    virtual CARAPI JumpDrawablesToCurrentState();

    virtual CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI_(Boolean) HasOverlappingRendering();

    virtual CARAPI GetFocusedRect(
        /* [in] */ IRect* r);

    virtual CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    virtual CARAPI_(Boolean) OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnCheckIsTextEditor();

    virtual CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

    virtual CARAPI ComputeScroll();

    virtual CARAPI_(void) Debug(
        /* [in] */ Int32 depth);

    /**
     * @hide
     */
    virtual CARAPI DispatchFinishTemporaryDetach();

    virtual CARAPI OnStartTemporaryDetach();

    virtual CARAPI OnFinishTemporaryDetach();

    virtual CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    virtual CARAPI SetSelected(
        /* [in] */ Boolean selected);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI CancelLongPress();

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI FindViewsWithText(
        /* [in, out] */ IArrayList* outViews,
        /* [in] */ ICharSequence* searched,
        /* [in] */ Int32 flags);

    virtual CARAPI_(Boolean) OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    virtual CARAPI SendAccessibilityEvent(
        /* [in] */ Int32 eventType);

    virtual CARAPI_(Boolean) PerformLongClick();

    virtual CARAPI_(Boolean) OnDragEvent(
        /* [in] */ IDragEvent* event);

    virtual CARAPI OnResolveDrawables(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetIterableTextForAccessibility();

    virtual CARAPI_(AutoPtr<ITextSegmentIterator>) GetIteratorForGranularity(
        /* [in] */ Int32 granularity);

    virtual CARAPI_(Int32) GetAccessibilitySelectionStart();

    virtual CARAPI SetAccessibilitySelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI OnRtlPropertiesChanged(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI_(Boolean) IsAccessibilitySelectionExtendable();

    virtual CARAPI_(Int32) GetAccessibilitySelectionEnd();

public: /*package*/

    /**
     * @return the Layout that is currently being used to display the hint text.
     * This can be null.
     */
    virtual CARAPI_(AutoPtr<ILayout>) GetHintLayout();

    virtual CARAPI_(AutoPtr<IUndoManager>) GetUndoManager();

    virtual CARAPI_(void) SetUndoManager(
        /* [in] */ IUndoManager* undoManager,
        /* [in] */ const String& tag);

    virtual CARAPI RemoveMisspelledSpans(
        /* [in] */ ISpannable* spannable);

    virtual CARAPI_(Boolean) IsSingleLine();

    /**
     * Removes the suggestion spans.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) RemoveSuggestionSpans(
        /* [in] */ ICharSequence* text);

    virtual CARAPI_(Int32) GetVerticalOffset(
        /* [in] */ Boolean forceNormal);

    virtual CARAPI_(void) InvalidateCursorPath();

    virtual CARAPI_(void) InvalidateCursor();

    virtual CARAPI InvalidateRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean invalidateCursor);

    /**
     * This is used to remove all style-impacting spans from text before new
     * extracted text is being replaced into it, so that we don't have any
     * lingering spans applied during the replace.
     */
    static CARAPI_(void) RemoveParcelableSpans(
        /* [in] */ ISpannable* spannable,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI_(Int32) ViewportToContentHorizontalOffset();

    virtual CARAPI_(Int32) ViewportToContentVerticalOffset();

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI_(void) SendOnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI_(void) SendAfterTextChanged(
        /* [in] */ IEditable* text);

    virtual CARAPI_(void) UpdateAfterEdit();

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI_(void) HandleTextChanged(
        /* [in] */ ICharSequence* buffer,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    /**
     * Not private so it can be called from an inner class without going
     * through a thunk.
     */
    virtual CARAPI SpanChange(
        /* [in] */ ISpanned* buf,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 oldStart,
        /* [in] */ Int32 newStart,
        /* [in] */ Int32 oldEnd,
        /* [in] */ Int32 newEnd);

    /**
     * Returns the TextView_textColor attribute from the
     * Resources.StyledAttributes, if set, or the TextAppearance_textColor
     * from the TextView_textAppearance attribute, if TextView_textColor
     * was not set directly.
     */
    static CARAPI GetTextColors(
        /* [in] */ IContext* context,
        /* [in] */ ITypedArray* attrs,
        /* [out] */ IColorStateList** list);

    /**
     * Returns the default color from the TextView_textColor attribute
     * from the AttributeSet, if set, or the default color from the
     * TextAppearance_textColor from the TextView_textAppearance attribute,
     * if TextView_textColor was not set directly.
     */
    static CARAPI_(Int32) GetTextColor(
        /* [in] */ IContext* context,
        /* [in] */ ITypedArray* attrs,
        /* [in] */ Int32 def);

    /**
     * @return True iff this TextView contains a text that can be edited.
     */
    virtual CARAPI_(Boolean) IsTextEditable();

    virtual CARAPI_(Boolean) TextCanBeSelected();

    virtual CARAPI OnLocaleChanged();

    virtual CARAPI_(void) SendAccessibilityEventTypeViewTextChanged(
        /* [in] */ ICharSequence* beforeText,
        /* [in] */ Int32 fromIndex,
        /* [in] */ Int32 removedCount,
        /* [in] */ Int32 addedCount);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTransformedText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI_(Boolean) CanCut();

    virtual CARAPI_(Boolean) CanCopy();

    virtual CARAPI_(Boolean) CanPaste();

    virtual CARAPI_(Boolean) SelectAllText();

    virtual CARAPI_(Float) ConvertToLocalHorizontalCoordinate(
        /* [in] */ Float y);

    virtual CARAPI_(Int32) GetLineAtCoordinate(
        /* [in] */ Float y);

    virtual CARAPI_(Boolean) IsInBatchEditMode();

    virtual CARAPI_(AutoPtr<ITextDirectionHeuristic>) GetTextDirectionHeuristic();

    /**
      * Deletes the range of text [start, end[.
      * @hide
      */
    virtual CARAPI_(void) DeleteText_internal(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI_(void) RemoveAdjacentSuggestionSpans(
        /* [in] */ Int32 pos);

protected:
    /**
      * Subclasses override this to specify that they have a KeyListener
      * by default even if not specifically called for in the XML options.
      */
     virtual CARAPI_(Boolean) GetDefaultEditable();

     /**
      * Subclasses override this to specify a default movement method.
      */
     virtual CARAPI_(AutoPtr<IMovementMethod>) GetDefaultMovementMethod();

     virtual CARAPI DrawableStateChanged();

     virtual CARAPI DrawableHotspotChanged(
        /* [in] */ Float x,
        /* [in] */ Float y);

     virtual CARAPI_(Boolean) SetFrame(
         /* [in] */ Int32 l,
         /* [in] */ Int32 t,
         /* [in] */ Int32 r,
         /* [in] */ Int32 b);

     virtual CARAPI OnAttachedToWindow();

     virtual CARAPI OnDetachedFromWindowInternal();

     virtual CARAPI_(Boolean) IsPaddingOffsetRequired();

     virtual CARAPI_(Int32) GetLeftPaddingOffset();

     virtual CARAPI_(Int32) GetTopPaddingOffset();

     virtual CARAPI_(Int32) GetBottomPaddingOffset();

     virtual CARAPI_(Int32) GetRightPaddingOffset();

     virtual CARAPI_(Boolean) VerifyDrawable(
         /* [in] */ IDrawable* who);

     virtual CARAPI OnCreateDrawableState(
         /* [in] */ Int32 extraSpace,
         /* [out] */ ArrayOf<Int32>** drawableState);

     virtual CARAPI_(void) OnDraw(
         /* [in] */ ICanvas* canvas);

     virtual CARAPI_(Int32) GetFadeTop(
         /* [in] */ Boolean offsetRequired);

     virtual CARAPI_(Int32) GetFadeHeight(
         /* [in] */ Boolean offsetRequired);

     /**
      * The width passed in is now the desired layout width,
      * not the full view width with padding.
      * {@hide}
      */
     virtual CARAPI_(void) MakeNewLayout(
         /* [in] */ Int32 w,
         /* [in] */ Int32 hintWidth,
         /* [in] */ IBoringLayoutMetrics* boring,
         /* [in] */ IBoringLayoutMetrics* hintBoring,
         /* [in] */ Int32 ellipsisWidth,
         /* [in] */ Boolean bringIntoView);

     CARAPI_(void) OnMeasure(
         /* [in] */ Int32 widthMeasureSpec,
         /* [in] */ Int32 heightMeasureSpec);

     CARAPI OnLayout(
         /* [in] */ Boolean changed,
         /* [in] */ Int32 left,
         /* [in] */ Int32 top,
         /* [in] */ Int32 right,
         /* [in] */ Int32 bottom);

     /**
      * This method is called when the text is changed, in case any
      * subclasses would like to know.
      *
      * @param text The text the TextView is displaying.
      * @param start The offset of the start of the range of the text
      *              that was modified.
      * @param before The offset of the former end of the range of the
      *               text that was modified.  If text was simply inserted,
      *               this will be the same as <code>start</code>.
      *               If text was replaced with new text or deleted, the
      *               length of the old text was <code>before-start</code>.
      * @param after The offset of the end of the range of the text
      *              that was modified.  If text was simply deleted,
      *              this will be the same as <code>start</code>.
      *              If text was replaced with new text or inserted,
      *              the length of the new text is <code>after-start</code>.
      */
     virtual CARAPI_(void) OnTextChanged(
         /* [in] */ ICharSequence* text,
         /* [in] */ Int32 start,
         /* [in] */ Int32 before,
         /* [in] */ Int32 after);

     /**
      * This method is called when the selection has changed, in case any
      * subclasses would like to know.
      *
      * @param selStart The new selection start location.
      * @param selEnd The new selection end location.
      */
     virtual CARAPI_(void) OnSelectionChanged(
         /* [in] */ Int32 selStart,
         /* [in] */ Int32 selEnd);

     virtual CARAPI_(void) OnFocusChanged(
         /* [in] */ Boolean focused,
         /* [in] */ Int32 direction,
         /* [in] */ IRect* previouslyFocusedRect);

     virtual CARAPI OnVisibilityChanged(
         /* [in] */ IView* changedView,
         /* [in] */ Int32 visibility);

     virtual CARAPI_(Float) GetLeftFadingEdgeStrength();

     virtual CARAPI_(Float) GetRightFadingEdgeStrength();

     virtual CARAPI_(Int32) ComputeHorizontalScrollRange();

     virtual CARAPI_(Int32) ComputeVerticalScrollRange();

     virtual CARAPI_(Int32) ComputeVerticalScrollExtent();

     virtual CARAPI_(void) OnScrollChanged(
         /* [in] */ Int32 horiz,
         /* [in] */ Int32 vert,
         /* [in] */ Int32 oldHoriz,
         /* [in] */ Int32 oldVert);

     /**
      * @hide
      */
     virtual CARAPI_(void) StopSelectionActionMode();

    /**
     * @hide
     */
     virtual CARAPI_(void) ResetResolvedDrawables();

    /**
     * @hide
     */
     virtual CARAPI_(void) ViewClicked(
         /* [in] */ IInputMethodManager* imm);

     virtual CARAPI_(void) ReplaceText_internal(
         /* [in] */ Int32 start,
         /* [in] */ Int32 end,
         /* [in] */ ICharSequence* chars);

     /**
      * Sets a span on the specified range of text
      * @hide
      */
     virtual CARAPI_(void) SetSpan_internal(
         /* [in] */ IInterface* span,
         /* [in] */ Int32 start,
         /* [in] */ Int32 end,
         /* [in] */ Int32 flags);

     /**
      * Moves the cursor to the specified offset position in text
      * @hide
      */
     virtual CARAPI_(void) SetCursorPosition_internal(
         /* [in] */ Int32 start,
         /* [in] */ Int32 end);

     CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

     CARAPI Init(
         /* [in] */ IContext* context);

     CARAPI Init(
         /* [in] */ IContext* context,
         /* [in] */ IAttributeSet* attrs);

     CARAPI Init(
         /* [in] */ IContext* context,
         /* [in] */ IAttributeSet* attrs,
         /* [in] */ Int32 defStyleAttr);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);


private:
    CARAPI_(void) SetTypefaceFromAttrs(
        /* [in] */ const String& familyName,
        /* [in] */ Int32 typefaceIndex,
        /* [in] */ Int32 styleIndex);

    CARAPI_(void) SetRelativeDrawablesIfNeeded(
        /* [in] */ IDrawable* start,
        /* [in] */ IDrawable* end);

    CARAPI_(void) SetKeyListenerOnly(
        /* [in] */ IKeyListener* input);

    CARAPI_(void) FixFocusableAndClickableSettings();

    CARAPI_(void) SetRawTextSize(
        /* [in] */ Float size);

    CARAPI_(void) UpdateTextColors();

    CARAPI SetText(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type,
        /* [in] */ Boolean notifyBefore,
        /* [in] */ Int32 oldlen);

    static CARAPI_(Boolean) IsMultilineInputType(
        /* [in] */ Int32 type);

    /**
     * It would be better to rely on the input type for everything. A password inputType should have
     * a password transformation. We should hence use isPasswordInputType instead of this method.
     *
     * We should:
     * - Call setInputType in setKeyListener instead of changing the input type directly (which
     * would install the correct transformation).
     * - Refuse the installation of a non-password transformation in setTransformation if the input
     * type is password.
     *
     * However, this is like this for legacy reasons and we cannot break existing apps. This method
     * is useful since it matches what the user can see (obfuscated text or not).
     *
     * @return true if the current transformation method is of the password type.
     */
    CARAPI_(Boolean) HasPasswordTransformationMethod();

    CARAPI_(Boolean) IsPasswordInputType(
        /* [in] */ Int32 inputType);

    static CARAPI_(Boolean) IsVisiblePasswordInputType(
        /* [in] */ Int32 inputType);

    CARAPI_(void) SetInputType(
        /* [in] */ Int32 type,
        /* [in] */ Boolean direct);

    CARAPI_(void) RestartMarqueeIfNeeded();

    /**
     * Sets the list of input filters on the specified Editable,
     * and includes mInput in the list if it is an InputFilter.
     */
    CARAPI SetFilters(
        /* [in] */ IEditable* e,
        /* [in] */ ArrayOf<Elastos::Droid::Text::IInputFilter*>* filters);

    CARAPI_(Int32) GetBottomVerticalOffset(
        /* [in] */ Boolean forceNormal);

    CARAPI_(void) InvalidateCursor(
        /* [in] */ Int32 a,
        /* [in] */ Int32 b,
        /* [in] */ Int32 c);

    CARAPI_(void) RegisterForPreDraw();

    CARAPI_(AutoPtr<IPath>) GetUpdatedHighlightPath();

    /**
     * Returns true if pressing ENTER in this field advances focus instead
     * of inserting the character.  This is true mostly in single-line fields,
     * but also in mail addresses and subjects which will display on multiple
     * lines but where it doesn't make sense to insert newlines.
     */
    CARAPI_(Boolean) ShouldAdvanceFocusOnEnter();

    /**
     * Returns true if pressing TAB in this field advances focus instead
     * of inserting the character.  Insert tabs only in multi-line editors.
     */
    CARAPI_(Boolean) ShouldAdvanceFocusOnTab();

    CARAPI_(Int32) DoKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ IKeyEvent* otherEvent);

    CARAPI_(void) NullLayouts();

    /**
     * Make a new Layout based on the already-measured size of the view,
     * on the assumption that it was measured correctly at some point.
     */
    CARAPI_(void) AssumeLayout();

    CARAPI_(LayoutAlignment) GetLayoutAlignment();

    CARAPI_(AutoPtr<ILayout>) MakeSingleLayout(
        /* [in] */ Int32 wantWidth,
        /* [in] */ IBoringLayoutMetrics* boring,
        /* [in] */ Int32 ellipsisWidth,
        /* [in] */ LayoutAlignment alignment,
        /* [in] */ Boolean shouldEllipsize,
        /* [in] */ TextUtilsTruncateAt effectiveEllipsize,
        /* [in] */ Boolean useSaved);

    CARAPI_(Boolean) CompressText(
        /* [in] */ Float width);

    static CARAPI_(Int32) Desired(
        /* [in] */ ILayout* layout);

    CARAPI_(Int32) GetDesiredHeight();

    CARAPI_(Int32) GetDesiredHeight(
        /* [in] */ ILayout* layout,
        /* [in] */ Boolean cap);

    /**
     * Check whether a change to the existing text layout requires a
     * new view layout.
     */
    CARAPI_(void) CheckForResize();

    /**
     * Check whether entirely new text requires a new view layout
     * or merely a new text layout.
     */
    CARAPI_(void) CheckForRelayout();

    CARAPI_(Boolean) IsShowingHint();

    /**
     * Returns true if anything changed.
     */
    CARAPI_(Boolean) BringTextIntoView();

    CARAPI_(void) GetInterestingRect(
        /* [in] */ IRect* r,
        /* [in] */ Int32 line);

    CARAPI_(void) ConvertFromViewportToContentCoordinates(
        /* [in] */ IRect* r);

    /**
     * Adds or remove the EditorInfo.TYPE_TEXT_FLAG_MULTI_LINE on the mInputType.
     * @param singleLine
     */
    CARAPI_(void) SetInputTypeSingleLine(
        /* [in] */ Boolean singleLine);

    CARAPI ApplySingleLine(
        /* [in] */ Boolean singleLine,
        /* [in] */ Boolean applyTransformation,
        /* [in] */ Boolean changeMaxLines);

    CARAPI_(Boolean) CanMarquee();

    CARAPI_(void) StartMarquee();

    CARAPI_(void) StopMarquee();

    CARAPI_(void) StartStopMarquee(
        /* [in] */ Boolean start);

    CARAPI_(void) SendBeforeTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    // Removes all spans that are inside or actually overlap the start..end range
    CARAPI_(void) RemoveIntersectingNonAdjacentSpans(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const InterfaceID& type);

    CARAPI_(Boolean) CanSelectText();

    CARAPI_(void) UpdateTextServicesLocaleAsync();

    CARAPI_(void) UpdateTextServicesLocaleLocked();

    /**
     * @return true if the user has explicitly allowed accessibility services
     * to speak passwords.
     */
    CARAPI_(Boolean) ShouldSpeakPasswordsForAccessibility();

    CARAPI_(void) Paste(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    CARAPI_(void) SetPrimaryClip(
        /* [in] */ IClipData* clip);

    CARAPI_(Int32) GetOffsetAtCoordinate(
        /* [in] */ Int32 line,
        /* [in] */ Float x);

    CARAPI_(void) CreateEditorIfNeeded();

    CARAPI_(void) InitFields();

    CARAPI_(Int32) GetBoxHeight(
        /* [in] */ ILayout* l);

    CARAPI_(void) UnregisterForPreDraw();

    CARAPI_(AutoPtr<ILocale>) GetTextServicesLocale(
        /* [in] */ Boolean allowNullLocale);

public:
    static const char* TEXT_VIEW_TAG;
    static const Boolean DEBUG_EXTRACT;

    // It is possible to have a selection even when mEditor is null (programmatically set, like when
    // a link is pressed). These highlight-related fields do not go in mEditor.
    Int32 mHighlightColor;// = 0x6633B5E5;

    // Although these fields are specific to editable text, they are not added to Editor because
    // they are defined by the TextView's style and are theme-dependent.
    Int32 mCursorDrawableRes;
    // These four fields, could be moved to Editor, since we know their default values and we
    // could condition the creation of the Editor to a non standard value. This is however
    // brittle since the hardcoded values here (such as
    // com.android.internal.R.drawable.text_select_handle_left) would have to be updated if the
    // default style is modified.

    Int32 mTextSelectHandleLeftRes;
    Int32 mTextSelectHandleRightRes;
    Int32 mTextSelectHandleRes;
    Int32 mTextEditSuggestionItemLayout;

private:
    friend class Marquee;
    friend class ChangeWatcher;
    friend class Editor;
    friend class EditText;
    friend class SuggestionsPopupWindow;
    friend class InsertionHandleView;
    friend class UserDictionaryListener;
    friend class UpdateTextServicesLocaleRunnable;

    // Enum for the "typeface" XML parameter.
    // TODO: How can we get this from the XML instead of hardcoding it here?
    static const Int32 SANS = 1;
    static const Int32 SERIF = 2;
    static const Int32 MONOSPACE = 3;

    // Bitfield for the "numeric" XML parameter.
    // TODO: How can we get this from the XML instead of hardcoding it here?
    static const Int32 SIGNED = 2;
    static const Int32 DECIMAL = 4;

    /**
     * Draw marquee text with fading edges as usual
     */
    static const Int32 MARQUEE_FADE_NORMAL = 0;

    /**
     * Draw marquee text as ellipsize end while inactive instead of with the fade.
     * (Useful for devices where the fade can be expensive if overdone)
     */
    static const Int32 MARQUEE_FADE_SWITCH_SHOW_ELLIPSIS = 1;

    /**
     * Draw marquee text with fading edges because it is currently active/animating.
     */
    static const Int32 MARQUEE_FADE_SWITCH_SHOW_FADE = 2;

    static const Int32 LINES = 1;
    static const Int32 EMS = 1;
    static const Int32 PIXELS = 2;

    static const AutoPtr<CRectF> TEMP_RECTF;// = new RectF();
    static Object sTempRectLock;

    // XXX should be much larger
    static const Int32 VERY_WIDE = 1024*1024;
    static const Int32 ANIMATED_SCROLL_GAP = 250;

    static AutoPtr<ArrayOf<Elastos::Droid::Text::IInputFilter*> > NO_FILTERS;// = new InputFilter[0];
    static AutoPtr<ISpanned> EMPTY_SPANNED;// = new SpannedString("");

    static const Int32 CHANGE_WATCHER_PRIORITY = 100;

    // New state used to change background based on whether this TextView is multiline.
    static AutoPtr<ArrayOf<Int32> > MULTILINE_STATE_SET;// = { R.attr.state_multiline };

    // System wide time for last cut or copy action.
    static Int64 LAST_CUT_OR_COPY_TIME;

    static AutoPtr<IBoringLayoutMetrics> UNKNOWN_BORING;

    AutoPtr<IColorStateList> mTextColor;
    AutoPtr<IColorStateList> mHintTextColor;
    AutoPtr<IColorStateList> mLinkTextColor;
    Int32 mCurTextColor;
    Int32 mCurHintTextColor;
    Boolean mFreezesText;
    Boolean mTemporaryDetach;
    Boolean mDispatchTemporaryDetach;

    AutoPtr<IEditableFactory> mEditableFactory;
    AutoPtr<ISpannableFactory> mSpannableFactory;

    Float mShadowRadius;
    Float mShadowDx;
    Float mShadowDy;

    Int32 mShadowColor;

    Boolean mPreDrawRegistered;
    Boolean mPreDrawListenerDetached;

    Boolean mPreventDefaultMovement;

    TextUtilsTruncateAt mEllipsize;

    AutoPtr<Drawables> mDrawables;

    AutoPtr<CharWrapper> mCharWrapper;

    AutoPtr<Marquee> mMarquee;
    Boolean mRestartMarquee;

    Int32 mMarqueeRepeatLimit;// = 3

    Int32 mLastLayoutDirection;// = -1;

    /**
     * On some devices the fading edges add a performance penalty if used
     * extensively in the same layout. This mode indicates how the marquee
     * is currently being shown, if applicable. (mEllipsize will == MARQUEE)
     */
    Int32 mMarqueeFadeMode;// = MARQUEE_FADE_NORMAL;

    /**
     * When mMarqueeFadeMode is not MARQUEE_FADE_NORMAL, this stores
     * the layout that should be used when the mode switches.
     */
    AutoPtr<ILayout> mSavedMarqueeModeLayout;

    //@ViewDebug.ExportedProperty(category = "text")
    AutoPtr<ICharSequence> mText;
    AutoPtr<ICharSequence> mTransformed;
    BufferType mBufferType;// = BufferType.NORMAL;

    AutoPtr<ICharSequence> mHint;
    AutoPtr<ILayout> mHintLayout;

    AutoPtr<IMovementMethod> mMovement;

    AutoPtr<ITransformationMethod> mTransformation;
    Boolean mAllowTransformationLengthChange;
    AutoPtr<ChangeWatcher> mChangeWatcher;

    List<AutoPtr<ITextWatcher> > mListeners;

    // display attributes
    AutoPtr<ITextPaint> mTextPaint;
    Boolean mUserSetTextScaleX;
    AutoPtr<ILayout> mLayout;

    Int32 mGravity;// = Gravity.TOP | Gravity.START;
    Boolean mHorizontallyScrolling;

    Int32 mAutoLinkMask;
    Boolean mLinksClickable;// = true;

    Float mSpacingMult;// = 1.0f;
    Float mSpacingAdd;// = 0.0f;

    Int32 mMaximum;// = Integer.MAX_VALUE;
    Int32 mMaxMode;// = LINES;
    Int32 mMinimum;// = 0;
    Int32 mMinMode;// = LINES;

    Int32 mOldMaximum;// = mMaximum;
    Int32 mOldMaxMode;// = mMaxMode;

    Int32 mMaxWidth;// = Integer.MAX_VALUE;
    Int32 mMaxWidthMode;// = PIXELS;
    Int32 mMinWidth;// = 0;
    Int32 mMinWidthMode;// = PIXELS;

    Boolean mSingleLine;
    Int32 mDesiredHeightAtMeasure;// = -1;
    Boolean mIncludePad;// = true;
    Int32 mDeferScroll;// = -1;

    // tmp primitives, so we don't alloc them on each draw
    AutoPtr<IRect>mTempRect;
    Int64 mLastScroll;
    AutoPtr<IScroller> mScroller;

    AutoPtr<IBoringLayoutMetrics> mBoring, mHintBoring;
    AutoPtr<IBoringLayout> mSavedLayout, mSavedHintLayout;

    AutoPtr<ITextDirectionHeuristic> mTextDir;

    AutoPtr<ArrayOf<Elastos::Droid::Text::IInputFilter*> > mFilters;// = NO_FILTERS;

    /* volatile */ AutoPtr<ILocale> mCurrentSpellCheckerLocaleCache;

    AutoPtr<IPath> mHighlightPath;
    AutoPtr<IPaint> mHighlightPaint;
    Boolean mHighlightPathBogus;// = true;

    /**
     * EditText specific data, created on demand when one of the Editor fields is used.
     * See {@link #createEditorIfNeeded()}.
     */
    AutoPtr<IEditor> mEditor;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_TEXTVIEW_H__
