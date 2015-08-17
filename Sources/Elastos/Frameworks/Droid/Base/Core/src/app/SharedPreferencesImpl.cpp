
#include <ext/frameworkdef.h>
#include "app/CActivityThread.h"
#include "app/CContextImpl.h"
//#include "app/QueuedWork.h"
#include "util/XmlUtils.h"
#include "app/SharedPreferencesImpl.h"
#include <os/FileUtils.h>
#include <os/Looper.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::CBoolean;
using Elastos::Core::CFloat;
using Elastos::Core::CInteger32;
using Elastos::Core::CInteger64;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CString;
using Elastos::Core::IBoolean;
using Elastos::Core::IFloat;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::IBlockGuard;
using Elastos::Core::CBlockGuard;
using Elastos::Core::IBlockGuardPolicy;
using Elastos::Droid::App::CActivityThread;
//using Elastos::Droid::App::QueuedWork;
using Elastos::Droid::Content::EIID_ISharedPreferences;
using Elastos::Droid::Content::EIID_ISharedPreferencesEditor;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CFile;
using Elastos::IO::CFileInputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::CIoUtils;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::ICloseable;
using Elastos::IO::IIoUtils;
using Elastos::Utility::Concurrent::CCountDownLatch;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::CObjectMap;
using Elastos::Utility::CHashSet;
using Libcore::IO::CLibcore;
using Libcore::IO::CStructStat;
using Libcore::IO::ILibcore;
using Libcore::IO::IOs;
using Libcore::IO::IStructStat;


namespace Elastos {
namespace Droid {
namespace App {


//==============================================================================
//          ++ SharedPreferencesImpl::MemoryCommitResult ++
//==============================================================================

SharedPreferencesImpl::MemoryCommitResult::MemoryCommitResult()
    : mChangesMade(FALSE)
    , mWriteToDiskResult(FALSE)
{
    CCountDownLatch::New(1,(ICountDownLatch**)&mWrittenToDiskLatch);
}

void SharedPreferencesImpl::MemoryCommitResult::SetDiskWriteResult(
    /* [in] */ Boolean result)
{
    mWriteToDiskResult = result;
    mWrittenToDiskLatch->CountDown();
}

//==============================================================================
//          -- SharedPreferencesImpl::MemoryCommitResult --
//==============================================================================


//==============================================================================
//          ++ SharedPreferencesImpl::EditorImpl ++
//==============================================================================

SharedPreferencesImpl::EditorImpl::AwaitCommitRunnable::AwaitCommitRunnable(
    /* [in] */ MemoryCommitResult* mcr)
    : mMcr(mcr)
{
}

ECode SharedPreferencesImpl::EditorImpl::AwaitCommitRunnable::Run()
{
    mMcr->mWrittenToDiskLatch->Await();
    return NOERROR;
}

SharedPreferencesImpl::EditorImpl::PostWriteRunnable::PostWriteRunnable(
    /* [in] */ AwaitCommitRunnable* awaitCommitRunnable)
    : mAwaitCommitRunnable(awaitCommitRunnable)
{
}

ECode SharedPreferencesImpl::EditorImpl::PostWriteRunnable::Run()
{
    mAwaitCommitRunnable->Run();
    //TODO
    Slogger::W(TAG, "[TODO] PostWriteRunnable::Run == There need the class QueuedWork...");
    //QueuedWork::Remove(mAwaitCommitRunnable);
    return NOERROR;
}

SharedPreferencesImpl::EditorImpl::NotifyListenersRunnable::NotifyListenersRunnable(
    /* [in] */ EditorImpl* host,
    /* [in] */ MemoryCommitResult* mcr)
    : mHost(host)
    , mMcr(mcr)
{
}

ECode SharedPreferencesImpl::EditorImpl::NotifyListenersRunnable::Run()
{
    mHost->NotifyListeners(mMcr);
    return NOERROR;
}

SharedPreferencesImpl::EditorImpl::EditorImpl(
    /* [in] */ SharedPreferencesImpl* host)
    : mClear(FALSE)
    , mHost(host)
{
}

CAR_INTERFACE_IMPL(SharedPreferencesImpl::EditorImpl, ISharedPreferencesEditor);

ECode SharedPreferencesImpl::EditorImpl::PutString(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    AutoLock lock(this);
    AutoPtr<ICharSequence> pValue;
    CString::New(value, (ICharSequence**)&pValue);
    mModified[key] = pValue;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::PutStringSet(
    /* [in] */ const String& key,
    /* [in] */ ISet* values)
{
    AutoLock lock(this);
    AutoPtr<ISet> newValues;
    if (values != NULL) {
        CHashSet::New(values, (ISet**)&newValues);
    }
    mModified[key] = newValues;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    AutoLock lock(this);
    AutoPtr<IInteger32> pValue;
    CInteger32::New(value, (IInteger32**)&pValue);
    mModified[key] = pValue;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    AutoLock lock(this);
    AutoPtr<IInteger64> pValue;
    CInteger64::New(value, (IInteger64**)&pValue);
    mModified[key] = pValue;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    AutoLock lock(this);
    AutoPtr<IFloat> pValue;
    CFloat::New(value, (IFloat**)&pValue);
    mModified[key] = pValue;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    AutoLock lock(this);
    AutoPtr<IBoolean> pValue;
    CBoolean::New(value, (IBoolean**)&pValue);
    mModified[key] = pValue;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::Remove(
    /* [in] */ const String& key)
{
    AutoLock lock(this);
    mModified[key] = this;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::Clear()
{
    AutoLock lock(this);
    mClear = TRUE;
    return NOERROR;
}

ECode SharedPreferencesImpl::EditorImpl::Apply()
{
    AutoPtr<MemoryCommitResult> mcr = CommitToMemory();
    AutoPtr<AwaitCommitRunnable> awaitCommitRunnable = new AwaitCommitRunnable(mcr);

    //TODO
    //QueuedWork::Add(awaitCommitRunnable);
    Slogger::W(TAG, "[TODO] Apply == There need the class QueuedWork...");
    AutoPtr<PostWriteRunnable> postWriteRunnable = new PostWriteRunnable(awaitCommitRunnable);
    mHost->EnqueueDiskWrite(mcr, postWriteRunnable);

    // Okay to notify the listeners before it's hit disk
    // because the listeners should always get the same
    // SharedPreferences instance back, which has the
    // changes reflected in memory.
    NotifyListeners(mcr);
    return NOERROR;
}

AutoPtr<SharedPreferencesImpl::MemoryCommitResult> SharedPreferencesImpl::EditorImpl::CommitToMemory()
{
    AutoPtr<SharedPreferencesImpl::MemoryCommitResult> mcr = new MemoryCommitResult();
    {
        AutoLock lock(mHost);
        // We optimistically don't make a deep copy until
        // a memory commit comes in when we're already
        // writing to disk.
        mcr->mMapToWriteToDisk = mHost->mMap;
        mHost->mDiskWritesInFlight++;

        Boolean hasListeners = mHost->mListeners.IsEmpty() == FALSE;
        if (hasListeners) {
            HashMap<AutoPtr<ISharedPreferencesOnSharedPreferenceChangeListener>, AutoPtr<IInterface> > \
                ::Iterator listenerIt;
            for (listenerIt = mHost->mListeners.Begin(); listenerIt != mHost->mListeners.End(); ++listenerIt) {
                mcr->mListeners.Insert(listenerIt->mFirst);
            }
        }

        {
            AutoLock lock(this);
            if (mClear) {
                if (!mHost->mMap->IsEmpty()) {
                    mcr->mChangesMade = TRUE;
                    mHost->mMap->Clear();
                }
                mClear = FALSE;
            }
            HashMap<String, AutoPtr<IInterface> >::Iterator it;
            for(it = mModified.Begin(); it != mModified.End(); ++it) {
                String k = it->mFirst;
                AutoPtr<IInterface> v = it->mSecond;
                HashMap<String, AutoPtr<IInterface> >::Iterator itr = mHost->mMap->Find(k);
                if (v == this) {  // magic value for a removal mutation
                    if (itr == mHost->mMap->End()) {
                        continue;
                    }
                    mHost->mMap->Erase(itr);
                } else {
                    // Boolean isSame = FALSE;
                    if (itr != mHost->mMap->End()) {
                        AutoPtr<IInterface> existingValue = itr->mSecond;
                        if (existingValue != NULL && existingValue == v) {
                            continue;
                        }
                    }
                    (*mHost->mMap)[k] = v;
                }

                mcr->mChangesMade = TRUE;
                if (hasListeners) {
                    mcr->mKeysModified.PushBack(k);
                }
            }
        mModified.Clear();
        }
    }
    return mcr;
}

ECode SharedPreferencesImpl::EditorImpl::Commit(
    /* [out] */ Boolean* succeded)
{
    VALIDATE_NOT_NULL(succeded);
    AutoPtr<MemoryCommitResult> mcr = CommitToMemory();
    mHost->EnqueueDiskWrite(
        mcr, NULL /* sync write on this thread okay */);

    ECode ec = mcr->mWrittenToDiskLatch->Await();
    if (FAILED(ec)) {
        *succeded = FALSE;
        return NOERROR;
    }
    NotifyListeners(mcr);
    *succeded = mcr->mWriteToDiskResult;
    return NOERROR;
}

void SharedPreferencesImpl::EditorImpl::NotifyListeners(
    /* [in] */ MemoryCommitResult* mcr)
{
    if (mcr->mKeysModified.IsEmpty()) {
        return;
    }
    if (Looper::GetMyLooper() == Looper::GetMainLooper()) {
        List<String>::ReverseIterator it;
        for (it = mcr->mKeysModified.RBegin(); it != mcr->mKeysModified.REnd(); ++it) {
            const String key = (String)*it;
            HashSet<AutoPtr<ISharedPreferencesOnSharedPreferenceChangeListener> >::Iterator itr;
            for (itr = mcr->mListeners.Begin(); itr != mcr->mListeners.End(); ++itr) {
                AutoPtr<ISharedPreferencesOnSharedPreferenceChangeListener> listener
                        = *itr;
                if (listener != NULL) {
                    listener->OnSharedPreferenceChanged(mHost, key);
                }
            }
        }
    } else {
        AutoPtr<NotifyListenersRunnable> notifyListenersRunnable = new NotifyListenersRunnable(this, mcr);
        // Run this function on the main thread.
        Boolean isSuccess = FALSE;
        CActivityThread::sMainThreadHandler->Post(notifyListenersRunnable, &isSuccess);
    }
}

//==============================================================================
//          -- SharedPreferencesImpl::EditorImpl --
//==============================================================================

SharedPreferencesImpl::LoadFromDiskLockedRunnable::LoadFromDiskLockedRunnable(
    /* [in] */ SharedPreferencesImpl* host)
    : mHost(host)
{
}

ECode SharedPreferencesImpl::LoadFromDiskLockedRunnable::Run()
{
    AutoLock lock(mHost);
    mHost->LoadFromDiskLocked();
    return NOERROR;
}

SharedPreferencesImpl::WriteToDiskRunnable::WriteToDiskRunnable(
    /* [in] */ MemoryCommitResult* mcr,
    /* [in] */ Runnable* postWriteRunnable,
    /* [in] */ SharedPreferencesImpl* host)
    : mHost(host)
    , mMcr(mcr)
    , mPostWriteRunnable(postWriteRunnable)
{
}

ECode SharedPreferencesImpl::WriteToDiskRunnable::Run()
{
    {
        AutoLock lock(mHost->mWritingToDiskLock);
        mHost->WriteToFile(mMcr);
    }
    {
        AutoLock lock(mHost);
        mHost->mDiskWritesInFlight--;
    }
    if (mPostWriteRunnable != NULL) {
        mPostWriteRunnable->Run();
    }
    return NOERROR;
}


const String SharedPreferencesImpl::TAG("SharedPreferencesImpl");
const Boolean SharedPreferencesImpl::DEBUG = FALSE;
AutoPtr<IInterface> SharedPreferencesImpl::mContent; // = new Object();//TODO

SharedPreferencesImpl::SharedPreferencesImpl(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode)
    : mFile(file)
    , mMode(mode)
    , mDiskWritesInFlight(0)
    , mLoaded(FALSE)
    , mStatTimestamp(0)
    , mStatSize(0)
{
    mMap = new HashMap<String, AutoPtr<IInterface> >();
    mBackupFile = MakeBackupFile(file);
    StartLoadFromDisk();
}

PInterface SharedPreferencesImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_ISharedPreferences) {
        return (PInterface)(ISharedPreferences*)this;
    }
    return NULL;
}

UInt32 SharedPreferencesImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SharedPreferencesImpl::Release()
{
    return ElRefBase::Release();
}

ECode SharedPreferencesImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void SharedPreferencesImpl::StartLoadFromDisk()
{
    {
        AutoLock lock(this);
        mLoaded = FALSE;
    }
    LoadFromDiskLockedRunnable* runnable = new LoadFromDiskLockedRunnable(this);
    AutoPtr<IThread> thread;
    CThread::New(runnable, String("SharedPreferencesImpl-load"), (IThread**)&thread);
    thread->Start();
}

void SharedPreferencesImpl::LoadFromDiskLocked()
{
    if (mLoaded) {
        return;
    }
    Boolean isExist = FALSE;
    mBackupFile->Exists(&isExist);
    if (isExist) {
        Boolean value = FALSE;
        mFile->Delete(&value);
        mBackupFile->RenameTo(mFile, &value);
    }

    // Debugging
    Boolean isCanRead = FALSE;
    if ((mFile->Exists(&isCanRead), isCanRead) && !(mFile->CanRead(&isCanRead), isCanRead)) {
        Slogger::W(TAG, "Attempt to read preferences file %p without permission", mFile.Get());
    }

    AutoPtr<IObjectMap> map;
    AutoPtr<IStructStat> stat;

    do {
        ECode ec = NOERROR;
        String path;
        mFile->GetPath(&path);
        AutoPtr<ILibcore> libcore;
        CLibcore::AcquireSingleton((ILibcore**)&libcore);
        AutoPtr<IOs> os;
        libcore->GetOs((IOs**)&os);
        ec = os->Stat(path, (IStructStat**)&stat);
        if (FAILED(ec)) {
            break;
        }

        mFile->CanRead(&isCanRead);
        if (isCanRead) {
            AutoPtr<IFileInputStream> fileStr;
            ec = CFileInputStream::New(mFile, (IFileInputStream**)&fileStr);
            if (FAILED(ec)) {
                break;
            }
            AutoPtr<IIoUtils> ioUtils;
            CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
            AutoPtr<IBufferedInputStream> str;
            AutoPtr<ICloseable> closeable = ICloseable::Probe(str);
            ec = CBufferedInputStream::New(fileStr, 16*1024, (IBufferedInputStream**)&str);
            if (FAILED(ec)) {
                ioUtils->CloseQuietly(closeable);
                break;
            }

            map = XmlUtils::ReadMapXml(str);
            ioUtils->CloseQuietly(closeable);
        }
    } while (FALSE);

    mLoaded = TRUE;
    if (map != NULL) {
        mMap->Clear();
        AutoPtr<ArrayOf<IInterface*> > keys;
        map->GetKeys((ArrayOf<IInterface*>**)&keys);
        assert(keys != NULL);
        String key;
        for (Int32 i = 0; i < keys->GetLength(); i++) {
            assert(ICharSequence::Probe((*keys)[i]) != NULL);
            ICharSequence::Probe((*keys)[i])->ToString(&key);
            AutoPtr<IInterface> value;
            map->Get((*keys)[i], (IInterface**)&value);
            (*mMap)[key] = value;
        }

        stat->GetMtime(&mStatTimestamp);
        stat->GetSize(&mStatSize);
    }

    NotifyAll();
}


AutoPtr<IFile> SharedPreferencesImpl::MakeBackupFile(
    /* [in] */ IFile* prefsFile)
{
    AutoPtr<IFile> file;
    String fileName;
    prefsFile->GetPath(&fileName);
    fileName = fileName + ".bak";
    CFile::New(fileName, (IFile**)&file);
    return file;
}

void SharedPreferencesImpl::StartReloadIfChangedUnexpectedly()
{
    AutoLock lock(this);
    // TODO: wait for any pending writes to disk?
    if (!HasFileChangedUnexpectedly()) {
        return;
    }
    StartLoadFromDisk();
}

Boolean SharedPreferencesImpl::HasFileChangedUnexpectedly()
{
    {
        AutoLock lock(this);
        if (mDiskWritesInFlight > 0) {
            // If we know we caused it, it's not unexpected.
            //if (DEBUG) Log.d(TAG, "disk write in flight, not unexpected.");
            return FALSE;
        }
    }

    AutoPtr<IStructStat> stat;
    /*
     * Metadata operations don't usually count as a block guard
     * violation, but we explicitly want this one.
     */
    AutoPtr<IBlockGuard> helper;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&helper);
    AutoPtr<IBlockGuardPolicy> policy;
    helper->GetThreadPolicy((IBlockGuardPolicy**)&policy);
    policy->OnReadFromDisk();
    String path;
    mFile->GetPath(&path);
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    ECode ec = os->Stat(path, (IStructStat**)&stat);
    if(FAILED(ec)) {
        return TRUE;
    }

    {
        AutoLock lock(this);
        Int64 st_mtime = 0;
        Int64 st_size = 0;
        stat->GetMtime(&st_mtime);
        stat->GetSize(&st_size);
        return mStatTimestamp != st_mtime || mStatSize != st_size;
    }
}

ECode SharedPreferencesImpl::RegisterOnSharedPreferenceChangeListener(
    /* [in] */ ISharedPreferencesOnSharedPreferenceChangeListener* listener)
{
    AutoLock lock(this);
    mListeners[listener] = mContent;
    return NOERROR;
}

ECode SharedPreferencesImpl::UnregisterOnSharedPreferenceChangeListener(
    /* [in] */ ISharedPreferencesOnSharedPreferenceChangeListener* listener)
{
    AutoLock lock(this);
    HashMap<AutoPtr<ISharedPreferencesOnSharedPreferenceChangeListener>, AutoPtr<IInterface> >::Iterator it = mListeners.Find(listener);
    mListeners.Erase(it);
    return NOERROR;
}

void SharedPreferencesImpl::AwaitLoadedLocked()
{
    if (!mLoaded) {
        // Raise an explicit StrictMode onReadFromDisk for this
        // thread, since the real read will be in a different
        // thread and otherwise ignored by StrictMode.

        // TODO CBlockGuard
        // AutoPtr<IBlockGuard> helper;
        // CBlockGuard::AcquireSingleton((IBlockGuard**)&helper);
        // AutoPtr<IBlockGuardPolicy> policy;
        // helper->GetThreadPolicy((IBlockGuardPolicy**)&policy);
        // policy->OnReadFromDisk();
    }
    while (!mLoaded) {
        Wait();
    }
}

ECode SharedPreferencesImpl::GetAll(
    /* [out] */ IObjectMap** result)
{
    VALIDATE_NOT_NULL(result);
    CObjectMap::New(result);

    AutoLock lock(this);
    AwaitLoadedLocked();
    //noinspection unchecked
    HashMap<String, AutoPtr<IInterface> >::Iterator it;
    for (it = mMap->Begin(); it != mMap->End(); ++it) {
        AutoPtr<ICharSequence> key;
        CString::New(it->mFirst, (ICharSequence**)&key);
        (*result)->Put(key, it->mSecond);
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::GetString(
    /* [in] */ const String& key,
    /* [in] */ const String& defValue,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    AutoLock lock(this);
    *value = defValue;

    AwaitLoadedLocked();
    if (key.IsNull()) {
        return NOERROR;
    }

    if (mMap->Find(key) != mMap->End()) {
        AutoPtr<IInterface> obj = mMap->Find(key)->mSecond;
        if (obj != NULL) {
            AutoPtr<ICharSequence> pValue = ICharSequence::Probe(obj);
            pValue->ToString(value);
        }
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::GetStringSet(
    /* [in] */ const String& key,
    /* [in] */ ISet* defValues,
    /* [out] */ ISet** values)
{
    VALIDATE_NOT_NULL(values)
    AutoLock lock(this);
    AwaitLoadedLocked();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    AutoPtr<ISet> v;
    if (it != mMap->End()) {
        v = ISet::Probe(it->mSecond);
    }
    *values = v;
    REFCOUNT_ADD(*values);
    return NOERROR;
}

ECode SharedPreferencesImpl::GetInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoLock lock(this);
    *value = defValue;

    AwaitLoadedLocked();
    if (key.IsNull()) {
        return NOERROR;
    }

    if (mMap->Find(key) != mMap->End()) {
        AutoPtr<IInterface> obj = mMap->Find(key)->mSecond;
        if (obj != NULL) {
            AutoPtr<IInteger32> pValue = IInteger32::Probe(obj);
            pValue->GetValue(value);
        }
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::GetInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 defValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    AutoLock lock(this);
    *value = defValue;

    AwaitLoadedLocked();
    if (key.IsNull()) {
        return NOERROR;
    }

    if (mMap->Find(key) != mMap->End()) {
        AutoPtr<IInterface> obj = mMap->Find(key)->mSecond;
        if (obj != NULL) {
            AutoPtr<IInteger64> pValue = IInteger64::Probe(obj);
            pValue->GetValue(value);
        }
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::GetFloat(
    /* [in] */ const String& key,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    AutoLock lock(this);
    *value = defValue;

    AwaitLoadedLocked();
    if (key.IsNull()) {
        return NOERROR;
    }

    if (mMap->Find(key) != mMap->End()) {
        AutoPtr<IInterface> obj = mMap->Find(key)->mSecond;
        if (obj != NULL) {
            AutoPtr<IFloat> pValue = IFloat::Probe(obj);
            pValue->GetValue(value);
        }
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::GetBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean defValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    AutoLock lock(this);
    *value = defValue;

    AwaitLoadedLocked();
    if (key.IsNull()) {
        return NOERROR;
    }

    if (mMap->Find(key) != mMap->End()) {
        AutoPtr<IInterface> obj = mMap->Find(key)->mSecond;
        if (obj != NULL) {
            AutoPtr<IBoolean> pValue = IBoolean::Probe(obj);
            pValue->GetValue(value);
        }
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::Contains(
    /* [in] */ const String& key,
    /* [out] */ Boolean* isContain) {
    VALIDATE_NOT_NULL(isContain);

    AutoLock lock(this);
    AwaitLoadedLocked();
    if (key.IsNull()) {
        *isContain = FALSE;
        return NOERROR;
    }

    if (mMap->Find(key) != mMap->End()) {
        *isContain = TRUE;
    } else {
        *isContain = FALSE;
    }
    return NOERROR;
}

ECode SharedPreferencesImpl::Edit(
    /* [out] */ ISharedPreferencesEditor** result)
{
    // TODO: remove the need to call awaitLoadedLocked() when
    // requesting an editor.  will require some work on the
    // Editor, but then we should be able to do:
    //
    //      context.getSharedPreferences(..).edit().putString(..).apply()
    //
    // ... all without blocking.
    VALIDATE_NOT_NULL(result);
    {
        AutoLock lock(this);
        AwaitLoadedLocked();
    }
    AutoPtr<EditorImpl> editorImpl = new EditorImpl(this);
    *result = (ISharedPreferencesEditor*)editorImpl->Probe(EIID_ISharedPreferencesEditor);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

void SharedPreferencesImpl::EnqueueDiskWrite(
    /* [in] */ MemoryCommitResult* mcr,
    /* [in] */ Runnable* postWriteRunnable)
{
    AutoPtr<WriteToDiskRunnable> writeToDiskRunnable =
        new WriteToDiskRunnable(mcr, postWriteRunnable, this);

    const Boolean isFromSyncCommit = (postWriteRunnable == NULL);

    // Typical #commit() path with fewer allocations, doing a write on
    // the current thread.
    if (isFromSyncCommit) {
        Boolean wasEmpty = FALSE;
        {
            AutoLock lock(this);
            wasEmpty = mDiskWritesInFlight == 1;
        }
        if (wasEmpty) {
            writeToDiskRunnable->Run();
            return;
        }
    }

    Slogger::W(TAG, "[TODO] EnqueueDiskWrite == There need the class QueuedWork...");
    writeToDiskRunnable->Run();
    //TODO
    // AutoPtr<IExecutorService> singleThreadExecutor;
    // QueuedWork::SingleThreadExecutor(singleThreadExecutor);
    // assert(singleThreadExecutor != NULL);
    // singleThreadExecutor->Execute(writeToDiskRunnable);
}

AutoPtr<IFileOutputStream> SharedPreferencesImpl::CreateFileOutputStream(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> str;
    if (CFileOutputStream::New(file, (IFileOutputStream**)&str) == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
        AutoPtr<IFile> parent;
        file->GetParentFile((IFile**)&parent);
        Boolean successed = FALSE;

        if (!(parent->Mkdir(&successed), successed)) {
            Slogger::E(TAG, "Couldn't create directory for SharedPreferences file %p", file);
            return NULL;
        }
        String path;
        parent->GetPath(&path);
        FileUtils::SetPermissions(
            path,
            FileUtils::sS_IRWXU | FileUtils::sS_IRWXG | FileUtils::sS_IXOTH,
            -1, -1);

        str = NULL;
        if (CFileOutputStream::New(file, (IFileOutputStream**)&str) == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
            Slogger::E(TAG, "Couldn't create SharedPreferences file %p", file);
        }
    }
    return str;
}

void SharedPreferencesImpl::WriteToFile(
    /* [in] */ MemoryCommitResult* mcr)
{
    // Rename the current file so it may be used as a backup during the next read
    Boolean isExist = FALSE;
    mFile->Exists(&isExist);
    if (isExist) {
        if (!mcr->mChangesMade) {
            // If the file already exists, but no changes were
            // made to the underlying map, it's wasteful to
            // re-write the file.  Return as if we wrote it
            // out.
            mcr->SetDiskWriteResult(TRUE);
            return;
        }
        Boolean isExistFile = FALSE;
        mBackupFile->Exists(&isExistFile);
        if (!isExistFile) {
            Boolean isSuccess = FALSE;
            mFile->RenameTo(mBackupFile, &isSuccess);
            if (!isSuccess) {
                //Log.e(TAG, "Couldn't rename file " + mFile
                //      + " to backup file " + mBackupFile);
                mcr->SetDiskWriteResult(FALSE);
                return;
            }
        } else {
            Boolean isDelete = FALSE;
            mFile->Delete(&isDelete);
        }
    }

    // Attempt to write the file, delete the backup and return true as atomically as
    // possible.  If any exception occurs, delete the new file; next time we will restore
    // from the backup.

    AutoPtr<IFileOutputStream> str = CreateFileOutputStream(mFile);
    if (str == NULL) {
        mcr->SetDiskWriteResult(FALSE);
        return;
    }

    assert(mcr->mMapToWriteToDisk != NULL);
    AutoPtr<IObjectMap> map;
    CObjectMap::New((IObjectMap**)&map);
    HashMap<String, AutoPtr<IInterface> >::Iterator ator = mcr->mMapToWriteToDisk->Begin();
    for (; ator != mcr->mMapToWriteToDisk->End(); ++ator) {
        AutoPtr<ICharSequence> key;
        CString::New(ator->mFirst, (ICharSequence**)&key);
        map->Put(key, ator->mSecond);
    }

    Boolean isSync = FALSE, isDeleteFile = FALSE;
    String path;
    AutoPtr<IStructStat> stat;
    AutoPtr<ILibcore> libcore;
    AutoPtr<IOs> os;
    FAIL_GOTO(XmlUtils::WriteMapXml(map, str), failed);
    FileUtils::Sync(str, &isSync);
    FAIL_GOTO(str->Close(), failed);
    FAIL_GOTO(mFile->GetPath(&path), failed);
    CContextImpl::SetFilePermissionsFromMode(path, mMode, 0);

    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    FAIL_GOTO(libcore->GetOs((IOs**)&os), failed);
    FAIL_GOTO(os->Stat(path, (IStructStat**)&stat), failed);
    {
        AutoLock lock(this);
        stat->GetMtime(&mStatTimestamp);
        stat->GetSize(&mStatSize);
    }

    // Writing was successful, delete the backup file if there is one.
    FAIL_GOTO(mBackupFile->Delete(&isDeleteFile), failed);
    mcr->SetDiskWriteResult(TRUE);
    return;

failed:
    // Clean up an unsuccessfully written file
    if (isExist) {
        Boolean mFileIsDelete = FALSE;
        mFile->Delete(&mFileIsDelete);
        if (mFileIsDelete) {
            //Log.e(TAG, "Couldn't clean up partially-written file " + mFile);
        }
    }
    mcr->SetDiskWriteResult(FALSE);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
