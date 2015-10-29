
#include "elastos/droid/location/CountryDetector.h"
#include "elastos/droid/os/CHandler.h"
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Os::CHandler;
using Elastos::Core::AutoLock;

namespace Elastos {
namespace Droid {
namespace Location {
//CountryDetector::CountryDetectorListenerTransport::CountryDetectorListenerTransportRunnnable
CountryDetector::CountryDetectorListenerTransport::CountryDetectorListenerTransportRunnnable::CountryDetectorListenerTransportRunnnable(
    /* [in] */ ICountry* country,
    /* [in] */ CountryDetectorListenerTransport* host)
    : mCountry(country)
    , mHost(host)
{}

ECode CountryDetector::CountryDetectorListenerTransport::CountryDetectorListenerTransportRunnnable::Run()
{
    mHost->mListener->OnCountryDetected(mCountry);
    return NOERROR;
}

//CountryDetector::CountryDetectorListenerTransport
CAR_INTERFACE_IMPL_2(CountryDetector::CountryDetectorListenerTransport, Object, ICountryDetectorListenerTransport, IICountryListener)

CountryDetector::CountryDetectorListenerTransport::CountryDetectorListenerTransport(
    /* [in] */ ICountryListener* listener,
    /* [in] */ ILooper* looper)
        : mListener(listener)
{
    if (looper != NULL) {
        CHandler::New(looper, (IHandler**)&mHandler);
    } else {
        CHandler::New((IHandler**)&mHandler);
    }
}

ECode CountryDetector::CountryDetectorListenerTransport::OnCountryDetected(
    /* [in] */ ICountry* country)
{
    AutoPtr<CountryDetectorListenerTransportRunnnable> r = new CountryDetectorListenerTransportRunnnable(country, this);
    Boolean result = FALSE;
    mHandler->Post(IRunnable::Probe(r), &result);
    return NOERROR;
}

//CountryDetector
const String CountryDetector::TAG("CountryDetector");

CAR_INTERFACE_IMPL(CountryDetector, Object, ICountryDetector)

CountryDetector::CountryDetector()
{}

CountryDetector::CountryDetector(
    /* [in] */ IICountryDetector* service)
{
    Init(service);
}

ECode CountryDetector::constructor(
    /* [in] */ IICountryDetector* service)
{
    return Init(service);
}

ECode CountryDetector::DetectCountry(
    /* [out] */ ICountry** country)
{
    VALIDATE_NOT_NULL(country);
    return mService->DetectCountry(country);
}


ECode CountryDetector::AddCountryListener(
    /* [in] */ ICountryListener* listener,
    /* [in] */ ILooper* looper)
{
    AutoLock lock(mListenersLock);

    HashMap<AutoPtr<ICountryListener>, AutoPtr<ICountryDetectorListenerTransport> >::Iterator iter
            = mListeners.Find(listener);
    if (iter == mListeners.End()) {
        AutoPtr<CountryDetectorListenerTransport> transport = new CountryDetectorListenerTransport(listener, looper);
        mService->AddCountryListener(IICountryListener::Probe(transport));
        mListeners[listener] = ICountryDetectorListenerTransport::Probe(transport);
    } else {
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}


ECode CountryDetector::RemoveCountryListener(
    /* [in] */ ICountryListener* listener)
{
    AutoLock lock(mListenersLock);

    HashMap<AutoPtr<ICountryListener>, AutoPtr<ICountryDetectorListenerTransport> >::Iterator iter
            = mListeners.Find(listener);
    AutoPtr<ICountryDetectorListenerTransport> transport;
    if (iter != mListeners.End()) {
        transport = iter->mSecond;
        mListeners.Erase(listener);
        mService->RemoveCountryListener(IICountryListener::Probe(transport));
    }
    else {
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CountryDetector::Init(
    /* [in] */ IICountryDetector* service)
{
    mService = service;
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos