#include "os/HandlerBase.h"

using Elastos::Droid::Os::EIID_IHandler;

namespace Elastos {
namespace Droid {
namespace Os {

CAR_INTERFACE_IMPL_2(HandlerBase, IHandler, IWeakReferenceSource)

IHANDLER_METHODS_IMPL(HandlerBase, Handler)

HandlerBase::HandlerBase(
    /* [in] */ Boolean async)
{
    Handler::Init(async);
}

HandlerBase::HandlerBase(
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
    : Handler(callback, takeStrongRefOfCallback, async)
{
}

HandlerBase::HandlerBase(
    /* [in] */ ILooper* looper,
    /* [in] */ Boolean async)
    : Handler(looper, async)
{
}

HandlerBase::HandlerBase(
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
    : Handler(looper, callback, takeStrongRefOfCallback, async)
{
}

HandlerBase::~HandlerBase()
{
}

ECode HandlerBase::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode HandlerBase::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
