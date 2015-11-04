
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/droid/webkit/WebViewFactory.h"
#include <elastos/core/AutoLock.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Core::AutoLock;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

// Default Provider factory class name.
// TODO: When the Chromium powered WebView is ready, it should be the default factory class.
const String  WebViewFactory::DEFAULT_WEBVIEW_FACTORY("android.webkit.WebViewClassic$Factory");
const String  WebViewFactory::CHROMIUM_WEBVIEW_FACTORY("com.android.webviewchromium.WebViewChromiumFactoryProvider");
const String  WebViewFactory::CHROMIUM_WEBVIEW_JAR("/system/framework/webviewchromium.jar");

const String  WebViewFactory::LOGTAG("WebViewFactory");

const Boolean WebViewFactory::DEBUG;

// Cache the factory both for efficiency, and ensure any one process gets all webviews from the
// same provider.
AutoPtr<IWebViewFactoryProvider> WebViewFactory::sProviderInstance;
Object WebViewFactory::sProviderLock;

AutoPtr<IWebViewFactoryProvider> WebViewFactory::GetProvider()
{
    AutoLock lock(sProviderLock);

    // For now the main purpose of this function (and the factory abstraction) is to keep
    // us honest and minimize usage of WebViewClassic internals when binding the proxy.
    if (sProviderInstance != NULL) return sProviderInstance;

    // For debug builds, we allow a system property to specify that we should use the
    // Chromium powered WebView. This enables us to switch between implementations
    // at runtime. For user (release) builds, don't allow this.
    Boolean bFlag;
    if (Build::IS_DEBUGGABLE && (SystemProperties::GetBoolean(String("webview.use_chromium"), FALSE, &bFlag), bFlag)) {
        assert(0);
//        StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskReads();
//        try {
//            sProviderInstance = LoadChromiumProvider();
//            if (DEBUG) Log.v(LOGTAG, "Loaded Chromium provider: " + sProviderInstance);
//        } finally {
//            StrictMode.setThreadPolicy(oldPolicy);
//        }
    }

    if (sProviderInstance == NULL) {
        if (DEBUG) Logger::V(LOGTAG, "Falling back to default provider: %s",
                DEFAULT_WEBVIEW_FACTORY.string());
        sProviderInstance = GetFactoryByName(DEFAULT_WEBVIEW_FACTORY,
                NULL/*WebViewFactory.class.GetClassLoader()*/);
        if (sProviderInstance == NULL) {
            if (DEBUG) Logger::V(LOGTAG, "Falling back to explicit linkage");
            assert(0);
            // TODO
            // sProviderInstance = new CWebViewClassic::Factory();
        }
    }
    return sProviderInstance;
}

// TODO: This allows us to have the legacy and Chromium WebView coexist for development
// and side-by-side testing. After transition, remove this when no longer required.
AutoPtr<IWebViewFactoryProvider> WebViewFactory::LoadChromiumProvider()
{
    assert(0);
//    ClassLoader clazzLoader = new PathClassLoader(CHROMIUM_WEBVIEW_JAR, null,
//            WebViewFactory.class.getClassLoader());
//    return getFactoryByName(CHROMIUM_WEBVIEW_FACTORY, clazzLoader);
    return NULL;
}

AutoPtr<IWebViewFactoryProvider> WebViewFactory::GetFactoryByName(
    /* [in] */ const String& providerName,
    /* [in] */ IClassLoader* loader)
{
//    try {
//        if (DEBUG) Log.v(LOGTAG, "attempt to load class " + providerName);
//        Class<?> c = Class.forName(providerName, true, loader);
//        if (DEBUG) Log.v(LOGTAG, "instantiating factory");
//        return (WebViewFactoryProvider) c.newInstance();
//    } catch (ClassNotFoundException e) {
//        Log.e(LOGTAG, "error loading " + providerName, e);
//    } catch (IllegalAccessException e) {
//        Log.e(LOGTAG, "error loading " + providerName, e);
//    } catch (InstantiationException e) {
//        Log.e(LOGTAG, "error loading " + providerName, e);
//    }
    AutoPtr<IWebViewFactoryProvider> provider;
    if (providerName.Equals(DEFAULT_WEBVIEW_FACTORY)) {
        assert(0);
        // TODO
        // provider = new CWebViewClassic::Factory();
    }
    return provider;
}

ECode WebViewFactory::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebViewFactory";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
