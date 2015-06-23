
#include "webkit/UrlInterceptRegistry.h"
#include <elastos/utility/etl/Algorithm.h>

namespace Elastos {
namespace Droid {
namespace Webkit {

String UrlInterceptRegistry::LOGTAG("intercept");

Boolean UrlInterceptRegistry::mDisabled = FALSE;

List< AutoPtr<IInterface> > UrlInterceptRegistry::mHandlerList;

Mutex UrlInterceptRegistry::sLock;

void UrlInterceptRegistry::SetUrlInterceptDisabled(
    /* [in] */ Boolean disabled)
{
    Mutex::Autolock lock(sLock);
    mDisabled = disabled;
}

Boolean UrlInterceptRegistry::UrlInterceptDisabled()
{
    Mutex::Autolock lock(sLock);
    return mDisabled;
}

Boolean UrlInterceptRegistry::RegisterHandler(
    /* [in] */ IUrlInterceptHandler* handler)
{
    Mutex::Autolock lock(sLock);
    if (Find(mHandlerList.Begin(), mHandlerList.End(), handler) == mHandlerList.End()) {
        mHandlerList.PushFront(handler);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

Boolean UrlInterceptRegistry::UnregisterHandler(
    /* [in] */ IUrlInterceptHandler* handler)
{
    Mutex::Autolock lock(sLock);
    mHandlerList.Remove(handler);
    return TRUE;
}

AutoPtr<ICacheManagerCacheResult> UrlInterceptRegistry::GetSurrogate(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers)
{
    Mutex::Autolock lock(sLock);

    if (UrlInterceptDisabled()) {
        return NULL;
    }

    List< AutoPtr<IInterface> >::Iterator iter = mHandlerList.Begin();
    //Iterator iter = getHandlers().listIterator();mHandlerList
    for(; iter != mHandlerList.End(); ++iter)
    {
        AutoPtr<IUrlInterceptHandler> handler = (IUrlInterceptHandler*)(*iter).Get();
        AutoPtr<ICacheManagerCacheResult> result;
        handler->Service(url, headers, (ICacheManagerCacheResult**)&result);
        if (result != NULL) {
            return result;
        }
    }

    return NULL;
}

AutoPtr<IPluginData> UrlInterceptRegistry::GetPluginData(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers)
{
    Mutex::Autolock lock(sLock);

    if (UrlInterceptDisabled()) {
        return NULL;
    }

    List< AutoPtr<IInterface> >::Iterator iter = mHandlerList.Begin();
    for(; iter != mHandlerList.End(); ++iter) {
        AutoPtr<IUrlInterceptHandler> handler = (IUrlInterceptHandler*)(*iter).Get();
        AutoPtr<IPluginData> data;
        handler->GetPluginData(url, headers, (IPluginData**)&data);
        if (data != NULL) {
            return data;
        }
    }

    return NULL;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
