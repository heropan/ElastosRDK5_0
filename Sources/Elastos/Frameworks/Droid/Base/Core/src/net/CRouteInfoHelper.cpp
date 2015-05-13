
#include "net/CRouteInfo.h"
#include "net/CRouteInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CRouteInfoHelper::MakeHostRoute(
    /* [in] */ IInetAddress* host,
    /* [out] */ IRouteInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CRouteInfo::MakeHostRouteEx(host, NULL, info);
}
        
ECode CRouteInfoHelper::MakeHostRouteEx(
    /* [in] */ IInetAddress* host,
    /* [in] */ IInetAddress* gateway,
    /* [out] */ IRouteInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CRouteInfo::MakeHostRouteEx(host, gateway, info);
}

/**
* Find the route from a Collection of routes that best matches a given address.
* May return null if no routes are applicable.
* @param routes a Collection of RouteInfos to chose from
* @param dest the InetAddress your trying to get to
* @return the RouteInfo from the Collection that best fits the given address
*/
//To-do: confirm the use of Collection<> (that is originally used here) in Elastos and replace List<>
ECode CRouteInfoHelper::SelectBestRoute(
    /* [in] */ IObjectContainer* routes,
    /* [in] */ IInetAddress* dest,
    /* [out] */ IRouteInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CRouteInfo::SelectBestRoute(routes, dest, info);
}

} //namespace Net
} //namespace Droid
} //namespace Elastos