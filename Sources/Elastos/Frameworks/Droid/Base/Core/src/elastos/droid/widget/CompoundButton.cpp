
#include "elastos/droid/widget/CompoundButton.h"
#include "elastos/droid/widget/CCompoundButtonSavedState.h"
#include "elastos/droid/graphics/drawable/Drawable.h"
#include <elastos/core/CoreUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Droid::View::IGravity;
using Elastos::Core::CoreUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Widget {

static AutoPtr<ArrayOf<Int32> > Init_CHECKED_STATE_SET()
{
    AutoPtr<ArrayOf<Int32> > temp = ArrayOf<Int32>::Alloc(1);
    (*temp)[0] = R::attr::state_checked;
    return temp;
}

AutoPtr<ArrayOf<Int32> > CompoundButton::CHECKED_STATE_SET = Init_CHECKED_STATE_SET();

//==============================================================================
//          CompoundButton::SavedState
//==============================================================================

CAR_INTERFACE_IMPL(CompoundButton::SavedState, View::BaseSavedState, ICompoundButtonSavedState);

CompoundButton::SavedState::SavedState()
    : mSavedStateChecked(FALSE)
{}

CompoundButton::SavedState::~SavedState()
{}

ECode CompoundButton::SavedState::constructor()
{
    return View::BaseSavedState::constructor();
}

ECode CompoundButton::SavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return View::BaseSavedState::constructor(superState);
}

ECode CompoundButton::SavedState::WriteToParcel(
    /* [in] */ IParcel* out)
{
    FAIL_RETURN(View::BaseSavedState::WriteToParcel(out));
    out->WriteBoolean(mSavedStateChecked);
    return NOERROR;
}

ECode CompoundButton::SavedState::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    FAIL_RETURN(View::BaseSavedState::ReadFromParcel(in));
    in->ReadBoolean(&mSavedStateChecked);
    return NOERROR;
}

ECode CompoundButton::SavedState::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder buider;
    buider += "CompoundButton.SavedState{";

    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 value;
    sys->IdentityHashCode((ICompoundButton*)this, &value);
    buider += StringUtils::ToHexString(value);
    buider += " checked=";
    buider += mSavedStateChecked;
    buider += "}";

    *str = buider.ToString();
    return NOERROR;
}

//==============================================================================
//          CompoundButton
//==============================================================================

CAR_INTERFACE_IMPL_2(CompoundButton, Button, ICompoundButton, ICheckable)

CompoundButton::CompoundButton()
    : mChecked(FALSE)
    , mButtonResource(0)
    , mBroadcasting(FALSE)
    , mButtonTintMode(NULL)
    , mHasButtonTint(FALSE)
    , mHasButtonTintMode(FALSE)
{}

ECode CompoundButton::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CompoundButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CompoundButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode CompoundButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    Button::constructor(context, attrs, defStyleAttr, defStyleRes);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CompoundButton),
            ArraySize(R::styleable::CompoundButton));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a));

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

ECode CompoundButton::Toggle()
{
    return SetChecked(!mChecked);
}

ECode CompoundButton::PerformClick(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    /*
     * XXX: These are tiny, need some surrounding 'expanded touch area',
     * which will need to be implemented in Button if we only override
     * performClick()
     */

    /* When clicked, toggle the state */

    Toggle();
    return Button::PerformClick(res);
}

ECode CompoundButton::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    VALIDATE_NOT_NULL(isChecked);
    *isChecked = mChecked;
    return NOERROR;
}

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

ECode CompoundButton::SetOnCheckedChangeListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    mOnCheckedChangeListener = listener;

    return NOERROR;
}

ECode CompoundButton::SetOnCheckedChangeWidgetListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    mOnCheckedChangeWidgetListener = listener;

    return NOERROR;
}

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

ECode CompoundButton::SetButtonDrawable(
    /* [in] */ IDrawable* d)
{
    if ((IDrawable*)mButtonDrawable.Get() != d) {
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

ECode CompoundButton::SetButtonTintList(
    /* [in] */ IColorStateList* tint)
{
    mButtonTintList = tint;
    mHasButtonTint = TRUE;

    ApplyButtonTint();
    return NOERROR;
}

ECode CompoundButton::GetButtonTintList(
    /* [out] */ IColorStateList** tint)
{
    VALIDATE_NOT_NULL(tint)
    *tint = mButtonTintList;
    return NOERROR;
}

ECode CompoundButton::SetButtonTintMode(
    /* [in] */ Elastos::Droid::Graphics::PorterDuffMode tintMode)
{
    mButtonTintMode = tintMode;
    mHasButtonTintMode = TRUE;

    ApplyButtonTint();
    return NOERROR;
}

ECode CompoundButton::GetButtonTintMode(
    /* [out] */ Elastos::Droid::Graphics::PorterDuffMode* tintMode)
{
    VALIDATE_NOT_NULL(tintMode);
    *tintMode = mButtonTintMode;
    return NOERROR;
}

void CompoundButton::ApplyButtonTint()
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
}

ECode CompoundButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Button::OnInitializeAccessibilityEvent(event);

    return IAccessibilityRecord::Probe(event)->SetClassName(CoreUtils::Convert("CompoundButton"));
}

ECode CompoundButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FAIL_RETURN(Button::OnInitializeAccessibilityNodeInfo(info));
    FAIL_RETURN(info->SetClassName(CoreUtils::Convert("CompoundButton")));
    info->SetCheckable(TRUE);
    info->SetChecked(mChecked);
    return NOERROR;
}

ECode CompoundButton::GetCompoundPaddingLeft(
    /* [out] */ Int32* left)
{
    VALIDATE_NOT_NULL(left);

    Int32 padding;
    Button::GetCompoundPaddingLeft(&padding);

    Boolean isLayoutRtl;
    if (IsLayoutRtl(&isLayoutRtl), !isLayoutRtl) {
        AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
        if (buttonDrawable != NULL) {
            Int32 w;
            buttonDrawable->GetIntrinsicWidth(&w);

            padding += w;
        }
    }
    *left = padding;
    return NOERROR;
}

ECode CompoundButton::GetCompoundPaddingRight(
    /* [out] */ Int32* right)
{
    VALIDATE_NOT_NULL(right);

    Int32 padding;
    Button::GetCompoundPaddingRight(&padding);

    Boolean isLayoutRtl;
    if (IsLayoutRtl(&isLayoutRtl), isLayoutRtl) {
        AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
        if (buttonDrawable != NULL) {

            Int32 w;
            buttonDrawable->GetIntrinsicWidth(&w);
            padding += w;
        }
    }
    *right = padding;
    return NOERROR;
}

ECode CompoundButton::GetHorizontalOffsetForDrawables(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = 0;

    AutoPtr<IDrawable> buttonDrawable = mButtonDrawable;
    if(buttonDrawable != NULL) {
        Int32 width;
        buttonDrawable->GetIntrinsicWidth(&width);
        *offset = width;
        return NOERROR;
    }
    else {
        return NOERROR;
    }
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
                case IGravity::BOTTOM: {
                    Int32 height;
                    GetHeight(&height);
                    top = height - drawableHeight;
                    break;
                }
                case IGravity::CENTER_VERTICAL: {
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
        }
        else {
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
    Boolean res;
    if (IsChecked(&res), res) {
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

ECode CompoundButton::JumpDrawablesToCurrentState()
{
    FAIL_RETURN(Button::JumpDrawablesToCurrentState());
    if (mButtonDrawable != NULL) {
        return mButtonDrawable->JumpToCurrentState();
    }

    return NOERROR;
}

AutoPtr<IParcelable> CompoundButton::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = Button::OnSaveInstanceState();

    AutoPtr<CCompoundButtonSavedState> ss;
    ASSERT_SUCCEEDED(CCompoundButtonSavedState::NewByFriend(
            superState, (CCompoundButtonSavedState**)&ss));

    IsChecked(&(ss->mSavedStateChecked));

   return ss;
}

void CompoundButton::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    AutoPtr<CCompoundButtonSavedState> ss =
            (CCompoundButtonSavedState*)ICompoundButtonSavedState::Probe(state);
    if (!ss) return;

    AutoPtr<IParcelable> superState;
    ss->GetSuperState((IParcelable**)&superState);

    Button::OnRestoreInstanceState(superState);
    SetChecked(ss->mSavedStateChecked);
    RequestLayout();
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
