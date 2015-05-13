#ifndef __NUMBERPICKER_H__
#define __NUMBERPICKER_H__

#include <elastos/Math.h>
#include "ext/frameworkext.h"
#include "view/VelocityTracker.h"
#include "view/accessibility/AccessibilityNodeProvider.h"
#include "text/method/NumberKeyListener.h"
#include "widget/EditText.h"
#include "widget/LinearLayout.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::Math;
using Elastos::Core::IStringBuilder;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Text::Format::IFormatter;
using Elastos::Droid::Text::Method::INumberKeyListener;
using Elastos::Droid::Text::Method::NumberKeyListener;
using Elastos::Droid::Text::ISpanned;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeProvider;
using Elastos::Droid::View::Accessibility::AccessibilityNodeProvider;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::View::IViewOnFocusChangeListener;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::VelocityTracker;
using Elastos::Droid::Widget::IEditText;
using Elastos::Droid::Widget::EditText;
using Elastos::Droid::Widget::INumberPickerFormatter;
using Elastos::Droid::Widget::INumberPickerOnValueChangeListener;
using Elastos::Droid::Widget::INumberPickerOnScrollListener;
using Elastos::Droid::Widget::INumberPickerFormatter;
using Elastos::Droid::Widget::IScroller;
using Elastos::Droid::Widget::IImageButton;

namespace Elastos {
namespace Droid {
namespace Widget {

extern "C" const InterfaceID EIID_AccessibilityNodeProviderImpl;

class NumberPicker : public LinearLayout
{
public:
    class InputTextFilter
        : public ElRefBase
        , public INumberKeyListener
        , public NumberKeyListener
        , public IInputFilter
    {
    public:
        InputTextFilter(
            /* [in] */ NumberPicker* host);

        CAR_INTERFACE_DECL()

        CARAPI_(Int32) GetInputType();

        CARAPI Filter(
            /* [in] */ ICharSequence* source,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ ISpanned* dest,
            /* [in] */ Int32 dstart,
            /* [in] */ Int32 dend,
            /* [out] */ ICharSequence** sou);

        CARAPI ClearMetaKeyStateEx(
            /* [in] */ Int64 state,
            /* [in] */ Int32 which,
            /* [out] */ Int64* ret);

        CARAPI Backspace(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* ret);

        CARAPI ForwardDelete(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* ret);
    protected:
        CARAPI_(AutoPtr<ArrayOf<Char32> >) GetAcceptedChars();

    private:
        NumberPicker* mHost;
    };

    class PressedStateHelper
        : public Runnable
    {
    public:
        PressedStateHelper(
            /* [in] */ NumberPicker* host);

        CARAPI_(void) Cancel();

        CARAPI_(void) ButtonPressDelayed(
            /* [in] */ Int32 button);

        CARAPI_(void) ButtonTapped(
            /* [in] */ Int32 button);

        CARAPI Run();

    public:
        const static Int32 BUTTON_INCREMENT = 1;
        const static Int32 BUTTON_DECREMENT = 2;

    private:
        const static Int32 MODE_PRESS = 1;
        const static Int32 MODE_TAPPED = 2;

        Int32 mManagedButton;
        Int32 mMode;

        NumberPicker* mHost;
    };

    class SetSelectionCommand
        : public Runnable
    {
    friend class NumberPicker;

    public:
        SetSelectionCommand(
            /* [in] */ INumberPicker* host);

        CARAPI Run();

    private:
        Int32 mSelectionStart;
        Int32 mSelectionEnd;

        AutoPtr<IWeakReference> mWeakHost;
    };

    class ChangeCurrentByOneFromLongPressCommand
        : public Runnable
    {
    public:
        ChangeCurrentByOneFromLongPressCommand(
            /* [in] */ NumberPicker* host);

        CARAPI_(void) SetStep(
            /* [in] */ Boolean increment);

        CARAPI Run();

    private:
        Boolean mIncrement;

        NumberPicker* mHost;
    };

    class BeginSoftInputOnLongPressCommand
        : public Runnable
    {
    public:
        BeginSoftInputOnLongPressCommand(
            /* [in] */ NumberPicker* host);

        CARAPI Run();

    private:
        NumberPicker* mHost;
    };

    class CustomEditText
        : public ElRefBase
        , public IEditText
        , public EditText
    {
    public:
        CustomEditText(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs);

        CAR_INTERFACE_DECL()

        CARAPI OnEditorAction(
            /* [in] */ Int32 actionCode);
    };

    class AccessibilityNodeProviderImpl
        : public ElRefBase
        , public IAccessibilityNodeProvider
        , public AccessibilityNodeProvider
    {
    public:
        AccessibilityNodeProviderImpl(
            /* [in] */ NumberPicker* host);

        CAR_INTERFACE_DECL()

        CARAPI CreateAccessibilityNodeInfo(
            /* [in] */ Int32 virtualViewId,
            /* [out] */ IAccessibilityNodeInfo** info);

        CARAPI FindAccessibilityNodeInfosByText(
        /* [in] */ const String& searched,
        /* [in] */ Int32 virtualViewId,
        /* [out] */ IObjectContainer** object);

        CARAPI PerformAction(
            /* [in] */ Int32 virtualViewId,
            /* [in] */ Int32 action,
            /* [in] */ IBundle* arguments,
            /* [out] */ Boolean* res);

        CARAPI_(void) SendAccessibilityEventForVirtualView(
            /* [in] */ Int32 virtualViewId,
            /* [in] */ Int32 eventType);

    private:
        CARAPI_(void) SendAccessibilityEventForVirtualText(
            /* [in] */ Int32 eventType);

        CARAPI_(void) SendAccessibilityEventForVirtualButton(
            /* [in] */ Int32 virtualViewId,
            /* [in] */ Int32 eventType,
            /* [in] */ const String& text);

        CARAPI_(void) FindAccessibilityNodeInfosByTextInChild(
            /* [in] */ const String& searchedLowerCase,
            /* [in] */ Int32 virtualViewId,
            /* [in] */ IObjectContainer* outResult);

        CARAPI_(AutoPtr<IAccessibilityNodeInfo>) CreateAccessibiltyNodeInfoForInputText();

        CARAPI_(AutoPtr<IAccessibilityNodeInfo>) CreateAccessibilityNodeInfoForVirtualButton(
            /* [in] */ Int32 virtualViewId,
            /* [in] */ const String& text,
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom);

        CARAPI_(AutoPtr<IAccessibilityNodeInfo>) CreateAccessibilityNodeInfoForNumberPicker(
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom);

        CARAPI_(Boolean) HasVirtualDecrementButton();

        CARAPI_(Boolean) HasVirtualIncrementButton();

        CARAPI_(String) GetVirtualDecrementButtonText();

        CARAPI_(String) GetVirtualIncrementButtonText();

        static CARAPI_(String) FormatNumberWithLocale(
            /* [in] */ Int32 value);

    private:
        const static Int32 UNDEFINED; // = Math::INT32_MIN_VALUE;
        const static Int32 VIRTUAL_VIEW_ID_INCREMENT = 1;
        const static Int32 VIRTUAL_VIEW_ID_INPUT = 2;
        const static Int32 VIRTUAL_VIEW_ID_DECREMENT = 3;

        AutoPtr<IRect> mTempRect;
        AutoPtr<ArrayOf<Int32> > mTempArray;

        Int32 mAccessibilityFocusedView;

        NumberPicker* mHost;

    friend class NumberPicker;
    };

    class NumberPickerOnCliskListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        NumberPickerOnCliskListener(
            /* [in] */ NumberPicker* host);

        CAR_INTERFACE_DECL()

        OnClick(
            /* [in] */ IView* v);
    private:
        NumberPicker* mHost;
    };

    class NumberPickerOnLongCliskListener
        : public ElRefBase
        , public IViewOnLongClickListener
    {
    public:
        NumberPickerOnLongCliskListener(
            /* [in] */ NumberPicker* host);

        CAR_INTERFACE_DECL()

        OnLongClick(
            /* [in] */ IView* v,
            /* [out] */ Boolean* result);
    private:
        NumberPicker* mHost;
    };

    class NumberPickerOnFocusChangeListener
        : public ElRefBase
        , public IViewOnFocusChangeListener
    {
    public:
        NumberPickerOnFocusChangeListener(
            /* [in] */ NumberPicker* host);

        CAR_INTERFACE_DECL()

        OnFocusChange(
            /* [in] */ IView* v,
            /* [in] */ Boolean hasFocus);
    private:
        NumberPicker* mHost;
    };

    class TwoDigitFormatter
        : public ElRefBase
        , public INumberPickerFormatter
    {
    public:
        TwoDigitFormatter();

        CAR_INTERFACE_DECL()

        CARAPI Format(
            /* [in] */ Int32 value,
            /* [out] */ String* str);

    private:
        CARAPI Init(
            /* [in] */ ILocale* locale);

        static CARAPI_(Char32) GetZeroDigit(
            /* [in] */ ILocale* locale);

        static CARAPI_(AutoPtr<IFormatter>) CreateFormatter(
            /* [in] */ ILocale* locale);

    private:
        StringBuilder mBuilder;
        Char32 mZeroDigit;
        AutoPtr< ArrayOf<IInterface*> > mArgs;
    };

public:
    static CARAPI_(AutoPtr<INumberPickerFormatter>) GetTwoDigitFormatter();

    NumberPicker(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyleAttr = R::attr::numberPickerStyle);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyleAttr = R::attr::numberPickerStyle);

    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) DispatchTrackballEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) DispatchHoverEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI ComputeScroll();

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI ScrollBy(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(Int32) GetSolidColor();

    virtual CARAPI SetOnValueChangedListener(
        /* [in] */ INumberPickerOnValueChangeListener* onValueChangedListener);

    virtual CARAPI SetOnScrollListener(
        /* [in] */ INumberPickerOnScrollListener* onScrollListener);

    virtual CARAPI SetFormatter(
        /* [in] */ INumberPickerFormatter* formatter);

    virtual CARAPI SetValue(
        /* [in] */ Int32 value);

    virtual CARAPI_(Boolean) GetWrapSelectorWheel();

    virtual CARAPI SetWrapSelectorWheel(
        /* [in] */ Boolean wrapSelectorWheel);

    virtual CARAPI SetOnLongPressUpdateInterval(
        /* [in] */ Int64 intervalMillis);

    virtual CARAPI_(Int32) GetValue();

    virtual CARAPI_(Int32) GetMinValue();

    virtual CARAPI SetMinValue(
        /* [in] */ Int32 minValue);

    virtual CARAPI_(Int32) GetMaxValue();

    virtual CARAPI SetMaxValue(
        /* [in] */ Int32 maxValue);

    virtual CARAPI_(AutoPtr<ArrayOf<String> >) GetDisplayedValues();

    virtual CARAPI SetDisplayedValues(
        /* [in] */ ArrayOf<String>* displayedValues);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI_(AutoPtr<IAccessibilityNodeProvider>) GetAccessibilityNodeProvider();

protected:
    NumberPicker();

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 letf,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(Float) GetTopFadingEdgeStrength();

    CARAPI_(Float) GetBottomFadingEdgeStrength();

    virtual CARAPI OnDetachedFromWindow();

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);



private:
    CARAPI_(Boolean) MoveToFinalScrollerPosition(
        /* [in] */ IScroller* scroller);

    CARAPI_(void) ShowSoftInput();

    CARAPI_(void) HideSoftInput();

    CARAPI_(void) TryComputeMaxWidth();

    CARAPI_(Int32) MakeMeasureSpec(
        /* [in] */ Int32 measureSpec,
        /* [in] */ Int32 maxSize);

    CARAPI_(Int32) ResolveSizeAndStateRespectingMinSize(
        /* [in] */ Int32 minSize,
        /* [in] */ Int32 measuredSize,
        /* [in] */ Int32 measureSpec);

    CARAPI_(void) InitializeSelectorWheelIndices();

    CARAPI_(void) SetValueInternal(
        /* [in] */ Int32 current,
        /* [in] */ Boolean notifyChange);

    CARAPI_(void) ChangeValueByOne(
        /* [in] */ Boolean increment);

    CARAPI_(void) InitializeSelectorWheel();

    CARAPI_(void) InitializeFadingEdges();

    CARAPI_(void) OnScrollerFinished(
        /* [in] */ IScroller* scroller);

    CARAPI_(void) OnScrollStateChange(
        /* [in] */ Int32 scrollState);

    CARAPI_(void) Fling(
        /* [in] */ Int32 velocityY);

    CARAPI_(Int32) GetWrappedSelectorIndex(
        /* [in] */ Int32 selectorIndex);

    CARAPI_(void) IncrementSelectorIndices(
        /* [in] */ ArrayOf<Int32>* selectorIndices);

    CARAPI_(void) DecrementSelectorIndices(
        /* [in] */ ArrayOf<Int32>* selectorIndices);

    CARAPI_(void) EnsureCachedScrollSelectorValue(
        /* [in] */ Int32 selectorIndex);

    CARAPI_(String) FormatNumber(
        /* [in] */ Int32 value);

    CARAPI_(void) ValidateInputTextView(
        /* [in] */ IView* v);

    CARAPI_(Boolean) UpdateInputTextView();

    CARAPI_(void) NotifyChange(
        /* [in] */ Int32 previous,
        /* [in] */ Int32 current);

    CARAPI_(void) PostChangeCurrentByOneFromLongPress(
        /* [in] */ Boolean increment,
        /* [in] */ Int64 delayMillis);

    CARAPI_(void) RemoveChangeCurrentByOneFromLongPress();

    CARAPI_(void) PostBeginSoftInputOnLongPressCommand();

    CARAPI_(void) RemoveBeginSoftInputCommand();

    CARAPI_(void) RemoveAllCallbacks();

    CARAPI_(Int32) GetSelectedPos(
        /* [in] */ const String& value);

    CARAPI_(void) PostSetSelectionCommand(
        /* [in] */ Int32 selectionStart,
        /* [in] */ Int64 selectionEnd);

    CARAPI_(Boolean) EnsureScrollWheelAdjusted();

private:
    friend class InputTextFilter;

    const static String NUMBERPICkER_NAME;

    const static Int32 SELECTOR_WHEEL_ITEM_COUNT = 3;

    const static Int64 DEFAULT_LONG_PRESS_UPDATE_INTERVAL = 300;

    const static Int32 SELECTOR_MIDDLE_ITEM_INDEX = SELECTOR_WHEEL_ITEM_COUNT / 2;

    const static Int32 SELECTOR_MAX_FLING_VELOCITY_ADJUSTMENT = 8;

    const static Int32 SELECTOR_ADJUSTMENT_DURATION_MILLIS = 800;

    const static Int32 SNAP_SCROLL_DURATION = 300;

    const static Float TOP_AND_BOTTOM_FADING_EDGE_STRENGTH = 0.9f;

    const static Int32 UNSCALED_DEFAULT_SELECTION_DIVIDER_HEIGHT = 2;

    const static Int32 UNSCALED_DEFAULT_SELECTION_DIVIDERS_DISTANCE = 48;

    const static Int32 DEFAULT_LAYOUT_RESOURCE_ID = R::layout::number_picker;

    const static Int32 SIZE_UNSPECIFIED = -1;

    static AutoPtr<INumberPickerFormatter> sTwoDigitFormatter;

    const static Char32 DIGIT_CHARACTERS[];

    AutoPtr<IImageButton> mInCrementButton;

    AutoPtr<IImageButton> mDecrementButton;

    AutoPtr<IEditText> mInputText;

    Int32 mSelectionDividersDistance;

    Int32 mMinHeight;

    Int32 mMaxHeight;

    Int32 mMinWidth;

    Int32 mMaxWidth;

    Boolean mComputeMaxWidth;

    Int32 mTextSize;

    Int32 mSelectorTextGapHeight;

    AutoPtr<ArrayOf<String> > mDisplayedValues;

    Int32 mMinValue;

    Int32 mMaxValue;

    Int32 mValue;

    AutoPtr<INumberPickerOnValueChangeListener> mOnValueChangeListener;

    AutoPtr<INumberPickerOnScrollListener> mOnScrollListener;

    AutoPtr<INumberPickerFormatter> mFormatter;

    Int64 mLongPressUpdateInterval;

    HashMap<Int32, String> mSelectorIndexToStringCache;

    AutoPtr<ArrayOf<Int32> > mSelectorIndices;

    AutoPtr<IPaint> mSelectorWheelPaint;

    AutoPtr<IDrawable> mVirtualButtonPressedDrawable;

    Int32 mSelectorElementHeight;

    Int32 mInitialScrollOffset;

    Int32 mCurrentScrollOffset;

    AutoPtr<IScroller> mFlingScroller;

    AutoPtr<IScroller> mAdjustScroller;

    Int32 mPreviousScrollerY;

    AutoPtr<SetSelectionCommand> mSetSelectionCommand;

    AutoPtr<ChangeCurrentByOneFromLongPressCommand> mChangeCurrentByOneFromLongPressCommand;

    AutoPtr<BeginSoftInputOnLongPressCommand> mBeginSoftInputOnLongPressCommand;

    Float mLastDownEventY;

    Int64 mLastDownEventTime;

    Float mLastDownOrMoveEventY;

    AutoPtr<VelocityTracker> mVelocityTracker;

    Int32 mTouchSlop;

    Int32 mMinimumFlingVelocity;

    Int32 mMaximumFlingVelocity;

    Boolean mWrapSelectorWheel;

    Int32 mSolidColor;

    Boolean mHasSelectorWheel;

    AutoPtr<IDrawable> mSelectionDivider;

    Int32 mSelectionDividerHeight;

    Int32 mScrollState;

    Boolean mIngonreMoveEvents;

    Boolean mShowSoftInputOnTap;

    Int32 mTopSelectionDividerTop;

    Int32 mBottomSelectionDividerBottom;

    Int32 mLastHoveredChildVirtualViewId;

    Boolean mIncrementVirtualButtonPressed;

    Boolean mDecrementVirtualButtonPressed;

    AutoPtr<AccessibilityNodeProviderImpl> mAccessibilityNodeProvider;

    AutoPtr<PressedStateHelper> mPressedStateHelper;

    AutoPtr<IViewOnClickListener> mNumberPickerOnCliskListener;

    AutoPtr<IViewOnLongClickListener> mNumberPickerOnLongCliskListener;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__NUMBERPICKER_H__

