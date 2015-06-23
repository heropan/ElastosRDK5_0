
#include "content/CSyncStats.h"
#include <ext/frameworkext.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::IStringBuilder;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Content {

CSyncStats::CSyncStats()
    : mNumAuthExceptions(0)
    , mNumIoExceptions(0)
    , mNumParseExceptions(0)
    , mNumConflictDetectedExceptions(0)
    , mNumInserts(0)
    , mNumUpdates(0)
    , mNumDeletes(0)
    , mNumEntries(0)
    , mNumSkippedEntries(0)
{}

CSyncStats::~CSyncStats()
{}

ECode CSyncStats::GetNumAuthExceptions(
    /* [out] */ Int64* numAuthExceptions)
{
    VALIDATE_NOT_NULL(numAuthExceptions)
    *numAuthExceptions = mNumAuthExceptions;
    return NOERROR;
}

ECode CSyncStats::SetNumAuthExceptions(
    /* [in] */ Int64 numAuthExceptions)
{
    mNumAuthExceptions = numAuthExceptions;
    return NOERROR;
}

ECode CSyncStats::GetNumIoExceptions(
    /* [out] */ Int64* numIoExceptions)
{
    VALIDATE_NOT_NULL(numIoExceptions)
    *numIoExceptions = mNumIoExceptions;
    return NOERROR;
}

ECode CSyncStats::SetNumIoExceptions(
    /* [in] */ Int64 numIoExceptions)
{
    mNumIoExceptions = numIoExceptions;
    return NOERROR;
}

ECode CSyncStats::GetNumParseExceptions(
    /* [out] */ Int64* numParseExceptions)
{
    VALIDATE_NOT_NULL(numParseExceptions)
    *numParseExceptions = mNumParseExceptions;
    return NOERROR;
}

ECode CSyncStats::SetNumParseExceptions(
    /* [in] */ Int64 numParseExceptions)
{
    mNumParseExceptions = numParseExceptions;
    return NOERROR;
}

ECode CSyncStats::GetNumConflictDetectedExceptions(
    /* [out] */ Int64* numConflictDetectedExceptions)
{
    VALIDATE_NOT_NULL(numConflictDetectedExceptions)
    *numConflictDetectedExceptions = mNumConflictDetectedExceptions;
    return NOERROR;
}

ECode CSyncStats::SetNumConflictDetectedExceptions(
    /* [in] */ Int64 numConflictDetectedExceptions)
{
    mNumConflictDetectedExceptions = numConflictDetectedExceptions;
    return NOERROR;
}

ECode CSyncStats::GetNumInserts(
    /* [out] */ Int64* numInserts)
{
    VALIDATE_NOT_NULL(numInserts)
    *numInserts = mNumInserts;
    return NOERROR;
}

ECode CSyncStats::SetNumInserts(
    /* [in] */ Int64 numInserts)
{
    mNumInserts = numInserts;
    return NOERROR;
}

ECode CSyncStats::GetNumUpdates(
    /* [out] */ Int64* numUpdates)
{
    VALIDATE_NOT_NULL(numUpdates)
    *numUpdates = mNumUpdates;
    return NOERROR;
}

ECode CSyncStats::SetNumUpdates(
    /* [in] */ Int64 numUpdates)
{
    mNumUpdates = numUpdates;
    return NOERROR;
}

ECode CSyncStats::GetNumDeletes(
    /* [out] */ Int64* numDeletes)
{
    VALIDATE_NOT_NULL(numDeletes)
    *numDeletes = mNumDeletes;
    return NOERROR;
}

ECode CSyncStats::SetNumDeletes(
    /* [in] */ Int64 numDeletes)
{
    mNumDeletes = numDeletes;
    return NOERROR;
}

ECode CSyncStats::GetNumEntries(
    /* [out] */ Int64* numEntries)
{
    VALIDATE_NOT_NULL(numEntries)
    *numEntries = mNumEntries;
    return NOERROR;
}

ECode CSyncStats::SetNumEntries(
    /* [in] */ Int64 numEntries)
{
    mNumEntries = numEntries;
    return NOERROR;
}

ECode CSyncStats::GetNumSkippedEntries(
    /* [out] */ Int64* numSkippedEntries)
{
    VALIDATE_NOT_NULL(numSkippedEntries)
    *numSkippedEntries = mNumSkippedEntries;
    return NOERROR;
}

ECode CSyncStats::SetNumSkippedEntries(
    /* [in] */ Int64 numSkippedEntries)
{
    mNumSkippedEntries = numSkippedEntries;
    return NOERROR;
}

ECode CSyncStats::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    sb->AppendString(String(" stats ["));

    if (mNumAuthExceptions > 0) {
        sb->AppendString(String(" numAuthExceptions: "));
        sb->AppendInt64(mNumAuthExceptions);
    }
    if (mNumIoExceptions > 0) {
        sb->AppendString(String(" numIoExceptions: "));
        sb->AppendInt64(mNumIoExceptions);
    }
    if (mNumParseExceptions > 0) {
        sb->AppendString(String(" numParseExceptions: "));
        sb->AppendInt64(mNumParseExceptions);
    }
    if (mNumConflictDetectedExceptions > 0) {
        sb->AppendString(String(" numConflictDetectedExceptions: "));
        sb->AppendInt64(mNumConflictDetectedExceptions);
    }
    if (mNumInserts > 0) {
        sb->AppendString(String(" numInserts: "));
        sb->AppendInt64(mNumInserts);
    }
    if (mNumUpdates > 0) {
        sb->AppendString(String(" numUpdates: "));
        sb->AppendInt64(mNumUpdates);
    }
    if (mNumDeletes > 0) {
        sb->AppendString(String(" numDeletes: "));
        sb->AppendInt64(mNumDeletes);
    }
    if (mNumEntries > 0) {
        sb->AppendString(String(" numEntries: "));
        sb->AppendInt64(mNumEntries);
    }
    if (mNumSkippedEntries > 0) {
        sb->AppendString(String(" numSkippedEntries: "));
        sb->AppendInt64(mNumSkippedEntries);
    }

    sb->AppendString(String("]"));
    return sb->ToString(str);
}

ECode CSyncStats::Clear()
{
    mNumAuthExceptions = 0;
    mNumIoExceptions = 0;
    mNumParseExceptions = 0;
    mNumConflictDetectedExceptions = 0;
    mNumInserts = 0;
    mNumUpdates = 0;
    mNumDeletes = 0;
    mNumEntries = 0;
    mNumSkippedEntries = 0;
    return NOERROR;
}

ECode CSyncStats::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt64(&mNumAuthExceptions);
    source->ReadInt64(&mNumIoExceptions);
    source->ReadInt64(&mNumParseExceptions);
    source->ReadInt64(&mNumConflictDetectedExceptions);
    source->ReadInt64(&mNumInserts);
    source->ReadInt64(&mNumUpdates);
    source->ReadInt64(&mNumDeletes);
    source->ReadInt64(&mNumEntries);
    source->ReadInt64(&mNumSkippedEntries);
    return NOERROR;
}

ECode CSyncStats::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt64(mNumAuthExceptions);
    dest->WriteInt64(mNumIoExceptions);
    dest->WriteInt64(mNumParseExceptions);
    dest->WriteInt64(mNumConflictDetectedExceptions);
    dest->WriteInt64(mNumInserts);
    dest->WriteInt64(mNumUpdates);
    dest->WriteInt64(mNumDeletes);
    dest->WriteInt64(mNumEntries);
    dest->WriteInt64(mNumSkippedEntries);
    return NOERROR;
}

ECode CSyncStats::constructor()
{
    mNumAuthExceptions = 0;
    mNumIoExceptions = 0;
    mNumParseExceptions = 0;
    mNumConflictDetectedExceptions = 0;
    mNumInserts = 0;
    mNumUpdates = 0;
    mNumDeletes = 0;
    mNumEntries = 0;
    mNumSkippedEntries = 0;
    return NOERROR;
}

}
}
}

