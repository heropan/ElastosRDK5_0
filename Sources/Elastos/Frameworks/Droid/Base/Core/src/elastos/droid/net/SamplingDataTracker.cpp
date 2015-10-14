
#include "elastos/droid/net/SamplingDataTracker.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(SamplingDataTracker, Object, ISamplingDataTracker)

const Boolean SamplingDataTracker::DBG = FALSE;
const String SamplingDataTracker::TAG = String("SamplingDataTracker");

SamplingDataTracker::SamplingDataTracker()
    : MINIMUM_SAMPLING_INTERVAL(15 * 1000)
    , MINIMUM_SAMPLED_PACKETS(30)
    , SAMPLING_DATA_LOCK(CreateSAMPLING_DATA_LOCK())
{}

ECode SamplingDataTracker::constructor()
{
    return NOERROR;
}

ECode SamplingDataTracker::GetSamplingSnapshots(
    /* [in] */ IMap* mapIfaceToSample)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader("/proc/net/dev"));
            // Skip over the line bearing column titles (there are 2 lines)
            String line;
            reader.readLine();
            reader.readLine();
            while ((line = reader.readLine()) != null) {
                // remove leading whitespace
                line = line.trim();
                String[] tokens = line.split("[ ]+");
                if (tokens.length < 17) {
                    continue;
                }
                /* column format is
                 * Interface  (Recv)bytes packets errs drop fifo frame compressed multicast \
                 *            (Transmit)bytes packets errs drop fifo colls carrier compress
                */
                String currentIface = tokens[0].split(":")[0];
                if (DBG) Slog.d(TAG, "Found data for interface " + currentIface);
                if (mapIfaceToSample.containsKey(currentIface)) {
                    try {
                        SamplingSnapshot ss = new SamplingSnapshot();
                        ss.mTxByteCount        = Long.parseLong(tokens[1]);
                        ss.mTxPacketCount      = Long.parseLong(tokens[2]);
                        ss.mTxPacketErrorCount = Long.parseLong(tokens[3]);
                        ss.mRxByteCount        = Long.parseLong(tokens[9]);
                        ss.mRxPacketCount      = Long.parseLong(tokens[10]);
                        ss.mRxPacketErrorCount = Long.parseLong(tokens[11]);
                        ss.mTimestamp          = SystemClock.elapsedRealtime();
                        if (DBG) {
                            Slog.d(TAG, "Interface = " + currentIface);
                            Slog.d(TAG, "ByteCount = " + String.valueOf(ss.mTxByteCount));
                            Slog.d(TAG, "TxPacketCount = " + String.valueOf(ss.mTxPacketCount));
                            Slog.d(TAG, "TxPacketErrorCount = "
                                    + String.valueOf(ss.mTxPacketErrorCount));
                            Slog.d(TAG, "RxByteCount = " + String.valueOf(ss.mRxByteCount));
                            Slog.d(TAG, "RxPacketCount = " + String.valueOf(ss.mRxPacketCount));
                            Slog.d(TAG, "RxPacketErrorCount = "
                                    + String.valueOf(ss.mRxPacketErrorCount));
                            Slog.d(TAG, "Timestamp = " + String.valueOf(ss.mTimestamp));
                            Slog.d(TAG, "---------------------------");
                        }
                        mapIfaceToSample.put(currentIface, ss);
                    } catch (NumberFormatException e) {
                        // just ignore this data point
                    }
                }
            }
            if (DBG) {
                Iterator it = mapIfaceToSample.entrySet().iterator();
                while (it.hasNext()) {
                    Map.Entry kvpair = (Map.Entry)it.next();
                    if (kvpair.getValue() == null) {
                        Slog.d(TAG, "could not find snapshot for interface " + kvpair.getKey());
                    }
                }
            }
        } catch(FileNotFoundException e) {
            Slog.e(TAG, "could not find /proc/net/dev");
        } catch (IOException e) {
            Slog.e(TAG, "could not read /proc/net/dev");
        } finally {
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                Slog.e(TAG, "could not close /proc/net/dev");
            }
        }

#endif
}

ECode SamplingDataTracker::StartSampling(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            mLastSample = s;
        }

#endif
}

ECode SamplingDataTracker::StopSampling(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mLastSample != null) {
                if (s.mTimestamp - mLastSample.mTimestamp > MINIMUM_SAMPLING_INTERVAL
                        && getSampledPacketCount(mLastSample, s) > MINIMUM_SAMPLED_PACKETS) {
                    mBeginningSample = mLastSample;
                    mEndingSample = s;
                    mLastSample = null;
                } else {
                    if (DBG) Slog.d(TAG, "Throwing current sample away because it is too small");
                }
            }
        }

#endif
}

ECode SamplingDataTracker::ResetSamplingData()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (DBG) Slog.d(TAG, "Resetting sampled network data");
        synchronized(mSamplingDataLock) {
            // We could just take another sample here and treat it as an
            // 'ending sample' effectively shortening sampling interval, but that
            // requires extra work (specifically, reading the sample needs to be
            // done asynchronously)
            mLastSample = null;
        }

#endif
}

ECode SamplingDataTracker::GetSampledTxByteCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return mEndingSample.mTxByteCount - mBeginningSample.mTxByteCount;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampledTxPacketCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return mEndingSample.mTxPacketCount - mBeginningSample.mTxPacketCount;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampledTxPacketErrorCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return mEndingSample.mTxPacketErrorCount - mBeginningSample.mTxPacketErrorCount;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampledRxByteCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return mEndingSample.mRxByteCount - mBeginningSample.mRxByteCount;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampledRxPacketCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return mEndingSample.mRxPacketCount - mBeginningSample.mRxPacketCount;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampledPacketCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return getSampledPacketCount(mBeginningSample, mEndingSample);

#endif
}

ECode SamplingDataTracker::GetSampledPacketCount(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* begin,
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* end,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (begin != null && end != null) {
            long rxPacketCount = end.mRxPacketCount - begin.mRxPacketCount;
            long txPacketCount = end.mTxPacketCount - begin.mTxPacketCount;
            return rxPacketCount + txPacketCount;
        } else {
            return LinkQualityInfo.UNKNOWN_LONG;
        }

#endif
}

ECode SamplingDataTracker::GetSampledPacketErrorCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mBeginningSample != null && mEndingSample != null) {
            long rxPacketErrorCount = getSampledRxPacketErrorCount();
            long txPacketErrorCount = getSampledTxPacketErrorCount();
            return rxPacketErrorCount + txPacketErrorCount;
        } else {
            return LinkQualityInfo.UNKNOWN_LONG;
        }

#endif
}

ECode SamplingDataTracker::GetSampledRxPacketErrorCount(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return mEndingSample.mRxPacketErrorCount - mBeginningSample.mRxPacketErrorCount;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampleTimestamp(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mEndingSample != null) {
                return mEndingSample.mTimestamp;
            } else {
                return LinkQualityInfo.UNKNOWN_LONG;
            }
        }

#endif
}

ECode SamplingDataTracker::GetSampleDuration(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            if (mBeginningSample != null && mEndingSample != null) {
                return (int) (mEndingSample.mTimestamp - mBeginningSample.mTimestamp);
            } else {
                return LinkQualityInfo.UNKNOWN_INT;
            }
        }

#endif
}

ECode SamplingDataTracker::SetCommonLinkQualityInfoFields(
    /* [in] */ ILinkQualityInfo* li)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mSamplingDataLock) {
            li.setLastDataSampleTime(getSampleTimestamp());
            li.setDataSampleDuration(getSampleDuration());
            li.setPacketCount(getSampledPacketCount());
            li.setPacketErrorCount(getSampledPacketErrorCount());
        }

#endif
}

ECode SamplingDataTracker::GetSAMPLING_DATA_LOCK(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = SAMPLING_DATA_LOCK;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

AutoPtr<IInterface> SamplingDataTracker::CreateSAMPLING_DATA_LOCK()
{
    AutoPtr<IInterface> rev;
    rev = (new Object)->Probe(EIID_IInterface);
    return rev;
}

//===================================================
// SamplingDataTrackerSamplingSnapshot
//===================================================
CAR_INTERFACE_IMPL(SamplingDataTrackerSamplingSnapshot, Object, ISamplingDataTrackerSamplingSnapshot)

ECode SamplingDataTrackerSamplingSnapshot::constructor()
{
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetTxByteCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTxByteCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetTxByteCount(
    /* [in] */ Int64 mTxByteCount)
{
    this->mTxByteCount = mTxByteCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetRxByteCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mRxByteCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetRxByteCount(
    /* [in] */ Int64 mRxByteCount)
{
    this->mRxByteCount = mRxByteCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetTxPacketCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTxPacketCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetTxPacketCount(
    /* [in] */ Int64 mTxPacketCount)
{
    this->mTxPacketCount = mTxPacketCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetRxPacketCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mRxPacketCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetRxPacketCount(
    /* [in] */ Int64 mRxPacketCount)
{
    this->mRxPacketCount = mRxPacketCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetTxPacketErrorCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTxPacketErrorCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetTxPacketErrorCount(
    /* [in] */ Int64 mTxPacketErrorCount)
{
    this->mTxPacketErrorCount = mTxPacketErrorCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetRxPacketErrorCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mRxPacketErrorCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetRxPacketErrorCount(
    /* [in] */ Int64 mRxPacketErrorCount)
{
    this->mRxPacketErrorCount = mRxPacketErrorCount;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::GetTimestamp(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTimestamp;
    return NOERROR;
}

ECode SamplingDataTrackerSamplingSnapshot::SetTimestamp(
    /* [in] */ Int64 mTimestamp)
{
    this->mTimestamp = mTimestamp;
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
