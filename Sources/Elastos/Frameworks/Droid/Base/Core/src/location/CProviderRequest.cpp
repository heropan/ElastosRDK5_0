
#include "location/CProviderRequest.h"
#include "location/CLocationRequest.h"
#include "util/CParcelableObjectContainer.h"
#include "util/TimeUtils.h"
#include <elastos/Math.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Utility::TimeUtils;

namespace Elastos {
namespace Droid {
namespace Location {

ECode CProviderRequest::constructor()
{
    mReportLocation = FALSE;
    mInterval = Elastos::Core::Math::INT64_MAX_VALUE;
    return NOERROR;
}

//@Override
ECode CProviderRequest::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

ECode CProviderRequest::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

    parcel->ReadBoolean(&mReportLocation);
    parcel->ReadInt64(&mInterval);

    Int32 count = 0;
    parcel->ReadInt32(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<ILocationRequest> request;
        CLocationRequest::New((ILocationRequest**)&request);
        IParcelable::Probe(request)->ReadFromParcel(parcel);
        mLocationRequests.PushBack(request);
    }

    return NOERROR;
}

//@Override
ECode CProviderRequest::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

    parcel->WriteBoolean(mReportLocation);
    parcel->WriteInt64(mInterval);

    parcel->WriteInt32(mLocationRequests.GetSize());
    List<AutoPtr<ILocationRequest> >::Iterator iter = mLocationRequests.Begin();
    while (iter != mLocationRequests.End()) {
        AutoPtr<IParcelable> temp = (IParcelable*)((*iter)->Probe(EIID_IParcelable));
        temp->WriteToParcel(parcel);
        iter++;
    }

    return NOERROR;
}

ECode CProviderRequest::GetReportLocation(
    /* [out] */ Boolean* reportLocation)
{
    VALIDATE_NOT_NULL(reportLocation);
    *reportLocation = mReportLocation;
    return NOERROR;
}

ECode CProviderRequest::SetReportLocation(
    /* [in] */ Boolean reportLocation)
{
    mReportLocation = reportLocation;
    return NOERROR;
}

ECode CProviderRequest::GetInterval(
    /* [out] */ Int64* interval)
{
    *interval = mInterval;
    return NOERROR;
}

ECode CProviderRequest::SetInterval(
    /* [in] */ Int64 interval)
{
    mInterval = interval;
    return NOERROR;
}

ECode CProviderRequest::GetLocationRequests(
    /* [out] */ IObjectContainer** requests)
{
    VALIDATE_NOT_NULL(requests);
    AutoPtr<IObjectContainer> array;
    CParcelableObjectContainer::New((IObjectContainer**)&array);

    List<AutoPtr<ILocationRequest> >::Iterator it = mLocationRequests.Begin();
    for (; it != mLocationRequests.End(); ++it) {
        array->Add((*it).Get());
    }
    *requests = array;
    INTERFACE_ADDREF(*requests);
    return NOERROR;
}

ECode CProviderRequest::SetLocationRequest(
    /* [in] */ ILocationRequest* request)
{
    mLocationRequests.PushBack(request);
    return NOERROR;
}

//@Override
ECode CProviderRequest::ToString(
    /* [out] */ String* strOut)
{
    VALIDATE_NOT_NULL(strOut);

    StringBuilder sb("ProviderRequest[");
    if (mReportLocation) {
        sb += "ON";
        sb += " mInterval=";
        TimeUtils::FormatDuration(mInterval, sb);
    } else {
        sb += "OFF";
    }
    sb += "]";

    *strOut = sb.ToString();

    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
