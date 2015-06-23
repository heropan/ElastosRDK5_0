#ifndef __CUSERHANDLE_H__
#define __CUSERHANDLE_H__

#include "_CUserHandle.h"
#include "os/UserHandle.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CUserHandle), public UserHandle
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 h);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetIdentifier(
            /* [out] */ Int32* id);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Equals(
        /* [in] */ IUserHandle* other,
        /* [out] */ Boolean* equals);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* equals);

    CARAPI GetHashCode(
        /* [out] */ Int32 * hash);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CUSERHANDLE_H__
