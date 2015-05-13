#ifndef __CGPSNIRESPONSE_H__
#define __CGPSNIRESPONSE_H__
#include "_CGpsNiResponse.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Location {

CarClass(CGpsNiResponse)
{
public:
    CGpsNiResponse();

    CARAPI constructor();

    /* User reponse, one of the values in GpsUserResponseType */
    CARAPI GetUserResponse(
        /* [out] */ Int32* userResponse);

    CARAPI SetUserResponse(
        /* [in] */ Int32 userResponse);

    /* Optional extra data to pass with the user response */
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

private:

    Int32 mUserResponse;
    AutoPtr<IBundle> mExtras;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif
