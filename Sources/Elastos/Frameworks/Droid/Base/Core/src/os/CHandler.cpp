
#include "os/CHandler.h"

namespace Elastos {
namespace Droid {
namespace Os {

IHANDLER_METHODS_IMPL(CHandler, Handler)

ECode CHandler::constructor()
{
    return Handler::Init();
}

ECode CHandler::constructor(
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback)
{
    return Handler::Init(callback, takeStrongRefOfCallback);
}

ECode CHandler::constructor(
    /* [in] */ ILooper* looper)
{
    return Handler::Init(looper);
}

ECode CHandler::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback)
{
    return Handler::Init(looper, callback, takeStrongRefOfCallback);
}

ECode CHandler::constructor(
    /* [in] */ Boolean async)
{
    return Handler::Init(async);
}

ECode CHandler::constructor(
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
{
    return Handler::Init(callback, takeStrongRefOfCallback, async);
}

ECode CHandler::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
{
    return Handler::Init(looper, callback, takeStrongRefOfCallback, async);
}

PInterface CHandler::Probe(
    /* [in] */ REIID riid)
{
    return _CHandler::Probe(riid);
}

ECode CHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return Handler::HandleMessage(msg);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
