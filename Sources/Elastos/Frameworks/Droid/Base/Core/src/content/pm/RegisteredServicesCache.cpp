
#include "content/pm/RegisteredServicesCache.h"
#ifdef DROID_CORE
#include "content/CComponentName.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "os/CHandler.h"
#include "os/CEnvironment.h"
#include "util/CAtomicFile.h"
#endif
#include "util/Xml.h"
#include "os/UserHandle.h"
#include "os/Handler.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::StringBuilder;
using Elastos::Core::CInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Org::Xmlpull::V1::IXmlSerializer;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::IAtomicFile;
using Elastos::Droid::Utility::CAtomicFile;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

RegisteredServicesCache::UserServices::UserServices()
{}

RegisteredServicesCache::UserServices::~UserServices()
{
}

RegisteredServicesCache::PackageReceiver::PackageReceiver(
    /* [in] */ RegisteredServicesCache* parent)
    : mParent(parent)
{}

ECode RegisteredServicesCache::PackageReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Int32 uid;
    intent->GetInt32Extra(IIntent::EXTRA_UID, -1, &uid);
    if (uid != -1) {
        mParent->GenerateServicesMap(UserHandle::GetUserId(uid));
    }

    return NOERROR;
}


RegisteredServicesCache::ExternalReceiver::ExternalReceiver(
    /* [in] */ RegisteredServicesCache* parent)
    : mParent(parent)
{}

ECode RegisteredServicesCache::ExternalReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // External apps can't coexist with multi-user, so scan owner
    mParent->GenerateServicesMap(IUserHandle::USER_OWNER);
    return NOERROR;
}


RegisteredServicesCache::ListenerRunnable::ListenerRunnable(
    /* [in] */ IInterface* type,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean removed,
    /* [in] */ IRegisteredServicesCacheListener* listener)
    : mType(type)
    , mUserId(userId)
    , mRemoved(removed)
    , mListener(listener)
{}

CAR_INTERFACE_IMPL(RegisteredServicesCache::ListenerRunnable, IRunnable);

ECode RegisteredServicesCache::ListenerRunnable::Run()
{
    return mListener->OnServiceChanged(mType, mUserId, mRemoved);
}


RegisteredServicesCache::ServiceInfo::ServiceInfo(
    /* [in] */ IInterface* type,
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 uid)
    : mType(type)
    , mComponentName(componentName)
    , mUid(uid)
{}

String RegisteredServicesCache::ServiceInfo::ToString()
{
    String s;
    mComponentName->ToString(&s);
    return String("ServiceInfo: ")/* + mType*/ + String(", ") + s + String(", uid ") + StringUtils::Int32ToString(mUid);
}


const String RegisteredServicesCache::TAG("PackageManager");

RegisteredServicesCache::RegisteredServicesCache(
    /* [in] */ IContext* context,
    /* [in] */ const String& interfaceName,
    /* [in] */ const String& metaDataName,
    /* [in] */ const String& attributeName,
    /* [in] */ IXmlSerializerAndParser* serializerAndParser)
    : mContext(context)
    , mInterfaceName(interfaceName)
    , mMetaDataName(metaDataName)
    , mAttributesName(attributeName)
    , mSerializerAndParser(serializerAndParser)
    , mPersistentServicesFileDidNotExist(FALSE)
{
    mPackageReceiver = (IBroadcastReceiver*)new RegisteredServicesCache::PackageReceiver(this);
    mPackageReceiver = (IBroadcastReceiver*)new RegisteredServicesCache::ExternalReceiver(this);

    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dataDir;
    env->GetDataDirectory((IFile**)&dataDir);
    AutoPtr<IFile> systemDir, syncDir, interFile;
    CFile::New(dataDir, String("system"), (IFile**)&systemDir);
    CFile::New(systemDir, String("registered_services"), (IFile**)&syncDir);
    CFile::New(syncDir, interfaceName + String(".xml"), (IFile**)&interFile);
    CAtomicFile::New(interFile, (IAtomicFile**)&mPersistentServicesFile);

    // Load persisted services from disk
    ReadPersistentServicesLocked();

    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New((IIntentFilter**)&intentFilter);
    intentFilter->AddAction(IIntent::ACTION_PACKAGE_ADDED);
    intentFilter->AddAction(IIntent::ACTION_PACKAGE_CHANGED);
    intentFilter->AddAction(IIntent::ACTION_PACKAGE_REMOVED);
    intentFilter->AddDataScheme(String("package"));
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiverAsUser(mPackageReceiver, UserHandle::ALL, intentFilter, String(NULL), NULL, (IIntent**)&intent);

    // Register for events related to sdcard installation.
    AutoPtr<IIntentFilter> sdFilter;
    CIntentFilter::New((IIntentFilter**)&sdFilter);
    sdFilter->AddAction(IIntent::ACTION_EXTERNAL_APPLICATIONS_AVAILABLE);
    sdFilter->AddAction(IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE);
    intent = NULL;
    mContext->RegisterReceiver(mExternalReceiver, sdFilter, (IIntent**)&intent);
}

AutoPtr<RegisteredServicesCache::UserServices> RegisteredServicesCache::FindOrCreateUserLocked(
    /* [in] */ Int32 userId)
{
    AutoPtr<UserServices> services;
    HashMap<Int32, AutoPtr<UserServices> >::Iterator it = mUserServices.Find(userId);
    if (it == mUserServices.End()) {
        services = new UserServices();
        mUserServices[userId] = services;
    } else {
        services = it->mSecond;
    }
    return services;
}

void RegisteredServicesCache::InvalidateCache(
    /* [in] */ Int32 userId)
{
    Mutex::Autolock lock(mServicesLock);

    AutoPtr<UserServices> user = FindOrCreateUserLocked(userId);
    if (user->mServices != NULL) {
        user->mServices = NULL;
    }
}

void RegisteredServicesCache::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* fout,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Int32 userId)
{
    Mutex::Autolock lock(mServicesLock);

    AutoPtr<UserServices> user = FindOrCreateUserLocked(userId);
    if (user->mServices != NULL) {
        fout->PrintStringln(String("RegisteredServicesCache: ") +
            StringUtils::Int32ToString(user->mServices->GetSize()) + String(" services"));

        HashMap<AutoPtr<IInterface>, AutoPtr<ServiceInfo> >::Iterator itr =
                user->mServices->Begin();
        for (; itr != user->mServices->End(); ++itr) {
            AutoPtr<ServiceInfo> info = itr->mSecond;
            fout->PrintStringln(String("  ") + info->ToString());
        }
    }
    else {
        fout->PrintStringln(String("RegisteredServicesCache: services not loaded"));
    }
}

AutoPtr<IRegisteredServicesCacheListener> RegisteredServicesCache::GetListener()
{
    Mutex::Autolock lock(mServicesLock);
    return mListener;
}

void RegisteredServicesCache::SetListener(
    /* [in] */ IRegisteredServicesCacheListener* listener,
    /* [in] */ IHandler* handler)
{
    if (handler == NULL) {
        AutoPtr<ILooper> looper;
        mContext->GetMainLooper((ILooper**)&looper);
        CHandler::New(looper, (IHandler**)&handler);
    }

    Mutex::Autolock lock(mLock);
    mHandler = handler;
    mListener = listener;
}

void RegisteredServicesCache::NotifyListener(
    /* [in] */ IInterface* type,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean removed)
{
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        Logger::D(TAG, "notifyListener: %p is %s", type, (removed ? "removed" : "added"));
    }
    AutoPtr<IRegisteredServicesCacheListener> listener;
    AutoPtr<IHandler> handler;
    {
        Mutex::Autolock lock(mLock);

        listener = mListener;
        handler = mHandler;
    }
    if (listener == NULL) {
        return;
    }

    AutoPtr<IRegisteredServicesCacheListener> listener2 = listener;
    AutoPtr<IRunnable> runnable = new ListenerRunnable(type, userId, removed, listener2);
    Boolean result;
    handler->Post(runnable, &result);
}

AutoPtr<RegisteredServicesCache::ServiceInfo> RegisteredServicesCache::GetServiceInfo(
    /* [in] */ IInterface* type,
    /* [in] */ Int32 userId)
{
    Mutex::Autolock lock(mServicesLock);
    // Find user and lazily populate cache
    AutoPtr<UserServices> user = FindOrCreateUserLocked(userId);
    if (user->mServices == NULL) {
        GenerateServicesMap(userId);
    }

    AutoPtr<ServiceInfo> info;
    HashMap<AutoPtr<IInterface>, AutoPtr<ServiceInfo> >::Iterator it = user->mServices->Find(type);
    if (it != user->mServices->End()){
        info = it->mSecond;
    }
    return info;
}

AutoPtr< List<AutoPtr<RegisteredServicesCache::ServiceInfo> > > RegisteredServicesCache::GetAllServices(
    /* [in] */ Int32 userId)
{
    Mutex::Autolock lock(mServicesLock);
    // Find user and lazily populate cache
    AutoPtr<UserServices> user = FindOrCreateUserLocked(userId);
    if (user->mServices == NULL) {
        GenerateServicesMap(userId);
    }

    AutoPtr< List<AutoPtr<RegisteredServicesCache::ServiceInfo> > > infos = new List<AutoPtr<RegisteredServicesCache::ServiceInfo> >();
    HashMap<AutoPtr<IInterface>, AutoPtr<ServiceInfo> >::Iterator itr = user->mServices->Begin();
    for (; itr != user->mServices->End(); ++itr) {
        infos->PushBack(itr->mSecond);
    }

    return infos;
}

Boolean RegisteredServicesCache::InSystemImage(
    /* [in] */ Int32 callerUid)
{
    AutoPtr<IPackageManager> pkgManager;
    mContext->GetPackageManager((IPackageManager**)&pkgManager);
    AutoPtr< ArrayOf<String> > packages;
    pkgManager->GetPackagesForUid(callerUid, (ArrayOf<String>**)&packages);
    for (Int32 i = 0; i < packages->GetLength(); i++) {
        String name = (*packages)[i];
        // try {
        AutoPtr<IPackageInfo> packageInfo;
        if (FAILED(pkgManager->GetPackageInfo(name, 0 /* flags */, (IPackageInfo**)&packageInfo))) {
            return FALSE;
        }
        AutoPtr<IApplicationInfo> appInfo;
        ASSERT_SUCCEEDED(packageInfo->GetApplicationInfo((IApplicationInfo**)&appInfo));
        Int32 flags;
        appInfo->GetFlags(&flags);
        if ((flags & IApplicationInfo::FLAG_SYSTEM) != 0) {
            return TRUE;
        }
        // } catch (PackageManager.NameNotFoundException e) {
        //     return false;
        // }
    }
    return FALSE;
}

void RegisteredServicesCache::GenerateServicesMap(
    /* [in] */ Int32 userId)
{
    Slogger::D(TAG, "generateServicesMap() for %d", userId);

    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    List<AutoPtr<ServiceInfo> > serviceInfos;
    AutoPtr<IObjectContainer> resolveInfos;
    AutoPtr<IIntent> intent;
    CIntent::New(mInterfaceName, (IIntent**)&intent);
    ASSERT_SUCCEEDED(pm->QueryIntentServicesAsUser(intent, IPackageManager::GET_META_DATA, userId,
            (IObjectContainer**)&resolveInfos));
    AutoPtr<IObjectEnumerator> objEnumerator;
    resolveInfos->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator);
    Boolean hasNext;
    while (objEnumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> current;
        objEnumerator->Current((IInterface**)&current);
        AutoPtr<IResolveInfo> resolveInfo = (IResolveInfo*)current->Probe(EIID_IResolveInfo);
        // try {
        AutoPtr<ServiceInfo> info;
        ECode ec = ParseServiceInfo(resolveInfo, (ServiceInfo**)&info);
        if (FAILED(ec)) {
            String s;
            resolveInfo->ToString(&s);
            Slogger::W(TAG, "Unable to load service info %s ec = 0x%08x", s.string(), ec);
            continue;
        }
        serviceInfos.PushBack(info);
        // } catch (XmlPullParserException e) {
        //     Log.w(TAG, "Unable to load service info " + resolveInfo.toString(), e);
        // } catch (IOException e) {
        //     Log.w(TAG, "Unable to load service info " + resolveInfo.toString(), e);
        // }
    }

    Mutex::Autolock lock(mServicesLock);

    AutoPtr<UserServices> user = FindOrCreateUserLocked(userId);
    Boolean firstScan = user->mServices == NULL;
    if (firstScan) {
        user->mServices = new HashMap<AutoPtr<IInterface>, AutoPtr<ServiceInfo> >();
    }
    else {
        user->mServices->Clear();
    }

    StringBuilder changes;
    Boolean changed = FALSE;
    List<AutoPtr<ServiceInfo> >::Iterator itr = serviceInfos.Begin();
    for (Int32 i = 0; itr != serviceInfos.End(); ++itr, ++i) {
        AutoPtr<ServiceInfo> info = *itr;
        // four cases:
        // - doesn't exist yet
        //   - add, notify user that it was added
        // - exists and the UID is the same
        //   - replace, don't notify user
        // - exists, the UID is different, and the new one is not a system package
        //   - ignore
        // - exists, the UID is different, and the new one is a system package
        //   - add, notify user that it was added
        AutoPtr<IInteger32> previousUid;
        HashMap<AutoPtr<IInterface>, AutoPtr<IInteger32> >::Iterator it = user->mPersistentServices.Find(info->mType);
        if (it != user->mPersistentServices.End()) {
            previousUid = it->mSecond;
        }
        if (previousUid == NULL) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                changes += "  New service added: ";
                changes += info->ToString();
                changes += "\n";
            }

            changed = TRUE;
            (*user->mServices)[info->mType] = info;
            AutoPtr<IInteger32> iuid;
            CInteger32::New(info->mUid, (IInteger32**)&iuid);
            (user->mPersistentServices)[info->mType] = iuid;
            if (!(mPersistentServicesFileDidNotExist && firstScan)) {
                NotifyListener(info->mType, userId, FALSE /* removed */);
            }
        }
        else {
            Int32 pUid;
            previousUid->GetValue(&pUid);
            if (pUid == info->mUid) {
                if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                    changes.AppendCStr("  Existing service (nop): ");
                    changes.AppendString(info->ToString());
                    changes.AppendCStr("\n");
                }
                (*user->mServices)[info->mType] = info;
            }
            else if (InSystemImage(info->mUid)
                    || !ContainsTypeAndUid(&serviceInfos, info->mType, pUid)) {
                if (InSystemImage(info->mUid)) {
                    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                        changes.AppendCStr("  System service replacing existing: ");
                        changes.AppendString(info->ToString());
                        changes.AppendCStr("\n");
                    }
                }
                else {
                    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                        changes.AppendCStr("  Existing service replacing a removed service: ");
                        changes.AppendString(info->ToString());
                        changes.AppendCStr("\n");
                    }
                }
                (*user->mServices)[info->mType] = info;
                AutoPtr<IInteger32> iuid;
                CInteger32::New(info->mUid, (IInteger32**)&iuid);
                (user->mPersistentServices)[info->mType] = iuid;
                NotifyListener(info->mType, userId, FALSE /* removed */);
            }
            else {
                if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                    // ignore
                    changes.AppendCStr("  Existing service with new uid ignored: ");
                    changes.AppendString(info->ToString());
                    changes.AppendCStr("\n");
                }
            }
        }
    }

    List<AutoPtr<IInterface> > toBeRemoved;
    HashMap<AutoPtr<IInterface>, AutoPtr<IInteger32> >::Iterator item = user->mPersistentServices.Begin();
    for (; item != user->mPersistentServices.End(); ++item) {
        AutoPtr<IInterface> v1 = item->mFirst;
        if (!ContainsType(&serviceInfos, v1)) {
            toBeRemoved.PushBack(v1);
        }
    }
    List<AutoPtr<IInterface> >::Iterator toBeRemItr = toBeRemoved.Begin();
    for (; toBeRemItr != toBeRemoved.End(); ++toBeRemItr) {
        AutoPtr<IInterface> v1 = *toBeRemItr;
        user->mPersistentServices.Erase(v1);
        NotifyListener(v1, userId, TRUE /* removed */);
        changed = TRUE;

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            changes.AppendCStr("  Service removed: ");
            changes.AppendObject(v1);
            changes.AppendCStr("\n");
        }
    }

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        if (changes.GetLength() > 0) {
            Logger::D(TAG, "generateServicesMap(%s): %d services:%s\n",
                    mInterfaceName.string(), serviceInfos.GetSize(), changes.ToString().string());
        }
        else {
            Logger::D(TAG, "generateServicesMap(%s): %d services unchanged",
                    mInterfaceName.string(), serviceInfos.GetSize());
        }
    }

    if (changed) {
        WritePersistentServicesLocked();
    }
}

Boolean RegisteredServicesCache::ContainsType(
    /* [in] */ List<AutoPtr<ServiceInfo> >* serviceInfos,
    /* [in] */ IInterface* type)
{
    List<AutoPtr<ServiceInfo> >::Iterator itr = serviceInfos->Begin();
    for (; itr != serviceInfos->End(); ++itr) {
        AutoPtr<ServiceInfo> info = *itr;
        if (info->mType.Get() == type) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean RegisteredServicesCache::ContainsTypeAndUid(
    /* [in] */ List<AutoPtr<ServiceInfo> >* serviceInfos,
    /* [in] */ IInterface* type,
    /* [in] */ Int32 uid)
{
    List<AutoPtr<ServiceInfo> >::Iterator itr = serviceInfos->Begin();
    for (; itr != serviceInfos->End(); ++itr) {
        AutoPtr<ServiceInfo> info = *itr;
        if (info->mType.Get() == type && info->mUid == uid) {
            return TRUE;
        }

    }
    return FALSE;
}

ECode RegisteredServicesCache::ParseServiceInfo(
    /* [in] */ IResolveInfo* service,
    /* [out] */ ServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    AutoPtr<IServiceInfo> si;
    service->GetServiceInfo((IServiceInfo**)&si);
    AutoPtr<IComponentName> componentName;
    String packageName, name;
    si->GetPackageName(&packageName);
    si->GetName(&name);
    CComponentName::New(packageName, name, (IComponentName**)&componentName);

    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);

    AutoPtr<IXmlResourceParser> parser;
    // try {
    si->LoadXmlMetaData(pm, mMetaDataName, (IXmlResourceParser**)&parser);
    if (parser == NULL) {
        Slogger::E(TAG, "No %s meta-data", mMetaDataName.string());
        *info = NULL;
        return E_XML_PULL_PARSER_EXCEPTION;
        // throw new XmlPullParserException("No " + mMetaDataName + " meta-data");
    }

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type;
    while (parser->Next(&type), type != IXmlPullParser::END_DOCUMENT
            && type != IXmlPullParser::START_TAG) {
    }

    String nodeName;
    parser->GetName(&nodeName);
    if (!mAttributesName.Equals(nodeName)) {
        Slogger::E(TAG, "Meta-data does not start with %s tag", mAttributesName.string());
        if (parser != NULL) parser->Close();
        return E_XML_PULL_PARSER_EXCEPTION;
        // throw new XmlPullParserException(
        //         "Meta-data does not start with " + mAttributesName +  " tag");
    }

    AutoPtr<IApplicationInfo> appInfo;
    si->GetApplicationInfo((IApplicationInfo**)&appInfo);
    AutoPtr<IResources> resources;
    pm->GetResourcesForApplication(appInfo, (IResources**)&resources);
    String pkgName;
    si->GetPackageName(&pkgName);
    AutoPtr<IInterface> v;
    ECode ec = ParseServiceAttributes(resources, pkgName, attrs, (IInterface**)&v);
    if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        Slogger::E(TAG, "Unable to load resources for pacakge %s", pkgName.string());
        if (parser != NULL) parser->Close();
        return E_XML_PULL_PARSER_EXCEPTION;
        // throw new XmlPullParserException(
        //             "Unable to load resources for pacakge " + si.packageName);
    }
    else if (FAILED(ec)) {
        if (parser != NULL) parser->Close();
        return ec;
    }

    if (v == NULL) {
        if (parser != NULL) parser->Close();
        return NOERROR;
    }
    AutoPtr<IServiceInfo> serviceInfo;
    service->GetServiceInfo((IServiceInfo**)&serviceInfo);
    AutoPtr<IApplicationInfo> applicationInfo;
    serviceInfo->GetApplicationInfo((IApplicationInfo**)&applicationInfo);
    Int32 uid;
    applicationInfo->GetUid(&uid);
    *info = new ServiceInfo(v, componentName, uid);
    REFCOUNT_ADD(*info);
    if (parser != NULL) parser->Close();

    return NOERROR;
    // } catch (NameNotFoundException e) {
    //     throw new XmlPullParserException(
    //             "Unable to load resources for pacakge " + si.packageName);
    // } finally {
    //     if (parser != null) parser.close();
    // }
}

void RegisteredServicesCache::ReadPersistentServicesLocked()
{
    mUserServices.Clear();
    if (mSerializerAndParser == NULL) {
        return;
    }
    AutoPtr<IFileInputStream> fis;
    // try {
    AutoPtr<IFile> file;
    ASSERT_SUCCEEDED(mPersistentServicesFile->GetBaseFile((IFile**)&file));
    Boolean exists;
    file->Exists(&exists);
    mPersistentServicesFileDidNotExist = !exists;
    if (mPersistentServicesFileDidNotExist) {
        return;
    }
    if (FAILED(mPersistentServicesFile->OpenRead((IFileInputStream**)&fis))) {
        Slogger::W(TAG, "Error reading persistent services, starting from scratch");
        if (fis != NULL) {
            // try {
            fis->Close();
            // } catch (java.io.IOException e1) {
            // }
        }
        return;
    }
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    if (FAILED(parser->SetInput(fis, String(NULL)))) {
        Slogger::W(TAG, "Error reading persistent services, starting from scratch");
        if (fis != NULL) {
            // try {
            fis->Close();
            // } catch (java.io.IOException e1) {
            // }
        }
        return;
    }
    Int32 eventType;
    if (FAILED(parser->GetEventType(&eventType))) {
        Slogger::W(TAG, "Error reading persistent services, starting from scratch");
        if (fis != NULL) {
            // try {
            fis->Close();
            // } catch (java.io.IOException e1) {
            // }
        }
        return;
    }
    while (eventType != IXmlPullParser::START_TAG) {
        if (FAILED(parser->Next(&eventType))) {
            Slogger::W(TAG, "Error reading persistent services, starting from scratch");
            if (fis != NULL) {
                // try {
                fis->Close();
                // } catch (java.io.IOException e1) {
                // }
            }
            return;
        }
    }
    String tagName;
    if (FAILED(parser->GetName(&tagName))) {
        Slogger::W(TAG, "Error reading persistent services, starting from scratch");
        if (fis != NULL) {
            // try {
            fis->Close();
            // } catch (java.io.IOException e1) {
            // }
        }
        return;
    }
    if (tagName.Equals("services")) {
        if (FAILED(parser->Next(&eventType))) {
            Slogger::W(TAG, "Error reading persistent services, starting from scratch");
            if (fis != NULL) {
                // try {
                fis->Close();
                // } catch (java.io.IOException e1) {
                // }
            }
            return;
        }
        Int32 depth;
        do {
            if (eventType == IXmlPullParser::START_TAG && (parser->GetDepth(&depth), depth == 2)) {
                if (FAILED(parser->GetName(&tagName))) {
                    Slogger::W(TAG, "Error reading persistent services, starting from scratch");
                    if (fis != NULL) {
                        // try {
                        fis->Close();
                        // } catch (java.io.IOException e1) {
                        // }
                    }
                    return;
                }
                if (tagName.Equals("service")) {
                    AutoPtr<IInterface> service;
                    mSerializerAndParser->CreateFromXml(parser, (IInterface**)&service);
                    if (service == NULL) {
                        break;
                    }
                    String uidString;
                    if (FAILED(parser->GetAttributeValue(String(NULL), String("uid"), &uidString))) {
                        Slogger::W(TAG, "Error reading persistent services, starting from scratch");
                        if (fis != NULL) {
                            // try {
                            fis->Close();
                            // } catch (java.io.IOException e1) {
                            // }
                        }
                        return;
                    }
                    Int32 uid = StringUtils::ParseInt32(uidString);
                    AutoPtr<UserServices> user = FindOrCreateUserLocked(UserHandle::GetUserId(uid));
                    AutoPtr<IInteger32> iuid;
                    CInteger32::New(uid, (IInteger32**)&iuid);
                    (user->mPersistentServices)[service] = iuid;
                }
            }
            if (FAILED(parser->Next(&eventType))) {
                Slogger::W(TAG, "Error reading persistent services, starting from scratch");
                if (fis != NULL) {
                    // try {
                    fis->Close();
                    // } catch (java.io.IOException e1) {
                    // }
                }
                return;
            }
        } while (eventType != IXmlPullParser::END_DOCUMENT);
    }
    // } catch (Exception e) {
    //     Log.w(TAG, "Error reading persistent services, starting from scratch", e);
    // } finally {
    //     if (fis != null) {
    //         try {
    //             fis.close();
    //         } catch (java.io.IOException e1) {
    //         }
    //     }
    // }
}

void RegisteredServicesCache::WritePersistentServicesLocked()
{
    if (mSerializerAndParser == NULL) {
        return;
    }
    AutoPtr<IFileOutputStream> fos;
    // try {
    if (FAILED(mPersistentServicesFile->StartWrite((IFileOutputStream**)&fos))) {
        Slogger::W(TAG, "Error writing accounts");
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    AutoPtr<IXmlSerializer> out;
    assert(0);
    // XmlSerializer out = new FastXmlSerializer();
    ECode ec = out->SetOutput(fos, String("utf-8"));
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    AutoPtr<IBoolean> ib;
    CBoolean::New(TRUE, (IBoolean**)&ib);
    ec = out->StartDocument(String(NULL), ib);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    ec = out->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    ec = out->WriteStartTag(String(NULL), String("services"));
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    HashMap<Int32, AutoPtr<UserServices> >::Iterator itr = mUserServices.Begin();
    for (; itr != mUserServices.End(); ++itr) {
        AutoPtr<UserServices> user = itr->mSecond;
        HashMap<AutoPtr<IInterface>, AutoPtr<IInteger32> >::Iterator item = user->mPersistentServices.Begin();
        for (; item != user->mPersistentServices.End(); ++item) {
            ec = out->WriteStartTag(String(NULL), String("service"));
            if (ec == (ECode)E_IO_EXCEPTION) {
                Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
                if (fos != NULL) {
                    mPersistentServicesFile->FailWrite(fos);
                }
                return;
            }
            Int32 value;
            item->mSecond->GetValue(&value);
            ec = out->WriteAttribute(String(NULL), String("uid"), StringUtils::Int32ToString(value));
            if (ec == (ECode)E_IO_EXCEPTION) {
                Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
                if (fos != NULL) {
                    mPersistentServicesFile->FailWrite(fos);
                }
                return;
            }
            ec = mSerializerAndParser->WriteAsXml(item->mFirst, out);
            if (ec == (ECode)E_IO_EXCEPTION) {
                Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
                if (fos != NULL) {
                    mPersistentServicesFile->FailWrite(fos);
                }
                return;
            }
            ec = out->WriteEndTag(String(NULL), String("service"));
            if (ec == (ECode)E_IO_EXCEPTION) {
                Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
                if (fos != NULL) {
                    mPersistentServicesFile->FailWrite(fos);
                }
                return;
            }
        }
    }
    ec = out->WriteEndTag(String(NULL), String("services"));
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    ec = out->EndDocument();
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    ec = mPersistentServicesFile->FinishWrite(fos);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Error writing accounts 0x%08x", ec);
        if (fos != NULL) {
            mPersistentServicesFile->FailWrite(fos);
        }
        return;
    }
    // } catch (java.io.IOException e1) {
    //     Log.w(TAG, "Error writing accounts", e1);
    //     if (fos != null) {
    //         mPersistentServicesFile.failWrite(fos);
    //     }
    // }
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
