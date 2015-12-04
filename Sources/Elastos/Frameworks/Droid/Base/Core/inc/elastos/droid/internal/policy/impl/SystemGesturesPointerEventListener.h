#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_SYSTEMGESTURESPOINTEREVENTLISTENER_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_SYSTEMGESTURESPOINTEREVENTLISTENER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IPointerEventListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/*
  * Listens for system-wide input gestures, firing callbacks when detected.
  * @hide
  */
class SystemGesturesPointerEventListener
    : public Object
    , public IPointerEventListener
{
public:
    CAR_INTERFACE_DECL()

    SystemGesturesPointerEventListener();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ISystemGesturesPointerEventListenerCallbacks* callbacks);

    // @Override
    CARAPI OnPointerEvent(
        /* [in] */ IMotionEvent* event);

private:
    /*
    static <T> T checkNull(
        / [in] / String name,
        / [in] / T* arg);
    */

    CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitMdownpointerid();

    CARAPI_(AutoPtr< ArrayOf<Float> >) MiddleInitMdownx();

    CARAPI_(AutoPtr< ArrayOf<Float> >) MiddleInitMdowny();

    CARAPI_(AutoPtr< ArrayOf<Int64> >) MiddleInitMdowntime();

    CARAPI_(void) CaptureDown(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 pointerIndex);

    CARAPI_(Int32) FindIndex(
        /* [in] */ Int32 pointerId);

    CARAPI_(Int32) DetectSwipe(
        /* [in] */ IMotionEvent* move);

    CARAPI_(Int32) DetectSwipe(
        /* [in] */ Int32 i,
        /* [in] */ Int64 time,
        /* [in] */ Float x,
        /* [in] */ Float y);

public:
    Int32 screenHeight;
    Int32 screenWidth;

private:
    static const String TAG;
    static const Boolean DEBUG;
    static const Int64 SWIPE_TIMEOUT_MS = 500;
    static const Int32 MAX_TRACKED_POINTERS = 32;
    // max per input system
    static const Int32 UNTRACKED_POINTER = -1;
    static const Int32 SWIPE_NONE = 0;
    static const Int32 SWIPE_FROM_TOP = 1;
    static const Int32 SWIPE_FROM_BOTTOM = 2;
    static const Int32 SWIPE_FROM_RIGHT = 3;
    /*const*/ Int32 mSwipeStartThreshold;
    /*const*/ Int32 mSwipeDistanceThreshold;
    /*const*/ AutoPtr<ISystemGesturesPointerEventListenerCallbacks> mCallbacks;
    AutoPtr<ArrayOf<Int32> > mDownPointerId;
    AutoPtr<ArrayOf<Float> > mDownX;
    AutoPtr<ArrayOf<Float> > mDownY;
    AutoPtr<ArrayOf<Int64> > mDownTime;
    Int32 mDownPointers;
    Boolean mSwipeFireable;
    Boolean mDebugFireable;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_SYSTEMGESTURESPOINTEREVENTLISTENER_H__

