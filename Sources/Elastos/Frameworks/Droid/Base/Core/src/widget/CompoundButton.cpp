
#include "widget/CompoundButton.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::IGravity;

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

CompoundButton::CompoundButtonSavedState::CompoundButtonSavedState()
    : ViewBaseSavedState()
    , mChecked(FALSE)
{
}

CompoundButton::CompoundButtonSavedState::CompoundButtonSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mChecked(FALSE)
{
}


CompoundButton::CompoundButtonSavedState::~CompoundButtonSavedState()
{}

ECode CompoundButton::CompoundButtonSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(ViewBaseSavedState::WriteToParcel(dest));
    dest->WriteBoolean(mChecked);
    return NOERROR;
}

ECode CompoundButton::CompoundButtonSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(ViewBaseSavedState::ReadFromParcel(source));
    source->ReadBoolean(&mChecked);
    return NOERROR;
}

ECode CompoundButton::CompoundButtonSavedState::Init()
{
    return ViewBaseSavedState::Init();
}

ECode CompoundButton::CompoundButtonSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

CompoundButton::CompoundButton()
    : mChecked(FALSE)
    , mButtonResource(0)
    , mBroadcasting(FALSE)
{}

CompoundButton::CompoundButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : Button(context, attrs, defStyle)
    , mChecked(FALSE)
    , mButtonResource(0)
    , mBroadcasting(FALSE)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
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
    return Button::PerformClick();
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

        NotifyAccessibilityStateChanged();

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
        GetResources()->GetDrawable(mButtonResource, (IDrawable**)&d);
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
    if (d != NULL) {
        if (mButtonDrawable != NULL) {
            mButtonDrawable->SetCallback(NULL);
            UnscheduleDrawable(mButtonDrawable);
        }
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        Boolean stateful;
        AutoPtr<ArrayOf<Int32> > drawableState = GetDrawableState();
        d->SetState(drawableState, &stateful);
        d->SetVisible(GetVisibility() == IView::VISIBLE, FALSE, &stateful);
        mButtonDrawable = d;
        mButtonDrawable->SetState(NULL, &stateful);
        Int32 height;
        mButtonDrawable->GetIntrinsicHeight(&height);
        SetMinHeight(height);
    }

    RefreshDrawableState();

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

AutoPtr<IDrawable> CompoundButton::GetButtonDrawable()
{
    return mButtonDrawable;
}

ECode CompoundButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Button::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> txt;
    CStringWrapper::New(String("CompoundButton"), (ICharSequence**)&txt);
    return event->SetClassName(txt);
}

ECode CompoundButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FAIL_RETURN(Button::OnInitializeAccessibilityNodeInfo(info));
    AutoPtr<ICharSequence> txt;
    CStringWrapper::New(String("CompoundButton"), (ICharSequence**)&txt);
    FAIL_RETURN(info->SetClassName(txt));
    info->SetCheckable(TRUE);
    info->SetChecked(mChecked);
    return NOERROR;
}

Int32 CompoundButton::GetCompoundPaddingLeft()
{
    Int32 padding = Button::GetCompoundPaddingLeft();
    if (!IsLayoutRtl()) {
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
    if (IsLayoutRtl()) {
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
    Button::OnDraw(canvas);

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
                    top = GetHeight() - drawableHeight;
                    break;
                case IGravity::CENTER_VERTICAL:
                    top = (GetHeight() - drawableHeight) / 2;
                    break;
        }
        Int32 bottom = top + drawableHeight;
        Int32 left = IsLayoutRtl() ? GetWidth() - drawableWidth : 0;
        Int32 right = IsLayoutRtl() ? GetWidth() : drawableWidth;

        buttonDrawable->SetBounds(left, top, right, bottom);
        buttonDrawable->Draw(canvas);
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
        AutoPtr<ArrayOf<Int32> > myDrawableState = GetDrawableState();

        Boolean state;
        // Set the state of the Drawable
        mButtonDrawable->SetState(myDrawableState, &state);

        Invalidate();
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
//    SetFreezesText(TRUE);
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

ECode CompoundButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(Button::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle);
}

ECode CompoundButton::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CompoundButton),
            ARRAY_SIZE(R::styleable::CompoundButton));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    AutoPtr<IDrawable> d;
    a->GetDrawable(R::styleable::CompoundButton_button, (IDrawable**)&d);
    if (d != NULL) {
        SetButtonDrawable(d);
    }

    Boolean checked;
    a->GetBoolean(R::styleable::CompoundButton_checked, FALSE, &checked);
    SetChecked(checked);

    a->Recycle();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
