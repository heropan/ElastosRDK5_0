
#include "am/TaskRecord.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <os/UserHandle.h>
#include <os/SystemClock.h>
#include "am/CActivityManagerService.h"

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringBuilder;
using Elastos::Core::IStringBuilder;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

TaskRecord::TaskRecord(
    /* [in] */ Int32 taskId,
    /* [in] */ IActivityInfo* info,
    /* [in] */ IIntent* intent)
    : mTaskId(taskId)
    , mNumActivities(0)
    , mLastActiveTime(0)
    , mRootWasReset(FALSE)
    , mAskedCompatMode(FALSE)
    , mUserId(0)
{
    assert(info != NULL && "TaskRecord::TaskRecord: IActivityInfo can not be NULL.");
    info->GetTaskAffinity(&mAffinity);
    SetIntent(intent, info);
}

TaskRecord::~TaskRecord()
{
    Slogger::D("TaskRecord", " =========== ~TaskRecord() %s", ToString().string());
}

Int64 TaskRecord::TouchActiveTime()
{
    mLastActiveTime = SystemClock::GetElapsedRealtime();
    return mLastActiveTime;
}

Int64 TaskRecord::GetInactiveDuration()
{
    return SystemClock::GetElapsedRealtime() - mLastActiveTime;
}

ECode TaskRecord::SetIntent(
    /* [in] */ IIntent* _intent,
    /* [in] */ IActivityInfo* info)
{
    VALIDATE_NOT_NULL(_intent);
    VALIDATE_NOT_NULL(info);
    mStringName = NULL;

    AutoPtr<IIntent> intent = _intent;
    String target;
    info->GetTargetActivity(&target);
    if (target.IsNull()) {
        if (intent != NULL) {
            // If this Intent has a selector, we want to clear it for the
            // recent task since it is not relevant if the user later wants
            // to re-launch the app.
            AutoPtr<IIntent> tempIntent;
            intent->GetSelector((IIntent**)&tempIntent);
            AutoPtr<IRect> tempRect;
            intent->GetSourceBounds((IRect**)&tempRect);
            if (tempIntent != NULL || tempRect != NULL) {
                intent = NULL;
                CIntent::New(_intent, (IIntent**)&intent);
                intent->SetSelector(NULL);
                intent->SetSourceBounds(NULL);
            }
        }

        if (CActivityManagerService::DEBUG_TASKS) {
            String intentDes;
            if (intent != NULL)
                intent->ToString(&intentDes);
            Slogger::V(CActivityManagerService::TAG, "Setting Intent of %s to %s",
                ToString().string(), intentDes.string());
        }

        mIntent = intent;
        if (intent != NULL) {
            mRealActivity = NULL;
            intent->GetComponent((IComponentName**)&mRealActivity);
        }
        mOrigActivity = NULL;
    }
    else {
        String cname;
        info->GetPackageName(&cname);
        AutoPtr<IComponentName> targetComponent;
        CComponentName::New(cname, target,
                (IComponentName**)&targetComponent);
        if (intent != NULL) {
            AutoPtr<IIntent> targetIntent;
            CIntent::New(intent, (IIntent**)&targetIntent);
            targetIntent->SetComponent(targetComponent);
            targetIntent->SetSelector(NULL);
            targetIntent->SetSourceBounds(NULL);
            if (CActivityManagerService::DEBUG_TASKS) {
                String intentDes;
                targetIntent->ToString(&intentDes);
                Slogger::V(CActivityManagerService::TAG, "Setting Intent of %s to target %s",
                    ToString().string(), intentDes.string());
            }

            mIntent = targetIntent;
            mRealActivity = targetComponent;
            mOrigActivity = NULL;
            intent->GetComponent((IComponentName**)&mOrigActivity);
        }
        else {
            mIntent = NULL;
            mRealActivity = targetComponent;
            String name;
            info->GetName(&name);
            mOrigActivity = NULL;
            CComponentName::New(cname, name, (IComponentName**)&mOrigActivity);
        }
    }

    Int32 flags;
    if (mIntent != NULL && (mIntent->GetFlags(&flags),
            flags & IIntent::FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) != 0) {
        // Once we are set to an Intent with this flag, we count this
        // task as having a true root activity.
        mRootWasReset = TRUE;
    }

    AutoPtr<IApplicationInfo> appInfo;
    info->GetApplicationInfo((IApplicationInfo**)&appInfo);
    if (appInfo != NULL) {
        Int32 uid;
        appInfo->GetUid(&uid);
        mUserId = UserHandle::GetUserId(uid);
    }
    return NOERROR;
}

void TaskRecord::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    if (mNumActivities != 0 || mRootWasReset || mUserId != 0) {
        pw->PrintString(prefix); pw->PrintString(String("numActivities=")); pw->PrintInt32(mNumActivities);
                pw->PrintString(String(" rootWasReset=")); pw->PrintInt32(mRootWasReset);
                pw->PrintString(String(" userId=")); pw->PrintInt32ln(mUserId);
    }
    if (!mAffinity.IsNull()) {
        pw->PrintString(prefix); pw->PrintString(String("affinity=")); pw->PrintStringln(mAffinity);
    }
    if (mIntent != NULL) {
        StringBuilder sb;
        sb += prefix; sb += "intent={";
        mIntent->ToShortString((IStringBuilder*)&sb, FALSE, TRUE, FALSE, TRUE);
        sb += '}';
        pw->PrintStringln(sb.ToString());
    }
    if (mAffinityIntent != NULL) {
        StringBuilder sb;
        sb += prefix; sb += "affinityIntent={";
        mAffinityIntent->ToShortString((IStringBuilder*)&sb, FALSE, TRUE, FALSE, TRUE);
        sb += '}';
        pw->PrintStringln(sb.ToString());
    }
    if (mOrigActivity != NULL) {
        pw->PrintString(prefix); pw->PrintString(String("origActivity="));
        String shortStr;
        mOrigActivity->FlattenToShortString(&shortStr);
        pw->PrintStringln(shortStr);
    }
    if (mRealActivity != NULL) {
        pw->PrintString(prefix); pw->PrintString(String("realActivity="));
        String shortStr;
        mRealActivity->FlattenToShortString(&shortStr);
        pw->PrintStringln(shortStr);
    }
    if (!mAskedCompatMode) {
        pw->PrintString(prefix); pw->PrintString(String("askedCompatMode=")); pw->PrintInt32ln(mAskedCompatMode);
    }
    pw->PrintString(prefix); pw->PrintString(String("lastThumbnail=")); //pw->Print(lastThumbnail);
            pw->PrintString(String(" lastDescription=")); //pw->Println(lastDescription);
    pw->PrintString(prefix); pw->PrintString(String("lastActiveTime=")); pw->PrintInt64(mLastActiveTime);
            pw->PrintString(String(" (inactive for "));
            pw->PrintInt64((GetInactiveDuration()/1000)); pw->PrintStringln(String("s)"));
}

String TaskRecord::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }

    StringBuilder sb(128);
    sb += "TaskRecord{";
    sb += StringUtils::Int32ToHexString((Int32)this);
    sb += " #";
    sb += mTaskId;
    if (!mAffinity.IsNull()) {
        sb += (" Affinity ");
        sb += (mAffinity);
    }
    else if (mIntent != NULL) {
        sb += (" Intent ");
        AutoPtr<IComponentName> component;
        mIntent->GetComponent((IComponentName**)&component);
        String info;
        component->FlattenToShortString(&info);
        sb += info;
    }
    else if (mAffinityIntent != NULL) {
        sb += (" AffinityIntent ");
        AutoPtr<IComponentName> component;
        mAffinityIntent->GetComponent((IComponentName**)&component);
        String info;
        component->FlattenToShortString(&info);
        sb += info;
    }
    else {
        sb += (" ??");
    }
    sb += (" UserId ");
    sb += (mUserId);
    sb += ('}');
    mStringName = sb.ToString();
    return mStringName;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
