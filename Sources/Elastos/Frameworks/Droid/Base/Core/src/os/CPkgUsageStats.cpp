
#include <elastos/coredef.h>
#include "os/CPkgUsageStats.h"

using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

ECode CPkgUsageStats::constructor(
    /* [in] */ const String& pkgName,
    /* [in] */ Int32 count,
    /* [in] */ Int64 time,
    /* [in] */ IObjectStringMap* lastResumeTimes)
{
    mPackageName = pkgName;
    mLaunchCount = count;
    mUsageTime = time;
    mComponentResumeTimes = lastResumeTimes;
    return NOERROR;
}

ECode CPkgUsageStats::constructor(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadString(&mPackageName))
    FAIL_RETURN(source->ReadInt32(&mLaunchCount))
    FAIL_RETURN(source->ReadInt64(&mUsageTime))
    Int32 N;
    FAIL_RETURN(source->ReadInt32(&N))
    for (Int32 i = 0; i < N; i++) {
        String component;
        FAIL_RETURN(source->ReadString(&component))
        Int64 lastResumeTime;
        FAIL_RETURN(source->ReadInt64(&lastResumeTime))
        AutoPtr<IInteger64> integer64;
        FAIL_RETURN(CInteger64::New(lastResumeTime, (IInteger64**)&integer64))
        FAIL_RETURN(mComponentResumeTimes->Put(component, (IInterface*)integer64))
    }
    return NOERROR;
}

ECode CPkgUsageStats::constructor(
    /* [in] */ Handle32 pStats)
{
    mPackageName = ((CPkgUsageStats*)pStats)->mPackageName;
    mLaunchCount = ((CPkgUsageStats*)pStats)->mLaunchCount;
    mUsageTime = ((CPkgUsageStats*)pStats)->mUsageTime;
    return NOERROR;
}

ECode CPkgUsageStats::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

    *contents = 0;
    return NOERROR;
}

CARAPI CPkgUsageStats::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    FAIL_RETURN(in->ReadString(&mPackageName))
    FAIL_RETURN(in->ReadInt32(&mLaunchCount))
    FAIL_RETURN(in->ReadInt64(&mUsageTime))
    Int32 N;
    FAIL_RETURN(in->ReadInt32(&N))
    for (Int32 i = 0; i < N; i++) {
        String component;
        FAIL_RETURN(in->ReadString(&component))
        Int64 lastResumeTime;
        FAIL_RETURN(in->ReadInt64(&lastResumeTime))
        AutoPtr<IInteger64> integer64;
        FAIL_RETURN(CInteger64::New(lastResumeTime, (IInteger64**)&integer64))
        FAIL_RETURN(mComponentResumeTimes->Put(component, (IInterface*)integer64))
    }
    return NOERROR;
}

ECode CPkgUsageStats::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mPackageName))
    FAIL_RETURN(dest->WriteInt32(mLaunchCount))
    FAIL_RETURN(dest->WriteInt64(mUsageTime))
    Int32 size;
    FAIL_RETURN(mComponentResumeTimes->GetSize(&size))
    FAIL_RETURN(dest->WriteInt32(size))

    AutoPtr<ArrayOf<String> > keyArray;
    AutoPtr<IObjectContainer> objContainer;
    FAIL_RETURN(mComponentResumeTimes->GetAllItems((ArrayOf<String>**)&keyArray, (IObjectContainer**)&objContainer))
    Int32 len = keyArray->GetLength();
    for (Int32 i = 0; i < len; i++) {
        FAIL_RETURN(dest->WriteString((*keyArray)[i]))
        AutoPtr<IInteger64> value;
        FAIL_RETURN(mComponentResumeTimes->Get((*keyArray)[i], (IInterface**)&value))
        Int64 svalue;
        FAIL_RETURN(value->GetValue(&svalue))
        FAIL_RETURN(dest->WriteInt64(svalue))
    }

    return NOERROR;
}

ECode CPkgUsageStats::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    *str = String("CPkgUsageStats{")
        + /* TODO: Integer.toHexString(System.identityHashCode(this)) */
        + String(" ") + mPackageName + String("}");
    return NOERROR;
}

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos
