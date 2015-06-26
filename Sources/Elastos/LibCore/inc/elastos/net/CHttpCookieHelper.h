#ifndef __ELASTOS_NET_CHTTPCOOKIEHELPER_H__
#define __ELASTOS_NET_CHTTPCOOKIEHELPER_H__

#include "_Elastos_Net_CHttpCookieHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Net {

CarClass(CHttpCookieHelper)
    , public Singleton
    , public IHttpCookieHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI IsDomainMatches(
        /* [in] */ const String& domainPattern,
        /* [in] */ const String& host,
        /* [out] */ Boolean* match);

private:
    /**
     * Returns true if {@code s.substring(firstCharacter)} contains a dot
     * between its first and last characters, exclusive. This considers both
     * {@code android.com} and {@code co.uk} to be fully qualified domain names,
     * but not {@code android.com.}, {@code .com}. or {@code android}.
     *
     * <p>Although this implements the cookie spec's definition of FQDN, it is
     * not general purpose. For example, this returns true for IPv4 addresses.
     */
    CARAPI_(Boolean) IsFullyQualifiedDomainName(
        /* [in] */ const String& s,
        /* [in] */ Int32 firstCharacter);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CHTTPCOOKIEHELPER_H__
