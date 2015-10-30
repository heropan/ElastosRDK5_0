
#ifndef __ELASTOS_DROID_WEBKIT_CHTTPAUTHHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_CHTTPAUTHHANDLER_H__

#include "_Elastos_Droid_Webkit_CHttpAuthHandler.h"

#include "elastos/droid/webkit/HttpAuthHandler.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CHttpAuthHandler), public HttpAuthHandler
{
public:
    CAR_OBJECT_DECL()

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

#endif // __ELASTOS_DROID_WEBKIT_CHTTPAUTHHANDLER_H__
