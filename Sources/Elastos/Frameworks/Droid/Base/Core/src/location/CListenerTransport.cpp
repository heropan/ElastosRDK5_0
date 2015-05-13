
#include "location/CListenerTransport.h"
#include "location/LocationManager.h"
#include "location/CLocation.h"
#include "os/CBundle.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CBundle;

namespace Elastos {
namespace Droid {
namespace Location {

const Int32 CListenerTransport::TYPE_LOCATION_CHANGED = 1;
const Int32 CListenerTransport::TYPE_STATUS_CHANGED = 2;
const Int32 CListenerTransport::TYPE_PROVIDER_ENABLED = 3;
const Int32 CListenerTransport::TYPE_PROVIDER_DISABLED = 4;

ECode CListenerTransport::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    mHost->HandleMessage(msg);

    return NOERROR;
}

ECode CListenerTransport::constructor(
    /* [in] */ Handle32 host,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    mHost = (LocationManager*)host;
    mListener = listener;

    if (looper == NULL) {
        mListenerHandler = new MyHandler(this);
    }
    else {
        mListenerHandler = new MyHandler(looper, this);
    }
    return NOERROR;
}

ECode CListenerTransport::OnLocationChanged(
    /* [in] */ ILocation* location)
{
    AutoPtr<IMessage> msg;
    mListenerHandler->ObtainMessageEx(TYPE_LOCATION_CHANGED, location, (IMessage**)&msg);
    Boolean result;
    return mListenerHandler->SendMessage(msg, &result);
}

ECode CListenerTransport::OnStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    AutoPtr<IBundle> b;
    CBundle::New((IBundle**)&b);
    b->PutString(String("provider"), provider);
    b->PutInt32(String("status"), status);
    if (extras != NULL) {
        b->PutBundle(String("extras"), extras);
    }

    AutoPtr<IMessage> msg;
    mListenerHandler->ObtainMessageEx(TYPE_STATUS_CHANGED, b, (IMessage**)&msg);
    Boolean result;
    return mListenerHandler->SendMessage(msg, &result);
}

ECode CListenerTransport::OnProviderEnabled(
    /* [in] */ const String& provider)
{
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(provider, (ICharSequence**)&seq);

    AutoPtr<IMessage> msg;
    mListenerHandler->ObtainMessageEx(TYPE_PROVIDER_ENABLED, seq, (IMessage**)&msg);
    Boolean result;
    return mListenerHandler->SendMessage(msg, &result);
}

ECode CListenerTransport::OnProviderDisabled(
    /* [in] */ const String& provider)
{
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(provider, (ICharSequence**)&seq);

    AutoPtr<IMessage> msg;
    mListenerHandler->ObtainMessageEx(TYPE_PROVIDER_DISABLED, seq, (IMessage**)&msg);
    Boolean result;
    return mListenerHandler->SendMessage(msg, &result);
}

ECode CListenerTransport::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case TYPE_LOCATION_CHANGED: {
            ILocation* location = ILocation::Probe(obj);
            AutoPtr<ILocation> nativeLocation;
            CLocation::New(location, (ILocation**)&nativeLocation);
            mListener->OnLocationChanged(nativeLocation);
            break;
        }
        case TYPE_STATUS_CHANGED: {
            IBundle* b = IBundle::Probe(obj);
            String provider;
            b->GetString(String("provider"), &provider);
            Int32 status;
            b->GetInt32(String("status"), &status);
            AutoPtr<IBundle> extras;
            b->GetBundle(String("extras"), (IBundle**)&extras);
            mListener->OnStatusChanged(provider, status, extras);
            break;
        }
        case TYPE_PROVIDER_ENABLED: {
            ICharSequence* seq = ICharSequence::Probe(obj);
            String provider;
            seq->ToString(&provider);
            mListener->OnProviderEnabled(provider);
            break;
        }

        case TYPE_PROVIDER_DISABLED: {
            ICharSequence* seq = ICharSequence::Probe(obj);
            String provider;
            seq->ToString(&provider);
            mListener->OnProviderDisabled(provider);
            break;
        }
    }

    // try {
    return mHost->mService->LocationCallbackFinished((IILocationListener*)(this->Probe(EIID_IILocationListener)));
    // } catch (RemoteException e) {
    //     Log.e(TAG, "locationCallbackFinished: RemoteException", e);
    // }
    return NOERROR;
}


ECode CListenerTransport::ToString(
    /* [out] */ String* str)
{
    *str = String("CListenerTransport");
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
