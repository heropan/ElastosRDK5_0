#include "location/CGpsNiResponse.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Location {

/* User reponse, one of the values in GpsUserResponseType */
CGpsNiResponse::CGpsNiResponse()
              : mUserResponse(0)
{}

ECode CGpsNiResponse::constructor()
{
    return NOERROR;
}

ECode CGpsNiResponse::GetUserResponse(
    /* [out] */ Int32* userResponse)
{
    VALIDATE_NOT_NULL(userResponse);
    *userResponse = mUserResponse;
    return NOERROR;
}

ECode CGpsNiResponse::SetUserResponse(
    /* [in] */ Int32 userResponse)
{
    mUserResponse = userResponse;
    return NOERROR;
}

/* Optional extra data to pass with the user response */
ECode CGpsNiResponse::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;
    INTERFACE_ADDREF(*extras);
    return NOERROR;
}

ECode CGpsNiResponse::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
