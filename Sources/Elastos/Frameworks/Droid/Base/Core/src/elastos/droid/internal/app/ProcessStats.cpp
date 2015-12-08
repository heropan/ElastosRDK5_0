
#include "elastos/droid/internal/app/ProcessStats.h"
#include "elastos/droid/internal/app/CProcessMap.h"
#include "elastos/droid/utility/CArrayMap.h"
#include <elastos/core/CoreUtils.h>

using Elastos::Droid::Utility::CArrayMap;
using Elastos::Core::CoreUtils;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

ProcessStats::PssAggr::PssAggr()
    : mPss(0)
    , mSamples(0)
{}

void ProcessStats::PssAggr::Add(
    /* [in] */ Int64 newPss,
    /* [in] */ Int64 newSamples)
{
    mPss = (Int64)((mPss * (Double)mSamples) + (newPss * (Double)newSamples))
            / (mSamples+newSamples);
    mSamples += newSamples;
}

AutoPtr<ArrayOf<Int32> > ProcessStats::InitALL_MEM_ADJ()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(4);
    (*array)[0] = ADJ_MEM_FACTOR_NORMAL;
    (*array)[1] = ADJ_MEM_FACTOR_MODERATE;
    (*array)[2] = ADJ_MEM_FACTOR_LOW;
    (*array)[3] = ADJ_MEM_FACTOR_CRITICAL;
    return array;
}

AutoPtr<ArrayOf<Int32> > ProcessStats::InitALL_SCREEN_ADJ()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(2);
    (*array)[0] = ADJ_SCREEN_OFF;
    (*array)[1] = ADJ_SCREEN_ON;
    return array;
}

AutoPtr<ArrayOf<Int32> > ProcessStats::InitNON_CACHED_PROC_STATES()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(9);
    (*array)[0] = STATE_PERSISTENT;
    (*array)[1] = STATE_TOP;
    (*array)[2] = STATE_IMPORTANT_FOREGROUND;
    (*array)[3] = STATE_IMPORTANT_BACKGROUND;
    (*array)[4] = STATE_BACKUP;
    (*array)[5] = STATE_HEAVY_WEIGHT;
    (*array)[6] = STATE_SERVICE;
    (*array)[7] = STATE_SERVICE_RESTARTING;
    (*array)[8] = STATE_RECEIVER;
    return array;
}

AutoPtr<ArrayOf<Int32> > ProcessStats::InitBACKGROUND_PROC_STATES()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(7);
    (*array)[0] = STATE_IMPORTANT_FOREGROUND;
    (*array)[1] = STATE_IMPORTANT_BACKGROUND;
    (*array)[2] = STATE_BACKUP;
    (*array)[3] = STATE_HEAVY_WEIGHT;
    (*array)[4] = STATE_SERVICE;
    (*array)[5] = STATE_SERVICE_RESTARTING;
    (*array)[6] = STATE_RECEIVER;
    return array;
}

AutoPtr<ArrayOf<Int32> > ProcessStats::InitPROCESS_STATE_TO_STATE()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(14);
    (*array)[0] = STATE_PERSISTENT;               // ActivityManager.PROCESS_STATE_PERSISTENT
    (*array)[1] = STATE_PERSISTENT;               // ActivityManager.PROCESS_STATE_PERSISTENT_UI
    (*array)[2] = STATE_TOP;                      // ActivityManager.PROCESS_STATE_TOP
    (*array)[3] = STATE_IMPORTANT_FOREGROUND;     // ActivityManager.PROCESS_STATE_IMPORTANT_FOREGROUND
    (*array)[4] = STATE_IMPORTANT_BACKGROUND;     // ActivityManager.PROCESS_STATE_IMPORTANT_BACKGROUND
    (*array)[5] = STATE_BACKUP;                   // ActivityManager.PROCESS_STATE_BACKUP
    (*array)[6] = STATE_HEAVY_WEIGHT;             // ActivityManager.PROCESS_STATE_HEAVY_WEIGHT
    (*array)[7] = STATE_SERVICE;                  // ActivityManager.PROCESS_STATE_SERVICE
    (*array)[8] = STATE_RECEIVER;                 // ActivityManager.PROCESS_STATE_RECEIVER
    (*array)[9] = STATE_HOME;                     // ActivityManager.PROCESS_STATE_HOME
    (*array)[10] = STATE_LAST_ACTIVITY;            // ActivityManager.PROCESS_STATE_LAST_ACTIVITY
    (*array)[11] = STATE_CACHED_ACTIVITY;          // ActivityManager.PROCESS_STATE_CACHED_ACTIVITY
    (*array)[12] = STATE_CACHED_ACTIVITY_CLIENT;   // ActivityManager.PROCESS_STATE_CACHED_ACTIVITY_CLIENT
    (*array)[13] = STATE_CACHED_EMPTY;             // ActivityManager.PROCESS_STATE_CACHED_EMPTY
    return array;
}

AutoPtr<ArrayOf<Int32> > ProcessStats::InitALL_PROC_STATES()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(13);
    (*array)[0] = STATE_TOP;
    (*array)[1] = STATE_IMPORTANT_FOREGROUND;
    (*array)[2] = STATE_IMPORTANT_BACKGROUND;
    (*array)[3] = STATE_BACKUP;
    (*array)[4] = STATE_HEAVY_WEIGHT;
    (*array)[5] = STATE_SERVICE;
    (*array)[6] = STATE_SERVICE_RESTARTING;
    (*array)[7] = STATE_RECEIVER;
    (*array)[8] = STATE_HOME;
    (*array)[9] = STATE_LAST_ACTIVITY;
    (*array)[10] = STATE_CACHED_ACTIVITY;
    (*array)[11] = STATE_CACHED_ACTIVITY_CLIENT;
    (*array)[12] = STATE_CACHED_EMPTY;
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitSTATE_NAMES()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(14);
    (*array)[0] = String("Persist");
    (*array)[1] = String("Top    ");
    (*array)[2] = String("ImpFg  ");
    (*array)[3] = String("ImpBg  ");
    (*array)[4] = String("Backup ");
    (*array)[5] = String("HeavyWt");
    (*array)[6] = String("Service");
    (*array)[7] = String("ServRst");
    (*array)[8] = String("Receivr");
    (*array)[9] = String("Home   ");
    (*array)[10] = String("LastAct");
    (*array)[11] = String("CchAct ");
    (*array)[12] = String("CchCAct");
    (*array)[13] = String("CchEmty");
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitADJ_SCREEN_NAMES_CSV()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    (*array)[0] = String("off");
    (*array)[1] = String("on");
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitADJ_MEM_NAMES_CSV()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(4);
    (*array)[0] = String("norm");
    (*array)[1] = String("mod");
    (*array)[2] = String("low");
    (*array)[3] = String("crit");
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitSTATE_NAMES_CSV()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(14);
    (*array)[0] = String("pers");
    (*array)[1] = String("top");
    (*array)[2] = String("impfg");
    (*array)[3] = String("impbg");
    (*array)[4] = String("backup");
    (*array)[5] = String("heavy");
    (*array)[6] = String("service");
    (*array)[7] = String("service-rs");
    (*array)[8] = String("receiver");
    (*array)[9] = String("home");
    (*array)[10] = String("lastact");
    (*array)[11] = String("cch-activity");
    (*array)[12] = String("cch-aclient");
    (*array)[13] = String("cch-empty");
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitADJ_SCREEN_TAGS()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    (*array)[0] = String("0");
    (*array)[1] = String("1");
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitADJ_MEM_TAGS()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(4);
    (*array)[0] = String("n");
    (*array)[1] = String("m");
    (*array)[2] = String("l");
    (*array)[3] = String("c");
    return array;
}

AutoPtr<ArrayOf<String> > ProcessStats::InitSTATE_TAGS()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(14);
    (*array)[0] = String("p");
    (*array)[1] = String("t");
    (*array)[2] = String("f");
    (*array)[3] = String("b");
    (*array)[4] = String("u");
    (*array)[5] = String("w");
    (*array)[6] = String("s");
    (*array)[7] = String("x");
    (*array)[8] = String("r");
    (*array)[9] = String("h");
    (*array)[10] = String("l");
    (*array)[11] = String("a");
    (*array)[12] = String("c");
    (*array)[13] = String("e");
    return array;
}

const String ProcessStats::TAG("ProcessStats");
const Boolean ProcessStats::DEBUG;
const Boolean ProcessStats::DEBUG_PARCEL;

const AutoPtr<ArrayOf<Int32> > ProcessStats::ALL_MEM_ADJ = ProcessStats::InitALL_MEM_ADJ();
const AutoPtr<ArrayOf<Int32> > ProcessStats::ALL_SCREEN_ADJ = ProcessStats::InitALL_SCREEN_ADJ();
const AutoPtr<ArrayOf<Int32> > ProcessStats::NON_CACHED_PROC_STATES = ProcessStats::InitNON_CACHED_PROC_STATES();
const AutoPtr<ArrayOf<Int32> > ProcessStats::BACKGROUND_PROC_STATES = ProcessStats::InitBACKGROUND_PROC_STATES();
const AutoPtr<ArrayOf<Int32> > ProcessStats::PROCESS_STATE_TO_STATE = ProcessStats::InitPROCESS_STATE_TO_STATE() ;
const AutoPtr<ArrayOf<Int32> > ProcessStats::ALL_PROC_STATES = ProcessStats::InitALL_PROC_STATES();

const AutoPtr<ArrayOf<String> > ProcessStats::STATE_NAMES = ProcessStats::InitSTATE_NAMES();
const AutoPtr<ArrayOf<String> > ProcessStats::ADJ_SCREEN_NAMES_CSV = ProcessStats::InitADJ_SCREEN_NAMES_CSV();
const AutoPtr<ArrayOf<String> > ProcessStats::ADJ_MEM_NAMES_CSV = ProcessStats::InitADJ_MEM_NAMES_CSV();
const AutoPtr<ArrayOf<String> > ProcessStats::STATE_NAMES_CSV = ProcessStats::InitSTATE_NAMES_CSV();
const AutoPtr<ArrayOf<String> > ProcessStats::ADJ_SCREEN_TAGS = ProcessStats::InitADJ_SCREEN_TAGS();
const AutoPtr<ArrayOf<String> > ProcessStats::ADJ_MEM_TAGS = ProcessStats::InitADJ_MEM_TAGS();
const AutoPtr<ArrayOf<String> > ProcessStats::STATE_TAGS = ProcessStats::InitSTATE_TAGS();

const String ProcessStats::CSV_SEP("\t");
const Int32 ProcessStats::PARCEL_VERSION;
const Int32 ProcessStats::MAGIC;

Int32 ProcessStats::OFFSET_TYPE_SHIFT = 0;
Int32 ProcessStats::OFFSET_TYPE_MASK = 0xff;
Int32 ProcessStats::OFFSET_ARRAY_SHIFT = 8;
Int32 ProcessStats::OFFSET_ARRAY_MASK = 0xff;
Int32 ProcessStats::OFFSET_INDEX_SHIFT = 16;
Int32 ProcessStats::OFFSET_INDEX_MASK = 0xffff;

const Int32 ProcessStats::LONGS_SIZE;

CAR_INTERFACE_IMPL_2(ProcessStats, Object, IProcessStats, IParcelable)

ProcessStats::ProcessStats()
    : mFlags(0)
    , mMemFactor(STATE_NOTHING)
    , mStartTime(0)
    , mSysMemUsageTableSize(0)
    , mTimePeriodStartClock(0)
    , mTimePeriodStartRealtime(0)
    , mTimePeriodEndRealtime(0)
    , mTimePeriodStartUptime(0)
    , mTimePeriodEndUptime(0)
    , mRunning(FALSE)
    , mNextLong(0)
    , mAddLongTableSize(0)
{
    CProcessMap::New((IProcessMap**)&mPackages);
    CProcessMap::New((IProcessMap**)&mProcesses);
    mMemFactorDurations = ArrayOf<Int64>::Alloc(ADJ_COUNT);
    mSysMemUsageArgs = ArrayOf<Int64>::Alloc(SYS_MEM_USAGE_COUNT);
}

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos
