
#include "elastos/droid/os/Build.h"
#include "elastos/droid/webkit/WebViewFactory.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::Build;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

const String WebViewFactory::CHROMIUM_WEBVIEW_FACTORY("com.android.webview.chromium.WebViewChromiumFactoryProvider");

const String WebViewFactory::NULL_WEBVIEW_FACTORY("com.android.webview.nullwebview.NullWebViewFactoryProvider");

const String WebViewFactory::CHROMIUM_WEBVIEW_NATIVE_RELRO_32("/data/misc/shared_relro/libwebviewchromium32.relro");

const String WebViewFactory::CHROMIUM_WEBVIEW_NATIVE_RELRO_64("/data/misc/shared_relro/libwebviewchromium64.relro");

const Int64 WebViewFactory::CHROMIUM_WEBVIEW_DEFAULT_VMSIZE_BYTES;

const String WebViewFactory::LOGTAG("WebViewFactory");

const Boolean WebViewFactory::DEBUG = FALSE;

// Cache the factory both for efficiency, and ensure any one process gets all webviews from the
// same provider.
AutoPtr<IWebViewFactoryProvider> WebViewFactory::sProviderInstance;
Object WebViewFactory::sProviderLock;
Boolean WebViewFactory::sAddressSpaceReserved = FALSE;
AutoPtr<IPackageInfo> WebViewFactory::sPackageInfo;

String WebViewFactory::GetWebViewPackageName()
{
    assert(0);
    // TODO
    // return AppGlobals.getInitialApplication().getString(
    //         com.android.internal.R.string.config_webViewPackageName);
    return String(NULL);
}

AutoPtr<IPackageInfo> WebViewFactory::GetLoadedPackageInfo()
{
    return sPackageInfo;
}

AutoPtr<IWebViewFactoryProvider> WebViewFactory::GetProvider()
{
    assert(0);
    // TODO
    // synchronized (sProviderLock) {
    //     // For now the main purpose of this function (and the factory abstraction) is to keep
    //     // us honest and minimize usage of WebView internals when binding the proxy.
    //     if (sProviderInstance != null) return sProviderInstance;

    //     Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "WebViewFactory.getProvider()");
    //     try {
    //         Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "WebViewFactory.loadNativeLibrary()");
    //         loadNativeLibrary();
    //         Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);

    //         Class<WebViewFactoryProvider> providerClass;
    //         Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "WebViewFactory.getFactoryClass()");
    //         try {
    //             providerClass = getFactoryClass();
    //         } catch (ClassNotFoundException e) {
    //             Log.e(LOGTAG, "error loading provider", e);
    //             throw new AndroidRuntimeException(e);
    //         } finally {
    //             Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);
    //         }

    //         StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskReads();
    //         Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "providerClass.newInstance()");
    //         try {
    //             sProviderInstance = providerClass.newInstance();
    //             if (DEBUG) Log.v(LOGTAG, "Loaded provider: " + sProviderInstance);
    //             return sProviderInstance;
    //         } catch (Exception e) {
    //             Log.e(LOGTAG, "error instantiating provider", e);
    //             throw new AndroidRuntimeException(e);
    //         } finally {
    //             Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);
    //             StrictMode.setThreadPolicy(oldPolicy);
    //         }
    //     } finally {
    //         Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);
    //     }
    // }
    return NULL;
}

// TODO
// private static Class<WebViewFactoryProvider> getFactoryClass() throws ClassNotFoundException {
//     Application initialApplication = AppGlobals.getInitialApplication();
//     try {
//         // First fetch the package info so we can log the webview package version.
//         String packageName = getWebViewPackageName();
//         sPackageInfo = initialApplication.getPackageManager().getPackageInfo(packageName, 0);
//         Log.i(LOGTAG, "Loading " + packageName + " version " + sPackageInfo.versionName +
//                       " (code " + sPackageInfo.versionCode + ")");

//         // Construct a package context to load the Java code into the current app.
//         Context webViewContext = initialApplication.createPackageContext(packageName,
//                 Context.CONTEXT_INCLUDE_CODE | Context.CONTEXT_IGNORE_SECURITY);
//         initialApplication.getAssets().addAssetPath(
//                 webViewContext.getApplicationInfo().sourceDir);
//         ClassLoader clazzLoader = webViewContext.getClassLoader();
//         Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "Class.forName()");
//         try {
//             return (Class<WebViewFactoryProvider>) Class.forName(CHROMIUM_WEBVIEW_FACTORY, true,
//                                                                  clazzLoader);
//         } finally {
//             Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);
//         }
//     } catch (PackageManager.NameNotFoundException e) {
//         // If the package doesn't exist, then try loading the null WebView instead.
//         // If that succeeds, then this is a device without WebView support; if it fails then
//         // swallow the failure, complain that the real WebView is missing and rethrow the
//         // original exception.
//         try {
//             return (Class<WebViewFactoryProvider>) Class.forName(NULL_WEBVIEW_FACTORY);
//         } catch (ClassNotFoundException e2) {
//             // Ignore.
//         }
//         Log.e(LOGTAG, "Chromium WebView package does not exist", e);
//         throw new AndroidRuntimeException(e);
//     }
// }

/**
 * Perform any WebView loading preparations that must happen in the zygote.
 * Currently, this means allocating address space to load the real JNI library later.
 */
void WebViewFactory::PrepareWebViewInZygote()
{
    assert(0);
    // TODO
    // try {
    //     System.loadLibrary("webviewchromium_loader");
    //     long addressSpaceToReserve =
    //             SystemProperties.getLong(CHROMIUM_WEBVIEW_VMSIZE_SIZE_PROPERTY,
    //             CHROMIUM_WEBVIEW_DEFAULT_VMSIZE_BYTES);
    //     sAddressSpaceReserved = nativeReserveAddressSpace(addressSpaceToReserve);

    //     if (sAddressSpaceReserved) {
    //         if (DEBUG) {
    //             Log.v(LOGTAG, "address space reserved: " + addressSpaceToReserve + " bytes");
    //         }
    //     } else {
    //         Log.e(LOGTAG, "reserving " + addressSpaceToReserve +
    //                 " bytes of address space failed");
    //     }
    // } catch (Throwable t) {
    //     // Log and discard errors at this stage as we must not crash the zygote.
    //     Log.e(LOGTAG, "error preparing native loader", t);
    // }
}

/**
 * Perform any WebView loading preparations that must happen at boot from the system server,
 * after the package manager has started or after an update to the webview is installed.
 * This must be called in the system server.
 * Currently, this means spawning the child processes which will create the relro files.
 */
void WebViewFactory::PrepareWebViewInSystemServer()
{
    AutoPtr< ArrayOf<String> > nativePaths;
    // try {
        nativePaths = GetWebViewNativeLibraryPaths();
    // } catch (Throwable t) {
    //     // Log and discard errors at this stage as we must not crash the system server.
    //     Log.e(LOGTAG, "error preparing webview native library", t);
    // }
    PrepareWebViewInSystemServer(nativePaths);
}

void WebViewFactory::PrepareWebViewInSystemServer(
    /* [in] */ ArrayOf<String>* nativeLibraryPaths)
{
    if (DEBUG) {
        Logger::V(LOGTAG, "creating relro files");
    }

    // We must always trigger createRelRo regardless of the value of nativeLibraryPaths. Any
    // unexpected values will be handled there to ensure that we trigger notifying any process
    // waiting on relreo creation.
    if (Build::SUPPORTED_32_BIT_ABIS->GetLength() > 0) {
        if (DEBUG) Logger::V(LOGTAG, "Create 32 bit relro");
        CreateRelroFile(FALSE /* is64Bit */, nativeLibraryPaths);
    }

    if (Build::SUPPORTED_64_BIT_ABIS->GetLength() > 0) {
        if (DEBUG) Logger::V(LOGTAG, "Create 64 bit relro");
        CreateRelroFile(TRUE /* is64Bit */, nativeLibraryPaths);
    }
}

void WebViewFactory::OnWebViewUpdateInstalled()
{
    assert(0);
    // TODO
    // String[] nativeLibs = null;
    // try {
    //     nativeLibs = WebViewFactory.getWebViewNativeLibraryPaths();
    //     if (nativeLibs != null) {
    //         long newVmSize = 0L;

    //         for (String path : nativeLibs) {
    //             if (DEBUG) Log.d(LOGTAG, "Checking file size of " + path);
    //             if (path == null) continue;
    //             File f = new File(path);
    //             if (f.exists()) {
    //                 long length = f.length();
    //                 if (length > newVmSize) {
    //                     newVmSize = length;
    //                 }
    //             }
    //         }

    //         if (DEBUG) {
    //             Log.v(LOGTAG, "Based on library size, need " + newVmSize +
    //                     " bytes of address space.");
    //         }
    //         // The required memory can be larger than the file on disk (due to .bss), and an
    //         // upgraded version of the library will likely be larger, so always attempt to
    //         // reserve twice as much as we think to allow for the library to grow during this
    //         // boot cycle.
    //         newVmSize = Math.max(2 * newVmSize, CHROMIUM_WEBVIEW_DEFAULT_VMSIZE_BYTES);
    //         Log.d(LOGTAG, "Setting new address space to " + newVmSize);
    //         SystemProperties.set(CHROMIUM_WEBVIEW_VMSIZE_SIZE_PROPERTY,
    //                 Long.toString(newVmSize));
    //     }
    // } catch (Throwable t) {
    //     // Log and discard errors at this stage as we must not crash the system server.
    //     Log.e(LOGTAG, "error preparing webview native library", t);
    // }
    // prepareWebViewInSystemServer(nativeLibs);
}

AutoPtr< ArrayOf<String> > WebViewFactory::GetWebViewNativeLibraryPaths()
{
    assert(0);
    // TODO
    // final String NATIVE_LIB_FILE_NAME = "libwebviewchromium.so";

    // PackageManager pm = AppGlobals.getInitialApplication().getPackageManager();
    // ApplicationInfo ai = pm.getApplicationInfo(getWebViewPackageName(), 0);

    // String path32;
    // String path64;
    // boolean primaryArchIs64bit = VMRuntime.is64BitAbi(ai.primaryCpuAbi);
    // if (!TextUtils.isEmpty(ai.secondaryCpuAbi)) {
    //     // Multi-arch case.
    //     if (primaryArchIs64bit) {
    //         // Primary arch: 64-bit, secondary: 32-bit.
    //         path64 = ai.nativeLibraryDir;
    //         path32 = ai.secondaryNativeLibraryDir;
    //     } else {
    //         // Primary arch: 32-bit, secondary: 64-bit.
    //         path64 = ai.secondaryNativeLibraryDir;
    //         path32 = ai.nativeLibraryDir;
    //     }
    // } else if (primaryArchIs64bit) {
    //     // Single-arch 64-bit.
    //     path64 = ai.nativeLibraryDir;
    //     path32 = "";
    // } else {
    //     // Single-arch 32-bit.
    //     path32 = ai.nativeLibraryDir;
    //     path64 = "";
    // }
    // if (!TextUtils.isEmpty(path32)) path32 += "/" + NATIVE_LIB_FILE_NAME;
    // if (!TextUtils.isEmpty(path64)) path64 += "/" + NATIVE_LIB_FILE_NAME;
    // return new String[] { path32, path64 };

    return NULL;
}

void WebViewFactory::CreateRelroFile(
    /* [in] */ Boolean is64Bit,
    /* [in] */ ArrayOf<String>* nativeLibraryPaths)
{
    assert(0);
    // TODO
    // final String abi =
    //         is64Bit ? Build.SUPPORTED_64_BIT_ABIS[0] : Build.SUPPORTED_32_BIT_ABIS[0];

    // // crashHandler is invoked by the ActivityManagerService when the isolated process crashes.
    // Runnable crashHandler = new Runnable() {
    //     @Override
    //     public void run() {
    //         try {
    //             Log.e(LOGTAG, "relro file creator for " + abi + " crashed. Proceeding without");
    //             getUpdateService().notifyRelroCreationCompleted(is64Bit, false);
    //         } catch (RemoteException e) {
    //             Log.e(LOGTAG, "Cannot reach WebViewUpdateService. " + e.getMessage());
    //         }
    //     }
    // };

    // try {
    //     if (nativeLibraryPaths == null
    //             || nativeLibraryPaths[0] == null || nativeLibraryPaths[1] == null) {
    //         throw new IllegalArgumentException(
    //                 "Native library paths to the WebView RelRo process must not be null!");
    //     }
    //     int pid = LocalServices.getService(ActivityManagerInternal.class).startIsolatedProcess(
    //             RelroFileCreator.class.getName(), nativeLibraryPaths, "WebViewLoader-" + abi, abi,
    //             Process.SHARED_RELRO_UID, crashHandler);
    //     if (pid <= 0) throw new Exception("Failed to start the relro file creator process");
    // } catch (Throwable t) {
    //     // Log and discard errors as we must not crash the system server.
    //     Log.e(LOGTAG, "error starting relro file creator for abi " + abi, t);
    //     crashHandler.run();
    // }
}

void WebViewFactory::LoadNativeLibrary()
{
    if (!sAddressSpaceReserved) {
        Logger::E(LOGTAG, "can't load with relro file; address space not reserved");
        return;
    }

    assert(0);
    // TODO
    // try {
    //     getUpdateService().waitForRelroCreationCompleted(VMRuntime.getRuntime().is64Bit());
    // } catch (RemoteException e) {
    //     Log.e(LOGTAG, "error waiting for relro creation, proceeding without", e);
    //     return;
    // }

    // try {
        AutoPtr< ArrayOf<String> > args = GetWebViewNativeLibraryPaths();
        Boolean result = NativeLoadWithRelroFile((*args)[0] /* path32 */,
                                                 (*args)[1] /* path64 */,
                                                 CHROMIUM_WEBVIEW_NATIVE_RELRO_32,
                                                 CHROMIUM_WEBVIEW_NATIVE_RELRO_64);
        if (!result) {
            Logger::W(LOGTAG, "failed to load with relro file, proceeding without");
        }
        else if (DEBUG) {
            Logger::V(LOGTAG, "loaded with relro file");
        }
    // } catch (PackageManager.NameNotFoundException e) {
    //     Log.e(LOGTAG, "Failed to list WebView package libraries for loadNativeLibrary", e);
    // }
}

// TODO
// AutoPtr<IWebViewUpdateService> WebViewFactory::GetUpdateService()
// {
//     return IWebViewUpdateService.Stub.asInterface(ServiceManager.getService("webviewupdate"));
// }

Boolean WebViewFactory::NativeReserveAddressSpace(
    /* [in] */ Int64 addressSpaceToReserve)
{
    assert(0);
    return FALSE;
}

Boolean WebViewFactory::NativeCreateRelroFile(
    /* [in] */ const String& lib32,
    /* [in] */ const String& lib64,
    /* [in] */ const String& relro32,
    /* [in] */ const String& relro64)
{
    assert(0);
    return FALSE;
}

Boolean WebViewFactory::NativeLoadWithRelroFile(
    /* [in] */ const String& lib32,
    /* [in] */ const String& lib64,
    /* [in] */ const String& relro32,
    /* [in] */ const String& relro64)
{
    assert(0);
    return FALSE;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
