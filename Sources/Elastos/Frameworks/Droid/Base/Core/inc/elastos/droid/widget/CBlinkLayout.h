#ifndef __ELASTOS_DROID_WIDGET_CBLINKLAYOUT_H__
#define __ELASTOS_DROID_WIDGET_CBLINKLAYOUT_H__

#include "_Elastos_Droid_Widget_CBlinkLayout.h"


#include "elastos/droid/widget/FrameLayout.h"


using Elastos::Droid::Os::IHandlerCallback;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::View::IOnScrollChangedListener;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::IOnPreDrawListener;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IFrameLayout;

namespace Elastos{
namespace Droid{
namespace Widget{

class BlinkLayout : public FrameLayout {
private:
    class BlickHandlerCallback
        : public ElRefBase
        , public IHandlerCallback
    {
    public:
        CAR_INTERFACE_DECL()

        BlickHandlerCallback(
            /* [in] */ IWeakReference* host)
            : mWeakHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);
    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

public:
    BlinkLayout();

    BlinkLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) MakeBlink();

protected:
    static const Int32 MESSAGE_BLINK;// = 0x42;
    static const Int32 BLINK_DELAY;// = 500;

    Boolean mBlink;
    Boolean mBlinkState;
    AutoPtr<IHandler> mBlinkHandler;
};

CarClass(CBlinkLayout), public BlinkLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CBLINKLAYOUT_H__
