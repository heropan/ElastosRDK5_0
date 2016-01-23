#ifndef __ELASTOS_DROID_SERVER_INPUT_InputApplicationHandle_H__
#define __ELASTOS_DROID_SERVER_INPUT_InputApplicationHandle_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/core/Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

class InputApplicationHandle
    : public Object
    , public IInputApplicationHandle
{
public:
    CAR_INTERFACE_DECL();

    InputApplicationHandle(
        /* [in] */ IInterface* appWindowToken);

    virtual ~InputApplicationHandle();

private:
    CARAPI_(void) NativeDispose();

private:
    // Pointer to the native input application handle.
    // This field is lazily initialized via JNI.
    //@SuppressWarnings("unused")
    Int64 mPtr;

    // The window manager's application window token.
    AutoPtr<IInterface> mAppWindowToken;

    // Application name.
    String mName;

    // Dispatching timeout.
    Int64 mDispatchingTimeoutNanos;
};

} // Input
} // Server
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_SERVER_INPUT_InputApplicationHandle_H__
