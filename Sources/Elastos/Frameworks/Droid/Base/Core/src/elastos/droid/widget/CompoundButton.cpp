
#include "elastos/droid/graphics/drawable/Drawable.h"
#include "elastos/droid/widget/CompoundButton.h"

using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Widget {

static AutoPtr<ArrayOf<Int32> > Init_CHECKED_STATE_SET()
{
    AutoPtr<ArrayOf<Int32> > temp = ArrayOf<Int32>::Alloc(1);
    (*temp)[0] = R::attr::state_checked;
    return temp;
}

AutoPtr<ArrayOf<Int32> > CompoundButton::CHECKED_STATE_SET =Init_CHECKED_STATE_SET();

///////////////////////////////////////////////////////////////////////
//              CompoundButton::CompoundButtonSavedState
///////////////////////////////////////////////////////////////////////
CompoundButton::CompoundButtonSavedState::CompoundButtonSavedState()
    : mChecked(FALSE)
{}

ECode CompoundButton::CompoundButtonSavedState::constructor()
{
    return NOERROR;
}

ECode CompoundButton::CompoundButtonSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return BaseSavedState::constructor(superState);
}

ECode CompoundButton::CompoundButtonSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    BaseSavedState::WriteToParcel(dest);
    dest->WriteBoolean(mChecked);
    return NOERROR;
}

ECode CompoundButton::CompoundButtonSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    BaseSavedState::ReadFromParcel(source);
    source->ReadBoolean(&mChecked);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////
//                      CompoundButton
///////////////////////////////////////////////////////////////////////
CAR_INTERFACE_IMPL_2(CompoundButton, Button, ICompoundButton, ICheckable)

CompoundButton::CompoundButton()
    : mChecked(FALSE)
    , mButtonResource(0)
    , mBroadcasting(FALSE)
    , mButtonTintMode(Elastos::Droid::Graphics::PorterDuffMode_NONE)
    , mHasButtonTint(FALSE)
    , mHasButtonTintMode(FALSE)
{}

ECode CompoundButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    Button::constructor(context, attrs, defStyleAttr, defStyleRes);
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyleAttr, defStyleRes));
    return NOERROR;
}

ECode CompoundButton::Toggle()
{
    return SetChecked(!mChecked);
}

Boolean CompoundButton::PerformClick()
{
    /*
     * XXX: These are tiny, need some surrounding 'expanded touch area',
     * which will need to be implemented in Button if we only override
     * performClick()
     */

    /* When clicked, toggle the state */
    Toggle();
    Boolean result;
    Button::PerformClick(&result);
    return result;
}

Boolean CompoundButton::IsChecked()
{
    return mChecked;
}

/**
 * <p>Changes the checked state of this button.</p>
 *
 * @param checked TRUE to check the button, FALSE to uncheck it
 */
ECode CompoundButton::SetChecked(
    /* [in] */ Boolean checked)
{
    if (mChecked != checked) {
        mChecked = checked;
        RefreshDrawableState();

        NotifyViewAccessibilityStateChangedIfNeeded(
            IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);

        // Avoid infinite recursions if setChecked() is called from a listener
        if (mBroadcasting) {
            return NOERROR;
        }

        mBroadcasting = TRUE;
        if (mOnCheckedChangeListener != NULL) {
            mOnCheckedChangeListener->OnCheckedChanged(
                THIS_PROBE(ICompoundButton), mChecked);
        }
        if (mOnCheckedChangeWidgetListener != NULL) {
            mOnCheckedChangeWidgetListener->OnCheckedChanged(
                THIS_PROBE(ICompoundButton), mChecked);
        }

        mBroadcasting = FALSE;
    }

    return NOERROR;
}

/**
 * Register a callback to be invoked when the checked state of this button
 * changes.
 *
 * @param listener the callback to call on checked state change
 */
ECode CompoundButton::SetOnCheckedChangeListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    mOnCheckedChangeListener = listener;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the checked state of this button
 * changes. This callback is used for internal purpose only.
 *
 * @param listener the callback to call on checked state change
 * @hide
 */
ECode CompoundButton::SetOnCheckedChangeWidgetListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    mOnCheckedChangeWidgetListener = listener;

    return NOERROR;
}

/**
 * Set the background to a given Drawable, identified by its resource id.
 *
 * @param resid the resource id of the drawable to use as the background
 */
ECode CompoundButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    if (resid != 0 && resid == mButtonResource) {
        return NOERROR;
    }

    mButtonResource = resid;

    AutoPtr<IDrawable> d;
    if (mButtonResource != 0) {
        AutoPtr<IContext> ctx;
        GetContext((IContext**)&ctx);
        ctx->GetDrawable(mButtonResource, (IDrawable**)&d);
    }
    return SetButtonDrawable(d);
}

/**
 * Set the background to a given Drawable
 *
 * @param d The Drawable to use as the background
 */
ECode CompoundButton::SetButtonDrawable(
    /* [in] */ IDrawable* d)
{
    if (mButtonDrawable != 0) {
        if (mButtonDrawable != NULL) {
            mButtonDrawable->SetCallback(NULL);
            UnscheduleDrawable(mButtonDrawable);
        }
        mButtonDrawable = d;

        if (d != NULL) {
            d->SetCallback(THIS_PROBE(IDrawableCallback));
            Int32 direction;
            GetLayoutDirection(&direction);
            d->SetLayoutDirection(direction);
            Boolean isStateful;
            if (d->IsStateful(&isStateful), isStateful) {
                AutoPtr< ArrayOf<Int32> > state;
                GetDrawableState((ArrayOf<Int32>**)&state);
                d->SetState(state, &isStateful);
            }
            Int32 visibility;
            GetVisibility(&visibility);
            Boolean isDif;
            d->SetVisible(visibility == IView::VISIBLE, FALSE, &isDif);
            d->GetIntrinsicHeight(&visibility);
            SetMinHeight(visibility);
            ApplyButtonTint();
        }
    }
    return NOERROR;
}


/**
 * Applies a tint to the button drawable. Does not modify the current tint
 * mode, which is {@link PorterDuff.Mode#SRC_IN} by default.
 * <p>
 * Subsequent calls to {@link #setButtonDrawable(Drawable)} will
 * automatically mutate the drawable and apply the specified tint and tint
 * mode using
 * {@link Drawable#setTintList(ColorStateList)}.
 *
 * @param tint the tint to apply, may be {@code null} to clear tint
 *
 * @attr ref android.R.styleable#CompoundButton_buttonTint
 * @see #setButtonTintList(ColorStateList)
 * @see Drawable#setTintList(ColorStateList)
 */
ECode CompoundButton::SetButtonTintList(
    /* [in] */ IColorStateList* tint)
{
    mButtonTintList = tint;
    mHasButtonTint = TRUE;

    ApplyButtonTint();
    return NOERROR;
}

/**
 * @return the tint applied to the button drawable
 * @attr ref android.R.styleable#CompoundButton_buttonTint
 * @see #setButtonTintList(ColorStateList)
 */
//@Nullable
ECode CompoundButton::GetButtonTintList(
        /* [out] */ IColorStateList** tint)
{
    VALIDATE_NOT_NULL(tint)
    *tint = mButtonTintList;
    return NOERROR;
}

/**
 * Specifies the blending mode used to apply the tint specified by
 * {@link #setButtonTintList(ColorStateList)}} to the button drawable. The
 * default mode is {@link PorterDuff.Mode#SRC_IN}.
 *
 * @param tintMode the blending mode used to apply the tint, may be
 *                 {@code null} to clear tint
 * @attr ref android.R.styleable#CompoundButton_buttonTintMode
 * @see #getButtonTintMode()
 * @see Drawable#setTintMode(PorterDuff.Mode)
 */
ECode CompoundButton::SetButtonTintMode(
    /* [in] */ Elastos::Droid::Graphics::PorterDuffMode tintMode)
{
    mButtonTintMode = tintMode;
    mHasButtonTintMode = TRUE;

    ApplyButtonTint();
    return NOERROR;
}

/**
 * @return the blending mode used to apply the tint to the button drawable
 * @attr ref android.R.styleable#CompoundButton_buttonTintMode
 * @see #setButtonTintMode(PorterDuff.Mode)
 */
//@Nullable
ECode CompoundButton::GetButtonTintMode(
    /* [out] */ Elastos::Droid::Graphics::PorterDuffMode* tintMode)
{
    VALIDATE_NOT_NULL(tintMode);
    *tintMode = mButtonTintMode;
    return NOERROR;
}

ECode CompoundButton::ApplyButtonTint()
{
    if (mButtonDrawable != NULL && (mHasButtonTint || mHasButtonTintMode)) {
        mButtonDrawable->Mutate((IDrawable**)&mButtonDrawable);

        if (mHasButtonTint) {
            mButtonDrawable->SetTintList(mButtonTintList);
        }

        if (mHasButtonTintMode) {
            mButtonDrawable->SetTintMode(mButtonTintMode);
        }
    }
    return NOERROR;
}


Int32 CompoundButton::GetHorizontalOffsetForDrawables()
{
    AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
    if(buttonDrawable != NULL)
    {
        Int32 width;
        buttonDrawable->GetIntrinsicWidth(&width);
        return width;
    }else{
        return 0;
    }
}

ECode CompoundButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Button::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> txt;
    CString::New(String("CompoundButton"), (ICharSequence**)&txt);
    return IAccessibilityRecord::Probe(event)->SetClassName(txt);
}

ECode CompoundButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FAIL_RETURN(Button::OnInitializeAccessibilityNodeInfo(info));
    AutoPtr<ICharSequence> txt;
    CString::New(String("CompoundButton"), (ICharSequence**)&txt);
    FAIL_RETURN(info->SetClassName(txt));
    info->SetCheckable(TRUE);
    info->SetChecked(mChecked);
    return NOERROR;
}

Int32 CompoundButton::GetCompoundPaddingLeft()
{
    Int32 padding = Button::GetCompoundPaddingLeft();
    Boolean isLayoutRtl;
    if (IsLayoutRtl(&isLayoutRtl), !isLayoutRtl) {
        AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
        if (buttonDrawable != NULL) {
            Int32 w;
            buttonDrawable->GetIntrinsicWidth(&w);

            padding += w;
        }
    }
    return padding;
}

Int32 CompoundButton::GetCompoundPaddingRight()
{
    Int32 padding = Button::GetCompoundPaddingRight();
    Boolean isLayoutRtl;
    if (IsLayoutRtl(&isLayoutRtl), isLayoutRtl) {
        AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
        if (buttonDrawable != NULL) {

            Int32 w;
            buttonDrawable->GetIntrinsicWidth(&w);
            padding += w;
        }
    }
    return padding;
}

void CompoundButton::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
    if (buttonDrawable != NULL) {
        Int32 verticalGravity = GetGravity() & IGravity::VERTICAL_GRAVITY_MASK;
        Int32 drawableHeight;
        buttonDrawable->GetIntrinsicHeight(&drawableHeight);

        Int32 drawableWidth;
        buttonDrawable->GetIntrinsicWidth(&drawableWidth);

        Int32 top = 0;
        switch (verticalGravity) {
                case IGravity::BOTTOM:
                {
                    Int32 height;
                    GetHeight(&height);
                    top = height - drawableHeight;
                    break;
                }
                case IGravity::CENTER_VERTICAL:
                {
                    Int32 height;
                    GetHeight(&height);
                    top = (height - drawableHeight) / 2;
                    break;
                }
                default:
                    top = 0;
        }
        Int32 bottom = top + drawableHeight;
        Int32 left = 0;
        Int32 right = drawableWidth;
        Boolean isLayoutRtl;
        if (IsLayoutRtl(&isLayoutRtl), isLayoutRtl) {
            Int32 width;
            GetWidth(&width);
            left = width - drawableWidth;
            right = width;
        }

        buttonDrawable->SetBounds(left, top, right, bottom);
        AutoPtr<IDrawable> background;
        GetBackground((IDrawable**)&background);
        if (background != NULL) {
            background->SetHotspotBounds(left, top, right, bottom);
        }
    }

    Button::OnDraw(canvas);

    if (buttonDrawable != NULL) {
        Int32 scrollX = mScrollX;
        Int32 scrollY = mScrollY;
        if (scrollX == 0 && scrollY == 0) {
            buttonDrawable->Draw(canvas);
        } else {
            canvas->Translate((Float)scrollX, (Float)scrollY);
            buttonDrawable->Draw(canvas);
            canvas->Translate((Float)-scrollX, (Float)-scrollY);
        }
    }
}

ECode CompoundButton::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState);
    *drawableState = NULL;

    AutoPtr<ArrayOf<Int32> > ds;
    FAIL_RETURN(Button::OnCreateDrawableState(extraSpace + 1, (ArrayOf<Int32>**)&ds));
    if (IsChecked()) {
        MergeDrawableStates(ds, CHECKED_STATE_SET);
    }
    *drawableState = ds;
    REFCOUNT_ADD(*drawableState);
    return NOERROR;
}

ECode CompoundButton::DrawableStateChanged()
{
    FAIL_RETURN(Button::DrawableStateChanged());

    if (mButtonDrawable != NULL) {
        AutoPtr<ArrayOf<Int32> > myDrawableState;
        GetDrawableState((ArrayOf<Int32>**)&myDrawableState);

        Boolean state;
        // Set the state of the Drawable
        mButtonDrawable->SetState(myDrawableState, &state);

        Invalidate();
    }

    return NOERROR;
}

//@Override
ECode CompoundButton::DrawableHotspotChanged(
        /* [in] */ Float x,
        /* [in] */ Float y)
{
    Button::DrawableHotspotChanged(x, y);

    if (mButtonDrawable != NULL) {
        mButtonDrawable->SetHotspot(x, y);
    }
    return NOERROR;
}

Boolean CompoundButton::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return Button::VerifyDrawable(who) || who == mButtonDrawable.Get();
}

AutoPtr<IParcelable> CompoundButton::OnSaveInstanceState()
{
//    // Force our ancestor class to save its state
//    AutoPtr<IParcelable> superState = Button::OnSaveInstanceState();
//
//    CompoundButtonSavedState* ss = new CompoundButtonSavedState(superState);
//
//    ss->mChecked = IsChecked();
//    return ss;
    //zhangjingcheng ,wait CCompoundButtonSavedState
    return NULL;
}

void CompoundButton::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
//    SavedState ss = (SavedState) state;
//
//    Button::OnRestoreInstanceState(ss.getSuperState());
//    SetChecked(ss.checked);
//    RequestLayout();
//    zhangjingcheng, wait CCompoundButtonSavedState;

}

ECode CompoundButton::JumpDrawablesToCurrentState()
{
    FAIL_RETURN(Button::JumpDrawablesToCurrentState());
    if (mButtonDrawable != NULL)
        return mButtonDrawable->JumpToCurrentState();
    return NOERROR;
}


ECode CompoundButton::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 defStyleRes)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CompoundButton),
            ARRAY_SIZE(R::styleable::CompoundButton));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, defStyleRes, (ITypedArray**)&a));

    AutoPtr<IDrawable> d;
    a->GetDrawable(R::styleable::CompoundButton_button, (IDrawable**)&d);
    if (d != NULL) {
        SetButtonDrawable(d);
    }
    Boolean hasValue;
    if (a->HasValue(R::styleable::CompoundButton_buttonTintMode, &hasValue), hasValue) {
        Int32 mode;
        a->GetInt32(R::styleable::CompoundButton_buttonTintMode, -1, &mode);
        Drawable::ParseTintMode(mode, mButtonTintMode, &mButtonTintMode);
        mHasButtonTintMode = TRUE;
    }

    if (a->HasValue(R::styleable::CompoundButton_buttonTint, &hasValue), hasValue) {
        a->GetColorStateList(R::styleable::CompoundButton_buttonTint, (IColorStateList**)&mButtonTintList);
        mHasButtonTint = TRUE;
    }

    Boolean checked;
    a->GetBoolean(R::styleable::CompoundButton_checked, FALSE, &checked);
    SetChecked(checked);

    a->Recycle();

    ApplyButtonTint();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
