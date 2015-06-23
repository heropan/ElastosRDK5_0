
#include "internal/ScrollingTabContainerView.h"
#include <elastos/core/Math.h>
#include "view/menu/CActionBarPolicy.h"
#include "view/CViewGroupLayoutParams.h"
#include "widget/CAbsListViewLayoutParams.h"
#include "text/TextUtils.h"
#include "view/animation/CDecelerateInterpolator.h"
#include "widget/CLinearLayout.h"
#include "widget/CSpinner.h"
#include "animation/CObjectAnimator.h"
#include "widget/Spinner.h"
#include "widget/CImageView.h"
#include "widget/CTextView.h"
#include "widget/Toast.h"

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::TextUtilsTruncateAt;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::Menu::IActionBarPolicy;
using Elastos::Droid::View::Menu::CActionBarPolicy;
using Elastos::Droid::Widget::IAbsListViewLayoutParams;
using Elastos::Droid::Widget::CAbsListViewLayoutParams;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;
using Elastos::Droid::View::Animation::CDecelerateInterpolator;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::CObjectAnimator;
using Elastos::Droid::Text::TextUtilsTruncateAt_END;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

static AutoPtr<ITimeInterpolator> InitIpt()
{
    AutoPtr<CDecelerateInterpolator> ipt;
    CDecelerateInterpolator::NewByFriend((CDecelerateInterpolator**)&ipt);
    return ipt;
}

const String ScrollingTabContainerView::TAG("ScrollingTabContainerView");
AutoPtr<ITimeInterpolator> ScrollingTabContainerView::sAlphaInterpolator = InitIpt();
const Int32 ScrollingTabContainerView::FADE_DURATION = 200;

ScrollingTabContainerView::AnimateToTabRunnable::AnimateToTabRunnable(
    /* [in] */ IView* tabView,
    /* [in] */ ScrollingTabContainerView* host)
{
    mTabView = tabView;
    mHost = host;
}

ECode ScrollingTabContainerView::AnimateToTabRunnable::Run()
{
    Int32 tLeft, width, tWidth;
    mTabView->GetLeft(&tLeft);
    width = mHost->GetWidth();
    mTabView->GetWidth(&tWidth);
    Int32 scrollPos = tLeft - (width - tWidth) / 2;
    mHost->SmoothScrollTo(scrollPos, 0);
    mHost->mTabSelector = NULL;
    return NOERROR;
}

ScrollingTabContainerView::ScrollingTabContainerView()
        : mAllowCollapse(FALSE)
        , mMaxTabWidth(0)
        , mStackedTabMaxWidth(0)
        , mContentHeight(0)
        , mSelectedTabIndex(0)
{
    mVisAnimListener = new VisibilityAnimListener(this);
}

ScrollingTabContainerView::ScrollingTabContainerView(
    /* [in] */ IContext* context) : HorizontalScrollView(context)
{
    mVisAnimListener = new VisibilityAnimListener(this);
    ASSERT_SUCCEEDED(InitInternal(context));
}

ECode ScrollingTabContainerView::Init(
    /* [in] */ IContext* context)
{
    HorizontalScrollView::Init(context);
    return InitInternal(context);
}

ECode ScrollingTabContainerView::InitInternal(
    /* [in] */ IContext* context)
{
    SetHorizontalScrollBarEnabled(FALSE);

    AutoPtr<IActionBarPolicy> abp;
    CActionBarPolicy::New(context, (IActionBarPolicy**)&abp);
    Int32 height;
    abp->GetTabContainerHeight(&height);
    SetContentHeight(height);
    abp->GetStackedTabMaxWidth(&mStackedTabMaxWidth);

    mTabLayout = CreateTabLayout();
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::MATCH_PARENT, (IViewGroupLayoutParams**)&params);
    AddView(IView::Probe(mTabLayout), params);
    return NOERROR;
}

void ScrollingTabContainerView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Boolean lockedExpanded = widthMode == MeasureSpec::EXACTLY;
    SetFillViewport(lockedExpanded);

    Int32 childCount;
    mTabLayout->GetChildCount(&childCount);
    if (childCount > 1 &&
            (widthMode == MeasureSpec::EXACTLY || widthMode == MeasureSpec::AT_MOST)) {
        if (childCount > 2) {
            mMaxTabWidth = (Int32) (MeasureSpec::GetSize(widthMeasureSpec) * 0.4f);
        } else {
            mMaxTabWidth = MeasureSpec::GetSize(widthMeasureSpec) / 2;
        }
        mMaxTabWidth = Elastos::Core::Math::Min(mMaxTabWidth, mStackedTabMaxWidth);
    } else {
        mMaxTabWidth = -1;
    }

    heightMeasureSpec = MeasureSpec::MakeMeasureSpec(mContentHeight, MeasureSpec::EXACTLY);

    Boolean canCollapse = !lockedExpanded && mAllowCollapse;

    if (canCollapse) {
        // See if we should expand
        mTabLayout->Measure(MeasureSpec::UNSPECIFIED, heightMeasureSpec);

        Int32 width;
        mTabLayout->GetMeasuredWidth(&width);
        if (width > MeasureSpec::GetSize(widthMeasureSpec)) {
            PerformCollapse();
        } else {
            PerformExpand();
        }
    } else {
        PerformExpand();
    }

    Int32 oldWidth = GetMeasuredWidth();
    HorizontalScrollView::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    Int32 newWidth = GetMeasuredWidth();

    if (lockedExpanded && oldWidth != newWidth) {
        // Recenter the tab display if we're at a new (scrollable) size.
        SetTabSelected(mSelectedTabIndex);
    }
}

/**
 * Indicates whether this view is collapsed into a dropdown menu instead
 * of traditional tabs.
 * @return TRUE if showing as a spinner
 */
Boolean ScrollingTabContainerView::IsCollapsed()
{
    AutoPtr<IViewParent> p;
    return mTabSpinner != NULL && (mTabSpinner->GetParent((IViewParent**)&p), p.Get() == THIS_PROBE(IViewParent));
}

ECode ScrollingTabContainerView::SetAllowCollapse(
    /* [in] */ Boolean allowCollapse)
{
    mAllowCollapse = allowCollapse;

    return NOERROR;
}

void ScrollingTabContainerView::PerformCollapse()
{
    if (IsCollapsed()) return;

    if (mTabSpinner == NULL) {
        mTabSpinner = CreateSpinner();
    }
    RemoveView(mTabLayout);
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::MATCH_PARENT, (IViewGroupLayoutParams**)&params);
    AddView(mTabSpinner, params);
    AutoPtr<IAdapter> apt;
    mTabSpinner->GetAdapter((IAdapter**)&apt);
    if (apt == NULL) {
        AutoPtr<TabAdapter> adapter = new TabAdapter(this);
        mTabSpinner->SetAdapter((IAdapter*)adapter->Probe(EIID_IAdapter));
    }
    if (mTabSelector != NULL) {
        RemoveCallbacks(mTabSelector);
        mTabSelector = NULL;
    }
    mTabSpinner->SetSelection(mSelectedTabIndex);
}

Boolean ScrollingTabContainerView::PerformExpand()
{
    if (!IsCollapsed()) return FALSE;

    RemoveView(mTabSpinner);
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::MATCH_PARENT, (IViewGroupLayoutParams**)&params);
    AddView(mTabLayout, params);
    Int32 position;
    mTabSpinner->GetSelectedItemPosition(&position);
    SetTabSelected(position);
    return FALSE;
}

ECode ScrollingTabContainerView::SetTabSelected(
    /* [in] */ Int32 position)
{
    mSelectedTabIndex = position;
    Int32 tabCount;
    mTabLayout->GetChildCount(&tabCount);

    for (Int32 i = 0; i < tabCount; i++) {
        AutoPtr<IView> child;
        mTabLayout->GetChildAt(i, (IView**)&child);
        Boolean isSelected = i == position;
        child->SetSelected(isSelected);
        if (isSelected) {
            AnimateToTab(position);
        }
    }

    return NOERROR;
}

ECode ScrollingTabContainerView::SetContentHeight(
    /* [in] */ Int32 contentHeight)
{
    mContentHeight = contentHeight;
    RequestLayout();

    return NOERROR;
}

AutoPtr<ILinearLayout> ScrollingTabContainerView::CreateTabLayout()
{
    AutoPtr<ILinearLayout> tabLayout;
    CLinearLayout::New(GetContext(), NULL,
        R::attr::actionBarTabBarStyle, (ILinearLayout**)&tabLayout);
    tabLayout->SetMeasureWithLargestChildEnabled(TRUE);
    tabLayout->SetGravity(IGravity::CENTER);

    AutoPtr<ILinearLayoutLayoutParams> lp;
    CLinearLayoutLayoutParams::New(
        ILinearLayoutLayoutParams::WRAP_CONTENT, ILinearLayoutLayoutParams::MATCH_PARENT, (ILinearLayoutLayoutParams**)&lp);

    tabLayout->SetLayoutParams(lp);
    return tabLayout;
}

AutoPtr<ISpinner> ScrollingTabContainerView::CreateSpinner()
{
    AutoPtr<ISpinner> spinner;
    CSpinner::New(GetContext(), NULL,
        R::attr::actionDropDownStyle, (ISpinner**)&spinner);


    AutoPtr<ILinearLayoutLayoutParams> lp;
    CLinearLayoutLayoutParams::New(
        ILinearLayoutLayoutParams::WRAP_CONTENT, ILinearLayoutLayoutParams::MATCH_PARENT, (ILinearLayoutLayoutParams**)&lp);

    spinner->SetLayoutParams(lp);
    reinterpret_cast<Spinner*>(spinner->Probe(EIID_Spinner))->SetOnItemClickListenerInt(THIS_PROBE(IAdapterViewOnItemClickListener));
    return spinner;
}

void ScrollingTabContainerView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    HorizontalScrollView::OnConfigurationChanged(newConfig);

    AutoPtr<IActionBarPolicy> abp;
    CActionBarPolicy::New(GetContext(), (IActionBarPolicy**)&abp);
    // Action bar can change size on configuration changes.
    // Reread the desired height from the theme-specified style.
    Int32 height;
    abp->GetTabContainerHeight(&height);
    SetContentHeight(height);
    abp->GetStackedTabMaxWidth(&mStackedTabMaxWidth);
}

ECode ScrollingTabContainerView::AnimateToVisibility(
    /* [in] */ Int32 visibility)
{
    if (mVisibilityAnim != NULL) {
        mVisibilityAnim->Cancel();
    }
    if (visibility == IView::VISIBLE) {
        if (GetVisibility() != IView::VISIBLE) {
            SetAlpha(0);
        }
        AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
        (*array)[0] = 1.f;
        AutoPtr<IObjectAnimator> anim = CObjectAnimator::OfFloat(THIS_PROBE(IScrollingTabContainerView), String("alpha"), array);
        anim->SetDuration(FADE_DURATION);
        anim->SetInterpolator(sAlphaInterpolator);

        anim->AddListener(mVisAnimListener->WithFinalVisibility(visibility));
        anim->Start();
    } else {
        AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
        (*array)[0] = 0.f;
        AutoPtr<IObjectAnimator> anim = CObjectAnimator::OfFloat(THIS_PROBE(IScrollingTabContainerView), String("alpha"), array);
        anim->SetDuration(FADE_DURATION);
        anim->SetInterpolator(sAlphaInterpolator);

        anim->AddListener(mVisAnimListener->WithFinalVisibility(visibility));
        anim->Start();
    }

    return NOERROR;
}

ECode ScrollingTabContainerView::AnimateToTab(
    /* [in] */ Int32 position)
{
    AutoPtr<IView> tabView;
    mTabLayout->GetChildAt(position, (IView**)&tabView);
    if (mTabSelector != NULL) {
        RemoveCallbacks(mTabSelector);
    }
    mTabSelector = new AnimateToTabRunnable(tabView, this);
    Post(mTabSelector);
    return NOERROR;
}

ECode ScrollingTabContainerView::OnAttachedToWindow()
{
    HorizontalScrollView::OnAttachedToWindow();
    if (mTabSelector != NULL) {
        // Re-post the selector we saved
        Post(mTabSelector);
    }
    return NOERROR;
}

ECode ScrollingTabContainerView::OnDetachedFromWindow()
{
    HorizontalScrollView::OnDetachedFromWindow();
    if (mTabSelector != NULL) {
        RemoveCallbacks(mTabSelector);
    }
    return NOERROR;
}

AutoPtr<ScrollingTabContainerView::TabView> ScrollingTabContainerView::CreateTabView(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Boolean forAdapter)
{
    AutoPtr<TabView> tabView = new TabView(this);
    tabView->Init(GetContext(), tab, forAdapter);
    if (forAdapter) {
        tabView->SetBackgroundDrawable(NULL);

        AutoPtr<IAbsListViewLayoutParams> lp;
        CAbsListViewLayoutParams::New(IAbsListViewLayoutParams::MATCH_PARENT,
            mContentHeight, (IAbsListViewLayoutParams**)&lp);
        tabView->SetLayoutParams(lp);
    } else {
        tabView->SetFocusable(TRUE);

        if (mTabClickListener == NULL) {
            mTabClickListener = new TabClickListener(this);
        }
        tabView->SetOnClickListener(mTabClickListener);
    }
    return tabView;
}

ECode ScrollingTabContainerView::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Boolean setSelected)
{
    AutoPtr<TabView> tabView = CreateTabView(tab, FALSE);
    AutoPtr<ILinearLayoutLayoutParams> lp;
    CLinearLayoutLayoutParams::New(0,
        ILinearLayoutLayoutParams::MATCH_PARENT, 1, (ILinearLayoutLayoutParams**)&lp);
    mTabLayout->AddView((IView*)tabView->Probe(EIID_IView), lp);

    if (mTabSpinner != NULL) {
        AutoPtr<IAdapter> tabAdapter;
        mTabSpinner->GetAdapter((IAdapter**)&tabAdapter);

        IBaseAdapter::Probe(tabAdapter)->NotifyDataSetChanged();
    }
    if (setSelected) {
        tabView->SetSelected(TRUE);
    }
    if (mAllowCollapse) {
        RequestLayout();
    }

    return NOERROR;
}

ECode ScrollingTabContainerView::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Int32 position,
    /* [in] */ Boolean setSelected)
{
    AutoPtr<TabView> tabView = CreateTabView(tab, FALSE);
    AutoPtr<ILinearLayoutLayoutParams> lp;
    CLinearLayoutLayoutParams::New(0,
        ILinearLayoutLayoutParams::MATCH_PARENT, 1, (ILinearLayoutLayoutParams**)&lp);
    mTabLayout->AddView((IView*)tabView->Probe(EIID_IView), position, lp);

    if (mTabSpinner != NULL) {
        AutoPtr<IAdapter> tabAdapter;
        mTabSpinner->GetAdapter((IAdapter**)&tabAdapter);
        IBaseAdapter::Probe(tabAdapter)->NotifyDataSetChanged();
    }
    if (setSelected) {
        tabView->SetSelected(TRUE);
    }
    if (mAllowCollapse) {
        RequestLayout();
    }

    return NOERROR;
}

ECode ScrollingTabContainerView::UpdateTab(
    /* [in] */ Int32 position)
{
    AutoPtr<IView> temp;
    mTabLayout->GetChildAt(position, (IView**)&temp);
    AutoPtr<TabView> tabView = (TabView*)(temp.Get());
    tabView->Update();

    if (mTabSpinner != NULL) {
        AutoPtr<IAdapter> adater;
        mTabSpinner->GetAdapter((IAdapter**)&adater);
        IBaseAdapter::Probe(adater)->NotifyDataSetChanged();
    }
    if (mAllowCollapse) {
        RequestLayout();
    }
    return NOERROR;
}

ECode ScrollingTabContainerView::RemoveTabAt(
    /* [in] */ Int32 position)
{
    mTabLayout->RemoveViewAt(position);
    if (mTabSpinner != NULL) {
        AutoPtr<IAdapter> adater;
        mTabSpinner->GetAdapter((IAdapter**)&adater);
        IBaseAdapter::Probe(adater)->NotifyDataSetChanged();
    }
    if (mAllowCollapse) {
        RequestLayout();
    }

    return NOERROR;
}

ECode ScrollingTabContainerView::RemoveAllTabs()
{
    mTabLayout->RemoveAllViews();
    if (mTabSpinner != NULL) {
        AutoPtr<IAdapter> adater;
        mTabSpinner->GetAdapter((IAdapter**)&adater);
        IBaseAdapter::Probe(adater)->NotifyDataSetChanged();
    }
    if (mAllowCollapse) {
        RequestLayout();
    }

    return NOERROR;
}

ECode ScrollingTabContainerView::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<TabView> tabView = (TabView*) view;
    tabView->GetTab()->Select();
    return NOERROR;
}

ScrollingTabContainerView::_TabView::_TabView(
    /* [in] */ ScrollingTabContainerView* host)
    : LinearLayout()
    , mHost(host)
{}

ECode ScrollingTabContainerView::_TabView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Boolean forList)
{
    return LinearLayout::Init(context, NULL, R::attr::actionBarTabStyle);
}

void ScrollingTabContainerView::_TabView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    // Re-measure if we went beyond our maximum size.
    if (mHost->mMaxTabWidth > 0 && GetMeasuredWidth() > mHost->mMaxTabWidth) {
        LinearLayout::OnMeasure(MeasureSpec::MakeMeasureSpec(mHost->mMaxTabWidth, MeasureSpec::EXACTLY),
                heightMeasureSpec);
    }
}

ILINEARLAYOUT_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IVIEWGROUP_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IVIEW_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IVIEWPARENT_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IVIEWMANAGER_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IDRAWABLECALLBACK_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IKEYEVENTCALLBACK_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(ScrollingTabContainerView::TabView, _TabView)

ScrollingTabContainerView::TabView::TabView(
    /* [in] */ ScrollingTabContainerView* host)
    : _TabView(host)
{}

ECode ScrollingTabContainerView::TabView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Boolean forList)
{
    _TabView::Init(context, tab, forList);
    mTab = tab;

    if (forList) {
        SetGravity(IGravity::START | IGravity::CENTER_VERTICAL);
    }

    Update();
    return NOERROR;
}

PInterface ScrollingTabContainerView::TabView::Probe(
            /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ILinearLayout*)this;
    } else if (riid == EIID_ILinearLayout) {
        return (ILinearLayout*)this;
    } else if (riid == EIID_IViewGroup) {
        return (IViewGroup*)(ILinearLayout*)this;
    } else if (riid == EIID_IView) {
        return (IView*)(ILinearLayout*)this;
    } else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    } else if (riid == EIID_IViewManager) {
        return (IViewManager*)this;
    } else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    } else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    } else if (riid == EIID_IAccessibilityEventSource) {
        return (IAccessibilityEventSource*)this;
    } else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    } else if (riid == EIID_IViewOnLongClickListener) {
        return (IViewOnLongClickListener*)this;
    } else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)(_TabView*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)(_TabView*)this);
    }
    return NULL;
}

UInt32 ScrollingTabContainerView::TabView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ScrollingTabContainerView::TabView::Release()
{
    return ElRefBase::Release();
}

ECode ScrollingTabContainerView::TabView::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IInterface*)(ILinearLayout *)this) {
        *pIID = EIID_ILinearLayout;
    } else if (object == (IInterface*)(IViewParent*)this) {
        *pIID = EIID_IViewParent;
    } else if (object == (IInterface*)(IViewManager*)this) {
        *pIID = EIID_IViewManager;
    } else if (object == (IInterface*)(IDrawableCallback*)this) {
        *pIID = EIID_IDrawableCallback;
    } else if (object == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    } else if (object == (IInterface*)(IAccessibilityEventSource*)this) {
        *pIID = EIID_IAccessibilityEventSource;
    }  else if (object == (IInterface*)(IViewOnLongClickListener*)this) {
        *pIID = EIID_IViewOnLongClickListener;
    } else if (object == reinterpret_cast<PInterface>((View*)this)) {
        *pIID = EIID_View;
    } else if (object == reinterpret_cast<PInterface>((ViewGroup*)this)) {
        *pIID = EIID_ViewGroup;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ScrollingTabContainerView::TabView::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    INTERFACE_ADDREF(*weakReference)
    return NOERROR;
}

ECode ScrollingTabContainerView::TabView::BindTab(
    /* [in] */ IActionBarTab* tab)
{
    mTab = tab;
    Update();

    return NOERROR;
}


ECode ScrollingTabContainerView::TabView::Update()
{
    AutoPtr<IActionBarTab> tab = mTab;
    AutoPtr<IView> custom;
    tab->GetCustomView((IView**)&custom);
    if (custom != NULL) {
        AutoPtr<IViewParent> customParent;
        custom->GetParent((IViewParent**)&customParent);
        if (customParent.Get() != (IViewParent*)this) {
            if (customParent != NULL)
                IViewManager::Probe(customParent)->RemoveView(custom);
            AddView(custom);
        }
        mCustomView = custom;
        if (mTextView != NULL) mTextView->SetVisibility(IView::GONE);
        if (mIconView != NULL) {
            mIconView->SetVisibility(IView::GONE);
            mIconView->SetImageDrawable(NULL);
        }
    } else {
        if (mCustomView != NULL) {
            RemoveView(mCustomView);
            mCustomView = NULL;
        }

        AutoPtr<IDrawable> icon;
        tab->GetIcon((IDrawable**)&icon);
        AutoPtr<ICharSequence> text;
        tab->GetText((ICharSequence**)&text);

        if (icon != NULL) {
            if (mIconView == NULL) {
                AutoPtr<IImageView> iconView;
                AutoPtr<IContext> ctx;
                GetContext((IContext**)&ctx);
                CImageView::New(ctx, (IImageView**)&iconView);
                AutoPtr<ILinearLayoutLayoutParams> lp;
                CLinearLayoutLayoutParams::New(ILinearLayoutLayoutParams::WRAP_CONTENT,
                        ILinearLayoutLayoutParams::WRAP_CONTENT, (ILinearLayoutLayoutParams**)&lp);
                lp->SetGravity(IGravity::CENTER_VERTICAL);
                iconView->SetLayoutParams(lp);
                ViewGroup::AddView(IView::Probe(iconView), 0);
                mIconView = iconView;
            }
            mIconView->SetImageDrawable(icon);
            mIconView->SetVisibility(IView::VISIBLE);
        } else if (mIconView != NULL) {
            mIconView->SetVisibility(IView::GONE);
            mIconView->SetImageDrawable(NULL);
        }

        Boolean hasText = !TextUtils::IsEmpty(text);
        if (hasText) {
            if (mTextView == NULL) {
                AutoPtr<ITextView> textView;
                AutoPtr<IContext> ctx;
                GetContext((IContext**)&ctx);
                CTextView::New(ctx, NULL, R::attr::actionBarTabTextStyle, (ITextView**)&textView);
                textView->SetEllipsize(TextUtilsTruncateAt_END);
                AutoPtr<ILinearLayoutLayoutParams> lp;
                CLinearLayoutLayoutParams::New(ILinearLayoutLayoutParams::WRAP_CONTENT,
                        ILinearLayoutLayoutParams::WRAP_CONTENT, (ILinearLayoutLayoutParams**)&lp);
                lp->SetGravity(IGravity::CENTER_VERTICAL);
                textView->SetLayoutParams(lp);
                AddView(textView);
                mTextView = textView;
            }
            mTextView->SetText(text);
            mTextView->SetVisibility(IView::VISIBLE);
        } else if (mTextView != NULL) {
            mTextView->SetVisibility(IView::GONE);
            mTextView->SetText(NULL);
        }

        AutoPtr<ICharSequence> cs;
        tab->GetContentDescription((ICharSequence**)&cs);
        if (mIconView != NULL) {
            mIconView->SetContentDescription(cs);
        }

        if (!hasText && !TextUtils::IsEmpty(cs)) {
            SetOnLongClickListener(this);
        } else {
            SetOnLongClickListener(NULL);
            SetLongClickable(FALSE);
        }
    }
    return NOERROR;
}

ECode ScrollingTabContainerView::TabView::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* rst)
{
    Int32 screenPos1;
    Int32 screenPos2;
    GetLocationOnScreen(&screenPos1, &screenPos2);

    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    Int32 width;
    GetWidth(&width);
    Int32 height;
    GetHeight(&height);
    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    AutoPtr<IDisplayMetrics> dm;
    r->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Int32 screenWidth;
    dm->GetWidthPixels(&screenWidth);

    AutoPtr<IToast> cheatSheet;
    AutoPtr<ICharSequence> cs;
    mTab->GetContentDescription((ICharSequence**)&cs);
    Toast::MakeText(context, cs, IToast::LENGTH_SHORT, (IToast**)&cheatSheet);
    // Show under the tab
    cheatSheet->SetGravity(IGravity::TOP | IGravity::CENTER_HORIZONTAL,
            (screenPos1 + width / 2) - screenWidth / 2, height);

    cheatSheet->Show();
    *rst = TRUE;
    return NOERROR;
}

AutoPtr<IActionBarTab> ScrollingTabContainerView::TabView::GetTab()
{
    return mTab;
}

ScrollingTabContainerView::_TabAdapter::_TabAdapter(
    /* [in] */ ScrollingTabContainerView* host)
{
    mHost = host;
}

Int32 ScrollingTabContainerView::_TabAdapter::GetCount()
{
    Int32 count;
    mHost->mTabLayout->GetChildCount(&count);

    return count;
}

AutoPtr<IInterface> ScrollingTabContainerView::_TabAdapter::GetItem(
    /* [in] */ Int32 position)
{
    AutoPtr<IView> v;
    mHost->mTabLayout->GetChildAt(position, (IView**)&v);
    return ((TabView*)v.Get())->GetTab();
}


Int64 ScrollingTabContainerView::_TabAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

AutoPtr<IView> ScrollingTabContainerView::_TabAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    if (convertView == NULL) {
        AutoPtr<IInterface> item = GetItem(position);
        convertView = mHost->CreateTabView(IActionBarTab::Probe(item) , TRUE);
    } else {
        AutoPtr<IInterface> item = GetItem(position);
        ((TabView*)convertView)->BindTab(IActionBarTab::Probe(item));
    }
    return convertView;
}

IADAPTER_METHODS_IMPL(ScrollingTabContainerView::TabAdapter, _TabAdapter)
IBASEADAPTER_METHODS_IMPL(ScrollingTabContainerView::TabAdapter, _TabAdapter)
ILISTADAPTER_METHODS_IMPL(ScrollingTabContainerView::TabAdapter, _TabAdapter)
ISPINNERADAPTER_METHODS_IMPL(ScrollingTabContainerView::TabAdapter, _TabAdapter)

ScrollingTabContainerView::TabAdapter::TabAdapter(
    /* [in] */ ScrollingTabContainerView* host)
    : _TabAdapter(host)
{}

PInterface ScrollingTabContainerView::TabAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IBaseAdapter*)this;
    } else if (riid == EIID_IBaseAdapter) {
        return (IBaseAdapter*)this;
    }  else if (riid == EIID_IAdapter) {
        return (IAdapter*)(IBaseAdapter*)this;
    } else if (riid == EIID_IListAdapter) {
        return (IListAdapter*)this;
    } else if (riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    }
    return NULL;
}

UInt32 ScrollingTabContainerView::TabAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ScrollingTabContainerView::TabAdapter::Release()
{
    return ElRefBase::Release();
}

ECode ScrollingTabContainerView::TabAdapter::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IBaseAdapter*)this) {
        *pIID = EIID_IBaseAdapter;
    } else if (object == (ISpinnerAdapter*)this) {
        *pIID = EIID_ISpinnerAdapter;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(ScrollingTabContainerView::TabClickListener, IViewOnClickListener)

ScrollingTabContainerView::TabClickListener::TabClickListener(
    /* [in] */ ScrollingTabContainerView* host)
{
    mHost = host;
}

ECode ScrollingTabContainerView::TabClickListener::OnClick(
    /* [in] */ IView* view)
{
    AutoPtr<TabView> tabView = (TabView*)view;
    tabView->GetTab()->Select();
    Int32 tabCount;
    mHost->mTabLayout->GetChildCount(&tabCount);
    for (Int32 i = 0; i < tabCount; i++) {
        AutoPtr<IView> child;
        mHost->mTabLayout->GetChildAt(i, (IView**)&child);
        child->SetSelected(child.Get() == view);
    }

    return NOERROR;
}


CAR_INTERFACE_IMPL(ScrollingTabContainerView::VisibilityAnimListener, IAnimatorListener)

ScrollingTabContainerView::VisibilityAnimListener::VisibilityAnimListener(
    /* [in] */ ScrollingTabContainerView* host)
{
    mHost = host;
    mCanceled = FALSE;
}

AutoPtr<ScrollingTabContainerView::VisibilityAnimListener> ScrollingTabContainerView::VisibilityAnimListener::WithFinalVisibility(
    /* [in] */ Int32 visibility)
{
    mFinalVisibility = visibility;
    return this;
}

ECode ScrollingTabContainerView::VisibilityAnimListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    mHost->SetVisibility(IView::VISIBLE);
    mHost->mVisibilityAnim = animation;
    mCanceled = FALSE;
    return NOERROR;
}

ECode ScrollingTabContainerView::VisibilityAnimListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mCanceled) return NOERROR;

    mHost->mVisibilityAnim = NULL;
    mHost->SetVisibility(mFinalVisibility);
    return NOERROR;
}

ECode ScrollingTabContainerView::VisibilityAnimListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    mCanceled = TRUE;
    return NOERROR;
}

ECode ScrollingTabContainerView::VisibilityAnimListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
