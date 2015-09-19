
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_LINEARLAYOUTWITHDEFAULTTOUCHRECEPIENT_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_LINEARLAYOUTWITHDEFAULTTOUCHRECEPIENT_H__

#include "LinearLayout.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

/**
 * Like a normal linear layout, but supports dispatching all otherwise unhandled
 * touch events to a particular descendant.  This is for the unlock screen, so
 * that a wider range of touch events than just the lock pattern widget can kick
 * off a lock pattern if the finger is eventually dragged into the bounds of the
 * lock pattern view.
 */
class LinearLayoutWithDefaultTouchRecepient : public LinearLayout
{

public:

    LinearLayoutWithDefaultTouchRecepient();

    LinearLayoutWithDefaultTouchRecepient(
        /* [in] */ IContext* context);

    LinearLayoutWithDefaultTouchRecepient(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI SetDefaultTouchRecepient(
        /* [in] */ IView* defaultTouchRecepient);

    virtual CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev);

private:

    AutoPtr<CRect> mTempRect;
    AutoPtr<IView> mDefaultTouchRecepient;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
