
#ifndef __CNETWORKTEMPLATEHELPER_H__
#define __CNETWORKTEMPLATEHELPER_H__

#include "_CNetworkTemplateHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkTemplateHelper)
{
public:
    CARAPI ForceAllNetworkTypes();

    CARAPI BuildTemplateMobileAll(
        /* [in] */ const String& subscriberId,
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateMobile3gLower(
        /* [in] */ const String& subscriberId,
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateMobile4g(
        /* [in] */ const String& subscriberId,
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateMobileWildcard(
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateWifiWildcard(
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateWifi(
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateWifi(
        /* [in] */ const String& networkId,
        /* [out] */ INetworkTemplate** result);

    CARAPI BuildTemplateEthernet(
        /* [out] */ INetworkTemplate** result);

};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __CNETWORKTEMPLATEHELPER_H__
