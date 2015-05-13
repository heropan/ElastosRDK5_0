
#ifndef __URLINTERCEPTREGISTRY_H__
#define __URLINTERCEPTREGISTRY_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::Utility::List;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

class UrlInterceptRegistry
{
public:
    static CARAPI_(void) SetUrlInterceptDisabled(
        /* [in] */ Boolean disabled);

    static CARAPI_(Boolean) UrlInterceptDisabled();

    static CARAPI_(Boolean) RegisterHandler(
        /* [in] */ IUrlInterceptHandler* handler);

    static CARAPI_(Boolean) UnregisterHandler(
        /* [in] */ IUrlInterceptHandler* handler);

    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) GetSurrogate(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers);

    static CARAPI_(AutoPtr<IPluginData>) GetPluginData(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers);

private:
    static String LOGTAG;

    static Boolean mDisabled;

    static List< AutoPtr<IInterface> > mHandlerList;

    static Mutex sLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__URLINTERCEPTREGISTRY_H__