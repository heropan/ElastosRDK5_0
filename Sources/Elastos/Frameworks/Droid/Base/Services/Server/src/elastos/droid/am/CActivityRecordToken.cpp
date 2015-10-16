#include "elastos/droid/ext/frameworkdef.h"
#include "am/CActivityRecordToken.h"
#include "am/ActivityRecord.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

CActivityRecordToken::CActivityRecordToken()
{
}

CActivityRecordToken::~CActivityRecordToken()
{
}

ECode CActivityRecordToken::constructor(
    /* [in] */ Handle32 activity)
{
    ActivityRecord* a = (ActivityRecord*)activity;
    if (a) {
        a->GetWeakReference((IWeakReference**)&mWeakActivity);
    }
    return NOERROR;
}

AutoPtr<ActivityRecord> CActivityRecordToken::GetActivityRecord()
{
    AutoPtr<ActivityRecord> activity;
    AutoPtr<IInterface> obj;
    mWeakActivity->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        activity = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));
    }

    return activity;
}

ECode CActivityRecordToken::WindowsDrawn()
{
    AutoPtr<ActivityRecord> ar = GetActivityRecord();
    if (ar != NULL) {
        return ar->WindowsDrawn();
    }
    return NOERROR;
}

ECode CActivityRecordToken::WindowsVisible()
{
    AutoPtr<ActivityRecord> ar = GetActivityRecord();
    if (ar != NULL) {
        return ar->WindowsVisible();
    }
    return NOERROR;
}

ECode CActivityRecordToken::WindowsGone()
{
    AutoPtr<ActivityRecord> ar = GetActivityRecord();
    if (ar != NULL) {
        return ar->WindowsGone();
    }
    return NOERROR;
}

ECode CActivityRecordToken::KeyDispatchingTimedOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ActivityRecord> ar = GetActivityRecord();
    if (ar != NULL) {
        return ar->KeyDispatchingTimedOut(result);
    }

    *result = FALSE;
    return NOERROR;
}

ECode CActivityRecordToken::GetKeyDispatchingTimeout(
    /* [out] */ Int64* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    AutoPtr<ActivityRecord> ar = GetActivityRecord();
    if (ar != NULL) {
        return ar->GetKeyDispatchingTimeout(timeout);
    }

    *timeout = 0;
    return NOERROR;
}

ECode CActivityRecordToken::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    if (!mStringName.IsNull()) {
        *str = mStringName;
        return NOERROR;
    }
    StringBuilder sb(128);
    sb += "Token{";
    sb += StringUtils::Int32ToHexString(Int32(this));
    sb += ", ActivityRecord:";

    AutoPtr<ActivityRecord> ar = GetActivityRecord();
    if (ar != NULL) {
        String astr;
        ar->ToString(&astr);
        sb += astr;
    }
    else {
        sb += "NULL";
    }

    sb += "}";
    *str = mStringName = sb.ToString();
    return NOERROR;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
