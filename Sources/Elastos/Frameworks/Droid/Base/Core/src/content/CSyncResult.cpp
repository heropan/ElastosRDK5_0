
#include "content/CSyncResult.h"
#include "content/CSyncStats.h"
#include <ext/frameworkext.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringBuffer.h>

namespace Elastos {
namespace Droid {
namespace Content {

CSyncResult::CSyncResult()
    : mSyncAlreadyInProgress(FALSE)
    , mTooManyDeletions(FALSE)
    , mTooManyRetries(FALSE)
    , mDatabaseError(FALSE)
    , mFullSyncRequested(FALSE)
    , mPartialSyncUnavailable(FALSE)
    , mMoreRecordsToGet(FALSE)
    , mDelayUntil(0)
    , mStats(NULL)
{}

CSyncResult::~CSyncResult()
{}

ECode CSyncResult::GetSyncAlreadyInProgress(
    /* [out] */ Boolean* syncAlreadyInProgress)
{
    VALIDATE_NOT_NULL(syncAlreadyInProgress)
    *syncAlreadyInProgress = mSyncAlreadyInProgress;
    return NOERROR;
}

ECode CSyncResult::GetTooManyDeletions(
    /* [out] */ Boolean* tooManyDeletions)
{
    VALIDATE_NOT_NULL(tooManyDeletions)
    *tooManyDeletions = mTooManyDeletions;
    return NOERROR;
}

ECode CSyncResult::SetTooManyDeletions(
    /* [in] */ Boolean tooManyDeletions)
{
    mTooManyDeletions = tooManyDeletions;
    return NOERROR;
}

ECode CSyncResult::GetTooManyRetries(
    /* [out] */ Boolean* tooManyRetries)
{
    VALIDATE_NOT_NULL(tooManyRetries)
    *tooManyRetries = mTooManyRetries;
    return NOERROR;
}

ECode CSyncResult::SetTooManyRetries(
    /* [in] */ Boolean tooManyRetries)
{
    mTooManyRetries = tooManyRetries;
    return NOERROR;
}

ECode CSyncResult::GetDatabaseError(
    /* [out] */ Boolean* databaseError)
{
    VALIDATE_NOT_NULL(databaseError)
    *databaseError = mDatabaseError;
    return NOERROR;
}

ECode CSyncResult::SetDatabaseError(
    /* [in] */ Boolean databaseError)
{
    mDatabaseError = databaseError;
    return NOERROR;
}

ECode CSyncResult::GetFullSyncRequested(
    /* [out] */ Boolean* fullSyncRequested)
{
    VALIDATE_NOT_NULL(fullSyncRequested)
    *fullSyncRequested = mFullSyncRequested;
    return NOERROR;
}

ECode CSyncResult::SetFullSyncRequested(
    /* [in] */ Boolean fullSyncRequested)
{
    mFullSyncRequested = fullSyncRequested;
    return NOERROR;
}

ECode CSyncResult::GetPartialSyncUnavailable(
    /* [out] */ Boolean* partialSyncUnavailable)
{
    VALIDATE_NOT_NULL(partialSyncUnavailable)
    *partialSyncUnavailable = mPartialSyncUnavailable;
    return NOERROR;
}

ECode CSyncResult::SetPartialSyncUnavailable(
    /* [in] */ Boolean partialSyncUnavailable)
{
    mPartialSyncUnavailable = partialSyncUnavailable;
    return NOERROR;
}

ECode CSyncResult::GetMoreRecordsToGet(
    /* [out] */ Boolean* moreRecordsToGet)
{
    VALIDATE_NOT_NULL(moreRecordsToGet)
    *moreRecordsToGet = mMoreRecordsToGet;
    return NOERROR;
}

ECode CSyncResult::SetMoreRecordsToGet(
    /* [in] */ Boolean moreRecordsToGet)
{
    mMoreRecordsToGet = moreRecordsToGet;
    return NOERROR;
}

ECode CSyncResult::GetDelayUntil(
    /* [out] */ Int64* delayUntil)
{
    VALIDATE_NOT_NULL(delayUntil)
    *delayUntil = mDelayUntil;
    return NOERROR;
}

ECode CSyncResult::SetDelayUntil(
    /* [in] */ Int64 delayUntil)
{
    mDelayUntil = delayUntil;
    return NOERROR;
}

ECode CSyncResult::GetStats(
    /* [out] */ ISyncStats** stats)
{
    VALIDATE_NOT_NULL(stats)
    *stats = mStats;
    REFCOUNT_ADD(*stats);
    return NOERROR;
}

ECode CSyncResult::HasHardError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError)
    Int64 numParseExceptions = 0;
    Int64 numConflictDetectedExceptions = 0;
    Int64 numAuthExceptions = 0;
    FAIL_RETURN(mStats->GetNumParseExceptions(&numParseExceptions))
    FAIL_RETURN(mStats->GetNumConflictDetectedExceptions(&numConflictDetectedExceptions))
    FAIL_RETURN(mStats->GetNumAuthExceptions(&numAuthExceptions))
    *hasError = numParseExceptions > 0
            || numConflictDetectedExceptions > 0
            || numAuthExceptions > 0
            || mTooManyDeletions
            || mTooManyRetries
            || mDatabaseError;

    return NOERROR;
}

ECode CSyncResult::HasSoftError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError)
    Int64 numIoExceptions = 0;
    FAIL_RETURN(mStats->GetNumIoExceptions(&numIoExceptions))
    *hasError = mSyncAlreadyInProgress || numIoExceptions > 0;
    return NOERROR;
}

ECode CSyncResult::HasError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError)
    Boolean ret = FALSE;
    *hasError = (HasSoftError(&ret), ret) || (HasHardError(&ret), ret);
    return NOERROR;
}

ECode CSyncResult::MadeSomeProgress(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int64 numDeletes = 0;
    Int64 numInserts = 0;
    Int64 numUpdates = 0;
    FAIL_RETURN(mStats->GetNumDeletes(&numDeletes))
    FAIL_RETURN(mStats->GetNumInserts(&numInserts))
    FAIL_RETURN(mStats->GetNumUpdates(&numUpdates))
    *result = ((numDeletes > 0) && !mTooManyDeletions)
            || numInserts > 0
            || numUpdates > 0;

    return NOERROR;
}

ECode CSyncResult::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

ECode CSyncResult::Clear()
{
    if (mSyncAlreadyInProgress) {
        //throw new UnsupportedOperationException(
        //        "you are not allowed to clear the ALREADY_IN_PROGRESS SyncStats");
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    mTooManyDeletions = FALSE;
    mTooManyRetries = FALSE;
    mDatabaseError = FALSE;
    mFullSyncRequested = FALSE;
    mPartialSyncUnavailable = FALSE;
    mMoreRecordsToGet = FALSE;
    mDelayUntil = 0;
    FAIL_RETURN(mStats->Clear())
    return NOERROR;
}

ECode CSyncResult::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    sb->AppendString(String("SyncResult:"));

    if (mSyncAlreadyInProgress) {
        sb->AppendString(String(" syncAlreadyInProgress: "));
        sb->AppendBoolean(mSyncAlreadyInProgress);
    }
    if (mTooManyDeletions) {
        sb->AppendString(String(" tooManyDeletions: "));
        sb->AppendBoolean(mTooManyDeletions);
    }
    if (mTooManyRetries) {
        sb->AppendString(String(" tooManyRetries: "));
        sb->AppendBoolean(mTooManyRetries);
    }
    if (mDatabaseError) {
        sb->AppendString(String(" databaseError: "));
        sb->AppendBoolean(mDatabaseError);
    }
    if (mFullSyncRequested) {
        sb->AppendString(String(" fullSyncRequested: "));
        sb->AppendBoolean(mFullSyncRequested);
    }
    if (mPartialSyncUnavailable) {
        sb->AppendString(String(" partialSyncUnavailable: "));
        sb->AppendBoolean(mPartialSyncUnavailable);
    }
    if (mMoreRecordsToGet) {
        sb->AppendString(String(" moreRecordsToGet: "));
        sb->AppendBoolean(mMoreRecordsToGet);
    }
    if (mDelayUntil > 0) {
        sb->AppendString(String(" delayUntil: "));
        sb->AppendInt64(mDelayUntil);
    }

    sb->AppendObject(mStats);
    return sb->ToString(str);
}

ECode CSyncResult::ToDebugString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Boolean ret = FALSE;
    Int64 numParseExceptions = 0;
    Int64 numConflictDetectedExceptions = 0;
    Int64 numAuthExceptions = 0;
    Int64 numIoExceptions = 0;
    AutoPtr<IStringBuffer> sb = new StringBuffer();
    FAIL_RETURN(mStats->GetNumParseExceptions(&numParseExceptions))
    FAIL_RETURN(mStats->GetNumConflictDetectedExceptions(&numConflictDetectedExceptions))
    FAIL_RETURN(mStats->GetNumAuthExceptions(&numAuthExceptions))
    FAIL_RETURN(mStats->GetNumIoExceptions(&numIoExceptions))

    if (mFullSyncRequested) {
        sb->AppendString(String("f1"));
    }
    if (mPartialSyncUnavailable) {
        sb->AppendString(String("r1"));
    }
    if ((HasHardError(&ret), ret)) {
        sb->AppendString(String("X1"));
    }
    if (numParseExceptions > 0) {
        sb->AppendString(String("e"));
        sb->AppendInt64(numParseExceptions);
    }
    if (numConflictDetectedExceptions > 0) {
        sb->AppendString(String("c"));
        sb->AppendInt64(numConflictDetectedExceptions);
    }
    if (numAuthExceptions > 0) {
        sb->AppendString(String("a"));
        sb->AppendInt64(numAuthExceptions);
    }
    if (mTooManyDeletions) {
        sb->AppendString(String("D1"));
    }
    if (mTooManyRetries) {
        sb->AppendString(String("R1"));
    }
    if (mDatabaseError) {
        sb->AppendString(String("b1"));
    }
    if ((HasSoftError(&ret), ret)) {
        sb->AppendString(String("x1"));
    }
    if (mSyncAlreadyInProgress) {
        sb->AppendString(String("l1"));
    }
    if (numIoExceptions > 0) {
        sb->AppendString(String("I"));
        sb->AppendInt64(numIoExceptions);
    }

    return sb->ToString(str);
}

ECode CSyncResult::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 value = 0;
    mSyncAlreadyInProgress = (source->ReadInt32(&value), value) != 0;
    mTooManyDeletions = (source->ReadInt32(&value), value) != 0;
    mTooManyRetries = (source->ReadInt32(&value), value) != 0;
    mDatabaseError = (source->ReadInt32(&value), value) != 0;
    mFullSyncRequested = (source->ReadInt32(&value), value) != 0;
    mPartialSyncUnavailable = (source->ReadInt32(&value), value) != 0;
    mMoreRecordsToGet = (source->ReadInt32(&value), value) != 0;
    source->ReadInt64(&mDelayUntil);
    if (NULL == mStats) {
        FAIL_RETURN(CSyncStats::New((ISyncStats**)&mStats))
    }
    AutoPtr<IParcelable> parcelable = (IParcelable*) mStats->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->ReadFromParcel(source))
    return NOERROR;
}

ECode CSyncResult::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mSyncAlreadyInProgress ? 1 : 0);
    dest->WriteInt32(mTooManyDeletions ? 1 : 0);
    dest->WriteInt32(mTooManyRetries ? 1 : 0);
    dest->WriteInt32(mDatabaseError ? 1 : 0);
    dest->WriteInt32(mFullSyncRequested ? 1 : 0);
    dest->WriteInt32(mPartialSyncUnavailable ? 1 : 0);
    dest->WriteInt32(mMoreRecordsToGet ? 1 : 0);
    dest->WriteInt64(mDelayUntil);
    AutoPtr<IParcelable> parcelable = (IParcelable*) mStats->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->WriteToParcel(dest))
    return NOERROR;
}

ECode CSyncResult::constructor()
{
    return constructor(FALSE);
}

ECode CSyncResult::constructor(
    /* [in] */ Boolean syncAlreadyInProgress)
{
    mSyncAlreadyInProgress = syncAlreadyInProgress;
    mTooManyDeletions = FALSE;
    mTooManyRetries = FALSE;
    mFullSyncRequested = FALSE;
    mPartialSyncUnavailable = FALSE;
    mMoreRecordsToGet = FALSE;
    mDelayUntil = 0;
    FAIL_RETURN(CSyncStats::New((ISyncStats**)&mStats))
    return NOERROR;
}

}
}
}

