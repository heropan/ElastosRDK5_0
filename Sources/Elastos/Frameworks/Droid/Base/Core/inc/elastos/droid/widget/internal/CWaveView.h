#include "_Elastos_Droid_Widget_Internal_CWaveView.h"
#include "elastos/droid/internal/WaveView.h"


using Elastos::Droid::Animation::IValueAnimator;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal{

CarClass(CWaveView) , public WaveView
{
public:

    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnAnimationUpdate(
        /* [in] */ IValueAnimator* animation);

    CARAPI SetOnTriggerListener(
        /* [in] */ IWaveViewOnTriggerListener* listener);

    CARAPI Reset();
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
