
#ifndef __ELASTOS_DROID_INTERNAL_APP_PROCESSSTATS_H__
#define __ELASTOS_DROID_INTERNAL_APP_PROCESSSTATS_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>

using Elastos::Core::IInteger32;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class ProcessStats
    : public Object
    , public IProcessStats
    , public IParcelable
{
public: // not public in java
    class PssAggr : public Object
    {
    public:
        PssAggr();

        void Add(
            /* [in] */ Int64 newPss,
            /* [in] */ Int64 newSamples);

    public:
        Int64 mPss;
        Int64 mSamples;
    };

public:
    ProcessStats();

    CAR_INTERFACE_DECL()

    CARAPI constructor()
    {
        Reset()
        return NOERROR;
    }

    CARAPI constructor(
        /* [in] */ Boolean running)
    {
        mRunning = running;
        Reset();
        return NOERROR;
    }

    CARAPI Add(
        /* [in] */ IProcessStats* _other)
    {
        ProcessStats* other = (ProcessStats*)_other;
        AutoPtr<IArrayMap> pkgMap;
        other->mPackages->GetMap((IArrayMap**)&pkgMap);
        Int32 size;
        pkgMap->GetSize(&size);
        for (Int32 ip = 0; ip < size; ip++) {
            AutoPtr<IInterface> key;
            pkgMap->GetKeyAt(ip, (IInterface**)&key);
            String pkgName;
            ICharSequence::Probe(pkgMap)->ToString(&pkgName);
            AutoPtr<IInterface> value;
            pkgMap->GetValueAt(ip, (IInterface**)&value);
            AutoPtr<ISparseArray> uids = ISparseArray::Probe(value);
            Int32 uSize;
            uids->GetSize(&uSize);
            for (Int32 iu = 0; iu < uSize; iu++) {
                Int32 uid;
                uids->KeyAt(iu, &uid);
                AutoPtr<IInterface> uValue;
                uids->ValueAt(iu, (IInterface**)&uValue);
                AutoPtr<ISparseArray> versions = ISparseArray::Probe(uValue);
                Int32 vSize;
                versions->GetSize(&vSize);
                for (Int32 iv = 0; iv < vSize; iv++) {
                    Int32 vers;
                    versions->KeyAt(iv, &vers);
                    AutoPtr<IInterface> vValue;
                    versions->ValueAt(iv, (IInterface**)&vValue);
                    PackageState* otherState = (PackageState*)IPackageState::Probe(vValue);
                    Int32 NPROCS;
                    otherState->mProcesses->GetSize(&NPROCS);
                    Int32 NSRVS;
                    otherState->mServices->GetSize(&NSRVS);
                    for (Int32 iproc = 0; iproc < NPROCS; iproc++) {
                        AutoPtr<IInterface> procValue;
                        otherState->mProcesses->GetValueAt(iproc, (IInterface**)&procValue);
                        ProcessState* otherProc = (ProcessState*)IProcessState::Probe(procValue);
                        if (otherProc->mCommonProcess != otherProc) {
                            if (DEBUG) Slogger::D(TAG, "Adding pkg %s uid %d vers %d proc %s",
                                pkgName.string(), uid, vers, otherProc->mName.string());
                            AutoPtr<IProcessState> _thisProc;
                            GetProcessStateLocked(pkgName, uid, vers,
                                otherProc->mName, (IProcessState**)&_thisProc);
                            AutoPtr<ProcessState> thisProc = (ProcessState*)_thisProc->Get();
                            if (thisProc->mCommonProcess == thisProc) {
                                if (DEBUG) Slogger::D(TAG, "Existing process is single-package, splitting");
                                thisProc->mMultiPackage = TRUE;
                                Int64 now = SystemClock::GetUptimeMillis();
                                AutoPtr<IPackageState> pkgState;
                                GetPackageStateLocked(pkgName, uid, vers, (IPackageState**)&_pkgState);
                                thisProc = thisProc->Clone(thisProc->mPackage, now);
                                ((PackageState*)pkgState.Get())->mProcesses->Put(
                                    CoreUtils::Convert(thisProc->mName), (IProcessState*)thisProc);
                            }
                            thisProc->Add(otherProc);
                        }
                    }
                    for (Int32 isvc = 0; isvc < NSRVS; isvc++) {
                        AutoPtr<IInterface> svcValue;
                        otherState->mServices->GetValueAt(isvc, (IInterface**)&svcValue);
                        ServiceState* otherSvc = (ServiceState*)IServiceState::Probe(svcValue);
                        if (DEBUG) Slogger::D(TAG, "Adding pkg %s uid %d service %s",
                            pkgName.string(), uid, otherSvc->mName.string());
                        AutoPtr<IServiceState> thisSvc;
                        GetServiceStateLocked(pkgName, uid, vers, otherSvc->mProcessName,
                            otherSvc->mName, (IServiceState**)&thisSvc);
                        ((ServiceState*)thisSvc.Get())->Add(otherSvc);
                    }
                }
            }
        }

        AutoPtr<IArrayMap> procMap;
        other->mProcesses->GetMap((IArrayMap**)&procMap);
        procMap->GetSize(&size);
        for (Int32 ip = 0; ip < size; ip++) {
            AutoPtr<IInterface> value;
            procMap->GetValueAt(ip, (IInterface**)&value);
            AutoPtr<ISparseArray> uids = ISparseArray::Probe(value);
            Int32 uSize;
            uids->GetSize(&uSize);
            for (Int32 iu = 0; iu < uSize; iu++) {
                Int32 uid;
                uids->KeyAt(iu, &uid);
                AutoPtr<IInterface> uValue;
                uids->ValueAt(iu, (IInterface**)&uValue);
                ProcessState* otherProc = (ProcessState*)IProcessState::Probe(uValue);
                AutoPtr<IInterface> _thisProc;
                mProcesses->Get(otherProc->mName, uid, (IInterface**)&_thisProc);
                AutoPtr<ProcessState> thisProc = (ProcessState*)IProcessState::Probe(_thisProc);
                if (DEBUG) Slogger::D(TAG, "Adding uid %d proc %s", uid, otherProc->mName.string());
                if (thisProc == NULL) {
                    if (DEBUG) Slogger::D(TAG, "Creating new process!");
                    thisProc = new ProcessState();
                    thisProc->constructor(this, otherProc->mPackage, uid, otherProc->mVersion,
                            otherProc->mName);
                    mProcesses->Put(otherProc->mName, uid, (IProcessState*)thisProc);
                    AutoPtr<IPackageState> _thisState;
                    GetPackageStateLocked(otherProc->mPackage, uid,
                        otherProc->mVersion, (IPackageState**)&_thisState);
                    PackageState* thisState = (PackageState*)_thisState.Get();
                    AutoPtr<ICharSequence> key = CoreUtils::Convert(otherProc->mName);
                    Boolean res;
                    thisState->mProcesses->ContainsKey(key, &res);
                    if (!res) {
                        thisState->mProcesses->Put(key, thisProc);
                    }
                }
                thisProc->Add(otherProc);
            }
        }

        for (Int32 i = 0; i < ADJ_COUNT; i++) {
            if (DEBUG) Slogger::D(TAG, "Total duration #%d inc by %lld from %lld"
                    i, (*other->mMemFactorDurations)[i], (*mMemFactorDurations)[i]);
            (*mMemFactorDurations)[i] += (*other->mMemFactorDurations)[i];
        }

        for (Int32 i = 0; i < other->mSysMemUsageTableSize; i++) {
            Int32 ent = (*other->mSysMemUsageTable)[i];
            Int32 state = (ent >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
            AutoPtr<ArrayOf<Int64> > longs = other->mLongs[(ent >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
            AddSysMemUsage(state, *longs, ((ent >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK));
        }

        if (other->mTimePeriodStartClock < mTimePeriodStartClock) {
            mTimePeriodStartClock = other->mTimePeriodStartClock;
            mTimePeriodStartClockStr = other->mTimePeriodStartClockStr;
        }
        mTimePeriodEndRealtime += other->mTimePeriodEndRealtime - other->mTimePeriodStartRealtime;
        mTimePeriodEndUptime += other->mTimePeriodEndUptime - other->mTimePeriodStartUptime;

        return NOERROR;
    }

    CARAPI AddSysMemUsage(
        /* [in] */ Int64 cachedMem,
        /* [in] */ Int64 freeMem,
        /* [in] */ Int64 zramMem,
        /* [in] */ Int64 kernelMem,
        /* [in] */ Int64 nativeMem)
    {
        if (mMemFactor != STATE_NOTHING) {
            Int32 state = mMemFactor * STATE_COUNT;
            (*mSysMemUsageArgs)[SYS_MEM_USAGE_SAMPLE_COUNT] = 1;
            for (Int32 i = 0; i < 3; i++) {
                (*mSysMemUsageArgs)[SYS_MEM_USAGE_CACHED_MINIMUM + i] = cachedMem;
                (*mSysMemUsageArgs)[SYS_MEM_USAGE_FREE_MINIMUM + i] = freeMem;
                (*mSysMemUsageArgs)[SYS_MEM_USAGE_ZRAM_MINIMUM + i] = zramMem;
                (*mSysMemUsageArgs)[SYS_MEM_USAGE_KERNEL_MINIMUM + i] = kernelMem;
                (*mSysMemUsageArgs)[SYS_MEM_USAGE_NATIVE_MINIMUM + i] = nativeMem;
            }
            AddSysMemUsage(state, *mSysMemUsageArgs, 0);
        }
        return NOERROR;
    }

    void AddSysMemUsage(
        /* [in] */ Int32 state,
        /* [in] */ const ArrayOf<Int64>& data,
        /* [in] */ Int32 dataOff)
    {
        Int32 idx = BinarySearch(*mSysMemUsageTable, mSysMemUsageTableSize, state);
        Int32 off;
        if (idx >= 0) {
            off = (*mSysMemUsageTable)[idx];
        }
        else {
            mAddLongTable = mSysMemUsageTable;
            mAddLongTableSize = mSysMemUsageTableSize;
            off = AddLongData(~idx, state, SYS_MEM_USAGE_COUNT);
            mSysMemUsageTable = mAddLongTable;
            mSysMemUsageTableSize = mAddLongTableSize;
        }
        AutoPtr<ArrayOf<Int64> > longs = mLongs[(off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
        idx = (off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK;
        AddSysMemUsage(*longs, idx, data, dataOff);
    }

    static void AddSysMemUsage(
        /* [in] */ const ArrayOf<Int64>& dstData,
        /* [in] */ Int32 dstOff,
        /* [in] */ const ArrayOf<Int64>& addData,
        /* [in] */ Int32 addOff)
    {
        Int64 dstCount = dstData[dstOff+SYS_MEM_USAGE_SAMPLE_COUNT];
        Int64 addCount = addData[addOff+SYS_MEM_USAGE_SAMPLE_COUNT];
        if (dstCount == 0) {
            dstData[dstOff+SYS_MEM_USAGE_SAMPLE_COUNT] = addCount;
            for (Int32 i = SYS_MEM_USAGE_CACHED_MINIMUM; i < SYS_MEM_USAGE_COUNT; i++) {
                dstData[dstOff+i] = addData[addOff+i];
            }
        }
        else if (addCount > 0) {
            dstData[dstOff+SYS_MEM_USAGE_SAMPLE_COUNT] = dstCount + addCount;
            for (Int32 i = SYS_MEM_USAGE_CACHED_MINIMUM; i < SYS_MEM_USAGE_COUNT; i+=3) {
                if (dstData[dstOff+i] > addData[addOff+i]) {
                    dstData[dstOff+i] = addData[addOff+i];
                }
                dstData[dstOff+i+1] = (Int64)(
                    ((dstData[dstOff+i+1]*(Double)dstCount)
                        + (addData[addOff+i+1]*(Double)addCount))
                        / (dstCount+addCount) );
                if (dstData[dstOff+i+2] < addData[addOff+i+2]) {
                    dstData[dstOff+i+2] = addData[addOff+i+2];
                }
            }
        }
    }

    private static void PrintScreenLabel(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 offset)
    {
        switch (offset) {
            case ADJ_NOTHING:
                pw->Print(String("     "));
                break;
            case ADJ_SCREEN_OFF:
                pw->Print(String("SOff/"));
                break;
            case ADJ_SCREEN_ON:
                pw->Print(String("SOn /"));
                break;
            default:
                pw->Print(String("????/"));
                break;
        }
    }

    static void PrintScreenLabelCsv(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 offset)
    {
        switch (offset) {
            case ADJ_NOTHING:
                break;
            case ADJ_SCREEN_OFF:
                pw->Print((*ADJ_SCREEN_NAMES_CSV)[0]);
                break;
            case ADJ_SCREEN_ON:
                pw->Print((*ADJ_SCREEN_NAMES_CSV)[1]);
                break;
            default:
                pw->Print(String("???"));
                break;
        }
    }

    private static void PrintMemLabel(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 offset,
        /* [in] */ Char32 sep)
    {
        switch (offset) {
            case ADJ_NOTHING:
                pw->Print(String("    "));
                if (sep != 0) pw->PrintChar(' ');
                break;
            case ADJ_MEM_FACTOR_NORMAL:
                pw->Print(String("Norm"));
                if (sep != 0) pw->Print(sep);
                break;
            case ADJ_MEM_FACTOR_MODERATE:
                pw->Print(String("Mod "));
                if (sep != 0) pw->Print(sep);
                break;
            case ADJ_MEM_FACTOR_LOW:
                pw->Print(String("Low "));
                if (sep != 0) pw->Print(sep);
                break;
            case ADJ_MEM_FACTOR_CRITICAL:
                pw->Print(String("Crit"));
                if (sep != 0) pw->Print(sep);
                break;
            default:
                pw->Print(String("????"));
                if (sep != 0) pw->Print(sep);
                break;
        }
    }

    static void PrintMemLabelCsv(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 offset)
    {
        if (offset >= ADJ_MEM_FACTOR_NORMAL) {
            if (offset <= ADJ_MEM_FACTOR_CRITICAL) {
                pw->Print((*ADJ_MEM_NAMES_CSV)[offset]);
            }
            else {
                pw->Print(String("???"));
            }
        }
    }

    static Int64 DumpSingleTime(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ const ArrayOf<Int64>& durations,
        /* [in] */ Int32 curState,
        /* [in] */ Int64 curStartTime,
        /* [in] */ Int64 now)
    {
        Int64 totalTime = 0;
        Int32 printedScreen = -1;
        for (Int32 iscreen = 0; iscreen < ADJ_COUNT; iscreen+=ADJ_SCREEN_MOD) {
            Int32 printedMem = -1;
            for (Int32 imem = 0; imem < ADJ_MEM_FACTOR_COUNT; imem++) {
                Int32 state = imem + iscreen;
                Int64 time = durations[state];
                String running("");
                if (curState == state) {
                    time += now - curStartTime;
                    if (pw != NULL) {
                        running = " (running)";
                    }
                }
                if (time != 0) {
                    if (pw != NULL) {
                        pw->Print(prefix);
                        PrintScreenLabel(pw, printedScreen != iscreen
                                ? iscreen : STATE_NOTHING);
                        printedScreen = iscreen;
                        PrintMemLabel(pw, printedMem != imem ? imem : STATE_NOTHING, (Char32)0);
                        printedMem = imem;
                        pw->Print(String(": "));
                        TimeUtils::FormatDuration(time, pw);
                        pw->Println(running);
                    }
                    totalTime += time;
                }
            }
        }
        if (totalTime != 0 && pw != NULL) {
            pw->Print(prefix);
            pw->Print(String("    TOTAL: "));
            TimeUtils::FormatDuration(totalTime, pw);
            pw->Println();
        }
        return totalTime;
    }

    static void DumpAdjTimesCheckin(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& sep,
        /* [in] */ const ArrayOf<Int64>& durations,
        /* [in] */ Int32 curState,
        /* [in] */ Int64 curStartTime,
        /* [in] */ Int64 now)
    {
        for (Int32 iscreen = 0; iscreen < ADJ_COUNT; iscreen += ADJ_SCREEN_MOD) {
            for (Int32 imem = 0; imem < ADJ_MEM_FACTOR_COUNT; imem++) {
                Int32 state = imem+iscreen;
                Int64 time = durations[state];
                if (curState == state) {
                    time += now - curStartTime;
                }
                if (time != 0) {
                    PrintAdjTagAndValue(pw, state, time);
                }
            }
        }
    }

    static void DumpServiceTimeCheckin(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& label,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 vers,
        /* [in] */ const String& serviceName,
        /* [in] */ ServiceState* svc,
        /* [in] */ Int32 serviceType,
        /* [in] */ Int32 opCount,
        /* [in] */ Int32 curState,
        /* [in] */ Int64 curStartTime,
        /* [in] */ Int64 now)
    {
        if (opCount <= 0) {
            return;
        }
        pw->Print(label);
        pw->Print(String(","));
        pw->Print(packageName);
        pw->Print(String(","));
        pw->Print(uid);
        pw->Print(String(","));
        pw->Print(vers);
        pw->Print(String(","));
        pw->Print(serviceName);
        pw->Print(String(","));
        pw->Print(opCount);
        Boolean didCurState = FALSE;
        for (Int32 i = 0; i < svc->mDurationsTableSize; i++) {
            Int32 off = (*svc->mDurationsTable)[i];
            Int32 type = (off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
            Int32 memFactor = type / ServiceState::SERVICE_COUNT;
            type %= ServiceState::SERVICE_COUNT;
            if (type != serviceType) {
                continue;
            }
            Int64 time = svc->mStats->GetInt64(off, 0);
            if (curState == memFactor) {
                didCurState = TRUE;
                time += now - curStartTime;
            }
            PrintAdjTagAndValue(pw, memFactor, time);
        }
        if (!didCurState && curState != STATE_NOTHING) {
            PrintAdjTagAndValue(pw, curState, now - curStartTime);
        }
        pw->Println();
    }

    static void ComputeProcessData(
        /* [in] */ IProcessState* _proc,
        /* [in] */ IProcessDataCollection* _data,
        /* [in] */ Int64 now)
    {
        ProcessState* proc = (ProcessState*)_proc;
        ProcessDataCollection* data = (ProcessDataCollection*)_data;
        data->mTotalTime = 0;
        data->mNumPss = data->mMinPss = data->mAvgPss = data->mMaxPss =
                data->mMinUss = data->mAvgUss = data->mMaxUss = 0;
        for (Int32 is = 0; is < data->mScreenStates->GetLength(); is++) {
            for (Int32 im = 0; im < data->mMemStates->GetLength(); im++) {
                for (Int32 ip = 0; ip < data->mProcStates->GetLength(); ip++) {
                    Int32 bucket = (((*data->mScreenStates)[is] + (*data->mMemStates)[im]) * STATE_COUNT)
                            + (*data->mProcStates)[ip];
                    data->mTotalTime += proc->GetDuration(bucket, now);
                    Int64 samples = proc->GetPssSampleCount(bucket);
                    if (samples > 0) {
                        Int64 minPss = proc->GetPssMinimum(bucket);
                        Int64 avgPss = proc->GetPssAverage(bucket);
                        Int64 maxPss = proc->GetPssMaximum(bucket);
                        Int64 minUss = proc->GetPssUssMinimum(bucket);
                        Int64 avgUss = proc->GetPssUssAverage(bucket);
                        Int64 maxUss = proc->GetPssUssMaximum(bucket);
                        if (data->mNumPss == 0) {
                            data->mMinPss = minPss;
                            data->mAvgPss = avgPss;
                            data->mMaxPss = maxPss;
                            data->mMinUss = minUss;
                            data->mAvgUss = avgUss;
                            data->mMaxUss = maxUss;
                        }
                        else {
                            if (minPss < data->mMinPss) {
                                data->mMinPss = minPss;
                            }
                            data->mAvgPss = (Int64)( ((data->mAvgPss*(Double)data->mNumPss)
                                    + (avgPss*(Double)samples)) / (data->mNumPss+samples) );
                            if (maxPss > data->mMaxPss) {
                                data->mMaxPss = maxPss;
                            }
                            if (minUss < data->mMinUss) {
                                data->mMinUss = minUss;
                            }
                            data->mAvgUss = (Int64)( ((data->mAvgUss*(Double)data->mNumPss)
                                    + (avgUss*(Double)samples)) / (data->mNumPss+samples) );
                            if (maxUss > data->mMaxUss) {
                                data->mMaxUss = maxUss;
                            }
                        }
                        data->mNumPss += samples;
                    }
                }
            }
        }
    }

    static Int64 ComputeProcessTimeLocked(
        /* [in] */ IProcessState* _proc,
        /* [in] */ const ArrayOf<Int32>& screenStates,
        /* [in] */ const ArrayOf<Int32>& memStates,
        /* [in] */ const ArrayOf<Int32>& procStates,
        /* [in] */ Int64 now)
    {
        ProcessState* proc = (ProcessState*)_proc;
        Int64 totalTime = 0;
        /*
        for (Int32 i = 0; i < proc->mDurationsTableSize; i++) {
            Int32 val = (*proc->mDurationsTable)[i];
            totalTime += proc->mState.GetInt64(val, 0);
            if ((val&0xff) == proc->mCurState) {
                totalTime += now - proc->mStartTime;
            }
        }
        */
        for (Int32 is = 0; is < screenStates.GetLength(); is++) {
            for (Int32 im = 0; im < memStates.GetLength(); im++) {
                for (Int32 ip = 0; ip < procStates.GetLength(); ip++) {
                    Int32 bucket = ((screenStates[is] + memStates[im]) * STATE_COUNT)
                            + procStates[ip];
                    totalTime += proc->GetDuration(bucket, now);
                }
            }
        }
        proc->mTmpTotalTime = totalTime;
        return totalTime;
    }

    CARAPI ComputeTotalMemoryUse(
        /* [in] */ ITotalMemoryUseCollection* _data,
        /* [in] */ Int64 now)
    {
        TotalMemoryUseCollection* data = (TotalMemoryUseCollection*)_data;
        data->mTotalTime = 0;
        for (Int32 i = 0; i < STATE_COUNT; i++) {
            (*data->mProcessStateWeight)[i] = 0;
            (*data->mProcessStatePss)[i] = 0;
            (*data->mProcessStateTime)[i] = 0;
            (*data->mProcessStateSamples)[i] = 0;
        }
        for (Int32 i = 0; i < SYS_MEM_USAGE_COUNT; i++) {
            (*data->mSysMemUsage)[i] = 0;
        }
        data->mSysMemCachedWeight = 0;
        data->mSysMemFreeWeight = 0;
        data->mSysMemZRamWeight = 0;
        data->mSysMemKernelWeight = 0;
        data->mSysMemNativeWeight = 0;
        data->mSysMemSamples = 0;
        AutoPtr<ArrayOf<Int64> > totalMemUsage = ArrayOf<Int64>::Alloc(SYS_MEM_USAGE_COUNT);
        for (Int32 i = 0; i < mSysMemUsageTableSize; i++) {
            Int32 ent = (*mSysMemUsageTable)[i];
            AutoPtr<ArrayOf<Int64> > longs = mLongs[(ent >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
            Int32 idx = (ent >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK;
            AddSysMemUsage(*totalMemUsage, 0, *longs, idx);
        }
        for (Int32 is = 0; is < data->mScreenStates->GetLength(); is++) {
            for (Int32 im = 0; im < data->mMemStates->GetLength(); im++) {
                Int32 memBucket = (*data->mScreenStates)[is] + (*data->mMemStates)[im];
                Int32 stateBucket = memBucket * STATE_COUNT;
                Int64 memTime = (*mMemFactorDurations)[memBucket];
                if (mMemFactor == memBucket) {
                    memTime += now - mStartTime;
                }
                data->mTotalTime += memTime;
                Int32 sysIdx = BinarySearch(mSysMemUsageTable, mSysMemUsageTableSize, stateBucket);
                AutoPtr<ArrayOf<Int64> > longs = totalMemUsage;
                Int32 idx = 0;
                if (sysIdx >= 0) {
                    Int32 ent = (*mSysMemUsageTable)[sysIdx];
                    AutoPtr<ArrayOf<Int64> > tmpLongs = mLongs[(ent >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
                    Int32 tmpIdx = (ent >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK;
                    if (tmpLongs[tmpIdx+SYS_MEM_USAGE_SAMPLE_COUNT] >= 3) {
                        AddSysMemUsage(*data->mSysMemUsage, 0, *longs, idx);
                        longs = tmpLongs;
                        idx = tmpIdx;
                    }
                }
                data->mSysMemCachedWeight += (*longs)[idx+SYS_MEM_USAGE_CACHED_AVERAGE]
                        * (Double)memTime;
                data->mSysMemFreeWeight += (*longs)[idx+SYS_MEM_USAGE_FREE_AVERAGE]
                        * (Double)memTime;
                data->mSysMemZRamWeight += (*longs)[idx+SYS_MEM_USAGE_ZRAM_AVERAGE]
                        * (Double)memTime;
                data->mSysMemKernelWeight += (*longs)[idx+SYS_MEM_USAGE_KERNEL_AVERAGE]
                        * (Double)memTime;
                data->mSysMemNativeWeight += (*longs)[idx+SYS_MEM_USAGE_NATIVE_AVERAGE]
                        * (Double)memTime;
                data->mSysMemSamples += (*longs)[idx+SYS_MEM_USAGE_SAMPLE_COUNT];
             }
        }

        AutoPtr<IArrayMap> procMap;
        mProcesses->GetMap((IArrayMap**)&procMap);
        procMap->GetSize(&size);
        for (Int32 iproc = 0; iproc < size; iproc++) {
            AutoPtr<IInterface> value;
            procMap->GetValueAt(iproc, (IInterface**)&value);
            AutoPtr<ISparseArray> uids = ISparseArray::Probe(value);
            Int32 uSize;
            uids->GetSize(&uSize);
            for (Int32 iu = 0; iu < uSize; iu++) {
                AutoPtr<IInterface> uValue;
                uids->ValueAt(iu, (IInterface**)&uValue);
                ProcessState* proc = (ProcessState*)IProcessState::Probe(uValue);
                AutoPtr<PssAggr> fgPss = new PssAggr();
                AutoPtr<PssAggr> bgPss = new PssAggr();
                AutoPtr<PssAggr> cachedPss = new PssAggr();
                Boolean havePss = FALSE;
                for (Int32 i = 0; i < proc->mDurationsTableSize; i++) {
                    Int32 off = (*proc->mDurationsTable)[i];
                    Int32 type = (off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                    Int32 procState = type % STATE_COUNT;
                    Int64 samples = proc->GetPssSampleCount(type);
                    if (samples > 0) {
                        Int64 avg = proc->GetPssAverage(type);
                        havePss = TRUE;
                        if (procState <= STATE_IMPORTANT_FOREGROUND) {
                            fgPss->Add(avg, samples);
                        }
                        else if (procState <= STATE_RECEIVER) {
                            bgPss->Add(avg, samples);
                        }
                        else {
                            cachedPss->Add(avg, samples);
                        }
                    }
                }
                if (!havePss) {
                    continue;
                }
                Boolean fgHasBg = FALSE;
                Boolean fgHasCached = FALSE;
                Boolean bgHasCached = FALSE;
                if (fgPss->mSamples < 3 && bgPss->mSamples > 0) {
                    fgHasBg = TRUE;
                    fgPss->Add(bgPss->mPss, bgPss->mSamples);
                }
                if (fgPss->mSamples < 3 && cachedPss->mSamples > 0) {
                    fgHasCached = TRUE;
                    fgPss->Add(cachedPss->mPss, cachedPss->mSamples);
                }
                if (bgPss->mSamples < 3 && cachedPss->mSamples > 0) {
                    bgHasCached = TRUE;
                    bgPss->Add(cachedPss->mPss, cachedPss->mSamples);
                }
                if (bgPss->mSamples < 3 && !fgHasBg && fgPss->mSamples > 0) {
                    bgPss->Add(fgPss->mPss, fgPss->mSamples);
                }
                if (cachedPss->mSamples < 3 && !bgHasCached && bgPss->mSamples > 0) {
                    cachedPss->Add(bgPss->mPss, bgPss->mSamples);
                }
                if (cachedPss->mSamples < 3 && !fgHasCached && fgPss->mSamples > 0) {
                    cachedPss->Add(fgPss->mPss, fgPss->mSamples);
                }
                for (Int32 i = 0; i < proc->mDurationsTableSize; i++) {
                    final Int32 off = (*proc->mDurationsTable)[i];
                    final Int32 type = (off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                    Int64 time = GetInt64(off, 0);
                    if (proc->mCurState == type) {
                        time += now - proc->mStartTime;
                    }
                    final Int32 procState = type % STATE_COUNT;
                    (*data->mProcessStateTime)[procState] += time;
                    Int64 samples = proc->GetPssSampleCount(type);
                    Int64 avg;
                    if (samples > 0) {
                        avg = proc->GetPssAverage(type);
                    }
                    else if (procState <= STATE_IMPORTANT_FOREGROUND) {
                        samples = fgPss->mSamples;
                        avg = fgPss->mPss;
                    }
                    else if (procState <= STATE_RECEIVER) {
                        samples = bgPss->mSamples;
                        avg = bgPss->mPss;
                    }
                    else {
                        samples = cachedPss->mSamples;
                        avg = cachedPss->mPss;
                    }
                    Double newAvg = ( (data->mProcessStatePss[procState]
                            * (Double)data->mProcessStateSamples[procState])
                                + (avg*(Double)samples)
                            ) / (data->mProcessStateSamples[procState]+samples);
                    data->mProcessStatePss[procState] = (Int64)newAvg;
                    data->mProcessStateSamples[procState] += samples;
                    data->mProcessStateWeight[procState] += avg * (Double)time;
                }
            }
        }
    }

    static void DumpProcessState(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ IProcessState* _proc,
        /* [in] */ const ArrayOf<Int32>& screenStates,
        /* [in] */ const ArrayOf<Int32>& memStates,
        /* [in] */ const ArrayOf<Int32>& procStates,
        /* [in] */ Int64 now)
    {
        ProcessState* proc = (ProcessState*)_proc;
        Int64 totalTime = 0;
        Int32 printedScreen = -1;
        for (Int32 is = 0; is < screenStates.GetLength(); is++) {
            Int32 printedMem = -1;
            for (Int32 im = 0; im < memStates.GetLength(); im++) {
                for (Int32 ip = 0; ip < procStates.GetLength(); ip++) {
                    Int32 iscreen = screenStates[is];
                    Int32 imem = memStates[im];
                    Int32 bucket = ((iscreen + imem) * STATE_COUNT) + procStates[ip];
                    Int64 time = proc->GetDuration(bucket, now);
                    String running("");
                    if (proc->mCurState == bucket) {
                        running = " (running)";
                    }
                    if (time != 0) {
                        pw->Print(prefix);
                        if (screenStates.GetLength() > 1) {
                            PrintScreenLabel(pw, printedScreen != iscreen
                                    ? iscreen : STATE_NOTHING);
                            printedScreen = iscreen;
                        }
                        if (memStates.GetLength() > 1) {
                            PrintMemLabel(pw, printedMem != imem ? imem : STATE_NOTHING, '/');
                            printedMem = imem;
                        }
                        pw->Print((*STATE_NAMES)[procStates[ip]]);
                        pw->Print(String(": "));
                        TimeUtils::FormatDuration(time, pw);
                        pw->Println(running);
                        totalTime += time;
                    }
                }
            }
        }
        if (totalTime != 0) {
            pw->Print(prefix);
            if (screenStates.GetLength() > 1) {
                PrintScreenLabel(pw, STATE_NOTHING);
            }
            if (memStates.GetLength() > 1) {
                PrintMemLabel(pw, STATE_NOTHING, '/');
            }
            pw->Print(String("TOTAL  : "));
            TimeUtils::FormatDuration(totalTime, pw);
            pw->Println();
        }
    }

    static void DumpProcessPss(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ IProcessState* _proc,
        /* [in] */ const ArrayOf<Int32>& screenStates,
        /* [in] */ const ArrayOf<Int32>& memStates,
        /* [in] */ const ArrayOf<Int32>& procStates)
    {
        ProcessState* proc = (ProcessState*)_proc;
        Boolean printedHeader = FALSE;
        Int32 printedScreen = -1;
        for (Int32 is = 0; is < screenStates.GetLength(); is++) {
            Int32 printedMem = -1;
            for (Int32 im = 0; im < memStates.GetLength(); im++) {
                for (Int32 ip = 0; ip < procStates.GetLength(); ip++) {
                    Int32 iscreen = screenStates[is];
                    Int32 imem = memStates[im];
                    Int32 bucket = ((iscreen + imem) * STATE_COUNT) + procStates[ip];
                    Int64 count = proc->GetPssSampleCount(bucket);
                    if (count > 0) {
                        if (!printedHeader) {
                            pw->Print(prefix);
                            pw->Print(String("PSS/USS ("));
                            pw->Print(proc->mPssTableSize);
                            pw->Println(String(" entries):"));
                            printedHeader = TRUE;
                        }
                        pw->Print(prefix);
                        pw->Print(String("  "));
                        if (screenStates.GetLength() > 1) {
                            PrintScreenLabel(pw, printedScreen != iscreen
                                    ? iscreen : STATE_NOTHING);
                            printedScreen = iscreen;
                        }
                        if (memStates.GetLength() > 1) {
                            PrintMemLabel(pw, printedMem != imem ? imem : STATE_NOTHING, '/');
                            printedMem = imem;
                        }
                        pw->Print((*STATE_NAMES)[procStates[ip]]);
                        pw->Print(String(": "));
                        pw->Print(count);
                        pw->Print(String(" samples "));
                        PrintSizeValue(pw, proc->GetPssMinimum(bucket) * 1024);
                        pw->Print(String(" "));
                        PrintSizeValue(pw, proc->GetPssAverage(bucket) * 1024);
                        pw->Print(String(" "));
                        PrintSizeValue(pw, proc->GetPssMaximum(bucket) * 1024);
                        pw->Print(String(" / "));
                        PrintSizeValue(pw, proc->GetPssUssMinimum(bucket) * 1024);
                        pw->Print(String(" "));
                        PrintSizeValue(pw, proc->GetPssUssAverage(bucket) * 1024);
                        pw->Print(String(" "));
                        PrintSizeValue(pw, proc->GetPssUssMaximum(bucket) * 1024);
                        pw->Println();
                    }
                }
            }
        }
        if (proc->mNumExcessiveWake != 0) {
            pw->Print(prefix);
            pw->Print(String("Killed for excessive wake locks: "));
            pw->Print(proc->mNumExcessiveWake);
            pw->Println(String(" times"));
        }
        if (proc->mNumExcessiveCpu != 0) {
            pw->Print(prefix);
            pw->Print(String("Killed for excessive CPU use: "));
            pw->Print(proc->mNumExcessiveCpu);
            pw->Println(String(" times"));
        }
        if (proc->mNumCachedKill != 0) {
            pw->Print(prefix);
            pw->Print(String("Killed from cached state: "));
            pw->Print(proc->mNumCachedKill);
            pw->Print(String(" times from pss "));
            PrintSizeValue(pw, proc->mMinCachedKillPss * 1024);
            pw->Print(String("-"));
            PrintSizeValue(pw, proc->mAvgCachedKillPss * 1024);
            pw->Print(String("-"));
            PrintSizeValue(pw, proc->mMaxCachedKillPss * 1024);
            pw->Println();
        }
    }

    Int64 GetSysMemUsageValue(
        /* [in] */ Int32 state,
        /* [in] */ Int32 index)
    {
        Int32 idx = BinarySearch(*mSysMemUsageTable, mSysMemUsageTableSize, state);
        return idx >= 0 ? GetInt64((*mSysMemUsageTable)[idx], index) : 0;
    }

    void DumpSysMemUsageCategory(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ const String& label,
        /* [in] */ Int32 bucket,
        /* [in] */ Int32 index)
    {
        pw->Print(prefix);
        pw->Print(label);
        pw->Print(String(": "));
        PrintSizeValue(pw, GetSysMemUsageValue(bucket, index) * 1024);
        pw->Print(String(" min, "));
        PrintSizeValue(pw, GetSysMemUsageValue(bucket, index + 1) * 1024);
        pw->Print(String(" avg, "));
        PrintSizeValue(pw, GetSysMemUsageValue(bucket, index+2) * 1024);
        pw->Println(String(" max"));
    }

    void DumpSysMemUsage(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ const ArrayOf<Int32>& screenStates,
        /* [in] */ const ArrayOf<Int32>& memStates)
    {
        Int32 printedScreen = -1;
        for (Int32 is = 0; is < screenStates.GetLength(); is++) {
            Int32 printedMem = -1;
            for (Int32 im = 0; im < memStates.GetLength(); im++) {
                Int32 iscreen = screenStates[is];
                Int32 imem = memStates[im];
                Int32 bucket = ((iscreen + imem) * STATE_COUNT);
                Int64 count = GetSysMemUsageValue(bucket, SYS_MEM_USAGE_SAMPLE_COUNT);
                if (count > 0) {
                    pw->Print(prefix);
                    if (screenStates.GetLength() > 1) {
                        PrintScreenLabel(pw, printedScreen != iscreen
                                ? iscreen : STATE_NOTHING);
                        printedScreen = iscreen;
                    }
                    if (memStates.GetLength() > 1) {
                        PrintMemLabel(pw, printedMem != imem ? imem : STATE_NOTHING, '\0');
                        printedMem = imem;
                    }
                    pw->Print(String(": "));
                    pw->Print(count);
                    pw->Println(String(" samples:"));
                    DumpSysMemUsageCategory(pw, prefix, String("  Cached"), bucket,
                            SYS_MEM_USAGE_CACHED_MINIMUM);
                    DumpSysMemUsageCategory(pw, prefix, String("  Free"), bucket,
                            SYS_MEM_USAGE_FREE_MINIMUM);
                    DumpSysMemUsageCategory(pw, prefix, String("  ZRam"), bucket,
                            SYS_MEM_USAGE_ZRAM_MINIMUM);
                    DumpSysMemUsageCategory(pw, prefix, String("  Kernel"), bucket,
                            SYS_MEM_USAGE_KERNEL_MINIMUM);
                    DumpSysMemUsageCategory(pw, prefix, String("  Native"), bucket,
                            SYS_MEM_USAGE_NATIVE_MINIMUM);
                }
            }
        }
    }

    static void DumpStateHeadersCsv(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& sep,
        /* [in] */ ArrayOf<Int32>* screenStates,
        /* [in] */ ArrayOf<Int32>* memStates,
        /* [in] */ ArrayOf<Int32>* procStates)
    {
        const Int32 NS = screenStates != NULL ? screenStates->GetLength() : 1;
        const Int32 NM = memStates != NULL ? memStates->GetLength() : 1;
        const Int32 NP = procStates != NULL ? procStates->GetLength() : 1;
        for (Int32 is = 0; is < NS; is++) {
            for (Int32 im = 0; im < NM; im++) {
                for (Int32 ip = 0; ip < NP; ip++) {
                    pw->Print(sep);
                    Boolean printed = FALSE;
                    if (screenStates != NULL && screenStates->GetLength() > 1) {
                        PrintScreenLabelCsv(pw, (*screenStates)[is]);
                        printed = TRUE;
                    }
                    if (memStates != NULL && memStates->GetLength() > 1) {
                        if (printed) {
                            pw->Print(String("-"));
                        }
                        PrintMemLabelCsv(pw, (*memStates)[im]);
                        printed = TRUE;
                    }
                    if (procStates != NULL && procStates->GetLength() > 1) {
                        if (printed) {
                            pw->Print(String("-"));
                        }
                        pw->Print((*STATE_NAMES_CSV)[(*procStates)[ip]]);
                    }
                }
            }
        }
    }

    static void DumpProcessStateCsv(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IProcessState* _proc,
        /* [in] */ Boolean sepScreenStates,
        /* [in] */ ArrayOf<Int32>* screenStates,
        /* [in] */ Boolean sepMemStates,
        /* [in] */ ArrayOf<Int32>* memStates,
        /* [in] */ Boolean sepProcStates,
        /* [in] */ ArrayOf<Int32>* procStates,
        /* [in] */ Int64 now)
    {
        ProcessState* proc = (ProcessState*)_proc;
        const Int32 NSS = sepScreenStates ? screenStates->GetLength() : 1;
        const Int32 NMS = sepMemStates ? memStates->GetLength() : 1;
        const Int32 NPS = sepProcStates ? procStates->GetLength() : 1;
        for (Int32 iss = 0; iss < NSS; iss++) {
            for (Int32 ims = 0; ims < NMS; ims++) {
                for (Int32 ips = 0; ips < NPS; ips++) {
                    const Int32 vsscreen = sepScreenStates ? screenStates[iss] : 0;
                    const Int32 vsmem = sepMemStates ? memStates[ims] : 0;
                    const Int32 vsproc = sepProcStates ? procStates[ips] : 0;
                    const Int32 NSA = sepScreenStates ? 1 : screenStates->GetLength();
                    const Int32 NMA = sepMemStates ? 1 : memStates->GetLength();
                    const Int32 NPA = sepProcStates ? 1 : procStates->GetLength();
                    Int64 totalTime = 0;
                    for (Int32 isa = 0; isa < NSA; isa++) {
                        for (Int32 ima = 0; ima < NMA; ima++) {
                            for (Int32 ipa = 0; ipa < NPA; ipa++) {
                                const Int32 vascreen = sepScreenStates ? 0 : screenStates[isa];
                                const Int32 vamem = sepMemStates ? 0 : memStates[ima];
                                const Int32 vaproc = sepProcStates ? 0 : procStates[ipa];
                                const Int32 bucket = ((vsscreen + vascreen + vsmem + vamem)
                                        * STATE_COUNT) + vsproc + vaproc;
                                totalTime += proc->GetDuration(bucket, now);
                            }
                        }
                    }
                    pw->Print(CSV_SEP);
                    pw->Print(totalTime);
                }
            }
        }
    }

    static void DumpProcessList(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ IArrayList* procs,
        /* [in] */ const ArrayOf<Int32>& screenStates,
        /* [in] */ const ArrayOf<Int32>& memStates,
        /* [in] */ const ArrayOf<Int32>& procStates,
        /* [in] */ Int64 now)
    {
        String innerPrefix = prefix + "  ";
        Int32 size;
        procs->GetSize(&size);
        for (Int32 i = size - 1; i >= 0; i--) {
            AutoPtr<IInterface> item;
            procs->Get(i, (IInterface**)&item);
            ProcessState* proc = (ProcessState*)IProcessState::Probe(item);
            pw->Print(prefix);
            pw->Print(proc->mName);
            pw->Print(String(" / "));
            UserHandle::FormatUid(pw, proc->mUid);
            pw->Print(String(" ("));
            pw->Print(proc->mDurationsTableSize);
            pw->Print(String(" entries)"));
            pw->Println(String(":"));
            DumpProcessState(pw, innerPrefix, proc, screenStates, memStates, procStates, now);
            if (proc->mPssTableSize > 0) {
                DumpProcessPss(pw, innerPrefix, proc, screenStates, memStates, procStates);
            }
        }
    }

    static void DumpProcessSummaryDetails(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IProcessState* proc,
        /* [in] */ const String& prefix,
        /* [in] */ String label,
        /* [in] */ ArrayOf<Int32>* screenStates,
        /* [in] */ ArrayOf<Int32>* memStates,
        /* [in] */ ArrayOf<Int32>* procStates,
        /* [in] */ Int64 now,
        /* [in] */ Int64 totalTime,
        /* [in] */ Boolean full)
    {
        AutoPtr<ProcessDataCollection> totals = new ProcessDataCollection(screenStates,
                memStates, procStates);
        ComputeProcessData(proc, totals, now);
        if (totals->mTotalTime != 0 || totals->mNumPss != 0) {
            if (prefix != NULL) {
                pw->Print(prefix);
            }
            if (label != NULL) {
                pw->Print(label);
            }
            totals->Print(pw, totalTime, full);
            if (prefix != NULL) {
                pw->Println();
            }
        }
    }

    static void DumpProcessSummaryLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ IArrayList* procs,
        /* [in] */ ArrayOf<Int32>* screenStates,
        /* [in] */ ArrayOf<Int32>* memStates,
        /* [in] */ ArrayOf<Int32>* procStates,
        /* [in] */ Boolean inclUidVers,
        /* [in] */ Int64 now,
        /* [in] */ Int64 totalTime)
    {
        Int32 size;
        procs->GetSize(&size);
        for (Int32 i = size - 1; i >= 0; i--) {
            AutoPtr<IInterface> item;
            procs->Get(i, (IInterface**)&item);
            ProcessState* proc = (ProcessState*)IProcessState::Probe(item);
            pw->Print(prefix);
            pw->Print(String("* "));
            pw->Print(proc->mName);
            pw->Print(String(" / "));
            UserHandle::FormatUid(pw, proc->mUid);
            pw->Print(String(" / v"));
            pw->Print(proc->mVersion);
            pw->Println(String(":"));
            DumpProcessSummaryDetails(pw, proc, prefix, String("         TOTAL: "),
                screenStates, memStates, procStates, now, totalTime, TRUE);
            AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(1);
            (*array)[0] = STATE_PERSISTENT;
            DumpProcessSummaryDetails(pw, proc, prefix, String("    Persistent: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_TOP;
            DumpProcessSummaryDetails(pw, proc, prefix, String("           Top: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_IMPORTANT_FOREGROUND;
            DumpProcessSummaryDetails(pw, proc, prefix, String("        Imp Fg: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_IMPORTANT_BACKGROUND;
            DumpProcessSummaryDetails(pw, proc, prefix, String("        Imp Bg: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_BACKUP;
            DumpProcessSummaryDetails(pw, proc, prefix, String("        Backup: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_HEAVY_WEIGHT;
            DumpProcessSummaryDetails(pw, proc, prefix, String("     Heavy Wgt: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_SERVICE;
            DumpProcessSummaryDetails(pw, proc, prefix, String("       Service: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_SERVICE_RESTARTING;
            DumpProcessSummaryDetails(pw, proc, prefix, String("    Service Rs: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_RECEIVER;
            DumpProcessSummaryDetails(pw, proc, prefix, String("      Receiver: "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_HOME;
            DumpProcessSummaryDetails(pw, proc, prefix, String("        (Home): "),
                screenStates, memStates, array, now, totalTime, TRUE);
            (*array)[0] = STATE_LAST_ACTIVITY;
            DumpProcessSummaryDetails(pw, proc, prefix, String("    (Last Act): "),
                screenStates, memStates, array, now, totalTime, TRUE);
            array = ArrayOf<Int32>::Alloc(3);
            (*array)[0] = STATE_CACHED_ACTIVITY;
            (*array)[1] = STATE_CACHED_ACTIVITY_CLIENT;
            (*array)[2] = STATE_CACHED_EMPTY;
            DumpProcessSummaryDetails(pw, proc, prefix, String("      (Cached): "),
                screenStates, memStates, array, now, totalTime, TRUE);
        }
    }

    static void PrintPercent(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Double fraction)
    {
        fraction *= 100;
        String str;
        if (fraction < 1) {
            str.AppendFormat("%.2f", fraction);
            pw->Print(str);
        }
        else if (fraction < 10) {
            str.AppendFormat("%.1f", fraction);
            pw->Print(str);
        }
        else {
            str.AppendFormat("%.0f", fraction);
            pw->Print(str);
        }
        pw->Print(String("%"));
    }

    static void PrintSizeValue(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int64 number)
    {
        Float result = number;
        String suffix("");
        if (result > 900) {
            suffix = "KB";
            result = result / 1024;
        }
        if (result > 900) {
            suffix = "MB";
            result = result / 1024;
        }
        if (result > 900) {
            suffix = "GB";
            result = result / 1024;
        }
        if (result > 900) {
            suffix = "TB";
            result = result / 1024;
        }
        if (result > 900) {
            suffix = "PB";
            result = result / 1024;
        }
        String value;
        if (result < 1) {
            value.AppendFormat("%.2f", result);
        }
        else if (result < 10) {
            value.AppendFormat("%.1f", result);
        }
        else if (result < 100) {
            value.AppendFormat("%.0f", result);
        }
        else {
            value.AppendFormat("%.0f", result);
        }
        pw->Print(value);
        pw->Print(suffix);
    }

    public static void DumpProcessListCsv(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IArrayList* procs,
        /* [in] */ Boolean sepScreenStates,
        /* [in] */ ArrayOf<Int32>* screenStates,
        /* [in] */ Boolean sepMemStates,
        /* [in] */ ArrayOf<Int32>* memStates,
        /* [in] */ Boolean sepProcStates,
        /* [in] */ ArrayOf<Int32>* procStates,
        /* [in] */ Int64 now)
    {
        pw->Print(String("process"));
        pw->Print(CSV_SEP);
        pw->Print(String("uid"));
        pw->Print(CSV_SEP);
        pw->Print(String("vers"));
        DumpStateHeadersCsv(pw, CSV_SEP, sepScreenStates ? screenStates : NULL,
                sepMemStates ? memStates : NULL, sepProcStates ? procStates : NULL);
        pw->Println();
        Int32 size;
        procs->GetSize(&size);
        for (Int32 i = size-1; i >= 0; i--) {
            AutoPtr<IInterface> item;
            procs->Get(i, (IInterface**)&item);
            ProcessState* proc = (ProcessState*)IProcessState::Probe(item);
            pw->Print(proc->mName);
            pw->Print(CSV_SEP);
            UserHandle::FormatUid(pw, proc->mUid);
            pw->Print(CSV_SEP);
            pw->Print(proc->mVersion);
            DumpProcessStateCsv(pw, proc, sepScreenStates, screenStates,
                    sepMemStates, memStates, sepProcStates, procStates, now);
            pw->Println();
        }
    }

    static Int32 PrintArrayEntry(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const ArrayOf<String>& array,
        /* [in] */ Int32 value,
        /* [in] */ Int32 mod)
    {
        Int32 index = value/mod;
        if (index >= 0 && index < array.GetLength()) {
            pw->Print(array[index]);
        }
        else {
            pw->PrintChar('?');
        }
        return value - index*mod;
    }

    static void PrintProcStateTag(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 state)
    {
        state = PrintArrayEntry(pw, *ADJ_SCREEN_TAGS,  state, ADJ_SCREEN_MOD*STATE_COUNT);
        state = PrintArrayEntry(pw, *ADJ_MEM_TAGS,  state, STATE_COUNT);
        PrintArrayEntry(pw, *STATE_TAGS,  state, 1);
    }

    static void PrintAdjTag(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 state)
    {
        state = PrintArrayEntry(pw, *DJ_SCREEN_TAGS,  state, ADJ_SCREEN_MOD);
        PrintArrayEntry(pw, *ADJ_MEM_TAGS, state, 1);
    }

    static void PrintProcStateTagAndValue(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 state,
        /* [in] */ Int64 value)
    {
        pw->PrintChar(',');
        PrintProcStateTag(pw, state);
        pw->PrintChar(':');
        pw->Print(value);
    }

    static void PrintAdjTagAndValue(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 state,
        /* [in] */ Int64 value)
    {
        pw->PrintChar(',');
        PrintAdjTag(pw, state);
        pw->PrintChar(':');
        pw->Print(value);
    }

    static void DumpAllProcessStateCheckin(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IProcessState* _proc,
        /* [in] */ Int64 now)
    {
        ProcessState* proc = (ProcessState*)_proc;
        Boolean didCurState = FALSE;
        for (Int32 i = 0; i < proc->mDurationsTableSize; i++) {
            Int32 off = (*proc->mDurationsTable)[i];
            Int32 type = (off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
            Int64 time = proc->mStats->GetInt64(off, 0);
            if (proc->mCurState == type) {
                didCurState = TRUE;
                time += now - proc->mStartTime;
            }
            PrintProcStateTagAndValue(pw, type, time);
        }
        if (!didCurState && proc->mCurState != STATE_NOTHING) {
            PrintProcStateTagAndValue(pw, proc->mCurState, now - proc->mStartTime);
        }
    }

    static void DumpAllProcessPssCheckin(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IProcessState* _proc)
    {
        ProcessState* proc = (ProcessState*)_proc;
        for (Int32 i = 0; i < proc->mPssTableSize; i++) {
            Int32 off = (*proc->mPssTable)[i];
            Int32 type = (off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
            Int64 count = proc->mStats->GetInt64(off, PSS_SAMPLE_COUNT);
            Int64 min = proc->mStats->GetInt64(off, PSS_MINIMUM);
            Int64 avg = proc->mStats->GetInt64(off, PSS_AVERAGE);
            Int64 max = proc->mStats->GetInt64(off, PSS_MAXIMUM);
            Int64 umin = proc->mStats->GetInt64(off, PSS_USS_MINIMUM);
            Int64 uavg = proc->mStats->GetInt64(off, PSS_USS_AVERAGE);
            Int64 umax = proc->mStats->GetInt64(off, PSS_USS_MAXIMUM);
            pw->PrintChar(',');
            PrintProcStateTag(pw, type);
            pw->PrintChar(':');
            pw->Print(count);
            pw->PrintChar(':');
            pw->Print(min);
            pw->PrintChar(':');
            pw->Print(avg);
            pw->PrintChar(':');
            pw->Print(max);
            pw->PrintChar(':');
            pw->Print(umin);
            pw->PrintChar(':');
            pw->Print(uavg);
            pw->PrintChar(':');
            pw->Print(umax);
        }
    }

    CARAPI Reset()
    {
        if (DEBUG) Slogger::D(TAG, "Resetting state of %s", mTimePeriodStartClockStr.string());
        ResetCommon();
        AutoPtr<IArrayMap> map;
        mPackages->GetMap((IArrayMap**)&map);
        map->Clear();
        map = NULL;
        mProcesses->GetMap((IArrayMap**)&map);
        map->Clear();
        mMemFactor = STATE_NOTHING;
        mStartTime = 0;
        if (DEBUG) Slogger::D(TAG, "State Reset; now %s", mTimePeriodStartClockStr.string());
        return NOERROR;
    }

    CARAPI ResetSafely()
    {
        if (DEBUG) Slogger::D(TAG, "Safely Resetting state of %s", mTimePeriodStartClockStr.string());
        ResetCommon();

        // First initialize use count of all common processes.
        const Int64 now = SystemClock::GetUptimeMillis();
        AutoPtr<IArrayMap> procMap;
        mProcesses->GetMap((IArrayMap**)&procMap);
        procMap->GetSize(&size);
        for (Int32 ip = size - 1; ip >= 0; ip--) {
            AutoPtr<IInterface> value;
            procMap->GetValueAt(ip, (IInterface**)&value);
            AutoPtr<ISparseArray> uids = ISparseArray::Probe(value);
            Int32 uSize;
            uids->GetSize(&uSize);
            for (Int32 iu = uSize - 1; iu >= 0; iu--) {
                AutoPtr<IInterface> uValue;
                uids->ValueAt(iu, (IInterface**)&uValue);
                ProcessState* proc = (ProcessState*)IProcessState::Probe(uValue);
                proc->mTmpNumInUse = 0;
           }
        }

        // Next Reset or prune all per-package processes, and for the ones that are Reset
        // track this back to the common processes.
        AutoPtr<IArrayMap> pkgMap;
        other->mPackages->GetMap((IArrayMap**)&pkgMap);
        pkgMap->GetSize(&size);
        for (Int32 ip = size - 1; ip >= 0; ip--) {
            AutoPtr<IInterface> value;
            pkgMap->GetValueAt(ip, (IInterface**)&value);
            AutoPtr<ISparseArray> uids = ISparseArray::Probe(value);
            Int32 uSize;
            uids->GetSize(&uSize);
            for (Int32 iu = uSize - 1; iu >= 0; iu--) {
                AutoPtr<IInterface> uValue;
                uids->ValueAt(iu, (IInterface**)&uValue);
                AutoPtr<ISparseArray> vpkgs = ISparseArray::Probe(uValue);
                Int32 vSize;
                vpkgs->GetSize(&vSize);
                for (Int32 iv = vSize - 1; iv >= 0; iv--) {
                    AutoPtr<IInterface> vValue;
                    vpkgs->ValueAt(iv, (IInterface**)&vValue);
                    PackageState* pkgState = (PackageState*)IPackageState::Probe(vValue);
                    Int32 NPROCS;
                    pkgState->mProcesses->GetSize(&NPROCS);
                    Int32 NSRVS;
                    pkgState->mServices->GetSize(&NSRVS);
                    for (Int32 iproc = NPROCS - 1; iproc >= 0; iproc--) {
                        AutoPtr<IInterface> procValue;
                        pkgState->mProcesses->GetValueAt(iproc, (IInterface**)&procValue);
                        ProcessState* ps = (ProcessState*)IProcessState::Probe(procValue);
                        if (ps->IsInUse()) {
                            ps->ResetSafely(now);
                            ps->mCommonProcess->mTmpNumInUse++;
                            ps->mCommonProcess->mTmpFoundSubProc = ps;
                        }
                        else {
                            ps->MakeDead();
                            pkgState->mProcesses->RemoveAt(iproc);
                        }
                    }

                    for (Int32 isvc = NSRVS - 1; isvc >= 0; isvc--) {
                        AutoPtr<IInterface> svcValue;
                        pkgState->mServices->GetValueAt(isvc, (IInterface**)&svcValue);
                        ServiceState* ss = (ServiceState*)IServiceState::Probe(svcValue);
                        if (ss->isInUse()) {
                            ss->ResetSafely(now);
                        }
                        else {
                            pkgState->mServices->RemoveAt(isvc);
                        }
                    }
                    pkgState->mProcesses->GetSize(&NPROCS);
                    pkgState->mServices->GetSize(&NSRVS);
                    if (NPROCS <= 0 && NSRVS <= 0) {
                        vpkgs->RemoveAt(iv);
                    }
                }
                vpkgs->GetSize(&vSize);
                if (vSize <= 0) {
                    uids->RemoveAt(iu);
                }
            }
            uids->GetSize(&uSize);
            if (uSize <= 0) {
                pkgMap->RemoveAt(ip);
            }
        }

        // Finally prune out any common processes that are no longer in use.
        procMap = NULL;
        mProcesses->GetMap((IArrayMap**)&procMap);
        procMap->GetSize(&size);
        for (Int32 ip = size - 1; ip >= 0; ip--) {
            AutoPtr<IInterface> value;
            procMap->GetValueAt(ip, (IInterface**)&value);
            AutoPtr<ISparseArray> uids = ISparseArray::Probe(value);
            Int32 uSize;
            uids->GetSize(&uSize);
            for (Int32 iu = uSize - 1; iu >= 0; iu--) {
                AutoPtr<IInterface> uValue;
                uids->ValueAt(iu, (IInterface**)&uValue);
                ProcessState* ps = (ProcessState*)IProcessState::Probe(uValue);
                if (ps.isInUse() || ps.mTmpNumInUse > 0) {
                    // If this is a process for multiple packages, we could at this point
                    // be back down to one package.  In that case, we want to revert back
                    // to a single shared ProcessState.  We can do this by converting the
                    // current package-specific ProcessState up to the shared ProcessState,
                    // throwing away the current one we have here (because nobody else is
                    // using it).
                    if (!ps.mActive && ps.mMultiPackage && ps.mTmpNumInUse == 1) {
                        // Here we go...
                        ps = ps.mTmpFoundSubProc;
                        ps.mCommonProcess = ps;
                        uids.setValueAt(iu, ps);
                    }
                    else {
                        ps.ResetSafely(now);
                    }
                }
                else {
                    ps.MakeDead();
                    uids.removeAt(iu);
                }
            }
            if (uids.size() <= 0) {
                procMap.removeAt(ip);
            }
        }

        mStartTime = now;
        if (DEBUG) Slogger::D(TAG, "State Reset; now " + mTimePeriodStartClockStr);
    }

    private void ResetCommon() {
        mTimePeriodStartClock = System.currentTimeMillis();
        buildTimePeriodStartClockStr();
        mTimePeriodStartRealtime = mTimePeriodEndRealtime = SystemClock::GetElapsedRealtime();
        mTimePeriodStartUptime = mTimePeriodEndUptime = SystemClock::GetUptimeMillis();
        mLongs.clear();
        mLongs->Add(new Int64[LONGS_SIZE]);
        mNextLong = 0;
        Arrays.fill(mMemFactorDurations, 0);
        mSysMemUsageTable = NULL;
        mSysMemUsageTableSize = 0;
        mStartTime = 0;
        mReadError = NULL;
        mFlags = 0;
        evaluateSystemProperties(TRUE);
    }

    public Boolean evaluateSystemProperties(
        /* [in] */ Boolean update) {
        Boolean changed = FALSE;
        String runtime = SystemProperties.get("persist.sys.dalvik.vm.lib.2",
                VMRuntime.getRuntime().vmLibrary());
        if (!Objects.equals(runtime, mRuntime)) {
            changed = TRUE;
            if (update) {
                mRuntime = runtime;
            }
        }
        return changed;
    }

    private void buildTimePeriodStartClockStr() {
        mTimePeriodStartClockStr = DateFormat.format("yyyy-MM-dd-HH-mm-ss",
                mTimePeriodStartClock).toString();
    }

    static final Int32[] BAD_TABLE = new Int32[0];

    private Int32[] readTableFromParcel(Parcel in,
        /* [in] */ const String& name,
    /* [in] */ const String& what) {
        final Int32 size = in.readInt();
        if (size < 0) {
            Slogger::W(TAG, "Ignoring existing stats; bad " + what + " table size: " + size);
            return BAD_TABLE;
        }
        if (size == 0) {
            return NULL;
        }
        final Int32[] table = new Int32[size];
        for (Int32 i = 0; i < size; i++) {
            table[i] = in.readInt();
            if (DEBUG_PARCEL) Slogger::I(TAG, "Reading in " + name + " table #" + i + ": "
                    + ProcessStats->PrintLongOffset(table[i]));
            if (!validateLongOffset(table[i])) {
                Slogger::W(TAG, "Ignoring existing stats; bad " + what + " table entry: "
                        + ProcessStats->PrintLongOffset(table[i]));
                return NULL;
            }
        }
        return table;
    }

    private void writeCompactedLongArray(Parcel out,
        /* [in] */ const ArrayOf<Int64>& array,
        /* [in] */ Int32 num) {
        for (Int32 i = 0; i < num; i++) {
            Int64 val = array[i];
            if (val < 0) {
                Slogger::W(TAG, "Time val negative: " + val);
                val = 0;
            }
            if (val <= Integer.MAX_VALUE) {
                out.writeInt((Int32)val);
            }
            else {
                Int32 top = ~((Int32)((val>>32)&0x7fffffff));
                Int32 bottom = (Int32)(val&0xfffffff);
                out.writeInt(top);
                out.writeInt(bottom);
            }
        }
    }

    private void readCompactedLongArray(Parcel in,
        /* [in] */ Int32 version,
        /* [in] */ const ArrayOf<Int64>& array,
    /* [in] */ Int32 num) {
        if (version <= 10) {
            in.readLongArray(array);
            return;
        }
        final Int32 alen = array.GetLength();
        if (num > alen) {
            throw new RuntimeException("bad array lengths: got " + num + " array is " + alen);
        }
        Int32 i;
        for (i = 0; i < num; i++) {
            Int32 val = in.readInt();
            if (val >= 0) {
                array[i] = val;
            }
            else {
                Int32 bottom = in.readInt();
                array[i] = (((Int64)~val)<<32) | bottom;
            }
        }
        while (i < alen) {
            array[i] = 0;
            i++;
        }
    }

    private void writeCommonString(Parcel out,
        /* [in] */ const String& name) {
        Integer index = mCommonStringToIndex.get(name);
        if (index != NULL) {
            out.writeInt(index);
            return;
        }
        index = mCommonStringToIndex.size();
        mCommonStringToIndex.put(name, index);
        out.writeInt(~index);
        out.writeString(name);
    }

    private String readCommonString(Parcel in,
        /* [in] */ Int32 version) {
        if (version <= 9) {
            return in.readString();
        }
        Int32 index = in.readInt();
        if (index >= 0) {
            return mIndexToCommonString.get(index);
        }
        index = ~index;
        String name = in.readString();
        while (mIndexToCommonString.size() <= index) {
            mIndexToCommonString->Add(NULL);
        }
        mIndexToCommonString.set(index, name);
        return name;
    }

    @Override
    public Int32 describeContents() {
        return 0;
    }

    @Override
    CARAPI WriteToParcel(Parcel out,
        /* [in] */ Int32 flags) {
        writeToParcel(out, SystemClock::GetUptimeMillis(), flags);
    }

    /** @hide */
    CARAPI WriteToParcel(Parcel out,
        /* [in] */ Int64 now,
        /* [in] */ Int32 flags) {
        out.writeInt(MAGIC);
        out.writeInt(PARCEL_VERSION);
        out.writeInt(STATE_COUNT);
        out.writeInt(ADJ_COUNT);
        out.writeInt(PSS_COUNT);
        out.writeInt(SYS_MEM_USAGE_COUNT);
        out.writeInt(LONGS_SIZE);

        mCommonStringToIndex = new ArrayMap<String, Integer>(mProcesses->MMap.size());

        // First commit all running times.
        ArrayMap<String, SparseArray<ProcessState>> procMap = mProcesses->GetMap();
        final Int32 NPROC = procMap.size();
        for (Int32 ip=0; ip<NPROC; ip++) {
            SparseArray<ProcessState> uids = procMap.valueAt(ip);
            final Int32 NUID = uids.size();
            for (Int32 iu=0; iu<NUID; iu++) {
                uids.valueAt(iu).commitStateTime(now);
            }
        }
        final ArrayMap<String, SparseArray<SparseArray<PackageState>>> pkgMap = mPackages->GetMap();
        final Int32 NPKG = pkgMap.size();
        for (Int32 ip=0; ip<NPKG; ip++) {
            final SparseArray<SparseArray<PackageState>> uids = pkgMap.valueAt(ip);
            final Int32 NUID = uids.size();
            for (Int32 iu=0; iu<NUID; iu++) {
                final SparseArray<PackageState> vpkgs = uids.valueAt(iu);
                final Int32 NVERS = vpkgs.size();
                for (Int32 iv=0; iv<NVERS; iv++) {
                    PackageState pkgState = vpkgs.valueAt(iv);
                    final Int32 NPROCS = pkgState.mProcesses->Size();
                    for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                        ProcessState proc = pkgState.mProcesses->ValueAt(iproc);
                        if (proc->mCommonProcess != proc) {
                            proc->commitStateTime(now);
                        }
                    }
                    final Int32 NSRVS = pkgState.mServices->Size();
                    for (Int32 isvc=0; isvc<NSRVS; isvc++) {
                        pkgState.mServices->ValueAt(isvc).commitStateTime(now);
                    }
                }
            }
        }

        out.writeLong(mTimePeriodStartClock);
        out.writeLong(mTimePeriodStartRealtime);
        out.writeLong(mTimePeriodEndRealtime);
        out.writeLong(mTimePeriodStartUptime);
        out.writeLong(mTimePeriodEndUptime);
        out.writeString(mRuntime);
        out.writeInt(mFlags);

        out.writeInt(mLongs.size());
        out.writeInt(mNextLong);
        for (Int32 i = 0; i < (mLongs.size()-1); i++) {
            AutoPtr<ArrayOf<Int64> > array = mLongs[i];
            writeCompactedLongArray(out, array, array.GetLength());
        }
        AutoPtr<ArrayOf<Int64> > lastLongs = mLongs[mLongs.size() - 1];
        writeCompactedLongArray(out, lastLongs, mNextLong);

        if (mMemFactor != STATE_NOTHING) {
            (*mMemFactorDurations)[mMemFactor] += now - mStartTime;
            mStartTime = now;
        }
        writeCompactedLongArray(out, mMemFactorDurations, mMemFactorDurations.GetLength());

        out.writeInt(mSysMemUsageTableSize);
        for (Int32 i = 0; i < mSysMemUsageTableSize; i++) {
            if (DEBUG_PARCEL) Slogger::I(TAG, "Writing sys mem usage #" + i + ": "
                    + printLongOffset((*mSysMemUsageTable)[i]));
            out.writeInt((*mSysMemUsageTable)[i]);
        }

        out.writeInt(NPROC);
        for (Int32 ip=0; ip<NPROC; ip++) {
            writeCommonString(out, procMap.keyAt(ip));
            final SparseArray<ProcessState> uids = procMap.valueAt(ip);
            final Int32 NUID = uids.size();
            out.writeInt(NUID);
            for (Int32 iu=0; iu<NUID; iu++) {
                out.writeInt(uids.keyAt(iu));
                final ProcessState proc = uids.valueAt(iu);
                writeCommonString(out, proc->mPackage);
                out.writeInt(proc->mVersion);
                proc->writeToParcel(out, now);
            }
        }
        out.writeInt(NPKG);
        for (Int32 ip=0; ip<NPKG; ip++) {
            writeCommonString(out, pkgMap.keyAt(ip));
            final SparseArray<SparseArray<PackageState>> uids = pkgMap.valueAt(ip);
            final Int32 NUID = uids.size();
            out.writeInt(NUID);
            for (Int32 iu=0; iu<NUID; iu++) {
                out.writeInt(uids.keyAt(iu));
                final SparseArray<PackageState> vpkgs = uids.valueAt(iu);
                final Int32 NVERS = vpkgs.size();
                out.writeInt(NVERS);
                for (Int32 iv=0; iv<NVERS; iv++) {
                    out.writeInt(vpkgs.keyAt(iv));
                    final PackageState pkgState = vpkgs.valueAt(iv);
                    final Int32 NPROCS = pkgState.mProcesses->Size();
                    out.writeInt(NPROCS);
                    for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                        writeCommonString(out, pkgState.mProcesses->KeyAt(iproc));
                        final ProcessState proc = pkgState.mProcesses->ValueAt(iproc);
                        if (proc->mCommonProcess == proc) {
                            // This is the same as the common process we wrote above.
                            out.writeInt(0);
                        }
                        else {
                            // There is separate data for this package's process.
                            out.writeInt(1);
                            proc->writeToParcel(out, now);
                        }
                    }
                    final Int32 NSRVS = pkgState.mServices->Size();
                    out.writeInt(NSRVS);
                    for (Int32 isvc=0; isvc<NSRVS; isvc++) {
                        out.writeString(pkgState.mServices->KeyAt(isvc));
                        final ServiceState svc = pkgState.mServices->ValueAt(isvc);
                        writeCommonString(out, svc->mProcessName);
                        svc->writeToParcel(out, now);
                    }
                }
            }
        }

        mCommonStringToIndex = NULL;
    }

    private Boolean readCheckedInt(Parcel in,
        /* [in] */ Int32 val,
        /* [in] */ const String& what) {
        Int32 got;
        if ((got=in.readInt()) != val) {
            mReadError = "bad " + what + ": " + got;
            return FALSE;
        }
        return TRUE;
    }

    static byte[] readFully(InputStream stream,
        /* [in] */ const ArrayOf<Int32>& outLen) throws IOException {
        Int32 pos = 0;
        final Int32 initialAvail = stream.available();
        byte[] data = new byte[initialAvail > 0 ? (initialAvail+1) : 16384];
        while (TRUE) {
            Int32 amt = stream.read(data, pos, data->mLength-pos);
            if (DEBUG_PARCEL) Slogger::I("foo", "Read " + amt + " bytes at " + pos
                    + " of avail " + data->mLength);
            if (amt < 0) {
                if (DEBUG_PARCEL) Slogger::I("foo", "**** FINISHED READING: pos=" + pos
                        + " len=" + data->mLength);
                outLen[0] = pos;
                return data;
            }
            pos += amt;
            if (pos >= data->mLength) {
                byte[] newData = new byte[pos+16384];
                if (DEBUG_PARCEL) Slogger::I(TAG, "Copying " + pos + " bytes to new array len "
                        + newData.GetLength());
                System.arraycopy(data, 0, newData, 0, pos);
                data = newData;
            }
        }
    }

    CARAPI Read(InputStream stream) {
        try {
            Int32[] len = new Int32[1];
            byte[] raw = readFully(stream, len);
            Parcel in = Parcel.obtain();
            in.unmarshall(raw, 0, len[0]);
            in.setDataPosition(0);
            stream.close();

            readFromParcel(in);
        } catch (IOException e) {
            mReadError = "caught exception: " + e;
        }
    }

    CARAPI ReadFromParcel(Parcel in) {
        final Boolean hadData = mPackages->GetMap().size() > 0
                || mProcesses->GetMap().size() > 0;
        if (hadData) {
            ResetSafely();
        }

        if (!readCheckedInt(in, MAGIC, "magic number")) {
            return;
        }
        Int32 version = in.readInt();
        if (version != PARCEL_VERSION) {
            mReadError = "bad version: " + version;
            return;
        }
        if (!readCheckedInt(in, STATE_COUNT, "state count")) {
            return;
        }
        if (!readCheckedInt(in, ADJ_COUNT, "adj count")) {
            return;
        }
        if (!readCheckedInt(in, PSS_COUNT, "pss count")) {
            return;
        }
        if (!readCheckedInt(in, SYS_MEM_USAGE_COUNT, "sys mem usage count")) {
            return;
        }
        if (!readCheckedInt(in, LONGS_SIZE, "longs size")) {
            return;
        }

        mIndexToCommonString = new ArrayList<String>();

        mTimePeriodStartClock = in.readLong();
        buildTimePeriodStartClockStr();
        mTimePeriodStartRealtime = in.readLong();
        mTimePeriodEndRealtime = in.readLong();
        mTimePeriodStartUptime = in.readLong();
        mTimePeriodEndUptime = in.readLong();
        mRuntime = in.readString();
        mFlags = in.readInt();

        final Int32 NLONGS = in.readInt();
        final Int32 NEXTLONG = in.readInt();
        mLongs.clear();
        for (Int32 i = 0; i < (NLONGS-1); i++) {
            while (i >= mLongs.size()) {
                mLongs->Add(new Int64[LONGS_SIZE]);
            }
            readCompactedLongArray(in, version, mLongs[i), LONGS_SIZE];
        }
        AutoPtr<ArrayOf<Int64> > longs = new Int64[LONGS_SIZE];
        mNextLong = NEXTLONG;
        readCompactedLongArray(in, version, longs, NEXTLONG);
        mLongs->Add(longs);

        readCompactedLongArray(in, version, mMemFactorDurations, mMemFactorDurations.GetLength());

        mSysMemUsageTable = readTableFromParcel(in, TAG, "sys mem usage");
        if (mSysMemUsageTable == BAD_TABLE) {
            return;
        }
        mSysMemUsageTableSize = mSysMemUsageTable != NULL ? (*mSysMemUsageTable).GetLength() : 0;

        Int32 NPROC = in.readInt();
        if (NPROC < 0) {
            mReadError = "bad process count: " + NPROC;
            return;
        }
        while (NPROC > 0) {
            NPROC--;
            final String procName = readCommonString(in, version);
            if (procName == NULL) {
                mReadError = "bad process name";
                return;
            }
            Int32 NUID = in.readInt();
            if (NUID < 0) {
                mReadError = "bad uid count: " + NUID;
                return;
            }
            while (NUID > 0) {
                NUID--;
                final Int32 uid = in.readInt();
                if (uid < 0) {
                    mReadError = "bad uid: " + uid;
                    return;
                }
                final String pkgName = readCommonString(in, version);
                if (pkgName == NULL) {
                    mReadError = "bad process package name";
                    return;
                }
                final Int32 vers = in.readInt();
                ProcessState proc = hadData ? mProcesses->Get(procName, uid) : NULL;
                if (proc != NULL) {
                    if (!proc->readFromParcel(in, FALSE)) {
                        return;
                    }
                }
                else {
                    proc = new ProcessState(this, pkgName, uid, vers, procName);
                    if (!proc->readFromParcel(in, TRUE)) {
                        return;
                    }
                }
                if (DEBUG_PARCEL) Slogger::D(TAG, "Adding process: " + procName + " " + uid
                        + " " + proc);
                mProcesses->Put(procName, uid, proc);
            }
        }

        if (DEBUG_PARCEL) Slogger::D(TAG, "Read " + mProcesses->GetMap().size() + " processes");

        Int32 NPKG = in.readInt();
        if (NPKG < 0) {
            mReadError = "bad package count: " + NPKG;
            return;
        }
        while (NPKG > 0) {
            NPKG--;
            final String pkgName = readCommonString(in, version);
            if (pkgName == NULL) {
                mReadError = "bad package name";
                return;
            }
            Int32 NUID = in.readInt();
            if (NUID < 0) {
                mReadError = "bad uid count: " + NUID;
                return;
            }
            while (NUID > 0) {
                NUID--;
                final Int32 uid = in.readInt();
                if (uid < 0) {
                    mReadError = "bad uid: " + uid;
                    return;
                }
                Int32 NVERS = in.readInt();
                if (NVERS < 0) {
                    mReadError = "bad versions count: " + NVERS;
                    return;
                }
                while (NVERS > 0) {
                    NVERS--;
                    final Int32 vers = in.readInt();
                    PackageState pkgState = new PackageState(pkgName, uid);
                    SparseArray<PackageState> vpkg = mPackages->Get(pkgName, uid);
                    if (vpkg == NULL) {
                        vpkg = new SparseArray<PackageState>();
                        mPackages->Put(pkgName, uid, vpkg);
                    }
                    vpkg.put(vers, pkgState);
                    Int32 NPROCS = in.readInt();
                    if (NPROCS < 0) {
                        mReadError = "bad package process count: " + NPROCS;
                        return;
                    }
                    while (NPROCS > 0) {
                        NPROCS--;
                        String procName = readCommonString(in, version);
                        if (procName == NULL) {
                            mReadError = "bad package process name";
                            return;
                        }
                        Int32 hasProc = in.readInt();
                        if (DEBUG_PARCEL) Slogger::D(TAG, "Reading package " + pkgName + " " + uid
                                + " process " + procName + " hasProc=" + hasProc);
                        ProcessState commonProc = mProcesses->Get(procName, uid);
                        if (DEBUG_PARCEL) Slogger::D(TAG, "Got common proc " + procName + " " + uid
                                + ": " + commonProc);
                        if (commonProc == NULL) {
                            mReadError = "no common proc: " + procName;
                            return;
                        }
                        if (hasProc != 0) {
                            // The process for this package is unique to the package; we
                            // need to load it.  We don't need to do anything about it if
                            // it is not unique because if someone later looks for it
                            // they will find and use it from the global procs.
                            ProcessState proc = hadData ? pkgState.mProcesses->Get(procName) : NULL;
                            if (proc != NULL) {
                                if (!proc->readFromParcel(in, FALSE)) {
                                    return;
                                }
                            }
                            else {
                                proc = new ProcessState(commonProc, pkgName, uid, vers, procName,
                                        0);
                                if (!proc->readFromParcel(in, TRUE)) {
                                    return;
                                }
                            }
                            if (DEBUG_PARCEL) Slogger::D(TAG, "Adding package " + pkgName + " process: "
                                    + procName + " " + uid + " " + proc);
                            pkgState.mProcesses->Put(procName, proc);
                        }
                        else {
                            if (DEBUG_PARCEL) Slogger::D(TAG, "Adding package " + pkgName + " process: "
                                    + procName + " " + uid + " " + commonProc);
                            pkgState.mProcesses->Put(procName, commonProc);
                        }
                    }
                    Int32 NSRVS = in.readInt();
                    if (NSRVS < 0) {
                        mReadError = "bad package service count: " + NSRVS;
                        return;
                    }
                    while (NSRVS > 0) {
                        NSRVS--;
                        String serviceName = in.readString();
                        if (serviceName == NULL) {
                            mReadError = "bad package service name";
                            return;
                        }
                        String processName = version > 9 ? readCommonString(in, version) : NULL;
                        ServiceState serv = hadData ? pkgState.mServices->Get(serviceName) : NULL;
                        if (serv == NULL) {
                            serv = new ServiceState(this, pkgName, serviceName, processName, NULL);
                        }
                        if (!serv.readFromParcel(in)) {
                            return;
                        }
                        if (DEBUG_PARCEL) Slogger::D(TAG, "Adding package " + pkgName + " service: "
                                + serviceName + " " + uid + " " + serv);
                        pkgState.mServices->Put(serviceName, serv);
                    }
                }
            }
        }

        mIndexToCommonString = NULL;

        if (DEBUG_PARCEL) Slogger::D(TAG, "Successfully read procstats!");
    }

    Int32 AddLongData(Int32 index,
        /* [in] */ Int32 type,
    /* [in] */ Int32 num) {
        Int32 off = allocLongData(num);
        mAddLongTable = GrowingArrayUtils.insert(
                mAddLongTable != NULL ? mAddLongTable : EmptyArray.INT,
                mAddLongTableSize, index, type | off);
        mAddLongTableSize++;
        return off;
    }

    Int32 allocLongData(Int32 num) {
        Int32 whichLongs = mLongs.size()-1;
        AutoPtr<ArrayOf<Int64> > longs = mLongs[whichLongs];
        if (mNextLong + num > longs.GetLength()) {
            longs = new Int64[LONGS_SIZE];
            mLongs->Add(longs);
            whichLongs++;
            mNextLong = 0;
        }
        Int32 off = (whichLongs<<OFFSET_ARRAY_SHIFT) | (mNextLong<<OFFSET_INDEX_SHIFT);
        mNextLong += num;
        return off;
    }

    Boolean validateLongOffset(Int32 off) {
        Int32 arr = (off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK;
        if (arr >= mLongs.size()) {
            return FALSE;
        }
        Int32 idx = (off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK;
        if (idx >= LONGS_SIZE) {
            return FALSE;
        }
        if (DEBUG_PARCEL) Slogger::D(TAG, "Validated Int64 " + printLongOffset(off)
                + ": " + GetInt64(off, 0));
        return TRUE;
    }

    static String printLongOffset(Int32 off) {
        StringBuilder sb = new StringBuilder(16);
        sb.append("a"); sb.append((off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK);
        sb.append("i"); sb.append((off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK);
        sb.append("t"); sb.append((off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK);
        return sb.toString();
    }

    void setLong(Int32 off,
        /* [in] */ Int32 index,
        /* [in] */ Int64 value) {
        AutoPtr<ArrayOf<Int64> > longs = mLongs[(off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
        longs[index + ((off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK)] = value;
    }

    Int64 GetInt64(Int32 off,
        /* [in] */ Int32 index) {
        AutoPtr<ArrayOf<Int64> > longs = mLongs[(off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
        return longs[index + ((off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK)];
    }

    static Int32 BinarySearch(Int32[] array,
        /* [in] */ Int32 size,
    /* [in] */ Int32 value) {
        Int32 lo = 0;
        Int32 hi = size - 1;

        while (lo <= hi) {
            Int32 mid = (lo + hi) >>> 1;
            Int32 midVal = (array[mid] >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;

            if (midVal < value) {
                lo = mid + 1;
            }
            else if (midVal > value) {
                hi = mid - 1;
            }
            else {
                return mid;  // value found
            }
        }
        return ~lo;  // value not present
    }

    public PackageState GetPackageStateLocked(String packageName,
        /* [in] */ Int32 uid,
    /* [in] */ Int32 vers) {
        SparseArray<PackageState> vpkg = mPackages->Get(packageName, uid);
        if (vpkg == NULL) {
            vpkg = new SparseArray<PackageState>();
            mPackages->Put(packageName, uid, vpkg);
        }
        PackageState as = vpkg.get(vers);
        if (as != NULL) {
            return as;
        }
        as = new PackageState(packageName, uid);
        vpkg.put(vers, as);
        return as;
    }

    public ProcessState GetProcessStateLocked(String packageName,
        /* [in] */ Int32 uid,
    /* [in] */ Int32 vers,
            String processName) {
        final PackageState pkgState = GetPackageStateLocked(packageName, uid, vers);
        ProcessState ps = pkgState.mProcesses->Get(processName);
        if (ps != NULL) {
            return ps;
        }
        ProcessState commonProc = mProcesses->Get(processName, uid);
        if (commonProc == NULL) {
            commonProc = new ProcessState(this, packageName, uid, vers, processName);
            mProcesses->Put(processName, uid, commonProc);
            if (DEBUG) Slogger::D(TAG, "GETPROC created new common " + commonProc);
        }
        if (!commonProc.mMultiPackage) {
            if (packageName.equals(commonProc.mPackage) && vers == commonProc.mVersion) {
                // This common process is not in use by multiple packages, and
                // is for the calling package, so we can just use it directly.
                ps = commonProc;
                if (DEBUG) Slogger::D(TAG, "GETPROC also using for pkg " + commonProc);
            }
            else {
                if (DEBUG) Slogger::D(TAG, "GETPROC need to split common proc!");
                // This common process has not been in use by multiple packages,
                // but it was created for a different package than the caller.
                // We need to convert it to a multi-package process.
                commonProc.mMultiPackage = TRUE;
                // To do this, we need to make two new process states, one a copy
                // of the current state for the process under the original package
                // name, and the second a free new process state for it as the
                // new package name.
                Int64 now = SystemClock::GetUptimeMillis();
                // First let's make a copy of the current process state and put
                // that under the now unique state for its original package name.
                final PackageState commonPkgState = GetPackageStateLocked(commonProc.mPackage,
                        uid, commonProc.mVersion);
                if (commonPkgState != NULL) {
                    ProcessState cloned = commonProc.clone(commonProc.mPackage, now);
                    if (DEBUG) Slogger::D(TAG, "GETPROC setting clone to pkg " + commonProc.mPackage
                            + ": " + cloned);
                    commonPkgState.mProcesses->Put(commonProc.mName, cloned);
                    // If this has active services, we need to update their process pointer
                    // to point to the new package-specific process state.
                    for (Int32 i = commonPkgState.mServices->Size()-1; i>=0; i--) {
                        ServiceState ss = commonPkgState.mServices->ValueAt(i);
                        if (ss.mProc == commonProc) {
                            if (DEBUG) Slogger::D(TAG, "GETPROC switching service to cloned: "
                                    + ss);
                            ss.mProc = cloned;
                        }
                        else if (DEBUG) {
                            Slogger::D(TAG, "GETPROC leaving proc of " + ss);
                        }
                    }
                }
                else {
                    Slogger::W(TAG, "Cloning proc state: no package state " + commonProc.mPackage
                            + "/" + uid + " for proc " + commonProc.mName);
                }
                // And now make a fresh new process state for the new package name.
                ps = new ProcessState(commonProc, packageName, uid, vers, processName, now);
                if (DEBUG) Slogger::D(TAG, "GETPROC created new pkg " + ps);
            }
        }
        else {
            // The common process is for multiple packages, we need to create a
            // separate object for the per-package data.
            ps = new ProcessState(commonProc, packageName, uid, vers, processName,
                    SystemClock::GetUptimeMillis());
            if (DEBUG) Slogger::D(TAG, "GETPROC created new pkg " + ps);
        }
        pkgState.mProcesses->Put(processName, ps);
        if (DEBUG) Slogger::D(TAG, "GETPROC adding new pkg " + ps);
        return ps;
    }

    public ProcessStats.ServiceState GetServiceStateLocked(String packageName,
        /* [in] */ Int32 uid,
    /* [in] */ Int32 vers,
            String processName,
            /* [in] */ const String& className) {
        final ProcessStats.PackageState as = GetPackageStateLocked(packageName, uid, vers);
        ProcessStats.ServiceState ss = as.mServices->Get(className);
        if (ss != NULL) {
            if (DEBUG) Slogger::D(TAG, "GETSVC: returning existing " + ss);
            return ss;
        }
        final ProcessStats.ProcessState ps = processName != NULL
                ? GetProcessStateLocked(packageName, uid, vers, processName) : NULL;
        ss = new ProcessStats.ServiceState(this, packageName, className, processName, ps);
        as.mServices->Put(className, ss);
        if (DEBUG) Slogger::D(TAG, "GETSVC: creating " + ss + " in " + ps);
        return ss;
    }

    private void dumpProcessInternalLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ IProcessState* _proc,
            Boolean dumpAll)
    {
        ProcessState* proc = (ProcessState*)_proc;
        if (dumpAll) {
            pw->Print(prefix);
            pw->Print(String("myID="));
                    pw->Print(Integer.toHexString(System.identityHashCode(proc)));
                    pw->Print(String(" mCommonProcess="));
                    pw->Print(Integer.toHexString(System.identityHashCode(proc->mCommonProcess)));
                    pw->Print(String(" mPackage=");
                        pw->Println(proc->mPackage));
            if (proc->mMultiPackage) {
                pw->Print(prefix);
                pw->Print(String("mMultiPackage=");
                    pw->Println(proc->mMultiPackage));
            }
            if (proc != proc->mCommonProcess) {
                pw->Print(prefix);
                pw->Print(String("Common Proc: ");
                    pw->Print(proc->mCommonProcess.mName));
                        pw->Print(String("/");
                            pw->Print(proc->mCommonProcess.mUid));
                        pw->Print(String(" pkg=");
                            pw->Println(proc->mCommonProcess.mPackage));
            }
        }
        if (proc->mActive) {
            pw->Print(prefix);
            pw->Print(String("mActive=");
                pw->Println(proc->mActive));
        }
        if (proc->mDead) {
            pw->Print(prefix);
            pw->Print(String("mDead=");
                pw->Println(proc->mDead));
        }
        if (proc->mNumActiveServices != 0 || proc->mNumStartedServices != 0) {
            pw->Print(prefix);
            pw->Print(String("mNumActiveServices=");
                pw->Print(proc->mNumActiveServices));
                    pw->Print(String(" mNumStartedServices="));
                    pw->Println(proc->mNumStartedServices);
        }
    }

    CARAPI DumpLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& reqPackage,
        /* [in] */ Int64 now, Boolean dumpSummary,
            Boolean dumpAll, Boolean activeOnly)
    {
        Int64 totalTime = DumpSingleTime(NULL, String(NULL), *mMemFactorDurations, mMemFactor,
                mStartTime, now);
        Boolean sepNeeded = FALSE;
        if (mSysMemUsageTable != NULL) {
            pw->Println(String("System memory usage:"));
            DumpSysMemUsage(pw, String("  "), *ALL_SCREEN_ADJ, *ALL_MEM_ADJ);
            sepNeeded = TRUE;
        }
        ArrayMap<String, SparseArray<SparseArray<PackageState>>> pkgMap = mPackages->GetMap();
        Boolean printedHeader = FALSE;
        for (Int32 ip=0; ip<pkgMap.size(); ip++) {
            final String pkgName = pkgMap.keyAt(ip);
            final SparseArray<SparseArray<PackageState>> uids = pkgMap.valueAt(ip);
            for (Int32 iu=0; iu<uids.size(); iu++) {
                final Int32 uid = uids.keyAt(iu);
                final SparseArray<PackageState> vpkgs = uids.valueAt(iu);
                for (Int32 iv=0; iv<vpkgs.size(); iv++) {
                    final Int32 vers = vpkgs.keyAt(iv);
                    final PackageState pkgState = vpkgs.valueAt(iv);
                    final Int32 NPROCS = pkgState.mProcesses->Size();
                    final Int32 NSRVS = pkgState.mServices->Size();
                    final Boolean pkgMatch = reqPackage == NULL || reqPackage.equals(pkgName);
                    if (!pkgMatch) {
                        Boolean procMatch = FALSE;
                        for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                            ProcessState proc = pkgState.mProcesses->ValueAt(iproc);
                            if (reqPackage.equals(proc->mName)) {
                                procMatch = TRUE;
                                break;
                            }
                        }
                        if (!procMatch) {
                            continue;
                        }
                    }
                    if (NPROCS > 0 || NSRVS > 0) {
                        if (!printedHeader) {
                            if (sepNeeded) pw->Println();
                            pw->Println(String("Per-Package Stats:"));
                            printedHeader = TRUE;
                            sepNeeded = TRUE;
                        }
                        pw->Print(String("  * ");
                            pw->Print(pkgName);
                        pw->Print(String(" / "));
                                UserHandle::FormatUid(pw, uid);
                                pw->Print(String(" / v"));
                                pw->Print(vers);
                                pw->Println(String(":"));
                    }
                    if (!dumpSummary || dumpAll) {
                        for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                            ProcessState proc = pkgState.mProcesses->ValueAt(iproc);
                            if (!pkgMatch && !reqPackage.equals(proc->mName)) {
                                continue;
                            }
                            if (activeOnly && !proc->isInUse()) {
                                pw->Print(String("      (Not active: "));
                                        pw->Print(pkgState.mProcesses->KeyAt(iproc));
                                        pw->Println(String(")"));
                                continue;
                            }
                            pw->Print(String("      Process "));
                            pw->Print(pkgState.mProcesses->KeyAt(iproc));
                            if (proc->mCommonProcess.mMultiPackage) {
                                pw->Print(String(" (multi, "));
                            }
                            else {
                                pw->Print(String(" (unique, "));
                            }
                            pw->Print(proc->mDurationsTableSize);
                            pw->Print(String(" entries)"));
                            pw->Println(String(":"));
                            DumpProcessState(pw, String("        "), proc, *ALL_SCREEN_ADJ, *ALL_MEM_ADJ,
                                    *ALL_PROC_STATES, now);
                            DumpProcessPss(pw, String("        "), proc, *ALL_SCREEN_ADJ, *ALL_MEM_ADJ,
                                    *ALL_PROC_STATES);
                            dumpProcessInternalLocked(pw, String("        "), proc, dumpAll);
                        }
                    }
                    else {
                        ArrayList<ProcessState> procs = new ArrayList<ProcessState>();
                        for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                            ProcessState proc = pkgState.mProcesses->ValueAt(iproc);
                            if (!pkgMatch && !reqPackage.equals(proc->mName)) {
                                continue;
                            }
                            if (activeOnly && !proc->isInUse()) {
                                continue;
                            }
                            procs->Add(proc);
                        }
                        DumpProcessSummaryLocked(pw, String("      "), procs, ALL_SCREEN_ADJ, ALL_MEM_ADJ,
                            NON_CACHED_PROC_STATES, FALSE, now, totalTime);
                    }
                    for (Int32 isvc=0; isvc<NSRVS; isvc++) {
                        ServiceState svc = pkgState.mServices->ValueAt(isvc);
                        if (!pkgMatch && !reqPackage.equals(svc->mProcessName)) {
                            continue;
                        }
                        if (activeOnly && !svc->isInUse()) {
                            pw->Print(String("      (Not active: "));
                                    pw->Print(pkgState.mServices->KeyAt(isvc));
                                    pw->Println(String(")"));
                            continue;
                        }
                        if (dumpAll) {
                            pw->Print(String("      Service "));
                        }
                        else {
                            pw->Print(String("      * "));
                        }
                        pw->Print(pkgState.mServices->KeyAt(isvc));
                        pw->Println(String(":"));
                        pw->Print(String("        Process: ");
                            pw->Println(svc->mProcessName));
                        dumpServiceStats(pw, String("        "), String("          "), String("    "), "Running", svc,
                                svc->mRunCount, ServiceState::SERVICE_RUN, svc->mRunState,
                                svc->mRunStartTime, now, totalTime, !dumpSummary || dumpAll);
                        dumpServiceStats(pw, String("        "), String("          "), String("    "), "Started", svc,
                                svc->mStartedCount, ServiceState::SERVICE_STARTED, svc->mStartedState,
                                svc->mStartedStartTime, now, totalTime, !dumpSummary || dumpAll);
                        dumpServiceStats(pw, String("        "), String("          "), String("      "), "Bound", svc,
                                svc->mBoundCount, ServiceState::SERVICE_BOUND, svc->mBoundState,
                                svc->mBoundStartTime, now, totalTime, !dumpSummary || dumpAll);
                        dumpServiceStats(pw, String("        "), String("          "), String("  "), "Executing", svc,
                                svc->mExecCount, ServiceState::SERVICE_EXEC, svc->mExecState,
                                svc->mExecStartTime, now, totalTime, !dumpSummary || dumpAll);
                        if (dumpAll) {
                            if (svc->mOwner != NULL) {
                                pw->Print(String("        mOwner=");
                                    pw->Println(svc->mOwner));
                            }
                            if (svc->mStarted || svc->mRestarting) {
                                pw->Print(String("        mStarted=");
                                    pw->Print(svc->mStarted));
                                pw->Print(String(" mRestarting=");
                                    pw->Println(svc->mRestarting));
                            }
                        }
                    }
                }
            }
        }

        ArrayMap<String, SparseArray<ProcessState>> procMap = mProcesses->GetMap();
        printedHeader = FALSE;
        Int32 numShownProcs = 0, numTotalProcs = 0;
        for (Int32 ip=0; ip<procMap.size(); ip++) {
            String procName = procMap.keyAt(ip);
            SparseArray<ProcessState> uids = procMap.valueAt(ip);
            for (Int32 iu=0; iu<uids.size(); iu++) {
                Int32 uid = uids.keyAt(iu);
                numTotalProcs++;
                ProcessState proc = uids.valueAt(iu);
                if (proc->mDurationsTableSize == 0 && proc->mCurState == STATE_NOTHING
                        && proc->mPssTableSize == 0) {
                    continue;
                }
                if (!proc->mMultiPackage) {
                    continue;
                }
                if (reqPackage != NULL && !reqPackage.equals(procName)
                        && !reqPackage.equals(proc->mPackage)) {
                    continue;
                }
                numShownProcs++;
                if (sepNeeded) {
                    pw->Println();
                }
                sepNeeded = TRUE;
                if (!printedHeader) {
                    pw->Println(String("Multi-Package Common Processes:"));
                    printedHeader = TRUE;
                }
                if (activeOnly && !proc->isInUse()) {
                    pw->Print(String("      (Not active: ");
                        pw->Print(procName);
                    pw->Println(String(")")));
                    continue;
                }
                pw->Print(String("  * ");
                    pw->Print(procName);
                pw->Print(String(" / "));
                        UserHandle::FormatUid(pw, uid);
                        pw->Print(String(" (");
                            pw->Print(proc->mDurationsTableSize));
                        pw->Print(String(" entries)");
                        pw->Println(String(":")));
                dumpProcessState(pw, String("        "), proc, *ALL_SCREEN_ADJ, *ALL_MEM_ADJ,
                        *ALL_PROC_STATES, now);
                DumpProcessPss(pw, String("        "), proc, *ALL_SCREEN_ADJ, *ALL_MEM_ADJ,
                        *ALL_PROC_STATES);
                dumpProcessInternalLocked(pw, String("        "), proc, dumpAll);
            }
        }
        if (dumpAll) {
            pw->Println();
            pw->Print(String("  Total procs: ");
                pw->Print(numShownProcs));
                    pw->Print(String(" shown of ");
                        pw->Print(numTotalProcs);
                    pw->Println(String(" total")));
        }

        if (sepNeeded) {
            pw->Println();
        }
        if (dumpSummary) {
            pw->Println(String("Summary:"));
            dumpSummaryLocked(pw, reqPackage, now, activeOnly);
        }
        else {
            dumpTotalsLocked(pw, now);
        }

        if (dumpAll) {
            pw->Println();
            pw->Println(String("Internal state:"));
            pw->Print(String("  Num Int64 arrays: ");
                pw->Println(mLongs.size()));
            pw->Print(String("  Next Int64 entry: ");
                pw->Println(mNextLong));
            pw->Print(String("  mRunning=");
                pw->Println(mRunning));
        }
    }

    public static Int64 dumpSingleServiceTime(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix, ServiceState service,
            Int32 serviceType,
            /* [in] */ Int32 curState,
            /* [in] */ Int64 curStartTime,
            /* [in] */ Int64 now) {
        Int64 totalTime = 0;
        Int32 printedScreen = -1;
        for (Int32 iscreen=0; iscreen<ADJ_COUNT; iscreen+=ADJ_SCREEN_MOD) {
            Int32 printedMem = -1;
            for (Int32 imem=0; imem<ADJ_MEM_FACTOR_COUNT; imem++) {
                Int32 state = imem+iscreen;
                Int64 time = service.GetDuration(serviceType, curState, curStartTime,
                        state, now);
                String running("");
                if (curState == state && pw != NULL) {
                    running = " (running)";
                }
                if (time != 0) {
                    if (pw != NULL) {
                        pw->Print(prefix);
                        PrintScreenLabel(pw, printedScreen != iscreen
                                ? iscreen : STATE_NOTHING);
                        printedScreen = iscreen;
                        PrintMemLabel(pw, printedMem != imem ? imem : STATE_NOTHING, (char)0);
                        printedMem = imem;
                        pw->Print(String(": "));
                        TimeUtils::FormatDuration(time, pw);
                        pw->Println(running);
                    }
                    totalTime += time;
                }
            }
        }
        if (totalTime != 0 && pw != NULL) {
            pw->Print(prefix);
            pw->Print(String("    TOTAL: "));
            TimeUtils::FormatDuration(totalTime, pw);
            pw->Println();
        }
        return totalTime;
    }

    void dumpServiceStats(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ const String& prefixInner,
            String headerPrefix,
            /* [in] */ const String& header, ServiceState service,
            Int32 count,
            /* [in] */ Int32 serviceType,
            /* [in] */ Int32 state,
            /* [in] */ Int64 startTime,
            /* [in] */ Int64 now,
            /* [in] */ Int64 totalTime,
            Boolean dumpAll) {
        if (count != 0) {
            if (dumpAll) {
                pw->Print(prefix);
                pw->Print(header);
                pw->Print(String(" op count ");
                    pw->Print(count);
                pw->Println(String(":")));
                dumpSingleServiceTime(pw, prefixInner, service, serviceType, state, startTime,
                        now);
            }
            else {
                Int64 myTime = dumpSingleServiceTime(NULL, NULL, service, serviceType, state,
                        startTime, now);
                pw->Print(prefix);
                pw->Print(headerPrefix);
                pw->Print(header);
                pw->Print(String(" count ");
                    pw->Print(count));
                pw->Print(String(" / time "));
                PrintPercent(pw, (Double)myTime/(Double)totalTime);
                pw->Println();
            }
        }
    }

    CARAPI DumpSummaryLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& reqPackage,
        /* [in] */ Int64 now, Boolean activeOnly) {
        Int64 totalTime = DumpSingleTime(NULL, String(NULL), *mMemFactorDurations, mMemFactor,
                mStartTime, now);
        dumpFilteredSummaryLocked(pw, NULL, String("  "), *ALL_SCREEN_ADJ, *ALL_MEM_ADJ,
                *ALL_PROC_STATES, *NON_CACHED_PROC_STATES, now, totalTime, reqPackage, activeOnly);
        pw->Println();
        dumpTotalsLocked(pw, now);
    }

    Int64 printMemoryCategory(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ const String& label, Double memWeight,
            Int64 totalTime,
            /* [in] */ Int64 curTotalMem,
            /* [in] */ Int32 samples) {
        if (memWeight != 0) {
            Int64 mem = (Int64)(memWeight * 1024 / totalTime);
            pw->Print(prefix);
            pw->Print(label);
            pw->Print(String(": "));
            PrintSizeValue(pw, mem);
            pw->Print(String(" ("));
            pw->Print(samples);
            pw->Print(String(" samples)"));
            pw->Println();
            return curTotalMem + mem;
        }
        return curTotalMem;
    }

    void dumpTotalsLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int64 now) {
        pw->Println(String("Run time Stats:"));
        DumpSingleTime(pw, String("  "), *mMemFactorDurations, mMemFactor, mStartTime, now);
        pw->Println();
        pw->Println(String("Memory usage:"));
        TotalMemoryUseCollection totalMem = new TotalMemoryUseCollection(*ALL_SCREEN_ADJ,
                *ALL_MEM_ADJ);
        computeTotalMemoryUse(totalMem, now);
        Int64 totalPss = 0;
        totalPss = printMemoryCategory(pw, String("  "), "Kernel ", totalMem.sysMemKernelWeight,
                totalMem.totalTime, totalPss, totalMem.sysMemSamples);
        totalPss = printMemoryCategory(pw, String("  "), "Native ", totalMem.sysMemNativeWeight,
                totalMem.totalTime, totalPss, totalMem.sysMemSamples);
        for (Int32 i = 0; i < STATE_COUNT; i++) {
            // Skip restarting service state -- that is not actually a running process.
            if (i != STATE_SERVICE_RESTARTING) {
                totalPss = printMemoryCategory(pw, String("  "), (*STATE_NAMES)[i],
                        totalMem.processStateWeight[i], totalMem.totalTime, totalPss,
                        totalMem.processStateSamples[i]);
            }
        }
        totalPss = printMemoryCategory(pw, String("  "), "Cached ", totalMem.sysMemCachedWeight,
                totalMem.totalTime, totalPss, totalMem.sysMemSamples);
        totalPss = printMemoryCategory(pw, String("  "), "Free   ", totalMem.sysMemFreeWeight,
                totalMem.totalTime, totalPss, totalMem.sysMemSamples);
        totalPss = printMemoryCategory(pw, String("  "), "Z-Ram   ", totalMem.sysMemZRamWeight,
                totalMem.totalTime, totalPss, totalMem.sysMemSamples);
        pw->Print(String("  TOTAL  : "));
        PrintSizeValue(pw, totalPss);
        pw->Println();
        printMemoryCategory(pw, String("  "), (*STATE_NAMES)[STATE_SERVICE_RESTARTING],
                totalMem.processStateWeight[STATE_SERVICE_RESTARTING], totalMem.totalTime, totalPss,
                totalMem.processStateSamples[STATE_SERVICE_RESTARTING]);
        pw->Println();
        pw->Print(String("          Start time: "));
        pw->Print(DateFormat.format("yyyy-MM-dd HH:mm:ss", mTimePeriodStartClock));
        pw->Println();
        pw->Print(String("  Total elapsed time: "));
        TimeUtils::FormatDuration(
                (mRunning ? SystemClock::GetElapsedRealtime() : mTimePeriodEndRealtime)
                        - mTimePeriodStartRealtime, pw);
        Boolean partial = TRUE;
        if ((mFlags&FLAG_SHUTDOWN) != 0) {
            pw->Print(String(" (shutdown)"));
            partial = FALSE;
        }
        if ((mFlags&FLAG_SYSPROPS) != 0) {
            pw->Print(String(" (sysprops)"));
            partial = FALSE;
        }
        if ((mFlags&FLAG_COMPLETE) != 0) {
            pw->Print(String(" (complete)"));
            partial = FALSE;
        }
        if (partial) {
            pw->Print(String(" (partial)"));
        }
        pw->PrintChar(' ');
        pw->Print(mRuntime);
        pw->Println();
    }

    void dumpFilteredSummaryLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& header,
        /* [in] */ const String& prefix,
        /* [in] */ ArrayOf<Int32>* screenStates,
        /* [in] */ ArrayOf<Int32>* memStates,
        /* [in] */ ArrayOf<Int32>* procStates,
        /* [in] */ ArrayOf<Int32>* sortProcStates,
        /* [in] */ Int64 now,
        /* [in] */ Int64 totalTime,
        /* [in] */ const String& reqPackage,
        /* [in] */ Boolean activeOnly)
    {
        ArrayList<ProcessState> procs = collectProcessesLocked(screenStates, memStates,
                procStates, sortProcStates, now, reqPackage, activeOnly);
        if (procs.size() > 0) {
            if (header != NULL) {
                pw->Println();
                pw->Println(header);
            }
            DumpProcessSummaryLocked(pw, prefix, procs, screenStates, memStates,
                    sortProcStates, TRUE, now, totalTime);
        }
    }

    public ArrayList<ProcessState> collectProcessesLocked(Int32[] screenStates,
        /* [in] */ const ArrayOf<Int32>& memStates,
            Int32[] procStates,
            /* [in] */ Int32 sortProcStates[],
            /* [in] */ Int64 now,
            /* [in] */ const String& reqPackage,
            Boolean activeOnly) {
        final ArraySet<ProcessState> foundProcs = new ArraySet<ProcessState>();
        final ArrayMap<String, SparseArray<SparseArray<PackageState>>> pkgMap = mPackages->GetMap();
        for (Int32 ip=0; ip<pkgMap.size(); ip++) {
            final String pkgName = pkgMap.keyAt(ip);
            final SparseArray<SparseArray<PackageState>> procs = pkgMap.valueAt(ip);
            for (Int32 iu=0; iu<procs.size(); iu++) {
                final SparseArray<PackageState> vpkgs = procs.valueAt(iu);
                final Int32 NVERS = vpkgs.size();
                for (Int32 iv=0; iv<NVERS; iv++) {
                    final PackageState state = vpkgs.valueAt(iv);
                    final Int32 NPROCS = state.mProcesses->Size();
                    final Boolean pkgMatch = reqPackage == NULL || reqPackage.equals(pkgName);
                    for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                        final ProcessState proc = state.mProcesses->ValueAt(iproc);
                        if (!pkgMatch && !reqPackage.equals(proc->mName)) {
                            continue;
                        }
                        if (activeOnly && !proc->isInUse()) {
                            continue;
                        }
                        foundProcs->Add(proc->mCommonProcess);
                    }
                }
            }
        }
        ArrayList<ProcessState> outProcs = new ArrayList<ProcessState>(foundProcs.size());
        for (Int32 i = 0; i < foundProcs.size(); i++) {
            ProcessState proc = foundProcs.valueAt(i);
            if (ComputeProcessTimeLocked(proc, screenStates, memStates, procStates, now) > 0) {
                outProcs->Add(proc);
                if (procStates != sortProcStates) {
                    ComputeProcessTimeLocked(proc, screenStates, memStates, sortProcStates, now);
                }
            }
        }
        Collections.sort(outProcs, new Comparator<ProcessState>() {
            @Override
            public Int32 compare(ProcessState lhs,
            /* [in] */ IProcessState* _rhs) {
                if (lhs.mTmpTotalTime < rhs.mTmpTotalTime) {
                    return -1;
                }
                else if (lhs.mTmpTotalTime > rhs.mTmpTotalTime) {
                    return 1;
                }
                return 0;
            }
        });
        return outProcs;
    }

    String collapseString(String pkgName,
        /* [in] */ const String& itemName) {
        if (itemName.startsWith(pkgName)) {
            final Int32 ITEMLEN = itemName.GetLength()();
            final Int32 PKGLEN = pkgName.GetLength()();
            if (ITEMLEN == PKGLEN) {
                return "";
            }
            else if (ITEMLEN >= PKGLEN) {
                if (itemName.charAt(PKGLEN) == '.') {
                    return itemName.substring(PKGLEN);
                }
            }
        }
        return itemName;
    }

    CARAPI DumpCheckinLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& reqPackage) {
        final Int64 now = SystemClock::GetUptimeMillis();
        final ArrayMap<String, SparseArray<SparseArray<PackageState>>> pkgMap = mPackages->GetMap();
        pw->Println(String("vers,5"));
        pw->Print(String("period,");
            pw->Print(mTimePeriodStartClockStr));
        pw->Print(String(",");
            pw->Print(mTimePeriodStartRealtime);
        pw->Print(String(","));
        pw->Print(mRunning ? SystemClock::GetElapsedRealtime() : mTimePeriodEndRealtime);
        Boolean partial = TRUE;
        if ((mFlags&FLAG_SHUTDOWN) != 0) {
            pw->Print(String(",shutdown"));
            partial = FALSE;
        }
        if ((mFlags&FLAG_SYSPROPS) != 0) {
            pw->Print(String(",sysprops"));
            partial = FALSE;
        }
        if ((mFlags&FLAG_COMPLETE) != 0) {
            pw->Print(String(",complete"));
            partial = FALSE;
        }
        if (partial) {
            pw->Print(String(",partial"));
        }
        pw->Println();
        pw->Print(String("config,");
            pw->Println(mRuntime));
        for (Int32 ip=0; ip<pkgMap.size(); ip++) {
            final String pkgName = pkgMap.keyAt(ip);
            if (reqPackage != NULL && !reqPackage.equals(pkgName)) {
                continue;
            }
            final SparseArray<SparseArray<PackageState>> uids = pkgMap.valueAt(ip);
            for (Int32 iu=0; iu<uids.size(); iu++) {
                final Int32 uid = uids.keyAt(iu);
                final SparseArray<PackageState> vpkgs = uids.valueAt(iu);
                for (Int32 iv=0; iv<vpkgs.size(); iv++) {
                    final Int32 vers = vpkgs.keyAt(iv);
                    final PackageState pkgState = vpkgs.valueAt(iv);
                    final Int32 NPROCS = pkgState.mProcesses->Size();
                    final Int32 NSRVS = pkgState.mServices->Size();
                    for (Int32 iproc=0; iproc<NPROCS; iproc++) {
                        ProcessState proc = pkgState.mProcesses->ValueAt(iproc);
                        pw->Print(String("pkgproc,"));
                        pw->Print(pkgName);
                        pw->Print(String(","));
                        pw->Print(uid);
                        pw->Print(String(","));
                        pw->Print(vers);
                        pw->Print(String(","));
                        pw->Print(collapseString(pkgName, pkgState.mProcesses->KeyAt(iproc)));
                        DumpAllProcessStateCheckin(pw, proc, now);
                        pw->Println();
                        if (proc->mPssTableSize > 0) {
                            pw->Print(String("pkgpss,"));
                            pw->Print(pkgName);
                            pw->Print(String(","));
                            pw->Print(uid);
                            pw->Print(String(","));
                            pw->Print(vers);
                            pw->Print(String(","));
                            pw->Print(collapseString(pkgName, pkgState.mProcesses->KeyAt(iproc)));
                            DumpAllProcessPssCheckin(pw, proc);
                            pw->Println();
                        }
                        if (proc->mNumExcessiveWake > 0 || proc->mNumExcessiveCpu > 0
                                || proc->mNumCachedKill > 0) {
                            pw->Print(String("pkgkills,"));
                            pw->Print(pkgName);
                            pw->Print(String(","));
                            pw->Print(uid);
                            pw->Print(String(","));
                            pw->Print(vers);
                            pw->Print(String(","));
                            pw->Print(collapseString(pkgName, pkgState.mProcesses->KeyAt(iproc)));
                            pw->Print(String(","));
                            pw->Print(proc->mNumExcessiveWake);
                            pw->Print(String(","));
                            pw->Print(proc->mNumExcessiveCpu);
                            pw->Print(String(","));
                            pw->Print(proc->mNumCachedKill);
                            pw->Print(String(","));
                            pw->Print(proc->mMinCachedKillPss);
                            pw->Print(String(":"));
                            pw->Print(proc->mAvgCachedKillPss);
                            pw->Print(String(":"));
                            pw->Print(proc->mMaxCachedKillPss);
                            pw->Println();
                        }
                    }
                    for (Int32 isvc=0; isvc<NSRVS; isvc++) {
                        String serviceName = collapseString(pkgName,
                                pkgState.mServices->KeyAt(isvc));
                        ServiceState svc = pkgState.mServices->ValueAt(isvc);
                        dumpServiceTimeCheckin(pw, "pkgsvc-run", pkgName, uid, vers, serviceName,
                                svc, ServiceState::SERVICE_RUN, svc->mRunCount,
                                svc->mRunState, svc->mRunStartTime, now);
                        dumpServiceTimeCheckin(pw, "pkgsvc-start", pkgName, uid, vers, serviceName,
                                svc, ServiceState::SERVICE_STARTED, svc->mStartedCount,
                                svc->mStartedState, svc->mStartedStartTime, now);
                        dumpServiceTimeCheckin(pw, "pkgsvc-bound", pkgName, uid, vers, serviceName,
                                svc, ServiceState::SERVICE_BOUND, svc->mBoundCount,
                                svc->mBoundState, svc->mBoundStartTime, now);
                        dumpServiceTimeCheckin(pw, "pkgsvc-exec", pkgName, uid, vers, serviceName,
                                svc, ServiceState::SERVICE_EXEC, svc->mExecCount,
                                svc->mExecState, svc->mExecStartTime, now);
                    }
                }
            }
        }

        ArrayMap<String, SparseArray<ProcessState>> procMap = mProcesses->GetMap();
        for (Int32 ip=0; ip<procMap.size(); ip++) {
            String procName = procMap.keyAt(ip);
            SparseArray<ProcessState> uids = procMap.valueAt(ip);
            for (Int32 iu=0; iu<uids.size(); iu++) {
                Int32 uid = uids.keyAt(iu);
                ProcessState procState = uids.valueAt(iu);
                if (procState.mDurationsTableSize > 0) {
                    pw->Print(String("proc,"));
                    pw->Print(procName);
                    pw->Print(String(","));
                    pw->Print(uid);
                    DumpAllProcessStateCheckin(pw, procState, now);
                    pw->Println();
                }
                if (procState.mPssTableSize > 0) {
                    pw->Print(String("pss,"));
                    pw->Print(procName);
                    pw->Print(String(","));
                    pw->Print(uid);
                    DumpAllProcessPssCheckin(pw, procState);
                    pw->Println();
                }
                if (procState.mNumExcessiveWake > 0 || procState.mNumExcessiveCpu > 0
                        || procState.mNumCachedKill > 0) {
                    pw->Print(String("kills,"));
                    pw->Print(procName);
                    pw->Print(String(","));
                    pw->Print(uid);
                    pw->Print(String(","));
                    pw->Print(procState.mNumExcessiveWake);
                    pw->Print(String(","));
                    pw->Print(procState.mNumExcessiveCpu);
                    pw->Print(String(","));
                    pw->Print(procState.mNumCachedKill);
                    pw->Print(String(","));
                    pw->Print(procState.mMinCachedKillPss);
                    pw->Print(String(":"));
                    pw->Print(procState.mAvgCachedKillPss);
                    pw->Print(String(":"));
                    pw->Print(procState.mMaxCachedKillPss);
                    pw->Println();
                }
            }
        }
        pw->Print(String("total"));
        DumpAdjTimesCheckin(pw, String(","), *mMemFactorDurations, mMemFactor,
                mStartTime, now);
        if (mSysMemUsageTable != NULL) {
            pw->Print(String("sysmemusage"));
            for (Int32 i = 0; i < mSysMemUsageTableSize; i++) {
                Int32 off = (*mSysMemUsageTable)[i];
                Int32 type = (off >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                pw->Print(String(","));
                PrintProcStateTag(pw, type);
                for (Int32 j=SYS_MEM_USAGE_SAMPLE_COUNT; j<SYS_MEM_USAGE_COUNT; j++) {
                    if (j > SYS_MEM_USAGE_CACHED_MINIMUM) {
                        pw->Print(String(":"));
                    }
                    pw->Print(GetInt64(off, j));
                }
            }
        }
        pw->Println();
        TotalMemoryUseCollection totalMem = new TotalMemoryUseCollection(*ALL_SCREEN_ADJ,
                *ALL_MEM_ADJ);
        computeTotalMemoryUse(totalMem, now);
        pw->Print(String("weights,"));
        pw->Print(totalMem.totalTime);
        pw->Print(String(","));
        pw->Print(totalMem.sysMemCachedWeight);
        pw->Print(String(":"));
        pw->Print(totalMem.sysMemSamples);
        pw->Print(String(","));
        pw->Print(totalMem.sysMemFreeWeight);
        pw->Print(String(":"));
        pw->Print(totalMem.sysMemSamples);
        pw->Print(String(","));
        pw->Print(totalMem.sysMemZRamWeight);
        pw->Print(String(":"));
        pw->Print(totalMem.sysMemSamples);
        pw->Print(String(","));
        pw->Print(totalMem.sysMemKernelWeight);
        pw->Print(String(":"));
        pw->Print(totalMem.sysMemSamples);
        pw->Print(String(","));
        pw->Print(totalMem.sysMemNativeWeight);
        pw->Print(String(":"));
        pw->Print(totalMem.sysMemSamples);
        for (Int32 i = 0; i < STATE_COUNT; i++) {
            pw->Print(String(","));
            pw->Print(totalMem.processStateWeight[i]);
            pw->Print(String(":"));
            pw->Print(totalMem.processStateSamples[i]);
        }
        pw->Println();
    }

    public static class DurationsTable {
        public final ProcessStats mStats;
        public final String mName;
        public Int32[] mDurationsTable;
        public Int32 mDurationsTableSize;

        public DurationsTable(
            /* [in] */ IProcessStats* stats,
            /* [in] */ const String& name) {
            mStats = stats;
            mName = name;
        }

        void copyDurationsTo(DurationsTable other) {
            if (mDurationsTable != NULL) {
                mStats->mAddLongTable = new Int32[mDurationsTable.GetLength()];
                mStats->mAddLongTableSize = 0;
                for (Int32 i = 0; i < mDurationsTableSize; i++) {
                    Int32 origEnt = mDurationsTable[i];
                    Int32 type = (origEnt >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                    Int32 newOff = mStats->AddLongData(i, type, 1);
                    (*mStats->mAddLongTable)[i] = newOff | type;
                    mStats->setLong(newOff, 0, mStats->GetInt64(origEnt, 0));
                }
                other->mDurationsTable = mStats->mAddLongTable;
                other->mDurationsTableSize = mStats->mAddLongTableSize;
            }
            else {
                other->mDurationsTable = NULL;
                other->mDurationsTableSize = 0;
            }
        }

        void addDurations(DurationsTable other) {
            for (Int32 i = 0; i < other->mDurationsTableSize; i++) {
                Int32 ent = other->mDurationsTable[i];
                Int32 state = (ent >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                if (DEBUG) Slogger::D(TAG, "Adding state " + state + " duration "
                        + other->mStats->GetInt64(ent, 0));
                addDuration(state, other->mStats->GetInt64(ent, 0));
            }
        }

        void ResetDurationsSafely() {
            mDurationsTable = NULL;
            mDurationsTableSize = 0;
        }

        void writeDurationsToParcel(Parcel out) {
            out.writeInt(mDurationsTableSize);
            for (Int32 i = 0; i < mDurationsTableSize; i++) {
                if (DEBUG_PARCEL) Slogger::I(TAG, "Writing in " + mName + " dur #" + i + ": "
                        + printLongOffset(mDurationsTable[i]));
                out.writeInt(mDurationsTable[i]);
            }
        }

        Boolean readDurationsFromParcel(Parcel in) {
            mDurationsTable = mStats->readTableFromParcel(in, mName, "durations");
            if (mDurationsTable == BAD_TABLE) {
                return FALSE;
            }
            mDurationsTableSize = mDurationsTable != NULL ? mDurationsTable.GetLength() : 0;
            return TRUE;
        }

        void addDuration(Int32 state,
            /* [in] */ Int64 dur) {
            Int32 idx = BinarySearch(mDurationsTable, mDurationsTableSize, state);
            Int32 off;
            if (idx >= 0) {
                off = mDurationsTable[idx];
            }
            else {
                mStats->mAddLongTable = mDurationsTable;
                mStats->mAddLongTableSize = mDurationsTableSize;
                off = mStats->AddLongData(~idx, state, 1);
                mDurationsTable = mStats->mAddLongTable;
                mDurationsTableSize = mStats->mAddLongTableSize;
            }
            AutoPtr<ArrayOf<Int64> > longs = mStats->mLongs[(off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
            if (DEBUG) Slogger::D(TAG, "Duration of " + mName + " state " + state + " inc by " + dur
                    + " from " + longs[(off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK]);
            longs[(off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK] += dur;
        }

        Int64 GetDuration(Int32 state,
            /* [in] */ Int64 now) {
            Int32 idx = BinarySearch(mDurationsTable, mDurationsTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mDurationsTable[idx], 0) : 0;
        }
    }

    final public static class ProcessStateHolder {
        public final Int32 appVersion;
        public ProcessStats.ProcessState state;

        public ProcessStateHolder(Int32 _appVersion) {
            appVersion = _appVersion;
        }
    }

    public static final class ProcessState extends DurationsTable {
        public ProcessState mCommonProcess;
        public final String mPackage;
        public final Int32 mUid;
        public final Int32 mVersion;

        //final AutoPtr<ArrayOf<Int64> > mDurations = new Int64[STATE_COUNT*ADJ_COUNT];
        Int32 mCurState = STATE_NOTHING;
        Int64 mStartTime;

        Int32 mLastPssState = STATE_NOTHING;
        Int64 mLastPssTime;
        Int32[] mPssTable;
        Int32 mPssTableSize;

        Boolean mActive;
        Int32 mNumActiveServices;
        Int32 mNumStartedServices;

        Int32 mNumExcessiveWake;
        Int32 mNumExcessiveCpu;

        Int32 mNumCachedKill;
        Int64 mMinCachedKillPss;
        Int64 mAvgCachedKillPss;
        Int64 mMaxCachedKillPss;

        Boolean mMultiPackage;
        Boolean mDead;

        public Int64 mTmpTotalTime;
        Int32 mTmpNumInUse;
        ProcessState mTmpFoundSubProc;

        /**
         * Create a new top-level process state, for the initial case where there is only
         * a single package running in a process.  The initial state is not running.
         */
        public ProcessState(
            /* [in] */ IProcessStats* processStats,
            /* [in] */ const String& pkg,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 vers,
            /* [in] */ const String& name) {
            super(processStats, name);
            mCommonProcess = this;
            mPackage = pkg;
            mUid = uid;
            mVersion = vers;
        }

        /**
         * Create a new per-package process state for an existing top-level process
         * state.  The current running state of the top-level process is also copied,
         * marked as started running at 'now'.
         */
        public ProcessState(ProcessState commonProcess,
            /* [in] */ const String& pkg,
            /* [in] */ Int32 uid,
        /* [in] */ Int32 vers,
        /* [in] */ const String& name,
                Int64 now) {
            super(commonProcess.mStats, name);
            mCommonProcess = commonProcess;
            mPackage = pkg;
            mUid = uid;
            mVersion = vers;
            mCurState = commonProcess.mCurState;
            mStartTime = now;
        }

        ProcessState clone(String pkg,
            /* [in] */ Int64 now) {
            ProcessState pnew = new ProcessState(this, pkg, mUid, mVersion, mName, now);
            copyDurationsTo(pnew);
            if (mPssTable != NULL) {
                mStats->mAddLongTable = new Int32[mPssTable.GetLength()];
                mStats->mAddLongTableSize = 0;
                for (Int32 i = 0; i < mPssTableSize; i++) {
                    Int32 origEnt = mPssTable[i];
                    Int32 type = (origEnt >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                    Int32 newOff = mStats->AddLongData(i, type, PSS_COUNT);
                    (*mStats->mAddLongTable)[i] = newOff | type;
                    for (Int32 j=0; j<PSS_COUNT; j++) {
                        mStats->setLong(newOff, j, mStats->GetInt64(origEnt, j));
                    }
                }
                pnew.mPssTable = mStats->mAddLongTable;
                pnew.mPssTableSize = mStats->mAddLongTableSize;
            }
            pnew.mNumExcessiveWake = mNumExcessiveWake;
            pnew.mNumExcessiveCpu = mNumExcessiveCpu;
            pnew.mNumCachedKill = mNumCachedKill;
            pnew.mMinCachedKillPss = mMinCachedKillPss;
            pnew.mAvgCachedKillPss = mAvgCachedKillPss;
            pnew.mMaxCachedKillPss = mMaxCachedKillPss;
            pnew.mActive = mActive;
            pnew.mNumActiveServices = mNumActiveServices;
            pnew.mNumStartedServices = mNumStartedServices;
            return pnew;
        }

        void add(ProcessState other) {
            addDurations(other);
            for (Int32 i = 0; i < other->mPssTableSize; i++) {
                Int32 ent = other->mPssTable[i];
                Int32 state = (ent >> OFFSET_TYPE_SHIFT) & OFFSET_TYPE_MASK;
                addPss(state, (Int32) other->mStats->GetInt64(ent, PSS_SAMPLE_COUNT),
                        other->mStats->GetInt64(ent, PSS_MINIMUM),
                        other->mStats->GetInt64(ent, PSS_AVERAGE),
                        other->mStats->GetInt64(ent, PSS_MAXIMUM),
                        other->mStats->GetInt64(ent, PSS_USS_MINIMUM),
                        other->mStats->GetInt64(ent, PSS_USS_AVERAGE),
                        other->mStats->GetInt64(ent, PSS_USS_MAXIMUM));
            }
            mNumExcessiveWake += other->mNumExcessiveWake;
            mNumExcessiveCpu += other->mNumExcessiveCpu;
            if (other->mNumCachedKill > 0) {
                addCachedKill(other->mNumCachedKill, other->mMinCachedKillPss,
                        other->mAvgCachedKillPss, other->mMaxCachedKillPss);
            }
        }

        void ResetSafely(
            /* [in] */ Int64 now) {
            ResetDurationsSafely();
            mStartTime = now;
            mLastPssState = STATE_NOTHING;
            mLastPssTime = 0;
            mPssTable = NULL;
            mPssTableSize = 0;
            mNumExcessiveWake = 0;
            mNumExcessiveCpu = 0;
            mNumCachedKill = 0;
            mMinCachedKillPss = mAvgCachedKillPss = mMaxCachedKillPss = 0;
        }

        void MakeDead() {
            mDead = TRUE;
        }

        private void ensureNotDead() {
            if (!mDead) {
                return;
            }
            Slogger::WtfStack(TAG, "ProcessState dead: name=" + mName
                    + " pkg=" + mPackage + " uid=" + mUid + " common.name=" + mCommonProcess.mName);
        }

        void writeToParcel(Parcel out,
            /* [in] */ Int64 now) {
            out.writeInt(mMultiPackage ? 1 : 0);
            writeDurationsToParcel(out);
            out.writeInt(mPssTableSize);
            for (Int32 i = 0; i < mPssTableSize; i++) {
                if (DEBUG_PARCEL) Slogger::I(TAG, "Writing in " + mName + " pss #" + i + ": "
                        + printLongOffset(mPssTable[i]));
                out.writeInt(mPssTable[i]);
            }
            out.writeInt(mNumExcessiveWake);
            out.writeInt(mNumExcessiveCpu);
            out.writeInt(mNumCachedKill);
            if (mNumCachedKill > 0) {
                out.writeLong(mMinCachedKillPss);
                out.writeLong(mAvgCachedKillPss);
                out.writeLong(mMaxCachedKillPss);
            }
        }

        Boolean readFromParcel(Parcel in, Boolean fully) {
            Boolean multiPackage = in.readInt() != 0;
            if (fully) {
                mMultiPackage = multiPackage;
            }
            if (DEBUG_PARCEL) Slogger::D(TAG, "Reading durations table...");
            if (!readDurationsFromParcel(in)) {
                return FALSE;
            }
            if (DEBUG_PARCEL) Slogger::D(TAG, "Reading pss table...");
            mPssTable = mStats->readTableFromParcel(in, mName, "pss");
            if (mPssTable == BAD_TABLE) {
                return FALSE;
            }
            mPssTableSize = mPssTable != NULL ? mPssTable.GetLength() : 0;
            mNumExcessiveWake = in.readInt();
            mNumExcessiveCpu = in.readInt();
            mNumCachedKill = in.readInt();
            if (mNumCachedKill > 0) {
                mMinCachedKillPss = in.readLong();
                mAvgCachedKillPss = in.readLong();
                mMaxCachedKillPss = in.readLong();
            }
            else {
                mMinCachedKillPss = mAvgCachedKillPss = mMaxCachedKillPss = 0;
            }
            return TRUE;
        }

        CARAPI MakeActive() {
            ensureNotDead();
            mActive = TRUE;
        }

        CARAPI MakeInactive() {
            mActive = FALSE;
        }

        public Boolean isInUse() {
            return mActive || mNumActiveServices > 0 || mNumStartedServices > 0
                    || mCurState != STATE_NOTHING;
        }

        /**
         * Update the current state of the given list of processes.
         *
         * @param state Current ActivityManager.PROCESS_STATE_*
         * @param memFactor Current mem factor constant.
         * @param now Current time.
         * @param pkgList Processes to update.
         */
        CARAPI SetState(
            /* [in] */ Int32 state,
            /* [in] */ Int32 memFactor,
            /* [in] */ Int64 now,
                ArrayMap<String, ProcessStateHolder> pkgList) {
            if (state < 0) {
                state = mNumStartedServices > 0
                        ? (STATE_SERVICE_RESTARTING+(memFactor*STATE_COUNT)) : STATE_NOTHING;
            }
            else {
                state = (*PROCESS_STATE_TO_STATE)[state] + (memFactor*STATE_COUNT);
            }

            // First update the common process.
            mCommonProcess.setState(state, now);

            // If the common process is not multi-package, there is nothing else to do.
            if (!mCommonProcess.mMultiPackage) {
                return;
            }

            if (pkgList != NULL) {
                for (Int32 ip=pkgList.size()-1; ip>=0; ip--) {
                    pullFixedProc(pkgList, ip).setState(state, now);
                }
            }
        }

        void setState(
            /* [in] */ Int32 state,
            /* [in] */ Int64 now) {
            ensureNotDead();
            if (mCurState != state) {
                //Slogger::I(TAG, "Setting state in " + mName + "/" + mPackage + ": " + state);
                commitStateTime(now);
                mCurState = state;
            }
        }

        void commitStateTime(
            /* [in] */ Int64 now) {
            if (mCurState != STATE_NOTHING) {
                Int64 dur = now - mStartTime;
                if (dur > 0) {
                    addDuration(mCurState, dur);
                }
            }
            mStartTime = now;
        }

        void incActiveServices(String serviceName) {
            if (DEBUG && "".equals(mName)) {
                RuntimeException here = new RuntimeException("here");
                here.fillInStackTrace();
                Slogger::D(TAG, "incActiveServices: " + this + " service=" + serviceName
                        + " to " + (mNumActiveServices+1), here);
            }
            if (mCommonProcess != this) {
                mCommonProcess.incActiveServices(serviceName);
            }
            mNumActiveServices++;
        }

        void decActiveServices(String serviceName) {
            if (DEBUG && "".equals(mName)) {
                RuntimeException here = new RuntimeException("here");
                here.fillInStackTrace();
                Slogger::D(TAG, "decActiveServices: " + this + " service=" + serviceName
                        + " to " + (mNumActiveServices-1), here);
            }
            if (mCommonProcess != this) {
                mCommonProcess.decActiveServices(serviceName);
            }
            mNumActiveServices--;
            if (mNumActiveServices < 0) {
                Slogger::WtfStack(TAG, "Proc active services underrun: pkg=" + mPackage
                        + " uid=" + mUid + " proc=" + mName + " service=" + serviceName);
                mNumActiveServices = 0;
            }
        }

        void incStartedServices(Int32 memFactor,
            /* [in] */ Int64 now,
            /* [in] */ const String& serviceName) {
            if (FALSE) {
                RuntimeException here = new RuntimeException("here");
                here.fillInStackTrace();
                Slogger::D(TAG, "incStartedServices: " + this + " service=" + serviceName
                        + " to " + (mNumStartedServices+1), here);
            }
            if (mCommonProcess != this) {
                mCommonProcess.incStartedServices(memFactor, now, serviceName);
            }
            mNumStartedServices++;
            if (mNumStartedServices == 1 && mCurState == STATE_NOTHING) {
                setState(STATE_SERVICE_RESTARTING + (memFactor*STATE_COUNT), now);
            }
        }

        void decStartedServices(Int32 memFactor,
            /* [in] */ Int64 now,
            /* [in] */ const String& serviceName) {
            if (FALSE) {
                RuntimeException here = new RuntimeException("here");
                here.fillInStackTrace();
                Slogger::D(TAG, "decActiveServices: " + this + " service=" + serviceName
                        + " to " + (mNumStartedServices-1), here);
            }
            if (mCommonProcess != this) {
                mCommonProcess.decStartedServices(memFactor, now, serviceName);
            }
            mNumStartedServices--;
            if (mNumStartedServices == 0 && (mCurState%STATE_COUNT) == STATE_SERVICE_RESTARTING) {
                setState(STATE_NOTHING, now);
            }
            else if (mNumStartedServices < 0) {
                Slogger::WtfStack(TAG, "Proc started services underrun: pkg="
                        + mPackage + " uid=" + mUid + " name=" + mName);
                mNumStartedServices = 0;
            }
        }

        CARAPI AddPss(
            /* [in] */ Int64 pss,
            /* [in] */ Int64 uss, Boolean always,
                ArrayMap<String, ProcessStateHolder> pkgList) {
            ensureNotDead();
            if (!always) {
                if (mLastPssState == mCurState && SystemClock::GetUptimeMillis()
                        < (mLastPssTime+(30*1000))) {
                    return;
                }
            }
            mLastPssState = mCurState;
            mLastPssTime = SystemClock::GetUptimeMillis();
            if (mCurState != STATE_NOTHING) {
                // First update the common process.
                mCommonProcess->AddPss(mCurState, 1, pss, pss, pss, uss, uss, uss);

                // If the common process is not multi-package, there is nothing else to do.
                if (!mCommonProcess.mMultiPackage) {
                    return;
                }

                if (pkgList != NULL) {
                    for (Int32 ip=pkgList.size()-1; ip>=0; ip--) {
                        pullFixedProc(pkgList, ip)->AddPss(mCurState, 1,
                                pss, pss, pss, uss, uss, uss);
                    }
                }
            }
        }

        void addPss(Int32 state,
            /* [in] */ Int32 inCount,
            /* [in] */ Int64 minPss,
        /* [in] */ Int64 avgPss,
        /* [in] */ Int64 maxPss,
        /* [in] */ Int64 minUss,
                Int64 avgUss,
                /* [in] */ Int64 maxUss) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            Int32 off;
            if (idx >= 0) {
                off = mPssTable[idx];
            }
            else {
                mStats->mAddLongTable = mPssTable;
                mStats->mAddLongTableSize = mPssTableSize;
                off = mStats->AddLongData(~idx, state, PSS_COUNT);
                mPssTable = mStats->mAddLongTable;
                mPssTableSize = mStats->mAddLongTableSize;
            }
            AutoPtr<ArrayOf<Int64> > longs = mStats->mLongs[(off >> OFFSET_ARRAY_SHIFT) & OFFSET_ARRAY_MASK];
            idx = (off >> OFFSET_INDEX_SHIFT) & OFFSET_INDEX_MASK;
            Int64 count = longs[idx+PSS_SAMPLE_COUNT];
            if (count == 0) {
                longs[idx+PSS_SAMPLE_COUNT] = inCount;
                longs[idx+PSS_MINIMUM] = minPss;
                longs[idx+PSS_AVERAGE] = avgPss;
                longs[idx+PSS_MAXIMUM] = maxPss;
                longs[idx+PSS_USS_MINIMUM] = minUss;
                longs[idx+PSS_USS_AVERAGE] = avgUss;
                longs[idx+PSS_USS_MAXIMUM] = maxUss;
            }
            else {
                longs[idx+PSS_SAMPLE_COUNT] = count+inCount;
                if (longs[idx+PSS_MINIMUM] > minPss) {
                    longs[idx+PSS_MINIMUM] = minPss;
                }
                longs[idx+PSS_AVERAGE] = (Int64)(
                        ((longs[idx+PSS_AVERAGE]*(Double)count)+(avgPss*(Double)inCount))
                                / (count+inCount) );
                if (longs[idx+PSS_MAXIMUM] < maxPss) {
                    longs[idx+PSS_MAXIMUM] = maxPss;
                }
                if (longs[idx+PSS_USS_MINIMUM] > minUss) {
                    longs[idx+PSS_USS_MINIMUM] = minUss;
                }
                longs[idx+PSS_USS_AVERAGE] = (Int64)(
                        ((longs[idx+PSS_USS_AVERAGE]*(Double)count)+(avgUss*(Double)inCount))
                                / (count+inCount) );
                if (longs[idx+PSS_USS_MAXIMUM] < maxUss) {
                    longs[idx+PSS_USS_MAXIMUM] = maxUss;
                }
            }
        }

        CARAPI ReportExcessiveWake(ArrayMap<String, ProcessStateHolder> pkgList) {
            ensureNotDead();
            mCommonProcess.mNumExcessiveWake++;
            if (!mCommonProcess.mMultiPackage) {
                return;
            }

            for (Int32 ip=pkgList.size()-1; ip>=0; ip--) {
                pullFixedProc(pkgList, ip).mNumExcessiveWake++;
            }
        }

        CARAPI ReportExcessiveCpu(ArrayMap<String, ProcessStateHolder> pkgList) {
            ensureNotDead();
            mCommonProcess.mNumExcessiveCpu++;
            if (!mCommonProcess.mMultiPackage) {
                return;
            }

            for (Int32 ip=pkgList.size()-1; ip>=0; ip--) {
                pullFixedProc(pkgList, ip).mNumExcessiveCpu++;
            }
        }

        private void addCachedKill(Int32 num,
            /* [in] */ Int64 minPss,
        /* [in] */ Int64 avgPss,
        /* [in] */ Int64 maxPss) {
            if (mNumCachedKill <= 0) {
                mNumCachedKill = num;
                mMinCachedKillPss = minPss;
                mAvgCachedKillPss = avgPss;
                mMaxCachedKillPss = maxPss;
            }
            else {
                if (minPss < mMinCachedKillPss) {
                    mMinCachedKillPss = minPss;
                }
                if (maxPss > mMaxCachedKillPss) {
                    mMaxCachedKillPss = maxPss;
                }
                mAvgCachedKillPss = (Int64)( ((mAvgCachedKillPss*(Double)mNumCachedKill) + avgPss)
                        / (mNumCachedKill+num) );
                mNumCachedKill += num;
            }
        }

        CARAPI ReportCachedKill(ArrayMap<String, ProcessStateHolder> pkgList,
            /* [in] */ Int64 pss) {
            ensureNotDead();
            mCommonProcess->AddCachedKill(1, pss, pss, pss);
            if (!mCommonProcess.mMultiPackage) {
                return;
            }

            for (Int32 ip=pkgList.size()-1; ip>=0; ip--) {
                pullFixedProc(pkgList, ip)->AddCachedKill(1, pss, pss, pss);
            }
        }

        ProcessState pullFixedProc(String pkgName) {
            if (mMultiPackage) {
                // The array map is still pointing to a common process state
                // that is now shared across packages.  Update it to point to
                // the new per-package state.
                SparseArray<PackageState> vpkg = mStats->mPackages->Get(pkgName, mUid);
                if (vpkg == NULL) {
                    throw new IllegalStateException("Didn't find package " + pkgName
                            + " / " + mUid);
                }
                PackageState pkg = vpkg.get(mVersion);
                if (pkg == NULL) {
                    throw new IllegalStateException("Didn't find package " + pkgName
                            + " / " + mUid + " vers " + mVersion);
                }
                ProcessState proc = pkg.mProcesses->Get(mName);
                if (proc == NULL) {
                    throw new IllegalStateException("Didn't create per-package process "
                            + mName + " in pkg " + pkgName + " / " + mUid + " vers " + mVersion);
                }
                return proc;
            }
            return this;
        }

        private ProcessState pullFixedProc(ArrayMap<String, ProcessStateHolder> pkgList,
                Int32 index) {
            ProcessStateHolder holder = pkgList.valueAt(index);
            ProcessState proc = holder.state;
            if (mDead && proc->mCommonProcess != proc) {
                // Somehow we are contining to use a process state that is dead, because
                // it was not being told it was active during the last commit.  We can recover
                // from this by generating a fresh new state, but this is bad because we
                // are losing whatever data we had in the old process state.
                Log.wtf(TAG, "Pulling dead proc: name=" + mName + " pkg=" + mPackage
                        + " uid=" + mUid + " common.name=" + mCommonProcess.mName);
                proc = mStats->GetProcessStateLocked(proc->mPackage, proc->mUid, proc->mVersion,
                        proc->mName);
            }
            if (proc->mMultiPackage) {
                // The array map is still pointing to a common process state
                // that is now shared across packages.  Update it to point to
                // the new per-package state.
                SparseArray<PackageState> vpkg = mStats->mPackages->Get(pkgList.keyAt(index),
                        proc->mUid);
                if (vpkg == NULL) {
                    throw new IllegalStateException("No existing package "
                            + pkgList.keyAt(index) + "/" + proc->mUid
                            + " for multi-proc " + proc->mName);
                }
                PackageState pkg = vpkg.get(proc->mVersion);
                if (pkg == NULL) {
                    throw new IllegalStateException("No existing package "
                            + pkgList.keyAt(index) + "/" + proc->mUid
                            + " for multi-proc " + proc->mName + " version " + proc->mVersion);
                }
                proc = pkg.mProcesses->Get(proc->mName);
                if (proc == NULL) {
                    throw new IllegalStateException("Didn't create per-package process "
                            + proc->mName + " in pkg " + pkg.mPackageName + "/" + pkg.mUid);
                }
                holder.state = proc;
            }
            return proc;
        }

        Int64 GetDuration(Int32 state,
            /* [in] */ Int64 now) {
            Int64 time = super.GetDuration(state, now);
            if (mCurState == state) {
                time += now - mStartTime;
            }
            return time;
        }

        Int64 getPssSampleCount(Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_SAMPLE_COUNT) : 0;
        }

        Int64 getPssMinimum(Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_MINIMUM) : 0;
        }

        Int64 getPssAverage(
            /* [in] */ Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_AVERAGE) : 0;
        }

        Int64 getPssMaximum(Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_MAXIMUM) : 0;
        }

        Int64 getPssUssMinimum(Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_USS_MINIMUM) : 0;
        }

        Int64 getPssUssAverage(
            /* [in] */ Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_USS_AVERAGE) : 0;
        }

        Int64 getPssUssMaximum(Int32 state) {
            Int32 idx = BinarySearch(mPssTable, mPssTableSize, state);
            return idx >= 0 ? mStats->GetInt64(mPssTable[idx], PSS_USS_MAXIMUM) : 0;
        }

        public String toString() {
            StringBuilder sb = new StringBuilder(128);
            sb.append("ProcessState{").append(Integer.toHexString(System.identityHashCode(this)))
                    .append(" ").append(mName).append("/").append(mUid)
                    .append(" pkg=").append(mPackage);
            if (mMultiPackage) sb.append(" (multi)");
            if (mCommonProcess != this) sb.append(" (sub)");
            sb.append("}");
            return sb.toString();
        }
    }

    public static final class ServiceState extends DurationsTable {
        public final String mPackage;
        public final String mProcessName;
        ProcessState mProc;

        Object mOwner;

        public static final Int32 SERVICE_RUN = 0;
        public static final Int32 SERVICE_STARTED = 1;
        public static final Int32 SERVICE_BOUND = 2;
        public static final Int32 SERVICE_EXEC = 3;
        static final Int32 SERVICE_COUNT = 4;

        Int32 mRunCount;
        public Int32 mRunState = STATE_NOTHING;
        Int64 mRunStartTime;

        Boolean mStarted;
        Boolean mRestarting;
        Int32 mStartedCount;
        public Int32 mStartedState = STATE_NOTHING;
        Int64 mStartedStartTime;

        Int32 mBoundCount;
        public Int32 mBoundState = STATE_NOTHING;
        Int64 mBoundStartTime;

        Int32 mExecCount;
        public Int32 mExecState = STATE_NOTHING;
        Int64 mExecStartTime;

        public ServiceState(
            /* [in] */ IProcessStats* processStats,
            /* [in] */ const String& pkg,
            /* [in] */ const String& name,
                String processName,
            /* [in] */ IProcessState* _proc) {
            super(processStats, name);
            mPackage = pkg;
            mProcessName = processName;
            mProc = proc;
        }

        CARAPI ApplyNewOwner(Object newOwner) {
            if (mOwner != newOwner) {
                if (mOwner == NULL) {
                    mOwner = newOwner;
                    mProc.incActiveServices(mName);
                }
                else {
                    // There was already an old owner, Reset this object for its
                    // new owner.
                    mOwner = newOwner;
                    if (mStarted || mBoundState != STATE_NOTHING || mExecState != STATE_NOTHING) {
                        Int64 now = SystemClock::GetUptimeMillis();
                        if (mStarted) {
                            if (DEBUG) Slogger::D(TAG, "Service has new owner " + newOwner
                                    + " from " + mOwner + " while started: pkg="
                                    + mPackage + " service=" + mName + " proc=" + mProc);
                            setStarted(FALSE, 0, now);
                        }
                        if (mBoundState != STATE_NOTHING) {
                            if (DEBUG) Slogger::D(TAG, "Service has new owner " + newOwner
                                    + " from " + mOwner + " while bound: pkg="
                                    + mPackage + " service=" + mName + " proc=" + mProc);
                            setBound(FALSE, 0, now);
                        }
                        if (mExecState != STATE_NOTHING) {
                            if (DEBUG) Slogger::D(TAG, "Service has new owner " + newOwner
                                    + " from " + mOwner + " while executing: pkg="
                                    + mPackage + " service=" + mName + " proc=" + mProc);
                            setExecuting(FALSE, 0, now);
                        }
                    }
                }
            }
        }

        CARAPI ClearCurrentOwner(Object owner, Boolean silently) {
            if (mOwner == owner) {
                mProc.decActiveServices(mName);
                if (mStarted || mBoundState != STATE_NOTHING || mExecState != STATE_NOTHING) {
                    Int64 now = SystemClock::GetUptimeMillis();
                    if (mStarted) {
                        if (!silently) {
                            Slogger::WtfStack(TAG, "Service owner " + owner
                                    + " cleared while started: pkg=" + mPackage + " service="
                                    + mName + " proc=" + mProc);
                        }
                        setStarted(FALSE, 0, now);
                    }
                    if (mBoundState != STATE_NOTHING) {
                        if (!silently) {
                            Slogger::WtfStack(TAG, "Service owner " + owner
                                    + " cleared while bound: pkg=" + mPackage + " service="
                                    + mName + " proc=" + mProc);
                        }
                        setBound(FALSE, 0, now);
                    }
                    if (mExecState != STATE_NOTHING) {
                        if (!silently) {
                            Slogger::WtfStack(TAG, "Service owner " + owner
                                    + " cleared while exec: pkg=" + mPackage + " service="
                                    + mName + " proc=" + mProc);
                        }
                        setExecuting(FALSE, 0, now);
                    }
                }
                mOwner = NULL;
            }
        }

        public Boolean isInUse() {
            return mOwner != NULL || mRestarting;
        }

        public Boolean isRestarting() {
            return mRestarting;
        }

        void add(ServiceState other) {
            addDurations(other);
            mRunCount += other->mRunCount;
            mStartedCount += other->mStartedCount;
            mBoundCount += other->mBoundCount;
            mExecCount += other->mExecCount;
        }

        void ResetSafely(
            /* [in] */ Int64 now) {
            ResetDurationsSafely();
            mRunCount = mRunState != STATE_NOTHING ? 1 : 0;
            mStartedCount = mStartedState != STATE_NOTHING ? 1 : 0;
            mBoundCount = mBoundState != STATE_NOTHING ? 1 : 0;
            mExecCount = mExecState != STATE_NOTHING ? 1 : 0;
            mRunStartTime = mStartedStartTime = mBoundStartTime = mExecStartTime = now;
        }

        void writeToParcel(Parcel out,
            /* [in] */ Int64 now) {
            writeDurationsToParcel(out);
            out.writeInt(mRunCount);
            out.writeInt(mStartedCount);
            out.writeInt(mBoundCount);
            out.writeInt(mExecCount);
        }

        Boolean readFromParcel(Parcel in) {
            if (!readDurationsFromParcel(in)) {
                return FALSE;
            }
            mRunCount = in.readInt();
            mStartedCount = in.readInt();
            mBoundCount = in.readInt();
            mExecCount = in.readInt();
            return TRUE;
        }

        void commitStateTime(
            /* [in] */ Int64 now) {
            if (mRunState != STATE_NOTHING) {
                addDuration(SERVICE_RUN + (mRunState*SERVICE_COUNT), now - mRunStartTime);
                mRunStartTime = now;
            }
            if (mStartedState != STATE_NOTHING) {
                addDuration(SERVICE_STARTED + (mStartedState*SERVICE_COUNT),
                        now - mStartedStartTime);
                mStartedStartTime = now;
            }
            if (mBoundState != STATE_NOTHING) {
                addDuration(SERVICE_BOUND + (mBoundState*SERVICE_COUNT), now - mBoundStartTime);
                mBoundStartTime = now;
            }
            if (mExecState != STATE_NOTHING) {
                addDuration(SERVICE_EXEC + (mExecState*SERVICE_COUNT), now - mExecStartTime);
                mExecStartTime = now;
            }
        }

        private void updateRunning(Int32 memFactor,
            /* [in] */ Int64 now) {
            final Int32 state = (mStartedState != STATE_NOTHING || mBoundState != STATE_NOTHING
                    || mExecState != STATE_NOTHING) ? memFactor : STATE_NOTHING;
            if (mRunState != state) {
                if (mRunState != STATE_NOTHING) {
                    addDuration(SERVICE_RUN + (mRunState*SERVICE_COUNT),
                            now - mRunStartTime);
                }
                else if (state != STATE_NOTHING) {
                    mRunCount++;
                }
                mRunState = state;
                mRunStartTime = now;
            }
        }

        CARAPI SetStarted(
            /* [in] */ Boolean started,
            /* [in] */ Int32 memFactor,
            /* [in] */ Int64 now) {
            if (mOwner == NULL) {
                Slogger::Wtf(TAG, "Starting service " + this + " without owner");
            }
            mStarted = started;
            updateStartedState(memFactor, now);
        }

        CARAPI SetRestarting(
            /* [in] */ Boolean restarting,
            /* [in] */ Int32 memFactor,
            /* [in] */ Int64 now) {
            mRestarting = restarting;
            updateStartedState(memFactor, now);
        }

        void updateStartedState(
            /* [in] */ Int32 memFactor,
            /* [in] */ Int64 now) {
            final Boolean wasStarted = mStartedState != STATE_NOTHING;
            final Boolean started = mStarted || mRestarting;
            final Int32 state = started ? memFactor : STATE_NOTHING;
            if (mStartedState != state) {
                if (mStartedState != STATE_NOTHING) {
                    addDuration(SERVICE_STARTED + (mStartedState*SERVICE_COUNT),
                            now - mStartedStartTime);
                }
                else if (started) {
                    mStartedCount++;
                }
                mStartedState = state;
                mStartedStartTime = now;
                mProc = mProc.pullFixedProc(mPackage);
                if (wasStarted != started) {
                    if (started) {
                        mProc.incStartedServices(memFactor, now, mName);
                    }
                    else {
                        mProc.decStartedServices(memFactor, now, mName);
                    }
                }
                updateRunning(memFactor, now);
            }
        }

        CARAPI SetBound(
            /* [in] */ Boolean bound,
            /* [in] */ Int32 memFactor,
            /* [in] */ Int64 now) {
            if (mOwner == NULL) {
                Slogger::Wtf(TAG, "Binding service " + this + " without owner");
            }
            final Int32 state = bound ? memFactor : STATE_NOTHING;
            if (mBoundState != state) {
                if (mBoundState != STATE_NOTHING) {
                    addDuration(SERVICE_BOUND + (mBoundState*SERVICE_COUNT),
                            now - mBoundStartTime);
                }
                else if (bound) {
                    mBoundCount++;
                }
                mBoundState = state;
                mBoundStartTime = now;
                updateRunning(memFactor, now);
            }
        }

        CARAPI SetExecuting(
            /* [in] */ Boolean executing,
            /* [in] */ Int32 memFactor,
            /* [in] */ Int64 now) {
            if (mOwner == NULL) {
                Slogger::Wtf(TAG, "Executing service " + this + " without owner");
            }
            final Int32 state = executing ? memFactor : STATE_NOTHING;
            if (mExecState != state) {
                if (mExecState != STATE_NOTHING) {
                    addDuration(SERVICE_EXEC + (mExecState*SERVICE_COUNT), now - mExecStartTime);
                }
                else if (executing) {
                    mExecCount++;
                }
                mExecState = state;
                mExecStartTime = now;
                updateRunning(memFactor, now);
            }
        }

        private Int64 GetDuration(Int32 opType,
            /* [in] */ Int32 curState,
            /* [in] */ Int64 startTime,
        /* [in] */ Int32 memFactor,
                Int64 now) {
            Int32 state = opType + (memFactor*SERVICE_COUNT);
            Int64 time = GetDuration(state, now);
            if (curState == memFactor) {
                time += now - startTime;
            }
            return time;
        }

        public String toString() {
            return "ServiceState{" + Integer.toHexString(System.identityHashCode(this))
                    + " " + mName + " pkg=" + mPackage + " proc="
                    + Integer.toHexString(System.identityHashCode(this)) + "}";
        }
    }

    public static final class PackageState {
        public final ArrayMap<String, ProcessState> mProcesses
                = new ArrayMap<String, ProcessState>();
        public final ArrayMap<String, ServiceState> mServices
                = new ArrayMap<String, ServiceState>();
        public final String mPackageName;
        public final Int32 mUid;

        public PackageState(String packageName,
            /* [in] */ Int32 uid) {
            mUid = uid;
            mPackageName = packageName;
        }
    }

    public static final class ProcessDataCollection {
        final Int32[] screenStates;
        final Int32[] memStates;
        final Int32[] procStates;

        public Int64 totalTime;
        public Int64 numPss;
        public Int64 minPss;
        public Int64 avgPss;
        public Int64 maxPss;
        public Int64 minUss;
        public Int64 avgUss;
        public Int64 maxUss;

        public ProcessDataCollection(Int32[] _screenStates,
            /* [in] */ ArrayOf<Int32>* _memStates,
            /* [in] */ ArrayOf<Int32>* _procStates) {
            screenStates = _screenStates;
            memStates = _memStates;
            procStates = _procStates;
        }

        void print(
            /* [in] */ IPrintWriter* pw,
            /* [in] */ Int64 overallTime, Boolean full) {
            if (totalTime > overallTime) {
                pw->Print(String("*"));
            }
            PrintPercent(pw, (Double) totalTime / (Double) overallTime);
            if (numPss > 0) {
                pw->Print(String(" ("));
                PrintSizeValue(pw, minPss * 1024);
                pw->Print(String("-"));
                PrintSizeValue(pw, avgPss * 1024);
                pw->Print(String("-"));
                PrintSizeValue(pw, maxPss * 1024);
                pw->Print(String("/"));
                PrintSizeValue(pw, minUss * 1024);
                pw->Print(String("-"));
                PrintSizeValue(pw, avgUss * 1024);
                pw->Print(String("-"));
                PrintSizeValue(pw, maxUss * 1024);
                if (full) {
                    pw->Print(String(" over "));
                    pw->Print(numPss);
                }
                pw->Print(String(")"));
            }
        }
    };

    public static class TotalMemoryUseCollection {
        final Int32[] screenStates;
        final Int32[] memStates;

        public TotalMemoryUseCollection(Int32[] _screenStates,
            /* [in] */ ArrayOf<Int32>* _memStates) {
            screenStates = _screenStates;
            memStates = _memStates;
        }

        public Int64 totalTime;
        public AutoPtr<ArrayOf<Int64> > processStatePss = new Int64[STATE_COUNT];
        public Double[] processStateWeight = new Double[STATE_COUNT];
        public AutoPtr<ArrayOf<Int64> > processStateTime = new Int64[STATE_COUNT];
        public Int32[] processStateSamples = new Int32[STATE_COUNT];
        public AutoPtr<ArrayOf<Int64> > sysMemUsage = new Int64[SYS_MEM_USAGE_COUNT];
        public Double sysMemCachedWeight;
        public Double sysMemFreeWeight;
        public Double sysMemZRamWeight;
        public Double sysMemKernelWeight;
        public Double sysMemNativeWeight;
        public Int32 sysMemSamples;
    };

private:
    static AutoPtr<ArrayOf<Int32> > InitALL_MEM_ADJ();

    static AutoPtr<ArrayOf<Int32> > InitALL_SCREEN_ADJ();

    static AutoPtr<ArrayOf<Int32> > InitNON_CACHED_PROC_STATES();

    static AutoPtr<ArrayOf<Int32> > InitBACKGROUND_PROC_STATES();

    static AutoPtr<ArrayOf<Int32> > InitPROCESS_STATE_TO_STATE();

    static AutoPtr<ArrayOf<Int32> > InitALL_PROC_STATES();

    static AutoPtr<ArrayOf<String> > InitSTATE_NAMES();

    static AutoPtr<ArrayOf<String> > InitADJ_SCREEN_NAMES_CSV();

    static AutoPtr<ArrayOf<String> > InitADJ_MEM_NAMES_CSV();

    static AutoPtr<ArrayOf<String> > InitSTATE_NAMES_CSV();

    static AutoPtr<ArrayOf<String> > InitADJ_SCREEN_TAGS();

    static AutoPtr<ArrayOf<String> > InitADJ_MEM_TAGS();

    static AutoPtr<ArrayOf<String> > InitSTATE_TAGS();

public:
    static const AutoPtr<ArrayOf<Int32> > ALL_MEM_ADJ;
    static const AutoPtr<ArrayOf<Int32> > ALL_SCREEN_ADJ;
    static const AutoPtr<ArrayOf<Int32> > NON_CACHED_PROC_STATES;
    static const AutoPtr<ArrayOf<Int32> > BACKGROUND_PROC_STATES;
    static const AutoPtr<ArrayOf<Int32> > ALL_PROC_STATES;
    static const AutoPtr<ArrayOf<String> > ADJ_SCREEN_NAMES_CSV;
    static const AutoPtr<ArrayOf<String> > ADJ_MEM_NAMES_CSV;
    static const AutoPtr<ArrayOf<String> > STATE_NAMES_CSV;

public: // not public in java
    static const String TAG;
    static const Boolean DEBUG = FALSE;
    static const Boolean DEBUG_PARCEL = FALSE;

    // Map from process states to the states we track.
    static const AutoPtr<ArrayOf<Int32> > PROCESS_STATE_TO_STATE ;
    static const AutoPtr<ArrayOf<String> > STATE_NAMES;
    static const AutoPtr<ArrayOf<String> > ADJ_SCREEN_TAGS;
    static const AutoPtr<ArrayOf<String> > ADJ_MEM_TAGS;
    static const AutoPtr<ArrayOf<String> > STATE_TAGS;
    static const String CSV_SEP ;

    // Where the "type"/"state" part of the data appears in an offset integer.
    static Int32 OFFSET_TYPE_SHIFT;
    static Int32 OFFSET_TYPE_MASK;
    // Where the "which array" part of the data appears in an offset integer.
    static Int32 OFFSET_ARRAY_SHIFT;
    static Int32 OFFSET_ARRAY_MASK;
    // Where the "index into array" part of the data appears in an offset integer.
    static Int32 OFFSET_INDEX_SHIFT;
    static Int32 OFFSET_INDEX_MASK;

    static const Int32 LONGS_SIZE = 4096;

private:
    // Current version of the parcel format.
    static const Int32 PARCEL_VERSION = 18;
    // In-memory Parcel magic number, used to detect attempts to unmarshall bad data
    static const Int32 MAGIC = 0x50535453;

public:
    String mReadError;
    String mTimePeriodStartClockStr;
    Int32 mFlags;

    AutoPtr<IProcessMap> mPackages;
    AutoPtr<IProcessMap> mProcesses;

    AutoPtr<ArrayOf<Int64> > mMemFactorDurations;
    Int32 mMemFactor;
    Int64 mStartTime;

    AutoPtr<ArrayOf<Int32> > mSysMemUsageTable;
    Int32 mSysMemUsageTableSize;
    AutoPtr<ArrayOf<Int64> > mSysMemUsageArgs;

    Int64 mTimePeriodStartClock;
    Int64 mTimePeriodStartRealtime;
    Int64 mTimePeriodEndRealtime;
    Int64 mTimePeriodStartUptime;
    Int64 mTimePeriodEndUptime;

public: // not public in java
    String mRuntime;
    Boolean mRunning;

    List<AutoPtr<ArrayOf<Int64> > > mLongs;
    Int32 mNextLong;

    AutoPtr<ArrayOf<Int32> > mAddLongTable;
    Int32 mAddLongTableSize;

    // For writing parcels.
    AutoPtr<HashMap<String, AutoPtr<IInteger32> > > mCommonStringToIndex;

    // For reading parcels.
    AutoPtr<List<String> > mIndexToCommonString;
};

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_PROCESSSTATS_H__
