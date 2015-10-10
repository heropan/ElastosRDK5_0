
#ifndef __ELASTOS_DROID_WEBKIT_CURLINTERCEPTREGISTRY_H__
#define __ELASTOS_DROID_WEBKIT_CURLINTERCEPTREGISTRY_H__

#include "_Elastos_Droid_Webkit_CUrlInterceptRegistry.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CUrlInterceptRegistry)
{
public:
    CARAPI SetUrlInterceptDisabled(
        /* [in] */ Boolean disabled);

    CARAPI UrlInterceptDisabled(
        /* [out] */ Boolean* disabled);

    CARAPI RegisterHandler(
        /* [in] */ IUrlInterceptHandler* handler,
        /* [out] */ Boolean* result);

    CARAPI UnregisterHandler(
        /* [in] */ IUrlInterceptHandler* handler,
        /* [out] */ Boolean* result);

    CARAPI GetSurrogate(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers,
        /* [out] */ ICacheManagerCacheResult** cacheResult);

    CARAPI GetPluginData(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers,
        /* [out] */ IPluginData** pluginData);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CURLINTERCEPTREGISTRY_H__
