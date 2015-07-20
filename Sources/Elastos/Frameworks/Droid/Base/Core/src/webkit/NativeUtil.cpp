
#include <string>
#include "app/CActivityManagerMemoryInfo.h"
#include "provider/Settings.h"
#include "provider/CSettingsGlobal.h"
#include "net/Uri.h"
#include "net/CUriHelper.h"
#include "webkit/NativeUtil.h"
#include "webkit/MimeTypeMap.h"
#include <elastos/utility/logging/Logger.h>
#include "webkit/DebugFlags.h"
#include "webkit/L10nUtils.h"
#include "webkit/HTML5Audio.h"
#include "webkit/HTML5VideoViewProxy.h"

using Elastos::IO::IFile;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::App::CActivityManagerMemoryInfo;
using Elastos::Droid::App::IActivityManagerMemoryInfo;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Provider::Settings;
// using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::Webkit::HTML5Audio;
using Elastos::Droid::Webkit::HTML5VideoViewProxy;
using Elastos::Droid::Webkit::CWebViewCore;
using Elastos::Droid::Webkit::IWebViewCore;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;
using Elastos::Utility::Logging::Logger;

typedef struct ElaNativeUtilCallback
{
    Elastos::String (*getDatabaseDirectory)();
    Elastos::String (*getCacheDirectory)();
    Elastos::String (*getPackageName)();
    Boolean (*canSatisfyMemoryAllocation)(Int64);
    Elastos::String (*getMIMETypeForExtension)(const Elastos::String&);
} ElaNativeUtilCallback;

Elastos::String NativeUtil_getDatabaseDirectory()
{
    String dir;
    Elastos::Droid::Webkit::NativeUtil::GetDatabaseDirectory(&dir);
    //because the std::string used here is not compatible with the one used in webkit
    return dir;//std::string(dir.string());
}

Elastos::String NativeUtil_getCacheDirectory()
{
    String dir;
    Elastos::Droid::Webkit::NativeUtil::GetCacheDirectory(&dir);
    if (Elastos::Droid::Webkit::DebugFlags::CACHE_MANAGER)
        Logger::D("NativeUtil", "NativeUtil_getCacheDirectory %s", dir.string());
    //because the std::string used here is not compatible with the one used in webkit
    return dir;//std::string(dir.string());
}

Elastos::String NativeUtil_getPackageName()
{
    String dir;
    Elastos::Droid::Webkit::NativeUtil::GetPackageName(&dir);
    //because the std::string used here is not compatible with the one used in webkit
    return dir;//std::string(dir.string());
}

Boolean NativeUtil_canSatisfyMemoryAllocation(
   /* [in] */ Int64 bytesRequested)
{
    Boolean result;
    Elastos::Droid::Webkit::NativeUtil::CanSatisfyMemoryAllocation(bytesRequested, &result);
    return result;
}

struct ElaInputStreamCallback
{
    Int32 (*read)(IInterface* inputStream, ArrayOf<Byte>* byteArray);
    void (*close)(IInterface* inputStream);
};

Int32 NativeUtil_InputStream_Read(IInterface* inputStream, ArrayOf<Byte>* byteArray)
{
    //Logger::D("NativeUtil", "NativeUtil_InputStream_Read, inputStream:%p", inputStream);
    AutoPtr<IInputStream> input = (IInputStream*)inputStream;
    //Logger::D("NativeUtil", "NativeUtil_InputStream_Read, inputStream:%p", input.Get());
    Int32 size = 0;
    input->ReadBytes(byteArray, &size);
    return size;
}

void NativeUtil_InputStream_Close(IInterface* inputStream)
{
    //Logger::D("NativeUtil", "NativeUtil_InputStream_Read, inputStream:%p", inputStream);
    AutoPtr<IInputStream> input = (IInputStream*)inputStream;
    //Logger::D("NativeUtil", "NativeUtil_InputStream_Read, inputStream:%p", input.Get());
    input->Close();
}

struct ElaChromiumL10nUtilsCallback
{
    Elastos::String (*getLocalisedString)(Int32 id);
};

struct ElaChromiumLocaleCallback
{
    Elastos::String (*getLocale)();
};

Elastos::String NativeUtil_GetLocale()
{
    AutoPtr<ILocale> locale;
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    localeHelper->GetDefault((ILocale**)&locale);
    Elastos::String localeStr;
    locale->ToString(&localeStr);

    return localeStr;
}

struct ElaChromiumNativeUtilCallback
{
    CARAPI (*getAutofillQueryUrl)(Elastos::String*);
    AutoPtr<IInterface> (*contentUrlStream)(const Elastos::String& url);
    Int32 (*read)(IInterface* obj, ArrayOf<Byte>*);
    void (*close)(IInterface* obj);
    Int64 (*contentUrlSize)(const Elastos::String& url);
};

AutoPtr<IInterface> NativeUtil_ContentUrlStream(const Elastos::String& url)
{
    return Elastos::Droid::Webkit::NativeUtil::ContentUrlStream(url);
}

struct ElaChromiumMimeUtilsCallback
{
    AutoPtr<IInterface> mObj;
    CARAPI (*guessMimeTypeFromExtension)(IInterface* obj,
                            const Elastos::String& extension,
                            Elastos::String* result);
    CARAPI (*guessExtensionFromMimeType)(IInterface* obj,
                            const Elastos::String& mimeType,
                            Elastos::String* result);
};

ECode NativeUtil_GuessMimeTypeFromExtension(IInterface* obj,
                            const Elastos::String& extension,
                            Elastos::String* result)
{
    AutoPtr<Elastos::Net::IMimeUtils> mimeUtils = Elastos::Net::IMimeUtils::Probe(obj);
    return mimeUtils->GuessMimeTypeFromExtension(extension, result);
}

ECode NativeUtil_GuessExtensionFromMimeType(IInterface* obj,
                            const Elastos::String& mimeType,
                            Elastos::String* result)
{
    AutoPtr<Elastos::Net::IMimeUtils> mimeUtils = Elastos::Net::IMimeUtils::Probe(obj);
    return mimeUtils->GuessExtensionFromMimeType(mimeType, result);
}

struct ElaChromiumUtilCallback
{
    AutoPtr<IInterface> (*arrayOfToIObjectContainer)(ArrayOf< AutoPtr< ArrayOf<Byte> > >*);
};

AutoPtr<IInterface> NativeUtil_ArrayOfToIObjectContainer(ArrayOf< AutoPtr< ArrayOf<Byte> > >* arrays)
{
    // TODO
    assert(0);
    return NULL;
}

struct ElaChromiumCertificateChainValidatorCallback
{
    CARAPI (*verifyServerCertificates)(IInterface* certChain,
                      const Elastos::String& sDomain,
                      const Elastos::String& authType,
                      IInterface** err);
};

ECode NativeUtil_VerifyServerCertificates(IInterface* certChain,
                      const Elastos::String& sDomain,
                      const Elastos::String& authType,
                      IInterface** err)
{
    // TODO
    assert(0);

    // VALIDATE_NOT_NULL(err);

    // AutoPtr<ICertificateChainValidatorHelper> certificateChainValidatorHelper;
    // CCertificateChainValidatorHelper::AcquireSingleton((ICertificateChainValidatorHelper**)&certificateChainValidatorHelper);
    // AutoPtr<IObjectContainer> container = IObjectContainer::Probe(certChain);
    // AutoPtr<ISslError> sslError;
    // certificateChainValidatorHelper->VerifyServerCertificates(container,
    //                        sDomain, authType, (ISslError**)&sslError);
    // *err = sslError;

    return E_NOT_IMPLEMENTED;
}

struct ElaChromiumSslErrorCallback
{
    CARAPI (*getPrimaryError)(IInterface* obj, Int32* error);
};

ECode NativeUtil_GetPrimaryError(IInterface* obj, Int32* error)
{
    VALIDATE_NOT_NULL(obj);
    AutoPtr<ISslError> sslError = ISslError::Probe(obj);
    return sslError->GetPrimaryError(error);
}

struct ElaMediaPlayerPrivateCallback
{
    void (*play)(IInterface* obj, const Elastos::String& url, Int32 position, Int32 videoLayerID);
    void (*enterFullscreenForVideoLayer)(IInterface* obj, const Elastos::String& url, Int32 videoLayerID);
    void (*teardown)(IInterface* obj);
    void (*seek)(IInterface* obj, Int32 time);
    void (*pause)(IInterface* obj);
    AutoPtr<IInterface> (*getInstance)(IInterface* webViewCore, Int32 nativePtr);
    void (*loadPoster)(IInterface* obj, const Elastos::String& url);
};

void NativeUtil_MediaPlayer_Play(IInterface* obj, const Elastos::String& url, Int32 position, Int32 videoLayerID)
{
    // obj: HTML5VideoViewProxy --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5VideoViewProxy> videoView = (HTML5VideoViewProxy*)hander;
    videoView->Play(url, position, videoLayerID);
}

void NativeUtil_MediaPlayer_EnterFullscreenForVideoLayer(IInterface* obj, const Elastos::String& url, Int32 videoLayerID)
{
    // obj: HTML5VideoViewProxy --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5VideoViewProxy> videoView = (HTML5VideoViewProxy*)hander;
    videoView->EnterFullscreenForVideoLayer(url, videoLayerID);
}

void NativeUtil_MediaPlayer_Teardown(IInterface* obj)
{
    // obj: HTML5VideoViewProxy --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5VideoViewProxy> videoView = (HTML5VideoViewProxy*)hander;
    videoView->Teardown();
}

void NativeUtil_MediaPlayer_Seek(IInterface* obj, Int32 time)
{
    // obj: HTML5VideoViewProxy --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5VideoViewProxy> videoView = (HTML5VideoViewProxy*)hander;
    videoView->Seek(time);
}

void NativeUtil_MediaPlayer_Pause(IInterface* obj)
{
    // obj: HTML5VideoViewProxy --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5VideoViewProxy> videoView = (HTML5VideoViewProxy*)hander;
    videoView->Pause();
}

AutoPtr<IInterface> NativeUtil_MediaPlayer_GetInstance(IInterface* _webViewCore, Int32 nativePtr)
{
    IWebViewCore* wvc = IWebViewCore::Probe(_webViewCore);
    CWebViewCore* webViewCore = (CWebViewCore*)wvc;
    // HTML5VideoViewProxy --> IHandler --> IInterface
    AutoPtr<HTML5VideoViewProxy> videoViewProxy = HTML5VideoViewProxy::GetInstance(webViewCore, nativePtr);
    IHandler* videoViewProxyHandler = (IHandler*)videoViewProxy;
    AutoPtr<IInterface> videoViewProxyInterface = (IInterface*)videoViewProxyHandler;
    return videoViewProxyInterface;
}

void NativeUtil_MediaPlayer_LoadPoster(IInterface* obj, const Elastos::String& url)
{
    // obj: HTML5VideoViewProxy --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5VideoViewProxy> videoView = (HTML5VideoViewProxy*)hander;
    videoView->LoadPoster(url);
}

struct ElaMediaPlayerPrivateAudioCallback
{
    void (*play)(IInterface* obj);
    void (*teardown)(IInterface* obj);
    void (*seek)(IInterface* obj, Int32 msec);
    void (*pause)(IInterface* obj);
    AutoPtr<IInterface> (*newInstance)(IInterface* webViewCore, Int32 nativePtr);
    void (*setDataSource)(IInterface* obj, const Elastos::String& url);
    Float (*getMaxTimeSeekable)(IInterface* obj);
};

void NativeUtil_MediaPlayerAudio_Play(IInterface* obj)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_Play, obj:%p", obj);
    // obj: HTML5Audio --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5Audio> audio = (HTML5Audio*)hander;
    audio->Play();
}

void NativeUtil_MediaPlayerAudio_Teardown(IInterface* obj)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_Teardown, obj:%p", obj);
    // obj: HTML5Audio --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5Audio> audio = (HTML5Audio*)hander;
    audio->Teardown();
}

void NativeUtil_MediaPlayerAudio_Seek(IInterface* obj, Int32 msec)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_Seek, obj:%p, msec:%d", obj, msec);
    // obj: HTML5Audio --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5Audio> audio = (HTML5Audio*)hander;
    audio->Seek(msec);
}

void NativeUtil_MediaPlayerAudio_Pause(IInterface* obj)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_Pause, obj:%p", obj);
    // obj: HTML5Audio --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5Audio> audio = (HTML5Audio*)hander;
    audio->Pause();
}

AutoPtr<IInterface> NativeUtil_MediaPlayerAudio_NewInstance(IInterface* _webViewCore, Int32 nativePtr)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_NewInstance, webCore:%p, nativePtr:0x%x", _webViewCore, nativePtr);
    IWebViewCore* wvc = IWebViewCore::Probe(_webViewCore);
    CWebViewCore* webViewCore = (CWebViewCore*)wvc;
    HTML5Audio* audio = new HTML5Audio(webViewCore, nativePtr);
    // HTML5Audio --> IHandler --> IInterface
    IHandler* hander = (IHandler*)audio;
    AutoPtr<IInterface> obj = (IInterface*)hander;
    return obj;
}

void NativeUtil_MediaPlayerAudio_SetDataSource(IInterface* obj, const Elastos::String& url)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_SetDataSource, obj:%p, url:%s", obj, url.string());
    // obj: HTML5Audio --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5Audio> audio = (HTML5Audio*)hander;
    audio->SetDataSource(url);
}

Float NativeUtil_MediaPlayerAudio_GetMaxTimeSeekable(IInterface* obj)
{
    //Logger::D("NativeUtil", "NativeUtil_MediaPlayerAudio_GetMaxTimeSeekable, obj:%p", obj);
    // obj: HTML5Audio --> IHandler --> IInterface
    IHandler* hander = IHandler::Probe(obj);
    AutoPtr<HTML5Audio> audio = (HTML5Audio*)hander;
    return audio->GetMaxTimeSeekable();
}

#ifdef __cplusplus
extern "C"
{
#endif
    void Elastos_NativeUtil_InitCallback(Int32 cb);
    void Elastos_InputStream_InitCallback(Int32 cb);
    void Elastos_Chromium_L10nUtils_InitCallback(Int32 cb);
    void Elastos_Chromium_LocaleInitCallback(Int32 cb);
    void Elastos_Chromium_NativeUtil_InitCallback(Int32 cb);
    void Elastos_Chromium_MimeUtils_Init(Int32 cb);
    void Elastos_ArrayOf_To_IObjectContainer_Init(Int32 cb);
    void Elastos_Chromium_CertificateChainValidator_Init(Int32 cb);
    void Elastos_Chromium_SslError_Init(Int32 cb);
    void Elastos_MediaPlayerPrivate_Init(Int32 cb);
    void Elastos_MediaPlayerPrivateAudio_Init(Int32 cb);
#ifdef __cplusplus
}
#endif

static ElaNativeUtilCallback* Init()
{
    static ElaNativeUtilCallback cb;
    cb.getDatabaseDirectory = NativeUtil_getDatabaseDirectory;
    cb.getCacheDirectory = NativeUtil_getCacheDirectory;
    cb.getPackageName = NativeUtil_getPackageName;
    cb.canSatisfyMemoryAllocation = NativeUtil_canSatisfyMemoryAllocation;
    cb.getMIMETypeForExtension = Elastos::Droid::Webkit::MimeTypeMap::MimeTypeFromExtension;
    Elastos_NativeUtil_InitCallback((Int32)&cb);

    static ElaInputStreamCallback inputStreamCB;
    inputStreamCB.read = NativeUtil_InputStream_Read;
    inputStreamCB.close = NativeUtil_InputStream_Close;
    Elastos_InputStream_InitCallback((Int32)&inputStreamCB);

    static ElaChromiumL10nUtilsCallback l10nUtilsCB;
    l10nUtilsCB.getLocalisedString = Elastos::Droid::Webkit::L10nUtils::GetLocalisedString;
    Elastos_Chromium_L10nUtils_InitCallback((Int32)&l10nUtilsCB);

    static ElaChromiumLocaleCallback localeCB;
    localeCB.getLocale = NativeUtil_GetLocale;
    Elastos_Chromium_LocaleInitCallback((Int32)&localeCB);

    static ElaChromiumNativeUtilCallback chromiumNativeUtilCB;
    chromiumNativeUtilCB.getAutofillQueryUrl = Elastos::Droid::Webkit::NativeUtil::GetAutofillQueryUrl;
    chromiumNativeUtilCB.contentUrlStream = NativeUtil_ContentUrlStream;
    chromiumNativeUtilCB.read = NativeUtil_InputStream_Read;
    chromiumNativeUtilCB.close = NativeUtil_InputStream_Close;
    chromiumNativeUtilCB.contentUrlSize = Elastos::Droid::Webkit::NativeUtil::ContentUrlSize;
    Elastos_Chromium_NativeUtil_InitCallback((Int32)&chromiumNativeUtilCB);

    static ElaChromiumMimeUtilsCallback mimeUtilsCB;
    static AutoPtr<IInterface> mimeUtils;
    Elastos::Net::CMimeUtils::AcquireSingleton((Elastos::Net::IMimeUtils**)&mimeUtils);
    mimeUtilsCB.mObj = mimeUtils;
    mimeUtilsCB.guessMimeTypeFromExtension = NativeUtil_GuessMimeTypeFromExtension;
    mimeUtilsCB.guessExtensionFromMimeType = NativeUtil_GuessExtensionFromMimeType;
    Elastos_Chromium_MimeUtils_Init((Int32)&mimeUtilsCB);

    static ElaChromiumUtilCallback chromiumUtilCB;
    chromiumUtilCB.arrayOfToIObjectContainer = NativeUtil_ArrayOfToIObjectContainer;
    Elastos_ArrayOf_To_IObjectContainer_Init((Int32)&chromiumUtilCB);

    static ElaChromiumCertificateChainValidatorCallback chainValidatorCB;
    chainValidatorCB.verifyServerCertificates = NativeUtil_VerifyServerCertificates;
    Elastos_Chromium_CertificateChainValidator_Init((Int32)&chainValidatorCB);

    static ElaChromiumSslErrorCallback sslErrorCB;
    sslErrorCB.getPrimaryError = NativeUtil_GetPrimaryError;
    Elastos_Chromium_SslError_Init((Int32)&sslErrorCB);

    static ElaMediaPlayerPrivateCallback mediaPlayerPrivateCB;
    mediaPlayerPrivateCB.play = NativeUtil_MediaPlayer_Play;
    mediaPlayerPrivateCB.enterFullscreenForVideoLayer = NativeUtil_MediaPlayer_EnterFullscreenForVideoLayer;
    mediaPlayerPrivateCB.teardown = NativeUtil_MediaPlayer_Teardown;
    mediaPlayerPrivateCB.seek = NativeUtil_MediaPlayer_Seek;
    mediaPlayerPrivateCB.pause = NativeUtil_MediaPlayer_Pause;
    mediaPlayerPrivateCB.getInstance = NativeUtil_MediaPlayer_GetInstance;
    mediaPlayerPrivateCB.loadPoster = NativeUtil_MediaPlayer_LoadPoster;
    Elastos_MediaPlayerPrivate_Init((Int32)&mediaPlayerPrivateCB);

    static struct ElaMediaPlayerPrivateAudioCallback mediaPlayerPrivateAudioCB;
    mediaPlayerPrivateAudioCB.play = NativeUtil_MediaPlayerAudio_Play;
    mediaPlayerPrivateAudioCB.teardown = NativeUtil_MediaPlayerAudio_Teardown;
    mediaPlayerPrivateAudioCB.seek = NativeUtil_MediaPlayerAudio_Seek;
    mediaPlayerPrivateAudioCB.pause = NativeUtil_MediaPlayerAudio_Pause;
    mediaPlayerPrivateAudioCB.newInstance = NativeUtil_MediaPlayerAudio_NewInstance;
    mediaPlayerPrivateAudioCB.setDataSource = NativeUtil_MediaPlayerAudio_SetDataSource;
    mediaPlayerPrivateAudioCB.getMaxTimeSeekable = NativeUtil_MediaPlayerAudio_GetMaxTimeSeekable;
    Elastos_MediaPlayerPrivateAudio_Init((Int32)&mediaPlayerPrivateAudioCB);

    return &cb;
}

static ElaNativeUtilCallback* sElaNativeUtilCallback = Init();

namespace Elastos {
namespace Droid {
namespace Webkit {

const String NativeUtil::LOGTAG("NativeUtil");
const String NativeUtil::ANDROID_CONTENT("content:"); //IURLUtil::CONTENT_BASE
// Used by the Chromium HTTP stack.
String NativeUtil::sDatabaseDirectory;
String NativeUtil::sCacheDirectory;
AutoPtr<IContext> NativeUtil::sContext;
Mutex NativeUtil::sLock;

ECode NativeUtil::CheckInitialized()
{
    if (sContext == NULL) {
        // throw new IllegalStateException("Call CookieSyncManager::createInstance() or create a webview before using this class");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

void NativeUtil::SetContext(
    /* [in] */ IContext* context)
{
    AutoLock lock(sLock);
    if (sContext != NULL) {
        return;
    }

    context->GetApplicationContext((IContext**)&sContext);
}

AutoPtr<IContext> NativeUtil::GetContext()
{
    AutoLock lock(sLock);
    return sContext;
}

/**
 * Called by JNI. Gets the application's database directory, excluding the trailing slash.
 * @return String The application's database directory
 */
ECode NativeUtil::GetDatabaseDirectory(
    /* [out] */ String* directory)
{
    VALIDATE_NOT_NULL(directory);

    AutoLock lock(sLock);

    FAIL_RETURN(CheckInitialized());

    if (sDatabaseDirectory.IsNull()) {
        AutoPtr<IFile> file;
        sContext->GetDatabasePath(String("dummy"), (IFile**)&file);
        file->GetParent(&sDatabaseDirectory);
    }

    *directory = sDatabaseDirectory;
    return NOERROR;
}

/**
 * Called by JNI. Gets the application's cache directory, excluding the trailing slash.
 * @return String The application's cache directory
 */
ECode NativeUtil::GetCacheDirectory(
    /* [out] */ String* directory)
{
    VALIDATE_NOT_NULL(directory);

    AutoLock lock(sLock);

    FAIL_RETURN(CheckInitialized());

    if (sCacheDirectory.IsNull()) {
        AutoPtr<IFile> cacheDir;
        sContext->GetCacheDir((IFile**)&cacheDir);
        if (cacheDir == NULL) {
            sCacheDirectory = "";
        }
        else {
            cacheDir->GetAbsolutePath(&sCacheDirectory);
        }
    }

    *directory = sCacheDirectory;
    return NOERROR;
}

/**
 * Called by JNI. Gets the application's package name.
 * @return String The application's package name
 */
ECode NativeUtil::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    AutoLock lock(sLock);

    FAIL_RETURN(CheckInitialized());

    return sContext->GetPackageName(name);
}

/**
 * Called by JNI. Calculates the size of an input stream by reading it.
 * @return long The size of the stream
 */
Int64 NativeUtil::ContentUrlSize(
    /* [in] */ const String& _url)
{
    AutoLock lock(sLock);

    String url = _url;
    // content://
    if (url.StartWith(ANDROID_CONTENT)) {
        //try {
            // Strip off MIME type. If we don't do this, we can fail to
            // load Gmail attachments, because the URL being loaded doesn't
            // exactly match the URL we have permission to read.
            Int32 mimeIndex = url.LastIndexOf('?');
            if (mimeIndex != -1) {
                url = url.Substring(0, mimeIndex);
            }
            AutoPtr<IUriHelper> uriHelper;
            CUriHelper::AcquireSingleton((IUriHelper**)&uriHelper);
            AutoPtr<IUri> uri;
            uriHelper->Parse(url, (IUri**)&uri);
            AutoPtr<IContentResolver> resolver;
            sContext->GetContentResolver((IContentResolver**)&resolver);
            AutoPtr<IInputStream> is;
            resolver->OpenInputStream(uri, (IInputStream**)&is);
            AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1024);
            Int32 n;
            Int32 size = 0;
            //try {
            while (is->ReadBytes(buffer, &n), n != -1) {
                size += n;
            }
            //} finally {
            is->Close();
            //}
            return size;
        //} catch (Exception e) {
        //    Log.e(LOGTAG, "Exception: " + url);
        //    return 0;
        //}
    }
    else {
        return 0;
    }
}

/**
 * Called by JNI.
 *
 * @return  Opened input stream to content
 * TODO: Make all content loading use this instead of BrowserFrame.java
 */
AutoPtr<IInputStream> NativeUtil::ContentUrlStream(
    /* [in] */ const String& _url)
{
    AutoLock lock(sLock);

    String url = _url;
    // content://
    if (url.StartWith(ANDROID_CONTENT)) {
        //try {
            // Strip off mimetype, for compatibility with ContentLoader.java
            // (used with Android HTTP stack, now removed).
            // If we don't do this, we can fail to load Gmail attachments,
            // because the URL being loaded doesn't exactly match the URL we
            // have permission to read.
            Int32 mimeIndex = url.LastIndexOf('?');
            if (mimeIndex != -1) {
                url = url.Substring(0, mimeIndex);
            }
            AutoPtr<IUri> uri;
            Uri::Parse(url, (IUri**)&uri);
            AutoPtr<IContentResolver> cr;
            sContext->GetContentResolver((IContentResolver**)&cr);
            AutoPtr<IInputStream> is;
            cr->OpenInputStream(uri, (IInputStream**)&is);
            return is;
        //} catch (Exception e) {
        //    Log.e(LOGTAG, "Exception: " + url);
        //    return null;
        //}
    }
    else {
        return NULL;
    }
}

ECode NativeUtil::GetAutofillQueryUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);

    AutoLock lock(sLock);

    FAIL_RETURN(CheckInitialized());
    // If the device has not checked in it won't have pulled down the system setting for the
    // Autofill Url. In that case we will not make autofill server requests.
    AutoPtr<IContentResolver> cr;
    sContext->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsGlobal> global;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&global);
    return global->GetString(cr,
            ISettingsGlobal::WEB_AUTOFILL_QUERY_URL, url);
}

ECode NativeUtil::CanSatisfyMemoryAllocation(
    /* [in] */ Int64 bytesRequested,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckInitialized());
    AutoPtr<IActivityManager> manager;
    sContext->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&manager);
    AutoPtr<IActivityManagerMemoryInfo> memInfo;
    manager->GetMemoryInfo((IActivityManagerMemoryInfo**)&memInfo);
    Int64 availMem, threshold;
    memInfo->GetAvailMem(&availMem);
    memInfo->GetThreshold(&threshold);
    Int64 leftToAllocate = availMem - threshold;
    Boolean lowMemory;
    memInfo->GetLowMemory(&lowMemory);
    *result = !lowMemory && bytesRequested < leftToAllocate;
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
