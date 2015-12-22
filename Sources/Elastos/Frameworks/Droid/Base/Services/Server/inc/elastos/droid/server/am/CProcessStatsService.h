
#ifndef __ELASTOS_DROID_SERVER_AM_CPROCESSSTATSSERVICE_H__
#define __ELASTOS_DROID_SERVER_AM_CPROCESSSTATSSERVICE_H__

#include "_Elastos_Droid_Server_Am_CProcessStatsService.h"
#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.Utility.h"
#include "Elastos.CoreLibrary.IO.h"
#include "Elastos.CoreLibrary.Utility.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Internal::App::IIProcessStats;
using Elastos::Droid::Internal::App::IProcessStats;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Utility::IAtomicFile;
using Elastos::Droid::View::IApplicationToken;
using Elastos::IO::IFile;
using Elastos::Utility::Concurrent::Locks::IReentrantLock;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class CActivityManagerService;

CarClass(CProcessStatsService)
    , public Object
    , public IIProcessStats
    , public IBinder
{
private:
    class CallbackRunnable : public Runnable
    {
    public:
        CallbackRunnable(
            /* [in] */ CProcessStatsService* host)
            : mHost(host)
        {}

        // @Override
        CARAPI Run()
        {
            AutoLock lock(mHost->mAm);
            if (mHost->mProcessStats->EvaluateSystemProperties(FALSE)) {
                Int32 flags;
                mHost->mProcessStats->GetFlags(&flags);
                flags |= IProcessStats::FLAG_SYSPROPS;
                mHost->mProcessStats->SetFlags(flags);
                mHost->WriteStateLocked(TRUE, TRUE);
                mHost->mProcessStats->EvaluateSystemProperties(TRUE);
            }
            return NOERROR;
        }

    private:
        CProcessStatsService* mHost;
    };

public:
    CProcessStatsService();

    ~CProcessStatsService();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

public:
    static const String TAG = "ProcessStatsService";
    static const Boolean DEBUG = FALSE;

    // Most data is kept in a sparse data structure: an integer array which integer
    // holds the type of the entry, and the identifier for a Int64 array that data
    // exists in and the offset into the array to find it.  The constants below
    // define the encoding of that data in an integer.

    static const Int32 MAX_HISTORIC_STATES = 8;   // Maximum number of historic states we will keep.
    static const String STATE_FILE_PREFIX = "state-"; // Prefix to use for state filenames.
    static const String STATE_FILE_SUFFIX = ".bin"; // Suffix to use for state filenames.
    static const String STATE_FILE_CHECKIN_SUFFIX = ".ci"; // State files that have checked in.
    static Int64 WRITE_PERIOD = 30*60*1000;      // Write file every 30 minutes or so.

    CActivityManagerService* mAm;
    AutoPtr<IFile> mBaseDir;
    AutoPtr<IProcessStats> mProcessStats;
    AutoPtr<IAtomicFile> mFile;
    Boolean mCommitPending;
    Boolean mShuttingDown;
    Int32 mLastMemOnlyState;
    Boolean mMemFactorLowered;

    AutoPtr<IReentrantLock> mWriteLock;
    Object mPendingWriteLock;
    AutoPtr<IAtomicFile> mPendingWriteFile;
    AutoPtr<IParcel> mPendingWrite;
    Boolean mPendingWriteCommitted;
    Int64 mLastWriteTime;

    ProcessStatsService(
        /* [in] */ CActivityManagerService* am,
        /* [in] */ IFile* file)
        : mAm(am)
        , mBaseDir(file)
        , mCommitPending(FALSE)
        , mShuttingDown(FALSE)
        , mLastMemOnlyState(-1)
        , mMemFactorLowered(FALSE)
        , mPendingWriteCommitted(FALSE)
        , mLastWriteTime(0)
    {
        CReentrantLock::New((IReentrantLock**)&mWriteLock);
        Boolean res;
        mBaseDir->Mkdirs(&res);
        CProcessStats::New(TRUE, (IProcessStats**)&mProcessStats);
        UpdateFile();
        AutoPtr<CallbackRunnable> runnable = new CallbackRunnable(this);
        AutoPtr<ISystemProperties> sysProps;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProps);
        sysProps->AddChangeCallback(runnable);
    }

    AutoPtr<IProcessState> GetProcessStateLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 versionCode,
        /* [in] */ const String& processName)
    {
        AutoPtr<IProcessState> procstat;
        mProcessStats->GetProcessStateLocked(packageName, uid, versionCode, processName,
            (IProcessState**)&procstat);
        return procstat;
    }

    AutoPtr<IServiceState> GetServiceStateLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 versionCode,
        /* [in] */ const String& processName,
        /* [in] */ const String& className)
    {
        AutoPtr<IServiceState> svcState;
        mProcessStats->GetServiceStateLocked(packageName, uid, versionCode, processName,
            className, (IServiceState**)&svcState);
        return svcState;
    }

    Boolean IsMemFactorLowered()
    {
        return mMemFactorLowered;
    }

    Boolean SetMemFactorLocked(Int32 memFactor, Boolean screenOn, Int64 now)
    {
        mMemFactorLowered = memFactor < mLastMemOnlyState;
        mLastMemOnlyState = memFactor;
        if (screenOn) {
            memFactor += ProcessStats.ADJ_SCREEN_ON;
        }
        if (memFactor != mProcessStats->mMemFactor) {
            if (mProcessStats->mMemFactor != ProcessStats.STATE_NOTHING) {
                mProcessStats->mMemFactorDurations[mProcessStats->mMemFactor]
                        += now - mProcessStats->mStartTime;
            }
            mProcessStats->mMemFactor = memFactor;
            mProcessStats->mStartTime = now;
            final ArrayMap<String, SparseArray<SparseArray<ProcessStats.PackageState>>> pmap
                    = mProcessStats->mPackages.getMap();
            for (Int32 ipkg=pmap.size()-1; ipkg>=0; ipkg--) {
                final SparseArray<SparseArray<ProcessStats.PackageState>> uids = pmap.valueAt(ipkg);
                for (Int32 iuid=uids.size()-1; iuid>=0; iuid--) {
                    final SparseArray<ProcessStats.PackageState> vers = uids.valueAt(iuid);
                    for (Int32 iver=vers.size()-1; iver>=0; iver--) {
                        final ProcessStats.PackageState pkg = vers.valueAt(iver);
                        final ArrayMap<String, IServiceState> services = pkg.mServices;
                        for (Int32 isvc=services.size()-1; isvc>=0; isvc--) {
                            final IServiceState service = services.valueAt(isvc);
                            if (service.isRestarting()) {
                                service.setRestarting(TRUE, memFactor, now);
                            } else if (service.isInUse()) {
                                if (service.mStartedState != ProcessStats.STATE_NOTHING) {
                                    service.setStarted(TRUE, memFactor, now);
                                }
                                if (service.mBoundState != ProcessStats.STATE_NOTHING) {
                                    service.setBound(TRUE, memFactor, now);
                                }
                                if (service.mExecState != ProcessStats.STATE_NOTHING) {
                                    service.setExecuting(TRUE, memFactor, now);
                                }
                            }
                        }
                    }
                }
            }
            return TRUE;
        }
        return FALSE;
    }

    Int32 GetMemFactorLocked()
    {
        return mProcessStats->mMemFactor != ProcessStats.STATE_NOTHING ? mProcessStats->mMemFactor : 0;
    }

    void AddSysMemUsageLocked(Int64 cachedMem, Int64 freeMem, Int64 zramMem, Int64 kernelMe
        ,
            Int64 nativeMem) {
        mProcessStats->addSysMemUsage(cachedMem, freeMem, zramMem, kernelMem, nativeMem);
    }

    Boolean ShouldWriteNowLocked(Int64 now)
    {
        if (now > (mLastWriteTime+WRITE_PERIOD)) {
            if (SystemClock.elapsedRealtime()
                    > (mProcessStats->mTimePeriodStartRealtime+ProcessStats.COMMIT_PERIOD) &&
                    SystemClock.uptimeMillis()
                    > (mProcessStats->mTimePeriodStartUptime+ProcessStats.COMMIT_UPTIME_PERIOD)) {
                mCommitPending = TRUE;
            }
            return TRUE;
        }
        return FALSE;
    }

    void ShutdownLocked()
    {
        Slog.w(TAG, "Writing process stats before shutdown...");
        mProcessStats->mFlags |= ProcessStats.FLAG_SHUTDOWN;
        writeStateSyncLocked();
        mShuttingDown = TRUE;
    }

    void WriteStateAsyncLocked()
    {
        writeStateLocked(FALSE);
    }

    void WriteStateSyncLocked()
    {
        writeStateLocked(TRUE);
    }

    private void writeStateLocked(Boolean sync) {
        if (mShuttingDown) {
            return;
        }
        Boolean commitPending = mCommitPending;
        mCommitPending = FALSE;
        writeStateLocked(sync, commitPending);
    }

    void WriteStateLocked(Boolean sync, final Boolean commit)
    {
        synchronized (mPendingWriteLock) {
            Int64 now = SystemClock.uptimeMillis();
            if (mPendingWrite == null || !mPendingWriteCommitted) {
                mPendingWrite = Parcel.obtain();
                mProcessStats->mTimePeriodEndRealtime = SystemClock.elapsedRealtime();
                mProcessStats->mTimePeriodEndUptime = now;
                if (commit) {
                    mProcessStats->mFlags |= ProcessStats.FLAG_COMPLETE;
                }
                mProcessStats->writeToParcel(mPendingWrite, 0);
                mPendingWriteFile = new AtomicFile(mFile.getBaseFile());
                mPendingWriteCommitted = commit;
            }
            if (commit) {
                mProcessStats->resetSafely();
                UpdateFile();
            }
            mLastWriteTime = SystemClock.uptimeMillis();
            Slog.i(TAG, "Prepared write state in " + (SystemClock.uptimeMillis()-now) + "ms");
            if (!sync) {
                BackgroundThread.getHandler().post(new Runnable() {
                    @Override void Run()
                    {
                        performWriteState();
                    }
                });
                return;
            }
        }

        performWriteState();
    }

    private void UpdateFile() {
        mFile = new AtomicFile(new File(mBaseDir, STATE_FILE_PREFIX
                + mProcessStats->mTimePeriodStartClockStr + STATE_FILE_SUFFIX));
        mLastWriteTime = SystemClock.uptimeMillis();
    }

    void performWriteState() {
        if (DEBUG) Slog.d(TAG, "Performing write to " + mFile.getBaseFile());
        Parcel data;
        AtomicFile file;
        synchronized (mPendingWriteLock) {
            data = mPendingWrite;
            file = mPendingWriteFile;
            mPendingWriteCommitted = FALSE;
            if (data == null) {
                return;
            }
            mPendingWrite = null;
            mPendingWriteFile = null;
            mWriteLock.lock();
        }

        FileOutputStream stream = null;
        try {
            stream = file.startWrite();
            stream.write(data.marshall());
            stream.flush();
            file.finishWrite(stream);
            if (DEBUG) Slog.d(TAG, "Write completed successfully!");
        } catch (IOException e) {
            Slog.w(TAG, "Error writing process statistics", e);
            file.failWrite(stream);
        } finally {
            data.recycle();
            trimHistoricStatesWriteLocked();
            mWriteLock.unlock();
        }
    }

    Boolean readLocked(ProcessStats stats, AtomicFile file) {
        try {
            FileInputStream stream = file.openRead();
            stats.read(stream);
            stream.close();
            if (stats.mReadError != null) {
                Slog.w(TAG, "Ignoring existing stats; " + stats.mReadError);
                if (DEBUG) {
                    ArrayMap<String, SparseArray<IProcessState>> procMap
                            = stats.mProcesses.getMap();
                    final Int32 NPROC = procMap.size();
                    for (Int32 ip=0; ip<NPROC; ip++) {
                        Slog.w(TAG, "Process: " + procMap.keyAt(ip));
                        SparseArray<IProcessState> uids = procMap.valueAt(ip);
                        final Int32 NUID = uids.size();
                        for (Int32 iu=0; iu<NUID; iu++) {
                            Slog.w(TAG, "  Uid " + uids.keyAt(iu) + ": " + uids.valueAt(iu));
                        }
                    }
                    ArrayMap<String, SparseArray<SparseArray<ProcessStats.PackageState>>> pkgMap
                            = stats.mPackages.getMap();
                    final Int32 NPKG = pkgMap.size();
                    for (Int32 ip=0; ip<NPKG; ip++) {
                        Slog.w(TAG, "Package: " + pkgMap.keyAt(ip));
                        SparseArray<SparseArray<ProcessStats.PackageState>> uids
                                = pkgMap.valueAt(ip);
                        final Int32 NUID = uids.size();
                        for (Int32 iu=0; iu<NUID; iu++) {
                            Slog.w(TAG, "  Uid: " + uids.keyAt(iu));
                            SparseArray<ProcessStats.PackageState> vers = uids.valueAt(iu);
                            final Int32 NVERS = vers.size();
                            for (Int32 iv=0; iv<NVERS; iv++) {
                                Slog.w(TAG, "    Vers: " + vers.keyAt(iv));
                                ProcessStats.PackageState pkgState = vers.valueAt(iv);
                                final Int32 NPROCS = pkgState.mProcesses.size();
                                for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                                    Slog.w(TAG, "      Process " + pkgState.mProcesses.keyAt(iproc)
                                            + ": " + pkgState.mProcesses.valueAt(iproc));
                                }
                                final Int32 NSRVS = pkgState.mServices.size();
                                for (Int32 isvc=0; isvc<NSRVS; isvc++) {
                                    Slog.w(TAG, "      Service " + pkgState.mServices.keyAt(isvc)
                                            + ": " + pkgState.mServices.valueAt(isvc));

                                }
                            }
                        }
                    }
                }
                return FALSE;
            }
        } catch (Throwable e) {
            stats.mReadError = "caught exception: " + e;
            Slog.e(TAG, "Error reading process statistics", e);
            return FALSE;
        }
        return TRUE;
    }

    private ArrayList<String> getCommittedFiles(Int32 minNum, Boolean inclCurrent,
            Boolean inclCheckedIn) {
        File[] files = mBaseDir.listFiles();
        if (files == null || files.length <= minNum) {
            return null;
        }
        ArrayList<String> filesArray = new ArrayList<String>(files.length);
        String currentFile = mFile.getBaseFile().getPath();
        if (DEBUG) Slog.d(TAG, "Collecting " + files.length + " files except: " + currentFile);
        for (Int32 i=0; i<files.length; i++) {
            File file = files[i];
            String fileStr = file.getPath();
            if (DEBUG) Slog.d(TAG, "Collecting: " + fileStr);
            if (!inclCheckedIn && fileStr.endsWith(STATE_FILE_CHECKIN_SUFFIX)) {
                if (DEBUG) Slog.d(TAG, "Skipping: already checked in");
                continue;
            }
            if (!inclCurrent && fileStr.equals(currentFile)) {
                if (DEBUG) Slog.d(TAG, "Skipping: current stats");
                continue;
            }
            filesArray.add(fileStr);
        }
        Collections.sort(filesArray);
        return filesArray;
    }

    void TrimHistoricStatesWriteLocked()
    {
        ArrayList<String> filesArray = getCommittedFiles(MAX_HISTORIC_STATES, FALSE, TRUE);
        if (filesArray == null) {
            return;
        }
        while (filesArray.size() > MAX_HISTORIC_STATES) {
            String file = filesArray.remove(0);
            Slog.i(TAG, "Pruning old procstats: " + file);
            (new File(file)).delete();
        }
    }

    Boolean dumpFilteredProcessesCsvLocked(PrintWriter pw, String header,
            Boolean sepScreenStates, Int32[] screenStates, Boolean sepMemStates, Int32[] memStates,
            Boolean sepProcStates, Int32[] procStates, Int64 now, String reqPackage) {
        ArrayList<IProcessState> procs = mProcessStats->collectProcessesLocked(
                screenStates, memStates, procStates, procStates, now, reqPackage, FALSE);
        if (procs.size() > 0) {
            if (header != null) {
                pw.println(header);
            }
            ProcessStats.dumpProcessListCsv(pw, procs, sepScreenStates, screenStates,
                    sepMemStates, memStates, sepProcStates, procStates, now);
            return TRUE;
        }
        return FALSE;
    }

    static Int32[] parseStateList(String[] states, Int32 mult, String arg, Boolean[] outSep,
            String[] outError) {
        ArrayList<Integer> res = new ArrayList<Integer>();
        Int32 lastPos = 0;
        for (Int32 i=0; i<=arg.length(); i++) {
            char c = i < arg.length() ? arg.charAt(i) : 0;
            if (c != ',' && c != '+' && c != ' ' && c != 0) {
                continue;
            }
            Boolean isSep = c == ',';
            if (lastPos == 0) {
                // We now know the type of op.
                outSep[0] = isSep;
            } else if (c != 0 && outSep[0] != isSep) {
                outError[0] = "inconsistent separators (can't mix ',' with '+')";
                return null;
            }
            if (lastPos < (i-1)) {
                String str = arg.substring(lastPos, i);
                for (Int32 j=0; j<states.length; j++) {
                    if (str.equals(states[j])) {
                        res.add(j);
                        str = null;
                        break;
                    }
                }
                if (str != null) {
                    outError[0] = "invalid word \"" + str + "\"";
                    return null;
                }
            }
            lastPos = i + 1;
        }

        Int32[] finalRes = new Int32[res.size()];
        for (Int32 i=0; i<res.size(); i++) {
            finalRes[i] = res.get(i) * mult;
        }
        return finalRes;
    }

    byte[] getCurrentStats(List<ParcelFileDescriptor> historic)
    {
        mAm.mContext.enforceCallingOrSelfPermission(
                android.Manifest.permission.PACKAGE_USAGE_STATS, null);
        Parcel current = Parcel.obtain();
        mWriteLock.lock();
        try {
            synchronized (mAm) {
                Int64 now = SystemClock.uptimeMillis();
                mProcessStats->mTimePeriodEndRealtime = SystemClock.elapsedRealtime();
                mProcessStats->mTimePeriodEndUptime = now;
                mProcessStats->writeToParcel(current, now, 0);
            }
            if (historic != null) {
                ArrayList<String> files = getCommittedFiles(0, FALSE, TRUE);
                if (files != null) {
                    for (Int32 i=files.size()-1; i>=0; i--) {
                        try {
                            ParcelFileDescriptor pfd = ParcelFileDescriptor.open(
                                    new File(files.get(i)), ParcelFileDescriptor.MODE_READ_ONLY);
                            historic.add(pfd);
                        } catch (IOException e) {
                            Slog.w(TAG, "Failure opening procstat file " + files.get(i), e);
                        }
                    }
                }
            }
        } finally {
            mWriteLock.unlock();
        }
        return current.marshall();
    }

    ParcelFileDescriptor GetStatsOverTime(Int64 minTime)
    {
        mAm.mContext.enforceCallingOrSelfPermission(
                android.Manifest.permission.PACKAGE_USAGE_STATS, null);
        mWriteLock.lock();
        try {
            Parcel current = Parcel.obtain();
            Int64 curTime;
            synchronized (mAm) {
                Int64 now = SystemClock.uptimeMillis();
                mProcessStats->mTimePeriodEndRealtime = SystemClock.elapsedRealtime();
                mProcessStats->mTimePeriodEndUptime = now;
                mProcessStats->writeToParcel(current, now, 0);
                curTime = mProcessStats->mTimePeriodEndRealtime
                        - mProcessStats->mTimePeriodStartRealtime;
            }
            if (curTime < minTime) {
                // Need to add in older stats to reach desired time.
                ArrayList<String> files = getCommittedFiles(0, FALSE, TRUE);
                if (files != null && files.size() > 0) {
                    current.setDataPosition(0);
                    ProcessStats stats = ProcessStats.CREATOR.createFromParcel(current);
                    current.recycle();
                    Int32 i = files.size()-1;
                    while (i >= 0 && (stats.mTimePeriodEndRealtime
                            - stats.mTimePeriodStartRealtime) < minTime) {
                        AtomicFile file = new AtomicFile(new File(files.get(i)));
                        i--;
                        ProcessStats moreStats = new ProcessStats(FALSE);
                        readLocked(moreStats, file);
                        if (moreStats.mReadError == null) {
                            stats.add(moreStats);
                            StringBuilder sb = new StringBuilder();
                            sb.append("Added stats: ");
                            sb.append(moreStats.mTimePeriodStartClockStr);
                            sb.append(", over ");
                            TimeUtils.formatDuration(moreStats.mTimePeriodEndRealtime
                                    - moreStats.mTimePeriodStartRealtime, sb);
                            Slog.i(TAG, sb.toString());
                        } else {
                            Slog.w(TAG, "Failure reading " + files.get(i+1) + "; "
                                    + moreStats.mReadError);
                            continue;
                        }
                    }
                    current = Parcel.obtain();
                    stats.writeToParcel(current, 0);
                }
            }
            final byte[] outData = current.marshall();
            current.recycle();
            final ParcelFileDescriptor[] fds = ParcelFileDescriptor.createPipe();
            Thread thr = new Thread("ProcessStats pipe output") {
                void Run()
                {
                    FileOutputStream fout = new ParcelFileDescriptor.AutoCloseOutputStream(fds[1]);
                    try {
                        fout.write(outData);
                        fout.close();
                    } catch (IOException e) {
                        Slog.w(TAG, "Failure writing pipe", e);
                    }
                }
            };
            thr.start();
            return fds[0];
        } catch (IOException e) {
            Slog.w(TAG, "Failed building output pipe", e);
        } finally {
            mWriteLock.unlock();
        }
        return null;
    }

    Int32 GetCurrentMemoryState()
    {
        synchronized (mAm) {
            return mLastMemOnlyState;
        }
    }

    private void dumpAggregatedStats(PrintWriter pw, Int64 aggregateHours, Int64 now,
            String reqPackage, Boolean isCompact, Boolean dumpDetails, Boolean dumpFullDetails,
            Boolean dumpAll, Boolean activeOnly) {
        ParcelFileDescriptor pfd = getStatsOverTime(aggregateHours*60*60*1000
                - (ProcessStats.COMMIT_PERIOD/2));
        if (pfd == null) {
            pw.println("Unable to build stats!");
            return;
        }
        ProcessStats stats = new ProcessStats(FALSE);
        InputStream stream = new ParcelFileDescriptor.AutoCloseInputStream(pfd);
        stats.read(stream);
        if (stats.mReadError != null) {
            pw.print("Failure reading: "); pw.println(stats.mReadError);
            return;
        }
        if (isCompact) {
            stats.dumpCheckinLocked(pw, reqPackage);
        } else {
            if (dumpDetails || dumpFullDetails) {
                stats.dumpLocked(pw, reqPackage, now, !dumpFullDetails, dumpAll, activeOnly);
            } else {
                stats.dumpSummaryLocked(pw, reqPackage, now, activeOnly);
            }
        }
    }

    static private void dumpHelp(PrintWriter pw) {
        pw.println("Process stats (procstats) dump options:");
        pw.println("    [--checkin|-c|--csv] [--csv-screen] [--csv-proc] [--csv-mem]");
        pw.println("    [--details] [--full-details] [--current] [--hours N] [--last N]");
        pw.println("    [--active] [--commit] [--reset] [--clear] [--write] [-h] [<package.name>]");
        pw.println("  --checkin: perform a checkin: print and delete old committed states.");
        pw.println("  --c: print only state in checkin format.");
        pw.println("  --csv: output data suitable for putting in a spreadsheet.");
        pw.println("  --csv-screen: on, off.");
        pw.println("  --csv-mem: norm, mod, low, crit.");
        pw.println("  --csv-proc: pers, top, fore, vis, precept, backup,");
        pw.println("    service, home, prev, cached");
        pw.println("  --details: dump per-package details, not just summary.");
        pw.println("  --full-details: dump all timing and active state details.");
        pw.println("  --current: only dump current state.");
        pw.println("  --hours: aggregate over about N last hours.");
        pw.println("  --last: only show the last committed stats at index N (starting at 1).");
        pw.println("  --active: only show currently active processes/services.");
        pw.println("  --commit: commit current stats to disk and reset to start new stats.");
        pw.println("  --reset: reset current stats, without committing.");
        pw.println("  --clear: clear all stats; does both --reset and deletes old stats.");
        pw.println("  --write: write current in-memory stats to disk.");
        pw.println("  --read: replace current stats with last-written stats.");
        pw.println("  -a: print everything.");
        pw.println("  -h: print this help text.");
        pw.println("  <package.name>: optional name of package to filter output by.");
    }

    @Override
    protected void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
        if (mAm.checkCallingPermission(android.Manifest.permission.DUMP)
                != PackageManager.PERMISSION_GRANTED) {
            pw.println("Permission Denial: can't dump procstats from from pid="
                    + Binder.getCallingPid() + ", uid=" + Binder.getCallingUid()
                    + " without permission " + android.Manifest.permission.DUMP);
            return;
        }

        Int64 ident = Binder.clearCallingIdentity();
        try {
            dumpInner(fd, pw, args);
        } finally {
            Binder.restoreCallingIdentity(ident);
        }
    }

    private void dumpInner(FileDescriptor fd, PrintWriter pw, String[] args) {
        final Int64 now = SystemClock.uptimeMillis();

        Boolean isCheckin = FALSE;
        Boolean isCompact = FALSE;
        Boolean isCsv = FALSE;
        Boolean currentOnly = FALSE;
        Boolean dumpDetails = FALSE;
        Boolean dumpFullDetails = FALSE;
        Boolean dumpAll = FALSE;
        Int32 aggregateHours = 0;
        Int32 lastIndex = 0;
        Boolean activeOnly = FALSE;
        String reqPackage = null;
        Boolean csvSepScreenStats = FALSE;
        Int32[] csvScreenStats = new Int32[] { ProcessStats.ADJ_SCREEN_OFF, ProcessStats.ADJ_SCREEN_ON};
        Boolean csvSepMemStats = FALSE;
        Int32[] csvMemStats = new Int32[] { ProcessStats.ADJ_MEM_FACTOR_CRITICAL};
        Boolean csvSepProcStats = TRUE;
        Int32[] csvProcStats = ProcessStats.ALL_PROC_STATES;
        if (args != null) {
            for (Int32 i=0; i<args.length; i++) {
                String arg = args[i];
                if ("--checkin".equals(arg)) {
                    isCheckin = TRUE;
                } else if ("-c".equals(arg)) {
                    isCompact = TRUE;
                } else if ("--csv".equals(arg)) {
                    isCsv = TRUE;
                } else if ("--csv-screen".equals(arg)) {
                    i++;
                    if (i >= args.length) {
                        pw.println("Error: argument required for --csv-screen");
                        dumpHelp(pw);
                        return;
                    }
                    Boolean[] sep = new Boolean[1];
                    String[] error = new String[1];
                    csvScreenStats = parseStateList(ProcessStats.ADJ_SCREEN_NAMES_CSV, ProcessStats.ADJ_SCREEN_MOD,
                            args[i], sep, error);
                    if (csvScreenStats == null) {
                        pw.println("Error in \"" + args[i] + "\": " + error[0]);
                        dumpHelp(pw);
                        return;
                    }
                    csvSepScreenStats = sep[0];
                } else if ("--csv-mem".equals(arg)) {
                    i++;
                    if (i >= args.length) {
                        pw.println("Error: argument required for --csv-mem");
                        dumpHelp(pw);
                        return;
                    }
                    Boolean[] sep = new Boolean[1];
                    String[] error = new String[1];
                    csvMemStats = parseStateList(ProcessStats.ADJ_MEM_NAMES_CSV, 1, args[i], sep, error);
                    if (csvMemStats == null) {
                        pw.println("Error in \"" + args[i] + "\": " + error[0]);
                        dumpHelp(pw);
                        return;
                    }
                    csvSepMemStats = sep[0];
                } else if ("--csv-proc".equals(arg)) {
                    i++;
                    if (i >= args.length) {
                        pw.println("Error: argument required for --csv-proc");
                        dumpHelp(pw);
                        return;
                    }
                    Boolean[] sep = new Boolean[1];
                    String[] error = new String[1];
                    csvProcStats = parseStateList(ProcessStats.STATE_NAMES_CSV, 1, args[i], sep, error);
                    if (csvProcStats == null) {
                        pw.println("Error in \"" + args[i] + "\": " + error[0]);
                        dumpHelp(pw);
                        return;
                    }
                    csvSepProcStats = sep[0];
                } else if ("--details".equals(arg)) {
                    dumpDetails = TRUE;
                } else if ("--full-details".equals(arg)) {
                    dumpFullDetails = TRUE;
                } else if ("--hours".equals(arg)) {
                    i++;
                    if (i >= args.length) {
                        pw.println("Error: argument required for --hours");
                        dumpHelp(pw);
                        return;
                    }
                    try {
                        aggregateHours = Integer.parseInt(args[i]);
                    } catch (NumberFormatException e) {
                        pw.println("Error: --hours argument not an Int32 -- " + args[i]);
                        dumpHelp(pw);
                        return;
                    }
                } else if ("--last".equals(arg)) {
                    i++;
                    if (i >= args.length) {
                        pw.println("Error: argument required for --last");
                        dumpHelp(pw);
                        return;
                    }
                    try {
                        lastIndex = Integer.parseInt(args[i]);
                    } catch (NumberFormatException e) {
                        pw.println("Error: --last argument not an Int32 -- " + args[i]);
                        dumpHelp(pw);
                        return;
                    }
                } else if ("--active".equals(arg)) {
                    activeOnly = TRUE;
                    currentOnly = TRUE;
                } else if ("--current".equals(arg)) {
                    currentOnly = TRUE;
                } else if ("--commit".equals(arg)) {
                    synchronized (mAm) {
                        mProcessStats->mFlags |= ProcessStats.FLAG_COMPLETE;
                        writeStateLocked(TRUE, TRUE);
                        pw.println("Process stats committed.");
                    }
                    return;
                } else if ("--reset".equals(arg)) {
                    synchronized (mAm) {
                        mProcessStats->resetSafely();
                        pw.println("Process stats reset.");
                    }
                    return;
                } else if ("--clear".equals(arg)) {
                    synchronized (mAm) {
                        mProcessStats->resetSafely();
                        ArrayList<String> files = getCommittedFiles(0, TRUE, TRUE);
                        if (files != null) {
                            for (Int32 fi=0; fi<files.size(); fi++) {
                                (new File(files.get(fi))).delete();
                            }
                        }
                        pw.println("All process stats cleared.");
                    }
                    return;
                } else if ("--write".equals(arg)) {
                    synchronized (mAm) {
                        writeStateSyncLocked();
                        pw.println("Process stats written.");
                    }
                    return;
                } else if ("--read".equals(arg)) {
                    synchronized (mAm) {
                        readLocked(mProcessStats, mFile);
                        pw.println("Process stats read.");
                    }
                    return;
                } else if ("-h".equals(arg)) {
                    dumpHelp(pw);
                    return;
                } else if ("-a".equals(arg)) {
                    dumpDetails = TRUE;
                    dumpAll = TRUE;
                } else if (arg.length() > 0 && arg.charAt(0) == '-'){
                    pw.println("Unknown option: " + arg);
                    dumpHelp(pw);
                    return;
                } else {
                    // Not an option, last argument must be a package name.
                    reqPackage = arg;
                    // Include all details, since we know we are only going to
                    // be dumping a smaller set of data.  In fact only the details
                    // contain per-package data, so this is needed to be able
                    // to dump anything at all when filtering by package.
                    dumpDetails = TRUE;
                }
            }
        }

        if (isCsv) {
            pw.print("Processes running summed over");
            if (!csvSepScreenStats) {
                for (Int32 i=0; i<csvScreenStats.length; i++) {
                    pw.print(" ");
                    ProcessStats.printScreenLabelCsv(pw, csvScreenStats[i]);
                }
            }
            if (!csvSepMemStats) {
                for (Int32 i=0; i<csvMemStats.length; i++) {
                    pw.print(" ");
                    ProcessStats.printMemLabelCsv(pw, csvMemStats[i]);
                }
            }
            if (!csvSepProcStats) {
                for (Int32 i=0; i<csvProcStats.length; i++) {
                    pw.print(" ");
                    pw.print(ProcessStats.STATE_NAMES_CSV[csvProcStats[i]]);
                }
            }
            pw.println();
            synchronized (mAm) {
                dumpFilteredProcessesCsvLocked(pw, null,
                        csvSepScreenStats, csvScreenStats, csvSepMemStats, csvMemStats,
                        csvSepProcStats, csvProcStats, now, reqPackage);
                /*
                dumpFilteredProcessesCsvLocked(pw, "Processes running while critical mem:",
                        FALSE, new Int32[] {ADJ_SCREEN_OFF, ADJ_SCREEN_ON},
                        TRUE, new Int32[] {ADJ_MEM_FACTOR_CRITICAL},
                        TRUE, new Int32[] {STATE_PERSISTENT, STATE_TOP, STATE_FOREGROUND, STATE_VISIBLE,
                                STATE_PERCEPTIBLE, STATE_BACKUP, STATE_SERVICE, STATE_HOME,
                                STATE_PREVIOUS, STATE_CACHED},
                        now, reqPackage);
                dumpFilteredProcessesCsvLocked(pw, "Processes running over all mem:",
                        FALSE, new Int32[] {ADJ_SCREEN_OFF, ADJ_SCREEN_ON},
                        FALSE, new Int32[] {ADJ_MEM_FACTOR_CRITICAL, ADJ_MEM_FACTOR_LOW,
                                ADJ_MEM_FACTOR_MODERATE, ADJ_MEM_FACTOR_MODERATE},
                        TRUE, new Int32[] {STATE_PERSISTENT, STATE_TOP, STATE_FOREGROUND, STATE_VISIBLE,
                                STATE_PERCEPTIBLE, STATE_BACKUP, STATE_SERVICE, STATE_HOME,
                                STATE_PREVIOUS, STATE_CACHED},
                        now, reqPackage);
                */
            }
            return;
        } else if (aggregateHours != 0) {
            pw.print("AGGREGATED OVER LAST "); pw.print(aggregateHours); pw.println(" HOURS:");
            dumpAggregatedStats(pw, aggregateHours, now, reqPackage, isCompact,
                    dumpDetails, dumpFullDetails, dumpAll, activeOnly);
            return;
        } else if (lastIndex > 0) {
            pw.print("LAST STATS AT INDEX "); pw.print(lastIndex); pw.println(":");
            ArrayList<String> files = getCommittedFiles(0, FALSE, TRUE);
            if (lastIndex >= files.size()) {
                pw.print("Only have "); pw.print(files.size()); pw.println(" data sets");
                return;
            }
            AtomicFile file = new AtomicFile(new File(files.get(lastIndex)));
            ProcessStats processStats = new ProcessStats(FALSE);
            readLocked(processStats, file);
            if (processStats.mReadError != null) {
                if (isCheckin || isCompact) pw.print("err,");
                pw.print("Failure reading "); pw.print(files.get(lastIndex));
                pw.print("; "); pw.println(processStats.mReadError);
                return;
            }
            String fileStr = file.getBaseFile().getPath();
            Boolean checkedIn = fileStr.endsWith(STATE_FILE_CHECKIN_SUFFIX);
            if (isCheckin || isCompact) {
                // Don't really need to lock because we uniquely own this object.
                processStats.dumpCheckinLocked(pw, reqPackage);
            } else {
                pw.print("COMMITTED STATS FROM ");
                pw.print(processStats.mTimePeriodStartClockStr);
                if (checkedIn) pw.print(" (checked in)");
                pw.println(":");
                if (dumpDetails || dumpFullDetails) {
                    processStats.dumpLocked(pw, reqPackage, now, !dumpFullDetails, dumpAll,
                            activeOnly);
                    if (dumpAll) {
                        pw.print("  mFile="); pw.println(mFile.getBaseFile());
                    }
                } else {
                    processStats.dumpSummaryLocked(pw, reqPackage, now, activeOnly);
                }
            }
            return;
        }

        Boolean sepNeeded = FALSE;
        if (dumpAll || isCheckin) {
            mWriteLock.lock();
            try {
                ArrayList<String> files = getCommittedFiles(0, FALSE, !isCheckin);
                if (files != null) {
                    for (Int32 i=0; i<files.size(); i++) {
                        if (DEBUG) Slog.d(TAG, "Retrieving state: " + files.get(i));
                        try {
                            AtomicFile file = new AtomicFile(new File(files.get(i)));
                            ProcessStats processStats = new ProcessStats(FALSE);
                            readLocked(processStats, file);
                            if (processStats.mReadError != null) {
                                if (isCheckin || isCompact) pw.print("err,");
                                pw.print("Failure reading "); pw.print(files.get(i));
                                pw.print("; "); pw.println(processStats.mReadError);
                                if (DEBUG) Slog.d(TAG, "Deleting state: " + files.get(i));
                                (new File(files.get(i))).delete();
                                continue;
                            }
                            String fileStr = file.getBaseFile().getPath();
                            Boolean checkedIn = fileStr.endsWith(STATE_FILE_CHECKIN_SUFFIX);
                            if (isCheckin || isCompact) {
                                // Don't really need to lock because we uniquely own this object.
                                processStats.dumpCheckinLocked(pw, reqPackage);
                            } else {
                                if (sepNeeded) {
                                    pw.println();
                                } else {
                                    sepNeeded = TRUE;
                                }
                                pw.print("COMMITTED STATS FROM ");
                                pw.print(processStats.mTimePeriodStartClockStr);
                                if (checkedIn) pw.print(" (checked in)");
                                pw.println(":");
                                // Don't really need to lock because we uniquely own this object.
                                // Always dump summary here, dumping all details is just too
                                // much crud.
                                if (dumpFullDetails) {
                                    processStats.dumpLocked(pw, reqPackage, now, FALSE, FALSE,
                                            activeOnly);
                                } else {
                                    processStats.dumpSummaryLocked(pw, reqPackage, now, activeOnly);
                                }
                            }
                            if (isCheckin) {
                                // Rename file suffix to mark that it has checked in.
                                file.getBaseFile().renameTo(new File(
                                        fileStr + STATE_FILE_CHECKIN_SUFFIX));
                            }
                        } catch (Throwable e) {
                            pw.print("**** FAILURE DUMPING STATE: "); pw.println(files.get(i));
                            e.printStackTrace(pw);
                        }
                    }
                }
            } finally {
                mWriteLock.unlock();
            }
        }
        if (!isCheckin) {
            if (!currentOnly) {
                if (sepNeeded) {
                    pw.println();
                }
                pw.println("AGGREGATED OVER LAST 24 HOURS:");
                dumpAggregatedStats(pw, 24, now, reqPackage, isCompact,
                        dumpDetails, dumpFullDetails, dumpAll, activeOnly);
                pw.println();
                pw.println("AGGREGATED OVER LAST 3 HOURS:");
                dumpAggregatedStats(pw, 3, now, reqPackage, isCompact,
                        dumpDetails, dumpFullDetails, dumpAll, activeOnly);
                sepNeeded = TRUE;
            }
            synchronized (mAm) {
                if (isCompact) {
                    mProcessStats->dumpCheckinLocked(pw, reqPackage);
                } else {
                    if (sepNeeded) {
                        pw.println();
                    }
                    pw.println("CURRENT STATS:");
                    if (dumpDetails || dumpFullDetails) {
                        mProcessStats->dumpLocked(pw, reqPackage, now, !dumpFullDetails, dumpAll,
                                activeOnly);
                        if (dumpAll) {
                            pw.print("  mFile="); pw.println(mFile.getBaseFile());
                        }
                    } else {
                        mProcessStats->dumpSummaryLocked(pw, reqPackage, now, activeOnly);
                    }
                }
            }
        }
    }
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_CPROCESSSTATSSERVICE_H__
