
#ifndef __CNETWORKSTATSFACTORY_H__
#define __CNETWORKSTATSFACTORY_H__

#include "_CNetworkStatsFactory.h"

using Elastos::IO::IFile;
using Elastos::Droid::Net::INetworkStats;

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
    CARAPI ReadNetworkStatsDetailEx(
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

#endif //__CNETWORKSTATSFACTORY_H__
