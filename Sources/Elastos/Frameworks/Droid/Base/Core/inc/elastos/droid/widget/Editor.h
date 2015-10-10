
#ifndef __ELASTOS_DROID_WIDGET_EDITOR_H__
#define __ELASTOS_DROID_WIDGET_EDITOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "view/ViewTreeObserver.h"
#include "view/ViewGroup.h"


#include "text/style/CSuggestionSpan.h"

#include "widget/BaseAdapter.h"

#include "os/Runnable.h"
#include "os/HandlerBase.h"
#include "os/HandlerRunnable.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;
using Elastos::Core::IComparator;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::HandlerRunnable;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::EIID_IBundle;
using Elastos::Droid::Content::IContext;
using namespace Elastos::Droid::Graphics;
using Elastos::Droid::Text::ISpanWatcher;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::ISpannableStringBuilder;
using Elastos::Droid::Text::ILayout;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::Style::ISuggestionSpan;
using Elastos::Droid::Text::Style::IEasyEditSpan;
using Elastos::Droid::Text::Style::ITextAppearanceSpan;
using Elastos::Droid::Text::Style::ISuggestionRangeSpan;
using Elastos::Droid::Text::Method::IWordIterator;
using Elastos::Droid::Text::Method::IKeyListener;
using Elastos::Droid::View::ViewTreeObserver;
using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IDisplayList;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IDragEvent;
using Elastos::Droid::View::IDragShadowBuilder;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IOnPreDrawListener;
using Elastos::Droid::View::IOnTouchModeChangeListener;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::InputMethod::IExtractedTextRequest;
using Elastos::Droid::View::InputMethod::IExtractedText;
using Elastos::Droid::View::InputMethod::ICorrectionInfo;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;

namespace Elastos {
namespace Droid {
namespace Widget {

class TextView;
class Editor;
class ActionPopupWindow;
class SuggestionsPopupWindow;
class InsertionHandleView;
class EasyEditSpanController;
class HandleView;

//==============================================================================
//              EditorRunnable
//==============================================================================

class ActionPopupShowerRunnable : public Runnable
{
public:
    ActionPopupShowerRunnable(
        /* [in] */ ActionPopupWindow* host)
        : mHost(host)
    {}

    virtual ECode Run();
private:
    AutoPtr<ActionPopupWindow> mHost;
};

class InsertionHandleViewHiderRunnable : public Runnable
{
public:
    InsertionHandleViewHiderRunnable(
        /* [in] */ InsertionHandleView* host)
        : mHost(host)
    {}

    virtual ECode Run();

private:
    InsertionHandleView* mHost;
};

class HidePopupRunnable : public Runnable
{
public:
    HidePopupRunnable(
        /* [in] */ EasyEditSpanController* host)
        : mHost(host)
    {}

    virtual ECode Run();

private:
    EasyEditSpanController* mHost;
};

class ShowSuggestionRunnable : public Runnable
{
public:
    ShowSuggestionRunnable(
        /* [in] */ Editor* host)
        : mHost(host)
    {}

    virtual ECode Run();

private:
    Editor* mHost;
};

//==============================================================================
//              InputContentType
//==============================================================================
class InputContentType : public ElRefBase
{
public:
    InputContentType();

    Int32 mImeOptions;
    String mPrivateImeOptions;
    AutoPtr<ICharSequence> mImeActionLabel;
    Int32 mImeActionId;
    AutoPtr<IBundle> mExtras;
    AutoPtr<IOnEditorActionListener> mOnEditorActionListener;
    Boolean mEnterDown;
};

//==============================================================================
//              InputMethodState
//==============================================================================

class InputMethodState  : public ElRefBase
{
public:
    InputMethodState();

    AutoPtr<IRect> mCursorRectInWindow;// = new Rect();
    AutoPtr<IRectF> mTmpRectF;// = new RectF();
    AutoPtr<ArrayOf<Float> > mTmpOffset;// = new Float[2];
    AutoPtr<IExtractedTextRequest> mExtractedTextRequest;
    AutoPtr<IExtractedText> mExtractedText;// = new ExtractedText();
    Int32 mBatchEditNesting;
    Boolean mCursorChanged;
    Boolean mSelectionModeChanged;
    Boolean mContentChanged;
    Int32 mChangedStart;
    Int32 mChangedEnd;
    Int32 mChangedDelta;
};

//==============================================================================
//              CustomPopupWindow
//==============================================================================
class _CustomPopupWindow
    : public PopupWindow
{
public:
    _CustomPopupWindow(
        /* [in] */ IContext* context,
        /* [in] */ Int32 defStyle,
        /* [in] */ Editor* editor,
        /* [in] */ SuggestionsPopupWindow* owner);

    virtual CARAPI Dismiss();

private:
    Editor* mEditor;
    SuggestionsPopupWindow* mOwner;
};

class CustomPopupWindow
    : public ElRefBase
    , public _CustomPopupWindow
    , public IPopupWindow
{
public:
    CAR_INTERFACE_DECL()
    IPOPUPWINDOW_METHODS_DECL()

    CustomPopupWindow(
        /* [in] */ IContext* context,
        /* [in] */ Int32 defStyle,
        /* [in] */ Editor* editor,
        /* [in] */ SuggestionsPopupWindow* owner);
};

//==============================================================================
//              TextViewPositionListener
//==============================================================================
class TextViewPositionListener
    : public ElRefBase
{
public:
    virtual ~TextViewPositionListener() {}
    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Int32 parentPositionX,
        /* [in] */ Int32 parentPositionY,
        /* [in] */ Boolean parentPositionChanged,
        /* [in] */ Boolean parentScrolled) = 0;
};

//==============================================================================
//              MyPopupWindow
//==============================================================================
class MyPopupWindow
    : public ElRefBase
    , public PopupWindow
    , public IPopupWindow
{
public:
    CAR_INTERFACE_DECL()
    IPOPUPWINDOW_METHODS_DECL()

    MyPopupWindow(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::popupWindowStyle);
};

//==============================================================================
//              PinnedPopupWindow
//==============================================================================
class PinnedPopupWindow : public TextViewPositionListener
{
public:
    PinnedPopupWindow(
        /* [in] */ Editor* editor);

    virtual ~PinnedPopupWindow();

    CARAPI Show();

    CARAPI Hide();

    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Int32 parentPositionX,
        /* [in] */ Int32 parentPositionY,
        /* [in] */ Boolean parentPositionChanged,
        /* [in] */ Boolean parentScrolled);

    CARAPI_(Boolean) IsShowing();

    virtual CARAPI Init();

protected:
    CARAPI MeasureContent();

    virtual CARAPI CreatePopupWindow() = 0;
    virtual CARAPI InitContentView() = 0;
    virtual CARAPI_(Int32) GetTextOffset() = 0;
    virtual CARAPI_(Int32) GetVerticalLocalPosition(
        /* [in] */ Int32 line) = 0;
    virtual CARAPI_(Int32) ClipVertically(
        /* [in] */ Int32 positionY) = 0;

private:
    CARAPI ComputeLocalPosition();

    CARAPI_(void) UpdatePosition(
        /* [in] */ Int32 parentPositionX,
        /* [in] */ Int32 parentPositionY);

protected:
    Editor* mEditor;
    AutoPtr<IPopupWindow> mPopupWindow;
    AutoPtr<IViewGroup> mContentView;
    Int32 mPositionX;
    Int32 mPositionY;
};

//==============================================================================
//              EasyEditPopupWindow
//==============================================================================
class EasyEditSpanController;

class EasyEditPopupWindow
    : public PinnedPopupWindow
    , public IViewOnClickListener
{
public:
    CAR_INTERFACE_DECL()

    EasyEditPopupWindow(
        /* [in] */ Editor* editor);

    virtual CARAPI CreatePopupWindow();

    virtual CARAPI InitContentView();

    virtual CARAPI_(Int32) GetTextOffset();

    virtual CARAPI_(Int32) GetVerticalLocalPosition(
        /* [in] */ Int32 line);

    virtual CARAPI_(Int32) ClipVertically(
        /* [in] */ Int32 positionY);

    CARAPI_(void) SetEasyEditSpan(
        /* [in] */ IEasyEditSpan* easyEditSpan);

    virtual CARAPI OnClick(
        /* [in] */ IView* view);

private:
    friend class EasyEditSpanController;
    static const Int32 POPUP_TEXT_LAYOUT;
    AutoPtr<ITextView> mDeleteTextView;
    AutoPtr<IEasyEditSpan> mEasyEditSpan;
};

//==============================================================================
//              SuggestionInfo
//==============================================================================
class SuggestionInfo
    : public ElRefBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL()

    SuggestionInfo();

    // range of actual suggestion within text
    Int32 mSuggestionStart;
    Int32 mSuggestionEnd;

    // the SuggestionSpan that this TextView represents
    AutoPtr<ISuggestionSpan> mSuggestionSpan;

    // the index of this suggestion inside suggestionSpan
    Int32 mSuggestionIndex;

    AutoPtr<ISpannableStringBuilder> mText;
    AutoPtr<ITextAppearanceSpan> mHighlightSpan;
};

//==============================================================================
//              SuggestionAdapter
//==============================================================================
class _SuggestionAdapter
    : public BaseAdapter
{
public:
    _SuggestionAdapter(
        /* [in] */ Editor* editor,
        /* [in] */ SuggestionsPopupWindow* popupWindow);

    virtual CARAPI_(Int32) GetCount();

    virtual CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);
private:
    Editor* mEditor;
    SuggestionsPopupWindow* mPopupWindow;
    AutoPtr<ILayoutInflater> mInflater;
};

class SuggestionAdapter
    : public _SuggestionAdapter
    , public IBaseAdapter
    , public ISpinnerAdapter
    , public ElRefBase
{
public:
    CAR_INTERFACE_DECL()

    IADAPTER_METHODS_DECL()
    IBASEADAPTER_METHODS_DECL()
    ILISTADAPTER_METHODS_DECL()
    ISPINNERADAPTER_METHODS_DECL()

    SuggestionAdapter(
        /* [in] */ Editor* editor,
        /* [in] */ SuggestionsPopupWindow* popupWindow);
};

//==============================================================================
//              SuggestionSpanComparator
//==============================================================================
class SuggestionsPopupWindow;

class SuggestionSpanComparator
    : public IComparator
    , public ElRefBase
{
public:
    CAR_INTERFACE_DECL()

    SuggestionSpanComparator(
        /* [in] */ SuggestionsPopupWindow* host);

    CARAPI Compare(
        /* [in] */ IInterface* lhs,
        /* [in] */ IInterface* rhs,
        /* [out] */ Int32* result);
private:
    SuggestionsPopupWindow* mHost;
};

//==============================================================================
//              SuggestionsPopupWindow
//==============================================================================
class SuggestionsPopupWindow
    : public PinnedPopupWindow
    , public IAdapterViewOnItemClickListener
{
public:
    CAR_INTERFACE_DECL()

    typedef HashMap<AutoPtr<ISuggestionSpan>, AutoPtr<IInteger32> > SuggestionSpanHashMap;
    typedef typename SuggestionSpanHashMap::Iterator SuggestionSpanIterator;

public:
    SuggestionsPopupWindow(
        /* [in] */ Editor* editor);

    virtual ~SuggestionsPopupWindow();

    virtual CARAPI CreatePopupWindow();

    virtual CARAPI InitContentView();

    virtual CARAPI_(Int32) GetTextOffset();

    virtual CARAPI_(Int32) GetVerticalLocalPosition(
        /* [in] */ Int32 line);

    virtual CARAPI_(Int32) ClipVertically(
        /* [in] */ Int32 positionY);

    CARAPI_(Boolean) IsShowingUp();

    CARAPI_(void) OnParentLostFocus();

    CARAPI_(AutoPtr<ArrayOf<ISuggestionSpan*> >) GetSuggestionSpans();

    CARAPI Show();

    CARAPI Hide();

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

protected:
    CARAPI MeasureContent();

private:
    CARAPI_(Boolean) UpdateSuggestions();

    CARAPI_(void) HighlightTextDifferences(
        /* [in] */ SuggestionInfo* suggestionInfo,
        /* [in] */ Int32 unionStart,
        /* [in] */ Int32 unionEnd);

    CARAPI_(void) QuickSort(ArrayOf<ISuggestionSpan*>* array, Int32 low, Int32 high);

private:
    friend class SuggestionSpanComparator;
    friend class _CustomPopupWindow;
    friend class CustomPopupWindow;
    friend class _SuggestionAdapter;
    friend class SuggestionAdapter;

    static const Int32 MAX_NUMBER_SUGGESTIONS;// = SuggestionSpan.SUGGESTIONS_MAX_SIZE;
    static const Int32 ADD_TO_DICTIONARY;// = -1;
    static const Int32 DELETE_TEXT;// = -2;
    AutoPtr<ArrayOf<SuggestionInfo *> > mSuggestionInfos;
    Int32 mNumberOfSuggestions;
    Boolean mCursorWasVisibleBeforeSuggestions;
    Boolean mIsShowingUp;// = false;
    AutoPtr<SuggestionAdapter> mSuggestionsAdapter;
    AutoPtr<SuggestionSpanComparator> mSuggestionSpanComparator;
    SuggestionSpanHashMap mSpansLengths;
};

/**
 * An ActionMode Callback class that is used to provide actions while in text selection mode.
 *
 * The default callback provides a subset of Select All, Cut, Copy and Paste actions, depending
 * on which of these this TextView supports.
 */
class SelectionActionModeCallback
    : public ElRefBase
    , public IActionModeCallback
{
public:
    CAR_INTERFACE_DECL()

    SelectionActionModeCallback(
        /* [in] */ Editor* editor);

    CARAPI OnCreateActionMode(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    CARAPI OnPrepareActionMode(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    CARAPI OnActionItemClicked(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result);

    CARAPI OnDestroyActionMode(
        /* [in] */ IActionMode* mode);

private:
    Editor*   mEditor;
};

//==============================================================================
//              ActionPopupWindow
//==============================================================================
class ActionPopupWindow
    : public PinnedPopupWindow
    , public IViewOnClickListener
{
public:
    CAR_INTERFACE_DECL()

    ActionPopupWindow(
        /* [in] */ Editor* editor);

    virtual ~ActionPopupWindow();

    virtual CARAPI CreatePopupWindow();

    virtual CARAPI InitContentView();

    virtual CARAPI_(Int32) GetTextOffset();

    virtual CARAPI_(Int32) GetVerticalLocalPosition(
        /* [in] */ Int32 line);

    virtual CARAPI_(Int32) ClipVertically(
        /* [in] */ Int32 positionY);

    CARAPI Show();

    CARAPI OnClick(
        /* [in] */ IView* view);

private:
    static const Int32 POPUP_TEXT_LAYOUT;// = com.android.internal.R.layout.text_edit_action_popup_text;
    AutoPtr<ITextView> mPasteTextView;
    AutoPtr<ITextView> mReplaceTextView;
};

//==============================================================================
//              HandleView
//==============================================================================

class HandleView
    : public Elastos::Droid::View::IView
    , public Elastos::Droid::Graphics::Drawable::IDrawableCallback
    , public Elastos::Droid::View::IKeyEventCallback
    , public Elastos::Droid::View::Accessibility::IAccessibilityEventSource
    , public Elastos::Droid::View::View
    , public TextViewPositionListener
    , public IWeakReferenceSource
{
public:
    CAR_INTERFACE_DECL()
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    HandleView(
        /* [in] */ IDrawable* drawableLtr,
        /* [in] */ IDrawable* drawableRtl,
        /* [in] */ Editor* editor);

    CARAPI_(Boolean) OffsetHasBeenChanged();

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) Show();

    virtual CARAPI_(void) Hide();

    virtual CARAPI_(void) ShowActionPopupWindow(
        /* [in] */ Int32 delay);

    CARAPI_(Boolean) IsShowing();

    virtual CARAPI_(Int32) GetCurrentCursorOffset() = 0;

    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Float x,
        /* [in] */ Float y) = 0;

    CARAPI_(Boolean) IsDragging();

    virtual CARAPI_(void) OnHandleMoved();

    virtual CARAPI_(void) OnDetached();

    CARAPI_(void) Init();

protected:
    CARAPI_(void) UpdateDrawable();

    virtual CARAPI_(Int32) GetHotspotX(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Boolean isRtlRun) = 0;

    CARAPI_(void) Dismiss();

    virtual CARAPI_(void) HideActionPopupWindow();

    virtual CARAPI_(void) UpdateSelection(
        /* [in] */ Int32 offset) = 0;

    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Int32 parentPositionX,
        /* [in] */ Int32 parentPositionY,
        /* [in] */ Boolean parentPositionChanged,
        /* [in] */ Boolean parentScrolled);

    CARAPI_(void) PositionAtCursorOffset(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean parentScrolled);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* event);

private:
    CARAPI_(void) StartTouchUpFilter(
        /* [in] */ Int32 offset);

    CARAPI_(void) AddPositionToTouchUpFilter(
        /* [in] */ Int32 offset);

    CARAPI_(void) FilterOnTouchUp();

    CARAPI_(Boolean) IsVisible();

protected:
    Editor* mEditor;

protected:
    AutoPtr<IDrawable> mDrawable;
    AutoPtr<IDrawable> mDrawableLtr;
    AutoPtr<IDrawable> mDrawableRtl;
    Int32 mHotspotX;
    // Transient action popup window for Paste and Replace actions
    AutoPtr<ActionPopupWindow> mActionPopupWindow;

private:
    // Touch-up filter: number of previous positions remembered
    static const Int32 HISTORY_SIZE = 5;
    static const Int32 TOUCH_UP_FILTER_DELAY_AFTER = 150;
    static const Int32 TOUCH_UP_FILTER_DELAY_BEFORE = 350;

    AutoPtr<IPopupWindow> mContainer;

    // Position with respect to the parent TextView
    Int32 mPositionX;
    Int32 mPositionY;

    Boolean mIsDragging;

    // Offset from touch position to mPosition
    Float mTouchToWindowOffsetX;
    Float mTouchToWindowOffsetY;

    // Offsets the hotspot point up, so that cursor is not hidden by the finger when moving up
    Float mTouchOffsetY;

    // Where the touch position should be on the handle to ensure a maximum cursor visibility
    Float mIdealVerticalOffset;

    // Parent's (TextView) previous position in window
    Int32 mLastParentX;
    Int32 mLastParentY;

    // Previous text character offset
    Int32 mPreviousOffset;// = -1;

    // Previous text character offset
    Boolean mPositionHasChanged;// = true;

    // Used to delay the appearance of the action popup window
    AutoPtr<IRunnable> mActionPopupShower;

    AutoPtr<ArrayOf<Int64> > mPreviousOffsetsTimes;//= new long[HISTORY_SIZE];
    AutoPtr<ArrayOf<Int32> > mPreviousOffsets;// = new Int32[HISTORY_SIZE];
    Int32 mPreviousOffsetIndex;// = 0;
    Int32 mNumberPreviousOffsets;// = 0;
};

//==============================================================================
//              InsertionHandleView
//==============================================================================
class InsertionHandleView : public HandleView
{
public:
    CAR_INTERFACE_DECL()

    InsertionHandleView(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Editor* editor);

    virtual void Show();

    void ShowWithActionPopup();

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Int32) GetCurrentCursorOffset();

    virtual CARAPI_(void) UpdateSelection(
        /* [in] */ Int32 offset);

    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    virtual CARAPI_(void) OnHandleMoved();

    virtual CARAPI_(void) OnDetached();

protected:
    virtual CARAPI_(Int32) GetHotspotX(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Boolean isRtlRun);

private:
    CARAPI_(void) HideAfterDelay();

    CARAPI_(void) RemoveHiderCallback();


private:
    static const Int32 DELAY_BEFORE_HANDLE_FADES_OUT;
    static const Int32 RECENT_CUT_COPY_DURATION;

    // Used to detect taps on the insertion handle, which will affect the ActionPopupWindow
    Float mDownPositionX;
    Float mDownPositionY;
    AutoPtr<IRunnable> mHider;
};

//==============================================================================
//              SelectionStartHandleView
//==============================================================================
class SelectionStartHandleView : public HandleView
{
public:
    CAR_INTERFACE_DECL()

    SelectionStartHandleView(
        /* [in] */ IDrawable* drawableLtr,
        /* [in] */ IDrawable* drawableRtl,
        /* [in] */ Editor* editor);

    virtual CARAPI_(Int32) GetCurrentCursorOffset();

    virtual CARAPI_(void) UpdateSelection(
        /* [in] */ Int32 offset);

    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    AutoPtr<ActionPopupWindow> GetActionPopupWindow();

protected:
    virtual CARAPI_(Int32) GetHotspotX(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Boolean isRtlRun);
};

//==============================================================================
//              SelectionEndHandleView
//==============================================================================
class SelectionEndHandleView : public HandleView
{
public:
    CAR_INTERFACE_DECL()

    SelectionEndHandleView(
        /* [in] */ IDrawable* drawableLtr,
        /* [in] */ IDrawable* drawableRtl,
        /* [in] */ Editor* editor);

    virtual CARAPI_(Int32) GetCurrentCursorOffset();

    virtual CARAPI_(void) UpdateSelection(
        /* [in] */ Int32 offset);

    virtual CARAPI_(void) UpdatePosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(void) SetActionPopupWindow(
        /* [in] */ ActionPopupWindow* actionPopupWindow);

protected:
    virtual CARAPI_(Int32) GetHotspotX(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Boolean isRtlRun);

};

//==============================================================================
//              CursorController
//==============================================================================
/**
 * A CursorController instance can be used to control a cursor in the text.
 */
class CursorController
    : public ElRefBase
    , public IOnTouchModeChangeListener
{
public:
    CAR_INTERFACE_DECL()

    CursorController(
        /* [in] */ Editor* editor);

    /**
     * Makes the cursor controller visible on screen.
     * See also {@link #hide()}.
     */
    virtual CARAPI Show();

    /**
     * Hide the cursor controller from screen.
     * See also {@link #show()}.
     */
    virtual CARAPI Hide();

    /**
     * Called when the view is detached from window. Perform house keeping task, such as
     * stopping Runnable thread that would otherwise keep a reference on the context, thus
     * preventing the activity from being recycled.
     */
    virtual CARAPI OnDetached();

    /**
     * Callback method to be invoked when the touch mode changes.
     *
     * @param isInTouchMode True if the view hierarchy is now in touch mode, false  otherwise.
     */
    virtual CARAPI OnTouchModeChanged(
        /* [in]*/ Boolean isInTouchMode);

protected:
    Editor* mEditor;
};

//==============================================================================
//              InsertionPointCursorController
//==============================================================================

class InsertionPointCursorController : public CursorController
{
public:
    InsertionPointCursorController(
        /* [in] */ Editor* editor);

    virtual CARAPI Show();

    virtual CARAPI Hide();

    virtual CARAPI OnTouchModeChanged(
        /* [in]*/ Boolean isInTouchMode);

    virtual CARAPI OnDetached();

    CARAPI_(void) ShowWithActionPopup();

private:
    AutoPtr<InsertionHandleView> GetHandle();

private:
    AutoPtr<InsertionHandleView> mHandle;
};

//==============================================================================
//              SelectionModifierCursorController
//==============================================================================

class SelectionModifierCursorController : public CursorController
{
public:
    SelectionModifierCursorController(
        /* [in] */ Editor* editor);

    virtual CARAPI Show();

    virtual CARAPI Hide();

    virtual CARAPI OnTouchModeChanged(
        /* [in]*/ Boolean isInTouchMode);

    virtual CARAPI OnDetached();

    CARAPI_(Int32) GetMinTouchOffset();

    CARAPI_(Int32) GetMaxTouchOffset();

    CARAPI_(void) ResetTouchOffsets();

    CARAPI_(void) OnTouchEvent(
        /* [in]*/ IMotionEvent* event);

    /**
     * @return true iff this controller is currently used to move the selection start.
     */
    Boolean IsSelectionStartDragged();

private:
    CARAPI_(void) InitDrawables();

    CARAPI_(void) InitHandles();

    void UpdateMinAndMaxOffsets(
        /* [in]*/ IMotionEvent* event);

private:
    static const Int32 DELAY_BEFORE_REPLACE_ACTION;

    // The cursor controller handles, lazily created when shown.
    AutoPtr<SelectionStartHandleView> mStartHandle;
    AutoPtr<SelectionEndHandleView> mEndHandle;

    // The offsets of that last touch down event. Remembered to start selection there.
    Int32 mMinTouchOffset;
    Int32 mMaxTouchOffset;

    // Double tap detection
    Int64 mPreviousTapUpTime;
    Float mDownPositionX;
    Float mDownPositionY;
    Boolean mGestureStayedInTapRegion;
};

//==============================================================================
//              PositionListener
//==============================================================================

class PositionListener
    : public IOnPreDrawListener
    , public ElRefBase
{
public:
    CAR_INTERFACE_DECL()

    PositionListener(
        /* [in] */ Editor* editor);

    virtual CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI_(void) AddSubscriber(
        /* [in] */ TextViewPositionListener* positionListener,
        /* [in] */ Boolean canMove);

    CARAPI_(void) RemoveSubscriber(
        /* [in] */ TextViewPositionListener* positionListener);

    CARAPI_(Int32) GetPositionX();

    CARAPI_(Int32) GetPositionY();

    CARAPI_(void) UpdatePosition();

    CARAPI_(void) OnScrollChanged();

private:
    // 3 handles
    // 3 ActionPopup [replace, suggestion, easyedit] (suggestionsPopup first hides the others)
    static const Int32 MAXIMUM_NUMBER_OF_LISTENERS = 6;
    AutoPtr<ArrayOf<TextViewPositionListener *> > mPositionListeners;//new TextViewPositionListener[MAXIMUM_NUMBER_OF_LISTENERS];
    AutoPtr<ArrayOf<Boolean> > mCanMove;//new boolean[MAXIMUM_NUMBER_OF_LISTENERS];
    Boolean mPositionHasChanged;// = true;
    // Absolute position of the TextView with respect to its parent window
    Int32 mPositionX;
    Int32 mPositionY;
    Int32 mNumberOfListeners;
    Boolean mScrollHasChanged;
    AutoPtr<ArrayOf<Int32> > mTempCoords;// new int[2];
    Editor* mEditor;
};

//==============================================================================
//              CorrectionHighlighter
//==============================================================================
class CorrectionHighlighter : public ElRefBase
{
public:
    CorrectionHighlighter(
        /* [in] */ Editor* editor);

    void Highlight(
        /* [in] */ ICorrectionInfo* info);

    void Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 cursorOffsetVertical);

    Boolean UpdatePaint();

    Boolean UpdatePath();

    void Invalidate(
        /* [in] */ Boolean delayed);

    void StopAnimation();

private:
    AutoPtr<IPath> mPath;// = new Path();
    AutoPtr<IPaint> mPaint;// = new Paint(Paint.ANTI_ALIAS_FLAG);
    Int32 mStart;
    Int32 mEnd;
    Int64 mFadingStartTime;
    AutoPtr<IRectF> mTempRectF;
    static const Int32 FADE_OUT_DURATION = 400;
    Editor* mEditor;
};

//==============================================================================
//              ErrorPopup
//==============================================================================
class _ErrorPopup
    : public PopupWindow
{
public:
    _ErrorPopup(
        /* [in]*/ TextView* textView,
        /* [in]*/ Int32 width,
        /* [in]*/ Int32 height);

    CARAPI_(void) FixDirection(
        /* [in]*/ Boolean above);

    CARAPI_(Int32) GetResourceId(
        /* [in]*/ Int32 currentId,
        /* [in]*/ Int32 index);

    virtual CARAPI Update(
        /* [in]*/ Int32 x,
        /* [in]*/ Int32 y,
        /* [in]*/ Int32 w,
        /* [in]*/ Int32 h,
        /* [in]*/ Boolean force);

    using PopupWindow::Update;

private:
    Boolean mAbove;// = false;
    TextView* mView;
    Int32 mPopupInlineErrorBackgroundId;// = 0;
    Int32 mPopupInlineErrorAboveBackgroundId;// = 0;
};

class ErrorPopup
    : public ElRefBase
    , public _ErrorPopup
    , public IPopupWindow
{
public:
    CAR_INTERFACE_DECL()
    IPOPUPWINDOW_METHODS_DECL()

    ErrorPopup(
        /* [in]*/ TextView* textView,
        /* [in]*/ Int32 width,
        /* [in]*/ Int32 height);
};


//==============================================================================
//              Blink
//==============================================================================

class Blink
    : public HandlerRunnable
{
public:
    Blink(
        /* [in]*/ Editor* editor);

    ~Blink();

    CARAPI Run();

    CARAPI_(void) Cancel();

    CARAPI_(void) Uncancel();

private:
    Boolean mCancelled;
    Editor* mEditor;
};

//==============================================================================
//              EasyEditSpanController
//==============================================================================

class EasyEditSpanController
    : public ElRefBase
    , public ISpanWatcher
{
public:
    CAR_INTERFACE_DECL();

    EasyEditSpanController(
        /* [in] */ Editor* editor);

    CARAPI OnSpanAdded(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanRemoved(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 ostart,
        /* [in] */ Int32 oend,
        /* [in] */ Int32 nstart,
        /* [in] */ Int32 nend);

    CARAPI_(void) Hide();

private:
    static const Int32 DISPLAY_TIMEOUT_MS = 3000; // 3 secs
    AutoPtr<EasyEditPopupWindow> mPopupWindow;

    Editor* mEditor;
    AutoPtr<IRunnable> mHidePopup;
};

//==============================================================================
//              DragLocalState
//==============================================================================
class DragLocalState
    : public ElRefBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL();

    DragLocalState(
        /* [in] */ TextView* sourceTextView,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

public:
    TextView* mSourceTextView;
    Int32 mStart;
    Int32 mEnd;
};

class UserDictionaryListener
    : public HandlerBase
{
public:
    UserDictionaryListener();

    CARAPI_(void) WaitForUserDictionaryAdded(
        /* [in] */ ITextView* tv,
        /* [in] */ const String& originalWord,
        /* [in] */ Int32 spanStart,
        /* [in] */ Int32 spanEnd);

    virtual CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

private:

    CARAPI_(void) OnUserDictionaryAdded(
        /* [in] */ const String& originalWord,
        /* [in] */ const String& addedWord);

    TextView* mTextView;
    String mOriginalWord;
    Int32 mWordStart;
    Int32 mWordEnd;

};
//==============================================================================
//              Editor
//==============================================================================

/**
 * Helper class used by TextView to handle editable text views.
 *
 * @hide
 */
class Editor : public ElRefBase
{
public:
    Editor(
        /* [in] */ TextView* textView);

    virtual ~Editor();

    CARAPI SetError(
        /* [in] */ ICharSequence* error,
        /* [in] */ IDrawable* icon);

    CARAPI_(AutoPtr<IWordIterator>) GetWordIterator();

    CARAPI_(Boolean) PerformLongClick(
        /* [in] */ Boolean handled);

    CARAPI BeginBatchEdit();

    CARAPI EndBatchEdit();

    /**
     * Called by the framework in response to a text auto-correction (such as fixing a typo using a
     * a dictionnary) from the current input method, provided by it calling
     * {@link InputConnection#commitCorrection} InputConnection.commitCorrection()}. The default
     * implementation flashes the background of the corrected word to provide feedback to the user.
     *
     * @param info The auto correct info about the text that was corrected.
     */
    CARAPI OnCommitCorrection(
        /* [in] */ ICorrectionInfo* info);

    CARAPI AddSpanWatchers(
        /* [in] */ ISpannable* text);

public: /* package */

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI_(void) CreateInputContentTypeIfNeeded();

    CARAPI_(void) CreateInputMethodStateIfNeeded();

    CARAPI_(Boolean) IsCursorVisible();

    CARAPI_(void) PrepareCursorControllers();

    /**
     * Hides the insertion controller and stops text selection mode, hiding the selection controller
     */
    CARAPI_(void) HideControllers();

    CARAPI_(void) OnScreenStateChanged(
        /* [in] */ Int32 screenState);

    CARAPI_(void) AdjustInputType(
        /* [in] */ Boolean password,
        /* [in] */ Boolean passwordInputType,
        /* [in] */ Boolean webPasswordInputType,
        /* [in] */ Boolean numberPasswordInputType);

    CARAPI_(void) SetFrame();

    CARAPI_(void) OnLocaleChanged();

    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction);

    CARAPI_(void) SendOnTextChanged(
        /* [in] */ Int32 start,
        /* [in] */ Int32 after);

    CARAPI_(void) OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    CARAPI_(void) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) EnsureEndedBatchEdit();

    CARAPI_(void) FinishBatchEdit(
        /* [in] */ InputMethodState* ims);

    CARAPI_(Boolean) ExtractText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ IExtractedText* outText);

    CARAPI_(Boolean) ReportExtractedText();

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ILayout* layout,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightPaint,
        /* [in] */ Int32 cursorOffsetVertical);

    /**
     * Invalidates all the sub-display lists that overlap the specified character range
     */
    CARAPI_(void) InvalidateTextDisplayList(
        /* [in] */ ILayout* layout,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) InvalidateTextDisplayList();

    CARAPI_(void) UpdateCursorsPositions();

    /**
     * @return true if the selection mode was actually started.
     */
    CARAPI_(Boolean) StartSelectionActionMode();

    CARAPI_(void) OnTouchUpEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * @return True if this view supports insertion handles.
     */
    CARAPI_(Boolean) HasInsertionController();

    /**
     * @return True if this view supports selection handles.
     */
    CARAPI_(Boolean) HasSelectionController();

    CARAPI_(AutoPtr<InsertionPointCursorController>) GetInsertionController();

    CARAPI_(AutoPtr<SelectionModifierCursorController>) GetSelectionController();

    CARAPI_(void) ShowSuggestions();

    CARAPI_(Boolean) AreSuggestionsShown();

    CARAPI_(void) OnScrollChanged();

    CARAPI_(void) MakeBlink();

protected:
    CARAPI_(void) StopSelectionActionMode();

    CARAPI_(void) OnDrop(
        /* [in] */ IDragEvent* event);

private:
    CARAPI_(void) ShowError();

    CARAPI_(void) SetErrorIcon(
        /* [in] */ IDrawable* icon);

    CARAPI_(void) HideError();

    /**
     * Returns the Y offset to make the pointy top of the error point
     * at the middle of the error icon.
     */
    CARAPI_(Int32) GetErrorX();

    /**
     * Returns the Y offset to make the pointy top of the error point
     * at the bottom of the error icon.
     */
    CARAPI_(Int32) GetErrorY();

    CARAPI_(void) HideInsertionPointCursorController();

    CARAPI_(void) HideSpanControllers();

    CARAPI_(void) HideCursorControllers();

    CARAPI_(void) UpdateSpellCheckSpans(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean createSpellChecker);

    CARAPI_(void) SuspendBlink();

    CARAPI_(void) ResumeBlink();

    CARAPI_(void) ChooseSize(
        /* [in] */ PopupWindow* pop,
        /* [in] */ ICharSequence* text,
        /* [in] */ TextView* tv);

    /**
     * Unlike {@link TextView#textCanBeSelected()}, this method is based on the <i>current</i> state
     * of the TextView. textCanBeSelected() has to be true (this is one of the conditions to have
     * a selection controller (see {@link #prepareCursorControllers()}), but this is not sufficient.
     */
    CARAPI_(Boolean) CanSelectText();

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

    /**
     * Adjusts selection to the word under last touch offset.
     * Return true if the operation was successfully performed.
     */
    CARAPI_(Boolean) SelectCurrentWord();

    CARAPI_(Int64) GetCharRange(
        /* [in] */ Int32 offset);

    CARAPI_(Boolean) TouchPositionIsInSelection();

    CARAPI_(AutoPtr<PositionListener>) GetPositionListener();

    CARAPI_(Boolean) IsPositionVisible(
        /* [in] */ Int32 positionX,
        /* [in] */ Int32 positionY);

    CARAPI_(Boolean) IsOffsetVisible(
        /* [in] */ Int32 offset);

    /** Returns true if the screen coordinates position (x,y) corresponds to a character displayed
     * in the view. Returns false when the position is in the empty space of left/right of text.
     */
    CARAPI_(Boolean) IsPositionOnText(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(Int64) GetLastTouchOffsets();

    /**
     * Downgrades to simple suggestions all the easy correction spans that are not a spell check
     * span.
     */
    CARAPI_(void) DowngradeEasyCorrectionSpans();

    CARAPI_(Int32) GetLastTapPosition();

    CARAPI_(Boolean) ExtractTextInternal(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 partialStartOffset,
        /* [in] */ Int32 partialEndOffset,
        /* [in] */ Int32 delta,
        /* [in] */ IExtractedText* outText);

    CARAPI_(void) DrawHardwareAccelerated(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ILayout* layout,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightPaint,
        /* [in] */ Int32 cursorOffsetVertical);

    CARAPI_(Int32) GetAvailableDisplayListIndex(
        /* [in] */ ArrayOf<Int32>* blockIndices,
        /* [in] */ Int32 numberOfBlocks,
        /* [in] */ Int32 searchStartIndex);

    CARAPI_(void) DrawCursor(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 cursorOffsetVertical);

    CARAPI_(Float) GetPrimaryHorizontal(
        /* [in] */ ILayout* layout,
        /* [in] */ ILayout* hintLayout,
        /* [in] */ Int32 offset);

    CARAPI_(Boolean) ExtractedTextModeWillBeStarted();

    /**
     * @return <code>true</code> if the cursor/current selection overlaps a {@link SuggestionSpan}.
     */
    CARAPI_(Boolean) IsCursorInsideSuggestionSpan();

    /**
     * @return <code>true</code> if the cursor is inside an {@link SuggestionSpan} with
     * {@link SuggestionSpan#FLAG_EASY_CORRECT} set.
     */
    CARAPI_(Boolean) IsCursorInsideEasyCorrectionSpan();

    CARAPI_(void) UpdateCursorPosition(
        /* [in] */ Int32 cursorIndex,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom,
        /* [in] */ Float horizontal);

    /**
     * @return True when the TextView isFocused and has a valid zero-length selection (cursor).
     */
    CARAPI_(Boolean) ShouldBlink();

    CARAPI_(AutoPtr<IDragShadowBuilder>) GetTextThumbnailBuilder(
        /* [in] */ ICharSequence* text);

private:
    friend class TextView;
    friend class _CustomPopupWindow;
    friend class CustomPopupWindow;
    friend class PinnedPopupWindow;
    friend class EasyEditPopupWindow;
    friend class SuggestionsPopupWindow;
    friend class _SuggestionAdapter;
    friend class SuggestionAdapter;
    friend class ActionPopupWindow;
    friend class HandleView;
    friend class InsertionHandleView;
    friend class SelectionStartHandleView;
    friend class SelectionEndHandleView;
    friend class InsertionPointCursorController;
    friend class SelectionModifierCursorController;
    friend class PositionListener;
    friend class CorrectionHighlighter;
    friend class EasyEditSpanController;
    friend class Blink;
    friend class SelectionActionModeCallback;

    static const String TAG;
    static const Int32 BLINK = 500;

    static AutoPtr<ArrayOf<Float> > TEMP_POSITION;
    static const Int32 DRAG_SHADOW_MAX_TEXT_LENGTH = 20;

    static const Int32 EXTRACT_NOTHING = -2;
    static const Int32 EXTRACT_UNKNOWN = -1;

    // Cursor Controllers.
    AutoPtr<InsertionPointCursorController> mInsertionPointCursorController;
    AutoPtr<SelectionModifierCursorController> mSelectionModifierCursorController;
    AutoPtr<IActionMode> mSelectionActionMode;
    Boolean mInsertionControllerEnabled;
    Boolean mSelectionControllerEnabled;

    // Used to highlight a word when it is corrected by the IME
    AutoPtr<CorrectionHighlighter> mCorrectionHighlighter;

    AutoPtr<InputContentType> mInputContentType;
    AutoPtr<InputMethodState> mInputMethodState;

    AutoPtr<ArrayOf<IDisplayList*> > mTextDisplayLists;
    Int32 mLastLayoutHeight;

    Boolean mFrozenWithFocus;
    Boolean mSelectionMoved;
    Boolean mTouchFocusSelected;
//
    AutoPtr<IKeyListener> mKeyListener;
    Int32 mInputType;// = EditorInfo.TYPE_NULL;

    Boolean mDiscardNextActionUp;
    Boolean mIgnoreActionUpEvent;

    Int64 mShowCursor;
    AutoPtr<Blink> mBlink;

    Boolean mCursorVisible;// = TRUE;
    Boolean mSelectAllOnFocus;
    Boolean mTextIsSelectable;

    AutoPtr<ICharSequence> mError;
    Boolean mErrorWasChanged;
    AutoPtr<ErrorPopup> mErrorPopup;

    /**
     * This flag is set if the TextView tries to display an error before it
     * is attached to the window (so its position is still unknown).
     * It causes the error to be shown later, when onAttachedToWindow()
     * is called.
     */
    Boolean mShowErrorAfterAttach;

    Boolean mInBatchEditControllers;
    Boolean mShowSoftInputOnFocus;// = TRUE;
    Boolean mPreserveDetachedSelection;
    Boolean mTemporaryDetach;

    AutoPtr<SuggestionsPopupWindow> mSuggestionsPopupWindow;
    AutoPtr<ISuggestionRangeSpan> mSuggestionRangeSpan;
    AutoPtr<IRunnable> mShowSuggestionRunnable;

    AutoPtr<ArrayOf<IDrawable *> > mCursorDrawable;// = new Drawable[2];
    Int32 mCursorCount; // Current number of used mCursorDrawable: 0 (resource=0), 1 or 2 (split)

    AutoPtr<IDrawable> mSelectHandleLeft;
    AutoPtr<IDrawable> mSelectHandleRight;
    AutoPtr<IDrawable> mSelectHandleCenter;

    // Global listener that detects changes in the global position of the TextView
    AutoPtr<PositionListener> mPositionListener;

    Float mLastDownPositionX;
    Float mLastDownPositionY;
    AutoPtr<IActionModeCallback> mCustomSelectionActionModeCallback;

    // Set when this TextView gained focus with some text selected. Will start selection mode.
    Boolean mCreatedWithASelection;

    AutoPtr<EasyEditSpanController> mEasyEditSpanController;

    AutoPtr<IWordIterator> mWordIterator;
    AutoPtr<ISpellChecker> mSpellChecker;

    AutoPtr<IRect> mTempRect;

    TextView* mTextView;
    AutoPtr<UserDictionaryListener> mUserDictionaryListener;//= new UserDictionaryListener();
    Object mTempPositionLock;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WIDGET_EDITOR_H__
