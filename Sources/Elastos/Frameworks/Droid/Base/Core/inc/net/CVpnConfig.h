
#ifndef __CVPNCONFIG_H__
#define __CVPNCONFIG_H__

#include "_CVpnConfig.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CarClass(CVpnConfig)
{
public:
    CVpnConfig();

    CARAPI constructor();

    CARAPI GetUser(
        /* [out] */ String* user);

    CARAPI SetUser(
        /* [in] */ const String& user);

    CARAPI GetInterfaze(
        /* [out] */ String* interfaze);

    CARAPI SetInterfaze(
        /* [in] */ const String& interfaze);

    CARAPI GetSession(
        /* [out] */ String* session);

    CARAPI SetSession(
        /* [in] */ const String& session);

    CARAPI GetMtu(
        /* [out] */ Int32* mtu);

    CARAPI SetMtu(
        /* [in] */ Int32 mtu);

    CARAPI GetAddresses(
        /* [out] */ String* addresses);

    CARAPI SetAddresses(
        /* [in] */ const String& addresses);

    CARAPI GetRoutes(
        /* [out] */ String* routes);

    CARAPI SetRoutes(
        /* [in] */ const String& routes);

    CARAPI GetDnsServers(
        /* [out] */ IObjectContainer** dnsServers);

    CARAPI SetDnsServers(
        /* [in] */ IObjectContainer* dnsServers);

    CARAPI GetSearchDomains(
        /* [out] */ IObjectContainer** searchDomains);

    CARAPI SetSearchDomains(
        /* [in] */ IObjectContainer* searchDomains);

    CARAPI GetConfigureIntent(
        /* [out] */ IPendingIntent** configureIntent);

    CARAPI SetConfigureIntent(
        /* [in] */ IPendingIntent* configureIntent);

    CARAPI GetStartTime(
        /* [out] */ Int64* startTime);

    CARAPI SetStartTime(
        /* [in] */ Int64 startTime);

    CARAPI GetLegacy(
        /* [out] */ Boolean* legacy);

    CARAPI SetLegacy(
        /* [in] */ Boolean legacy);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    static CARAPI GetIntentForConfirmation(
        /* [out] */ IIntent** intent);

    static CARAPI GetIntentForStatusPanel(
        /* [in] */ IContext* context,
        /* [in] */ IVpnConfig* config,
        /* [out] */ IPendingIntent** intent);

public:
    String mUser;
    String mInterfaze;
    String mSession;
    Int32 mMtu;
    String mAddresses;
    String mRoutes;
    AutoPtr<IObjectContainer> mDnsServers;
    AutoPtr<IObjectContainer> mSearchDomains;
    AutoPtr<IPendingIntent> mConfigureIntent;
    Int64 mStartTime;
    Boolean mLegacy;
};

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CVPNCONFIG_H__
