
#include "net/CNetworkStatsFactory.h"
#include "ext/frameworkext.h"

using Elastos::IO::CFile;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

ECode CNetworkStatsFactory::constructor()
{
    AutoPtr<IFile> file;
    CFile::New(String("/proc/"), (IFile**)&file);
    return constructor(file);
}

ECode CNetworkStatsFactory::constructor(
    /* [in] */ IFile* procRoot)
{
    AutoPtr<IFile> mStatsXtIfaceAll;
    CFile::New(procRoot, String("net/xt_qtaguid/iface_stat_all"), (IFile**)&mStatsXtIfaceAll);
    CFile::New(procRoot, String("net/xt_qtaguid/iface_stat_fmt"), (IFile**)&mStatsXtIfaceFmt);
    CFile::New(procRoot, String("net/xt_qtaguid/stats"), (IFile**)&mStatsXtUid);
    return NOERROR;
}

/**
 * Parse and return interface-level summary {@link NetworkStats} measured
 * using {@code /proc/net/dev} style hooks, which may include non IP layer
 * traffic. Values monotonically increase since device boot, and may include
 * details about inactive interfaces.
 *
 * @throws IllegalStateException when problem parsing stats.
 */
ECode CNetworkStatsFactory::ReadNetworkStatsSummaryDev(
    /* [out] */ INetworkStats** stats)
{
    VALIDATE_NOT_NULL(stats);
    *stats = NULL;

    // TODO: need class CProcFileReader
    // AutoPtr<IStrictMode> mode;
    // CStrictMode::AcquireSingleton((IStrictMode**)&mode);
    // AutoPtr<IStrictModeThreadPolicy> savedPolicy;
    // mode->AllowThreadDiskReads((IStrictModeThreadPolicy**)&savedPolicy);

    // CNetworkStats::New(SystemClock::ElapsedRealtime(), 6, stats);
    // assert(stats != NULL);
    // AutoPtr<INetworkStatsEntry> entry;
    // CNetworkStatsEntry::New((INetworkStatsEntry**)&entry);

    // AutoPtr<IProcFileReader> reader;
    // // try {
    // AutoPtr<IFileInputStream> fstream;
    // CFileInputStream::New(mStatsXtIfaceAll, (IFileInputStream**)&fstream);
    // CProcFileReader::New(fstream, (IProcFileReader**)&reader);

    // Boolean hasData;
    // reader->HasMoreData(&hasData);
    // while (hasData) {
    //     String iface;
    //     reader->NextString(&iface);
    //     entry->SetIface(iface);
    //     entry->SetUid(INetworkStats::UID_ALL);
    //     entry->SetSet(INetworkStats::SET_ALL);
    //     entry->SetTag(INetworkStats::TAG_NONE);

    //     Int32 nvalue;
    //     reader->NextInt(&nvalue);
    //     Boolean active = nvalue != 0;

    //     // always include snapshot values
    //     Int64 value;
    //     reader->NextInt64(&value);
    //     entry->SetRxBytes(value);
    //     reader->NextInt64(&value);
    //     entry->SetRxPackets(value);
    //     reader->NextInt64(&value);
    //     entry->SetTxBytes(value);
    //     reader->NextInt64(&value);
    //     entry->SetTxPackets(value);

    //     // fold in active numbers, but only when active
    //     if (active) {
    //         Int64 value, evalue;
    //         reader->NextInt64(&value);
    //         entry->GetRxBytes(&evalue);
    //         entry->SetRxBytes(value + evalue);
    //         reader->NextInt64(&value);
    //         entry->GetRxPackets(&evalue);
    //         entry->SetRxPackets(value + evalue);
    //         reader->NextInt64(&value);
    //         entry->GetTxBytes(&evalue);
    //         entry->SetTxBytes(value + evalue);
    //         reader->NextInt64(&value);
    //         entry->GetTxPackets(&evalue);
    //         entry->SetTxPackets(value + evalue);
    //     }

    //     stats->AddValues(entry);
    //     reader->FinishLine();
    // }
    // // } catch (NullPointerException e) {
    // //     throw new IllegalStateException("problem parsing stats: " + e);
    // // } catch (NumberFormatException e) {
    // //     throw new IllegalStateException("problem parsing stats: " + e);
    // // } catch (IOException e) {
    // //     throw new IllegalStateException("problem parsing stats: " + e);
    // // } finally {
    // //     IoUtils.closeQuietly(reader);
    // //     StrictMode.setThreadPolicy(savedPolicy);
    // // }
    return E_NOT_IMPLEMENTED;
}

/**
 * Parse and return interface-level summary {@link NetworkStats}. Designed
 * to return only IP layer traffic. Values monotonically increase since
 * device boot, and may include details about inactive interfaces.
 *
 * @throws IllegalStateException when problem parsing stats.
 */
ECode CNetworkStatsFactory::ReadNetworkStatsSummaryXt(
    /* [out] */ INetworkStats** stats)
{
    VALIDATE_NOT_NULL(stats);
    *stats = NULL;

    // final StrictMode.ThreadPolicy savedPolicy = StrictMode.allowThreadDiskReads();

    // // return null when kernel doesn't support
    // if (!mStatsXtIfaceFmt.exists()) return null;

    // final NetworkStats stats = new NetworkStats(SystemClock.elapsedRealtime(), 6);
    // final NetworkStats.Entry entry = new NetworkStats.Entry();

    // ProcFileReader reader = null;
    // try {
    //     // open and consume header line
    //     reader = new ProcFileReader(new FileInputStream(mStatsXtIfaceFmt));
    //     reader.finishLine();

    //     while (reader.hasMoreData()) {
    //         entry.iface = reader.nextString();
    //         entry.uid = INetworkStats::UID_ALL;
    //         entry.set = INetworkStats::SET_ALL;
    //         entry.tag = INetworkStats::TAG_NONE;

    //         entry.rxBytes = reader.nextLong();
    //         entry.rxPackets = reader.nextLong();
    //         entry.txBytes = reader.nextLong();
    //         entry.txPackets = reader.nextLong();

    //         stats.addValues(entry);
    //         reader.finishLine();
    //     }
    // } catch (NullPointerException e) {
    //     throw new IllegalStateException("problem parsing stats: " + e);
    // } catch (NumberFormatException e) {
    //     throw new IllegalStateException("problem parsing stats: " + e);
    // } catch (IOException e) {
    //     throw new IllegalStateException("problem parsing stats: " + e);
    // } finally {
    //     IoUtils.closeQuietly(reader);
    //     StrictMode.setThreadPolicy(savedPolicy);
    // }
    // return stats;
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkStatsFactory::ReadNetworkStatsDetail(
    /* [out] */ INetworkStats** stats)
{
    VALIDATE_NOT_NULL(stats);
    *stats = NULL;
    return ReadNetworkStatsDetailEx(INetworkStats::UID_ALL, stats);
}

/**
 * Parse and return {@link NetworkStats} with UID-level details. Values
 * monotonically increase since device boot.
 *
 * @throws IllegalStateException when problem parsing stats.
 */
ECode CNetworkStatsFactory::ReadNetworkStatsDetailEx(
    /* [in] */ Int32 limitUid,
    /* [out] */ INetworkStats** stats)
{
    VALIDATE_NOT_NULL(stats);
    *stats = NULL;
    // final StrictMode.ThreadPolicy savedPolicy = StrictMode.allowThreadDiskReads();

    // final NetworkStats stats = new NetworkStats(SystemClock.elapsedRealtime(), 24);
    // final NetworkStats.Entry entry = new NetworkStats.Entry();

    // int idx = 1;
    // int lastIdx = 1;

    // ProcFileReader reader = null;
    // try {
    //     // open and consume header line
    //     reader = new ProcFileReader(new FileInputStream(mStatsXtUid));
    //     reader.finishLine();

    //     while (reader.hasMoreData()) {
    //         idx = reader.nextInt();
    //         if (idx != lastIdx + 1) {
    //             throw new IllegalStateException(
    //                     "inconsistent idx=" + idx + " after lastIdx=" + lastIdx);
    //         }
    //         lastIdx = idx;

    //         entry.iface = reader.nextString();
    //         entry.tag = kernelToTag(reader.nextString());
    //         entry.uid = reader.nextInt();
    //         entry.set = reader.nextInt();
    //         entry.rxBytes = reader.nextLong();
    //         entry.rxPackets = reader.nextLong();
    //         entry.txBytes = reader.nextLong();
    //         entry.txPackets = reader.nextLong();

    //         if (limitUid == INetworkStats::UID_ALL || limitUid == entry.uid) {
    //             stats.addValues(entry);
    //         }

    //         reader.finishLine();
    //     }
    // } catch (NullPointerException e) {
    //     throw new IllegalStateException("problem parsing idx " + idx, e);
    // } catch (NumberFormatException e) {
    //     throw new IllegalStateException("problem parsing idx " + idx, e);
    // } catch (IOException e) {
    //     throw new IllegalStateException("problem parsing idx " + idx, e);
    // } finally {
    //     IoUtils.closeQuietly(reader);
    //     StrictMode.setThreadPolicy(savedPolicy);
    // }

    // return stats;
    return E_NOT_IMPLEMENTED;
}

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos
