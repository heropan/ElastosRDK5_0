
#include <elastos/droid/server/connectivity/PacManager.h>
#include <elastos/droid/server/IoThread.h>
#include <elastos/core/AutoLock.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::App::IPendingIntentHelper;
using Elastos::Droid::App::CPendingIntentHelper;
using Elastos::Droid::Service::IoThread;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

const String PacManager::PAC_PACKAGE("com.android.pacprocessor");
const String PacManager::PAC_SERVICE("com.android.pacprocessor.PacService");
const String PacManager::PAC_SERVICE_NAME("com.android.net.IProxyService");

const String PacManager::PROXY_PACKAGE("com.android.proxyhandler");
const String PacManager::PROXY_SERVICE("com.android.proxyhandler.ProxyService");

const String PacManager::TAG("PacManager");

const String PacManager::ACTION_PAC_REFRESH("android.net.proxy.PAC_REFRESH");

const String PacManager::DEFAULT_DELAYS("8 32 120 14400 43200");
const Int32 PacManager::DELAY_1 = 0;
const Int32 PacManager::DELAY_4 = 3;
const Int32 PacManager::DELAY_LONG = 4;

/** Keep these values up-to-date with ProxyService.java */
const String PacManager::KEY_PROXY("keyProxy");

//============================================================================
// PacManager::PacDownloaderRunnable
//============================================================================
PacManager::PacDownloaderRunnable::PacDownloaderRunnable(
    /* [in] */ PacManager* host)
    : mHost(host)
{}

ECode PacManager::PacDownloaderRunnable::Run()
{
    String file;
    Object& lockObj = mHost->mProxyLock;
    synchronized(lockObj) {
        AutoPtr<IUriHelper> helper;
        CUriHelper::AcquireSingleton((IUriHelper**)&helper);
        AutoPtr<IUri> emptyUri;
        helper->GetEMPTY((IUri**)&emptyUri);
        if (Object::Equals(emptyUri, mPacUrl)) return NOERROR;

        ECode ec = mHost->Get(mHost->mPacUrl, &file);
        if (ec == (ECode)(E_IO_EXCEPTION) {
            file = StringNULL);
            Logger::W("PacManager", "Failed to load PAC file: IOException");
        }
    }

    if (file != NULL) {
            Object& lockObj = mHost->mProxyLock;
            synchronized(lockObj) {
            if (!file.Equals(mHost->mCurrentPac)) {
                mHost->SetCurrentProxyScript(file);
            }
        }
        mHost->mHasDownloaded = TRUE;
        mHost->SendProxyIfNeeded();
        mHost->LongSchedule();
    }
    else {
        mHost->Reschedule();
    }
    return NOERROR;
}

//============================================================================
// PacManager::PacRefreshIntentReceiver
//============================================================================

PacManager::PacRefreshIntentReceiver::PacRefreshIntentReceiver(
    /* [in] */ PacManager* host)
    : mHost(host)
{}

ECode PacManager::PacRefreshIntentReceiverOnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    IoThread::GetHandler()->Post(mPacDownloader);
    return NOERROR;
}

//============================================================================
// PacManager::ServiceConnection
//============================================================================
CAR_INTREFACE_DECL(PacManager::ServiceConnection, Object, IServiceConnection)

PacManager::ServiceConnection::ServiceConnection(
    /* [in] */ PacManager* host)
    : mHost(host)
{}

ECode PacManager::ServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* component)
{
    Object& lockObj = mHost->mProxyLock;
    synchronized(lockObj) {
        mHost->mProxyService = NULL;
    }
    return NOERROR;
}

ECode PacManager::ServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* component,
    /* [in] */ IBinder* binder)
{
    Object& lockObj = mHost->mProxyLock;
    synchronized(lockObj) {
        Logger::D("PacManager::ServiceConnection", "Adding service %s %s",
            PacManager::PAC_SERVICE_NAME.string(), Object::ToString(binder).string());

        AutoPtr<IServiceManager> srvMrg;
        CServiceManager::AcquireSingleton((IServiceManager**)&srvMrg);
        srvMrg->AddService(PacManager::PAC_SERVICE_NAME, binder);
        mProxyService = IIProxyService::Probe(binder);
        if (mProxyService == NULL) {
            Logger::E("PacManager::ServiceConnection", "No proxy service");
        }
        else {
            ECode ec = mProxyService->StartPacSystem();
            if (ec == (ECode)E_REMOTE_EXCEPTION)
                Logger::E("PacManager::ServiceConnection", "Unable to reach ProxyService - PAC will not be started");
            }
            IoThread::GetHandler()->Post(mHost->mPacDownloader);
        }
    }
    return NOERROR;
}

//============================================================================
// PacManager::ProxyConnection
//============================================================================
CAR_INTREFACE_DECL(PacManager::ProxyConnection, Object, IServiceConnection)

PacManager::ProxyConnection::ProxyConnection(
    /* [in] */ PacManager* host)
    : mHost(host)
{}

ECode PacManager::ProxyConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* component)
{
    return NOERROR;
}

ECode PacManager::ProxyConnection::OnServiceConnected(
    /* [in] */ IComponentName* component,
    /* [in] */ IBinder* binder)
{
    AutoPtr<IIProxyCallback> callbackService = IIProxyCallback::Probe(binder);
    if (callbackService != NULL) {

        assert(0 && "TODO");
        // callbackService.getProxyPort(new IProxyPortListener.Stub() {
        //     //@Override
        //     public CARAPI SetProxyPort(
        //         /* [in] */ Int32 port)
        //     {
        //         if (mLastPort != -1) {
        //             // Always need to send if port changed
        //             mHost->mHasSentBroadcast = FALSE;
        //         }
        //         mLastPort = port;
        //         if (port != -1) {
        //             Logger::D("PacManager::ProxyConnection", "Local proxy is bound on %d", port);
        //             mHost->SendProxyIfNeeded();
        //         } else {
        //             Logger::E(TAG, "Received invalid port from Local Proxy,"
        //                 " PAC will not be operational");
        //         }
        //     }
        // });
    }
}
    return NOERROR;
}

//============================================================================
// PacManager
//============================================================================
PacManager::PacManager(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ Int32 proxyMessage)
    : mCurrentDelay(0)
    , mLastPort(0)
    , mHasSentBroadcast(0)
    , mHasDownloaded(0)
    , mProxyMessage(0)
{
    mContext = context;
    mLastPort = -1;

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> emptyUri;
    helper->GetEMPTY((IUri**)&emptyUri);
    mPacUrl = emptyUri;

    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    AutoPtr<IIntent> intent;
    CIntent(ACTION_PAC_REFRESH, (IIntent**)&intent);
    helper->GetBroadcast(context, 0, intent, 0, (IPendingIntent**)&mPacRefreshIntent)

    AutoPtr<IBroadcastReceiver> receiver = new PacRefreshIntentReceiver(this);
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(ACTION_PAC_REFRESH, (IIntentFilter**)&filter);
    AutoPtr<IIntent> stickyIntent;
    context->RegisterReceiver(receiver, filter, ((IIntent**)stickyIntent);
    mConnectivityHandler = handler;
    mProxyMessage = proxyMessage;
}

AutoPtr<IAlarmManager> PacManager::GetAlarmManager()
{
    if (mAlarmManager == NULL) {
        AutoPtr<IInterface> obj;
        mContext->GetSystemService(IContext::ALARM_SERVICE, (IInterface**)&obj);
        mAlarmManager = IAlarmManager::Probe(obj);
    }
    return mAlarmManager;
}

Boolean PacManager::SetCurrentProxyScriptUrl(
    /* [in] */ IProxyInfo* proxy)
{
    AutoLock lock(this);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> emptyUri;
    helper->GetEMPTY((IUri**)&emptyUri);

    AutoPtr<IUri> pacFileUri;
    proxy->GetPacFileUrl((IUri**)&pacFileUri);
    if (!Object::Equals(emptyUri, pacFileUri)) {
        Int32 port;
        if (Object::Equals(pacFileUri, mPacUrl) && (proxy->GetPort(&port), port) > 0)) {
            // Allow to send broadcast, nothing to do.
            return FALSE;
        }
        synchronized(mProxyLock) {
            mPacUrl = NULL;
            proxy->GetPacFileUrl((IUri**)&mPacUrl);
        }
        mCurrentDelay = DELAY_1;
        mHasSentBroadcast = FALSE;
        mHasDownloaded = FALSE;
        GetAlarmManager()->Cancel(mPacRefreshIntent);
        Bind();
        return TRUE;
    }
    else {
        GetAlarmManager()->Cancel(mPacRefreshIntent);
        synchronized(mProxyLock) {
            mPacUrl = emptyUri;
            mCurrentPac = NULL;
            if (mProxyService != NULL) {
                ECode ec = mProxyService->StopPacSystem();
                if (ec == (ECode)E_REMOTE_EXCEPTION) {
                    Logger::W(TAG, "Failed to stop PAC service");
                }
                Unbind();
            }
        }
    }

    return FALSE;
}

ECode PacManager::Get(
    /* [in] */ IUri* pacUri,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = String(NULL);

    String str = Object::ToString(pacFileUri);
    AutoPtr<IURL> url;
    CURL::New(str, (IURL**)&url);
    AutoPtr<IURLConnection> urlConnection;
    FAIL_RETURN(url->OpenConnection(Elastos::Droid::Net::IProxy::NO_PROXY))
    AutoPtr<IInputStream> is;
    urlConnection->GetInputStream((IInputStream**)&is);
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    AutoPtr< ArrayOf<Byte> > bytes;
    streams->ReadFully(is, (ArrayOf<Byte>**)&bytes)
    String str(*bytes);
    *result = str;
    return NOERROR;
}

Int32 PacManager::GetNextDelay(
    /* [in] */ Int32 currentDelay)
{
   if (++currentDelay > DELAY_4) {
       return DELAY_4;
   }
   return currentDelay;
}

void PacManager::LongSchedule()
{
    mCurrentDelay = DELAY_1;
    SetDownloadIn(DELAY_LONG);
}

void PacManager::Reschedule()
{
    mCurrentDelay = GetNextDelay(mCurrentDelay);
    SetDownloadIn(mCurrentDelay);
}

String PacManager::GetPacChangeDelay()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);

    /** Check system properties for the default value then use secure settings value, if any. */
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    StringBuilder sb("conn.");
    sb += ISettingsGlobal::PAC_CHANGE_DELAY;
    String defaultDelay;
    sysProp->Get(sb.ToString(), DEFAULT_DELAYS, &defaultDelay);
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    String val;
    sg->GetString(cr, ISettingsGlobal::PAC_CHANGE_DELAY, &val);
    return (val.IsNull()) ? defaultDelay : val;
}

Int64 PacManager::GetDownloadDelay(
    /* [in] */ Int32 delayIndex)
{
    String delay = GetPacChangeDelay();
    AutoPtr<ArrayOf<String> > list;
    StringUtils::Split(delay, " ", (ArrayOf<String>**)&list);
    if (delayIndex < list->GetLength()) {
        String str = (*list)[delayIndex];
        return StringUtils::ParseInt64(str);
    }
    return 0;
}

void PacManager::SetDownloadIn(
    /* [in] */ Int32 delayIndex)
{
    Int64 delay = GetDownloadDelay(delayIndex);
    Int64 timeTillTrigger = 1000 * delay + SystemClock::GetElapsedRealtime();
    GetAlarmManager()->Set(IAlarmManager::ELAPSED_REALTIME, timeTillTrigger, mPacRefreshIntent);
}

Boolean PacManager::SetCurrentProxyScript(
    /* [in] */ const String& script)
{
    if (mProxyService == NULL) {
        Logger::E(TAG, "SetCurrentProxyScript: no proxy service");
        return FALSE;
    }
    // try {
    ECode ec = mProxyService->SetPacFile(script);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(TAG, "Unable to set PAC file");
        return FALSE;
    }

    mCurrentPac = script;
    return TRUE;
}

void PacManager::Bind()
{
    if (mContext == NULL) {
        Logger::E(TAG, "No context for binding");
        return;
    }

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetClassName(PAC_PACKAGE, PAC_SERVICE);
    // Already bound no need to Bind again.
    if ((mProxyConnection != NULL) && (mConnection != NULL)) {
        if (mLastPort != -1) {
            AutoPtr<IProxyInfo> info;
            CProxyInfo::New(mPacUrl, mLastPort, (IProxyInfo**)&info);
            SendPacBroadcast(info);
        } else {
            Logger::E(TAG, "Received invalid port from Local Proxy,"
                    + " PAC will not be operational");
        }
        return;
    }

    mConnection = new ServiceConnection(this);
    mContext->BindService(intent, mConnection,
        IContext::BIND_AUTO_CREATE | IContext::BIND_NOT_FOREGROUND | IContext::BIND_NOT_VISIBLE);

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetClassName(PROXY_PACKAGE, PROXY_SERVICE);
    mProxyConnection = new ServiceConnection(this);
    mContext->BindService(intent, mProxyConnection,
        IContext::BIND_AUTO_CREATE | IContext::BIND_NOT_FOREGROUND | IContext::BIND_NOT_VISIBLE);
}

void PacManager::Unbind()
{
    if (mConnection != NULL) {
        mContext->UnBindService(mConnection);
        mConnection = NULL;
    }
    if (mProxyConnection != NULL) {
        mContext->UnBindService(mProxyConnection);
        mProxyConnection = NULL;
    }
    mProxyService = NULL;
    mLastPort = -1;
}

void PacManager::SendPacBroadcast(
    /* [in] */ IProxyInfo* proxy)
{
    AutoPtr<IMessage> msg;
    mConnectivityHandler->ObtainMessage(mProxyMessage, proxy, (IMessage**)&msg);
    mConnectivityHandler->SendMessage(msg, &bval);
}

void PacManager::SendProxyIfNeeded()
{
    AutoLock lock(this);
    if (!mHasDownloaded || (mLastPort == -1)) {
        return;
    }
    if (!mHasSentBroadcast) {
        AutoPtr<IProxyInfo> info;
        CProxyInfo::New(mPacUrl, mLastPort, (IProxyInfo**)&info);
        SendPacBroadcast(info);
        mHasSentBroadcast = TRUE;
    }
}



} // Connectivity
} // Server
} // Droid
} // Elastos
