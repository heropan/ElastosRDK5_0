
#ifndef __ELASTOS_DROID_NET_CNETWORKTEMPLATE_H__
#define __ELASTOS_DROID_NET_CNETWORKTEMPLATE_H__

#include "_CNetworkTemplate.h"
#include <elastos/Mutex.h>


namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkTemplate)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 matchRule,
        /* [in] */ const String& subscriberId,
        /* [in] */ const String& networkId);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI EqualsEx(
        /* [in] */ INetworkTemplate* obj,
        /* [out] */ Boolean* result);

    CARAPI GetMatchRule(
        /* [out] */ Int32* result);

    CARAPI GetSubscriberId(
        /* [out] */ String* result);

    CARAPI GetNetworkId(
        /* [out] */ String* result);

    CARAPI Matches(
        /* [in] */ INetworkIdentity* ident,
        /* [out] */ Boolean* result);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    static CARAPI ForceAllNetworkTypes();

    static CARAPI BuildTemplateMobileAll(
        /* [in] */ const String& subscriberId,
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateMobile3gLower(
        /* [in] */ const String& subscriberId,
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateMobile4g(
        /* [in] */ const String& subscriberId,
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateMobileWildcard(
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateWifiWildcard(
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateWifi(
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateWifiEx(
        /* [in] */ const String& networkId,
        /* [out] */ INetworkTemplate** result);

    static CARAPI BuildTemplateEthernet(
        /* [out] */ INetworkTemplate** result);

private:
    CARAPI_(Boolean) MatchesMobile(
        /* [in] */ INetworkIdentity* ident);

    CARAPI_(Boolean) MatchesMobile3gLower(
        /* [in] */ INetworkIdentity* ident);

    CARAPI_(Boolean) MatchesMobile4g(
        /* [in] */ INetworkIdentity* ident);

    CARAPI_(Boolean) MatchesWifi(
        /* [in] */ INetworkIdentity* ident);

    CARAPI_(Boolean) MatchesEthernet(
        /* [in] */ INetworkIdentity* ident);

    CARAPI_(Boolean) MatchesMobileWildcard(
        /* [in] */ INetworkIdentity* ident);

    CARAPI_(Boolean) MatchesWifiWildcard(
        /* [in] */ INetworkIdentity* ident);

    static CARAPI_(String) GetMatchRuleName(
        /* [in] */ Int32 matchRule);

    static CARAPI EnsureSubtypeAvailable();


private:

    /**
     * Set of {@link NetworkInfo#getType()} that reflect data usage.
     */
    static AutoPtr< ArrayOf<Int32> > DATA_USAGE_NETWORK_TYPES;

    static Boolean sForceAllNetworkTypes;
    Int32 mMatchRule;
    String mSubscriberId;
    String mNetworkId;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKTEMPLATE_H__

