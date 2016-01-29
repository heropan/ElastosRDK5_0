
#include "elastos/droid/server/MmsServiceBroker.h"
#include <elastos/droid/Manifest.h>
#include <elastos/droid/os/Binder.h>

using Elastos::Droid::Manifest;
using Elastos::Droid::Os::Binder;

namespace Elastos {
namespace Droid {
namespace Server {

const String MmsServiceBroker::TAG("MmsServiceBroker");

const ComponentName MmsServiceBroker::MMS_SERVICE_COMPONENT =
        new ComponentName("com.android.mms.service", "com.android.mms.service.MmsService");

const Int32 MmsServiceBroker::MSG_TRY_CONNECTING = 1;

const AutoPtr<IUri> MmsServiceBroker::FAKE_SMS_SENT_URI = Uri->Parse("content://sms/sent/0");
const AutoPtr<IUri> MmsServiceBroker::FAKE_MMS_SENT_URI = Uri->Parse("content://mms/sent/0");
const AutoPtr<IUri> MmsServiceBroker::FAKE_SMS_DRAFT_URI = Uri->Parse("content://sms/draft/0");
const AutoPtr<IUri> MmsServiceBroker::FAKE_MMS_DRAFT_URI = Uri->Parse("content://mms/draft/0");

const Int64 MmsServiceBroker::SERVICE_CONNECTION_WAIT_TIME_MS = 4 * 1000L; // 4 seconds
const Int64 MmsServiceBroker::RETRY_DELAY_ON_DISCONNECTION_MS = 3 * 1000L; // 3 seconds

//==========================================================================================
// MmsServiceBroker::BinderService
//==========================================================================================

CAR_INTERFACE_IMPL_2(MmsServiceBroker::BinderService, Object, IIMms, IBinder)

ECode MmsServiceBroker::BinderService::constructor(
    /* [in] */ ISystemService* mmsServiceBroker)
{
    mHost = (MmsServiceBroker*)mmsServiceBroker;
    return NOERROR;
}

//@Override
ECode MmsServiceBroker::BinderService::SendMessage(
    /* [in] */ Int64 subId,
    /* [in] */ const String& callingPkg,
    /* [in] */ IUri* contentUri,
    /* [in] */ const String& locationUrl,
    /* [in] */ IBundle* configOverrides,
    /* [in] */ IPendingIntent* sentIntent)
{
    ECode ec = mHost->mContext->EnforceCallingPermission(Manifest::permission::SEND_SMS, "Send MMS message");
    FAIL_RETURN(ec)

    AutoPtr<IAppOpsManager> aom = mHost->GetAppOpsManager();
    if (->NoteOp(AppOpsManager.OP_SEND_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        return;
    }
    GetServiceGuarded()->SendMessage(subId, callingPkg, contentUri, locationUrl,
            configOverrides, sentIntent);
}

//@Override
ECode MmsServiceBroker::BinderService::DownloadMessage(
    /* [in] */ Int64 subId,
    /* [in] */ const String& callingPkg,
    /* [in] */ const String& locationUrl,
    /* [in] */ IUri* contentUri,
    /* [in] */ IBundle* configOverrides,
    /* [in] */ IPendingIntent* downloadedIntent)
{
    mContext->EnforceCallingPermission(Manifest::permission::RECEIVE_MMS,
            "Download MMS message");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_RECEIVE_MMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        return;
    }
    GetServiceGuarded()->DownloadMessage(subId, callingPkg, locationUrl, contentUri,
            configOverrides, downloadedIntent);
}

//@Override
ECode MmsServiceBroker::BinderService::UpdateMmsSendStatus(
    /* [in] */ Int32 messageRef,
    /* [in] */ ArrayOf<Byte>* pdu,
    /* [in] */ Int32 status)
{
    EnforceCarrierPrivilege();
    GetServiceGuarded()->UpdateMmsSendStatus(messageRef, pdu, status);
}

//@Override
ECode MmsServiceBroker::BinderService::UpdateMmsDownloadStatus(
    /* [in] */ Int32 messageRef,
    /* [in] */ Int32 status)
{
    EnforceCarrierPrivilege();
    GetServiceGuarded()->UpdateMmsDownloadStatus(messageRef, status);
}

//@Override
ECode MmsServiceBroker::BinderService::GetCarrierConfigValues(
    /* [in] */ Int64 subId,
    /* [out] */ IBundle* bundle)
{
    return GetServiceGuarded()->GetCarrierConfigValues(subId);
}

//@Override
ECode MmsServiceBroker::BinderService::ImportTextMessage(
    /* [in] */ const String& callingPkg,
    /* [in] */ const String& address,
    /* [in] */ Int32 type,
    /* [in] */ const String& text,
    /* [in] */ Int64 timestampMillis,
    /* [in] */ Boolean seen,
    /* [in] */ Boolean read,
    /* [out] */ IUri** uri)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS, "Import SMS message");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        // Silently fail AppOps failure due to not being the default SMS app
        // while writing the TelephonyProvider
        return FAKE_SMS_SENT_URI;
    }
    return GetServiceGuarded()->ImportTextMessage(
            callingPkg, address, type, text, timestampMillis, seen, read);
}

//@Override
ECode MmsServiceBroker::BinderService::ImportMultimediaMessage(
    /* [in] */ const String& callingPkg,
    /* [in] */ Uri contentUri,
    /* [in] */ const String& messageId,
    /* [in] */ Int64 timestampSecs,
    /* [in] */ Boolean seen,
    /* [in] */ Boolean read,
    /* [out] */ IUri** uri)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS, "Import MMS message");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        // Silently fail AppOps failure due to not being the default SMS app
        // while writing the TelephonyProvider
        return FAKE_MMS_SENT_URI;
    }
    return GetServiceGuarded()->ImportMultimediaMessage(
            callingPkg, contentUri, messageId, timestampSecs, seen, read);
}

//@Override
ECode MmsServiceBroker::BinderService::DeleteStoredMessage(
    /* [in] */ const String& callingPkg,
    /* [in] */ IUri* messageUri,
    /* [out] */ Boolean* result)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS,
            "Delete SMS/MMS message");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        return FALSE;
    }
    return GetServiceGuarded()->DeleteStoredMessage(callingPkg, messageUri);
}

//@Override
ECode MmsServiceBroker::BinderService::DeleteStoredConversation(
    /* [in] */ const String& callingPkg,
    /* [in] */ Int64 conversationId,
    /* [out] */ Boolean* result)
        {
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS, "Delete conversation");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        return FALSE;
    }
    return GetServiceGuarded()->DeleteStoredConversation(callingPkg, conversationId);
}

//@Override
ECode MmsServiceBroker::BinderService::UpdateStoredMessageStatus(
    /* [in] */ const String& callingPkg,
    /* [in] */ IUri* messageUri,
    /* [in] */ IContentValues* statusValues,
    /* [out] */ Boolean* result)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS,
            "Update SMS/MMS message");
    return GetServiceGuarded()
            .UpdateStoredMessageStatus(callingPkg, messageUri, statusValues);
}

//@Override
ECode MmsServiceBroker::BinderService::ArchiveStoredConversation(
    /* [in] */ const String& callingPkg,
    /* [in] */ Int64 conversationId,
    /* [in] */ Boolean archived,
    /* [out] */ Boolean* result)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS,
            "Update SMS/MMS message");
    return GetServiceGuarded()
            .ArchiveStoredConversation(callingPkg, conversationId, archived);
}

//@Override
ECode MmsServiceBroker::BinderService::AddTextMessageDraft(
    /* [in] */ const String& callingPkg,
    /* [in] */ const String& address,
    /* [in] */ const String& text,
    /* [out] */ IUri** uri)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS, "Add SMS draft");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        // Silently fail AppOps failure due to not being the default SMS app
        // while writing the TelephonyProvider
        return FAKE_SMS_DRAFT_URI;
    }
    return GetServiceGuarded()->AddTextMessageDraft(callingPkg, address, text);
}

//@Override
ECode MmsServiceBroker::BinderService::AddMultimediaMessageDraft(
    /* [in] */ const String& callingPkg,
    /* [in] */ IUri* contentUri,
    /* [out] */ IUri** uri)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS, "Add MMS draft");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        // Silently fail AppOps failure due to not being the default SMS app
        // while writing the TelephonyProvider
        return FAKE_MMS_DRAFT_URI;
    }
    return GetServiceGuarded()->AddMultimediaMessageDraft(callingPkg, contentUri);
}

//@Override
ECode MmsServiceBroker::BinderService::SendStoredMessage(
    /* [in] */ Int64 subId,
    /* [in] */ const String& callingPkg,
    /* [in] */ IUri* messageUri,
    /* [in] */ IBundle* configOverrides,
    /* [in] */ IPendingIntent* sentIntent)
{
    mContext->EnforceCallingPermission(Manifest::permission::SEND_SMS,
            "Send stored MMS message");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_SEND_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        return;
    }
    GetServiceGuarded()->SendStoredMessage(subId, callingPkg, messageUri, configOverrides,
            sentIntent);
}

//@Override
ECode MmsServiceBroker::BinderService::SetAutoPersisting(
    /* [in] */ const String& callingPkg,
    /* [in] */ Boolean enabled)
{
    mContext->EnforceCallingPermission(Manifest::permission::WRITE_SMS, "Set auto persist");
    if (GetAppOpsManager()->NoteOp(AppOpsManager.OP_WRITE_SMS, Binder::GetCallingUid(),
            callingPkg) != AppOpsManager.MODE_ALLOWED) {
        return;
    }
    GetServiceGuarded()->SetAutoPersisting(callingPkg, enabled);
}

//@Override
ECode MmsServiceBroker::BinderService::GetAutoPersisting(
    /* [out] */ Boolean* result)
{
    return GetServiceGuarded()->GetAutoPersisting();
}


//==========================================================================================
// MmsServiceBroker::ConnectionHandler
//==========================================================================================
class ConnectionHandler
    : public Handler
{
public:
ConnectionHandler(
    /* [in] */ MmsServiceBroker* host);

//@Override
ECode MmsServiceBroker::ConnectionHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    switch (msg.what) {
        case MSG_TRY_CONNECTING:
            TryConnecting();
            break;
        default:
            Slogger::E(TAG, "Unknown message");
    }
}
private:
    MmsServiceBroker* mHost;
};

//==========================================================================================
// MmsServiceBroker::Connection
//==========================================================================================
class Connection
    : public Object
    , public IServiceConnection
{
public:
    Connection(
        /* [in] */ MmsServiceBroker* host);

    //@Override
    ECode MmsServiceBroker::Connection::OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service)
    {
        Slogger::I(TAG, "MmsService connected");
        synchronized(MmsServiceBroker.this) {
            mService = IMms.Stub->AsInterface(service);
            MmsServiceBroker.this->NotifyAll();
        }
    }

    //@Override
    ECode MmsServiceBroker::Connection::OnServiceDisconnected(
        /* [in] */ IComponentName* name)
    {
        Slogger::I(TAG, "MmsService unexpectedly disconnected");
        synchronized(MmsServiceBroker.this) {
            mService = NULL;
            MmsServiceBroker.this->NotifyAll();
        }
        // Retry connecting, but not too eager (with a delay)
        // since it may come back by itself.
        mConnectionHandler->SendMessageDelayed(
                mConnectionHandler->ObtainMessage(MSG_TRY_CONNECTING),
                RETRY_DELAY_ON_DISCONNECTION_MS);
    }
private:
    MmsServiceBroker* mHost;
};


//==========================================================================================
// MmsServiceBroker
//==========================================================================================
MmsServiceBroker();

ECode MmsServiceBroker::constructor(
    /* [in] */ IContext* context)
{
    Super(context);
    mContext = context;
    mService = NULL;
}

//@Override
ECode MmsServiceBroker::OnStart()
{
    PublishBinderService("imms", new BinderService());
}

ECode MmsServiceBroker::SystemRunning()
{
    TryConnecting();
}

private:
void TryConnecting()
{
    Slogger::I(TAG, "Connecting to MmsService");
    synchronized(this) {
        if (mService != NULL) {
            Slogger::D(TAG, "Already connected");
            return;
        }
        final Intent intent = new Intent();
        intent->SetComponent(MMS_SERVICE_COMPONENT);
        try {
            if (!mContext->BindService(intent, mConnection, Context.BIND_AUTO_CREATE)) {
                Slogger::E(TAG, "Failed to bind to MmsService");
            }
        } catch (SecurityException e) {
            Slogger::E(TAG, "Forbidden to bind to MmsService", e);
        }
    }
}

void EnsureService()
{
    synchronized(this) {
        if (mService == NULL) {
            // Service is not connected. Try blocking connecting.
            Slogger::W(TAG, "MmsService not connected. Try connecting...");
            mConnectionHandler->SendMessage(
                    mConnectionHandler->ObtainMessage(MSG_TRY_CONNECTING));
            final Int64 shouldEnd =
                    SystemClock->ElapsedRealtime() + SERVICE_CONNECTION_WAIT_TIME_MS;
            Int64 waitTime = SERVICE_CONNECTION_WAIT_TIME_MS;
            while (waitTime > 0) {
                try {
                    // TODO: consider using Java concurrent construct instead of raw object wait
                    this->Wait(waitTime);
                } catch (InterruptedException e) {
                    Slogger::W(TAG, "Connection wait interrupted", e);
                }
                if (mService != NULL) {
                    // Success
                    return;
                }
                // Calculate remaining waiting time to make sure we wait the full timeout period
                waitTime = shouldEnd - SystemClock->ElapsedRealtime();
            }
            // Timed out. Something's really wrong.
            Slogger::E(TAG, "Can not connect to MmsService (timed out)");
            throw new RuntimeException("Timed out in connecting to MmsService");
        }
    }
}

/**
 * Making sure when we obtain the mService instance it is always valid.
 * Throws {@link RuntimeException} when it is empty.
 */
AutoPtr<IIMms> GetServiceGuarded()
{
    EnsureService();
    return mService;
}

AutoPtr<IAppOpsManager> GetAppOpsManager()
{
    if (mAppOpsManager == NULL) {
        mAppOpsManager = (AppOpsManager) mContext->GetSystemService(Context.APP_OPS_SERVICE);
    }
    return mAppOpsManager;
}

AutoPtr<IPackageManager> GetPackageManager()
{
    if (mPackageManager == NULL) {
        mPackageManager = mContext->GetPackageManager();
    }
    return mPackageManager;
}

AutoPtr<ITelephonyManager> GetTelephonyManager()
{
    if (mTelephonyManager == NULL) {
        mTelephonyManager = (TelephonyManager) mContext->GetSystemService(
                Context.TELEPHONY_SERVICE);
    }
    return mTelephonyManager;
}

/*
 * Throws a security exception unless the caller has carrier privilege.
 */
ECode MmsServiceBroker::EnforceCarrierPrivilege()
{
    String[] packages = GetPackageManager()->GetPackagesForUid(Binder::GetCallingUid());
    for (String pkg : packages) {
        if (GetTelephonyManager()->CheckCarrierPrivilegesForPackage(pkg) ==
                TelephonyManager.CARRIER_PRIVILEGE_STATUS_HAACCESS) {
            return;
        }
    }
    throw new SecurityException("No carrier privilege");
}


} // namespace Server
} // namespace Droid
} // namespace Elastos
