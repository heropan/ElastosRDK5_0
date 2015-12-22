
#include "pm/CPackageManagerService.h"
#include "pm/CResourcesChangedReceiver.h"
#include "pm/PackageSetting.h"
#include "pm/SELinuxMMAC.h"
#include "pm/CParcelFileDescriptorFactory.h"
#include "SystemConfig.h"
#include "ServiceThread.h"
#include "CDeviceStorageMonitorService.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/content/PackageHelper.h"
#include "elastos/droid/os/SELinux.h"
#include "elastos/droid/os/FileUtils.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/Environment.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/text/TextUtils.h"
#include "util/Xml.h"
#include "util/XmlUtils.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/Arrays.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CString;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::CInteger32;
using Elastos::Core::CBoolean;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::App::Admin::IIDevicePolicyManager;
using Elastos::Droid::App::Backup::IIBackupManager;
using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IComponentNameHelper;
using Elastos::Droid::Content::CComponentNameHelper;
using Elastos::Droid::Content::Pm::CActivityInfo;
using Elastos::Droid::Content::Pm::CApplicationInfo;
using Elastos::Droid::Content::Pm::CFeatureInfo;
using Elastos::Droid::Content::Pm::CParceledListSlice;
using Elastos::Droid::Content::Pm::CPermissionInfo;
using Elastos::Droid::Content::Pm::CPermissionInfoHelper;
using Elastos::Droid::Content::Pm::CResolveInfo;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IPackageCleanItem;
using Elastos::Droid::Content::Pm::IPermissionInfoHelper;
using Elastos::Droid::Content::Pm::CPackageCleanItem;
using Elastos::Droid::Content::Pm::CPackageStats;
using Elastos::Droid::Content::Pm::CVerificationParams;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Internal::Content::IPackageHelper;
using Elastos::Droid::Internal::Content::PackageHelper;
using Elastos::Droid::Internal::Content::INativeLibraryHelper;
using Elastos::Droid::Internal::Content::CNativeLibraryHelper;
using Elastos::Droid::Internal::Content::CPackageHelper;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::Environment;
using Elastos::Droid::Os::FileUtils;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::SELinux;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IParcelFileDescriptorHelper;
using Elastos::Droid::Os::CParcelFileDescriptorHelper;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::Droid::Utility::CSparseArray;
using Elastos::Droid::Utility::ILogHelper;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::Server::IDeviceStorageMonitorService;
using Elastos::Droid::Server::SystemConfig;
using Elastos::Droid::Server::ServiceThread;
using Elastos::Droid::Text::TextUtils;
using Elastos::IO::CFile;
using Elastos::IO::CFileHelper;
using Elastos::IO::CFileReader;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::ICloseable;
using Elastos::IO::IFileHelper;
using Elastos::IO::IFileReader;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IPrintWriter;
using Elastos::IO::CPrintWriter;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Security::IPublicKey;
using Elastos::Utility::Arrays;
using Elastos::Utility::CHashSet;
using Elastos::Utility::IIterator;
using Elastos::Utility::CDate;
using Elastos::Utility::IDate;
using Elastos::Utility::CArrayMap;
using Elastos::Utility::CHashMap;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger64;
using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Logging::Logger;
using Libcore::IO::CLibcore;
using Libcore::IO::ILibcore;
using Libcore::IO::IOs;
using Libcore::IO::IStructStat;
using Libcore::IO::IOsConstants;
using Libcore::IO::COsConstants;
using Libcore::IO::IBufferedOutputStream;
using Libcore::IO::CBufferedOutputStream;
using Libcore::IO::IBufferedInputStream;
using Libcore::IO::CBufferedInputStream;

#ifndef HASH_FUNC_FOR_AUTOPTR_ASECINSTALLARGS
#define HASH_FUNC_FOR_AUTOPTR_ASECINSTALLARGS
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Server::Pm::CPackageManagerService::AsecInstallArgs)
#endif

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

static const String READ_EXTERNAL_STORAGE = Elastos::Droid::Manifest::permission::READ_EXTERNAL_STORAGE;

//==============================================================================
//                  CPackageManagerService::PendingPackageBroadcasts
//==============================================================================

CPackageManagerService::PendingPackageBroadcasts::PendingPackageBroadcasts()
{

    CSparseArray::New(2, (ISparseArray**)&mUidMap);
}

AutoPtr<IList> CPackageManagerService::PendingPackageBroadcasts::Get(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName)
{
    AutoPtr<IHashMap> packages = GetOrAllocate(userId);
    AutoPtr<ICharSequence> cs;
    CString::New(packageName, (ICharSequence**)&cs);
    AutoPtr<IInterface> value;
    packages->Get(cs, (IInterface**)&value);
    return IList::Probe(value);
}

void CPackageManagerService::PendingPackageBroadcasts::Put(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName,
    /* [in] */ IArrayList* components)
{
    AutoPtr<IHashMap> packages = GetOrAllocate(userId);
    AutoPtr<ICharSequence> cs;
    CString::New(packageName, (ICharSequence**)&cs);
    packages->Put(cs, components);
}

void CPackageManagerService::PendingPackageBroadcasts::Remove(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName)
{
    AutoPtr<IInterface> value;
    mUidMap->Get(userId, (IInterface**)&value);
    AutoPtr<IHashMap> packages = IHashMap::Probe(value);
    if (packages != NULL) {
        AutoPtr<ICharSequence> cs;
        CString::New(packageName, (ICharSequence**)&cs);
        packages->Remove(cs);
    }
}

void CPackageManagerService::PendingPackageBroadcasts::Remove(
    /* [in] */ Int32 userId)
{
    mUidMap->Remove(userId);
}

Int32 CPackageManagerService::PendingPackageBroadcasts::UserIdCount()
{
    Int32 size;
    mUidMap->GetSize(&size);
    return size;
}

Int32 CPackageManagerService::PendingPackageBroadcasts::UserIdAt(
    /* [in] */ Int32 n)
{
    Int32 at;
    mUidMap->KeyAt(n, &at);
    return at;
}

AutoPtr<IHashMap> CPackageManagerService::PendingPackageBroadcasts::PackagesForUserId(
    /* [in] */ Int32 userId)
{
    AutoPtr<IInterface> value;
    mUidMap->Get(userId, (IInterface**)&value);
    AutoPtr<IHashMap> packages = IHashMap::Probe(value);
    return packages;
}

Int32 CPackageManagerService::PendingPackageBroadcasts::Size()
{
    // total number of pending broadcast entries across all userIds
    Int32 size;
    mUidMap->GetSize(&size);
    Int32 num = 0;
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> value;
        mUidMap->ValueAt(i, (IInterface**)&value);
        AutoPtr<IHashMap> packages = IHashMap::Probe(value);
        Int32 count;
        packages->GetSize(&count);
        num += count;
    }
    return num;
}

void CPackageManagerService::PendingPackageBroadcasts::Clear()
{
    mUidMap->Clear();
}

AutoPtr<IHashMap> CPackageManagerService::PendingPackageBroadcasts::GetOrAllocate(
    /* [in] */ Int32 userId)
{
    AutoPtr<IInterface> value;
    mUidMap->Get(userId, (IInterface**)&value);
    AutoPtr<IHashMap> map = IHashMap::Probe(value);
    if (map == NULL) {
        CHashMap::New((IHashMap**)&map);
        mUidMap->Put(userId, map);
    }
    return map;
}


//==============================================================================
//                  CPackageManagerService::DefaultContainerConnection
//==============================================================================

CAR_INTERFACE_IMPL(CPackageManagerService::DefaultContainerConnection, Object, IServiceConnection)

ECode CPackageManagerService::DefaultContainerConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    if (DEBUG_SD_INSTALL)
        Slogger::I(TAG, "onServiceConnected");

    AutoPtr<IMediaContainerService> imcs = IMediaContainerService::Probe(service);
    assert(imcs != NULL);

    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(CPackageManagerService::MCS_BOUND, imcs, (IMessage**)&msg);
    Boolean result;
    return mHost->mHandler->SendMessage(msg, &result);
}

ECode CPackageManagerService::DefaultContainerConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    if (DEBUG_SD_INSTALL) Slogger::I(TAG, "onServiceDisconnected");
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::PackageUsage::WriteThread
//==============================================================================

CPackageManagerService::PackageUsage::WriteThread::WriteThread(
    /* [in] */ const String& threadName,
    /* [in] */ PackageUsage* host)
    : mHost(host)
{
    Thread::constructor(threadName);
}

CPackageManagerService::PackageUsage::WriteThread::Run()
{
    // try {
    mHost->WriteInternal();
    // } finally {
    //     mBackgroundWriteRunning.set(false);
    // }
    mHost->mBackgroundWriteRunning->Set(FALSE);
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::PackageUsage
//==============================================================================

const Int32 CPackageManagerService::PackageUsage::WRITE_INTERVAL;

CPackageManagerService::PackageUsage::PackageUsage(
    /* [in] */ CPackageManagerService* host)
    : mIsHistoricalPackageUsageAvailable(TRUE)
    , mHost(host)
{
    CAtomicInteger64::New(0, (IAtomicInteger64**)&mLastWritten);
    CAtomicBoolean::New(FALSE, (IAtomicBoolean**)&mBackgroundWriteRunning);
}

Boolean CPackageManagerService::PackageUsageIsHistoricalPackageUsageAvailable()
{
    return mIsHistoricalPackageUsageAvailable;
}

void CPackageManagerService::PackageUsage::Write(
    /* [in] */ Boolean force)
{
    if (force) {
        WriteInternal();
        return;
    }
    Int64 value;
    if ((mLastWritten->Get(&value), (SystemClock::GetElapsedRealtime() - value < WRITE_INTERVAL)
            && !DEBUG_DEXOPT) {
        return;
    }
    Boolean result;
    if (mBackgroundWriteRunning->CompareAndSet(FALSE, TRUE, &result), result) {
        AutoPtr<WriteThread> thread = new WriteThread(String("PackageUsage_DiskWriter"), this);
        thread->Start();
    }
}

void CPackageManagerService::PackageUsage::WriteInternal()
{
    synchronized (mHost->mPackagesLock) {
        synchronized (mFileLock) {
            AutoPtr<IAtomicFile> file = GtFile();
            AutoPtr<IFileOutputStream> f;
            // try {
            file->StartWrite((IFileOutputStream**)&f);
            AutoPtr<IBufferedOutputStream> out;
            CBufferedOutputStream::New(IOutputStream::Probe(f), (IBufferedOutputStream**)&out);
            AutoPtr<IFile> baseF;
            file->GetBaseFile((IFile**)&baseF);
            String path;
            baseF->GetPath(&path);
            FileUtils::SetPermissions(path, 0660, IProcess::SYSTEM_UID, IProcess::PACKAGE_INFO_GID);
            StringBuilder sb;
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mHost->mPackages.Begin();
            for (; it != mhost->mPackages.End(); ++it) {
                AutoPtr<PackageParser::Package> pkg = it->mSecond;
                if (pkg->mLastPackageUsageTimeInMills == 0) {
                    continue;
                }
                sb.SetLength(0);
                sb.Append(pkg->mPackageName);
                sb.AppendChar(' ');
                sb.Append((Int64)pkg->mLastPackageUsageTimeInMills);
                sb.AppendChar('\n');
                IOutputStream::Probe(out)->Write(sb.ToString().GetBytes(/*StandardCharsets.US_ASCII)*/);
            }
            IFlushable::Probe(out)->Flush();
            file->FinishWrite(f);
            // } catch (IOException e) {
            //     if (f != null) {
            //         file.failWrite(f);
            //     }
            //     Log.e(TAG, "Failed to write package usage times", e);
            // }
        }
    }
    mLastWritten->Set(SystemClock::GetElapsedRealtime());
}

void CPackageManagerService::PackageUsage::ReadLP()
{
    synchronized (mFileLock) {
        AutoPtr<IAtomicFile> file = GetFile();
        AutoPtr<IBufferedInputStream> in;
        // try {
        AutoPtr<IFileInputStream> fileIn;
        file->OpenRead((IFileInputStream**)&fileIn);
        CBufferedInputStream::New(IInputStream::Probe(fileIn), (IBufferedInputStream**)&in);
        StringBuilder sb;
        while (TRUE) {
            AutoPtr<IInputStream> is = IInputStream::Probe(in);
            String packageName;
            if (FAILED(ReadToken(is, sb, ' ', &packageName))) {
                Logger::W(CPackageManagerService::TAG, "Failed to read package usage times");
                break;
            }
            if (packageName.IsNull()) {
                break;
            }
            String timeInMillisString;
            if (FAILED(ReadToken(is, sb, '\n', &timeInMillisString))) {
                Logger::W(CPackageManagerService::TAG, "Failed to read package usage times");
                break;
            }
            if (timeInMillisString.IsNull()) {
                Logger::E(CPackageManagerService::TAG, "Failed to find last usage time for package %s", packageName.string());
                return E_IO_EXCEPTION;
            }
            AutoPtr<PackageParser::Package> pkg;
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mHost->mPackages.Find(packageName);
            if (it != mHost->mPackages.End()) {
                pkg = it->mSecond;
            }
            if (pkg == NULL) {
                continue;
            }
            // try {
            // Int64 timeInMillis = StringUtils::ParseInt64(timeInMillisString);
            // } catch (NumberFormatException e) {
            //     throw new IOException("Failed to parse " + timeInMillisString
            //                           + " as a long.", e);
            // }
            pkg->mLastPackageUsageTimeInMills = StringUtils::ParseInt64(timeInMillisString);
        }
        // } catch (FileNotFoundException expected) {
        //     mIsHistoricalPackageUsageAvailable = false;
        // } catch (IOException e) {
        //     Log.w(TAG, "Failed to read package usage times", e);
        // } finally {
        //     IoUtils.closeQuietly(in);
        // }
        AutoPtr<IIoUtils> ioUtils;
        CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
        ioUtils->CloseQuietly(ICloseable::Probe(in));
    }
    mLastWritten->Set(SystemClock::GetElapsedRealtime());
}

ECode CPackageManagerService::PackageUsage::ReadToken(
    /* [in] */ IInputStream* in,
    /* [in] */ StringBuilder sb,
    /* [in] */ Char32 endOfToken,
    /* [in] */ String* token)
{
    sb.SetLength(0);
    while (TRUE) {
        Int32 ch;
        in->Read(&ch);
        if (ch == -1) {
            if (sb.GetLength() == 0) {
                *token = String(NULL);
            }
            Logger::E(CPackageManagerService::TAG, "Unexpected EOF");
            return E_IO_EXCEPTION;
        }
        if (ch == endOfToken) {
            *token = sb.ToString();
        }
        sb.AppendChar((Char)ch);
    }
    return NOERROR;
}

AutoPtr<IAtomicFile> CPackageManagerService::PackageUsage::GetFile()
{
    AutoPtr<IFile> dataDir = Environment::GetDataDirectory((IFile**)&dataDir);
    AutoPtr<IFile> systemDir;
    CFile::New(dataDir, String("system"), (IFile**)&systemDir);
    AutoPtr<IFile> fname;
    CFile::New(systemDir, String("package-usage.list"), (IFile**)&fname);
    AutoPtr<IAtomicFile> af;
    CAtomicFile::New(fname, (IAtomicFile**)&af);
    return af;
}


//==============================================================================
//                  CPackageManagerService::PackageHandler
//==============================================================================

Boolean CPackageManagerService::PackageHandler::ConnectToService()
{
    if (CPackageManagerService::DEBUG_SD_INSTALL)
        Logger::I(TAG, "Trying to bind to DefaultContainerService");

    AutoPtr<IIntent> service;
    CIntent::New((IIntent**)&service);
    service->SetComponent(DEFAULT_CONTAINER_COMPONENT);
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_DEFAULT);
    Boolean result
    if (mHost->mContext->BindService(service, mHsot->mDefContainerConn,
            IContext::BIND_AUTO_CREATE, IUserHandle::OWNER, &result), result) {
        Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
        mBound = TRUE;
        return TRUE;
    }
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
    return FALSE;
}

void CPackageManagerService::PackageHandler::DisconnectService()
{
    mHost->mContainerService = NULL;
    mBound = FALSE;
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_DEFAULT);
    mHost->mContext->UnbindService(mHost->mDefContainerConn);
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
}


ECode CPackageManagerService::PackageHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    // try {
    DoHandleMessage(msg);
    // } finally {
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
    // }
    return NOERROR;
}

void CPackageManagerService::PackageHandler::DoHandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    switch (what) {
        case CPackageManagerService::INIT_COPY:{
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<HandlerParams> params = (HandlerParams*)obj.Get();
            if (DEBUG_INSTALL) {
                // Slog.i(TAG, "init_copy idx=" + idx + ": " + params);
            }
            // If a bind was already initiated we dont really
            // need to do anything. The pending install
            // will be processed later on.
            if (!mBound) {
                // If this is the only one pending we might
                // have to bind to the service again.
                if (!ConnectToService()) {
                    Slogger::E(TAG, "Failed to bind to media container service");
                    params->ServiceError();
                    return;
                }
                else {
                    // Once we bind to the service, the first
                    // pending request will be processed.
                    mPendingInstalls.PushBack(params);
                }
            }
            else {
                mPendingInstalls.PushBack(params);
                // Already bound to the service. Just make
                // sure we trigger off processing the first request.
                if (idx == 0) {
                    Boolean result;
                    mHandler->SendEmptyMessage(MCS_BOUND, &result);
                }
            }
            break;
        }
        case CPackageManagerService::MCS_BOUND:{
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IMediaContainerService> service = IMediaContainerService::Probe(obj);
            if (CPackageManagerService::DEBUG_INSTALL)
                Slogger::I(TAG, "mcs_bound");

            if (service != NULL) {
                mHost->mContainerService = service;
            }

            if (mHost->mContainerService == NULL) {
                // Something seriously wrong. Bail out
                Slogger::E(TAG, "Cannot bind to media container service");
                List<AutoPtr<HandlerParams> >::Iterator iter = mPendingInstalls.Begin();
                for (; iter != mPendingInstalls.End(); ++iter) {
                    (*iter)->ServiceError();
                }
                mPendingInstalls.Clear();
            }
            else if (mPendingInstalls.IsEmpty() == FALSE) {
                List<AutoPtr<HandlerParams> >::Iterator iter = mPendingInstalls.Begin();
                AutoPtr<HandlerParams> params = *iter;
                if (params != NULL) {
                    if (params->StartCopy()) {
                        // We are done...  look for more work or to
                        // go idle.
                        if (DEBUG_SD_INSTALL)
                            Logger::I(TAG, "Checking for more work or unbind...");
                        // Delete pending install
                        if (mPendingInstalls.IsEmpty() == FALSE) {
                            mPendingInstalls.PopFront();
                        }
                        if (mPendingInstalls.IsEmpty()) {
                            if (mBound) {
                                if (DEBUG_SD_INSTALL)
                                    Logger::I(TAG, "Posting delayed MCS_UNBIND");

                                mHost->mHandler->RemoveMessages(MCS_UNBIND);

                                // Unbind after a little delay, to avoid
                                // continual thrashing.
                                AutoPtr<IMessage> ubmsg;
                                mHost->mHandler->ObtainMessage(MCS_UNBIND, (IMessage**)&ubmsg);
                                Boolean result;
                                mHost->mHandler->SendMessageDelayed(ubmsg, 10000, &result);
                            }
                        }
                        else {
                            // There are more pending requests in queue.
                            // Just post MCS_BOUND message to trigger processing
                            // of next pending install.
                            if (DEBUG_SD_INSTALL)
                                Logger::I(TAG, "Posting MCS_BOUND for next work");

                            Boolean result;
                            mHost->mHandler->SendEmptyMessage(MCS_BOUND, &result);
                        }
                    }
                    params = NULL;
                }
            }
            else {
                // Should never happen ideally.
                Slogger::W(TAG, "Empty queue");
            }
            break;
        }
        case CPackageManagerService::MCS_RECONNECT:{
            if (DEBUG_INSTALL)
                Slogger::I(TAG, "mcs_reconnect");
            if (mPendingInstalls.IsEmpty() == FALSE) {
                if (mBound) {
                    DisconnectService();
                }

                if (!ConnectToService()) {
                    Slogger::E(TAG, "Failed to bind to media container service");
                    List<AutoPtr<HandlerParams> >::Iterator iter = mPendingInstalls.Begin();
                    for (; iter != mPendingInstalls.End(); ++iter) {
                        // Indicate service bind error
                        (*iter)->ServiceError();
                    }
                    mPendingInstalls.Clear();
                }
            }
            break;
        }
        case CPackageManagerService::MCS_UNBIND:{
            // If there is no actual work left, then time to unbind.
            if (DEBUG_INSTALL)
                Slogger::I(TAG, "mcs_unbind");

            if (mPendingInstalls.IsEmpty() && mPendingVerification.IsEmpty()) {
                if (mBound) {
                    if (DEBUG_INSTALL)
                        Slogger::I(TAG, "calling disconnectService()");

                    DisconnectService();
                }
            }
            else if (mPendingInstalls.IsEmpty() == FALSE) {
                // There are more pending requests in queue.
                // Just post MCS_BOUND message to trigger processing
                // of next pending install.
                Boolean result;
                mHost->mHandler->SendEmptyMessage(MCS_BOUND, &result);
            }
            break;
        }
        case CPackageManagerService::MCS_GIVE_UP:{
            if (DEBUG_INSTALL) {
                Slogger::I(TAG, "mcs_giveup too many retries");
            }
            mPendingInstalls.PopFront();
            break;
        }
        case CPackageManagerService::SEND_PENDING_BROADCAST:{
            AutoPtr< ArrayOf<String> > packages;
            AutoPtr< ArrayOf<IList*> > components;
            Int32 size = 0;
            AutoPtr< ArrayOf<Int32> > uids;
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_DEFAULT);
            synchronized (mHost->mPackagesLock) {
                if (mHost->mPendingBroadcasts == NULL) {
                    return;
                }
                size = mHost->mPendingBroadcasts->Size();
                if (size <= 0) {
                    // Nothing to be done. Just return
                    return;
                }
                packages = ArrayOf<String>::Alloc(size);
                components = ArrayOf<IList*>::Alloc(size);
                uids = ArrayOf<Int32>::Alloc(size);
                Int32 i = 0;  // filling out the above arrays

                for (Int32 n = 0; n < mHost->mPendingBroadcasts->UserIdCount(); n++) {
                    Int32 packageUserId = mHost->mPendingBroadcasts->UserIdAt(n);
                    AutoPtr<ISet> set;
                    mHost->mPendingBroadcasts->PackagesForUserId(packageUserId)->GetEntrySet((ISet**)&set);
                    AutoPtr<IIterator> it;
                    set->GetIterator((IIterator**)&it);
                    Boolean hasNext;
                    while ((it->HasNext(&hasNext), hasNext) && i < size) {
                        AutoPtr<IInterface> next;
                        it->GetNext((IInterface**)&next);
                        AutoPtr<IMapEntry> ent = IMapEntry::Probe(next);
                        AutoPtr<IInterface> key;
                        ent->GetKey((IInterface**)&key);
                        String keyStr;
                        ICharSequence::Probe(key)->ToString(&keyStr);
                        (*packages)[i] = keyStr
                        AutoPtr<IInterface> value;
                        ent->GetValue((IInterface**)&value);
                        components->Set(i, IList::Probe(value));
                        HashMap<String, AutoPtr<PackageSetting> >::Iterator pkgit
                                = mHost->mSettings->mPackages.Find(keyStr);
                        AutoPtr<PackageSetting> ps;
                        if (it != mHost->mSettings->mPackages.End()) {
                            ps = it->mSecond;
                        }
                        (*uids)[i] = (ps != NULL) ?
                                UserHandle::GetUid(packageUserId, ps->mAppId) : -1;
                        i++;
                    }
                }
                size = i;
                mHost->mPendingBroadcasts.clear();
            }
            // Send broadcasts
            for (Int32 i = 0; i < size; i++) {
                mHost->SendPackageChangedBroadcast((*packages)[i], TRUE, (*components)[i], (*uids)[i]);
            }
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
            break;
        }
        case CPackageManagerService::START_CLEANING_PACKAGE:{
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<ICharSequence> seq = ICharSequence::Probe(obj);
            String str;
            seq->ToString(&str);
            Int32 userId = arg1;
            Boolean andCode = arg2 != 0;
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_DEFAULT);
            synchronized (mHost->mPackagesLock) {
                if (userId == IUserHandle::USER_ALL) {
                    AutoPtr<ArrayOf<Int32> > users = CPackageManagerService::sUserManager->GetUserIds();
                    Int32 len = users->GetLength();
                    for (Int32 i = 0; i < len; i++) {
                        Int32 user = (*users)[i];
                        AutoPtr<IPackageCleanItem> item;
                        CPackageCleanItem::New(user, packageName, andCode, (IPackageCleanItem**)&item);
                        mHost->mSettings->AddPackageToCleanLPw(item);
                    }
                }
                else {
                    AutoPtr<IPackageCleanItem> item;
                    CPackageCleanItem::New(userId, packageName, andCode, (IPackageCleanItem**)&item);
                    mHost->mSettings->AddPackageToCleanLPw(item);
                }
            }
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
            mHost->StartCleaningPackages();
            break;
        }
        case CPackageManagerService::POST_INSTALL:{
            Int32 token = arg1;
            if (CPackageManagerService::DEBUG_INSTALL)
                Logger::V(TAG, "Handling post-install for %d", token);
            HashMap<Int32, AutoPtr<PostInstallData> >::Iterator find = mHost->mRunningInstalls.Find(token);
            AutoPtr<PostInstallData> data = (find != mHost->mRunningInstalls.End() ? find->mSecond : NULL);
            mHost->mRunningInstalls.Erase(find);
            Boolean deleteOld = FALSE;

            if (data != NULL) {
                AutoPtr<InstallArgs> args = data->mArgs;
                AutoPtr<PackageInstalledInfo> res = data->mRes;

                if (res->mReturnCode == IPackageManager::INSTALL_SUCCEEDED) {
                    res->mRemovedInfo->SendBroadcast(FALSE, TRUE, FALSE);
                    AutoPtr<IBundle> extras;
                    CBundle::New(1, (IBundle**)&extras);
                    extras->PutInt32(IIntent::EXTRA_UID, res->mUid);
                    // Determine the set of users who are adding this
                    // package for the first time vs. those who are seeing
                    // an update.
                    AutoPtr<ArrayOf<Int32> > firstUsers;
                    AutoPtr<ArrayOf<Int32> > updateUsers = ArrayOf<Int32>::Alloc(0);
                    if (res->mOrigUsers == NULL || res->mOrigUsers->GetLength() == 0) {
                        firstUsers = res->mNewUsers;
                    }
                    else {
                        firstUsers = ArrayOf<Int32>::Alloc(0);
                        for (Int32 i = 0; i < res->mNewUsers->GetLength(); i++) {
                            Int32 user = (*res->mNewUsers)[i];
                            Boolean isNew = TRUE;
                            for (Int32 j = 0; j < res->mOrigUsers->GetLength(); j++) {
                                if ((*res->mOrigUsers)[j] == user) {
                                    isNew = FALSE;
                                    break;
                                }
                            }
                            if (isNew) {
                                AutoPtr<ArrayOf<Int32> > newFirst =
                                        ArrayOf<Int32>::Alloc(firstUsers->GetLength() + 1);
                                newFirst->Copy(firstUsers);
                                (*newFirst)[firstUsers->GetLength()] = user;
                                firstUsers = newFirst;
                            }
                            else {
                                AutoPtr<ArrayOf<Int32> > newUpdate =
                                        ArrayOf<Int32>::Alloc(updateUsers->GetLength()+1);
                                newUpdate->Copy(updateUsers);
                                (*newUpdate)[updateUsers->GetLength()] = user;
                                updateUsers = newUpdate;
                            }
                        }
                    }
                    String packageName;
                    res->mPkg->mApplicationInfo->GetPackageName(&packageName);
                    mHost->SendPackageBroadcast(
                            IIntent::ACTION_PACKAGE_ADDED, packageName,
                            extras, String(NULL), NULL, firstUsers);
                    Boolean update = res->mRemovedInfo->mRemovedPackage != NULL;
                    if (update) {
                        extras->PutBoolean(IIntent::EXTRA_REPLACING, TRUE);
                    }
                    mHost->SendPackageBroadcast(
                            IIntent::ACTION_PACKAGE_ADDED, packageName,
                            extras, String(NULL), NULL, updateUsers);
                    if (update) {
                        mHost->SendPackageBroadcast(
                                IIntent::ACTION_PACKAGE_REPLACED, packageName,
                                extras, String(NULL), NULL, updateUsers);
                        mHost->SendPackageBroadcast(
                                IIntent::ACTION_MY_PACKAGE_REPLACED,
                                String(NULL), NULL, packageName, NULL, updateUsers);

                        // treat asec-hosted packages like removable media on upgrade
                        if (mHost->IsForwardLocked(res->mPkg) || mHost->IsExternal(res->mPkg)) {
                            if (CPackageManagerService::DEBUG_INSTALL) {
                                Slogger::I(TAG, "upgrading pkg %p is ASEC-hosted -> AVAILABLE", res->mPkg.Get());
                            }
                            AutoPtr< ArrayOf<Int32> > uidArray = ArrayOf<Int32>::Alloc(1);
                            (*uidArray)[0] = res->mPkg->mApplicationInfo->GetUid(&((*uidArray)[0]));
                            List<String> pkgList;
                            String packageName;
                            res->mPkg->mApplicationInfo->GetPackageName(&packageName);
                            pkgList.PushBack(packageName);
                            mHost->SendResourcesChangedBroadcast(TRUE, TRUE, pkgList, uidArray, NULL);
                        }
                    }
                    if (res->mRemovedInfo->mArgs != NULL) {
                        // Remove the replaced package's older resources safely now
                        deleteOld = TRUE;
                    }

                    // Log current value of "unknown sources" setting
                    // EventLog.writeEvent(EventLogTags.UNKNOWN_SOURCES_ENABLED,
                    //     getUnknownSourcesSettings());
                }
                // Force a gc to clear up things
                // Runtime.getRuntime().gc();
                // We delete after a gc for applications  on sdcard.
                if (deleteOld) {
                    synchronized (mHost->mInstallLock) {
                        res->mRemovedInfo->mArgs->DoPostDeleteLI(TRUE);
                    }
                }
                if (args->mObserver != NULL) {
                    // try {
                    AutoPtr<IBundle> extras = mHost->ExtrasForInstallResult(res);
                    args->mObserver->OnPackageInstalled(res->mName, res->mReturnCode,
                            res->mReturnMsg, extras);
                    // } catch (RemoteException e) {
                    //     Slog.i(TAG, "Observer no longer exists.");
                    // }
                }
            }
            else {
                Slogger::E(TAG, "Bogus post-install token %d", token);
            }
            break;
        }
        case CPackageManagerService::UPDATED_MEDIA_STATUS:{
            Boolean reportStatus = arg1 == 1;
            Boolean doGc = arg2 == 1;
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<ISet> set = ISet::Probe(obj);
            if (CPackageManagerService::DEBUG_SD_INSTALL)
                Logger::I(TAG, "Got message UPDATED_MEDIA_STATUS");
            if (CPackageManagerService::DEBUG_SD_INSTALL)
                Logger::I(TAG, "reportStatus=%d, doGc = %d", reportStatus, doGc);
            if (doGc) {
                // Force a gc to clear up stale containers.
                // Runtime.getRuntime().gc();
            }
            if (args != NULL) {
                // @SuppressWarnings("unchecked")
                if (CPackageManagerService::DEBUG_SD_INSTALL) Logger::I(TAG, "Unloading all containers");
                // Unload containers
                mHost->UnloadAllContainers(args);
            }
            if (reportStatus) {
                // try {
                if (CPackageManagerService::DEBUG_SD_INSTALL) Logger::I(TAG, "Invoking MountService call back");
                AutoPtr<IPackageHelper> helper;
                CPackageHelper::AcquireSingleton((IPackageHelper**)&helper);
                AutoPtr<IMountService> service;
                helper->GetMountService((IMountService**)&service);
                service->FinishMediaUpdate();
                // } catch (RemoteException e) {
                //     Log.e(TAG, "MountService not running?");
                // }
            }
            break;
        }
        case CPackageManagerService::WRITE_SETTINGS:{
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_DEFAULT);
            synchronized (mHost->mPackagesLock) {
                mHost->mHandler->RemoveMessages(CPackageManagerService::WRITE_SETTINGS);
                mHost->mHandler->RemoveMessages(CPackageManagerService::WRITE_PACKAGE_RESTRICTIONS);
                mHost->mSettings->WriteLPr();
                mHost->mDirtyUsers.Clear();
            }
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
            break;
        }
        case CPackageManagerService::WRITE_PACKAGE_RESTRICTIONS:{
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_DEFAULT);
            synchronized (mHost->mPackagesLock) {
                mHost->mHandler->RemoveMessages(WRITE_PACKAGE_RESTRICTIONS);
                HashSet<Int32>::Iterator it = mHost->mDirtyUsers.Begin();
                for (; it != mDirtyUsers.End(); ++it) {
                    mHost->mSettings->WritePackageRestrictionsLPr(*it);
                }
                mHost->mDirtyUsers.Clear();
            }
            Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
            break;
        }
        case CPackageManagerService::CHECK_PENDING_VERIFICATION:{
            HashMap<Int32, AutoPtr<PackageVerificationState> >::Iterator it
                    = mHost->mPendingVerification.Find(verificationId);
            AutoPtr<PackageVerificationState> state;
            if (it != mHost->mPendingVerification.End()) state = it->mSecond;

            if ((state != NULL) && !state->TimeoutExtended()) {
                AutoPtr<InstallArgs> args = state->GetInstallArgs();
                AutoPtr<IUri> originUri = Uri::FromFile(args->mOrigin->mResolvedFile);

                Slogger::I(TAG, "Verification timed out for %p", originUri);
                mHost->mPendingVerification.Erase(it);

                Int32 ret = IPackageManager::INSTALL_FAILED_VERIFICATION_FAILURE;

                if (GetDefaultVerificationResponse() == IPackageManager::VERIFICATION_ALLOW) {
                    Slogger::I(TAG, "Continuing with installation of %p", originUri);
                    state->SetVerifierResponse(Binder::GetCallingUid(),
                            IPackageManager::VERIFICATION_ALLOW_WITHOUT_SUFFICIENT);
                    AutoPtr<IUserHandle> user = state->GetInstallArgs()->GetUser();
                    mHost->BroadcastPackageVerified(verificationId, originUri,
                            IPackageManager::VERIFICATION_ALLOW, user);
                    // try {
                    args->CopyPkg(mContainerService, TRUE, &ret);
                    // } catch (RemoteException e) {
                    //     Slog.e(TAG, "Could not contact the ContainerService");
                    // }
                }
                else {
                    AutoPtr<IUserHandle> user = state->GetInstallArgs()->GetUser();
                    mHost->BroadcastPackageVerified(verificationId, originUri,
                            IPackageManager::VERIFICATION_REJECT, user);
                }

                mHost->ProcessPendingInstall(args, ret);
                Boolean result;
                mHost->mHandler->SendEmptyMessage(MCS_UNBIND, &result);
            }
            break;
        }
        case CPackageManagerService::PACKAGE_VERIFIED:{
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<PackageVerificationResponse> response = (PackageVerificationResponse*)obj.Get();
            HashMap<Int32, AutoPtr<PackageVerificationState> >::Iterator it
                    = mHost->mPendingVerification.Find(verificationId);
            AutoPtr<PackageVerificationState> state;
            if (it != mHost->mPendingVerification.End()) state = it->mSecond;
            if (state == NULL) {
                Slogger::W(TAG, "Invalid verification token %d received", verificationId);
                return;
            }

            state->SetVerifierResponse(response->mCallerUid, response->mCode);

            if (state->IsVerificationComplete()) {
                mHost->mPendingVerification.Erase(it);

                AutoPtr<InstallArgs> args = state->GetInstallArgs();
                AutoPtr<IUri> originUri = Uri::FromFile(args->mOrigin->mResolvedFile);

                Int32 ret;
                if (state->IsInstallAllowed()) {
                    ret = IPackageManager::INSTALL_FAILED_INTERNAL_ERROR;
                    mHost->BroadcastPackageVerified(verificationId, originUri,
                            response->mCode, state->GetInstallArgs()->GetUser());
                    // try {
                    args->CopyPkg(mContainerService, TRUE, &ret);
                    // } catch (RemoteException e) {
                    //     Slog.e(TAG, "Could not contact the ContainerService");
                    // }
                }
                else {
                    ret = IPackageManager::INSTALL_FAILED_VERIFICATION_FAILURE;
                }

                mHost->ProcessPendingInstall(args, ret);
                Boolean result;
                mHost->mHandler->SendEmptyMessage(MCS_UNBIND, &result);
            }
            break;
        }
    }
}


//==============================================================================
//                  CPackageManagerService::ActivityIntentResolver
//==============================================================================

CPackageManagerService::ActivityIntentResolver::ActivityIntentResolver(
    /* [in] */ CPackageManagerService* owner)
    : mActivities(23)
    , mFlags(0)
    , mHost(owner)
{}

AutoPtr<List< AutoPtr<IResolveInfo> > >
CPackageManagerService::ActivityIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    mFlags = defaultOnly ? IPackageManager::MATCH_DEFAULT_ONLY : 0;
    return Super::QueryIntent(intent, resolvedType, defaultOnly, userId);
}

AutoPtr<List<AutoPtr<IResolveInfo> > >
CPackageManagerService::ActivityIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
        (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0, userId);
}

AutoPtr<List<AutoPtr<IResolveInfo> > >
CPackageManagerService::ActivityIntentResolver::QueryIntentForPackage(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<AutoPtr<PackageParser::Activity> >* packageActivities,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    if (packageActivities == NULL) {
        return NULL;
    }
    mFlags = flags;
    Boolean defaultOnly = (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0;
    List<AutoPtr<ArrayOf<PackageParser::ActivityIntentInfo*> > > listCut;

    List<AutoPtr<PackageParser::Activity> >::Iterator it;
    for (it = packageActivities->Begin(); it != packageActivities->End(); ++it) {
        List<AutoPtr<PackageParser::ActivityIntentInfo> >& intentFilters = (*it)->mIntents;
        if (!intentFilters.IsEmpty()) {
            AutoPtr< ArrayOf<PackageParser::ActivityIntentInfo*> > array =
                    ArrayOf<PackageParser::ActivityIntentInfo*>::Alloc(intentFilters.GetSize());
            Int32 i;
            List<AutoPtr<PackageParser::ActivityIntentInfo> >::Iterator ait;
            for (ait = intentFilters.Begin(), i = 0; ait != intentFilters.End(); ++ait, ++i) {
                array->Set(i, *ait);
            }
            listCut.PushBack(array);
        }
    }
    AutoPtr<List<AutoPtr<IResolveInfo> > > result =
            Super::QueryIntentFromList(intent, resolvedType, defaultOnly, &listCut, userId);
    return result;
}

void CPackageManagerService::ActivityIntentResolver::AddActivity(
    /* [in] */ PackageParser::Activity* a,
    /* [in] */ const String& type)
{
    assert(a != NULL);
    AutoPtr<IApplicationInfo> appInfo;
    a->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Boolean systemApp = CPackageManagerService::IsSystemApp(appInfo);
    mActivities[a->GetComponentName()] = a;
//     if (DEBUG_SHOW_INFO)
//         Log.v(
//         TAG, "  " + type + " " +
//         (a.info.nonLocalizedLabel != null ? a.info.nonLocalizedLabel : a.info.name) + ":");
//     if (DEBUG_SHOW_INFO)
//         Log.v(TAG, "    Class=" + a.info.name);
    List<AutoPtr<PackageParser::ActivityIntentInfo> >::Iterator it;
    for (it = a->mIntents.Begin(); it != a->mIntents.End(); ++it) {
        AutoPtr<PackageParser::ActivityIntentInfo> intent = *it;
        if (!systemApp && intent->GetPriority() > 0 && String("activity").Equals(type)) {
            intent->SetPriority(0);
//             Log.w(TAG, "Package " + a.info.applicationInfo.packageName + " has activity "
//                     + a.className + " with priority > 0, forcing to 0");
        }
//         if (DEBUG_SHOW_INFO) {
//             Log.v(TAG, "    IntentFilter:");
//             intent.dump(new LogPrinter(Log.VERBOSE, TAG), "      ");
//         }
//         if (!intent.debugCheck()) {
//             Log.w(TAG, "==> For Activity " + a.info.name);
//         }
        AddFilter(intent);
    }
}

void CPackageManagerService::ActivityIntentResolver::RemoveActivity(
    /* [in] */ PackageParser::Activity* a,
    /* [in] */ const String& type)
{
    mActivities.Erase(a->GetComponentName());
//     if (DEBUG_SHOW_INFO) {
//         Log.v(TAG, "  " + type + " "
//                 + (a.info.nonLocalizedLabel != null ? a.info.nonLocalizedLabel
//                         : a.info.name) + ":");
//         Log.v(TAG, "    Class=" + a.info.name);
//     }
    List<AutoPtr<PackageParser::ActivityIntentInfo> >::Iterator it;
    for (it = a->mIntents.Begin(); it != a->mIntents.End(); ++it) {
//        if (DEBUG_SHOW_INFO) {
//            Log.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Log.VERBOSE, TAG), "      ");
//        }
        RemoveFilter(*it);
    }
}

Boolean CPackageManagerService::ActivityIntentResolver::AllowFilterResult(
    /* [in] */ PackageParser::ActivityIntentInfo* filter,
    /* [in] */ List<AutoPtr<IResolveInfo> >* dest)
{
    AutoPtr<IActivityInfo> filterAi = filter->mActivity->mInfo;
    String filterAiName;
    filterAi->GetName(&filterAiName);
    String filterAiPackageName;
    filterAi->GetPackageName(&filterAiPackageName);
    List<AutoPtr<IResolveInfo> >::ReverseIterator iter = dest->RBegin();
    for (; iter != dest->REnd(); ++iter) {
        AutoPtr<IActivityInfo> destAi;
        (*iter)->GetActivityInfo((IActivityInfo**)&destAi);
        String destAiName;
        destAi->GetName(&destAiName);
        String destAiPackageName;
        destAi->GetPackageName(&destAiPackageName);
        if (destAiName == filterAiName
            && destAiPackageName == filterAiPackageName) {
            return FALSE;
        }
    }
    return TRUE;
}

AutoPtr< ArrayOf<PackageParser::ActivityIntentInfo*> >
CPackageManagerService::ActivityIntentResolver::NewArray(
    /* [in] */ Int32 size)
{
    return ArrayOf<PackageParser::ActivityIntentInfo*>::Alloc(size);
}

Boolean CPackageManagerService::ActivityIntentResolver::IsFilterStopped(
    /* [in] */ PackageParser::ActivityIntentInfo* filter,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return TRUE;
    AutoPtr<PackageParser::Package> p = filter->mActivity->mOwner;
    if (p != NULL) {
        AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
        if (ps != NULL) {
            // System apps are never considered stopped for purposes of
            // filtering, because there may be no way for the user to
            // actually re-launch them.
            return (ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0
                    && ps->GetStopped(userId);
        }
    }
    return FALSE;
}

Boolean CPackageManagerService::ActivityIntentResolver::IsPackageForFilter(
    /* [in] */ const String& packageName,
    /* [in] */ PackageParser::ActivityIntentInfo* info)
{
    return packageName.Equals(info->mActivity->mOwner->mPackageName);
}

AutoPtr<IResolveInfo> CPackageManagerService::ActivityIntentResolver::NewResult(
    /* [in] */ PackageParser::ActivityIntentInfo* info,
    /* [in] */ Int32 match,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;

    if (!mHost->mSettings->IsEnabledLPr(info->mActivity->mInfo, mFlags, userId)) {
        return NULL;
    }

    AutoPtr<PackageParser::Activity> activity = info->mActivity;

    AutoPtr<IApplicationInfo> aInfo;
    activity->mInfo->GetApplicationInfo((IApplicationInfo**)&aInfo);
    Int32 flags;
    aInfo->GetFlags(&flags);
    if (mHost->mSafeMode && (flags & IApplicationInfo::FLAG_SYSTEM) == 0) {
        return NULL;
    }

    AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(activity->mOwner->mExtras->Probe(EIID_PackageSetting));
    if (ps == NULL) {
        return NULL;
    }
    AutoPtr<IActivityInfo> ai = PackageParser::GenerateActivityInfo(activity, mFlags,
            ps->ReadUserState(userId), userId);
    if (ai == NULL) {
        return NULL;
    }
    AutoPtr<IResolveInfo> res;
    CResolveInfo::New((IResolveInfo**)&res);
    res->SetActivityInfo(ai);

    if ((mFlags & IPackageManager::GET_RESOLVED_FILTER) != 0) {
        res->SetFilter((IIntentFilter*)info->Probe(EIID_IIntentFilter));
    }
    res->SetPriority(info->GetPriority());
    res->SetPreferredOrder(activity->mOwner->mPreferredOrder);
    // //System.out.println("Result: " + res.activityInfo.className +
    // //                   " = " + res.priority);
    res->SetMatch(match);
    res->SetIsDefault(info->mHasDefault);
    res->SetLabelRes(info->mLabelRes);
    res->SetNonLocalizedLabel(info->mNonLocalizedLabel);
    if (userNeedsBadging(userId)) {
        res->SetNoResourceId(TRUE);
    }
    else {
        res->SetIcon(info->mIcon);
    }
    AutoPtr<IApplicationInfo> appInfo;
    ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
    res->SetSystem(CPackageManagerService::IsSystemApp(appInfo));
    return res;
}

void CPackageManagerService::ActivityIntentResolver::SortResults(
    /* [in] */ List<AutoPtr<IResolveInfo> >* results)
{
//      Collections.sort(results, mResolvePrioritySorter);
}

void CPackageManagerService::ActivityIntentResolver::DumpFilter(
    /* [in] */ IPrintWriter* out,
    /* [in] */ const String& prefix,
    /* [in] */ PackageParser::ActivityIntentInfo* filter)
{
    out->Print(prefix);
    out->Print((Int32)filter->mActivity);
    out->Print(' ');
    out->Print(filter->mActivity->PrintComponentShortName(out));
    out->Print(String(" filter "));
    out->Println((Int32)filter);
}


//==============================================================================
//                  CPackageManagerService::ServiceIntentResolver
//==============================================================================

CPackageManagerService::ServiceIntentResolver::ServiceIntentResolver(
    /* [in] */ CPackageManagerService* owner)
    : mServices(11)
    , mFlags(0)
    , mHost(owner)
{}

AutoPtr<List<AutoPtr<IResolveInfo> > >
CPackageManagerService::ServiceIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ Int32 userId)
{
    mFlags = defaultOnly ? IPackageManager::MATCH_DEFAULT_ONLY : 0;
    return Super::QueryIntent(intent, resolvedType, defaultOnly, userId);
}

AutoPtr<List<AutoPtr<IResolveInfo> > >
CPackageManagerService::ServiceIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
            (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0, userId);
}

AutoPtr<List< AutoPtr<IResolveInfo> > >
CPackageManagerService::ServiceIntentResolver::QueryIntentForPackage(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<AutoPtr<PackageParser::Service> >* packageServices,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    if (packageServices == NULL) {
        return NULL;
    }
    mFlags = flags;
    Boolean defaultOnly = (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0;
    List<AutoPtr< ArrayOf<PackageParser::ServiceIntentInfo*> > > listCut;

    List<AutoPtr<PackageParser::ServiceIntentInfo> > intentFilters;
    List<AutoPtr<PackageParser::Service> >::Iterator it;
    for (it = packageServices->Begin(); it != packageServices->End(); ++it) {
        List< AutoPtr<PackageParser::ServiceIntentInfo> >& intentFilters = (*it)->mIntents;
        if (!intentFilters.IsEmpty()) {
            AutoPtr< ArrayOf<PackageParser::ServiceIntentInfo*> > array =
                    ArrayOf<PackageParser::ServiceIntentInfo*>::Alloc(intentFilters.GetSize());
            Int32 i;
            List<AutoPtr<PackageParser::ServiceIntentInfo> >::Iterator ait;
            for (ait = intentFilters.Begin(), i = 0; ait != intentFilters.End(); ++ait, ++i) {
                array->Set(i, *ait);
            }
            listCut.PushBack(array);
        }
    }
    AutoPtr<List<AutoPtr<IResolveInfo> > > result =
            Super::QueryIntentFromList(intent, resolvedType, defaultOnly, &listCut, userId);
    listCut.Clear();
    return result;
}

void CPackageManagerService::ServiceIntentResolver::AddService(
    /* [in] */ PackageParser::Service* s)
{
    AutoPtr<IComponentName> cn = s->GetComponentName();
    mServices[cn] = s;

//    if (DEBUG_SHOW_INFO) {
//        Log.v(TAG, "  "
//                + (s.info.nonLocalizedLabel != null
//                ? s.info.nonLocalizedLabel : s.info.name) + ":");
//        Log.v(TAG, "    Class=" + s.info.name);
//    }
    List< AutoPtr<PackageParser::ServiceIntentInfo> >::Iterator it;
    for (it = s->mIntents.Begin(); it != s->mIntents.End(); ++it) {
//        if (DEBUG_SHOW_INFO) {
//            Log.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Log.VERBOSE, TAG), "      ");
//        }
//        if (!intent.debugCheck()) {
//            Log.w(TAG, "==> For Service " + s.info.name);
//        }
        AddFilter(*it);
    }
}

void CPackageManagerService::ServiceIntentResolver::RemoveService(
    /* [in] */ PackageParser::Service* s)
{
    mServices.Erase(s->GetComponentName());
//    if (DEBUG_SHOW_INFO) {
//        Log.v(TAG, "  " + (s.info.nonLocalizedLabel != null
//                ? s.info.nonLocalizedLabel : s.info.name) + ":");
//        Log.v(TAG, "    Class=" + s.info.name);
//    }
    List<AutoPtr<PackageParser::ServiceIntentInfo> >::Iterator it;
    for (it = s->mIntents.Begin(); it != s->mIntents.End(); ++it) {
//        if (DEBUG_SHOW_INFO) {
//            Log.v(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Log.VERBOSE, TAG), "      ");
//        }
        RemoveFilter(*it);
    }
}

Boolean CPackageManagerService::ServiceIntentResolver::AllowFilterResult(
    /* [in] */ PackageParser::ServiceIntentInfo* filter,
    /* [in] */ List<AutoPtr<IResolveInfo> >* dest)
{
    AutoPtr<IServiceInfo> filterSi = filter->mService->mInfo;
    String filterSiName;
    filterSi->GetName(&filterSiName);
    String filterSiPackageName;
    filterSi->GetPackageName(&filterSiPackageName);
    List< AutoPtr<IResolveInfo> >::ReverseIterator iter = dest->RBegin();
    for (; iter != dest->REnd(); ++iter) {
        AutoPtr<IServiceInfo> destSi;
        (*iter)->GetServiceInfo((IServiceInfo**)&destSi);
        String destSiName;
        destSi->GetName(&destSiName);
        String destSiPackageName;
        destSi->GetPackageName(&destSiPackageName);
        if (destSiName == filterSiName
            && destSiPackageName == filterSiPackageName) {
            return FALSE;
        }
    }

    return TRUE;
}

AutoPtr< ArrayOf<PackageParser::ServiceIntentInfo*> >
CPackageManagerService::ServiceIntentResolver::NewArray(
    /* [in] */ Int32 size)
{
    return ArrayOf<PackageParser::ServiceIntentInfo*>::Alloc(size);
}

Boolean CPackageManagerService::ServiceIntentResolver::IsFilterStopped(
    /* [in] */ PackageParser::ServiceIntentInfo* filter,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return TRUE;
    AutoPtr<PackageParser::Package> p = filter->mService->mOwner;
    if (p != NULL) {
        AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
        if (ps != NULL) {
            // System apps are never considered stopped for purposes of
            // filtering, because there may be no way for the user to
            // actually re-launch them.
            return (ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0
                    && ps->GetStopped(userId);
        }
    }
    return FALSE;
}

Boolean CPackageManagerService::ServiceIntentResolver::IsPackageForFilter(
    /* [in] */ const String& packageName,
    /* [in] */ PackageParser::ServiceIntentInfo* info)
{
    return packageName.Equals(info->mService->mOwner->mPackageName);
}

AutoPtr<IResolveInfo> CPackageManagerService::ServiceIntentResolver::NewResult(
    /* [in] */ PackageParser::ServiceIntentInfo* info,
    /* [in] */ Int32 match,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    if (!mHost->mSettings->IsEnabledLPr(info->mService->mInfo, mFlags, userId)) {
        return NULL;
    }
    AutoPtr<PackageParser::Service> service = info->mService;
    AutoPtr<IApplicationInfo> srvAppInfo;
    service->mInfo->GetApplicationInfo((IApplicationInfo**)&srvAppInfo);
    Int32 srvFlags;
    srvAppInfo->GetFlags(&srvFlags);
    if (mHost->mSafeMode && (srvFlags & IApplicationInfo::FLAG_SYSTEM) == 0) {
        return NULL;
    }
    AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(service->mOwner->mExtras->Probe(EIID_PackageSetting));
    if (ps == NULL) {
        return NULL;
    }
    AutoPtr<IServiceInfo> si = PackageParser::GenerateServiceInfo(service, mFlags,
            ps->ReadUserState(userId), userId);
    if (si == NULL) {
        return NULL;
    }
    AutoPtr<IResolveInfo> res;
    CResolveInfo::New((IResolveInfo**)&res);
    res->SetServiceInfo(si);
    if ((mFlags & IPackageManager::GET_RESOLVED_FILTER) != 0) {
        res->SetFilter((IIntentFilter*)info->Probe(EIID_IIntentFilter));
    }
    res->SetPriority(info->GetPriority());
    res->SetPreferredOrder(service->mOwner->mPreferredOrder);
    //System.out.println("Result: " + res.activityInfo.className +
    //                   " = " + res.priority);
    res->SetMatch(match);
    res->SetIsDefault(info->mHasDefault);
    res->SetLabelRes(info->mLabelRes);
    res->SetNonLocalizedLabel(info->mNonLocalizedLabel);
    res->SetIcon(info->mIcon);
    srvAppInfo = NULL;
    si->GetApplicationInfo((IApplicationInfo**)&srvAppInfo);
    res->SetSystem(CPackageManagerService::IsSystemApp(srvAppInfo));
    return res;
}

void CPackageManagerService::ServiceIntentResolver::SortResults(
    /* [in] */ List< AutoPtr<IResolveInfo> >* results)
{
//      Collections.sort(results, mResolvePrioritySorter);
}

void CPackageManagerService::ServiceIntentResolver::DumpFilter(
    /* [in] */ IPrintWriter* out,
    /* [in] */ const String& prefix,
    /* [in] */ PackageParser::ServiceIntentInfo* filter)
{
    out->Print(prefix);
    out->Print((Int32)filter->mService);
    out->Print(' ');
    out->Print(filter->mService->PrintComponentShortName(out));
    out->Print(String(" filter "));
    out->Println((Int32)filter);
}


//==============================================================================
//                  CPackageManagerService::ProviderIntentResolver
//==============================================================================

CPackageManagerService::ProviderIntentResolver::ProviderIntentResolver(
    /* [in] */ CPackageManagerService* owner)
    : mFlags(0)
    , mHost(owner)
{}

AutoPtr<List<AutoPtr<IResolveInfo> > >
CPackageManagerService::ProviderIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ Int32 userId)
{
    mFlags = defaultOnly ? IPackageManager::MATCH_DEFAULT_ONLY : 0;
    return super::QueryIntent(intent, resolvedType, defaultOnly, userId);
}

AutoPtr<List<AutoPtr<IResolveInfo> > >
CPackageManagerService::ProviderIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
            (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0, userId);
}

AutoPtr<List< AutoPtr<IResolveInfo> > >
CPackageManagerService::ProviderIntentResolver::QueryIntentForPackage(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<AutoPtr<PackageParser::Service> >* packageServices,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    if (packageServices == NULL) return NULL;
    mFlags = flags;
    Boolean defaultOnly = (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0;
    List<AutoPtr< ArrayOf<PackageParser::ProviderIntentInfo*> > > listCut;

    List<AutoPtr<PackageParser::ProviderIntentInfo> > intentFilters;
    List<AutoPtr<PackageParser::Service> >::Iterator it;
    for (it = packageServices->Begin(); it != packageServices->End(); ++it) {
        intentFilters = (*it)->mIntents;
        if (!intentFilters.IsEmpty()) {
            AutoPtr< ArrayOf<PackageParser::ProviderIntentInfo*> > array =
                    ArrayOf<PackageParser::ProviderIntentInfo*>::Alloc(intentFilters.GetSize());
            Int32 i;
            List<AutoPtr<PackageParser::ProviderIntentInfo> >::Iterator ait;
            for (ait = intentFilters.Begin(), i = 0; ait != intentFilters.End(); ++ait, ++i) {
                array->Set(i, *ait);
            }
            listCut.PushBack(array);
        }
    }
    return Super::QueryIntentFromList(intent, resolvedType, defaultOnly, &listCut, userId);
}

void CPackageManagerService::ProviderIntentResolver::AddProvider(
    /* [in] */ PackageParser::Provider* p)
{
    if (mHost->mProviders->ContainsKey(p->GetComponentName())) {
        // Slogger::W(TAG, "Provider " + p.getComponentName() + " already defined; ignoring");
        return;
    }

    mProviders->Put(p->GetComponentName(), p);
    if (DEBUG_SHOW_INFO) {
        AutoPtr<ICharSequence> cs;
        IPackageItemInfo::Probe(p->mInfo)->GetNonLocalizedLabel((ICharSequence**)&cs);
        String str;
        if (cs) {
            cs->ToString(&str);
        }
        else {
            IPackageItemInfo::Probe(p->mInfo)->GetName(&str);
        }
        Logger::V(TAG, "  %s:", str.string());
        IPackageItemInfo::Probe(p->mInfo)->GetName(&str);
        Logger::V(TAG, "    Class=%s", str.string());
    }
    List<AutoPtr<PackageParser::ProviderIntentInfo> >::Iterator it;
    for (it = p->mIntents.Begin(); it != p->mIntents.End(); ++it) {
        // if (DEBUG_SHOW_INFO) {
        //     Log.v(TAG, "    IntentFilter:");
        //     intent.dump(new LogPrinter(Log.VERBOSE, TAG), "      ");
        // }
        // if (!intent.debugCheck()) {
        //     Log.w(TAG, "==> For Provider " + p.info.name);
        // }
        AddFilter(*it);
    }
}

void CPackageManagerService::ProviderIntentResolver::RemoveProvider(
    /* [in] */ PackageParser::Provider* p)
{
    mProviders.Erase(p->GetComponentName());
    // if (DEBUG_SHOW_INFO) {
    //     Log.v(TAG, "  " + (p.info.nonLocalizedLabel != null
    //             ? p.info.nonLocalizedLabel : p.info.name) + ":");
    //     Log.v(TAG, "    Class=" + p.info.name);
    // }
    List<AutoPtr<PackageParser::ProviderIntentInfo> >::Iterator it;
    for (it = p->mIntents.Begin(); it != p->mIntents.End(); ++it) {
        // if (DEBUG_SHOW_INFO) {
        //     Log.v(TAG, "    IntentFilter:");
        //     intent.dump(new LogPrinter(Log.VERBOSE, TAG), "      ");
        // }
        RemoveFilter(*it);
    }
}

Boolean CPackageManagerService::ProviderIntentResolver::AllowFilterResult(
    /* [in] */ PackageParser::ProviderIntentInfo* filter,
    /* [in] */ List<AutoPtr<IResolveInfo> >* dest)
{
    AutoPtr<IProviderInfo> filterPi = filter->mProvider->mInfo;
    List<AutoPtr<IResolveInfo> >::ReverseIterator rit = dest->RBegin();
    for (; rit != dest->REnd(); ++rit) {
        AutoPtr<IProviderInfo> destPi;
        (*rit)->GetProviderInfo((IProviderInfo**)&destPi);
        String destPiName, filterPiName;
        destPi->GetName(&destPiName);
        filterPi->GetName(&filterPiName);
        String destPiPkgName, filterPiPkgName;
        destPi->GetPackageName(&destPiPkgName);
        filterPi->GetPackageName(&filterPiPkgName);
        if (destPiName == filterPiName && destPiPkgName == filterPiPkgName) {
            return FALSE;
        }
    }
    return TRUE;
}

AutoPtr< ArrayOf<PackageParser::ProviderIntentInfo*> >
CPackageManagerService::ProviderIntentResolver::NewArray(
    /* [in] */ Int32 size)
{
    return ArrayOf<PackageParser::ProviderIntentInfo*>::Alloc(size);
}

Boolean CPackageManagerService::ProviderIntentResolver::IsFilterStopped(
    /* [in] */ PackageParser::ProviderIntentInfo* filter,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return TRUE;
    AutoPtr<PackageParser::Package> p = filter->mProvider->mOwner;
    if (p != NULL) {
        AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
        if (ps != NULL) {
            // System apps are never considered stopped for purposes of
            // filtering, because there may be no way for the user to
            // actually re-launch them.
            return (ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0
                    && ps->GetStopped(userId);
        }
    }
    return FALSE;
}

Boolean CPackageManagerService::ProviderIntentResolver::IsPackageForFilter(
    /* [in] */ const String& packageName,
    /* [in] */ PackageParser::ProviderIntentInfo* info)
{
    return packageName.Equals(info->mProvider->mOwner->mPackageName);
}

AutoPtr<IResolveInfo> CPackageManagerService::ProviderIntentResolver::NewResult(
    /* [in] */ PackageParser::ProviderIntentInfo* info,
    /* [in] */ Int32 match,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    if (!mHost->mSettings->IsEnabledLPr(info->mProvider->mInfo, mFlags, userId)) {
        return NULL;
    }
    AutoPtr<PackageParser::Provider> provider = info->mProvider;
    AutoPtr<IApplicationInfo> appInfo;
    provider->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 flags;
    if (mHost->mSafeMode && (appInfo->GetFlags(&flags), (flags & IApplicationInfo::FLAG_SYSTEM) == 0) {
        return NULL;
    }
    AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(provider->mOwner->mExtras->Probe(EIID_PackageSetting));
    if (ps == NULL) {
        return NULL;
    }
    AutoPtr<IProviderInfo> pi = PackageParser::GenerateProviderInfo(provider, mFlags,
            ps->ReadUserState(userId), userId);
    if (pi == NULL) {
        return NULL;
    }
    AutoPtr<IResolveInfo> res;
    CResolveInfo::New((IResolveInfo**)&res);
    res->SetProviderInfo(pi);
    if ((mFlags & IPackageManager::GET_RESOLVED_FILTER) != 0) {
        res->SetFilter((IIntentFilter*)filter->Probe(EIID_IIntentFilter));
    }
    res->SetPriority(info->GetPriority());
    res->SetPreferredOrder(provider->mOwner->mPreferredOrder);
    res->SetMatch(match);
    res->SetIsDefault(info->mHasDefault);
    res->SetLabelRes(info->mLabelRes);
    res->SetNonLocalizedLabel(info->mNonLocalizedLabel);
    res->SetIcon(info->mIcon);
    appInfo = NULL;
    pi->GetApplicationInfo((IApplicationInfo**)&appInfo);
    res->SetSystem(CPackageManagerService::IsSystemApp(srvAppInfo));
    return res;
}

void CPackageManagerService::ProviderIntentResolver::SortResults(
    /* [in] */ List< AutoPtr<IResolveInfo> >* results)
{
    // Collections.sort(results, mResolvePrioritySorter);
}

void CPackageManagerService::ProviderIntentResolver::DumpFilter(
    /* [in] */ IPrintWriter* out,
    /* [in] */ const String& prefix,
    /* [in] */ PackageParser::ProviderIntentInfo* filter)
{
    out->Print(prefix);
    out->Print((Int32)filter->mProvider);
    out->Print(' ');
    out->Print(filter->mService->PrintComponentShortName(out));
    out->Print(String(" filter "));
    out->Println((Int32)filter);
}


//==============================================================================
//                  CPackageManagerService::ResolvePrioritySorter
//==============================================================================

CAR_INTERFACE_IMPL(CPackageManagerService::ResolvePrioritySorter, Object, IComparator)

ECode CPackageManagerService::ResolvePrioritySorter::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IResolveInfo> r1 = IResolveInfo::Probe(lhs);
    AutoPtr<IResolveInfo> r2 = IResolveInfo::Probe(rhs);
    Int32 v1, v2;
    r1->GetPriority(&v1);
    r2->GetPriority(&v2);
    //System.out.println("Comparing: q1=" + q1 + " q2=" + q2);
    if (v1 != v2) {
        *result = (v1 > v2) ? -1 : 1;
        return NOERROR;
    }
    r1->GetPreferredOrder(&v1);
    r2->GetPreferredOrder(&v2);
    if (v1 != v2) {
        *result = (v1 > v2) ? -1 : 1;
        return NOERROR;
    }
    Boolean b1, b2;
    r1->GetIsDefault(&b1);
    r2->GetIsDefault(&b2);
    if (b1 != b2) {
        *result = b1 ? -1 : 1;
        return NOERROR;
    }
    r1->GetMatch(&v1);
    r2->GetMatch(&v2);
    //System.out.println("Comparing: m1=" + m1 + " m2=" + m2);
    if (v1 != v2) {
        *result = (v1 > v2) ? -1 : 1;
        return NOERROR;
    }
    r1->GetSystem(&b1);
    r2->GetSystem(&b2);
    if (b1 != b2) {
        *result = b1 ? -1 : 1;
        return NOERROR;
    }
    *result = 0;
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::ProviderInitOrderSorter
//==============================================================================

CAR_INTERFACE_IMPL(CPackageManagerService::ProviderInitOrderSorter, Object, IComparator)

ECode CPackageManagerService::ProviderInitOrderSorter::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IProviderInfo> p1 = IProviderInfo::Probe(lhs);
    AutoPtr<IProviderInfo> p2 = IProviderInfo::Probe(rhs);
    Int32 v1, v2;
    p1->GetInitOrder(&v1);
    p2->GetInitOrder(&v2);
    *result = (v1 > v2) ? -1 : ((v1 < v2) ? 1 : 0);
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::ProcessPendingInstallRunnable
//==============================================================================

ECode CPackageManagerService::ProcessPendingInstallRunnable::Run()
{
    mHost->mHandler->RemoveCallbacks(this);
    // Result object to be returned
    AutoPtr<PackageInstalledInfo> res = new PackageInstalledInfo();
    res->mReturnCode = currentStatus;
    res->mUid = -1;
    res->mPkg = NULL;
    res->mRemovedInfo = new PackageRemovedInfo(mHost);
    if (res->mReturnCode == IPackageManager::INSTALL_SUCCEEDED) {
        args->DoPreInstall(res->mReturnCode);
        synchronized (mHost->mInstallLock) {
            AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
            if (readBuffer == NULL) {
                Slogger::E(TAG, "Failed install package: out of memory!");
                return;
            }
            InstallPackageLI(args, TRUE, res, readBuffer);
        }
        args->DoPostInstall(res->mReturnCode, res->mUid);
    }

    // A restore should be performed at this point if (a) the install
    // succeeded, (b) the operation is not an update, and (c) the new
    // package has not opted out of backup participation.
    Boolean update = res->mRemovedInfo->mRemovedPackage != NULL;
    Int32 flags = 0;
    if (res->mPkg != NULL) {
        res->mPkg->mApplicationInfo->GetFlags(&flags);
    }
    Boolean doRestore = !update && ((flags & IApplicationInfo::FLAG_ALLOW_BACKUP) != 0);

    // Set up the post-install work request bookkeeping.  This will be used
    // and cleaned up by the post-install event handling regardless of whether
    // there's a restore pass performed.  Token values are >= 1.
    Int32 token;
    if (mHost->mNextInstallToken < 0) mHost->mNextInstallToken = 1;
    token = mHost->mNextInstallToken++;

    AutoPtr<PostInstallData> data = new PostInstallData(args, res);
    mHost->mRunningInstalls[token] = data;
    if (DEBUG_INSTALL)
        Logger::V(TAG, "+ starting restore round-trip %d", token);

    if (res->mReturnCode == IPackageManager::INSTALL_SUCCEEDED && doRestore) {
        // Pass responsibility to the Backup Manager.  It will perform a
        // restore if appropriate, then pass responsibility back to the
        // Package Manager to run the post-install observer callbacks
        // and broadcasts.
        AutoPtr<IIBackupManager> bm = IIBackupManager::Probe(
                ServiceManager::GetService(IContext::BACKUP_SERVICE));
        if (bm != NULL) {
            if (DEBUG_INSTALL)
                Logger::V(TAG, "token %d to BM for possible restore", token);

            String packageName;
            res->mPkg->mApplicationInfo->GetPackageName(&packageName);
            if (FAILED(bm->RestoreAtInstall(packageName, token))) {
                Slogger::E(TAG, "Exception trying to enqueue restore");
                doRestore = FALSE;
            }
        }
        else {
            Slogger::E(TAG, "Backup Manager not found!");
            doRestore = FALSE;
        }
    }

    if (!doRestore) {
        // No restore possible, or the Backup Manager was mysteriously not
        // available -- just fire the post-install work request directly.
        if (DEBUG_INSTALL)
            Logger::V(TAG, "No restore - queue post-install for %d", token);

        AutoPtr<IMessage> msg;
        mHost->mHandler->ObtainMessage(POST_INSTALL, (IMessage**)&msg);
        msg->SetArg1(token);
        Boolean result;
        mHost->mHandler->SendMessage(msg, &result);
    }
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::HandlerParams
//==============================================================================

const Int32 CPackageManagerService::HandlerParams::MAX_RETRIES;

AutoPtr<IUserHandle> CPackageManagerService::HandlerParams::GetUser()
{
    return mUser;
}

Boolean CPackageManagerService::HandlerParams::StartCopy()
{
    Boolean res;
//      try {
    if (DEBUG_INSTALL) Slogger::I(TAG, "startCopy %p: %p", mUser.Get(), this);

    if (++mRetries > MAX_RETRIES) {
        Slogger::W(TAG, "Failed to invoke remote methods on default container service. Giving up");
        Boolean result;
        mHost->mHandler->SendEmptyMessage(CPackageManagerService::MCS_GIVE_UP, &result);
        HandleServiceError();
        return FALSE;
    }
    else {
        ECode ec = HandleStartCopy();
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            if (DEBUG_INSTALL)
                Slogger::I(TAG, "Posting install MCS_RECONNECT");

            Boolean result;
            mHost->mHandler->SendEmptyMessage(CPackageManagerService::MCS_RECONNECT, &result);
            res = FALSE;
        }
        else if (FAILED(ec)) {
            Slogger::E(TAG, "HandleStartCopy failed ec = 0x%0x", ec);
            res = FALSE;
        }
        else {
            res = TRUE;
        }
    }

    HandleReturnCode();
    return res;
}

void CPackageManagerService::HandlerParams::ServiceError()
{
    if (DEBUG_INSTALL) Slogger::I(TAG, "serviceError");
    HandleServiceError();
    HandleReturnCode();
}


//==============================================================================
//                  CPackageManagerService::MeasureParams
//==============================================================================

CPackageManagerService::MeasureParams::MeasureParams(
    /* [in] */ IPackageStats* stats,
    /* [in] */ IUserHandle* userHandle,
    /* [in] */ IPackageStatsObserver* observer,
    /* [in] */ CPackageManagerService* owner)
    : HandlerParams(userHandle, owner)
    , mStats(stats)
    , mObserver(observer)
{}

ECode CPackageManagerService::MeasureParams::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int32 hashCode;
    system->IdentityHashCode((IObject*)this, &hashCode);
    String pkgN;
    mStats->GetPackageName(&pkgN);
    *str = String("MeasureParams{") + StringUtils::ToHexString(hashCode) + pkgN + "}";
    return NOERROR;
}

ECode CPackageManagerService::MeasureParams::HandleStartCopy()
{
    synchronized (mHost->mInstallLock) {
        String packageName;
        mStats->GetPackageName(&packageName);
        Int32 userHandle;
        mStats->GetUserHandle(&userHandle);
        mSuccess = mHost->GetPackageSizeInfoLI(packageName, userHandle, mStats);
    }

    if (mSuccess) {
        Boolean mounted;
        if (Environment::IsExternalStorageEmulated()) {
            mounted = TRUE;
        }
        else {
            String status = Environment::GetExternalStorageState(&status);
            mounted = (IEnvironment::MEDIA_MOUNTED.Equals(status)
                    || IEnvironment::MEDIA_MOUNTED_READ_ONLY.Equals(status));
        }

        if (mounted) {
            Int32 userHandle;
            mStats->GetUserHandle(&userHandle);
            AutoPtr<UserEnvironment> userEnv = new Environment::UserEnvironment(userHandle);

            String pkgN;
            mStats->GetPackageName(&pkgN);
            AutoPtr<ArrayOf<IFile*> > fs1 = userEnv->BuildExternalStorageAppCacheDirs(pkgN);
            mStats->SetExternalCacheSize(mHost->CalculateDirectorySize(mContainerService, fs1);

            AutoPtr<ArrayOf<IFile*> > fs2 = userEnv->BuildExternalStorageAppDataDirs(pkgN);
            mStats->SetExternalDataSize(CalculateDirectorySize(mContainerService, fs2);

            // Always subtract cache size, since it's a subdirectory
            Int32 size, cacheSize;
            mStats->GetExternalCodeSize(&size);
            mStats->GetExternalCacheSize(&cacheSize);
            size -= cacheSize;
            mStats->SetExternalDataSize(size);

            AutoPtr<ArrayOf<IFile*> > fs3 = userEnv->BuildExternalStorageAppMediaDirs(pkgN);
            mStats->SetExternalMediaSize(CalculateDirectorySize(mContainerService, fs3);

            AutoPtr<ArrayOf<IFile*> > fs4 = userEnv->BuildExternalStorageAppObbDirs(pkgN);
            mStats->SetExternalObbSize(CalculateDirectorySize(mContainerService, fs4);
        }
    }
    return NOERROR;
}

void CPackageManagerService::MeasureParams::HandleReturnCode()
{
    if (mObserver != NULL) {
        // try {
        if(FAILED(mObserver->OnGetStatsCompleted(mStats, mSuccess))) {
            Slogger::I(TAG, "Observer no longer exists.");
        }
        // } catch (RemoteException e) {
        //     Slog.i(TAG, "Observer no longer exists.");
        // }
    }
}

void CPackageManagerService::MeasureParams::HandleServiceError()
{
    String packageName;
    mStats->GetPackageName(&packageName);
    Slogger::E(TAG, "Could not measure application %s external storage", packageName.string());
}


//==============================================================================
//                  CPackageManagerService::OriginInfo
//==============================================================================

CPackageManagerService::OriginInfo::OriginInfo(
    /* [in] */ IFile* file,
    /* [in] */ const String& cid,
    /* [in] */ Boolean staged,
    /* [in] */ Boolean existing)
    : mFile(file)
    , mCid(cid)
    , mStaged(staged)
    , mExisting(existing)
{
    if (!cid.IsNull()) {
        mResolvedPath = PackageHelper::GetSdDir(cid);
        CFile::New(mResolvedPath, (IFile**)&mResolvedFile);
    }
    else if (file != NULL) {
        mFile->GetAbsolutePath(&mResolvedPath);
        mResolvedFile = mFile;
    }
    else {
        mResolvedPath = String(NULL);
        mResolvedFile = NULL;
    }
}

AutoPtr<OriginInfo> CPackageManagerService::OriginInfo::FromNothing()
{
    return new OriginInfo(NULL, String(NULL), FALSE, FALSE);
}

AutoPtr<OriginInfo> CPackageManagerService::OriginInfo::FromUntrustedFile(
    /* [in] */ IFile* file)
{
    return new OriginInfo(file, String(NULL), FALSE, FALSE);
}

AutoPtr<OriginInfo> CPackageManagerService::OriginInfo::FromExistingFile(
    /* [in] */ IFile* file)
{
    return new OriginInfo(file, String(NULL), FALSE, TRUE);
}

AutoPtr<OriginInfo> CPackageManagerService::OriginInfo::FromStagedFile(
    /* [in] */ IFile* file)
{
    return new OriginInfo(file, String(NULL), TRUE, FALSE);
}

AutoPtr<OriginInfo> CPackageManagerService::OriginInfo::FromStagedContainer(
    /* [in] */ const String& cid)
{
    return new OriginInfo(NULL, cid, TRUE, FALSE);
}


//==============================================================================
//                  CPackageManagerService::InstallParams::CopyBroadcastReceiver
//==============================================================================

ECode CPackageManagerService::InstallParams::CopyBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(CHECK_PENDING_VERIFICATION, (IMessage**)&msg);
    msg->SetArg1(mId);
    Boolean result;
    mHost->mHandler->SendMessageDelayed(msg, mHost->GetVerificationTimeout(), &result);

    //[Added by wanli]: Release mPendingResult, because there is a circle.
    return BroadcastReceiver::SetPendingResult(NULL);
}


//==============================================================================
//                  CPackageManagerService::InstallParams
//==============================================================================

CPackageManagerService::InstallParams::InstallParams(
    /* [in] */ OriginInfo* origin,
    /* [in] */ IIPackageInstallObserver2* observer,
    /* [in] */ Int32 installFlags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IVerificationParams* verificationParams,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& packageAbiOverride,
    /* [in] */ CPackageManagerService* owner)
    : HandlerParams(user, owner)
    , mObserver(observer)
    , mInstallFlags(installFlags)
    , mInstallerPackageName(installerPackageName)
    , mVerificationParams(verificationParams)
    , mPackageAbiOverride(packageAbiOverride)
    , mRet(0)
{}

ECode CPackageManagerService::InstallParams::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 hashCode;
    sys->IdentityHashCode((IObject*)this, &hashCode);
    String fileStr;
    IObject::Probe(mOrigin->mFile)->ToString(&fileStr);
    *str = String("InstallParams{") + StringUtils::ToHexString(hashCode)
            + " file=" + fileStr + " cid=" + mOrigin->mCid + "}";
    return NOERROR;
}

AutoPtr<IManifestDigest> CPackageManagerService::InstallParams::GetManifestDigest()
{
    if (mVerificationParams == NULL) {
        return NULL;
    }
    AutoPtr<IManifestDigest> digest;
    mVerificationParams->GetManifestDigest((IManifestDigest**)&digest);
    return digest;
}

Int32 CPackageManagerService::InstallParams::InstallLocationPolicy(
    /* [in] */ IPackageInfoLite* pkgLite)
{
    String packageName;
    pkgLite->GetPackageName(&packageName);
    Int32 installLocation;
    pkgLite->GetInstallLocation(&installLocation);
    Boolean onSd = (mInstallFlags & IPackageManager::INSTALL_EXTERNAL) != 0;
    // reader
    synchronized (mHost->mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it
                = mHost->mPackages.Find(packageName);
        if (it != mHost->mPackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg != NULL) {
            if ((mInstallFlags & IPackageManager::INSTALL_REPLACE_EXISTING) != 0) {
                // Check for downgrading.
                if ((mInstallFlags & IPackageManager::INSTALL_ALLOW_DOWNGRADE) == 0) {
                    Int32 versionCode;
                    pkgLite->GetVersionCode(&versionCode);
                    if (versionCode < pkg->mVersionCode) {
                        // Slog.w(TAG, "Can't install update of " + packageName
                        //         + " update version " + pkgLite.versionCode
                        //         + " is older than installed version "
                        //         + pkg.mVersionCode);
                        return IPackageHelper::RECOMMEND_FAILED_VERSION_DOWNGRADE;
                    }
                }
                // Check for updated system application.
                Int32 pkgFlags;
                pkg->mApplicationInfo->GetFlags(&pkgFlags);
                if ((pkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0) {
                    if (onSd) {
                        Slogger::W(TAG, "Cannot install update to system app on sdcard");
                        return IPackageHelper::RECOMMEND_FAILED_INVALID_LOCATION;
                    }
                    return IPackageHelper::RECOMMEND_INSTALL_INTERNAL;
                }
                else {
                    if (onSd) {
                        // Install flag overrides everything.
                        return IPackageHelper::RECOMMEND_INSTALL_EXTERNAL;
                    }
                    // If current upgrade specifies particular preference
                    if (installLocation == IPackageInfo::INSTALL_LOCATION_INTERNAL_ONLY) {
                        // Application explicitly specified internal.
                        return IPackageHelper::RECOMMEND_INSTALL_INTERNAL;
                    }
                    else if (installLocation == IPackageInfo::INSTALL_LOCATION_PREFER_EXTERNAL) {
                        // App explictly prefers external. Let policy decide
                    }
                    else {
                        // Prefer previous location
                        if (mHost->IsExternal(pkg)) {
                            return IPackageHelper::RECOMMEND_INSTALL_EXTERNAL;
                        }
                        return IPackageHelper::RECOMMEND_INSTALL_INTERNAL;
                    }
                }
            }
            else {
                // Invalid install. Return error code
                return IPackageHelper::RECOMMEND_FAILED_ALREADY_EXISTS;
            }
        }
    }
    // All the special cases have been taken care of.
    // Return result based on recommended install location.
    if (onSd) {
        return IPackageHelper::RECOMMEND_INSTALL_EXTERNAL;
    }
    pkgLite->GetRecommendedInstallLocation(&installLocation);
    return installLocation;
}

ECode CPackageManagerService::InstallParams::HandleStartCopy()
{
    Int32 ret = IPackageManager::INSTALL_SUCCEEDED;

    // If we're already staged, we've firmly committed to an install location
    if (mOrigin->mStaged) {
        if (mOrigin->mFile != NULL) {
            mInstallFlags |= IPackageManager::INSTALL_INTERNAL;
            mInstallFlags &= ~IPackageManager::INSTALL_EXTERNAL;
        }
        else if (!mOrigin->mCid.IsNull()) {
            installFlags |= IPackageManager::INSTALL_EXTERNAL;
            installFlags &= ~IPackageManager::INSTALL_INTERNAL;
        }
        else {
            Slogger::E(CPackageManagerService::TAG, "Invalid stage location");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    Boolean onSd = (installFlags & IPackageManager::INSTALL_EXTERNAL) != 0;
    Boolean onInt = (installFlags & IPackageManager::INSTALL_INTERNAL) != 0;

    AutoPtr<IPackageInfoLite> pkgLite;

    if (onInt && onSd) {
        // Check if both bits are set.
        Slogger::W(TAG, "Conflicting flags specified for installing on both internal and external");
        ret = IPackageManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
    }
    else {
        pkgLite = mHost->mContainerService->GetMinimalPackageInfo(mOrigin->mResolvedPath, mInstallFlags,
                mPackageAbiOverride);

        /*
         * If we have too little free space, try to free cache
         * before giving up.
         */
        Int32 recommendedInstallLocation;
        if (!mOrigin->mStaged &&
                (pkgLite->GetRecommendedInstallLocation(&recommendedInstallLocation),
                        recommendedInstallLocation == IPackageHelper::RECOMMEND_FAILED_INSUFFICIENT_STORAGE) {
            // TODO: focus freeing disk space on the target device
            AutoPtr<IStorageManagerHelper> helper;
            CStorageManagerHelper::AcquireSingleton((IStorageManagerHelper**)&helper);
            AutoPtr<IStorageManager> storage;
            helper->From(mHost->mContext, (IStorageManager**)&storage);
            Int64 lowThreshold;
            storage->GetStorageLowBytes(Environment::GetDataDirectory(), &lowThreshold);

            Int64 sizeBytes;
            mHot->mContainerService->CalculateInstalledSize(
                    mOrigin->mResolvedPath, CPackageManagerService::IsForwardLocked(), packageAbiOverride, &sizeBytes);

            if (mHost->mInstaller->FreeCache(sizeBytes + lowThreshold) >= 0) {
                mHost->mContainerService->GetMinimalPackageInfo(mOrigin->mResolvedPath,
                        mInstallFlags, mPackageAbiOverride, (IPackageInfoLite**)&pkgLite);
            }

            /*
             * The cache free must have deleted the file we
             * downloaded to install.
             *
             * TODO: fix the "freeCache" call to not delete
             *       the file we care about.
             */
            if (pkgLite->GetRecommendedInstallLocation(&recommendedInstallLocation),
                    recommendedInstallLocation == IPackageHelper::RECOMMEND_FAILED_INVALID_URI) {
                pkgLite->SetRecommendedInstallLocation(IPackageHelper::RECOMMEND_FAILED_INSUFFICIENT_STORAGE);
            }
        }
    }

    if (ret == IPackageManager::INSTALL_SUCCEEDED) {
        Int32 loc;
        pkgLite->GetRecommendedInstallLocation(&loc);
        if (loc == IPackageHelper::RECOMMEND_FAILED_INVALID_LOCATION){
            ret = IPackageManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
        }
        else if (loc == IPackageHelper::RECOMMEND_FAILED_ALREADY_EXISTS){
            ret = IPackageManager::INSTALL_FAILED_ALREADY_EXISTS;
        }
        else if (loc == IPackageHelper::RECOMMEND_FAILED_INSUFFICIENT_STORAGE){
            ret = IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
        }
        else if (loc == IPackageHelper::RECOMMEND_FAILED_INVALID_APK) {
            ret = IPackageManager::INSTALL_FAILED_INVALID_APK;
        }
        else if (loc == IPackageHelper::RECOMMEND_FAILED_INVALID_URI) {
            ret = IPackageManager::INSTALL_FAILED_INVALID_URI;
        }
        else if (loc == IPackageHelper::RECOMMEND_MEDIA_UNAVAILABLE) {
            ret = IPackageManager::INSTALL_FAILED_MEDIA_UNAVAILABLE;
        }
        else {
            // Override with defaults if needed.
            loc = InstallLocationPolicy(pkgLite);
            if (loc == IPackageHelper::RECOMMEND_FAILED_VERSION_DOWNGRADE) {
                ret = IPackageManager::INSTALL_FAILED_VERSION_DOWNGRADE;
            }
            else if (!onSd && !onInt) {
                // Override install location with flags
                if (loc == IPackageHelper::RECOMMEND_INSTALL_EXTERNAL) {
                    // Set the flag to install on external media.
                    mInstallFlags |= IPackageManager::INSTALL_EXTERNAL;
                    mInstallFlags &= ~IPackageManager::INSTALL_INTERNAL;
                }
                else {
                    // Make sure the flag for installing on external
                    // media is unset
                    mInstallFlags |= IPackageManager::INSTALL_INTERNAL;
                    mInstallFlags &= ~IPackageManager::INSTALL_EXTERNAL;
                }
            }
        }
    }

    mArgs = mHost->CreateInstallArgs(this);
    if (ret == IPackageManager::INSTALL_SUCCEEDED) {
        /*
         * ADB installs appear as UserHandle.USER_ALL, and can only be performed by
         * UserHandle.USER_OWNER, so use the package verifier for UserHandle.USER_OWNER.
         */
        Int32 userIdentifier;
        GetUser()->GetIdentifier(&userIdentifier);
        if (userIdentifier == IUserHandle::USER_ALL
                && ((mInstallFlags & IPackageManager::INSTALL_FROM_ADB) != 0)) {
            userIdentifier = IUserHandle::USER_OWNER;
        }

        /*
         * Determine if we have any installed package verifiers. If we
         * do, then we'll defer to them to verify the packages.
         */
        Int32 requiredUid = -1;
        if (mHost->mRequiredVerifierPackage != NULL) {
            mHost->GetPackageUid(mHost->mRequiredVerifierPackage, userIdentifier, &requiredUid);
        }
        if (!mOrigin->mExisting && requiredUid != -1
                && mHost->IsVerificationEnabled(userIdentifier, mInstallFlags)) {
            AutoPtr<IIntent> verification;
            CIntent::New(IIntent::ACTION_PACKAGE_NEEDS_VERIFICATION, (IIntent**)&verification);
            AutoPtr<IFile> f;
            CFile::New(mOrigin->mResolvedPath, (IFile**)&f);
            AutoPtr<IUri> u = Uri::FromFile(f);
            verification->SetDataAndType(u, CPackageManagerService::PACKAGE_MIME_TYPE);
            verification->AddFlags(IIntent::FLAG_GRANT_READ_URI_PERMISSION);

            AutoPtr<IObjectContainer> receivers;
            mHost->QueryIntentReceivers(verification,
                    PACKAGE_MIME_TYPE, IPackageManager::GET_DISABLED_COMPONENTS,
                    0 /* TODO: Which userId? */, (IObjectContainer**)&receivers);
            AutoPtr<IObjectEnumerator> enumerator;
            receivers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            AutoPtr<List< AutoPtr<IResolveInfo> > > receiverslist = new List< AutoPtr<IResolveInfo> >();
            Boolean hasNext = FALSE;
            while (enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                enumerator->Current((IInterface**)&obj);
                receiverslist->PushBack(IResolveInfo::Probe(obj));
            }

            // if (DEBUG_VERIFY) {
            //     Slogger::D(TAG, "Found %d" + receivers.size() + " verifiers for intent "
            //             + verification.toString() + " with " + pkgLite.verifiers.length
            //             + " optional verifiers");
            // }

            const Int32 verificationId = mHost->mPendingVerificationToken++;

            verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_ID, verificationId);

            verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_INSTALLER_PACKAGE,
                    mInstallerPackageName);

            verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_INSTALL_FLAGS, mInstallFlags);

            String pkgName;
            pkgLite->GetPackageName(&pkgName);
            verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_PACKAGE_NAME,
                    pkgName);

            Int32 vCode;
            pkgLite->GetVersionCode(&vCode);
            verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_VERSION_CODE,
                    vCode);

            if (mVerificationParams != NULL) {
                AutoPtr<IUri> uri;
                mVerificationParams->GetVerificationURI((IUri**)&uri);
                if (uri != NULL) {
                    verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_URI,
                        IParcelable::Probe(uri));
                }
                uri = NULL;
                mVerificationParams->GetOriginatingURI((IUri**)&uri);
                if (uri != NULL) {
                    verification->PutExtra(IIntent::EXTRA_ORIGINATING_URI,
                            IParcelable::Probe(uri));
                }
                uri = NULL;
                mVerificationParams->GetReferrer((IUri**)&uri);
                if (uri != NULL) {
                    verification->PutExtra(IIntent::EXTRA_REFERRER,
                            IParcelable::Probe(uri));
                }
                Int32 uid = 0;
                mVerificationParams->GetOriginatingUid(&uid);
                if (uid >= 0) {
                    verification->PutExtra(IIntent::EXTRA_ORIGINATING_UID,
                            uid);
                }
                uid = 0;
                mVerificationParams->GetInstallerUid(&uid);
                if (uid >= 0) {
                    verification->PutExtra(IPackageManager::EXTRA_VERIFICATION_INSTALLER_UID,
                            uid);
                }
            }

            AutoPtr<PackageVerificationState> verificationState = new PackageVerificationState(
                    requiredUid, mArgs);

            mHost->mPendingVerification[verificationId] = verificationState;

            AutoPtr<List< AutoPtr<IComponentName> > > sufficientVerifiers = mHost->MatchVerifiers(pkgLite,
                    receiverslist, verificationState);

            /*
             * If any sufficient verifiers were listed in the package
             * manifest, attempt to ask them.
             */
            if (sufficientVerifiers != NULL) {
                List< AutoPtr<IComponentName> >::Iterator it = sufficientVerifiers->Begin();
                if (it == sufficientVerifiers->End()) {
                    Slogger::I(TAG, "Additional verifiers required, but none installed.");
                    ret = IPackageManager::INSTALL_FAILED_VERIFICATION_FAILURE;
                }
                else {
                    for (; it != sufficientVerifiers->End(); ++it) {
                        AutoPtr<IComponentName> verifierComponent = *it;

                        AutoPtr<IIntent> sufficientIntent;
                        CIntent::New(verification, (IIntent**)&sufficientIntent);
                        sufficientIntent->SetComponent(verifierComponent);

                        mHost->mContext->SendBroadcastAsUser(sufficientIntent, GetUser());
                    }
                }
            }

            AutoPtr<IComponentName> requiredVerifierComponent = mHost->MatchComponentForVerifier(
                    mHost->mRequiredVerifierPackage, receiverslist);
            if (ret == IPackageManager::INSTALL_SUCCEEDED
                    && mHost->mRequiredVerifierPackage != NULL) {
                /*
                 * Send the intent to the required verification agent,
                 * but only start the verification timeout after the
                 * target BroadcastReceivers have run.
                 */
                verification->SetComponent(requiredVerifierComponent);
                AutoPtr<IBroadcastReceiver> receiver = new CopyBroadcastReceiver(mHost, verificationId);
                mHost->mContext->SendOrderedBroadcastAsUser(verification, GetUser(),
                        Elastos::Droid::Manifest::permission::PACKAGE_VERIFICATION_AGENT,
                        receiver, NULL, 0, String(NULL), NULL);

                /*
                 * We don't want the copy to proceed until verification
                 * succeeds, so null out this field.
                 */
                mArgs = NULL;
            }
        } else {
            /*
             * No package verification is enabled, so immediately start
             * the remote call to initiate copy using temporary file.
             */
            mArgs->CopyPkg(mHost->mContainerService, TRUE, &ret);
        }
    }

    mRet = ret;
    return NOERROR;
}

void CPackageManagerService::InstallParams::HandleReturnCode()
{
    // If mArgs is null, then MCS couldn't be reached. When it
    // reconnects, it will try again to install. At that point, this
    // will succeed.
    if (mArgs != NULL) {
        mHost->ProcessPendingInstall(mArgs, mRet);
    }
}

void CPackageManagerService::InstallParams::HandleServiceError()
{
    mArgs = mHost->CreateInstallArgs(this);
    mRet = IPackageManager::INSTALL_FAILED_INTERNAL_ERROR;
}

Boolean CPackageManagerService::InstallParams::IsForwardLocked()
{
    return (mInstallFlags & IPackageManager::INSTALL_FORWARD_LOCK) != 0;
}

AutoPtr<IUri> CPackageManagerService::InstallParams::GetPackageUri()
{
    if (mTempPackage != NULL) {
        AutoPtr<IUri> packageUri;
        ASSERT_SUCCEEDED(Uri::FromFile(mTempPackage, (IUri**)&packageUri));
        return packageUri;
    }
    else {
        return mPackageURI;
    }
}


//==============================================================================
//                  CPackageManagerService::InstallArgs
//==============================================================================

CPackageManagerService::InstallArgs::InstallArgs(
    /* [in] */ OriginInfo* origin,
    /* [in] */ IPackageInstallObserver2* observer,
    /* [in] */ Int32 installFlags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IManifestDigest* manifestDigest,
    /* [in] */ IUserHandle* user,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ const String& abiOverride,
    /* [in] */ CPackageManagerService* owner)
    : mOrigin(origin)
    , mObserver(observer)
    , mInstallFlags(installFlags)
    , mInstallerPackageName(installerPackageName)
    , mManifestDigest(manifestDigest)
    , mUser(user)
    , mAbiOverride(abiOverride)
    , mInstructionSets(instructionSets)
    , mHost(owner)
    , mIsEpk(FALSE)
{
    // for epk
    if (packageURI != NULL) {
        String path;
        packageURI->GetPath(&path);
        if (path.EndWith(".epk"))
            mIsEpk = TRUE;
    }
}

CPackageManagerService::InstallArgs::Init(
    /* [in] */ OriginInfo* origin,
    /* [in] */ IPackageInstallObserver2* observer,
    /* [in] */ Int32 installFlags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IManifestDigest* manifestDigest,
    /* [in] */ IUserHandle* user,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ const String& abiOverride,
    /* [in] */ CPackageManagerService* owner)
{
    mOrigin = origin;
    mObserver = observer;
    mInstallFlags = installFlags;
    mInstallerPackageName = installerPackageName;
    mManifestDigest = manifestDigest;
    mUser = user;
    mAbiOverride = abiOverride;
    mInstructionSets = instructionSets;
    mHost = owner;
    // for epk
    if (packageURI != NULL) {
        String path;
        packageURI->GetPath(&path);
        if (path.EndWith(".epk"))
            mIsEpk = TRUE;
    }
}

Int32 CPackageManagerService::InstallArgs::DoPreCopy()
{
    return IPackageManager::INSTALL_SUCCEEDED;
}

Int32 CPackageManagerService::InstallArgs::DoPostCopy(
    /* [in] */ Int32 uid)
{
    return IPackageManager::INSTALL_SUCCEEDED;
}

Boolean CPackageManagerService::InstallArgs::IsFwdLocked()
{
    return (mInstallFlags & IPackageManager::INSTALL_FORWARD_LOCK) != 0;
}

AutoPtr<IUserHandle> CPackageManagerService::InstallArgs::GetUser()
{
    return mUser;
}


//==============================================================================
//                  CPackageManagerService::FileInstallArgs
//==============================================================================

CPackageManagerService::FileInstallArgs::FileInstallArgs(
    /* [in] */ InstallParams* params,
    /* [in] */ CPackageManagerService* owner)
    : InstallArgs(
        params->mOrigin,
        params->mObserver,
        params->mInstallFlags,
        params->mInstallerPackageName,
        params->GetManifestDigest(),
        params->GetUser(),
        NULL,
        params->mPackageAbiOverride,
        owner)
{
    if (IsFwdLocked()) {
        Slogger::E(CPackageManagerService::TAG, "Forward locking only supported in ASEC");
        assert(0);
        // throw new IllegalArgumentException("Forward locking only supported in ASEC");
    }
}

CPackageManagerService::FileInstallArgs::FileInstallArgs(
    /* [in] */ const String& codePath,
    /* [in] */ const String& resourcePath,
    /* [in] */ const String& legacyNativeLibraryPath,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ CPackageManagerService* owner)
    : InstallArgs(
        OriginInfo::FromNothing(),
        NULL,
        0,
        String(NULL),
        NULL,
        NULL,
        instructionSets,
        String(NULL),
        owner)
{
    if (!codePath.IsNull()) {
        CFile::New(codePath, (IFile**)&mCodeFile);
    }
    if (!resourcePath.IsNull()) {
        CFile::New(resourcePath, (IFile**)&mResourceFile);
    }
    if (!legacyNativeLibraryPath.IsNull()) {
        CFile::New(legacyNativeLibraryPath, (IFile**)&mLegacyNativeLibraryPath);
    }
}

ECode CPackageManagerService::FileInstallArgs::CheckFreeStorage(
    /* [in] */ IIMediaContainerService* imcs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    String absolutePath;
    mOrigin->mFile->GetAbsolutePath(&absolutePath);
    Int64 sizeBytes;
    FAIL_RETURN(imcs->CalculateInstalledSize(absolutePath, IsFwdLocked(), mAbiOverride, &sizeBytes))

    AutoPtr<IStorageManagerHelper> helper;
    CStorageManagerHelper::AcquireSingleton((IStorageManagerHelper**)&helper);
    AutoPtr<IStorageManager> storage;
    helper->From(mHost->mContext, (IStorageManager**)&storage);
    Int64 bytes;
    storage->GetstorageBytesUntilLow(Environment::GetDataDirectory(), &bytes);
    *result = (sizeBytes <= bytes);
    return NOERROR;
}

ECode CPackageManagerService::FileInstallArgs::CopyPkg(
    /* [in] */ IIMediaContainerService* imcs,
    /* [in] */ Boolean temp,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;

    if (mOrigin->mStaged) {
        Slogger::D(TAG, "%p already staged; skipping copy");
        mCodeFile = mOrigin->mFile;
        mResourceFile = mOrigin->mFile;
        *result = PackageManager.INSTALL_SUCCEEDED;
        return NOERROR;
    }

    // try {
    AutoPtr<IFile> tempDir;
    if (FAILED(mHost->mInstallerService->AllocateInternalStageDirLegacy((IFile**)tempDir))) {
        Slogger::W(TAG, "Failed to create copy file:");
        *result = IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
        return NOERROR;
    }
    mCodeFile = tempDir;
    mResourceFile = tempDir;
    // } catch (IOException e) {
    //     Slog.w(TAG, "Failed to create copy file: " + e);
    //     return PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
    // }

    AutoPtr<IIParcelFileDescriptorFactory> target;
    CParcelFileDescriptorFactory::New(mCodeFile, (IIParcelFileDescriptorFactory**)&target);
    Int32 ret = IPackageManager::INSTALL_SUCCEEDED;
    String absolutePath;
    mOrigin->mFile->GetAbsolutePath(&absolutePath);
    imcs->CopyPackage(absolutePath, target, &ret);
    if (ret != IPackageManager::INSTALL_SUCCEEDED) {
        Slogger::E(TAG, "Failed to copy package");
        *result = ret;
        return NOERROR;
    }

    AutoPtr<IFile> libraryRoot;
    CFile::New(mCodeFile, INativeLibraryHelper::LIB_DIR_NAME, (IFile**)&libraryRoot);
    AutoPtr<INativeLibraryHelperHandle> handle;
    // try {
    AutoPtr<INativeLibraryHelperHandleHelper> helper;
    CNativeLibraryHelperHandleHelper::AcquireSingleton((INativeLibraryHelperHandleHelper**)&helper);
    if (FAILED(helper->Create(mCodeFile, (INativeLibraryHelperHandle**)&handle))) {
        Slogger::E(CPackageManagerService::TAG, "Copying native libraries failed");
        ret = IPackageManager::INSTALL_FAILED_INTERNAL_ERROR;
        goto fail;
    }
    AutoPtr<INativeLibraryHelper> libHelper;
    CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&libHelper);
    libHelper->CopyNativeBinariesWithOverride(handle, libraryRoot, abiOverride, &ret);
    // } catch (IOException e) {
    //     Slog.e(TAG, "Copying native libraries failed", e);
    //     ret = PackageManager.INSTALL_FAILED_INTERNAL_ERROR;
    // } finally {
    //     IoUtils.closeQuietly(handle);
    // }
fail:
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ioUtils->CloseQuietly(handle);
    *result = ret;
    return NOERROR;
}

Int32 CPackageManagerService::FileInstallArgs::DoPreInstall(
    /* [in] */ Int32 status)
{
    if (status != IPackageManager::INSTALL_SUCCEEDED) {
        CleanUp();
    }
    return status;
}

Boolean CPackageManagerService::FileInstallArgs::DoRename(
    /* [in] */ Int32 status,
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ const String& oldCodePath)
{
    if (status != IPackageManager::INSTALL_SUCCEEDED) {
        CleanUp();
        return FALSE;
    }
    else {
        AutoPtr<IFile> beforeCodeFile = mCodeFile;
        AutoPtr<IFile> afterCodeFile = GetNextCodePath(pkg->mPackageName);

        Slogger::D(TAG, "Renaming %p to %p", beforeCodeFile.Get(), afterCodeFile.Get());
        // try {
        String absolutePath, absolutePath1;
        beforeCodeFile->GetAbsolutePath(&absolutePath);
        afterCodeFile->GetAbsolutePath(&absolutePath1);
        if (FAILED(Os::Rename(absolutePath, absolutePath1))) {
            Slogger::D(CPackageManagerService::TAG, "Failed to rename");
            return FALSE;
        }
        // } catch (ErrnoException e) {
        //     Slog.d(TAG, "Failed to rename", e);
        //     return false;
        // }

        if (!SELinux::RestoreconRecursive(afterCodeFile)) {
            Slogger::D(CPackageManagerService::TAG, "Failed to restorecon");
            return FALSE;
        }

        // Reflect the rename internally
        mCodeFile = afterCodeFile;
        mResourceFile = afterCodeFile;

        // Reflect the rename in scanned details
        afterCodeFile->GetAbsolutePath(&pkg->mCodePath);
        pkg->mBaseCodePath = FileUtils::RewriteAfterRename(beforeCodeFile, afterCodeFile,
                pkg->mBaseCodePath);
        pkg->mSplitCodePaths = FileUtils::RewriteAfterRename(beforeCodeFile, afterCodeFile,
                pkg->mSplitCodePaths);

        // Reflect the rename in app info
        pkg->mApplicationInfo->SetCodePath(pkg->mCodePath);
        pkg->mApplicationInfo->SetBaseCodePath(pkg->mBaseCodePath);
        pkg->mApplicationInfo->SetSplitCodePaths(pkg->mSplitCodePaths);
        pkg->mApplicationInfo->SetResourcePath(pkg->mCodePath);
        pkg->mApplicationInfo->SetBaseResourcePath(pkg->mBaseCodePath);
        pkg->mApplicationInfo->SetSplitResourcePaths(pkg->mSplitCodePaths);

        return TRUE;
    }
}

Int32 CPackageManagerService::FileInstallArgs::DoPostInstall(
    /* [in] */ Int32 status,
    /* [in] */ Int32 uid)
{
    if (status != IPackageManager::INSTALL_SUCCEEDED) {
        CleanUp();
    }
    return status;
}

String CPackageManagerService::FileInstallArgs::GetCodePath()
{
    String str(NULL);
    if (mCodeFile != NULL) {
        mCodeFile->GetAbsolutePath(&str);
    }
    return str;
}

String CPackageManagerService::FileInstallArgs::GetResourcePath()
{
    String str(NULL);
    if (mResourceFile != NULL) {
        mResourceFile->GetAbsolutePath(&str);
    }
    return str;
}

String CPackageManagerService::FileInstallArgs::GetLegacyNativeLibraryPath()
{
    String str(NULL);
    if (mLegacyNativeLibraryPath != NULL) {
        mLegacyNativeLibraryPath->GetAbsolutePath(&str);
    }
    return str;
}

Boolean CPackageManagerService::FileInstallArgs::CleanUp()
{
    Boolean exists;
    if (mCodeFile == NULL || (mCodeFile->Exists(&exists), !exists)) {
        return FALSE;
    }

    Boolean isDirectory;
    if (mCodeFile->IsDirectory(&isDirectory), isDirectory) {
        FileUtils::DeleteContents(codeFile);
    }
    mCodeFile->Delete();

    if (mResourceFile != NULL && !FileUtils::Contains(mCodeFile, mResourceFile)) {
        mResourceFile->Delete();
    }

    if (mLegacyNativeLibraryPath != NULL && !FileUtils::Contains(mCodeFile, mLegacyNativeLibraryPath)) {
        if (!FileUtils::DeleteContents(mLegacyNativeLibraryPath)) {
            Slogger::W(TAG, "Couldn't delete native library directory %p", mLegacyNativeLibraryPath.Get());
        }
        mLegacyNativeLibraryPath->delete();
    }

    return TRUE;
}

ECode CPackageManagerService::FileInstallArgs::CleanUpResourcesLI()
{
    // Try enumerating all code paths before deleting
    AutoPtr<List<String> > allCodePaths;
    Boolean exists;
    if (mCodeFile != NULL && (mCodeFile->Exists(&exists), exists)) {
        // try {
        AutoPtr<PackageLite> pkg = PackageParser::ParsePackageLite(mCodeFile, 0);
        allCodePaths = pkg->GetAllCodePaths();
        // } catch (PackageParserException e) {
        //     // Ignored; we tried our best
        // }
    }

    CleanUp();

    if (allCodePaths->Begin() != allCodePaths->End()) {
        if (mInstructionSets == NULL) {
            Slogger::E(CPackageManagerService::TAG, "instructionSet == null");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        AutoPtr< ArrayOf <String> > dexCodeInstructionSets
                = CPackageManagerService::GetDexCodeInstructionSets(instructionSets);
        List<String>::Iterator it = allCodePaths->Begin();
        for (; it != allCodePaths->End(); ++it) {
            String codePath = *it;
            for (Int32 i = 0; i < dexCodeInstructionSets->GetLength(); ++i) {
                Int32 retCode = mHost->mInstaller->Rmdex(codePath, (*dexCodeInstructionSets)[i]);
                if (retCode < 0) {
                    Slogger::W(CPackageManagerService::TAG, "Couldn't remove dex file for package:  at location %s, retcode=%d"
                            , codePath.string(), retCode);
                    // we don't consider this to be a failure of the core package deletion
                }
            }
        }
    }
}

Boolean CPackageManagerService::FileInstallArgs::DoPostDeleteLI(
    /* [in] */ Boolean del)
{
    // XXX err, shouldn't we respect the delete flag?
    CleanUpResourcesLI();
    return TRUE;
}


//==============================================================================
//                  CPackageManagerService::FileInstallArgs
//==============================================================================

const String CPackageManagerService::AsecInstallArgs::RES_FILE_NAME("pkg.apk");
const String CPackageManagerService::AsecInstallArgs::PUBLIC_RES_FILE_NAME("res.zip");

CPackageManagerService::AsecInstallArgs::AsecInstallArgs(
    /* [in] */ InstallParams* params,
    /* [in] */ CPackageManagerService* owner)
{
    AutoPtr<IManifestDigest> dig = params->GetManifestDigest();
    Init(params->mOrigin, params->mObserver, params->mInstallFlags,
            params->mInstallerPackageName, dig,
            params->GetUser(), NULL /* instruction sets */,
            params->mPackageAbiOverride, owner)
}

CPackageManagerService::AsecInstallArgs::AsecInstallArgs(
    /* [in] */ const String& _fullCodePath,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ Boolean isExternal,
    /* [in] */ Boolean isForwardLocked,
    /* [in] */ CPackageManagerService* owner)
{
    String fullCodePath = _fullCodePath;
    AutoPtr<OriginInfo> origin = OriginInfo::FromNothing();
    Init(origin, NULL,
            (isExternal ? INSTALL_EXTERNAL : 0) | (isForwardLocked ? INSTALL_FORWARD_LOCK : 0),
            String(NULL), NULL, NULL,
            instructionSets, String(NULL), owner);
    // Hackily pretend we're still looking at a full code path
    if (!fullCodePath.EndWith(RES_FILE_NAME)) {
        AutoPtr<IFile> f;
        CFile::New(fullCodePath, RES_FILE_NAME, (IFile**)&f);
        f->GetAbsolutePath(&fullCodePath);
    }

    // Extract cid from fullCodePath
    Int32 eidx = fullCodePath.LastIndexOf("/");
    String subStr1 = fullCodePath.Substring(0, eidx);
    Int32 sidx = subStr1.LastIndexOf("/");
    mCid = subStr1.Substring(sidx+1, eidx);
    SetMountPath(subStr1);
}

CPackageManagerService::AsecInstallArgs::AsecInstallArgs(
    /* [in] */ const String& cid,
    /* [in] */ Boolean isForwardLocked,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ CPackageManagerService* owner)
{
    AutoPtr<OriginInfo> origin = OriginInfo::FromNothing();
    Init(origin, NULL, (IsAsecExternal(cid) ? INSTALL_EXTERNAL : 0)
            | (isForwardLocked ? INSTALL_FORWARD_LOCK : 0), String(NULL), NULL, NULL,
            instructionSets, String(NULL), owner);
    mCid = cid;
    SetMountPath(PackageHelper::GetSdDir(cid));
}

void CPackageManagerService::AsecInstallArgs::CreateCopyFile()
{
    mCid = mHost->mInstallerService->AllocateExternalStageCidLegacy();
}

ECode CPackageManagerService::AsecInstallArgs::CheckFreeStorage(
    /* [in] */ IIMediaContainerService* imcs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    Int64 sizeBytes;
    FAIL_RETURN(imcs->CalculateInstalledSize(packagePath, IsFwdLocked(),
            abiOverride, &sizeBytes))

    AutoPtr<IFile> target;
    if (IsExternal()) {
        AutoPtr<UserEnvironment> env = new Environment::UserEnvironment(IUserHandle::USER_OWNER);
        target = env->GetExternalStorageDirectory();
    }
    else {
        target = Environment::GetDataDirectory();
    }

    AutoPtr<IStorageManagerHelper> helper;
    CStorageManagerHelper::AcquireSingleton((IStorageManagerHelper**)&helper);
    AutoPtr<IStorageManager> storage;
    helper->From(mHost->mContext, (IStorageManager**)&storage);
    Int64 bytes;
    storage->GetstorageBytesUntilLow(target, &bytes);
    *result = (sizeBytes <= bytes);
    return NOERROR;
}

ECode CPackageManagerService::AsecInstallArgs::CopyPkg(
    /* [in] */ IIMediaContainerService* imcs,
    /* [in] */ Boolean temp,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;

    if (mOrigin->mStaged) {
        Slogger::D(TAG, "%s already staged; skipping copy", mOrigin->mCid.string());
        mCid = mOrigin->mCid;
        SetMountPath(PackageHelper::GetSdDir(cid));
        *result = IPackageManager::INSTALL_SUCCEEDED;
        return NOERROR;
    }

    if (temp) {
        CreateCopyFile();
    }
    else {
        /*
         * Pre-emptively destroy the container since it's destroyed if
         * copying fails due to it existing anyway.
         */
        PackageHelper::DestroySdDir(mCid);
    }

    String absolutePath;
    mOrigin->mFile->GetAbsolutePath(&absolutePath);
    String newMountPath;
    FAIL_RETURN(imcs->CopyPackageToContainer(absolutePath, mCid,
            mHost->GetEncryptKey(), IsExternal(), IsFwdLocked(),
            CPackageManagerService::DeriveAbiOverride(abiOverride, NULL /* settings */), &newMountPath))

    if (!newMountPath.IsNull()) {
        SetMountPath(newMountPath);
        *result = IPackageManager::INSTALL_SUCCEEDED;
    } else {
        *result = IPackageManager::INSTALL_FAILED_CONTAINER_ERROR;
    }
    return NOERROR;
}

String CPackageManagerService::AsecInstallArgs::GetCodePath()
{
    return mPackagePath;
}

String CPackageManagerService::AsecInstallArgs::GetResourcePath()
{
    return mResourcePath;
}

String CPackageManagerService::AsecInstallArgs::GetLegacyNativeLibraryPath()
{
    return mLegacyNativeLibraryPath;
}

Int32 CPackageManagerService::AsecInstallArgs::DoPreInstall(
    /* [in] */ Int32 status)
{
    if (status != IPackageManager::INSTALL_SUCCEEDED) {
        // Destroy container
        PackageHelper::DestroySdDir(mCid);
    }
    else {
        Boolean mounted = PackageHelper::IsContainerMounted(mCid);
        if (!mounted) {
            String newMountPath = PackageHelper::MountSdDir(mCid,
                    mHost->GetEncryptKey(), IProcess::SYSTEM_UID);
            if (!newMountPath.IsNull()) {
                SetMountPath(newMountPath);
            }
            else {
                return IPackageManager::INSTALL_FAILED_CONTAINER_ERROR;
            }
        }
    }
    return status;
}

Boolean CPackageManagerService::AsecInstallArgs::DoRename(
    /* [in] */ Int32 status,
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ const String& oldCodePath)
{
    String newCacheId = GetNextCodePath(oldCodePath, pkg->mPackageName, String("/") + RES_FILE_NAME);
    String newMountPath = NULL;
    if (PackageHelper::IsContainerMounted(mCid)) {
        // Unmount the container
        if (!PackageHelper::UnMountSdDir(mCid)) {
            Slogger::I(TAG, "Failed to unmount %s before renaming", mCid.string());
            return FALSE;
        }
    }
    if (!PackageHelper::RenameSdDir(mCid, newCacheId)) {
        Slogger::E(TAG, "Failed to rename %s to %s which might be stale. Will try to clean up.",
                mCid.string(), newCacheId.string());
        // Clean up the stale container and proceed to recreate.
        if (!PackageHelper::DestroySdDir(newCacheId)) {
            Slogger::E(TAG, "Very strange. Cannot clean up stale container %s", newCacheId.string());
            return FALSE;
        }
        // Successfully cleaned up stale container. Try to rename again.
        if (!PackageHelper::RenameSdDir(mCid, newCacheId)) {
            Slogger::E(TAG, "Failed to rename %s to %s inspite of cleaning it up.",
                    mCid.string(), newCacheId.string());
            return FALSE;
        }
    }
    if (!PackageHelper::IsContainerMounted(newCacheId)) {
        Slogger::W(TAG, "Mounting container %s", newCacheId.string());
        newMountPath = PackageHelper::MountSdDir(newCacheId,
                mHost->GetEncryptKey(), IProcess::SYSTEM_UID);
    }
    else {
        newCachePath = PackageHelper::GetSdDir(newCacheId);
    }
    if (newMountPath.IsNull()) {
        Slogger::W(TAG, "Failed to get cache path for %s", newCacheId.string());
        return FALSE;
    }
    Slogger::I(TAG, "Succesfully renamed %s to %s at new path: %s",
            mCid.string(), newCacheId.string(), newMountPath.string());
    mCid = newCacheId;

    AutoPtr<IFile> beforeCodeFile;
    CFile::New(mPackagePath, (IFile**)&beforeCodeFile);
    SetMountPath(newMountPath);
    AutoPtr<IFile> afterCodeFile;
    CFile::New(mPackagePath, (IFile**)&afterCodeFile);

    // Reflect the rename in scanned details
    afterCodeFile->GetAbsolutePath(&pkg->mCodePath);
    pkg->mBaseCodePath = FileUtils::RewriteAfterRename(beforeCodeFile, afterCodeFile,
            pkg->mBaseCodePath);
    pkg->mSplitCodePaths = FileUtils::RewriteAfterRename(beforeCodeFile, afterCodeFile,
            pkg->mSplitCodePaths);

    // Reflect the rename in app info
    pkg->mApplicationInfo->SetCodePath(pkg->mCodePath);
    pkg->mApplicationInfo->SetBaseCodePath(pkg->mBaseCodePath);
    pkg->mApplicationInfo->SetSplitCodePaths(pkg->mSplitCodePaths);
    pkg->mApplicationInfo->SetResourcePath(pkg->mCodePath);
    pkg->mApplicationInfo->SetBaseResourcePath(pkg->mBaseCodePath);
    pkg->mApplicationInfo->SetSplitResourcePaths(pkg->mSplitCodePaths);
    return TRUE;
}

void CPackageManagerService::AsecInstallArgs::SetMountPath(
    /* [in] */ const String& mountPath)
{
    AutoPtr<IFile> mountFile, monolithicFile;
    CFile::New(mountPath, (IFile**)&mountFile);

    CFile::New(mountFile, RES_FILE_NAME, (IFile**)&mountFile);
    Boolean exists;
    if (monolithicFile->Exists(&exists), exists) {
        monolithicFile->GetAbsolutePath(&mPackagePath);
        if (IsFwdLocked()) {
            AutoPtr<IFile> f;
            CFile::New(mountFile, PUBLIC_RES_FILE_NAME, (IFile**)&f);
            f->GetAbsolutePath(&mResourcePath);
        }
        else {
            mResourcePath = mPackagePath;
        }
    }
    else {
        mountFile->GetAbsolutePath(&mPackagePath);
        mResourcePath = mPackagePath;
    }

    AutoPtr<IFile> f;
    CFile::New(mountFile, LIB_DIR_NAME, (IFile**)&f);
    CFile::New(mountFile, LIB_DIR_NAME, (IFile**)&f);
    f->GetAbsolutePath(&mLegacyNativeLibraryPath);
}

Int32 CPackageManagerService::AsecInstallArgs::DoPostInstall(
    /* [in] */ Int32 status,
    /* [in] */ Int32 uid)
{
    if (status != IPackageManager::INSTALL_SUCCEEDED) {
        CleanUp();
    }
    else {
        Int32 groupOwner;
        String protectedFile;
        if (IsFwdLocked()) {
            groupOwner = UserHandle::GetSharedAppGid(uid);
            protectedFile = RES_FILE_NAME;
        }
        else {
            groupOwner = -1;
            protectedFile = NULL;
        }

        if (uid < IProcess::FIRST_APPLICATION_UID
                || !PackageHelper::FixSdPermissions(mCid, groupOwner, protectedFile)) {
            Slogger::E(TAG, "Failed to finalize %s", mCid.string());
            PackageHelper::DestroySdDir(mCid);
            return IPackageManager::INSTALL_FAILED_CONTAINER_ERROR;
        }

        Boolean mounted = PackageHelper::IsContainerMounted(mCid);
        if (!mounted) {
            PackageHelper::MountSdDir(mCid, mHost->GetEncryptKey(), Process::MyUid());
        }
    }
    return status;
}

void CPackageManagerService::AsecInstallArgs::CleanUp()
{
    if (DEBUG_SD_INSTALL) Slogger::I(TAG, "cleanUp");

    // Destroy secure container
    PackageHelper::DestroySdDir(mCid);
}

AutoPtr<List<String> > CPackageManagerService::AsecInstallArgs::GetAllCodePaths()
{
    AutoPtr<IFile> codeFile;
    CFile::New(GetCodePath(), (IFile**)&codeFile);
    Boolean exists;
    if (codeFile != NULL && (codeFile->Exists(&exists), exists)) {
        // try {
        AutoPtr<PackageLite> pkg = PackageParser::ParsePackageLite(codeFile, 0);
        return pkg->GetAllCodePaths();
        // } catch (PackageParserException e) {
        //     // Ignored; we tried our best
        // }
    }
    return new List<String>();
}

ECode CPackageManagerService::AsecInstallArgs::CleanUpResourcesLI()
{
    // Enumerate all code paths before deleting
    AutoPtr<List<String> > paths = GetAllCodePaths();
    return CleanUpResourcesLI(paths);
}

ECode CPackageManagerService::AsecInstallArgs::CleanUpResourcesLI(
    /* [in] */ List<String>* allCodePaths)
{
    CleanUp();

    if (allCodePaths->Begin() != allCodePaths->End()) {
        if (mInstructionSets == NULL) {
            Slogger::E(CPackageManagerService::TAG, "instructionSet == null");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        AutoPtr< ArrayOf<String> > dexCodeInstructionSets
                = CPackageManagerService::GetDexCodeInstructionSets(mInstructionSets);
        List<String>::Iterator it = allCodePaths->End();
        for (; it != allCodePaths->End(); ++it) {
            String codePath = *it;
            for (Int32 i = 0; i < dexCodeInstructionSets->GetLength(); ++i) {
                Int32 retCode = mHost->mInstaller->Rmdex(codePath, (*dexCodeInstructionSets)[i]);
                if (retCode < 0) {
                    Slogger::W(TAG, "Couldn't remove dex file for package:  at location %s, retcode=%d"
                            , mCodePath.string(), retCode);
                    // we don't consider this to be a failure of the core package deletion
                }
            }
        }
    }
    return NOERROR;
}

Boolean CPackageManagerService::AsecInstallArgs::MatchContainer(
    /* [in] */ const String& app)
{
    if (mCid.StartWith(app)) {
        return TRUE;
    }
    return FALSE;
}

String CPackageManagerService::AsecInstallArgs::GetPackageName()
{
    return mHost->GetAsecPackageName(mCid);
}

Boolean CPackageManagerService::AsecInstallArgs::DoPostDeleteLI(
    /* [in] */ Boolean del)
{
    if (DEBUG_SD_INSTALL) Slogger::I(CPackageManagerService::TAG, "doPostDeleteLI() del=%d", del);
    AutoPtr<List<String> > allCodePaths = GetAllCodePaths();
    Boolean ret = FALSE;
    Boolean mounted = PackageHelper::IsContainerMounted(mCid);
    if (mounted) {
        // Unmount first
        if (PackageHelper::UnMountSdDir(cid)) {
            mounted = FALSE;
        }
    }
    if (!mounted && del) {
        CleanUpResourcesLI(allCodePaths);
    }
    return !mounted;
}

Int32 CPackageManagerService::AsecInstallArgs::DoPreCopy()
{
    if (IsFwdLocked()) {
        Int32 uid;
        mHost->GetPackageUid(DEFAULT_CONTAINER_PACKAGE, 0, &uid);
        if (!PackageHelper::FixSdPermissions(mCid, uid, RES_FILE_NAME)) {
            return IPackageManager::INSTALL_FAILED_CONTAINER_ERROR;
        }
    }

    return IPackageManager::INSTALL_SUCCEEDED;
}

Int32 CPackageManagerService::AsecInstallArgs::DoPostCopy(
    /* [in] */ Int32 uid)
{
    if (IsFwdLocked()) {
        if (uid < IProcess::FIRST_APPLICATION_UID
                || !PackageHelper::FixSdPermissions(mCid, UserHandle::GetSharedAppGid(uid), RES_FILE_NAME)) {
            Slogger::E(TAG, "Failed to finalize %s", mCid.string());
            PackageHelper::DestroySdDir(mCid);
            return IPackageManager::INSTALL_FAILED_CONTAINER_ERROR;
        }
    }

    return IPackageManager::INSTALL_SUCCEEDED;
}


//==============================================================================
//                  CPackageManagerService::PackageInstalledInfo
//==============================================================================

void CPackageManagerService::PackageInstalledInfo::SetError(
    /* [in] */ Int32 code,
    /* [in] */ const String& msg)
{
    mReturnCode = code;
    mReturnMsg = msg;
    Slogger::W(CPackageManagerService::TAG, "%s", msg.string());
}

void CPackageManagerService::PackageInstalledInfo::SetError(
    /* [in] */ const String& msg,
    /* [in] */ Int32 error,
    /* [in] */ ECode e/*PackageParserException e*/)
{
    mReturnCode = error;
    // mReturnMsg = ExceptionUtils.getCompleteMessage(msg, e);
    Slogger::W(CPackageManagerService::TAG, "%s, 0x%08x", msg.string(), e);
}


//==============================================================================
//                  CPackageManagerService::PackageRemovedInfo
//==============================================================================

void CPackageManagerService::PackageRemovedInfo::SendBroadcast(
    /* [in] */ Boolean fullRemove,
    /* [in] */ Boolean replacing,
    /* [in] */ Boolean removedForAllUsers)
{
    AutoPtr<IBundle> extras;
    CBundle::New(1, (IBundle**)&extras);
    extras->PutInt32(IIntent::EXTRA_UID, mRemovedAppId >= 0 ? mRemovedAppId : mUid);
    extras->PutBoolean(IIntent::EXTRA_DATA_REMOVED, fullRemove);
    if (replacing) {
        extras->PutBoolean(IIntent::EXTRA_REPLACING, TRUE);
    }
    extras->PutBoolean(IIntent::EXTRA_REMOVED_FOR_ALL_USERS, removedForAllUsers);
    if (!mRemovedPackage.IsNull()) {
        mHost->SendPackageBroadcast(IIntent::ACTION_PACKAGE_REMOVED, mRemovedPackage,
                extras, String(NULL), NULL, mRemovedUsers);
        if (fullRemove && !replacing) {
            mHost->SendPackageBroadcast(IIntent::ACTION_PACKAGE_FULLY_REMOVED, mRemovedPackage,
                    extras, String(NULL), NULL, mRemovedUsers);
        }
    }
    if (mRemovedAppId >= 0) {
        mHost->SendPackageBroadcast(IIntent::ACTION_UID_REMOVED, String(NULL),
                extras, String(NULL), NULL, mRemovedUsers);
    }
}


//==============================================================================
//                  CPackageManagerService::ClearStorageConnection
//==============================================================================

CAR_INTERFACE_IMPL(CPackageManagerService::ClearStorageConnection, Object, IServiceConnection)

ECode CPackageManagerService::ClearStorageConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    synchronized (this) {
        mContainerService = IIMediaContainerService::Probe(service);
        NotifyAll();
    }

    return NOERROR;
}

ECode CPackageManagerService::ClearStorageConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::NotifyRunnable
//==============================================================================

ECode CPackageManagerService::NotifyRunnable::Run()
{
    mHost->mHandler->RemoveCallbacks(this);

    Int32 retCode = -1;
    synchronized (mHost->mInstallLock) {
        retCode = mHost->mInstaller->FreeCache(mFreeStorageSize);
        if (retCode < 0) {
            Slogger::W(TAG, "Couldn't clear application caches");
        }
    }
    if (mObserver != NULL) {
        // try {
        mObserver->OnRemoveCompleted(String(NULL), (retCode >= 0));
        // } catch (RemoteException e) {
        //     Slog.w(TAG, "RemoveException when invoking call back");
        // }
    }
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::FreeStorageRunnable
//==============================================================================

ECode CPackageManagerService::FreeStorageRunnable::Run()
{
    mHost->mHandler->RemoveCallbacks(this);
    Int32 retCode = -1;
    synchronized (mHost->mInstallLock) {
        retCode = mHost->mInstaller->FreeCache(mFreeStorageSize);
        if (retCode < 0) {
            Slogger::W(TAG, "Couldn't clear application caches");
        }
    }
    if(mPi != NULL) {
        // try {
            // Callback via pending intent
        Int32 code = (retCode >= 0) ? 1 : 0;
        mPi->SendIntent(NULL, code, NULL, NULL, NULL);
        // } catch (SendIntentException e1) {
        //     Slog.i(TAG, "Failed to send pending intent");
        // }
    }
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::PackageComparator
//==============================================================================

CAR_INTERFACE_IMPL(CPackageManagerService::PackageComparator, Object, IComparator)

ECode CPackageManagerService::PackageComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<PackageParser::Package> p1 = (PackageParser::Package*)(IObject*)lhs;
    AutoPtr<PackageParser::Package> p2 = (PackageParser::Package*)(IObject*)rhs;
    *result = p1->mOverlayPriority - p2->mOverlayPriority;
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::DeleteFilenameFilter
//==============================================================================

CAR_INTERFACE_IMPL(CPackageManagerService::DeleteFilenameFilter, Object, IFilenameFilter)

ECode CPackageManagerService::DeleteFilenameFilter::Accept(
    /* [in] */ IFile* dir,
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    *succeeded = name.StartWith("vmdl") && name.EndWith(".tmp");
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::DeletePackageRunnable
//==============================================================================

ECode CPackageManagerService::DeletePackageRunnable::Run()
{
    mHost->mHandler->RemoveCallbacks(this);
    Int32 returnCode = mhost->deletePackageX(mPackageName, mUserId, mFlags);
    if (mObserver != NULL) {
        if (FAILED(observer->OnPackageDeleted(mPackageName, returnCode, String(NULL)))) {
            Logger::I(TAG, "Observer no longer exists.");
        }
    }
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::ClearRunnable
//==============================================================================

ECode CPackageManagerService::ClearRunnable::Run()
{
    mHost->mHandler->RemoveCallbacks(this);

    Boolean succeeded;
    synchronized (mHost->mInstallLock) {
        succeeded = mHost->ClearApplicationUserDataLI(mPackageName, mUserId);
    }
    mHost->ClearExternalStorageDataSync(mPackageName, mUserId, TRUE);
    if (succeeded) {
        // invoke DeviceStorageMonitor's update method to clear any notifications
        AutoPtr<DeviceStorageMonitorInternal> dsm = LocalServices::GetService(EIID_DeviceStorageMonitorInternal);
        if (dsm != NULL) {
            dsm->CheckMemory();
        }
    }
    if (mObserver != NULL) {
        // try {
        if (FAILED(mObserver->OnRemoveCompleted(mPackageName, succeeded))) {
            Logger::I(TAG, "Observer no longer exists.");
        }
        // } catch (RemoteException e) {
        //     Log.i(TAG, "Observer no longer exists.");
        // }
    } //end if observer
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::DeleteRunnable
//==============================================================================

ECode CPackageManagerService::DeleteRunnable::Run()
{
    mHost->mHandler->RemoveCallbacks(this);

    Boolean succeded;
    synchronized (mHost->mInstallLock) {
        succeded = mHost->DeleteApplicationCacheFilesLI(mPackageName, mUserId);
    }
    mHost->ClearExternalStorageDataSync(mPackageName, mUserId, FALSE);
    if (mObserver != NULL) {
        // try {
        if (FAILED(mObserver->OnRemoveCompleted(mPackageName, succeded))) {
            Logger::I(TAG, "Observer no longer exists.");
        }
        // } catch (RemoteException e) {
        //     Log.i(TAG, "Observer no longer exists.");
        // }
    } //end if observer
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::DumpState
//==============================================================================

const Int32 CPackageManagerService::DumpState::DUMP_LIBS;
const Int32 CPackageManagerService::DumpState::DUMP_FEATURES;
const Int32 CPackageManagerService::DumpState::DUMP_RESOLVERS;
const Int32 CPackageManagerService::DumpState::DUMP_PERMISSIONS;
const Int32 CPackageManagerService::DumpState::DUMP_PACKAGES;
const Int32 CPackageManagerService::DumpState::DUMP_SHARED_USERS;
const Int32 CPackageManagerService::DumpState::DUMP_MESSAGES;
const Int32 CPackageManagerService::DumpState::DUMP_PROVIDERS;
const Int32 CPackageManagerService::DumpState::DUMP_VERIFIERS;
const Int32 CPackageManagerService::DumpState::DUMP_PREFERRED;
const Int32 CPackageManagerService::DumpState::DUMP_PREFERRED_XML ;
const Int32 CPackageManagerService::DumpState::DUMP_KEYSETS ;
const Int32 CPackageManagerService::DumpState::DUMP_VERSION ;
const Int32 CPackageManagerService::DumpState::DUMP_INSTALLS ;
const Int32 CPackageManagerService::DumpState::OPTION_SHOW_FILTERS;

Boolean CPackageManagerService::DumpState::IsDumping(
    /* [in] */ Int32 type)
{
    if (mTypes == 0 && type != DUMP_PREFERRED_XML) {
        return TRUE;
    }

    return (mTypes & type) != 0;
}

void CPackageManagerService::DumpState::SetDump(
    /* [in] */ Int32 type)
{
    mTypes |= type;
}

Boolean CPackageManagerService::DumpState::IsOptionEnabled(
    /* [in] */ Int32 option)
{
    return (mOptions & option) != 0;
}

void CPackageManagerService::DumpState::SetOptionEnabled(
    /* [in] */ Int32 option)
{
    mOptions |= option;
}

Boolean CPackageManagerService::DumpState::OnTitlePrinted()
{
    Boolean printed = mTitlePrinted;
    mTitlePrinted = TRUE;
    return printed;
}

Boolean CPackageManagerService::DumpState::GetTitlePrinted()
{
    return mTitlePrinted;
}

void CPackageManagerService::DumpState::SetTitlePrinted(
    /* [in] */ Boolean enabled)
{
    mTitlePrinted = enabled;
}

AutoPtr<SharedUserSetting> CPackageManagerService::DumpStateGetSharedUser()
{
    return mSharedUser;
}

void CPackageManagerService::DumpState::SetSharedUser(
    /* [in] */ SharedUserSetting* user)
{
    mSharedUser = user;
}


//==============================================================================
//                  CPackageManagerService::UpdateExternalMediaStatusRunnable
//==============================================================================

ECode CPackageManagerService::UpdateExternalMediaStatusRunnable::Run()
{
    mHost->UpdateExternalMediaStatusInner(mMediaStatus, mReportStatus, TRUE);
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService::RemoveUnusedPackagesRunnable
//==============================================================================

ECode CPackageManagerService::RemoveUnusedPackagesRunnable::Run()
{
    mHost->DeletePackageX(mPackageName, mUserHandle, 0);
    return NOERROR;
}


//==============================================================================
//                  CPackageManagerService
//==============================================================================

const String CPackageManagerService::TAG("PackageManager");
const Boolean CPackageManagerService::DEBUG_SETTINGS;
const Boolean CPackageManagerService::DEBUG_PREFERRED;
const Boolean CPackageManagerService::DEBUG_UPGRADE;
const Boolean CPackageManagerService::DEBUG_INSTALL;
const Boolean CPackageManagerService::DEBUG_REMOVE;
const Boolean CPackageManagerService::DEBUG_BROADCASTS;
const Boolean CPackageManagerService::DEBUG_SHOW_INFO;
const Boolean CPackageManagerService::DEBUG_PACKAGE_INFO;
const Boolean CPackageManagerService::DEBUG_INTENT_MATCHING;
const Boolean CPackageManagerService::DEBUG_PACKAGE_SCANNING;
const Boolean CPackageManagerService::DEBUG_VERIFY;
const Boolean CPackageManagerService::DEBUG_DEXOPT;
const Boolean CPackageManagerService::DEBUG_ABI_SELECTION;
const Boolean CPackageManagerService::DEBUG_SD_INSTALL;
const Int32 CPackageManagerService::RADIO_UID;
const Int32 CPackageManagerService::LOG_UID;
const Int32 CPackageManagerService::NFC_UID;
const Int32 CPackageManagerService::BLUETOOTH_UID;
const Int32 CPackageManagerService::SHELL_UID;
const Int32 CPackageManagerService::MAX_PERMISSION_TREE_FOOTPRINT;
const String CPackageManagerService::INSTALL_PACKAGE_SUFFIX("-");
const Int32 CPackageManagerService::SCAN_NO_DEX;
const Int32 CPackageManagerService::SCAN_FORCE_DEX;
const Int32 CPackageManagerService::SCAN_UPDATE_SIGNATURE;
const Int32 CPackageManagerService::SCAN_NEW_INSTALL;
const Int32 CPackageManagerService::SCAN_NO_PATHS;
const Int32 CPackageManagerService::SCAN_UPDATE_TIME;
const Int32 CPackageManagerService::SCAN_DEFER_DEX;
const Int32 CPackageManagerService::SCAN_BOOTING;
const Int32 CPackageManagerService::SCAN_TRUSTED_OVERLAY;
const Int32 CPackageManagerService::SCAN_DELETE_DATA_ON_FAILURES;
const Int32 CPackageManagerService::SCAN_REPLACING;
const Int32 CPackageManagerService::REMOVE_CHATTY;
const Int64 CPackageManagerService::WATCHDOG_TIMEOUT;
const Int64 CPackageManagerService::DEFAULT_MANDATORY_FSTRIM_INTERVAL;
const Boolean CPackageManagerService::DEFAULT_VERIFY_ENABLE;
const Int64 CPackageManagerService::DEFAULT_VERIFICATION_TIMEOUT;
const Int32 CPackageManagerService::DEFAULT_VERIFICATION_RESPONSE;
const String CPackageManagerService::DEFAULT_CONTAINER_PACKAGE("com.android.defcontainer");

static AutoPtr<IComponentName> Init_DEFAULT_CONTAINER_COMPONENT()
{
    AutoPtr<IComponentName> component;
    CComponentName::New(CPackageManagerService::DEFAULT_CONTAINER_PACKAGE,
            String("com.android.defcontainer.DefaultContainerService"), (IComponentName**)&component);
    return component;
}
const AutoPtr<IComponentName> CPackageManagerService::DEFAULT_CONTAINER_COMPONENT =  Init_DEFAULT_CONTAINER_COMPONENT();
const String CPackageManagerService::PACKAGE_MIME_TYPE("application/vnd.android.package-archive");
const String CPackageManagerService::VENDOR_OVERLAY_DIR("/vendor/overlay");
const String CPackageManagerService::VENDOR_APP_DIR("/system/vendor/app");
String CPackageManagerService::sPreferredInstructionSet;
const String CPackageManagerService::IDMAP_PREFIX("/data/resource-cache/");
const String CPackageManagerService::IDMAP_SUFFIX("@idmap");
const Int32 CPackageManagerService::SEND_PENDING_BROADCAST;
const Int32 CPackageManagerService::MCS_BOUND;
const Int32 CPackageManagerService::END_COPY;
const Int32 CPackageManagerService::INIT_COPY;
const Int32 CPackageManagerService::MCS_UNBIND;
const Int32 CPackageManagerService::START_CLEANING_PACKAGE;
const Int32 CPackageManagerService::FIND_INSTALL_LOC;
const Int32 CPackageManagerService::POST_INSTALL;
const Int32 CPackageManagerService::MCS_RECONNECT;
const Int32 CPackageManagerService::MCS_GIVE_UP;
const Int32 CPackageManagerService::UPDATED_MEDIA_STATUS;
const Int32 CPackageManagerService::WRITE_SETTINGS;
const Int32 CPackageManagerService::WRITE_PACKAGE_RESTRICTIONS;
const Int32 CPackageManagerService::PACKAGE_VERIFIED;
const Int32 CPackageManagerService::CHECK_PENDING_VERIFICATION;
const Int32 CPackageManagerService::WRITE_SETTINGS_DELAY;
const Int32 CPackageManagerService::BROADCAST_DELAY;
AutoPtr<CUserManagerService> CPackageManagerService::sUserManager;
const Int32 CPackageManagerService::DEX_OPT_SKIPPED;
const Int32 CPackageManagerService::DEX_OPT_PERFORMED;
const Int32 CPackageManagerService::DEX_OPT_DEFERRED;
const Int32 CPackageManagerService::DEX_OPT_FAILED;
const Int32 CPackageManagerService::UPDATE_PERMISSIONS_ALL;
const Int32 CPackageManagerService::UPDATE_PERMISSIONS_REPLACE_PKG;
const Int32 CPackageManagerService::UPDATE_PERMISSIONS_REPLACE_ALL;

AutoPtr<IComparator> InitResolvePrioritySorter()
{
    return (IComparator*)new ResolvePrioritySorter();
}
static AutoPtr<IComparator> mResolvePrioritySorter = InitResolvePrioritySorter();

AutoPtr<IComparator> InitProviderInitOrderSorter()
{
    return (IComparator*)new ProviderInitOrderSorter();
}
static AutoPtr<IComparator> mProviderInitOrderSorter = InitProviderInitOrderSorter();
const String CPackageManagerService::SD_ENCRYPTION_KEYSTORE_NAME("AppsOnSD");
const String CPackageManagerService::SD_ENCRYPTION_ALGORITHM("AES");

CPackageManagerService::CPackageManagerService()
    : mSdkVersion(Build::VERSION::SDK_INT)
    , mFactoryTest(FALSE)
    , mOnlyCore(FALSE)
    , mLazyDexOpt(FALSE)
    , mDefParseFlags(0)
    , mRestoredSettings(FALSE)
    , mFoundPolicyFile(FALSE)
    , mSystemReady(FALSE)
    , mSafeMode(FALSE)
    , mHasSystemUidErrors(FALSE)
    , mResolverReplaced(FALSE)
    , mNextInstallToken(1)
    , mShouldRestoreconData(SELinuxMMAC::ShouldRestorecon())
    , mPendingVerificationToken(0)
    , mMediaMounted(FALSE)
    , mLastScanError(0)
{
    mActivities = new ActivityIntentResolver(this);
    mReceivers = new ActivityIntentResolver(this);
    mServices = new ServiceIntentResolver(this);
    CActivityInfo::New((IActivityInfo**)&mResolveActivity);
    CResolveInfo::New((IResolveInfo**)&mResolveInfo);
    mDefContainerConn = new DefaultContainerConnection(this);
    mProviders = new ProviderIntentResolver();
    mPendingBroadcasts = new PendingPackageBroadcasts();
    mPackageUsage = new PackageUsage(this);
}

CPackageManagerService::~CPackageManagerService()
{}

CAR_INTERFACE_IMPL(CPackageManagerService, Object, IIPackageManager)

CAR_OBJECT_IMPL(CPackageManagerService)

AutoPtr<IBundle> CPackageManagerService::ExtrasForInstallResult(
    /* [in] */ PackageInstalledInfo* res)
{
    AutoPtr<IBundle> extras;
    switch (res->mReturnCode) {
        case IPackageManager::INSTALL_FAILED_DUPLICATE_PERMISSION: {
            CBundle::New((IBundle**)&extras);
            extras->PutString(IPackageManager::EXTRA_FAILURE_EXISTING_PERMISSION,
                    res->mOrigPermission);
            extras->PutString(IPackageManager::EXTRA_FAILURE_EXISTING_PACKAGE,
                    res->mOrigPackage);
            break;
        }
    }
    return extras;
}

void CPackageManagerService::ScheduleWriteSettingsLocked()
{
    Boolean bval;
    mHandler->HasMessages(WRITE_SETTINGS, &bval);
    if (!bval) {
        mHandler->SendEmptyMessageDelayed(WRITE_SETTINGS,
            WRITE_SETTINGS_DELAY, &bval);
    }
}

void CPackageManagerService::ScheduleWritePackageRestrictionsLocked(
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return;

    mDirtyUsers.Insert(userId);
    Boolean bval;
    mHandler->HasMessages(WRITE_PACKAGE_RESTRICTIONS, &bval);
    if (!bval) {
        mHandler->SendEmptyMessageDelayed(WRITE_PACKAGE_RESTRICTIONS,
            WRITE_SETTINGS_DELAY, &bval);
    }
}

AutoPtr<IIPackageManager> CPackageManagerService::Main(
    /* [in] */ IContext* context,
    /* [in] */ Installer* installer,
    /* [in] */ Boolean factoryTest,
    /* [in] */ Boolean onlyCore)
{
    AutoPtr<IIPackageManager> m;
    CPackageManagerService::New(context, (Handle32)installer,
            factoryTest, onlyCore, (IIPackageManager**)&m);
    ServiceManager::AddService(String("package"), m);
    return m;
}

AutoPtr< ArrayOf<String> > CPackageManagerService::SplitString(
    /* [in] */ const String& str,
    /* [in] */ Char32 sep)
{
    Int32 count = 1;
    Int32 i = 0;
    while ((i = str.IndexOf(sep, i)) >= 0) {
        count++;
        i++;
    }

    AutoPtr< ArrayOf<String> > res = ArrayOf<String>::Alloc(count);
    i = 0;
    count = 0;
    Int32 lastI = 0;
    while ((i = str.IndexOf(sep, i)) >= 0) {
        (*res)[count] = str.Substring(lastI, i);
        count++;
        i++;
        lastI = i;
    }
    (*res)[count] = str.Substring(lastI, str.GetLength());
    return res;
}

void CPackageManagerService::GetDefaultDisplayMetrics(
    /* [in] */ IContext* context,
    /* [in] */ IDisplayMetrics* metrics)
{
    AutoPtr<IInterface> service;
    context->GetSystemService(IContext::DISPLAY_SERVICE, (IInterface**)&service);
    AutoPtr<IDisplayManager> displayManager = IDisplayManager::Probe(service);
    AutoPtr<IDisplay> d;
    displayManager->GetDisplay(IDisplay::DEFAULT_DISPLAY, (IDisplay**)&d);
    d->GetMetrics(metrics);
}

ECode CPackageManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Handle32 installer,
    /* [in] */ Boolean factoryTest,
    /* [in] */ Boolean onlyCore)
{
//     EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_START,
//             SystemClock.uptimeMillis());

    if (mSdkVersion <= 0) {
        Slogger::W(TAG, "**** ro.build.version.sdk not set!");
    }

    mContext = context;
    mFactoryTest = factoryTest;
    mOnlyCore = onlyCore;
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String value;
    sysProp->Get(String("ro.build.type"), &value);
    mLazyDexOpt = String("eng").Equals(value);
    CDisplayMetrics::New((IDisplayMetrics**)&mMetrics);
    mSettings = new Settings(context);
    mSettings->AddSharedUserLPw(String("android.uid.system"), IProcess::SYSTEM_UID,
            IApplicationInfo::FLAG_SYSTEM | IApplicationInfo::FLAG_PRIVILEGED);
    mSettings->AddSharedUserLPw(String("android.uid.phone"), RADIO_UID,
            IApplicationInfo::FLAG_SYSTEM | IApplicationInfo::FLAG_PRIVILEGED);
    mSettings->AddSharedUserLPw(String("android.uid.log"), LOG_UID,
            IApplicationInfo::FLAG_SYSTEM | IApplicationInfo::FLAG_PRIVILEGED);
    mSettings->AddSharedUserLPw(String("android.uid.nfc"), NFC_UID,
            IApplicationInfo::FLAG_SYSTEM | IApplicationInfo::FLAG_PRIVILEGED);
    mSettings->AddSharedUserLPw(String("android.uid.bluetooth"), BLUETOOTH_UID,
            IApplicationInfo::FLAG_SYSTEM | IApplicationInfo::FLAG_PRIVILEGED);
    mSettings->AddSharedUserLPw(String("android.uid.shell"), SHELL_UID,
            IApplicationInfo::FLAG_SYSTEM | IApplicationInfo::FLAG_PRIVILEGED);

    String separateProcesses;
    sysProp->Get(String("debug.separate_processes"), &separateProcesses);
    if (!separateProcesses.IsNullOrEmpty()) {
        if (String("*").Equals(separateProcesses)) {
            mDefParseFlags = PackageParser::PARSE_IGNORE_PROCESSES;
            mSeparateProcesses = NULL;
            Slogger::W(TAG, "Running with debug.separate_processes: * (ALL)");
        } else {
            mDefParseFlags = 0;
            StringUtils::Split(separateProcesses, String(","), (ArrayOf<String>**)&mSeparateProcesses);
            Slogger::W(TAG, String("Running with debug.separate_processes: ") + separateProcesses);
        }
    } else {
        mDefParseFlags = 0;
        mSeparateProcesses = NULL;
    }

    mInstaller = (Installer*)installer;

    GetDefaultDisplayMetrics(context, mMetrics);

    AutoPtr<SystemConfig> systemConfig = SystemConfig::GetInstance();
    mGlobalGids = systemConfig->GetGlobalGids();
    mSystemPermissions = systemConfig->GetSystemPermissions();
    mAvailableFeatures = systemConfig->GetAvailableFeatures();
    // ActionsCode(songzhining, new code: add extra hardware feature support)
    mExtraFeatures = systemConfig->GetExtraFeatures();

    AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
    if (readBuffer == NULL) {
        Slogger::E(TAG, "Failed to create CPackageManagerService: out of memory!");
        return E_OUT_OF_MEMORY_ERROR;
    }

    synchronized (mInstallLock) {
        // writer
        synchronized (mPackages) {
            mHandlerThread = new ServiceThread(TAG,
                    IProcess::THREAD_PRIORITY_BACKGROUND, TRUE /*allowIo*/);
            mHandlerThread->Start();
            AutoPtr<ILooper> looper;
            mHandlerThread->GetLooper((ILooper**)&looper);
            mHandler = new PackageHandler(looper, this);
            // Watchdog.getInstance().addThread(mHandler, WATCHDOG_TIMEOUT);

            AutoPtr<IFile> dataDir = Environment::GetDataDirectory((IFile**)&dataDir);
            CFile::New(dataDir, String("data"), (IFile**)&mAppDataDir);
            CFile::New(dataDir, String("app"), (IFile**)&mAppInstallDir);
            CFile::New(dataDir, String("app-lib"), (IFile**)&mAppLib32InstallDir);
            AutoPtr<IFile> asecInternal;
            CFile::New(dataDir, String("app-asec"), (IFile**)&asecInternal);
            asecInternal->GetPath(&mAsecInternalPath);
            CFile::New(dataDir, String("user"), (IFile**)&mUserAppDataDir);
            CFile::New(dataDir, String("app-private"), (IFile**)&mDrmAppPrivateInstallDir);

            //for epk
            AutoPtr<IFile> ecoDir;
            CFile::New(dataDir, String("elastos"), (IFile**)&ecoDir);
            Boolean res;
            ecoDir->Mkdir(&res);
            if (res) {
                String path;
                ecoDir->GetPath(&path);

                AutoPtr<ILibcore> libcore;
                CLibcore::AcquireSingleton((ILibcore**)&libcore);
                AutoPtr<IOs> os;
                libcore->GetOs((IOs**)&os);
                AutoPtr<IOsConstants> osConsts;
                COsConstants::AcquireSingleton((IOsConstants**)&osConsts);
                Int32 v1, v2, v3;
                osConsts->GetOsConstant(String("S_IRWXU"), &v1);
                osConsts->GetOsConstant(String("S_IRWXG"), &v2);
                osConsts->GetOsConstant(String("S_IRWXO"), &v3);
                os->Chmod(path, v1 | v2 | v3);
            }

            CUserManagerService::NewByFriend(
                    context, (Handle32)this, (Handle32)&mInstallLock, (Handle32)&mPackagesLock,
                    (CUserManagerService**)&sUserManager);

            // Propagate permission configuration in to package manager.
            Map<String, AutoPtr<SystemConfig::PermissionEntry> >& permConfig
                    = systemConfig->GetPermissions();
            Map<String, AutoPtr<SystemConfig::PermissionEntry> >::Iterator confIt = permConfig.Begin();
            for (; confIt != permConfig.End; ++confIt) {
                AutoPtr<SystemConfig::PermissionEntry> perm = confIt->mSecond;
                HashMap<String, AutoPtr<BasePermission> >::Iterator permIt = mSettings->mPermissions.Find(perm->mName);
                AutoPtr<BasePermission> bp = permIt != mSettings->mPermissions.End() ? permIt->mSecond : NULL;
                if (bp == NULL) {
                    bp = new BasePermission(perm->mName, String("android"), BasePermission::TYPE_BUILTIN);
                    mSettings->mPermissions[perm->mName] = bp;
                }
                if (perm->mGids != NULL) {
                    bp->mGids = AppendInts(bp->mGids, perm->mGids);
                }
            }

            Map<String, String>& libConfig = systemConfig->GetSharedLibraries();
            Map<String, String>::Iterator libConfIt = libConfig.Begin();
            for (; libConfIt != libConfig.End(); ++libConfIt) {
                AutoPtr<SharedLibraryEntry> entry = new SharedLibraryEntry(libConfIt->mSecond, String(NULL));
                mSharedLibraries[libConfIt->mFirst] = entry;
            }

            mFoundPolicyFile = SELinuxMMAC::ReadInstallPolicy();

            AutoPtr<IList> users;
            sUserManager->GetUsers(FALSE, (IList**)&users);
            mRestoredSettings = mSettings->ReadLPw(this, users, mSdkVersion, mOnlyCore);

            AutoPtr<IResourcesHelper> resH;
            CResourcesHelper::AcquireSingleton((IResourcesHelper**)&resH);
            AutoPtr<IResources> res;
            resH->GetSystem((IResources**)&res);
            String customResolverActivity;
            res->GetString(R::string::config_customResolverActivity, &customResolverActivity);
            if (TextUtils::IsEmpty(customResolverActivity)) {
                customResolverActivity = String(NULL);
            }
            else {
                AutoPtr<IComponentNameHelper> cnHelper;
                CComponentNameHelper::AcquireSingleton((IComponentNameHelper**)&cnHelper);
                mCustomResolverComponentName = NULL;
                cnHelper->UnflattenFromString(customResolverActivity, (IComponentName**)&mCustomResolverComponentName);
            }

            Int64 startTime = SystemClock::GetUptimeMillis();

//         EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_SYSTEM_SCAN_START,
//                 startTime);

            // Set flag to monitor and not change apk file paths when
            // scanning install directories.
            Int32 scanFlags = SCAN_NO_PATHS | SCAN_DEFER_DEX | SCAN_BOOTING;

            HashSet<String> alreadyDexOpted;

            /**
             * Add everything in the in the boot class path to the
             * list of process files because dexopt will have been run
             * if necessary during zygote startup.
             */
            AutoPtr<ISystem> system;
            CSystem::AcquireSingleton((ISystem**)&system);
            String bootClassPath;
            system->GetEnv(String("BOOTCLASSPATH"), &bootClassPath);
            system->GetEnv(String("SYSTEMSERVERCLASSPATH"), &systemServerClassPath);

            if (!bootClassPath.IsNull()) {
                AutoPtr< ArrayOf<String> > bootClassPathElements = SplitString(bootClassPath, ':');
                for (Int32 i = 0; i < bootClassPathElements->GetLength(); ++i) {
                    alreadyDexOpted.Insert((*bootClassPathElements)[i]);
                }
            }
            else {
                Slogger::W(TAG, "No BOOTCLASSPATH found!");
            }

            if (!systemServerClassPath.IsNull()) {
                AutoPtr< ArrayOf<String> > systemServerClassPathElements = SplitString(systemServerClassPath, ':');
                for (Int32 i = 0; i < systemServerClassPathElements->GetLength(); ++i) {
                    alreadyDexOpted.Insert((*systemServerClassPathElements)[i]);
                }
            }
            else {
                Slogger::W(TAG, "No SYSTEMSERVERCLASSPATH found!");
            }

            Boolean didDexOptLibraryOrTool = FALSE;

            List<String>& allInstructionSets = GetAllInstructionSets();
            AutoPtr< ArrayOf<String> > dexCodeInstructionSets = GetDexCodeInstructionSets(allInstructionSets);

            /**
             * Ensure all external libraries have had dexopt run on them.
             */
            // TODO
            // if (mSharedLibraries.Begin() != mSharedLibraries.End()) {
            //     // NOTE: For now, we're compiling these system "shared libraries"
            //     // (and framework jars) into all available architectures. It's possible
            //     // to compile them only when we come across an app that uses them (there's
            //     // already logic for that in scanPackageLI) but that adds some complexity.
            //     for (Int32 i = 0; i < dexCodeInstructionSets->GetLength(); ++i) {
            //         String dexCodeInstructionSet = (*dexCodeInstructionSets)[i];
            //         HashMap<String, AutoPtr<SharedLibraryEntry> >::Iterator libIt = mSharedLibraries.Begin();
            //         for (; libIt != mSharedLibraries.End(); ++libIt) {
            //             String lib = libIt->mSecond->mPath;
            //             if (lib.IsNull()) {
            //                 continue;
            //             }

            //             // try {
            //             Byte dexoptRequired = DexFile.isDexOptNeededInternal(lib, null, dexCodeInstructionSet, false);
            //             if (dexoptRequired != DexFile.UP_TO_DATE) {
            //                 alreadyDexOpted.add(lib);

            //                 // The list of "shared libraries" we have at this point is
            //                 if (dexoptRequired == DexFile.DEXOPT_NEEDED) {
            //                     mInstaller.dexopt(lib, Process.SYSTEM_UID, true, dexCodeInstructionSet);
            //                 } else {
            //                     mInstaller.patchoat(lib, Process.SYSTEM_UID, true, dexCodeInstructionSet);
            //                 }
            //                 didDexOptLibraryOrTool = true;
            //             }
            //             } catch (FileNotFoundException e) {
            //                 Slog.w(TAG, "Library not found: " + lib);
            //             } catch (IOException e) {
            //                 Slog.w(TAG, "Cannot dexopt " + lib + "; is it an APK or JAR? "
            //                         + e.getMessage());
            //             }
            //         }
            //     }
            // }

            AutoPtr<IFile> frameworkDir;
            AutoPtr<IFile> rootDir = Environment::GetRootDirectory();
            CFile::New(rootDir, String("framework"), (IFile**)&frameworkDir);

            // Gross hack for now: we know this file doesn't contain any
            // code, so don't dexopt it to avoid the resulting log spew.
            String frameworkDirP;
            frameworkDir->GetPath(&frameworkDirP);
            alreadyDexOpted.Insert(frameworkDirP + String("/framework-res.apk"));

            // Gross hack for now: we know this file is only part of
            // the boot class path for art, so don't dexopt it to
            // avoid the resulting log spew.
            alreadyDexOpted.Insert(frameworkDirP + String("/core-libart.jar"));

            /**
             * And there are a number of commands implemented in Java, which
             * we currently need to do the dexopt on so that they can be
             * run from a non-root shell.
             */
            AutoPtr< ArrayOf<String> > frameworkFiles;
            frameworkDir->List((ArrayOf<String>**)&frameworkFiles);
            if (frameworkFiles != NULL) {
                // TODO: We could compile these only for the most preferred ABI. We should
                // first double check that the dex files for these commands are not referenced
                // by other system apps.
                for (Int32 i = 0; i < dexCodeInstructionSets->GetLength(); ++i) {
                    String dexCodeInstructionSet = (*dexCodeInstructionSets)[i];
                    for (Int32 j = 0; j < frameworkFiles->GetLength(); ++j) {
                        AutoPtr<IFile> libPath;
                        CFile::New(frameworkDir, (*frameworkFiles)[j], (IFile**)&libPath);
                        String path;
                        libPath->GetPath(&path);
                        // Skip the file if we already did it.
                        if (alreadyDexOpted.Find(path) != alreadyDexOpted.End()) {
                            continue;
                        }
                        // Skip the file if it is not a type we want to dexopt.
                        if (!path.EndWith(".apk") && !path.EndWith(".jar")) {
                            continue;
                        }
                        // TODO
                        // try {
                        //     byte dexoptRequired = DexFile.isDexOptNeededInternal(path, null,
                        //                                                          dexCodeInstructionSet,
                        //                                                          false);
                        //     if (dexoptRequired == DexFile.DEXOPT_NEEDED) {
                        //         mInstaller.dexopt(path, Process.SYSTEM_UID, true, dexCodeInstructionSet);
                        //         didDexOptLibraryOrTool = true;
                        //     } else if (dexoptRequired == DexFile.PATCHOAT_NEEDED) {
                        //         mInstaller.patchoat(path, Process.SYSTEM_UID, true, dexCodeInstructionSet);
                        //         didDexOptLibraryOrTool = true;
                        //     }
                        // } catch (FileNotFoundException e) {
                        //     Slog.w(TAG, "Jar not found: " + path);
                        // } catch (IOException e) {
                        //     Slog.w(TAG, "Exception reading jar: " + path, e);
                        // }
                    }
                }
            }

            // Collect vendor overlay packages.
            // (Do this before scanning any apps.)
            // For security and version matching reason, only consider
            // overlay packages if they reside in VENDOR_OVERLAY_DIR.
            AutoPtr<IFile> vendorOverlayDir;
            CFile::New(VENDOR_OVERLAY_DIR, (IFile**)&vendorOverlayDir);
            ScanDirLI(vendorOverlayDir, PackageParser::PARSE_IS_SYSTEM
                    | PackageParser::PARSE_IS_SYSTEM_DIR, scanFlags | SCAN_TRUSTED_OVERLAY, 0, readBuffer);

            // Find base frameworks (resource packages without code).
            ScanDirLI(frameworkDir, PackageParser::PARSE_IS_SYSTEM
                    | PackageParser::PARSE_IS_SYSTEM_DIR
                    | PackageParser::PARSE_IS_PRIVILEGED,
                    scanFlags | SCAN_NO_DEX, 0, readBuffer);

            // Collected privileged system packages.
            AutoPtr<IFile> privilegedAppDir;
            CFile::New(rootDir, String("priv-app"), (IFile**)&privilegedAppDir);
            ScanDirLI(privilegedAppDir, PackageParser::PARSE_IS_SYSTEM
                    | PackageParser::PARSE_IS_SYSTEM_DIR
                    | PackageParser::PARSE_IS_PRIVILEGED, scanFlags, 0,readBuffer);

            // Collect ordinary system packages.
            AutoPtr<IFile> systemAppDir;
            CFile::New(rootDir, String("app"), (IFile**)&systemAppDir);
            ScanDirLI(systemAppDir, PackageParser::PARSE_IS_SYSTEM
                    | PackageParser::PARSE_IS_SYSTEM_DIR, scanFlags, 0, readBuffer);

            // Collect all vendor packages.
            /* ActionsCode(songzhining, change code: fix vendor app path) */
            AutoPtr<IFile> vendorAppDir;
            CFile::New(VENDOR_APP_DIR, (IFile**)&vendorAppDir);
            // try {
            AutoPtr<IFile> temp = vendorAppDir;
            vendorAppDir = NULL;
            temp->GetCanonicalFile((IFile**)&vendorAppDir);
            // } catch (IOException e) {
            //     // failed to look up canonical path, continue with original one
            // }
            ScanDirLI(vendorAppDir, PackageParser::PARSE_IS_SYSTEM
                    | PackageParser::PARSE_IS_SYSTEM_DIR, scanFlags, 0, readBuffer);

            // Collect all OEM packages.
            AutoPtr<IFile> oemDir = Environment::GetOemDirectory((IFile**)&oemDir);
            AutoPtr<IFile> oemAppDir;
            CFile::New(oemDir, String("app"), (IFile**)&oemDir);
            scanDirLI(oemAppDir, PackageParser::PARSE_IS_SYSTEM
                    | PackageParser::PARSE_IS_SYSTEM_DIR, scanFlags, 0, readBuffer);

            if (DEBUG_UPGRADE) Logger::V(TAG, "Running installd update commands");
            mInstaller->MoveFiles();

            // Prune any system packages that no longer exist.
            List<String> possiblyDeletedUpdatedSystemApps;
            Map<String, AutoPtr<IFile> > expectingBetter;
            if (!mOnlyCore) {
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Begin();
                while (it != mSettings->mPackages.End()) {
                    AutoPtr<PackageSetting> ps = it->mSecond;

                    /*
                     * If this is not a system app, it can't be a
                     * disable system app.
                     */
                    if ((ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0) {
                        ++it;
                        continue;
                    }

                    /*
                     * If the package is scanned, it's not erased.
                     */
                    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator psit = mPackages.Find(ps->mName);
                    AutoPtr<PackageParser::Package> scannedPkg;
                    if (psit != mPackages.End()) scannedPkg = psit->mSecond;
                    if (scannedPkg != NULL) {
                        /*
                         * If the system app is both scanned and in the
                         * disabled packages list, then it must have been
                         * added via OTA. Remove it from the currently
                         * scanned package so the previously user-installed
                         * application can be scanned.
                         */
                        if (mSettings->IsDisabledSystemPackageLPr(ps->mName)) {
                            LogCriticalInfo(ILogHelper::WARN, String("Expecting better updated system app for ")
                                    + ps->mName + "; removing system app.  Last known codePath="
                                    + ps->mCodePathString + ", installStatus=" + StringUtils::ToString(ps->mInstallStatus)
                                    + ", versionCode=" + StringUtils::ToString(ps->mVersionCode) + "; scanned versionCode="
                                    + StringUtils::ToString(scannedPkg->mVersionCode));
                            RemovePackageLI(ps, TRUE);
                            expectingBetter[ps->mName] = ps->mCodePath;
                        }

                        ++it;
                        continue;
                    }

                    if (!mSettings->IsDisabledSystemPackageLPr(ps->mName)) {
                        mSettings->mPackages.Erase(it++);
                        String msg = String("System package ") + ps->mName
                                + " no longer exists; wiping its data";
                        ReportSettingsProblem(5/*TODO: Log.WARN*/, msg);
                        RemoveDataDirsLI(ps->mName);
                    }
                    else {
                        AutoPtr<PackageSetting> disabledPs = mSettings->GetDisabledSystemPkgLPr(ps->mName);
                        Boolean exists = FALSE;
                        if (disabledPs->mCodePath == NULL || (disabledPs->mCodePath->Exists(&exists), !exists)) {
                            possiblyDeletedUpdatedSystemApps.PushBack(ps->mName);
                        }
                        ++it;
                    }
                }
            }

            //look for any incomplete package installations
            AutoPtr<List< AutoPtr<PackageSetting> > > deletePkgsList = mSettings->GetListOfIncompleteInstallPackagesLPr();
            //clean up list
            List< AutoPtr<PackageSetting> >::Iterator dit;
            for (dit = deletePkgsList->Begin(); dit != deletePkgsList->End(); ++dit) {
                //clean up here
                CleanupInstallFailedPackage(*dit);
            }
            //delete tmp files
            DeleteTempPackageFiles();

            // Remove any shared userIDs that have no associated packages
            mSettings->PruneSharedUsersLPw();

            if (!mOnlyCore) {
    //             EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_DATA_SCAN_START,
    //                     SystemClock.uptimeMillis());
                ScanDirLI(mAppInstallDir, 0, scanFlags, 0, readBuffer);

                ScanDirLI(mDrmAppPrivateInstallDir, PackageParser::PARSE_FORWARD_LOCK,
                        scanFlags, 0, readBuffer);

                /**
                 * Remove disable package settings for any updated system
                 * apps that were removed via an OTA. If they're not a
                 * previously-updated app, remove them completely.
                 * Otherwise, just revoke their system-level permissions.
                 */
                List<String>::Iterator pit;
                for (pit = possiblyDeletedUpdatedSystemApps.Begin(); pit != possiblyDeletedUpdatedSystemApps.End(); ++pit) {
                    String deletedAppName = *pit;
                    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator pkgit = mPackages.Find(deletedAppName);
                    AutoPtr<PackageParser::Package> deletedPkg;
                    if (pkgit != mPackages.End()) deletedPkg = pkgit->mSecond;
                    mSettings->RemoveDisabledSystemPackageLPw(deletedAppName);

                    String msg;
                    if (deletedPkg == NULL) {
                        msg = String("Updated system package ") + deletedAppName
                                + " no longer exists; wiping its data";
                        RemoveDataDirsLI(deletedAppName);
                    }
                    else {
                        msg = String("Updated system app + ") + deletedAppName
                                + " no longer present; removing system privileges for "
                                + deletedAppName;

                        Int32 flags;
                        deletedPkg->mApplicationInfo->GetFlags(&flags);
                        deletedPkg->mApplicationInfo->SetFlags(flags & ~IApplicationInfo::FLAG_SYSTEM);

                        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(deletedAppName);
                        assert(it != mSettings->mPackages.End());
                        AutoPtr<PackageSetting> deletedPs = it->mSecond;
                        deletedPs->mPkgFlags &= ~IApplicationInfo::FLAG_SYSTEM;
                    }
                    LogCriticalInfo(ILogHelper::WARN, msg);
                }

                /**
                 * Make sure all system apps that we expected to appear on
                 * the userdata partition actually showed up. If they never
                 * appeared, crawl back and revive the system version.
                 */
                Map<String, AutoPtr<IFile> >::Iterator betterIt = expectingBetter.Begin();
                for (; betterIt != expectingBetter.End(); ++betterIt) {
                    String packageName = betterIt->mFirst;
                    if (mPackages.Find(packageName) == mPackages.End()) {
                        AutoPtr<IFile> scanFile = betterIt->mSecond;

                        LogCriticalInfo(ILogHelper::WARN, String("Expected better ") + packageName
                                + " but never showed up; reverting to system");

                        Int32 reparseFlags;
                        if (FileUtils::Contains(privilegedAppDir, scanFile)) {
                            reparseFlags = PackageParser::PARSE_IS_SYSTEM
                                    | PackageParser::PARSE_IS_SYSTEM_DIR
                                    | PackageParser::PARSE_IS_PRIVILEGED;
                        }
                        else if (FileUtils::Contains(systemAppDir, scanFile)) {
                            reparseFlags = PackageParser::PARSE_IS_SYSTEM
                                    | PackageParser::PARSE_IS_SYSTEM_DIR;
                        }
                        else if (FileUtils::Contains(vendorAppDir, scanFile)) {
                            reparseFlags = PackageParser::PARSE_IS_SYSTEM
                                    | PackageParser::PARSE_IS_SYSTEM_DIR;
                        }
                        else if (FileUtils::Contains(oemAppDir, scanFile)) {
                            reparseFlags = PackageParser::PARSE_IS_SYSTEM
                                    | PackageParser::PARSE_IS_SYSTEM_DIR;
                        }
                        else {
                            Slogger::E(TAG, "Ignoring unexpected fallback path %p", scanFile.Get());
                            continue;
                        }

                        mSettings->EnableSystemPackageLPw(packageName);

                        // try {
                        AutoPtr<PackageParser::Package> pkg;
                        ECode ec = ScanPackageLI(scanFile, reparseFlags, scanFlags, 0,
                                NULL, readBuffer, (PackageParser::Package**)&pkg);
                        if (FAILED(ec)) {
                            Slogger::E(TAG, "Failed to parse original system package: 0x%08x", ec);
                        }
                        // } catch (PackageManagerException e) {
                        //     Slog.e(TAG, "Failed to parse original system package: "
                        //             + e.getMessage());
                        // }
                    }
                }
            }

            // Now that we know all of the shared libraries, update all clients to have
            // the correct library paths.
            UpdateAllSharedLibrariesLPw();
            HashMap<String, AutoPtr<SharedUserSetting> >& col = mSettings->GetAllSharedUsersLPw();
            HashMap<String, AutoPtr<SharedUserSetting> >::Iterator colIt = col.Begin();
            for (; colIt != col.End(); ++colIt) {
                AutoPtr<SharedUserSetting> userSetting = colIt->mSecond;
                // NOTE: We ignore potential failures here during a system scan (like
                // the rest of the commands above) because there's precious little we
                // can do about it. A settings error is reported, though.
                AdjustCpuAbisForSharedUserLPw(userSetting->mPackages, NULL /* scanned package */,
                        FALSE /* force dexopt */, FALSE /* defer dexopt */);
            }
            while (col->HasNext(&hasNext), hasNext) {

            }

            // Now that we know all the packages we are keeping,
            // read and update their last usage times.
            mPackageUsage->ReadLP();


//         EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_SCAN_END,
//                 SystemClock.uptimeMillis());
            Slogger::I(TAG, " ==== Time to scan packages: %lld ms", SystemClock::GetUptimeMillis() - startTime);

            // scan package of elastos
            //todo: parse Elastos apps
            CApplicationInfo::New((IApplicationInfo**)&mElastosApplication);
            mElastosApplication->SetProcessName(String("SystemServer"));
            mElastosApplication->SetPackageName(String("Elastos.Droid"));


            // If the platform SDK has changed since the last time we booted,
            // we need to re-grant app permission to catch any new ones that
            // appear.  This is really a hack, and means that apps can in some
            // cases get permissions that the user didn't initially explicitly
            // allow...  it would be nice to have some better way to handle
            // this situation.
            const Boolean regrantPermissions = mSettings->mInternalSdkPlatform
                    != mSdkVersion;
            if (regrantPermissions) {
                Slogger::I(TAG, "Platform changed from %d to %d; regranting permissions for internal storage",
                    mSettings->mInternalSdkPlatform , mSdkVersion);
            }
            mSettings->mInternalSdkPlatform = mSdkVersion;

            UpdatePermissionsLPw(String(NULL), NULL, UPDATE_PERMISSIONS_ALL
                    | (regrantPermissions
                            ? (UPDATE_PERMISSIONS_REPLACE_PKG | UPDATE_PERMISSIONS_REPLACE_ALL)
                            : 0));

            // If this is the first boot, and it is a normal boot, then
            // we need to initialize the default preferred apps.
            if (!mRestoredSettings && !onlyCore) {
                mSettings->ReadDefaultPreferredAppsLPw(this, 0);
            }

            // If this is first boot after an OTA, and a normal boot, then
            // we need to clear code cache directories.
            if (!Build::FINGERPRINT.Equals(mSettings->mFingerprint) && !onlyCore) {
                Slogger::I(TAG, "Build fingerprint changed; clearing code caches");
                HashMap<String, AutoPtr<PackageSetting> >::Iterator pkgIt = mSettings->mPackages.Begin();
                for (; pkgIt != mSettings->mPackages.End(); ++pkgIt) {
                    DeleteCodeCacheDirsLI(pkgIt->mFirst);
                }
                mSettings->mFingerprint = Build::FINGERPRINT;
            }

            // All the changes are done during package scanning.
            mSettings->UpdateInternalDatabaseVersion();

            // can downgrade to reader
            mSettings->WriteLPr();

//         EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_READY,
//                 SystemClock.uptimeMillis());

            mRequiredVerifierPackage = GetRequiredVerifierLPr();
        } // synchronized (mPackagesLock)
    } // synchronized (mInstallLock)

    mInstallerService = new PackageInstallerService(context, this, mAppInstallDir);

    // Now after opening every single application zip, make sure they
    // are all flushed.  Not really needed, but keeps things nice and
    // tidy.
    // Runtime.getRuntime().gc();

    return NOERROR;
}

ECode CPackageManagerService::IsFirstBoot(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = !mRestoredSettings;
    return NOERROR;
}

ECode CPackageManagerService::IsOnlyCoreApps(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mOnlyCore;
    return NOERROR;
}

String CPackageManagerService::GetRequiredVerifierLPr()
{
    AutoPtr<IIntent> verification;
    CIntent::New(IIntent::ACTION_PACKAGE_NEEDS_VERIFICATION, (IIntent**)&verification);
    AutoPtr<IObjectContainer> receivers;
    QueryIntentReceivers(verification, PACKAGE_MIME_TYPE,
            IPackageManager::GET_DISABLED_COMPONENTS, 0 /* TODO: Which userId? */, (IObjectContainer**)&receivers);

    String requiredVerifier;

    AutoPtr<IObjectEnumerator> enumerator;
    receivers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        enumerator->Current((IInterface**)&obj);
        AutoPtr<IResolveInfo> info = IResolveInfo::Probe(obj);

        AutoPtr<IActivityInfo> aInfo;
        info->GetActivityInfo((IActivityInfo**)&aInfo);
        if (aInfo == NULL) {
            continue;
        }

        String packageName;
        aInfo->GetPackageName(&packageName);

        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        AutoPtr<PackageSetting> ps;
        if (it != mSettings->mPackages.End()) ps = it->mSecond;
        if (ps == NULL) {
            continue;
        }

        AutoPtr<GrantedPermissions> gp = ps->mSharedUser != NULL ? ps->mSharedUser : ps;
        if (gp->mGrantedPermissions.Find(Elastos::Droid::Manifest::permission::PACKAGE_VERIFICATION_AGENT)
                    == ps->mGrantedPermissions.End()) {
            continue;
        }

        if (!requiredVerifier.IsNull()) {
            // throw new RuntimeException("There can be only one required verifier");
            Slogger::E(TAG, "There can be only one required verifier");
        }

        requiredVerifier = packageName;
    }

    return requiredVerifier;
}

void CPackageManagerService::CleanupInstallFailedPackage(
    /* [in] */ PackageSetting* ps)
{
    LogCriticalInfo(ILogHelper::WARN, String("Cleaning up incompletely installed app: ") + ps->mName);

    RemoveDataDirsLI(ps->mName);
    if (ps->mCodePath != NULL) {
        Boolean isDirectory;
        if (ps->mCodePath->IsDirectory(&isDirectory), isDirectory) {
            FileUtils::DeleteContents(ps->mCodePath);
        }
        Boolean succeeded;
        ps->mCodePath->Delete(&succeeded);
    }
    Boolean equals;
    if (ps->mResourcePath != NULL
            && (IObject::Probe(ps->mResourcePath)->Equals(ps->mCodePath, &equals), !equals)) {
        Boolean isDirectory;
        if (ps->mResourcePath->IsDirectory(&isDirectory), isDirectory) {
            FileUtils::DeleteContents(ps->mResourcePath);
        }
        Boolean succeeded;
        ps->mResourcePath->Delete(&succeeded);
    }
    mSettings->RemovePackageLPw(ps->mName);
}

void CPackageManagerService::ReadPermissionsFromXml(
    /* [in] */ IFile* permFile)
{
    AutoPtr<IFileReader> permReader;
    if (FAILED(CFileReader::New(permFile, (IFileReader**)&permReader))) {
        // Slog.w(TAG, "Couldn't find or open permissions file " + permFile);
        return;
    }

    // try {
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInput(permReader);

    FAIL_GOTO(XmlUtils::BeginDocument(parser, String("permissions")), Exception);

    while (TRUE) {
        FAIL_GOTO(XmlUtils::NextElement(parser), Exception);
        Int32 type = 0;
        if (parser->GetEventType(&type), type == IXmlPullParser::END_DOCUMENT) {
            break;
        }

        String name;
        parser->GetName(&name);
        if (String("group").Equals(name)) {
            String gidStr;
            parser->GetAttributeValue(String(NULL), String("gid"), &gidStr);
            if (!gidStr.IsNull()) {
                Int32 gid = StringUtils::ParseInt32(gidStr);
                mGlobalGids = AppendInt(mGlobalGids.Get(), gid);
            }
            else {
                // Slog.w(TAG, "<group> without gid at "
                //         + parser.getPositionDescription());
            }

            FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
            continue;
        }
        else if (String("permission").Equals(name)) {
            String perm;
            parser->GetAttributeValue(String(NULL), String("name"), &perm);
            if (perm.IsNull()) {
                // Slog.w(TAG, "<permission> without name at "
                //         + parser.getPositionDescription());
                FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
                continue;
            }
            ReadPermission(parser, perm);
        }
        else if (String("assign-permission").Equals(name)) {
            String perm;
            parser->GetAttributeValue(String(NULL), String("name"), &perm);
            if (perm.IsNull()) {
                String parserPosition;
                parser->GetPositionDescription(&parserPosition);
                Slogger::W(TAG, "<assign-permission> without name at parserPosition:%s", parserPosition.string());
                FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
                continue;
            }
            String uidStr;
            parser->GetAttributeValue(String(NULL), String("uid"), &uidStr);
            if (uidStr.IsNull()) {
                String parserPosition;
                parser->GetPositionDescription(&parserPosition);
                Slogger::W(TAG, "<assign-permission> without uid at parserPosition:%s", parserPosition.string());
                FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
                continue;
            }
            Int32 uid = Process::GetUidForName(uidStr);
            if (uid < 0) {
                String parserPosition;
                parser->GetPositionDescription(&parserPosition);
                Slogger::W(TAG, "<assign-permission> with unknown uid \" uidStr:%s \" at parserPosition:%s "
                    ,uidStr.string(), parserPosition.string());
                FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
                continue;
            }

            AutoPtr< HashSet<String> > perms = mSystemPermissions[uid];
            if (perms == NULL) {
                perms = new HashSet<String>();
                mSystemPermissions[uid] = perms;
            }
            perms->Insert(perm);
            FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
        }
        else if (String("library").Equals(name)) {
            String lname, lfile;
            parser->GetAttributeValue(String(NULL), String("name"), &lname);
            parser->GetAttributeValue(String(NULL), String("file"), &lfile);
            if (lname.IsNull()) {
                // Slog.w(TAG, "<library> without name at "
                //         + parser.getPositionDescription());
            }
            else if (lfile.IsNull()) {
                // Slog.w(TAG, "<library> without file at "
                //         + parser.getPositionDescription());
            }
            else {
                //Logger::I(TAG, "Got library " + lname + " in " + lfile);
                mSharedLibraries[lname] = lfile;
            }
            FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
            continue;
        }
        else if (String("feature").Equals(name)) {
            String fname;
            parser->GetAttributeValue(String(NULL), String("name"), &fname);
            if (fname.IsNull()) {
                // Slog.w(TAG, "<feature> without name at "
                //         + parser.getPositionDescription());
            }
            else {
                //Logger::I(TAG, "Got feature " + fname);
                AutoPtr<IFeatureInfo> fi;
                ASSERT_SUCCEEDED(CFeatureInfo::New((IFeatureInfo**)&fi));
                fi->SetName(fname);
                mAvailableFeatures[fname] = fi;
            }
            FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
            continue;
        }
        else {
            FAIL_GOTO(XmlUtils::SkipCurrentTag(parser), Exception);
            continue;
        }
    }
    ICloseable::Probe(permReader)->Close();
    return;
    // } catch (XmlPullParserException e) {
    //     Slog.w(TAG, "Got execption parsing permissions.", e);
    // } catch (IOException e) {
    //     Slog.w(TAG, "Got execption parsing permissions.", e);
    // }
Exception:
    Slogger::W(TAG, "Got execption parsing permissions.");
}

void CPackageManagerService::ReadPermission(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& name)
{
    AutoPtr<BasePermission> bp;
    HashMap<String, AutoPtr<BasePermission> >::Iterator it =
            mSettings->mPermissions.Find(name);
    if (it != mSettings->mPermissions.End()) {
        bp = it->mSecond;
    }

    if (bp == NULL) {
        bp = new BasePermission(name, String(NULL), BasePermission::TYPE_BUILTIN);
        mSettings->mPermissions[name] = bp;
    }
    Int32 type = 0, outerDepth = 0, depth = 0;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG
                || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (String("group").Equals(tagName)) {
            String gidStr;
            parser->GetAttributeValue(String(NULL), String("gid"), &gidStr);
            if (!gidStr.IsNull()) {
                Int32 gid = Process::GetGidForName(gidStr);
                bp->mGids = AppendInt(bp->mGids, gid);
            }
            else {
                String ps;
                Slogger::W(TAG, "<group> without gid at %s", (parser->GetPositionDescription(&ps), ps.string()));
            }
        }
        XmlUtils::SkipCurrentTag(parser);
    }
}

AutoPtr< ArrayOf<Int32> > CPackageManagerService::AppendInt(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ Int32 val)
{
    AutoPtr< ArrayOf<Int32> > ret;
    if (cur == NULL) {
        ret = ArrayOf<Int32>::Alloc(1);
        (*ret)[0] = val;
        return ret;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            return cur;
        }
    }
    ret = ArrayOf<Int32>::Alloc(N + 1);
    ret->Copy(cur, N);
    (*ret)[N] = val;
    return ret;
}

AutoPtr< ArrayOf<Int32> > CPackageManagerService::AppendInts(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ ArrayOf<Int32>* add)
{
    if (add == NULL) return cur;
    if (cur == NULL) return add;
    AutoPtr< ArrayOf<Int32> > ret = cur;
    const Int32 N = add->GetLength();
    for (Int32 i = 0; i < N; i++) {
        ret = AppendInt(ret, (*add)[i]);
    }
    return ret;
}

AutoPtr< ArrayOf<Int32> > CPackageManagerService::RemoveInt(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ Int32 val)
{
    if (cur == NULL) {
        return NULL;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            AutoPtr< ArrayOf<Int32> > ret = ArrayOf<Int32>::Alloc(N - 1);
            if (i > 0) {
                ret->Copy(cur, 0, i);
            }
            if (i < (N - 1)) {
                ret->Copy(i, cur, i + 1, N - i - 1);
            }
            return ret;
        }
    }
    return cur;
}

AutoPtr< ArrayOf<Int32> > CPackageManagerService::RemoveInts(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ ArrayOf<Int32>* rem)
{
    if (rem == NULL) return cur;
    if (cur == NULL) return cur;
    AutoPtr< ArrayOf<Int32> > ret = cur;
    const Int32 N = rem->GetLength();
    for (Int32 i = 0; i < N; i++) {
        ret = RemoveInt(ret, (*rem)[i]);
    }
    return ret;
}

AutoPtr<IPackageInfo> CPackageManagerService::GeneratePackageInfo(
    /* [in] */ PackageParser::Package* p,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    AutoPtr<PackageSetting> ps;
    if (p->mExtras != NULL) {
        ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
    }
    if (ps == NULL) {
        return NULL;
    }
    AutoPtr<GrantedPermissions> gp = ps->mSharedUser != NULL ?
            (GrantedPermissions*)ps->mSharedUser.Get() : (GrantedPermissions*)ps.Get();
    AutoPtr<PackageUserState> state = ps->ReadUserState(userId);
    return PackageParser::GeneratePackageInfo(p, g->mGids, flags,
            ps->mFirstInstallTime, ps->mLastUpdateTime, &gp->mGrantedPermissions,
            state, userId);
}

ECode CPackageManagerService::IsPackageAvailable(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!sUserManager->Exists(userId)) {
        *result = FALSE;
        return NOERROR;
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("is package available")))
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        AutoPtr<PackageParser::Package> p = (it != mPackages.End()) ? it->mSecond : NULL;
        if (p != NULL) {
            AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
            if (ps != NULL) {
                AutoPtr<PackageUserState> state = ps->ReadUserState(userId);
                if (state != NULL) {
                    *result = PackageParser::IsAvailable(state);
                }
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode CPackageManagerService::GetPackageInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IPackageInfo** pkgInfo)
{
    VALIDATE_NOT_NULL(pkgInfo)
    *pkgInfo = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get package info")))
    // reader
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> p;
        if (!packageName.IsNullOrEmpty()) {
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
            if (it != mPackages.End()) {
                p = it->mSecond;
            }
        }
        if (DEBUG_PACKAGE_INFO)
            Logger::V(TAG, "getPackageInfo %s: %p", packageName.string(), p.Get());
        if (p != NULL) {
            AutoPtr<IPackageInfo> pi = GeneratePackageInfo(p, flags, userId);
            *pkgInfo = pi;
            REFCOUNT_ADD(*pkgInfo)
            return NOERROR;
        }
        if((flags & IPackageManager::GET_UNINSTALLED_PACKAGES) != 0) {
            AutoPtr<IPackageInfo> pi = GeneratePackageInfoFromSettingsLPw(packageName, flags, userId);
            *pkgInfo = pi;
            REFCOUNT_ADD(*pkgInfo)
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CPackageManagerService::CurrentToCanonicalPackageNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr< ArrayOf<String> > out = ArrayOf<String>::Alloc(names.GetLength());
    synchronized (mPackagesLock) {
        for (Int32 i = names.GetLength() - 1; i >= 0; i--) {
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
                    mSettings->mPackages.Find(names[i]);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            (*out)[i] = ps != NULL && !ps->mRealName.IsNull() ? ps->mRealName : names[i];
        }
    }
    *result = out;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPackageManagerService::CanonicalToCurrentPackageNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr< ArrayOf<String> > out = ArrayOf<String>::Alloc(names.GetLength());
    synchronized (mPackagesLock) {
        for (Int32 i = names.GetLength() - 1; i >= 0; i--) {
            String cur;
            HashMap<String, String>::Iterator it =
                    mSettings->mRenamedPackages.Find(names[i]);
            if (it != mSettings->mRenamedPackages.End()) {
                cur = it->mSecond;
            }
            (*out)[i] = !cur.IsNull() ? cur : names[i];
        }
    }
    *result = out;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPackageManagerService::GetPackageUid(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid)

    if (!sUserManager->Exists(userId)) {
        *uid = -1;
        return NOERROR;
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get package uid")))
    // reader
    synchronized (mPackagesLock) {

        AutoPtr<PackageParser::Package> p;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            p = it->mSecond;
        }
        if(p != NULL) {
            Int32 appUid;
            p->mApplicationInfo->GetUid(&appUid);
            *uid = UserHandle::GetUid(userId, appUid);
            return NOERROR;
        }
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator psit =
                mSettings->mPackages.Find(packageName);
        if (psit != mSettings->mPackages.End()) {
            ps = psit->mSecond;
        }
        if((ps == NULL) || (ps->mPkg == NULL) || (ps->mPkg->mApplicationInfo == NULL)) {
            *uid = -1;
            return NOERROR;
        }
        p = ps->mPkg;
        if (p != NULL) {
            Int32 appUid;
            p->mApplicationInfo->GetUid(&appUid);
            *uid = UserHandle::GetUid(userId, appUid);
            return NOERROR;
        }
        else {
            *uid = -1;
            return NOERROR;
        }
    }
}

ECode CPackageManagerService::GetPackageGids(
    /* [in] */ const String& packageName,
    /* [out, callee] */ ArrayOf<Int32>** gids)
{
    VALIDATE_NOT_NULL(gids)

    // reader
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> p;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            p = it->mSecond;
        }
        if (DEBUG_PACKAGE_INFO)
            Logger::V(TAG, "getPackageGids%s:%p", packageName.string(), p.Get());
        if (p != NULL) {
            AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
            *gids = ps->GetGids();
            REFCOUNT_ADD(*gids)
            return NOERROR;
        }
    }
    // stupid thing to indicate an error.
    *gids = ArrayOf<Int32>::Alloc(0);
    REFCOUNT_ADD(*gids)
    return NOERROR;
}

AutoPtr<IPermissionInfo> CPackageManagerService::GeneratePermissionInfo(
    /* [in] */ BasePermission* bp,
    /* [in] */ Int32 flags)
{
    if (bp->mPerm != NULL) {
        return PackageParser::GeneratePermissionInfo(bp->mPerm, flags);
    }
    AutoPtr<IPermissionInfo> pi;
    ASSERT_SUCCEEDED(CPermissionInfo::New((IPermissionInfo**)&pi))
    pi->SetName(bp->mName);
    pi->SetPackageName(bp->mSourcePackage);
    AutoPtr<ICharSequence> label;
    CString::New(bp->mName, (ICharSequence**)&label);
    pi->SetNonLocalizedLabel(label);
    pi->SetProtectionLevel(bp->mProtectionLevel);
    return pi;
}

ECode CPackageManagerService::GetPermissionInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionInfo** info)
{
    VALIDATE_NOT_NULL(info)

    // reader
    synchronized (mPackagesLock) {
        AutoPtr<BasePermission> p;
        HashMap<String, AutoPtr<BasePermission> >::Iterator it =
                mSettings->mPermissions.Find(name);
        if (it != mSettings->mPermissions.End()) {
            p = it->mSecond;
        }
        if (p != NULL) {
            AutoPtr<IPermissionInfo> pi = GeneratePermissionInfo(p, flags);
            *info = pi;
            REFCOUNT_ADD(*info)
            return NOERROR;
        }
    }
    *info = NULL;
    return NOERROR;
}

ECode CPackageManagerService::QueryPermissionsByGroup(
    /* [in] */ const String& group,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)
    *infos = NULL;

    // reader
    synchronized (mPackagesLock) {
        AutoPtr<IArrayList> al;
        CArrayList::New((IArrayList**)&al));
        AutoPtr<IList> out = IList::Probe(al);

        HashMap<String, AutoPtr<BasePermission> >::Iterator it;
        for (it = mSettings->mPermissions.Begin(); it != mSettings->mPermissions.End(); ++it) {
            AutoPtr<BasePermission> p = it->mSecond;
            if (group.IsNull()) {
                String pGroup;
                if (p->mPerm == NULL || (p->mPerm->mInfo->GetGroup(&pGroup), pGroup.IsNull())) {
                    AutoPtr<IPermissionInfo> info = GeneratePermissionInfo(p, flags);
                    out->Add(info);
                }
            }
            else {
                String pGroup;
                if (p->mPerm != NULL &&
                        (p->mPerm->mInfo->GetGroup(&pGroup), group.Equals(pGroup))) {
                    AutoPtr<IPermissionInfo> info = PackageParser::GeneratePermissionInfo(p->mPerm, flags);
                    out->Add(info);
                }
            }
        }

        Int32 size;
        if (out->GetSize(&size), size > 0) {
            *infos = out;
            REFCOUNT_ADD(*infos)
            return NOERROR;
        }
        *infos = mPermissionGroups.Find(group) != mPermissionGroups.End() ? out : NULL;
        REFCOUNT_ADD(*infos)
    }
    return NOERROR;
}

ECode CPackageManagerService::GetPermissionGroupInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionGroupInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    if (name.IsNull()) {
        return NOERROR;
    }

    // reader
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::PermissionGroup> pg;
        HashMap<String, AutoPtr<PackageParser::PermissionGroup> >::Iterator it =
                mPermissionGroups.Find(name);
        if (it != mPermissionGroups.End()) {
            pg = it->mSecond;
        }
        AutoPtr<IPermissionGroupInfo> pgi =
                PackageParser::GeneratePermissionGroupInfo(pg, flags);
        *info = pgi;
        REFCOUNT_ADD(*info)
    }
    return NOERROR;
}

ECode CPackageManagerService::GetAllPermissionGroups(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)

    // reader
    synchronized (mPackagesLock) {
        AutoPtr<IArrayList> al;
        CArrayList::New((IArrayList**)&al));
        AutoPtr<IList> out = IList::Probe(al);
        HashMap<String, AutoPtr<PackageParser::PermissionGroup> >::Iterator it;
        for (it = mPermissionGroups.Begin(); it != mPermissionGroups.End(); ++it) {
            AutoPtr<PackageParser::PermissionGroup> pg = it->mSecond;
            AutoPtr<IPermissionGroupInfo> pgi = PackageParser::GeneratePermissionGroupInfo(pg, flags);
            out->Add(pgi);
        }
        *infos = out;
        REFCOUNT_ADD(*infos)
    }
    return NOERROR;
}

AutoPtr<IApplicationInfo> CPackageManagerService::GenerateApplicationInfoFromSettingsLPw(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    AutoPtr<PackageSetting> ps;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
            mSettings->mPackages.Find(packageName);
    if (it != mSettings->mPackages.End()) {
        ps = it->mSecond;
    }
    if(ps != NULL) {
        if(ps->mPkg == NULL) {
            AutoPtr<IPackageInfo> pInfo = GeneratePackageInfoFromSettingsLPw(
                    packageName, flags, userId);
            if(pInfo != NULL) {
                AutoPtr<IApplicationInfo> appInfo;
                pInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                return appInfo;
            }
            return NULL;
        }
        return PackageParser::GenerateApplicationInfo(ps->mPkg, flags,
                ps->ReadUserState(userId), userId);
    }
    return NULL;
}

AutoPtr<IPackageInfo> CPackageManagerService::GeneratePackageInfoFromSettingsLPw(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NULL;
    AutoPtr<PackageSetting> ps;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
            mSettings->mPackages.Find(packageName);
    if (it != mSettings->mPackages.End()) {
        ps = it->mSecond;
    }
    if(ps != NULL) {
        AutoPtr<PackageParser::Package> pkg = ps->mPkg;
        if(pkg == NULL) {
            if ((flags & IPackageManager::GET_UNINSTALLED_PACKAGES) == 0) {
                return NULL;
            }
            // Only data remains, so we aren't worried about code paths
            pkg = new PackageParser::Package(packageName);
            pkg->mApplicationInfo->SetPackageName(packageName);
            pkg->mApplicationInfo->SetFlags(ps->mPkgFlags | IApplicationInfo::FLAG_IS_DATA_ONLY);
            String dataDir;
            GetDataPathForPackage(packageName, 0)->GetPath(&dataDir);
            pkg->mApplicationInfo->SetDataDir(dataDir);
            pkg->mApplicationInfo->SetPrimaryCpuAbi(ps->mPrimaryCpuAbiString);
            pkg->mApplicationInfo->SetSecondaryCpuAbi(ps->mSecondaryCpuAbiString);
        }
        return GeneratePackageInfo(pkg, flags, userId);
    }
    return NULL;
}

ECode CPackageManagerService::GetApplicationInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IApplicationInfo** appInfo)
{
    VALIDATE_NOT_NULL(appInfo)
    *appInfo = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }

    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get application info")))
    // writer
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> p;
        if (!packageName.IsNull()) {
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
            if (it != mPackages.End()) {
                p = it->mSecond;
            }
        }
        if (DEBUG_PACKAGE_INFO) Logger::V(TAG, "getApplicationInfo %s: %p", packageName.string(), p.Get());
        if (p != NULL) {
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator pit =
                mSettings->mPackages.Find(packageName);
            if (pit != mSettings->mPackages.End()) {
                ps = pit->mSecond;
            }
            if (ps == NULL) {
                return NOERROR;
            }
            // Note: isEnabledLP() does not apply here - always return info
            AutoPtr<IApplicationInfo> info = PackageParser::GenerateApplicationInfo(
                    p, flags, ps->ReadUserState(userId), userId);
            *appInfo = info;
            REFCOUNT_ADD(*appInfo)
            return NOERROR;
        }

        if (packageName.Equals("android") || packageName.Equals("system")) {
            *appInfo = mElastosApplication;
            REFCOUNT_ADD(*appInfo)
            return NOERROR;
        }
        if((flags & IPackageManager::GET_UNINSTALLED_PACKAGES) != 0) {
            AutoPtr<IApplicationInfo> info = GenerateApplicationInfoFromSettingsLPw(packageName, flags, userId);
            *appInfo = info;
            REFCOUNT_ADD(*appInfo)
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::FreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IPackageDataObserver* observer)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            String("Manifest.permission.CLEAR_APP_CACHE") , String(NULL)))
    // Queue up an async operation since clearing cache may take a little while.
    AutoPtr<IRunnable> runnable = new NotifyRunnable(this, freeStorageSize, observer);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

ECode CPackageManagerService::FreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            String("Manifest.permission.CLEAR_APP_CACHE"), String(NULL)))
    // Queue up an async operation since clearing cache may take a little while.
    AutoPtr<IRunnable> runnable = new FreeStorageRunnable(this, freeStorageSize, pi);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

ECode CPackageManagerService::FreeStorage(
    /* [in] */ Int64 freeStorageSize)
{
    synchronized (mInstallLock) {
        if (mInstaller->FreeCache(freeStorageSize) < 0) {
            Slogger::E(TAG, "Failed to free enough space");
            return E_IO_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::GetActivityInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IActivityInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get activity info")))

    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Activity> a;
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Activity> >::Iterator it =
                mActivities->mActivities.Find(component);
        if (it != mActivities->mActivities.End()) {
            a = it->mSecond;
        }

        // if (DEBUG_PACKAGE_INFO) Log.v(TAG, "getActivityInfo " + component + ": " + a);
        if (a != NULL && mSettings->IsEnabledLPr((IComponentInfo*)a->mInfo.Get(), flags, userId)) {
            String pkgName;
            component->GetPackageName(&pkgName);
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
                mSettings->mPackages.Find(pkgName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            if (ps == NULL) {
                return NOERROR;
            }
            AutoPtr<IActivityInfo> ai = PackageParser::GenerateActivityInfo(a, flags,
                    ps->ReadUserState(userId), userId);
            *info = ai;
            REFCOUNT_ADD(*info)
            return NOERROR;
        }
        Boolean isEqual = FALSE;
        if (mResolveComponentName->Equals(component, &isEqual), isEqual) {
            AutoPtr<PackageUserState> state = new PackageUserState();
            *info = PackageParser::GenerateActivityInfo(mResolveActivity, flags, state, userId);
            REFCOUNT_ADD(*info)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::ActivitySupportsIntent(
    /* [in] */ IComponentName* className,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    synchronized (mPackagesLock) {
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Activity> >::Iterator it
                = mActivities->mActivities->Find(component);
        AutoPtr<PackageParser::Activity> a = (it != mActivities->mActivities.End()) ? it->mSecond, NULL;
        if (a == NULL) {
            *result = FALSE;
            return NOERROR;
        }
        List< AutoPtr<II> >::Iterator intentIt = a->mIntents.Begin();
        for (; intentIt != a->mIntents.End(); ++intentIt) {
            String action, scheme;
            intent->GetAction(&action);
            intent->GetScheme(&scheme);
            AutoPtr<IUri> data;
            intent->GetData((IUri**)&data);
            AutoPtr< ArrayOf<String> > categories;
            intent->GetCategories((ArrayOf<String>**)&categories);
            Int32 value;
            if (IIntentFilter::Probe(*intentIt)->Match(action, resolvedType,
                    scheme, data, categories, TAG, &value), value >= 0) {
                *result = TRUE;
                return NOERROR;
            }
        }
        *result = FALSE;
    }
    return NOERROR;
}

ECode CPackageManagerService::GetReceiverInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IActivityInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get receiver info")))

    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Activity> a;
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Activity> >::Iterator it =
                mReceivers->mActivities.Find(component);
        if (it != mReceivers->mActivities.End()) {
            a = it->mSecond;
        }
        if (DEBUG_PACKAGE_INFO) Logger::V(TAG, "getReceiverInfo %p: %p", component, a.Get());
        if (a != NULL && mSettings->IsEnabledLPr((IComponentInfo*)a->mInfo.Get(), flags, userId)) {
            String pkgName;
            component->GetPackageName(&pkgName);
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
                mSettings->mPackages.Find(pkgName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            if (ps == NULL) {
                return NOERROR;
            }
            AutoPtr<IActivityInfo> ai = PackageParser::GenerateActivityInfo(a, flags,
                    ps->ReadUserState(userId), userId);
            *info = ai;
            REFCOUNT_ADD(*info)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::GetServiceInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IServiceInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }

    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get service info")))

    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Service> s;
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Service> >::Iterator it =
                mServices->mServices.Find(component);
        if (it != mServices->mServices.End()) {
            s = it->mSecond;
        }
        if (DEBUG_PACKAGE_INFO) Logger::V(TAG, "getServiceInfo %p: %p", component, s.Get());
        if (s != NULL && mSettings->IsEnabledLPr(IComponentInfo::Probe(s->mInfo), flags, userId)) {
            String pkgName;
            component->GetPackageName(&pkgName);
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
                mSettings->mPackages.Find(pkgName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            if (ps == NULL) {
                return NOERROR;
            }
            AutoPtr<IServiceInfo> si = PackageParser::GenerateServiceInfo(s, flags,
                    ps->ReadUserState(userId), userId);
            *info = si;
            REFCOUNT_ADD(*info)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::GetProviderInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IProviderInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("get provider info")))

    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Provider> p;
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Provider> >::Iterator it =
                mProviders->mProviders.Find(component);
        if (it != mProviders->mProviders.End()) {
            p = it->mSecond;
        }
        if (DEBUG_PACKAGE_INFO) Logger::V(TAG, "getProviderInfo %p: %p", component, p.Get());
        if (p != NULL && mSettings->IsEnabledLPr((IComponentInfo*)p->mInfo.Get(), flags, userId)) {
            String pkgName;
            component->GetPackageName(&pkgName);
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
                mSettings->mPackages.Find(pkgName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            if (ps == NULL) {
                return NOERROR;
            }
            AutoPtr<IProviderInfo> pi = PackageParser::GenerateProviderInfo(p, flags,
                    ps->ReadUserState(userId), userId);
            *info = pi;
            REFCOUNT_ADD(*info)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::GetSystemSharedLibraryNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names)

    synchronized (mPackagesLock) {
        Int32 size = mSharedLibraries.GetSize();
        if (size > 0) {
            AutoPtr< ArrayOf<String> > libs = ArrayOf<String>::Alloc(size);
            HashMap<String, String>::Iterator it;
            Int32 i;
            for (it = mSharedLibraries.Begin(), i = 0; it != mSharedLibraries.End(); ++it, ++i) {
                (*libs)[i] = it->mFirst;
            }
            *names = libs;
            REFCOUNT_ADD(*names)
            return NOERROR;
        }
        *names = NULL;
    }
    return NOERROR;
}

AutoPtr< HashMap<String, AutoPtr<IFeatureInfo> > > CPackageManagerService::GetFeaturesLocked()
{
    AutoPtr<ISystemProperties> prop;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&prop);
    Int32 hasExtra;
    prop->GetInt32(String("persist.sys.extra_features"), 1, &hasExtra);
    return (hasExtra > 0) ? mExtraFeatures : mAvailableFeatures;
}

ECode CPackageManagerService::GetSystemAvailableFeatures(
    /* [out] */ ArrayOf<IFeatureInfo*>** infos)
{
    VALIDATE_NOT_NULL(infos)

    synchronized (mPackagesLock) {
        // ActionsCode(songzhining, change code: add extra hardware feature support)
        AutoPtr< HashMap<String, AutoPtr<IFeatureInfo> > > featSet = GetFeaturesLocked();
        Int32 size = featSet->GetSize();
        if (size > 0) {
            AutoPtr< ArrayOf<IFeatureInfo*> > feats = ArrayOf<IFeatureInfo*>::Alloc(size + 1);
            HashMap<String, AutoPtr<IFeatureInfo> >::Iterator it;
            Int32 i;
            for (it = featSet->Begin(), i = 0; it != featSet->End(); ++it, ++i) {
                feats->Set(i, it->mSecond);
            }
            AutoPtr<IFeatureInfo> fi;
            FAIL_RETURN(CFeatureInfo::New((IFeatureInfo**)&fi));
            AutoPtr<ISystemProperties> sysProp;
            CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
            Int32 value;
            sysProp->GetInt32(String("ro.opengles.version"), IFeatureInfo::GL_ES_VERSION_UNDEFINED, &value);
            fi->SetReqGlEsVersion(value);
            feats->Set(size, fi);
            *infos = feats;
            REFCOUNT_ADD(*infos)
            return NOERROR;
        }
        *infos = NULL;
    }
    return NOERROR;
}

ECode CPackageManagerService::HasSystemFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    synchronized (mPackagesLock) {
        *result = mAvailableFeatures->Find(name) != mAvailableFeatures.End();
    }
    return NOERROR;
}

ECode CPackageManagerService::CheckValidCaller(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 userId)
{
    if (UserHandle::GetUserId(uid) == userId || uid == IProcess::SYSTEM_UID || uid == 0) {
        return NOERROR;
    }

    Slogger::E(TAG, "Caller uid=%d is not privileged to communicate with user=%d", uid, userId);
    return E_SECURITY_EXCEPTION;
}

ECode CPackageManagerService::CheckPermission(
    /* [in] */ const String& permName,
    /* [in] */ const String& pkgName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> p;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(pkgName);
        if (it != mPackages.End()) {
            p = it->mSecond;
        }
        if (p != NULL && p->mExtras != NULL) {
            AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
            if (ps->mSharedUser != NULL) {
                if (ps->mSharedUser->mGrantedPermissions.Find(permName)
                     != ps->mSharedUser->mGrantedPermissions.End()) {
                    *result = IPackageManager::PERMISSION_GRANTED;
                    return NOERROR;
                }
            }
            else if (ps->mGrantedPermissions.Find(permName)
                != ps->mGrantedPermissions.End()) {
                *result = IPackageManager::PERMISSION_GRANTED;
                return NOERROR;
            }
        }
    }
    *result = IPackageManager::PERMISSION_DENIED;
    return NOERROR;
}

ECode CPackageManagerService::CheckUidPermission(
    /* [in] */ const String& permName,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    synchronized (mPackagesLock) {
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(UserHandle::GetAppId(uid));
        if (obj != NULL) {
            GrantedPermissions* gp = NULL;
            if (obj->Probe(EIID_PackageSetting) != NULL) {
                PackageSetting* ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
                gp = (GrantedPermissions*)ps;
            }
            else if (obj->Probe(EIID_SharedUserSetting) != NULL) {
                SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
                gp = (GrantedPermissions*)sus;
            }
            assert(gp != NULL);

            if (gp->mGrantedPermissions.Find(permName) != gp->mGrantedPermissions.End()) {
                *result = IPackageManager::PERMISSION_GRANTED;
                return NOERROR;
            }
        }
        else {
            AutoPtr< HashSet<String> > perms;
            HashMap<Int32, AutoPtr< HashSet<String> > >::Iterator it = mSystemPermissions.Find(uid);
            if (it != mSystemPermissions.End()) {
                perms = it->mSecond;
            }
            if (perms != NULL && perms->Find(permName) != perms->End()) {
                *result = IPackageManager::PERMISSION_GRANTED;
                return NOERROR;
            }
        }
    }
    *result = IPackageManager::PERMISSION_DENIED;
    return NOERROR;
}

ECode CPackageManagerService::EnforceCrossUserPermission(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean requireFullPermission,
    /* [in] */ Boolean checkShell,
    /* [in] */ const String& message)
{
    if (userId < 0) {
        Slogger::E(TAG, "Invalid userId %d", userId);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (checkShell) {
        FAIL_RETURN(EnforceShellRestriction(IUserManager::DISALLOW_DEBUGGING_FEATURES, callingUid, userId))
    }
    if (userId == UserHandle::GetUserId(callingUid)) return NOERROR;
    if (callingUid != IProcess::SYSTEM_UID && callingUid != 0) {
        if (requireFullPermission) {
            return mContext->EnforceCallingOrSelfPermission(
                    Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS_FULL, message);
        }
        else {
            // try {
            if (FAILED(mContext->EnforceCallingOrSelfPermission(
                    Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS_FULL, message))) {
                return mContext->EnforceCallingOrSelfPermission(
                        Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS, message);
            }
            // } catch (SecurityException se) {
            //     mContext.enforceCallingOrSelfPermission(
            //             Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS, message);
            // }
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::EnforceShellRestriction(
    /* [in] */ const String& restriction,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 userHandle)
{
    if (callingUid == IProcess::SHELL_UID) {
        if (userHandle >= 0
                && sUserManager->HasUserRestriction(restriction, userHandle)) {
            Slogger::E(TAG, "Shell does not have permission to access user %d", userHandle);
            return E_SECURITY_EXCEPTION;
        }
        else if (userHandle < 0) {
            Slogger::E(TAG, "Unable to check shell permission for user %d\n\t", userHandle
                    /*Debug.getCallers(3)*/);
        }
    }
    return NOERROR;
}

AutoPtr<BasePermission> CPackageManagerService::FindPermissionTreeLP(
    /* [in] */ const String& permName)
{
    HashMap<String, AutoPtr<BasePermission> >::Iterator it;
    for (it = mSettings->mPermissionTrees.Begin();
         it != mSettings->mPermissionTrees.End(); ++it) {
        AutoPtr<BasePermission> bp = it->mSecond;
        if (permName.StartWith(bp->mName) &&
                permName.GetLength() > bp->mName.GetLength() &&
                permName.GetChar(bp->mName.GetLength()) == '.') {
            return bp;
        }
    }
    return NULL;
}

ECode CPackageManagerService::CheckPermissionTreeLP(
    /* [in] */ const String& permName,
    /* [out] */ BasePermission** permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = NULL;

    if (!permName.IsNull()) {
        AutoPtr<BasePermission> bp = FindPermissionTreeLP(permName);
        if (bp != NULL) {
             if (bp->mUid == UserHandle::GetAppId(Binder::GetCallingUid())) {
                *permission = bp;
                REFCOUNT_ADD(*permission)
                return NOERROR;
            }
            Slogger::E(TAG, "Calling uid %d is not allowed to add to permission tree %s owned by uid %d",
                    Binder::GetCallingUid(), bp->mName.string(), bp->mUid);
            return E_SECURITY_EXCEPTION;
        }
    }
    Slogger::E(TAG, "No permission tree found for %s", permName.string());
    return E_SECURITY_EXCEPTION;
}

Boolean CPackageManagerService::CompareStrings(
    /* [in] */ ICharSequence* s1,
    /* [in] */ ICharSequence* s2)
{
    if (s1 == NULL) {
        return s2 == NULL;
    }
    if (s2 == NULL) {
        return FALSE;
    }
    String str1, str2;
    s1->ToString(&str1);
    s2->ToString(&str2);
    return CompareStrings(str1, str2);
}

Boolean CPackageManagerService::CompareStrings(
    /* [in] */ const String& s1,
    /* [in] */ const String& s2)
{
    if (s1.IsNull()) {
        return s2.IsNull();
    }
    if (s2.IsNull()) {
        return FALSE;
    }
    return s1.Equals(s2);
}

Boolean CPackageManagerService::ComparePermissionInfos(
    /* [in] */ IPermissionInfo* pi1,
    /* [in] */ IPermissionInfo* pi2)
{
    assert(pi1 && pi2);

    Int32 pi1Icon, pi2Icon;
    pi1->GetIcon(&pi1Icon);
    pi2->GetIcon(&pi2Icon);
    if (pi1Icon != pi2Icon) return FALSE;

    Int32 pi1Logo, pi2Logo;
    pi1->GetLogo(&pi1Logo);
    pi2->GetLogo(&pi2Logo);
    if (pi1Logo != pi2Logo) return FALSE;

    Int32 pi1Level, pi2Level;
    pi1->GetProtectionLevel(&pi1Level);
    pi2->GetProtectionLevel(&pi2Level);
    if (pi1Level != pi2Level) return FALSE;

    String pi1Name, pi2Name;
    pi1->GetName(&pi1Name);
    pi2->GetName(&pi2Name);
    if (!CompareStrings(pi1Name, pi2Name)) return FALSE;

    AutoPtr<ICharSequence> pi1Label, pi2Label;
    pi1->GetNonLocalizedLabel((ICharSequence**)&pi1Label);
    pi2->GetNonLocalizedLabel((ICharSequence**)&pi2Label);
    if (!CompareStrings(pi1Label, pi2Label)) return FALSE;

    // We'll take care of setting this one.
    String pi1PName, pi2PName;
    pi1->GetPackageName(&pi1PName);
    pi2->GetPackageName(&pi2PName);
    if (!CompareStrings(pi1PName, pi2PName)) return FALSE;
    // These are not currently stored in settings.
    //if (!compareStrings(pi1.group, pi2.group)) return false;
    //if (!compareStrings(pi1.nonLocalizedDescription, pi2.nonLocalizedDescription)) return false;
    //if (pi1.labelRes != pi2.labelRes) return false;
    //if (pi1.descriptionRes != pi2.descriptionRes) return false;
    return TRUE;
}

Int32 CPackageManagerService::PermissionInfoFootprint(
    /* [in] */ IPermissionInfo* info)
{
    String name;
    info->GetName(&name);
    Int32 size = name.GetLength();
    AutoPtr<IPackageItemInfo> pkgInfo = IPackageItemInfo::Probe(IPermissionInfo);
    AutoPtr<ICharSequence> label;
    pkgInfo->GetNonLocalizedLabel((ICharSequence**)&label);
    if (nonLocalizedLabel != NULL) {
        Int32 len;
        label->GetLength(&len);
        size += len;
    }
    AutoPtr<ICharSequence> des;
    info->GetNonLocalizedDescription((ICharSequence**)&des);
    if (des != NULL) {
        Int32 len;
        des->GetLength(&len);
        size += len;
    }
    return size;
}

Int32 CPackageManagerService::CalculateCurrentPermissionFootprintLocked(
    /* [in] */ BasePermission* tree)
{
    Int32 size = 0;
    HashMap<String, AutoPtr<BasePermission> >::Iterator it = mSettings->mPermissions.Begin();
    for (; it != mSettings->mPermissions.End(); ++it) {
        AutoPtr<BasePermission> perm = it->mSecond;
    }
    for (BasePermission perm : mSettings.mPermissions.values()) {
        if (perm->mUid == tree->mUid) {
            size += perm->mName.GetLength() + PermissionInfoFootprint(perm->mPerm->mInfo);
        }
    }
    return size;
}

ECode CPackageManagerService::EnforcePermissionCapLocked(
    /* [in] */ IPermissionInfo* info,
    /* [in] */ BasePermission* tree)
{
    // We calculate the max size of permissions defined by this uid and throw
    // if that plus the size of 'info' would exceed our stated maximum.
    if (tree->mUid != IProcess::SYSTEM_UID) {
        Int32 curTreeSize = CalculateCurrentPermissionFootprintLocked(tree);
        if (curTreeSize + PermissionInfoFootprint(info) > MAX_PERMISSION_TREE_FOOTPRINT) {
            Slogger::E(TAG, "Permission tree size cap exceeded");
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::AddPermissionLocked(
    /* [in] */ IPermissionInfo* info,
    /* [in] */ Boolean async,
    /* [out] */ Boolean* isAdded)
{
    VALIDATE_NOT_NULL(isAdded)
    *isAdded = FALSE;

    Int32 infoLabelRes;
    info->GetLabelRes(&infoLabelRes);
    AutoPtr<ICharSequence> infoLabel;
    info->GetNonLocalizedLabel((ICharSequence**)&infoLabel);
    if (infoLabelRes == 0 && infoLabel == NULL) {
        Slogger::E(TAG, "Label must be specified in permission");
        return E_SECURITY_EXCEPTION;
    }
    String infoName;
    info->GetName(&infoName);
    AutoPtr<BasePermission> tree;
    FAIL_RETURN(CheckPermissionTreeLP(infoName, (BasePermission**)&tree))
    AutoPtr<BasePermission> bp;
    HashMap<String, AutoPtr<BasePermission> >::Iterator it =
            mSettings->mPermissions.Find(infoName);
    if (it != mSettings->mPermissions.End()) {
        bp = it->mSecond;
    }
    Boolean added = bp == NULL;
    Boolean changed = TRUE;
    Int32 fixedLevel;
    info->GetProtectionLevel(&fixedLevel);
    AutoPtr<IPermissionInfoHelper> helper;
    CPermissionInfoHelper::AcquireSingleton((IPermissionInfoHelper**)&helper);
    helper->FixProtectionLevel(fixedLevel, &fixedLevel);
    if (added) {
        FAIL_RETURN(EnforcePermissionCapLocked(info, tree))
        bp = new BasePermission(infoName, tree->mSourcePackage,
                BasePermission::TYPE_DYNAMIC);
    }
    else if (bp->mType != BasePermission::TYPE_DYNAMIC) {
         // throw new SecurityException(
         //        "Not allowed to modify non-dynamic permission "
         //        + info.name);
        *isAdded = FALSE;
        return E_SECURITY_EXCEPTION;
    }
    else {
        if (bp->mProtectionLevel == fixedLevel
            && bp->mPerm->mOwner == tree->mPerm->mOwner
            && bp->mUid == tree->mUid
            && ComparePermissionInfos(bp->mPerm->mInfo, info)) {
            changed = FALSE;
        }
    }
    bp->mProtectionLevel = fixedLevel;
    AutoPtr<IPermissionInfo> pinfo;
    ASSERT_SUCCEEDED(CPermissionInfo::New(info, (IPermissionInfo**)&pinfo))
    pinfo->SetProtectionLevel(fixedLevel);
    bp->mPerm = new PackageParser::Permission(tree->mPerm->mOwner, pinfo);
    String treePName;
    tree->mPerm->mInfo->GetPackageName(&treePName);
    bp->mPerm->mInfo->SetPackageName(treePName);
    bp->mUid = tree->mUid;
    if (added) {
        String name;
        pinfo->GetName(&name);
        mSettings->mPermissions[name] = bp;
    }
    if (changed) {
        if (!async) {
            mSettings->WriteLPr();
        }
        else {
            ScheduleWriteSettingsLocked();
        }
    }
    *isAdded = added;
    return NOERROR;
}

ECode CPackageManagerService::AddPermission(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    VALIDATE_NOT_NULL(isAdded)
    synchronized (mPackagesLock) {
        return AddPermissionLocked(info, FALSE, isAdded);
    }
}

ECode CPackageManagerService::AddPermissionAsync(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    VALIDATE_NOT_NULL(isAdded)
    synchronized (mPackagesLock) {
        return AddPermissionLocked(info, TRUE, isAdded);
    }
}

ECode CPackageManagerService::RemovePermission(
    /* [in] */ const String& name)
{
    synchronized (mPackagesLock) {
        AutoPtr<BasePermission> tree;
        FAIL_RETURN(CheckPermissionTreeLP(name, (BasePermission**)&tree))
        AutoPtr<BasePermission> bp;
        HashMap<String, AutoPtr<BasePermission> >::Iterator it = mSettings->mPermissions.Find(name);
        if (it != mSettings->mPermissions.End()) {
            bp = it->mSecond;
        }
        if (bp != NULL) {
            if (bp->mType != BasePermission::TYPE_DYNAMIC) {
                // throw new SecurityException(
                //                 "Not allowed to modify non-dynamic permission "
                //                 + name);
                return E_SECURITY_EXCEPTION;
            }
            mSettings->mPermissions.Erase(name);
            mSettings->WriteLPr();
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::CheckGrantRevokePermissions(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ BasePermission* bp)
{
    Int32 i = 0;
    List<String>::Iterator it = Find(pkg->mRequestedPermissions.Begin(), pkg->mRequestedPermissions.End(), bp->mName);
    for (; it != pkg->mRequestedPermissions.End(); ++it, ++i) {
        if (*it == bp->mName) break;
    }
    if (it == pkg->mRequestedPermissions.End()) {
        Slogger::E(TAG, "Package %s has not requested permission %s", pkg->mPackageName.string(), bp->mName.string());
        return E_SECURITY_EXCEPTION;
    }
    Boolean isNormal =
            ((bp->mProtectionLevel & IPermissionInfo::PROTECTION_MASK_BASE)
                    == IPermissionInfo::PROTECTION_NORMAL);
    Boolean isDangerous =
            ((bp->mProtectionLevel & IPermissionInfo::PROTECTION_MASK_BASE)
                    == IPermissionInfo::PROTECTION_DANGEROUS);
    Boolean isDevelopment =
            ((bp->mProtectionLevel & IPermissionInfo::PROTECTION_FLAG_DEVELOPMENT) != 0);

    if (!isNormal && !isDangerous && !isDevelopment) {
        Slogger::E(TAG, "Permission %s is not a changeable permission type", bp->mName.string());
        return E_SECURITY_EXCEPTION;
    }

    if (isNormal || isDangerous) {
        if (pkg->mRequestedPermissionsRequired[i]) {
            Slogger::E(TAG, "Can't change %s. It is required by the application", bp->mName.string());
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::GrantPermission(
    /* [in] */ const String& packageName,
    /* [in] */ const String& permissionName)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::GRANT_REVOKE_PERMISSIONS, String(NULL)))

    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator pkgit =
                mPackages.Find(packageName);
        AutoPtr<PackageParser::Package> pkg;
        if (pkgit != mPackages.End()) pkg = pkgit->mSecond;
        if (pkg == NULL) {
            Slogger::E(TAG, "Unknown package: %p", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        HashMap<String, AutoPtr<BasePermission> >::Iterator bpit = mSettings->mPermissions.Find(permissionName);
        AutoPtr<BasePermission> bp;
        if (bpit != mSettings->mPermissions.End()) bp = bpit->mSecond;
        if (bp == NULL) {
            Slogger::E(TAG, "Unknown permission: %s", permissionName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        CheckGrantRevokePermissions(pkg, bp);

        AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(pkg->mExtras->Probe(EIID_PackageSetting));
        if (ps == NULL) {
            return NOERROR;
        }
        AutoPtr<GrantedPermissions> gp = (ps->mSharedUser != NULL) ? (GrantedPermissions*)ps->mSharedUser : (GrantedPermissions*)ps;
        if (gp->mGrantedPermissions.Find(permissionName) == gp->mGrantedPermissions.End()) {
            gp->mGrantedPermissions.Insert(permissionName);
            if (ps->mHaveGids) {
                gp->mGids = AppendInts(gp->mGids, bp->mGids);
            }
            mSettings->WriteLPr();
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::RevokePermission(
    /* [in] */ const String& packageName,
    /* [in] */ const String& permissionName)
{
    Int32 changedAppId = -1;

    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator pkgit = mPackages.Find(packageName);
        if (pkgit != mPackages.End()) pkg = pkgit->mSecond;
        if (pkg == NULL) {
            Slogger::E(TAG, "Unknown package: %s", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        Int32 uid;
        pkg->mApplicationInfo->GetUid(&uid);
        if (uid != Binder::GetCallingUid()) {
            FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
                    String("Manifest::permission::GRANT_REVOKE_PERMISSIONS"), String(NULL)))
        }
        AutoPtr<BasePermission> bp;
        HashMap<String, AutoPtr<BasePermission> >::Iterator bpit = mSettings->mPermissions.Find(permissionName);
        if (bpit != mSettings->mPermissions.End()) bp = bpit->mSecond;
        if (bp == NULL) {
            Slogger::E(TAG, "Unknown permission: %s", permissionName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        CheckGrantRevokePermissions(pkg, bp);

        AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(pkg->mExtras->Probe(EIID_PackageSetting));
        if (ps == NULL) {
            return NOERROR;
        }
        AutoPtr<GrantedPermissions> gp = (ps->mSharedUser != NULL) ? (GrantedPermissions*)ps->mSharedUser : (GrantedPermissions*)ps;
        if (gp->mGrantedPermissions.Find(permissionName) != gp->mGrantedPermissions.End()) {
            gp->mGrantedPermissions.Erase(permissionName);
            if (ps->mHaveGids) {
                gp->mGids = RemoveInts(gp->mGids, bp->mGids);
            }
            mSettings->WriteLPr();
            changedAppId = ps->mAppId;
        }

        if (changedAppId >= 0) {
            // We changed the perm on someone, kill its processes.
            AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
            if (am != NULL) {
                AutoPtr<IUserHandle> handle;
                CUserHandle::AcquireSingleton((IUserHandle**)&handle);
                Int32 callingUserId;
                handle->GetCallingUserId(&callingUserId);
                Int64 ident = Binder::ClearCallingIdentity();
                // try {
                //XXX we should only revoke for the calling user's app permissions,
                // but for now we impact all users.
                //am.killUid(UserHandle.getUid(callingUserId, changedAppId),
                //        "revoke " + permissionName);
                AutoPtr< ArrayOf<Int32> > users = sUserManager->GetUserIds();
                for (Int32 i = 0; i < users->GetLength(); ++users) {
                    Int32 uid;
                    handle->GetUid(user, changedAppId, &uid);
                    am->KillUid(uid, String("revoke ") + permissionName);
                }
                // } catch (RemoteException e) {
                // } finally {
                //     Binder.restoreCallingIdentity(ident);
                // }
                Binder::RestoreCallingIdentity(indent);
            }
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::IsProtectedBroadcast(
    /* [in] */ const String& actionName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    synchronized (mPackagesLock) {
        if (actionName.IsNull()) {
            *result = FALSE;
            return NOERROR;
        }

        *result = mProtectedBroadcasts.Find(actionName) != mProtectedBroadcasts.End();
    }
    return NOERROR;
}

ECode CPackageManagerService::CheckSignatures(
    /* [in] */ const String& pkg1,
    /* [in] */ const String& pkg2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> p1;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it1 = mPackages.Find(pkg1);
        if (it1 != mPackages.End()) {
            p1 = it1->mSecond;
        }
        AutoPtr<PackageParser::Package> p2;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it2 = mPackages.Find(pkg2);
        if (it2 != mPackages.End()) {
            p2 = it2->mSecond;
        }
        if (p1 == NULL || p1->mExtras == NULL
            || p2 == NULL || p2->mExtras == NULL) {
            *result = IPackageManager::SIGNATURE_UNKNOWN_PACKAGE;
            return NOERROR;
        }
        *result = CompareSignatures(p1->mSignatures, p2->mSignatures);
    }
    return NOERROR;
}

ECode CPackageManagerService::CheckUidSignatures(
    /* [in] */ Int32 uid1,
    /* [in] */ Int32 uid2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    // Map to base uids.
    uid1 = UserHandle::GetAppId(uid1);
    uid2 = UserHandle::GetAppId(uid2);
    //reader
    synchronized (mPackagesLock) {
        AutoPtr< ArrayOf<ISignature*> > s1;
        AutoPtr< ArrayOf<ISignature*> > s2;
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(uid1);
        if (obj != NULL) {
            if (obj->Probe(EIID_SharedUserSetting) != NULL) {
                SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
                s1 = sus->mSignatures->mSignatures;
            }
            else if (obj->Probe(EIID_PackageSetting) != NULL) {
                AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
                s1 = ps->mSignatures->mSignatures;
            }
            else {
                *result = IPackageManager::SIGNATURE_UNKNOWN_PACKAGE;
                return NOERROR;
            }
        }
        else {
            *result = IPackageManager::SIGNATURE_UNKNOWN_PACKAGE;
            return NOERROR;
        }
        obj = mSettings->GetUserIdLPr(uid2);
        if (obj != NULL) {
            if (obj->Probe(EIID_SharedUserSetting) != NULL) {
                SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
                s2 = sus->mSignatures->mSignatures;
            }
            else if (obj->Probe(EIID_PackageSetting) != NULL) {
                AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
                s2 = ps->mSignatures->mSignatures;
            }
            else {
                *result = IPackageManager::SIGNATURE_UNKNOWN_PACKAGE;
                return NOERROR;
            }
        }
        else {
            *result = IPackageManager::SIGNATURE_UNKNOWN_PACKAGE;
            return NOERROR;
        }
        *result = CompareSignatures(s1, s2);
    }
    return NOERROR;
}

Int32 CPackageManagerService::CompareSignatures(
    /* [in] */ ArrayOf<ISignature*>* s1,
    /* [in] */ ArrayOf<ISignature*>* s2)
{
    // Slogger::W(TAG, "TODO: CompareSignatures: delete");
    // TODO
    return IPackageManager::SIGNATURE_MATCH;

    if (s1 == NULL) {
        return s2 == NULL
                ? IPackageManager::SIGNATURE_NEITHER_SIGNED
                : IPackageManager::SIGNATURE_FIRST_NOT_SIGNED;
    }

    if (s2 == NULL) {
        return IPackageManager::SIGNATURE_SECOND_NOT_SIGNED;
    }

    if (s1->GetLength() != s2->GetLength()) {
        return IPackageManager::SIGNATURE_NO_MATCH;
    }

    // Since both signature sets are of size 1, we can compare without HashSets.
    if (s1->GetLength() == 1) {
        Boolean equals;
        IObject::Probe((*s1)[0])->Equals((*s2)[0], &equals);
        return equals ? IPackageManager::SIGNATURE_MATCH : IPackageManager::SIGNATURE_NO_MATCH;
    }

    AutoPtr<Set<ISignature*> > set1 = new Set<ISignature*>();
    for (Int32 i = 0; i < s1->GetLength(); i++) {
        set1->Insert((*s1)[i]);
    }
    AutoPtr<Set<ISignature*> > set2 = new Set<ISignature*>();
    for (Int32 i = 0; i < s2->GetLength(); i++) {
        set2->Insert((*s2)[i]);
    }
    // Make sure s2 contains all signatures in s1.
    Boolean isEqual = *set1 == *set2;
    if (isEqual) {
        return IPackageManager::SIGNATURE_MATCH;
    }
    return IPackageManager::SIGNATURE_NO_MATCH;
}

Boolean CPackageManagerService::IsCompatSignatureUpdateNeeded(
    /* [in] */ PackageParser::Package* scannedPkg)
{
    return (IsExternal(scannedPkg) && mSettings->IsExternalDatabaseVersionOlderThan(
            Settings::DatabaseVersion::SIGNATURE_END_ENTITY))
            || (!IsExternal(scannedPkg) && mSettings->IsInternalDatabaseVersionOlderThan(
                    Settings::DatabaseVersion::SIGNATURE_END_ENTITY));
}

Int32 CPackageManagerService::CompareSignaturesCompat(
    /* [in] */ PackageSignatures* existingSigs,
    /* [in] */ PackageParser::Package* scannedPkg)
{
    if (!IsCompatSignatureUpdateNeeded(scannedPkg)) {
        return IPackageManager::SIGNATURE_NO_MATCH;
    }

    AutoPtr<IHashSet> existingSet;
    CHashSet::New((IHashSet**)&existingSigs);
    for (Int32 i = 0; i < existingSigs->mSignatures->GetLength(); ++i) {
        existingSet->Add((*existingSigs->mSignatures)[i]);
    }
    AutoPtr<IHashSet> scannedCompatSet;
    CHashSet::New((IHashSet**)&scannedCompatSet);
    for (Int32 i = 0; i < scannedPkg->mSignatures->GetLength(); ++i) {
        AutoPtr<ISignature> sig = (*scannedPkg->mSignatures)[i];
        // try {
        AutoPtr< ArrayOf<ISignature*> > chainSignatures;
        if (FAILED(sig->GetChainSignatures((ArrayOf<ISignature*>**)&chainSignatures))) {
            scannedCompatSet->Add(sig);
            continue;
        }
        for (Int32 j = 0; j < chainSignatures->GetLength(); ++j) {
            scannedCompatSet->Add((*chainSignatures)[j]);
        }
        // } catch (CertificateEncodingException e) {
        //     scannedCompatSet.add(sig);
        // }
    }
    /*
     * Make sure the expanded scanned set contains all signatures in the
     * existing one.
     */
    Boolean equals;
    if (IObject::Probe(scannedCompatSet)->Equals(existingSet, &equals), equals) {
        // Migrate the old signatures to the new scheme.
        existingSigs->AssignSignatures(scannedPkg->mSignatures);
        // The new KeySets will be re-added later in the scanning process.
        synchronized (mPackagesLock) {
            mSettings->mKeySetManagerService->RemoveAppKeySetDataLPw(scannedPkg->mPackageName);
        }
        return IPackageManager::SIGNATURE_MATCH;
    }
    return IPackageManager::SIGNATURE_NO_MATCH;
}

Boolean CPackageManagerService::IsRecoverSignatureUpdateNeeded(
    /* [in] */ PackageParser::Package* scannedPkg)
{
    if (IsExternal(scannedPkg)) {
        return mSettings->IsExternalDatabaseVersionOlderThan(
                Settings::DatabaseVersion::SIGNATURE_MALFORMED_RECOVER);
    }
    else {
        return mSettings-IsInternalDatabaseVersionOlderThan(
                Settings::DatabaseVersion::SIGNATURE_MALFORMED_RECOVER);
    }
}

Int32 CPackageManagerService::CompareSignaturesRecover(
    /* [in] */ PackageSignatures* existingSigs,
    /* [in] */ PackageParser::Package* scannedPkg)
{
    if (!IsRecoverSignatureUpdateNeeded(scannedPkg)) {
        return IPackageManager::SIGNATURE_NO_MATCH;
    }

    String msg(NULL);
    // try {
    AutoPtr<ISignatureHelper> helper;
    CSignatureHelper::AcquireSingleton((ISignatureHelper**)&helper);
    Boolean isMatch;
    if (helper->AreEffectiveMatch(existingSigs->mSignatures, scannedPkg->mSignatures, &isMatch), isMatch) {
        LogCriticalInfo(ILogHelper::INFO, String("Recovered effectively matching certificates for ")
                + scannedPkg->mPackageName);
        return IPackageManager::SIGNATURE_MATCH;
    }
    // } catch (CertificateException e) {
    //     msg = e.getMessage();
    // }

    LogCriticalInfo(ILogHelper::INFO,
            String("Failed to recover certificates for ") + scannedPkg->mPackageName + ": " + msg);
    return IPackageManager::SIGNATURE_NO_MATCH;
}

ECode CPackageManagerService::GetPackagesForUid(
    /* [in] */ Int32 uid,
    /* [out, callee] */ ArrayOf<String>** packages)
{
    VALIDATE_NOT_NULL(packages)

    uid = UserHandle::GetAppId(uid);
    //reader
    synchronized (mPackagesLock) {
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(uid);
        if (obj && obj->Probe(EIID_SharedUserSetting) != NULL) {
            SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
            const Int32 N = sus->mPackages.GetSize();
            AutoPtr< ArrayOf<String> > res = ArrayOf<String>::Alloc(N);
            HashSet<PackageSetting*>::Iterator it;
            Int32 i;
            for (it = sus->mPackages.Begin(), i = 0; it != sus->mPackages.End(); ++it, ++i) {
                (*res)[i] = (*it)->mName;
            }
            *packages = res;
            REFCOUNT_ADD(*packages)
            return NOERROR;
        }
        else if (obj && obj->Probe(EIID_PackageSetting) != NULL) {
            PackageSetting* ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
            AutoPtr< ArrayOf<String> > res = ArrayOf<String>::Alloc(1);
            (*res)[0] = ps->mName;
            *packages = res;
            REFCOUNT_ADD(*packages)
            return NOERROR;
        }
    }
    *packages = NULL;
    return NOERROR;
}

ECode CPackageManagerService::GetNameForUid(
    /* [in] */ Int32 uid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)

    //reader
    synchronized (mPackagesLock) {
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(UserHandle::GetAppId(uid));
        if (obj->Probe(EIID_SharedUserSetting) != NULL) {
            SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
            StringBuilder sb;
            sb.Append(sus->mName);
            sb.Append(":");
            sb.Append(sus->mUserId);
            *name = sb.ToString();
            return NOERROR;
        }
        else if (obj->Probe(EIID_PackageSetting) != NULL) {
            PackageSetting* ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
            *name = ps->mName;
            return NOERROR;
        }
    }
    *name = NULL;
    return NOERROR;
}

ECode CPackageManagerService::GetUidForSharedUser(
    /* [in] */ const String& sharedUserName,
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid)

    if (sharedUserName.IsNull()) {
        *uid = -1;
        return NOERROR;
    }
    // reader
    synchronized (mPackagesLock) {
        AutoPtr<SharedUserSetting> suid = mSettings->GetSharedUserLPw(sharedUserName, 0, FALSE);
        if (suid == NULL) {
            *uid = -1;
            return NOERROR;
        }
        *uid = suid->mUserId;
    }
    return NOERROR;
}

ECode CPackageManagerService::GetFlagsForUid(
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;
    synchronized (mPackagesLock) {
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(UserHandle::GetAppId(uid));
        if (obj->Probe(EIID_SharedUserSetting) != NULL) {
            AutoPtr<SharedUserSetting> sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
            *result = sus->mPkgFlags;
        }
        else if (obj->Probe(EIID_PackageSetting) != NULL) {
            AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
            *result = ps->PkgFlags;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::IsUidPrivileged(
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    uid = UserHandle::GetAppId(uid);
    // reader
    synchronized (mPackagesLock) {
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(uid);
        if (obj->Probe(EIID_SharedUserSetting) != NULL) {
            AutoPtr<SharedUserSetting> sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
            HashSet<AutoPtr<PackageSetting> >::Iterator it = sus->mPackages.Begin();
            for (; it != sus->mPackages.End(); ++it) {
                if ((*it)->IsPrivileged()) {
                    *result = TRUE;
                }
            }
        }
        else if (obj->Probe(EIID_PackageSetting) != NULL) {
            AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
            *result = ps->IsPrivileged();
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::ResolveIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IResolveInfo** resolveInfo)
{
    VALIDATE_NOT_NULL(resolveInfo)
    *resolveInfo = NULL;

   if (!sUserManager->Exists(userId)) return NOERROR;
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            FALSE, FALSE, String("resolve intent")))
    AutoPtr<IList> query;
    QueryIntentActivities(intent, resolvedType, flags, userId, (IList**)&query);
    return ChooseBestActivity(intent, resolvedType, flags, query, userId, resolveInfo);
}

ECode CPackageManagerService::SetLastChosenActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ IComponentName* activity)
{
    Int32 userId = UserHandle::GetCallingUserId();
    if (DEBUG_PREFERRED) {
        Logger::V(TAG, "setLastChosenActivity intent=%p resolvedType=%s flags=%d filter=%p match=%d activity=%p",
                intent, resolvedType.string(), flags, filter, match, activity);
        // filter.dump(new PrintStreamPrinter(System.out), "    ");
    }
    intent->SetComponent(NULL);
    AutoPtr<IList> query = QueryIntentActivities(intent, resolvedType, flags, userId);
    // Find any earlier preferred or last chosen entries and nuke them
    FindPreferredActivity(intent, resolvedType,
            flags, query, 0, FALSE, TRUE, FALSE, userId);
    // Add the new activity as the last chosen for this filter
    AddPreferredActivityInternal(filter, match, NULL, activity, FALSE, userId,
            String("Setting last chosen"));
}

ECode CPackageManagerService::GetLastChosenActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** info)
{
    VALIDATE_NOT_NULL(info)
    Int32 userId = UserHandle::GetCallingUserId();
    if (DEBUG_PREFERRED) Logger::V(TAG, "Querying last chosen activity for %p", intent);
    AutoPtr<IList> query = QueryIntentActivities(intent, resolvedType, flags, userId);
    *info = FindPreferredActivity(intent, resolvedType, flags, query, 0,
            FALSE, FALSE, FALSE, userId);
    REFCOUNT_ADD(*info)
    return NOERROR;
}

ECode CPackageManagerService::ChooseBestActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ IList* query,
    /* [in] */ Int32 userId,
    /* [out] */ IResolveInfo** resolveInfo)
{
    VALIDATE_NOT_NULL(resolveInfo)

    if (query != NULL) {
        Int32 size;
        query->GetSize(&size);
        if (size == 1) {
            AutoPtr<IInterface> value;
            query->Get(0, (IInterface**)&value);
            *resolveInfo = IResolveInfo::Probe(value);
            REFCOUNT_ADD(*resolveInfo)
            return NOERROR;
        }
        else if (size > 1) {
            Int32 flags;
            intent->GetFlags(&flags);
            Boolean debug = ((flags & IIntent::FLAG_DEBUG_LOG_RESOLUTION) != 0);
            // If there is more than one activity with the same priority,
            // then let the user decide between them.
            AutoPtr<IInterface> value0, value1;
            query->Get(0, (IInterface**)&value0);
            query->Get(1, (IInterface**)&value1);
            AutoPtr<IResolveInfo> r0 = IResolveInfo::Probe(value0);
            AutoPtr<IResolveInfo> r1 = IResolveInfo::Probe(value1);
            if (DEBUG_INTENT_MATCHING || debug) {
                AutoPtr<IActivityInfo> ai;
                r0->GetActivityInfo((IActivityInfo**)&ai);
                String name;
                ai->GetName(&name);
                Int32 p0, p1;
                r0->GetPriority(&p0);
                r1->GetPriority(&r1);
                ai = NULL;
                r1->GetActivityInfo((IActivityInfo**)&ai);
                String name1;
                ai->GetName(&name1);
                Slogger::V(TAG, "%s=%d vs %s=%d", name.string(), p0, name1.string(), p1);
            }
            // If the first activity has a higher priority, or a different
            // default, then it is always desireable to pick it.
            Int32 r0Value, r1Value;
            Boolean r0Result, r1Result;
            if ((r0->GetPriority(&r0Value), r1->GetPriority(&r1Value), r0Value != r1Value)
                    || (r0->GetPreferredOrder(&r0Value), r1->GetPreferredOrder(&r1Value), r0Value != r1Value)
                    || (r0->GetIsDefault(&r0Result), r1->GetIsDefault(&r1Result), r0Result != r1Result)) {
                *resolveInfo = r0;
                REFCOUNT_ADD(*resolveInfo)
                return NOERROR;
            }
            // If we have saved a preference for a preferred activity for
            // this Intent, use that.
            r0->GetPriority(&r0Value);
            AutoPtr<IResolveInfo> ri = FindPreferredActivity(intent, resolvedType,
                    flags, query, r0Value, TRUE, FALSE, debug, userId);
            if (ri != NULL) {
                *resolveInfo = ri;
                REFCOUNT_ADD(*resolveInfo)
                return NOERROR;
            }
            if (userId != 0) {
                CResolveInfo::New(mResolveInfo, (IResolveInfo**)&ri);
                AutoPtr<IActivityInfo> ainfo1, ainfo2;
                ri->GetActivityInfo((IActivityInfo**)&ainfo1);
                CActivityInfo::New(ainfo1, (IActivityInfo**)&ainfo2);
                ri->SetActivityInfo(ainfo2);
                AutoPtr<IApplicationInfo> appinfo1, appinfo2;
                ainfo2->GetApplicationInfo((IApplicationInfo**)&appinfo1);
                CApplicationInfo::New(appinfo1, (IApplicationInfo**)&appinfo2);
                ainfo2->SetApplicationInfo(appinfo2);
                Int32 uid;
                appinfo2->GetUid(&uid);
                appinfo2->SetUid(UserHandle::GetUid(userId, UserHandle::GetAppId(uid)));
                *resolveInfo = ri;
                REFCOUNT_ADD(*resolveInfo)
                return NOERROR;
            }
            *resolveInfo = mResolveInfo;
            REFCOUNT_ADD(*resolveInfo)
            return NOERROR;
        }
    }
    *resolveInfo = NULL;
    return NOERROR;
}

AutoPtr<IResolveInfo> CPackageManagerService::FindPersistentPreferredActivityLP(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ IList* query,
    /* [in] */ Boolean debug,
    /* [in] */ Int32 userId)
{
    AutoPtr<PersistentPreferredIntentResolver> ppir;
    HashMap<Int32, AutoPtr<PersistentPreferredIntentResolver> >::Iterator ppirIt
            = mSettings->mPersistentPreferredActivities.Find(userId);
    if (ppirIt != mSettings->mPersistentPreferredActivities.End()) {
        ppir = ppirIt->mSecond;
    }
    // Get the list of persistent preferred activities that handle the intent
    if (DEBUG_PREFERRED || debug) Slogger::V(TAG, "Looking for presistent preferred activities...");
    AutoPtr< List<AutoPtr<PersistentPreferredActivity> > > pprefs = ppir != NULL
            ? ppir->QueryIntent(intent, resolvedType,
                    (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0, userId)
            : NULL;
    if (pprefs != NULL && pprefs->Begin() != pprefs->End()) {
        List<AutoPtr<PersistentPreferredActivity> >::Iterator pprefIt = pprefs->Begin();
        for (; pprefIt != pprefs->End(); ++pprefIt) {
            AutoPtr<PersistentPreferredActivity> ppa = *pprefIt;
            if (DEBUG_PREFERRED || debug) {
                Slogger::V(TAG, "Checking PersistentPreferredActivity ds=%s\n  component=%p"
                        , (ppa->CountDataSchemes() > 0 ? ppa->GetDataScheme(0).string() : "<none>")
                        , ppa->mComponent.Get());
                // ppa.dump(new LogPrinter(Log.VERBOSE, TAG, Log.LOG_ID_SYSTEM), "  ");
            }
            AutoPtr<IActivityInfo> ai;
            GetActivityInfo(ppa->mComponent, flags | IPackageManager::GET_DISABLED_COMPONENTS,
                    userId, (IActivityInfo**)&ai);
            if (DEBUG_PREFERRED || debug) {
                Slogger::V(TAG, "Found persistent preferred activity:");
                if (ai != NULL) {
                    // ai.dump(new LogPrinter(Log.VERBOSE, TAG, Log.LOG_ID_SYSTEM), "  ");
                }
                else {
                    Slogger::V(TAG, "  null");
                }
            }
            if (ai == NULL) {
                // This previously registered persistent preferred activity
                // component is no longer known. Ignore it and do NOT remove it.
                continue;
            }
            Boolean hasNext;
            while (query->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> value;
                query->GetNext((IInterface**)&value);
                AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(value);
                AutoPtr<IActivityInfo> riai;
                ri->GetActivityInfo((IActivityInfo**)&riai);
                AutoPtr<IApplicationInfo> appInfo, riappInfo;
                ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
                riai->GetApplicationInfo((IApplicationInfo**)&riappInfo)
                String pkgN, ripkgN;
                appInfo->GetPackageName(&pkgN);
                riAppInfo->GetPackageName(&ripkgN);
                if (!ripkgN.Equals(pkgN)) {
                    continue;
                }
                String name, riname;
                ai->GetName(&name);
                riai->GetName(&riname);
                if (!riname.Equals(name)) {
                    continue;
                }
                //  Found a persistent preference that can handle the intent.
                if (DEBUG_PREFERRED || debug) {
                    Slogger::V(TAG, "Returning persistent preferred activity: %s/%s",
                            ripkgN.string(), riname.string());
                }
                return ri;
            }
        }
    }
    return NULL;
}

AutoPtr<IResolveInfo> CPackageManagerService::FindPreferredActivity(
    /* [in] */ IIntent* _intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ IList* query,
    /* [in] */ Int32 priority,
    /* [in] */ Boolean always,
    /* [in] */ Boolean removeMatches,
    /* [in] */ Boolean debug,
    /* [in] */ Int32 userId)
{
    AutoPtr<IIntent> intent = _intent;
    if (!sUserManager->Exists(userId)) return NULL;
    // writer
    synchronized (mPackagesLock) {
        AutoPtr<IIntent> selector;
        if (intent->GetSelector((IIntent**)&selector), selector != NULL) {
            AutoPtr<IIntent> temp = intent;
            intent = NULL;
            temp->GetSelector((IIntent**)&intent);
        }
        if (DEBUG_PREFERRED) intent->AddFlags(IIntent::FLAG_DEBUG_LOG_RESOLUTION);

        // Try to find a matching persistent preferred activity.
        AutoPtr<IResolveInfo> pri = FindPersistentPreferredActivityLP(intent, resolvedType, flags, query,
                debug, userId);

        // If a persistent preferred activity matched, use it.
        if (pri != NULL) {
            return pri;
        }

        AutoPtr<PreferredIntentResolver> pir;
        HashMap<Int32, AutoPtr<PreferredIntentResolver> > = mSettings.mPreferredActivities.get(userId);
        // Get the list of preferred activities that handle the intent
        if (DEBUG_PREFERRED || debug) Slog.v(TAG, "Looking for preferred activities...");
        AutoPtr< List<AutoPtr<PreferredActivity> > > prefs = pir != NULL
                ? pir->QueryIntent(intent, resolvedType,
                        (flags & IPackageManager::MATCH_DEFAULT_ONLY) != 0, userId)
                : NULL;
        if (prefs != NULL && prefs->Begin() > prefs->End()) {
            Boolean changed = FALSE;
            // try {
            // First figure out how good the original match set is.
            // We will only allow preferred activities that came
            // from the same match quality.
            Int32 match = 0;

            if (DEBUG_PREFERRED || debug) Slogger::V(TAG, "Figuring out best match...");

            Boolean hasNext;
            while (query->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> value;
                query->GetNext((IInterface**)&value);
                AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(value);
                if (DEBUG_PREFERRED || debug) {
                    AutoPtr<IActivityInfo> ai;
                    ri->GetActivityInfo((IActivityInfo**)&ai);
                    Slogger::V(TAG, "Match for %p: 0x%d", ai.Get(), match);
                }
                Int32 riMatch;
                if (ri->GetMatch(&riMatch), riMatch > match) {
                    match = riMatch;
                }
            }

            if (DEBUG_PREFERRED || debug) Slogger::V(TAG, "Best match: 0x%d", match);

            match &= IIntentFilter::MATCH_CATEGORY_MASK;
            List<AutoPtr<PreferredActivity> >::Iterator prefIt = prefs->Begin();
            for (; prefIt != prefs->End(); ++prefIt) {
                AutoPtr<PreferredActivity> pa = *prefIt;
                if (DEBUG_PREFERRED || debug) {
                    Slogger::V(TAG, "Checking PreferredActivity ds=%s\n  component=%p"
                            + (pa->CountDataSchemes() > 0 ? pa->GetDataScheme(0).string() : "<none>")
                            , pa->mPref->mComponent.Get());
                    // pa.dump(new LogPrinter(Log.VERBOSE, TAG, Log.LOG_ID_SYSTEM), "  ");
                }
                if (pa->mPref->mMatch != match) {
                    if (DEBUG_PREFERRED || debug) Slogger::V(TAG, "Skipping bad match %d", pa->mPref->mMatch));
                    continue;
                }
                // If it's not an "always" type preferred activity and that's what we're
                // looking for, skip it.
                if (always && !pa->mPref->mAlways) {
                    if (DEBUG_PREFERRED || debug) Slogger::V(TAG, "Skipping mAlways=false entry");
                    continue;
                }
                AutoPtr<IActivityInfo> ai;
                GetActivityInfo(pa->mPref->mComponent, flags | IPackageManager::GET_DISABLED_COMPONENTS,
                        userId, (IActivityInfo**)&ai);
                if (DEBUG_PREFERRED || debug) {
                    Slogger::V(TAG, "Found preferred activity:");
                    if (ai != NULL) {
                        // ai.dump(new LogPrinter(Log.VERBOSE, TAG, Log.LOG_ID_SYSTEM), "  ");
                    }
                    else {
                        Slogger::V(TAG, "  null");
                    }
                }
                if (ai == NULL) {
                    // This previously registered preferred activity
                    // component is no longer known.  Most likely an update
                    // to the app was installed and in the new version this
                    // component no longer exists.  Clean it up by removing
                    // it from the preferred activities list, and skip it.
                    Slogger::W(TAG, "Removing dangling preferred activity: %p", pa->mPref->mComponent.Get());
                    pir->RemoveFilter(pa);
                    changed = TRUE;
                    continue;
                }
                Boolean hasNext;
                while (query->HasNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> value;
                    query->GetNext((IInterface**)&value);
                    AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(value);
                    AutoPtr<IActivityInfo> riai;
                    ri->GetActivityInfo((IActivityInfo**)&riai);
                    AutoPtr<IApplicationInfo> riAppInfo, appInfo;
                    riai->GetApplicationInfo((IApplicationInfo**)&riAppInfo);
                    ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
                    String riPkgName, pkgName;
                    riAppInfo->GetPackageName(&riPkgName);
                    appInfo->GetPackageName(&pkgName);
                    if (!riPkgName.Equals(pkgName)) {
                        continue;
                    }
                    String riName, name;
                    riai->GetName(&riName);
                    ai->GetName(&name);
                    if (!riName.Equals(name)) {
                        continue;
                    }

                    if (removeMatches) {
                        pir->RemoveFilter(pa);
                        changed = TRUE;
                        if (DEBUG_PREFERRED) {
                            Slogger::V(TAG, "Removing match %p", pa->mPref->mComponent.Get());
                        }
                        break;
                    }

                    // Okay we found a previously set preferred or last chosen app.
                    // If the result set is different from when this
                    // was created, we need to clear it and re-ask the
                    // user their preference, if we're looking for an "always" type entry.
                    if (always && !pa->mPref->SameSet(query, priority)) {
                        Slogger::I(TAG, "Result set changed, dropping preferred activity for %p type %d"
                                , intent, resolvedType);
                        if (DEBUG_PREFERRED) {
                            Slogger::V(TAG, "Removing preferred activity since set changed %p"
                                    , pa->mPref->mComponent.Get());
                        }
                        pir->RemoveFilter(pa);
                        // Re-add the filter as a "last chosen" entry (!always)
                        AutoPtr<PreferredActivity> lastChosen = new PreferredActivity(
                                pa, pa->mPref->mMatch, NULL, pa->mPref->mComponent, FALSE);
                        pir->AddFilter(lastChosen);
                        changed = TRUE;
                        if (changed) {
                            if (DEBUG_PREFERRED) {
                                Slogger::V(TAG, "Preferred activity bookkeeping changed; writing restrictions");
                            }
                            mSettings->WritePackageRestrictionsLPr(userId);
                        }
                        return NULL;
                    }

                    // Yay! Either the set matched or we're looking for the last chosen
                    if (DEBUG_PREFERRED || debug) {
                        String ripkgN;
                        riai->GetPackageName(&ripkgN);
                        Slogger::V(TAG, "Returning preferred activity: %s/%s", ripkgN.string(), name.string());
                    }
                    if (changed) {
                        if (DEBUG_PREFERRED) {
                            Slogger::V(TAG, "Preferred activity bookkeeping changed; writing restrictions");
                        }
                        mSettings->WritePackageRestrictionsLPr(userId);
                    }
                    return ri;
                }
            }
            // } finally {
            //     if (changed) {
            //         if (DEBUG_PREFERRED) {
            //             Slog.v(TAG, "Preferred activity bookkeeping changed; writing restrictions");
            //         }
            //         mSettings.writePackageRestrictionsLPr(userId);
            //     }
            // }
            if (changed) {
                if (DEBUG_PREFERRED) {
                    Slogger::V(TAG, "Preferred activity bookkeeping changed; writing restrictions");
                }
                mSettings->WritePackageRestrictionsLPr(userId);
            }
        }
    }
    if (DEBUG_PREFERRED || debug) Slogger::V(TAG, "No preferred activity to return");
    return NULL;
}

ECode CPackageManagerService::CanForwardTo(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 sourceUserId,
    /* [in] */ Int32 targetUserId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS_FULL, String(NULL)))
    AutoPtr< List<AutoPtr<CrossProfileIntentFilter> > > matches =
            GetMatchingCrossProfileIntentFilters(intent, resolvedType, sourceUserId);
    if (matches != NULL) {
        List<AutoPtr<CrossProfileIntentFilter> >::Iterator it = matches->Begin();
        for (; it != matches->End(); ++it) {
            if ((*it)->GetTargetUserId() == targetUserId) {
                *result = TRUE;
                break;
            }
        }
    }
    return NOERROR;
}

AutoPtr< List<AutoPtr<CrossProfileIntentFilter> > > CPackageManagerService::GetMatchingCrossProfileIntentFilters(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 userId)
{
    AutoPtr<CrossProfileIntentResolver> resolver;
    HashMap<Int32, AutoPtr<CrossProfileIntentResolver> >::Iterator it
            = mSettings->mCrossProfileIntentResolvers.Find(userId);
    if (it != mSettings->mCrossProfileIntentResolvers.End()) {
        resolver = it->mSecond;
    }
    if (resolver != NULL) {
        return resolver->QueryIntent(intent, resolvedType, false, userId);
    }
    return NULL;
}

ECode CPackageManagerService::QueryIntentActivities(
    /* [in] */ IIntent* _intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out, callee] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)

    AutoPtr<IIntent> intent = _intent;
    if (!sUserManager->Exists(userId)) {
        return CParcelableObjectContainer::New(infos);
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId, FALSE, FALSE, String("query intent activities")))
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp == NULL) {
        AutoPtr<IIntent> selector;
        if (intent->GetSelector((IIntent**)&selector), selector != NULL) {
            intent = selector;
            intent->GetComponent((IComponentName**)&comp);
        }
    }

    if (comp != NULL) {
        AutoPtr<IArrayList> al;
        CArrayList::New(1, (IArrayList**)&al);
        AutoPtr<IList> list = IList::Probe(al);
        AutoPtr<IActivityInfo> ai;
        GetActivityInfo(comp, flags, userId, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetActivityInfo(ai);
            list->Add(ri);
        }
        *infos = list;
        REFCOUNT_ADD(*infos)
        return NOERROR;
    }

    // reader
    synchronized (mPackagesLock) {
        String pkgName;
        intent->GetPackage(&pkgName);
        if (pkgName.IsNull()) {
            AutoPtr< List<AutoPtr<CrossProfileIntentFilter> > > matchingFilters =
                    GetMatchingCrossProfileIntentFilters(intent, resolvedType, userId);
            // Check for results that need to skip the current profile.
            AutoPtr<IResolveInfo> resolveInfo  = QuerySkipCurrentProfileIntents(matchingFilters, intent,
                    resolvedType, flags, userId);
            if (resolveInfo != NULL) {
                AutoPtr<IArrayList> al;
                CArrayList::New(1, (IArrayList**)&al);
                AutoPtr<IList> result = IList::Probe(al);
                result->Add(resolveInfo);
                *infos = result;
                REFCOUNT_ADD(*infos)
                return NOERROR;
            }
            // Check for cross profile results.
            AutoPtr<IResolveInfo> = QueryCrossProfileIntents(
                    matchingFilters, intent, resolvedType, flags, userId);

            // Check for results in the current profile.
            AutoPtr<IList> result = mActivities->QueryIntent(intent, resolvedType, flags, userId);
            if (resolveInfo != NULL) {
                result->Add(resolveInfo);
                AutoPtr<ICollections> cols;
                CCollections::AcquireSingleton((ICollections**)&cols);
                cols->Sort(result, mResolvePrioritySorter);
            }
            *infos = result;
            return NOERROR;
        }
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator pit = mPackages.Find(pkgName);
        if (pit != mPackages.End()) {
            pkg = pit->mSecond;
        }
        if (pkg != NULL) {
            *infos = mActivities->QueryIntentForPackage(
                    intent, resolvedType, flags, &pkg->mActivities, userId);
            REFCOUNT_ADD(*infos)
            return NOERROR;
        }
        AutoPtr<IArrayList> al;
        CArrayList::New((IArrayList**)&al);
        *infos = IList::Probe(al);
        REFCOUNT_ADD(*infos)
    }
    return NOERROR;
}

AutoPtr<IResolveInfo> CPackageManagerService::QuerySkipCurrentProfileIntents(
    /* [in] */ List<AutoPtr<CrossProfileIntentFilter> >* matchingFilters,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 sourceUserId)
{
    if (matchingFilters != NULL) {
        List<AutoPtr<CrossProfileIntentFilter> >::Iterator it = matchingFilters->Begin();
        for (; it != matchingFilters->End(); ++it) {
            AutoPtr<CrossProfileIntentFilter> filter = *it;
            if ((filter->GetFlags() & IPackageManager::SKIP_CURRENT_PROFILE) != 0) {
                // Checking if there are activities in the target user that can handle the
                // intent.
                AutoPtr<IResolveInfo> resolveInfo = CheckTargetCanHandle(filter, intent, resolvedType,
                        flags, sourceUserId);
                if (resolveInfo != NULL) {
                    return resolveInfo;
                }
            }
        }
    }
    return NULL;
}

AutoPtr<IResolveInfo> CPackageManagerService::QueryCrossProfileIntents(
    /* [in] */ List<AutoPtr<CrossProfileIntentFilter> >* matchingFilters,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 sourceUserId)
{
    if (matchingFilters != NULL) {
        // Two {@link CrossProfileIntentFilter}s can have the same targetUserId and
        // match the same intent. For performance reasons, it is better not to
        // run queryIntent twice for the same userId
        HashMap<Int32, Boolean> alreadyTriedUserIds;
        List<AutoPtr<CrossProfileIntentFilter> >::Iterator it = matchingFilters->Begin();
        for (; it != matchingFilters->End(); ++it) {
            AutoPtr<CrossProfileIntentFilter> filter = *it;
            Int32 targetUserId = filter->GetTargetUserId();
            if ((filter->GetFlags() & IPackageManager::SKIP_CURRENT_PROFILE) == 0
                    && (alreadyTriedUserIds.Find(targetUserId) == alreadyTriedUserIds.End()) {
                // Checking if there are activities in the target user that can handle the
                // intent.
                AutoPtr<IResolveInfo> resolveInfo = CheckTargetCanHandle(filter, intent, resolvedType,
                        flags, sourceUserId);
                if (resolveInfo != NULL) return resolveInfo;
                alreadyTriedUserIds[targetUserId] = TRUE;
            }
        }
    }
    return NULL;
}

AutoPtr<IResolveInfo> CPackageManagerService::CheckTargetCanHandle(
    /* [in] */ CrossProfileIntentFilter* filter,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 sourceUserId)
{
    AutoPtr<IList> resultTargetUser = mActivities->QueryIntent(intent,
            resolvedType, flags, filter.getTargetUserId());
    Boolean isEmpty;
    if (resultTargetUser != NULL && (resultTargetUser->IsEmpty(&isEmpty), !isEmpty)) {
        return CreateForwardingResolveInfo(filter, sourceUserId, filter->GetTargetUserId());
    }
    return NULL;
}

AutoPtr<IResolveInfo> CPackageManagerService::CreateForwardingResolveInfo(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 sourceUserId,
    /* [in] */ Int32 targetUserId)
{
    AutoPtr<IResolveInfo> forwardingResolveInfo;
    CResolveInfo::New((IResolveInfo**)&forwardingResolveInfo);
    String className;
    if (targetUserId == IUserHandle::USER_OWNER) {
        className = IIntentForwarderActivity::FORWARD_INTENT_TO_USER_OWNER;
    }
    else {
        className = IIntentForwarderActivity::FORWARD_INTENT_TO_MANAGED_PROFILE;
    }
    String pkgName;
    mAndroidApplication->GetPackageName(&pkgName);
    AutoPtr<IComponentName> forwardingActivityComponentName;
    CComponentName::New(pkgName, className, (IComponentName**)&forwardingActivityComponentName);
    AutoPtr<IActivityInfo> forwardingActivityInfo;
    GetActivityInfo(forwardingActivityComponentName, 0, sourceUserId, (IActivityInfo**)&forwardingActivityInfo);
    if (targetUserId == IUserHandle::USER_OWNER) {
        forwardingActivityInfo->SetShowUserIcon(IUserHandle::USER_OWNER);
        forwardingResolveInfo->SetNoResourceId(TRUE);
    }
    forwardingResolveInfo->SetActivityInfo(forwardingActivityInfo);
    forwardingResolveInfo->SetPriority(0);
    forwardingResolveInfo->SetPreferredOrder(0);
    forwardingResolveInfo->SetMatch(0);
    forwardingResolveInfo->SetIsDefault(TRUE);
    forwardingResolveInfo->SetFilter(filter);
    forwardingResolveInfo->SetTargetUserId(targetUserId);
    return forwardingResolveInfo;
}

ECode CPackageManagerService::QueryIntentActivityOptions(
    /* [in] */ IComponentName* caller,
    /* [in] */ ArrayOf<IIntent*>* specifics,
    /* [in] */ ArrayOf<String>* specificTypes,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)
    *infos = NULL;

    if (!sUserManager->Exists(userId)) {
        AutoPtr<ICollections> cols;
        CCollections::AcquireSingleton((ICollections**)&cols);
        return cols->EmptyList(infos);
    }
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId, FALSE, FALSE,
            String("query intent activity options")))
    String resultsAction;
    intent->GetAction(&resultsAction);

    AutoPtr<IList> results;
    QueryIntentActivities(intent, resolvedType, flags | IPackageManager::GET_RESOLVED_FILTER,
            userId, (IList**)&result);

    if (DEBUG_INTENT_MATCHING) {
        Logger::V(TAG, "Query %p: %p", intent, results.Get());
    }

    Int32 specificsPos = 0;
    Int32 N;

    // todo: note that the algorithm used here is O(N^2).  This
    // isn't a problem in our current environment, but if we start running
    // into situations where we have more than 5 or 10 matches then this
    // should probably be changed to something smarter...

    // First we go through and resolve each of the specific items
    // that were supplied, taking care of removing any corresponding
    // duplicate items in the generic resolve list.
    if (specifics != NULL) {
        for (Int32 i = 0; i < specifics->GetLength(); i++) {
            AutoPtr<IIntent> sintent = (*specifics)[i];
            if (sintent == NULL) {
                continue;
            }

            if (DEBUG_INTENT_MATCHING) {
                Logger::V(TAG, "Specific #%d: %p", i, sintent.Get());
            }

            String action;
            sintent->GetAction(&action);
            if (!resultsAction.IsNull() && resultsAction.Equals(action)) {
                // If this action was explicitly requested, then don't
                // remove things that have it.
                action = NULL;
            }

            AutoPtr<IResolveInfo> ri;
            AutoPtr<IActivityInfo> ai;

            AutoPtr<IComponentName> comp;
            sintent->GetComponent((IComponentName**)&comp);
            if (comp == NULL) {
                ResolveIntent(sintent, specificTypes != NULL ? (*specificTypes)[i] : String(NULL),
                        flags, userId, (IResolveInfo**)&ri);
                if (ri == NULL) {
                    continue;
                }
                if (ri == mResolveInfo) {
                    // ACK!  Must do something better with this.
                }
                ri->GetActivityInfo((IActivityInfo**)&ai);
                String aiName, aiPName;
                ai->GetName(&aiName);
                AutoPtr<IApplicationInfo> aiAppInfo;
                ai->GetApplicationInfo((IApplicationInfo**)&aiAppInfo);
                aiAppInfo->GetPackageName(&aiPName);
                FAIL_RETURN(CComponentName::New(
                    aiPName, aiName, (IComponentName**)&comp));
            }
            else {
                GetActivityInfo(comp, flags, userId, (IActivityInfo**)&ai);
                if (ai == NULL) {
                    continue;
                }
            }

            // Look for any generic query activities that are duplicates
            // of this specific one, and remove them from the results.
            if (DEBUG_INTENT_MATCHING) Logger::V(TAG, "Specific #%d: %p", i, ai.Get());

            results->GetSize(&N);
            Int32 j;
            for (j = specificsPos; j < N; j++) {
                AutoPtr<IInterface> value;
                results->Get(j, (IInterface**)&value);
                AutoPtr<IResolveInfo> sri = IResolveInfo::Probe(value);
                String clsName, pkgName;
                comp->GetClassName(&clsName);
                comp->GetPackageName(&pkgName);
                AutoPtr<IActivityInfo> sriAInfo;
                sri->GetActivityInfo((IActivityInfo**)&sriAInfo);
                AutoPtr<IApplicationInfo> sriAppInfo;
                sriAInfo->GetApplicationInfo((IApplicationInfo**)&sriAppInfo);
                String sriName, sriPName;
                sriAInfo->GetName(&sriName);
                sriAppInfo->GetPackageName(&sriPName);
                AutoPtr<IIntentFilter> sriFilter;
                sri->GetFilter((IIntentFilter**)&sriFilter);
                Boolean isMatched;
                if ((sriName.Equals(clsName) && sriPName.Equals(pkgName))
                    || (!action.IsNull() && (sriFilter->MatchAction(action, &isMatched), isMatched))) {
                    results->Remove(j);
                    if (DEBUG_INTENT_MATCHING) Logger::V(TAG, "Removing duplicate item from %d due to specific %d", j, specificsPos);
                    if (ri == NULL) {
                        ri = sri;
                    }
                    j--;
                    N--;
                }
            }

            // Add this specific item to its proper place.
            if (ri == NULL) {
                CResolveInfo::New((IResolveInfo**)&ri);
                ri->SetActivityInfo(ai);
            }
            results->Add(specificsPos, ri);
            ri->SetSpecificIndex(i);
            specificsPos++;
        }
    }

    // Now we go through the remaining generic results and remove any
    // duplicate actions that are found here.
    results->GetSize(&N);
    for (Int32 i = specificsPos; i < N-1; i++) {
        AutoPtr<IInterface> value;
        results->Get(i, (IInterface**)&value);
        AutoPtr<IResolveInfo> rii = IResolveInfo::Probe(value);
        AutoPtr<IIntentFilter> riiFilter;
        rii->GetFilter((IIntentFilter**)&riiFilter);
        if (riiFilter == NULL) {
            continue;
        }

        // Iterate over all of the actions of this result's intent
        // filter...  typically this should be just one.
        AutoPtr< ArrayOf<String> > actions;
        riiFilter->GetActions((ArrayOf<String>**)&actions);
        if (actions == NULL) {
            continue;
        }
        for (Int32 j; j < actions->GetLength(); ++j) {
            const String& action = (*actions)[j];
            if (!resultsAction.IsNull() && resultsAction.Equals(action)) {
                // If this action was explicitly requested, then don't
                // remove things that have it.
                continue;
            }
            for (Int32 k = i + 1; k < N; k++) {
                AutoPtr<IInterface> valuek;
                results->Get(k, (IInterface**)&valuek);
                AutoPtr<IResolveInfo> rij = IResolveInfo::Probe(valuek);
                AutoPtr<IIntentFilter> rijFilter;
                rij->GetFilter((IIntentFilter**)&rijFilter);
                Boolean result;
                if (rijFilter != NULL && (rijFilter->HasAction(action, &result), result)) {
                    results->Remove(k);
                    if (DEBUG_INTENT_MATCHING)
                        Logger::V(TAG, "Removing duplicate item from %d due to action %s at %d", k, action, i);
                    k--;
                    N--;
                }
            }
        }

        // If the caller didn't request filter information, drop it now
        // so we don't have to marshall/unmarshall it.
        if ((flags & IPackageManager::GET_RESOLVED_FILTER) == 0) {
            rii->SetFilter(NULL);
        }
    }

    // Filter out the caller activity if so requested.
    if (caller != NULL) {
        results->GetSize(&N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IInterface> value;
            results->Get(i, (IInterface**)&value);
            AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(value);
            AutoPtr<IActivityInfo> ainfo;
            ri->GetActivityInfo((IActivityInfo**)&ainfo);
            String pkgName, clsName;
            caller->GetPackageName(&pkgName);
            caller->GetClassName(&clsName);
            AutoPtr<IApplicationInfo> appInfo;
            ainfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String aName, aPName;
            ainfo->GetName(&aName);
            appInfo->GetPackageName(&aPName);
            if (pkgName.Equals(aPName) && clsName.Equals(aName)) {
                results->Remove(i);
                break;
            }
        }
    }

    // If the caller didn't request filter information,
    // drop them now so we don't have to
    // marshall/unmarshall it.
    if ((flags & IPackageManager::GET_RESOLVED_FILTER) == 0) {
        results->GetSize(&N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IInterface> value;
            results->Get(i, (IInterface**)&value);
            AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(value);
            ri->SetFilter(NULL);
        }
    }

    // if (DEBUG_INTENT_MATCHING) Logger::V(TAG, "Result: " + results);

    *infos = results;
    REFCOUNT_ADD(*infos)
    return NOERROR;
}

ECode CPackageManagerService::QueryIntentReceivers(
    /* [in] */ IIntent* _intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IList** receivers)
{
    VALIDATE_NOT_NULL(receivers)
    *receivers = NULL;

    AutoPtr<IIntent> intent = _intent;
    if (!sUserManager->Exists(userId)) {
        AutoPtr<ICollections> cols;
        CCollections::AcquireSingleton((ICollections**)&cols);
        return cols->EmptyList(receivers);
    }
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp == NULL) {
        AutoPtr<IIntent> selector;
        if (intent->GetSelector((IIntent**)&selector), selector != NULL) {
            intent = selector;
            intent->GetComponent((IComponentName**)&comp);
        }
    }
    if (comp != NULL) {
        AutoPtr<IArrayList> al;
        CArrayList::New(1, (IArrayList**)&al);
        *receivers = IList::Probe(al);
        AutoPtr<IActivityInfo> ai;
        GetReceiverInfo(comp, flags, userId, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetActivityInfo(ai);
            (*receivers)->Add(ri);
        }
        REFCOUNT_ADD(*receivers)
        return NOERROR;
    }

    // reader
    synchronized (mPackagesLock) {
        String pkgName;
        intent->GetPackage(&pkgName);
        if (pkgName.IsNull()) {
            *receivers = mReceivers->QueryIntent(intent, resolvedType, flags, userId);
            REFCOUNT_ADD(*receivers)
            return NOERROR;
        }
        AutoPtr<PackageParser::Package> pkg;
        HashMap< String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(pkgName);
        if (it != mPackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg != NULL) {
            *receivers = mReceivers->QueryIntentForPackage(intent, resolvedType, flags,
                    pkg->mReceivers, userId);
            REFCOUNT_ADD(*receivers)
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::ResolveService(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IResolveInfo** resolveInfo)
{
    VALIDATE_NOT_NULL(resolveInfo)
    *resolveInfo = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }
    AutoPtr<IList> query;
    QueryIntentServices(intent, resolvedType, flags, userId, (IList**)&query);
    if (query != NULL) {
        Int32 count;
        query->GetSize(&count);
        if (count >= 1) {
            // If there is more than one service with the same priority,
            // just arbitrarily pick the first one.
            AutoPtr<IInterface> value;
            query->Get(0, (IInterface**)&value);
            *resolveInfo = IResolveInfo::Probe(value);
            REFCOUNT_ADD(*resolveInfo)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::QueryIntentServices(
    /* [in] */ IIntent* _intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IList** services)
{
    VALIDATE_NOT_NULL(services)
    *services = NULL;

    AutoPtr<IIntent> intent = _intent;
    if (!sUserManager->Exists(userId)) {
        AutoPtr<ICollections> cols;
        CCollections::AcquireSingleton((ICollections**)&cols);
        return cols->EmptyList(receivers);
    }
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp == NULL) {
        AutoPtr<IIntent> selector;
        if (intent->GetSelector((IIntent**)&selector), selector != NULL) {
            intent = selector;
            intent->GetComponent((IComponentName**)&comp);
        }
    }
    if (comp != NULL) {
        AutoPtr<IArrayList> al;
        CArrayList::New(1, (IArrayList**)&al);
        *services = IList::Probe(al);
        AutoPtr<IServiceInfo> si;
        GetServiceInfo(comp, flags, userId, (IServiceInfo**)&si);
        if (si != NULL) {
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetServiceInfo(si);
            (*services)->Add(ri);
        }
        REFCOUNT_ADD(*services)
        return NOERROR;
    }

    // reader
    synchronized (mPackagesLock) {
        String pkgName;
        intent->GetPackage(&pkgName);
        if (pkgName.IsNull()) {
            *services = mServices->QueryIntent(intent, resolvedType, flags, userId);
            REFCOUNT_ADD(*services)
            return NOERROR;
        }
        AutoPtr<PackageParser::Package> pkg;
        HashMap< String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(pkgName);
        if (it != mPackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg != NULL) {
            *services = mServices->QueryIntentForPackage(intent, resolvedType, flags,
                pkg->mServices, userId);
            REFCOUNT_ADD(*services)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::QueryIntentContentProviders(
    /* [in] */ IIntent* _intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)
    *infos = NULL;

    AutoPtr<IIntent> intent = _intent;
    if (!sUserManager->Exists(userId)) {
        AutoPtr<ICollections> cols;
        CCollections::AcquireSingleton((ICollections**)&cols);
        return cols->EmptyList(receivers);
    }
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp == NULL) {
        AutoPtr<IIntent> selector;
        if (intent->GetSelector((IIntent**)&selector), selector != NULL) {
            intent = selector;
            intent->GetComponent((IComponentName**)&comp);
        }
    }
    if (comp != NULL) {
        AutoPtr<IArrayList> al;
        CArrayList::New(1, (IArrayList**)&al);
        *infos = IList::Probe(al);
        AutoPtr<IProviderInfo> pi;
        GetProviderInfo(comp, flags, userId, (IProviderInfo**)&pi);
        if (pi != NULL) {
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetProviderInfo(pi);
            (*infos)->Add(ri);
        }
        REFCOUNT_ADD(*infos)
        return NOERROR;
    }

    // reader
    synchronized (mPackagesLock) {
        String pkgName;
        intent->GetPackage(&pkgName);
        if (pkgName.IsNull()) {
            *infos = mProviders->QueryIntent(intent, resolvedType, flags, userId);
            REFCOUNT_ADD(*infos)
            return NOERROR;
        }
        AutoPtr<PackageParser::Package> pkg;
        HashMap< String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(pkgName);
        if (it != mPackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg != NULL) {
            *infos = mProviders->QueryIntentForPackage(
                    intent, resolvedType, flags, pkg.providers, userId);
            REFCOUNT_ADD(*infos)
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::GetInstalledPackages(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice)
    *slice = NULL;

    Boolean listUninstalled = (flags & IPackageManager::GET_UNINSTALLED_PACKAGES) != 0;

    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId,
            TRUE, FALSE, String("get installed packages")))

    // writer
    synchronized (mPackagesLock) {
        AutoPtr<IArrayList> list;
        if (listUninstalled) {
            CArrayList::New(mSettings->mPackages.GetSize(), (IArrayList**)&list);
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Begin();
            for (; it != mSettings->mPackages.End(); ++it) {
                AutoPtr<PackageSetting> ps = it->mSecond;
                AutoPtr<IPackageInfo> pi;
                if (ps->mPkg != NULL) {
                    pi = GeneratePackageInfo(ps->mPkg, flags, userId);
                }
                else {
                    pi = GeneratePackageInfoFromSettingsLPw(ps->mName, flags, userId);
                }
                if (pi != NULL) {
                    list->Add(pi);
                }
            }
        }
        else {
            CArrayList::New(mPackages.GetSize(), (IArrayList**)&list);
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
            for (; it != mPackages.End(); ++it) {
                AutoPtr<PackageParser::Package> p = it->mSecond;
                AutoPtr<IPackageInfo> pi = GeneratePackageInfo(p, flags, userId);
                if (pi != NULL) {
                    list->Add(pi);
                }
            }
        }

        return CParceledListSlice::New(IList::Probe(list), slice);
    }
}

void CPackageManagerService::AddPackageHoldingPermissions(
    /* [in] */ IArrayList* list,
    /* [in] */ PackageSetting* ps,
    /* [in] */ ArrayOf<String>* permissions,
    /* [in] */ ArrayOf<Boolean>* tmp,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    Int32 numMatch = 0;
    AutoPtr<GrantedPermissions> gp = ps->mSharedUser != NULL
            ? (GrantedPermissions*)ps->mSharedUser.Get()
            : (GrantedPermissions*)ps;
    for (Int32 i = 0; i < permissions->GetLength(); i++) {
        HashSet<String>::Iterator it = gp->mGrantedPermissions.Find((*permissions)[i]);
        if (it != gp->mGrantedPermissions.End()) {
            (*tmp)[i] = TRUE;
            numMatch++;
        }
        else {
            (*tmp)[i] = FALSE;
        }
    }
    if (numMatch == 0) {
        return;
    }
    AutoPtr<IPackageInfo> pi;
    if (ps->mPkg != NULL) {
        pi = GeneratePackageInfo(ps->mPkg, flags, userId);
    }
    else {
        pi = GeneratePackageInfoFromSettingsLPw(ps->mName, flags, userId);
    }
    // The above might return null in cases of uninstalled apps or install-state
    // skew across users/profiles.
    if (pi != NULL) {
        if ((flags & IPackageManager::GET_PERMISSIONS) == 0) {
            if (numMatch == permissions->GetLength()) {
                pi->mRequestedPermissions = permissions;
            }
            else {
                pi->mRequestedPermissions = ArrayOf<String>::Alloc(numMatch);
                numMatch = 0;
                for (Int32 i = 0; i < permissions->GetLength(); i++) {
                    if ((*tmp)[i]) {
                        (*pi->mRequestedPermissions)[numMatch] = (*permissions)[i];
                        numMatch++;
                    }
                }
            }
        }
        list->Add(pi);
    }
}

ECode CPackageManagerService::GetPackagesHoldingPermissions(
    /* [in] */ ArrayOf<String>* permissions,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice)
    *slice = NULL;

    Boolean listUninstalled = (flags & IPackageManager::GET_UNINSTALLED_PACKAGES) != 0;

    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId, TRUE,
            , FALSE, String("get installed packages")))

    // writer
    synchronized (mPackagesLock) {
        AutoPtr<IArrayList> list;
        if (listUninstalled) {
            CArrayList::New(mSettings->mPackages.GetSize(), (IArrayList**)&list);
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Begin();
            for (; it != mSettings->mPackages.End(); ++it) {
                AutoPtr<PackageSetting> ps = it->mSecond;
                AutoPtr<IPackageInfo> pi;
                if (ps->mPkg != NULL) {
                    pi = GeneratePackageInfo(ps->mPkg, flags, userId);
                }
                else {
                    pi = GeneratePackageInfoFromSettingsLPw(ps->mName, flags, userId);
                }
                if (pi != NULL) {
                    list->Add(pi);
                }
            }
        }
        else {
            CArrayList::New(mPackages.GetSize(), (IArrayList**)&list);
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
            for (; it != mPackages.End(); ++it) {
                AutoPtr<IPackageInfo> pi = GeneratePackageInfo(it->mSecond, flags, userId);
                if (pi != NULL) {
                    list->Add(pi);
                }
            }
        }

        return CParceledListSlice::New(IList::Probe(list), slice);
    }
}

ECode CPackageManagerService::GetInstalledApplications(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice)
    *slice = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }
    Boolean listUninstalled = (flags & IPackageManager::GET_UNINSTALLED_PACKAGES) != 0;

    // writer
    synchronized (mPackagesLock) {
        AutoPtr<IArrayList> list;
        if (listUninstalled) {
            CArrayList::New(mSettings->mPackages.GetSize(), (IArrayList**)&list);
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Begin();
            for (; it != mSettings->mPackages.End(); ++it) {
                AutoPtr<PackageSetting> ps = it->mSecond;
                AutoPtr<IApplicationInfo> ai;
                if (ps->mPkg != NULL) {
                    ai = PackageParser::GenerateApplicationInfo(ps->mPkg, flags,
                            ps->ReadUserState(userId), userId);
                }
                else {
                    ai = GenerateApplicationInfoFromSettingsLPw(ps->mName, flags, userId);
                }
                if (ai != NULL) {
                    list->Add(ai);
                }
            }
        }
        else {
            CArrayList::New(mPackages.GetSize(), (IArrayList**)&list);
            HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
            for (; it != mPackages.End(); ++it) {
                AutoPtr<PackageParser::Package> p = it->mSecond;
                if (p->mExtras != NULL) {
                    AutoPtr<PackageSettings> s = reinterpret_cast<PackageSetting*>(p->mExtras->Probe(EIID_PackageSetting));
                    AutoPtr<IApplicationInfo> ai = PackageParser::GenerateApplicationInfo(p, flags,
                            s->ReadUserState(userId), userId);
                    if (ai != NULL) {
                        list->Add(ai);
                    }
                }
            }
        }

        return CParceledListSlice::New(IList::Probe(list), slice);
    }
}

ECode CPackageManagerService::GetPersistentApplications(
    /* [in] */ Int32 flags,
    /* [out] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)
    AutoPtr<IArrayList> finalList;
    CArrayList::New((IArrayList**)&finalList);

    // reader
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
        Int32 userId = UserHandle::GetCallingUserId();
        for (; it != mPackages.End(); ++it) {
            AutoPtr<PackageParser::Package> p = it->mSecond;
            Int32 flags;
            if (p->mApplicationInfo != NULL
                    && (p->mApplicationInfo->GetFlags(&flags), (flags & IApplicationInfo::FLAG_PERSISTENT) != 0)
                    && (!mSafeMode || IsSystemApp(p))) {
                AutoPtr<PackageSetting> ps;
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(p->mPackageName);
                if (it != mSettings->mPackages.End()) {
                    ps = it->mSecond;
                }
                if (ps != NULL) {
                    AutoPtr<IApplicationInfo> ai = PackageParser::GenerateApplicationInfo(p, flags,
                            ps->ReadUserState(userId), userId);
                    if (ai != NULL) {
                        finalList->Add(ai);
                    }
                }
            }
        }
    }

    *infos = IList::Probe(finalList);
    REFCOUNT_ADD(*infos)
    return NOERROR;
}

ECode CPackageManagerService::ResolveContentProvider(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IProviderInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    if (!sUserManager->Exists(userId)) {
        return NOERROR;
    }

    //reader
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Provider> provider;
        HashMap< String, AutoPtr<PackageParser::Provider> >::Iterator it = mProvidersByAuthority.Find(name);
        if (it != mProvidersByAuthority.End()) {
            provider = it->mSecond;
        }
        AutoPtr<PackageSetting> ps;
        if (provider != NULL) {
            HashMap< String, AutoPtr<PackageSetting> >::Iterator it =
                    mSettings->mPackages.Find(provider->mOwner->mPackageName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
        }
        if (ps != NULL) {
            AutoPtr<IApplicationInfo> pAppInfo;
            provider->mInfo->GetApplicationInfo((IApplicationInfo**)&pAppInfo);
            Int32 pFlags;
            pAppInfo->GetFlags(&pFlags);
            if (mSettings->IsEnabledLPr(provider->mInfo, flags, userId)
                && (!mSafeMode || (pFlags & IApplicationInfo::FLAG_SYSTEM) != 0)) {
                AutoPtr<IProviderInfo> pi = PackageParser::GenerateProviderInfo(
                        provider, flags, ps->ReadUserState(userId), userId);
                *info = pi;
                REFCOUNT_ADD(*info)
            }
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::QuerySyncProviders(
    /* [in] */ IList* outNames,
    /* [in] */ IList* outInfo)
{
    // reader
    synchronized (mPackagesLock) {
        HashMap< String, AutoPtr<PackageParser::Provider> >::Iterator it;
        Int32 userId = UserHandle::GetCallingUserId();
        for (it = mProvidersByAuthority.Begin(); it != mProvidersByAuthority.End(); ++it) {
            AutoPtr<PackageParser::Provider> p = it->mSecond;
            AutoPtr<PackageSetting> ps;
            HashMap< String, AutoPtr<PackageSetting> >::Iterator it =
                    mSettings->mPackages.Find(p->mOwner->mPackageName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            if (ps != NULL) {
                AutoPtr<IApplicationInfo> appInfo;
                p->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                Int32 flags;
                appInfo->GetFlags(&flags);
                if (p->mSyncable && (!mSafeMode || (flags & IApplicationInfo::FLAG_SYSTEM) != 0)) {
                    AutoPtr<IProviderInfo> info = PackageParser::GenerateProviderInfo(p, 0,
                                ps->ReadUserState(userId), userId);
                    if (info != NULL) {
                        AutoPtr<ICharSequence> name;
                        CString::New(it->mFirst, (ICharSequence**)&name);
                        outNames->Add(name);
                        outInfo->Add(info);
                    }
                }
            }
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::QueryContentProviders(
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 flags,
    /* [out] */ IList** providers)
{
    VALIDATE_NOT_NULL(providers)
    *providers = NULL;

    AutoPtr<IArrayList> finalList;
    // reader
    synchronized (mPackagesLock) {
        HashMap< AutoPtr<IComponentName>, AutoPtr<PackageParser::Provider> >::Iterator it;
        Int32 userId = !processName.IsNull() ?
                UserHandle::GetUserId(uid) : UserHandle::GetCallingUserId();
        for (it = mProvidersByComponent.Begin(); it != mProvidersByComponent.End(); ++it) {
            AutoPtr<PackageParser::Provider> p = it->mSecond;
            AutoPtr<PackageSetting> ps;
            HashMap< String, AutoPtr<PackageSetting> >::Iterator it =
                    mSettings->mPackages.Find(p->mOwner->mPackageName);
            if (it != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }

            String pAuth, pPName;
            p->mInfo->GetAuthority(&pAuth);
            p->mInfo->GetProcessName(&pPName);
            AutoPtr<IApplicationInfo> pAppInfo;
            p->mInfo->GetApplicationInfo((IApplicationInfo**)&pAppInfo);
            Int32 pUid, pFlags;
            pAppInfo->GetUid(&pUid);
            pAppInfo->GetFlags(&pFlags);
            if (ps != NULL && !pAuth.IsNull()
                    && (processName.IsNull()
                            || (pPName.Equals(processName) && UserHandle::IsSameApp(pUid, uid)))
                    && mSettings->IsEnabledLPr(p->mInfo, flags, userId)
                    && (!mSafeMode || (pFlags & IApplicationInfo::FLAG_SYSTEM) != 0)) {
                if (finalList == NULL) {
                    CArrayList::New(3, (IArrayList**)&finalList);
                }
                AutoPtr<IProviderInfo> info = PackageParser::GenerateProviderInfo(p, flags,
                        p->ReadUserState(userId), userId);
                if (info != NULL) {
                    finalList->Add(info);
                }
            }
        }
    }

    if (finalList != NULL) {
        AutoPtr<ICollections> cols;
        CCollections::AcquireSingleton((ICollections**)&cols);
        cols->Sort(IList::Probe(finalList), mProviderInitOrderSorter);
    }

    *providers = IList::Probe(finalList);
    REFCOUNT_ADD(*providers)
    return NOERROR;
}

ECode CPackageManagerService::GetInstrumentationInfo(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 flags,
    /* [out] */ IInstrumentationInfo** instInfo)
{
    VALIDATE_NOT_NULL(instInfo)

    // reader
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Instrumentation> i;
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Instrumentation> >::Iterator it =
                mInstrumentation.Find(name);
        if (it != mInstrumentation.End()) {
            i = it->mSecond;
        }
        AutoPtr<IInstrumentationInfo> info = PackageParser::GenerateInstrumentationInfo(i, flags);
        *instInfo = info;
        REFCOUNT_ADD(*instInfo)
    }
    return NOERROR;
}

ECode CPackageManagerService::QueryInstrumentation(
    /* [in] */ const String& targetPackage,
    /* [in] */ Int32 flags,
    /* [out] */ IList** infos)
{
    VALIDATE_NOT_NULL(infos)
    AutoPtr<IArrayList> finalList;
    CArrayList::New((IArrayList**)&finalList);

    // reader
    synchronized (mPackagesLock) {
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Instrumentation> >::Iterator it;
        for (it = mInstrumentation.Begin(); it != mInstrumentation.End(); ++it) {
            AutoPtr<PackageParser::Instrumentation> i = it->mSecond;
            String iTargetPackage;
            i->mInfo->GetTargetPackage(&iTargetPackage);
            if (targetPackage.IsNull() || targetPackage.Equals(iTargetPackage)) {
                AutoPtr<IInstrumentationInfo> ii =
                        PackageParser::GenerateInstrumentationInfo(i, flags);
                if (ii != NULL) {
                    finalList->Add(ii);
                }
            }
        }
    }
    *infos = IList::Probe(finalList);
    REFCOUNT_ADD(*infos)
    return NOERROR;
}

void CPackageManagerService::CreateIdmapsForPackageLI(
    /* [in] */ PackageParser::Package* pkg)
{
    AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > overlays;
    HashMap<String, AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > >::Iterator it
            = mOverlays.Find(pkg->mPackageName);
    if (it != mOverlays.End()) {
        overlays = it->mSecond;
    }
    if (overlays == NULL) {
        Slogger::W(TAG, "Unable to create idmap for %s: no overlay packages", pkg->mPackageName.string());
        return;
    }
    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator overlayIt = overlays->Begin();
    for (; overlayIt != overlays->End(); ++overlayIt) {
        // Not much to do if idmap fails: we already logged the error
        // and we certainly don't want to abort installation of pkg simply
        // because an overlay didn't fit properly. For these reasons,
        // ignore the return value of createIdmapForPackagePairLI.
        CreateIdmapForPackagePairLI(pkg, overlayIt->mSecond);
    }
}

Boolean CPackageManagerService::CreateIdmapForPackagePairLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ PackageParser::Package* opkg)
{
    if (!opkg->mTrustedOverlay) {
        Slogger::W(TAG, "Skipping target and overlay pair %s and %s: overlay not trusted",
                pkg->mBaseCodePath.string(), opkg->mBaseCodePath.string());
        return FALSE;
    }
    AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > overlaySet;
    HashMap<String, AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > >::Iterator it
            = mOverlays.Find(pkg->mPackageName);
    if (it != mOverlays.End()) {
        overlaySet = it->mSecond;
    }
    if (overlaySet == NULL) {
        Slogger::E(TAG, "was about to create idmap for %s and %s but target package has no known overlays",
                pkg->mBaseCodePath.string(), opkg->mBaseCodePath.string());
        return FALSE;
    }
    Int32 uid;
    pkg->mApplicationInfo->GetUid(&uid);
    Int32 sharedGid = UserHandle::GetSharedAppGid(uid);
    // TODO: generate idmap for split APKs
    if (mInstaller->Idmap(pkg->mBaseCodePath, opkg->mBaseCodePath, sharedGid) != 0) {
        Slogger::E(TAG, "Failed to generate idmap for %s and %s", pkg->mBaseCodePath.string(), opkg->mBaseCodePath.string());
        return FALSE;
    }
    AutoPtr<IHashMap> hm;
    CHashMap::New((IHashMap**)&hm);
    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator setIt = overlaySet->Begin();
    for (; setIt != overlaySet->End(); ++setIt) {
        AutoPtr<ICharSequence> cs;
        CString::New(setIt->mFirst, (ICharSequence**)&cs);
        hm->Put(cs, (IInterface*)setIt->mSecond.Get());
    }

    AutoPtr<ICollection> col;
    hm->GetValue((ICollection**)&col);
    AutoPtr< ArrayOf<IInterface*> > inArray = ArrayOf<IInterface*>::Alloc(0);
    AutoPtr< ArrayOf<IInterface*> > overlayArray;
    col->ToArray(inArray, (ArrayOf<IInterface*>**)&overlayArray);
    AutoPtr<IComparator> cmp = (IComparator*)new PackageComparator();
    Arrays::Sort(overlayArray, cmp);

    AutoPtr< ArrayOf<String> > dirs = ArrayOf<String>::Alloc(overlayArray->GetLength());
    for (Int32 i = 0; i < overlayArray->GetLength(); ++i) {
        AutoPtr<PackageParser::Package> p = (PackageParser::Package*)(IObject*)(*overlayArray)[i];
        (*dirs)[i] = p->mBaseCodePath;
    }
    pkg->mApplicationInfo->SetResourceDirs(dirs);
    return TRUE;
}

void CPackageManagerService::ScanDirLI(
    /* [in] */ IFile* dir,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ Int64 currentTime,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    AutoPtr< ArrayOf<IFile> > files;
    dir->ListFiles((ArrayOf<IFile>**)&files);
    if (ArrayUtils::IsEmpty(files)) {
        Logger::D(TAG, "No files in app dir %p", dir);
        return;
    }

    if (DEBUG_PACKAGE_SCANNING) {
        Logger::D(TAG, "Scanning app dir %p scanFlags=%d flags=%d", dir, scanFlags, parseFlags);
    }

    // ActionsCode(authro:songzhining, comment: fix uninstall bug for apk in vendor/app/app.)
    String path;
    dir->GetPath(&path);
    Boolean isVendorApp = path.StartWith(VENDOR_APP_DIR);
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        AutoPtr<IFile> file = (*files)[i];
        Boolean isDirectory;
        String name;
        file->GetName(&name);
        Boolean isPackage = (IsApkFile(file) ||
                ((file->IsDirectory(&isDirectory), isDirectory) && !isVendorApp))
                && !PackageInstallerService::IsStageName(name);
        if (!isPackage) {
            // Ignore entries which are not packages
            continue;
        }
        // try {
        AutoPtr<PackageParser::Package> pkg
        if (FAILED(ScanPackageLI(file, parseFlags | PackageParser::PARSE_MUST_BE_APK,
                scanFlags, currentTime, NULL, readBuffer, (PackageParser::Package**)&pkg))) {
            Slogger::W(TAG, "Failed to parse %p", file.Get());

            // Delete invalid userdata apps
            if ((parseFlags & PackageParser::PARSE_IS_SYSTEM) == 0 /*&& e.error == PackageManager.INSTALL_FAILED_INVALID_APK*/) {
                String str;
                IObject::Probe(file)->ToString(&str);
                LogCriticalInfo(ILogHelper::WARN, String("Deleting invalid package at ") + str);
                if (file->IsDirectory(&isDirectory), isDirectory) {
                    FileUtils::DeleteContents(file);
                }
                file->Delete();
            }
        }
        // } catch (PackageManagerException e) {
        //     Slog.w(TAG, "Failed to parse " + file + ": " + e.getMessage());

        //     // Delete invalid userdata apps
        //     if ((parseFlags & PackageParser.PARSE_IS_SYSTEM) == 0 &&
        //             e.error == PackageManager.INSTALL_FAILED_INVALID_APK) {
        //         logCriticalInfo(Log.WARN, "Deleting invalid package at " + file);
        //         if (file.isDirectory()) {
        //             FileUtils.deleteContents(file);
        //         }
        //         file.delete();
        //     }
        // }
    }
}

AutoPtr<IFile> CPackageManagerService::GetSettingsProblemFile()
{
    AutoPtr<IFile> dataDir, systemDir, fname;
    dataDir = Environment::GetDataDirectory((IFile**)&dataDir);
    if (FAILED(ec)) {
        Logger::E(TAG, "Failed to get Environment root directory. ec=%08x", ec);
        return NULL;
    }

    Boolean bval;
    dataDir->Exists(&bval);
    if (!bval) {
        ec = dataDir->Mkdir(&bval);
        if (FAILED(ec) || !bval) {
            Logger::E(TAG, "Failed to create Environment root directory. ec=%08x", ec);
            return NULL;
        }
    }

    CFile::New(dataDir, String("system"), (IFile**)&systemDir);
    systemDir->Exists(&bval);
    if (!bval) {
        ec = systemDir->Mkdir(&bval);
        if (FAILED(ec) || !bval) {
            Logger::E(TAG, "Failed to create directory /data/system/. ec=%08x", ec);
            return NULL;
        }
    }

    CFile::New(systemDir, String("uiderrors.txt"), (IFile**)&fname);
    fname->Exists(&bval);
    if (!bval) {
        ec = fname->CreateNewFile(&bval);
        if (FAILED(ec) || !bval) {
            Logger::E(TAG, "Failed to create /data/system/uiderrors.txt. ec=%08x", ec);
            return NULL;
        }
    }

    return fname;
}

void CPackageManagerService::ReportSettingsProblem(
    /* [in] */ Int32 priority,
    /* [in] */ const String& msg)
{
    LogCriticalInfo(priority, msg);


    //try {
    AutoPtr<IFile> fname = GetSettingsProblemFile();
    AutoPtr<IFileOutputStream> out;
    CFileOutputStream::New(fname, TRUE, (IFileOutputStream**)&out);
    assert(out != NULL);

    AutoPtr<IPrintWriter> pw;
    CPrintWriter::New(out, (IPrintWriter**)&pw);
    assert(pw != NULL);

    AutoPtr<ISimpleDateFormat> formatter;
    CSimpleDateFormat::New((ISimpleDateFormat**)&formatter);
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    AutoPtr<IDate> date;
    CDate::New(now, (IDate**)&date);
    String dateString;
    formatter->FormatDate(date, &dateString);
    pw->PrintStringln(dateString + String(": ") + msg);
    ICloseable::Probe(pw)->Close();
    String strfname;
    fname->ToString(&strfname);
    FileUtils::SetPermissions(strfname,
        FileUtils::sS_IRWXU | FileUtils::sS_IRWXG | FileUtils::sS_IROTH, -1, -1);
    //} catch (java.io.IOException e) {
    //}
    Slogger::I(TAG, msg);
}

void CPackageManagerService::LogCriticalInfo(
    /* [in] */ Int32 priority,
    /* [in] */ const String& msg)
{
    Slogger::Println(priority, TAG.string(), msg.string());
    // EventLogTags.writePmCriticalInfo(msg);
    //try {
    AutoPtr<IFile> fname = GetSettingsProblemFile();
    AutoPtr<IFileOutputStream> out;
    CFileOutputStream::New(fname, TRUE, (IFileOutputStream**)&out);
    AutoPtr<IFastPrintWriter> fpw;
    CFastPrintWriter::New(out, (IFastPrintWriter**)&fpw);
    AutoPtr<IPrintWriter> pw = IPrintWriter::Probe(fpw);
    AutoPtr<ISimpleDateFormat> formatter;
    CSimpleDateFormat::New((ISimpleDateFormat**)&formatter);
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    AutoPtr<IDate> date;
    CDate::New(now, (IDate**)&date);
    String dateString;
    formatter->FormatDate(date, &dateString);
    pw->PrintStringln(dateString + ": " + msg);
    ICloseable::Probe(pw)->Close();
    String strfname;
    fname->ToString(&strfname);
    FileUtils::SetPermissions(strfname, FileUtils::sS_IRWXU | FileUtils::sS_IRWXG | FileUtils::sS_IROTH, -1, -1);
    //} catch (java.io.IOException e) {
    //}
}

ECode CPackageManagerService::CollectCertificatesLI(
    /* [in] */ PackageParser* pp,
    /* [in] */ PackageSetting* ps,
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ IFile* srcFile,
    /* [in] */ Int32 parseFlags,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    Boolean equals;
    Int64 time;
    if (ps != NULL
            && (IObject::Probe(ps->mCodePath)->Equals(srcFile, &equals), equals)
            && (srcFile->GetLastModified(&time), ps->mTimeStamp == time)
            && !IsCompatSignatureUpdateNeeded(pkg)
            && !IsRecoverSignatureUpdateNeeded(pkg)) {
        Int64 mSigningKeySetId = ps->mKeySetData->GetProperSigningKeySet();
        if (ps->mSignatures->mSignatures != NULL
                && ps->mSignatures->mSignatures->GetLength() != 0
                && mSigningKeySetId != PackageKeySetData::KEYSET_UNASSIGNED) {
            // Optimization: reuse the existing cached certificates
            // if the package appears to be unchanged.
            pkg->mSignatures = ps->mSignatures->mSignatures;
            AutoPtr<KeySetManagerService> ksms = mSettings->mKeySetManagerService;
            synchronized (mPackagesLock) {
                pkg->mSigningKeys = ksms->GetPublicKeysFromKeySetLPr(mSigningKeySetId);
            }
            return NOERROR;
        }

        Slogger::W(TAG, "PackageSetting for %s is missing signatures.  Collecting certs again to recover them.", ps->mName);
    }
    else {
        String str;
        IObject::Probe(srcFile)->ToString(&str);
        Logger::I(TAG, "%s changed; collecting certs", str.string());
    }

    // try {
    if (FAILED(pp->CollectCertificates(pkg, parseFlags, readBuffer))) {
        mLastScanError = pp->GetParseError();
        return E_PACKAGE_MANAGER_EXCEPTION;
    }
    if (FAILED(pp->collectManifestDigest(pkg))) {
        mLastScanError = pp->GetParseError();
        return E_PACKAGE_MANAGER_EXCEPTION;
    }
    // } catch (PackageParserException e) {
    //     throw PackageManagerException.from(e);
    // }
}

ECode CPackageManagerService::ScanPackageLI(
    /* [in] */ IFile* scanFile,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ Int64 currentTime,
    /* [in] */ IUserHandle* user,
    /* [in] */ ArrayOf<Byte>* readBuffer,
    /* [out] */ PackageParser::Packag** _pkg)
{
    VALIDATE_NOT_NULL(_pkg)
    *_pkg = NULL;

    if (DEBUG_INSTALL) Slogger::D(TAG, "Parsing: %p", scanFile);
    parseFlags |= mDefParseFlags;
    AutoPtr<PackageParser> pp = new PackageParser();
    pp->SetSeparateProcesses(mSeparateProcesses);
    pp->SetOnlyCoreApps(mOnlyCore);
    pp->SetDisplayMetrics(mMetrics);

    if ((scanFlags & SCAN_TRUSTED_OVERLAY) != 0) {
        parseFlags |= PackageParser::PARSE_TRUSTED_OVERLAY;
    }

    AutoPtr<PackageParser::Package> pkg;
    // try {
    if (FAILED(pp->ParsePackage(scanFile, parseFlags, (PackageParser::Package**)&pkg))) {
        mLastScanError = pp->GetParseError();
        return E_PACKAGE_MANAGER_EXCEPTION;
    }
    // } catch (PackageParserException e) {
    //     throw PackageManagerException.from(e);
    // }

    AutoPtr<PackageSetting> ps;
    AutoPtr<PackageSetting> updatedPkg;
    // reader
    synchronized (mPackagesLock) {
        // Look to see if we already know about this package.
        String oldName;
        HashMap<String, String>::Iterator it = mSettings->mRenamedPackages.Find(pkg->mPackageName);
        if (it != mSettings->mRenamedPackages.End()) {
            oldName = it->mSecond;
        }
        if (pkg->mOriginalPackages != NULL && !oldName.IsNull() &&
            Find(pkg->mOriginalPackages->Begin(), pkg->mOriginalPackages->End(), oldName)
            != pkg->mOriginalPackages->End()) {
            // This package has been renamed to its original name.  Let's
            // use that.
            ps = mSettings->PeekPackageLPr(oldName);
        }
        // If there was no original package, see one for the real package name.
        if (ps == NULL) {
            ps = mSettings->PeekPackageLPr(pkg->mPackageName);
        }
        // Check to see if this package could be hiding/updating a system
        // package.  Must look for it either under the original or real
        // package name depending on our state.
        updatedPkg = mSettings->GetDisabledSystemPkgLPr(ps != NULL ? ps->mName : pkg->mPackageName);
        if (DEBUG_INSTALL && updatedPkg != NULL) Slogger::D(TAG, "updatedPkg = %p", updatedPkg.Get());
    }
    Boolean updatedPkgBetter = FALSE;
    // First check if this is a system package that may involve an update
    if (updatedPkg != NULL && (parseFlags & PackageParser::PARSE_IS_SYSTEM) != 0) {
        Boolean isEqual = FALSE;
        if (ps != NULL && (IObject::Probe(ps->mCodePath)->Equals(scanFile, &isEqual), !isEqual)) {
            // The path has changed from what was last scanned...  check the
            // version of the new path against what we have stored to determine
            // what to do.
            if (DEBUG_INSTALL) Slogger::D(TAG, "Path changing from %p", ps->mCodePath.Get());
            if (pkg->mVersionCode < ps->mVersionCode) {
                // The system package has been updated and the code path does not match
                // Ignore entry. Skip it.
                String str;
                IObject::Probe(scanFile)->ToString(&str);
                LogCriticalInfo(ILogHelper::INFO, String("Package ") + ps->mName + " at " + str
                            + " ignored: updated version " + StringUtils::ToString(ps->mVersionCode)
                            + " better than this " + StringUtils::ToString(pkg->mVersionCode));
                Boolean equals;
                if (IObject::Probe(updatedPkg->mCodePath)->Equals(scanFile, &equals), !equals) {
                    Slogger::W(TAG, "Code path for hidden system pkg : %s changing from %s to %s"
                            , ps->mName.string(), updatedPkg->mCodePathString.string(), str.string());
                    updatedPkg->mCodePath = scanFile;
                    updatedPkg->mCodePathString = str;
                    // This is the point at which we know that the system-disk APK
                    // for this package has moved during a reboot (e.g. due to an OTA),
                    // so we need to reevaluate it for privilege policy.
                    if (LocationIsPrivileged(scanFile)) {
                        updatedPkg->mPkgFlags |= IApplicationInfo::FLAG_PRIVILEGED;
                    }
                }
                updatedPkg->mPkg = pkg;
                mLastScanError = IIPackageManager::INSTALL_FAILED_DUPLICATE_PACKAGE;
                return E_PACKAGE_MANAGER_EXCEPTION;
                // throw new PackageManagerException(INSTALL_FAILED_DUPLICATE_PACKAGE, null);
            }
            else {
                // The current app on the system partion is better than
                // what we have updated to on the data partition; switch
                // back to the system partition version.
                // At this point, its safely assumed that package installation for
                // apps in system partition will go through. If not there won't be a working
                // version of the app
                // writer
                synchronized (mPackagesLock) {
                    // Just remove the loaded entries from package lists.
                    mPackages.Erase(ps->mName);
                }
                String str;
                IObject::Probe(scanFile)->ToString(&str);
                LogCriticalInfo(ILogHelper::WARN, String("Package ") + ps->mName + " at " + str
                        + " reverting from " + ps->mCodePathString
                        + ": new version " + StringUtils::ToString(pkg->mVersionCode)
                        + " better than installed " + StringUtils::ToString(ps->mVersionCode));

                AutoPtr<InstallArgs> args = CreateInstallArgsForExisting(PackageFlagsToInstallFlags(ps),
                            ps->mCodePathString, ps->mResourcePathString, ps->mLegacyNativeLibraryPathString,
                            GetAppDexInstructionSets(ps));
                synchronized (mPackagesLock) {
                    args->CleanUpResourcesLI();
                }
                synchronized (mPackagesLock) {
                    mSettings->EnableSystemPackageLPw(ps->mName);
                }
                updatedPkgBetter = TRUE;
            }
        }
    }

    if (updatedPkg != NULL) {
        // An updated system app will not have the PARSE_IS_SYSTEM flag set initially
        parseFlags |= PackageParser::PARSE_IS_SYSTEM;

        // An updated privileged app will not have the PARSE_IS_PRIVILEGED
        // flag set initially
        if ((updatedPkg->mPkgFlags & IApplicationInfo::FLAG_PRIVILEGED) != 0) {
            parseFlags |= PackageParser::PARSE_IS_PRIVILEGED;
        }
    }
    // Verify certificates against what was last scanned
    CollectCertificatesLI(pp, ps, pkg, scanFile, parseFlags, readBuffer);

    /*
     * A new system app appeared, but we already had a non-system one of the
     * same name installed earlier.
     */
    Boolean shouldHideSystemApp = FALSE;
    if (updatedPkg == NULL && ps != NULL
            && (parseFlags & PackageParser::PARSE_IS_SYSTEM_DIR) != 0 && !IsSystemApp(ps)) {
        /*
         * Check to make sure the signatures match first. If they don't,
         * wipe the installed application and its data.
         */
        if (CompareSignatures(ps->mSignatures->mSignatures, pkg->mSignatures)
                != IPackageManager::SIGNATURE_MATCH) {
            LogCriticalInfo(ILogHelper::WARN, String("Package ") + ps->mName + " appeared on system, but"
                    + " signatures don't match existing userdata copy; removing");
            DeletePackageLI(pkg->mPackageName, NULL, TRUE, NULL, NULL, 0, NULL, FALSE, readBuffer);
            ps = NULL;
        }
        else {
            /*
             * If the newly-added system app is an older version than the
             * already installed version, hide it. It will be scanned later
             * and re-added like an update.
             */
            if (pkg->mVersionCode < ps->mVersionCode) {
                shouldHideSystemApp = TRUE;
                String str;
                IObject::Probe(scanFile)->ToString(&str);
                LogCriticalInfo(ILogHelper::INFO, String("Package ") + ps->mName + " appeared at " + str
                        + " but new version " + StringUtils::ToString(pkg->mVersionCode) + " better than installed "
                        + StringUtils::ToString(ps->mVersionCode) + "; hiding system");
            }
            else {
                /*
                 * The newly found system app is a newer version that the
                 * one previously installed. Simply remove the
                 * already-installed application and replace it with our own
                 * while keeping the application data.
                 */
                String str;
                IObject::Probe(scanFile)->ToString(&str);
                LogCriticalInfo(ILogHelper::WARN, String("Package ") + ps->mName + " at " + str
                        + " reverting from " + ps->mCodePathString + ": new version "
                        + StringUtils::ToString(pkg->mVersionCode) + " better than installed " + StringUtils::ToString(ps->mVersionCode));
                AutoPtr<InstallArgs> args = CreateInstallArgsForExisting(PackageFlagsToInstallFlags(ps),
                        ps->mCodePathString, ps->mResourcePathString, ps->mLegacyNativeLibraryPathString,
                        GetAppDexInstructionSets(ps));
                synchronized (mPackagesLock) {
                    args->CleanUpResourcesLI();
                }
            }
        }
    }

    // The apk is forward locked (not public) if its code and resources
    // are kept in different files. (except for app in either system or
    // vendor path).
    // TODO grab this value from PackageSettings
    if ((parseFlags & PackageParser::PARSE_IS_SYSTEM_DIR) == 0) {
        Boolean equals;
        if (ps != NULL &&
                (IObject::Probe(ps->mCodePath)->Equals(ps->mResourcePath, &equals), !equals)) {
            parseFlags |= PackageParser::PARSE_FORWARD_LOCK;
        }
    }

    // TODO: extend to support forward-locked splits
    String resourcePath = NULL;
    String baseResourcePath = NULL;
    if ((parseFlags & PackageParser::PARSE_FORWARD_LOCK) != 0 && !updatedPkgBetter) {
        if (ps != NULL && !ps->mResourcePathString.IsNull()) {
            resourcePath = ps->mResourcePathString;
            baseResourcePath = ps->mResourcePathString;
        }
        else {
            // Should not happen at all. Just log an error.
            Slogger::E(TAG, "Resource path not set for pkg : %s", pkg->mPackageName.string());
        }
    }
    else {
        resourcePath = pkg->mCodePath;
        baseResourcePath = pkg->mBaseCodePath;
    }

    // Set application objects path explicitly.
    pkg->mApplicationInfo->SetCodePath(pkg->mCodePath);
    pkg->mApplicationInfo->SetBaseCodePath(pkg->mBaseCodePath);
    pkg->mApplicationInfo->SetSplitCodePaths(pkg->mSplitCodePaths);
    pkg->mApplicationInfo->SetResourcePath(resourcePath);
    pkg->mApplicationInfo->SetBaseResourcePath(baseResourcePath);
    pkg->mApplicationInfo->SetSplitResourcePaths(pkg->mSplitCodePaths);

    // Note that we invoke the following method only if we are about to unpack an application
    AutoPtr<PackageParser::Package> scannedPkg;
    ScanPackageLI(pkg, parseFlags, scanFlags | SCAN_UPDATE_SIGNATURE,
            currentTime, user, readBuffer, (PackageParser::Package**)&scannedPkg);

    /*
     * If the system app should be overridden by a previously installed
     * data, hide the system app now and let the /data/app scan pick it up
     * again.
     */
    if (shouldHideSystemApp) {
        synchronized (mPackagesLock) {
            /*
             * We have to grant systems permissions before we hide, because
             * grantPermissions will assume the package update is trying to
             * expand its permissions.
             */
            GrantPermissionsLPw(pkg, TRUE, pkg->mPackageName);
            mSettings->DisableSystemPackageLPw(pkg->mPackageName);
        }
    }

    *_pkg = scannedPkg;
    REFCOUNT_ADD(*_pkg)
    return NOERROR;
}

String CPackageManagerService::FixProcessName(
    /* [in] */ const String& defProcessName,
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid)
{
    if (processName.IsNull()) {
        return defProcessName;
    }
    return processName;
}

ECode CPackageManagerService::VerifySignaturesLP(
    /* [in] */ PackageSetting* pkgSetting,
    /* [in] */ PackageParser::Package* pkg)
{
    if (pkgSetting->mSignatures->mSignatures != NULL) {
        // Already existing package. Make sure signatures match
        Boolean match = CompareSignatures(pkgSetting->mSignatures->mSignatures, pkg->mSignatures)
                == IPackageManager::SIGNATURE_MATCH;
        if (!match) {
            match = compareSignaturesCompat(pkgSetting->mSignatures, pkg)
                    == IPackageManager::SIGNATURE_MATCH;
        }
        if (!match) {
            match = compareSignaturesRecover(pkgSetting->mSignatures, pkg)
                    == IPackageManager::SIGNATURE_MATCH;
        }
        if (!match) {
            Slogger::E(TAG, "Package %s signatures do not match the previously installed version; ignoring!",
                    pkg->mPackageName.string());
            mLastScanError = IPackageManager::INSTALL_FAILED_UPDATE_INCOMPATIBLE;
            return E_PACKAGE_MANAGER_EXCEPTION;
        }
    }

    // Check for shared user signatures
    if (pkgSetting->mSharedUser != NULL && pkgSetting->mSharedUser->mSignatures->mSignatures != NULL) {
        // Already existing package. Make sure signatures match
        Boolean match = CompareSignatures(pkgSetting->mSharedUser->mSignatures->mSignatures,
                pkg->mSignatures) == PackageManager::SIGNATURE_MATCH;
        if (!match) {
            match = CompareSignaturesCompat(pkgSetting->mSharedUser->mSignatures, pkg)
                    == PackageManager::SIGNATURE_MATCH;
        }
        if (!match) {
            match = CompareSignaturesRecover(pkgSetting->mSharedUser->mSignatures, pkg)
                    == PackageManager::SIGNATURE_MATCH;
        }
        if (!match) {
            Slogger::E(TAG, "Package %s has no signatures that match those in shared user %s; ignoring!",
                    pkg->mPackageName.string(), pkgSetting->mSharedUser->mName.string());
            mLastScanError = IPackageManager::INSTALL_FAILED_SHARED_USER_INCOMPATIBLE;
            return E_PACKAGE_MANAGER_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::EnforceSystemOrRoot(
    /* [in] */ const String& message)
{
    const Int32 uid = Binder::GetCallingUid();
    if (uid != IProcess::SYSTEM_UID && uid != 0) {
        // throw new SecurityException(message);
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}

ECode CPackageManagerService::PerformBootDexOpt()
{
    FAIL_RETURN(EnforceSystemOrRoot(String("Only the system can request dexopt be performed")))

    // Before everything else, see whether we need to fstrim.
    // try {
    AutoPtr<IPackageHelper> pkgH;
    CPackageHelper::AcquireSingleton((IPackageHelper**)&pkgH);
    AutoPtr<IMountService> ms;
    pkgH->GetMountService((IMountService**)&ms);
    if (ms != NULL) {
        AutoPtr<ISettingsGlobal> global;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&global);
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        Int64 interval;
        global->GetInt64(cr, ISettingsGlobal::FSTRIM_MANDATORY_INTERVAL,
                DEFAULT_MANDATORY_FSTRIM_INTERVAL, &interval);
        if (interval > 0) {
            AutoPtr<ISystem> sys;
            CSystem::AcquireSingleton((ISystem**)&sys);
            Int64 current, last;
            sys->GetCurrentTimeMillis(&current);
            ms->GetLastMaintenance(&last);
            Int64 timeSinceLast = current - last;
            if (timeSinceLast > interval) {
                Slogger::W(TAG, "No disk maintenance in %d; running immediately", timeSinceLast);
                if (!IsFirstBoot()) {
                    // try {
                    AutoPtr<IResources> res;
                    mContext->GetResources((IResources**)&res);
                    String str;
                    res->GetString(R::string::android_upgrading_fstrim, &str);
                    ActivityManagerNative::GetDefault()->ShowBootMessage(str, TRUE);
                    // } catch (RemoteException e) {
                    // }
                }
                ms->RunMaintenance();
            }
        }
    }
    else {
        Slogger::E(TAG, "Mount service unavailable!");
    }
    // } catch (RemoteException e) {
    //     // Can't happen; MountService is local
    // }

    AutoPtr< HashSet<AutoPtr<PackageParser::Package> > > pkgs;
    synchronized (mPackagesLock) {
        pkgs = mDeferredDexOpt;
        mDeferredDexOpt = NULL;
    }

    if (pkgs != NULL) {
        // Sort apps by importance for dexopt ordering. Important apps are given more priority
        // in case the device runs out of space.
        List<AutoPtr<PackageParser::Package> > sortedPkgs;
        // Give priority to core apps.
        HashSet<AutoPtr<PackageParser::Package> >::Iterator pkgIt = pkgs->Begin();
        while (pkgIt != pkgs->End()) {
            AutoPtr<PackageParser::Package> pkg = *pkgIt;
            if (pkg->mCoreApp) {
                if (DEBUG_DEXOPT) {
                    Logger::I(TAG, "Adding core app : %d "/* + sortedPkgs.size()*/, pkg->mPackageName.string());
                }
                sortedPkgs.PushBack(pkg);
                pkgIt = pkgs->Erase(pkgIt);
            }
            else {
                ++pkgIt;
            }
        }
        // Give priority to system apps that listen for pre boot complete.
        AutoPtr<IIntent> intent;
        CIntent::New(IIntent::ACTION_PRE_BOOT_COMPLETED, (IIntent**)&intent);
        HashSet<String>& pkgNames = GetPackageNamesForIntent(intent);
        pkgIt = pkgs->Begin();
        while (pkgIt != pkgs->End()) {
            AutoPtr<PackageParser::Package> pkg = *pkgIt;
            if (pkgNames.Find(pkg->mPackageName) != pkgNames.End()) {
                if (DEBUG_DEXOPT) {
                    Logger::I(TAG, "Adding pre boot system app : %s"/* + sortedPkgs.size()*/, pkg->mPackageName.string());
                }
                sortedPkgs.PushBack(pkg);
                pkgIt = pkgs->Erase(pkgIt);
            }
            else {
                ++pkgIt;
            }
        }
        // Give priority to system apps.
        pkgIt = pkgs->Begin();
        while (pkgIt != pkgs->End()) {
            AutoPtr<PackageParser::Package> pkg = *pkgIt;
            if (IsSystemApp(pkg) && !IsUpdatedSystemApp(pkg)) {
                if (DEBUG_DEXOPT) {
                    Logger::I(TAG, "Adding system app : %s"/* + sortedPkgs.size()*/, pkg->mPackageName.string());
                }
                sortedPkgs.PushBack(pkg);
                pkgIt = pkgs->Erase(pkgIt);
            }
            else {
                ++pkgIt;
            }
        }
        // Give priority to updated system apps.
        pkgIt = pkgs->Begin();
        while (pkgIt != pkgs->End()) {
            AutoPtr<PackageParser::Package> pkg = *pkgIt;
            if (IsUpdatedSystemApp(pkg)) {
                if (DEBUG_DEXOPT) {
                    Logger::I(TAG, "Adding updated system app : %s"/* + sortedPkgs.size()*/, pkg->mPackageName.string());
                }
                sortedPkgs.PushBack(pkg);
                pkgIt = pkgs->Erase(pkgIt);
            }
            else {
                ++pkgIt;
            }
        }
        // Give priority to apps that listen for boot complete.
        intent = NULL;
        CIntent::New(IIntent::ACTION_BOOT_COMPLETED, (IIntent**)&intent);
        pkgNames = GetPackageNamesForIntent(intent);
        pkgIt = pkgs->Begin();
        while (pkgIt != pkgs->End()) {
            AutoPtr<PackageParser::Package> pkg = *pkgIt;
            if (pkgNames.Find(pkg->mPackageName) != pkgNames.End()) {
                if (DEBUG_DEXOPT) {
                    Logger::I(TAG, "Adding boot app : %s"/* + sortedPkgs.size()*/, pkg->mPackageName.string());
                }
                sortedPkgs.PushBack(pkg);
                pkgIt = pkgs->Erase(pkgIt);
            }
            else {
                ++pkgIt;
            }
        }
        // Filter out packages that aren't recently used.
        FilterRecentlyUsedApps(pkgs);
        // Add all remaining apps.
        pkgIt = pkgs->Begin();
        for (; pkgIt != pkgs->End(); ++pkgIt) {
            AutoPtr<PackageParser::Package> pkg = *pkgIt;
            if (DEBUG_DEXOPT) {
                Logger::I(TAG, "Adding app : %s"/* + sortedPkgs.size()*/, pkg->mPackageName.string());
            }
            sortedPkgs.PushBack(pkg);
        }

        Int32 i = 0;
        Int64 total = sortedPkgs.GetSize();
        AutoPtr<IFile> dataDir = Environment::GetDataDirectory();
        AutoPtr<IStorageManagerHelper> storageManagerHelper;
        CStorageManagerHelper::AcquireSingleton((IStorageManagerHelper**)&storageManagerHelper);
        AutoPtr<IStorageManager> storageManager;
        storageManagerHelper->From(mContext, (IStorageManager**)&storageManager);
        Int64 lowThreshold;
        storageManager->GetStorageLowBytes(dataDir, &lowThreshold);
        if (lowThreshold == 0) {
            Slogger::E(TAG, "Invalid low memory threshold");
            return E_ILLEGAL_STATE_EXCEPTION
        }
        List<AutoPtr<PackageParser::Package> >::Iterator sortedPkgIt = sortedPkgs.Begin();
        for (; sortedPkgIt != sortedPkgs.End(); ++sortedPkgIt) {
            AutoPtr<PackageParser::Package> pkg = *sortedPkgIt;
            Int64 usableSpace;
            dataDir->GetUsableSpace(&usableSpace);
            if (usableSpace < lowThreshold) {
                Logger::W(TAG, "Not running dexopt on remaining apps due to low memory: %d", usableSpace);
                break;
            }
            PerformBootDexOpt(pkg, ++i, total);
        }
    }
}

void CPackageManagerService::FilterRecentlyUsedApps(
    /* [in] */ HashSet<AutoPtr<PackageParser::Package> >* pkgs)
{
    // Filter out packages that aren't recently used.
    //
    // The exception is first boot of a non-eng device (aka !mLazyDexOpt), which
    // should do a full dexopt.
    if (mLazyDexOpt || (!IsFirstBoot() && mPackageUsage->IsHistoricalPackageUsageAvailable())) {
        // TODO: add a property to control this?
        Int64 dexOptLRUThresholdInMinutes;
        if (mLazyDexOpt) {
            dexOptLRUThresholdInMinutes = 30; // only last 30 minutes of apps for eng builds.
        }
        else {
            dexOptLRUThresholdInMinutes = 7 * 24 * 60; // apps used in the 7 days for users.
        }
        Int64 dexOptLRUThresholdInMills = dexOptLRUThresholdInMinutes * 60 * 1000;

        Int32 skipped = 0;
        AutoPtr<ISystem> sys;
        CSystem::AcquireSingleton((ISystem**)&sys);
        Int64 now;
        sys->GetCurrentTimeMillis(&now);
        HashSet<AutoPtr<PackageParser::Package> >::Iterator it = pkgs->Begin();
        while (it != pkgs->End()) {
            AutoPtr<PackageParser::Package> pkg = *it;
            Int64 then = pkg->mLastPackageUsageTimeInMills;
            if (then + dexOptLRUThresholdInMills < now) {
                if (DEBUG_DEXOPT) {
                    AutoPtr<IDate> d;
                    CDate::New(then, (IDate**)&d);
                    String str;
                    IObject::Probe(d)->ToString(&str);
                    Logger::I(TAG, "Skipping dexopt of %s last resumed: %s",
                            pkg->mPackageName.string(), ((then == 0) ? "never" : str.string()));
                }
                it = pkgs->Erase(it);
                skipped++;
            }
            else {
                ++it;
            }
        }
        if (DEBUG_DEXOPT) {
            logged::I(TAG, "Skipped optimizing %d of ", skipped/*, total*/);
        }
    }
}

HashSet<String>& CPackageManagerService::GetPackageNamesForIntent(
    /* [in] */ IIntent* intent)
{
    AutoPtr<IList> ris;
    // try {
    AppGlobals::GetPackageManager()->QueryIntentReceivers(
            intent, String(NULL), 0, IUserHandle::USER_OWNER, (IList**)&ris);
    // } catch (RemoteException e) {
    // }
    HashSet<String> pkgNames;
    if (ris != NULL) {
        Boolean hasNext;
        while (ris->HasNext(&hasNext), hasNext) {
            AutoPtr<IInterface> value;
            ris->GetNext((IInterface**)&value);
            AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(value);
            AutoPtr<IActivityInfo> ai;
            ri->GetActivityInfo((IActivityInfo**)&ai);
            String name;
            ai->GetPackageName(&name);
            pkgNames.Insert(name);
        }
    }
    return pkgNames;
}

void CPackageManagerService::PerformBootDexOpt(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 curr,
    /* [in] */ Int32 total)
{
    if (DEBUG_DEXOPT) {
        Logger::I(TAG, "Optimizing app %d of %d: %s", curr, total, pkg->mPackageName.string());
    }
    if (!IsFirstBoot()) {
        // try {
        ActivityManagerNative.getDefault().showBootMessage(
                mContext.getResources().getString(R.string.android_upgrading_apk,
                        curr, total), true);
        // } catch (RemoteException e) {
        // }
    }
    AutoPtr<PackageParser::Package> p = pkg;
    synchronized (mInstallLock) {
        PerformDexOptLI(p, NULL /* instruction sets */, FALSE /* force dex */,
                FALSE /* defer */, TRUE /* include dependencies */);
    }
}

ECode CPackageManagerService::PerformDexOptIfNeeded(
    /* [in] */ const String& packageName,
    /* [in] */ const String& instructionSet,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = PerformDexOpt(packageName, instructionSet, FALSE);
    return NOERROR;
}

String CPackageManagerService::GetPrimaryInstructionSet(
    /* [in] */ IApplicationInfo* info)
{
    if (info->mPrimaryCpuAbi == NULL) {
        return GetPreferredInstructionSet();
    }

    assert(0);
    // return VMRuntime.getInstructionSet(info.primaryCpuAbi);
}

Boolean CPackageManagerService::PerformDexOpt(
    /* [in] */ const String& packageName,
    /* [in] */ const String& instructionSet,
    /* [in] */ Boolean backgroundDexopt)
{
    Boolean dexopt = mLazyDexOpt || backgroundDexopt;
    Boolean updateUsage = !backgroundDexopt;  // Don't update usage if this is just a backgroundDexopt
    if (!dexopt && !updateUsage) {
        // We aren't going to dexopt or update usage, so bail early.
        return FALSE;
    }
    AutoPtr<PackageParser::Package> p;
    String targetInstructionSet;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            p = it->mSecond;
        }
        if (p == NULL) {
            return FALSE;
        }
        if (updateUsage) {
            AutoPtr<ISystem> sys;
            CSystem::AcquireSingleton((ISystem**)&sys);
            sys->GetCurrentTimeMillis(&p->mLastPackageUsageTimeInMills);
        }
        mPackageUsage->Write(FALSE);
        if (!dexopt) {
            // We aren't going to dexopt, so bail early.
            return FALSE;
        }

        targetInstructionSet = instructionSet != NULL ? instructionSet :
                GetPrimaryInstructionSet(p->mApplicationInfo);
        AutoPtr<ICharSequence> cs;
        CString::New(targetInstructionSet, (ICharSequence**)&cs);
        Boolean contains;
        if (p->mDexOptPerformed->Contains(cs, &contains), contains) {
            return FALSE;
        }
    }

    synchronized (mInstallLock) {
        AutoPtr< ArrayOf<String> > instructionSets = ArrayOf<String>::Alloc(1);
        (*instructionSets)[0] = targetInstructionSet;
        return PerformDexOptLI(p, instructionSets, FALSE /* force dex */, FALSE /* defer */,
                TRUE /* include dependencies */) == DEX_OPT_PERFORMED;
    }
}

AutoPtr< HashSet<String> > CPackageManagerService::GetPackagesThatNeedDexOpt()
{
    AutoPtr< HashSet<String> > pkgs;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
        for (; it != mPackages.End(); ++it) {
            AutoPtr<PackageParser::Package> p = it->mSecond;
            // if (DEBUG_DEXOPT) {
            //     Logger::I(TAG, p.packageName + " mDexOptPerformed=" + p.mDexOptPerformed.toArray());
            // }
            Boolean isEmpty;
            if (p->mDexOptPerformed->IsEmpty(&isEmpty), !isEmpty) {
                continue;
            }
            if (pkgs == NULL) {
                pkgs = new HashSet<String>();
            }
            pkgs->Insert(p->mPackageName);
        }
    }
    return pkgs;
}

void CPackageManagerService::Shutdown()
{
    mPackageUsage->Write(TRUE);
}

void CPackageManagerService::PerformDexOptLibsLI(
    /* [in] */ List<String>* libs,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ Boolean forceDex,
    /* [in] */ Boolean defer,
    /* [in] */ HashSet<String>* done)
{
    List<String>::Iterator libIt = libs->Begin();
    for (; libIt != libs->End(); ++libIt) {
        AutoPtr<PackageParser::Package> libPkg;
        String libName;
        synchronized (mPackagesLock) {
            libName = *it;
            AutoPtr<SharedLibraryEntry> lib;
            HashMap<String, AutoPtr<SharedLibraryEntry> >::Iterator it = mSharedLibraries.Find(libName);
            if (it != mSharedLibraries.End()) {
                lib = it->mSecond;
            }
            if (lib != NULL && !lib->mApk.IsNull()) {
                HashMap<String, AutoPtr<PackageParser::Package> >::Iterator pkgIt = mPackages.Find(lib->mApk);
                if (pkgIt != mPackages.End()) {
                    libPkg = pkgIt->mSecond;
                }
            }
            else {
                libPkg = NULL;
            }
        }
        if (libPkg != NULL && done->Find(libName) == done->End()) {
            PerformDexOptLI(libPkg, instructionSets, forceDex, defer, done);
        }
    }
}

Int32 CPackageManagerService::PerformDexOptLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ ArrayOf<String>* targetInstructionSets,
    /* [in] */ Boolean forceDex,
    /* [in] */ Boolean defer,
    /* [in] */ HashSet<String>* done)
{
    AutoPtr< ArrayOf<String> > instructionSets = targetInstructionSets != NULL ?
            targetInstructionSets : GetAppDexInstructionSets(pkg->mApplicationInfo);

    if (done != NULL) {
        done->Insert(pkg->mPackageName);
        if (pkg->mUsesLibraries != NULL) {
            PerformDexOptLibsLI(pkg->mUsesLibraries, instructionSets, forceDex, defer, done);
        }
        if (pkg->mUsesOptionalLibraries != NULL) {
            PerformDexOptLibsLI(pkg->mUsesOptionalLibraries, instructionSets, forceDex, defer, done);
        }
    }

    Int32 flags;
    if ((pkg->mApplicationInfo->GetFlags(flags), (flags & IApplicationInfo::FLAG_HAS_CODE) == 0) {
        return DEX_OPT_SKIPPED;
    }

    Boolean vmSafeMode = (flags & IApplicationInfo::FLAG_VM_SAFE_MODE) != 0;

    AutoPtr<List<String> > paths = pkg->GetAllCodePathsExcludingResourceOnly();
    Boolean performedDexOpt = FALSE;
    // There are three basic cases here:
    // 1.) we need to dexopt, either because we are forced or it is needed
    // 2.) we are defering a needed dexopt
    // 3.) we are skipping an unneeded dexopt
    AutoPtr< ArrayOf<String> > dexCodeInstructionSets = GetDexCodeInstructionSets(instructionSets);
    for (Int32 i = 0; i < dexCodeInstructionSets->GetLength(); ++i) {
        String dexCodeInstructionSet = (*dexCodeInstructionSets)[i];
        AutoPtr<ICharSequence> cs;
        CString::New(dexCodeInstructionSet, (ICharSequence**)&cs);
        Boolean contains;
        if (!forceDex && (pkg->mDexOptPerformed->Contains(cs, &contains), contains)) {
            continue;
        }

        List<String>::Iterator pathIt = paths->Begin();
        for (; pathIt != paths->End(); ++pathIt) {
            String path = *pathIt;
            // try {
            // This will return DEXOPT_NEEDED if we either cannot find any odex file for this
            // patckage or the one we find does not match the image checksum (i.e. it was
            // compiled against an old image). It will return PATCHOAT_NEEDED if we can find a
            // odex file and it matches the checksum of the image but not its base address,
            // meaning we need to move it.
            // TODO:
            // Byte isDexOptNeeded = DexFile.isDexOptNeededInternal(path,
            //         pkg.packageName, dexCodeInstructionSet, defer);
            if (forceDex /*|| (!defer && isDexOptNeeded == DexFile.DEXOPT_NEEDED)*/) {
                AutoPtr<IApplicationInfo> ai;
                pkg->GetApplicationInfo((IApplicationInfo**)&ai);
                String packageName;
                ai->GetPackageName(&packageName);
                Logger::I(TAG, "Running dexopt on: %s pkg=%s isa=%s vmSafeMode=%d"
                        , path.string(), packageName.string(), dexCodeInstructionSet.string(), vmSafeMode);
                Int32 uid;
                ai->GetUid(&uid);
                Int32 sharedGid = UserHandle::GetSharedAppGid(uid);
                Int32 ret = mInstaller->Dexopt(path, sharedGid, !IsForwardLocked(pkg),
                        pkg->mPackageName, dexCodeInstructionSet, vmSafeMode);

                if (ret < 0) {
                    // Don't bother running dexopt again if we failed, it will probably
                    // just result in an error again. Also, don't bother dexopting for other
                    // paths & ISAs.
                    return DEX_OPT_FAILED;
                }

                performedDexOpt = TRUE;
            }
            else if (!defer /*&& isDexOptNeeded == DexFile.PATCHOAT_NEEDED*/) {
                AutoPtr<IApplicationInfo> ai;
                pkg->GetApplicationInfo((IApplicationInfo**)&ai);
                String packageName;
                ai->GetPackageName(&packageName);
                Logger::I(TAG, "Running patchoat on: %s", packageName.string());
                Int32 uid;
                ai->GetUid(&uid);
                Int32 sharedGid = UserHandle::GetSharedAppGid(uid);
                Int32 ret = mInstaller->Patchoat(path, sharedGid, !IsForwardLocked(pkg),
                        pkg->mPackageName, dexCodeInstructionSet);

                if (ret < 0) {
                    // Don't bother running patchoat again if we failed, it will probably
                    // just result in an error again. Also, don't bother dexopting for other
                    // paths & ISAs.
                    return DEX_OPT_FAILED;
                }

                performedDexOpt = TRUE;
            }

            // We're deciding to defer a needed dexopt. Don't bother dexopting for other
            // paths and instruction sets. We'll deal with them all together when we process
            // our list of deferred dexopts.
            if (defer /*&& isDexOptNeeded != DexFile.UP_TO_DATE*/) {
                if (mDeferredDexOpt == NULL) {
                    mDeferredDexOpt = new HashSet<AutoPtr<PackageParser::Package> >();
                }
                mDeferredDexOpt->Insert(pkg);
                return DEX_OPT_DEFERRED;
            }
            // } catch (FileNotFoundException e) {
            //     Slog.w(TAG, "Apk not found for dexopt: " + path);
            //     return DEX_OPT_FAILED;
            // } catch (IOException e) {
            //     Slog.w(TAG, "IOException reading apk: " + path, e);
            //     return DEX_OPT_FAILED;
            // } catch (StaleDexCacheError e) {
            //     Slog.w(TAG, "StaleDexCacheError when reading apk: " + path, e);
            //     return DEX_OPT_FAILED;
            // } catch (Exception e) {
            //     Slog.w(TAG, "Exception when doing dexopt : ", e);
            //     return DEX_OPT_FAILED;
            // }
        }

        // At this point we haven't failed dexopt and we haven't deferred dexopt. We must
        // either have either succeeded dexopt, or have had isDexOptNeededInternal tell us
        // it isn't required. We therefore mark that this package doesn't need dexopt unless
        // it's forced. performedDexOpt will tell us whether we performed dex-opt or skipped
        // it.
        AutoPtr<ICharSequence> setCs;
        CString::New(dexCodeInstructionSet, (ICharSequence**)&setCs);
        pkg->mDexOptPerformed->Add(cs);
    }

    // If we've gotten here, we're sure that no error occurred and that we haven't
    // deferred dex-opt. We've either dex-opted one more paths or instruction sets or
    // we've skipped all of them because they are up to date. In both cases this
    // package doesn't need dexopt any longer.
    return performedDexOpt ? DEX_OPT_PERFORMED : DEX_OPT_SKIPPED;
}

AutoPtr< ArrayOf<String> > CPackageManagerService::GetAppDexInstructionSets(
    /* [in] */ IApplicationInfo* info)
{
    String primaryCpuAbi;
    if (info->GetPrimaryCpuAbi(&primaryCpuAbi), !primaryCpuAbi.IsNull()) {
        String secondaryCpuAbi;
        if (info->GetSecondaryCpuAbi(&secondaryCpuAbi), !secondaryCpuAbi.IsNull()) {
            AutoPtr< ArrayOf<String> > sets = ArrayOf<String>::Alloc(2);
            // (*sets)[0] = VMRuntime.getInstructionSet(primaryCpuAbi);
            // (*sets)[1] = VMRuntime.getInstructionSet(secondaryCpuAbi);
            // return sets;
            assert(0);
        }
        else {
            AutoPtr< ArrayOf<String> > sets = ArrayOf<String>::Alloc(1);
            // (*sets)[0] = VMRuntime.getInstructionSet(primaryCpuAbi);
            // return sets;
            assert(0);
        }
    }

    AutoPtr< ArrayOf<String> > sets = ArrayOf<String>::Alloc(1);
    (*sets)[0] = GetPreferredInstructionSet();
    return sets;
}

AutoPtr< ArrayOf<String> > CPackageManagerService::GetAppDexInstructionSets(
    /* [in] */ PackageSetting* ps)
{
    if (!ps->mPrimaryCpuAbiString.IsNull()) {
        if (!ps->mSecondaryCpuAbiString.IsNull()) {
            AutoPtr< ArrayOf<String> > sets = ArrayOf<String>::Alloc(2);
            // (*sets)[0] = VMRuntime.getInstructionSet(ps->mPrimaryCpuAbi);
            // (*sets)[1] = VMRuntime.getInstructionSet(ps->mSecondaryCpuAbi);
            // return sets;
            assert(0);
        }
        else {
            AutoPtr< ArrayOf<String> > sets = ArrayOf<String>::Alloc(1);
            // (*sets)[0] = VMRuntime.getInstructionSet(ps->mPrimaryCpuAbi);
            // return sets;
            assert(0);
        }
    }

    AutoPtr< ArrayOf<String> > sets = ArrayOf<String>::Alloc(1);
    (*sets)[0] = GetPreferredInstructionSet();
    return sets;
}

String CPackageManagerService::GetPreferredInstructionSet()
{
    if (sPreferredInstructionSet.IsNull()) {
        assert(0);
        // sPreferredInstructionSet = VMRuntime.getInstructionSet(Build.SUPPORTED_ABIS[0]);
    }

    return sPreferredInstructionSet;
}

List<String>& CPackageManagerService::GetAllInstructionSets()
{
    AutoPtr< ArrayOf<String> > allAbis = Build::SUPPORTED_ABIS;
    List<String> allInstructionSets;

    for (Int32 i = 0; i < allAbis->GetLength(); ++i) {
        String abi = (*allAbis)[i];
        // String instructionSet = VMRuntime.getInstructionSet(abi);
        // if (!allInstructionSets.contains(instructionSet)) {
        //     allInstructionSets.add(instructionSet);
        // }
        assert(0);
    }

    return allInstructionSets;
}

String CPackageManagerService::GetDexCodeInstructionSet(
    /* [in] */ const String& sharedLibraryIsa)
{
    AutoPtr<ISystemProperties> prop;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&prop);
    String dexCodeIsa;
    prop->Get(String("ro.dalvik.vm.isa.") + sharedLibraryIsa, &dexCodeIsa);
    return (dexCodeIsa.IsEmpty() ? sharedLibraryIsa : dexCodeIsa);
}

AutoPtr< ArrayOf<String> > CPackageManagerService::GetDexCodeInstructionSets(
    /* [in] */ ArrayOf<String>* instructionSets)
{
    Int32 len = instructionSets->GetLength();
    AutoPtr< ArrayOf<String> > dexCodeInstructionSets = ArrayOf<String>::Alloc(len);
    for (Int32 i = 0; i < len, i++) {
        (*dexCodeInstructionSets)[i] = GetDexCodeInstructionSet((*instructionSets)[i]);
    }
    return dexCodeInstructionSets;
}

AutoPtr< ArrayOf<String> > CPackageManagerService::GetAllDexCodeInstructionSets()
{
    AutoPtr< ArrayOf<String> > supportedInstructionSets = ArrayOf<String>::Alloc(Build::SUPPORTED_ABIS->GetLength());
    for (Int32 i = 0; i < supportedInstructionSets->GetLength(); i++) {
        String abi = (*Build::SUPPORTED_ABIS)[i];
        assert(0);
        // supportedInstructionSets[i] = VMRuntime.getInstructionSet(abi);
    }
    return GetDexCodeInstructionSets(supportedInstructionSets);
}

ECode CPackageManagerService::ForceDexOpt(
    /* [in] */ const String& packageName)
{
    FAIL_RETURN(EnforceSystemOrRoot("forceDexOpt")))

    AutoPtr<PackageParser::Package> pkg;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
        if (it != mPackages.End()) {
            pkg  = it->mSecond;
        }
        if (pkg == NULL) {
            Logger::E(TAG, "Missing package: %s", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    synchronized (mInstallLock) {
        AutoPtr< ArrayOf<String> > instructionSets = ArrayOf<String>::Alloc(1);
        (*instructionSets)[0] = GetPrimaryInstructionSet(pkg->mApplicationInfo);
        Int32 res = PerformDexOptLI(pkg, instructionSets, TRUE, FALSE, TRUE);
        if (res != DEX_OPT_PERFORMED) {
            Slogger::E(TAG, "Failed to dexopt: %d", res);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    return NOERROR;
}

Int32 CPackageManagerService::PerformDexOptLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ ArrayOf<String>* instructionSets,
    /* [in] */ Boolean forceDex,
    /* [in] */ Boolean defer,
    /* [in] */ Boolean inclDependencies)
{
    AutoPtr< HashSet<String> > done;
    if (inclDependencies && (pkg->mUsesLibraries != NULL || pkg->mUsesOptionalLibraries != NULL)) {
        done = new HashSet<String>();
        done->Insert(pkg->mPackageName);
    }
    else {
        done = NULL;
    }
    return PerformDexOptLI(pkg, instructionSets,  forceDex, defer, done);
}

Boolean CPackageManagerService::VerifyPackageUpdateLPr(
    /* [in] */ PackageSetting* oldPkg,
    /* [in] */ PackageParser::Package* newPkg)
{
    if ((oldPkg->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0) {
        Slogger::W(TAG, "Unable to update from %s to %s: old package not in system partition",
                oldPkg->mName.string(), newPkg->mPackageName.string());
        return FALSE;
    }
    else if (mPackages.Find(oldPkg->mName) != mPackages.End()) {
        Slogger::W(TAG, "Unable to update from %s to %s: old package still exists",
                oldPkg->mName.string(), newPkg->mPackageName.string());
        return FALSE;
    }
    return TRUE;
}

AutoPtr<IFile> CPackageManagerService::GetDataPathForUser(
    /* [in] */ Int32 userId)
{
    AutoPtr<IFile> file;
    String path;
    mUserAppDataDir->GetAbsolutePath(&path);
    AutoPtr<IFileHelper> fh;
    CFileHelper::AcquireSingleton((IFileHelper**)&fh);
    String separator;
    fh->GetSeparator(&separator);
    StringBuilder sb(path);
    sb += separator;
    sb += userId;
    CFile::New(sb.ToString(), (IFile**)&file);
    return file;
}

AutoPtr<IFile> CPackageManagerService::GetDataPathForPackage(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId)
{
    /*
     * Until we fully support multiple users, return the directory we
     * previously would have. The PackageManagerTests will need to be
     * revised when this is changed back..
     */
    AutoPtr<IFile> file;
    if (userId == 0) {
        CFile::New(mAppDataDir, packageName, (IFile**)&file);
    }
    else {
        String path;
        mUserAppDataDir->GetAbsolutePath(&path);
        AutoPtr<IFileHelper> fh;
        CFileHelper::AcquireSingleton((IFileHelper**)&fh);
        String separator;
        fh->GetSeparator(&separator);
        StringBuilder sb(path);
        sb += separator;
        sb += userId;
        sb += separator;
        sb += packageName;
        CFile::New(sb.ToString(), (IFile**)&file);
    }

    return file;
}

Int32 CPackageManagerService::CreateDataDirsLI(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [in] */ const String& seinfo)
{
    AutoPtr<ArrayOf<Int32> > users = sUserManager->GetUserIds();
    Int32 res = mInstaller->Install(packageName, uid, uid, seinfo);
    if (res < 0) {
        return res;
    }
    Int32 len = users->GetLength();
    for (Int32 i = 0; i < len; i++) {
        Int32 user = (*users)[i];
        if (user != 0) {
            res = mInstaller->CreateUserData(packageName,
                    UserHandle::GetUid(user, uid), user, seinfo);
            if (res < 0) {
                return res;
            }
        }
    }
    return res;
}

Int32 CPackageManagerService::RemoveDataDirsLI(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<Int32> > users = sUserManager->GetUserIds();
    Int32 res = 0;
    Int32 len = users->GetLength();
    for (Int32 i = 0; i < len; i++) {
        Int32 user = (*users)[i];
        Int32 resInner = mInstaller->Remove(packageName, user);
        if (resInner < 0) {
            res = resInner;
        }
    }
    return res;
}

Int32 CPackageManagerService::DeleteCodeCacheDirsLI(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<Int32> > users = sUserManager->GetUserIds();
    Int32 res = 0;
    for (Int32 i = 0; i < users->GetLength(); i++) {
        Int32 resInner = mInstaller->DeleteCodeCacheFiles(packageName, user);
        if (resInner < 0) {
            res = resInner;
        }
    }
    return res;
}

void CPackageManagerService::AddSharedLibraryLPw(
    /* [in] */ List<String>* usesLibraryFiles,
    /* [in] */ SharedLibraryEntry* file,
    /* [in] */ PackageParser::Package* changingLib)
{
    if (!file->mPath.IsNull()) {
        usesLibraryFiles->PushBack(file->mPath);
        return;
    }
    AutoPtr<PackageParser::Package> p;
    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(file->mApk);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if (changingLib != NULL && changingLib->mPackageName.Equals(file->mApk)) {
        // If we are doing this while in the middle of updating a library apk,
        // then we need to make sure to use that new apk for determining the
        // dependencies here.  (We haven't yet finished committing the new apk
        // to the package manager state.)
        if (p == NULL || p->mPackageName.Equals(changingLib->mPackageName)) {
            p = changingLib;
        }
    }
    if (p != NULL) {
        AutoPtr<List<String> > paths = p->GetAllCodePaths();
        usesLibraryFiles->Insert(usesLibraryFiles->End(), paths->Begin(), paths->End());
    }
}

ECode CPackageManagerService::UpdateSharedLibrariesLPw(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ PackageParser::Package* changingLib)
{
    if (pkg->mUsesLibraries != NULL || pkg->mUsesOptionalLibraries != NULL) {
        AutoPtr<List<String> > usesLibraryFiles = new List<String>();
        if (pkg->mUsesLibraries != NULL) {
            List<String>::Iterator it = pkg->mUsesLibraries->Begin();
            for (; it != pkg->mUsesLibraries->End(); ++it) {
                AutoPtr<SharedLibraryEntry> file;
                HashMap<String, AutoPtr<SharedLibraryEntry> >::Iterator libIt = mSharedLibraries.Find(*it);
                if (libIt != mSharedLibraries.End() {
                    file = libIt->mSecond;
                }
                if (file == NULL) {
                    Slogger::E(TAG, "%sPackage %s requires unavailable shared library %s; failing!"
                            , IPackageManager::INSTALL_FAILED_MISSING_SHARED_LIBRARY.string(), pkg->mPackageName.string()
                            , (*it).string());
                    mLastScanError = IPackageManager::INSTALL_FAILED_MISSING_SHARED_LIBRARY;
                    return E_PACKAGE_MANAGER_EXCEPTION;
                }
                AddSharedLibraryLPw(usesLibraryFiles, file, changingLib);
            }
        }

        N = pkg.usesOptionalLibraries != null ? pkg.usesOptionalLibraries.size() : 0;
        if (pkg->mUsesOptionalLibraries != NULL) {
            List<String>::Iterator it = pkg->mUsesOptionalLibraries->Begin();
            for (; it != pkg->mUsesOptionalLibraries->End(); ++it) {
                AutoPtr<SharedLibraryEntry> file;
                HashMap<String, AutoPtr<SharedLibraryEntry> >::Iterator libIt = mSharedLibraries.Find(*it);
                if (libIt != mSharedLibraries.End() {
                    file = libIt->mSecond;
                }
                if (file == NULL) {
                    Slogger::W(TAG, "Package %s desires unavailable shared library %s; ignoring!"
                            , pkg->mPackageName.string(), (*it).string());
                }
                else {
                    AddSharedLibraryLPw(usesLibraryFiles, file, changingLib);
                }
            }
        }
        N = usesLibraryFiles.size();
        if (usesLibraryFiles->Begin() != usesLibraryFiles->End()) {
            pkg->mUsesLibraryFiles = usesLibraryFiles;
        }
        else {
            pkg->mUsesLibraryFiles = NULL;
        }
    }

    return NOERROR;
}

Boolean CPackageManagerService::HasString(
    /* [in] */ List<String>* list,
    /* [in] */ List<String>* which)
{
    if (list == NULL) {
        return FALSE;
    }
    List<String>::ReverseIterator listRIt = list->RBegin();
    for (; listRIt != list->REnd(); ++listRIt) {
        List<String>::ReverseIterator whichRIt = which->RBegin();
        for (; whichRIt != which->REnd(); ++whichRIt) {
            if ((*whichRIt).Equals(*listRIt)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void CPackageManagerService::UpdateAllSharedLibrariesLPw()
{
    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
    for (; it != mpackages.End(); ++it) {
        // try {
        if (FAILED(UpdateSharedLibrariesLPw(it->mSecond, NULL))) {
            Slogger::E(TAG, "updateAllSharedLibrariesLPw failed: ");
        }
        // } catch (PackageManagerException e) {
        //     Slog.e(TAG, "updateAllSharedLibrariesLPw failed: " + e.getMessage());
        // }
    }
}

AutoPtr<List<AutoPtr<PackageParser::Package> > > CPackageManagerService::UpdateAllSharedLibrariesLPw(
    /* [in] */ PackageParser::Package* changingPkg)
{
    AutoPtr<List<AutoPtr<PackageParser::Package> > > res;
    HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Begin();
    for (; it != mpackages.End(); ++it) {
        if (HasString(pkg->mUsesLibraries, changingPkg->mLibraryNames)
                || HasString(pkg->mUsesOptionalLibraries, changingPkg->mLibraryNames)) {
            AutoPtr<PackageParser::Package> pkg = it->mSecond;
            if (res == NULL) {
                res = new List<AutoPtr<PackageParser::Package> >();
            }
            res->PushBack(pkg);
            // try {
            if (FAILED(UpdateSharedLibrariesLPw(pkg, changingPkg))) {
                Slogger::E(TAG, "updateAllSharedLibrariesLPw failed: ");
            }
            // } catch (PackageManagerException e) {
            //     Slog.e(TAG, "updateAllSharedLibrariesLPw failed: " + e.getMessage());
            // }
        }
    }
    return res;
}

String CPackageManagerService::DeriveAbiOverride(
    /* [in] */ const String& abiOverride,
    /* [in] */ PackageSetting* settings)
{
    String cpuAbiOverride = NULL;

    if (INativeLibraryHelper::CLEAR_ABI_OVERRIDE.Equals(abiOverride)) {
        cpuAbiOverride = NULL;
    }
    else if (!abiOverride.IsNull()) {
        cpuAbiOverride = abiOverride;
    }
    else if (settings != NULL) {
        cpuAbiOverride = settings->mCpuAbiOverrideString;
    }

    return cpuAbiOverride;
}

AutoPtr<PackageParser::Package> CPackageManagerService::ScanPackageLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime,
    /* [in] */ IUserHandle* user,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    Boolean success = FALSE;
    // try {
    AutoPtr<PackageParser::Package> res;
    if (FAILED(ScanPackageDirtyLI(pkg, parseFlags, scanFlags,
            currentTime, user, (PackageParser::Package**)&res))) {
        goto fail;
    }
    success = TRUE;

fail:
    if (!success && (scanFlags & SCAN_DELETE_DATA_ON_FAILURES) != 0) {
        RemoveDataDirsLI(pkg->mPackageName);
    }
    return res;
    // } finally {
    //     if (!success && (scanFlags & SCAN_DELETE_DATA_ON_FAILURES) != 0) {
    //         removeDataDirsLI(pkg.packageName);
    //     }
    // }
}

ECode CPackageManagerService::ScanPackageDirtyLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ Int64 currentTime,
    /* [in] */ IUserHandle* user,
    /* [out] */ PackageParser::Package* outPkg)
{
    VALIDATE_NOT_NULL(outPkg)
    *outPkg = NULL;

    AutoPtr<IFile> scanFile;
    CFile::New(pkg->mCodePath, (IFile**)&scanFile);
    String codePath, resPath;
    if ((pkg->mApplicationInfo->GetCodePath(&codePath), codePath.IsNull()) ||
            (pkg->mApplicationInfo->GetResourcePath(&resPath), resPath.IsNull())) {
        // Bail out. The resource and code paths haven't been set.
        Slogger::E(TAG, "%s Code and resource paths haven't been set correctly %d"
                , IPackageManager::INSTALL_FAILED_INVALID_APK);
        mLastScanError = IPackageManager::INSTALL_FAILED_INVALID_APK;
        return E_PACKAGE_MANAGER_EXCEPTION;
    }

    if ((parseFlags & PackageParser::PARSE_IS_SYSTEM) != 0) {
        Int32 flags;
        pkg->mApplicationInfo->GetFlags(&flags);
        flags |= IApplicationInfo::FLAG_SYSTEM;
        pkg->mApplicationInfo->SetFlags(flags);
    }
    else {
        // Only allow system apps to be flagged as core apps.
        pkg->mCoreApp = FALSE;
    }

    if ((parseFlags & PackageParser::PARSE_IS_PRIVILEGED) != 0) {
        Int32 flags;
        pkg->mApplicationInfo->GetFlags(&flags);
        flags |= IApplicationInfo::FLAG_PRIVILEGED;
        pkg->mApplicationInfo->SetFlags(flags);
    }

    String packageName;
    if (mCustomResolverComponentName != NULL &&
            (mCustomResolverComponentName->GetPackageName(&packageName), packageName.Equals(pkg->mPackageName))) {
        SetUpCustomResolverActivity(pkg);
    }

    if (pkg->mPackageName.Equals("android")) {
        synchronized (mPackagesLock) {
            if (mElastosApplication != NULL) {
                Slogger::W(TAG, "*************************************************");
                Slogger::W(TAG, "Core android package being redefined.  Skipping.");
                // Slogger::W(TAG, " file=" + scanFile);
                Slogger::W(TAG, "*************************************************");
                mLastScanError = IPackageManager::INSTALL_FAILED_DUPLICATE_PACKAGE;
                Slogger::E(TAG, "%s Core android package being redefined.  Skipping. %d"
                        , IPackageManager::INSTALL_FAILED_DUPLICATE_PACKAGE);
                return E_PACKAGE_MANAGER_EXCEPTION;
            }

            // Set up information for our fall-back user intent resolution activity.
            mPlatformPackage = pkg;
            pkg->mVersionCode = mSdkVersion;
            mElastosApplication = pkg->mApplicationInfo;

            if (!mResolverReplaced) {
                mResolveActivity->SetApplicationInfo(mElastosApplication);
                mResolveActivity->SetName(String("com.android.internal.app.ResolverActivity") /*ResolverActivity.class.getName()*/);
                String pkgName;
                mElastosApplication->GetPackageName(&pkgName);
                mResolveActivity->SetPackageName(pkgName);
                mResolveActivity->SetProcessName(String("system:ui"));
                mResolveActivity->SetLaunchMode(IActivityInfo::LAUNCH_MULTIPLE);
                mResolveActivity->SetDocumentLaunchMode(IActivityInfo::DOCUMENT_LAUNCH_NEVER);
                mResolveActivity->SetFlags(IActivityInfo::FLAG_EXCLUDE_FROM_RECENTS);
                mResolveActivity->SetTheme(R::style::Theme_Holo_Dialog_Alert);
                mResolveActivity->SetExported(TRUE);
                mResolveActivity->SetEnabled(TRUE);
                mResolveInfo->SetActivityInfo(mResolveActivity);
                mResolveInfo->SetPriority(0);
                mResolveInfo->SetPreferredOrder(0);
                mResolveInfo->SetMatch(0);
                String elastosPkgName;
                mElastosApplication->GetPackageName(&elastosPkgName);
                String raName;
                mResolveActivity->GetName(&raName);
                mResolveComponentName = NULL;
                CComponentName::New(elastosPkgName, raName, (IComponentName**)&mResolveComponentName);
            }
        }
    }

    if (DEBUG_PACKAGE_SCANNING) {
        if ((parseFlags & PackageParser::PARSE_CHATTY) != 0)
            Logger::D(TAG, "Scanning package %s", pkg->mPackageName.string());
    }

    if (mPackages.Find(pkg->mPackageName) != mPackages.End()
            || mSharedLibraries.Find(pkg->mPackageName) != mSharedLibraries.End()) {
        Slogger::E(TAG, "Application package %s already installed.  Skipping duplicate."
                , pkg->mPackageName.string());
        mLastScanError = IPackageManager::INSTALL_FAILED_DUPLICATE_PACKAGE;
        return E_PACKAGE_MANAGER_EXCEPTION;
    }

    // Initialize package source and resource directories
    pkg->mApplicationInfo->GetCodePath(&codePath);
    pkg->mApplicationInfo->GetResourcePath(&resPath);
    AutoPtr<IFile> destCodeFile, destResourceFile;
    ASSERT_SUCCEEDED(CFile::New(codePath, (IFile**)&destCodeFile))
    ASSERT_SUCCEEDED(CFile::New(resPath, (IFile**)&destResourceFile))

    AutoPtr<SharedUserSetting> suid;
    AutoPtr<PackageSetting> pkgSetting;

    if (!IsSystemApp(pkg)) {
        // Only system apps can use these features.
        pkg->mOriginalPackages = NULL;
        pkg->mRealPackage = NULL;
        pkg->mAdoptPermissions = NULL;
    }

    // writer
    synchronized (mPackagesLock) {
        if (!pkg->mSharedUserId.IsNull()) {
            suid = mSettings->GetSharedUserLPw(pkg->mSharedUserId, 0, TRUE);
            if (suid == NULL) {
                Slogger::E(TAG, "Creating application package %s for shared user failed", pkg->mPackageName.string());
                mLastScanError = IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
                return E_PACKAGE_MANAGER_EXCEPTION;
            }
            if (DEBUG_PACKAGE_SCANNING) {
                if ((parseFlags & PackageParser::PARSE_CHATTY) != 0)
                    Logger::D(TAG, "Shared UserID %d (uid=%d): packages=", pkg->mSharedUserId, suid->mUserId,/*suid.packages*/);
            }
        }

        // Check if we are renaming from an original package name.
        AutoPtr<PackageSetting> origPackage;
        String realName;
        if (pkg->mOriginalPackages != NULL) {
            // This package may need to be renamed to a previously
            // installed name.  Let's check on that...
            String renamed;
            HashMap<String, String>::Iterator it = mSettings->mRenamedPackages.Find(pkg->mRealPackage);
            if (it != mSettings->mRenamedPackages.End()) {
                renamed = it->mSecond;
            }
            if (!renamed.IsNull() && Find(pkg->mOriginalPackages->Begin(),
                pkg->mOriginalPackages->End(), renamed) != pkg->mOriginalPackages->End()) {
                // This package had originally been installed as the
                // original name, and we have already taken care of
                // transitioning to the new one.  Just update the new
                // one to continue using the old name.
                realName = pkg->mRealPackage;
                if (!pkg->mPackageName.Equals(renamed)) {
                    // Callers into this function may have already taken
                    // care of renaming the package; only do it here if
                    // it is not already done.
                    pkg->SetPackageName(renamed);
                }
            }
            else {
                List<String>::ReverseIterator rit;
                for (rit = pkg->mOriginalPackages->RBegin(); rit != pkg->mOriginalPackages->REnd(); ++rit) {
                    if ((origPackage = mSettings->PeekPackageLPr(*rit)) != NULL) {
                        // We do have the package already installed under its
                        // original name...  should we use it?
                        if (!VerifyPackageUpdateLPr(origPackage, pkg)) {
                            // New package is not compatible with original.
                            origPackage = NULL;
                            continue;
                        }
                        else if (origPackage->mSharedUser != NULL) {
                            // Make sure uid is compatible between packages.
                            if (!origPackage->mSharedUser->mName.Equals(pkg->mSharedUserId)) {
//                                 Slog.w(TAG, "Unable to migrate data from " + origPackage.name
//                                                 + " to " + pkg.packageName + ": old uid "
//                                                 + origPackage.sharedUser.name
//                                                 + " differs from " + pkg.mSharedUserId);
                                origPackage = NULL;
                                continue;
                            }
                        }
                        else {
//                             if (DEBUG_UPGRADE) Log.v(TAG, "Renaming new package "
//                                             + pkg.packageName + " to old name " + origPackage.name);
                        }
                        break;
                    }
                }
            }
        }

        if (mTransferedPackages.Find(pkg->mPackageName) != mTransferedPackages.End()) {
            Slogger::W(TAG, "Package %s was transferred to another, but its .apk remains",
                    pkg->mPackageName.string());
        }

        // Just create the setting, don't add it yet. For already existing packages
        // the PkgSetting exists already and doesn't have to be created.
        String nativeLibraryRootDir, primaryCpuAbi, secondaryCpuAbi;
        pkg->mApplicationInfo->GetNativeLibraryRootDir(&nativeLibraryRootDir);
        pkg->mApplicationInfo->GetPrimaryCpuAbi(&primaryCpuAbi);
        pkg->mApplicationInfo->GetSecondaryCpuAbi(&secondaryCpuAbi);
        Int32 flags;
        pkg->mApplicationInfo->GetFlags(&flags);
        pkgSetting = mSettings->GetPackageLPw(pkg, origPackage, realName, suid, destCodeFile,
                destResourceFile, nativeLibraryRootDir, primaryCpuAbi, secondaryCpuAbi, flags, user, FALSE);
        if (pkgSetting == NULL) {
            Slogger::E(TAG, "Creating application package %s failed", pkg->mPackageName.string();
            mLastScanError = IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
            return E_PACKAGE_MANAGER_EXCEPTION;
        }

        if (pkgSetting->mOrigPackage != NULL) {
            // If we are first transitioning from an original package,
            // fix up the new package's name now.  We need to do this after
            // looking up the package under its new name, so getPackageLP
            // can take care of fiddling things correctly.
            pkg->SetPackageName(origPackage->mName);

            // File a report about this.
            String msg = String("New package ") + pkgSetting->mRealName
                        + " renamed to replace old package " + pkgSetting->mName;
            ReportSettingsProblem(ILogHelper::WARN, msg);

            // Make a note of it.
            mTransferedPackages.Insert(origPackage->mName);

            // No longer need to retain this.
            pkgSetting->mOrigPackage = NULL;
        }

        if (!realName.IsNull()) {
            // Make a note of it.
            mTransferedPackages.Insert(pkg->mPackageName);
        }

        if (mSettings->IsDisabledSystemPackageLPr(pkg->mPackageName)) {
            Int32 flags;
            pkg->mApplicationInfo->GetFlags(&flags);
            flags |= IApplicationInfo::FLAG_UPDATED_SYSTEM_APP;
            pkg->mApplicationInfo->SetFlags(flags);
        }

        if ((parseFlags & PackageParser::PARSE_IS_SYSTEM_DIR) == 0) {
            // Check all shared libraries and map to their actual file path.
            // We only do this here for apps not on a system dir, because those
            // are the only ones that can fail an install due to this.  We
            // will take care of the system apps by updating all of their
            // library paths after the scan is done.
            UpdateSharedLibrariesLPw(pkg, NULL);
        }

        if (mFoundPolicyFile) {
            SELinuxMMAC::AssignSeinfoValue(pkg);
        }

        pkg->mApplicationInfo->SetUid(pkgSetting->mAppId);
        pkg->mExtras = pkgSetting;

        if (!pkgSetting->mKeySetData->IsUsingUpgradeKeySets() || pkgSetting->mSharedUser != NULL) {
            // try {
            ECode ec = VerifySignaturesLP(pkgSetting, pkg)
            if (FAILED(ec)) {
                if ((parseFlags & PackageParser::PARSE_IS_SYSTEM_DIR) == 0) {
                    return ec;
                }
                // The signature has changed, but this package is in the system
                // image...  let's recover!
                pkgSetting->mSignatures->mSignatures = pkg->mSignatures;
                // However...  if this package is part of a shared user, but it
                // doesn't match the signature of the shared user, let's fail.
                // What this means is that you can't change the signatures
                // associated with an overall shared user, which doesn't seem all
                // that unreasonable.
                if (pkgSetting->mSharedUser != NULL) {
                    if (CompareSignatures(pkgSetting->mSharedUser->mSignatures->mSignatures,
                            pkg->mSignatures) != IPackageManager::SIGNATURE_MATCH) {
                        Slogger::E(TAG, "Signature mismatch for shared user : %p", pkgSetting->mSharedUser.Get());
                        mLastScanError = IPackageManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES;
                        return E_PACKAGE_MANAGER_EXCEPTION;
                    }
                }
                // File a report about this.
                String msg = String("System package ") + pkg->mPackageName + " signature changed; retaining data.";
                ReportSettingsProblem(ILogHelper::WARN, msg);
            }
            else {
                // We just determined the app is signed correctly, so bring
                // over the latest parsed certs.
                pkgSetting->mSignatures->mSignatures = pkg->mSignatures;
            }
            // } catch (PackageManagerException e) {
            // }
        }
        else {
            if (!CheckUpgradeKeySetLP(pkgSetting, pkg)) {
                Slogger::E(TAG, "Package %s upgrade keys do not match the previously installed version",
                        pkg->mPackageName.string());
                mLastScanError = IPackageManager::INSTALL_FAILED_UPDATE_INCOMPATIBLE;
                return E_PACKAGE_MANAGER_EXCEPTION;
            }
            else {
                // We just determined the app is signed correctly, so bring
                // over the latest parsed certs.
                pkgSetting->mSignatures->mSignatures = pkg->mSignatures;
            }
        }

        // Verify that this new package doesn't have any content providers
        // that conflict with existing packages.  Only do this if the
        // package isn't already installed, since we don't want to break
        // things that are installed.
        if ((scanFlags & SCAN_NEW_INSTALL) != 0) {
            List< AutoPtr<PackageParser::Provider> >::Iterator it;
            for (it = pkg->mProviders.Begin(); it != pkg->mProviders.End(); ++it) {
                AutoPtr<PackageParser::Provider> p = *it;
                String auth;
                if (p->mInfo->GetAuthority(&auth), !auth.IsNull()) {
                    AutoPtr<ArrayOf<String> > names;
                    StringUtils::Split(auth, String(";"), (ArrayOf<String>**)&names);
                    for (Int32 j = 0; j < names->GetLength(); j++) {
                        String name = (*names)[j];
                        HashMap<String, AutoPtr<PackageParser::Provider> >::Iterator otherIt
                                = mProvidersByAuthority.Find(name);
                        if (otherIt != mProvidersByAuthority.End()) {
                            AutoPtr<PackageParser::Provider> other = otherIt->mSecond;
                            String otherPackageName("?");
                            AutoPtr<IComponentName> cn;
                            if (other != NULL && (other->GetComponentName((IComponentName**)&cn), cn != NULL)) {
                                cn->GetPackageName(&otherPackageName);
                            }
                            String aiPackageName;
                            pkg->mApplicationInfo->GetPackageName(&aiPackageName);
                            Slogger::E(TAG, "Can't install because provider name %s (in package %s) is already used by %s",
                                    name.string(), aiPackageName.string(), otherPackageName.string());
                            mLastScanError = IPackageManager::INSTALL_FAILED_CONFLICTING_PROVIDER;
                            return E_PACKAGE_MANAGER_EXCEPTION;
                        }
                    }
                }
            }
        }

        if (pkg->mAdoptPermissions != NULL) {
            // This package wants to adopt ownership of permissions from
            // another package.
            List<String>::ReverseIterator rit;
            for (rit = pkg->mAdoptPermissions->RBegin(); rit != pkg->mAdoptPermissions->REnd(); ++rit) {
                String origName = *rit;
                AutoPtr<PackageSetting> orig = mSettings->PeekPackageLPr(origName);
                if (orig != NULL) {
                    if (VerifyPackageUpdateLPr(orig, pkg)) {
                        Slogger::I(TAG, "Adopting permissions from %s to %s",
                                origName.string(), pkg->mPackageName.string());
                        mSettings->TransferPermissionsLPw(origName, pkg->mPackageName);
                    }
                }
            }
        }
    }

    String pkgName = pkg->mPackageName;

    Int64 scanFileTime = 0;
    scanFile->LastModified(&scanFileTime);
    Boolean forceDex = (scanFlags & SCAN_FORCE_DEX) != 0;
    String pkgAppPkgName, pkgAppProcName;
    pkg->mApplicationInfo->GetPackageName(&pkgAppPkgName);
    pkg->mApplicationInfo->GetProcessName(&pkgAppProcName);
    Int32 pkgAppUid;
    pkg->mApplicationInfo->GetUid(&pkgAppUid);
    pkgAppProcName = FixProcessName(pkgAppPkgName, pkgAppProcName, pkgAppUid);
    pkg->mApplicationInfo->SetProcessName(pkgAppProcName);

    AutoPtr<IFile> dataPath;
    if (mPlatformPackage.Get() == pkg) {
        // The system package is special.
        AutoPtr<IFile> dir = Environment::GetDataDirectory();
        ASSERT_SUCCEEDED(CFile::New(dir, String("system"), (IFile**)&dataPath));
        String dataDir;
        dataPath->GetPath(&dataDir);
        pkg->mApplicationInfo->SetDataDir(dataDir);
    }
    else {
        // This is a normal package, need to make its data directory.
        dataPath = GetDataPathForPackage(pkg->mPackageName, 0);
        Boolean uidError = FALSE;
        Boolean isExists = FALSE;
        if (dataPath->Exists(&isExists), isExists) {
            Int32 currentUid = 0;
//             try {
            String path;
            dataPath->GetPath(&path);
            AutoPtr<ILibcore> libcore;
            CLibcore::AcquireSingleton((ILibcore**)&libcore);
            AutoPtr<IOs> os;
            libcore->GetOs((IOs**)&os);
            AutoPtr<IStructStat> stat;
            os->Stat(path, (IStructStat**)&stat);
            stat->GetUid(&currentUid);
//             } catch (ErrnoException e) {
//                 Slog.e(TAG, "Couldn't stat path " + dataPath.getPath(), e);
//             }

            // If we have mismatched owners for the data path, we have a problem.
            Int32 pkgAppUid;
            pkg->mApplicationInfo->GetUid(&pkgAppUid);
            if (currentUid != pkgAppUid) {
                Boolean recovered = FALSE;
                if (currentUid == 0) {
                    // The directory somehow became owned by root.  Wow.
                    // This is probably because the system was stopped while
                    // installd was in the middle of messing with its libs
                    // directory.  Ask installd to fix that.
                    Int32 ret = mInstaller->FixUid(pkgName, pkgAppUid, pkgAppUid);
                    if (ret >= 0) {
                        recovered = TRUE;
                        String msg = String("Package ") + pkg->mPackageName + " unexpectedly changed to uid 0; recovered to "
                                + StringUtils::ToString(pkgAppUid);
                        ReportSettingsProblem(ILogHelper::WARN, msg);
                    }
                }
                if (!recovered && ((parseFlags & PackageParser::PARSE_IS_SYSTEM) != 0
                        || (scanFlags & SCAN_BOOTING) != 0)) {
                    // If this is a system app, we can at least delete its
                    // current data so the application will still work.
                    Int32 ret = RemoveDataDirsLI(pkgName);
                    if (ret >= 0) {
                        // TODO: Kill the processes first
                        // Old data gone!
                        String prefix = (parseFlags & PackageParser::PARSE_IS_SYSTEM) != 0
                                ? String("System package ") : String("Third party package ");
                        String msg = prefix + pkg->mPackageName + " has changed from uid: " + StringUtils::ToString(currentUid)
                                + " to " + StringUtils::ToString(pkgAppUid) + "; old data erased";
                        ReportSettingsProblem(ILogHelper::WARN, msg);
                        recovered = TRUE;

                        // And now re-install the app.
                        String seinfo;
                        pkg->mApplicationInfo->GetSeinfo(&seinfo);
                        ret = CreateDataDirsLI(pkgName, pkgAppUid, seinfo);
                        if (ret == -1) {
                            // Ack should not happen!
                            msg = prefix + pkg->mPackageName
                                    + " could not have data directory re-created after delete.";
                            ReportSettingsProblem(ILogHelper::WARN, msg);
                            Slogger::E(TAG, "%s", msg.string());
                            mLastScanError = IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
                            return E_PACKAGE_MANAGER_EXCEPTION;
                        }
                    }
                    if (!recovered) {
                        mHasSystemUidErrors = TRUE;
                    }
                }
                else if (!recovered) {
                    // If we allow this install to proceed, we will be broken.
                    // Abort, abort!
                    Slogger::E(TAG, "scanPackageLI");
                    mLastScanError = IPackageManager::INSTALL_FAILED_UID_CHANGED;
                    return E_PACKAGE_MANAGER_EXCEPTION;
                }
                if (!recovered) {
                    String dataDir = String("/mismatched_uid/settings_") + StringUtils::ToString(pkgAppUid) + "/fs_"
                            + StringUtils::ToString(currentUid);
                    pkg->mApplicationInfo->SetDataDir(dataDir);
                    pkg->mApplicationInfo->SetNativeLibraryDir(dataDir);
                    pkg->mApplicationInfo->SetNativeLibraryRootDir(dataDir);
                    String msg = String("Package ") + pkg->mPackageName + " has mismatched uid: "
                            + StringUtils::ToString(currentUid) + " on disk, "
                            + StringUtils::ToString(pkgAppUid) + " in settings";
                    // writer
                    synchronized (mPackagesLock) {
                        mSettings->mReadMessages->Append(msg);
                        mSettings->mReadMessages->AppendChar('\n');
                        uidError = TRUE;
                        if (!pkgSetting->mUidError) {
                            ReportSettingsProblem(ILogHelper::ERROR, msg);
                        }
                    }
                }
            }
            String dataDir;
            dataPath->GetPath(&dataDir);
            pkg->mApplicationInfo->SetDataDir(dataDir);
            if (mShouldRestoreconData) {
                Slogger::I(TAG, "SELinux relabeling of %s issued.", pkg->mPackageName.string());
                String seinfo;
                pkg->mApplicationInfo->GetSeinfo(&seinfo);
                mInstaller->RestoreconData(pkg->mPackageName, seinfo, pkgAppUid);
            }
        }
        else {
            if (DEBUG_PACKAGE_SCANNING) {
                if ((parseFlags & PackageParser::PARSE_CHATTY) != 0)
                    Logger::V(TAG, "Want this data dir: %p", dataPath.Get());
            }
            //invoke installer to do the actual installation
            Int32 pkgAppUid;
            pkg->mApplicationInfo->GetUid(&pkgAppUid);
            String seinfo;
            pkg->mApplicationInfo->GetSeinfo(&seinfo);
            Int32 ret = CreateDataDirsLI(pkgName, pkgAppUid, seinfo);

            if (ret < 0) {
                // Error from installer
                Slogger::E(TAG, "Unable to create data dirs [errorCode=%d]", ret);
                mLastScanError = IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
                return E_PACKAGE_MANAGER_EXCEPTION;
            }

            Boolean isExist;
            if (dataPath->Exists(&isExist), isExist) {
                String path;
                dataPath->GetPath(&path);
                pkg->mApplicationInfo->SetDataDir(path);
            }
            else {
                Slogger::W(TAG, "Unable to create data directory: %p", dataPath.Get());
                pkg->mApplicationInfo->SetDataDir(String(NULL));
            }
        }

        pkgSetting->mUidError = uidError;
    }

    String path;
    scanFile->GetPath(&path);
    String codePath, cpuAbiOverride;
    pkg->mApplicationInfo->GetCodePath(&codePath);
    DeriveAbiOverride(pkg->mCpuAbiOverride, pkgSetting);
    if (IsSystemApp(pkg) && !IsUpdatedSystemApp(pkg)) {
        // ActionsCode(songzhining, change code: add vendor apk lib support)
        if (IsVendorApp(pkg)) {
            // force link vendor apk lib to /vendor/app/lib/package name/
            Slogger::I(TAG, "Force linking native library dir for %s", path.string());
            String vendorLibDir = VENDOR_APP_DIR + "/lib/" + pkg->mPackageName;
            AutoPtr< ArrayOf<Int32> > userIds = sUserManager->GetUserIds();
            pkg->mApplicationInfo->SetNativeLibraryDir(vendorLibDir);
            synchronized (mInstallLock) {
                for (Int32 i = 0; i < userIds->GetLength(); ++i) {
                    Int32 userId = (*userIds)[i];
                    if (mInstaller->mLinkNativeLibraryDirectory(pk->mPackageName,
                            vendorLibDir, userId) < 0) {
                        Slogger::W(TAG, "Failed linking native library dir (user=%d)", userId);
                        *outPkg = NULL;
                        return NOERROR;
                    }
                }
            }
        }
        else {
            SetBundledAppAbisAndRoots(pkg, pkgSetting);

            // If we haven't found any native libraries for the app, check if it has
            // renderscript code. We'll need to force the app to 32 bit if it has
            // renderscript bitcode.
            String primaryCpuAbi, secondaryCpuAbi;
            if ((pkg->mApplicationInfo->GetPrimaryCpuAbi(&primaryCpuAbi), primaryCpuAbi.IsNull())
                    && (pkg->mApplicationInfo->GetSecondaryCpuAbi(&secondaryCpuAbi), secondaryCpuAbi.IsNull())
                    && Build::SUPPORTED_64_BIT_ABIS->GetLength() >  0) {
                AutoPtr<INativeLibraryHelperHandle> handle;
                // try {
                AutoPtr<INativeLibraryHelperHandleHelper> hh;
                CNativeLibraryHelperHandleHelper::AcquireSingleton((INativeLibraryHelperHandleHelper**)&hh);
                hh->Create(scanFile, (INativeLibraryHelperHandle**)&handle);
                AutoPtr<INativeLibraryHelper> nlh;
                CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&nlh);
                Boolean hasRenderscriptBitcode;
                if (nlh->HasRenderscriptBitcode(handle, &hasRenderscriptBitcode), hasRenderscriptBitcode) {
                    pkg->mApplicationInfo->SetPrimaryCpuAbi((*Build::SUPPORTED_32_BIT_ABIS)[0]);
                }
                // } catch (IOException ioe) {
                //     Slog.w(TAG, "Error scanning system app : " + ioe);
                // } finally {
                //     IoUtils.closeQuietly(handle);
                // }
                AutoPtr<IIoUtils> ioUtils;
                CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
                ioUtils->CloseQuietly(handle)''
            }

            SetNativeLibraryPaths(pkg);
        }
    }
    else {
        // TODO: We can probably be smarter about this stuff. For installed apps,
        // we can calculate this information at install time once and for all. For
        // system apps, we can probably assume that this information doesn't change
        // after the first boot scan. As things stand, we do lots of unnecessary work.

        // Give ourselves some initial paths; we'll come back for another
        // pass once we've determined ABI below.
        SetNativeLibraryPaths(pkg);

        Boolean isAsec = IsForwardLocked(pkg) || IsExternal(pkg);
        String nativeLibraryRootStr;
        pkg->mApplicationInfo->GetNativeLibraryRootDir(&nativeLibraryRootStr);
        Boolean useIsaSpecificSubdirs;
        pkg->mApplicationInfo->GetNativeLibraryRootRequiresIsa();

        AutoPtr<INativeLibraryHelperHandle> handle;
        // try {
        AutoPtr<INativeLibraryHelperHandleHelper> hh;
        CNativeLibraryHelperHandleHelper::AcquireSingleton((INativeLibraryHelperHandleHelper**)&hh);
        hh->Create(scanFile, (INativeLibraryHelperHandle**)&handle);
        // TODO(multiArch): This can be null for apps that didn't go through the
        // usual installation process. We can calculate it again, like we
        // do during install time.
        //
        // TODO(multiArch): Why do we need to rescan ASEC apps again ? It seems totally
        // unnecessary.
        AutoPtr<IFile> nativeLibraryRoot;
        CFile::New(nativeLibraryRootStr, (IFile**)&nativeLibraryRoot);

        // Null out the abis so that they can be recalculated.
        pkg->mPpplicationInfo->SetPrimaryCpuAbi(String(NULL));
        pkg->mPpplicationInfo->SetSecondaryCpuAbi(String(NULL));
        if (IsMultiArch(pkg->mApplicationInfo)) {
            // Warn if we've set an abiOverride for multi-lib packages..
            // By definition, we need to copy both 32 and 64 bit libraries for
            // such packages.
            if (!pkg->mCpuAbiOverride.IsNull()
                    && !INativeLibraryHelper::CLEAR_ABI_OVERRIDE.Equals(pkg->mCpuAbiOverride)) {
                Slogger::W(TAG, "Ignoring abiOverride for multi arch application.");
            }

            Int32 abi32 = IPackageManager::NO_NATIVE_LIBRARIES;
            Int32 abi64 = IPackageManager::NO_NATIVE_LIBRARIES;
            if (Build::SUPPORTED_32_BIT_ABIS->GetLength() > 0) {
                AutoPtr<INativeLibraryHelper> nlh;
                CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&nlh);
                if (isAsec) {
                    nlh->FindSupportedAbi(handle, Build::SUPPORTED_32_BIT_ABIS, &abi32);
                }
                else {
                    nlh->CopyNativeBinariesForSupportedAbi(handle, nativeLibraryRoot,
                            Build::SUPPORTED_32_BIT_ABIS, useIsaSpecificSubdirs, &abi32);
                }
            }

            MaybeThrowExceptionForMultiArchCopy(
                    String("Error unpackaging 32 bit native libs for multiarch app."), abi32);

            if (Build::SUPPORTED_64_BIT_ABIS->GetLength() > 0) {
                AutoPtr<INativeLibraryHelper> nlh;
                CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&nlh);
                if (isAsec) {
                    abi64 = nlh->FindSupportedAbi(handle, Build::SUPPORTED_64_BIT_ABIS, &abi64);
                }
                else {
                    nlh->CopyNativeBinariesForSupportedAbi(handle, nativeLibraryRoot,
                            Build::SUPPORTED_64_BIT_ABIS, useIsaSpecificSubdirs, &abi64);
                }
            }

            MaybeThrowExceptionForMultiArchCopy(
                    String("Error unpackaging 64 bit native libs for multiarch app."), abi64);

            if (abi64 >= 0) {
                pkg->mApplicationInfo->SetPrimaryCpuAbi((*Build::SUPPORTED_64_BIT_ABIS)[abi64]);
            }

            if (abi32 >= 0) {
                String abi = (*Build::SUPPORTED_32_BIT_ABIS)[abi32];
                if (abi64 >= 0) {
                    pkg->mApplicationInfo->SetSecondaryCpuAbi(abi);
                }
                else {
                    pkg->mApplicationInfo->SetPrimaryCpuAbi(abi);
                }
            }
        }
        else {
            AutoPtr< ArrayOf<String> > abiList;
            if (cpuAbiOverride != NULL) {
                abiList = ArrayOf<String>::Alloc(1);
                (*abiList)[0] = cpuAbiOverride;
            }
            else {
                abiList = Build::SUPPORTED_ABIS;
            }

            // Enable gross and lame hacks for apps that are built with old
            // SDK tools. We must scan their APKs for renderscript bitcode and
            // not launch them if it's present. Don't bother checking on devices
            // that don't have 64 bit support.
            AutoPtr<INativeLibraryHelper> nlh;
            CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&nlh);
            Boolean needsRenderScriptOverride = FALSE;
            Boolean hasRenderscriptBitcode;
            if (Build::SUPPORTED_64_BIT_ABIS->GetLength() > 0 && cpuAbiOverride.IsNull() &&
                    (nlh->HasRenderscriptBitcode(handle, &hasRenderscriptBitcode), hasRenderscriptBitcode)) {
                abiList = Build::SUPPORTED_32_BIT_ABIS;
                needsRenderScriptOverride = TRUE;
            }

            Int32 copyRet;
            if (isAsec) {
                nlh->FindSupportedAbi(handle, abiList, &copyRet);
            }
            else {
                nlh->CopyNativeBinariesForSupportedAbi(handle,
                        nativeLibraryRoot, abiList, useIsaSpecificSubdirs, &copyRet);
            }

            if (copyRet < 0 && copyRet != IPackageManager::NO_NATIVE_LIBRARIES) {
                *outPkg = NULL;
                Slogger::E(TAG, "Error unpackaging native libs for app, errorCode=%d", copyRet);
                mLastScanError = IPackageManager::INSTALL_FAILED_INTERNAL_ERROR;
                return E_PACKAGE_MANAGER_EXCEPTION;
            }

            if (copyRet >= 0) {
                pkg->mApplicationInfo->SetPrimaryCpuAbi((*abiList)[copyRet]);
            }
            else if (copyRet == IPackageManager::NO_NATIVE_LIBRARIES && !cpuAbiOverride.IsNull()) {
                pkg->mApplicationInfo->SetPrimaryCpuAbi(cpuAbiOverride);
            }
            else if (needsRenderScriptOverride) {
                pkg->mApplicationInfo->SetPrimaryCpuAbi((*abiList)[0]);
            }
        }
        // } catch (IOException ioe) {
        //     Slog.e(TAG, "Unable to get canonical file " + ioe.toString());
        // } finally {
        //     IoUtils.closeQuietly(handle);
        // }
        AutoPtr<IIoUtils> ioUtils;
        CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
        ioUtils->CloseQuietly(handle);

        // Now that we've calculated the ABIs and determined if it's an internal app,
        // we will go ahead and populate the nativeLibraryPath.
        SetNativeLibraryPaths(pkg);

        if (DEBUG_INSTALL) Slogger::I(TAG, "Linking native library dir for %s", path.string());
        AutoPtr< ArrayOf<Int32> > userIds = sUserManager->GetUserIds();
        synchronized (mInstallLock) {
            // Create a native library symlink only if we have native libraries
            // and if the native libraries are 32 bit libraries. We do not provide
            // this symlink for 64 bit libraries.
            String appPrimaryCpuAbi;
            if ((pkg->mApplicationInfo->GetPrimaryCpuAbi(appPrimaryCpuAbi), !appPrimaryCpuAbi.IsNull())
                    /*&& !VMRuntime.is64BitAbi(pkg->mApplicationInfo->primaryCpuAbi)*/) {
                String nativeLibPath;
                pkg->mApplicationInfo->GetNativeLibraryDir(&nativeLibPath);
                for (Int32 i = 0; i < userIds->GetLength(); ++i) {
                    if (mInstaller->LinkNativeLibraryDirectory(pkg->mPackageName, nativeLibPath, userId) < 0) {
                        Slogger::E(TAG, "Failed linking native library dir (user=%d)", userId);
                        mLastScanError = IPackageManager::INSTALL_FAILED_INTERNAL_ERROR;
                        return E_PACKAGE_MANAGER_EXCEPTION;
                    }
                }
            }
        }
    }

    // This is a special case for the "system" package, where the ABI is
    // dictated by the zygote configuration (and init.rc). We should keep track
    // of this ABI so that we can deal with "normal" applications that run under
    // the same UID correctly.
    if (mPlatformPackage == pkg) {
        assert(0);
        // TODO
        // pkg->mApplicationInfo->primaryCpuAbi = VMRuntime.getRuntime().is64Bit() ?
        //         Build.SUPPORTED_64_BIT_ABIS[0] : Build.SUPPORTED_32_BIT_ABIS[0];
    }

    pkg->mApplicationInfo->GetPrimaryCpuAbi(&pkgSetting->mPrimaryCpuAbiString);
    pkg->mApplicationInfo->GetSecondaryCpuAbi(&pkgSetting->mSecondaryCpuAbiString);
    pkgSetting->mCpuAbiOverrideString = cpuAbiOverride;
    // Copy the derived override back to the parsed package, so that we can
    // update the package settings accordingly.
    pkg->mCpuAbiOverride = cpuAbiOverride;

    if (DEBUG_ABI_SELECTION) {
        String appInfoPkgName, appInfoNativeLibRD;
        pkg->mApplicationInfo->GetPackageName(&appInfoPkgName);
        pkg->mApplicationInfo->GetNativeLibraryRootDir(&appInfoNativeLibRD);
        Boolean nativeLibraryRootRequiresIsa;
        pkg->mApplicationInfo->GetNativeLibraryRootRequiresIsa(&nativeLibraryRootRequiresIsa);
        Slogger::D(TAG, "Resolved nativeLibraryRoot for %s to root=%s, isa=%d", appInfoPkgName.string()
                , appInfoNativeLibRD.string(), nativeLibraryRootRequiresIsa);
    }

    // Push the derived path down into PackageSettings so we know what to
    // clean up at uninstall time.
    pkg->mApplicationInfo->GetNativeLibraryRootDir(&pkgSetting->mLegacyNativeLibraryPathString);

    if (DEBUG_ABI_SELECTION) {
        String appInfoPrimaryCpuAbi, appInfoSecondaryCpuAbi;
        pkg->mApplicationInfo->GetPrimaryCpuAbi(&appInfoPrimaryCpuAbi);
        pkg->mApplicationInfo->GetSecondaryCpuAbi(&appInfoSecondaryCpuAbi);
        Logger::D(TAG, "Abis for package[%s] are primary=%s  secondary=%s", pkg->mPackageName.string(),
                appInfoPrimaryCpuAbi.string(), appInfoSecondaryCpuAbi.string());
    }

    if ((scanFlags & SCAN_BOOTING) == 0 && !pkgSetting->mSharedUser.IsNull()) {
        // We don't do this here during boot because we can do it all
        // at once after scanning all existing packages.
        //
        // We also do this *before* we perform dexopt on this package, so that
        // we can avoid redundant dexopts, and also to make sure we've got the
        // code and package path correct.
        AdjustCpuAbisForSharedUserLPw(pkgSetting->mSharedUser->mPackages,
                pkg, forceDex, (scanFlags & SCAN_DEFER_DEX) != 0);
    }

    if ((scanFlags & SCAN_NO_DEX) == 0) {
        if (PerformDexOptLI(pkg, NULL /* instruction sets */, forceDex,
                (scanFlags & SCAN_DEFER_DEX) != 0, FALSE) == DEX_OPT_FAILED) {
            Slogger::E(TAG, "scanPackageLI")
            *outPkg = NULL;
            mLastScanError = IPackageManager::INSTALL_FAILED_DEXOPT;
            return E_PACKAGE_MANAGER_EXCEPTION;
        }
    }

    if (mFactoryTest &&
            Find(pkg->mRequestedPermissions.Begin(), pkg->mRequestedPermissions.End(),
                    Elastos::Droid::Manifest::permission::FACTORY_TEST)
            != pkg->mRequestedPermissions.End()) {
        Int32 flags;
        pkg->mApplicationInfo->GetFlags(&flags);
        flags |= IApplicationInfo::FLAG_FACTORY_TEST;
        pkg->mApplicationInfo->SetFlags(flags);
    }

    AutoPtr< List<AutoPtr<PackageParser::Package> > > clientLibPkgs;

    // writer
    synchronized (mPackagesLock) {
        Int32 flags;
        if (pkg->mApplicationInfo->GetFlags(&flags), (flags & IApplicationInfo::FLAG_SYSTEM) != 0) {
            // Only system apps can add new shared libraries.
            if (pkg->mLibraryNames != NULL) {
                List<String>::Iterator libNameIt = pkg->mLibraryNames->Begin();
                for (; libNameIt != pkg->mLibraryNames->End(); ++libNameIt) {
                    String name = *libNameIt;
                    Boolean allowed = FALSE;
                    if (IsUpdatedSystemApp(pkg)) {
                        // New library entries can only be added through the
                        // system image.  This is important to get rid of a lot
                        // of nasty edge cases: for example if we allowed a non-
                        // system update of the app to add a library, then uninstalling
                        // the update would make the library go away, and assumptions
                        // we made such as through app install filtering would now
                        // have allowed apps on the device which aren't compatible
                        // with it.  Better to just have the restriction here, be
                        // conservative, and create many fewer cases that can negatively
                        // impact the user experience.
                        AutoPtr<PackageSetting> sysPs = mSettings->GetDisabledSystemPkgLPr(pkg->mPackageName);
                        if (sysPs->mPkg != NULL && sysPs->mPkg->mLibraryNames != NULL) {
                            List<String>::Iterator sysLibNameIt = sysPs->mPkg->mLibraryNames->Begin();
                            for (; sysLibNameIt != sysPs->mPkg->mLibraryNames->End(); ++sysLibNameIt) {
                                if (name.Equals(*sysLibNameIt)) {
                                    allowed = TRUE;
                                    break;
                                }
                            }
                        }
                    }
                    else {
                        allowed = TRUE;
                    }
                    if (allowed) {
                        if (mSharedLibraries.Find(name) == mSharedLibraries.End()) {
                            AutoPtr<SharedLibraryEntry> libEntry = new SharedLibraryEntry(String(NULL), pkg->mPackageName);
                            mSharedLibraries[name] = libEntry;
                        }
                        else if (!name.Equals(pkg->mPackageName)) {
                            Slogger::W(TAG, "Package %s libray %s already exists; skipping",
                                    pkg->mPackageName.string(), name.string());
                        }
                    }
                    else {
                        Slogger::W(TAG, "Package %s declares lib %s that is not declared on system image; skipping",
                                pkg->mPackageName.string(), name.string());
                    }
                }
                if ((scanFlags&SCAN_BOOTING) == 0) {
                    // If we are not booting, we need to update any applications
                    // that are clients of our shared library.  If we are booting,
                    // this will all be done once the scan is complete.
                    clientLibPkgs = UpdateAllSharedLibrariesLPw(pkg);
                }
            }
        }
    }

    // We also need to dexopt any apps that are dependent on this library.  Note that
    // if these fail, we should abort the install since installing the library will
    // result in some apps being broken.
    if (clientLibPkgs != NULL) {
        if (!pkg->mIsEpk && (scanFlags & SCAN_NO_DEX) == 0) {
            List<AutoPtr<PackageParser::Package> >::Iterator pkgIt = clientLibPkgs->Begin();
            for (; pkgIt != clientLibPkgs->End(); ++pkgIt) {
                AutoPtr<PackageParser::Package> clientPkg = *pkgIt;
                if (PerformDexOptLI(clientPkg, NULL /* instruction sets */, forceDex,
                        (scanFlags & SCAN_DEFER_DEX) != 0, FALSE) == DEX_OPT_FAILED) {
                    *outPkg = NULL;
                    Slogger::E(TAG, "scanPackageLI failed to dexopt clientLibPkgs");
                    mLastScanError = IPackageManager::INSTALL_FAILED_DEXOPT;
                    return E_PACKAGE_MANAGER_EXCEPTION;
                }
            }
        }
    }

    if (pkg->mIsEpk) {
        if (MoveEcoFilesLI(pkg) != IPackageManager::INSTALL_SUCCEEDED) {
            mLastScanError = IPackageManager::INSTALL_FAILED_INVALID_APK;
            *outPkg = NULL;
            return NOERROR;
        }
    }

    // Request the ActivityManager to kill the process(only for existing packages)
    // so that we do not end up in a confused state while the user is still using the older
    // version of the application while the new one gets installed.
    if ((scanFlags & SCAN_REPLACING) != 0) {
        String pkgAppPkgName;
        pkg->mApplicationInfo->GetPackageName(&pkgAppPkgName);
        Int32 pkgAppUid;
        pkg->mApplicationInfo->GetUid(&pkgAppUid);
        KillApplication(pkgAppPkgName, pkgAppUid, String("update pkg"));
    }

    // writer
    synchronized (mPackagesLock) {
        // We don't expect installation to fail beyond this point

        // Add the new setting to mSettings
        mSettings->InsertPackageSettingLPw(pkgSetting, pkg);
        // Add the new setting to mPackages
        String pkgAppPkgName;
        pkg->mApplicationInfo->GetPackageName(&pkgAppPkgName);
        mPackages[pkgAppPkgName] = pkg;
        // Make sure we don't accidentally delete its data.
        List<AutoPtr<IPackageCleanItem> >::Iterator pciit = mSettings->mPackagesToBeCleaned.Begin()
        while (pciit != mSettings->mPackagesToBeCleaned.End()) {
            String pciPkgName;
            (*pciit)->GetPackageName(&pciPkgName);
            if (pkgName.Equals(pciPkgName)) {
                pciit = mSettings->mPackagesToBeCleaned.Erase(pciit);
            }
            else ++pciit;
        }

        // Take care of first install / last update times.
        if (currentTime != 0) {
            if (pkgSetting->mFirstInstallTime == 0) {
                pkgSetting->mFirstInstallTime = pkgSetting->mLastUpdateTime = currentTime;
            }
            else if ((scanFlags & SCAN_UPDATE_TIME) != 0) {
                pkgSetting->mLastUpdateTime = currentTime;
            }
        }
        else if (pkgSetting->mFirstInstallTime == 0) {
            // We need *something*.  Take time time stamp of the file.
            pkgSetting->mFirstInstallTime = pkgSetting->mLastUpdateTime = scanFileTime;
        }
        else if ((parseFlags & PackageParser::PARSE_IS_SYSTEM_DIR) != 0) {
            if (scanFileTime != pkgSetting->mTimeStamp) {
                // A package on the system image has changed; consider this
                // to be an update.
                pkgSetting->mLastUpdateTime = scanFileTime;
            }
        }

        // Add the package's KeySets to the global KeySetManagerService
        AutoPtr<KeySetManagerService> ksms = mSettings->mKeySetManagerService;
        // try {
        // Old KeySetData no longer valid.
        ksms->RemoveAppKeySetDataLPw(pkg->mPackageName);
        ksms->AddSigningKeySetToPackageLPw(pkg->mPackageName, pkg->mSigningKeys);
        if (pkg->mKeySetMapping != NULL) {
            AutoPtr<ISet> keySet;
            pkg->mKeySetMapping->GetEntrySet((ISet**)&keySet);
            AutoPtr<IIterator> keySetIt;
            keySet->GetIterator((IIterator**)&keySetIt);
            Boolean hasNext;
            while (keySetIt->HasNext) {
                AutoPtr<IInterface> value;
                keySetIt->GetNext((IInterface**)&value);
                AutoPtr<IMapEntry> me = IMapEntry::Probe(value);
                AutoPtr<IInterface> value;
                if (entry->GetValue((IInterface**)&value), value != NULL) {
                    AutoPtr<IInterface> key;
                    me->GetKey((IInterface**)&key);
                    String keyStr;
                    ICharSequence::Probe(key)->ToString(&keyStr);
                    ksms->AddDefinedKeySetToPackageLPw(pkg->PackageName, IArraySet::Probe(value), keyStr);
                }
            }
            if (pkg->mUpgradeKeySets != NULL) {
                AutoPtr<ISet> upgradeKeySets = ISet::Probe(pkg->mUpgradeKeySets);
                keySetIt = NULL;
                upgradeKeySets->GetIterator((IIterator**)&keySetIt);
                while (keySetIt->HasNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> keyset;
                    keySetIt->GetNext((IInterface**)&keyset);
                    String upgradeAlias;
                    ICharSequence::Probe(keyset)->ToString(&upgradeAlias);
                    ksms->AddUpgradeKeySetToPackageLPw(pkg->mPackageName, upgradeAlias);
                }
            }
        }
        // } catch (NullPointerException e) {
        //     Slog.e(TAG, "Could not add KeySet to " + pkg.packageName, e);
        // } catch (IllegalArgumentException e) {
        //     Slog.e(TAG, "Could not add KeySet to malformed package" + pkg.packageName, e);
        // }

        String pkgAppProcName;
        pkg->mApplicationInfo->GetProcessName(&pkgAppProcName);
        Int32 pkgAppUid;
        pkg->mApplicationInfo->GetUid(&pkgAppUid);
        AutoPtr<StringBuilder> r;
        List< AutoPtr<PackageParser::Provider> >::Iterator ppit;
        for (ppit = pkg->mProviders.Begin(); ppit != pkg->mProviders.End(); ++ppit) {
            AutoPtr<PackageParser::Provider> p = *ppit;
            String pInfoProcName;
            p->mInfo->GetProcessName(&pInfoProcName);
            pInfoProcName = FixProcessName(pkgAppProcName, pInfoProcName, pkgAppUid);
            p->mInfo->SetProcessName(pInfoProcName);
            mProviders->AddProvider(p);
            Boolean pInfoIsSyncable;
            p->mInfo->GetIsSyncable(&pInfoIsSyncable);
            p->mSyncable = pInfoIsSyncable;
            String pInfoAuth;
            if (p->mInfo->GetAuthority(&pInfoAuth), !pInfoAuth.IsNull()) {
                AutoPtr<ArrayOf<String> > names;
                StringUtils::Split(pInfoAuth, String(";"), (ArrayOf<String>**)&names);
                p->mInfo->SetAuthority(String(NULL));
                for (Int32 j = 0; j < names->GetLength(); j++) {
                    String name = (*names)[j];
                    if (j == 1 && p->mSyncable) {
                        // We only want the first authority for a provider to possibly be
                        // syncable, so if we already added this provider using a different
                        // authority clear the syncable flag. We copy the provider before
                        // changing it because the mContentProviders object contains a reference
                        // to a provider that we don't want to change.
                        // Only do this for the second authority since the resulting provider
                        // object can be the same for all future authorities for this provider.
                        p = new PackageParser::Provider(p);
                        p->mSyncable = FALSE;
                    }
                    if (mProvidersByAuthority.Find(name) == mProvidersByAuthority.End()) {
                        mProvidersByAuthority[name] = p;
                        String auth;
                        p->mInfo->GetAuthority(&auth);
                        if (auth.IsNull()) {
                            p->mInfo->SetAuthority(name);
                        }
                        else {
                            p->mInfo->SetAuthority(auth + ";" + name);
                        }
//                         if (DEBUG_PACKAGE_SCANNING) {
//                             if ((parseFlags & PackageParser::PARSE_CHATTY) != 0)
//                                 Log.d(TAG, "Registered content provider: " + names[j]
//                                         + ", className = " + p.info.name + ", isSyncable = "
//                                         + p.info.isSyncable);
//                         }
                    }
                    else {
                        AutoPtr<PackageParser::Provider> other;
                        HashMap<String, AutoPtr<PackageParser::Provider> >::Iterator it
                                = mProvidersByAuthority.Find(name);
                        if (it != mProvidersByAuthority.End()) {
                            other = it->mSecond;
                        }
//                         Slog.w(TAG, "Skipping provider name " + names[j] +
//                                 " (in package " + pkg->mApplicationInfo->packageName +
//                                 "): name already used by "
//                                 + ((other != null && other.getComponentName() != null)
//                                         ? other.getComponentName().getPackageName() : "?"));
                    }
                }
            }
            if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuilder(256);
                }
                else {
                    r->AppendChar(' ');
                }
                String pInfoName;
                p->mInfo->GetName(&pInfoName);
                r->Append(pInfoName);
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Providers: " + r);
//         }

        r = NULL;
        List< AutoPtr<PackageParser::Service> >::Iterator psit;
        for (psit = pkg->mServices.Begin(); psit != pkg->mServices.End(); ++psit) {
            AutoPtr<PackageParser::Service> s = *psit;
            String sInfoProcName;
            s->mInfo->GetProcessName(&sInfoProcName);
            sInfoProcName = FixProcessName(pkgAppProcName, sInfoProcName, pkgAppUid);
            s->mInfo->SetProcessName(sInfoProcName);
            mServices->AddService(s);
            if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuilder(256);
                }
                else {
                    r->AppendChar(' ');
                }
                String sInfoName;
                s->mInfo->GetName(&sInfoName);
                r->Append(sInfoName);
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Services: " + r);
//         }

        r = NULL;
        List< AutoPtr<PackageParser::Activity> >::Iterator pait;
        for (pait = pkg->mReceivers.Begin(); pait != pkg->mReceivers.End(); ++pait) {
            AutoPtr<PackageParser::Activity> a = *pait;
            String aInfoProcName;
            a->mInfo->GetProcessName(&aInfoProcName);
            aInfoProcName = FixProcessName(pkgAppProcName, aInfoProcName, pkgAppUid);
            a->mInfo->SetProcessName(aInfoProcName);
            mReceivers->AddActivity(a, String("receiver"));
            if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuilder(256);
                }
                else {
                    r->AppendChar(' ');
                }
                String aInfoName;
                a->mInfo->GetName(&aInfoName);
                r->Append(aInfoName);
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Receivers: " + r);
//         }

        r = NULL;

        /**
         * actions_code(ywwang, start)
         * hook package info
         *
         */
        // com.actions.hooker.HookPackageInfoCaller.callHookActivityInfo(pkg);
        for (pait = pkg->mActivities.Begin(); pait != pkg->mActivities.End(); ++pait) {
            AutoPtr<PackageParser::Activity> a = *pait;
            String aInfoProcName;
            a->mInfo->GetProcessName(&aInfoProcName);
            aInfoProcName = FixProcessName(pkgAppProcName, aInfoProcName, pkgAppUid);
            a->mInfo->SetProcessName(aInfoProcName);
            mActivities->AddActivity(a, String("activity"));
            if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuilder(256);
                }
                else {
                    r->AppendChar(' ');
                }
                String aInfoName;
                a->mInfo->GetName(&aInfoName);
                r->Append(aInfoName);
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Activities: " + r);
//         }

        r = NULL;
        List< AutoPtr<PackageParser::PermissionGroup> >::Iterator ppgit;
        for (ppgit = pkg->mPermissionGroups.Begin(); ppgit != pkg->mPermissionGroups.End(); ++ppgit) {
            AutoPtr<PackageParser::PermissionGroup> pg = *ppgit;
            String name;
            pg->mInfo->GetName(&name);
            AutoPtr<PackageParser::PermissionGroup> cur = mPermissionGroups[name];
            if (cur == NULL) {
                mPermissionGroups[name] = pg;
                if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                    if (r == NULL) {
                        r = new StringBuilder(256);
                    }
                    else {
                        r->AppendChar(' ');
                    }
                    String pgInfoName;
                    pg->mInfo->GetName(&pgInfoName);
                    r->Append(pgInfoName);
                }
            }
            else {
//                 Slog.w(TAG, "Permission group " + pg.info.name + " from package "
//                         + pg.info.packageName + " ignored: original from "
//                         + cur.info.packageName);
                if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                    if (r == NULL) {
                        r = new StringBuilder(256);
                    }
                    else {
                        r->AppendChar(' ');
                    }
                    r->Append("DUP:");
                    String pgInfoName;
                    pg->mInfo->GetName(&pgInfoName);
                    r->Append(pgInfoName);
                }
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Permission Groups: " + r);
//         }

        r = NULL;
        List< AutoPtr<PackageParser::Permission> >::Iterator ppmit;
        for (ppmit = pkg->mPermissions.Begin(); ppmit != pkg->mPermissions.End(); ++ppmit) {
            AutoPtr<PackageParser::Permission> p = *ppmit;
            String pInfoName, pInfoPkgName, pInfoGroup;
            p->mInfo->GetName(&pInfoName);
            p->mInfo->GetPackageName(&pInfoPkgName);
            p->mInfo->GetGroup(&pInfoGroup);
            HashMap<String, AutoPtr<BasePermission> >& permissionMap = p->mTree
                    ? mSettings->mPermissionTrees : mSettings->mPermissions;
            p->mGroup = NULL;
            if (!pInfoGroup.IsNull()) {
                HashMap<String, AutoPtr<PackageParser::PermissionGroup> >::Iterator it =
                    mPermissionGroups.Find(pInfoGroup);
                if (it != mPermissionGroups.End()) {
                    p->mGroup = it->mSecond;
                }
            }
            if (pInfoGroup.IsNull() || p->mGroup != NULL) {
                AutoPtr<BasePermission> bp = permissionMap[pInfoName];

                // Allow system apps to redefine non-system permissions
                if (bp != NULL && !Objects::Equals(bp->mSourcePackage, pInfoPkgName)) {
                    Boolean currentOwnerIsSystem = (bp->mPerm != NULL && IsSystemApp(bp->mPerm->mOwner));
                    if (IsSystemApp(p->mOwner)) {
                        if (bp->mType == BasePermission::TYPE_BUILTIN && bp->mPerm == NULL) {
                            // It's a built-in permission and no owner, take ownership now
                            bp->mPackageSetting = pkgSetting;
                            bp->mPerm = p;
                            pkg->mApplicationInfo->GetUid(&bp->mUid);
                            p->mInfo->GetPackageName(&bp->mSourcePackage);
                        }
                        else if (!currentOwnerIsSystem) {
                            String ownerstr;
                            p->mOwner->ToString(&ownerstr);
                            String msg = String("New decl ") + ownerstr + " of permission  "
                                    + pInfoName + " is system; overriding " + bp->mSourcePackage;
                            ReportSettingsProblem(ILogHelper::WARN, msg);
                            bp = NULL;
                        }
                    }
                }

                if (bp == NULL) {
                    bp = new BasePermission(pInfoName, pInfoPkgName, BasePermission::TYPE_NORMAL);
                    permissionMap[pInfoName] = bp;
                }
                if (bp->mPerm == NULL) {
                    if (bp->mSourcePackage.IsNull()
                        || bp->mSourcePackage.Equals(pInfoPkgName)) {
                        AutoPtr<BasePermission> tree = FindPermissionTreeLP(pInfoName);
                        if (tree == NULL
                            || tree->mSourcePackage.Equals(pInfoPkgName)) {
                            bp->mPackageSetting = pkgSetting;
                            bp->mPerm = p;
                            pkg->mApplicationInfo->GetUid(&bp->mUid);
                            p->mInfo->GetPackageName(&bp->mSourcePackage);
                            if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                                if (r == NULL) {
                                    r = new StringBuilder;
                                }
                                else {
                                    r->AppendChar(' ');
                                }
                                r->AppendString(pInfoName);
                            }
                        }
                        else {
//                             Slog.w(TAG, "Permission " + p.info.name + " from package "
//                                     + p.info.packageName + " ignored: base tree "
//                                     + tree.name + " is from package "
//                                     + tree.sourcePackage);
                        }
                    }
                    else {
//                         Slog.w(TAG, "Permission " + p.info.name + " from package "
//                                 + p.info.packageName + " ignored: original from "
//                                 + bp.sourcePackage);
                    }
                }
                else if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                    if (r == NULL) {
                        r = new StringBuilder;
                    }
                    else {
                        r->AppendChar(' ');
                    }
                    r->AppendCStr("DUP:");
                    r->AppendString(pInfoName);
                }
                if (bp->mPerm == p) {
                    p->mInfo->GetProtectionLevel(&bp->mProtectionLevel);
                }
            }
            else {
//                 Slog.w(TAG, "Permission " + p.info.name + " from package "
//                             + p.info.packageName + " ignored: no group "
//                             + p.group);
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Permissions: " + r);
//         }

        String pkgAppSrcDir, pkgAppPubSrcDir, pkgAppDataDir, pkgAppNatLibDir;
        pkg->mApplicationInfo->GetSourceDir(&pkgAppSrcDir);
        pkg->mApplicationInfo->GetPublicSourceDir(&pkgAppPubSrcDir);
        pkg->mApplicationInfo->GetDataDir(&pkgAppDataDir);
        pkg->mApplicationInfo->GetNativeLibraryDir(&pkgAppNatLibDir);
        AutoPtr<ArrayOf<String> > splitSourceDirs, splitPublicSourceDirs;
        pkg->mApplicationInfo->GetSplitSourceDirs((ArrayOf<String>**)&splitSourceDirs);
        pkg->mApplicationInfo->GetSplitPublicSourceDirs((ArrayOf<String>**)&splitPublicSourceDirs);
        r = NULL;
        List< AutoPtr<PackageParser::Instrumentation> >::Iterator piit;
        for (piit = pkg->mInstrumentation.Begin(); piit != pkg->mInstrumentation.End(); ++piit) {
            AutoPtr<PackageParser::Instrumentation> a = *piit;
            a->mInfo->SetPackageName(pkgAppPkgName);
            a->mInfo->SetSourceDir(pkgAppSrcDir);
            a->mInfo->SetPublicSourceDir(pkgAppPubSrcDir);
            a->mInfo->SetSplitSourceDirs(splitSourceDirs);
            a->mInfo->SetSplitPublicSourceDirs(splitPublicSourceDirs);
            a->mInfo->SetDataDir(pkgAppDataDir);

            // TODO: Update instrumentation.nativeLibraryDir as well ? Does it
            // need other information about the application, like the ABI and what not ?
            a->mInfo->SetNativeLibraryDir(pkgAppNatLibDir);
            AutoPtr<IComponentName> comp = a->GetComponentName();
            mInstrumentation[comp] = a;
            if ((parseFlags & PackageParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuilder;
                }
                else {
                    r->AppendChar(' ');
                }
                String aInfoName;
                a->mInfo->GetName(&aInfoName);
                r->AppendString(aInfoName);
            }
        }
//         if (r != NULL) {
//             if (DEBUG_PACKAGE_SCANNING) Log.d(TAG, "  Instrumentation: " + r);
//         }

        if (pkg->mProtectedBroadcasts != NULL) {
            List<String>::Iterator it;
            for (it = pkg->mProtectedBroadcasts->Begin(); it != pkg->mProtectedBroadcasts->End(); ++it) {
                mProtectedBroadcasts.Insert(*it);
            }
        }

        pkgSetting->SetTimeStamp(scanFileTime);

        // Create idmap files for pairs of (packages, overlay packages).
        // Note: "android", ie framework-res.apk, is handled by native layers.
        if (!pkg->mOverlayTarget.IsNull()) {
            // This is an overlay package.
            if (!pkg->mOverlayTarget.IsNull() && !pkg->mOverlayTarget.Equals("android")) {
                if (mOverlays.Find(pkg->mOverlayTarget) == mOverlays.End()) {
                    AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > pps
                            = new HashMap<String, AutoPtr<PackageParser::Package> >();
                    mOverlays[pkg->mOverlayTarget] = pps;
                }
                AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > map;
                HashMap<String, AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > >::Iterator overlayIt
                        = mOverlays.Find(pkg->mOverlayTarget);
                if (overlayIt != mOverlays.End()) {
                    map = overlayIt->mSecond;
                }
                (*map)[pkg->mPackageName] = pkg;
                AutoPtr<PackageParser::Package> orig;
                HashMap<String, AutoPtr<PackageParser::Package> >::Iterator pkgIt = mPackages.Find(pkg->mOverlayTarget);
                if (pkgIt != mPackages.End()) {
                    orig = pkgIt->mSecond;
                }
                if (orig != NULL && !CreateIdmapForPackagePairLI(orig, pkg)) {
                    Slogger::E(TAG, "scanPackageLI failed to createIdmap");
                    *outPkg = NULL;
                    mLastScanError = IPackageManager::INSTALL_FAILED_UPDATE_INCOMPATIBLE;
                    return E_PACKAGE_MANAGER_EXCEPTION;
                }
            }
        }
        else if (mOverlays.Find(pkg->mPackageName) != mOverlays.End() &&
                !pkg->mPackageName.Equals("android")) {
            // This is a regular package, with one or more known overlay packages.
            CreateIdmapsForPackageLI(pkg);
        }
    }

    *outPkg = pkg;
    REFCOUNT_ADD(*outPkg)
    return NOERROR;
}

void CPackageManagerService::AdjustCpuAbisForSharedUserLPw(
    /* [in] */ HashSet<AutoPtr<PackageSetting> >& packagesForUser,
    /* [in] */ PackageParser::Package* scannedPackage,
    /* [in] */ Boolean forceDexOpt,
    /* [in] */ Boolean deferDexOpt)
{
    String requiredInstructionSet(NULL);
    if (scannedPackage != NULL &&
            (scannedPackage->mApplicationInfo->GetPrimaryCpuAbi(&primaryCpuAbi), !primaryCpuAbi.IsNull())) {
        assert(0);
        // requiredInstructionSet = VMRuntime.getInstructionSet(
        //          scannedPackage.applicationInfo.primaryCpuAbi);
    }

    AutoPtr<PackageSetting> requirer;
    HashSet<AutoPtr<PackageSetting> >::Iterator pkgForUserIt = packagesForUser.Begin();
    for (; pkgForUserIt != packagesForUser.End(); ++pkgForUserIt) {
        AutoPtr<PackageSetting> ps = *pkgForUserIt;
        // If packagesForUser contains scannedPackage, we skip it. This will happen
        // when scannedPackage is an update of an existing package. Without this check,
        // we will never be able to change the ABI of any package belonging to a shared
        // user, even if it's compatible with other packages.
        if (scannedPackage == NULL || !scannedPackage->mPackageName.Equals(ps->mName)) {
            if (ps->mPrimaryCpuAbiString.IsNull()) {
                continue;
            }
            // TODO
            String instructionSet;/* = VMRuntime.getInstructionSet(ps.primaryCpuAbiString);*/
            if (!requiredInstructionSet.IsNull() && !instructionSet.Equals(requiredInstructionSet)) {
                // We have a mismatch between instruction sets (say arm vs arm64) warn about
                // this but there's not much we can do.
                String str;
                if (requirer) {
                    requirer->ToString(&str);
                }
                else str("[caller]");
                String psStr;
                ps->ToString(&psStr);
                String errorMessage = String("Instruction set mismatch, ")
                        + str + " requires " + requiredInstructionSet + " whereas " + psStr
                        + " requires " + instructionSet;
                Slogger::W(TAG, "%s", errorMessage.string());
            }

            if (requiredInstructionSet.IsNull()) {
                requiredInstructionSet = instructionSet;
                requirer = ps;
            }
        }
    }

    if (!requiredInstructionSet.IsNull()) {
        String adjustedAbi;
        if (requirer != NULL) {
            // requirer != null implies that either scannedPackage was null or that scannedPackage
            // did not require an ABI, in which case we have to adjust scannedPackage to match
            // the ABI of the set (which is the same as requirer's ABI)
            adjustedAbi = requirer->mPrimaryCpuAbiString;
            if (scannedPackage != NULL) {
                scannedPackage->mApplicationInfo->SetPrimaryCpuAbi(adjustedAbi);
            }
        }
        else {
            // requirer == null implies that we're updating all ABIs in the set to
            // match scannedPackage.
            scannedPackage->mApplicationInfo->GetPrimaryCpuAbi(&adjustedAbi);
        }

        pkgForUserIt = packagesForUser.Begin();
        for (; pkgForUserIt != packagesForUser.End(); ++pkgForUserIt) {
            AutoPtr<PackageSetting> ps = *pkgForUserIt;
            if (scannedPackage == NULL || !scannedPackage->mPackageName.Equals(ps->mName)) {
                if (!ps->mPrimaryCpuAbiString.IsNull()) {
                    continue;
                }

                ps->mPrimaryCpuAbiString = adjustedAbi;
                if (ps->mPkg != NULL && ps->mPkg->mApplicationInfo != NULL) {
                    ps->mPkg->mApplicationInfo->SetPrimaryCpuAbi(adjustedAbi);
                    Slogger::I(TAG, "Adjusting ABI for : %s to %s", ps->mName.string(), adjustedAbi.string());

                    if (PerformDexOptLI(ps->mPkg, NULL /* instruction sets */, forceDexOpt,
                            deferDexOpt, TRUE) == DEX_OPT_FAILED) {
                        ps->mPrimaryCpuAbiString = NULL;
                        ps->mPkg->mApplicationInfo->SetPrimaryCpuAbi(String(NULL));
                        return;
                    }
                    else {
                        mInstaller->Rmdex(ps->mCodePathString,
                                GetDexCodeInstructionSet(GetPreferredInstructionSet()));
                    }
                }
            }
        }
    }
}

void CPackageManagerService::SetUpCustomResolverActivity(
    /* [in] */ PackageParser::Package* pkg)
{
    synchronized (mPackagesLock) {
        mResolverReplaced = TRUE;
        // Set up information for custom user intent resolution activity.
        mResolveActivity->SetApplicationInfo(pkg->mApplicationInfo);
        String className;
        mCustomResolverComponentName->GetClassName(&className);
        mResolveActivity->SetName(className);
        String packageName;
        pkg->mApplicationInfo->GetPackageName(&packageName);
        mResolveActivity->SetPackageName(packageName);
        mResolveActivity->SetProcessName(String(NULL));
        mResolveActivity->SetLaunchMode(IActivityInfo::LAUNCH_MULTIPLE);
        mResolveActivity->SetFlags(IActivityInfo::FLAG_EXCLUDE_FROM_RECENTS |
                IActivityInfo::FLAG_FINISH_ON_CLOSE_SYSTEM_DIALOGS);
        mResolveActivity->SetTheme(0);
        mResolveActivity->SetExported(TRUE);
        mResolveActivity->SetEnabled(TRUE);
        mResolveInfo->SetActivityInfo(mResolveActivity);
        mResolveInfo->SetPriority(0);
        mResolveInfo->SetPreferredOrder(0);
        mResolveInfo->SetMatch(0);
        mResolveComponentName = mCustomResolverComponentName;
        Slogger::I(TAG, "Replacing default ResolverActivity with custom activity: %p", mResolveComponentName.Get());
    }
}

String CPackageManagerService::CalculateBundledApkRoot(
    /* [in] */ const String& codePathString)
{
    AutoPtr<IFile> codePath;
    CFile::New(codePathString, (IFile**)&codePath);
    AutoPtr<IFile> codeRoot;
    AutoPtr<IFile> rootDir, oemDir, vendorDir;
    if (rootDir = Environment::GetRootDirectory(), FileUtils::Contains(rootDir, codePath)) {
        codeRoot = rootDir;
    }
    else if (oemDir= Environment::GetOemDirectory(), FileUtils::Contains(oemDir, codePath)) {
        codeRoot = oemDir;
    }
    else if (vendorDir = Environment::GetVendorDirectory(), FileUtils::Contains(vendorDir, codePath)) {
        codeRoot = vendorDir;
    }
    else {
        // Unrecognized code path; take its top real segment as the apk root:
        // e.g. /something/app/blah.apk => /something
        // try {
        AutoPtr<IFile> f;
        codePath->GetCanonicalFile((IFile**)&f);
        AutoPtr<IFile> parent;
        f->GetParentFile((IFile**)&parent);    // non-null because codePath is a file
        AutoPtr<IFile> tmp;
        while (parent->GetParentFile((IFile**)&tmp), tmp != NULL) {
            f = parent;
            parent = tmp;
        }
        codeRoot = f;
        Slogger::W(TAG, "Unrecognized code path %p - using %p", codePath.Get(), codeRoot.Get());
        // } catch (IOException e) {
        //     // Can't canonicalize the code path -- shenanigans?
        //     Slog.w(TAG, "Can't canonicalize code path " + codePath);
        //     return Environment.getRootDirectory().getPath();
        // }
    }
    String str;
    codeRoot->GetPath(&str);
    return str;
}

void CPackageManagerService::SetNativeLibraryPaths(
    /* [in] */ PackageParser::Package* pkg)
{
    AutoPtr<IApplicationInfo> info = pkg->mApplicationInfo;
    String codePath = pkg->mCodePath;
    AutoPtr<IFile> codeFile;
    CFile::New(codePath, (IFile**)&codeFile);
    Boolean bundledApp = IsSystemApp(info) && !IsUpdatedSystemApp(info);
    Boolean asecApp = IsForwardLocked(info) || IsExternal(info);

    info->SetNativeLibraryRootDir(String(NULL));
    info->SetNativeLibraryRootRequiresIsa(FALSE);
    info->SetNativeLibraryDir(String(NULL));
    info->SetSecondaryNativeLibraryDir(String(NULL));

    if (IsApkFile(codeFile)) {
        // Monolithic install
        if (bundledApp) {
            // If "/system/lib64/apkname" exists, assume that is the per-package
            // native library directory to use; otherwise use "/system/lib/apkname".
            String apkRoot = CalculateBundledApkRoot(info.sourceDir);
            // TODO
            Boolean is64Bit = FALSE;/* = VMRuntime.is64BitInstructionSet(
                    getPrimaryInstructionSet(info));*/

            // This is a bundled system app so choose the path based on the ABI.
            // if it's a 64 bit abi, use lib64 otherwise use lib32. Note that this
            // is just the default path.
            String apkName = DeriveCodePathName(codePath);
            String libDir = is64Bit ? LIB64_DIR_NAME : LIB_DIR_NAME;
            AutoPtr<IFile> apkRootF;
            CFile::New(apkRoot, (IFile**)&apkRootF);
            AutoPtr<ArrayOf<String> > attrs = ArrayOf<String>::Alloc(2);
            (*attrs)[0] = libDir;
            (*attrs)[1] = apkName;
            AutoPtr<IFile> path;
            path = Environment::BuildPath(apkRootF, attrs);
            String absPath;
            path->GetAbsolutePath(&absPath);
            info->SetNativeLibraryRootDir(absPath);

            String secondaryCpuAbi;
            if (info->GetSecondaryCpuAbi(&secondaryCpuAbi) !secondaryCpuAbi.IsNull()) {
                String secondaryLibDir = is64Bit ? LIB_DIR_NAME : LIB64_DIR_NAME;
                attrs = ArrayOf<String>::Alloc(2);
                (*attrs)[0] = secondaryLibDir;
                (*attrs)[1] = apkName;
                path = Environment::BuildPath(apkRootF, attrs);
                path->GetAbsolutePath(&absPath);
                info->SetSecondaryNativeLibraryDir(absPath);
            }
        }
        else if (asecApp) {
            AutoPtr<IFile> parentF;
            codeFile->GetParentFile((IFile**)&parentF);
            AutoPtr<IFile> f;
            CFile::Mew(parentF, LIB_DIR_NAME, (IFile**)&f);
            string absolutePath;
            f->GetAbsolutePath(&absolutePath);
            info->SetNativeLibraryRootDir(absolutePath);
        }
        else {
            String apkName = DeriveCodePathName(codePath);
            AutoPtr<IFile> f;
            CFile::New(mAppLib32InstallDir, apkName, (IFile**)&f);
            String absolutePath;
            f->GetAbsolutePath(&absolutePath);
            info->SetNativeLibraryRootDir(absolutePath);
        }

        info->SetNativeLibraryRootRequiresIsa(FALSE);
        String rootDir;
        info->GetNativeLibraryRootDir(&rootDir);
        info->SetNativeLibraryDir(rootDir);
    }
    else {
        // Cluster install
        String apkName = DeriveCodePathName(codePath);
        AutoPtr<IFile> f;
        CFile::New(codeFile, LIB_DIR_NAME, (IFile**)&f);
        String absolutePath;
        f->GetAbsolutePath(&absolutePath);
        info->SetNativeLibraryRootDir(absolutePath);
        info->SetNativeLibraryRootRequiresIsa(TRUE);

        f = NULL;
        CFile::New(absolutePath, GetPrimaryInstructionSet(info), (IFile**)&f);
        f->GetAbsolutePath(&absolutePath);
        info->SetNativeLibraryDir(absolutePath);

        String secondaryCpuAbi;
        if (info->GetSecondaryCpuAbi, !secondaryCpuAbi.IsNull()) {
            assert(0);
            // info.secondaryNativeLibraryDir = new File(info.nativeLibraryRootDir,
            //         VMRuntime.getInstructionSet(info.secondaryCpuAbi)).getAbsolutePath();
        }
    }
}

void CPackageManagerService::SetBundledAppAbisAndRoots(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ PackageSetting* pkgSetting)
{
    String codePath;
    pkg->mApplicationInfo->GetCodePath(&codePath);
    String apkName = DeriveCodePathName(codePath);

    // If "/system/lib64/apkname" exists, assume that is the per-package
    // native library directory to use; otherwise use "/system/lib/apkname".
    String sourceDir;
    pkg->mApplicationInfo->GetSourceDir(&sourceDir);
    String apkRoot = CalculateBundledApkRoot(sourceDir);
    SetBundledAppAbi(pkg, apkRoot, apkName);
    // pkgSetting might be null during rescan following uninstall of updates
    // to a bundled app, so accommodate that possibility.  The settings in
    // that case will be established later from the parsed package.
    //
    // If the settings aren't null, sync them up with what we've just derived.
    // note that apkRoot isn't stored in the package settings.
    if (pkgSetting != NULL) {
        pkg->mApplicationInfo->GetPrimaryCpuAbi(&pkgSetting->mPrimaryCpuAbiString);
        pkg->mApplicationInfo->GetSecondaryCpuAbi(&pkgSetting->mSecondaryCpuAbiString);
    }
}

void CPackageManagerService::SetBundledAppAbi(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ const String& apkRoot,
    /* [in] */ const String& apkName)
{
    AutoPtr<IFile> codeFile;
    CFile::New(pkg->mCodePath, (IFile**)&codeFile);

    Boolean has64BitLibs;
    Boolean has32BitLibs;
    if (IsApkFile(codeFile)) {
        // Monolithic install
        AutoPtr<IFile> lib64F;
        CFile::New(LIB64_DIR_NAME, apkName, (IFile**)&lib64F);
        String lib64Path;
        lib64F->GetPath(&lib64Path);
        AutoPtr<IFile> apkRootF;
        CFile::New(apkRoot, lib64Path, (IFile**)&apkRootF);
        apkRootF->Exists(&has64BitLibs);
        AutoPtr<IFile> libF;
        CFile::New(LIB_DIR_NAME, apkName, (IFile**)&libF);
        String libPath;
        libF->GetPath(&libPath);
        AutoPtr<IFile> apkRootF1;
        CFile::New(apkRoot, libPath, (IFile**)&apkRootF1);
        apkRootF1->Exists(&has32BitLibs);
    }
    else {
        // Cluster install
        AutoPtr<IFile> rootDir;
        CFile::New(codeFile, LIB_DIR_NAME, (IFile**)&rootDir);
        if (!ArrayUtils::IsEmpty(Build::SUPPORTED_64_BIT_ABIS)
                && !TextUtils::IsEmpty((*Build::SUPPORTED_64_BIT_ABIS)[0])) {
            // TODO
            String isa = NULL;/*VMRuntime.getInstructionSet(Build.SUPPORTED_64_BIT_ABIS[0]);*/
            AutoPtr<IFile> rootDirF;
            CFile::New(rootDir, isa, (IFile**)&rootDirF);
            rootDirF->Exists(&has64BitLibs);
        }
        else {
            has64BitLibs = FALSE;
        }
        if (!ArrayUtils::IsEmpty(Build::SUPPORTED_32_BIT_ABIS)
                && !TextUtils::IsEmpty((*Build::SUPPORTED_32_BIT_ABIS)[0])) {
            // TODO
            String isa = NULL;/*VMRuntime.getInstructionSet(Build.SUPPORTED_32_BIT_ABIS[0]);*/
            AutoPtr<IFile> rootDirF;
            CFile::New(rootDir, isa, (IFile**)&rootDirF);
            rootDirF->Exists(&has32BitLibs);
        }
        else {
            has32BitLibs = FALSE;
        }
    }

    if (has64BitLibs && !has32BitLibs) {
        // The package has 64 bit libs, but not 32 bit libs. Its primary
        // ABI should be 64 bit. We can safely assume here that the bundled
        // native libraries correspond to the most preferred ABI in the list.

        pkg->mApplicationInfo->SetPrimaryCpuAbi((*Build::SUPPORTED_64_BIT_ABIS)[0]);
        pkg->mApplicationInfo->SetSecondaryCpuAbi(String(NULL));
    }
    else if (has32BitLibs && !has64BitLibs) {
        // The package has 32 bit libs but not 64 bit libs. Its primary
        // ABI should be 32 bit.

        pkg->mApplicationInfo->SetPrimaryCpuAbi((*Build::SUPPORTED_32_BIT_ABIS)[0]);
        pkg->mApplicationInfo->SetSecondaryCpuAbi(String(NULL));
    }
    else if (has32BitLibs && has64BitLibs) {
        // The application has both 64 and 32 bit bundled libraries. We check
        // here that the app declares multiArch support, and warn if it doesn't.
        //
        // We will be lenient here and record both ABIs. The primary will be the
        // ABI that's higher on the list, i.e, a device that's configured to prefer
        // 64 bit apps will see a 64 bit primary ABI,

        Int32 flags
        if ((pkg->mApplicationInfo->GetFlags(&flags), (flags & IApplicationInfo::FLAG_MULTIARCH) == 0) {
            Slogger::E(TAG, "Package: %p has multiple bundled libs, but is not multiarch.", pkg);
        }
        // TODO
        // if (VMRuntime.is64BitInstructionSet(GetPreferredInstructionSet())) {
        //     pkg->mApplicationInfo->SetPrimaryCpuAbi((*Build::SUPPORTED_64_BIT_ABIS)[0]);
        //     pkg->mApplicationInfo->SetSecondaryCpuAbi((*Build::SUPPORTED_32_BIT_ABIS)[0]);
        // }
        // else {
        //     pkg->mApplicationInfo->SetPrimaryCpuAbi((*Build::SUPPORTED_32_BIT_ABIS)[0]);
        //     pkg->mApplicationInfo->SetSecondaryCpuAbi((*Build::SUPPORTED_64_BIT_ABIS)[0]);
        // }
    }
    else {
        pkg->mApplicationInfo->SetPrimaryCpuAbi(String(NULL));
        pkg->mApplicationInfo->SetSecondaryCpuAbi(String(NULL));
    }
}

void CPackageManagerService::KillApplication(
    /* [in] */ const String& pkgName,
    /* [in] */ Int32 appId,
    /* [in] */ const String& reason)
{
    // Request the ActivityManager to kill the process(only for existing packages)
    // so that we do not end up in a confused state while the user is still using the older
    // version of the application while the new one gets installed.
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am != NULL) {
        am->KillApplicationWithAppId(pkgName, appId, reason);
    }
}

void CPackageManagerService::RemovePackageLI(
    /* [in] */ PackageSetting* ps,
    /* [in] */ Boolean chatty)
{
    if (DEBUG_INSTALL) {
        if (chatty)
            Logger::D(TAG, "Removing package %s", ps->mName.string());
    }

    // writer
    synchronized (mPackagesLock) {
        mPackages.Erase(ps->mName);
        AutoPtr<PackageParser::Package> pkg = ps->mPkg;
        if (pkg != NULL) {
            CleanPackageDataStructuresLILPw(pkg, chatty);
        }
    }
}

void CPackageManagerService::RemoveInstalledPackageLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Boolean chatty)
{
    if (DEBUG_INSTALL) {
        if (chatty)
            Log.d(TAG, "Removing package " + pkg->mApplicationInfo->packageName);
    }

    // writer
    synchronized (mPackagesLock) {
        String pkgName;
        pkg->mApplicationInfo->GetPackageName(&pkgName);
        mPackages.Erase(pkgName);
        CleanPackageDataStructuresLILPw(pkg, chatty);
    }
}

void CPackageManagerService::CleanPackageDataStructuresLILPw(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Boolean chatty)
{
    AutoPtr<StringBuilder> r;
    List< AutoPtr<PackageParser::Provider> >::Iterator it;
    for (it = pkg->mProviders.Begin(); it != pkg->mProviders.End(); ++it) {
        AutoPtr<PackageParser::Provider> p = *it;
        mProviders->RemoveProvider(p);
        String authority;
        if (p->mInfo->GetAuthority(&authority), authority.IsNull()) {
            /* There was another ContentProvider with this authority when
             * this app was installed so this authority is null,
             * Ignore it as we don't have to unregister the provider.
             */
            continue;
        }
        AutoPtr<ArrayOf<String> > names;
        StringUtils::Split(authority, String(";"), (ArrayOf<String>**)&names);
        for (Int32 j = 0; j < names->GetLength(); j++) {
            HashMap<String, AutoPtr<PackageParser::Provider> >::Iterator pIt =
                    mProvidersByAuthority.Find((*names)[j]);
            if (pIt != mProviders.End()) {
                if (pIt->mSecond.Get() == p.Get()) {
                    mProvidersByAuthority.Erase(pIt);
                    if (DEBUG_REMOVE) {
                        Boolean syncable;
                        p->mInfo->GetIsSyncable(&syncable);
                        if (chatty)
                            Logger::D(TAG, "Unregistered content provider: %s, className = %s, isSyncable = %d",
                                    (*names)[j].string(), name.string(), syncable);
                    }
                }
            }
        }
        if (DEBUG_REMOVE && chatty) {
            if (r == NULL) {
                r = new StringBuilder(256);
            }
            else {
                r->AppendChar(' ');
            }
            r->Append(name);
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Providers: %s", r->ToString().string());
    }

    r = NULL;
    List< AutoPtr<PackageParser::Service> >::Iterator sIt;
    for (sIt = pkg->mServices.Begin(); sIt != pkg->mServices.End(); ++sIt) {
        AutoPtr<PackageParser::Service> s = *sIt;
        mServices->RemoveService(s);
        if (chatty) {
            if (r == NULL) {
                r = new StringBuilder(256);
            }
            else {
                r->AppendChar(' ');
            }
            String sname;
            s->mInfo->GetName(&sname);
            r->Append(sname);
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Services: %s", r->ToString().string());
    }

    r = NULL;
    List< AutoPtr<PackageParser::Activity> >::Iterator reIt;
    for (reIt = pkg->mReceivers.Begin(); reIt != pkg->mReceivers.End(); ++reIt) {
        AutoPtr<PackageParser::Activity> a = *reIt;
        mReceivers->RemoveActivity(a, String("receiver"));
        if (DEBUG_REMOVE && chatty) {
            if (r == NULL) {
                r = new StringBuilder(256);
            }
            else {
                r->AppendChar(' ');
            }
            String aname;
            a->mInfo->GetName(&aname);
            r->Append(aname);
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Receivers: %s", r->ToString().string());
    }

    r = NULL;
    List< AutoPtr<PackageParser::Activity> >::Iterator aIt;
    for (aIt = pkg->mActivities.Begin(); aIt != pkg->mActivities.End(); ++aIt) {
        AutoPtr<PackageParser::Activity> a = *aIt;
        mActivities->RemoveActivity(a, String("activity"));
        if (DEBUG_REMOVE && chatty) {
            if (r == NULL) {
                r = new StringBuilder(256);
            }
            else {
                r->AppendChar(' ');
            }
            String aname;
            a->mInfo->GetName(&aname);
            r->Append(aname);
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Activities: %s", r->ToString().string());
    }

    r = NULL;
    List< AutoPtr<PackageParser::Permission> >::Iterator pIt;
    for (pIt = pkg->mPermissions.Begin(); pIt != pkg->mPermissions.End(); ++pIt) {
        AutoPtr<PackageParser::Permission> p = *pIt;
        String pname;
        p->mInfo->GetName(&pname);
        HashMap<String, AutoPtr<BasePermission> >::Iterator bpIt = mSettings->mPermissions.Find(pname);
        AutoPtr<BasePermission> bp;
        if (bpIt != mSettings->mPermissions.End()) bp = bpIt->mSecond;
        if (bp == NULL) {
            HashMap<String, AutoPtr<BasePermission> >::Iterator tIt = mSettings->mPermissionTrees.Find(pname);
            if (tIt != mSettings->mPermissionTrees.End()) bp = tIt->mSecond;
        }
        if (bp != NULL && bp->mPerm.Get() == p.Get()) {
            bp->mPerm = NULL;
            if (DEBUG_REMOVE && chatty) {
                if (r == NULL) {
                    r = new StringBuilder(256);
                }
                else {
                    r->AppendChar(' ');
                }
                r->Append(pname);
            }
        }
        Int32 protectionLevel;
        if (p->mInfo->GetProtectionLevel(&protectionLevel),
                (protectionLevel & IPermissionInfo::PROTECTION_FLAG_APPOP) != 0) {
            AutoPtr<HashSet<String> > appOpPerms;
            HashMap<String, AutoPtr<HashSet<String> > >::Iterator pkgIt = mAppOpPermissionPackages.Find(pname);
            if (pkgIt != mAppOpPermissionPackages.End()) {
                appOpPerms = pkgIt->mSecond;
            }
            if (appOpPerms != NULL) {
                appOpPerms->Erase(pkg->mPackageName);
            }
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Permissions: %s", r->ToString().string());
    }

    N = pkg.requestedPermissions.size();
    r = NULL;
    List<String>::Iterator permissionIt = pkg->mRequestedPermissions.Begin();
    for (; permissionIt != pkg->mRequestedPermissions.End(); ++permissionIt) {
        String perm = *permissionIt;
        AutoPtr<BasePermission> bp;
        HashMap<String, AutoPtr<BasePermission> >::Iterator basePermissionIt = mSettings->mPermissions.Find(perm);
        if (basePermissionIt @= mSettings->mPermissions.End()) {
            bp = basePermissionIt->mSecond;
        }
        if (bp != NULL && (bp->mProtectionLevel & IPermissionInfo::PROTECTION_FLAG_APPOP) != 0) {
            AutoPtr<HashSet<String> > appOpPerms;
            HashMap<String, AutoPtr<HashSet<String> > >::Iterator pkgIt = mAppOpPermissionPackages.Find(perm);
            if (pkgIt != mAppOpPermissionPackages.End()) {
                appOpPerms = pkgIt->mSecond;
            }
            if (appOpPerms != NULL) {
                appOpPerms->Erase(pkg->mPackageName);
                if (appOpPerms->Begin() == appOpPerms->End()) {
                    mAppOpPermissionPackages.Erase(perm);
                }
            }
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Permissions: %s", r->ToString().string());
    }

    r = NULL;
    List< AutoPtr<PackageParser::Instrumentation> >::Iterator iIt;
    for (iIt = pkg->mInstrumentation.Begin(); iIt != pkg->mInstrumentation.End(); ++iIt) {
        AutoPtr<PackageParser::Instrumentation> a = *iIt;
        mInstrumentation.Erase(a->GetComponentName());
        if (DEBUG_REMOVE && chatty) {
            if (r == NULL) {
                r = new StringBuilder(256);
            }
            else {
                r->AppendChar(' ');
            }
            String aname;
            a->mInfo->GetName(&aname);
            r->Append(aname);
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Instrumentation: %s", r->ToString().string());
    }

    r = NULL;
    Int32 flags;
    if ((pkg->mApplicationInfo->GetFlags(&flags), (flags & IApplicationInfo::FLAG_SYSTEM) != 0) {
        // Only system apps can hold shared libraries.
        if (pkg->mLibraryNames != NULL) {
            List<String>::Iterator libNameIt = pkg->mLibraryNames->Begin();
            for (; libNameIt != pkg->mLibraryNames->End(); ++libNameIt) {
                String name = *libNameIt;
                AutoPtr<SharedLibraryEntry> cur;
                HashMap<String, AutoPtr<SharedLibraryEntry> >::Iterator sharedLibIt = mSharedLibraries.Find(name);
                if (sharedLibIt != mSharedLibraries.End()) {
                    cur = sharedLibIt->mSecond();
                }
                if (cur != NULL && cur->mApk != NULL && cur->mApk.Equals(pkg->mPackageName)) {
                    mSharedLibraries.Erase(name);
                    if (DEBUG_REMOVE && chatty) {
                        if (r == NULL) {
                            r = new StringBuilder(256);
                        }
                        else {
                            r->AppendChar(' ');
                        }
                        r->Append(name);
                    }
                }
            }
        }
    }
    if (r != NULL) {
        if (DEBUG_REMOVE) Logger::D(TAG, "  Libraries: %s", r->ToString().string());
    }
}

Boolean CPackageManagerService::HasPermission(
    /* [in] */ PackageParser::Package* pkgInfo,
    /* [in] */ const String& perm)
{
    List< AutoPtr<PackageParser::Permission> >::ReverseIterator rit;
    for (rit = pkgInfo->mPermissions.RBegin(); rit != pkgInfo->mPermissions.REnd(); ++rit) {
        AutoPtr<PackageParser::Permission> p = *rit;
        String name;
        if (p->mInfo->GetName(&name), name.Equals(perm)) {
            return TRUE;
        }
    }
    return FALSE;
}

void CPackageManagerService::UpdatePermissionsLPw(
    /* [in] */ const String& changingPkg,
    /* [in] */ PackageParser::Package* pkgInfo,
    /* [in] */ Int32 flags)
{
    // Make sure there are no dangling permission trees.
    HashMap<String, AutoPtr<BasePermission> >::Iterator pit = mSettings->mPermissionTrees.Begin();
    while (pit != mSettings->mPermissionTrees.End()) {
        AutoPtr<BasePermission> bp = pit->mSecond;
        if (bp->mPackageSetting == NULL) {
            // We may not yet have parsed the capsule, so just see if
            // we still know about its settings->
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it
                    = mSettings->mPackages.Find(bp->mSourcePackage);
            if (it != mSettings->mPackages.End()) {
                bp->mPackageSetting = it->mSecond;
            }
        }
        if (bp->mPackageSetting == NULL) {
//             Slog.w(TAG, "Removing dangling permission tree: " + bp.name
//                     + " from package " + bp.sourcePackage);
            mSettings->mPermissionTrees.Erase(pit++);
        }
        else if (!changingPkg.IsNull() && changingPkg.Equals(bp->mSourcePackage)) {
            if (pkgInfo == NULL || !HasPermission(pkgInfo, bp->mName)) {
//                 Slog.i(TAG, "Removing old permission tree: " + bp.name
//                         + " from package " + bp.sourcePackage);
                flags |= UPDATE_PERMISSIONS_ALL;
                mSettings->mPermissionTrees.Erase(pit++);
            }
            else ++pit;
        }
        else ++pit;
    }

    // Make sure all dynamic permissions have been assigned to a package,
    // and make sure there are no dangling permissions.
    pit = mSettings->mPermissions.Begin();
    while (pit != mSettings->mPermissions.End()) {
        AutoPtr<BasePermission> bp = pit->mSecond;
        if (bp->mType == BasePermission::TYPE_DYNAMIC) {
//             if (DEBUG_SETTINGS) Log.v(TAG, "Dynamic permission: name="
//                     + bp.name + " pkg=" + bp.sourcePackage
//                     + " info=" + bp.pendingInfo);
            if (bp->mPackageSetting == NULL && bp->mPendingInfo != NULL) {
                AutoPtr<BasePermission> tree = FindPermissionTreeLP(bp->mName);
                if (tree != NULL && tree->mPerm != NULL) {
                    bp->mPackageSetting = tree->mPackageSetting;
                    AutoPtr<IPermissionInfo> info;
                    CPermissionInfo::New(bp->mPendingInfo, (IPermissionInfo**)&info);
                    bp->mPerm = new PackageParser::Permission(tree->mPerm->mOwner, info);
                    String name;
                    tree->mPerm->mInfo->GetPackageName(&name);
                    bp->mPerm->mInfo->SetPackageName(name);
                    bp->mPerm->mInfo->SetName(bp->mName);
                    bp->mUid = tree->mUid;
                }
            }
        }
        if (bp->mPackageSetting == NULL) {
            // We may not yet have parsed the capsule, so just see if
            // we still know about its settings->
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it
                    = mSettings->mPackages.Find(bp->mSourcePackage);
            if (it != mSettings->mPackages.End()) {
                bp->mPackageSetting = it->mSecond;
            }
        }
        if (bp->mPackageSetting == NULL) {
//             Slog.w(TAG, "Removing dangling permission: " + bp.name
//                     + " from package " + bp.sourcePackage);
            mSettings->mPermissions.Erase(pit++);
        }
        else if (!changingPkg.IsNull() && changingPkg.Equals(bp->mSourcePackage)) {
            if (pkgInfo == NULL || !HasPermission(pkgInfo, bp->mName)) {
//                 Slog.i(TAG, "Removing old permission: " + bp.name
//                         + " from package " + bp.sourcePackage);
                flags |= UPDATE_PERMISSIONS_ALL;
                mSettings->mPermissions.Erase(pit++);
            }
            else ++pit;
        }
        else ++pit;
    }

    // Now update the permissions for all packages, in particular
    // replace the granted permissions of the system packages.
    if ((flags & UPDATE_PERMISSIONS_ALL) != 0) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it;
        for (it = mPackages.Begin(); it != mPackages.End(); ++it) {
            AutoPtr<PackageParser::Package> pkg = it->mSecond;
            if (pkg.Get() != pkgInfo) {
                GrantPermissionsLPw(pkg, (flags & UPDATE_PERMISSIONS_REPLACE_ALL) != 0, changingPkg);
            }
        }
    }

    if (pkgInfo != NULL) {
        GrantPermissionsLPw(pkgInfo, (flags & UPDATE_PERMISSIONS_REPLACE_PKG) != 0, changingPkg);
    }
}

void CPackageManagerService::GrantPermissionsLPw(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Boolean replace,
    /* [in] */ const String& packageOfInterest)
{
    AutoPtr<PackageSetting> ps = reinterpret_cast<PackageSetting*>(pkg->mExtras->Probe(EIID_PackageSetting));
    if (ps == NULL) {
        return;
    }
    AutoPtr<GrantedPermissions> gp = ps->mSharedUser != NULL
        ? (GrantedPermissions*)ps->mSharedUser : (GrantedPermissions*)ps;
    HashSet<String>* origPermissions = &gp->mGrantedPermissions;
    Boolean changedPermission = FALSE;
    Boolean isNewOrigPermissions = FALSE;

    if (replace) {
        ps->mPermissionsFixed = FALSE;
        if (gp.Get() == (GrantedPermissions*)ps.Get()) {
            isNewOrigPermissions = TRUE;
            origPermissions = new HashSet<String>(
                    gp->mGrantedPermissions.Begin(), gp->mGrantedPermissions.End());
            gp->mGrantedPermissions.Clear();
            gp->mGids = mGlobalGids;
        }
    }

    if (gp->mGids == NULL) {
        gp->mGids = mGlobalGids;
    }

    List<String>::Iterator rpit;
    for (rpit = pkg->mRequestedPermissions.Begin(); rpit != pkg->mRequestedPermissions.End(); ++rpit) {
        String name = *rpit;
        Boolean required = pkg->mRequestedPermissionsRequired[i];
        AutoPtr<BasePermission> bp;
        HashMap< String, AutoPtr<BasePermission> >::Iterator it = mSettings->mPermissions.Find(name);
        if (it != mSettings->mPermissions.End()) {
            bp = it->mSecond;
        }
        if (DEBUG_INSTALL) {
            if (gp != ps) {
                Logger::I(TAG, "Package %s checking %s: %p", pkg->mPackageName.string(), name.string(), bp.Get());
            }
        }
        if (bp == NULL || bp->mPackageSetting == NULL) {
            if (packageOfInterest.IsNull() || packageOfInterest.Equals(pkg->mPackageName)) {
                Slogger::W(TAG, "Unknown permission %s in package %s", name.string(), pkg->mPackageName.string());
            }
            continue;
        }

        String perm = bp->mName;
        Boolean allowed;
        Boolean allowedSig = FALSE;
        if ((bp->mProtectionLevel & IPermissionInfo::PROTECTION_FLAG_APPOP) != 0) {
            // Keep track of app op permissions.
            AutoPtr<HashSet<String> > pkgs;
            HashMap<String, AutoPtr<HashSet<String> > >::Iterator pkgIt = mAppOpPermissionPackages.Find(bp->mName);
            if (pkgIt != mAppOpPermissionPackages.End()) {
                pkgs = pkgIt->mSecond;
            }
            if (pkgs == NULL) {
                pkgs = new HashSet<String>();
                mAppOpPermissionPackages[bp->mName] = pkgs;
            }
            pkgs->Insert(pkg->mPackageName);
        }
        Int32 level = bp->mProtectionLevel & IPermissionInfo::PROTECTION_MASK_BASE;
        if (level == IPermissionInfo::PROTECTION_NORMAL
                || level == IPermissionInfo::PROTECTION_DANGEROUS) {
            // We grant a normal or dangerous permission if any of the following
            // are true:
            // 1) The permission is required
            // 2) The permission is optional, but was granted in the past
            // 3) The permission is optional, but was requested by an
            //    app in /system (not /data)
            //
            // Otherwise, reject the permission.
            allowed = (required || (origPermissions->Find(perm) != origPermissions->End())
                    || (IsSystemApp(ps) && !IsUpdatedSystemApp(ps)));
        }
        else if (bp->mPackageSetting == NULL) {
            // This permission is invalid; skip it.
            allowed = FALSE;
        }
        else if (level == IPermissionInfo::PROTECTION_SIGNATURE) {
            allowed = GrantSignaturePermission(perm, pkg, bp, origPermissions);
            if (allowed) {
                allowedSig = TRUE;
            }
        }
        else {
            allowed = FALSE;
        }
        if (DEBUG_INSTALL) {
            if (gp != ps) {
                Logger::I(TAG, "Package %s granting %s", pkg->mPackageName.string(), perm.string());
            }
        }
        if (allowed) {
            if (!iIsSystemApp(ps) && ps->mPermissionsFixed) {
                // If this is an existing, non-system package, then
                // we can't add any new permissions to it.
                if (!allowedSig && gp->mGrantedPermissions.Find(perm) == gp->mGrantedPermissions.End()) {
                    // Except...  if this is a permission that was added
                    // to the platform (note: need to only do this when
                    // updating the platform).
                    allowed = IsNewPlatformPermissionForPackage(perm, pkg);
                }
            }
            if (allowed) {
                if (gp->mGrantedPermissions.Find(perm) == gp->mGrantedPermissions.End()) {
                    changedPermission = TRUE;
                    gp->mGrantedPermissions.Insert(perm);
                    gp->mGids = AppendInts(gp->mGids, bp->mGids);
                }
                else if (!ps->mHaveGids) {
                    gp->mGids = appendInts(gp->mGids, bp->mGids);
                }
            }
            else {
                if (packageOfInterest.IsNull() || packageOfInterest.Equals(pkg->mPackageName)) {
                    Slogger::W(TAG, "Not granting permission %s to package %s because it was previously installed without",
                            perm.string(), pkg->mPackageName.string());
                }
            }
        }
        else {
            if (gp->mGrantedPermissions.Find(perm) != gp->mGrantedPermissions.End()) {
                gp->mGrantedPermissions.Erase(perm);
                changedPermission = TRUE;
                gp->mGids = RemoveInts(gp->mGids, bp->mGids);
                Int32 flags;
                pkg->mApplicationInfo->GetFlags(&flags);
                Slogger::I(TAG, "Un-granting permission %s from package %s (protectionLevel=%d flags=0x%d)",
                        perm.string(), pkg->mPackageName.string(), bp->mProtectionLevel, flags);
            }
            else if ((bp->mProtectionLevel & IPermissionInfo::PROTECTION_FLAG_APPOP) == 0) {
                // Don't print warning for app op permissions, since it is fine for them
                // not to be granted, there is a UI for the user to decide.
                if (packageOfInterest.IsNull() || packageOfInterest.Equals(pkg->mPackageName)) {
                    Int32 flags;
                    pkg->mApplicationInfo->GetFlags(&flags);
                    Slogger::W(TAG, "Not granting permission %s to package %s (protectionLevel=%d flags=0x%d)",
                            perm.string(), pkg->mPackageName.string(), bp->mProtectionLevel, flags);
                }
            }
        }
    }

    if ((changedPermission || replace) && !ps->mPermissionsFixed &&
            !IsSystemApp(ps) || IsUpdatedSystemApp(ps)) {
        // This is the first that we have heard about this package, so the
        // permissions we have now selected are fixed until explicitly
        // changed.
        ps->mPermissionsFixed = TRUE;
    }
    ps->mHaveGids = TRUE;

    if (isNewOrigPermissions) {
        delete origPermissions;
    }
}

Boolean CPackageManagerService::IsNewPlatformPermissionForPackage(
    /* [in] */ const String& perm,
    /* [in] */ PackageParser::Package* pkg)
{
    Boolean allowed = FALSE;
    for (Int32 ip = 0; ip < PackageParser::NEW_PERMISSIONS->GetLength(); ip++) {
        AutoPtr<PackageParser::NewPermissionInfo> npi = (*PackageParser::NEW_PERMISSIONS)[ip];
        if (npi->mName.Equals(perm)
                && pkg->mApplicationInfo->mTargetSdkVersion < npi->mSdkVersion) {
            allowed = TRUE;
            Logger::I(TAG, "Auto-granting %s to old pkg %s", perm.string(), pkg->mPackageName.string());
            break;
        }
    }
    return allowed;
}

Boolean CPackageManagerService::GrantSignaturePermission(
    /* [in] */ const String& perm,
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ BasePermission* bp,
    /* [in] */ HashSet<String>* origPermissions)
{
    Boolean allowed;
    allowed = (CompareSignatures(bp->mPackageSetting->mSignatures->mSignatures, pkg->mSignatures)
                    == IPackageManager::SIGNATURE_MATCH)
            || (compareSignatures(mPlatformPackage->mSignatures, pkg->mSignatures)
                    == IPackageManager::SIGNATURE_MATCH);
    if (!allowed && (bp->mProtectionLevel
            & IPermissionInfo::PROTECTION_FLAG_SYSTEM) != 0) {
        if (IsSystemApp(pkg)) {
            // For updated system applications, a system permission
            // is granted only if it had been defined by the original application.
            if (IsUpdatedSystemApp(pkg)) {
                AutoPtr<PackageSetting> sysPs = mSetting->GetDisabledSystemPkgLPr(pkg->mPackageName);
                AutoPtr<GrantedPermissions> origGp = sysPs->mSharedUser != NULL
                        ? sysPs->mSharedUser : (GrantedPermissions*)sysPs.Get();

                if (origGp->mGrantedPermissions.Find(perm) != origGp->mGrantedPermissions.End()) {
                    // If the original was granted this permission, we take
                    // that grant decision as read and propagate it to the
                    // update.
                    allowed = TRUE;
                }
                else {
                    // The system apk may have been updated with an older
                    // version of the one on the data partition, but which
                    // granted a new system permission that it didn't have
                    // before.  In this case we do want to allow the app to
                    // now get the new permission if the ancestral apk is
                    // privileged to get it.
                    if (sysPs->mPkg != NULL && sysPs->IsPrivileged()) {
                        List<String>::Iterator permIt = sysPs->mPkg->mRequestedPermissions.Begin();
                        for (; permIt != sysPs->mPkg->mRequestedPermissions.End(); ++permIt) {
                            if (perm.Equals(*permIt)) {
                                allowed = TRUE;
                                break;
                            }
                        }
                    }
                }
            }
            else {
                allowed = IsPrivilegedApp(pkg);
            }
        }
    }
    if (!allowed && (bp->mProtectionLevel
            & IPermissionInfo::PROTECTION_FLAG_DEVELOPMENT) != 0) {
        // For development permissions, a development permission
        // is granted only if it was already granted.
        allowed = (origPermissions->Find(perm) != origPermissions->End());
    }
    return allowed;
}

void CPackageManagerService::SendPackageBroadcast(
    /* [in] */ const String& action,
    /* [in] */ const String& pkg,
    /* [in] */ IBundle* extras,
    /* [in] */ const String& targetPkg,
    /* [in] */ IIntentReceiver* finishedReceiver,
    /* [in] */ ArrayOf<Int32>* _userIds)
{
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am != NULL) {
        AutoPtr<ArrayOf<Int32> > userIds = _userIds;
        if (userIds == NULL) {
            am->GetRunningUserIds((ArrayOf<Int32>**)&userIds);
        }

        // TODO:delete
        //
        if (userIds == NULL) {
            userIds = ArrayOf<Int32>::Alloc(1);
            (*userIds)[0] = 0;
        }

        Int32 length = userIds->GetLength();
        for (Int32 i = 0; i < length; i++) {
            Int32 id = (*userIds)[i];
            AutoPtr<IUri> uri = !pkg.IsNull() ? (Uri::FromParts(
                    String("package"), pkg, String(NULL), (IUri**)&uri), uri) : NULL;
            AutoPtr<IIntent> intent;
            CIntent::New(action, uri, (IIntent**)&intent);
            if (extras != NULL) {
                intent->PutExtras(extras);
            }
            if (!targetPkg.IsNull()) {
                intent->SetPackage(targetPkg);
            }
            // Modify the UID when posting to other users
            Int32 uid;
            intent->GetInt32Extra(IIntent::EXTRA_UID, -1, &uid);
            if (uid > 0 && UserHandle::GetUserId(uid) != id) {
                uid = UserHandle::GetUid(id, UserHandle::GetAppId(uid));
                intent->PutExtra(IIntent::EXTRA_UID, uid);
            }
            intent->PutExtra(IIntent::EXTRA_USER_HANDLE, id);
            intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
            // if (DEBUG_BROADCASTS) {
            //     RuntimeException here = new RuntimeException("here");
            //     here.fillInStackTrace();
            //     Slog.d(TAG, "Sending to user " + id + ": "
            //             + intent.toShortString(false, true, false, false)
            //             + " " + intent.getExtras(), here);
            // }
            Int32 res;
            am->BroadcastIntent(NULL, intent, String(NULL), finishedReceiver,
                    0, String(NULL), NULL, String(NULL), IAppOpsManager::OP_NONE,
                    finishedReceiver != NULL, FALSE, id, &res);
        }
    }
}

Boolean CPackageManagerService::IsExternalMediaAvailable()
{
    if (mMediaMounted) {
        return TRUE;
    }
    else {
        return Environment::IsExternalStorageEmulated();
    }
}

ECode CPackageManagerService::NextPackageToClean(
    /* [in] */ IPackageCleanItem* lastPackage,
    /* [out] */ IPackageCleanItem** nextPackage)
{
    VALIDATE_NOT_NULL(nextPackage);
    *nextPackage = NULL;

    // writer
    synchronized (mPackagesLock) {
        if (!IsExternalMediaAvailable()) {
            // If the external storage is no longer mounted at this point,
            // the caller may not have been able to delete all of this
            // packages files and can not delete any more.  Bail.
            return NOERROR;
        }
        List<AutoPtr<IPackageCleanItem> > pkgs = mSettings->mPackagesToBeCleaned;
        if (lastPackage != NULL) {
            pkgs.Erase(lastPackage);
        }
        if (pkgs.Begin() != pkgs.End()) {
            *nextPackage = *pkgs.Begin();
            REFCOUNT_ADD(*nextPackage)
        }
    }

    return NOERROR;
}

void CPackageManagerService::SchedulePackageCleaning(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean andCode)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(START_CLEANING_PACKAGE,
            userId, andCode ? 1 : 0, packageName, (IMessage**)&msg);
    if (mSystemReady) {
        msg->SendToTarget();
    }
    else {
        if (mPostSystemReadyMessages == NULL) {
            mPostSystemReadyMessages = new List<AutoPtr<IMessage> >();
        }
        mPostSystemReadyMessages->PushBack(msg);
    }
}

void CPackageManagerService::StartCleaningPackages()
{
    // reader
    synchronized (mPackagesLock) {
        if (!IsExternalMediaAvailable()) {
            return;
        }
        if (mSettings->mPackagesToBeCleaned.IsEmpty()) {
            return;
        }
    }
    AutoPtr<IIntent> intent;
    CIntent::New(IPackageManager::ACTION_CLEAN_EXTERNAL_STORAGE, (IIntent**)&intent);
    intent->SetComponent(DEFAULT_CONTAINER_COMPONENT);
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am != NULL) {
        AutoPtr<IComponentName> name;
        am->StartService(NULL, intent, String(NULL), IUserHandle::USER_OWNER, (IComponentName**)&name);
    }
}

ECode CPackageManagerService::InstallPackage(
    /* [in] */ const String& originPath,
    /* [in] */ IIPackageInstallObserver2* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IVerificationParams* verificationParams,
    /* [in] */ const String& packageAbiOverride)
{
    return InstallPackageAsUser(originPath, observer, installFlags, installerPackageName,
            verificationParams, packageAbiOverride, UserHandle::GetCallingUserId());
}

ECode CPackageManagerService::InstallPackageAsUser(
    /* [in] */ const String& originPath,
    /* [in] */ IIPackageInstallObserver2* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IVerificationParams* verificationParams,
    /* [in] */ const String& packageAbiOverride,
    /* [in] */ Int32 userId)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::INSTALL_PACKAGES, String(NULL)))

    Int32 callingUid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(callingUid, userId, TRUE, TRUE, String("installPackageAsUser")))

    if (IsUserRestricted(userId, IUserManager::DISALLOW_INSTALL_APPS)) {
        // try {
        if (observer != NULL) {
            observer->OnPackageInstalled(String(""), INSTALL_FAILED_USER_RESTRICTED, String(NULL), NULL);
        }
        // } catch (RemoteException re) {
        // }
        return NOERROR;
    }

    if ((callingUid == IProcess::SHELL_UID) || (callingUid == IProcess::ROOT_UID)) {
        installFlags |= IPackageManager::INSTALL_FROM_ADB;

    }
    else {
        // Caller holds INSTALL_PACKAGES permission, so we're less strict
        // about installerPackageName.

        installFlags &= ~IPackageManager::INSTALL_FROM_ADB;
        installFlags &= ~IPackageManager::INSTALL_ALL_USERS;
    }

    AutoPtr<IUserHandle> user;
    if ((installFlags & IPackageManager::INSTALL_ALL_USERS) != 0) {
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetAll((IUserHandle**)&user);
    }
    else {
        CUserHandle::New(userId, (IUserHandle**)&user);
    }

    verificationParams->SetInstallerUid(callingUid);

    AutoPtr<IFile> originFile;
    CFile::New(originPath, (IFile**)&originFile);
    AutoPtr<OriginInfo> origin = OriginInfo::FromUntrustedFile(originFile);

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(INIT_COPY, (IMessage**)&msg);
    AutoPtr<InstallParams> params = new InstallParams(origin, observer, installFlags,
            installerPackageName, verificationParams, user, packageAbiOverride);
    msg->SetObj((IInterface*)params);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void CPackageManagerService::InstallStage(
    /* [in] */ const String& packageName,
    /* [in] */ IFile* stagedDir,
    /* [in] */ const String& stagedCid,
    /* [in] */ IIPackageInstallObserver2* observer,
    /* [in] */ IPackageInstallerSessionParams* params,
    /* [in] */ const String& installerPackageName,
    /* [in] */ Int32 installerUid,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IVerificationParams> verifParams;
    AutoPtr<IUri> oriUri, referrerUri;
    params->GetOriginatingURI((IUri**)&oriUri);
    params->GetReferrerUri((IUri**)&referrerUri);
    CVerificationParams::New(NULL, oriUri, referrerUri, installerUid, NULL);

    AutoPtr<OriginInfo> origin;
    if (stagedDir != NULL) {
        origin = OriginInfo::FromStagedFile(stagedDir);
    }
    else {
        origin = OriginInfo::FromStagedContainer(stagedCid);
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(INIT_COPY, (IMessage**)&msg);
    Int32 installFlags;
    params->GetInstallFlags(&installFlags);
    String abiOverride;
    params->GetAbiOverride(&abiOverride);
    AutoPtr<InstallParams> params = new InstallParams(origin, observer, installFlags,
            installerPackageName, verificationParams, user, abiOverride);
    msg->SetObj((IInterface*)params);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void CPackageManagerService::SendPackageAddedForUser(
    /* [in] */ const String& packageName,
    /* [in] */ PackageSetting* pkgSetting,
    /* [in] */ Int32 userId)
{
    AutoPtr<IBundle> extras;
    CBundle(1, (IBundle**)&extras);
    extras->PutInt32(IIntent::EXTRA_UID, UserHandle::GetUid(userId, pkgSetting->mAppId));

    AutoPtr< ArrayOf<Int32> > userIds = ArrayOf<Int32>::Alloc(1);
    (*userIds)[0] = userId;
    SendPackageBroadcast(IIntent::ACTION_PACKAGE_ADDED, packageName, extras, String(NULL), NULL, userIds);
    // try {
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    Boolean isSystem = IsSystemApp(pkgSetting) || IsUpdatedSystemApp(pkgSetting);
    Boolean isUserRunning;
    if (isSystem && (am->IsUserRunning(userId, FALSE, &isUserRunning), isUserRunning)) {
        // The just-installed/enabled app is bundled on the system, so presumed
        // to be able to run automatically without needing an explicit launch.
        // Send it a BOOT_COMPLETED if it would ordinarily have gotten one.
        AutoPtr<IIntent> bcIntent;
        CIntent::New(IIntent::ACTION_BOOT_COMPLETED, (IIntent**)&bcIntent);
        bcIntent->AddFlags(IIntent::FLAG_INCLUDE_STOPPED_PACKAGES)
        bcIntent->SetPackage(packageName);
        Boolean result;
        if (FAILED(am->BroadcastIntent(NULL, bcIntent, String(NULL), NULL, 0, String(NULL),
                NULL, String(NULL), IAppOpsManager::OP_NONE, FALSE, FALSE, userId, &result))) {
            Slogger::W(TAG, "Unable to bootstrap installed package");
        }
    }
    // } catch (RemoteException e) {
    //     // shouldn't happen
    //     Slog.w(TAG, "Unable to bootstrap installed package", e);
    // }
}

ECode CPackageManagerService::SetApplicationHiddenSettingAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean hide,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Core::Manifest::permission::MANAGE_USERS, String(NULL)))
    AutoPtr<PackageSetting> pkgSetting;
    Int32 uid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(uid, userId, TRUE, TRUE,
            String("setApplicationHiddenSetting for user ") + StringUtils::ToString(userId)))

    if (hidden && IsPackageDeviceAdmin(packageName, userId)) {
        Slogger::W(TAG, "Not hiding package %s: has active device admin", packageName.string());
        return NOERROR;
    }

    Int64 callingId = Binder::ClearCallingIdentity();
    // try {
    Boolean sendAdded = FALSE;
    Boolean sendRemoved = FALSE;
    // writer
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        if (it != mSettings->mPackages.End()) {
            pkgSetting = it->mSecond;
        }
        if (pkgSetting == NULL) {
            Binder::RestoreCallingIdentity(callingId);
            return NOERROR;
        }
        if (pkgSetting->GetHidden(userId) != hidden) {
            pkgSetting->SetHidden(hidden, userId);
            mSettings->WritePackageRestrictionsLPr(userId);
            if (hidden) {
                sendRemoved = TRUE;
            }
            else {
                sendAdded = TRUE;
            }
        }
    }
    if (sendAdded) {
        SendPackageAddedForUser(packageName, pkgSetting, userId);
        *result = TRUE;
        Binder::RestoreCallingIdentity(callingId);
        return NOERROR
    }
    if (sendRemoved) {
        KillApplication(packageName, UserHandle::GetUid(userId, pkgSetting->mAppId),
                String("hiding pkg"));
        SendApplicationHiddenForUser(packageName, pkgSetting, userId);
    }
    // } finally {
    //     Binder.restoreCallingIdentity(callingId);
    // }
    Binder::RestoreCallingIdentity(callingId);
    *result = FALSE;
    return NOERROR;
}

void CPackageManagerService::SendApplicationHiddenForUser(
    /* [in] */ const String& packageName,
    /* [in] */ PackageSetting* pkgSetting,
    /* [in] */ Int32 userId)
{
    AutoPtr<PackageRemovedInfo> info = new PackageRemovedInfo();
    info->mRemovedPackage = packageName;
    info->mRemovedUsers = ArrayOf<Int32>::Alloc(1);
    (*info->mRemovedUsers)[0] = userId;
    info->mUid = UserHandle::GetUid(userId, pkgSetting->mAppId);
    info->SendBroadcast(FALSE, FALSE, FALSE);
}

ECode CPackageManagerService::GetApplicationHiddenSettingAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Core::Manifest::permission::MANAGE_USERS, String(NULL)))
    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(), userId, TRUE,
            FALSE, String("getApplicationHidden for user ") + StringUtils::ToString(userId)))
    AutoPtr<PackageSetting> pkgSetting;
    Int64 callingId = Binder::ClearCallingIdentity();
    // try {
    // writer
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        if (it != mSettings->mPackages.End()) {
            pkgSetting = it->mSecond;
        }
        if (pkgSetting == NULL) {
            *result = TRUE;
            Binder::RestoreCallingIdentity(callingId);
            return NOERROR;
        }
        *result = pkgSetting->GetHidden(userId);
    }
    // } finally {
    //     Binder.restoreCallingIdentity(callingId);
    // }
    Binder::RestoreCallingIdentity(callingId);
    return NOERROR;
}

ECode CPackageManagerService::InstallExistingPackageAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::INSTALL_PACKAGES, String(NULL)))
    AutoPtr<PackageSetting> pkgSetting;
    Int32 uid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(uid, userId, TRUE, TRUE,
            String("installExistingPackage for user " + StringUtils::ToString(userId))))
    if (IsUserRestricted(userId, UserManager.DISALLOW_INSTALL_APPS)) {
        *result = IPackageManager::INSTALL_FAILED_USER_RESTRICTED;
        return NOERROR;
    }

    Int64 callingId = Binder::ClearCallingIdentity();
    // try {
    Boolean sendAdded = FALSE;
    AutoPtr<IBundle> extras;
    CBundle::New(1, (IBundle**)&extras);

    // writer
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator psIt = mSettings->mPackages.Find(packageName);
        if (psIt != mSettings->mPackages.End()) pkgSetting = psIt->mSecond;
        if (pkgSetting == NULL) {
            *result = IPackageManager::INSTALL_FAILED_INVALID_URI;
            Binder::RestoreCallingIdentity(callingId);
            return NOERROR;
        }
        if (!pkgSetting->GetInstalled(userId)) {
            pkgSetting->SetInstalled(TRUE, userId);
            pkgSetting->SetHidden(FALSE, userId);
            mSettings->WritePackageRestrictionsLPr(userId);
            sendAdded = TRUE;
        }
    }

    if (sendAdded) {
        SendPackageAddedForUser(packageName, pkgSetting, userId);
    }
    // } finally {
    Binder::RestoreCallingIdentity(callingId);
    // }

    *result = IPackageManager::INSTALL_SUCCEEDED;
    return NOERROR;
}

Boolean CPackageManagerService::IsUserRestricted(
    /* [in] */ Int32 userId,
    /* [in] */ const String& restrictionKey)
{
    AutoPtr<IBundle> restrictions = sUserManager->GetUserRestrictions(userId);
    Boolean value;
    if (restrictions->GetBoolean(restrictionKey, FALSE, &value), value) {
        Logger::W(TAG, "User is restricted: %s", restrictionKey.string());
        return TRUE;
    }
    return FALSE;
}

ECode CPackageManagerService::VerifyPendingInstall(
    /* [in] */ Int32 id,
    /* [in] */ Int32 verificationCode)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::PACKAGE_VERIFICATION_AGENT,
            String("Only package verification agents can verify applications")))

    AutoPtr<PackageVerificationResponse> response = new PackageVerificationResponse(
            verificationCode, Binder::GetCallingUid());

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(PACKAGE_VERIFIED, (IMessage**)&msg);
    msg->SetObj(response);
    msg->SetArg1(id);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

ECode CPackageManagerService::ExtendVerificationTimeout(
    /* [in] */ Int32 id,
    /* [in] */ Int32 verificationCodeAtTimeout,
    /* [in] */ Int64 millisecondsToDelay)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::PACKAGE_VERIFICATION_AGENT,
            String("Only package verification agents can extend verification timeouts")))

    HashMap<Int32, AutoPtr<PackageVerificationState> >::Iterator sIt = mPendingVerification.Find(id);
    AutoPtr<PackageVerificationState> state;
    if (sIt != mPendingVerification.End()) state = sIt->mSecond;
    AutoPtr<PackageVerificationResponse> response = new PackageVerificationResponse(
            verificationCodeAtTimeout, Binder::GetCallingUid());

    if (millisecondsToDelay > IPackageManager::MAXIMUM_VERIFICATION_TIMEOUT) {
        millisecondsToDelay = IPackageManager::MAXIMUM_VERIFICATION_TIMEOUT;
    }
    if (millisecondsToDelay < 0) {
        millisecondsToDelay = 0;
    }
    if ((verificationCodeAtTimeout != IPackageManager::VERIFICATION_ALLOW)
            && (verificationCodeAtTimeout != IPackageManager::VERIFICATION_REJECT)) {
        verificationCodeAtTimeout = IPackageManager::VERIFICATION_REJECT;
    }

    if ((state != NULL) && !state->TimeoutExtended()) {
        state->ExtendTimeout();

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(PACKAGE_VERIFIED, (IMessage**)&msg);
        msg->SetObj(response);
        msg->SetArg1(id);
        Boolean result;
        mHandler->SendMessageDelayed(msg, millisecondsToDelay, &result);
    }
    return NOERROR;
}

ECode CPackageManagerService::BroadcastPackageVerified(
    /* [in] */ Int32 verificationId,
    /* [in] */ IUri* packageUri,
    /* [in] */ Int32 verificationCode,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_PACKAGE_VERIFIED, (IIntent**)&intent);
    intent->SetDataAndType(packageUri, PACKAGE_MIME_TYPE);
    intent->AddFlags(IIntent::FLAG_GRANT_READ_URI_PERMISSION);
    intent->PutExtra(IPackageManager::EXTRA_VERIFICATION_ID, verificationId);
    intent->PutExtra(IPackageManager::EXTRA_VERIFICATION_RESULT, verificationCode);

    mContext->SendBroadcastAsUser(intent, user,
            Elastos::Droid::Manifest::permission::PACKAGE_VERIFICATION_AGENT);
    return NOERROR;
}

AutoPtr<IComponentName> CPackageManagerService::MatchComponentForVerifier(
    /* [in] */ const String& packageName,
    /* [in] */ List<AutoPtr<IResolveInfo> >* receivers)
{
    AutoPtr<IActivityInfo> targetReceiver;

    List<AutoPtr<IResolveInfo> >::Iterator it;
    for (it = receivers->Begin(); it != receivers->End(); ++it) {
        AutoPtr<IResolveInfo> info = *it;
        AutoPtr<IActivityInfo> actInfo;
        info->GetActivityInfo((IActivityInfo**)&actInfo);
        if (actInfo == NULL) {
            continue;
        }

        String pkgName;
        if (actInfo->GetPackageName(&pkgName), packageName.Equals(pkgName)) {
            targetReceiver = actInfo;
            break;
        }
    }

    if (targetReceiver == NULL) {
        return NULL;
    }

    String pkgName, name;
    targetReceiver->GetPackageName(&pkgName);
    targetReceiver->GetName(&name);
    AutoPtr<IComponentName> component;
    CComponentName::New(pkgName, name, (IComponentName**)&component);
    return component;
}

AutoPtr<List<AutoPtr<IComponentName> > > CPackageManagerService::MatchVerifiers(
    /* [in] */ IPackageInfoLite* pkgInfo,
    /* [in] */ List< AutoPtr<IResolveInfo> >* receivers,
    /* [in] */ PackageVerificationState* verificationState)
{
    AutoPtr<ArrayOf<IVerifierInfo*> > verifiers;
    pkgInfo->GetVerifiers((ArrayOf<IVerifierInfo*>**)&verifiers);
    if (verifiers->GetLength() == 0) {
        return NULL;
    }

    Int32 N = verifiers->GetLength();
    AutoPtr<List<AutoPtr<IComponentName> > > sufficientVerifiers = new List<AutoPtr<IComponentName> >(N + 1);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IVerifierInfo> verifierInfo = (*verifiers)[i];

        String vPkgName;
        verifierInfo->GetPackageName(&vPkgName);
        AutoPtr<IComponentName> comp = MatchComponentForVerifier(vPkgName,
                receivers);
        if (comp == NULL) {
            continue;
        }

        const Int32 verifierUid = GetUidForVerifier(verifierInfo);
        if (verifierUid == -1) {
            continue;
        }

        if (DEBUG_VERIFY) {
            Slogger::D(TAG, "Added sufficient verifier %s with the correct signature", vPkgName.string());
        }
        sufficientVerifiers->PushBack(comp);
        verificationState->AddSufficientVerifier(verifierUid);
    }

    return sufficientVerifiers;
}

Int32 CPackageManagerService::GetUidForVerifier(
    /* [in] */ IVerifierInfo* verifierInfo)
{
    synchronized (mPackages) {
        String vPkgName;
        verifierInfo->GetPackageName(&vPkgName);
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(vPkgName);
        AutoPtr<PackageParser::Package> pkg;
        if (it != mPackages.End()) pkg = it->mSecond;
        if (pkg == NULL) {
            return -1;
        }
        else if (pkg->mSignatures->GetLength() != 1) {
            Slogger::I(TAG, "Verifier package %s has more than one signature; ignoring", vPkgName.string());
            return -1;
        }

        /*
         * If the public key of the package's signature does not match
         * our expected public key, then this is a different package and
         * we should skip.
         */

        AutoPtr<ArrayOf<Byte> > expectedPublicKey;
        // try {
        AutoPtr<ISignature> verifierSig = (*pkg->mSignatures)[0];
        AutoPtr<IPublicKey> publicKey;
        if (FAILED(verifierSig->GetPublicKey((IPublicKey**)&publicKey)) ) return -1;
        if (FAILED(publicKey->GetEncoded((ArrayOf<Byte>**)&expectedPublicKey)) ) return -1;
        // } catch (CertificateException e) {
        //     return -1;
        // }
        AutoPtr<IPublicKey> infoPublicKey;
        verifierInfo->GetPublicKey((IPublicKey**)&infoPublicKey);
        AutoPtr<ArrayOf<Byte> > actualPublicKey;
        infoPublicKey->GetEncoded((ArrayOf<Byte>**)&actualPublicKey);

        if (!Arrays::Equals(actualPublicKey, expectedPublicKey)) {
            Slogger::I(TAG, "Verifier package %s does not have the expected public key; ignoring",
                    verifierInfo->mPackageName.string());
            return -1;
        }

        Int32 uid;
        pkg->mApplicationInfo->GetUid(&uid);
        return uid;
    }
}

ECode CPackageManagerService::FinishPackageInstall(
    /* [in] */ Int32 token)
{
    FAIL_RETURN(EnforceSystemOrRoot(String("Only the system is allowed to finish installs")))

    if (DEBUG_INSTALL) {
        Slogger::V(TAG, "BM finishing package install for %d", token);
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(POST_INSTALL, (IMessage**)&msg);
    msg->SetArg1(token);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

Int64 CPackageManagerService::GetVerificationTimeout()
{
    AutoPtr<ISettingsGlobal> sGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Int64 value;
    sGlobal->GetInt64(resolver,
            ISettingsGlobal::PACKAGE_VERIFIER_TIMEOUT,
            DEFAULT_VERIFICATION_TIMEOUT, &value);
    return value;
}

Int32 CPackageManagerService::GetDefaultVerificationResponse()
{
    AutoPtr<ISettingsGlobal> sGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Int32 value;
    sGlobal->GetInt32(resolver,
            ISettingsGlobal::PACKAGE_VERIFIER_DEFAULT_RESPONSE,
            DEFAULT_VERIFICATION_RESPONSE, &value);
    return value;
}

Boolean CPackageManagerService::IsVerificationEnabled(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 installFlags)
{
    if (!DEFAULT_VERIFY_ENABLE) {
        return FALSE;
    }

    Boolean ensureVerifyAppsEnabled = IsUserRestricted(userId, IUserManager::ENSURE_VERIFY_APPS);

    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsGlobal> sGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);

    // Check if installing from ADB
    if ((installFlags & IPackageManager::INSTALL_FROM_ADB) != 0) {
        // Do not run verification in a test harness environment
        AutoPtr<IActivityManagerHelper> amHelper;
        CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
        Boolean isRunning;
        if (amHelper->IsRunningInTestHarness(&isRunning), isRunning) {
            return FALSE;
        }
        if (ensureVerifyAppsEnabled) {
            return TRUE;
        }
        // Check if the developer does not want package verification for ADB installs
        Int32 value;
        sGlobal->GetInt32(resolver, ISettingsGlobal::PACKAGE_VERIFIER_INCLUDE_ADB, 1, &value);
        if (value == 0) {
            return FALSE;
        }
    }

    if (ensureVerifyAppsEnabled) {
        return TRUE;
    }

    Int32 ivalue;
    sGlobal->GetInt32(resolver, ISettingsGlobal::PACKAGE_VERIFIER_ENABLE, 1, &ivalue);
    return ivalue == 1;
}

Int32 CPackageManagerService::GetUnknownSourcesSettings()
{
    AutoPtr<ISettingsGlobal> sGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Int32 value;
    sGlobal->GetInt32(resolver,
            ISettingsGlobal::INSTALL_NON_MARKET_APPS,
            -1, &value);
    return value;
}

ECode CPackageManagerService::SetInstallerPackageName(
    /* [in] */ const String& targetPackage,
    /* [in] */ const String& installerPackageName)
{
    const Int32 uid = Binder::GetCallingUid();
    // writer
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(targetPackage);
        AutoPtr<PackageSetting> targetPackageSetting;
        if (it != mSettings->mPackages.End()) targetPackageSetting = it->mSecond;
        if (targetPackageSetting == NULL) {
            Slogger::E(TAG, "Unknown target package: %s", targetPackage.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        AutoPtr<PackageSetting> installerPackageSetting;
        if (!installerPackageName.IsNull()) {
            it = mSettings->mPackages.Find(installerPackageName);
            if (it != mSettings->mPackages.End()) {
                installerPackageSetting = it->mSecond;
            }
            if (installerPackageSetting == NULL) {
                Slogger::E(TAG, "Unknown installer package: %s", installerPackageName.string());
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
        else {
            installerPackageSetting = NULL;
        }

        AutoPtr<ArrayOf<ISignature*> > callerSignature;
        AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(uid);
        if (obj != NULL) {
            if (obj->Probe(EIID_SharedUserSetting) != NULL) {
                SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
                callerSignature = sus->mSignatures->mSignatures;
            }
            else if (obj->Probe(EIID_PackageSetting) != NULL) {
                PackageSetting* ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
                callerSignature = ps->mSignatures->mSignatures;
            }
            else {
                Slogger::E(TAG, "Bad object %p for uid %d", obj.Get(), uid);
                return E_SECURITY_EXCEPTION;
            }
        }
        else {
            Slogger::E(TAG, "Bad Unknown calling uid %d", uid);
            return E_SECURITY_EXCEPTION;
        }

        // Verify: can't set installerPackageName to a package that is
        // not signed with the same cert as the caller.
        if (installerPackageSetting != NULL) {
            if (CompareSignatures(callerSignature,
                    installerPackageSetting->mSignatures->mSignatures)
                    != IPackageManager::SIGNATURE_MATCH) {
                Slogger::E(TAG, "Caller does not have same cert as new installer package %s",
                        installerPackageName.string());
                return E_SECURITY_EXCEPTION;
            }
        }

        // Verify: if target already has an installer package, it must
        // be signed with the same cert as the caller.
        if (!targetPackageSetting->mInstallerPackageName.IsNull()) {
            HashMap<String, AutoPtr<PackageSetting> >::Iterator sit = mSettings->mPackages.Find(
                    targetPackageSetting->mInstallerPackageName);
            AutoPtr<PackageSetting> setting;
            if (sit != mSettings->mPackages.End()) setting = sit->mSecond;
            // If the currently set package isn't valid, then it's always
            // okay to change it.
            if (setting != NULL) {
                if (CompareSignatures(callerSignature,
                        setting->mSignatures->mSignatures)
                        != IPackageManager::SIGNATURE_MATCH) {
                    Slogger::E(TAG, "Caller does not have same cert as old installer package %s",
                            targetPackageSetting->mInstallerPackageName.string());
                    return E_SECURITY_EXCEPTION;
                }
            }
        }

        // Okay!
        targetPackageSetting->mInstallerPackageName = installerPackageName;
        ScheduleWriteSettingsLocked();
    }
    return NOERROR;
}

ECode CPackageManagerService::ProcessPendingInstall(
    /* [in] */ InstallArgs* args,
    /* [in] */ Int32 currentStatus)
{
    // Queue up an async operation since the package installation may take a little while.
    AutoPtr<IRunnable> runnable = new ProcessPendingInstallRunnable(args, currentStatus, this);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

Int64 CPackageManagerService::CalculateDirectorySize(
    /* [in] */ IIMediaContainerService* mcs,
    /* [in] */ ArrayOf<IFile*>* paths)
{
    Int64 result = 0;
    for (Int32 i = 0; i < paths->GetLength(); ++i) {
        String absolutePath;
        (*paths)[i]->GetAbsolutePath(&absolutePath);
        Int64 value;
        mcs->CalculateDirectorySize(absolutePath, &value);
        result += value;
    }
    return result;
}

void CPackageManagerService::ClearDirectory(
    /* [in] */ IIMediaContainerService* mcs,
    /* [in] */ ArrayOf<IFile*>* paths)
{
    for (Int32 i = 0; i < paths->GetLength(); ++i) {
        // try {
        String absolutePath;
        (*paths)[i]->GetAbsolutePath(&absolutePath);
        mcs->ClearDirectory(absolutePath);
        // } catch (RemoteException e) {
        // }
    }
}

Boolean CPackageManagerService::InstallOnSd(
    /* [in] */ Int32 installFlags)
{
    if ((installFlags & IPackageManager::INSTALL_INTERNAL) != 0) {
        return FALSE;
    }
    if ((installFlags & IPackageManager::INSTALL_EXTERNAL) != 0) {
        return TRUE;
    }
    return FALSE;
}

Boolean CPackageManagerService::InstallForwardLocked(
    /* [in] */ Int32 installFlags)
{
    return (installFlags & IPackageManager::INSTALL_FORWARD_LOCK) != 0;
}

AutoPtr<InstallArgs> CPackageManagerService::CreateInstallArgs(
    /* [in] */ InstallParams* params)
{
    if (InstallOnSd(params->mInstallFlags) || params->IsForwardLocked()) {
        return new AsecInstallArgs(params, this);
    }
    else {
        return new FileInstallArgs(params, this);
    }
}

AutoPtr<InstallArgs> CPackageManagerService::CreateInstallArgsForExisting(
    /* [in] */ Int32 installFlags,
    /* [in] */ const String& codePath,
    /* [in] */ const String& resourcePath,
    /* [in] */ const String& nativeLibraryRoot,
    /* [in] */ ArrayOf<String>* instructionSets)
{
    Boolean isInAsec;
    String path;
    if (InstallOnSd(installFlags)) {
        /* Apps on SD card are always in ASEC containers. */
        isInAsec = TRUE;
    }
    else if (InstallForwardLocked(installFlags)
            && (mDrmAppPrivateInstallDir->GetAbsolutePath(&path), !codePath.StartWith(path))) {
        /*
         * Forward-locked apps are only in ASEC containers if they're the
         * new style
         */
        isInAsec = TRUE;
    }
    else {
        isInAsec = FALSE;
    }

    if (isInAsec) {
        return new AsecInstallArgs(codePath, instructionSets,
                InstallOnSd(installFlags), InstallForwardLocked(installFlags), this);
    }
    else {
        return new FileInstallArgs(codePath, resourcePath, nativeLibraryRoot,
                instructionSets, this);
    }
}

// Used by package mover
AutoPtr<InstallArgs>
CPackageManagerService::CreateInstallArgs(
    /* [in] */ IUri* packageURI,
    /* [in] */ Int32 flags,
    /* [in] */ const String& pkgName,
    /* [in] */ const String& dataDir)
{
    if (InstallOnSd(flags) || InstallForwardLocked(flags)) {
        String path;
        packageURI->GetPath(&path);
        String cid = GetNextCodePath(path, pkgName, String("/")
                + AsecInstallArgs::RES_FILE_NAME);
        return new AsecInstallArgs(packageURI, cid, InstallOnSd(flags),
                InstallForwardLocked(flags), this);
    }
    else {
        return new FileInstallArgs(packageURI, pkgName, dataDir, this);
    }
}

Boolean CPackageManagerService::IsAsecExternal(
    /* [in] */ const String& cid)
{
    String asecPath = PackageHelper::GetSdFilesystem(cid);
    return !asecPath.StartWith(mAsecInternalPath);
}

ECode CPackageManagerService::MaybeThrowExceptionForMultiArchCopy(
    /* [in] */ const String& message,
    /* [in] */ Int32 copyRet)
{
    if (copyRet < 0) {
        if (copyRet != IPackageManager::NO_NATIVE_LIBRARIES &&
                copyRet != IPackageManager::INSTALL_FAILED_NO_MATCHING_ABIS) {
            Slogger::E(TAG, "%s", message.string());
            mLastScanError = copyRet;
            return E_PACKAGE_MANAGER_EXCEPTION;
        }
    }
    return NOERROR;
}

String CPackageManagerService::CidFromCodePath(
    /* [in] */ const String& fullCodePath)
{
    Int32 eidx = fullCodePath.LastIndexOf("/");
    String subStr1 = fullCodePath.Substring(0, eidx);
    Int32 sidx = subStr1.LastIndexOf("/");
    return subStr1.Substring(sidx + 1, eidx);
}

String CPackageManagerService::GetAsecPackageName(
    /* [in] */ const String& packageCid)
{
    Int32 idx = packageCid.LastIndexOf("-");
    if (idx == -1) {
        return packageCid;
    }
    return packageCid.Substring(0, idx);
}

String CPackageManagerService::GetNextCodePath(
    /* [in] */ const String& oldCodePath,
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix)
{
    String idxStr("");
    Int32 idx = 1;
    // Fall back to default value of idx=1 if prefix is not
    // part of oldCodePath
    if (!oldCodePath.IsNull()) {
        String subStr = oldCodePath;
        // Drop the suffix right away
        if (subStr.EndWith(suffix)) {
            subStr = subStr.Substring(0, subStr.GetLength() - suffix.GetLength());
        }
        // If oldCodePath already contains prefix find out the
        // ending index to either increment or decrement.
        Int32 sidx = subStr.LastIndexOf(prefix);
        if (!suffix.IsNull() && subStr.EndWith(suffix)) {
            subStr = subStr.Substring(sidx + prefix.GetLength());
            if (!subStr.IsNull()) {
                if (subStr.StartWith(INSTALL_PACKAGE_SUFFIX)) {
                    subStr = subStr.Substring(INSTALL_PACKAGE_SUFFIX.GetLength());
                }
//                try {
                idx = StringUtils::ParseInt32(subStr);
                if (idx <= 1) {
                    idx++;
                }
                else {
                    idx--;
                }
//                } catch(NumberFormatException e) {
//                }
            }
        }
    }
    idxStr = INSTALL_PACKAGE_SUFFIX + StringUtils::Int32ToString(idx);
    return prefix + idxStr;
}

AutoPtr<IFile> CPackageManagerService::GetNextCodePath(
    /* [in] */ const String& packageName)
{
    Int32 suffix = 1;
    AutoPtr<IFile> result;
    Boolean exists;
    do {
        result = NULL;
        CFile::New(mAppInstallDir, packageName + "-" + StringUtils::ToString(suffix), (IFile**)&result);
        suffix++;
    } while (result->Exists(), exists);
    return result;
}

Boolean CPackageManagerService::IgnoreCodePath(
    /* [in] */ const String& fullPathStr)
{
    String apkName = DeriveCodePathName(fullPathStr);
    Int32 idx = apkName.LastIndexOf(INSTALL_PACKAGE_SUFFIX);
    if (idx != -1 && ((idx + 1) < apkName.GetLength())) {
        // Make sure the package ends with a numeral
        String version = apkName.Substring(idx + 1);
//         try {
        if (StringUtils::ParseInt32(version, 10, -1) != -1) return TRUE;
//         } catch (NumberFormatException e) {}
    }
    return FALSE;
}

String CPackageManagerService::DeriveCodePathName(
    /* [in] */ const String& codePath)
{
    if (codePath.IsNull()) {
        return String(NULL);
    }
    AutoPtr<IFile> codeFile;
    CFile::New(codePath, (IFile**)&codeFile);
    String name;
    codeFile->GetName(&name);
    Boolean isDirectory;
    if (codeFile->IsDirectory(&isDirectory), isDirectory) {
        return name;
    }
    else if (name.EndWith(".apk") || name.EndWith(".tmp")) {
        Int32 lastDot = name.LastIndexOf('.');
        return name.Substring(0, lastDot);
    }
    else {
        Slogger::W(TAG, "Odd, %s doesn't look like an APK", codePath.string());
        return String(NULL);
    }
}

void CPackageManagerService::InstallNewPackageLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& installerPackageName,
    /* [in] */ PackageInstalledInfo* res,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    // Remember this for later, in case we need to rollback this install
    String pkgName = pkg->mPackageName;

    if (DEBUG_INSTALL) Slogger::D(TAG, "installNewPackageLI: %p", pkg);
    Boolean dataDirExists;
    GetDataPathForPackage(pkg->mPackageName, 0)->Exists(&dataDirExists);
    synchronized(mPackagesLock) {
        if (mSettings->mRenamedPackages.Find(pkgName) != mSettings->mRenamedPackages.End()) {
            // A package with the same name is already installed, though
            // it has been renamed to an older name.  The package we
            // are trying to install should be installed as an update to
            // the existing one, but that has not been requested, so bail.
//             Slog.w(TAG, "Attempt to re-install " + pkgName
//                     + " without first uninstalling package running as "
//                     + mSettings.mRenamedPackages.get(pkgName));
            String value(NULL);
            HashMap<String, String>::Iterator it = mSettings->mRenamedPackages.Find(pkgName);
            if (it != mSettings->mRenamedPackages.End()) {
                value = it->mSecond;
            }
            res->SetError(IPackageManager::INSTALL_FAILED_ALREADY_EXISTS,
                    String("Attempt to re-install ") + pkgName
                    + " without first uninstalling package running as " + value);

            return;
        }
        if (mPackages.Find(pkgName) != mPackages.End()) {
            // Don't allow installation over an existing package with the same name.
            res->SetError(IPackageManager::INSTALL_FAILED_ALREADY_EXISTS, "Attempt to re-install " + pkgName
                    + " without first uninstalling.");
            return;
        }
    }

    // try {
    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int64 millis;
    sys->GetCurrentTimeMillis(&millis);
    AutoPtr<PackageParser::Package> newPackage;
    ECode ec = ScanPackageLI(pkg, parseFlags, scanFlags, millis, user, readBuffer,
            (PackageParser::Package**)&newPackage);
    if (FAILED(ec)) {
        res->SetError(mLastScanError, String("Package couldn't be installed in ") + pkg->mCodePath, ec);
    }

    UpdateSettingsLI(newPackage, installerPackageName, NULL, NULL, res);
    // delete the partially installed application. the data directory will have to be
    // restored if it was already existing
    if (res->mReturnCode != IPackageManager::INSTALL_SUCCEEDED) {
        // remove package from internal structures.  Note that we want deletePackageX to
        // delete the package data and cache directories that it created in
        // scanPackageLocked, unless those directories existed before we even tried to
        // install.
        DeletePackageLI(pkgName, IUserHandle::ALL, FALSE, NULL, NULL,
                dataDirExists ? IPackageManager::DELETE_KEEP_DATA : 0, res->mRemovedInfo, TRUE, readBuffer);
    }
    // } catch (PackageManagerException e) {
    //     res.setError("Package couldn't be installed in " + pkg.codePath, e);
    // }
}

Boolean CPackageManagerService::CheckUpgradeKeySetLP(
    /* [in] */ PackageSetting* oldPS,
    /* [in] */ PackageParser::Package* newPkg)
{
    // Upgrade keysets are being used.  Determine if new package has a superset of the
    // required keys.
    AutoPtr<ArrayOf<Int64> > upgradeKeySets = oldPS->mKeySetData->GetUpgradeKeySets();
    AutoPtr<KeySetManagerService> ksms = mSettings->mKeySetManagerService;
    for (Int32 i = 0; i < upgradeKeySets->GetLength(); i++) {
        AutoPtr<IArraySet> upgradeSet = ksms->GetPublicKeysFromKeySetLPr((*upgradeKeySets)[i]);
        Boolean containsAll;
        if (ISet::Probe(newPkg->mSigningKeys)->ContainsAll(ICollection::(upgradeSet),
                &containsAll), containsAll) {
            return TRUE;
        }
    }
    return FALSE;
}

void CPackageManagerService::ReplacePackageLI(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& installerPackageName,
    /* [in] */ PackageInstalledInfo* res,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    AutoPtr<PackageParser::Package> oldPackage;
    String pkgName = pkg->mPackageName;
    AutoPtr<ArrayOf<Int32> > allUsers;
    AutoPtr<ArrayOf<Boolean> > perUserInstalled;
    // First find the old package info and check signatures
    synchronized(mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it =
                mPackages.Find(pkgName);
        if (it != mPackages.End()) {
            oldPackage = it->mSecond;
        }
        if (DEBUG_INSTALL) Slogger::D(TAG, "replacePackageLI: new=%p, old=%p", pkg, oldPackage.Get());
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator pkgIt = mSettings->mPackages.Find(pkgName);
        if (pkgIt != mSettings->mPackages.End) {
            ps = it->mSecond;
        }
        if (ps == NULL || !ps->mKeySetData->IsUsingUpgradeKeySets() || ps->mSharedUser != NULL) {
            // default to original signature matching
            if (CompareSignatures(oldPackage->mSignatures, pkg->mSignatures)
                    != IPackageManager::SIGNATURE_MATCH) {
                res->SetError(IPackageManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES,
                        String("New package has a different signature: ") + pkgName);
                return;
            }
        }
        else {
            if(!CheckUpgradeKeySetLP(ps, pkg)) {
                res->SetError(IPackageManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES,
                        String("New package not signed by keys specified by upgrade-keysets: ")
                        + pkgName);
                return;
            }
        }

        // In case of rollback, remember per-user/profile install state
        allUsers = sUserManager->GetUserIds();
        perUserInstalled = ArrayOf<Boolean>::Alloc(allUsers->GetLength());
        for (Int32 i = 0; i < allUsers->GetLength(); i++) {
            (*perUserInstalled)[i] = ps != NULL ? ps->GetInstalled((*allUsers)[i]) : FALSE;

        }
    }
    Boolean sysPkg = IsSystemApp(oldPackage);
    if (sysPkg) {
        ReplaceSystemPackageLI(oldPackage, pkg, parseFlags, scanFlags,
                user, allUsers, perUserInstalled, installerPackageName, res, readBuffer);
    }
    else {
        ReplaceNonSystemPackageLI(oldPackage, pkg, parseFlags, scanFlags,
                user, allUsers, perUserInstalled, installerPackageName, res, readBuffer);
    }
}

void CPackageManagerService::ReplaceNonSystemPackageLI(
    /* [in] */ PackageParser::Package* deletedPackage,
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ IUserHandle* user,
    /* [in] */ ArrayOf<Int32>* allUsers,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ const String& installerPackageName,
    /* [in] */ PackageInstalledInfo* res,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    AutoPtr<PackageParser::Package> newPackage;
    String pkgName = deletedPackage->mPackageName;
    Boolean deletedPkg = TRUE;
    Boolean updatedSettings = FALSE;

    if (DEBUG_INSTALL) Slogger::D(TAG, "replaceNonSystemPackageLI: new=%p, old=%p", pkg, deletedPackage);
    Int64 origUpdateTime;
    if (pkg->mExtras != NULL) {
        AutoPtr<PackageSetting> extras = reinterpret_cast<PackageSetting*>(pkg->mExtras->Probe(EIID_PackageSetting));
        origUpdateTime = extras->mLastUpdateTime;
    }
    else {
        origUpdateTime = 0;
    }

    // First delete the existing package while retaining the data directory
    if (!DeletePackageLI(pkgName, NULL, TRUE, NULL, NULL, IPackageManager::DELETE_KEEP_DATA,
            res->mRemovedInfo, TRUE, readBuffer)) {
        // If the existing package wasn't successfully deleted
        res->SetError(IPackageManager::INSTALL_FAILED_REPLACE_COULDNT_DELETE,
                String("replaceNonSystemPackageLI"));
        deletedPkg = FALSE;
    }
    else {
        // Successfully deleted the old package; proceed with replace.

        // If deleted package lived in a container, give users a chance to
        // relinquish resources before killing.
        if (IsForwardLocked(deletedPackage) || IsExternal(deletedPackage)) {
            if (DEBUG_INSTALL) {
                Slogger::I(TAG, "upgrading pkg %p is ASEC-hosted -> UNAVAILABLE", deletedPackage);
            }
            AutoPtr<ArrayOf<Int32> > uidArray = ArrayOf<Int32>::Alloc(1);
            deletedPackage->mApplicationInfo->GetUid(&(*uidArray)[0]);
            List<String> pkgList;
            String pkgName;
            deletedPackage->mApplicationInfo->GetPackageName(&pkgName);
            pkgList.PushBack(pkgName);
            SendResourcesChangedBroadcast(FALSE, TRUE, pkgList, uidArray, NULL);
        }

        DeleteCodeCacheDirsLI(pkgName);
        // try {
        AutoPtr<ISystem> sys;
        CSystem::AcquireSingleton((ISystem**)&sys);
        Int64 millis;
        sys->GetCurrentTimeMillis(&millis);
        AutoPtr<PackageParser::Package> newPackage;
        ECode ec = ScanPackageLI(pkg, parseFlags, scanFlags | SCAN_UPDATE_TIME,
                millis, user, readBuffer, (PackageParser::Package**)&newPackage);
        if (FAILED(ec)) {
            res->SetError(mLastScanError, String("Package couldn't be installed in ") + pkg->mCodePath, ec);
        }
        else {
            UpdateSettingsLI(newPackage, installerPackageName, allUsers, perUserInstalled, res);
            UpdatedSettings = true;
        }
        // } catch (PackageManagerException e) {
        //     res.setError("Package couldn't be installed in " + pkg.codePath, e);
        // }
    }

    if (res->mReturnCode != IPackageManager::INSTALL_SUCCEEDED) {
        // remove package from internal structures.  Note that we want deletePackageX to
        // delete the package data and cache directories that it created in
        // scanPackageLocked, unless those directories existed before we even tried to
        // install.
        if(updatedSettings) {
            if (DEBUG_INSTALL) Slogger::D(TAG, "Install failed, rolling pack: %s", pkgName.string());
            DeletePackageLI(pkgName, NULL, TRUE, allUsers, perUserInstalled, IPackageManager::DELETE_KEEP_DATA,
                    res->mRemovedInfo, TRUE, readBuffer);
        }
        // Since we failed to install the new package we need to restore the old
        // package that we deleted.
        if(deletedPkg) {
            if (DEBUG_INSTALL) Slogger::D(TAG, "Install failed, reinstalling: %p", deletedPackage);
            AutoPtr<IFile> restoreFile;
            CFile::New(deletedPackage->mCodePath, (IFile**)&restoreFile);
            // Parse old package
            Boolean oldOnSd = IsExternal(deletedPackage);
            Int32 oldParseFlags  = mDefParseFlags | PackageParser::PARSE_CHATTY |
                    (IsForwardLocked(deletedPackage) ? PackageParser::PARSE_FORWARD_LOCK : 0) |
                    (oldOnSd ? PackageParser::PARSE_ON_SDCARD : 0);
            Int32 oldScanFlags = SCAN_UPDATE_SIGNATURE | SCAN_UPDATE_TIME;
            // try {
            AutoPtr<PackageParser::Package> pkg;
            if (FAILED(ScanPackageLI(restoreFile, oldParseFlags, oldScanFlags, origUpdateTime,
                    NULL, readBuffer, (PackageParser::Package**)&pkg))) {
                Slogger::E(TAG, "Failed to restore package : %s after failed upgrade: ", pkgName.string());
                return;
            }
            // } catch (PackageManagerException e) {
            //     Slog.e(TAG, "Failed to restore package : " + pkgName + " after failed upgrade: "
            //             + e.getMessage());
            //     return;
            // }
            // Restore of old package succeeded. Update permissions.
            // writer
            synchronized (mPackagesLock) {
                UpdatePermissionsLPw(deletedPackage->mPackageName, deletedPackage,
                        UPDATE_PERMISSIONS_ALL);
                // can downgrade to reader
                mSettings->WriteLPr();
            }
            Slogger::I(TAG, "Successfully restored package : %s after failed upgrade", pkgName.string());
        }
    }
}

void CPackageManagerService::ReplaceSystemPackageLI(
    /* [in] */ PackageParser::Package* deletedPackage,
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanFlags,
    /* [in] */ IUserHandle* user,
    /* [in] */ ArrayOf<Int32>* allUsers,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ const String& installerPackageName,
    /* [in] */ PackageInstalledInfo* res,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    if (DEBUG_INSTALL) Slogger::D(TAG, "replaceSystemPackageLI: new=%p, old=%p", pkg, deletedPackage);
    Boolean disabledSystem = FALSE;
    Boolean updatedSettings = FALSE;
    parseFlags |= PackageParser::PARSE_IS_SYSTEM;
    Int32 flags;
    if (deletedPackage->mApplicationInfo->GetFlags(&flags), (flags & IApplicationInfo::FLAG_PRIVILEGED) != 0) {
        parseFlags |= PackageParser::PARSE_IS_PRIVILEGED;
    }
    String packageName = deletedPackage->mPackageName;
    if (packageName.IsNull()) {
        res->SetError(IPackageManager::INSTALL_FAILED_REPLACE_COULDNT_DELETE,
                String("Attempt to delete null packageName."));
        return;
    }
    AutoPtr<PackageParser::Package> oldPkg;
    AutoPtr<PackageSetting> oldPkgSetting;
    // reader
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            oldPkg = it->mSecond;
        }
        HashMap<String, AutoPtr<PackageSetting> >::Iterator pit =
                mSettings->mPackages.Find(packageName);
        if (pit != mSettings->mPackages.End()) {
            oldPkgSetting = pit->mSecond;
        }
        if((oldPkg == NULL) || (oldPkg->mApplicationInfo == NULL) ||
                (oldPkgSetting == NULL)) {
            res->SetError(IPackageManager::INSTALL_FAILED_REPLACE_COULDNT_DELETE,
                    String("Couldn't find package:") + packageName + " information");
            return;
        }
    }

    Int32 oldPkgUid;
    oldPkg->mApplicationInfo->GetUid(&oldPkgUid);
    KillApplication(packageName, oldPkgUid, String("replace sys pkg"));

    res->mRemovedInfo->mUid = oldPkgUid;
    res->mRemovedInfo->mRemovedPackage = packageName;
    // Remove existing system package
    RemovePackageLI(oldPkgSetting, TRUE);
    // writer
    synchronized (mPackagesLock) {
        disabledSystem = mSettings->DisableSystemPackageLPw(packageName);
        if (!disabledSystem && deletedPackage != NULL) {
            // We didn't need to disable the .apk as a current system package,
            // which means we are replacing another update that is already
            // installed.  We need to make sure to delete the older one's .apk.
            String codePath, resourcePath, natLibDir;
            deletedPackage->mApplicationInfo->GetCodePath(&codePath);
            deletedPackage->mApplicationInfo->GetResourcePath(&resourcePath);
            deletedPackage->mApplicationInfo->GetNativeLibraryRootDir(&natLibDir);
            res->mRemovedInfo->mArgs = CreateInstallArgsForExisting(0, codePath, resourcePath,
                    natLibDir,
            GetAppDexInstructionSets(deletedPackage->mApplicationInfo));
        }
        else {
            res->mRemovedInfo->mArgs = NULL;
        }
    }

    // Successfully disabled the old package. Now proceed with re-installation
    DeleteCodeCacheDirsLI(packageName);
    mLastScanError = IPackageManager::INSTALL_SUCCEEDED;

    res->mReturnCode = IPackageManager::INSTALL_SUCCEEDED;
    Int32 pkgFlags;
    pkg->mApplicationInfo->GetFlags(&pkgFlags);
    pkgFlags |= IApplicationInfo::FLAG_UPDATED_SYSTEM_APP;
    pkg->mApplicationInfo->SetFlags(pkgFlags);

    AutoPtr<PackageParser::Package> newPackage;
    // try {
    ECode ec = ScanPackageLI(pkg, parseFlags, scanFlags, 0, user, readBuffer, (PackageParser::Package**)&newPackage);
    if (FAILED(ec)) {
        res->SetError(mLastScanError, String("Package couldn't be installed in ") + pkg->mCodePath, ec);
    }
    else {
        if (newPackage->mExtras != NULL) {
            AutoPtr<PackageSetting> newPkgSetting = reinterpret_cast<PackageSetting*>(newPackage->mExtras->Probe(EIID_PackageSetting));
            newPkgSetting->mFirstInstallTime = oldPkgSetting->mFirstInstallTime;
            AutoPtr<ISystem> sys;
            CSystem::AcquireSingleton((ISystem**)&sys);
            sys->GetCurrentTimeMillis(&newPkgSetting->mLastUpdateTime);

            // is the update attempting to change shared user? that isn't going to work...
            if (oldPkgSetting->mSharedUser != newPkgSetting->mSharedUser) {
                String str1, str2;
                oldPkgSetting->mSharedUser->ToString(&str1);
                newPkgSetting->mSharedUser->ToString(&str2);
                res->SetError(IPackageManager::INSTALL_FAILED_SHARED_USER_INCOMPATIBLE,
                        String("Forbidding shared user change from ") + str1
                        + " to " + str2);
                updatedSettings = TRUE;
            }
        }
    }

    if (res->mReturnCode == IPackageManager::INSTALL_SUCCEEDED) {
        UpdateSettingsLI(newPackage, installerPackageName, allUsers, perUserInstalled, res);
        UpdatedSettings = TRUE;
    }
    // } catch (PackageManagerException e) {
    //     res.setError("Package couldn't be installed in " + pkg.codePath, e);
    // }

    if (res->mReturnCode != IPackageManager::INSTALL_SUCCEEDED) {
        // Re installation failed. Restore old information
        // Remove new pkg information
        if (newPackage != NULL) {
            RemoveInstalledPackageLI(newPackage, TRUE);
        }
        // Add back the old system package
        // try {
        newPackage = NULL;
        if (FAILED(ScanPackageLI(oldPkg, parseFlags, SCAN_UPDATE_SIGNATURE, 0, user,
                readBuffer, (PackageParser::Package**)&newPackage))) {
            Slogger::E(TAG, "Failed to restore original package: ");
        }
        // } catch (PackageManagerException e) {
        //     Slog.e(TAG, "Failed to restore original package: " + e.getMessage());
        // }
        // Restore the old system information in Settings
        synchronized (mPackagesLock) {
            if (disabledSystem) {
                mSettings->EnableSystemPackageLPw(packageName);
            }
            if (updatedSettings) {
                mSettings->SetInstallerPackageName(packageName, oldPkgSetting->mInstallerPackageName);
            }
            mSettings->WriteLPr();
        }
    }
}

// for epk
Int32 CPackageManagerService::MoveEcoFilesLI(
    /* [in] */ PackageParser::Package* newPackage)
{
    AutoPtr<INativeLibraryHelper> nlHelper;
    CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&nlHelper);
    Int32 retCode;
    nlHelper->CopyEcoLI(newPackage->mPath, newPackage->mPackageName + ".eco", String("/data/elastos"), &retCode);
    if (retCode != 1) {
        Slogger::E(TAG, "eco file doesn't exist, skipping move: %s", newPackage->mPath.string());
        return IPackageManager::INSTALL_FAILED_INVALID_APK;
    }

    return IPackageManager::INSTALL_SUCCEEDED;
}

void CPackageManagerService::UpdateSettingsLI(
    /* [in] */ PackageParser::Package* newPackage,
    /* [in] */ const String& installerPackageName,
    /* [in] */ ArrayOf<Int32>* allUsers,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ PackageInstalledInfo* res)
{
    String pkgName = newPackage->mPackageName;
    synchronized (mPackagesLock) {
        //write settings. the installStatus will be incomplete at this stage.
        //note that the new package setting would have already been
        //added to mPackages. It hasn't been persisted yet.
        mSettings->SetInstallStatus(pkgName, PackageSettingBase::PKG_INSTALL_INCOMPLETE);
        mSettings->WriteLPr();
    }

    if (DEBUG_INSTALL) Slogger::D(TAG, "New package installed in %s", newPackage->mCodePath.string());

    synchronized (mPackagesLock) {
        UpdatePermissionsLPw(newPackage->mPackageName, newPackage,
                UPDATE_PERMISSIONS_REPLACE_PKG |
                    (newPackage->mPermissions.Begin() != newPackage->mPermissions.End() ? UPDATE_PERMISSIONS_ALL : 0));
        // For system-bundled packages, we assume that installing an upgraded version
        // of the package implies that the user actually wants to run that new code,
        // so we enable the package.
        if (IsSystemApp(newPackage)) {
            // NB: implicit assumption that system package upgrades apply to all users
            if (DEBUG_INSTALL) {
                Slogger::D(TAG, "Implicitly enabling system package on upgrade: %s", pkgName.string());
            }
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings.mPackages.Find(pkgName);
            if (it != mSettings.mPackages.End()) {
                ps = it->mSecond
            }
            if (ps != NULL) {
                if (res->mOrigUsers != NULL) {
                    for (Int32 i = 0; i < res->mOrigUsers->GetLength(); ++i) {
                        ps->SetEnabled(COMPONENT_ENABLED_STATE_DEFAULT,
                                (*res->mOrigUsers)[i], installerPackageName);
                    }
                }
                // Also convey the prior install/uninstall state
                if (allUsers != NULL && perUserInstalled != NULL) {
                    for (Int32 i = 0; i < allUsers->GetLength(); i++) {
                        if (DEBUG_INSTALL) {
                            Slogger::D(TAG, "    user %d => %d", (*allUsers)[i], (*perUserInstalled)[i]);
                        }
                        ps->SetInstalled((*perUserInstalled)[i], (*allUsers)[i]);
                    }
                    // these install state changes will be persisted in the
                    // upcoming call to mSettings.writeLPr().
                }
            }
        }

        res->mName = pkgName;
        newPackage->mApplicationInfo->GetUid(&res->mUid);
        res->mPkg = newPackage;
        mSettings->SetInstallStatus(pkgName, PackageSettingBase::PKG_INSTALL_COMPLETE);
        mSettings->SetInstallerPackageName(pkgName, installerPackageName);
        res->mReturnCode = IPackageManager::INSTALL_SUCCEEDED;
        //to update install status
        mSettings->WriteLPr();
    }
}

void CPackageManagerService::InstallPackageLI(
    /* [in] */ InstallArgs* args,
    /* [in] */ PackageInstalledInfo* res,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    Int32 installFlags = args->mInstallFlags;
    String installerPackageName = args->mInstallerPackageName;
    AutoPtr<IFile> tmpPackageFile;
    CFile::New(args->GetCodePath(), (IFile**)&tmpPackageFile);
    Boolean forwardLocked = ((installFlags & IPackageManager::INSTALL_FORWARD_LOCK) != 0);
    Boolean onSd = ((installFlags & IPackageManager::INSTALL_EXTERNAL) != 0);
    Boolean replace = FALSE;
    Int32 scanFlags = SCAN_NEW_INSTALL | SCAN_FORCE_DEX | SCAN_UPDATE_SIGNATURE;
    // Result object to be returned
    res->mReturnCode = IPackageManager::INSTALL_SUCCEEDED;

    if (DEBUG_INSTALL) Slogger::D(TAG, "installPackageLI: path=%p", tmpPackageFile.Get());
    // Retrieve PackageSettings and parse package
    Int32 parseFlags = mDefParseFlags | PackageParser::PARSE_CHATTY |
            (forwardLocked ? PackageParser::PARSE_FORWARD_LOCK : 0) |
            (onSd ? PackageParser::PARSE_ON_SDCARD : 0);
    AutoPtr<PackageParser> pp = new PackageParser();
    pp->SetSeparateProcesses(mSeparateProcesses);
    pp->SetDisplayMetrics(mMetrics);

    AutoPtr<PackageParser::Package> pkg;
    // try {
    ECode ec = pp->ParsePackage(tmpPackageFile, parseFlags, (PackageParser::Package**)&pkg);
    if (FAILED(ec)) {
        res->SetError(pp->GetParseError(), String("Failed parse during installPackageLI"), ec);
        return;
    }
    // } catch (PackageParserException e) {
    //     res.setError("Failed parse during installPackageLI", e);
    //     return;
    // }

    // Mark that we have an install time CPU ABI override.
    pkg->mCpuAbiOverride = args->mAbiOverride;

    String pkgName = res->mName = pkg->mPackageName;
    Int32 pkgFlags;
    pkg->mApplicationInfo->GetFlags(&pkgFlags);
    if ((pkgFlags & IApplicationInfo::FLAG_TEST_ONLY) != 0) {
        if ((installFlags & IPackageManager::INSTALL_ALLOW_TEST) == 0) {
            res->SetError(IPackageManager::INSTALL_FAILED_TEST_ONLY, String("installPackageLI"));
            return;
        }
    }

    // try {
    if (FAILED(pp->CollectCertificates(pkg, parseFlags, readBuffer))) {
        res->SetError(pp->GetParseError(), String("Failed collect during installPackageLI"), ec);
        return;
    }

    if (FAILED(pp->CollectManifestDigest(pkg))) {
        res->SetError(pp->GetParseError(), String("Failed collect during installPackageLI"), ec);
        return;
    }
    // } catch (PackageParserException e) {
    //     res.setError("Failed collect during installPackageLI", e);
    //     return;
    // }

    if (GET_CERTIFICATES && FAILED(pp->CollectCertificates(pkg, parseFlags, readBuffer))) {
        res->mReturnCode = pp->GetParseError();
        return;
    }
    /* If the installer passed in a manifest digest, compare it now. */
    if (args->mManifestDigest != NULL) {
//         if (DEBUG_INSTALL) {
//             final String parsedManifest = pkg.manifestDigest == null ? "null"
//                     : pkg.manifestDigest.toString();
//             Slog.d(TAG, "Comparing manifests: " + args.manifestDigest.toString() + " vs. "
//                     + parsedManifest);
//         }

        Boolean isEqual;
        if (args->mManifestDigest->Equals(pkg->mManifestDigest, &isEqual), !isEqual) {
            res->SetError(IPackageManager::INSTALL_FAILED_PACKAGE_CHANGED, String("Manifest digest changed"));
            return;
        }
    }
//        else if (DEBUG_INSTALL) {
//         final String parsedManifest = pkg.manifestDigest == null
//                 ? "null" : pkg.manifestDigest.toString();
//         Slog.d(TAG, "manifestDigest was not present, but parser got: " + parsedManifest);
//     }

    // Get rid of all references to package scan path via parser.
    pp = NULL;
    String oldCodePath;
    Boolean systemApp = FALSE;
    synchronized (mPackagesLock) {
        // Check whether the newly-scanned package wants to define an already-defined perm
        List< AutoPtr<Permission> >::ReverseIterator permRIt = pkg->mPermissions.RBegin();
        while (permRIt != pkg->mPermissions.REnd()) {
            AutoPtr<PackageParser::Permission> perm = *permRIt;
            AutoPtr<BasePermission> bp;
            String infoName;
            perm->mInfo->GetName(&infoName);
            HashMap<String, AutoPtr<BasePermission> >::Iterator permIt = mSettings->mPermissions.Find(infoName);
            if (permIt != mSettings->mPermissions.End()) {
                bp = permIt->mSecond;
            }
            if (bp != NULL) {
                // If the defining package is signed with our cert, it's okay.  This
                // also includes the "updating the same package" case, of course.
                // "updating same package" could also involve key-rotation.
                Boolean sigsOk;
                if (!bp->mSourcePackage.Equals(pkg->mPackageName)
                        || (bp->mPackageSetting->Probe(EIID_PackageSetting) == NULL)
                        || !bp->mPackageSetting->mKeySetData->IsUsingUpgradeKeySets()
                        || ((PackageSetting*)bp->mPackageSetting)->mSharedUser != NULL) {
                    sigsOk = CompareSignatures(bp->mPackageSetting->mSignatures->mSignatures,
                            pkg->mSignatures) == IPackageManager::SIGNATURE_MATCH;
                }
                else {
                    sigsOk = CheckUpgradeKeySetLP((PackageSetting*)bp->mPackageSetting, pkg);
                }
                if (!sigsOk) {
                    // If the owning package is the system itself, we log but allow
                    // install to proceed; we fail the install on all other permission
                    // redefinitions.
                    if (!bp->mSourcePackage.Equals("android")) {
                        res->SetError(IPackageManager::INSTALL_FAILED_DUPLICATE_PERMISSION,
                                String("Package "), pkg->mPackageName + " attempting to redeclare permission "
                                + infoName + " already owned by " + bp->mSourcePackage);
                        res->mOrigPermission = infoName;
                        res->mOrigPackage = bp->mSourcePackage;
                        return;
                    }
                    else {
                        Slogger::W(TAG, "Package %s attempting to redeclare system permission %s; ignoring new declaration",
                                pkg->mPackageName.string(), infoName.string());
                        permRIt = List< AutoPtr<Permission> >::ReverseIterator(
                                pkg->mPermissions.Erase(--(permRIt.GetBase())));
                        continue;
                    }
                }
            }
            ++permRIt;
        }

        // Check if installing already existing package
        if ((installFlags & IPackageManager::INSTALL_REPLACE_EXISTING) != 0) {
            String oldName;
            HashMap<String, String>::Iterator it = mSettings->mRenamedPackages.Find(pkgName);
            if (it != mSettings->mRenamedPackages.End()) {
                oldName = it->mSecond;
            }
            if (pkg->mOriginalPackages != NULL
                    && Find(pkg->mOriginalPackages->Begin(),
                        pkg->mOriginalPackages->End(), oldName)
                        != pkg->mOriginalPackages->End()
                    && mPackages.Find(oldName) != mPackages.End()) {
                // This package is derived from an original package,
                // and this device has been updating from that original
                // name.  We must continue using the original name, so
                // rename the new package here.
                pkg->SetPackageName(oldName);
                pkgName = pkg->mPackageName;
                replace = TRUE;
                if (DEBUG_INSTALL) Slogger::D(TAG, "Replacing existing renamed package: oldName=%s pkgName=%s"
                        oldName.string(), pkgName.string());
            }
            else if (mPackages.Find(pkgName) != mPackages.End()) {
                // This package, under its official name, already exists
                // on the device; we should replace it.
                replace = TRUE;
            }
        }

        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it =
                mSettings->mPackages.Find(pkgName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        if (ps != NULL) {
            if (DEBUG_INSTALL) Slogger::D(TAG, "Existing package: %p", ps.Get());
            oldCodePath = ps->mCodePathString;
            if (ps->mPkg != NULL && ps->mPkg->mApplicationInfo != NULL) {
                Int32 flags;
                ps->mPkg->mApplicationInfo->GetFlags(&flags);
                systemApp = (flags & IApplicationInfo::FLAG_SYSTEM) != 0;
            }
            res->mOrigUsers = ps->QueryInstalledUsers(*sUserManager->GetUserIds(), TRUE);
        }
    }

    if (systemApp && onSd) {
        // Disable updates to system apps on sdcard
        res->SetError(IPackageManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION,
                String("Cannot install updates to system apps on sdcard"));
        return;
    }

    if (!args->DoRename(res->mReturnCode, pkgName, oldCodePath)) {
        res->SetError(IPackageManager::INSTALL_FAILED_INSUFFICIENT_STORAGE, String("Failed rename"));
        return;
    }

    // Set application objects path explicitly after the rename
    SetApplicationInfoPaths(pkg, args->GetCodePath(), args->GetResourcePath());
    pkg->mApplicationInfo->SetNativeLibraryDir(args->GetNativeLibraryPath());
    if (replace) {
        ReplacePackageLI(pkg, parseFlags, scanFlags | SCAN_REPLACING, args->mUser,
                installerPackageName, res, readBuffer);
    }
    else {
        InstallNewPackageLI(pkg, parseFlags, scanFlags | SCAN_DELETE_DATA_ON_FAILURES,
                args->mUser, installerPackageName, res, readBuffer);
    }

    synchronized (mPackagesLock) {
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(pkgName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        if (ps != NULL) {
            res->mNewUsers = ps->QueryInstalledUsers(*sUserManager->GetUserIds(), TRUE);
        }
    }
}

Boolean CPackageManagerService::IsForwardLocked(
    /* [in] */ PackageParser::Package* pkg)
{
    Int32 flags;
    pkg->mApplicationInfo->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_FORWARD_LOCK) != 0;
}

Boolean CPackageManagerService::IsForwardLocked(
    /* [in] */ IApplicationInfo* pkg)
{
    Int32 flags;
    info->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_FORWARD_LOCK) != 0;
}

Boolean CPackageManagerService::IsForwardLocked(
    /* [in] */ PackageSetting* ps)
{
    return (ps->mPkgFlags & IApplicationInfo::FLAG_FORWARD_LOCK) != 0;
}

Boolean CPackageManagerService::IsMultiArch(
    /* [in] */ PackageSetting* info)
{
    return (ps->mPkgFlags & IApplicationInfo::FLAG_MULTIARCH) != 0;
}

Boolean CPackageManagerService::IsMultiArch(
    /* [in] */ IApplicationInfo* info)
{
    Int32 flags;
    info->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_MULTIARCH) != 0;
}

Boolean CPackageManagerService::IsExternal(
    /* [in] */ PackageParser::Package* pkg)
{
    Int32 flags;
    pkg->mApplicationInfo->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_EXTERNAL_STORAGE) != 0;
}

Boolean CPackageManagerService::IsExternal(
    /* [in] */ PackageSetting* ps)
{
    return (ps->mPkgFlags & IApplicationInfo::FLAG_EXTERNAL_STORAGE) != 0;
}

Boolean CPackageManagerService::IsExternal(
    /* [in] */ IApplicationInfo* info)
{
    Int32 flags;
    info->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_EXTERNAL_STORAGE) != 0;
}

Boolean CPackageManagerService::IsSystemApp(
    /* [in] */ PackageParser::Package* pkg)
{
    Int32 flags;
    pkg->mApplicationInfo->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_SYSTEM) != 0;
}

Boolean CPackageManagerService::IsPrivilegedApp(
    /* [in] */ PackageParser::Package* pkg)
{
    Int32 flags;
    pkg->mApplicationInfo->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_PRIVILEGED) != 0;
}

Boolean CPackageManagerService::IsSystemApp(
    /* [in] */ IApplicationInfo* info)
{
    Int32 flags;
    info->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_SYSTEM) != 0;
}

Boolean CPackageManagerService::IsSystemApp(
    /* [in] */ PackageSetting* ps)
{
    return (ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0;
}

Boolean CPackageManagerService::IsSystemApp(
    /* [in] */ PackageSetting* ps)
{
    return (ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0;
}

Boolean CPackageManagerService::IsUpdatedSystemApp(
    /* [in] */ PackageSetting* ps)
{
    return (ps->mPkgFlags & IApplicationInfo::FLAG_UPDATED_SYSTEM_APP) != 0;
}

Boolean CPackageManagerService::IsUpdatedSystemApp(
    /* [in] */ PackageParser::Package* pkg)
{
    Int32 flags;
    pkg->mApplicationInfo->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_UPDATED_SYSTEM_APP) != 0;
}

Boolean CPackageManagerService::IsUpdatedSystemApp(
    /* [in] */ IApplicationInfo* info)
{
    Int32 flags;
    info->GetFlags(&flags);
    return (flags & IApplicationInfo::FLAG_UPDATED_SYSTEM_APP) != 0;
}

Boolean CPackageManagerService::IsVendorApp(
    /* [in] */ PackageParser::Package* pkg)
{
    return !IsUpdatedSystemApp(pkg) && pkg->mCodePath.StartWith(VENDOR_APP_DIR);
}

Int32 CPackageManagerService::PackageFlagsToInstallFlags(
    /* [in] */ PackageSetting* ps)
{
    Int32 installFlags = 0;
    if (IsExternal(ps)) {
        installFlags |= IPackageManager::INSTALL_EXTERNAL;
    }
    if (IsForwardLocked(ps)) {
        installFlags |= IPackageManager::INSTALL_FORWARD_LOCK;
    }
    return installFlags;
}

void CPackageManagerService::DeleteTempPackageFiles()
{
    AutoPtr<IFilenameFilter> filter = new DeleteFilenameFilter();

    DeleteTempPackageFilesInDirectory(mAppInstallDir, filter);
    DeleteTempPackageFilesInDirectory(mDrmAppPrivateInstallDir, filter);
    AutoPtr< ArrayOf<IFile*> > files;
    mDrmAppPrivateInstallDir->ListFiles((ArrayOf<IFile*>**)&files);
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        (*files)[i]->Delete();
    }
}

ECode CPackageManagerService::DeletePackageAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ IIPackageDeleteObserver* observer,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 flags)
{
    AutoPtr<IPackageInstallObserver> deleteObserver;
    CLegacyPackageDeleteObserver::New(observer, (IPackageInstallObserver**)&deleteObserver);
    return DeletePackage(packageName, deleteObserver, userId, flags);
}

ECode CPackageManagerService::DeletePackage(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDeleteObserver* observer,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 flags)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::DELETE_PACKAGES, String(NULL)))
    Int32 uid = Binder::GetCallingUid();
    if (UserHandle::GetUserId(uid) != userId) {
        FAIL_RETURN(mContext->EnforceCallingPermission(
                Elastos::Core::Manifest::permission::INTERACT_ACROSS_USERS_FULL,
                String("deletePackage for user ") + StringUtils::ToString(userId)))
    }
    if (IsUserRestricted(userId, IUserManager::DISALLOW_UNINSTALL_APPS)) {
        // try {
        return observer->OnPackageDeleted(packageName,
                IPackageManager::DELETE_FAILED_USER_RESTRICTED, String(NULL));
        // } catch (RemoteException re) {
        // }
    }

    Boolean uninstallBlocked = FALSE;
    if ((flags & IPackageManager::DELETE_ALL_USERS) != 0) {
        AutoPtr< ArrayOf<Int32> > users = sUserManager->GetUserIds();
        for (Int32 i = 0; i < users->GetLength(); ++i) {
            if (GetBlockUninstallForUser(packageName, (*users)[i])) {
                uninstallBlocked = TRUE;
                break;
            }
        }
    }
    else {
        uninstallBlocked = GetBlockUninstallForUser(packageName, userId);
    }
    if (uninstallBlocked) {
        // try {
        return observer->OnPackageDeleted(packageName, IPackageManager::DELETE_FAILED_OWNER_BLOCKED,
                    String(NULL));
        // } catch (RemoteException re) {
        // }
    }

    if (DEBUG_REMOVE) {
        Slogger::D(TAG, "deletePackageAsUser: pkg=%s user=%d", packageName.string(), userId);
    }
    // Queue up an async operation since the package deletion may take a little while.
    AutoPtr<IRunnable> runnable = new DeletePackageRunnable(packageName, observer, flags, userId, this);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

Boolean CPackageManagerService::IsPackageDeviceAdmin(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId)
{
    AutoPtr<IIDevicePolicyManager> dpm = IIDevicePolicyManager::Probe(
            ServiceManager::GetService(IContext::DEVICE_POLICY_SERVICE));
    // try {
    if (dpm != NULL) {
        Boolean isDeviceOwner;
        if (FAILED(dpm->IsDeviceOwner(&isDeviceOwner))) {
            return FALSE;
        }
        if (isDeviceOwner) {
            return TRUE;
        }
        AutoPtr< ArrayOf<Int32> > users;
        if (userId == IUserHandle::USER_ALL) {
            users = sUserManager->GetUserIds();
        }
        else {
            users = ArrayOf<Int32>::Alloc(1);
            (*users)[0] = userId;
        }
        for (Int32 i = 0; i < users->GetLength(); ++i) {
            Boolean result;
            if (FAILED(dpm->PackageHasActiveAdmins(&result))) {
                return FALSE;
            }
            if (result) {
                return TRUE;
            }
        }
    }
    // } catch (RemoteException e) {
    // }
    return FALSE;
}

Int32 CPackageManagerService::DeletePackageX(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 flags)
{
    AutoPtr<PackageRemovedInfo> info = new PackageRemovedInfo(this);
    Boolean res = FALSE;

    AutoPtr<IUserHandle> removeForUser;
    if ((flags & IPackageManager::DELETE_ALL_USERS) != 0) {
        removeForUser = UserHandle::ALL;
    }
    else {
        CUserHandle::New(userId, (IUserHandle**)&removeForUser);
    }

    Boolean removedForAllUsers = FALSE;
    Boolean systemUpdate = FALSE;
    // for the uninstall-updates case and restricted profiles, remember the per-
    // userhandle installed state
    AutoPtr< ArrayOf<Int32> > allUsers;
    AutoPtr< ArrayOf<Boolean> > perUserInstalled;
    synchronized (mPackagesLock) {
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        allUsers = sUserManager->GetUserIds();
        perUserInstalled = ArrayOf<Boolean>::Alloc(allUsers->GetLength());
        for (Int32 i = 0; i < allUsers->GetLength(); i++) {
            (*perUserInstalled)[i] = ps != NULL ? ps->GetInstalled((*allUsers)[i]) : FALSE;
        }
    }

    synchronized (mInstallLock) {
        if (DEBUG_REMOVE) Slogger::D(TAG, "deletePackageX: pkg=%s user=%d", packageName.string(), userId);

        AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
        if (readBuffer == NULL) {
            Slogger::E(TAG, "Failed delete package: out of memory!");
            return IPackageManager::DELETE_FAILED_INTERNAL_ERROR;
        }
        res = DeletePackageLI(packageName, removeForUser, TRUE, allUsers, perUserInstalled,
                flags | REMOVE_CHATTY, info, TRUE, readBuffer);
        systemUpdate = info->mIsRemovedPackageSystemUpdate;
        if (res && !systemUpdate && (mPackages.Find(packageName) == mPackages.End())) {
            removedForAllUsers = TRUE;
        }
        if (DEBUG_REMOVE) Slogger::D(TAG, "delete res: systemUpdate=%d removedForAllUsers=%d",
                systemUpdate, removedForAllUsers);
    }

    if (res) {
        info->SendBroadcast(TRUE, systemUpdate, removedForAllUsers);

        // If the removed capsule was a system update, the old system capsuled
        // was re-enabled; we need to broadcast this information
        if (systemUpdate) {
            AutoPtr<IBundle> extras;
            ASSERT_SUCCEEDED(CBundle::New(1, (IBundle**)&extras));
            extras->PutInt32(IIntent::EXTRA_UID, info->mRemovedAppId >= 0
                    ? info->mRemovedAppId : info->mUid);
            extras->PutBoolean(IIntent::EXTRA_REPLACING, TRUE);

            SendPackageBroadcast(IIntent::ACTION_PACKAGE_ADDED, packageName,
                    extras, String(NULL), NULL, NULL);
            SendPackageBroadcast(IIntent::ACTION_PACKAGE_REPLACED, packageName,
                    extras, String(NULL), NULL, NULL);
            SendPackageBroadcast(IIntent::ACTION_MY_PACKAGE_REPLACED, String(NULL),
                    NULL, packageName, NULL, NULL);
        }
    }
    // Force a gc here.
    // Runtime.getRuntime().gc();
    // Delete the resources here after sending the broadcast to let
    // other processes clean up before deleting resources.
    if (info->mArgs != NULL) {
        synchronized (mInstallLock) {
            info->mArgs->DoPostDeleteLI(TRUE);
        }
    }
    return res ? IPackageManager::DELETE_SUCCEEDED : IPackageManager::DELETE_FAILED_INTERNAL_ERROR;
}

void CPackageManagerService::RemovePackageDataLI(
    /* [in] */ PackageSetting* ps,
    /* [in] */ ArrayOf<Int32>* allUserHandles,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ PackageRemovedInfo* outInfo,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean writeSettings)
{
    String packageName = ps->mName;
    if (DEBUG_REMOVE) Slogger::D(TAG, "removePackageDataLI: %p", ps);
    RemovePackageLI(ps, (flags & REMOVE_CHATTY) != 0);
    // Retrieve object to delete permissions for shared user later on
    AutoPtr<PackageSetting> deletedPs;
    // reader
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator itor
                = mSettings->mPackages.Find(packageName);
        if (itor != mSettings->mPackages.End()) {
            deletedPs = itor->mSecond;
        }
        if (outInfo != NULL) {
            outInfo->mRemovedPackage = packageName;
            outInfo->mRemovedUsers = deletedPs != NULL
                    ? deletedPs->QueryInstalledUsers(*sUserManager->GetUserIds(), TRUE)
                    : NULL;
        }
    }
    if ((flags & IPackageManager::DELETE_KEEP_DATA) == 0) {
        RemoveDataDirsLI(packageName);
        SchedulePackageCleaning(packageName, IUserHandle::USER_ALL, TRUE);
    }
    // writer
    synchronized (mPackagesLock) {
        if (deletedPs != NULL) {
            if ((flags & IPackageManager::DELETE_KEEP_DATA) == 0) {
                if (outInfo != NULL) {
                    mSettings->mKeySetManagerService->RemoveAppKeySetDataLPw(packageName);
                    outInfo->mRemovedAppId = mSettings->RemovePackageLPw(packageName);
                }
                if (deletedPs != NULL) {
                    UpdatePermissionsLPw(deletedPs->mName, NULL, 0);
                    if (deletedPs->mSharedUser != NULL) {
                        // remove permissions associated with package
                        mSettings->UpdateSharedUserPermsLPw(deletedPs, mGlobalGids);
                    }
                }
                ClearPackagePreferredActivitiesLPw(deletedPs->mName, IUserHandle::USER_ALL);
            }
            // make sure to preserve per-user disabled state if this removal was just
            // a downgrade of a system app to the factory package
            if (allUserHandles != NULL && perUserInstalled != NULL) {
                if (DEBUG_REMOVE) {
                    Slogger::D(TAG, "Propagating install state across downgrade");
                }
                for (Int32 i = 0; i < allUserHandles->GetLength(); i++) {
                    if (DEBUG_REMOVE) {
                        Slogger::D(TAG, "    user %d => %d", (*allUserHandles)[i], (*perUserInstalled)[i]);
                    }
                    ps->SetInstalled((*perUserInstalled)[i], (*allUserHandles)[i]);
                }
            }
        }
        // can downgrade to reader
        if (writeSettings) {
            // Save settings now
            mSettings->WriteLPr();
        }
    }
    if (outInfo != NULL) {
        // A user ID was deleted here. Go through all users and remove it
        // from KeyStore.
        RemoveKeystoreDataIfNeeded(IUserHandle::USER_ALL, outInfo->mRemovedAppId);
    }
}

Boolean CPackageManagerService::LocationIsPrivileged(
    /* [in] */ IFile* path)
{
    // try {
    AutoPtr<IFile> rootDir = Environment::GetRootDirectory();
    AutoPtr<IFile> f;
    CFile::New(rootDir, String("priv-app"), (IFile**)&f);
    String privilegedAppDir;
    f->GetCanonicalPath(&privilegedAppDir);
    String canonicalPath;
    path->GetCanonicalPath(&canonicalPath);
    return canonicalPath.StartWith(privilegedAppDir);
    // } catch (IOException e) {
    //     Slog.e(TAG, "Unable to access code path " + path);
    // }
    // return false;
}

Boolean CPackageManagerService::DeleteSystemPackageLI(
    /* [in] */ PackageSetting* newPs,
    /* [in] */ ArrayOf<Int32>* allUserHandles,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ Int32 flags,
    /* [in] */ PackageRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    Boolean applyUserRestrictions
                = (allUserHandles != NULL) && (perUserInstalled != NULL);
    AutoPtr<PackageSetting> disabledPs;
    // Confirm if the system package has been updated
    // An updated system app can be deleted. This will also have to restore
    // the system pkg from system partition
    // reader
    synchronized (mPackagesLock) {
        disabledPs = mSettings->GetDisabledSystemPkgLPr(newPs->mName);
    }
    if (DEBUG_REMOVE) Slogger::D(TAG, "deleteSystemPackageLI: newPs=%p disabledPs=%p", newPs, disabledPs.Get());
    if (disabledPs == NULL) {
        Slogger::W(TAG, "Attempt to delete unknown system package %s", newPs->mName.string());
        return FALSE;
    }
    else if (DEBUG_REMOVE) {
        Slogger::D(TAG, "Deleting system pkg from data partition");
    }
    if (DEBUG_REMOVE) {
        if (applyUserRestrictions) {
            Slogger::D(TAG, "Remembering install states:");
            for (Int32 i = 0; i < allUserHandles->GetLength(); i++) {
                Slogger::D(TAG, "   u=%d inst=%d", (*allUserHandles)[i], (*perUserInstalled)[i]);
            }
        }
    }
    // Delete the updated package
    outInfo->mIsRemovedPackageSystemUpdate = TRUE;
    if (disabledPs->mVersionCode < newPs->mVersionCode) {
        // Delete data for downgrades
        flags &= ~IPackageManager::DELETE_KEEP_DATA;
    }
    else {
        // Preserve data by setting flag
        flags |= IPackageManager::DELETE_KEEP_DATA;
    }
    Boolean ret = DeleteInstalledPackageLI(newPs, TRUE, flags,
            allUserHandles, perUserInstalled, outInfo, writeSettings);
    if (!ret) {
        return FALSE;
    }
    // writer
    synchronized (mPackagesLock) {
        // Reinstate the old system package
        mSettings->EnableSystemPackageLPw(newPs->mName);
        // Remove any native libraries from the upgraded package.
        AutoPtr<INativeLibraryHelper> helper;
        CNativeLibraryHelper::AcquireSingleton((INativeLibraryHelper**)&helper);
        Boolean removed;
        helper->RemoveNativeBinariesLI(newPs->mLegacyNativeLibraryPathString, &removed);
    }
    // Install the system package
    if (DEBUG_REMOVE) Slogger::D(TAG, "Re-installing system package: %p", disabledPs.Get());
    Int32 parseFlags = PackageParser::PARSE_MUST_BE_APK | PackageParser::PARSE_IS_SYSTEM;
    if (LocationIsPrivileged(disabledPs->mCodePath)) {
        parseFlags |= PackageParser::PARSE_IS_PRIVILEGED;
    }

    AutoPtr<PackageParser::Package> newPkg;
    // try {
    if (FAILED(scanPackageLI(disabledPs->mCodePath, parseFlags,
            SCAN_NO_PATHS, 0, String(NULL), readBuffer, (PackageParser::Package**)&newPkg))) {
        Slogger::W(TAG, "Failed to restore system package:%s", newPs->mName);
        return FALSE;
    }
    // } catch (PackageManagerException e) {
    //     Slog.w(TAG, "Failed to restore system package:" + newPs.name + ": " + e.getMessage());
    //     return false;
    // }

    // writer
    synchronized (mPackagesLock) {
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(newPkg->mPackageName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        UpdatePermissionsLPw(newPkg->mPackageName, newPkg,
                UPDATE_PERMISSIONS_ALL | UPDATE_PERMISSIONS_REPLACE_PKG);
        if (applyUserRestrictions) {
            if (DEBUG_REMOVE) {
                Slogger::D(TAG, "Propagating install state across reinstall");
            }
            for (Int32 i = 0; i < allUserHandles->GetLength(); i++) {
                if (DEBUG_REMOVE) {
                    Slogger::D(TAG, "    user %d => %d", (*allUserHandles)[i], (*perUserInstalled)[i]);
                }
                ps->SetInstalled((*perUserInstalled)[i], (*allUserHandles)[i]);
            }
            // Regardless of writeSettings we need to ensure that this restriction
            // state propagation is persisted
            mSettings->WriteAllUsersPackageRestrictionsLPr();
        }
        // can downgrade to reader here
        if (writeSettings) {
            mSettings->WriteLPr();
        }
    }
    return TRUE;
}

Boolean CPackageManagerService::DeleteInstalledPackageLI(
    /* [in] */ PackageSetting* ps,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Int32>* allUserHandles,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ PackageRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    if (outInfo != NULL) {
        outInfo->mUid = ps->mAppId;
    }

    // Delete package data from Int32ernal structures and also remove data if flag is set
    RemovePackageDataLI(ps, allUserHandles, perUserInstalled, outInfo, flags, writeSettings);

    // Delete application code and resources
    if (deleteCodeAndResources && (outInfo != NULL)) {
        // TODO can pick up from CapsuleSettings as well
        outInfo->mArgs = CreateInstallArgsForExisting(PackageFlagsToInstallFlags(ps),
                ps->mCodePathString, ps->mResourcePathString, ps->mLegacyNativeLibraryPathString,
                GetAppDexInstructionSets(ps));
        if (DEBUG_SD_INSTALL) Slogger::I(TAG, "args=%p", outInfo->mArgs.Get());
    }
    return TRUE;
}

ECode CPackageManagerService::SetBlockUninstallForUser(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean blockUninstall,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Core::Manifest::permission::DELETE_PACKAGES, String(NULL)))
    synchronized (mPackagesLock) {
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        if (ps == NULL) {
            Logger::I(TAG, "Package doesn't exist in set block uninstall %s", packageName.string());
            return NOERROR;
        }
        if (!ps->GetInstalled(userId)) {
            // Can't block uninstall for an app that is not installed or enabled.
            Logger::I(TAG, "Package not installed in set block uninstall %s", packageName.string());
            return NOERROR;
        }
        ps->SetBlockUninstall(blockUninstall, userId);
        mSettings->WritePackageRestrictionsLPr(userId);
    }
    *result = TRUE;
    return NOERROR;
}

ECode CPackageManagerService::GetBlockUninstallForUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    synchronized (mPackagesLock) {
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        if (ps == NULL) {
            Logger::I(TAG, "Package doesn't exist in get block uninstall %s", packageName.string());
            *result = FALSE;
            return NOERROR
        }
        *result = ps->GetBlockUninstall(userId);
    }
    return NOERROR;
}

Boolean CPackageManagerService::DeletePackageLI(
    /* [in] */ const String& packageName,
    /* [in] */ IUserHandle* user,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ ArrayOf<Int32>* allUserHandles,
    /* [in] */ ArrayOf<Boolean>* perUserInstalled,
    /* [in] */ Int32 flags,
    /* [in] */ PackageRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    if (packageName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete null packageName.");
        return FALSE;
    }
    if (DEBUG_REMOVE) Slogger::D(TAG, "deletePackageLI: %s user %p", packageName.string(), user);
    AutoPtr<PackageSetting> ps;
    Boolean dataOnly = FALSE;
    Int32 removeUser = -1;
    Int32 appId = -1;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(packageName);
        if (it != mSettings->mPackages.End()) {
            ps = it->mSecond;
        }
        if (ps == NULL) {
            Slogger::W(TAG, "Package named '%s' doesn't exist.", packageName.string());
            return FALSE;
        }
        Int32 id;
        if ((!IsSystemApp(ps) || (flags & IPackageManager::DELETE_SYSTEM_APP) != 0) && user != NULL
                && (user->GetIdentifier(&id), id != IUserHandle::USER_ALL) {
            // The caller is asking that the package only be deleted for a single
            // user.  To do this, we just mark its uninstalled state and delete
            // its data.  If this is a system app, we only allow this to happen if
            // they have set the special DELETE_SYSTEM_APP which requests different
            // semantics than normal for uninstalling system apps.
            if (DEBUG_REMOVE) Slogger::D(TAG, "Only deleting for single user");
            ps->SetUserState(id, IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT,
                    FALSE, //installed
                    TRUE,  //stopped
                    TRUE,  //notLaunched
                    FALSE, //hidden
                    String(NULL), NULL, NULL,
                    FALSE // blockUninstall
                    );
            if (!IsSystemApp(ps)) {
                AutoPtr< ArrayOf<Int32> > userIds = sUserManager->GetUserIds();
                if (ps->IsAnyInstalled(userIds)) {
                    // Other user still have this package installed, so all
                    // we need to do is clear this user's data and save that
                    // it is uninstalled.
                    if (DEBUG_REMOVE) Slogger::D(TAG, "Still installed by other users");
                    user->GetIdentifier(&removeUser);
                    appId = ps->mAppId;
                    mSettings->WritePackageRestrictionsLPr(removeUser);
                }
                else {
                    // We need to set it back to 'installed' so the uninstall
                    // broadcasts will be sent correctly.
                    if (DEBUG_REMOVE) Slogger::D(TAG, "Not installed by other users, full delete");
                    user->GetIdentifier(&id);
                    ps->SetInstalled(TRUE, id);
                }
            }
            else {
                // This is a system app, so we assume that the
                // other users still have this package installed, so all
                // we need to do is clear this user's data and save that
                // it is uninstalled.
                if (DEBUG_REMOVE) Slogger::D(TAG, "Deleting system app");
                user->GetIdentifier(&removeUser);
                appId = ps->mAppId;
                mSettings->WritePackageRestrictionsLPr(removeUser);
            }
        }
    }

    if (removeUser >= 0) {
        // From above, we determined that we are deleting this only
        // for a single user.  Continue the work here.
        if (DEBUG_REMOVE) Slogger::D(TAG, "Updating install state for user: %d", removeUser);
        if (outInfo != NULL) {
            outInfo->mRemovedPackage = packageName;
            outInfo->mRemovedAppId = appId;
            outInfo->mRemovedUsers = ArrayOf<Int32>::Alloc(1);
            (*outInfo->mRemovedUsers)[0] = removeUser;
        }
        mInstaller->ClearUserData(packageName, removeUser);
        RemoveKeystoreDataIfNeeded(removeUser, appId);
        SchedulePackageCleaning(packageName, removeUser, FALSE);
        return TRUE;
    }

    if (dataOnly) {
        // Delete application data first
        if (DEBUG_REMOVE) Slogger::D(TAG, "Removing package data only");
        RemovePackageDataLI(ps, NULL, NULL, outInfo, flags, writeSettings);
        return TRUE;
    }
    Boolean ret = FALSE;
    if (IsSystemApp(ps)) {
        if (DEBUG_REMOVE) Slogger::D(TAG, "Removing system package:%s", ps->mName.string());
        // When an updated system application is deleted we delete the existing resources as well and
        // fall back to existing code in system partition
        ret = DeleteSystemPackageLI(ps, allUserHandles, perUserInstalled, flags, outInfo, writeSettings, readBuffer);
    }
    else {
        if (DEBUG_REMOVE) Slogger::D(TAG, "Removing non-system package:%s", ps->mName.string());
        // Kill application pre-emptively especially for apps on sd.
        KillApplication(packageName, ps->mAppId, String("uninstall pkg"));
        ret = DeleteInstalledPackageLI(ps, deleteCodeAndResources, flags,
                allUserHandles, perUserInstalled, outInfo, writeSettings);
    }
    return ret;
}

void CPackageManagerService::ClearExternalStorageDataSync(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean allData)
{
    Boolean mounted = FALSE;
    if (Environment::IsExternalStorageEmulated()) {
        mounted = true;
    }
    else {
        String status = Environment::GetExternalStorageState();
        mounted = (IEnvironment::MEDIA_MOUNTED.Equals(status)
                || IEnvironment::MEDIA_MOUNTED_READ_ONLY.Equals(status));
    }

    if (!mounted) {
        return;
    }

    AutoPtr<IIntent> containerIntent;
    CIntent::New((IIntent**)&containerIntent);
    containerIntent->SetComponent(DEFAULT_CONTAINER_COMPONENT);

    AutoPtr<ArrayOf<Int32> > users;
    if (userId == IUserHandle::USER_ALL) {
        users = sUserManager->GetUserIds();
    }
    else {
        users = ArrayOf<Int32>::Alloc(1);
        (*users)[0] = userId;
    }

    AutoPtr<ClearStorageConnection> conn = new ClearStorageConnection();
    Boolean res = FALSE;
    mContext->BindService(containerIntent, conn, IContext::BIND_AUTO_CREATE, IUserHandle::USER_OWNER, &res);
    if (res) {
        // try {
        for (Int32 i = 0; i < users->GetLength(); i++) {
            Int32 curUser = (*users)[i];
            Int64 timeout = SystemClock::GetUptimeMillis() + 5000;
            synchronized (conn) {
                Int64 now = SystemClock::GetUptimeMillis();
                while (conn->mContainerService == NULL && now < timeout) {
                    // try {
                    conn->Wait(timeout - now);
                    // } catch (InterruptedException e) {
                    // }
                }
            }

            if (conn->mContainerService == NULL) {
                break;
            }

            AutoPtr<UserEnvironment> userEnv = new Environment::UserEnvironment(curUser);
            AutoPtr<IFile> dir = userEnv->BuildExternalStorageAppCacheDirs(packageName);
            ClearDirectory(conn->mContainerService, dir);
            if (allData) {
                dir = userEnv->BuildExternalStorageAppDataDirs(packageName);
                ClearDirectory(conn->mContainerService, dir);
                dir = userEnv->BuildExternalStorageAppMediaDirs(packageName);
                ClearDirectory(conn->mContainerService, dir);
            }
        }
        // } finally {
        mContext->UnbindService(conn);
        // }
    }
}

ECode CPackageManagerService::ClearApplicationUserData(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDataObserver* observer,
    /* [in] */ Int32 userId)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CLEAR_APP_USER_DATA, String(NULL)))

    FAIL_RETURN(EnforceCrossUserPermission(Binder::GetCallingUid(),
            userId, TRUE, FALSE, String("clear application data")))

    // Queue up an async operation since the package deletion may take a little while.
    AutoPtr<IRunnable> runnable = new ClearRunnable(this, packageName, observer, userId);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

Boolean CPackageManagerService::ClearApplicationUserDataLI(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId)
{
    if (packageName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete null packageName.");
        return FALSE;
    }

    // Try finding details about the requested package
    AutoPtr<PackageParser::Package> pkg;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg == NULL) {
            AutoPtr<PackageSetting> ps;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator settingIt = mSettings->mPackages.Find(packageName);
            if (settingIt != mSettings->mPackages.End()) {
                ps = it->mSecond;
            }
            if (ps != NULL) {
                pkg = ps->mPkg;
            }
        }
    }
    if (pkg == NULL) {
        Slogger::W(TAG, "Package named '%s' doesn't exist.", packageName.string());
    }

    // Always delete data directories for package, even if we found no other
    // record of app. This helps users recover from UID mismatches without
    // resorting to a full data wipe.
    Int32 retCode = mInstaller->ClearUserData(packageName, userId);
    if (retCode < 0) {
        Slogger::W(TAG, "Couldn't remove cache files for package: %s", packageName.string());
        return FALSE;
    }

    if (pkg == NULL) {
        return FALSE;
    }

    if (pkg != NULL && pkg->mApplicationInfo != NULL) {
        Int32 appId;
        pkg->mApplicationInfo->GetUid(&appId);
        RemoveKeystoreDataIfNeeded(userId, appId);
    }

    // Create a native library symlink only if we have native libraries
    // and if the native libraries are 32 bit libraries. We do not provide
    // this symlink for 64 bit libraries.
    String primaryCpuAbi;
    if (pkg != NULL && (pkg->mApplicationInfo->GetPrimaryCpuAbi(&primaryCpuAbi), !primaryCpuAbi.IsNull())
            /*&& !VMRuntime.is64BitAbi(pkg.applicationInfo.primaryCpuAbi)*/) {
        String nativeLibPath;
        pkg->mApplicationInfo->GetNativeLibraryDir(&nativeLibPath);
        if (mInstaller->LinkNativeLibraryDirectory(pkg->mPackageName, nativeLibPath, userId) < 0) {
            Slogger::W(TAG, "Failed linking native library dir");
            return FALSE;
        }
    }

    return TRUE;
}

void CPackageManagerService::RemoveKeystoreDataIfNeeded(
    /* [in] */ Int32 userId,
    /* [in] */ Int32 appId)
{
    if (appId < 0) {
        return;
    }

    AutoPtr<IKeyStoreHelper> helper;
    CKeyStoreHelper::AcquireSingleton((IKeyStoreHelper**)&helper);
    AutoPtr<IKeyStore> keyStore;
    helper->GetInstance((IKeyStore**)&keyStore);
    if (keyStore != NULL) {
        if (userId == IUserHandle::USER_ALL) {
            AutoPtr<ArrayOf<Int32> > userIds = sUserManager->GetUserIds();
            for (Int32 i = 0; i < userIds->GetLength(); ++i) {
                keyStore->ClearUid(UserHandle::GetUid((*userIds)[i], appId));
            }
        }
        else {
            keyStore->ClearUid(UserHandle::GetUid(userId, appId));
        }
    }
    else {
        Slogger::W(TAG, "Could not contact keystore to clear entries for app id %d", appId);
    }
}

ECode CPackageManagerService::DeleteApplicationCacheFiles(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDataObserver* observer)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::DELETE_CACHE_FILES, String(NULL)))

    // Queue up an async operation since the package deletion may take a little while.
    Int32 userId = UserHandle::GetCallingUserId();
    AutoPtr<IRunnable> runnable = new DeleteRunnable(this, packageName, observer, userId);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

Boolean CPackageManagerService::DeleteApplicationCacheFilesLI(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId)
{
    if (packageName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete null packageName.");
        return FALSE;
    }
    AutoPtr<PackageParser::Package> p;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            p = it->mSecond;
        }
    }
    if (p == NULL) {
        Slogger::W(TAG, "Package named '%s' doesn't exist.", packageName.string());
        return FALSE;
    }
    if (p->mApplicationInfo == NULL) {
        Slogger::W(TAG, "Package " + packageName + " has no applicationInfo.");
        return FALSE;
    }
    Int32 retCode = mInstaller->DeleteCacheFiles(packageName, userId);
    if (retCode < 0) {
        Slogger::W(TAG, "Couldn't remove cache files for package: %s u%d", packageName.string(), userId);
        return FALSE;
    }
    return TRUE;
}

ECode CPackageManagerService::GetPackageSizeInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userHandle,
    /* [in] */ IPackageStatsObserver* observer)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::GET_PACKAGE_SIZE, String(NULL)))
    if (packageName.IsNull()) {
        Slogger::E(TAG, "Attempt to get size of null packageName");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IPackageStats> stats;
    CPackageStats::New(packageName, userHandle, (IPackageStats**)&stats);

    /*
     * Queue up an async operation since the package measurement may take a
     * little while.
     */
    Int32 uh;
    stats->GetUserHandle(&uh);
    AutoPtr<IUserHandle> newUserHandle;
    CUserHandle::New(uh, (IUserHandle**)&newUserHandle);
    AutoPtr<MeasureParams> measureParams = new MeasureParams(stats, newUserHandle, observer, this);

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(INIT_COPY, (IMessage**)&msg);
    msg->SetObj(measureParams);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

Boolean CPackageManagerService::GetPackageSizeInfoLI(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userHandle,
    /* [in] */ IPackageStats* pStats)
{
    if (packageName.IsNull()) {
        Slogger::W(TAG, "Attempt to get size of null packageName.");
        return FALSE;
    }
    AutoPtr<PackageParser::Package> p;
    Boolean dataOnly = FALSE;
    String libDirRoot(NULL);
    String asecPath(NULL);
    AutoPtr<PackageSetting> ps;
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            p = it->mSecond;
        }
        HashMap<String, AutoPtr<PackageSetting> >::Iterator settingIt = mSettings->mPackages.Find(packageName);
        if (settingIt != mSettings->mPackages.End()) {

        }
        if (p == NULL) {
            dataOnly = TRUE;
            if ((ps == NULL) || (ps->mPkg == NULL)) {
                Slogger::W(TAG, "Package named '%s' doesn't exist.", packageName.string());
                return FALSE;
            }
            p = ps->mPkg;
        }
        if (ps != null) {
            libDirRoot = ps->mLegacyNativeLibraryPathString;
        }
        if (p != NULL && (IsExternal(p) || IsForwardLocked(p))) {
            String codePath;
            p->mApplicationInfo->GetBaseCodePath(&codePath);
            String secureContainerId = CidFromCodePath(codePath);
            String srcDir;
            p->mApplicationInfo->GetSourceDir(&srcDir);
            String secureContainerId = CidFromCodePath(srcDir);
            if (!secureContainerId.IsNull()) {
                asecPath = PackageHelper::GetSdFilesystem(secureContainerId);
            }
        }
    }
    // TODO: extend to measure size of split APKs
    // TODO(multiArch): Extend getSizeInfo to look at the full subdirectory tree,
    // not just the first level.
    // TODO(multiArch): Extend getSizeInfo to look at *all* instruction sets, not
    // just the primary.
    AutoPtr< ArrayOf<String> > sets = GetAppDexInstructionSets(ps);
    AutoPtr< ArrayOf<String> > dexCodeInstructionSets = GetDexCodeInstructionSets(sets);
    Int32 res = mInstaller->GetSizeInfo(packageName, userHandle, p->mBaseCodePath, libDirRoot,
            publicSrcDir, asecPath, dexCodeInstructionSets, pStats);
    if (res < 0) {
        return FALSE;
    }

    // Fix-up for forward-locked applications in ASEC containers.
    if (!IsExternal(p)) {
        Int64 codeSize, extCodeSize;
        pStats->GetCodeSize(&codeSize);
        pStats->GetExternalCodeSize(&extCodeSize);
        codeSize += extCodeSize;
        pStats->SetCodeSize(codeSize);
        pStats->SetExternalCodeSize(0);
    }

    return TRUE;
}

ECode CPackageManagerService::AddPackageToPreferred(
    /* [in] */ const String& packageName)
{
    Slogger::W(TAG, "addPackageToPreferred: this is now a no-op");
    return NOERROR;
}

ECode CPackageManagerService::RemovePackageFromPreferred(
    /* [in] */ const String& packageName)
{
    Slogger::W(TAG, "removePackageFromPreferred: this is now a no-op");
    return NOERROR;
}

ECode CPackageManagerService::GetPreferredPackages(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    return CParcelableObjectContainer::New(infos);
}

Int32 CPackageManagerService::GetUidTargetSdkVersionLockedLPr(
    /* [in] */ Int32 uid)
{
    AutoPtr<IInterface> obj = mSettings->GetUserIdLPr(uid);
    if (obj->Probe(EIID_SharedUserSetting) != NULL) {
        SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(obj->Probe(EIID_SharedUserSetting));
        Int32 vers = Build::VERSION_CODES::CUR_DEVELOPMENT;
        HashSet<PackageSetting*>::Iterator it;
        for (it = sus->mPackages.Begin(); it != sus->mPackages.End(); ++it) {
            AutoPtr<PackageSetting> ps = *it;
            if (ps->mPkg != NULL) {
                Int32 v;
                ps->mPkg->mApplicationInfo->GetTargetSdkVersion(&v);
                if (v < vers) vers = v;
            }
        }
        return vers;
    }
    else if (obj->Probe(EIID_PackageSetting) != NULL) {
        PackageSetting* ps = reinterpret_cast<PackageSetting*>(obj->Probe(EIID_PackageSetting));
        if (ps->mPkg != NULL) {
            Int32 version;
            ps->mPkg->mApplicationInfo->GetTargetSdkVersion(&version);
            return version;
        }
    }
    return Build::VERSION_CODES::CUR_DEVELOPMENT;
}

ECode CPackageManagerService::AddPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity,
    /* [in] */ Int32 userId)
{
    return AddPreferredActivityInternal(filter, match, set, activity, TRUE, userId,
            String("Adding preferred"));
}

ECode CPackageManagerService::AddPreferredActivityInternal(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity,
    /* [in] */ Boolean always,
    /* [in] */ Int32 userId,
    /* [in] */ const String& opname)
{
    // writer
    Int32 callingUid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(callingUid, userId, TRUE, FALSE, String("add preferred activity")))
    Int32 count;
    if (filter->CountActions(&count), count == 0) {
        Slogger::W(TAG, "Cannot set a preferred activity with no filter actions");
        return NOERROR;
    }

    synchronized (mPackages) {
        Int32 perm;
        FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
                Elastos::Droid::Manifest::permission::SET_PREFERRED_APPLICATIONS, &perm)))
        if (perm != IPackageManager::PERMISSION_GRANTED) {
            if (GetUidTargetSdkVersionLockedLPr(callingUid) < Build::VERSION_CODES::FROYO) {
                Slogger::W(TAG, "Ignoring addPreferredActivity() from uid %d", callingUid);
                return NOERROR;
            }
            FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
                Elastos::Droid::Manifest::permission::SET_PREFERRED_APPLICATIONS, String(NULL)))
        }

        AutoPtr<PreferredIntentResolver> pir = mSettings->EditPreferredActivitiesLPw(userId);
        String str;
        activity->FlattenToShortString(&str)
        Slogger::I(TAG, "%s activity %s for user %d:", opname.string(), str.string(), userId);
        // filter.dump(new LogPrinter(Log.INFO, TAG), "  ");
        mSettings->EditPreferredActivitiesLPw(userId)->AddFilter(
                new PreferredActivity(filter, match, set, activity, always));
        mSettings->WritePackageRestrictionsLPr(userId);
    }

    return NOERROR;
}

ECode CPackageManagerService::ReplacePreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity)
{
    Int32 count = 0;
    if (filter->CountActions(&count), count != 1) {
        Slogger::E(TAG, "replacePreferredActivity expects filter to have only 1 action.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 countA = 0, countP = 0, countS = 0, countT = 0;
    if ((filter->CountDataAuthorities(&countA), countA != 0)
            || (filter->CountDataPaths(&countP), countP != 0)
            || (filter->CountDataSchemes(&countS), countS > 1)
            || (filter->CountDataTypes(&countT), countT != 0)) {
        Slogger::E(TAG, "replacePreferredActivity expects filter to have no data authorities, paths, or types; and at most one scheme.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 callingUid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(callingUid, userId, TRUE, FALSE,
            String("replace preferred activity")))
    synchronized (mPackagesLock) {
        Int32 result = 0;
        FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
                Elastos::Droid::Manifest::permission::SET_PREFERRED_APPLICATIONS, &result));
        if (result != IPackageManager::PERMISSION_GRANTED) {
            if (GetUidTargetSdkVersionLockedLPr(callingUid)
                    < Build::VERSION_CODES::FROYO) {
                Slogger::W(TAG, "Ignoring replacePreferredActivity() from uid %d",
                            Binder::GetCallingUid());
                return NOERROR;
            }
            FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
                    Elastos::Droid::Manifest::permission::SET_PREFERRED_APPLICATIONS, String(NULL)));
        }

        AutoPtr<PreferredIntentResolver> pir;
        HashMap<Int32, AutoPtr<PreferredIntentResolver> >::Iterator aIt = mSettings->mPreferredActivities.Find(userId);
        if (aIt != mSettings->mPreferredActivities.End()) {
            pir = aIt->mSecond;
        }
        if (pir != NULL) {
            // Get all of the existing entries that exactly match this filter.
            AutoPtr<ArrayOf<PreferredActivity*> > existing = pir->FindFilters(filter);
            if (existing != NULL && existing->GetLength() == 1) {
                AutoPtr<PreferredActivity> cur = (*existing)[0];
                if (DEBUG_PREFERRED) {
                    Slogger::I(TAG, "Checking replace of preferred:");
                    // filter.dump(new LogPrinter(Log.INFO, TAG), "  ");
                    if (!cur->mPref->mAlways) {
                        Slogger::I(TAG, "  -- CUR; not mAlways!");
                    }
                    else {
                        Slogger::I(TAG, "  -- CUR: mMatch=%d", cur->mPref->mMatch);
                        Slogger::I(TAG, "  -- CUR: mSet=%s", Arrays::ToString(cur->mPref->mSetComponents).string());
                        Slogger::I(TAG, "  -- CUR: mComponent=%s", cur->mPref->mShortComponent.string());
                        Slogger::I(TAG, "  -- NEW: mMatch=%d", (match & IIntentFilter::MATCH_CATEGORY_MASK));
                        Slogger::I(TAG, "  -- CUR: mSet=%s", Arrays::ToString(set));
                        String str;
                        activity->FlattenToShortString(&str);
                        Slogger::I(TAG, "  -- CUR: mComponent=%s", str.string());
                    }
                }
                Boolean equals;
                if (cur->mPref->mAlways &&
                        (IObject::Probe(cur->mPref->mComponent)->Equals(activity, &equals), equals)
                        && cur->mPref->mMatch == (match & IIntentFilter::MATCH_CATEGORY_MASK)
                        && cur->mPref->SameSet(set)) {
                    // Setting the preferred activity to what it happens to be already
                    if (DEBUG_PREFERRED) {
                        Slogger::I(TAG, "Replacing with same preferred activity %s for user %d:",
                                cur->mPref->mShortComponent.string(), userId);
                        // filter.dump(new LogPrinter(Log.INFO, TAG), "  ");
                    }
                    return NOERROR;
                }
            }

            if (existing != NULL) {
                if (DEBUG_PREFERRED) {
                    Slogger::I(TAG, "%d existing preferred matches for:", existing->GetLength());
                    // filter.dump(new LogPrinter(Log.INFO, TAG), "  ");
                }
                for (Int32 i = 0; i < existing->GetLength(); i++) {
                    AutoPtr<PreferredActivity> pa = (*existing)[i];
                    if (DEBUG_PREFERRED) {
                        Slogger::I(TAG, "Removing existing preferred activity %p:", pa->mPref->mComponent.Get());
                        // pa.dump(new LogPrinter(Log.INFO, TAG), "  ");
                    }
                    pir->RemoveFilter(pa);
                }
            }
        }
        return AddPreferredActivityInternal(filter, match, set, activity, TRUE, userId,
                String("Replacing preferred"));
    }

    return NOERROR;
}

ECode CPackageManagerService::ClearPackagePreferredActivities(
    /* [in] */ const String& packageName)
{
    Int32 uid = Binder::GetCallingUid();
    // writer
    synchronized (mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            pkg = it->mSecond;
        }
        Int32 pkgUid;
        if (pkg == NULL || (pkg->mApplicationInfo->GetUid(&pkgUid), pkgUid != uid)) {
            Int32 result = 0;
            FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
                    Elastos::Droid::Manifest::permission::SET_PREFERRED_APPLICATIONS, &result))
            if (result != IPackageManager::PERMISSION_GRANTED) {
                if (GetUidTargetSdkVersionLockedLPr(
                        Binder::GetCallingUid()) < Build::VERSION_CODES::FROYO) {
                    Slogger::W(TAG, "Ignoring clearPackagePreferredActivities() from uid %d", Binder::GetCallingUid());
                    return NOERROR;
                }
                FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
                        Elastos::Droid::Manifest::permission::SET_PREFERRED_APPLICATIONS, String(NULL)))
            }
        }

        Int32 user = UserHandle::GetCallingUserId();
        if (ClearPackagePreferredActivitiesLPw(packageName, user)) {
            mSettings->WritePackageRestrictionsLPr(user);
            ScheduleWriteSettingsLocked();
        }
    }

    return NOERROR;
}

Boolean CPackageManagerService::ClearPackagePreferredActivitiesLPw(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId)
{
    AutoPtr<List<AutoPtr<PreferredActivity> > > removed;
    Boolean changed = FALSE;

    HashMap<Int32, AutoPtr<PreferredIntentResolver> >::Iterator iter;
    for (iter = mSettings->mPreferredActivities.Begin();
        iter != mSettings->mPreferredActivities.End(); ++iter) {
        Int32 thisUserId = iter->mFirst;
        AutoPtr<PreferredIntentResolver> pir = iter->mSecond;
        if (userId != IUserHandle::USER_ALL && userId != thisUserId) {
            continue;
        }

        AutoPtr<ISet> filters = pir->FilterSet();
        Set<AutoPtr<PreferredActivity> >::Iterator setIter = filters->Begin();
        for (; setIter != filters->End(); ++setIter) {
            AutoPtr<PreferredActivity> pa = *setIter;
            // Mark entry for removal only if it matches the package name
            // and the entry is of type "always".
            String packageName;
            if (packageName.IsNull() ||
                    ((pa->mPref->mComponent->GetPackageName(&packageName), packageName.Equals(packageName))
                            && pa->mPref->mAlways)) {
                if (removed == NULL) {
                    removed = new List<AutoPtr<PreferredActivity> >();
                }
                removed.PushBack(pa);
            }
        }

        if (removed != NULL) {
            List<AutoPtr<PreferredActivity> >::Iterator listIter;
            for (listIter = removed.Begin(); listIter != removed.End(); ++listIter) {
                pir->RemoveFilter(*listIter);
            }
            changed = TRUE;
        }
    }
    return changed;
}

ECode CPackageManagerService::ResetPreferredActivities(
    /* [in] */ Int32 userId)
{
    /* TODO: Actually use userId. Why is it being passed in? */
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Core::Manifest::permission::SET_PREFERRED_APPLICATIONS, String(NULL)))
    // writer
    synchronized (mPackagesLock) {
        Int32 user = UserHandle::GetCallingUserId();
        ClearPackagePreferredActivitiesLPw(String(NULL), user);
        mSettings->ReadDefaultPreferredAppsLPw(this, user);
        mSettings->WritePackageRestrictionsLPr(user);
        ScheduleWriteSettingsLocked();
    }
}

ECode CPackageManagerService::GetPreferredActivities(
    /* [in, out] */ IList* outFilters,
    /* [in, out] */ IList* outActivities,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)

    Int32 num = 0;
    Int32 userId = UserHandle::GetCallingUserId();
    // reader
    synchronized (mPackagesLock) {
        HashMap<Int32, AutoPtr<PreferredIntentResolver> >::Iterator it = mSettings->mPreferredActivities.Find(userId);
        AutoPtr<PreferredIntentResolver> pir;
        if (it != mSettings->mPreferredActivities.End()) pir = it->mSecond;
        if (pir != NULL) {
            AutoPtr<ISet> activities = pir->FilterSet();
            AutoPtr<IIterator> it;
            activities->GetIterator((IIterator**)&it);
            Boolean hasNext;
            while (it->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> value;
                it->GetNext((IInterface**)&value);
                AutoPtr<PreferredActivity> pa = (PreferredActivity*)(IObject*)value.Get();
                String pkgN;
                if (packageName.IsNull()
                        || ((pa->mPref->mComponent->GetPackageName(&pkgN), pkgN.Equals(packageName))
                                && pa->mPref->mAlways)) {
                    pa->mPref->mComponent->GetPackageName(&pkgN);
                    if (packageName.IsNull() || pkgN.Equals(packageName)) {
                        if (outFilters != NULL) {
                            AutoPtr<IIntentFilter> filter;
                            CIntentFilter::New(pa, (IIntentFilter**)&filter);
                            outFilters->Add(filter);
                        }
                        if (outActivities != NULL) {
                            outActivities->Add(pa->mPref->mComponent);
                        }
                    }
                }
            }
        }
    }

    *count = num;
    return NOERROR;
}

ECode CPackageManagerService::AddPersistentPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ IComponentName* activity,
    /* [in] */ Int32 userId)
{
    Int32 callingUid = Binder::GetCallingUid();
    if (callingUid != IProcess::SYSTEM_UID) {
        Slogger::E(TAG, "addPersistentPreferredActivity can only be run by the system");
        return E_SECURITY_EXCEPTION;
    }
    Int32 count;
    if (filter->CountActions(&count), count == 0) {
        Slogger::W(TAG, "Cannot set a preferred activity with no filter actions");
        return NOERROR;
    }
    synchronized (mPackagesLock) {
        Slogger::I(TAG, "Adding persistent preferred activity %p for user %d :", activity, userId);
        // filter.dump(new LogPrinter(Log.INFO, TAG), "  ");
        AutoPtr<PersistentPreferredActivity> a = new PersistentPreferredActivity(filter, activity);
        mSettings->EditPersistentPreferredActivitiesLPw(userId)->AddFilter(a);
        mSettings->WritePackageRestrictionsLPr(userId);
    }
    return NOERROR;
}

ECode CPackageManagerService::ClearPackagePersistentPreferredActivities(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId)
{
    Int32 callingUid = Binder::GetCallingUid();
    if (callingUid != IProcess::SYSTEM_UID) {
        Slogger::E(TAG, "clearPackagePersistentPreferredActivities can only be run by the system");
        return E_SECURITY_EXCEPTION;
    }
    AutoPtr<List<AutoPtr<PersistentPreferredActivity> > > removed;
    Boolean changed = FALSE;
    synchronized (mPackagesLock) {
        HashMap<Int32, AutoPtr<PersistentPreferredIntentResolver> >::Iterator it
                = mSettings->mPersistentPreferredActivities.Begin();
        for (Int32 i = 0; i < mSettings->mPersistentPreferredActivities.End(); ++it) {
            Int32 thisUserId = it->mFirst;
            AutoPtr<PersistentPreferredIntentResolver> ppir = it->mSecond;
            if (userId != thisUserId) {
                continue;
            }
            AutoPtr<IIterator> it = ppir->FilterIterator();
            Boolean hasNext;
            while (it->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> value;
                it->GetNext((IInterface**)&value);
                AutoPtr<PersistentPreferredActivity> ppa = (PersistentPreferredActivity*)(IObject*)value.Get();
                // Mark entry for removal only if it matches the package name.
                String pkgN;
                if (ppa->mComponent->GetPackageName(&pkgN), pkgN.Equals(packageName)) {
                    if (removed == NULL) {
                        removed = new List<AutoPtr<PersistentPreferredActivity> >();
                    }
                    removed->PushBack(ppa);
                }
            }
            if (removed != NULL) {
                List<AutoPtr<PersistentPreferredActivity> >::Iterator removedIt = removed->Begin();
                for (; removedIt != removed->End(); ++removedIt) {
                    ppir->RemoveFilter(*removedIt);
                }
                changed = TRUE;
            }
        }

        if (changed) {
            mSettings->WritePackageRestrictionsLPr(userId);
        }
    }
    return NOERROR;
}

ECode CPackageManagerService::AddCrossProfileIntentFilter(
    /* [in] */ IIntentFilter* intentFilter,
    /* [in] */ const String& ownerPackage,
    /* [in] */ Int32 ownerUserId,
    /* [in] */ Int32 sourceUserId,
    /* [in] */ Int32 targetUserId,
    /* [in] */ Int32 flags)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Core::Manifest::permission::INTERACT_ACROSS_USERS_FULL, String(NULL)))
    Int32 callingUid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceOwnerRights(ownerPackage, ownerUserId, callingUid))
    FAIL_RETURN(EnforceShellRestriction(IUserManager::DISALLOW_DEBUGGING_FEATURES, callingUid, sourceUserId))
    Int32 count;
    if (intentFilter->CountActions(&count), count == 0) {
        Slogger::W(TAG, "Cannot set a crossProfile intent filter with no filter actions");
        return;
    }
    synchronized (mPackagesLock) {
        AutoPtr<CrossProfileIntentFilter> filter = new CrossProfileIntentFilter(intentFilter,
                ownerPackage, UserHandle::GetUserId(callingUid), targetUserId, flags);
        mSettings->EditCrossProfileIntentResolverLPw(sourceUserId)->AddFilter(filter);
        mSettings->WritePackageRestrictionsLPr(sourceUserId);
    }
    return NOERROR;
}

ECode CPackageManagerService::ClearCrossProfileIntentFilters(
    /* [in] */ Int32 sourceUserId,
    /* [in] */ const String& ownerPackage,
    /* [in] */ Int32 ownerUserId)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Core::Manifest::permission::INTERACT_ACROSS_USERS_FULL, String(NULL)))
    Int32 callingUid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceOwnerRights(ownerPackage, ownerUserId, callingUid))
    FAIL_RETURN(EnforceShellRestriction(IUserManager::DISALLOW_DEBUGGING_FEATURES, callingUid, sourceUserId))
    Int32 callingUserId = UserHandle::GetUserId(callingUid);
    synchronized (mPackagesLock) {
        AutoPtr<CrossProfileIntentResolver> resolver =
                mSettings->EditCrossProfileIntentResolverLPw(sourceUserId);
        AutoPtr<ISet> activities = resolver->FilterSet();
        AutoPtr<ICollection> col = ICollection::Probe(activities);
        AutoPtr<IHashSet> set;
        CHashSet::New(col, (IHashSet**)&set);
        AutoPtr<IIterator> it;
        set->GetIterator((IIterator**)&it);
        Boolean hasNext;
        while (it->HasNext(&hasNext), hasNext) {
            AutoPtr<IInterface> value;
            it->GetNext((IInterface**)&value);
            AutoPtr<CrossProfileIntentFilter> filter = (CrossProfileIntentFilter*)(IObject*)value.Get();
            if (filter->GetOwnerPackage().Equals(ownerPackage)
                    && filter->GetOwnerUserId() == callingUserId) {
                resolver->RemoveFilter(filter);
            }
        }
        mSettings->WritePackageRestrictionsLPr(sourceUserId);
    }
    return NOERROR;
}

ECode CPackageManagerService::EnforceOwnerRights(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 callingUid)
{
    // The system owns everything.
    if (UserHandle::GetAppId(callingUid) == IProcess::SYSTEM_UID) {
        return;
    }
    Int32 callingUserId = UserHandle::GetUserId(callingUid);
    if (callingUserId != userId) {
        Slogger::E(TAG, "calling uid %d pretends to own %s on user %d but belongs to user %d",
                callingUid, pkg.string(), userId, callingUserId);
        return E_SECURITY_EXCEPTION;
    }
    AutoPtr<IPackageInfo> pi;
    GetPackageInfo(pkg, 0, callingUserId, (IPackageInfo**)&pi);
    if (pi == NULL) {
        Slogger::E(TAG, "Unknown package %s on user %d", pkg.string(), callingUserId);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IApplicationInfo> ai;
    pi->GetApplicationInfo((IApplicationInfo**)&ai);
    Int32 uid;
    ai->GetUid(&uid);
    if (!UserHandle::IsSameApp(uid, callingUid)) {
        Slogger::E(TAG, "Calling uid %d does not own package ", callingUid, pkg.string());
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}

ECode CPackageManagerService::GetHomeActivities(
    /* [in] */ IList* allHomeCandidates,
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name)

    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_MAIN, (IIntent**)&intent);
    intent->AddCategory(Intent.CATEGORY_HOME);

    Int32 callingUserId = UserHandle::GetCallingUserId();
    AutoPtr<IList> list;
    QueryIntentActivities(intent, String(NULL),
            IPackageManager::GET_META_DATA, callingUserId, (IList**)&list);
    AutoPtr<IResolveInfo> preferred = FindPreferredActivity(intent, String(NULL), 0, list, 0,
            TRUE, FALSE, FALSE, callingUserId);

    allHomeCandidates->Clear();
    if (list != NULL) {
        AutoPtr<IIterator> it;
        list->GetIterator((IIterator**)&it);
        Boolean hasNext;
        while (it->HasNext(&hasNext), hasNext) {
            AutoPtr<IInterface> value;
            it->GetNext((IInterface**)&value);
            AutoPtr<IResolveInfo> ri =IResolveInfo::Probe(value);
            allHomeCandidates->Add(ri);
        }
    }
    AutoPtr<IActivityInfo> ai;
    if (preferred == NULL || (preferred->GetActivityInfo((IActivityInfo**)&ai), ai == NULL)) {
        *name = NULL;
        return NOERROR;
    }
    else {
        String packageName, n;
        ai->GetPackageName(&packageName);
        ai->GetName(&n);
        return CComponentName::New(packageName, n, name);
    }
}

ECode CPackageManagerService::SetApplicationEnabledSetting(
    /* [in] */ const String& appPackageName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [in] */ const String& _callingPackage)
{
    String callingPackage = _callingPackage;
    if (!sUserManager->Exists(userId)) return NOERROR;
    if (callingPackage.IsNull()) {
        callingPackage = StringUtils::ToString(Binder::GetCallingUid());
    }
    return SetEnabledSetting(appPackageName, String(NULL), newState, flags, userId, callingPackage);
}

ECode CPackageManagerService::SetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NOERROR;
    String pkgName, clsName;
    componentName->GetPackageName(&pkgName);
    componentName->GetClassName(&clsName);
    return SetEnabledSetting(pkgName, clsName, newState, flags, userId, String(NULL));
}

ECode CPackageManagerService::SetEnabledSetting(
    /* [in] */ const String& packageName,
    /* [in] */ const String& className,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [in] */ const String& _callingPackage)
{
    String callingPackage = _callingPackage;

    if (!(newState == IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT
          || newState == IPackageManager::COMPONENT_ENABLED_STATE_ENABLED
          || newState == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED
          || newState == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_USER
          || newState == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_UNTIL_USED)) {
        Slogger::E(TAG, "Invalid new component state: %d", newState);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<PackageSetting> pkgSetting;
    Int32 uid = Binder::GetCallingUid();
    Int32 permission = 0;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CHANGE_COMPONENT_ENABLED_STATE, &permission))
    FAIL_RETURN(EnforceCrossUserPermission(uid, userId, FALSE, TRUE, String("set enabled")))
    Boolean allowedByPermission = (permission == IPackageManager::PERMISSION_GRANTED);
    Boolean sendNow = FALSE;
    Boolean isApp = (className.IsNull());
    String componentName = isApp ? packageName : className;
    Int32 packageUid = -1;
    AutoPtr<List<String> > components;

    // writer
    synchronized (mPackagesLock) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator psit = mSettings->mPackages.Find(packageName);
        if (psit != mSettings->mPackages.End()) {
            pkgSetting = psit->mSecond;
        }
        if (pkgSetting == NULL) {
            if (className.IsNull()) {
                Slogger::E(TAG, "Unknown package: %s", packageName.string());
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            Slogger::E("Unknown component: %s/%s", packageName.string(), className.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        // Allow root and verify that userId is not being specified by a different user
        if (!allowedByPermission && !UserHandle::IsSameApp(uid, pkgSetting->mAppId)) {
            Slogger::E(TAG, "Permission Denial: attempt to change component state from pid=%d, uid=%d, package uid=%d",
                    Binder::GetCallingPid(), uid, pkgSetting->mAppId);
            return E_SECURITY_EXCEPTION;
        }
        if (className.IsNull()) {
            // We're dealing with an application/package level state change
            if (pkgSetting->GetEnabled(userId) == newState) {
                // Nothing to do
                return NOERROR;
            }
            if (newState == IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT
                    || newState == IPackageManager::COMPONENT_ENABLED_STATE_ENABLED) {
                // Don't care about who enables an app.
                callingPackage = String(NULL);
            }
            pkgSetting->SetEnabled(newState, userId, callingPackage);
            // pkgSetting.pkg.mSetEnabled = newState;
        }
        else {
            // We're dealing with a component level state change
            // First, verify that this is a valid class name.
            AutoPtr<PackageParser::Package> pkg = pkgSetting->mPkg;
            if (pkg == NULL || !pkg->HasComponentClassName(className)) {
                Int32 targetSdkVersion;
                pkg->mApplicationInfo->GetTargetSdkVersion(&targetSdkVersion);
                if (targetSdkVersion >= Build::VERSION_CODES::JELLY_BEAN) {
                    // throw new IllegalArgumentException();
                    Slogger::E(TAG, "Component class %s does not exist in %s", className.string()
                            , packageName.string());
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
                else {
                    Slogger::W(TAG, "Failed setComponentEnabledSetting: component class %s does not exist in %s",
                            className.string(), packageName.string());
                }
            }
            switch (newState) {
            case IPackageManager::COMPONENT_ENABLED_STATE_ENABLED:
                if (!pkgSetting->EnableComponentLPw(className, userId)) {
                    return NOERROR;
                }
                break;
            case IPackageManager::COMPONENT_ENABLED_STATE_DISABLED:
                if (!pkgSetting->DisableComponentLPw(className, userId)) {
                    return NOERROR;
                }
                break;
            case IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT:
                if (!pkgSetting->RestoreComponentLPw(className, userId)) {
                    return NOERROR;
                }
                break;
            default:
                Slogger::E(TAG, "Invalid new component state: %d", newState);
                return NOERROR;
            }
        }
        mSettings->WritePackageRestrictionsLPr(userId);
        components = mPendingBroadcasts->Get(userId, packageName);
        Boolean newPackage = components == NULL;
        if (newPackage) {
            components = new List<String>();
        }
        if (Find(components->Begin(), components->End(), componentName) == components->End()) {
            components->PushBack(componentName);
        }
        if ((flags & IPackageManager::DONT_KILL_APP) == 0) {
            sendNow = TRUE;
            // Purge entry from pending broadcast list if another one exists already
            // since we are sending one right away.
            mPendingBroadcasts->Remove(userId, packageName);
        }
        else {
            if (newPackage) {
                mPendingBroadcasts->Put(userId, packageName, components);
            }

            Boolean hasMessage;
            mHandler->HasMessages(SEND_PENDING_BROADCAST, &hasMessage);
            if (!hasMessage) {
                // Schedule a message
                Boolean result;
                mHandler->SendEmptyMessageDelayed(SEND_PENDING_BROADCAST, BROADCAST_DELAY, &result);
            }
        }
    }

    Int64 callingId = Binder::ClearCallingIdentity();
    // try {
    if (sendNow) {
        packageUid = UserHandle::GetUid(userId, pkgSetting->mAppId);
        SendPackageChangedBroadcast(packageName,
                (flags & IPackageManager::DONT_KILL_APP) != 0, components, packageUid);
    }
    // } finally {
    Binder::RestoreCallingIdentity(callingId);
    // }
    return NOERROR;
}

void CPackageManagerService::SendPackageChangedBroadcast(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean killFlag,
    /* [in] */ List<String>* componentNames,
    /* [in] */ Int32 packageUid)
{
    if (DEBUG_INSTALL) {
        Logger::V(TAG, "Sending package changed: package=%s components=%p", packageName.string(),
                componentNames);
    }
    AutoPtr<IBundle> extras;
    ASSERT_SUCCEEDED(CBundle::New(4, (IBundle**)&extras));
    extras->PutString(IIntent::EXTRA_CHANGED_COMPONENT_NAME, componentNames->GetFront());
    AutoPtr< ArrayOf<String> > nameList = ArrayOf<String>::Alloc(componentNames->GetSize());
    Int32 i;
    List<String>::ConstIterator it;
    for (it = componentNames->Begin(), i = 0; it != componentNames->End(); ++it, ++i) {
        (*nameList)[i] = *it;
    }
    extras->PutStringArray(IIntent::EXTRA_CHANGED_COMPONENT_NAME_LIST, nameList);
    extras->PutBoolean(IIntent::EXTRA_DONT_KILL_APP, killFlag);
    extras->PutInt32(IIntent::EXTRA_UID, packageUid);
    AutoPtr<ArrayOf<Int32> > iArray = ArrayOf<Int32>::Alloc(1);
    iArray->Set(0, UserHandle::GetUserId(packageUid));
    SendPackageBroadcast(IIntent::ACTION_PACKAGE_CHANGED, packageName, extras, String(NULL), NULL,
            iArray);
}

ECode CPackageManagerService::SetPackageStoppedState(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean stopped,
    /* [in] */ Int32 userId)
{
    if (!sUserManager->Exists(userId)) return NOERROR;
    const Int32 uid = Binder::GetCallingUid();
    Int32 permission;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CHANGE_COMPONENT_ENABLED_STATE, &permission))
    Boolean allowedByPermission = (permission == IPackageManager::PERMISSION_GRANTED);
    FAIL_RETURN(EnforceCrossUserPermission(uid, userId, TRUE, TRUE, String("stop package")))
    // writer
    AutoLock lock(mPackagesLock);
    Boolean setted;
    mSettings->SetPackageStoppedStateLPw(packageName, stopped, allowedByPermission,
            uid, userId, &setted);
    if (setted) {
        ScheduleWritePackageRestrictionsLocked(userId);
    }

    return NOERROR;
}

ECode CPackageManagerService::GetInstallerPackageName(
    /* [in] */ const String& packageName,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    // reader
    synchronized (mPackages) {
        return mSettings->GetInstallerPackageNameLPr(packageName, name);
    }
}

ECode CPackageManagerService::GetApplicationEnabledSetting(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(setting);

    if (!sUserManager->Exists(userId)) {
        *setting = IPackageManager::COMPONENT_ENABLED_STATE_DISABLED;
        return NOERROR;
    }
    Int32 uid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(uid, userId, FALSE, FALSE, String("get enabled")))
    // reader
    AutoLock Lock(mPackagesLock);

    return mSettings->GetApplicationEnabledSettingLPr(packageName, userId, setting);
}

ECode CPackageManagerService::GetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* setting)
{
    if (!sUserManager->Exists(userId)) {
        *setting = IPackageManager::COMPONENT_ENABLED_STATE_DISABLED;
        return NOERROR;
    }
    Int32 uid = Binder::GetCallingUid();
    FAIL_RETURN(EnforceCrossUserPermission(uid, userId, FALSE, FALSE, String("get component enabled")))
    // reader
    synchronized (mPackages) {
        return mSettings->GetComponentEnabledSettingLPr(componentName, userId, setting);
    }
}

ECode CPackageManagerService::EnterSafeMode()
{
    FAIL_RETURN(EnforceSystemOrRoot(String("Only the system can request entering safe mode")))

    if (!mSystemReady) {
        mSafeMode = TRUE;
    }
    return NOERROR;
}

ECode CPackageManagerService::SystemReady()
{
    mSystemReady = TRUE;

    // Read the compatibilty setting when the system is ready.
    AutoPtr<ISettingsGlobal> sGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Int32 value;
    sGlobal->GetInt32(
            resolver,
            ISettingsGlobal::COMPATIBILITY_MODE, 1, &value);
    Boolean compatibilityModeEnabled = value == 1;
    PackageParser::SetCompatibilityModeEnabled(compatibilityModeEnabled);
    if (DEBUG_SETTINGS) {
        Logger::D(TAG, "compatibility mode:%d", compatibilityModeEnabled);
    }

    synchronized (mPackagesLock) {
        // Verify that all of the preferred activity components actually
        // exist.  It is possible for applications to be updated and at
        // that point remove a previously declared activity component that
        // had been set as a preferred activity.  We try to clean this up
        // the next time we encounter that preferred activity, but it is
        // possible for the user flow to never be able to return to that
        // situation so here we do a sanity check to make sure we haven't
        // left any junk around.
        List<AutoPtr<PreferredActivity> > removed;
        HashMap<Int32, AutoPtr<PreferredIntentResolver> >::Iterator it;
        for (it = mSettings->mPreferredActivities.Begin(); it != mSettings->mPreferredActivities.End(); ++it) {
            AutoPtr<PreferredIntentResolver> pir = it->mSecond;
            removed.Clear();
            Set< AutoPtr<PreferredActivity> >::Iterator paIt;
            for (paIt = pir->FilterSet()->Begin(); paIt != pir->FilterSet()->End(); ++paIt) {
                AutoPtr<PreferredActivity> pa = *paIt;
                HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Activity> >::Iterator ait =
                        mActivities->mActivities.Find(pa->mPref->mComponent);
                if (ait != mActivities->mActivities.End()) {
                    if (ait->mSecond == NULL) {
                        removed.PushBack(pa);
                    }
                }
            }
            if (!removed.IsEmpty()) {
                List<AutoPtr<PreferredActivity> >::Iterator reIt;
                for (reIt = removed.Begin(); reIt != removed.End(); ++reIt) {
                    AutoPtr<PreferredActivity> pa = *reIt;
                    Slogger::W(TAG, "Removing dangling preferred activity: %p",
                            pa->mPref->mComponent.Get());
                    pir->RemoveFilter(pa);
                }
                mSettings->WritePackageRestrictionsLPr(
                        it->mFirst);
            }
        }
    }
    sUserManager->SystemReady();

    // Kick off any messages waiting for system ready
    if (mPostSystemReadyMessages != NULL) {
        List<AutoPtr<IMessage> >::Iterator it = mPostSystemReadyMessages->Begin();
        for (; it != mPostSystemReadyMessages->End(); ++it) {
            (*it)->SendToTarget();
        }
        mPostSystemReadyMessages = NULL;
    }
    return NOERROR;
}

ECode CPackageManagerService::IsSafeMode(
    /* [out] */ Boolean* isSafeMode)
{
    VALIDATE_NOT_NULL(isSafeMode);

    *isSafeMode = mSafeMode;
    return NOERROR;
}

ECode CPackageManagerService::HasSystemUidErrors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mHasSystemUidErrors;
    return NOERROR;
}

String CPackageManagerService::ArrayToString(
    /* [in] */ ArrayOf<Int32>* array)
{
    StringBuffer buf(128);
    buf.AppendChar('[');
    if (array != NULL) {
        for (Int32 i = 0; i < array->GetLength(); i++) {
            if (i > 0) buf.AppendCStr(", ");
            buf.AppendInt32((*array)[i]);
        }
    }
    buf.AppendChar(']');
    return buf.ToString();
}

String CPackageManagerService::GetEncryptKey()
{
    // try {
    // AutoPtr<ISystemKeyStoreHelper> helper;
    // CSystemKeyStoreHelper::AcquireSingleton((ISystemKeyStoreHelper**)&helper);
    // AutoPtr<ISystemKeyStore> keyStore;
    // helper->GetInstance((ISystemKeyStore**)&keyStore);
    // String sdEncKey;
    // keyStore->RetrieveKeyHexString(SD_ENCRYPTION_KEYSTORE_NAME, &sdEncKey);
    // if (sdEncKey.IsNull()) {
    //     if (FAILED(keyStore->generateNewKeyHexString(128,
    //             SD_ENCRYPTION_ALGORITHM, SD_ENCRYPTION_KEYSTORE_NAME, &sdEncKey))) {
    //         Slogger::E(TAG, "Failed to create encryption keys with exception: ");
    //         return String(NULL);
    //     }
    //     if (sdEncKey.IsNull()) {
    //         Slogger::E(TAG, "Failed to create encryption keys");
    //         return String(NULL);
    //     }
    // }
    // return sdEncKey;
    // } catch (NoSuchAlgorithmException nsae) {
    //     Slog.e(TAG, "Failed to create encryption keys with exception: " + nsae);
    //     return null;
    // } catch (IOException ioe) {
    //     Slog.e(TAG, "Failed to retrieve encryption keys with exception: " + ioe);
    //     return null;
    // }
    Slogger::E(TAG, "TODO:CPackageManagerService::GetEncryptKey is not implemented!");
    return String("4eb26b94a3996346c363db11b14f5f2b");
}

ECode CPackageManagerService::UpdateExternalMediaStatus(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
    Int32 callingUid = Binder::GetCallingUid();
    if (callingUid != 0 && callingUid != IProcess::SYSTEM_UID) {
        // throw new SecurityException("Media status can only be updated by the system");
        return E_SECURITY_EXCEPTION;
    }
    // reader; this apparently protects mMediaMounted, but should probably
    // be a different lock in that case.
    synchronized (mPackagesLock) {
        Logger::I(TAG, "Updating external media status from %s to %s",
                (mMediaMounted ? "mounted" : "unmounted"), (mediaStatus ? "mounted" : "unmounted"));
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, "updateExternalMediaStatus:: mediaStatus=%d, mMediaMounted=%d",
                    mediaStatus, mMediaMounted);
        }
        if (mediaStatus == mMediaMounted) {
            AutoPtr<IMessage> msg;
            mHandler->ObtainMessage(UPDATED_MEDIA_STATUS, (IMessage**)&msg);
            msg->SetArg1(reportStatus ? 1 : 0);
            msg->SetArg2(-1);
            Boolean result;
            return mHandler->SendMessage(msg, &result);
        }
        mMediaMounted = mediaStatus;
    }
    // Queue up an async operation since the package installation may take a
    // little while.
    AutoPtr<IRunnable> runnable = new UpdateExternalMediaStatusRunnable(this, mediaStatus, reportStatus);
    Boolean result;
    return mHandler->Post(runnable, &result);
}

ECode CPackageManagerService::ScanAvailableAsecs()
{
    UpdateExternalMediaStatusInner(TRUE, FALSE, FALSE);
    if (mShouldRestoreconData) {
        SELinuxMMAC::SetRestoreconDone();
        mShouldRestoreconData = FALSE;
    }
    return NOERROR;
}

void CPackageManagerService::UpdateExternalMediaStatusInner(
    /* [in] */ Boolean isMounted,
    /* [in] */ Boolean reportStatus,
    /* [in] */ Boolean externalStorage)
{
    HashMap<AutoPtr<AsecInstallArgs>, String> processCids;
    AutoPtr< ArrayOf<Int32> > uidArr = EmptyArray::INT;

    AutoPtr<IPackageHelper> helper;
    CPackageHelper::AcquireSingleton((IPackageHelper**)&helper);
    AutoPtr< ArrayOf<String> > list;
    helper->GetSecureContainerList((ArrayOf<String>**)&list);
    if (ArrayUtils::IsEmpty(list)) {
        Logger::I(TAG, "No secure containers found");
    }
    else {
        // Process list of secure containers and categorize them
        // as active or stale based on their package internal state.
        AutoPtr< ArrayOf<Int32> > uidList = ArrayOf<Int32>::Alloc(list->GetLength());
        Int32 num = 0;
        // reader
        synchronized (mPackagesLock) {
            for (Int32 i = 0; i < list->GetLength(); i++) {
                String cid = (*list)[i];
                // Leave stages untouched for now; installer service owns them
                if (PackageInstallerService::IsStageName(cid)) continue;

                if (DEBUG_SD_INSTALL) Logger::I(TAG, "Processing container %s", cid.string());
                String pkgName = GetAsecPackageName(cid);
                if (pkgName.IsNull()) {
                    Slogger::I(TAG, "Found stale container %s with no package name", cid.string());
                    continue;
                }
                if (DEBUG_SD_INSTALL) {
                    Logger::I(TAG, "Looking for pkg : %s", pkgName.string());
                }

                AutoPtr<PackageSetting> ps;
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mSettings->mPackages.Find(pkgName);
                if (it != mSettings->mPackages.End()) {
                    ps = it->mSecond;
                }
                if (ps == NULL) {
                    Slogger::I(TAG, "Found stale container %s with no matching settings", cid.string());
                    continue;
                }

                /*
                 * Skip packages that are not external if we're unmounting
                 * external storage.
                 */
                if (externalStorage && !isMounted && !IsExternal(ps)) {
                    continue;
                }

                AutoPtr<AsecInstallArgs> args = new AsecInstallArgs(cid,
                        GetAppDexInstructionSets(ps), IsForwardLocked(ps), this);
                // The package status is changed only if the code path
                // matches between settings and the container id.
                if (!ps->mCodePathString.IsNull() && ps->mCodePathString.StartWith(args->GetCodePath())) {
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, "Container : %s corresponds to pkg : %s at code path: %s", cid.string(), pkgName.string(),
                                ps->mCodePathString.string());
                    }

                    // We do have a valid package installed on sdcard
                    processCids[args] = ps->mCodePathString;
                    Int32 uid = ps->mAppId;
                    if (uid != -1) {
                        uidArr = ArrayUtils::AppendInt32(uidArr, uid);
                    }
                }
                else {
                    Slogger::I(TAG, "Found stale container %s: expected codePath=%s",
                            cid.string(), ps->mCodePathString.string());
                }
            }
        }

        Arrays::Sort(uidArr);
    }

    // Process capsules with valid entries->
    if (isMounted) {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Loading packages");
        AutoPtr<List<String> > removeCids;
        LoadMediaPackages(processCids, uidArr, &removeCids);
        StartCleaningPackages();
        mInstallerService->OnSecureContainersAvailable();
    }
    else {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Unloading packages");
        UnloadMediaPackages(processCids, uidArr, reportStatus);
    }
}

ECode CPackageManagerService::SendResourcesChangedBroadcast(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean replacing,
    /* [in] */ List<String>& pkgList,
    /* [in] */ ArrayOf<Int32>* uidArr,
    /* [in] */ IIntentReceiver* finishedReceiver)
{
    Int32 size = pkgList.GetSize();
    if (size > 0) {
        AutoPtr< ArrayOf<String> > pkgs = ArrayOf<String>::Alloc(size);
        List<String>::ConstIterator it;
        Int32 i;
        for (it = pkgList.Begin(), i = 0; it != pkgList.End(); ++it, ++i) {
            (*pkgs)[i] = *it;
        }
        // Send broadcasts here
        AutoPtr<IBundle> extras;
        CBundle::New((IBundle**)&extras);
        extras->PutStringArray(IIntent::EXTRA_CHANGED_PACKAGE_LIST, pkgs);
        if (uidArr != NULL) {
            extras->PutInt32Array(IIntent::EXTRA_CHANGED_UID_LIST, uidArr);
        }
        if (replacing) {
            extras->PutBoolean(IIntent::EXTRA_REPLACING, replacing);
        }
        String action = mediaStatus
                ? IIntent::ACTION_EXTERNAL_APPLICATIONS_AVAILABLE
                : IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE;
        SendPackageBroadcast(action, String(NULL), extras, String(NULL), finishedReceiver, NULL);
    }
    return NOERROR;
}

void CPackageManagerService::LoadMediaPackages(
    /* [in] */ HashMap<AutoPtr<AsecInstallArgs>, String>& processCids,
    /* [in] */ ArrayOf<Int32>* uidArr)
{
    List<String> pkgList;
    AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
    if (readBuffer == NULL) {
        Slogger::E(TAG, "LoadMediaPackages out of memory!");
        return;
    }

    HashMap<AutoPtr<AsecInstallArgs>, String>::Iterator it;
    for (it = processCids.Begin(); it != processCids.End(); ++it) {
        AutoPtr<AsecInstallArgs> args = it->mFirst;
        String codePath = it->mSecond;
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, "Loading container : %s", args->mCid.string());
        }
        Int32 retCode = IPackageManager::INSTALL_FAILED_CONTAINER_ERROR;
        // try {
        // Make sure there are no container errors first.
        if (args->DoPreInstall(IPackageManager::INSTALL_SUCCEEDED) != IPackageManager::INSTALL_SUCCEEDED) {
            Slogger::E(TAG, "Failed to mount cid : %s when installing from sdcard", args->mCid.string());
            continue;
        }
        // Check code path here.
        if (codePath.IsNull() || !codePath.StartWith(args->GetCodePath())) {
            Slogger::E(TAG, "Container %s cachepath %s does not match one in settings %s", args->mCid.string(),
                    args->GetCodePath().string(), codePath.string());
            continue;
        }
        // Parse package
        Int32 parseFlags = mDefParseFlags;
        if (args->IsExternal()) {
            parseFlags |= PackageParser::PARSE_ON_SDCARD;
        }
        if (args->IsFwdLocked()) {
            parseFlags |= PackageParser::PARSE_FORWARD_LOCK;
        }

        synchronized (mInstallLock) {
            AutoPtr<PackageParser::Package> pkg;
            // try {
            AutoPtr<IFile> f;
            CFile::New(codePath, (IFile**)&f);
            if (FAILED(ScanPackageLI(f, parseFlags, 0, 0, String(NULL), readBuffer, (PackageParser::Package**)&pkg))) {
                Slogger::W(TAG, "Failed to scan %s", codePath.string());
            }
            // } catch (PackageManagerException e) {
            //     Slog.w( + e.getMessage());
            // }
            // Scan the package
            if (pkg != NULL) {
                /*
                 * TODO why is the lock being held? doPostInstall is
                 * called in other places without the lock. This needs
                 * to be straightened out.
                 */
                // writer
                synchronized (mPackagesLock) {
                    retCode = IPackageManager::INSTALL_SUCCEEDED;
                    pkgList.PushBack(pkg->mPackageName);
                    // Post process args
                    Int32 uid;
                    pkg->mApplicationInfo->GetUid(&uid);
                    args->DoPostInstall(IPackageManager::INSTALL_SUCCEEDED, uid);
                }
            }
            else {
                Slogger::I(TAG, "Failed to install pkg from  %s from sdcard", codePath.string());
            }
        }

        // } finally {
        if (retCode != IPackageManager::INSTALL_SUCCEEDED) {
            // Don't destroy container here. Wait till gc clears things
            // up.
            removeCids->PushBack(args->mCid);
        }
        // }
    }
    // writer
    synchronized (mPackagesLock) {
        // If the platform SDK has changed since the last time we booted,
        // we need to re-grant app permission to catch any new ones that
        // appear. This is really a hack, and means that apps can in some
        // cases get permissions that the user didn't initially explicitly
        // allow... it would be nice to have some better way to handle
        // this situation.
        Boolean regrantPermissions = mSettings->mExternalSdkPlatform != mSdkVersion;
        if (regrantPermissions)
            Slogger::I(TAG, "Platform changed from %d to %d; regranting permissions for external storage",
                    mSettings->mExternalSdkPlatform, mSdkVersion);
        mSettings->mExternalSdkPlatform = mSdkVersion;

        // Make sure group IDs have been assigned, and any permission
        // changes in other apps are accounted for
        UpdatePermissionsLPw(String(NULL), NULL, UPDATE_PERMISSIONS_ALL
                | (regrantPermissions
                ? (UPDATE_PERMISSIONS_REPLACE_PKG|UPDATE_PERMISSIONS_REPLACE_ALL)
                : 0));

        mSettings->UpdateExternalDatabaseVersion();

        // can downgrade to reader
        // Persist settings
        mSettings->WriteLPr();
    }
    // Send a broadcast to let everyone know we are done processing
    if (!pkgList.IsEmpty()) {
        SendResourcesChangedBroadcast(TRUE, pkgList, &uidArr, NULL);
    }
}

void CPackageManagerService::UnloadAllContainers(
    /* [in] */ ISet* cidArgs) //Set< AutoPtr<AsecInstallArgs> >
{
    if (cidArgs == NULL) {
        return;
    }

    // Just unmount all valid containers.
    AutoPtr<IIterator> it;
    cidArgs->GetIterator((IIterator**)&it);
    Boolean next;
    while (it->HasNext(&next), next) {
        AutoPtr<IInterface> obj;
        it->Next((IInterface**)&obj);
        AsecInstallArgs* arg = (AsecInstallArgs*)obj.Get();
        synchronized (mInstallLock) {
            arg->DoPostDeleteLI(FALSE);
        }
    }
}

ECode CPackageManagerService::UnloadMediaPackages(
    /* [in] */ HashMap<AutoPtr<AsecInstallArgs>, String>& processCids,
    /* [in] */ ArrayOf<Int32>& uidArr,
    /* [in] */ Boolean reportStatus)
{
    if (DEBUG_SD_INSTALL) {
        Logger::I(TAG, "unloading media packages");
    }
    List<String> pkgList;
    List<AutoPtr<AsecInstallArgs> > failedList;
    AutoPtr<ISet> keys; // Set< AutoPtr<AsecInstallArgs> >
    CHashSet::New((ISet**)&keys);
    HashMap<AutoPtr<AsecInstallArgs>, String>::Iterator it;
    Boolean temp;
    AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
    if (readBuffer == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    for (it = processCids.Begin(); it != processCids.End(); ++it) {
        AutoPtr<AsecInstallArgs> args = it->mFirst;
        keys->Add(args.Get(), &temp);

        String pkgName = args->GetPackageName();
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, "Trying to unload pkg : %s", pkgName.string());
        }
        // Delete package internally
        AutoPtr<PackageRemovedInfo> outInfo = new PackageRemovedInfo(this);
        synchronized (mInstallLock) {
            Boolean res = DeletePackageLI(pkgName, NULL, FALSE, NULL, NULL,
                    IPackageManager::DELETE_KEEP_DATA, outInfo, FALSE, readBuffer);
            if (res) {
                pkgList.PushBack(pkgName);
            }
            else {
                Slogger::E(TAG, "Failed to delete pkg from sdcard : %s", pkgName.string());
                failedList.PushBack(args);
            }
        }
    }

    // reader
    synchronized (mPackagesLock) {
        // We didn't update the settings after removing each package;
        // write them now for all packages.
        mSettings->WriteLPr();
    }

    // We have to absolutely send UPDATED_MEDIA_STATUS only
    // after confirming that all the receivers processed the ordered
    // broadcast when packages get disabled, force a gc to clean things up.
    // and unload all the containers.
    if (!pkgList.IsEmpty()) {
        AutoPtr<IIntentReceiver> receiver;
        CResourcesChangedReceiver::New(IIPackageManager::Probe(this), keys, reportStatus,
                (IIntentReceiver**)&receiver);
        SendResourcesChangedBroadcast(FALSE, FALSE, pkgList, &uidArr, receiver);
    }
    else {
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(UPDATED_MEDIA_STATUS, (IMessage**)&msg);
        msg->SetObj(keys);
        msg->SetArg1(reportStatus ? 1 : 0);
        msg->SetArg2(1);
        Boolean result;
        return mHandler->SendMessage(msg, &result);
    }
    return NOERROR;
}

ECode CPackageManagerService::MovePackage(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageMoveObserver* observer,
    /* [in] */ Int32 flags)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MOVE_PACKAGE, String(NULL)));
    AutoPtr<IUserHandle> user;
    CUserHandle::New(UserHandle::GetCallingUserId(), (IUserHandle**)&user);
    Int32 returnCode = IPackageManager::MOVE_SUCCEEDED;
    Int32 currInstallFlags = 0;
    Int32 newInstallFlags = 0;

    AutoPtr<IFile> codeFile;
    String installerPackageName(NULL);
    String packageAbiOverride(NULL);


    // reader
    synchronized (mPackages) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            pkg = it->mSecond;
        }
        AutoPtr<PackageSetting> ps;
        HashMap<String, AutoPtr<PackageSetting> >::Iterator pkgIt = mSettings->mPackages.Find(packageName);
        if (pkgIt != mSettings->mPackages.End()) {
            ps = pkgIt->mSecond;
        }
        if (pkg == NULL || ps == NULL) {
            returnCode =  IPackageManager::MOVE_FAILED_DOESNT_EXIST;
        }
        else {
            // Disable moving fwd locked apps and system capsules
            if (pkg->mApplicationInfo != NULL && IsSystemApp(pkg)) {
                Slogger::W(TAG, "Cannot move system application");
                returnCode = IPackageManager::MOVE_FAILED_SYSTEM_PACKAGE;
            }
            else if (pkg->mOperationPending) {
                Slogger::W(TAG, "Attempt to move package which has pending operations");
                returnCode = IPackageManager::MOVE_FAILED_OPERATION_PENDING;
            }
            else {
                // Find install location first
                if ((flags & IPackageManager::MOVE_EXTERNAL_MEDIA) != 0
                    && (flags & IPackageManager::MOVE_INTERNAL) != 0) {
                    Slogger::W(TAG, "Ambigous flags specified for move location.");
                    returnCode = IPackageManager::MOVE_FAILED_INVALID_LOCATION;
                }
                else {
                    newInstallFlags = (flags & IPackageManager::MOVE_EXTERNAL_MEDIA) != 0 ?
                            IPackageManager::INSTALL_EXTERNAL : IPackageManager::INSTALL_INTERNAL;
                    currInstallFlags = isExternal(pkg) ?
                            IPackageManager::INSTALL_EXTERNAL : IPackageManager::INSTALL_INTERNAL;

                    if (newInstallFlags == currInstallFlags) {
                        Slogger::W(TAG, "No move required. Trying to move to same location");
                        returnCode = IPackageManager::MOVE_FAILED_INVALID_LOCATION;
                    }
                    else {
                        if (IsForwardLocked(pkg)) {
                            currInstallFlags |= IPackageManager::INSTALL_FORWARD_LOCK;
                            newInstallFlags |= IPackageManager::INSTALL_FORWARD_LOCK;
                        }
                    }
                }
                if (returnCode == IPackageManager::MOVE_SUCCEEDED) {
                    pkg->mOperationPending = TRUE;
                }
            }

            codeFile = NULL;
            CFile::New(pkg->mCodePath, (IFile**)&codeFile);
            installerPackageName = ps->mInstallerPackageName;
            packageAbiOverride = ps->mCpuAbiOverrideString;
        }
    }

    if (returnCode != IPackageManager::MOVE_SUCCEEDED) {
        // try {
        return observer->PackageMoved(packageName, returnCode);
        // } catch (RemoteException ignored) {
        // }
    }

    AutoPtr<IIPackageInstallObserver2> installObserver;
    CPackageInstallObserver2::New((IIPackageManager*)this, observer, (IIPackageInstallObserver2**)&installObserver);

    // Treat a move like reinstalling an existing app, which ensures that we
    // process everythign uniformly, like unpacking native libraries.
    newInstallFlags |= IPackageManager::INSTALL_REPLACE_EXISTING;

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(INIT_COPY, (IMessage**)&msg);
    AutoPtr<OriginInfo> origin = OriginInfo::FromExistingFile(codeFile);
    // begin from this
    msg->SetObj = new InstallParams(origin, installObserver, newInstallFlags,
            installerPackageName, null, user, packageAbiOverride);
    mHandler.sendMessage(msg);
}

ECode CPackageManagerService::SetInstallLocation(
    /* [in] */ Int32 loc,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        Elastos::Droid::Manifest::permission::WRITE_SECURE_SETTINGS, String(NULL)))
    Int32 location = 0;
    if (GetInstallLocation(&location), location == loc) {
        *result = TRUE;
        return NOERROR;
    }
    if (loc == IPackageHelper::APP_INSTALL_AUTO
        || loc == IPackageHelper::APP_INSTALL_INTERNAL
        || loc == IPackageHelper::APP_INSTALL_EXTERNAL) {
        AutoPtr<ISettingsGlobal> sGlobal;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);
        AutoPtr<IContentResolver> resolver;
        mContext->GetContentResolver((IContentResolver**)&resolver);
        Boolean value;
        sGlobal->PutInt32(resolver, ISettingsGlobal::DEFAULT_INSTALL_LOCATION, loc, &value);
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CPackageManagerService::GetInstallLocation(
    /* [out] */ Int32* loc)
{
    VALIDATE_NOT_NULL(loc)

    AutoPtr<ISettingsGlobal> sGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sGlobal);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    return sGlobal->GetInt32(resolver, ISettingsGlobal::DEFAULT_INSTALL_LOCATION,
            IPackageHelper::APP_INSTALL_AUTO, loc);
}

void CPackageManagerService::CleanUpUserLILPw(
    /* [in] */ CUserManagerService* userManager,
    /* [in] */ Int32 userHandle)
{
    mDirtyUsers.Erase(userHandle);
    mSettings->RemoveUserLPw(userHandle);
    mPendingBroadcasts->Remove(userHandle);
    if (mInstaller != NULL) {
        // Technically, we shouldn't be doing this with the package lock
        // held.  However, this is very rare, and there is already so much
        // other disk I/O going on, that we'll let it slide for now.
        mInstaller->RemoveUserDataDirs(userHandle);
    }
    mUserNeedsBadging.Erase(userHandle);
    RemoveUnusedPackagesLILPw(userManager, userHandle);
}

void CPackageManagerService::RemoveUnusedPackagesLILPw(
    /* [in] */ CUserManagerService* userManager,
    /* [in] */ Int32 userHandle)
{
    Boolean DEBUG_CLEAN_APKS = FALSE;
    AutoPtr<ArrayOf<Int32> > users = userManager->GetUserIdsLPr();
    HashMap<String, AutoPtr<PackageSetting> >::Iterator psit = mSettings->mPackages.Begin();
    for (; psit != mSettings->mPackages.End(); ++psit) {
        AutoPtr<PackageSetting> ps = *psit;
        if (ps->mPkg == NULL) {
            continue;
        }
        String packageName = ps->mPkg->mPackageName;
        // Skip over if system app
        if ((ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0) {
            continue;
        }
        if (DEBUG_CLEAN_APKS) {
            Slogger::I(TAG, "Checking package " + packageName);
        }
        Boolean keep = FALSE;
        for (Int32 i = 0; i < users->GetLength(); i++) {
            if ((*users)[i] != userHandle && ps->GetInstalled((*users)[i])) {
                keep = true;
                if (DEBUG_CLEAN_APKS) {
                    Slogger::I(TAG, "  Keeping package %s for user %d", packageName.string(), (*users)[i]);
                }
                break;
            }
        }
        if (!keep) {
            if (DEBUG_CLEAN_APKS) {
                Slogger::I(TAG, "  Removing package %s", packageName.string());
            }
            AutoPtr<IRunnable> runnable = new RemoveUnusedPackagesRunnable(this, packageName, userHandle);
            Boolean result;
            mHandler->Post(runnable, &result);
        }
    }
}

ECode CPackageManagerService::GetVerifierDeviceIdentity(
    /* [out] */ IVerifierDeviceIdentity** identity)
{
    VALIDATE_NOT_NULL(identity);
    *identity = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::PACKAGE_VERIFICATION_AGENT,
            String("Only package verification agents can read the verifier device identity")))

    synchronized (mPackagesLock) {
        *identity = mSettings->GetVerifierDeviceIdentityLPw();
        REFCOUNT_ADD(*identity)
    }
    return NOERROR;
}

ECode CPackageManagerService::SetPermissionEnforced(
    /* [in] */ const String& permission,
    /* [in] */ Boolean enforced)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::GRANT_REVOKE_PERMISSIONS, String(NULL)))
    if (READ_EXTERNAL_STORAGE.Equals(permission)) {
        synchronized (mPackagesLock) {
            Boolean value;
            if (mSettings->mReadExternalStorageEnforced == NULL
                    || (mSettings->mReadExternalStorageEnforced->GetValue(&value), value != enforced)) {
                AutoPtr<IBoolean> bv;
                CBoolean::New(enforced, (IBoolean**)&bv);
                mSettings->mReadExternalStorageEnforced = bv;
                mSettings->WriteLPr();
            }
        }
        // kill any non-foreground processes so we restart them and
        // grant/revoke the GID.
        AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
        if (am != NULL) {
            Int64 token = Binder::ClearCallingIdentity();
            // try {
            Boolean result;
            ECode ec = am->KillProcessesBelowForeground(String("setPermissionEnforcement"), &result);
            // } catch (RemoteException e) {
            // } finally {
            //     Binder.restoreCallingIdentity(token);
            // }
            Binder::RestoreCallingIdentity(token);
            return ec;
        }
    }
    else {
        Slogger::E(TAG, "No selective enforcement for %s", permission.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CPackageManagerService::IsPermissionEnforced(
    /* [in] */ const String& permission,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    return NOERROR;
}

ECode CPackageManagerService::IsStorageLow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Int64 token = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<DeviceStorageMonitorInternal> dsm = LocalServices->GetService(EIID_DeviceStorageMonitorInternal);
    if (dsm != NULL) {
        *result = dsm->IsMemoryLow();
    }
    else {
        *result = FALSE;
    }
    // } finally {
    //     Binder.restoreCallingIdentity(token);
    // }
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
}

ECode CPackageManagerService::GetPackageInstaller(
    /* [out] */ IIPackageInstaller** installer)
{
    VALIDATE_NOT_NULL(installer)
    *installer = mInstallerService;
    REFCOUNT_ADD(*installer)
    return NOERROR;
}

Boolean CPackageManagerService::UserNeedsBadging(
    /* [in] */ Int32 userId)
{
    HashMap<Int32, Boolean>::Iterator it = mUserNeedsBadging.Find(userId);
    if (it == mUserNeedsBadging.End()) {
        AutoPtr<IUserInfo> userInfo;
        Int64 token = Binder::ClearCallingIdentity();
        // try {
        sUserManager->GetUserInfo(userId, (IUserInfo**)&userInfo);
        // } finally {
        //     Binder.restoreCallingIdentity(token);
        // }
        Binder::RestoreCallingIdentity(token);
        Boolean b;
        Boolean isManagedProfile;
        if (userInfo != NULL && (userInfo->IsManagedProfile(&isManagedProfile), isManagedProfile)) {
            b = TRUE;
        }
        else {
            b = FALSE;
        }
        mUserNeedsBadging[userId] = b;
        return b;
    }
    return it->mSecond;
}

ECode CPackageManagerService::GetKeySetByAlias(
    /* [in] */ const String& packageName,
    /* [in] */ const String& alias,
    /* [out] */ IKeySet** set)
{
    VALIDATE_NOT_NULL(set)
    *set = NULL;

    if (packageName.IsNull() || alias.IsNull()) {
        return NOERROR;
    }
    synchronized(mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mpackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg == NULL) {
            Slogger::W(TAG, "KeySet requested for unknown package:%s", packageName.string());
            Slogger::E(TAG, "Unknown package: %s", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        AutoPtr<KeySetHandle> h =
                mSettings->mKeySetManagerService->GetKeySetByAliasAndPackageNameLPr(packageName, alias);
        return CKeySet::New(IBinder::Probe(h), set);
    }
}

ECode CPackageManagerService::GetSigningKeySet(
    /* [in] */ const String& packageName,
    /* [out] */ IKeySet** set)
{
    VALIDATE_NOT_NULL(set)
    *set = NULL
    if (packageName.IsNull()) {
        return NOERROR;
    }
    synchronized(mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mpackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg == NULL) {
            Slogger::W(TAG, "KeySet requested for unknown package:%s", packageName.string());
            Slogger::E(TAG, "Unknown package: %s", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        Int32 uid;
        if ((pkg->mApplicationInfo->GetUid(&uid), uid != Binder::GetCallingUid())
                && IProcess::SYSTEM_UID != Binder::GetCallingUid()) {
            Slogger::E(TAG, "May not access signing KeySet of other apps.");
            return E_SECURITY_EXCEPTION;
        }
        AutoPtr<KeySetHandle> h =
                mSettings->mKeySetManagerService->GetSigningKeySetByPackageNameLPr(packageName);
        return CKeySet::New(IBinder::Probe(h), set);
    }
}

ECode CPackageManagerService::IsPackageSignedByKeySet(
    /* [in] */ const String& packageName,
    /* [in] */ IKeySet* ks,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (packageName.IsNull() || ks == NULL) {
        return NOERROR;
    }
    synchronized(mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mpackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg == NULL) {
            Slogger::W(TAG, "KeySet requested for unknown package:%s", packageName.string());
            Slogger::E(TAG, "Unknown package: %s", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        AutoPtr<IBinder> ksh;
        ks->GetToken((IBinder**)&ksh);
        AutoPtr<KeySetHandle> h = reinterpret_cast<KeySetHandle*>(ksh->Probe(EIID_KeySetHandle));
        if (h != NULL) {
            *result = mSettings->mKeySetManagerService->PackageIsSignedByLPr(packageName, h);
            return NOERROR;
        }
        *result = FALSE;
    }
    return NOERROR;
}

ECode CPackageManagerService::IsPackageSignedByKeySetExactly(
    /* [in] */ const String& packageName,
    /* [in] */ IKeySet* ks,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (packageName.IsNull() || ks == NULL) {
        return NOERROR;
    }
    synchronized(mPackagesLock) {
        AutoPtr<PackageParser::Package> pkg;
        HashMap<String, AutoPtr<PackageParser::Package> >::Iterator it = mPackages.Find(packageName);
        if (it != mpackages.End()) {
            pkg = it->mSecond;
        }
        if (pkg == NULL) {
            Slogger::W(TAG, "KeySet requested for unknown package:%s", packageName.string());
            Slogger::E(TAG, "Unknown package: %s", packageName.string());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        AutoPtr<IBinder> ksh;
        ks->GetToken((IBinder**)&ksh);
        AutoPtr<KeySetHandle> h = reinterpret_cast<KeySetHandle*>(ksh->Probe(EIID_KeySetHandle));
        if (h != NULL) {
            *result = mSettings->mKeySetManagerService->PackageIsSignedByExactlyLPr(packageName, h);
            return NOERROR;
        }
        *result = FALSE;
    }
    return NOERROR;
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
