
#include "elastos/droid/widget/CheckedTextView.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::IGravity;

namespace Elastos {
namespace Droid {
namespace Widget {

AutoPtr<ArrayOf<Int32> > CheckedTextView::CHECKED_STATE_SET = ArrayOf<Int32>::Alloc(1);

CheckedTextView::CheckedTextView()
    : mChecked(FALSE)
    , mCheckMarkResource(0)
    , mBasePadding(0)
    , mCheckMarkWidth(0)
    , mNeedRequestlayout(0)
{
    (*CHECKED_STATE_SET)[0] = R::attr::state_checked;
}

CheckedTextView::CheckedTextView(
    /* [in] */ IContext* context)
    : TextView(context)
    , mChecked(FALSE)
    , mCheckMarkResource(0)
    , mBasePadding(0)
    , mCheckMarkWidth(0)
    , mNeedRequestlayout(0)
{
    (*CHECKED_STATE_SET)[0] = R::attr::state_checked;
    Init(context);
}

CheckedTextView::CheckedTextView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : TextView(context, attrs)
    , mChecked(FALSE)
    , mCheckMarkResource(0)
    , mBasePadding(0)
    , mCheckMarkWidth(0)
    , mNeedRequestlayout(0)
{
    (*CHECKED_STATE_SET)[0] = R::attr::state_checked;
    Init(context, attrs, R::attr::checkedTextViewStyle);
}

CheckedTextView::CheckedTextView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : TextView(context, attrs, defStyle)
    , mChecked(FALSE)
    , mCheckMarkResource(0)
    , mBasePadding(0)
    , mCheckMarkWidth(0)
    , mNeedRequestlayout(0)
{
    Init(context, attrs, defStyle);
}

ECode CheckedTextView::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

ECode CheckedTextView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, R::attr::checkedTextViewStyle);
}

ECode CheckedTextView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    TextView::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CheckedTextView),
            ARRAY_SIZE(R::styleable::CheckedTextView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IDrawable> d;
    a->GetDrawable(R::styleable::CheckedTextView_checkMark, (IDrawable**)&d);
    if (d != NULL) {
        SetCheckMarkDrawable(d);
    }

    Boolean checked;
    a->GetBoolean(R::styleable::CheckedTextView_checked, FALSE, &checked);
    SetChecked(checked);

    a->Recycle();

    return NOERROR;
}

ECode CheckedTextView::Toggle()
{
    SetChecked(!mChecked);

    return NOERROR;
}

Boolean CheckedTextView::IsChecked()
{
    return mChecked;
}

/**
 * <p>Changes the checked state of this text view.</p>
 *
 * @param checked true to check the text, FALSE to uncheck it
 */
ECode CheckedTextView::SetChecked(
    /* [in] */ Boolean checked)
{
    if (mChecked != checked) {
        mChecked = checked;
        RefreshDrawableState();
        NotifyAccessibilityStateChanged();
    }

    return NOERROR;
}


/**
 * Set the checkmark to a given Drawable, identified by its resourece id. This will be drawn
 * when {@link #isChecked()} is true.
 *
 * @param resid The Drawable to use for the checkmark.
 */
ECode CheckedTextView::SetCheckMarkDrawable(
    /* [in] */ Int32 resid)
{
    if (resid != 0 && resid == mCheckMarkResource) {
        return NOERROR;
    }

    mCheckMarkResource = resid;

    AutoPtr<IDrawable> d;
    if (mCheckMarkResource != 0) {
        GetResources()->GetDrawable(mCheckMarkResource, (IDrawable**)&d);
    }
    SetCheckMarkDrawable(d);

    return NOERROR;
}

/**
 * Set the checkmark to a given Drawable. This will be drawn when {@link #isChecked()} is true.
 *
 * @param d The Drawable to use for the checkmark.
 */
ECode CheckedTextView::SetCheckMarkDrawable(
    /* [in] */ IDrawable* d)
{
    if (mCheckMarkDrawable != NULL) {
        mCheckMarkDrawable->SetCallback(NULL);
        UnscheduleDrawable(mCheckMarkDrawable);
    }
    mNeedRequestlayout = (d != mCheckMarkDrawable);
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));

        Boolean res;
        d->SetVisible(GetVisibility() == IView::VISIBLE, FALSE, &res);

        d->SetState(CHECKED_STATE_SET, &res);

        Int32 height;
        d->GetIntrinsicHeight(&height);
        SetMinHeight(height);

        d->GetIntrinsicWidth(&mCheckMarkWidth);

        AutoPtr<ArrayOf<Int32> > drawableState = GetDrawableState();
        d->SetState(drawableState, &res);
    } else {
        mCheckMarkWidth = 0;
    }
    mCheckMarkDrawable = d;
    // Do padding resolution. This will call internalSetPadding() and do a requestLayout() if needed.
    ResolvePadding();

    return NOERROR;
}

/**
 * Gets the checkmark drawable
 *
 * @return The drawable use to represent the checkmark, if any.
 *
 * @see #setCheckMarkDrawable(Drawable)
 * @see #setCheckMarkDrawable(int)
 *
 * @attr ref android.R.styleable#CheckedTextView_checkMark
 */
AutoPtr<IDrawable> CheckedTextView::GetCheckMarkDrawable()
{
    return mCheckMarkDrawable;
}

/**
 * @hide
 */
//@Override
void CheckedTextView::InternalSetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    TextView::InternalSetPadding(left, top, right, bottom);
    SetBasePadding(IsLayoutRtl());
}

//@Override
ECode CheckedTextView::OnRtlPropertiesChanged(
    /* [in] */ Int32 layoutDirection)
{
    TextView::OnRtlPropertiesChanged(layoutDirection);
    UpdatePadding();
    return NOERROR;
}

void CheckedTextView::UpdatePadding()
{
    ResetPaddingToInitialValues();
    Int32 newPadding = (mCheckMarkDrawable != NULL) ?
            mCheckMarkWidth + mBasePadding : mBasePadding;
    if (IsLayoutRtl()) {
        mNeedRequestlayout |= (mPaddingLeft != newPadding);
        mPaddingLeft = newPadding;
    } else {
        mNeedRequestlayout |= (mPaddingRight != newPadding);
        mPaddingRight = newPadding;
    }
    if (mNeedRequestlayout) {
        RequestLayout();
        mNeedRequestlayout = FALSE;
    }
}

void CheckedTextView::SetBasePadding(
    /* [in] */ Boolean isLayoutRtl)
{
    if (isLayoutRtl) {
        mBasePadding = mPaddingLeft;
    } else {
        mBasePadding = mPaddingRight;
    }
}

void CheckedTextView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    TextView::OnDraw(canvas);

    AutoPtr<IDrawable> checkMarkDrawable = mCheckMarkDrawable;
    if (checkMarkDrawable != NULL) {
        Int32 verticalGravity = GetGravity() & IGravity::VERTICAL_GRAVITY_MASK;
        Int32 height;
        checkMarkDrawable->GetIntrinsicHeight(&height);

        Int32 y = 0;

        switch (verticalGravity) {
            case IGravity::BOTTOM:
                y = GetHeight() - height;
                break;
            case IGravity::CENTER_VERTICAL:
                y = (GetHeight() - height) / 2;
                break;
        }

        Boolean isLayoutRtl = IsLayoutRtl();
        Int32 width = GetWidth();
        Int32 top = y;
        Int32 bottom = top + height;
        Int32 left;
        Int32 right;
        if (isLayoutRtl) {
            left = mBasePadding;
            right = left + mCheckMarkWidth;
        } else {
            right = width - mBasePadding;
            left = right - mCheckMarkWidth;
        }
        checkMarkDrawable->SetBounds( left, top, right, bottom);

        checkMarkDrawable->Draw(canvas);
    }
}

ECode CheckedTextView::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState);
    *drawableState = NULL;

    AutoPtr<ArrayOf<Int32> > ds;
    TextView::OnCreateDrawableState(extraSpace + 1, (ArrayOf<Int32>**)&ds);
    if (IsChecked()) {
        MergeDrawableStates(ds, CHECKED_STATE_SET);
    }
    *drawableState = ds;
    REFCOUNT_ADD(*drawableState);
    return NOERROR;
}

ECode CheckedTextView::DrawableStateChanged()
{
    TextView::DrawableStateChanged();

    if (mCheckMarkDrawable != NULL) {
        AutoPtr<ArrayOf<Int32> > myDrawableState = GetDrawableState();

        Boolean res;
        // Set the state of the Drawable
        mCheckMarkDrawable->SetState(myDrawableState, &res);

        Invalidate();
    }

    return NOERROR;
}

ECode CheckedTextView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnInitializeAccessibilityEvent(event);
    String classNameStr("CheckedTextView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    event->SetClassName(className);

    return NOERROR;
}

ECode CheckedTextView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    String classNameStr("CheckedTextView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    info->SetClassName(className);
    info->SetCheckable(TRUE);
    info->SetChecked(mChecked);

    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
