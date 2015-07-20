#include "ServiceWatcher.h"
#include "os/Handler.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Utility::Logging::Slogger;

namespace Elastos{
namespace Droid{
namespace Server{
namespace Location{

const String ServiceWatcher::EXTRA_SERVICE_VERSION("serviceVersion");

CAR_INTERFACE_IMPL(ServiceWatcher, IServiceConnection)

ServiceWatcher::ServiceWatcherPackageMonitor::ServiceWatcherPackageMonitor(
    /* [in] */ ServiceWatcher* host) : mHost(host)
{}

ECode ServiceWatcher::ServiceWatcherPackageMonitor::OnPackageUpdateFinished(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    AutoLock lock(mHost->mLock);
    if(packageName.Equals(mHost->mPackageName)) {
        // package updated, make sure to rebind
        mHost->UnbindLocked();
    }

    // check the updated package in case it is better
    mHost->BindBestPackageLocked(packageName);
    return NOERROR;
}

ECode ServiceWatcher::ServiceWatcherPackageMonitor::OnPackageAdded(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    AutoLock lock(mHost->mLock);

    if (packageName.Equals(mHost->mPackageName)) {
        // package updated, make sure to rebind
        mHost->UnbindLocked();
    }
    // check the new package is case it is better
    mHost->BindBestPackageLocked(packageName);

    return NOERROR;
}

ECode ServiceWatcher::ServiceWatcherPackageMonitor::OnPackageRemoved(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    AutoLock lock(mHost->mLock);

    if (packageName.Equals(mHost->mPackageName)) {
        mHost->UnbindLocked();
        // the currently bound package was removed,
        // need to search for a new package
        mHost->BindBestPackageLocked(String(NULL));
    }
    return NOERROR;
}

List<HashSet<AutoPtr<ISignature> > > ServiceWatcher::GetSignatureSets(
    /* [in] */ IContext* context,
    /* [in] */ List<String>* initialPackageNames) {
    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    List<HashSet<AutoPtr<ISignature> > > sigSets;

    List<String>::Iterator it = initialPackageNames->Begin();
    for (; it != initialPackageNames->End(); it++) {
        String pkg = *it;
        HashSet<AutoPtr<ISignature> > set;
        AutoPtr<IPackageInfo> packageInfo;
        pm->GetPackageInfo(pkg, IPackageManager::GET_SIGNATURES, (IPackageInfo**)&packageInfo);
        AutoPtr<ArrayOf<ISignature*> > sigs;
        packageInfo->GetSignatures((ArrayOf<ISignature*>**)&sigs);
        for(Int32 i = 0; i < sigs->GetLength(); i++)
        {
            set.Insert((*sigs)[i]);
        }
        sigSets.PushBack(set);
    }
    return sigSets;
}


ServiceWatcher::ServiceWatcher(
    /* [in] */ IContext* context,
    /* [in] */ const String& logTag,
    /* [in] */ const String& action,
    /* [in] */ List<String>* initialPackageNames,
    /* [in] */ IRunnable* newServiceWork,
    /* [in] */ IHandler* handler,
    /* [in] */ Int32 userId)
    : mTag(logTag)
    , mVersion(0)
    , mCurrentUserId(0)
{
    mContext = context;
    mAction = action;
    mContext->GetPackageManager((IPackageManager**)&mPm);
    mNewServiceWork = newServiceWork;
    mHandler = handler;
    mCurrentUserId = userId;
    mPackageMonitor = new ServiceWatcherPackageMonitor(this);
    // TODO: temporary ignore
    // mSignatureSets = GetSignatureSets(context, initialPackageNames);
}

Boolean ServiceWatcher::Start()
{
    {
        AutoLock lock(mLock);
        if (!BindBestPackageLocked(String(NULL))) return FALSE;
    }

    mPackageMonitor->Register(mContext, NULL, UserHandle::ALL, TRUE);
    return TRUE;
}

Boolean ServiceWatcher::BindBestPackageLocked(
    /* [in] */ const String& justCheckThisPackage)
{
    AutoPtr<IIntent> intent;
    CIntent::New(mAction, (IIntent**)&intent);
    if (!justCheckThisPackage.IsNull())
    {
        intent->SetPackage(justCheckThisPackage);
    }

    AutoPtr<IIntent> paraIntent;
    CIntent::New(mAction, (IIntent**)&paraIntent);
    AutoPtr<IObjectContainer> rInfos;
    mPm->QueryIntentServicesAsUser(paraIntent, IPackageManager::GET_META_DATA, mCurrentUserId, (IObjectContainer**)&rInfos);
    Int32 bestVersion = Math::INT32_MIN_VALUE;
    String bestPackage = String(NULL);
    AutoPtr<IObjectEnumerator> enu;
    rInfos->GetObjectEnumerator((IObjectEnumerator**)&enu);

    Boolean hasNext;
    while(enu->MoveNext(&hasNext), hasNext)
    {
        AutoPtr<IInterface> infoTemp;
        enu->Current((IInterface**)&infoTemp);
        AutoPtr<IResolveInfo> rInfo = IResolveInfo::Probe(infoTemp);
        AutoPtr<IServiceInfo> sInfo;
        rInfo->GetServiceInfo((IServiceInfo**)&sInfo);
        String packageName;
        sInfo->GetPackageName(&packageName);


        AutoPtr<IPackageInfo> pInfo;
        ECode ec = mPm->GetPackageInfo(packageName, IPackageManager::GET_SIGNATURES, (IPackageInfo**)&pInfo);
        AutoPtr<ArrayOf<ISignature*> > sigsTemp;

        pInfo->GetSignatures((ArrayOf<ISignature*>**)&sigsTemp);
        if (!IsSignatureMatch(sigsTemp) || ec != NOERROR) {
//            Log.w(mTag, packageName + " resolves service " + mAction +
//                  ", but has wrong signature, ignoring");
            Slogger::W(mTag, "%s resolves service , but has wrong signature, ignoring", packageName.string());
            continue;
        }



        Int32 version = 0;
        AutoPtr<IBundle> metaData;
        sInfo->GetMetaData((IBundle**)&metaData);
        if (metaData != NULL) {
            metaData->GetInt32(EXTRA_SERVICE_VERSION, 0, &version);
        }

        if (version > mVersion) {
            bestVersion = version;
            bestPackage = packageName;
        }
    }

//    if (D) Log.d(mTag, String.format("bindBestPackage for %s : %s found %d, %s", mAction,
//            (justCheckThisPackage == null ? "" : "(" + justCheckThisPackage + ") "),
//            rInfos.size(),
//            (bestPackage == null ? "no new best package" : "new best packge: " + bestPackage)));

    if (!bestPackage.IsNull()) {
        BindToPackageLocked(bestPackage, bestVersion);
        return TRUE;
    }
    return FALSE;
}

void ServiceWatcher::UnbindLocked()
{
    String pkg;
    pkg = mPackageName;
    mPackageName = String(NULL);
    mVersion = Math::INT32_MIN_VALUE;
    if (!pkg.IsNull()) {
//        if (D) Log.d(mTag, "unbinding " + pkg);
        mContext->UnbindService(this);
    }
}

void ServiceWatcher::BindToPackageLocked(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 version)
{
    UnbindLocked();
    AutoPtr<IIntent> intent;
    CIntent::New(mAction, (IIntent**)&intent);
    intent->SetPackage(packageName);
    mPackageName = packageName;
    mVersion = version;
//    if (D) Log.d(mTag, "binding " + packageName + " (version " + version + ")");
    Boolean result;
    mContext->BindService(intent, this, IContext::BIND_AUTO_CREATE | IContext::BIND_NOT_FOREGROUND
            | IContext::BIND_NOT_VISIBLE, mCurrentUserId, &result);
}

Boolean ServiceWatcher::IsSignatureMatch(
    /* [in] */ ArrayOf<ISignature*>* signatures,
    /* [in] */ List<HashSet<AutoPtr<ISignature> > > sigSets)
{
    if (signatures == NULL) return FALSE;

    // build hashset of input to test against
    HashSet<AutoPtr<ISignature> > inputSet;

    for (Int32 i = 0; i < signatures->GetLength(); i++)
    {
        inputSet.Insert((*signatures)[i]);
    }

    // test input against each of the signature sets
    List<HashSet<AutoPtr<ISignature> > >::Iterator it = sigSets.Begin();
    for (; it != sigSets.End(); it++)
    {
        Boolean hasFind = FALSE;
        HashSet<AutoPtr<ISignature> > set = *it;
        HashSet<AutoPtr<ISignature> >::Iterator outIterator = set.Begin();
        for(; outIterator != set.End(); ++outIterator)
        {
            hasFind= FALSE;
            HashSet<AutoPtr<ISignature> >::Iterator innerIterator = inputSet.Begin();
            for(; innerIterator != inputSet.End(); ++innerIterator)
            {
                Boolean equals;
                (*outIterator)->Equals(*innerIterator, &equals);
                if(equals) {
                    hasFind = TRUE;
                    break;
                }
            }
            if(!hasFind) break;
        }
        if(hasFind) return TRUE;
    }
    return FALSE;
}

Boolean ServiceWatcher::IsSignatureMatch(
    /* [in] */ ArrayOf<ISignature*>* signatures)
{
    // TODO: temporary ignore
    // return IsSignatureMatch(signatures, mSignatureSets);
    return TRUE;
}

ECode ServiceWatcher::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* binder)
{
    AutoLock lock(mLock);
    String packageName;
    name->GetPackageName(&packageName);
    if (packageName.Equals(mPackageName)) {
        if (D) Slogger::D(mTag, "%s connected", packageName.string());
        mBinder = binder;
        if (mHandler != NULL && mNewServiceWork != NULL) {
            Boolean rst;
            mHandler->Post( mNewServiceWork, &rst);
        }
    }
    else {
        Slogger::W(mTag, "unexpected onServiceConnected: %s", packageName.string());
    }
    return NOERROR;
}

ECode ServiceWatcher::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    AutoLock lock(mLock);
    String packageName;
    name->GetPackageName(&packageName);
    if (D) Slogger::D(mTag, "%s disconnected", packageName.string());

    if (packageName.Equals(mPackageName)) {
        mBinder = NULL;
    }
    return NOERROR;
}

String ServiceWatcher::GetBestPackageName()
{
    AutoLock lock(mLock);
    return mPackageName;
}

Int32 ServiceWatcher::GetBestVersion()
{
    AutoLock lock(mLock);
    return mVersion;
}

AutoPtr<IBinder> ServiceWatcher::GetBinder()
{
    AutoLock lock(mLock);
    return mBinder;
}

void ServiceWatcher::SwitchUser(
    /* [in] */ Int32 userId)
{
    AutoLock lock(mLock);
    UnbindLocked();
    mCurrentUserId = userId;
    BindBestPackageLocked(String(NULL));
}

} // namespace Location
} // namespace Server
} // namespace Droid
} // namespace Elastos
