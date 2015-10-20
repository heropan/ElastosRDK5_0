#include <elastos/core/Math.h>
#include "elastos/droid/content/res/CCompatibilityInfo.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/utility/CTypedValue.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/view/CViewConfiguration.h"
#include "elastos/droid/view/accessibility/CAccessibilityManager.h"
#include "elastos/droid/view/accessibility/CAccessibilityEvent.h"
#include "elastos/droid/view/accessibility/CAccessibilityNodeInfo.h"
#include "elastos/droid/view/inputmethod/CInputMethodManager.h"
#include "elastos/droid/widget/NumberPicker.h"
#include "elastos/droid/widget/CScroller.h"
#include "elastos/droid/widget/CNumberPicker.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;
using Elastos::Core::CInteger32;
using Elastos::Core::CStringWrapper;

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::Res::CCompatibilityInfo;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Text::Method::EIID_INumberKeyListener;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityNodeProvider;
using Elastos::Droid::View::EIID_IViewOnLongClickListener;
using Elastos::Droid::View::EIID_IViewOnFocusChangeListener;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;
using Elastos::Droid::View::Accessibility::CAccessibilityNodeInfo;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::Widget::CScroller;
using Elastos::Droid::Widget::CNumberPicker;

namespace Elastos {
namespace Droid {
namespace Widget {

// {0d646379-4c49-4e75-b38b-7b661f0d1aa1}
extern "C" const InterfaceID EIID_AccessibilityNodeProviderImpl =
    { 0x0d646379, 0x4c49, 0x4e75, { 0xb3, 0x8b, 0x7b, 0x66, 0x1f, 0x0d, 0x1a, 0xa1 } };

const String NumberPicker::NUMBERPICkER_NAME("NumberPicker");

const Int32 NumberPicker::AccessibilityNodeProviderImpl::UNDEFINED = Elastos::Core::Math::INT32_MIN_VALUE;
const Int32 NumberPicker::AccessibilityNodeProviderImpl::VIRTUAL_VIEW_ID_INCREMENT;
const Int32 NumberPicker::AccessibilityNodeProviderImpl::VIRTUAL_VIEW_ID_INPUT;
const Int32 NumberPicker::AccessibilityNodeProviderImpl::VIRTUAL_VIEW_ID_DECREMENT;

const Char32 NumberPicker::DIGIT_CHARACTERS[]= {
    // Latin digits are the common case
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // Arabic-Indic
    0x0660/*'\u0660'*/, 0x0661/*'\u0661'*/, 0x0662/*'\u0662'*/, 0x0663/*'\u0663'*/,
    0x0664/*'\u0664'*/, 0x0665/*'\u0665'*/, 0x0666/*'\u0666'*/, 0x0667/*'\u0667'*/,
    0x0668/*'\u0668'*/, 0x0669/*'\u0669'*/,
    // Extended Arabic-Indic
    0x06f0/*'\u06f0'*/, 0x06f1/*'\u06f1'*/, 0x06f2/*'\u06f2'*/, 0x06f3/*'\u06f3'*/,
    0x06f4/*'\u06f4'*/, 0x06f5/*'\u06f5'*/, 0x06f6/*'\u06f6'*/, 0x06f7/*'\u06f7'*/,
    0x06f8/*'\u06f8'*/, 0x06f9/*'\u06f9'*/
};

AutoPtr<INumberPickerFormatter> NumberPicker::sTwoDigitFormatter;

CAR_INTERFACE_IMPL(NumberPicker::InputTextFilter, INumberKeyListener)
CAR_INTERFACE_IMPL(NumberPicker::CustomEditText, IEditText)
CAR_INTERFACE_IMPL(NumberPicker::NumberPickerOnLongCliskListener, IViewOnLongClickListener)
CAR_INTERFACE_IMPL(NumberPicker::TwoDigitFormatter, INumberPickerFormatter)
CAR_INTERFACE_IMPL(NumberPicker::NumberPickerOnCliskListener, IViewOnClickListener)
CAR_INTERFACE_IMPL(NumberPicker::NumberPickerOnFocusChangeListener, IViewOnFocusChangeListener)

AutoPtr<INumberPickerFormatter> NumberPicker::GetTwoDigitFormatter()
{
    if (sTwoDigitFormatter == NULL) {
        sTwoDigitFormatter = new NumberPicker::TwoDigitFormatter();
    }
    return sTwoDigitFormatter;
}

NumberPicker::NumberPicker()
    : mValue(0)
    , mLongPressUpdateInterval(DEFAULT_LONG_PRESS_UPDATE_INTERVAL)
    , mInitialScrollOffset(Elastos::Core::Math::INT32_MIN_VALUE)
    , mScrollState(0)
{
    mSelectorIndices = ArrayOf<Int32>::Alloc(SELECTOR_WHEEL_ITEM_COUNT);
}

NumberPicker::NumberPicker(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
    : LinearLayout(context, attrs, defStyleAttr)
    , mValue(0)
    , mLongPressUpdateInterval(DEFAULT_LONG_PRESS_UPDATE_INTERVAL)
    , mInitialScrollOffset(Elastos::Core::Math::INT32_MIN_VALUE)
    , mScrollState(0)
{
    mSelectorIndices = ArrayOf<Int32>::Alloc(SELECTOR_WHEEL_ITEM_COUNT);
    Init(context, attrs, defStyleAttr);
}

ECode NumberPicker::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    LinearLayout::Init(context, attrs, defStyleAttr);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::NumberPicker),
            ARRAY_SIZE(R::styleable::NumberPicker));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, defStyleAttr, 0, (ITypedArray**)&a));

    Int32 layoutResID = 0;
    a->GetResourceId(R::styleable::NumberPicker_internalLayout, DEFAULT_LAYOUT_RESOURCE_ID, &layoutResID);

    mHasSelectorWheel = (layoutResID != DEFAULT_LAYOUT_RESOURCE_ID);

    a->GetColor(R::styleable::NumberPicker_solidColor, 0, &mSolidColor);

    a->GetDrawable(R::styleable::NumberPicker_selectionDivider, (IDrawable**)&mSelectionDivider);

    AutoPtr<IDisplayMetrics> display;
    GetResources()->GetDisplayMetrics((IDisplayMetrics**)&display);
    Int32 defSelectionDividerHeight = (Int32)CTypedValue::ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP, UNSCALED_DEFAULT_SELECTION_DIVIDER_HEIGHT, display);
    a->GetDimensionPixelSize(R::styleable::NumberPicker_selectionDividerHeight, defSelectionDividerHeight, &mSelectionDividerHeight);
    Int32 defSelectionDividerDistance = (Int32)CTypedValue::ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP, UNSCALED_DEFAULT_SELECTION_DIVIDERS_DISTANCE, display);
    a->GetDimensionPixelSize(R::styleable::NumberPicker_selectionDividersDistance, defSelectionDividerDistance, &mSelectionDividersDistance);

    a->GetDimensionPixelSize(R::styleable::NumberPicker_internalMinHeight, SIZE_UNSPECIFIED, &mMinHeight);
    a->GetDimensionPixelSize(R::styleable::NumberPicker_internalMaxHeight, SIZE_UNSPECIFIED, &mMaxHeight);

    if(mMinHeight != SIZE_UNSPECIFIED && mMaxHeight != SIZE_UNSPECIFIED && mMinHeight > mMaxHeight) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    a->GetDimensionPixelSize(R::styleable::NumberPicker_internalMinWidth, SIZE_UNSPECIFIED, &mMinWidth);
    a->GetDimensionPixelSize(R::styleable::NumberPicker_internalMaxWidth, SIZE_UNSPECIFIED, &mMaxWidth);

    if(mMinWidth != SIZE_UNSPECIFIED && mMaxWidth != SIZE_UNSPECIFIED && mMinWidth > mMaxWidth) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mComputeMaxWidth = (mMaxWidth == SIZE_UNSPECIFIED);

    a->GetDrawable(R::styleable::NumberPicker_virtualButtonPressedDrawable, (IDrawable**)&mVirtualButtonPressedDrawable);

    a->Recycle();

    mPressedStateHelper = new PressedStateHelper(this);

    SetWillNotDraw(!mHasSelectorWheel);

    AutoPtr<IInterface> i;
    GetContext()->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&i);
    AutoPtr<ILayoutInflater> inflater = ILayoutInflater::Probe(i);
    AutoPtr<IView> v;
    inflater->Inflate(layoutResID, (IViewGroup*)this->Probe(EIID_IViewGroup), TRUE, (IView**)&v);

    if(!mHasSelectorWheel) {
        mInCrementButton = (IImageButton*)IImageButton::Probe(FindViewById(R::id::increment));
        mNumberPickerOnCliskListener = new NumberPickerOnCliskListener(this);
        mInCrementButton->SetOnClickListener(mNumberPickerOnCliskListener);
        mNumberPickerOnLongCliskListener = new NumberPickerOnLongCliskListener(this);
        mInCrementButton->SetOnLongClickListener(mNumberPickerOnLongCliskListener);
    } else {
        mInCrementButton = NULL;
    }

    if(!mHasSelectorWheel) {
        mDecrementButton = (IImageButton*)IImageButton::Probe(FindViewById(R::id::decrement));
        mNumberPickerOnCliskListener = new NumberPickerOnCliskListener(this);
        mDecrementButton->SetOnClickListener(mNumberPickerOnCliskListener);
        mNumberPickerOnLongCliskListener = new NumberPickerOnLongCliskListener(this);
        mDecrementButton->SetOnLongClickListener(mNumberPickerOnLongCliskListener);
    } else {
        mDecrementButton = NULL;
    }

    mInputText = (IEditText*)IEditText::Probe(FindViewById(R::id::numberpicker_input));
    AutoPtr<IViewOnFocusChangeListener> listener = new NumberPickerOnFocusChangeListener(this);
    mInputText->SetOnFocusChangeListener(listener);

    AutoPtr<ArrayOf<IInputFilter*> > array = ArrayOf<IInputFilter*>::Alloc(1);
    AutoPtr<IInputFilter> inputFilter = new InputTextFilter(this);
    array->Set(0, inputFilter);
    mInputText->SetFilters(array);

    mInputText->SetRawInputType(IInputType::TYPE_CLASS_NUMBER);
    mInputText->SetImeOptions(IEditorInfo::IME_ACTION_DONE);

    AutoPtr<IViewConfiguration> configuration = CViewConfiguration::Get(context);
    configuration->GetScaledTouchSlop(&mTouchSlop);
    configuration->GetScaledMinimumFlingVelocity(&mMinimumFlingVelocity);
    configuration->GetScaledMaximumFlingVelocity(&mMaximumFlingVelocity);
    mMaximumFlingVelocity = mMaximumFlingVelocity / SELECTOR_MAX_FLING_VELOCITY_ADJUSTMENT;

    Float size = 0;
    mInputText->GetTextSize(&size);
    mTextSize = (Int32)size;
    AutoPtr<IPaint> paint;
    FAIL_RETURN(CPaint::New((IPaint**)&paint));
    paint->SetAntiAlias(TRUE);
    paint->SetTextAlign(Elastos::Droid::Graphics::PaintAlign_CENTER);
    paint->SetTextSize(mTextSize);
    AutoPtr<ITypeface> face;
    mInputText->GetTypeface((ITypeface**)&face);
    paint->SetTypeface(face);

    AutoPtr<IColorStateList> colors;
    mInputText->GetTextColors((IColorStateList**)&colors);
    Int32 color = 0;
    colors->GetColorForState(ENABLED_STATE_SET.Get(), IColor::WHITE, &color);
    paint->SetColor(color);
    mSelectorWheelPaint = paint;

    CScroller::New(GetContext(), NULL, TRUE, (IScroller**)&mFlingScroller);
    CScroller::New(GetContext(), (IScroller**)&mAdjustScroller);


    UpdateInputTextView();
    if(GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }

    return NOERROR;
}

Boolean NumberPicker::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if(!mHasSelectorWheel || !IsEnabled()) {
        return FALSE;
    }
    Int32 action = 0;
    event->GetActionMasked(&action);
    switch (action) {
        case IMotionEvent::ACTION_DOWN:
        {
            RemoveAllCallbacks();
            mInputText->SetVisibility(IView::INVISIBLE);
            event->GetY(&mLastDownEventY);
            event->GetY(&mLastDownOrMoveEventY);
            event->GetEventTime(&mLastDownEventTime);
            mIngonreMoveEvents = FALSE;
            mShowSoftInputOnTap = FALSE;

            if(mLastDownEventY < mTopSelectionDividerTop) {
                if(mScrollState == INumberPickerOnScrollListener::SCROLL_STATE_IDLE) {
                    mPressedStateHelper->ButtonPressDelayed(PressedStateHelper::BUTTON_DECREMENT);
                }
            } else if (mLastDownEventY > mBottomSelectionDividerBottom) {
                if (mScrollState == INumberPickerOnScrollListener::SCROLL_STATE_IDLE) {
                    mPressedStateHelper->ButtonPressDelayed(PressedStateHelper::BUTTON_INCREMENT);
                }
            }
            GetParent()->RequestDisallowInterceptTouchEvent(TRUE);
            Boolean isFlingFinished = FALSE;
            mFlingScroller->IsFinished(&isFlingFinished);
            Boolean isAdjustFinished = FALSE;
            mAdjustScroller->IsFinished(&isAdjustFinished);
            if(!isFlingFinished) {
                mFlingScroller->ForceFinished(TRUE);
                mAdjustScroller->ForceFinished(TRUE);
                OnScrollStateChange(INumberPickerOnScrollListener::SCROLL_STATE_IDLE);
            } else if (!isAdjustFinished) {
                mFlingScroller->ForceFinished(TRUE);
                mAdjustScroller->ForceFinished(TRUE);
            } else if (mLastDownEventY < mTopSelectionDividerTop) {
                HideSoftInput();
                PostChangeCurrentByOneFromLongPress(FALSE, CViewConfiguration::GetLongPressTimeout());
            } else if (mLastDownEventY > mBottomSelectionDividerBottom) {
                HideSoftInput();
                PostChangeCurrentByOneFromLongPress(TRUE, CViewConfiguration::GetLongPressTimeout());
            } else {
                mShowSoftInputOnTap = TRUE;
                PostBeginSoftInputOnLongPressCommand();
            }
            return TRUE;
        }
    }
    return FALSE;
}

Boolean NumberPicker::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if(!IsEnabled() || !mHasSelectorWheel) {
        return FALSE;
    }
    if(mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
    mVelocityTracker->AddMovement(event);
    Int32 action = 0;
    event->GetActionMasked(&action);
    switch (action) {
        case IMotionEvent::ACTION_MOVE:
        {
            if(mIngonreMoveEvents) {
                break;
            }
            Float currentMoveY = 0;
            event->GetY(&currentMoveY);
            if(mScrollState != INumberPickerOnScrollListener::SCROLL_STATE_TOUCH_SCROLL) {
                Int32 deltaDownY = (Int32)Elastos::Core::Math::Abs(currentMoveY - mLastDownEventY);
                if(deltaDownY > mTouchSlop) {
                    RemoveAllCallbacks();
                    OnScrollStateChange(INumberPickerOnScrollListener::SCROLL_STATE_TOUCH_SCROLL);
                }
            } else {
                Int32 deltaMoveY = (Int32)(currentMoveY - mLastDownOrMoveEventY);
                ScrollBy(0, deltaMoveY);
                Invalidate();
            }
            mLastDownOrMoveEventY = currentMoveY;
        }
        break;

        case IMotionEvent::ACTION_UP:
        {
            RemoveBeginSoftInputCommand();
            RemoveChangeCurrentByOneFromLongPress();
            mPressedStateHelper->Cancel();
            AutoPtr<VelocityTracker> velocityTracker = mVelocityTracker;
            velocityTracker->ComputeCurrentVelocity(1000, (Float)mMaximumFlingVelocity);
            Float y;
            velocityTracker->GetYVelocity(&y);
            Int32 initialVelocity = (Int32)y;
            if(Elastos::Core::Math::Abs(initialVelocity) > mMinimumFlingVelocity) {
                Fling(initialVelocity);
                OnScrollStateChange(INumberPickerOnScrollListener::SCROLL_STATE_FLING);
            } else {
                Float fy = 0;
                event->GetY(&fy);
                Int32 eventY = (Int32)fy;
                Int32 deltaMoveY = (Int32)(Elastos::Core::Math::Abs(eventY - mLastDownEventY));
                Int64 deltaTime = 0;
                event->GetEventTime(&deltaTime);
                deltaTime -= mLastDownEventTime;
                if(deltaMoveY <= mTouchSlop && deltaTime < CViewConfiguration::GetTapTimeout()) {
                    if(mShowSoftInputOnTap) {
                        mShowSoftInputOnTap = FALSE;
                        ShowSoftInput();
                    } else {
                        Int32 selectorIndexOffset = (eventY / mSelectorElementHeight) - SELECTOR_MIDDLE_ITEM_INDEX;
                        if(selectorIndexOffset > 0) {
                            ChangeValueByOne(TRUE);
                            mPressedStateHelper->ButtonTapped(PressedStateHelper::BUTTON_INCREMENT);
                        } else if(selectorIndexOffset < 0) {
                            ChangeValueByOne(FALSE);
                            mPressedStateHelper->ButtonTapped(PressedStateHelper::BUTTON_DECREMENT);
                        }
                    }
                } else {
                    EnsureScrollWheelAdjusted();
                }
                OnScrollStateChange(INumberPickerOnScrollListener::SCROLL_STATE_IDLE);
            }
            mVelocityTracker->Recycle();
            mVelocityTracker = NULL;
        }
        break;
    }
    return TRUE;
}

Boolean NumberPicker::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action = 0;
    event->GetActionMasked(&action);
    switch (action) {
        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP:
            RemoveAllCallbacks();
            break;
    }
    return LinearLayout::DispatchTouchEvent(event);
}

Boolean NumberPicker::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode = 0;
    event->GetKeyCode(&keyCode);
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_CENTER:
        case IKeyEvent::KEYCODE_ENTER:
            RemoveAllCallbacks();
            break;
    }
    return LinearLayout::DispatchKeyEvent(event);
}

Boolean NumberPicker::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action = 0;
    event->GetActionMasked(&action);
    switch (action) {
        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP:
            RemoveAllCallbacks();
            break;
    }
    return LinearLayout::DispatchTrackballEvent(event);;
}

Boolean NumberPicker::DispatchHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    if(!mHasSelectorWheel) {
        return LinearLayout::DispatchHoverEvent(event);
    }
    AutoPtr<IAccessibilityManager> am;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&am);
    Boolean isEnabled = FALSE;
    am->IsEnabled(&isEnabled);
    if(isEnabled) {
        Float fy = 0;
        event->GetY(&fy);
        Int32 eventY = (Int32)fy;
        Int32 hoveredVirtualViewId = 0;
        if (eventY < mTopSelectionDividerTop) {
            hoveredVirtualViewId = AccessibilityNodeProviderImpl::VIRTUAL_VIEW_ID_DECREMENT;
        } else if (eventY > mBottomSelectionDividerBottom) {
            hoveredVirtualViewId = AccessibilityNodeProviderImpl::VIRTUAL_VIEW_ID_INCREMENT;
        } else {
            hoveredVirtualViewId = AccessibilityNodeProviderImpl::VIRTUAL_VIEW_ID_INPUT;
        }

        Int32 action = 0;
        event->GetActionMasked(&action);
        AutoPtr<IAccessibilityNodeProvider> provider = GetAccessibilityNodeProvider();
        AutoPtr<AccessibilityNodeProviderImpl> providerImpl = reinterpret_cast<AccessibilityNodeProviderImpl*>(provider->Probe(EIID_AccessibilityNodeProviderImpl));
        Boolean res = FALSE;
        switch (action) {
            case IMotionEvent::ACTION_HOVER_ENTER:
                providerImpl->SendAccessibilityEventForVirtualView(hoveredVirtualViewId, IAccessibilityEvent::TYPE_VIEW_HOVER_ENTER);
                mLastHoveredChildVirtualViewId = hoveredVirtualViewId;
                providerImpl->PerformAction(hoveredVirtualViewId, IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS, NULL, &res);
                break;
            case IMotionEvent::ACTION_HOVER_MOVE:
                if(mLastHoveredChildVirtualViewId != hoveredVirtualViewId &&
                    mLastHoveredChildVirtualViewId != IView::NO_ID) {
                    providerImpl->SendAccessibilityEventForVirtualView(mLastHoveredChildVirtualViewId, IAccessibilityEvent::TYPE_VIEW_HOVER_EXIT);
                    providerImpl->SendAccessibilityEventForVirtualView(hoveredVirtualViewId, IAccessibilityEvent::TYPE_VIEW_HOVER_ENTER);
                    mLastHoveredChildVirtualViewId = hoveredVirtualViewId;
                    providerImpl->PerformAction(hoveredVirtualViewId, IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS, NULL, &res);
                }
                break;
            case IMotionEvent::ACTION_HOVER_EXIT:
                providerImpl->SendAccessibilityEventForVirtualView(hoveredVirtualViewId, IAccessibilityEvent::TYPE_VIEW_HOVER_EXIT);
                mLastHoveredChildVirtualViewId = IView::NO_ID;
                break;
        }
    }
    return FALSE;
}

ECode NumberPicker::ComputeScroll()
{
    AutoPtr<IScroller> scroller = mFlingScroller;
    Boolean flingFinished = FALSE;
    scroller->IsFinished(&flingFinished);
    if(flingFinished) {
        scroller = mAdjustScroller;
        Boolean adjustFinished = FALSE;
        scroller->IsFinished(&adjustFinished);
        if(adjustFinished) {
            return NOERROR;
        }
    }
    Boolean res = FALSE;
    scroller->ComputeScrollOffset(&res);
    Int32 currentScrollerY = 0;
    scroller->GetCurrY(&currentScrollerY);
    if(mPreviousScrollerY == 0) {
        scroller->GetStartY(&mPreviousScrollerY);
    }
    ScrollBy(0, currentScrollerY - mPreviousScrollerY);
    mPreviousScrollerY = currentScrollerY;
    Boolean isFinished = FALSE;
    scroller->IsFinished(&isFinished);
    if(isFinished) {
        OnScrollerFinished(scroller);
    } else {
        Invalidate();
    }

    return NOERROR;
}

ECode NumberPicker::SetEnabled(
    /* [in] */ Boolean enabled)
{
    LinearLayout::SetEnabled(enabled);
    if(!mHasSelectorWheel) {
        mInCrementButton->SetEnabled(enabled);
    }
    if(!mHasSelectorWheel) {
        mDecrementButton->SetEnabled(enabled);
    }
    mInputText->SetEnabled(enabled);
    return NOERROR;
}

ECode NumberPicker::ScrollBy(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    AutoPtr<ArrayOf<Int32> > selectorIndices = mSelectorIndices;
    if (!mWrapSelectorWheel && y > 0
        && (*selectorIndices)[SELECTOR_MIDDLE_ITEM_INDEX] <= mMinValue) {
        mCurrentScrollOffset = mInitialScrollOffset;
        return NOERROR;
    }

    if (!mWrapSelectorWheel && y < 0
        && (*selectorIndices)[SELECTOR_MIDDLE_ITEM_INDEX] >= mMaxValue) {
        mCurrentScrollOffset = mInitialScrollOffset;
        return NOERROR;
    }

    mCurrentScrollOffset += y;
    while (mCurrentScrollOffset - mInitialScrollOffset > mSelectorTextGapHeight) {
        mCurrentScrollOffset -= mSelectorElementHeight;
        DecrementSelectorIndices(selectorIndices);
        SetValueInternal((*selectorIndices)[SELECTOR_MIDDLE_ITEM_INDEX], TRUE);
        if (!mWrapSelectorWheel && (*selectorIndices)[SELECTOR_MIDDLE_ITEM_INDEX] <= mMinValue) {
            mCurrentScrollOffset = mInitialScrollOffset;
        }
    }

    while (mCurrentScrollOffset - mInitialScrollOffset < -mSelectorTextGapHeight) {
        mCurrentScrollOffset += mSelectorElementHeight;
        IncrementSelectorIndices(selectorIndices);
        SetValueInternal((*selectorIndices)[SELECTOR_MIDDLE_ITEM_INDEX], TRUE);
        if (!mWrapSelectorWheel && (*selectorIndices)[SELECTOR_MIDDLE_ITEM_INDEX] >= mMaxValue) {
            mCurrentScrollOffset = mInitialScrollOffset;
        }
    }
    return NOERROR;
}

Int32 NumberPicker::GetSolidColor()
{
    return mSolidColor;
}

ECode NumberPicker::SetOnValueChangedListener(
    /* [in] */ INumberPickerOnValueChangeListener* onValueChangedListener)
{
    mOnValueChangeListener = onValueChangedListener;
    return NOERROR;
}

ECode NumberPicker::SetOnScrollListener(
    /* [in] */ INumberPickerOnScrollListener* onScrollListener)
{
    mOnScrollListener = onScrollListener;
    return NOERROR;
}

ECode NumberPicker::SetFormatter(
    /* [in] */ INumberPickerFormatter* formatter)
{
    if(formatter == mFormatter) {
        return NOERROR;
    }
    mFormatter = formatter;
    InitializeSelectorWheelIndices();
    UpdateInputTextView();
    return NOERROR;
}

ECode NumberPicker::SetValue(
    /* [in] */ Int32 value)
{
    SetValueInternal(value, FALSE);
    return NOERROR;
}

Boolean NumberPicker::GetWrapSelectorWheel()
{
    return mWrapSelectorWheel;
}

ECode NumberPicker::SetWrapSelectorWheel(
    /* [in] */ Boolean wrapSelectorWheel)
{
    Boolean wrappingAllowed = (mMaxValue - mMinValue) >= (*mSelectorIndices).GetLength();
    if((!wrapSelectorWheel || wrappingAllowed) && wrapSelectorWheel != mWrapSelectorWheel) {
        mWrapSelectorWheel = wrapSelectorWheel;
    }
    return NOERROR;
}

ECode NumberPicker::SetOnLongPressUpdateInterval(
    /* [in] */ Int64 intervalMillis)
{
    mLongPressUpdateInterval = intervalMillis;
    return NOERROR;
}

Int32 NumberPicker::GetValue()
{
    return mValue;
}

Int32 NumberPicker::GetMinValue()
{
    return mMinValue;
}

ECode NumberPicker::SetMinValue(
    /* [in] */ Int32 minValue)
{
    if(mMinValue == minValue) {
        return NOERROR;
    }
    if(minValue < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mMinValue = minValue;
    if(mMinValue > mValue) {
        mValue = mMinValue;
    }

    Boolean wrapSelectorWheel = mMaxValue - mMinValue > (*mSelectorIndices).GetLength();
    SetWrapSelectorWheel(wrapSelectorWheel);
    InitializeSelectorWheelIndices();
    UpdateInputTextView();
    TryComputeMaxWidth();
    Invalidate();
    return NOERROR;
}

Int32 NumberPicker::GetMaxValue()
{
    return mMaxValue;
}

ECode NumberPicker::SetMaxValue(
    /* [in] */ Int32 maxValue)
{
    if(mMaxValue == maxValue) {
        return NOERROR;
    }
    if(maxValue < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mMaxValue = maxValue;
    if(mMaxValue < mValue) {
        mValue = mMaxValue;
    }

    Boolean wrapSelectorWheel = mMaxValue - mMinValue > (*mSelectorIndices).GetLength();
    SetWrapSelectorWheel(wrapSelectorWheel);
    InitializeSelectorWheelIndices();
    UpdateInputTextView();
    TryComputeMaxWidth();
    Invalidate();
    return NOERROR;
}

AutoPtr<ArrayOf<String> > NumberPicker::GetDisplayedValues()
{
    return mDisplayedValues;
}

ECode NumberPicker::SetDisplayedValues(
    /* [in] */ ArrayOf<String>* displayedValues)
{
    if(mDisplayedValues.Get() == displayedValues) {
        return NOERROR;
    }
    mDisplayedValues = displayedValues;
    if(mDisplayedValues) {
        mInputText->SetRawInputType(IInputType::TYPE_CLASS_TEXT
            | IInputType::TYPE_TEXT_FLAG_NO_SUGGESTIONS);
    } else {
        mInputText->SetRawInputType(IInputType::TYPE_CLASS_NUMBER);
    }
    UpdateInputTextView();
    InitializeSelectorWheelIndices();
    TryComputeMaxWidth();
    return NOERROR;
}

ECode NumberPicker::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(NUMBERPICkER_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    event->SetScrollable(TRUE);
    event->SetScrollY((mMinValue + mValue) * mSelectorElementHeight);
    event->SetMaxScrollY((mMaxValue - mMinValue) * mSelectorElementHeight);
    return NOERROR;
}

AutoPtr<IAccessibilityNodeProvider> NumberPicker::GetAccessibilityNodeProvider()
{
    if(!mHasSelectorWheel) {
        return LinearLayout::GetAccessibilityNodeProvider();
    }
    if(mAccessibilityNodeProvider == NULL) {
        mAccessibilityNodeProvider = new AccessibilityNodeProviderImpl(this);
    }
    return mAccessibilityNodeProvider;
}

void NumberPicker::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 letf,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if(!mHasSelectorWheel) {
        LinearLayout::OnLayout(changed, letf, top, right, bottom);
        return;
    }

    Int32 msrdWdth = GetMeasuredWidth();
    Int32 msrdHght = GetMeasuredHeight();

    Int32 inptTxtMsrdWdth = 0;
    mInputText->GetMeasuredWidth(&inptTxtMsrdWdth);
    Int32 inptTxtMsrdHght = 0;
    mInputText->GetMeasuredHeight(&inptTxtMsrdHght);

    Int32 inptTxtLeft = (msrdWdth - inptTxtMsrdWdth) / 2;
    Int32 inptTxtTop = (msrdHght - inptTxtMsrdHght) / 2;
    Int32 inptTxtRight = inptTxtLeft + inptTxtMsrdWdth;
    Int32 inptTxtBottom = inptTxtTop + inptTxtMsrdHght;

    mInputText->Layout(inptTxtLeft, inptTxtTop, inptTxtRight, inptTxtBottom);

    if(changed) {
        InitializeSelectorWheel();
        InitializeFadingEdges();
        mTopSelectionDividerTop = (GetHeight() - mSelectionDividersDistance) / 2 - mSelectionDividerHeight;

        mBottomSelectionDividerBottom = mTopSelectionDividerTop + 2 * mSelectionDividerHeight + mSelectionDividersDistance;
    }
}

void NumberPicker::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if(!mHasSelectorWheel) {
        LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
        return;
    }
    Int32 newWihthMeasureSpec = MakeMeasureSpec(widthMeasureSpec, mMaxWidth);
    Int32 newHeightMeasureSpec = MakeMeasureSpec(heightMeasureSpec, mMaxHeight);
    LinearLayout::OnMeasure(newWihthMeasureSpec, newHeightMeasureSpec);
    Int32 widthSize = ResolveSizeAndStateRespectingMinSize(mMinWidth, GetMeasuredWidth(), widthMeasureSpec);
    Int32 heightSize = ResolveSizeAndStateRespectingMinSize(mMinHeight, GetMeasuredHeight(), heightMeasureSpec);
    SetMeasuredDimension(widthSize, heightSize);
}

Float NumberPicker::GetTopFadingEdgeStrength()
{
    return TOP_AND_BOTTOM_FADING_EDGE_STRENGTH;
}

Float NumberPicker::GetBottomFadingEdgeStrength()
{
    return TOP_AND_BOTTOM_FADING_EDGE_STRENGTH;
}

ECode NumberPicker::OnDetachedFromWindow()
{
    RemoveAllCallbacks();
    return NOERROR;
}

void NumberPicker::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    if(!mHasSelectorWheel) {
        LinearLayout::OnDraw(canvas);
        return;
    }
    Float x = (mRight - mLeft) / 2;
    Float y = mCurrentScrollOffset;
    if(mVirtualButtonPressedDrawable && mScrollState == INumberPickerOnScrollListener::SCROLL_STATE_IDLE) {
        Boolean res = FALSE;
        if(mDecrementVirtualButtonPressed) {
            mVirtualButtonPressedDrawable->SetState(PRESSED_STATE_SET, &res);
            mVirtualButtonPressedDrawable->SetBounds(0, 0, mRight, mTopSelectionDividerTop);
            mVirtualButtonPressedDrawable->Draw(canvas);
        }
        if(mIncrementVirtualButtonPressed) {
            mVirtualButtonPressedDrawable->SetState(PRESSED_STATE_SET, &res);
            mVirtualButtonPressedDrawable->SetBounds(0, mBottomSelectionDividerBottom, mRight, mBottom);
            mVirtualButtonPressedDrawable->Draw(canvas);
        }
    }

    AutoPtr<ArrayOf<Int32> > selectorIndices = mSelectorIndices;
    for(Int32 i = 0; i < selectorIndices->GetLength(); i++) {
        Int32 selectorIndex = (*selectorIndices)[i];
        String scrollSelectorValue = mSelectorIndexToStringCache[selectorIndex];
        Int32 visible = 0;
        mInputText->GetVisibility(&visible);
        if(i != SELECTOR_MIDDLE_ITEM_INDEX || visible != IView::VISIBLE) {
            canvas->DrawText(scrollSelectorValue, x, y, mSelectorWheelPaint);
        }
        y += mSelectorElementHeight;
    }
    if(mSelectionDivider) {
        Int32 topOfTopDivider = mTopSelectionDividerTop;
        Int32 bottomOfTopDivider = topOfTopDivider + mSelectionDividerHeight;
        mSelectionDivider->SetBounds(0, topOfTopDivider, mRight, bottomOfTopDivider);
        mSelectionDivider->Draw(canvas);

        Int32 bottomOfBottomDivider = mBottomSelectionDividerBottom;
        Int32 topOfBottomDivider = bottomOfBottomDivider - mSelectionDividerHeight;
        mSelectionDivider->SetBounds(0, topOfBottomDivider, mRight, bottomOfBottomDivider);
        mSelectionDivider->Draw(canvas);
    }
}

Boolean NumberPicker::MoveToFinalScrollerPosition(
    /* [in] */ IScroller* scroller)
{
    scroller->ForceFinished(TRUE);
    Int32 finalY = 0;
    scroller->GetFinalY(&finalY);
    Int32 currY = 0;
    scroller->GetCurrY(&currY);

    Int32 amountToScroll = finalY - currY;
    Int32 futureScrollOffset = (mCurrentScrollOffset + amountToScroll) % mSelectorElementHeight;
    Int32 overshootAdjustment = mInitialScrollOffset - futureScrollOffset;

    if(overshootAdjustment != 0) {
        if(Elastos::Core::Math::Abs(overshootAdjustment) > mSelectorElementHeight / 2) {
            if(overshootAdjustment > 0) {
                overshootAdjustment -= mSelectorElementHeight;
            } else {
                overshootAdjustment += mSelectorElementHeight;
            }
        }
        amountToScroll += overshootAdjustment;
        ScrollBy(0, amountToScroll);
        return TRUE;
    }
    return FALSE;
}

void NumberPicker::ShowSoftInput()
{
    AutoPtr<IInputMethodManager> inputMethodManager = CInputMethodManager::PeekInstance();
    if(inputMethodManager != NULL) {
        if(mHasSelectorWheel) {
            mInputText->SetVisibility(IView::VISIBLE);
        }
        Boolean res = FALSE;
        mInputText->RequestFocus(&res);
        inputMethodManager->ShowSoftInput(IView::Probe(mInputText), 0, &res);
    }
}

void NumberPicker::HideSoftInput()
{
    AutoPtr<IInputMethodManager> inputMethodManager = CInputMethodManager::PeekInstance();
    if (inputMethodManager == NULL) {
        return;
    }
    Boolean isActive = FALSE;
    inputMethodManager->IsActive(IView::Probe(mInputText), &isActive);
    if(inputMethodManager && isActive) {
        Boolean res = FALSE;
        inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        if(mHasSelectorWheel) {
            mInputText->SetVisibility(IView::INVISIBLE);
        }
    }
}

void NumberPicker::TryComputeMaxWidth()
{
    if(!mComputeMaxWidth) {
        return;
    }
    Int32 maxTextWidth = 0;
   if(mDisplayedValues == NULL) {
        Float maxDigitWidth = 0;
        for (Int32 i = 0; i <= 9; i++) {
            Float digitWidth = 0;
            mSelectorWheelPaint->MeasureText(mAccessibilityNodeProvider->FormatNumberWithLocale(i), &digitWidth);
            if(digitWidth > maxDigitWidth) {
                maxDigitWidth = digitWidth;
            }
        }
        Int32 numberOfDigits = 0;
        Int32 current = mMaxValue;
        while(current > 0) {
            numberOfDigits++;
            current /= 10;
        }
        maxTextWidth = (Int32)(numberOfDigits * maxDigitWidth);
    } else {
        Int32 valueCount = mDisplayedValues->GetLength();
        for(Int32 i = 0; i < valueCount; i++) {
            Float textWidth = 0;
            mSelectorWheelPaint->MeasureText((*mDisplayedValues)[i], &textWidth);
            if(textWidth > maxTextWidth) {
                maxTextWidth = (Int32)textWidth;
            }
        }
    }
    Int32 paddingLeft = 0;
    mInputText->GetPaddingLeft(&paddingLeft);
    Int32 paddingRight = 0;
    mInputText->GetPaddingRight(&paddingRight);
    maxTextWidth += paddingLeft + paddingRight;
    if(mMaxWidth != maxTextWidth) {
        if(maxTextWidth > mMinWidth){
            mMaxWidth = maxTextWidth;
        } else {
            mMaxWidth = mMinWidth;
     }
    Invalidate();
    }
}

Int32 NumberPicker::MakeMeasureSpec(
    /* [in] */ Int32 measureSpec,
    /* [in] */ Int32 maxSize)
{
    if (maxSize == SIZE_UNSPECIFIED) {
        return measureSpec;
    }
    Int32 size = View::MeasureSpec::GetSize(measureSpec);
    Int32 mode = View::MeasureSpec::GetMode(measureSpec);
    switch (mode) {
        case View::MeasureSpec::EXACTLY:
            return measureSpec;
        case View::MeasureSpec::AT_MOST:
            return View::MeasureSpec::MakeMeasureSpec(Elastos::Core::Math::Min(size, maxSize), View::MeasureSpec::EXACTLY);
        case View::MeasureSpec::UNSPECIFIED:
            return View::MeasureSpec::MakeMeasureSpec(maxSize, View::MeasureSpec::EXACTLY);
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

Int32 NumberPicker::ResolveSizeAndStateRespectingMinSize(
    /* [in] */ Int32 minSize,
    /* [in] */ Int32 measuredSize,
    /* [in] */ Int32 measureSpec)
{
    if(minSize != SIZE_UNSPECIFIED) {
        Int32 desiredWidth = Elastos::Core::Math::Max(minSize, measuredSize);
        return ResolveSizeAndState(desiredWidth, measureSpec, 0);
    } else {
        return measuredSize;
    }
}

void NumberPicker::InitializeSelectorWheelIndices()
{
    mSelectorIndexToStringCache.Clear();
    AutoPtr<ArrayOf<Int32> > selectorIndices = mSelectorIndices;
    Int32 current = GetValue();
    for(Int32 i = 0; i < (*mSelectorIndices).GetLength(); i++) {
        Int32 selectorIndex = current + (i - SELECTOR_MIDDLE_ITEM_INDEX);
        if(mWrapSelectorWheel) {
            selectorIndex = GetWrappedSelectorIndex(selectorIndex);
        }
        (*selectorIndices)[i] = selectorIndex;
        EnsureCachedScrollSelectorValue((*selectorIndices)[i]);
    }
}

void NumberPicker::SetValueInternal(
    /* [in] */ Int32 current,
    /* [in] */ Boolean notifyChange)
{
    if (mValue == current) {
        return;
    }
    if (mWrapSelectorWheel) {
        current = GetWrappedSelectorIndex(current);
    } else {
        current = Elastos::Core::Math::Max(current, mMinValue);
        current = Elastos::Core::Math::Min(current, mMaxValue);
    }
    Int32 previous = mValue;
    mValue = current;
    UpdateInputTextView();
    if (notifyChange) {
        NotifyChange(previous, current);
    }
    InitializeSelectorWheelIndices();
    Invalidate();
}

void NumberPicker::ChangeValueByOne(
    /* [in] */ Boolean increment)
{
    if(mHasSelectorWheel) {
        mInputText->SetVisibility(IView::INVISIBLE);
        if(!MoveToFinalScrollerPosition(mFlingScroller)) {
            MoveToFinalScrollerPosition(mAdjustScroller);
        }
        mPreviousScrollerY = 0;
        if(increment) {
            mFlingScroller->StartScroll(0, 0, 0, -mSelectorElementHeight, SNAP_SCROLL_DURATION);
        } else {
            mFlingScroller->StartScroll(0, 0, 0, mSelectorElementHeight, SNAP_SCROLL_DURATION);
        }
        Invalidate();
    } else {
        if(increment) {
            SetValueInternal(mValue + 1, TRUE);
        } else {
            SetValueInternal(mValue - 1, TRUE);
        }
    }
}
///////////////////////////
void NumberPicker::InitializeSelectorWheel()
{
    InitializeSelectorWheelIndices();
    AutoPtr<ArrayOf<Int32> > selectorIndices = mSelectorIndices;
    Int32 totalTextHeight = (selectorIndices->GetLength()) * mTextSize;
    Float totalTextGapHeight = (mBottom - mTop) - totalTextHeight;
    Float TextGapCount = (*selectorIndices).GetLength();
    mSelectorTextGapHeight = (Int32)(totalTextGapHeight / TextGapCount + 0.5f);
    mSelectorElementHeight = mTextSize + mSelectorTextGapHeight;
    Int32 baseLine = 0;
    mInputText->GetBaseline(&baseLine);
    Int32 top = 0;
    mInputText->GetTop(&top);
    Int32 editTextTextPosition = baseLine + top;
    mInitialScrollOffset = editTextTextPosition - (mSelectorElementHeight * SELECTOR_MIDDLE_ITEM_INDEX);
    mCurrentScrollOffset = mInitialScrollOffset;
    UpdateInputTextView();
}

void NumberPicker::InitializeFadingEdges()
{
    SetVerticalFadingEdgeEnabled(TRUE);
    SetFadingEdgeLength((mBottom - mTop - mTextSize) / 2);
}

void NumberPicker::OnScrollerFinished(
    /* [in] */ IScroller* scroller)
{
    if (scroller == mFlingScroller) {
        if (!EnsureScrollWheelAdjusted()) {
            UpdateInputTextView();
        }
        OnScrollStateChange(INumberPickerOnScrollListener::SCROLL_STATE_IDLE);
    } else {
        if (mScrollState != INumberPickerOnScrollListener::SCROLL_STATE_TOUCH_SCROLL) {
            UpdateInputTextView();
        }
    }
}

void NumberPicker::OnScrollStateChange(
    /* [in] */ Int32 scrollState)
{
    if(mScrollState == scrollState) {
        return;
    }
    mScrollState = scrollState;
    if(mOnScrollListener) {
        mOnScrollListener->OnScrollStateChange((INumberPicker*)this->Probe(EIID_INumberPicker), scrollState);
    }
}

void NumberPicker::Fling(
    /* [in] */ Int32 velocityY)
{
    mPreviousScrollerY = 0;
    if(velocityY > 0) {
        mFlingScroller->Fling(0, 0, 0, velocityY, 0, 0, 0, Elastos::Core::Math::INT32_MAX_VALUE);
    } else {
        mFlingScroller->Fling(0, Elastos::Core::Math::INT32_MAX_VALUE, 0, velocityY, 0, 0, 0, Elastos::Core::Math::INT32_MAX_VALUE);
    }
    Invalidate();
}

Int32 NumberPicker::GetWrappedSelectorIndex(
    /* [in] */ Int32 selectorIndex)
{
    if (selectorIndex > mMaxValue) {
        return mMinValue + (selectorIndex - mMaxValue) % (mMaxValue - mMinValue) - 1;
    } else if (selectorIndex < mMinValue) {
        return mMaxValue - (mMinValue - selectorIndex) % (mMaxValue - mMinValue) + 1;
    }
    return selectorIndex;
}

void NumberPicker::IncrementSelectorIndices(
    /* [in] */ ArrayOf<Int32>* selectorIndices)
{
    for (Int32 i = 0; i < selectorIndices->GetLength() - 1; i++) {
        (*selectorIndices)[i] = (*selectorIndices)[i + 1];
    }
    Int32 nextScrollSelectorIndex = (*selectorIndices)[selectorIndices->GetLength() - 2] + 1;
    if(mWrapSelectorWheel && nextScrollSelectorIndex > mMaxValue) {
        nextScrollSelectorIndex = mMinValue;
    }
    (*selectorIndices)[selectorIndices->GetLength() - 1] = nextScrollSelectorIndex;
    EnsureCachedScrollSelectorValue(nextScrollSelectorIndex);
}

void NumberPicker::DecrementSelectorIndices(
    /* [in] */ ArrayOf<Int32>* selectorIndices)
{
    for (Int32 i = selectorIndices->GetLength() - 1; i > 0; i--) {
        (*selectorIndices)[i] = (*selectorIndices)[i - 1];
    }
    Int32 nextScrollSelectorIndex = (*selectorIndices)[1] - 1;
    if(mWrapSelectorWheel && nextScrollSelectorIndex < mMinValue) {
        nextScrollSelectorIndex = mMaxValue;
    }
    (*selectorIndices)[0] = nextScrollSelectorIndex;
    EnsureCachedScrollSelectorValue(nextScrollSelectorIndex);
}

void NumberPicker::EnsureCachedScrollSelectorValue(
    /* [in] */ Int32 selectorIndex)
{
    String scrollSelectorValue = mSelectorIndexToStringCache[selectorIndex];
    if(scrollSelectorValue) {
        return;
    }
    if(selectorIndex < mMinValue || selectorIndex > mMaxValue) {
        scrollSelectorValue = String("");
    } else {
        if(mDisplayedValues) {
            Int32 displayedValuesIndex = selectorIndex - mMinValue;
            scrollSelectorValue = (*mDisplayedValues)[displayedValuesIndex];
        } else {
            scrollSelectorValue = FormatNumber(selectorIndex);
        }
    }
    mSelectorIndexToStringCache[selectorIndex] = scrollSelectorValue;
}

String NumberPicker::FormatNumber(
    /* [in] */ Int32 value)
{
    if(mFormatter) {
        String str = String(NULL);
        mFormatter->Format(value, &str);
        return str;
    } else {
        return mAccessibilityNodeProvider->FormatNumberWithLocale(value);
    }
}

void NumberPicker::ValidateInputTextView(
    /* [in] */ IView* v)
{
    AutoPtr<ICharSequence> charSeq;
    (ITextView::Probe(v))->GetText((ICharSequence**)&charSeq);
    String str = String(NULL);
    charSeq->ToString(&str);
    if(str.IsEmpty()) {
        UpdateInputTextView();
    } else {
        Int32 current = GetSelectedPos(str);
        SetValueInternal(current, TRUE);
    }
}

Boolean NumberPicker::UpdateInputTextView()
{
    String text = String(NULL);
    if(!mDisplayedValues) {
        text = FormatNumber(mValue);
    } else {
        text = (*mDisplayedValues)[mValue - mMinValue];
    }
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(text, (ICharSequence**)&csq);
    AutoPtr<ICharSequence> itext;
    mInputText->GetText((ICharSequence**)&itext);
    String inputText = String(NULL);
    itext->ToString(&inputText);
    if(!TextUtils::IsEmpty(csq) && !text.Equals(inputText)) {
        mInputText->SetText(csq);
        return TRUE;
    }
    return FALSE;
}

void NumberPicker::NotifyChange(
    /* [in] */ Int32 previous,
    /* [in] */ Int32 current)
{
    if(mOnValueChangeListener) {
        mOnValueChangeListener->OnValueChange((INumberPicker*)this->Probe(EIID_INumberPicker), previous, mValue);
    }
}

void NumberPicker::PostChangeCurrentByOneFromLongPress(
    /* [in] */ Boolean increment,
    /* [in] */ Int64 delayMillis)
{
    if(!mChangeCurrentByOneFromLongPressCommand) {
        mChangeCurrentByOneFromLongPressCommand = new ChangeCurrentByOneFromLongPressCommand(this);
    } else {
        RemoveCallbacks(mChangeCurrentByOneFromLongPressCommand);
    }
    mChangeCurrentByOneFromLongPressCommand->SetStep(increment);
    PostDelayed(mChangeCurrentByOneFromLongPressCommand, delayMillis);
}

void NumberPicker::RemoveChangeCurrentByOneFromLongPress()
{
    if(mChangeCurrentByOneFromLongPressCommand) {
        RemoveCallbacks(mChangeCurrentByOneFromLongPressCommand);
    }
}

void NumberPicker::PostBeginSoftInputOnLongPressCommand()
{
    if(!mBeginSoftInputOnLongPressCommand) {
        mBeginSoftInputOnLongPressCommand = new BeginSoftInputOnLongPressCommand(this);
    } else {
        RemoveCallbacks(mBeginSoftInputOnLongPressCommand);
    }
    PostDelayed(mBeginSoftInputOnLongPressCommand, CViewConfiguration::GetLongPressTimeout());
}

void NumberPicker::RemoveBeginSoftInputCommand()
{
    if(mBeginSoftInputOnLongPressCommand) {
        RemoveCallbacks(mBeginSoftInputOnLongPressCommand);
    }
}

void NumberPicker::RemoveAllCallbacks()
{
    if(mChangeCurrentByOneFromLongPressCommand) {
        RemoveCallbacks(mChangeCurrentByOneFromLongPressCommand);
    }
    if(mSetSelectionCommand) {
        RemoveCallbacks(mSetSelectionCommand);
    }
    if(mBeginSoftInputOnLongPressCommand) {
        RemoveCallbacks(mBeginSoftInputOnLongPressCommand);
    }

    mPressedStateHelper->Cancel();
}

Int32 NumberPicker::GetSelectedPos(
    /* [in] */ const String& value)
{
    if(!mDisplayedValues) {
        //try{
            return StringUtils::ParseInt32(value);
        //} catch{

        //}
    } else {
        String str;
        for (Int32 i = 0; i < (*mDisplayedValues).GetLength(); i++) {
            str = (*mDisplayedValues)[i];
            if(str.StartWithIgnoreCase(value)) {
                return mMinValue + i;
            }
        }
        //try{
            return StringUtils::ParseInt32(value);
        //} catch{

        //}
    }
    return mMinValue;
}

void NumberPicker::PostSetSelectionCommand(
    /* [in] */ Int32 selectionStart,
    /* [in] */ Int64 selectionEnd)
{
    if(!mSetSelectionCommand) {
        mSetSelectionCommand = new SetSelectionCommand((INumberPicker*)this->Probe(EIID_INumberPicker));
    } else {
        RemoveCallbacks(mSetSelectionCommand);
    }
    mSetSelectionCommand->mSelectionStart = selectionStart;
    mSetSelectionCommand->mSelectionEnd = selectionEnd;
    Post(mSetSelectionCommand);
}

Boolean NumberPicker::EnsureScrollWheelAdjusted()
{
    Int32 deltaY = mInitialScrollOffset - mCurrentScrollOffset;
    if(deltaY != 0) {
        mPreviousScrollerY = 0;
        if(Elastos::Core::Math::Abs(deltaY) > mSelectorElementHeight / 2) {
            deltaY += (deltaY > 0) ? -mSelectorElementHeight : mSelectorElementHeight;
        }
        mAdjustScroller->StartScroll(0, 0, 0, deltaY, SELECTOR_ADJUSTMENT_DURATION_MILLIS);
        Invalidate();
        return TRUE;
    }
    return FALSE;
}

//==============================================================================================
//          NumberPicker::InputTextFilter
//==============================================================================================

NumberPicker::InputTextFilter::InputTextFilter(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

Int32 NumberPicker::InputTextFilter::GetInputType()
{
    return IInputType::TYPE_CLASS_TEXT;
}

ECode NumberPicker::InputTextFilter::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /* [out] */ ICharSequence** sou)
{
    if(!mHost->mDisplayedValues) {
        AutoPtr<ICharSequence> filtered = NumberKeyListener::Filter(source, start, end, dest, dstart, dend);
        if(!filtered) {
            source->SubSequence(start, end, (ICharSequence**)&filtered);
        }

        AutoPtr<ICharSequence> res;
        dest->SubSequence(0, dstart, (ICharSequence**)&res);
        String result = String(NULL);
        res->ToString(&result);
        String subString = result;
        filtered->ToString(&result);
        subString += result;
        Int32 len = 0;
        dest->GetLength(&len);
        res = NULL;
        dest->SubSequence(dend, len, (ICharSequence**)&res);
        res->ToString(&result);
        subString += result;

        if(subString.Equals("")) {
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(subString, (ICharSequence**)&seq);
            *sou = seq;
            REFCOUNT_ADD(*sou);
            return NOERROR;
        }

        Int32 val = mHost->GetSelectedPos(subString);
        if(val > mHost->mMaxValue) {
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(String(""), (ICharSequence**)&seq);
            *sou = seq;
            REFCOUNT_ADD(*sou);
            return NOERROR;
        } else {
            *sou = filtered;
            REFCOUNT_ADD(*sou);
            return NOERROR;
        }

    } else {
        AutoPtr<ICharSequence> filtered;
        source->SubSequence(start, end, (ICharSequence**)&filtered);
        if(TextUtils::IsEmpty(filtered)) {
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(String(""), (ICharSequence**)&seq);
            *sou = seq;
            REFCOUNT_ADD(*sou);
            return NOERROR;
        }
        AutoPtr<ICharSequence> res;
        dest->SubSequence(0, dstart, (ICharSequence**)&res);
        String result = String(NULL);
        res->ToString(&result);
        String subString = result;
        filtered->ToString(&result);
        subString += result;
        Int32 len = 0;
        dest->GetLength(&len);
        res = NULL;
        dest->SubSequence(dend, len, (ICharSequence**)&res);
        res->ToString(&result);
        subString += result;

        subString = subString.ToLowerCase();
        for(Int32 i = 0; i < mHost->mDisplayedValues->GetLength(); i++) {
            String valLowerCase = (*mHost->mDisplayedValues)[i].ToLowerCase();
            if(valLowerCase.StartWith(subString)) {
                mHost->PostSetSelectionCommand(subString.GetLength(), valLowerCase.GetLength());
                AutoPtr<ICharSequence> seq;
                CStringWrapper::New(valLowerCase, (ICharSequence**)&seq);
                AutoPtr<ICharSequence> charsequence;
                seq->SubSequence(dstart, valLowerCase.GetLength(), (ICharSequence**)&charsequence);
                *sou = charsequence;
                REFCOUNT_ADD(*sou);
                return NOERROR;
            }
        }
        AutoPtr<ICharSequence> seq;
        CStringWrapper::New(String(""), (ICharSequence**)&seq);
        *sou = seq;
        REFCOUNT_ADD(*sou);
        return NOERROR;
    }
}

ECode NumberPicker::InputTextFilter::ClearMetaKeyState(
    /* [in] */ Int64 state,
    /* [in] */ Int32 which,
    /* [out] */ Int64* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = NumberKeyListener::ClearMetaKeyState(state, which);
    return NOERROR;
}

ECode NumberPicker::InputTextFilter::Backspace(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = NumberKeyListener::Backspace(view, content, keyCode, event);
    return NOERROR;
}

ECode NumberPicker::InputTextFilter::ForwardDelete(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = NumberKeyListener::ForwardDelete(view, content, keyCode, event);
    return NOERROR;
}

AutoPtr<ArrayOf<Char32> > NumberPicker::InputTextFilter::GetAcceptedChars()
{
    UInt32 len = sizeof(DIGIT_CHARACTERS) / sizeof(DIGIT_CHARACTERS[0]);
    AutoPtr<ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(len);
    for (Int32 i = 0; i < len; i++) {
        (*chars)[i] = DIGIT_CHARACTERS[i];
    }
    return chars;
}

//==============================================================================================
//          NumberPicker::PressedStateHelper
//==============================================================================================

NumberPicker::PressedStateHelper::PressedStateHelper(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

void NumberPicker::PressedStateHelper::Cancel()
{
    mMode = 0;
    mManagedButton = 0;
    mHost->RemoveCallbacks(this);
    if(mHost->mIncrementVirtualButtonPressed) {
        mHost->mIncrementVirtualButtonPressed = FALSE;
        mHost->Invalidate(0, mHost->mBottomSelectionDividerBottom, mHost->mRight, mHost->mBottom);
    }
    mHost->mDecrementVirtualButtonPressed = FALSE;
    if(mHost->mDecrementVirtualButtonPressed) {
        mHost->Invalidate(0, 0, mHost->mRight, mHost->mTopSelectionDividerTop);
    }
}

void NumberPicker::PressedStateHelper::ButtonPressDelayed(
    /* [in] */ Int32 button)
{
    Cancel();
    mMode = MODE_PRESS;
    mManagedButton = button;
    mHost->PostDelayed(this, CViewConfiguration::GetTapTimeout());
}

void NumberPicker::PressedStateHelper::ButtonTapped(
    /* [in] */ Int32 button)
{
    Cancel();
    mMode = MODE_TAPPED;
    mManagedButton = button;
    mHost->Post(this);
}

ECode NumberPicker::PressedStateHelper::Run()
{
    switch (mMode) {
        case MODE_PRESS:
            switch (mManagedButton) {
                case BUTTON_INCREMENT:
                    mHost->mIncrementVirtualButtonPressed = TRUE;
                    mHost->Invalidate(0, mHost->mBottomSelectionDividerBottom, mHost->mRight, mHost->mBottom);
                    break;
                case BUTTON_DECREMENT:
                    mHost->mDecrementVirtualButtonPressed = TRUE;
                    mHost->Invalidate(0, 0, mHost->mRight, mHost->mTopSelectionDividerTop);
            }
            break;
        case MODE_TAPPED:
            switch (mManagedButton) {
                case BUTTON_INCREMENT:
                    if(!mHost->mIncrementVirtualButtonPressed) {
                        mHost->PostDelayed(this, CViewConfiguration::GetPressedStateDuration());
                    }
                    mHost->mIncrementVirtualButtonPressed ^= TRUE;
                    mHost->Invalidate(0, mHost->mBottomSelectionDividerBottom, mHost->mRight, mHost->mBottom);
                    break;
                case BUTTON_DECREMENT:
                    if(!mHost->mDecrementVirtualButtonPressed) {
                        mHost->PostDelayed(this, CViewConfiguration::GetPressedStateDuration());
                    }
                    mHost->mDecrementVirtualButtonPressed ^= TRUE;
                    mHost->Invalidate(0, 0, mHost->mRight, mHost->mTopSelectionDividerTop);
                    break;
            }
    }
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::SetSelectionCommand
//==============================================================================================

NumberPicker::SetSelectionCommand::SetSelectionCommand(
    /* [in] */ INumberPicker* host)
    : mSelectionStart(0)
    , mSelectionEnd(0)
{
    IWeakReferenceSource::Probe(host)->GetWeakReference((IWeakReference**)&mWeakHost);
    assert(mWeakHost);
}

ECode NumberPicker::SetSelectionCommand::Run()
{
    AutoPtr<INumberPicker> num;
    mWeakHost->Resolve(EIID_INumberPicker, (IInterface**)&num);
    if (num == NULL) {
        return NOERROR;
    }
    NumberPicker* host = (NumberPicker*)(CNumberPicker*)num.Get();
    host->mInputText->SetSelection(mSelectionStart, mSelectionEnd);
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::ChangeCurrentByOneFromLongPressCommand
//==============================================================================================

NumberPicker::ChangeCurrentByOneFromLongPressCommand::ChangeCurrentByOneFromLongPressCommand(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

void NumberPicker::ChangeCurrentByOneFromLongPressCommand::SetStep(
    /* [in] */ Boolean increment)
{
    mIncrement = increment;
}

ECode NumberPicker::ChangeCurrentByOneFromLongPressCommand::Run()
{
    mHost->ChangeValueByOne(mIncrement);
    mHost->PostDelayed(this, mHost->mLongPressUpdateInterval);
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::BeginSoftInputOnLongPressCommand
//==============================================================================================

NumberPicker::BeginSoftInputOnLongPressCommand::BeginSoftInputOnLongPressCommand(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

ECode NumberPicker::BeginSoftInputOnLongPressCommand::Run()
{
    mHost->ShowSoftInput();
    mHost->mIngonreMoveEvents = TRUE;
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::CustomEditText
//==============================================================================================

NumberPicker::CustomEditText::CustomEditText(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    EditText::Init(context, attrs);
}

ECode NumberPicker::CustomEditText::OnEditorAction(
    /* [in] */ Int32 actionCode)
{
    EditText::OnEditorAction(actionCode);
    if(actionCode == IEditorInfo::IME_ACTION_NONE) {
        View::ClearFocus();
    }
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::AccessibilityNodeProviderImpl
//==============================================================================================

PInterface NumberPicker::AccessibilityNodeProviderImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IAccessibilityNodeProvider) {
        return reinterpret_cast<PInterface>((IAccessibilityNodeProvider*)this);
    } else if(riid == EIID_IInterface) {
        return (PInterface)(IAccessibilityNodeProvider*)this;
    } else if (riid == EIID_AccessibilityNodeProviderImpl) {
        return reinterpret_cast<PInterface>((AccessibilityNodeProviderImpl*)this);
    } else if (riid == EIID_IInterface) {
        return (PInterface)(IAccessibilityNodeProvider*)this;
    }
    return NULL;
}

UInt32 NumberPicker::AccessibilityNodeProviderImpl::AddRef()
{
    return ElRefBase::AddRef();
}


UInt32 NumberPicker::AccessibilityNodeProviderImpl::Release()
{
    return ElRefBase::Release();
}

ECode NumberPicker::AccessibilityNodeProviderImpl::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
     VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IAccessibilityNodeProvider *)this) {
            *iid = EIID_IAccessibilityNodeProvider ;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

NumberPicker::AccessibilityNodeProviderImpl::AccessibilityNodeProviderImpl(
    /* [in] */ NumberPicker* host) :
    mAccessibilityFocusedView(UNDEFINED),
    mHost(host)
{
    CRect::New((IRect**)&mTempRect);
    mTempArray = ArrayOf<Int32>::Alloc(2);
}

ECode NumberPicker::AccessibilityNodeProviderImpl::CreateAccessibilityNodeInfo(
    /* [in] */ Int32 virtualViewId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    switch (virtualViewId) {
        case IView::NO_ID:
        {
            AutoPtr<IAccessibilityNodeInfo> nodeInfo = CreateAccessibilityNodeInfoForNumberPicker(mHost->mScrollX, mHost->mScrollY,
                mHost->mScrollX + (mHost->mRight - mHost->mLeft), mHost->mScrollY + (mHost->mBottom - mHost->mTop));
            *info = nodeInfo.Get();
            REFCOUNT_ADD(*info);
        }
            return NOERROR;
        case VIRTUAL_VIEW_ID_DECREMENT:
        {
            AutoPtr<IAccessibilityNodeInfo> nodeInfo = CreateAccessibilityNodeInfoForVirtualButton(VIRTUAL_VIEW_ID_DECREMENT, GetVirtualDecrementButtonText(),
                mHost->mScrollX, mHost->mScrollY, mHost->mScrollX + (mHost->mRight - mHost->mLeft),
                mHost->mTopSelectionDividerTop + mHost->mSelectionDividerHeight);
            *info = nodeInfo.Get();
            REFCOUNT_ADD(*info);
        }
            return NOERROR;
        case VIRTUAL_VIEW_ID_INPUT:
        {
            AutoPtr<IAccessibilityNodeInfo> nodeInfo = CreateAccessibiltyNodeInfoForInputText();
            *info = nodeInfo.Get();
            REFCOUNT_ADD(*info);
        }
            return NOERROR;
        case VIRTUAL_VIEW_ID_INCREMENT:
        {
            AutoPtr<IAccessibilityNodeInfo> nodeInfo = CreateAccessibilityNodeInfoForVirtualButton(VIRTUAL_VIEW_ID_INCREMENT, GetVirtualIncrementButtonText(),
                mHost->mScrollX, mHost->mBottomSelectionDividerBottom - mHost->mSelectionDividerHeight,
                mHost->mScrollX + (mHost->mRight - mHost->mLeft), mHost->mScrollY + (mHost->mBottom - mHost->mTop));
            *info = nodeInfo.Get();
            REFCOUNT_ADD(*info);
        }
            return NOERROR;
    }
    AccessibilityNodeProvider::CreateAccessibilityNodeInfo(virtualViewId, info);
    return NOERROR;
}

ECode NumberPicker::AccessibilityNodeProviderImpl::FindAccessibilityNodeInfosByText(
    /* [in] */ const String& searched,
    /* [in] */ Int32 virtualViewId,
    /* [out] */ IObjectContainer** object)
{
    VALIDATE_NOT_NULL(object);
    if(searched.IsEmpty()) {
        *object = NULL;
        return NOERROR;
    }
    String unConstsearched = searched.ToLowerCase();
    String searchedLowerCase = unConstsearched;
    AutoPtr<IObjectContainer> result;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&result));
    switch (virtualViewId) {
        case IView::NO_ID:
            FindAccessibilityNodeInfosByTextInChild(searchedLowerCase, VIRTUAL_VIEW_ID_DECREMENT, result);
            FindAccessibilityNodeInfosByTextInChild(searchedLowerCase, VIRTUAL_VIEW_ID_INPUT, result);
            FindAccessibilityNodeInfosByTextInChild(searchedLowerCase, VIRTUAL_VIEW_ID_INCREMENT, result);
            *object = result.Get();
            REFCOUNT_ADD(*object);
            return NOERROR;
        case VIRTUAL_VIEW_ID_DECREMENT:
        case VIRTUAL_VIEW_ID_INCREMENT:
        case VIRTUAL_VIEW_ID_INPUT:
            FindAccessibilityNodeInfosByTextInChild(searchedLowerCase, virtualViewId, result);
            *object = result.Get();
            REFCOUNT_ADD(*object);
            return NOERROR;
    }
    AccessibilityNodeProvider::FindAccessibilityNodeInfosByText(unConstsearched, virtualViewId, object);
    return NOERROR;
}

ECode NumberPicker::AccessibilityNodeProviderImpl::PerformAction(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    switch(virtualViewId) {
        case IView::NO_ID:
            switch (action) {
                case IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS:
                {
                    if(mAccessibilityFocusedView != virtualViewId) {
                        mAccessibilityFocusedView = virtualViewId;
                        mHost->RequestAccessibilityFocus();
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
                case IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS:
                {
                    if(mAccessibilityFocusedView == virtualViewId) {
                        mAccessibilityFocusedView = UNDEFINED;
                        mHost->ClearAccessibilityFocus();
                        *res = TRUE;
                        return NOERROR;
                    }
                    *res = FALSE;
                    return NOERROR;
                }
                case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD:
                {
                    if(mHost->IsEnabled() && (mHost->GetWrapSelectorWheel() || mHost->GetValue() < mHost->GetMaxValue())) {
                        mHost->ChangeValueByOne(TRUE);
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
                case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD:
                {
                    if(mHost->IsEnabled() && (mHost->GetWrapSelectorWheel() || mHost->GetValue() > mHost->GetMinValue())) {
                        mHost->ChangeValueByOne(TRUE);
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
            }
            break;
        case VIRTUAL_VIEW_ID_INPUT:
            switch (action) {
                case IAccessibilityNodeInfo::ACTION_FOCUS:
                {
                    Boolean isFocused = FALSE;
                    mHost->mInputText->IsFocused(&isFocused);
                    if(mHost->IsEnabled() && !isFocused) {
                        mHost->mInputText->RequestFocus(res);
                        return NOERROR;
                    }
                }
                    break;
                case IAccessibilityNodeInfo::ACTION_CLEAR_FOCUS:
                {
                        Boolean isFocused = FALSE;
                        mHost->mInputText->IsFocused(&isFocused);
                        if(mHost->IsEnabled() && isFocused) {
                            mHost->mInputText->ClearFocus();
                            *res = TRUE;
                            return NOERROR;
                        }
                        *res = FALSE;
                        return NOERROR;
                }
                case IAccessibilityNodeInfo::ACTION_CLICK:
                {
                    if(mHost->IsEnabled()) {
                        mHost->ShowSoftInput();
                        *res = TRUE;
                        return NOERROR;
                    }
                    *res = FALSE;
                    return NOERROR;
                }
                case IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS:
                {
                    if(mAccessibilityFocusedView != virtualViewId) {
                        mAccessibilityFocusedView = virtualViewId;
                        SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
                        mHost->mInputText->Invalidate();
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
                case IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS:
                {
                    if(mAccessibilityFocusedView == virtualViewId) {
                        mAccessibilityFocusedView = UNDEFINED;
                        SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED);
                        mHost->mInputText->Invalidate();
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
                default:
                    mHost->mInputText->PerformAccessibilityAction(action, arguments, res);
                    return NOERROR;
            }
                *res = FALSE;
                return NOERROR;
        case VIRTUAL_VIEW_ID_INCREMENT:
            switch (action) {
                case IAccessibilityNodeInfo::ACTION_CLICK:
                {
                    if(mHost->IsEnabled()) {
                        mHost->ChangeValueByOne(TRUE);
                        SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_CLICKED);
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
                case IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS:
                {
                    if(mAccessibilityFocusedView != virtualViewId) {
                        mAccessibilityFocusedView = virtualViewId;
                        SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
                        mHost->Invalidate(0, mHost->mBottomSelectionDividerBottom, mHost->mRight, mHost->mBottom);
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
                case IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS:
                {
                    if(mAccessibilityFocusedView == virtualViewId) {
                        mAccessibilityFocusedView = UNDEFINED;
                        SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED);
                        mHost->Invalidate(0, mHost->mBottomSelectionDividerBottom, mHost->mRight, mHost->mBottom);
                        *res = TRUE;
                        return NOERROR;
                    }
                }
                    *res = FALSE;
                    return NOERROR;
            }
                *res = FALSE;
                return NOERROR;

            case VIRTUAL_VIEW_ID_DECREMENT:
                switch (action) {
                    case IAccessibilityNodeInfo::ACTION_CLICK:
                    {
                        if(mHost->IsEnabled()) {
                            Boolean increment = (virtualViewId == VIRTUAL_VIEW_ID_INCREMENT);
                            mHost->ChangeValueByOne(increment);
                            SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_CLICKED);
                            *res = TRUE;
                            return NOERROR;
                        }
                    }
                        *res = FALSE;
                        return NOERROR;
                    case IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS:
                    {
                        if(mAccessibilityFocusedView != virtualViewId) {
                            mAccessibilityFocusedView = virtualViewId;
                            SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
                            mHost->Invalidate(0, 0, mHost->mRight, mHost->mTopSelectionDividerTop);
                            *res = TRUE;
                            return NOERROR;
                        }
                    }
                        *res = FALSE;
                        return NOERROR;
                    case IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS:
                    {
                        if(mAccessibilityFocusedView == virtualViewId) {
                            mAccessibilityFocusedView = UNDEFINED;
                            SendAccessibilityEventForVirtualView(virtualViewId, IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED);
                            mHost->Invalidate(0, 0, mHost->mRight, mHost->mTopSelectionDividerTop);
                            *res = TRUE;
                            return NOERROR;
                        }
                    }
                        *res = FALSE;
                        return NOERROR;
                }
                    *res = FALSE;
                    return NOERROR;

    }
    AccessibilityNodeProvider::PerformAction(virtualViewId, action, arguments, res);
    return NOERROR;
}

void NumberPicker::AccessibilityNodeProviderImpl::SendAccessibilityEventForVirtualView(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 eventType)
{
    switch (virtualViewId) {
        case VIRTUAL_VIEW_ID_DECREMENT:
        {
            if(HasVirtualDecrementButton()) {
                SendAccessibilityEventForVirtualButton(virtualViewId, eventType, GetVirtualDecrementButtonText());
            }
        }
            break;

        case VIRTUAL_VIEW_ID_INPUT:
        {
            SendAccessibilityEventForVirtualText(eventType);
        }
            break;

        case VIRTUAL_VIEW_ID_INCREMENT:
        {
            if(HasVirtualIncrementButton()) {
                SendAccessibilityEventForVirtualButton(virtualViewId, eventType, GetVirtualIncrementButtonText());
            }
        }
            break;
    }
}

void NumberPicker::AccessibilityNodeProviderImpl::SendAccessibilityEventForVirtualText(
    /* [in] */ Int32 eventType)
{
    AutoPtr<IAccessibilityManager> am;
    CAccessibilityManager::GetInstance(mHost->mContext, (IAccessibilityManager**)&am);
    Boolean isEnabled = FALSE;
    am->IsEnabled(&isEnabled);
    if(isEnabled) {
        AutoPtr<IAccessibilityEvent> event;
        CAccessibilityEvent::Obtain(eventType, (IAccessibilityEvent**)&event);
        mHost->mInputText->OnInitializeAccessibilityEvent(event);
        mHost->mInputText->OnPopulateAccessibilityEvent(event);
        event->SetSource((IView*)mHost->Probe(EIID_IView), VIRTUAL_VIEW_ID_INPUT);
        mHost->RequestSendAccessibilityEvent((IView*)mHost->Probe(EIID_IView), event);
    }
}

void NumberPicker::AccessibilityNodeProviderImpl::SendAccessibilityEventForVirtualButton(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 eventType,
    /* [in] */ const String& text)
{
    AutoPtr<IAccessibilityManager> am;
    CAccessibilityManager::GetInstance(mHost->mContext, (IAccessibilityManager**)&am);
    Boolean isEnabled = FALSE;
    am->IsEnabled(&isEnabled);
    if(isEnabled) {
        AutoPtr<IAccessibilityEvent> event;
        CAccessibilityEvent::Obtain(eventType, (IAccessibilityEvent**)&event);
        String str = String("Button");
        AutoPtr<ICharSequence> strCsq;
        CStringWrapper::New(str, (ICharSequence**)&strCsq);
        event->SetClassName(strCsq);
        AutoPtr<IObjectContainer> object;
        event->GetText((IObjectContainer**)&object);
        AutoPtr<ICharSequence> textCsq;
        CStringWrapper::New(str, (ICharSequence**)&textCsq);
        object->Add(textCsq);
        event->SetEnabled(mHost->IsEnabled());
        event->SetSource((IView*)mHost->Probe(EIID_IView), virtualViewId);
        mHost->RequestSendAccessibilityEvent((IView*)mHost->Probe(EIID_IView), event);
    }
}

void NumberPicker::AccessibilityNodeProviderImpl::FindAccessibilityNodeInfosByTextInChild(
    /* [in] */ const String& searchedLowerCase,
    /* [in] */ Int32 virtualViewId,
    /* [in] */ IObjectContainer* outResult)
{
    switch (virtualViewId) {
        case VIRTUAL_VIEW_ID_DECREMENT:
        {
            String text = GetVirtualDecrementButtonText();
            text = text.ToLowerCase();
            if(text.IsEmpty() && text.Contains(searchedLowerCase)) {
                AutoPtr<IAccessibilityNodeInfo> info;
                CreateAccessibilityNodeInfo(VIRTUAL_VIEW_ID_DECREMENT, (IAccessibilityNodeInfo**)&info);
                outResult->Add(info);
            }
        }
            return;
        case VIRTUAL_VIEW_ID_INPUT:
        {
            AutoPtr<ICharSequence> text;
            mHost->mInputText->GetText((ICharSequence**)&text);
            String textStr = String(NULL);
            text->ToString(&textStr);
            textStr = textStr.ToLowerCase();
            AutoPtr<IAccessibilityNodeInfo> info;
            CreateAccessibilityNodeInfo(VIRTUAL_VIEW_ID_INPUT, (IAccessibilityNodeInfo**)&info);
            if(!TextUtils::IsEmpty(text) && textStr.Contains(searchedLowerCase)) {
                outResult->Add(info);
            }

            AutoPtr<ICharSequence> contentDesc;
            mHost->mInputText->GetText((ICharSequence**)&contentDesc);
            String contStr = String(NULL);
            text->ToString(&contStr);
            contStr = contStr.ToLowerCase();
            if(!TextUtils::IsEmpty(text) && contStr.Contains(searchedLowerCase)) {
                outResult->Add(info);
            }
        }
            break;
        case VIRTUAL_VIEW_ID_INCREMENT:
        {
            String text = GetVirtualIncrementButtonText();
            text = text.ToLowerCase();
            if(!text.IsEmpty() && text.Contains(searchedLowerCase)) {
                AutoPtr<IAccessibilityNodeInfo> info;
                CreateAccessibilityNodeInfo(VIRTUAL_VIEW_ID_INCREMENT, (IAccessibilityNodeInfo**)&info);
                outResult->Add(info);
            }
         }
            return;
    }
}

AutoPtr<IAccessibilityNodeInfo> NumberPicker::AccessibilityNodeProviderImpl::CreateAccessibiltyNodeInfoForInputText()
{
    AutoPtr<IAccessibilityNodeInfo> info;
    mHost->mInputText->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&info);
    info->SetSource((IView*)mHost->Probe(EIID_IView), VIRTUAL_VIEW_ID_INPUT);
    if(mAccessibilityFocusedView != VIRTUAL_VIEW_ID_INPUT) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS);
    }
    if(mAccessibilityFocusedView == VIRTUAL_VIEW_ID_INPUT) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS);
    }
    return info;
}

AutoPtr<IAccessibilityNodeInfo> NumberPicker::AccessibilityNodeProviderImpl::CreateAccessibilityNodeInfoForVirtualButton(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ const String& text,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IAccessibilityNodeInfo> info;
    CAccessibilityNodeInfo::Obtain((IAccessibilityNodeInfo**)&info);
    String str = String("Button");
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    info->SetClassName(seq);
    String packageName = String(NULL);
    mHost->mContext->GetPackageName(&packageName);
    seq = NULL;
    CStringWrapper::New(packageName, (ICharSequence**)&seq);
    info->SetPackageName(seq);
    info->SetSource((IView*)mHost->Probe(EIID_IView), virtualViewId);
    info->SetParent((IView*)mHost->Probe(EIID_IView));
    seq = NULL;
    CStringWrapper::New(text, (ICharSequence**)&seq);
    info->SetText(seq);
    info->SetClickable(TRUE);
    info->SetLongClickable(TRUE);
    info->SetEnabled(mHost->IsEnabled());

    AutoPtr<IRect> boundsInParent = mTempRect;
    boundsInParent->Set(mHost->mLeft, mHost->mTop, mHost->mRight, mHost->mBottom);
    info->SetVisibleToUser(mHost->IsVisibleToUser(boundsInParent));
    info->SetBoundsInParent(boundsInParent);

    AutoPtr<IRect> boundsInScreen = boundsInParent;
    AutoPtr<ArrayOf<Int32> > locationOnScreen = mTempArray;
    Int32 x = 0, y = 0;
    mHost->GetLocationOnScreen(&x, &y);
    (*locationOnScreen)[0] = x;
    (*locationOnScreen)[1] = y;
    boundsInScreen->Offset(x, y);
    info->SetBoundsInScreen(boundsInScreen);

    if(mAccessibilityFocusedView != virtualViewId) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS);
    }
    if(mAccessibilityFocusedView == virtualViewId) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS);
    }
    if(mHost->IsEnabled()) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLICK);
    }

    return info;
}

AutoPtr<IAccessibilityNodeInfo> NumberPicker::AccessibilityNodeProviderImpl::CreateAccessibilityNodeInfoForNumberPicker(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IAccessibilityNodeInfo> info;
    CAccessibilityNodeInfo::Obtain((IAccessibilityNodeInfo**)&info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(NUMBERPICkER_NAME, (ICharSequence**)&seq);
    info->SetClassName(seq);
    String packageName = String(NULL);
    mHost->mContext->GetPackageName(&packageName);
    seq = NULL;
    CStringWrapper::New(packageName, (ICharSequence**)&seq);
    info->SetPackageName(seq);
    info->SetSource((IView*)mHost->Probe(EIID_IView));

    if(HasVirtualDecrementButton()) {
        info->AddChild((IView*)mHost->Probe(EIID_IView), VIRTUAL_VIEW_ID_DECREMENT);
    }
    info->AddChild((IView*)this->Probe(EIID_IView), VIRTUAL_VIEW_ID_INPUT);
    if(HasVirtualIncrementButton()) {
        info->AddChild((IView*)mHost->Probe(EIID_IView), VIRTUAL_VIEW_ID_INCREMENT);
    }

    info->SetParent(IView::Probe(mHost->GetParentForAccessibility()));
    info->SetEnabled(mHost->IsEnabled());
    info->SetScrollable(TRUE);

    AutoPtr<IContext> context = mHost->GetContext();
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ICompatibilityInfo> cominfo;
    resources->GetCompatibilityInfo((ICompatibilityInfo**)&cominfo);
    AutoPtr<CCompatibilityInfo> cInof = (CCompatibilityInfo*)cominfo.Get();
    Float applicationScale = cInof->mApplicationScale;
    AutoPtr<IRect> boundsInParent = mTempRect;
    boundsInParent->Set(mHost->mLeft, mHost->mTop, mHost->mRight, mHost->mBottom);
    boundsInParent->Scale(applicationScale);
    info->SetBoundsInParent(boundsInParent);
    info->SetVisibleToUser(mHost->IsVisibleToUser());

    AutoPtr<IRect> boundsInScreen = boundsInParent;
    AutoPtr<ArrayOf<Int32> > locationOnScreen = mTempArray;
    Int32 x = 0, y = 0;
    mHost->GetLocationOnScreen(&x, &y);
    (*locationOnScreen)[0] = x;
    (*locationOnScreen)[1] = y;
    boundsInScreen->Offset(x, y);
    boundsInScreen->Scale(applicationScale);
    info->SetBoundsInScreen(boundsInScreen);

    if(mAccessibilityFocusedView != IView::NO_ID) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS);
    }
    if(mAccessibilityFocusedView == IView::NO_ID) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS);
    }
    if(mHost->IsEnabled()) {
        if(mHost->GetWrapSelectorWheel() || mHost->GetValue() < mHost->GetMaxValue()) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
        }
        if(mHost->GetWrapSelectorWheel() || mHost->GetValue() > mHost->GetMinValue()) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
        }
    }
    return info;
}

Boolean NumberPicker::AccessibilityNodeProviderImpl::HasVirtualDecrementButton()
{
    return mHost->GetWrapSelectorWheel() || mHost->GetValue() > mHost->GetMinValue();
}

Boolean NumberPicker::AccessibilityNodeProviderImpl::HasVirtualIncrementButton()
{
    return mHost->GetWrapSelectorWheel() || mHost->GetValue() < mHost->GetMaxValue();
}

String NumberPicker::AccessibilityNodeProviderImpl::GetVirtualDecrementButtonText()
{
    Int32 value = mHost->mValue - 1;
    if(mHost->mWrapSelectorWheel) {
        value = mHost->GetWrappedSelectorIndex(value);
    }
    if(value >= mHost->mMinValue) {
        return (mHost->mDisplayedValues == NULL) ? mHost->FormatNumber(value)
            : (*mHost->mDisplayedValues)[value - mHost->mMinValue];
    }
    return String(NULL);
}

String NumberPicker::AccessibilityNodeProviderImpl::GetVirtualIncrementButtonText()
{
    Int32 value = mHost->mValue + 1;
    if(mHost->mWrapSelectorWheel) {
        value = mHost->GetWrappedSelectorIndex(value);
    }
    if(value <= mHost->mMaxValue) {
        return (mHost->mDisplayedValues == NULL) ? mHost->FormatNumber(value)
            : (*mHost->mDisplayedValues)[value - mHost->mMinValue];
    }
    return String(NULL);
}

String NumberPicker::AccessibilityNodeProviderImpl::FormatNumberWithLocale(
    /* [in] */ Int32 value)
{

    String result("");
    result.AppendFormat("%d", value);
    return result;
}

//==============================================================================================
//          NumberPicker::NumberPickerOnCliskListener
//==============================================================================================

NumberPicker::NumberPickerOnCliskListener::NumberPickerOnCliskListener(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

ECode NumberPicker::NumberPickerOnCliskListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->HideSoftInput();
    mHost->mInputText->ClearFocus();
    Int32 id = 0;
    v->GetId(&id);
    if(id == R::id::increment) {
        mHost->ChangeValueByOne(TRUE);
    } else {
        mHost->ChangeValueByOne(FALSE);
    }
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::NumberPickerOnLongCliskListener
//==============================================================================================


NumberPicker::NumberPickerOnLongCliskListener::NumberPickerOnLongCliskListener(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

ECode NumberPicker::NumberPickerOnLongCliskListener::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mHost->HideSoftInput();
    mHost->mInputText->ClearFocus();
    Int32 id = 0;
    v->GetId(&id);
    if(id == R::id::increment) {
        mHost->PostChangeCurrentByOneFromLongPress(TRUE, 0);
    } else {
        mHost->PostChangeCurrentByOneFromLongPress(FALSE, 0);
    }
    *result = FALSE;
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::NumberPickerOnFocusChangeListener
//==============================================================================================

NumberPicker::NumberPickerOnFocusChangeListener::NumberPickerOnFocusChangeListener(
    /* [in] */ NumberPicker* host) :
    mHost(host)
{
}

ECode NumberPicker::NumberPickerOnFocusChangeListener::OnFocusChange(
    /* [in] */ IView* v,
    /* [in] */ Boolean hasFocus)
{
    if(hasFocus) {
        mHost->mInputText->SelectAll();
    } else {
        mHost->mInputText->SetSelection(0, 0);
        mHost->ValidateInputTextView(v);
    }
    return NOERROR;
}

//==============================================================================================
//          NumberPicker::TwoDigitFormatter
//==============================================================================================

NumberPicker::TwoDigitFormatter::TwoDigitFormatter()
{
    mArgs = ArrayOf<IInterface*>::Alloc(1);
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> locale;
    helper->GetDefault((ILocale**)&locale);
    Init(locale);
}

ECode NumberPicker::TwoDigitFormatter::Format(
    /* [in] */ Int32 value,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> currentLocale;
    helper->GetDefault((ILocale**)&currentLocale);
    if(mZeroDigit != GetZeroDigit(currentLocale)) {
        Init(currentLocale);
    }

    AutoPtr<IInteger32> integer;
    CInteger32::New(value, (IInteger32**)&integer);
    AutoPtr<IInterface> i = integer->Probe(EIID_IInterface);
    mArgs->Set(0, i);
    Int32 length = 0;
    mBuilder.GetLength(&length);
    mBuilder.Delete(0, length);
    Int32 ivalue = 0;
    IInteger32::Probe((*mArgs)[0])->GetValue(&ivalue);
    String result("");
    result.AppendFormat("%d", ivalue);
    *str = result;
    return NOERROR;
}

ECode NumberPicker::TwoDigitFormatter::Init(
    /* [in] */ ILocale* locale)
{
    mZeroDigit = GetZeroDigit(locale);
    return NOERROR;
}

Char32 NumberPicker::TwoDigitFormatter::GetZeroDigit(
    /* [in] */ ILocale* locale)
{
    AutoPtr<ILocaleDataHelper> helper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&helper);
    AutoPtr<ILocaleData> data;
    helper->Get(locale, (ILocaleData**)&data);
    Char32 zerodigit = 0;
    data->GetZeroDigit(&zerodigit);
    return zerodigit;
}

AutoPtr<IFormatter> NumberPicker::TwoDigitFormatter::CreateFormatter(
    /* [in] */ ILocale* locale)
{
    //assert(0 && "TODO");
    return NULL;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos






















