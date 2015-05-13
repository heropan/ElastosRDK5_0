
#ifndef __CMESSENGER_H__
#define __CMESSENGER_H__

#include "_CMessenger.h"

using Elastos::Droid::Os::IIMessenger;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CMessenger)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IHandler* handler);

    CARAPI constructor(
        /* [in] */ IIMessenger* target);

    CARAPI Send(
        /* [in] */ IMessage* message);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetIMessenger(
        /* [out] */ IIMessenger** mgr);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashcode);

    CARAPI EqualsEx(
        /* [in] */ IMessenger* other,
        /* [out] */ Boolean* equals);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* equals);

private:
    AutoPtr<IIMessenger> mTarget;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CMESSENGER_H__
