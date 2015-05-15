
#ifndef __CSEMAPHORE_H__
#define __CSEMAPHORE_H__

#include "cmdef.h"
#include "_CSemaphore.h"
#include "AbstractQueuedSynchronizer.h"

using Elastos::Utility::Concurrent::Locks::ICondition;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CSemaphore)
{
public:
    enum CLSID {
        CLSID_Sync,
        CLSID_NonfairSync,
        CLSID_FairSync,
    };

    class Sync
        : public ElLightRefBase
        , public Locks::AbstractQueuedSynchronizer
    {
    public:
        Sync(
            /* [in] */ Int32 permits);

        virtual CARAPI_(Sync*) Probe(
            /* [in] */ Int32 clsID) = 0;

        CARAPI GetPermits(
            /* [out] */ Int32* out);

        CARAPI NonfairTryAcquireShared(
            /* [in] */ Int32 acquires,
            /* [out] */ Int32* out);

        CARAPI TryReleaseShared(
            /* [in] */ Int32 releases,
            /* [out] */ Boolean* out);

        CARAPI ReducePermits(
            /* [in] */ Int32 reductions);

        CARAPI DrainPermits(
            /* [out] */ Int32* out);

    private:
        static const Int64 serialVersionUID = 1192457210091910933L;
    };

    class NonfairSync : public Sync
    {
    public:
        NonfairSync(
            /* [in] */ Int32 permits);

        CARAPI_(Sync*) Probe(
            /* [in] */ Int32 clsID);

        CARAPI TryAcquireShared(
            /* [in] */ Int32 acquires,
            /* [out] */ Int32* out);

    private:
        static const Int64 serialVersionUID = -2694183684443567898L;
    };

    class FairSync : public Sync
    {
    public:
        FairSync(
            /* [in] */ Int32 permits);

        CARAPI_(Sync*) Probe(
            /* [in] */ Int32 clsID);

        CARAPI TryAcquireShared(
            /* [in] */ Int32 acquires,
            /* [out] */ Int32* out);

    private:
        static const Int64 serialVersionUID = 2014338818796000944L;
    };

public:
    constructor(
        /* [in] */ Int32 permits);

    constructor(
        /* [in] */ Int32 permits,
        /* [in] */ Boolean fair);

    CARAPI Acquire();

    CARAPI AcquireUninterruptibly();

    CARAPI TryAcquire(
        /* [out] */ Boolean* value);

    CARAPI TryAcquireEx(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ Boolean* value);

    CARAPI ToRelease();

    CARAPI AcquireEx(
        /* [in] */ Int32 permits);

    CARAPI AcquireUninterruptiblyEx(
        /* [in] */ Int32 permits);

    CARAPI TryAcquireEx2(
        /* [in] */ Int32 permits,
        /* [out] */ Boolean* value);

    CARAPI TryAcquireEx3(
        /* [in] */ Int32 permits,
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ Boolean* value);

    CARAPI ToReleaseEx(
        /* [in] */ Int32 permits);

    CARAPI AvailablePermits(
        /* [out] */ Int32* value);

    CARAPI DrainPermits(
        /* [out] */ Int32* value);

    CARAPI IsFair(
        /* [out] */ Boolean* value);

    CARAPI HasQueuedThreads(
        /* [out] */ Boolean* value);

    CARAPI GetQueueLength(
        /* [out] */ Int32* value);

    CARAPI ToString(
        /* [out] */ String* str);

protected:
    CARAPI ReducePermits(
        /* [in] */ Int32 reduction);

    CARAPI GetQueuedThreads(
        /* [out] */ ICollection** out);

private:
    static const Int64 serialVersionUID = -3222578661600680210L;
    AutoPtr<Sync> mSync;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CSEMAPHORE_H__