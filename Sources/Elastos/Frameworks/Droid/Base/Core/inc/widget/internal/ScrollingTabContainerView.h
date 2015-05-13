
#ifndef __SCROLLINGTABCONTAINERVIEW_H__
#define __SCROLLINGTABCONTAINERVIEW_H__

#include "widget/HorizontalScrollView.h"
#include "widget/LinearLayoutMacro.h"
#include "widget/BaseAdapter.h"
#include "widget/AdapterMacro.h"
#include "widget/LinearLayout.h"
#include "os/Runnable.h"
#include "view/ViewMacro.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::View::EIID_IViewOnLongClickListener;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::App::IActionBarTab;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::ITimeInterpolator;
/**
 * This widget implements the dynamic action bar tab behavior that can change
 * across different configurations or circumstances.
 */
namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class ScrollingTabContainerView : public HorizontalScrollView
{
private:
    class AnimateToTabRunnable : public Runnable
    {
    public:
        AnimateToTabRunnable(
            /* [in] */ IView* tabView,
            /* [in] */ ScrollingTabContainerView* host);
        CARAPI Run();
    private:
        AutoPtr<IView> mTabView;
        ScrollingTabContainerView* mHost;
    };

    class _TabView: public LinearLayout
    {
    public:
        _TabView(
            /* [in] */ ScrollingTabContainerView* host);

        CARAPI Init(
            /* [in] */ IContext* context,
            /* [in] */ IActionBarTab* tab,
            /* [in] */ Boolean forList);

        virtual CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);
    protected:
        AutoPtr<IActionBarTab> mTab;
        AutoPtr<ITextView> mTextView;
        AutoPtr<IImageView> mIconView;
        AutoPtr<IView> mCustomView;
        ScrollingTabContainerView* mHost;
    };

    class TabView
        : public _TabView
        , public ILinearLayout
        , public IViewParent
        , public IViewManager
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
        , public IViewOnLongClickListener
        , public IWeakReferenceSource
        , public ElRefBase
    {
    public:
        ILINEARLAYOUT_METHODS_DECL()
        IVIEWGROUP_METHODS_DECL()
        IVIEW_METHODS_DECL()
        IVIEWPARENT_METHODS_DECL()
        IVIEWMANAGER_METHODS_DECL()
        IDRAWABLECALLBACK_METHODS_DECL()
        IKEYEVENTCALLBACK_METHODS_DECL()
        IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

        TabView(
            /* [in] */ ScrollingTabContainerView* host);

        CARAPI Init(
            /* [in] */ IContext* context,
            /* [in] */ IActionBarTab* tab,
            /* [in] */ Boolean forList);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        CARAPI BindTab(
            /* [in] */ IActionBarTab* tab);

        CARAPI Update();

        CARAPI OnLongClick(
            /* [in] */ IView* v,
            /* [out] */ Boolean* rst);

        CARAPI_(AutoPtr<IActionBarTab>) GetTab();
    };

    class _TabAdapter : public BaseAdapter
    {
    public:
        _TabAdapter(
            /* [in] */ ScrollingTabContainerView* host);

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
        ScrollingTabContainerView* mHost;
    };

    class TabAdapter
        : public _TabAdapter
        , public IBaseAdapter
        , public ISpinnerAdapter
        , public ElRefBase
    {
    public:
        IADAPTER_METHODS_DECL()

        IBASEADAPTER_METHODS_DECL()

        ILISTADAPTER_METHODS_DECL()

        ISPINNERADAPTER_METHODS_DECL()

        TabAdapter(
            /* [in] */ ScrollingTabContainerView* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    class TabClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        TabClickListener(
            /* [in] */ ScrollingTabContainerView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* view);
    private:
        ScrollingTabContainerView* mHost;
    };

protected:
    class VisibilityAnimListener
        : public IAnimatorListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        VisibilityAnimListener(
            /* [in] */ ScrollingTabContainerView* host);

        virtual CARAPI_(AutoPtr<VisibilityAnimListener>) WithFinalVisibility(
            /* [in] */ Int32 visibility);

        CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationRepeat(
            /* [in] */ IAnimator* animation);

    private:
        Boolean mCanceled;
        Int32 mFinalVisibility;
        ScrollingTabContainerView* mHost;
    };

public:
    ScrollingTabContainerView();

    ScrollingTabContainerView(
        /* [in] */ IContext* context);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI SetAllowCollapse(
        /* [in] */ Boolean allowCollapse);

    virtual CARAPI SetTabSelected(
        /* [in] */ Int32 position);

    virtual CARAPI SetContentHeight(
        /* [in] */ Int32 contentHeight);

    virtual CARAPI AnimateToVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI AnimateToTab(
        /* [in] */ Int32 position);

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI AddTab(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Boolean setSelected);

    virtual CARAPI AddTab(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Int32 position,
        /* [in] */ Boolean setSelected);

    virtual CARAPI UpdateTab(
        /* [in] */ Int32 position);

    virtual CARAPI RemoveTabAt(
        /* [in] */ Int32 position);

    virtual CARAPI RemoveAllTabs();

    virtual CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

protected:
    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI Init(
        /* [in] */ IContext* context);
private:
    /**
     * Indicates whether this view is collapsed into a dropdown menu instead
     * of traditional tabs.
     * @return true if showing as a spinner
     */
    CARAPI_(Boolean) IsCollapsed();

    CARAPI_(void) PerformCollapse();

    CARAPI_(Boolean) PerformExpand();

    CARAPI_(AutoPtr<ILinearLayout>) CreateTabLayout();

    CARAPI_(AutoPtr<ISpinner>) CreateSpinner();

    AutoPtr<TabView> CreateTabView(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Boolean forAdapter);

    CARAPI InitInternal(
        /* [in] */ IContext* context);

protected:
    AutoPtr<IAnimator> mVisibilityAnim;
    AutoPtr<VisibilityAnimListener> mVisAnimListener;
private:

    static const String TAG;
    static AutoPtr<ITimeInterpolator> sAlphaInterpolator;
    static const Int32 FADE_DURATION;

    AutoPtr<IRunnable> mTabSelector;
    AutoPtr<TabClickListener> mTabClickListener;

    AutoPtr<ILinearLayout> mTabLayout;
    AutoPtr<ISpinner> mTabSpinner;
    Boolean mAllowCollapse;

    Int32 mMaxTabWidth;
    Int32 mStackedTabMaxWidth;
    Int32 mContentHeight;
    Int32 mSelectedTabIndex;


};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
