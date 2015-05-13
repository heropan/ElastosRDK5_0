
#ifndef __ACTIONBAROVERLAYLAYOUT_H__
#define __ACTIONBAROVERLAYLAYOUT_H__

#include "widget/FrameLayout.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Internal::App::IActionBarImpl;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {
/**
 * Special layout for the containing of an overlay action bar (and its
 * content) to correctly handle fitting system windows when the content
 * has request that its layout ignore them.
 */
class ActionBarOverlayLayout : public FrameLayout
{
public:
    ActionBarOverlayLayout(
        /* [in] */ IContext* context);

    ActionBarOverlayLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI SetActionBar(
        /* [in] */ IActionBarImpl* impl);

    virtual CARAPI SetShowingForActionMode(
        /* [in] */ Boolean showing);

    virtual CARAPI OnWindowSystemUiVisibilityChanged(
        /* [in] */ Int32 visible);

    CARAPI_(void) PullChildren();

protected:
    ActionBarOverlayLayout();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    CARAPI_(Boolean) FitSystemWindows(
        /* [in] */ IRect* insets);

private:
    CARAPI_(void) InitInternal(
        /* [in] */ IContext* context);

    CARAPI_(Boolean) ApplyInsets(
        /* [in] */ IView* view,
        /* [in] */ IRect* insets,
        /* [in] */ Boolean left,
        /* [in] */ Boolean top,
        /* [in] */ Boolean bottom,
        /* [in] */ Boolean right);

private:
    Int32 mActionBarHeight;
    AutoPtr<IActionBarImpl> mActionBar;
    Int32 mWindowVisibility; //= View.VISIBLE;
    AutoPtr<IView> mContent;
    AutoPtr<IView> mActionBarTop;
    AutoPtr<IActionBarContainer> mContainerView;
    AutoPtr<IActionBarView> mActionView;
    AutoPtr<IView> mActionBarBottom;
    Int32 mLastSystemUiVisibility;
    AutoPtr<IRect> mZeroRect;// = new Rect(0, 0, 0, 0);

public:
    static const AutoPtr<ArrayOf<Int32> > mActionBarSizeAttr;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ACTIONBAROVERLAYLAYOUT_H__
