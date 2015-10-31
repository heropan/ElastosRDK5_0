
#include "elastos/droid/location/LocalListenerHelper.h"

namespace Elastos {
namespace Droid {
namespace Location {
CAR_INTERFACE_IMPL(LocalListenerHelper, Object, ILocalListenerHelper)

LocalListenerHelper::LocalListenerHelper(
    /* [in] */ const String& name)
        : mTag(name)
{
    // Preconditions.checkNotNull(name);
}

ECode LocalListenerHelper::Add(
    /* [in] */ IInterface* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // Preconditions.checkNotNull(listener);

    synchronized(this) {
        // we need to register with the service first, because we need to find out if the
        // service will actually support the request before we attempt anything
        if (mListeners.IsEmpty()) {
            Boolean registeredWithService = FALSE;

            ECode ec = RegisterWithServer(&registeredWithService);
            if (FAILED(ec)) {
                Logger::E(mTag, "Error handling first listener.");
                *result = FALSE;
                return E_REMOTE_EXCEPTION;
            }

            if (!registeredWithService) {
                Logger::E(mTag, "Unable to register listener transport.");
                *result = FALSE;
                return NOERROR;
            }
        }

        HashSet<IInterface* >::Iterator iter = mListeners.Find(listener);
        if (iter != mListeners.End()) {
            *result = TRUE;
            return NOERROR;
        }
        mListeners.Insert(listener);

        *result = TRUE;
    }
    return NOERROR;
}

ECode LocalListenerHelper::Remove(
    /* [in] */ IInterface* listener)
{
    // Preconditions.checkNotNull(listener);

    synchronized (this) {
        Boolean removed = FALSE;
        HashSet<IInterface* >::Iterator iter = mListeners.Find(listener);
        if (iter != mListeners.End()) {
            mListeners.Erase(listener);
            removed = TRUE;
        }
        Boolean isLastRemoved = removed && mListeners.IsEmpty();

        if (isLastRemoved) {
            ECode ec = UnregisterFromServer();
            if (FAILED(ec)) {
                return E_REMOTE_EXCEPTION;
            }
        }
    }
    return NOERROR;
}

ECode LocalListenerHelper::Foreach(
    /* [in] */ ILocalListenerHelperListenerOperation* operation)
{
#if 0 // Belows needs to be improved
    AutoPtr<ICollection> listeners;
    synchronized (this) {
        AutoPtr<IArrayList> sl;

        AutoPtr<ICollection> coll = ICollection::Probe(mListeners2);
        CArrayList::New(coll, (IArrayList**)&sl);
        listeners = ICollection::Probe(sl);
    }

    AutoPtr<IIterator> iter;
    listeners->GetIterator((IIterator**)&iter);
    for(;iter != NULL; ++iter) {
        ECode ec = operation->Execute(listener);
        if (FAILED(ec)) {
            Logger::E(mTag, "Error in monitored listener.");
            // don't return, give a fair chance to all listeners to receive the event
        }
    }
#endif
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
