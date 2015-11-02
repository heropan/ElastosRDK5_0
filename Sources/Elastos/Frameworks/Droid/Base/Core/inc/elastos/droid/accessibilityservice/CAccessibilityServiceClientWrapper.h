#ifndef __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICECLIENTWRAPPER_H__
#define __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICECLIENTWRAPPER_H__

#include "_Elastos_Droid_AccessibilityService_CAccessibilityServiceClientWrapper.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Os::IHandlerCaller;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

CarClass(CAccessibilityServiceClientWrapper)
    , public Object
    , public IAccessibilityServiceClientWrapper
    , public IIAccessibilityServiceClient
    //, public IHandlerCallerCallback
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CAccessibilityServiceClientWrapper();

    ~CAccessibilityServiceClientWrapper();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* looper,
        /* [in] */ IAccessibilityServiceCallbacks* callback);

    CARAPI SetConnection(
        /* [in] */ IIAccessibilityServiceConnection* connection,
        /* [in] */ Int32 connectionId);

    CARAPI OnInterrupt();

    CARAPI OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnGesture(
        /* [in] */ Int32 gestureId);

    CARAPI ClearAccessibilityCache();

    CARAPI OnKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 sequence);

    CARAPI ExecuteMessage(
        /* [in] */ IMessage* message);

    CARAPI ToString(
        /* [out] */ String* info);

public:
    static const Int32 DO_SET_SET_CONNECTION;
    static const Int32 DO_ON_INTERRUPT;
    static const Int32 DO_ON_ACCESSIBILITY_EVENT;
    static const Int32 DO_ON_GESTURE;
    static const Int32 DO_CLEAR_ACCESSIBILITY_CACHE;
    static const Int32 DO_ON_KEY_EVENT;

    Int32 mConnectionId;

    AutoPtr<IHandlerCaller> mCaller;

    AutoPtr<IAccessibilityServiceCallbacks> mCallback;
};

}
}
}

#endif // __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICECLIENTWRAPPER_H__
