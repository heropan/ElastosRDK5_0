#ifndef __ELASTOS_DROID_SERVER_INPUT_InputApplicationHandle_H__
#define __ELASTOS_DROID_SERVER_INPUT_InputApplicationHandle_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/core/Object.h"

using Elastos::Droid::Server::Input::IInputApplicationHandle;

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

    InputApplicationHandle();

    virtual ~InputApplicationHandle();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInterface* appWindowToken);
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

private:
    void NativeDispose();
};

} // Input
} // Server
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_SERVER_INPUT_InputApplicationHandle_H__
