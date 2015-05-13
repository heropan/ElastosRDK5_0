
#include "widget/internal/ActionBarContextView.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewGroupMarginLayoutParams.h"
#include "view/animation/CDecelerateInterpolator.h"
#include "view/LayoutInflater.h"
#include "util/CDisplayMetrics.h"
#include "view/menu/CActionMenuPresenter.h"
#include "animation/CAnimatorSet.h"
#include "animation/CObjectAnimator.h"
#include "text/TextUtils.h"
#include <elastos/Math.h>

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::CObjectAnimator;
using Elastos::Droid::Animation::IAnimatorSet;
using Elastos::Droid::Animation::CAnimatorSet;
using Elastos::Droid::Animation::IAnimatorSetBuilder;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;
using Elastos::Droid::View::Animation::CDecelerateInterpolator;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::CViewGroupMarginLayoutParams;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::Menu::IActionMenuView;
using Elastos::Droid::View::Menu::CActionMenuPresenter;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

String ActionBarContextView::TAG = String("ActionBarContextView");

CAR_INTERFACE_IMPL(ActionBarContextView::CloseButtonListener, IViewOnClickListener)

ActionBarContextView::CloseButtonListener::CloseButtonListener(
    /* [in] */ IActionMode* mode) : mMode(mode)
{}

ECode ActionBarContextView::CloseButtonListener::OnClick(
    /* [in] */ IView* v)
{
    mMode->Finish();
    return NOERROR;
}

ActionBarContextView::ActionBarContextView()
    : mTitleStyleRes(0)
    , mSubtitleStyleRes(0)
    , mTitleOptional(FALSE)
    , mAnimateInOnLayout(FALSE)
    , mAnimationMode(0)
{
}

ECode ActionBarContextView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AbsActionBarView::Init(context, attrs, defStyle);

    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(
        const_cast<Int32*>(R::styleable::ActionMode),
        ARRAY_SIZE(R::styleable::ActionMode));
    context->ObtainStyledAttributesEx3(attrs, values, defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IDrawable> dr;
    a->GetDrawable(
        R::styleable::ActionMode_background, (IDrawable**)&dr);
    SetBackgroundDrawable(dr);
    a->GetResourceId(
        R::styleable::ActionMode_titleTextStyle, 0, &mTitleStyleRes);
    a->GetResourceId(
        R::styleable::ActionMode_subtitleTextStyle, 0, &mSubtitleStyleRes);

    a->GetLayoutDimensionEx(
        R::styleable::ActionMode_height, 0, &mContentHeight);

    a->GetDrawable(
        R::styleable::ActionMode_backgroundSplit, (IDrawable**)&mSplitBackground);

    a->Recycle();
    return NOERROR;
}

ECode ActionBarContextView::OnDetachedFromWindow()
{
    AbsActionBarView::OnDetachedFromWindow();
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        mActionMenuPresenter->HideOverflowMenu(&res);
        mActionMenuPresenter->HideSubMenus(&res);
    }
    return NOERROR;
}

ECode ActionBarContextView::SetSplitActionBar(
    /* [in] */ Boolean split)
{
    if (mSplitActionBar != split) {
        if (mActionMenuPresenter != NULL) {
            // Mode is already active; move everything over and adjust the menu itself.
            AutoPtr<IViewGroupLayoutParams> layoutParams;
            CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
                    IViewGroupLayoutParams::MATCH_PARENT, (IViewGroupLayoutParams**)&layoutParams);
            if (!split) {
                AutoPtr<IMenuView> view;
                mActionMenuPresenter->GetMenuView(THIS_PROBE(IViewGroup), (IMenuView**)&view);
                mMenuView = IActionMenuView::Probe(view);
                mMenuView->SetBackgroundDrawable(NULL);
                AutoPtr<IViewParent> oldParent;;
                mMenuView->GetParent((IViewParent**)&oldParent);
                IViewManager* vm = IViewManager::Probe(oldParent);
                if (vm != NULL) vm->RemoveView(mMenuView);
                AddView(mMenuView, layoutParams);
            } else {
                AutoPtr<IResources> rs;
                GetContext()->GetResources((IResources**)&rs);

                AutoPtr<IDisplayMetrics> dm;
                rs->GetDisplayMetrics((IDisplayMetrics**)&dm);
                Int32 widthPixels;
                dm->GetWidthPixels(&widthPixels);
                // Allow full screen width in split mode.

                mActionMenuPresenter->SetWidthLimit(widthPixels, TRUE);
                // No limit to the item count; use whatever will fit.
                mActionMenuPresenter->SetItemLimit(Elastos::Core::Math::INT32_MAX_VALUE);
                // Span the whole width
                layoutParams->SetWidth(IViewGroupLayoutParams::MATCH_PARENT);
                layoutParams->SetHeight(mContentHeight);
                AutoPtr<IMenuView> view;
                mActionMenuPresenter->GetMenuView(THIS_PROBE(IViewGroup), (IMenuView**)&view);
                mMenuView = IActionMenuView::Probe(view);
                mMenuView->SetBackgroundDrawable(mSplitBackground);
                AutoPtr<IViewParent> oldParent;
                mMenuView->GetParent((IViewParent**)&oldParent);
                IViewManager* vm = IViewManager::Probe(oldParent);
                if (vm != NULL) vm->RemoveView(mMenuView);
                mSplitView->AddViewEx3(mMenuView, layoutParams);
            }
        }
        AbsActionBarView::SetSplitActionBar(split);
    }

    return NOERROR;
}

ECode ActionBarContextView::SetContentHeight(
    /* [in] */ Int32 height)
{
    mContentHeight = height;

    return NOERROR;
}

ECode ActionBarContextView::SetCustomView(
    /* [in] */ IView* view)
{
    if (mCustomView != NULL) {
        RemoveView(mCustomView);
    }
    mCustomView = view;
    if (mTitleLayout != NULL) {
        RemoveView(mTitleLayout);
        mTitleLayout = NULL;
    }
    if (view != NULL) {
        AddView(view);
    }
    RequestLayout();

    return NOERROR;
}

ECode ActionBarContextView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    InitTitle();
    return NOERROR;
}

ECode ActionBarContextView::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    mSubtitle = subtitle;
    InitTitle();
    return NOERROR;
}

AutoPtr<ICharSequence> ActionBarContextView::GetTitle()
{
    return mTitle;
}

AutoPtr<ICharSequence> ActionBarContextView::GetSubtitle()
{
    return mSubtitle;
}

void ActionBarContextView::InitTitle()
{
    if (mTitleLayout == NULL) {
        AutoPtr<ILayoutInflater> inflater;
        LayoutInflater::From(GetContext(), (ILayoutInflater**)&inflater);
        assert(inflater != NULL);
        AutoPtr<IView> view;
        inflater->Inflate(R::layout::action_bar_title_item,
            THIS_PROBE(IViewGroup), (IView**)&view);

        mTitleLayout = ILinearLayout::Probe(GetChildAt(GetChildCount() - 1));
        mTitleLayout->FindViewById(R::id::action_bar_title, (IView**)&mTitleView);
        mTitleLayout->FindViewById(R::id::action_bar_subtitle, (IView**)&mSubtitleView);
        if (mTitleStyleRes != 0) {
            mTitleView->SetTextAppearance(mContext, mTitleStyleRes);
        }
        if (mSubtitleStyleRes != 0) {
            mSubtitleView->SetTextAppearance(mContext, mSubtitleStyleRes);
        }
    }

    mTitleView->SetText(mTitle);
    mSubtitleView->SetText(mSubtitle);

    Boolean hasTitle = !TextUtils::IsEmpty(mTitle);
    Boolean hasSubtitle = !TextUtils::IsEmpty(mSubtitle);
    mSubtitleView->SetVisibility(hasSubtitle ? IView::VISIBLE : IView::GONE);
    mTitleLayout->SetVisibility(hasTitle || hasSubtitle ? IView::VISIBLE : IView::GONE);

    AutoPtr<IViewParent> vp;
    mTitleLayout->GetParent((IViewParent**)&vp);
    if (vp == NULL) {
        AddView(mTitleLayout);
    }
}

ECode ActionBarContextView::InitForMode(
    /* [in] */ IActionMode* mode)
{
    AutoPtr<IViewParent> vp;
    if (mClose == NULL) {
        AutoPtr<ILayoutInflater> inflater;
        LayoutInflater::From(mContext, (ILayoutInflater**)&inflater);
        inflater->InflateEx2(R::layout::action_mode_close_item,
            THIS_PROBE(IViewGroup), FALSE, (IView**)&mClose);
        AddView(mClose);
    } else if ((mClose->GetParent((IViewParent**)&vp), vp) == NULL) {
        AddView(mClose);
    }

    AutoPtr<IView> closeButton;
    mClose->FindViewById(R::id::action_mode_close_button, (IView**)&closeButton);
    AutoPtr<IViewOnClickListener> l = new CloseButtonListener(mode);
    closeButton->SetOnClickListener(l);

    AutoPtr<IMenuBuilder> menu;
    mode->GetMenu((IMenu**)&menu);
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        mActionMenuPresenter->DismissPopupMenus(&res);
    }
    CActionMenuPresenter::New(mContext, (IActionMenuPresenter**)&mActionMenuPresenter);
    mActionMenuPresenter->SetReserveOverflow(TRUE);

    AutoPtr<IViewGroupLayoutParams> layoutParams;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::MATCH_PARENT, (IViewGroupLayoutParams**)&layoutParams);
    if (!mSplitActionBar) {

        menu->AddMenuPresenter(mActionMenuPresenter.Get());
        AutoPtr<IMenuView> view;
        mActionMenuPresenter->GetMenuView(THIS_PROBE(IViewGroup), (IMenuView**)&view);
        mMenuView = IActionMenuView::Probe(view);
        mMenuView->SetBackgroundDrawable(NULL);
        AddView(mMenuView, layoutParams);
    } else {
        AutoPtr<IResources> rs;
        GetContext()->GetResources((IResources**)&rs);

        AutoPtr<IDisplayMetrics> dm;
        rs->GetDisplayMetrics((IDisplayMetrics**)&dm);

        // Allow full screen width in split mode.
        mActionMenuPresenter->SetWidthLimit(
                ((CDisplayMetrics*)dm.Get())->mWidthPixels, TRUE);
        // No limit to the item count; use whatever will fit.
        mActionMenuPresenter->SetItemLimit(Elastos::Core::Math::INT32_MAX_VALUE);
        // Span the whole width
        layoutParams->SetWidth(IViewGroupLayoutParams::MATCH_PARENT);
        layoutParams->SetHeight(mContentHeight);
        menu->AddMenuPresenter(mActionMenuPresenter.Get());
        AutoPtr<IMenuView> view;
        mActionMenuPresenter->GetMenuView(THIS_PROBE(IViewGroup), (IMenuView**)&view);
        mMenuView = IActionMenuView::Probe(view);
        mMenuView->SetBackgroundDrawable(mSplitBackground);
        mSplitView->AddViewEx3(mMenuView, layoutParams);
    }

    mAnimateInOnLayout = TRUE;
    return NOERROR;
}

ECode ActionBarContextView::CloseMode()
{
    if (mAnimationMode == ANIMATE_OUT) {
        // Called again during close; just finish what we were doing.
        return NOERROR;
    }
    if (mClose == NULL) {
        KillMode();
        return NOERROR;
    }

    FinishAnimation();
    mAnimationMode = ANIMATE_OUT;
    mCurrentAnimation = MakeOutAnimation();
    mCurrentAnimation->Start();

    return NOERROR;
}

void ActionBarContextView::FinishAnimation()
{
    AutoPtr<IAnimator> a = mCurrentAnimation;
    if (a != NULL) {
        mCurrentAnimation = NULL;
        a->End();
    }
}

ECode ActionBarContextView::KillMode()
{
    FinishAnimation();
    RemoveAllViews();
    if (IViewManager::Probe(mSplitView) != NULL) {
        IViewManager::Probe(mSplitView)->RemoveView(mMenuView);
    }
    mCustomView = NULL;
    mMenuView = NULL;
    mAnimateInOnLayout = FALSE;

    return NOERROR;
}

Boolean ActionBarContextView::ShowOverflowMenu()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        return mActionMenuPresenter->ShowOverflowMenu(&res);
    }
    return FALSE;
}

Boolean ActionBarContextView::HideOverflowMenu()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        return mActionMenuPresenter->HideOverflowMenu(&res);
    }
    return FALSE;
}

Boolean ActionBarContextView::IsOverflowMenuShowing()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        return mActionMenuPresenter->IsOverflowMenuShowing(&res);
    }
    return FALSE;
}

AutoPtr<IViewGroupLayoutParams> ActionBarContextView::GenerateDefaultLayoutParams()
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CViewGroupMarginLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
        IViewGroupLayoutParams::WRAP_CONTENT, (IViewGroupMarginLayoutParams**)&lp);
    // Used by custom views if they don't supply layout params. Everything else
    // added to an ActionBarContextView should have them already.
    return lp;
}

ECode ActionBarContextView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IViewGroupMarginLayoutParams> lp;
    FAIL_RETURN(CViewGroupMarginLayoutParams::New(GetContext(), attrs, (IViewGroupMarginLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    INTERFACE_ADDREF(*params);
    return NOERROR;
}

void ActionBarContextView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    if (widthMode != MeasureSpec::EXACTLY) {
        /*throw new IllegalStateException(getClass().getSimpleName() + " can only be used " +
                "with android:layout_width=\"match_parent\" (or fill_parent)");*/
    }

    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    if (heightMode == MeasureSpec::UNSPECIFIED) {
        /*throw new IllegalStateException(getClass().getSimpleName() + " can only be used " +
                "with android:layout_height=\"wrap_content\"");*/
    }

    Int32 contentWidth = MeasureSpec::GetSize(widthMeasureSpec);

    Int32 maxHeight = mContentHeight > 0 ? mContentHeight : MeasureSpec::GetSize(heightMeasureSpec);

    Int32 verticalPadding = GetPaddingTop() + GetPaddingBottom();
    Int32 availableWidth = contentWidth - GetPaddingLeft() - GetPaddingRight();
    Int32 height = maxHeight - verticalPadding;
    Int32 childSpecHeight = MeasureSpec::MakeMeasureSpec(height, MeasureSpec::AT_MOST);

    if (mClose != NULL) {
        availableWidth = MeasureChildView(mClose, availableWidth, childSpecHeight, 0);
        AutoPtr<IViewGroupMarginLayoutParams> lp;
        mClose->GetLayoutParams((IViewGroupLayoutParams**)&lp);

        availableWidth -= ((CViewGroupMarginLayoutParams*)lp.Get())->mLeftMargin +
            ((CViewGroupMarginLayoutParams*)lp.Get())->mRightMargin;
    }

    AutoPtr<IViewParent> vp;
    if (mMenuView != NULL && (mMenuView->GetParent((IViewParent**)&vp), vp.Get()) == THIS_PROBE(IViewParent)) {
        availableWidth = MeasureChildView(mMenuView, availableWidth,
                childSpecHeight, 0);
    }

    if (mTitleLayout != NULL && mCustomView == NULL) {
        if (mTitleOptional) {
            Int32 titleWidthSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
            mTitleLayout->Measure(titleWidthSpec, childSpecHeight);
            Int32 titleWidth;
            mTitleLayout->GetMeasuredWidth(&titleWidth);

            Boolean titleFits = titleWidth <= availableWidth;
            if (titleFits) {
                availableWidth -= titleWidth;
            }
            mTitleLayout->SetVisibility(titleFits ? IView::VISIBLE : IView::GONE);
        } else {
            availableWidth = MeasureChildView(mTitleLayout, availableWidth, childSpecHeight, 0);
        }
    }

    if (mCustomView != NULL) {
        AutoPtr<IViewGroupLayoutParams> lp;
        mCustomView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        Int32 w;
        lp->GetWidth(&w);
        Int32 customWidthMode = w != IViewGroupLayoutParams::WRAP_CONTENT ?
                MeasureSpec::EXACTLY : MeasureSpec::AT_MOST;
        Int32 customWidth = w >= 0 ? Elastos::Core::Math::Min(w, availableWidth) : availableWidth;
        Int32 h;
        lp->GetHeight(&h);
        Int32 customHeightMode = h != IViewGroupLayoutParams::WRAP_CONTENT ?
                MeasureSpec::EXACTLY : MeasureSpec::AT_MOST;
        Int32 customHeight = h >= 0 ? Elastos::Core::Math::Min(h, height) : height;
        mCustomView->Measure(MeasureSpec::MakeMeasureSpec(customWidth, customWidthMode),
            MeasureSpec::MakeMeasureSpec(customHeight, customHeightMode));
    }

    if (mContentHeight <= 0) {
        Int32 measuredHeight = 0;
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> v = GetChildAt(i);
            Int32 paddedViewHeight;
            v->GetMeasuredHeight(&paddedViewHeight);

            paddedViewHeight += verticalPadding;

            if (paddedViewHeight > measuredHeight) {
                measuredHeight = paddedViewHeight;
            }
        }
        SetMeasuredDimension(contentWidth, measuredHeight);
    } else {
        SetMeasuredDimension(contentWidth, maxHeight);
    }
}

AutoPtr<IAnimator> ActionBarContextView::MakeInAnimation()
{
    assert(mClose != NULL);
    Int32 width = 0;
    mClose->GetWidth(&width);

    AutoPtr<IViewGroupMarginLayoutParams> layoutParams;
    mClose->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
    Int32 leftMargin;
    layoutParams->GetLeftMargin(&leftMargin);
    mClose->SetTranslationX(-width - leftMargin);
    AutoPtr<ArrayOf<Float> > tmpValues = ArrayOf<Float>::Alloc(1);
    (*tmpValues)[0] = 0.f;
    AutoPtr<IObjectAnimator> buttonAnimator = CObjectAnimator::OfFloat(mClose, String("translationX"), tmpValues);
    buttonAnimator->SetDuration(200);
    buttonAnimator->AddListener(THIS_PROBE(IAnimatorListener));

    AutoPtr<IDecelerateInterpolator> decele;
    CDecelerateInterpolator::New((IDecelerateInterpolator**)&decele);
    buttonAnimator->SetInterpolator(decele);

    AutoPtr<IAnimatorSet> set;
    CAnimatorSet::New((IAnimatorSet**)&set);
    AutoPtr<IAnimatorSetBuilder> b;
    set->Play(buttonAnimator, (IAnimatorSetBuilder**)&b);

    if (mMenuView != NULL) {
        Int32 count = 0;
        mMenuView->GetChildCount(&count);
        if (count > 0) {
            for (Int32 i = count - 1, j = 0; i >= 0; i--, j++) {
                AutoPtr<IView> child;
                mMenuView->GetChildAt(i, (IView**)&child);
                child->SetScaleY(0);

                AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(2);
                (*values)[0] = 0.f;
                (*values)[1] = 1.f;
                AutoPtr<IObjectAnimator> a = CObjectAnimator::OfFloat(child, String("scaleY"), values);
                a->SetDuration(300);
                b->With(a);
            }
        }
    }
    return set;
}

AutoPtr<IAnimator> ActionBarContextView::MakeOutAnimation()
{
    Int32 width = 0;
    mClose->GetWidth(&width);
    AutoPtr<IViewGroupMarginLayoutParams> layoutParams;
    mClose->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
    Int32 leftMargin;
    layoutParams->GetLeftMargin(&leftMargin);
    AutoPtr<ArrayOf<Float> > tmpValues = ArrayOf<Float>::Alloc(1);
    (*tmpValues)[0] = -width - leftMargin;
    AutoPtr<IObjectAnimator> buttonAnimator = CObjectAnimator::OfFloat(mClose, String("translationX"), tmpValues);
    buttonAnimator->SetDuration(200);
    buttonAnimator->AddListener(THIS_PROBE(IAnimatorListener));
    AutoPtr<IDecelerateInterpolator> decele;
    CDecelerateInterpolator::New((IDecelerateInterpolator**)&decele);
    buttonAnimator->SetInterpolator(decele);

    AutoPtr<IAnimatorSet> set;
    CAnimatorSet::New((IAnimatorSet**)&set);
    AutoPtr<IAnimatorSetBuilder> b;
    set->Play(buttonAnimator, (IAnimatorSetBuilder**)&b);

    if (mMenuView != NULL) {
        Int32 count;
        mMenuView->GetChildCount(&count);
        if (count > 0) {
            for (Int32 i = 0; i < 0; i++) {
                AutoPtr<IView> child;
                mMenuView->GetChildAt(i, (IView**)&child);
                child->SetScaleY(0);

                AutoPtr<ArrayOf<Float> > tmpValues = ArrayOf<Float>::Alloc(1);
                (*tmpValues)[0] = 0.f;
                AutoPtr<IObjectAnimator> a = CObjectAnimator::OfFloat(child, String("scaleY"), tmpValues);
                a->SetDuration(300);
                b->With(a);
            }
        }
    }
    return set;
}

void ActionBarContextView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Boolean isLayoutRtl = IsLayoutRtl();
    Int32 x = isLayoutRtl ? r - l - GetPaddingRight() : GetPaddingLeft();
    Int32 y = GetPaddingTop();
    Int32 contentHeight = b - t - GetPaddingTop() - GetPaddingBottom();

    Int32 visible = 0;
    if (mClose != NULL && (mClose->GetVisibility(&visible), visible) != IView::GONE) {
        AutoPtr<IViewGroupMarginLayoutParams> lp;
        mClose->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        Int32 leftMargin;
        Int32 rightMargin;
        lp->GetLeftMargin(&leftMargin);
        lp->GetRightMargin(&rightMargin);
        Int32 startMargin = isLayoutRtl ? rightMargin : leftMargin;
        Int32 endMargin = isLayoutRtl ? leftMargin : rightMargin;
        x = Next(x, startMargin, isLayoutRtl);
        x += PositionChild(mClose, x, y, contentHeight, isLayoutRtl);
        x = Next(x, endMargin, isLayoutRtl);

        if (mAnimateInOnLayout) {
            mAnimationMode = ANIMATE_IN;
            mCurrentAnimation = MakeInAnimation();
            mCurrentAnimation->Start();
            mAnimateInOnLayout = FALSE;
        }
    }


    if (mTitleLayout != NULL && mCustomView == NULL && (mTitleLayout->GetVisibility(&visible), visible) != IView::GONE) {
        x += PositionChild(mTitleLayout, x, y, contentHeight, isLayoutRtl);
    }

    if (mCustomView != NULL) {
        x += PositionChild(mCustomView, x, y, contentHeight, isLayoutRtl);
    }

    x = isLayoutRtl ? GetPaddingLeft() : r - l - GetPaddingRight();

    if (mMenuView != NULL) {
        x += PositionChild(mMenuView, x, y, contentHeight, !isLayoutRtl);
    }
}

ECode ActionBarContextView::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode ActionBarContextView::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mAnimationMode == ANIMATE_OUT) {
        KillMode();
    }
    mAnimationMode = ANIMATE_IDLE;
    return NOERROR;
}

ECode ActionBarContextView::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode ActionBarContextView::OnAnimationRepeat(
    /* [in] */ IAnimator* animation) {
    return NOERROR;
}

Boolean ActionBarContextView::ShouldDelayChildPressedState()
{
    return FALSE;
}

ECode ActionBarContextView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    assert(event != NULL);
    Int32 type = 0;
    event->GetEventType(&type);
    if (type == IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED) {
        // Action mode started
        event->SetSource((IView*)this->Probe(EIID_IView));
        AutoPtr<ICharSequence> name;
        CStringWrapper::New(String("CActionBarContextView"), (ICharSequence**)&name);
        event->SetClassName(name);

        String pkgName;
        GetContext()->GetPackageName(&pkgName);
        name->Release();
        CStringWrapper::New(pkgName, (ICharSequence**)&name);
        event->SetPackageName(name);
        event->SetContentDescription(mTitle);
    } else {
        AbsActionBarView::OnInitializeAccessibilityEvent(event);
    }

    return NOERROR;
}

ECode ActionBarContextView::SetTitleOptional(
    /* [in] */ Boolean titleOptional)
{
    if (titleOptional != mTitleOptional) {
        RequestLayout();
    }
    mTitleOptional = titleOptional;
    return NOERROR;
}

Boolean ActionBarContextView::IsTitleOptional()
{
    return mTitleOptional;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
