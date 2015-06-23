
#include "am/ActiveServices.h"
#include "am/CActivityManagerService.h"
#include "am/ProcessRecord.h"
#include "am/AppBindRecord.h"
#include "am/ConnectionRecord.h"
#include "am/IntentBindRecord.h"
#include "app/AppGlobals.h"
#include "os/Binder.h"
#include "os/Handler.h"
#include "os/UserHandle.h"
#include "util/TimeUtils.h"
#include "Manifest.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IFlushable;
using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::App::AppGlobals;
using Elastos::Droid::Utility::TimeUtils;
using Elastos::Droid::Content::Pm::CServiceInfo;
using Elastos::Droid::Content::CIntentFilterComparison;
using Elastos::Droid::Content::IComponentNameHelper;
using Elastos::Droid::Content::CComponentNameHelper;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::UserHandle;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// {38a4362d-5432-4f9b-adb5-9ae9404fd3ea}
extern "C" const InterfaceID EIID_ServiceRestarter =
        { 0x38a4362d, 0x5432, 0x4f9b, { 0xad, 0xb5, 0x9a, 0xe9, 0x40, 0x4f, 0xd3, 0xea } };

const String ActiveServices::TAG = CActivityManagerService::TAG;
const String ActiveServices::TAG_MU = CActivityManagerService::TAG_MU;
const Boolean ActiveServices::DEBUG_SERVICE = CActivityManagerService::DEBUG_SERVICE;
const Boolean ActiveServices::DEBUG_SERVICE_EXECUTING = CActivityManagerService::DEBUG_SERVICE_EXECUTING;
const Boolean ActiveServices::DEBUG_MU = CActivityManagerService::DEBUG_MU;


ActiveServices::ServiceMap::ServiceMap()
    : mServicesByNamePerUser(20)
    , mServicesByIntentPerUser(20)
{}

ActiveServices::ServiceMap::~ServiceMap()
{
    mServicesByNamePerUser.Clear();
    mServicesByIntentPerUser.Clear();
}

AutoPtr<CServiceRecord> ActiveServices::ServiceMap::GetServiceByName(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 callingUser)
{
    // TODO: Deal with global services
    if (ActiveServices::DEBUG_MU) {
        Slogger::V(TAG_MU, "getServiceByName(%p), callingUser = %d", name, callingUser);
    }
    AutoPtr<IComponentNameCServiceRecordHashMap> map = GetServices(callingUser);
    AutoPtr<CServiceRecord> record;
    IComponentNameCServiceRecordHashMap::Iterator it = map->Find(name);
    if (it != map->End()) {
        record = it->mSecond;
    }
    return record;
}

AutoPtr<CServiceRecord> ActiveServices::ServiceMap::GetServiceByName(
    /* [in] */ IComponentName* name)
{
    return GetServiceByName(name, -1);
}

AutoPtr<CServiceRecord> ActiveServices::ServiceMap::GetServiceByIntent(
    /* [in] */ IIntentFilterComparison* filter,
    /* [in] */ Int32 callingUser)
{
    // TODO: Deal with global services
    if (ActiveServices::DEBUG_MU) {
        Slogger::V(TAG_MU, "getServiceByIntent(%p), callingUser = %d", filter, callingUser);
    }
    AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> map = GetServicesByIntent(callingUser);
    AutoPtr<CServiceRecord> record;
    IIntentFilterComparisonCServiceRecordHashMap::Iterator it = map->Find(filter);
    if (it != map->End()) {
        record = it->mSecond;
    }
    return record;
}

AutoPtr<CServiceRecord> ActiveServices::ServiceMap::GetServiceByIntent(
    /* [in] */ IIntentFilterComparison* filter)
{
    return GetServiceByIntent(filter, -1);
}

void ActiveServices::ServiceMap::PutServiceByName(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 callingUser,
    /* [in] */ CServiceRecord* value)
{
    // TODO: Deal with global services
    AutoPtr<IComponentNameCServiceRecordHashMap> map = GetServices(callingUser);
    (*map)[name] = value;
}

void ActiveServices::ServiceMap::PutServiceByIntent(
    /* [in] */ IIntentFilterComparison* filter,
    /* [in] */ Int32 callingUser,
    /* [in] */ CServiceRecord* value)
{
    // TODO: Deal with global services
    AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> map = GetServicesByIntent(callingUser);
    (*map)[filter] = value;
}

void ActiveServices::ServiceMap::RemoveServiceByName(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 callingUser)
{
   // TODO: Deal with global services
   AutoPtr<IComponentNameCServiceRecordHashMap> map = GetServices(callingUser);
   AutoPtr<CServiceRecord> removed = (*map)[name];
   map->Erase(name);
   if (ActiveServices::DEBUG_MU)
       Slogger::V(TAG, "removeServiceByName user=%d name=%p removed=%p", callingUser, name, removed.Get());
}

void ActiveServices::ServiceMap::RemoveServiceByIntent(
    /* [in] */ IIntentFilterComparison* filter,
    /* [in] */ Int32 callingUser)
{
   // TODO: Deal with global services
   AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> map = GetServicesByIntent(callingUser);
   AutoPtr<CServiceRecord> removed = (*map)[filter];
   map->Erase(filter);
   if (ActiveServices::DEBUG_MU)
       Slogger::V(TAG_MU, "removeServiceByIntent user=%d intent=%p removed=%p", callingUser, filter, removed.Get());
}

// Collection<CServiceRecord> getAllServices(int callingUser) {
//     // TODO: Deal with global services
//     return getServices(callingUser).values();
// }

AutoPtr<IComponentNameCServiceRecordHashMap> ActiveServices::ServiceMap::GetServices(
    /* [in] */ Int32 callingUser)
{
    AutoPtr<IComponentNameCServiceRecordHashMap> map;
    HashMap<Int32, AutoPtr<IComponentNameCServiceRecordHashMap> >::Iterator it =
            mServicesByNamePerUser.Find(callingUser);
    if (it != mServicesByNamePerUser.End()) {
        map = it->mSecond;
    }
    if (map == NULL) {
        map = new IComponentNameCServiceRecordHashMap();
        mServicesByNamePerUser[callingUser] = map;
    }
    return map;
}

AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> ActiveServices::ServiceMap::GetServicesByIntent(
    /* [in] */ Int32 callingUser)
{
    AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> map;
    HashMap<Int32, AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> >::Iterator it =
            mServicesByIntentPerUser.Find(callingUser);
    if (it != mServicesByIntentPerUser.End()) {
        map = it->mSecond;
    }
    if (map == NULL) {
        map = new IIntentFilterComparisonCServiceRecordHashMap();
        mServicesByIntentPerUser[callingUser] = map;
    }
    return map;
}


ActiveServices::ServiceLookupResult::ServiceLookupResult(
    /* [in] */ CServiceRecord* record,
    /* [in] */ const String& permission)
    : mRecord(record)
    , mPermission(permission)
{}


ActiveServices::ServiceRestarter::ServiceRestarter(
    /* [in] */ ActiveServices* owner)
    : mService(NULL)
    , mOwner(owner)
{}

PInterface ActiveServices::ServiceRestarter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    else if (riid == EIID_ServiceRestarter) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 ActiveServices::ServiceRestarter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ActiveServices::ServiceRestarter::Release()
{
    return ElRefBase::Release();
}

ECode ActiveServices::ServiceRestarter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

void ActiveServices::ServiceRestarter::SetService(
    /* [in] */ CServiceRecord* service)
{
    mService = service;
}

ECode ActiveServices::ServiceRestarter::Run()
{
    Object::Autolock lock(mOwner->mAm->mLock);
    return mOwner->PerformServiceRestartLocked(mService);
}

ActiveServices::ActiveServices(
    /* [in] */ CActivityManagerService* service)
    : mAm(service)
    , mServiceConnections(50)
{
    mServiceMap = new ServiceMap();
}

ActiveServices::~ActiveServices()
{
    mServiceConnections.Clear();
    mPendingServices.Clear();
    mRestartingServices.Clear();
    mStoppingServices.Clear();
}

ECode ActiveServices::StartServiceLocked(
    /* [in] */ IApplicationThread* caller,
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 userId,
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = NULL;

   AutoPtr<IBundle> extras;
   service->GetExtras((IBundle**)&extras);
   if (DEBUG_SERVICE) Slogger::V(TAG, "startService: %p type=%s args=%p", service, resolvedType.string(), extras.Get());

   if (caller != NULL) {
       AutoPtr<ProcessRecord> callerApp = mAm->GetRecordForAppLocked(caller);
       if (callerApp == NULL) {
//             throw new SecurityException(
//                     "Unable to find app for caller " + caller
//                     + " (pid=" + Binder.getCallingPid()
//                     + ") when starting service " + service);
           return E_SECURITY_EXCEPTION;
       }
   }

   AutoPtr<ServiceLookupResult> res =
       RetrieveServiceLocked(service, resolvedType,
               callingPid, callingUid, userId, TRUE);
   if (res == NULL) {
       return NOERROR;
   }
   if (res->mRecord == NULL) {
       return CComponentName::New(String("!"), (!res->mPermission.IsNull())
               ? res->mPermission : String("private to package"), name);
   }
   AutoPtr<CServiceRecord> r = res->mRecord;
   Int32 flags;
   service->GetFlags(&flags);
   AutoPtr<NeededUriGrants> neededGrants = mAm->CheckGrantUriPermissionFromIntentLocked(
           callingUid, r->mPackageName, service, flags, NULL);
   if (UnscheduleServiceRestartLocked(r)) {
       if (DEBUG_SERVICE) Slogger::V(TAG, "START SERVICE WHILE RESTART PENDING: %p", r.Get());
   }
   r->mStartRequested = TRUE;
   r->mCallStart = FALSE;
   AutoPtr<CServiceRecord::StartItem> item = new CServiceRecord::StartItem(r, FALSE, r->MakeNextStartId(),
           service, neededGrants);
   r->mPendingStarts.PushBack(item);
   r->mLastActivity = SystemClock::GetUptimeMillis();
   r->mStats->GetBatteryStats()->Lock();
   r->mStats->StartRunningLocked();
   r->mStats->GetBatteryStats()->Unlock();
   String error = BringUpServiceLocked(r, flags, FALSE);
   if (!error.IsNull()) {
       return CComponentName::New(String("!!"), error, name);
   }
   *name = r->mName;
   INTERFACE_ADDREF(*name);
   return NOERROR;
}

ECode ActiveServices::StopServiceLocked(
    /* [in] */ CServiceRecord* service)
{
    service->mStats->GetBatteryStats()->Lock();
    service->mStats->StopRunningLocked();
    service->mStats->GetBatteryStats()->Unlock();
    service->mStartRequested = FALSE;
    service->mCallStart = FALSE;
    BringDownServiceLocked(service, FALSE);
    return NOERROR;
}

ECode ActiveServices::StopServiceLocked(
    /* [in] */ IApplicationThread* caller,
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* result)
{
  VALIDATE_NOT_NULL(result);
  *result = 0;

   if (DEBUG_SERVICE) Slogger::V(TAG, "stopService: %p type=%s", service, resolvedType.string());

   AutoPtr<ProcessRecord> callerApp = mAm->GetRecordForAppLocked(caller);
   if (caller != NULL && callerApp == NULL) {
//         throw new SecurityException(
//                 "Unable to find app for caller " + caller
//                 + " (pid=" + Binder.getCallingPid()
//                 + ") when stopping service " + service);
       return E_SECURITY_EXCEPTION;
   }

   // If this service is active, make sure it is stopped.
   AutoPtr<ServiceLookupResult> r = RetrieveServiceLocked(service, resolvedType,
           Binder::GetCallingPid(), Binder::GetCallingUid(), userId, FALSE);
   if (r != NULL) {
       if (r->mRecord != NULL) {
           Int64 origId = Binder::ClearCallingIdentity();
//             try {
           StopServiceLocked(r->mRecord);
//             } finally {
           Binder::RestoreCallingIdentity(origId);
//             }
           *result = 1;
           return NOERROR;
       }
       *result = -1;
       return NOERROR;
   }

   *result = 0;
   return NOERROR;
}

ECode ActiveServices::PeekServiceLocked(
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [out] */ IBinder** token)
{
  VALIDATE_NOT_NULL(token);
   AutoPtr<ServiceLookupResult> r = RetrieveServiceLocked(service, resolvedType,
           Binder::GetCallingPid(), Binder::GetCallingUid(),
           UserHandle::GetCallingUserId(), FALSE);

   AutoPtr<IBinder> ret = NULL;
   if (r != NULL) {
       // r.record is null if findServiceLocked() failed the caller permission check
       if (r->mRecord == NULL) {
//             throw new SecurityException(
//                     "Permission Denial: Accessing service " + r.record.name
//                     + " from pid=" + Binder.getCallingPid()
//                     + ", uid=" + Binder.getCallingUid()
//                     + " requires " + r.permission);
           return E_SECURITY_EXCEPTION;
       }

      HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IntentBindRecord> >::Iterator it =
                r->mRecord->mBindings.Find(r->mRecord->mIntent);
      if (it != r->mRecord->mBindings.End()) {
          AutoPtr<IntentBindRecord> ib = it->mSecond;
          if (ib != NULL) {
              ret = ib->mBinder;
          }
      }
   }

   *token = ret;
   INTERFACE_ADDREF(*token);
   return NOERROR;
}

ECode ActiveServices::StopServiceTokenLocked(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 startId,
    /* [out] */ Boolean* succeeded)
{
   if (DEBUG_SERVICE) Slogger::V(TAG, "stopServiceToken: %p %p startId=%d", className, token, startId);
   AutoPtr<CServiceRecord> r = FindServiceLocked(className, token, UserHandle::GetCallingUserId());
   if (r != NULL) {
       if (startId >= 0) {
           // Asked to only stop if done with all work.  Note that
           // to avoid leaks, we will take this as dropping all
           // start items up to and including this one.
           AutoPtr<CServiceRecord::StartItem> si = r->FindDeliveredStart(startId, FALSE);
           if (si != NULL) {
               while (!r->mDeliveredStarts.IsEmpty()) {
                   AutoPtr<CServiceRecord::StartItem> cur = r->mDeliveredStarts.GetFront();
                   r->mDeliveredStarts.PopFront();
                   cur->RemoveUriPermissionsLocked();
                   if (cur == si) {
                       break;
                   }
               }
           }

           if (r->GetLastStartId() != startId) {
               *succeeded = FALSE;
               return NOERROR;
           }

           if (r->mDeliveredStarts.IsEmpty() == FALSE) {
               Slogger::W(TAG, "stopServiceToken startId %d is last, but have %d remaining args", startId, r->mDeliveredStarts.GetSize());
           }
       }

       Slogger::E(TAG, "// TODO: r->mStats->GetBatteryStats() is NULL Line: %d", __LINE__);
       r->mStats->GetBatteryStats()->Lock();
       r->mStats->StopRunningLocked();
       r->mStartRequested = FALSE;
       r->mCallStart = FALSE;
       r->mStats->GetBatteryStats()->Unlock();
       Int64 origId = Binder::ClearCallingIdentity();
       BringDownServiceLocked(r, FALSE);
       Binder::RestoreCallingIdentity(origId);
       *succeeded = TRUE;
       return NOERROR;
   }
   *succeeded = FALSE;
   return NOERROR;
}

ECode ActiveServices::SetServiceForegroundLocked(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ Boolean removeNotification)
{
   const Int32 userId = UserHandle::GetCallingUserId();
   const Int64 origId = Binder::ClearCallingIdentity();
//     try {
   AutoPtr<CServiceRecord> r = FindServiceLocked(className, token, userId);
   if (r != NULL) {
       if (id != 0) {
           if (notification == NULL) {
            Binder::RestoreCallingIdentity(origId);
//                     throw new IllegalArgumentException("null notification");
               return E_ILLEGAL_ARGUMENT_EXCEPTION;
           }
           if (r->mForegroundId != id) {
               r->CancelNotification();
               r->mForegroundId = id;
           }
           Int32 flags;
           notification->GetFlags(&flags);
           notification->SetFlags(flags|INotification::FLAG_FOREGROUND_SERVICE);
           r->mForegroundNoti = notification;
           r->mIsForeground = TRUE;
           r->PostNotification();
           if (r->mApp != NULL) {
               UpdateServiceForegroundLocked(r->mApp, TRUE);
           }
       } else {
           if (r->mIsForeground) {
               r->mIsForeground = FALSE;
               if (r->mApp != NULL) {
                   mAm->UpdateLruProcessLocked(r->mApp, FALSE);
                   UpdateServiceForegroundLocked(r->mApp, TRUE);
               }
           }
           if (removeNotification) {
               r->CancelNotification();
               r->mForegroundId = 0;
               r->mForegroundNoti = NULL;
           }
       }
   }
//     } finally {
   Binder::RestoreCallingIdentity(origId);
//     }
   return NOERROR;
}

ECode ActiveServices::UpdateServiceForegroundLocked(
    /* [in] */ ProcessRecord* proc,
    /* [in] */ Boolean oomAdj)
{
   Boolean anyForeground = FALSE;
   HashSet< AutoPtr<CServiceRecord> >::Iterator it;
   for (it = proc->mServices.Begin(); it != proc->mServices.End(); ++it) {
       AutoPtr<CServiceRecord> sr = *it;
       if (sr->mIsForeground) {
           anyForeground = TRUE;
           break;
       }
   }
   if (anyForeground != proc->mForegroundServices) {
       proc->mForegroundServices = anyForeground;
       if (oomAdj) {
           mAm->UpdateOomAdjLocked();
       }
   }
   return NOERROR;
}

ECode ActiveServices::BindServiceLocked(
    /* [in] */ IApplicationThread* caller,
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* _service,
    /* [in] */ const String& resolvedType,
    /* [in] */ IIServiceConnection* connection,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* result)
{
    AutoPtr<IBinder> binder = IBinder::Probe(connection);
    if (DEBUG_SERVICE) {
        Slogger::V(TAG, "bindService: %p type=%s conn=%p flags=0x%s",
            _service, resolvedType.string(), binder.Get()
            , StringUtils::Int32ToHexString(flags).string());
    }
    AutoPtr<ProcessRecord> callerApp = mAm->GetRecordForAppLocked(caller);
    if (callerApp == NULL) {
        // throw new SecurityException(
        //         "Unable to find app for caller " + caller
        //         + " (pid=" + Binder.getCallingPid()
        //         + ") when binding service " + service);
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<ActivityRecord> activity;
    if (token != NULL) {
        activity = mAm->mMainStack->IsInStackLocked(token);
        if (activity == NULL) {
            Slogger::W(TAG, "Binding with unknown activity: %p", token);
            *result = 0;
            return NOERROR;
        }
    }

    AutoPtr<IIntent> service = _service;
    Int32 clientLabel = 0;
    AutoPtr<IPendingIntent> clientIntent;

    Int32 uid;
    callerApp->mInfo->GetUid(&uid);
    if (uid == IProcess::SYSTEM_UID) {
        // Hacky kind of thing -- allow system stuff to tell us
        // what they are, so we can report this elsewhere for
        // others to know why certain services are running.
//         try {
        AutoPtr<IParcelable> parcel;
        service->GetParcelableExtra(IIntent::EXTRA_CLIENT_INTENT, (IParcelable**)&parcel);
//        clientIntent = (PendingIntent)parcel;
//         } catch (RuntimeException e) {
//         }
        if (clientIntent != NULL) {
            service->GetInt32Extra(IIntent::EXTRA_CLIENT_LABEL, 0, &clientLabel);
            if (clientLabel != 0) {
                // There are no useful extras in the intent, trash them.
                // System code calling with this stuff just needs to know
                // this will happen.
                service = NULL;
                _service->CloneFilter((IIntent**)&service);
            }
        }
    }

    AutoPtr<ServiceLookupResult> res =
        RetrieveServiceLocked(service, resolvedType,
                Binder::GetCallingPid(), Binder::GetCallingUid(), userId, TRUE);
    if (res == NULL) {
        *result = 0;
        return NOERROR;
    }
    if (res->mRecord == NULL) {
        *result = -1;
        return NOERROR;
    }
    AutoPtr<CServiceRecord> s = res->mRecord;

    const Int64 origId = Binder::ClearCallingIdentity();

//     try {
    if (UnscheduleServiceRestartLocked(s)) {
        if (DEBUG_SERVICE) Slogger::V(TAG, "BIND SERVICE WHILE RESTART PENDING: %p", s.Get());
    }

    AutoPtr<AppBindRecord> b = s->RetrieveAppBindingLocked(service, callerApp);
    AutoPtr<ConnectionRecord> c = new ConnectionRecord(b, activity,
            connection, flags, clientLabel, clientIntent);

    AutoPtr<ConnectionRecordList> clist = s->mConnections[binder];
    if (clist == NULL) {
        clist = new ConnectionRecordList();
        s->mConnections[binder] = clist;
    }
    clist->PushBack(c);
    b->mConnections.Insert(c);
    if (activity != NULL) {
        if (activity->mConnections == NULL) {
            activity->mConnections = new HashSet<AutoPtr<ConnectionRecord> >();
        }
        activity->mConnections->Insert(c);
    }
    b->mClient->mConnections.Insert(c);
    if ((c->mFlags & IContext::BIND_ABOVE_CLIENT) != 0) {
        b->mClient->mHasAboveClient = TRUE;
    }
    clist = mServiceConnections[binder];
    if (clist == NULL) {
        clist = new ConnectionRecordList();
        mServiceConnections[binder] = clist;
    }
    clist->PushBack(c);

    if ((flags & IContext::BIND_AUTO_CREATE) != 0) {
        s->mLastActivity = SystemClock::GetUptimeMillis();
        Int32 flags;
        service->GetFlags(&flags);
        if (!BringUpServiceLocked(s, flags, FALSE).IsNull()) {
            *result = 0;
            return NOERROR;
        }
    }

    if (s->mApp != NULL) {
        // This could have made the service more important.
        mAm->UpdateOomAdjLocked(s->mApp);
    }

    if (DEBUG_SERVICE) Slogger::V(TAG, "Bind %p with %p: received=%d apps=%d doRebind=%d", s.Get(), b.Get(), b->mIntent->mReceived
            , b->mIntent->mApps.GetSize(), b->mIntent->mDoRebind);

    if (s->mApp != NULL && b->mIntent->mReceived) {
        // Service is already running, so we can immediately
        // publish the connection.
//        try {
        c->mConn->Connected(s->mName, b->mIntent->mBinder);
//        } catch (Exception e) {
//            Slogger::W(TAG, "Failure sending service " + s.shortName
//                    + " to connection " + c.conn.asBinder()
//                    + " (in " + c.binding.client.processName + ")", e);
//        }

        // If this is the first app connected back to this binding,
        // and the service had previously asked to be told when
        // rebound, then do so.
        if (b->mIntent->mApps.GetSize() == 1 && b->mIntent->mDoRebind) {
            RequestServiceBindingLocked(s, b->mIntent, TRUE);
        }
    }
    else if (!b->mIntent->mRequested) {
        RequestServiceBindingLocked(s, b->mIntent, FALSE);
    }
//     } finally {
    Binder::RestoreCallingIdentity(origId);
//     }

    *result = 1;
    return NOERROR;
}

ECode ActiveServices::PublishServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ IIntent* intent,
    /* [in] */ IBinder* service)
{
    const Int64 origId = Binder::ClearCallingIdentity();
    // try {
    if (DEBUG_SERVICE) Slogger::V(TAG, "PUBLISHING %p %p: %p", r
            , intent, service);
    if (r != NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&filter);
        AutoPtr<IntentBindRecord> b;
        HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IntentBindRecord> >::Iterator it =
                r->mBindings.Find(filter);
        if (it != r->mBindings.End()) {
            b = it->mSecond;
        }
        if (b != NULL && !b->mReceived) {
            b->mBinder = service;
            b->mRequested = TRUE;
            b->mReceived = TRUE;
            if (r->mConnections.IsEmpty() == FALSE) {
                HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator cit;
                for (cit = r->mConnections.Begin(); cit != r->mConnections.End(); ++cit) {
                    ConnectionRecordListIterator crit;
                    for (crit = cit->mSecond->Begin(); crit != cit->mSecond->End(); ++crit) {
                        AutoPtr<ConnectionRecord> c = *crit;
                        Boolean isEqual;
                        filter->Equals(c->mBinding->mIntent->mIntent, &isEqual);
                        if (!isEqual) {
                            if (DEBUG_SERVICE) Slogger::V(
                                    TAG, "Not publishing to: %p", c.Get());
                            if (DEBUG_SERVICE) Slogger::V(
                                    TAG, "Bound intent: %p", c->mBinding->mIntent->mIntent.Get());
                            if (DEBUG_SERVICE) Slogger::V(
                                    TAG, "Published intent: %p", intent);
                            continue;
                        }
                        if (DEBUG_SERVICE) Slogger::V(TAG, "Publishing to: %p", c.Get());
                        // try {
                        c->mConn->Connected(r->mName, service);
                        // } catch (Exception e) {
                        //     Slog.w(TAG, "Failure sending service " + r.name +
                        //           " to connection " + c.conn.asBinder() +
                        //           " (in " + c.binding.client.processName + ")", e);
                        // }
                    }
                }
            }
        }

        List< AutoPtr<CServiceRecord> >::Iterator sit =
                Find(mStoppingServices.Begin(), mStoppingServices.End(), AutoPtr<CServiceRecord>(r));
        ServiceDoneExecutingLocked(r, sit != mStoppingServices.End());
    }
    // } finally {
    Binder::RestoreCallingIdentity(origId);
    // }
    return NOERROR;
}

ECode ActiveServices::UnbindServiceLocked(
    /* [in] */ IIServiceConnection* connection,
    /* [out] */ Boolean* result)
{
    AutoPtr<IBinder> binder = IBinder::Probe(connection);
    if (DEBUG_SERVICE) Slogger::V(TAG, "unbindService: conn=%p", binder.Get());
    AutoPtr<ConnectionRecordList> clist;
    HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator it =
            mServiceConnections.Find(binder);
    if (it != mServiceConnections.End()) {
        clist = it->mSecond;
    }
    if (clist == NULL) {
        Slogger::W(TAG, "Unbind failed: could not find connection for %p", binder.Get());
        *result = FALSE;
        return NOERROR;
    }

    Int64 origId = Binder::ClearCallingIdentity();
//     try {
    while (clist->Begin() != clist->End()) {
        AutoPtr<ConnectionRecord> r = clist->GetFront();
        RemoveConnectionLocked(r, NULL, NULL);

        if (r->mBinding->mService->mApp != NULL) {
            // This could have made the service less important.
            mAm->UpdateOomAdjLocked(r->mBinding->mService->mApp);
        }
    }
//     } finally {
    Binder::RestoreCallingIdentity(origId);
//     }

    *result = TRUE;
    return NOERROR;
}

ECode ActiveServices::UnbindFinishedLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean doRebind)
{
    const Int64 origId = Binder::ClearCallingIdentity();
//     try {
    if (r != NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&filter);
        AutoPtr<IntentBindRecord> b = r->mBindings[filter];
        if (DEBUG_SERVICE) Slogger::V(TAG, "unbindFinished in %p at %p: apps=%d", r
                , b.Get(), (b != NULL ? b->mApps.GetSize() : 0));

        Boolean inStopping = FALSE;
        List< AutoPtr<CServiceRecord> >::Iterator it = Find(mStoppingServices.Begin(), mStoppingServices.End(), AutoPtr<CServiceRecord>(r));
        if (it != mStoppingServices.End()) inStopping = TRUE;
        if (b != NULL) {
            if (b->mApps.IsEmpty() == FALSE && !inStopping) {
               // Applications have already bound since the last
                // unbind, so just rebind right here.
                RequestServiceBindingLocked(r, b, TRUE);
            } else {
                // Note to tell the service the next time there is
                // a new client.
                b->mDoRebind = TRUE;
            }
        }

        ServiceDoneExecutingLocked(r, inStopping);
    }
//     } finally {
    Binder::RestoreCallingIdentity(origId);
//     }
    return NOERROR;
}

AutoPtr<CServiceRecord> ActiveServices::FindServiceLocked(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 userId)
{
    AutoPtr<CServiceRecord> r = mServiceMap->GetServiceByName(name, userId);
    return r == NULL ? NULL : (r->Probe(EIID_IBinder) == token ? r : NULL);
}

AutoPtr<ActiveServices::ServiceLookupResult> ActiveServices::RetrieveServiceLocked(
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean createIfNeeded)
{
    AutoPtr<CServiceRecord> r;
    if (DEBUG_SERVICE) Slogger::V(TAG, "retrieveServiceLocked: %p type=%s callingUid=%d", service
            , resolvedType.string(), callingUid);

    mAm->HandleIncomingUser(callingPid, callingUid, userId,
            FALSE, TRUE, String("service"), String(NULL), &userId);

    String serviceName;
    AutoPtr<IComponentName> component;
    service->GetComponent((IComponentName**)&component);
    if (component != NULL) {
        component->FlattenToString(&serviceName);
        r = mServiceMap->GetServiceByName(component, userId);
    }
    if (r == NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(service, (IIntentFilterComparison**)&filter);
        r = mServiceMap->GetServiceByIntent(filter, userId);
    }
    if (r == NULL) {
//         try {
        AutoPtr<IResolveInfo> rInfo;
        AppGlobals::GetPackageManager()->ResolveService(
            service, resolvedType, CActivityManagerService::STOCK_PM_FLAGS,
            userId, (IResolveInfo**)&rInfo);
        AutoPtr<IServiceInfo> sInfo;
        if (rInfo != NULL) {
            rInfo->GetServiceInfo((IServiceInfo**)&sInfo);
        }
        if (sInfo == NULL) {
            Slogger::W(TAG, "Unable to start service %s[%p] U=%d: not found", serviceName.string(), service, userId);
            return NULL;
        }
        AutoPtr<IApplicationInfo> appInfo;
        sInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        String spName, sname;
        appInfo->GetPackageName(&spName);
        sInfo->GetName(&sname);
        AutoPtr<IComponentName> name;
        CComponentName::New(spName, sname, (IComponentName**)&name);
        if (userId > 0) {
            String pName;
            sInfo->GetProcessName(&pName);
            AutoPtr<IApplicationInfo> aInfo;
            sInfo->GetApplicationInfo((IApplicationInfo**)&aInfo);
            String name;
            sInfo->GetName(&name);
            Int32 flags;
            sInfo->GetFlags(&flags);
            if (mAm->IsSingleton(pName, aInfo, name, flags)) {
                userId = 0;
           }
           AutoPtr<IServiceInfo> temp;
           CServiceInfo::New(sInfo, (IServiceInfo**)&temp);
           sInfo = temp;
           sInfo->SetApplicationInfo(mAm->GetAppInfoForUser(aInfo, userId));
        }
        r = mServiceMap->GetServiceByName(name, userId);
        if (r == NULL && createIfNeeded) {
            AutoPtr<IIntent> ci;
            service->CloneFilter((IIntent**)&ci);
            AutoPtr<IIntentFilterComparison> filter;
            CIntentFilterComparison::New(ci, (IIntentFilterComparison**)&filter);
            AutoPtr<ServiceRestarter> res = new ServiceRestarter(this);
            AutoPtr<BatteryStatsImpl::Uid::Pkg::Serv> ss;
            AutoPtr<BatteryStatsImpl> stats = mAm->mBatteryStatsService->GetActiveStatistics();
            {
                stats->Lock();
                AutoPtr<IApplicationInfo> appInfo;
                sInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                Int32 sInfoUid;
                appInfo->GetUid(&sInfoUid);
                String sInfoPkgName, sInfoName;
                sInfo->GetPackageName(&sInfoPkgName);
                sInfo->GetName(&sInfoName);
                ss = stats->GetServiceStatsLocked(
                        sInfoUid, sInfoPkgName, sInfoName);
                stats->Unlock();
            }
            CServiceRecord::NewByFriend((CServiceRecord**)&r);
            r->Init(mAm, ss, name, filter, sInfo, res);
            res->SetService(r);
            Int32 uid;
            r->mAppInfo->GetUid(&uid);
            mServiceMap->PutServiceByName(name, UserHandle::GetUserId(uid), r);
            mServiceMap->PutServiceByIntent(filter, UserHandle::GetUserId(uid), r);

            // Make sure this component isn't in the pending list.
            List< AutoPtr<CServiceRecord> >::Iterator it = mPendingServices.Begin();
            while (it != mPendingServices.End()) {
                AutoPtr<CServiceRecord> pr = *it;
                Boolean isEqual;
                AutoPtr<IApplicationInfo> appInfo;
                pr->mServiceInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                Int32 aUid;
                appInfo->GetUid(&aUid);
                AutoPtr<IApplicationInfo> sappInfo;
                sInfo->GetApplicationInfo((IApplicationInfo**)&sappInfo);
                Int32 sUid;
                sappInfo->GetUid(&sUid);
                if (aUid == sUid && pr->mName->Equals(name, &isEqual), isEqual) {
                    it = mPendingServices.Erase(it);
                }
                else ++it;
            }
        }
//         } catch (RemoteException ex) {
//             // pm is in same process, this will never happen.
//         }
    }
    if (r != NULL) {
        Int32 uid = 0;
        r->mAppInfo->GetUid(&uid);
        AutoPtr<ServiceLookupResult> result;
        if (mAm->CheckComponentPermission(r->mPermission,
                callingPid, callingUid, uid, r->mExported)
                != IPackageManager::PERMISSION_GRANTED) {
            if (!r->mExported) {
                Slogger::W(TAG, "Permission Denial: Accessing service %p from pid=%d, uid=%d that is not exported from uid %d",
                        r->mName.Get(), callingPid, callingUid, uid);
                StringBuilder sb("not exported from uid ");
                sb += uid;
                result = new ServiceLookupResult(NULL, sb.ToString());
                return result;
            }
            Slogger::W(TAG, "Permission Denial: Accessing service %p from pid=%d, uid=%d requires %s", r->mName.Get()
                    , callingPid, callingUid, r->mPermission.string());
            result = new ServiceLookupResult(NULL, r->mPermission);
            return result;
        }
        result = new ServiceLookupResult(r, String(NULL));
        return result;
    }
    return NULL;
}

void ActiveServices::BumpServiceExecutingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ const String& why)
{
    if (DEBUG_SERVICE)
        Logger::V(TAG, ">>> EXECUTING %s of %p in app %p", why.string(), r, r->mApp.Get());
    else if (DEBUG_SERVICE_EXECUTING)
        Logger::V(TAG, ">>> EXECUTING %s of %s", why.string(), r->mShortName.string());
    Int64 now = SystemClock::GetUptimeMillis();
    if (r->mExecuteNesting == 0 && r->mApp != NULL) {
        if (r->mApp->mExecutingServices.IsEmpty()) {
            AutoPtr<IMessage> msg;
            mAm->mHandler->ObtainMessage(CActivityManagerService::SERVICE_TIMEOUT_MSG, (IMessage**)&msg);
            msg->SetObj(r->mApp ? r->mApp->Probe(EIID_IInterface) : NULL);
            Boolean result;
            mAm->mHandler->SendMessageAtTime(msg, now + SERVICE_TIMEOUT, &result);
        }
        r->mApp->mExecutingServices.Insert(r);
    }
    r->mExecuteNesting++;
    r->mExecutingStart = now;
}

Boolean ActiveServices::RequestServiceBindingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ IntentBindRecord* i,
    /* [in] */ Boolean rebind)
{
    if (r->mApp == NULL || r->mApp->mThread == NULL) {
        // If service is not currently running, can't yet bind.
        return FALSE;
    }
    if ((!i->mRequested || rebind) && i->mApps.IsEmpty() == FALSE) {
        // try {
        BumpServiceExecutingLocked(r, String("bind"));
        AutoPtr<IIntent> intent;
        i->mIntent->GetIntent((IIntent**)&intent);
        ECode ec = r->mApp->mThread->ScheduleBindService(r, intent, rebind);
        if (SUCCEEDED(ec)) {
            if (!rebind) {
                i->mRequested = TRUE;
            }
            i->mHasBound = TRUE;
            i->mDoRebind = FALSE;
        }
        else {
            if (DEBUG_SERVICE) {
                Slogger::V(TAG, "Crashed while binding %p", r);
            }
            return FALSE;
        }
        // } catch (RemoteException e) {
        //     if (DEBUG_SERVICE) Slog.v(TAG, "Crashed while binding " + r);
        //     return false;
        // }
    }
    return TRUE;
}

Boolean ActiveServices::ScheduleServiceRestartLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean allowCancel)
{
    Boolean canceled = FALSE;

    const Int64 now = SystemClock::GetUptimeMillis();

    AutoPtr<IApplicationInfo> appInfo;
    r->mServiceInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 flags;
    appInfo->GetFlags(&flags);
    if ((flags &IApplicationInfo::FLAG_PERSISTENT) == 0) {
        Int64 minDuration = SERVICE_RESTART_DURATION;
        Int64 resetTime = SERVICE_RESET_RUN_DURATION;

        // Any delivered but not yet finished starts should be put back
        // on the pending list.
        if (!r->mDeliveredStarts.IsEmpty()) {
            List< AutoPtr<CServiceRecord::StartItem> >::ReverseIterator rit;
            for (rit = r->mDeliveredStarts.RBegin(); rit != r->mDeliveredStarts.REnd(); ++rit) {
                AutoPtr<CServiceRecord::StartItem> si = *rit;
                si->RemoveUriPermissionsLocked();
                if (si->mIntent == NULL) {
                    // We'll generate this again if needed.
                }
                else if (!allowCancel || (si->mDeliveryCount < CServiceRecord::MAX_DELIVERY_COUNT
                        && si->mDoneExecutingCount < CServiceRecord::MAX_DONE_EXECUTING_COUNT)) {
                    r->mPendingStarts.Insert(0, si);
                    Int64 dur = SystemClock::GetUptimeMillis() - si->mDeliveredTime;
                    dur *= 2;
                    if (minDuration < dur) minDuration = dur;
                    if (resetTime < dur) resetTime = dur;
                }
                else {
                    Slogger::W(TAG, "Canceling start item %p in service %p", si->mIntent.Get(), r->mName.Get());
                    canceled = TRUE;
                }
            }
            r->mDeliveredStarts.Clear();
        }

        r->mTotalRestartCount++;
        if (r->mRestartDelay == 0) {
            r->mRestartCount++;
            r->mRestartDelay = minDuration;
        }
        else {
            // If it has been a "reasonably long time" since the service
            // was started, then reset our restart duration back to
            // the beginning, so we don't infinitely increase the duration
            // on a service that just occasionally gets killed (which is
            // a normal case, due to process being killed to reclaim memory).
            if (now > (r->mRestartTime + resetTime)) {
                r->mRestartCount = 1;
                r->mRestartDelay = minDuration;
            }
            else {
                if ((flags & IApplicationInfo::FLAG_PERSISTENT) != 0) {
                    // Services in peristent processes will restart much more
                    // quickly, since they are pretty important.  (Think SystemUI).
                    r->mRestartDelay += minDuration / 2;
                }
                else {
                    r->mRestartDelay *= SERVICE_RESTART_DURATION_FACTOR;
                    if (r->mRestartDelay < minDuration) {
                        r->mRestartDelay = minDuration;
                    }
                }
            }
        }

        r->mNextRestartTime = now + r->mRestartDelay;

        // Make sure that we don't end up restarting a bunch of services
        // all at the same time.
        Boolean repeat;
        do {
            repeat = FALSE;
            List< AutoPtr<CServiceRecord> >::ReverseIterator rit;
            for (rit = mRestartingServices.RBegin(); rit != mRestartingServices.REnd(); ++rit) {
                AutoPtr<CServiceRecord> r2 = *rit;
                if (r2.Get() != r && r->mNextRestartTime
                        >= (r2->mNextRestartTime - SERVICE_MIN_RESTART_TIME_BETWEEN)
                        && r->mNextRestartTime
                        < (r2->mNextRestartTime + SERVICE_MIN_RESTART_TIME_BETWEEN)) {
                    r->mNextRestartTime = r2->mNextRestartTime + SERVICE_MIN_RESTART_TIME_BETWEEN;
                    r->mRestartDelay = r->mNextRestartTime - now;
                    repeat = TRUE;
                    break;
                }
            }
        } while (repeat);

    }
    else {
        // Persistent processes are immediately restrted, so there is no
        // reason to hold of on restarting their services.
        r->mTotalRestartCount++;
        r->mRestartCount = 0;
        r->mRestartDelay = 0;
        r->mNextRestartTime = now;
    }

    AutoPtr<CServiceRecord> sr = r;
    if (Find(mRestartingServices.Begin(), mRestartingServices.End(), sr)
            == mRestartingServices.End()) {
        mRestartingServices.PushBack(r);
    }

    mAm->mHandler->RemoveCallbacks(r->mRestarter);
    Boolean result;
    mAm->mHandler->PostAtTime(r->mRestarter, r->mNextRestartTime, &result);
    r->mNextRestartTime = SystemClock::GetUptimeMillis() + r->mRestartDelay;
    Slogger::W(TAG, "Scheduling restart of crashed service %s in %dms"
            , r->mShortName.string(), r->mRestartDelay);
//     EventLog.writeEvent(EventLogTags.AM_SCHEDULE_SERVICE_RESTART,
//             r.userId, r.shortName, r.restartDelay);

    return canceled;
}

ECode ActiveServices::PerformServiceRestartLocked(
    /* [in] */ CServiceRecord* r)
{
    if (Find(mRestartingServices.Begin(), mRestartingServices.End(), AutoPtr<CServiceRecord>(r))
            == mRestartingServices.End()) {
        return NOERROR;
    }
    AutoPtr<IIntent> intent;
    r->mIntent->GetIntent((IIntent**)&intent);
    Int32 flags;
    intent->GetFlags(&flags);
    BringUpServiceLocked(r, flags, TRUE);
    return NOERROR;
}

Boolean ActiveServices::UnscheduleServiceRestartLocked(
    /* [in] */ CServiceRecord* r)
{
    if (r->mRestartDelay == 0) {
        return FALSE;
    }
    r->ResetRestartCounter();
    mRestartingServices.Remove(r);
    mAm->mHandler->RemoveCallbacks(r->mRestarter);
    return TRUE;
}

String ActiveServices::BringUpServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Int32 intentFlags,
    /* [in] */ Boolean whileRestarting)
{
    //Slog.i(TAG, "Bring up service:");
    //r.dump("  ");

    if (r->mApp != NULL && r->mApp->mThread != NULL) {
        SendServiceArgsLocked(r, FALSE);
        return String(NULL);
    }

    if (!whileRestarting && r->mRestartDelay > 0) {
        // If waiting for a restart, then do nothing.
        return String(NULL);
    }

    if (DEBUG_SERVICE) Slogger::V(TAG, "Bringing up %p %p", r, r->mIntent.Get());

    // We are now bringing the service up, so no longer in the
    // restarting state.
    mRestartingServices.Remove(r);

    // Make sure that the user who owns this service is started.  If not,
    // we don't want to allow it to run.
    if (mAm->mStartedUsers[r->mUserId] == NULL) {
        StringBuilder msg("Unable to launch app ");
        String pName;
        r->mAppInfo->GetPackageName(&pName);
        msg += pName;
        msg += "/";
        Int32 uid;
        r->mAppInfo->GetUid(&uid);
        msg += uid;
        msg += " for service ";
        AutoPtr<IIntent> intent;
        r->mIntent->GetIntent((IIntent**)&intent);
        msg += intent;
        msg += ": user ";
        msg += r->mUserId;
        msg += " is stopped";
        Slogger::W(TAG, msg.ToString().string());
        BringDownServiceLocked(r, TRUE);
        return msg.ToString();
    }

    // Service is now being launched, its package can't be stopped.
//     try {
    ECode ec = AppGlobals::GetPackageManager()->SetPackageStoppedState(
            r->mPackageName, FALSE, r->mUserId);
    if (!SUCCEEDED(ec)) Slogger::W(TAG, "Failed trying to unstop package %s: %d"
            , r->mPackageName.string(), ec);
//     } catch (RemoteException e) {
//     } catch (IllegalArgumentException e) {
//         Slog.w(TAG, "Failed trying to unstop package "
//                 + r.packageName + ": " + e);
//     }

    Int32 siFlags;
    r->mServiceInfo->GetFlags(&siFlags);
    const Boolean isolated = (siFlags & IServiceInfo::FLAG_ISOLATED_PROCESS) != 0;
    const String procName = r->mProcessName;
    AutoPtr<ProcessRecord> app;

    if (!isolated) {
        Int32 rUid;
        r->mAppInfo->GetUid(&rUid);
        app = mAm->GetProcessRecordLocked(procName, rUid);
        if (DEBUG_MU) {
            Int32 uid;
            r->mAppInfo->GetUid(&uid);
            Slogger::V(TAG_MU, "bringUpServiceLocked: appInfo.uid=%d app=%p", uid, app.Get());
        }
        if (app != NULL && app->mThread != NULL) {
//             try {
            String packageName;
            r->mAppInfo->GetPackageName(&packageName);
            app->AddPackage(packageName);
            RealStartServiceLocked(r, app);
            return String(NULL);
//             } catch (RemoteException e) {
//                 Slog.w(TAG, "Exception when starting service " + r.shortName, e);
//             }

            // If a dead object exception was thrown -- fall through to
            // restart the application.
        }
    }
    else {
        // If this service runs in an isolated process, then each time
        // we call startProcessLocked() we will get a new isolated
        // process, starting another process if we are currently waiting
        // for a previous process to come up.  To deal with this, we store
        // in the service any current isolated process it is running in or
        // waiting to have come up.
        app = r->mIsolatedProc;
    }

    // Not running -- get it started, and enqueue this service record
    // to be executed when the app comes up.
    if (app == NULL) {
        if ((app = mAm->StartProcessLocked(procName, r->mAppInfo, TRUE, intentFlags,
                String("service"), r->mName, FALSE, isolated)) == NULL) {
            StringBuilder msg("Unable to launch app ");
            String pName;
            r->mAppInfo->GetPackageName(&pName);
            msg += pName;
            msg += "/";
            Int32 uid;
            r->mAppInfo->GetUid(&uid);
            msg += uid;
            msg += " for service ";
            AutoPtr<IIntent> intent;
            r->mIntent->GetIntent((IIntent**)&intent);
            msg += intent;
            msg += ": process is bad";
            Slogger::W(TAG, msg.ToString().string());
            BringDownServiceLocked(r, TRUE);
            return msg.ToString();
        }
        if (isolated) {
            r->mIsolatedProc = app;
        }
    }

    if (Find(mPendingServices.Begin(), mPendingServices.End(), AutoPtr<CServiceRecord>(r))
            == mPendingServices.End()) {
        mPendingServices.PushBack(r);
    }

    return String(NULL);
}

ECode ActiveServices::RequestServiceBindingsLocked(
    /* [in] */ CServiceRecord* r)
{
    HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IntentBindRecord> >::Iterator it;
    for (it = r->mBindings.Begin(); it != r->mBindings.End(); ++it) {
        AutoPtr<IntentBindRecord> i = it->mSecond;
        if (!RequestServiceBindingLocked(r, i, FALSE)) {
            break;
        }
    }
    return NOERROR;
}

ECode ActiveServices::RealStartServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ ProcessRecord* app)
{
    if (app->mThread == NULL) {
        // throw new RemoteException();
        return E_REMOTE_EXCEPTION;
    }
    if (DEBUG_MU) {
        Int32 uid;
        r->mAppInfo->GetUid(&uid);
        Slogger::V(TAG_MU, "realStartServiceLocked, CServiceRecord.uid = %d, ProcessRecord.uid = %d", uid
                , app->mUid);
    }
    r->mApp = app;
    r->mRestartTime = r->mLastActivity = SystemClock::GetUptimeMillis();

    app->mServices.Insert(r);
    BumpServiceExecutingLocked(r, String("create"));
    mAm->UpdateLruProcessLocked(app, TRUE);

    Boolean created = FALSE;
//     try {
//TODO:
//    EventLogTags.writeAmCreateService(
//                        r.userId, System.identityHashCode(r), r.shortName, r.app.pid);
    r->mStats->GetBatteryStats()->Lock();
    r->mStats->StartLaunchedLocked();
    r->mStats->GetBatteryStats()->Unlock();
    String pkgName;
    r->mServiceInfo->GetPackageName(&pkgName);
    mAm->EnsurePackageDexOpt(pkgName);
    AutoPtr<IApplicationInfo> appInfo;
    r->mServiceInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    app->mThread->ScheduleCreateService(r, r->mServiceInfo,
    mAm->CompatibilityInfoForPackageLocked(appInfo));
    r->PostNotification();
    created = TRUE;
//     } finally {
    if (!created) {
        app->mServices.Erase(r);
        ScheduleServiceRestartLocked(r, FALSE);
    }
//     }

    RequestServiceBindingsLocked(r);

    // If the service is in the started state, and there are no
    // pending arguments, then fake up one so its onStartCommand() will
    // be called.
    if (r->mStartRequested && r->mCallStart && r->mPendingStarts.IsEmpty()) {
        AutoPtr<CServiceRecord::StartItem> item = new CServiceRecord::StartItem(r, FALSE, r->MakeNextStartId(),
                NULL, NULL);
        r->mPendingStarts.PushBack(item);
    }

    SendServiceArgsLocked(r, TRUE);
    return NOERROR;
}

ECode ActiveServices::SendServiceArgsLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean oomAdjusted)
{
    const Int32 N = r->mPendingStarts.GetSize();
    if (N == 0) {
        return NOERROR;
    }

   while (r->mPendingStarts.IsEmpty() == FALSE) {
//         try {
        AutoPtr<CServiceRecord::StartItem> si = r->mPendingStarts[0];
        r->mPendingStarts.Remove(0);
        if (DEBUG_SERVICE) Slogger::V(TAG, "Sending arguments to: %p %p args=%p"
                , r, r->mIntent.Get(), si->mIntent.Get());
        if (si->mIntent == NULL && N > 1) {
            // If somehow we got a dummy null intent in the middle,
            // then skip it.  DO NOT skip a null intent when it is
            // the only one in the list -- this is to support the
            // onStartCommand(null) case.
            continue;
        }
        si->mDeliveredTime = SystemClock::GetUptimeMillis();
        r->mDeliveredStarts.PushBack(si);
        si->mDeliveryCount++;
        if (si->mNeededGrants != NULL) {
            mAm->GrantUriPermissionUncheckedFromIntentLocked(si->mNeededGrants,
                    si->GetUriPermissionsLocked());
        }
        BumpServiceExecutingLocked(r, String("start"));
        if (!oomAdjusted) {
            oomAdjusted = TRUE;
            mAm->UpdateOomAdjLocked(r->mApp);
        }
        Int32 flags = 0;
        if (si->mDeliveryCount > 1) {
            flags |= IService::START_FLAG_RETRY;
        }
        if (si->mDoneExecutingCount > 0) {
            flags |= IService::START_FLAG_REDELIVERY;
        }
        r->mApp->mThread->ScheduleServiceArgs(r, si->mTaskRemoved, si->mId, flags, si->mIntent);
//         } catch (RemoteException e) {
//             // Remote process gone...  we'll let the normal cleanup take
//             // care of this.
//             if (DEBUG_SERVICE) Slog.v(TAG, "Crashed while scheduling start: " + r);
//             break;
//         } catch (Exception e) {
//             Slog.w(TAG, "Unexpected exception", e);
//             break;
//        }
   }
   return NOERROR;
}

ECode ActiveServices::BringDownServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean force)
{
   //Slog.i(TAG, "Bring down service:");
   //r.dump("  ");

   // Does it still need to run?
   if (!force && r->mStartRequested) {
       return NOERROR;
   }
   HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator it;
   for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
       if (!force) {
           // XXX should probably keep a count of the number of auto-create
           // connections directly in the service.
           AutoPtr<ConnectionRecordList>  cr = it->mSecond;
           ConnectionRecordListIterator cit;
           for (cit = cr->Begin(); cit != cr->End(); ++ cit) {
               if (((*cit)->mFlags&IContext::BIND_AUTO_CREATE) != 0) {
                   return NOERROR;
               }
           }
       }

       // Report to all of the connections that the service is no longer
       // available.
       AutoPtr<ConnectionRecordList>  c = it->mSecond;
       ConnectionRecordListIterator cit;
       for (cit = c->Begin(); cit != c->End(); ++cit) {
            AutoPtr<ConnectionRecord> cr = *cit;
            // There is still a connection to the service that is
            // being brought down.  Mark it as dead.
            cr->mServiceDead = TRUE;
//                 try {
            ECode ec = cr->mConn->Connected(r->mName, NULL);
            if (FAILED(ec)) Slogger::W(TAG, "Failure disconnecting service %p to connection %p (in %s)%d"
                  , r->mName.Get(), cr->mConn.Get()
                  , cr->mBinding->mClient->mProcessName.string(), ec);
//                 } catch (Exception e) {
//                     Slog.w(TAG, "Failure disconnecting service " + r.name +
//                           " to connection " + c.get(i).conn.asBinder() +
//                           " (in " + c.get(i).binding.client.processName + ")", e);
//                 }
        }
    }

    // Tell the service that it has been unbound.
    if (r->mApp != NULL && r->mApp->mThread != NULL) {
        HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IntentBindRecord> >::Iterator it;
        for (it = r->mBindings.Begin(); it != r->mBindings.End(); ++it) {
            AutoPtr<IntentBindRecord> ibr = it->mSecond;
            if (DEBUG_SERVICE) Slogger::V(TAG, "Bringing down binding %p: hasBound=%d", ibr.Get()
                    , ibr->mHasBound);
            if (r->mApp != NULL && r->mApp->mThread != NULL && ibr->mHasBound) {
//                 try {
                    BumpServiceExecutingLocked(r, String("bring down unbind"));
                    mAm->UpdateOomAdjLocked(r->mApp);
                    ibr->mHasBound = FALSE;
                    AutoPtr<IIntent> intent;
                    ibr->mIntent->GetIntent((IIntent**)&intent);
                    r->mApp->mThread->ScheduleUnbindService(r, intent);
//                 } catch (Exception e) {
//                     Slog.w(TAG, "Exception when unbinding service "
//                             + r.shortName, e);
//                     serviceDoneExecutingLocked(r, true);
//                 }
            }
        }
    }

    if (DEBUG_SERVICE) Slogger::V(TAG, "Bringing down %p %p", r, r->mIntent.Get());
//    EventLogTags.writeAmDestroyService(
//                    r.userId, System.identityHashCode(r), (r.app != null) ? r.app.pid : -1);

    mServiceMap->RemoveServiceByName(r->mName, r->mUserId);
    mServiceMap->RemoveServiceByIntent(r->mIntent, r->mUserId);
    r->mTotalRestartCount = 0;
    UnscheduleServiceRestartLocked(r);

    // Also make sure it is not on the pending list.
    List< AutoPtr<CServiceRecord> >::Iterator pIt;
    for (pIt = mPendingServices.Begin(); pIt != mPendingServices.End();) {
        if ((*pIt).Get() == r) {
            pIt = mPendingServices.Erase(pIt);
            if (DEBUG_SERVICE) Slogger::V(TAG, "Removed pending: %p", r);
        }
        else {
          ++pIt;
        }
    }

    r->CancelNotification();
    r->mIsForeground = FALSE;
    r->mForegroundId = 0;
    r->mForegroundNoti = NULL;

    // Clear start entries.
    r->ClearDeliveredStartsLocked();
    r->mPendingStarts.Clear();

    if (r->mApp != NULL) {
        r->mStats->GetBatteryStats()->Lock();
        r->mStats->StopLaunchedLocked();
        r->mStats->GetBatteryStats()->Unlock();
        r->mApp->mServices.Erase(r);
        if (r->mApp->mThread != NULL) {
//             try {
                BumpServiceExecutingLocked(r, String("stop"));
                mStoppingServices.PushBack(r);
                mAm->UpdateOomAdjLocked(r->mApp);
                ECode ec = r->mApp->mThread->ScheduleStopService(r);
                if (FAILED(ec)) {
                    Slogger::W(TAG, "Exception when stopping service %s ecode: %d"
                          , r->mShortName.string(), ec);
                    ServiceDoneExecutingLocked(r, TRUE);
                }
//             } catch (Exception e) {
//                 Slog.w(TAG, "Exception when stopping service "
//                         + r.shortName, e);
//                 serviceDoneExecutingLocked(r, true);
//             }
            UpdateServiceForegroundLocked(r->mApp, FALSE);
        } else {
            if (DEBUG_SERVICE) Slogger::V(
                TAG, "Removed service that has no process: %p", r);
        }
    } else {
        if (DEBUG_SERVICE) Slogger::V(
            TAG, "Removed service that is not running: %p", r);
    }

    if (r->mBindings.IsEmpty() == FALSE) {
        r->mBindings.Clear();
    }

    if (r->mRestarter != NULL && r->mRestarter->Probe(EIID_ServiceRestarter) != NULL) {
        ServiceRestarter* sr = reinterpret_cast<ServiceRestarter*>(r->mRestarter->Probe(EIID_ServiceRestarter));
        sr->SetService(NULL);
    }
    return NOERROR;
}

ECode ActiveServices::RemoveConnectionLocked(
    /* [in] */ ConnectionRecord* c,
    /* [in] */ ProcessRecord* skipApp,
    /* [in] */ ActivityRecord* skipAct)
{
    AutoPtr<IBinder> binder = IBinder::Probe(c->mConn);
    AutoPtr<AppBindRecord> b = c->mBinding;
    AutoPtr<CServiceRecord> s = b->mService;
    AutoPtr<ConnectionRecordList>  clist;
    HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator it =
            s->mConnections.Find(binder);
    if (it != s->mConnections.End()) {
        clist = it->mSecond;
    }
    if (clist != NULL) {
        clist->Remove(c);
        if (clist->Begin() == clist->End()) {
            s->mConnections.Erase(it);
        }
    }
    b->mConnections.Erase(c);
    if (c->mActivity != NULL && c->mActivity.Get() != skipAct) {
        if (c->mActivity->mConnections != NULL) {
            c->mActivity->mConnections->Erase(c);
        }
    }
    if (b->mClient != skipApp) {
        b->mClient->mConnections.Erase(c);
        if ((c->mFlags&IContext::BIND_ABOVE_CLIENT) != 0) {
            b->mClient->UpdateHasAboveClientLocked();
        }
    }
    HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator cit =
            mServiceConnections.Find(binder);
    if (cit != mServiceConnections.End()) {
        clist = cit->mSecond;
    }
    if (clist != NULL) {
        clist->Remove(c);
        if (clist->Begin() == clist->End()) {
            mServiceConnections.Erase(cit);
        }
    }

    if (b->mConnections.Begin() == b->mConnections.End()) {
        b->mIntent->mApps.Erase(b->mClient);
    }

    if (!c->mServiceDead) {
        if (DEBUG_SERVICE) Slogger::V(TAG, "Disconnecting binding %p: shouldUnbind=%d", b->mIntent.Get()
                , b->mIntent->mHasBound);
        if (s->mApp != NULL && s->mApp->mThread != NULL && b->mIntent->mApps.IsEmpty()
                && b->mIntent->mHasBound) {
//             try {
            BumpServiceExecutingLocked(s, String("unbind"));
            mAm->UpdateOomAdjLocked(s->mApp);
            b->mIntent->mHasBound = FALSE;
            // Assume the client doesn't want to know about a rebind;
            // we will deal with that later if it asks for one.
            b->mIntent->mDoRebind = FALSE;
            AutoPtr<IIntent> bIntent;
            ECode ec = b->mIntent->mIntent->GetIntent((IIntent**)&bIntent);
            FAIL_GOTO(ec, __EXIT__)

            ec = s->mApp->mThread->ScheduleUnbindService(s, bIntent);
            FAIL_GOTO(ec, __EXIT__)

__EXIT__:
            if (FAILED(ec)) {
                Slogger::W(TAG, "Exception when unbinding service %s", s->mShortName.string());
                ServiceDoneExecutingLocked(s, TRUE);
            }
//             } catch (Exception e) {
//                 Slog.w(TAG, "Exception when unbinding service " + s.shortName, e);
//                 serviceDoneExecutingLocked(s, true);
//             }
        }

        if ((c->mFlags & IContext::BIND_AUTO_CREATE) != 0) {
            BringDownServiceLocked(s, FALSE);
        }
    }
    return NOERROR;
}

ECode ActiveServices::ServiceDoneExecutingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Int32 type,
    /* [in] */ Int32 startId,
    /* [in] */ Int32 res)
{
    Boolean inStopping = FALSE;
    List< AutoPtr<CServiceRecord> >::Iterator it = Find(mStoppingServices.Begin(), mStoppingServices.End(), AutoPtr<CServiceRecord>(r));
    if (it != mStoppingServices.End()) inStopping = TRUE;
    if (r != NULL) {
        if (type == 1) {
            // This is a call from a service start...  take care of
            // book-keeping.
            r->mCallStart = TRUE;
            switch (res) {
                case IService::START_STICKY_COMPATIBILITY:
                case IService::START_STICKY: {
                    // We are done with the associated start arguments.
                    r->FindDeliveredStart(startId, TRUE);
                    // Don't stop if killed.
                    r->mStopIfKilled = FALSE;
                    break;
                }
                case IService::START_NOT_STICKY: {
                    // We are done with the associated start arguments.
                    r->FindDeliveredStart(startId, TRUE);
                    if (r->GetLastStartId() == startId) {
                        // There is no more work, and this service
                        // doesn't want to hang around if killed.
                        r->mStopIfKilled = TRUE;
                    }
                    break;
                }
                case IService::START_REDELIVER_INTENT: {
                    // We'll keep this item until they explicitly
                    // call stop for it, but keep track of the fact
                    // that it was delivered.
                    AutoPtr<CServiceRecord::StartItem> si = r->FindDeliveredStart(startId, FALSE);
                    if (si != NULL) {
                        si->mDeliveryCount = 0;
                        si->mDoneExecutingCount++;
                        // Don't stop if killed.
                        r->mStopIfKilled = TRUE;
                    }
                    break;
                }
                case IService::START_TASK_REMOVED_COMPLETE: {
                    // Special processing for onTaskRemoved().  Don't
                    // impact normal onStartCommand() processing.
                    r->FindDeliveredStart(startId, TRUE);
                    break;
                }
                default:
//                     throw new IllegalArgumentException(
//                             "Unknown service start result: " + res);
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            if (res == IService::START_STICKY_COMPATIBILITY) {
                r->mCallStart = FALSE;
            }
        }
        const Int64 origId = Binder::ClearCallingIdentity();
        ServiceDoneExecutingLocked(r, inStopping);
        Binder::RestoreCallingIdentity(origId);
    }
    else {
        Slogger::W(TAG, "Done executing unknown service from pid %d"
                , Binder::GetCallingPid());
    }
    return NOERROR;
}

ECode ActiveServices::ServiceDoneExecutingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean inStopping)
{
    if (DEBUG_SERVICE) Slogger::V(TAG, "<<< DONE EXECUTING %p: nesting=%d, inStopping=%d, app=%p", r,
            r->mExecuteNesting, inStopping, r->mApp.Get());
    else if (DEBUG_SERVICE_EXECUTING) Slogger::V(TAG, "<<< DONE EXECUTING %s", r->mShortName.string());
    r->mExecuteNesting--;
    if (r->mExecuteNesting <= 0 && r->mApp != NULL) {
        if (DEBUG_SERVICE) Slogger::V(TAG, "Nesting at 0 of %s", r->mShortName.string());

        r->mApp->mExecutingServices.Erase(r);
        if (r->mApp->mExecutingServices.IsEmpty()) {
            if (DEBUG_SERVICE || DEBUG_SERVICE_EXECUTING) Slogger::V(TAG,
                    "No more executingServices of %s", r->mShortName.string());

            mAm->mHandler->RemoveMessages(
                CActivityManagerService::SERVICE_TIMEOUT_MSG,
                r->mApp->Probe(EIID_IInterface));
        }
        if (inStopping) {
            if (DEBUG_SERVICE)
                Slogger::V(TAG, "doneExecuting remove stopping %p", r);
            mStoppingServices.Remove(r);
            r->mBindings.Clear();
        }
        mAm->UpdateOomAdjLocked(r->mApp);
    }
    return NOERROR;
}

ECode ActiveServices::AttachApplicationLocked(
    /* [in] */ ProcessRecord* proc,
    /* [in] */ const String& processName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean didSomething = FALSE;
    // Collect any services that are waiting for this process to come up.
    if (!mPendingServices.IsEmpty()) {
        AutoPtr<CServiceRecord> sr;
//         try {
        List<AutoPtr<CServiceRecord> >::Iterator it = mPendingServices.Begin();
        while (it != mPendingServices.End()) {
            sr = *it;
            Int32 srUid;
            sr->mAppInfo->GetUid(&srUid);
            if (proc != sr->mIsolatedProc && (proc->mUid != srUid
                    || !processName.Equals(sr->mProcessName))) {
                ++it;
                continue;
            }

            it = mPendingServices.Erase(it);
            RealStartServiceLocked(sr, proc);
            didSomething = TRUE;
        }
//         } catch (Exception e) {
//             Slog.w(TAG, "Exception in new application when starting service "
//                     + sr.shortName, e);
//             throw e;
//         }
    }
    // Also, if there are any services that are waiting to restart and
    // would run in this process, now is a good time to start them.  It would
    // be weird to bring up the process but arbitrarily not let the services
    // run at this point just because their restart time hasn't come up.
    if (!mRestartingServices.IsEmpty()) {
        AutoPtr<CServiceRecord> sr;
        List< AutoPtr<CServiceRecord> >::Iterator it;
        for (it = mRestartingServices.Begin(); it != mRestartingServices.End(); ++it) {
            sr = *it;
            Int32 srUid;
            sr->mAppInfo->GetUid(&srUid);
            if (proc != sr->mIsolatedProc && (proc->mUid != srUid
                    || !processName.Equals(sr->mProcessName))) {
                continue;
            }

            mAm->mHandler->RemoveCallbacks(sr->mRestarter);
            Boolean result;
            mAm->mHandler->Post(sr->mRestarter, &result);
        }
    }
    *result = didSomething;
    return NOERROR;
}

ECode ActiveServices::ProcessStartTimedOutLocked(
    /* [in] */ ProcessRecord* proc)
{
    List< AutoPtr<CServiceRecord> >::Iterator it;
    for (it = mPendingServices.Begin(); it != mPendingServices.End();) {
        AutoPtr<CServiceRecord> sr = *it;
        Int32 appUid = 0;
        sr->mAppInfo->GetUid(&appUid);
        if ((proc->mUid == appUid
                && proc->mProcessName.Equals(sr->mProcessName))
                || sr->mIsolatedProc.Get() == proc) {
            Slogger::W(TAG, "Forcing bringing down service: %p", sr.Get());
            sr->mIsolatedProc = NULL;
            it = mPendingServices.Erase(it);
            BringDownServiceLocked(sr, TRUE);
        }
        else {
          ++it;
        }
    }
    return NOERROR;
}

Boolean ActiveServices::CollectForceStopServicesLocked(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean evenPersistent,
    /* [in] */ Boolean doit,
    /* [in] */ IComponentNameCServiceRecordHashMap* services,
    /* [in] */ List<AutoPtr<CServiceRecord> >* result)
{
    Boolean didSomething = FALSE;
    IComponentNameCServiceRecordHashMap::Iterator it;
    for (it = services->Begin(); it != services->End(); ++it) {
        AutoPtr<CServiceRecord> service = it->mSecond;
        if ((name == NULL || service->mPackageName.Equals(name))
                && (service->mApp == NULL || evenPersistent || !service->mApp->mPersistent)) {
            if (!doit) {
                return TRUE;
            }
            didSomething = TRUE;
            Slogger::I(TAG, "  Force stopping service %p", service.Get());
            if (service->mApp != NULL) {
                service->mApp->mRemoved = TRUE;
            }
            service->mApp = NULL;
            service->mIsolatedProc = NULL;
            result->PushBack(service);
        }
    }
    return didSomething;
}

ECode ActiveServices::ForceStopLocked(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean evenPersistent,
    /* [in] */ Boolean doit,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean didSomething = FALSE;
    List<AutoPtr<CServiceRecord> > services;
    if (userId == IUserHandle::USER_ALL) {
        HashMap<Int32, AutoPtr<IComponentNameCServiceRecordHashMap> >::Iterator it;
        for (it = mServiceMap->mServicesByNamePerUser.Begin(); it != mServiceMap->mServicesByNamePerUser.End(); ++it) {
            didSomething |= CollectForceStopServicesLocked(name, userId, evenPersistent,
                    doit, it->mSecond, &services);
            if (!doit && didSomething) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    else {
        HashMap<Int32, AutoPtr<IComponentNameCServiceRecordHashMap> >::Iterator it;
        it = mServiceMap->mServicesByNamePerUser.Find(userId);
        if (it != mServiceMap->mServicesByNamePerUser.End()) {
            AutoPtr<IComponentNameCServiceRecordHashMap> items = it->mSecond;
            didSomething = CollectForceStopServicesLocked(name, userId, evenPersistent,
                    doit, items, &services);
        }
    }

    List<AutoPtr<CServiceRecord> >::Iterator sit;
    for (sit = services.Begin(); sit != services.End(); ++sit) {
        BringDownServiceLocked(*sit, TRUE);
    }
    *result = didSomething;
    return NOERROR;
}

ECode ActiveServices::CleanUpRemovedTaskLocked(
    /* [in] */ TaskRecord* tr,
    /* [in] */ IComponentName* component,
    /* [in] */ IIntent* baseIntent)
{
    List<AutoPtr<CServiceRecord> > services;
    IComponentNameCServiceRecordHashMap::Iterator it;
    AutoPtr<IComponentNameCServiceRecordHashMap> sMap = mServiceMap->GetServices(tr->mUserId);
    for (it = sMap->Begin(); it != sMap->End(); ++it) {
        AutoPtr<CServiceRecord> sr = it->mSecond;
        String pkgName;
        component->GetPackageName(&pkgName);
        if (sr->mPackageName.Equals(pkgName)) {
            services.PushBack(sr);
        }
    }

    // Take care of any running services associated with the app.
    List<AutoPtr<CServiceRecord> >::Iterator sit;
    for (sit = services.Begin(); sit != services.End(); ++sit) {
        AutoPtr<CServiceRecord> sr = *sit;
        if (sr->mStartRequested) {
            Int32 flags;
            sr->mServiceInfo->GetFlags(&flags);
            if ((flags&IServiceInfo::FLAG_STOP_WITH_TASK) != 0) {
                Slogger::I(TAG, "Stopping service %s: remove task", sr->mShortName.string());
                StopServiceLocked(sr);
            } else {
                sr->mPendingStarts.PushBack(new CServiceRecord::StartItem(sr, TRUE,
                        sr->MakeNextStartId(), baseIntent, NULL));
                if (sr->mApp != NULL && sr->mApp->mThread != NULL) {
                    SendServiceArgsLocked(sr, FALSE);
                }
            }
        }
    }
    return NOERROR;
}

ECode ActiveServices::KillServicesLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean allowRestart)
{
    // Report disconnected services.
    if (FALSE) {
        // XXX we are letting the client link to the service for
        // death notifications.
        if (app->mServices.Begin() != app->mServices.End()) {
            HashSet< AutoPtr<CServiceRecord> >::Iterator it;
            for (it = app->mServices.Begin(); it != app->mServices.End(); ++it) {
                AutoPtr<CServiceRecord> r = *it;
                if (r->mConnections.Begin() != r->mConnections.End()) {
                    CServiceRecord::ConnectionHashMap::Iterator rit;
                    for(rit = r->mConnections.Begin(); rit != r->mConnections.End(); ++rit) {
                        AutoPtr<ConnectionRecordList> cl = rit->mSecond;
                        ConnectionRecordListIterator clit;
                        for (clit = cl->Begin(); clit != cl->End(); ++clit) {
                            AutoPtr<ConnectionRecord> c = *clit;
                            if (c->mBinding->mClient != app) {
                                // try {
                                //     // c.conn.connected(r.className, null);
                                // } catch (Exception e) {
                                //     // todo: this should be asynchronous!
                                //     Slog.w(TAG, "Exception thrown disconnected servce "
                                //           + r.shortName
                                //           + " from app " + app.processName, e);
                                // }
                            }
                        }
                    }
                }
            }
        }
    }

    // Clean up any connections this application has to other services.
    if (app->mConnections.Begin() != app->mConnections.End()) {
        HashSet< AutoPtr<ConnectionRecord> >::Iterator it;
        for (it = app->mConnections.Begin(); it != app->mConnections.End(); ++it) {
            AutoPtr<ConnectionRecord> r = *it;
            RemoveConnectionLocked(r, app, NULL);
        }
    }
    app->mConnections.Clear();

    if (app->mServices.Begin() != app->mServices.End()) {
        // Any services running in the application need to be placed
        // back in the pending list.
        HashSet< AutoPtr<CServiceRecord> >::Iterator it;
        for (it = app->mServices.Begin(); it != app->mServices.End(); ++it) {
            AutoPtr<CServiceRecord> sr = *it;
            sr->mStats->GetBatteryStats()->Lock();
            sr->mStats->StopLaunchedLocked();
            sr->mStats->GetBatteryStats()->Unlock();
            sr->mApp = NULL;
            sr->mIsolatedProc = NULL;
            sr->mExecuteNesting = 0;
            List< AutoPtr<CServiceRecord> >::Iterator fit = Find(mStoppingServices.Begin(), mStoppingServices.End(), sr);
            if (fit != mStoppingServices.End()) {
                mStoppingServices.Remove(sr);
                if (DEBUG_SERVICE) Slogger::V(TAG, "killServices remove stopping %p", sr.Get());
            }

            Boolean hasClients = sr->mBindings.Begin() != sr->mBindings.End();
            if (hasClients) {
                HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IntentBindRecord> >::Iterator it;
                for (it = sr->mBindings.Begin(); it != sr->mBindings.End(); ++it) {
                    AutoPtr<IntentBindRecord> b = it->mSecond;
                    if (DEBUG_SERVICE) Slogger::V(TAG, "Killing binding %p: shouldUnbind=%d", b.Get(), b->mHasBound);
                    b->mBinder = NULL;
                    b->mRequested = b->mReceived = b->mHasBound = FALSE;
                }
            }

            AutoPtr<IApplicationInfo> appInfo;
            sr->mServiceInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 flags;
            appInfo->GetFlags(&flags);
            if (sr->mCrashCount >= 2 && (flags&IApplicationInfo::FLAG_PERSISTENT) == 0) {
                Slogger::W(TAG, "Service crashed %d times, stopping: %p", sr->mCrashCount, sr.Get());
//                 EventLog.writeEvent(EventLogTags.AM_SERVICE_CRASHED_TOO_MUCH,
//                         sr.userId, sr.crashCount, sr.shortName, app.pid);
                BringDownServiceLocked(sr, TRUE);
            } else if (!allowRestart) {
                BringDownServiceLocked(sr, TRUE);
            } else {
                Boolean canceled = ScheduleServiceRestartLocked(sr, TRUE);

                // Should the service remain running?  Note that in the
                // extreme case of so many attempts to deliver a command
                // that it failed we also will stop it here.
                if (sr->mStartRequested && (sr->mStopIfKilled || canceled)) {
                    if (sr->mPendingStarts.Begin() == sr->mPendingStarts.End()) {
                        sr->mStartRequested = FALSE;
                        if (!hasClients) {
                            // Whoops, no reason to restart!
                            BringDownServiceLocked(sr, TRUE);
                        }
                    }
                }
            }
        }

        if (!allowRestart) {
            app->mServices.Clear();
        }
    }

    // Make sure we have no more records on the stopping list.
    List< AutoPtr<CServiceRecord> >::ReverseIterator sit = mStoppingServices.RBegin();
    while (sit != mStoppingServices.REnd()) {
        AutoPtr<CServiceRecord> sr = *sit;
        if (sr->mApp.Get() == app) {
            List< AutoPtr<CServiceRecord> >::ReverseIterator tmpRIt(sit);
            mStoppingServices.Erase((++tmpRIt).GetBase());
            if (DEBUG_SERVICE) Slogger::V(TAG, "killServices remove stopping %p", sr.Get());
        }
        else {
          ++sit;
        }
    }

    app->mExecutingServices.Clear();
    return NOERROR;
}

AutoPtr<IActivityManagerRunningServiceInfo> ActiveServices::MakeRunningServiceInfoLocked(
    /* [in] */ CServiceRecord* r)
{
    AutoPtr<IActivityManagerRunningServiceInfo> info;
    ASSERT_SUCCEEDED(CActivityManagerRunningServiceInfo::New((IActivityManagerRunningServiceInfo**)&info));
    info->SetService(r->mName);
    if (r->mApp != NULL) {
        info->SetPid(r->mApp->mPid);
    }
    Int32 aUid;
    r->mAppInfo->GetUid(&aUid);
    info->SetUid(aUid);
    info->SetProcess(r->mProcessName);
    info->SetForeground(r->mIsForeground);
    info->SetActiveSince(r->mCreateTime);
    info->SetStarted(r->mStartRequested);
    info->SetClientCount(r->mConnections.GetSize());
    info->SetCrashCount(r->mCrashCount);
    info->SetLastActivityTime(r->mLastActivity);
    Int32 flags;
    info->GetFlags(&flags);
    if (r->mIsForeground) {
        flags |= IActivityManagerRunningServiceInfo::FLAG_FOREGROUND;
    }
    if (r->mStartRequested) {
        flags |= IActivityManagerRunningServiceInfo::FLAG_STARTED;
    }
    if (r->mApp != NULL && r->mApp->mPid == CActivityManagerService::MY_PID) {
        flags |= IActivityManagerRunningServiceInfo::FLAG_SYSTEM_PROCESS;
    }
    if (r->mApp != NULL && r->mApp->mPersistent) {
        flags |= IActivityManagerRunningServiceInfo::FLAG_PERSISTENT_PROCESS;
    }
    info->SetFlags(flags);

    CServiceRecord::ConnectionHashMap::Iterator it;
    for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
        AutoPtr<ConnectionRecordList> connl = it->mSecond;
        ConnectionRecordListIterator cit;
        for (cit = connl->Begin(); cit != connl->End(); ++cit) {
            AutoPtr<ConnectionRecord> conn = *cit;
            if (conn->mClientLabel != 0) {
                String pkgName;
                conn->mBinding->mClient->mInfo->GetPackageName(&pkgName);
                info->SetClientPackage(pkgName);
                info->SetClientLabel(conn->mClientLabel);
                return info;
            }
        }
    }
    return info;
}

AutoPtr< List<AutoPtr<IActivityManagerRunningServiceInfo> > > ActiveServices::GetRunningServiceInfoLocked(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags)
{
    AutoPtr< List<AutoPtr<IActivityManagerRunningServiceInfo> > > res = new List<AutoPtr<IActivityManagerRunningServiceInfo> >();

    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();
//     try {
    AutoPtr<IActivityManagerHelper> amHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
    Int32 result;
    amHelper->CheckUidPermission(
                Elastos::Droid::Manifest::Permission::INTERACT_ACROSS_USERS_FULL,
                uid, &result);
    if (result == IPackageManager::PERMISSION_GRANTED) {
        AutoPtr< ArrayOf<Int32> > users = mAm->GetUsersLocked();
        for (Int32 ui = 0; ui < users->GetLength() && (Int32)res->GetSize() < maxNum; ui++) {
            IComponentNameCServiceRecordHashMap::Iterator it;
            AutoPtr<IComponentNameCServiceRecordHashMap> sMap = mServiceMap->GetServices((*users)[ui]);
            if (!sMap->IsEmpty()) {
                for (it = sMap->Begin(); it != sMap->End() && (Int32)res->GetSize() < maxNum; ++it) {
                    AutoPtr<CServiceRecord> r = it->mSecond;
                    res->PushBack(MakeRunningServiceInfoLocked(r));
                }
            }
        }

        List< AutoPtr<CServiceRecord> >::Iterator srit;
        for (srit = mRestartingServices.Begin(); srit != mRestartingServices.End() && (Int32)res->GetSize() < maxNum; ++srit) {
            AutoPtr<CServiceRecord> r = *srit;
            AutoPtr<IActivityManagerRunningServiceInfo> info =
                    MakeRunningServiceInfoLocked(r);
            info->SetRestarting(r->mNextRestartTime);
            res->PushBack(info);
        }
    } else {
        Int32 userId = UserHandle::GetUserId(uid);
        IComponentNameCServiceRecordHashMap::Iterator it;
        AutoPtr<IComponentNameCServiceRecordHashMap> sMap = mServiceMap->GetServices(userId);
        if (!sMap->IsEmpty()) {
            for (it = sMap->Begin(); it != sMap->End() && (Int32)res->GetSize() < maxNum; ++it) {
                AutoPtr<CServiceRecord> r = it->mSecond;
                res->PushBack(MakeRunningServiceInfoLocked(r));
            }
        }

        List< AutoPtr<CServiceRecord> >::Iterator srit;
        for (srit = mRestartingServices.Begin(); srit != mRestartingServices.End() && (Int32)res->GetSize() < maxNum; ++srit) {
            AutoPtr<CServiceRecord> r = *srit;
            if (r->mUserId == userId) {
                AutoPtr<IActivityManagerRunningServiceInfo> info =
                        MakeRunningServiceInfoLocked(r);
                info->SetRestarting(r->mNextRestartTime);
                res->PushBack(info);
            }
        }
    }
//     } finally {
    Binder::RestoreCallingIdentity(ident);
//     }

    return res;
}

ECode ActiveServices::GetRunningServiceControlPanelLocked(
    /* [in] */ IComponentName* name,
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    Int32 userId = UserHandle::GetUserId(Binder::GetCallingUid());
    AutoPtr<CServiceRecord> r = mServiceMap->GetServiceByName(name, userId);
    if (r != NULL) {
        CServiceRecord::ConnectionHashMap::Iterator it;
        for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
            AutoPtr<ConnectionRecordList> conn = it->mSecond;
            ConnectionRecordListIterator cit;
            for (cit = conn->Begin(); cit != conn->End(); ++cit) {
                if ((*cit)->mClientIntent != NULL) {
                    *intent = (*cit)->mClientIntent;
                    INTERFACE_ADDREF(*intent);
                    return NOERROR;
                }
            }
        }
    }
    *intent = NULL;
    return NOERROR;
}

ECode ActiveServices::ServiceTimeout(
    /* [in] */ ProcessRecord* proc)
{
    String anrMessage;

    {
        Mutex::Autolock lock(mLock);
        if (proc->mExecutingServices.IsEmpty() || proc->mThread == NULL) {
            return NOERROR;
        }
        Int64 maxTime = SystemClock::GetUptimeMillis() - SERVICE_TIMEOUT;
        HashSet< AutoPtr<CServiceRecord> >::Iterator it;
        AutoPtr<CServiceRecord> timeout;
        Int64 nextTime = 0;
        for (it = proc->mExecutingServices.Begin(); it != proc->mExecutingServices.End(); ++it) {
            AutoPtr<CServiceRecord> sr = *it;
            if (sr->mExecutingStart < maxTime) {
                timeout = sr;
                break;
            }
            if (sr->mExecutingStart > nextTime) {
                nextTime = sr->mExecutingStart;
            }
        }
        List< AutoPtr<ProcessRecord> >::Iterator pIt = Find(mAm->mLruProcesses.Begin()
                , mAm->mLruProcesses.End(), AutoPtr<ProcessRecord>(proc));
        if (timeout != NULL && pIt != mAm->mLruProcesses.End()) {
            Slogger::W(TAG, "Timeout executing service: %p", timeout.Get());
            anrMessage = String("Executing service ") + timeout->mShortName;
        }
        else {
            AutoPtr<IMessage> msg;
            mAm->mHandler->ObtainMessage(CActivityManagerService::SERVICE_TIMEOUT_MSG, (IMessage**)&msg);
            msg->SetObj(proc->Probe(EIID_IInterface));
            Boolean result;
            mAm->mHandler->SendMessageAtTime(msg, nextTime + SERVICE_TIMEOUT, &result);
        }
    }

    if (!anrMessage.IsNull()) {
        mAm->AppNotResponding(proc, NULL, NULL, FALSE, anrMessage);
    }
    return NOERROR;
}

/**
 * Prints a list of ServiceRecords (dumpsys activity services)
 */
Boolean ActiveServices::DumpServicesLocked(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Int32 opti,
    /* [in] */ Boolean dumpAll,
    /* [in] */ Boolean dumpClient,
    /* [in] */ const String& dumpPackage)
{
    Boolean needSep = FALSE;

    AutoPtr<CActivityManagerService::ItemMatcher> matcher = new CActivityManagerService::ItemMatcher();
    matcher->Build(args, opti);

    pw->PrintStringln(String("ACTIVITY MANAGER SERVICES (dumpsys activity services)"));
//     try {
        AutoPtr< ArrayOf<Int32> > users = mAm->GetUsersLocked();
        for (Int32 i = 0; i < users->GetLength(); ++i) {
            Int32 user = (*users)[i];
            AutoPtr<IComponentNameCServiceRecordHashMap> sMap = mServiceMap->GetServices(user);
            IComponentNameCServiceRecordHashMap::Iterator it;
            if (sMap->Begin() != sMap->End()) {
                Boolean printed = FALSE;
                Int64 nowReal = SystemClock::GetElapsedRealtime();
                needSep = FALSE;
                for (it = sMap->Begin(); it != sMap->End(); ++it) {
                    AutoPtr<CServiceRecord> r = it->mSecond;
                    if (!matcher->Match(r->Probe(EIID_IInterface), r->mName)) {
                        continue;
                    }
                    String pkgName;
                    r->mAppInfo->GetPackageName(&pkgName);
                    if (dumpPackage != NULL && !dumpPackage.Equals(pkgName)) {
                        continue;
                    }
                    if (!printed) {
                        if (user != 0) {
                            pw->Println();
                        }
                        StringBuilder sb("  User ");
                        sb += user;
                        sb += " active services:";
                        pw->PrintStringln(sb.ToString());
                        printed = TRUE;
                    }
                    if (needSep) {
                        pw->Println();
                    }
                    pw->PrintString(String("  * "));
                    pw->PrintObjectln(r->Probe(EIID_IInterface));
                    if (dumpAll) {
//                         r->Dump(pw, "    ");
                        needSep = TRUE;
                    } else {
                        pw->PrintString(String("    app="));
                        pw->PrintObjectln((IInterface*)r->mApp);
                        pw->PrintString(String("    created="));
                        TimeUtils::FormatDuration(r->mCreateTime, nowReal, pw);
                        pw->PrintString(String(" started="));
                        pw->PrintBoolean(r->mStartRequested);
                        pw->PrintString(String(" connections="));
                        pw->PrintInt32ln(r->mConnections.GetSize());
                        if (r->mConnections.Begin() != r->mConnections.End()) {
                            pw->PrintStringln(String("    Connections:"));
                            HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator it;
                            for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
                                AutoPtr<ConnectionRecordList> clist = it->mSecond;
                                ConnectionRecordListIterator cit;
                                for (cit = clist->Begin(); cit != clist->End(); ++cit) {
                                    AutoPtr<ConnectionRecord> conn = *cit;
                                    pw->PrintString(String("      "));
                                    AutoPtr<IIntent> intent;
                                    conn->mBinding->mIntent->mIntent->GetIntent((IIntent**)&intent);
                                    String intentString;
                                    intent->ToShortString(FALSE, FALSE, FALSE, FALSE, &intentString);
                                    pw->PrintString(intentString);
                                    pw->PrintString(String(" -> "));
                                    AutoPtr<ProcessRecord> proc = conn->mBinding->mClient;
                                    pw->PrintStringln(proc != NULL ? proc->ToShortString() : String("null"));
                                }
                            }
                        }
                    }
                    if (dumpClient && r->mApp != NULL && r->mApp->mThread != NULL) {
                        pw->PrintStringln(String("    Client:"));
                        IFlushable::Probe(pw)->Flush();
                        // try {
                        TransferPipe tp;
                            // try {
                        AutoPtr<IFileDescriptor> fileDes;
                        tp.GetWriteFd()->GetFileDescriptor((IFileDescriptor**)&fileDes);
                        r->mApp->mThread->DumpService(fileDes, r, args);
                        tp.SetBufferPrefix(String("      "));
                        // Short timeout, since blocking here can
                        // deadlock with the application.
                        tp.Go(fd, 2000);
                            // } finally {
                        tp.Kill();
                        //     }
                        // } catch (IOException e) {
                        //     pw.println("      Failure while dumping the service: " + e);
                        // } catch (RemoteException e) {
                        //     pw.println("      Got a RemoteException while dumping the service");
                        // }
                        needSep = TRUE;
                    }
                }
                needSep = printed;
            }
        }
    // } catch (Exception e) {
    //     Log.w(TAG, "Exception in dumpServicesLocked: " + e);
    // }

    if (mPendingServices.Begin() != mPendingServices.End()) {
        Boolean printed = FALSE;
        List< AutoPtr<CServiceRecord> >::Iterator it;
        for (it = mPendingServices.Begin(); it != mPendingServices.End(); ++it) {
            AutoPtr<CServiceRecord> r = *it;
            if (!matcher->Match(r->Probe(EIID_IInterface), r->mName)) {
                continue;
            }
            String pkgName;
            r->mAppInfo->GetPackageName(&pkgName);
            if (dumpPackage != NULL && !dumpPackage.Equals(pkgName)) {
                continue;
            }
            if (!printed) {
                if (needSep) pw->PrintStringln(String(" "));
                needSep = TRUE;
                pw->PrintStringln(String("  Pending services:"));
                printed = TRUE;
            }
            pw->PrintString(String("  * Pending "));
            pw->PrintObjectln(r->Probe(EIID_IInterface));
//TODO:
//            r->Dump(pw, String("    "));
        }
        needSep = TRUE;
    }

    if (!mRestartingServices.IsEmpty()) {
        Boolean printed = FALSE;
        List< AutoPtr<CServiceRecord> >::Iterator it;
        for (it = mRestartingServices.Begin(); it != mRestartingServices.End(); ++it) {
            AutoPtr<CServiceRecord> r = *it;
            if (!matcher->Match(r->Probe(EIID_IInterface), r->mName)) {
                continue;
            }
            String pkgName;
            r->mAppInfo->GetPackageName(&pkgName);
            if (dumpPackage != NULL && !dumpPackage.Equals(pkgName)) {
                continue;
            }
            if (!printed) {
                if (needSep) pw->PrintStringln(String(" "));
                needSep = TRUE;
                pw->PrintStringln(String("  Restarting services:"));
                printed = TRUE;
            }
            pw->PrintString(String("  * Restarting "));
            pw->PrintObjectln(r->Probe(EIID_IInterface));
//TODO:
//            r->Dump(pw, String("    "));
        }
        needSep = TRUE;
    }

    if (mStoppingServices.Begin() != mStoppingServices.End()) {
        Boolean printed = FALSE;
        List< AutoPtr<CServiceRecord> >::Iterator it;
        for (it = mStoppingServices.Begin(); it != mStoppingServices.End(); ++it) {
            AutoPtr<CServiceRecord> r = *it;
            if (!matcher->Match(r->Probe(EIID_IInterface), r->mName)) {
                continue;
            }
            String pkgName;
            r->mAppInfo->GetPackageName(&pkgName);
            if (dumpPackage != NULL && !dumpPackage.Equals(pkgName)) {
                continue;
            }
            if (!printed) {
                if (needSep) pw->PrintStringln(String(" "));
                needSep = TRUE;
                pw->PrintStringln(String("  Stopping services:"));
                printed = TRUE;
            }
            pw->PrintString(String("  * Stopping "));
            pw->PrintObjectln(r->Probe(EIID_IInterface));
//TODO:
//            r->Dump(pw, String("    "));
        }
        needSep = TRUE;
    }

    if (dumpAll) {
        if (!mServiceConnections.IsEmpty()) {
            Boolean printed = FALSE;
            HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> >::Iterator it;
            for (it = mServiceConnections.Begin(); it != mServiceConnections.End(); ++it) {
                AutoPtr<ConnectionRecordList> r = it->mSecond;
                ConnectionRecordListIterator rit;
                for (rit = r->Begin(); rit != r->End(); ++rit) {
                    AutoPtr<ConnectionRecord> cr = *rit;
                    if (!matcher->Match(cr->mBinding->mService->Probe(EIID_IInterface), cr->mBinding->mService->mName)) {
                        continue;
                    }
                    String pName;
                    cr->mBinding->mClient->mInfo->GetPackageName(&pName);
                    if (dumpPackage != NULL && (cr->mBinding->mClient == NULL
                            || !dumpPackage.Equals(pName))) {
                        continue;
                    }
                    if (!printed) {
                        if (needSep) pw->PrintStringln(String(" "));
                        needSep = TRUE;
                        pw->PrintStringln(String("  Connection bindings to services:"));
                        printed = TRUE;
                    }
                    pw->PrintString(String("  * "));
//TODO:
//                    pw->PrintObjectln(cr);
//                    cr->Dump(pw, String("    "));
                }
            }
            needSep = TRUE;
        }
    }

    return needSep;
}

Boolean ActiveServices::DumpService(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Int32 opti,
    /* [in] */ Boolean dumpAll)
{
    List<AutoPtr<CServiceRecord> > services;

    {
        Mutex::Autolock lock(mLock);
        AutoPtr< ArrayOf<Int32> > users = mAm->GetUsersLocked();
        if (String("all").Equals(name)) {
            for (Int32 i = 0; i < users->GetLength(); ++i) {
                Int32 user = (*users)[i];
                AutoPtr<IComponentNameCServiceRecordHashMap> sMap = mServiceMap->GetServices(user);
                IComponentNameCServiceRecordHashMap::Iterator it;
                for (it = sMap->Begin(); it != sMap->End(); ++it) {
                    AutoPtr<CServiceRecord> r1 = it->mSecond;
                    services.PushBack(r1);
                }
            }
        } else {
            AutoPtr<IComponentName> componentName = NULL;
            AutoPtr<IComponentNameHelper> cnHelper;
            CComponentNameHelper::AcquireSingleton((IComponentNameHelper**)&cnHelper);
            if (name != String(NULL)) cnHelper->UnflattenFromString(name, (IComponentName**)&componentName);
            Int32 objectId = 0;
            String lname = name;
            if (componentName == NULL) {
                // Not a '/' separated full component name; maybe an object ID?
                // try {
                    objectId = StringUtils::ParseInt32(name, 16);
                    lname = String(NULL);
                    componentName = NULL;
                // } catch (RuntimeException e) {
                // }
            }

            for (Int32 i = 0; i < users->GetLength(); ++i) {
                Int32 user = (*users)[i];
                AutoPtr<IComponentNameCServiceRecordHashMap> sMap = mServiceMap->GetServices(user);
                IComponentNameCServiceRecordHashMap::Iterator it;
                for (it = sMap->Begin(); it != sMap->End(); ++it) {
                    AutoPtr<CServiceRecord> r1 = it->mSecond;
                    if (componentName != NULL) {
                        Boolean equal = FALSE;
                        r1->mName->Equals(componentName, &equal);
                        if (equal) {
                            services.PushBack(r1);
                        }
                    } else if (lname != NULL) {
                        String nString;
                        r1->mName->FlattenToString(&nString);
                        if (nString.Contains(lname)) {
                            services.PushBack(r1);
                        }
                    } else if ((Int32)r1.Get() == objectId) {
                       services.PushBack(r1);
                    }
                }
            }
        }
    }

    if (services.Begin() == services.End()) {
        return FALSE;
    }

    Boolean needSep = FALSE;
    List<AutoPtr<CServiceRecord> >::Iterator it;
    for (it = services.Begin(); it != services.End(); ++it) {
        if (needSep) {
            pw->Println();
        }
        needSep = TRUE;
        DumpService(String(""), fd, pw, *it, args, dumpAll);
    }
    return TRUE;
}

void ActiveServices::DumpService(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ CServiceRecord* r,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Boolean dumpAll)
{
    String innerPrefix = prefix + "  ";
    {
        Mutex::Autolock lock(mLock);
        pw->PrintString(prefix);
        pw->PrintString(String("SERVICE "));
        pw->PrintString(r->mShortName);
        pw->PrintString(String(" "));
        pw->PrintInt32((Int32)r);
        pw->PrintString(String(" pid="));
        if (r->mApp != NULL) pw->PrintInt32ln(r->mApp->mPid);
        else pw->PrintStringln(String("(not running)"));
        if (dumpAll) {
//TODO:
//            r->Dump(pw, innerPrefix);
        }
    }
    if (r->mApp != NULL && r->mApp->mThread != NULL) {
        pw->PrintString(prefix); pw->PrintStringln(String("  Client:"));
        IFlushable::Probe(pw)->Flush();
//         try {
            TransferPipe tp;
//             try {
            AutoPtr<IFileDescriptor> fileDes;
            tp.GetWriteFd()->GetFileDescriptor((IFileDescriptor**)&fileDes);
            r->mApp->mThread->DumpService(fileDes, r, args);
            tp.SetBufferPrefix(prefix + "    ");
            tp.Go(fd);
//             } finally {
            tp.Kill();
//             }
//         } catch (IOException e) {
//             pw.println(prefix + "    Failure while dumping the service: " + e);
//         } catch (RemoteException e) {
//             pw.println(prefix + "    Got a RemoteException while dumping the service");
//         }
    }
}

}
}
}
}
