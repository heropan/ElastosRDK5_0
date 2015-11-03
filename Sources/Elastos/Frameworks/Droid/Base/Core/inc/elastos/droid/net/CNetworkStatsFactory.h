
#ifndef __ELASTOS_DROID_INTERNAL_NET_CNETWORKSTATSFACTORY_H__
#define __ELASTOS_DROID_INTERNAL_NET_CNETWORKSTATSFACTORY_H__

#include "_Elastos_Droid_Internal_Net_CNetworkStatsFactory.h"

using Elastos::Droid::Net::INetworkStats;

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CarClass(CNetworkStatsFactory)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFile* procRoot);

    /**
     * Parse and return interface-level summary {@link NetworkStats} measured
     * using {@code /proc/net/dev} style hooks, which may include non IP layer
     * traffic. Values monotonically increase since device boot, and may include
     * details about inactive interfaces.
     *
     * @throws IllegalStateException when problem parsing stats.
     */
    CARAPI ReadNetworkStatsSummaryDev(
        /* [out] */ INetworkStats** stats);

    /**
     * Parse and return interface-level summary {@link NetworkStats}. Designed
     * to return only IP layer traffic. Values monotonically increase since
     * device boot, and may include details about inactive interfaces.
     *
     * @throws IllegalStateException when problem parsing stats.
     */
    CARAPI ReadNetworkStatsSummaryXt(
        /* [out] */ INetworkStats** stats);

    CARAPI ReadNetworkStatsDetail(
        /* [out] */ INetworkStats** stats);

    /**
     * Parse and return {@link NetworkStats} with UID-level details. Values
     * monotonically increase since device boot.
     *
     * @throws IllegalStateException when problem parsing stats.
     */
    CARAPI ReadNetworkStatsDetail(
        /* [in] */ Int32 limitUid,
        /* [out] */ INetworkStats** stats);

private:
    /** Path to {@code /proc/net/xt_qtaguid/iface_stat_all}. */
    AutoPtr<IFile> mStatsXtIfaceAll;
    /** Path to {@code /proc/net/xt_qtaguid/iface_stat_fmt}. */
    AutoPtr<IFile> mStatsXtIfaceFmt;
    /** Path to {@code /proc/net/xt_qtaguid/stats}. */
    AutoPtr<IFile> mStatsXtUid;
};

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_NET_CNETWORKSTATSFACTORY_H__
