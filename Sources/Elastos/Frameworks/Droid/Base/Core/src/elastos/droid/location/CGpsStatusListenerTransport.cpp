
#include "elastos/droid/location/CGpsStatusListenerTransport.h"
#include "elastos/droid/location/LocationManager.h"

namespace Elastos {
namespace Droid {
namespace Location {

const Int32 CGpsStatusListenerTransport::NMEA_RECEIVED = 1000;

ECode CGpsStatusListenerTransport::GpsHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    if (what == CGpsStatusListenerTransport::NMEA_RECEIVED) {
        mHost->HandleNmeaReceived();
    }
    else {
        mHost->HandleGpsChanged(what);
    }

    return NOERROR;
}

CGpsStatusListenerTransport::Nmea::Nmea(
    /* [in] */ Int64 timestamp,
    /* [in] */ const String& nmea)
    : mTimestamp(timestamp)
    , mNmea(nmea)
{}

ECode CGpsStatusListenerTransport::constructor(
    /* [in] */ Handle32 host,
    /* [in] */ IGpsStatusListener* listener)
{
    mHost = (LocationManager*)host;
    mListener = listener;
    mGpsHandler = new GpsHandler(this);
    return NOERROR;
}

ECode CGpsStatusListenerTransport::constructor(
    /* [in] */ Handle32 host,
    /* [in] */ IGpsStatusNmeaListener* listener)
{
    mHost = (LocationManager*)host;
    mNmeaListener = listener;
    mGpsHandler = new GpsHandler(this);
    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnGpsStarted()
{
    if (mListener != NULL) {
        Boolean result;
        return mGpsHandler->SendEmptyMessage(IGpsStatus::GpsStatus_GPS_EVENT_STARTED, &result);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnGpsStopped()
{
    if (mListener != NULL) {
        Boolean result;
        return mGpsHandler->SendEmptyMessage(IGpsStatus::GpsStatus_GPS_EVENT_STOPPED, &result);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnFirstFix(
    /* [in] */ Int32 ttff)
{
    if (mListener != NULL) {
        mHost->mGpsStatus->SetTimeToFirstFix(ttff);
        Boolean result;
        return mGpsHandler->SendEmptyMessage(IGpsStatus::GpsStatus_GPS_EVENT_FIRST_FIX, &result);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnSvStatusChanged(
    /* [in] */ Int32 svCount,
    /* [in] */ ArrayOf<Int32>* prns,
    /* [in] */ ArrayOf<Float>* snrs,
    /* [in] */ ArrayOf<Float>* elevations,
    /* [in] */ ArrayOf<Float>* azimuths,
    /* [in] */ Int32 ephemerisMask,
    /* [in] */ Int32 almanacMask,
    /* [in] */ Int32 usedInFixMask)
{
    if (mListener != NULL) {
        mHost->mGpsStatus->SetStatus(svCount, prns, snrs, elevations, azimuths,
                ephemerisMask, almanacMask, usedInFixMask);

        mGpsHandler->RemoveMessages(IGpsStatus::GpsStatus_GPS_EVENT_SATELLITE_STATUS);
        Boolean result;
        return mGpsHandler->SendEmptyMessage(IGpsStatus::GpsStatus_GPS_EVENT_SATELLITE_STATUS, &result);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnNmeaReceived(
    /* [in] */ Int64 timestamp,
    /* [in] */ const String& nmea)
{
    if (mNmeaListener != NULL) {
        {
            AutoLock lock(mNmeaBufferLock);

            AutoPtr<Nmea> nmeaObj = new Nmea(timestamp, nmea);
            mNmeaBuffer.PushBack(nmeaObj);
        }


        mGpsHandler->RemoveMessages(NMEA_RECEIVED);
        Boolean result;
        return mGpsHandler->SendEmptyMessage(NMEA_RECEIVED, &result);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::HandleGpsChanged(
    /* [in] */ Int32 event)
{
    // synchronize on mGpsStatus to ensure the data is copied atomically.
    AutoLock lock(mHost->mGpsStatusLock);

    return mListener->OnGpsStatusChanged(event);
}

ECode CGpsStatusListenerTransport::HandleNmeaReceived()
{
    AutoLock lock(mNmeaBufferLock);

    List< AutoPtr<Nmea> >::Iterator it;
    for (it = mNmeaBuffer.Begin(); it != mNmeaBuffer.End(); ++it) {
        Nmea* nmea = *it;
        mNmeaListener->OnNmeaReceived(nmea->mTimestamp, nmea->mNmea);
    }
    mNmeaBuffer.Clear();
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
