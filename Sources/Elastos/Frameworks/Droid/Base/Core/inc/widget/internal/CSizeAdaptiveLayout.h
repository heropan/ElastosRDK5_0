
#include "_CSizeAdaptiveLayout.h"

#include "widget/internal/SizeAdaptiveLayout.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CSizeAdaptiveLayout), public SizeAdaptiveLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Visible for testing
     * @hide
     */
    CARAPI GetTransitionAnimation(
        /* [out] */ IAnimator** animator);

    /**
     * Visible for testing
     * @hide
     */
    CARAPI GetModestyPanel(
        /* [out] */ IView** panel);

    CARAPI OnAttachedToWindow();
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
