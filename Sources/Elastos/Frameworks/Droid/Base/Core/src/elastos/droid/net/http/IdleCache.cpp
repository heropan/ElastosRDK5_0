
#include "elastos/droid/net/http/IdleCache.h"

using Elastos::Droid::Os::ISystemClock;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Int32 IdleCache::IDLE_CACHE_MAX = 8;
const Int32 IdleCache::EMPTY_CHECK_MAX = 5;
const Int32 IdleCache::TIMEOUT = 6 * 1000;
const Int32 IdleCache::CHECK_INTERVAL = 2 * 1000;

IdleCache::IdleCache()
    : mCount(0)
    , mCached(0)
    , mReused(0)
{
#if 0 // TODO: Translate codes below
    mEntries = ArrayOf<Entry*>::Alloc(IDLE_CACHE_MAX);
    for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
        (*mEntries)[i] = new Entry();
    }
#endif
}

ECode IdleCache::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
        mEntries[i] = new Entry();
    }
#endif
}

ECode IdleCache::CacheConnection(
    /* [in] */ IHttpHost* host,
    /* [in] */ IConnection* connection,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    Boolean ret = FALSE;

    if (HttpLog::LOGV) {
        HttpLog::V(String("IdleCache size ") + StringUtils::Int32ToString(mCount) + String(" host ")/* + host*/);
    }

    if (mCount < IDLE_CACHE_MAX) {
        Int64 time;
        // time = SystemClock::uptimeMillis();
        for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
            Entry* entry = (*mEntries)[i];
            if (entry->mHost == NULL) {
                entry->mHost = host;
                // entry->mConnection = connection;
                entry->mTimeout = time + TIMEOUT;
                mCount++;
                if (HttpLog::LOGV) mCached++;
                ret = true;
                if (mThread == NULL) {
                    mThread = new IdleReaper(this);
                    mThread->Start();
                }
                break;
            }
        }
    }
    return ret;
#endif
}

ECode IdleCache::GetConnection(
    /* [in] */ IHttpHost* host,
    /* [out] */ IConnection** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    Connection* ret = NULL;

    if (mCount > 0) {
        for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
            Entry* entry = (*mEntries)[i];
            AutoPtr<IHttpHost> eHost = entry->mHost;
            // TODO:
            if (eHost != NULL /*&& eHost->Equals(host)*/) {
                ret = entry->mConnection;
                entry->mHost = NULL;
                entry->mConnection = NULL;
                mCount--;
                if (HttpLog::LOGV) mReused++;
                break;
            }
        }
    }

    *conn = ret;
    return NOERROR;
#endif
}

ECode IdleCache::Clear()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    for (Int32 i = 0; mCount > 0 && i < IDLE_CACHE_MAX; i++) {
        Entry* entry = (*mEntries)[i];
        if (entry->mHost != NULL) {
            entry->mHost = NULL;
            // entry->mConnection->CloseConnection();
            // entry->mConnection = NULL;
            mCount--;
        }
    }

    return NOERROR;
#endif
}

ECode IdleCache::ClearIdle()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    if (mCount > 0) {
        Int64 time;
        // time = SystemClock::uptimeMillis();
        for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
            Entry* entry = (*mEntries)[i];
            if (entry->mHost != NULL && time > entry->mTimeout) {
                entry->mHost = NULL;
                // entry->mConnection->CloseConnection();
                // entry->mConnection = NULL;
                mCount--;
            }
        }
    }

    return NOERROR;
#endif
}

//===================================================
// IdleCache::IdleReaper
//===================================================
ECode IdleCache::IdleReaper::Run()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Int32 check = 0;

    SetName(String("IdleReaper"));
    // AutoPtr<Elastos::Os::IProcessHelper> helper;
    // Elastos::os::CProcessHelper::AcquireSingleton((Elastos::Os::IProcessHelper**)&helper);
    // helper->SetThreadPriority(
    //         Elastos::os::Process::THREAD_PRIORITY_BACKGROUND);

    {
        AutoLock lock(mParent->mLock);

        while (check < EMPTY_CHECK_MAX) {
            Wait(CHECK_INTERVAL);

            if (mParent->mCount == 0) {
                check++;
            } else {
                check = 0;
                mParent->ClearIdle();
            }
        }
        mParent->mThread = NULL;
    }

    if (HttpLog::LOGV) {
        HttpLog::V(String("IdleCache IdleReaper shutdown: cached ") + StringUtils::Int32ToString(mParent->mCached) +
                  String(" reused ") + StringUtils::Int32ToString(mParent->mReused));
        mParent->mCached = 0;
        mParent->mReused = 0;
    }

    return NOERROR;
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
