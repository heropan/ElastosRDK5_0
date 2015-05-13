
#include "net/CNetworkStatsSession.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

AutoPtr<NetworkStatsCollection> CNetworkStatsSession::GetUidComplete()
{
    if (mUidComplete == NULL) {
        Mutex::Autolock lock(mHost->_m_syncLock);
        mUidComplete = mHost->mUidRecorder->GetOrLoadCompleteLocked();
    }
    return mUidComplete;
}

AutoPtr<NetworkStatsCollection> CNetworkStatsSession::GetUidTagComplete()
{
    if (mUidTagComplete == NULL) {
        Mutex::Autolock lock(mHost->_m_syncLock);
        mUidTagComplete = mHost->mUidTagRecorder->GetOrLoadCompleteLocked();
    }
    return mUidTagComplete;
}

ECode CNetworkStatsSession::GetSummaryForNetwork(
    /* [in] */ INetworkTemplate* templ,
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<INetworkStats> stats = mHost->InternalGetSummaryForNetwork(templ, start, end);
    *result = stats;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CNetworkStatsSession::GetHistoryForNetwork(
    /* [in] */ INetworkTemplate* templ,
    /* [in] */ Int32 fields,
    /* [out] */ INetworkStatsHistory** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<INetworkStatsHistory> history = mHost->InternalGetHistoryForNetwork(templ, fields);
    *result = history;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CNetworkStatsSession::GetSummaryForAllUid(
    /* [in] */ INetworkTemplate* templ,
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ Boolean includeTags,
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<INetworkStats> stats = GetUidComplete()->GetSummary(templ, start, end);
    if (includeTags) {
        AutoPtr<INetworkStats> tagStats = GetUidTagComplete()->GetSummary(templ, start, end);
        stats->CombineAllValues(tagStats);
    }
    *result = stats;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CNetworkStatsSession::GetHistoryForUid(
    /* [in] */ INetworkTemplate* templ,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 set,
    /* [in] */ Int32 tag,
    /* [in] */ Int32 fields,
    /* [out] */ INetworkStatsHistory** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<INetworkStatsHistory> history;
    if (tag == INetworkStats::TAG_NONE) {
        history = GetUidComplete()->GetHistory(templ, uid, set, tag, fields);
    }
    else {
        history = GetUidTagComplete()->GetHistory(templ, uid, set, tag, fields);
    }
    *result = history;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CNetworkStatsSession::Close()
{
    mUidComplete = NULL;
    mUidTagComplete = NULL;
    return NOERROR;
}

ECode CNetworkStatsSession::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CNetworkStatsService*)host;
    return NOERROR;
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
