
#include "elastos/droid/webkit/CHttpAuthHandler.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

PInterface CHttpAuthHandler::Probe(
    /* [in] */ REIID riid)
{
    return _CHttpAuthHandler::Probe(riid);
}

IHANDLER_METHODS_IMPL(CHttpAuthHandler, Handler);

ECode CHttpAuthHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return HttpAuthHandler::HandleMessage(msg);
}

ECode CHttpAuthHandler::UseHttpAuthUsernamePassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return HttpAuthHandler::UseHttpAuthUsernamePassword(result);
}

ECode CHttpAuthHandler::Cancel()
{
    return  HttpAuthHandler::Cancel();
}

ECode CHttpAuthHandler::Proceed(
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    return HttpAuthHandler::Proceed(username, password);
}

ECode CHttpAuthHandler::SuppressDialog(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return HttpAuthHandler::SuppressDialog(result);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
