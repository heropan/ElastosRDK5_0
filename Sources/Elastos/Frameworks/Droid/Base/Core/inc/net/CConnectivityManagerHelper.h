
#ifndef __CCONNECTIVITYMANAGERHELPER_H__
#define __CCONNECTIVITYMANAGERHELPER_H__

#include "_CConnectivityManagerHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CConnectivityManagerHelper)
{
public:
    CARAPI IsNetworkTypeValid(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    CARAPI GetNetworkTypeName(
        /* [in] */ Int32 networkType,
        /* [out] */ String* result);

    CARAPI  IsNetworkTypeMobile(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    CARAPI  From(
        /* [in] */ IContext* ctx,
        /* [out] */ IConnectivityManager** result);

private:
    // TODO: Add your private member variables here.
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __CCONNECTIVITYMANAGERHELPER_H__
