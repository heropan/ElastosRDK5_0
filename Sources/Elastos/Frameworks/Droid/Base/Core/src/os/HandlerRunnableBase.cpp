#include "os/HandlerRunnableBase.h"

using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Os {

CAR_INTERFACE_IMPL_2(HandlerRunnableBase, IHandler, IRunnable)

IHANDLER_METHODS_IMPL(HandlerRunnableBase, Handler)

HandlerRunnableBase::HandlerRunnableBase(
    /* [in] */ Boolean async)
{
    Handler::Init(async);
}

HandlerRunnableBase::HandlerRunnableBase(
    /* [in] */ ILooper* looper,
    /* [in] */ Boolean async)
    : Handler(looper, async)
{
}

HandlerRunnableBase::HandlerRunnableBase(
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
    : Handler(callback, takeStrongRefOfCallback, async)
{
}

HandlerRunnableBase::HandlerRunnableBase(
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
    : Handler(looper, callback, takeStrongRefOfCallback, async)
{
}

HandlerRunnableBase::~HandlerRunnableBase()
{
}

ECode HandlerRunnableBase::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
