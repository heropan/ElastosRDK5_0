
#ifndef __PROVIDERREQUEST_H__
#define __PROVIDERREQUEST_H__

#include "ext/frameworkext.h"
#include "_CProviderRequest.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::Mutex;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Location {

/** @hide */
CarClass(CProviderRequest)
{
public:
    CARAPI constructor();

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI ToString(
        /* [out] */ String* strOut);

    CARAPI GetReportLocation(
        /* [out] */ Boolean* reportLocation);

    CARAPI SetReportLocation(
        /* [in] */ Boolean reportLocation);

    CARAPI GetInterval(
        /* [out] */ Int64* interval);

    CARAPI SetInterval(
        /* [in] */ Int64 interval);

    CARAPI GetLocationRequests(
        /* [out] */ IObjectContainer** requests);

    CARAPI SetLocationRequest(
        /* [in] */ ILocationRequest* request);

public:
    /** Location reporting is requested (true) */
    Boolean mReportLocation;// = false;

    /** The smallest requested interval */
    Int64 mInterval;// = Long.MAX_VALUE;

    /**
     * A more detailed set of requests.
     * <p>Location Providers can optionally use this to
     * fine tune location updates, for example when there
     * is a high power slow interval request and a
     * low power fast interval request.
     */
    List<AutoPtr<ILocationRequest> > mLocationRequests;

};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__PROVIDERREQUEST_H__
