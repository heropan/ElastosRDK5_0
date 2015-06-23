
#ifndef  __PREFERENCEFRAMELAYOUT_H__
#define  __PREFERENCEFRAMELAYOUT_H__

#include "widget/FrameLayout.h"
#include "widget/FrameLayoutLayoutParams.h"




using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
using Elastos::Droid::Widget::FrameLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Preference {

extern "C" const InterfaceID EIID_PreferenceFrameLayoutParams;

class PreferenceFrameLayout
    : public FrameLayout
    , public IFrameLayout
    , public IViewParent
    , public IViewManager
    , public IDrawableCallback
    , public IKeyEventCallback
    , public IAccessibilityEventSource
    , public ElRefBase
{
public:
    class LayoutParams
        : public FrameLayoutLayoutParams
        , public IFrameLayoutLayoutParams
        , public ElRefBase
    {
    public:
        LayoutParams(
            /* [in] */ IContext* c,
            /* [in] */ IAttributeSet* attrs);

        LayoutParams(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CAR_INTERFACE_DECL()

        IVIEWGROUPLP_METHODS_DECL()
        IVIEWGROUPMARGINLP_METHODS_DECL()

        CARAPI GetGravity(
            /* [out] */ Int32* gravity);

        CARAPI SetGravity(
            /* [in] */ Int32 gravity);

    public:
        Boolean mRemoveBorders;
    };

public:
    PreferenceFrameLayout(
        /* [in] */ IContext* context);

    PreferenceFrameLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    PreferenceFrameLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CAR_INTERFACE_DECL()

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI GetForegroundGravity(
        /* [out] */ Int32* foregroundGravity);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI GetMeasureAllChildren(
        /* [out] */ Boolean* measureAll);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);

private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    static const Int32 DEFAULT_BORDER_TOP = 0;
    static const Int32 DEFAULT_BORDER_BOTTOM = 0;
    static const Int32 DEFAULT_BORDER_LEFT = 0;
    static const Int32 DEFAULT_BORDER_RIGHT = 0;

    Int32 mBorderTop;
    Int32 mBorderBottom;
    Int32 mBorderLeft;
    Int32 mBorderRight;
    Boolean mPaddingApplied;
};

} // Preference
} // Droid
} // Elastos

#endif  // __PREFERENCEFRAMELAYOUT_H__
