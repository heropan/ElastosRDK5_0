#ifndef __ELASTOS_DROID_PACKAGES_SYSTEMUI_CSEARCHPANELVIEW_H__
#define __ELASTOS_DROID_PACKAGES_SYSTEMUI_CSEARCHPANELVIEW_H__

#include "_Elastos_Droid_Packages_SystemUI_CSearchPanelView.h"
#include "_SystemUI.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Media.h"
#include "Elastos.Droid.Os.h"
#include <elastos/droid/widget/FrameLayout.h>

using Elastos::Droid::App::IActivityOptions;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Media::IAudioAttributes;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Packages::SystemUI::StatusBar::IBaseStatusBar;
using Elastos::Droid::Packages::SystemUI::StatusBar::IStatusBarPanel;

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {

CarClass(CSearchPanelView)
    , public FrameLayout
    , public ISearchPanelView
    , public IStatusBarPanel
{
private:
    class Runnable1: public Runnable
    {
    public:
        Runnable1(
            /* [in] */ CSearchPanelView* host,
            /* [in] */ IIntent* intent,
            /* [in] */ IActivityOptions* opts);

        // @Override
        CARAPI Run();

    private:
        CSearchPanelView* mHost;
        AutoPtr<IIntent> mIntent;
        AutoPtr<IActivityOptions> mOpts;
    };

    class Runnable2: public Runnable
    {
    public:
        Runnable2(
            /* [in] */ CSearchPanelView* host);

        // @Override
        CARAPI Run();

    private:
        CSearchPanelView* mHost;
    };

    class Runnable3: public Runnable
    {
    public:
        Runnable3(
            /* [in] */ CSearchPanelView* host);

        // @Override
        CARAPI Run();

    private:
        CSearchPanelView* mHost;
    };

    class Runnable4: public Runnable
    {
    public:
        Runnable4(
            /* [in] */ CSearchPanelView* host);

        // @Override
        CARAPI Run();

    private:
        CSearchPanelView* mHost;
    };

public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CSearchPanelView();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI ReplaceDrawable(
        /* [in] */ IImageView* v,
        /* [in] */ IComponentName* component,
        /* [in] */ const String& name);

    // @Override
    CARAPI IsInContentArea(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Boolean* result);

    CARAPI Show(
        /* [in] */ Boolean show,
        /* [in] */ Boolean animate);

    CARAPI Hide(
        /* [in] */ Boolean animate);

    // @Override
    CARAPI DispatchHoverEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    /**
     * Whether the panel is showing, or, if it's animating, whether it will be
     * when the animation is done.
     */
    CARAPI IsShowing(
        /* [out] */ Boolean* showing);

    CARAPI SetBar(
        /* [in] */ IBaseStatusBar* bar);

    CARAPI IsAssistantAvailable(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI SetHorizontal(
        /* [in] */ Boolean horizontal);

protected:
    // @Override
    CARAPI OnFinishInflate();

private:
    CARAPI_(void) StartAssistActivity();

    CARAPI_(void) MaybeSwapSearchIcon();

    CARAPI_(void) Vibrate();

    CARAPI_(void) StartEnterAnimation();

    CARAPI_(void) StartAbortAnimation();

    CARAPI_(void) StartExitAnimation();

    static CARAPI_(Boolean) InitStatic();

private:
    static const String TAG;
    static const String ASSIST_ICON_METADATA_NAME;

    static Boolean sInit;
    static AutoPtr<IAudioAttributes> VIBRATION_ATTRIBUTES;

    AutoPtr<IContext> mContext;
    AutoPtr<IBaseStatusBar> mBar;

    AutoPtr<ISearchPanelCircleView> mCircle;
    AutoPtr<IImageView> mLogo;
    AutoPtr<IView> mScrim;

    Int32 mThreshold;
    Boolean mHorizontal;

    Boolean mLaunching;
    Boolean mDragging;
    Boolean mDraggedFarEnough;
    Float mStartTouch;
    Float mStartDrag;
    Boolean mLaunchPending;
};

} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_PACKAGES_SYSTEMUI_CSEARCHPANELVIEW_H__
