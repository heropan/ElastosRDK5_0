
#ifndef __CROUTEINFOHELPER_H__
#define __CROUTEINFOHELPER_H__

#include "_CRouteInfoHelper.h"

using Elastos::Net::IInetAddress;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CRouteInfoHelper)
{
public:
    CARAPI MakeHostRoute(
        /* [in] */ IInetAddress* host,
        /* [out] */ IRouteInfo** info);

    CARAPI MakeHostRoute(
        /* [in] */ IInetAddress* host,
        /* [in] */ IInetAddress* gateway,
        /* [out] */ IRouteInfo** info);

    /**
    * Find the route from a Collection of routes that best matches a given address.
    * May return null if no routes are applicable.
    * @param routes a Collection of RouteInfos to chose from
    * @param dest the InetAddress your trying to get to
    * @return the RouteInfo from the Collection that best fits the given address
    */
    //To-do: confirm the use of Collection<> (that is originally used here) in Elastos and replace List<>
    CARAPI SelectBestRoute(
        /* [in] */ IObjectContainer* routes,
        /* [in] */ IInetAddress* dest,
        /* [out] */ IRouteInfo** info);

};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __CROUTEINFOHELPER_H__
