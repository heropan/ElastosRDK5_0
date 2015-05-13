
#ifndef __CHTTPAUTHHANDLER_H__
#define __CHTTPAUTHHANDLER_H__

#include "_CHttpAuthHandler.h"
#include "os/HandlerMacro.h"
#include "webkit/HttpAuthHandler.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CHttpAuthHandler), public HttpAuthHandler
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IHANDLER_METHODS_DECL();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    CARAPI UseHttpAuthUsernamePassword(
        /* [out] */ Boolean* result);

    CARAPI Cancel();

    CARAPI Proceed(
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI SuppressDialog(
        /* [out] */ Boolean* result);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CHTTPAUTHHANDLER_H__
