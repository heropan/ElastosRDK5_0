#ifndef __CACCESSIBILITYSERVICECLIENTWRAPPER_H__
#define __CACCESSIBILITYSERVICECLIENTWRAPPER_H__

#include "_CAccessibilityServiceClientWrapper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IHandlerCaller;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;


namespace Elastos {
namespace Droid {
namespace AccessibilityService {

CarClass(CAccessibilityServiceClientWrapper)
{
public:
    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ ILooper * looper,
        /* [in] */ IAccessibilityServiceCallbacks* callback);

    CARAPI SetConnection(
        /* [in] */ IAccessibilityServiceConnection* connection,
        /* [in] */ Int32 connectionId);

    CARAPI OnInterrupt();

    CARAPI OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnGesture(
        /* [in] */ Int32 gestureId);

    CARAPI ExecuteMessage(
        /* [in] */ IMessage* message);

    CARAPI ToString(
        /* [out] */ String* info);

public:
    static const Int32 DO_SET_SET_CONNECTION;
    static const Int32 DO_ON_INTERRUPT;
    static const Int32 DO_ON_ACCESSIBILITY_EVENT;
    static const Int32 DO_ON_GESTURE;

    AutoPtr<IHandlerCaller> mCaller;

    AutoPtr<IAccessibilityServiceCallbacks> mCallback;
};

}
}
}

#endif // __CACCESSIBILITYSERVICECLIENTWRAPPER_H__
