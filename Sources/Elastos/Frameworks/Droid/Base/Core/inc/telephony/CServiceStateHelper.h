
#ifndef __CSERVICESTATEHELPER_H__
#define __CSERVICESTATEHELPER_H__

#include "_CServiceStateHelper.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Telephony {

CarClass(CServiceStateHelper)
{
public:
    CARAPI NewFromBundle(
        /* [in] */ IBundle* m,
        /* [out] */ IServiceState** res);

    CARAPI RilRadioTechnologyToString(
        /* [in] */ Int32 rt,
        /* [out] */ String* res);

    CARAPI IsGsm(
        /* [in] */ Int32 radioTechnology,
        /* [out] */ Boolean* res);

    CARAPI IsCdma(
        /* [in] */ Int32 radioTechnology,
        /* [out] */ Boolean* res);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CSERVICESTATEHELPER_H__
