
#ifndef __ELASTOS_DROID_PREFERENCE_PREFERENCEFRAMELAYOUT_H__
#define  __ELASTOS_DROID_PREFERENCE_PREFERENCEFRAMELAYOUT_H__

#include "elastos/droid/ext/frameworkext.h"
//#include "elastos/droid/widget/FrameLayout.h"
// #include "elastos/droid/widget/FrameLayoutLayoutParams.h"

using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::Widget::IFrameLayout;
//using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
// using Elastos::Droid::Widget::FrameLayoutLayoutParams;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::Res::ITypedArray;

namespace Elastos {
namespace Droid {
namespace Preference {

class PreferenceFrameLayout
    //: public FrameLayout
    : public Object
    , public IPreferenceFrameLayout
    // , public IFrameLayout
    // , public IView
    // , public IViewGroup
    // , public IViewParent
    // , public IViewManager
    // , public IDrawableCallback
    // , public IKeyEventCallback
    // , public IAccessibilityEventSource
{
public:
    class LayoutParams
        //: public FrameLayoutLayoutParams
        : public Object
        , public IFrameLayoutLayoutParams
    {
    public:
        LayoutParams(
            /* [in] */ IContext* c,
            /* [in] */ IAttributeSet* attrs);

        LayoutParams(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CAR_INTERFACE_DECL()
        // IVIEWGROUPLP_METHODS_DECL();
        // IVIEWGROUPMARGINLP_METHODS_DECL();

        CARAPI GetGravity(
            /* [out] */ Int32* gravity);

        CARAPI SetGravity(
            /* [in] */ Int32 gravity);

    public:
        Boolean mRemoveBorders;
    };

public:
    PreferenceFrameLayout();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    CAR_INTERFACE_DECL()
    // IVIEW_METHODS_DECL();
    // IVIEWGROUP_METHODS_DECL();
    // IVIEWPARENT_METHODS_DECL();
    // IVIEWMANAGER_METHODS_DECL();
    // IDRAWABLECALLBACK_METHODS_DECL();
    // IKEYEVENTCALLBACK_METHODS_DECL();
    // IACCESSIBILITYEVENTSOURCE_METHODS_DECL();

    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    CARAPI AddView(
        /* [in] */ IView* child);

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

#endif  // __ELASTOS_DROID_PREFERENCE_PREFERENCEFRAMELAYOUT_H__
