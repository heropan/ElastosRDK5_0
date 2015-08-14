
#ifndef __ELASTOS_DROID_INTERNAL_NET_CLEGACYVPNINFO_H__
#define __ELASTOS_DROID_INTERNAL_NET_CLEGACYVPNINFO_H__

#include "_CLegacyVpnInfo.h"

using Elastos::Droid::App::IPendingIntent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CarClass(CLegacyVpnInfo)
{
public:
    CLegacyVpnInfo();

    CARAPI constructor();

    CARAPI GetKey(
        /* [out] */ String* key);

    CARAPI SetKey(
        /* [in] */ const String& key);

    CARAPI GetState(
        /* [out] */ Int32* state);

    CARAPI SetState(
        /* [in] */ Int32 state);

    CARAPI GetIntent(
        /* [out] */ IPendingIntent** intent);

    CARAPI SetIntent(
        /* [in] */ IPendingIntent* intent);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    String mKey;
    Int32 mState;
    AutoPtr<IPendingIntent> mIntent;
};

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_NET_CLEGACYVPNINFO_H__
