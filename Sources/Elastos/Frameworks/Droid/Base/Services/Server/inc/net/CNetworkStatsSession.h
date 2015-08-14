
#ifndef __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSESSION_H__
#define __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSESSION_H__

#include "_CNetworkStatsSession.h"
#include "net/CNetworkStatsService.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

CarClass(CNetworkStatsSession)
{
public:
    /** Return network layer usage summary for traffic that matches template. */
    CARAPI GetSummaryForNetwork(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [out] */ INetworkStats** result);

    /** Return historical network layer stats for traffic that matches template. */
    CARAPI GetHistoryForNetwork(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int32 fields,
        /* [out] */ INetworkStatsHistory** result);

    /** Return network layer usage summary per UID for traffic that matches template. */
    CARAPI GetSummaryForAllUid(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ Boolean includeTags,
        /* [out] */ INetworkStats** result);

    /** Return historical network layer stats for specific UID traffic that matches template. */
    CARAPI GetHistoryForUid(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 set,
        /* [in] */ Int32 tag,
        /* [in] */ Int32 fields,
        /* [out] */ INetworkStatsHistory** result);

    CARAPI Close();

    CARAPI constructor(
        /* [in] */ Handle32 host);

private:
    CARAPI_(AutoPtr<NetworkStatsCollection>) GetUidComplete();

    CARAPI_(AutoPtr<NetworkStatsCollection>) GetUidTagComplete();

private:
    AutoPtr<NetworkStatsCollection> mUidComplete;
    AutoPtr<NetworkStatsCollection> mUidTagComplete;

    CNetworkStatsService* mHost;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSESSION_H__
