
#include "ext/frameworkdef.h"
#include "net/CNetworkStats.h"
#include "net/CNetworkStatsEntry.h"
#include "os/SystemClock.h"
#include "util/ArrayUtils.h"
#include "util/SparseArray.h"
#include <elastos/core/Math.h>
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/core/StringUtils.h>

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::SparseArray;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::IO::ICharArrayWriter;
using Elastos::IO::CCharArrayWriter;
using Elastos::IO::CPrintWriter;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Net {

CNetworkStats::CNetworkStats()
    : mElapsedRealtime(0)
    , mSize(0)
{}

ECode CNetworkStats::constructor()
{
    return NOERROR;
}

ECode CNetworkStats::constructor(
    /* [in] */ Int64 elapsedRealtime,
    /* [in] */ Int32 initialSize)
{
    mElapsedRealtime = elapsedRealtime;
    mSize = 0;
    mIface = ArrayOf<String>::Alloc(initialSize);
    mUid = ArrayOf<Int32>::Alloc(initialSize);
    mSet = ArrayOf<Int32>::Alloc(initialSize);
    mTag = ArrayOf<Int32>::Alloc(initialSize);
    mRxBytes = ArrayOf<Int64>::Alloc(initialSize);
    mRxPackets = ArrayOf<Int64>::Alloc(initialSize);
    mTxBytes = ArrayOf<Int64>::Alloc(initialSize);
    mTxPackets = ArrayOf<Int64>::Alloc(initialSize);
    mOperations = ArrayOf<Int64>::Alloc(initialSize);
    return NOERROR;
}

//@Override
ECode CNetworkStats::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt64(mElapsedRealtime);
    dest->WriteInt32(mSize);
    dest->WriteArrayOfString(mIface);
    dest->WriteArrayOf((Handle32)mUid.Get());
    dest->WriteArrayOf((Handle32)mSet.Get());
    dest->WriteArrayOf((Handle32)mTag.Get());
    dest->WriteArrayOf((Handle32)mRxBytes.Get());
    dest->WriteArrayOf((Handle32)mRxPackets.Get());
    dest->WriteArrayOf((Handle32)mTxBytes.Get());
    dest->WriteArrayOf((Handle32)mTxPackets.Get());
    dest->WriteArrayOf((Handle32)mOperations.Get());
    return NOERROR;
}

//@Override
ECode CNetworkStats::Clone(
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<INetworkStats> clone;
    CNetworkStats::New(mElapsedRealtime, mSize, (INetworkStats**)&clone);
    for (Int32 i = 0; i < mSize; i++) {
        AutoPtr<INetworkStatsEntry> entry;
        GetValues(i, entry, (INetworkStatsEntry**)&entry);
        clone->AddValues(entry);
    }
    *result = clone;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

// @VisibleForTesting
ECode CNetworkStats::AddIfaceValues(
    /* [in] */ const String& iface,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets)
{
    return AddValues(iface, UID_ALL, SET_DEFAULT, TAG_NONE,
        rxBytes, rxPackets, txBytes, txPackets, 0ll);
}

// @VisibleForTesting
ECode CNetworkStats::AddValues(
    /* [in] */ const String& iface,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 set,
    /* [in] */ Int32 tag,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations)
{
    AutoPtr<INetworkStatsEntry> ent;
    CNetworkStatsEntry::New(
            iface, uid, set, tag, rxBytes, rxPackets, txBytes, txPackets, operations, (INetworkStatsEntry**)&ent);
    return AddValues(ent);
}

/**
 * Add new stats entry, copying from given {@link Entry}. The {@link Entry}
 * object can be recycled across multiple calls.
 */
ECode CNetworkStats::AddValues(
    /* [in] */ INetworkStatsEntry* entry)
{
    if (mSize >= mIface->GetLength()) {
        Int32 newLength = Elastos::Core::Math::Max(mIface->GetLength(), 10) * 3 / 2;
        AutoPtr< ArrayOf<String> > iface = ArrayOf<String>::Alloc(newLength);
        iface->Copy(mIface);
        mIface = iface;
        AutoPtr< ArrayOf<Int32> > uid = ArrayOf<Int32>::Alloc(newLength);
        uid->Copy(mUid);
        mUid = uid;
        AutoPtr< ArrayOf<Int32> > set = ArrayOf<Int32>::Alloc(newLength);
        set->Copy(mSet);
        mSet = set;
        AutoPtr< ArrayOf<Int32> > tag = ArrayOf<Int32>::Alloc(newLength);
        tag->Copy(mTag);
        mTag = tag;
        AutoPtr< ArrayOf<Int64> > rxBytes = ArrayOf<Int64>::Alloc(newLength);
        rxBytes->Copy(mRxBytes);
        mRxBytes = rxBytes;
        AutoPtr< ArrayOf<Int64> > rxPackets = ArrayOf<Int64>::Alloc(newLength);
        rxPackets->Copy(mRxPackets);
        mRxPackets = rxPackets;
        AutoPtr< ArrayOf<Int64> > txBytes = ArrayOf<Int64>::Alloc(newLength);
        txBytes->Copy(mTxBytes);
        mTxBytes = txBytes;
        AutoPtr< ArrayOf<Int64> > txPackets = ArrayOf<Int64>::Alloc(newLength);
        txPackets->Copy(mTxPackets);
        mTxPackets = txPackets;
        AutoPtr< ArrayOf<Int64> > operations = ArrayOf<Int64>::Alloc(newLength);
        operations->Copy(mOperations);
        mOperations = operations;
    }

    String iface;
    Int32 uid, set, tag;
    Int64 rxBytes, rxPackets, txBytes, txPackets, operations;
    entry->GetIface(&iface);
    entry->GetUid(&uid);
    entry->GetSet(&set);
    entry->GetTag(&tag);
    entry->GetRxBytes(&rxBytes);
    entry->GetRxPackets(&rxPackets);
    entry->GetTxBytes(&txBytes);
    entry->GetTxPackets(&txPackets);
    entry->GetOperations(&operations);
    (*mIface)[mSize] = iface;
    (*mUid)[mSize] = uid;
    (*mSet)[mSize] = set;
    (*mTag)[mSize] = tag;
    (*mRxBytes)[mSize] = rxBytes;
    (*mRxPackets)[mSize] = rxPackets;
    (*mTxBytes)[mSize] = txBytes;
    (*mTxPackets)[mSize] = txPackets;
    (*mOperations)[mSize] = operations;
    mSize++;
    return NOERROR;
}

/**
 * Return specific stats entry.
 */
ECode CNetworkStats::GetValues(
    /* [in] */ Int32 i,
    /* [in] */ INetworkStatsEntry* recycle,
    /* [out] */ INetworkStatsEntry** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsEntry> entry;
    if (recycle != NULL) {
         entry = recycle;
    }
    else {
        CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);
    }
    entry->SetIface((*mIface)[i]);
    entry->SetUid((*mUid)[i]);
    entry->SetSet((*mSet)[i]);
    entry->SetTag((*mTag)[i]);
    entry->SetRxBytes((*mRxBytes)[i]);
    entry->SetRxPackets((*mRxPackets)[i]);
    entry->SetTxBytes((*mTxBytes)[i]);
    entry->SetTxPackets((*mTxPackets)[i]);
    entry->SetOperations((*mOperations)[i]);
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CNetworkStats::GetElapsedRealtime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mElapsedRealtime;
    return NOERROR;
}

/**
 * Return age of this {@link NetworkStats} object with respect to
 * {@link SystemClock#elapsedRealtime()}.
 */
ECode CNetworkStats::GetElapsedRealtimeAge(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = SystemClock::GetElapsedRealtime() - mElapsedRealtime;
    return NOERROR;
}

ECode CNetworkStats::GetSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSize;
    return NOERROR;
}

// @VisibleForTesting
ECode CNetworkStats::GetInternalSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIface->GetLength();
    return NOERROR;
}

//@Deprecated
ECode CNetworkStats::CombineValues(
    /* [in] */ const String& iface,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 tag,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations)
{
    return CombineValues(iface, uid, SET_DEFAULT,
        tag, rxBytes, rxPackets, txBytes, txPackets, operations);
}

ECode CNetworkStats::CombineValues(
    /* [in] */ const String& iface,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 set,
    /* [in] */ Int32 tag,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations)
{
    AutoPtr<INetworkStatsEntry> ent;
    CNetworkStatsEntry::New(iface, uid, set, tag,
        rxBytes, rxPackets, txBytes, txPackets, operations, (INetworkStatsEntry**)&ent);
    return CombineValues(ent);
}

/**
 * Combine given values with an existing row, or create a new row if
 * {@link #findIndex(String, int, int, int)} is unable to find match. Can
 * also be used to subtract values from existing rows.
 */
ECode CNetworkStats::CombineValues(
    /* [in] */ INetworkStatsEntry* entry)
{
    String iface;
    Int32 uid, set, tag;
    entry->GetIface(&iface);
    entry->GetUid(&uid);
    entry->GetSet(&set);
    entry->GetTag(&tag);
    Int32 i;
    FindIndex(iface, uid, set, tag, &i);

    if (i == -1) {
        // only create new entry when positive contribution
        AddValues(entry);
    }
    else {
        Int64 rxBytes, rxPackets, txBytes, txPackets, operations;
        entry->GetRxBytes(&rxBytes);
        entry->GetRxPackets(&rxPackets);
        entry->GetTxBytes(&txBytes);
        entry->GetTxPackets(&txPackets);
        entry->GetOperations(&operations);
        (*mRxBytes)[i] += rxBytes;
        (*mRxPackets)[i] += rxPackets;
        (*mTxBytes)[i] += txBytes;
        (*mTxPackets)[i] += txPackets;
        (*mOperations)[i] += operations;
    }
    return NOERROR;
}

/**
 * Combine all values from another {@link NetworkStats} into this object.
 */
ECode CNetworkStats::CombineAllValues(
    /* [in] */ INetworkStats* another)
{
    AutoPtr<INetworkStatsEntry> entry;
    Int32 n;
    another->GetSize(&n);
    for (Int32 i = 0; i < n; i++) {
        AutoPtr<INetworkStatsEntry> outEntry;
        another->GetValues(i, entry, (INetworkStatsEntry**)&outEntry);
        CombineValues(outEntry);
        entry = outEntry;
    }
    return NOERROR;
}

/**
 * Find first stats index that matches the requested parameters.
 */
ECode CNetworkStats::FindIndex(
    /* [in] */ const String& iface,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 set,
    /* [in] */ Int32 tag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    for (Int32 i = 0; i < mSize; i++) {
        if (uid == (*mUid)[i] && set == (*mSet)[i] && tag == (*mTag)[i]
                && iface.Equals((*mIface)[i])) {
            *result = i;
            return NOERROR;
        }
    }
    *result = -1;
    return NOERROR;
}

/**
 * Find first stats index that matches the requested parameters, starting
 * search around the hinted index as an optimization.
 */
// @VisibleForTesting
ECode CNetworkStats::FindIndexHinted(
    /* [in] */ const String& iface,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 set,
    /* [in] */ Int32 tag,
    /* [in] */ Int32 hintIndex,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    for (Int32 offset = 0; offset < mSize; offset++) {
        Int32 halfOffset = offset / 2;

        // search outwards from hint index, alternating forward and backward
        Int32 i;
        if (offset % 2 == 0) {
            i = (hintIndex + halfOffset) % mSize;
        }
        else {
            i = (mSize + hintIndex - halfOffset - 1) % mSize;
        }
        if (uid == (*mUid)[i] && set == (*mSet)[i] && tag == (*mTag)[i]
                && iface.Equals((*mIface)[i])) {
            *result = i;
            return NOERROR;
        }
    }
    *result = -1;
    return NOERROR;
}

/**
 * Splice in {@link #operations} from the given {@link NetworkStats} based
 * on matching {@link #uid} and {@link #tag} rows. Ignores {@link #iface},
 * since operation counts are at data layer.
 */
ECode CNetworkStats::SpliceOperationsFrom(
    /* [in] */ INetworkStats* stats)
{
    for (Int32 i = 0; i < mSize; i++) {
        Int32 j;
        stats->FindIndex((*mIface)[i], (*mUid)[i], (*mSet)[i], (*mTag)[i], &j);
        if (j == -1) {
            (*mOperations)[i] = 0;
        }
        else {
            AutoPtr< ArrayOf<Int64> > operations;
            stats->GetOperations((ArrayOf<Int64>**)&operations);
            mOperations->Set(i, (*operations)[j]);
        }
    }
    return NOERROR;
}

/**
 * Return list of unique interfaces known by this data structure.
 */
ECode CNetworkStats::GetUniqueIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);

    HashSet<String> ifaces;
    for (Int32 i = 0; i < mIface->GetLength(); i++) {
        String iface = (*mIface)[i];
        if (!iface.Equals(IFACE_ALL)) {
            ifaces.Insert(iface);
        }
    }

    AutoPtr< ArrayOf<String> > resArray = ArrayOf<String>::Alloc(ifaces.GetSize());
    HashSet<String>::Iterator iter = ifaces.Begin();
    for (Int32 i = 0;  iter != ifaces.End(); ++iter, ++i) {
        (*resArray)[i] = *iter;
    }
    *result = resArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * Return list of unique UIDs known by this data structure.
 */
ECode CNetworkStats::GetUniqueUids(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);
    HashMap<Int32, Boolean> uids(mUid->GetLength());
    for (Int32 i = 0; i < mUid->GetLength(); i++) {
        uids[(*mUid)[i]] = TRUE;
    }

    Int32 size = uids.GetSize();
    AutoPtr< ArrayOf<Int32> > outputArray = ArrayOf<Int32>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        outputArray->Set(i, uids[i]);
    }
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * Return total bytes represented by this snapshot object, usually used when
 * checking if a {@link #subtract(NetworkStats)} delta passes a threshold.
 */
ECode CNetworkStats::GetTotalBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsEntry> entry;
    GetTotal(NULL, (INetworkStatsEntry**)&entry);
    Int64 rxBytes, txBytes;
    entry->GetRxBytes(&rxBytes);
    entry->GetTxBytes(&txBytes);
    *result = rxBytes + txBytes;
    return NOERROR;
}

/**
 * Return total of all fields represented by this snapshot object.
 */
ECode CNetworkStats::GetTotal(
    /* [in] */ INetworkStatsEntry* recycle,
    /* [out] */ INetworkStatsEntry** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsEntry> entry = GetTotal(recycle, NULL, UID_ALL, FALSE);
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return total of all fields represented by this snapshot object matching
 * the requested {@link #uid}.
 */
ECode CNetworkStats::GetTotal(
    /* [in] */ INetworkStatsEntry* recycle,
    /* [in] */ Int32 limitUid,
    /* [out] */ INetworkStatsEntry** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsEntry> entry = GetTotal(recycle, NULL, limitUid, FALSE);
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return total of all fields represented by this snapshot object matching
 * the requested {@link #iface}.
 */
ECode CNetworkStats::GetTotal(
    /* [in] */ INetworkStatsEntry* recycle,
    /* [in] */ IObjectContainer* limitIface,
    /* [out] */ INetworkStatsEntry** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsEntry> entry = GetTotal(recycle, limitIface, UID_ALL, FALSE);
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CNetworkStats::GetTotalIncludingTags(
    /* [in] */ INetworkStatsEntry* recycle,
    /* [out] */ INetworkStatsEntry** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsEntry> entry = GetTotal(recycle, NULL, UID_ALL, TRUE);
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return total of all fields represented by this snapshot object matching
 * the requested {@link #iface} and {@link #uid}.
 *
 * @param limitIface Set of {@link #iface} to include in total; or {@code
 *            NULL} to include all ifaces.
 */
AutoPtr<INetworkStatsEntry> CNetworkStats::GetTotal(
    /* [in] */ INetworkStatsEntry* recycle,
    /* [in] */ IObjectContainer* limitIface,
    /* [in] */ Int32 limitUid,
    /* [in] */ Boolean includeTags)
{
    AutoPtr<INetworkStatsEntry> entry;
    if (recycle != NULL) {
        entry = recycle;
    }
    else {
        CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);
    }

    entry->SetIface(IFACE_ALL);
    entry->SetUid(limitUid);
    entry->SetSet(SET_ALL);
    entry->SetTag(TAG_NONE);
    entry->SetRxBytes(0);
    entry->SetRxPackets(0);
    entry->SetTxBytes(0);
    entry->SetTxPackets(0);
    entry->SetOperations(0);

    for (Int32 i = 0; i < mSize; i++) {
        Boolean bol;
        AutoPtr<ICharSequence> chars;
        CString::New((*mIface)[i], (ICharSequence**)&chars);
        Boolean matchesUid = (limitUid == UID_ALL) || (limitUid == (*mUid)[i]);

        Boolean matchesIface = TRUE;
        if (limitIface) {
            limitIface->Contains(chars, &matchesIface);
        }

        if (matchesUid && matchesIface) {
            // skip specific tags, since already counted in TAG_NONE
            if ((*mTag)[i] != TAG_NONE && !includeTags) continue;

            Int64 rxBytes, rxPackets, txBytes, txPackets, operations;
            entry->GetRxBytes(&rxBytes);
            entry->SetRxBytes(rxBytes + (*mRxBytes)[i]);
            entry->GetRxPackets(&rxPackets);
            entry->SetRxPackets(rxPackets + (*mRxPackets)[i]);
            entry->GetTxBytes(&txBytes);
            entry->SetTxBytes(txBytes + (*mTxBytes)[i]);
            entry->GetTxPackets(&txPackets);
            entry->SetTxPackets(txPackets + (*mTxPackets)[i]);
            entry->GetOperations(&operations);
            entry->SetOperations(operations + (*mOperations)[i]);
        }
    }
    return entry;
}


/**
 * Subtract the given {@link NetworkStats}, effectively leaving the delta
 * between two snapshots in time. Assumes that statistics rows collect over
 * time, and that none of them have disappeared.
 */
ECode CNetworkStats::Subtract(
    /* [in] */ INetworkStats* right,
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStats> networkstats;
    Subtract(this, right, NULL, NULL, (INetworkStats**)&networkstats);
    *result = networkstats;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Subtract the two given {@link NetworkStats} objects, returning the delta
 * between two snapshots in time. Assumes that statistics rows collect over
 * time, and that none of them have disappeared.
 * <p>
 * If counters have rolled backwards, they are clamped to {@code 0} and
 * reported to the given {@link NonMonotonicObserver}.
 */
ECode CNetworkStats::Subtract(
    /* [in] */ INetworkStats* left,
    /* [in] */ INetworkStats* right,
    /* [in] */ INonMonotonicObserver* observer,
    /* [in] */ IInterface* cookie,
    /* [out] */ INetworkStats** result)
{
    Int64 leftTime;
    left->GetElapsedRealtime(&leftTime);
    Int64 rightTime;
    right->GetElapsedRealtime(&rightTime);
    Int64 deltaRealtime = leftTime - rightTime;
    if (deltaRealtime < 0) {
        if (observer != NULL) {
            observer->FoundNonMonotonic(left, -1, right, -1, cookie);
        }
        deltaRealtime = 0;
    }

//result will have our rows, and elapsed time between snapshots
    AutoPtr<INetworkStatsEntry> entry;
    CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);
    AutoPtr<INetworkStats> networkstats;
    Int32 leftSize;
    left->GetSize(&leftSize);
    CNetworkStats::New(deltaRealtime, leftSize, (INetworkStats**)&networkstats);
    for (Int32 i = 0; i < leftSize; i++) {
        AutoPtr< ArrayOf<String> > IFaceArray;
        AutoPtr< ArrayOf<Int32> > UidArray;
        AutoPtr< ArrayOf<Int32> > SetArray;
        AutoPtr< ArrayOf<Int32> > TagArray;
        left->GetIFace((ArrayOf<String>**)&IFaceArray);
        left->GetUid((ArrayOf<Int32>**)&UidArray);
        left->GetSet((ArrayOf<Int32>**)&SetArray);
        left->GetTag((ArrayOf<Int32>**)&TagArray);
        entry->SetIface((*IFaceArray)[i]);
        entry->SetUid((*UidArray)[i]);
        entry->SetSet((*SetArray)[i]);
        entry->SetTag((*TagArray)[i]);
        // find remote row that matches, and subtract
        Int32 j;
        right->FindIndexHinted((*IFaceArray)[i], (*UidArray)[i], (*SetArray)[i], (*TagArray)[i], i, &j);

            AutoPtr< ArrayOf<Int64> > leftRxBytes, rightRxBytes;
            AutoPtr< ArrayOf<Int64> > leftRxPackets, rightRxPackets;
            AutoPtr< ArrayOf<Int64> > leftTxBytes, rightTxBytes;
            AutoPtr< ArrayOf<Int64> > leftTxPackets, rightTxPackets;
            AutoPtr< ArrayOf<Int64> > leftOperations, rightOperations;

            left->GetRxBytes((ArrayOf<Int64>**)&leftRxBytes);
            left->GetRxPackets((ArrayOf<Int64>**)&leftRxPackets);
            left->GetTxBytes((ArrayOf<Int64>**)&leftTxBytes);
            left->GetTxPackets((ArrayOf<Int64>**)&leftTxPackets);
            left->GetOperations((ArrayOf<Int64>**)&leftOperations);

            right->GetRxBytes((ArrayOf<Int64>**)&rightRxBytes);
            right->GetRxPackets((ArrayOf<Int64>**)&rightRxPackets);
            right->GetTxBytes((ArrayOf<Int64>**)&rightTxBytes);
            right->GetTxPackets((ArrayOf<Int64>**)&rightTxPackets);
            right->GetOperations((ArrayOf<Int64>**)&rightOperations);

        if (j == -1) {
            // newly appearing row, return entire value
            entry->SetRxBytes((*leftRxBytes)[i]);
            entry->SetRxPackets((*leftRxPackets)[i]);
            entry->SetTxBytes((*leftTxBytes)[i]);
            entry->SetTxPackets((*leftTxPackets)[i]);
            entry->SetOperations((*leftOperations)[i]);
        }
        else {
            // existing row, subtract remote value
            entry->SetRxBytes((*leftRxBytes)[i] - (*rightRxBytes)[j]);
            entry->SetRxPackets((*leftRxPackets)[i] - (*rightRxPackets)[j]);
            entry->SetTxBytes((*leftTxBytes)[i] - (*rightTxBytes)[j]);
            entry->SetTxPackets((*leftTxPackets)[i] - (*rightTxPackets)[j]);
            entry->SetOperations((*leftOperations)[i] - (*rightOperations)[j]);

            Int64 entryRxbyte;
            entry->GetRxBytes(&entryRxbyte);
            Int64 entryRxpackets;
            entry->GetRxPackets(&entryRxpackets);
            Int64 entryTxbytes;
            entry->GetTxBytes(&entryTxbytes);
            Int64 entryTxpackets;
            entry->GetTxPackets(&entryTxpackets);
            Int64 entryOperations;
            entry->GetOperations(&entryOperations);
            if (entryRxbyte < 0 || entryRxpackets < 0 || entryTxbytes < 0 ||
                entryTxpackets < 0 || entryOperations < 0) {
                if (observer != NULL) {
                    observer->FoundNonMonotonic(left, i, right, j, cookie);
                }
                entry->SetRxBytes(Elastos::Core::Math::Max((Int32)entryRxbyte, 0));
                entry->SetRxPackets(Elastos::Core::Math::Max((Int32)entryRxpackets, 0));
                entry->SetTxBytes(Elastos::Core::Math::Max((Int32)entryTxbytes, 0));
                entry->SetTxPackets(Elastos::Core::Math::Max((Int32)entryTxpackets, 0));
                entry->SetOperations(Elastos::Core::Math::Max((Int32)entryOperations, 0));
            }
        }

        networkstats->AddValues(entry);
    }
    *result = networkstats;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return total statistics grouped by {@link #iface}; doesn't mutate the
 * original structure.
 */
ECode CNetworkStats::GroupedByIface(
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<INetworkStats> stats;
    CNetworkStats::New(mElapsedRealtime, 10, (INetworkStats**)&stats);

    AutoPtr<INetworkStatsEntry> entry;
    CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);
    entry->SetUid(UID_ALL);
    entry->SetSet(SET_ALL);
    entry->SetTag(TAG_NONE);
    entry->SetOperations(0L);

    for (Int32 i = 0; i < mSize; i++) {
        // skip specific tags, since already counted in TAG_NONE
        if ((*mTag)[i] != TAG_NONE) continue;

        entry->SetIface((*mIface)[i]);
        entry->SetRxBytes((*mRxBytes)[i]);
        entry->SetRxPackets((*mRxPackets)[i]);
        entry->SetTxBytes((*mTxBytes)[i]);
        entry->SetTxPackets((*mTxPackets)[i]);
        stats->CombineValues(entry);
    }
    *result = stats;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return total statistics grouped by {@link #uid}; doesn't mutate the
 * original structure.
 */
ECode CNetworkStats::GroupedByUid(
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<INetworkStats> stats;
    CNetworkStats::New(mElapsedRealtime, 10, (INetworkStats**)&stats);

    AutoPtr<INetworkStatsEntry> entry;
    CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);
    entry->SetIface(IFACE_ALL);
    entry->SetSet(SET_ALL);
    entry->SetTag(TAG_NONE);

    for (Int32 i = 0; i < mSize; i++) {
        // skip specific tags, since already counted in TAG_NONE
        if ((*mTag)[i] != TAG_NONE) continue;

        entry->SetUid((*mUid)[i]);
        entry->SetRxBytes((*mRxBytes)[i]);
        entry->SetRxPackets((*mRxPackets)[i]);
        entry->SetTxBytes((*mTxBytes)[i]);
        entry->SetTxPackets((*mTxPackets)[i]);
        entry->SetOperations((*mOperations)[i]);
        stats->CombineValues(entry);
    }
    *result = stats;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return all rows except those attributed to the requested UID; doesn't
 * mutate the original structure.
 */
ECode CNetworkStats::WithoutUids(
    /* [in] */ const ArrayOf<Int32>& uids,
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<INetworkStats> stats;
    CNetworkStats::New(mElapsedRealtime, 10, (INetworkStats**)&stats);

    AutoPtr<INetworkStatsEntry> entry;
    CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);
    for (Int32 i = 0; i < mSize; i++) {
        AutoPtr<INetworkStatsEntry> newEntry;
        GetValues(i, entry, (INetworkStatsEntry**)&newEntry);
        Int32 uid;
        newEntry->GetUid(&uid);
        if (!ArrayUtils::Contains((ArrayOf<Int32>*)&uids, uid)) {
            stats->AddValues(newEntry);
        }
        entry = newEntry;
    }
    *result = stats;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CNetworkStats::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IPrintWriter* pw)
{
    pw->PrintString(prefix);
    pw->PrintString(String("NetworkStats: elapsedRealtime=")); pw->PrintInt64(mElapsedRealtime);
    for (Int32 i = 0; i < mSize; i++) {
        pw->PrintString(prefix);
        pw->PrintString(String("  [")); pw->PrintInt32(i); pw->PrintString(String("]"));
        pw->PrintString(String(" iface=")); pw->PrintString((*mIface)[i]);
        pw->PrintString(String(" uid=")); pw->PrintInt32((*mUid)[i]);
        String set;
        SetToString((*mSet)[i], &set);
        pw->PrintString(String(" set=")); pw->PrintString(set);
        String tag;
        TagToString((*mTag)[i], &tag);
        pw->PrintString(String(" tag=")); pw->PrintString(tag);
        pw->PrintString(String(" rxBytes=")); pw->PrintInt64((*mRxBytes)[i]);
        pw->PrintString(String(" rxPackets=")); pw->PrintInt64((*mRxPackets)[i]);
        pw->PrintString(String(" txBytes=")); pw->PrintInt64((*mTxBytes)[i]);
        pw->PrintString(String(" txPackets=")); pw->PrintInt64((*mTxPackets)[i]);
        pw->PrintString(String(" operations=")); pw->PrintInt64((*mOperations)[i]);
    }
    return NOERROR;
}

///**
// * Return text description of {@link #set} value.
// */
ECode CNetworkStats::SetToString(
    /* [in] */ Int32 set,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    switch (set) {
        case SET_ALL:
            *result = String("ALL");
            break;
        case SET_DEFAULT:
            *result = String("DEFAULT");
            break;
        case SET_FOREGROUND:
            *result = String("FOREGROUND");
            break;
        default:
            *result = String("UNKNOWN");
            break;
    }
    return NOERROR;
}

///**
// * Return text description of {@link #tag} value.
// */
ECode CNetworkStats::TagToString(
    /* [in] */ Int32 tag,
    /* [out] */ String* result)
{
    String str = StringUtils::Int32ToHexString(tag);
    str += String("0x");
    *result = str;
    return NOERROR;
}

//@Override
ECode CNetworkStats::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharArrayWriter> writer;
    CCharArrayWriter::New((ICharArrayWriter**)&writer);
    AutoPtr<IPrintWriter> printwriter;
    CPrintWriter::New(writer, (IPrintWriter**)&printwriter);
    Dump(String(""), printwriter);
    //writer->ToString();
    return NOERROR;
}

ECode CNetworkStats::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->ReadInt64(&mElapsedRealtime);
    parcel->ReadInt32(&mSize);
    parcel->ReadArrayOf((Handle32*)&mIface);
    parcel->ReadArrayOf((Handle32*)&mUid );
    parcel->ReadArrayOf((Handle32*)&mSet);
    parcel->ReadArrayOf((Handle32*)&mTag);
    parcel->ReadArrayOf((Handle32*)&mRxBytes);
    parcel->ReadArrayOf((Handle32*)&mRxPackets);
    parcel->ReadArrayOf((Handle32*)&mTxBytes);
    parcel->ReadArrayOf((Handle32*)&mTxPackets);
    parcel->ReadArrayOf((Handle32*)&mOperations);
    return NOERROR;
}

ECode CNetworkStats::GetIFace(
    /* [out, callee] */ ArrayOf<String>** face)
{
    VALIDATE_NOT_NULL(face);
    *face = mIface;
    ARRAYOF_ADDREF(*face);
    return NOERROR;
}

ECode CNetworkStats::GetUid(
    /* [out, callee] */ ArrayOf<Int32>** uid)
{
    VALIDATE_NOT_NULL(uid);
    *uid = mUid;
    ARRAYOF_ADDREF(*uid);
    return NOERROR;
}

ECode CNetworkStats::GetSet(
    /* [out, callee] */ ArrayOf<Int32>** set)
{
    VALIDATE_NOT_NULL(set);
    *set = mSet;
    ARRAYOF_ADDREF(*set);
    return NOERROR;
}

ECode CNetworkStats::GetTag(
    /* [out, callee] */ ArrayOf<Int32>** tag)
{
    VALIDATE_NOT_NULL(tag);
    *tag = mTag;
    ARRAYOF_ADDREF(*tag);
    return NOERROR;
}

ECode CNetworkStats::GetRxBytes(
    /* [out, callee] */ ArrayOf<Int64>** rxBytes)
{
    VALIDATE_NOT_NULL(rxBytes);
    *rxBytes = mRxBytes;
    ARRAYOF_ADDREF(*rxBytes);
    return NOERROR;
}

ECode CNetworkStats::GetRxPackets(
    /* [out, callee] */ ArrayOf<Int64>** rxPackets)
{
    VALIDATE_NOT_NULL(rxPackets);
    *rxPackets = mRxPackets;
    ARRAYOF_ADDREF(*rxPackets);
    return NOERROR;
}

ECode CNetworkStats::GetTxBytes(
    /* [out, callee] */ ArrayOf<Int64>** txBytes)
{
    VALIDATE_NOT_NULL(txBytes);
    *txBytes = mTxBytes;
    ARRAYOF_ADDREF(*txBytes);
    return NOERROR;
}

ECode CNetworkStats::GetTxPackets(
    /* [out, callee] */ ArrayOf<Int64>** txPackets)
{
    VALIDATE_NOT_NULL(txPackets);
    *txPackets = mTxPackets;
    ARRAYOF_ADDREF(*txPackets);
    return NOERROR;
}

ECode CNetworkStats::GetOperations(
    /* [out, callee] */ ArrayOf<Int64>** operations)
{
    VALIDATE_NOT_NULL(operations);
    *operations = mOperations;
    ARRAYOF_ADDREF(*operations);
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
