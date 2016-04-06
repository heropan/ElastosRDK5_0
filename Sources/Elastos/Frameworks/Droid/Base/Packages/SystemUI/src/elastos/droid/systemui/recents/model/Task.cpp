
#include "elastos/droid/systemui/recents/model/TaskGrouping.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Graphics::IColor;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recents {
namespace Model {

CAR_INTERFACE_IMPL(Task::ComponentNameKey, Object, IComponentNameKey)

Task::ComponentNameKey::ComponentNameKey(
    /* [in] */ IComponentName* cn,
    /* [in] */ Int32 user)
    : mComponent(cn)
    , mUserId(user)
{
}

// @Override
ECode Task::ComponentNameKey::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    // return Objects.hash(mComponent, mUserId);
    *hashCode = 1;
    if (mComponent != NULL) {
        IObject::Probe(mComponent)->GetHashCode(hashCode);
    }
    *hashCode = 31 * (*hashCode) + mUserId;
    return NOERROR;
}

// @Override
ECode Task::ComponentNameKey::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!IComponentNameKey::Probe(o)) {
        *result = FALSE;
        return NOERROR;
    }
    ComponentNameKey* other = (ComponentNameKey*)IComponentNameKey::Probe(o);
    Boolean res;
    *result = (IObject::Probe(mComponent)->Equals(other->mComponent, &res), res) &&
            mUserId == other->mUserId;
    return NOERROR;
}

CAR_INTERFACE_IMPL(Task::TaskKey, Object, ITaskKey)

Task::TaskKey::TaskKey(
    /* [in] */ Int32 id,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 userId,
    /* [in] */ Int64 firstActiveTime,
    /* [in] */ Int64 lastActiveTime)
    : mId(id)
    , mBaseIntent(intent)
    , mUserId(userId)
    , mFirstActiveTime(firstActiveTime)
    , mLastActiveTime(lastActiveTime)
{
    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    mComponentNameKey = new ComponentNameKey(component, mUserId);
}

/** Returns the component name key for this task. */
AutoPtr<Task::ComponentNameKey> Task::TaskKey::GetComponentNameKey()
{
    return mComponentNameKey;
}

ECode Task::TaskKey::GetComponentNameKey(
    /* [out] */ IComponentNameKey** cnk)
{
    VALIDATE_NOT_NULL(cnk)
    *cnk = mComponentNameKey;
    REFCOUNT_ADD(*cnk)
    return NOERROR;
}

// @Override
ECode Task::TaskKey::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!ITaskKey::Probe(o)) {
        *result = FALSE;
        return NOERROR;
    }
    TaskKey* other = (TaskKey*)ITaskKey::Probe(o);
    *result = mId == other->mId && mUserId == other->mUserId;
    return NOERROR;
}

// @Override
ECode Task::TaskKey::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (mId << 5) + mUserId;
    return NOERROR;
}

// @Override
ECode Task::TaskKey::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb("Task.Key: ");
    sb += mId;
    sb += ", ";
    sb += "u: ";
    sb += mUserId;
    sb += ", ";
    sb += "lat: ";
    sb += mLastActiveTime;
    sb += ", ";
    AutoPtr<IComponentName> component;
    mBaseIntent->GetComponent((IComponentName**)&component);
    String pkgName;
    component->GetPackageName(&pkgName);
    sb += pkgName;
    return sb.ToString(str);
}

CAR_INTERFACE_IMPL(Task, Object, ITask)

Task::Task()
    : mTaskAffiliation(0)
    , mTaskAffiliationColor(0)
    , mIsLaunchTarget(FALSE)
    , mColorPrimary(0)
    , mUseLightOnPrimaryColor(FALSE)
    , mIsActive(FALSE)
    , mLockToThisTask(FALSE)
    , mLockToTaskEnabled(FALSE)
{
    // Only used by RecentsService for task rect calculations.
}

Task::Task(
    /* [in] */ TaskKey* key,
    /* [in] */ Boolean isActive,
    /* [in] */ Int32 taskAffiliation,
    /* [in] */ Int32 taskAffiliationColor,
    /* [in] */ const String& activityTitle,
    /* [in] */ IDrawable* activityIcon,
    /* [in] */ Int32 colorPrimary,
    /* [in] */ Boolean lockToThisTask,
    /* [in] */ Boolean lockToTaskEnabled,
    /* [in] */ IBitmap* icon,
    /* [in] */ const String& iconFilename)
    : mKey(key)
    , mTaskAffiliation(taskAffiliation)
    , mTaskAffiliationColor(taskAffiliationColor)
    , mIsLaunchTarget(FALSE)
    , mActivityIcon(activityIcon)
    , mActivityLabel(activityTitle)
    , mIsActive(isActive)
    , mLockToThisTask(lockToTaskEnabled && lockToThisTask)
    , mLockToTaskEnabled(lockToTaskEnabled)
    , mIcon(icon)
    , mIconFilename(iconFilename)
{
    Boolean isInAffiliationGroup = (taskAffiliation != key->mId);
    Boolean hasAffiliationGroupColor = isInAffiliationGroup && (taskAffiliationColor != 0);
    mColorPrimary = hasAffiliationGroupColor ? taskAffiliationColor : colorPrimary;
    assert(0);
    // mUseLightOnPrimaryColor = Utilities::ComputeContrastBetweenColors(mColorPrimary,
    //         IColor::WHITE) > 3.0f;
}

/** Copies the other task. */
ECode Task::CopyFrom(
    /* [in] */ ITask* _o)
{
    AutoPtr<Task> o = (Task*)_o;
    mKey = o->mKey;
    mTaskAffiliation = o->mTaskAffiliation;
    mTaskAffiliationColor = o->mTaskAffiliationColor;
    mActivityLabel = o->mActivityLabel;
    mActivityIcon = o->mActivityIcon;
    mColorPrimary = o->mColorPrimary;
    mUseLightOnPrimaryColor = o->mUseLightOnPrimaryColor;
    mIsActive = o->mIsActive;
    mLockToThisTask = o->mLockToThisTask;
    mLockToTaskEnabled = o->mLockToTaskEnabled;
    return NOERROR;
}

/** Set the callbacks */
ECode Task::SetCallbacks(
    /* [in] */ ITaskCallbacks* cb)
{
    mCb = cb;
    return NOERROR;
}

/** Set the grouping */
ECode Task::SetGroup(
    /* [in] */ TaskGrouping* group)
{
    if (group != NULL && mGroup != NULL) {
        Logger::E("Task", "This task is already assigned to a group.");
        return E_RUNTIME_EXCEPTION;
    }
    mGroup = (TaskGrouping*)group;
    return NOERROR;
}

/** Notifies the callback listeners that this task has been loaded */
ECode Task::NotifyTaskDataLoaded(
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ IDrawable* applicationIcon)
{
    mApplicationIcon = applicationIcon;
    mThumbnail = thumbnail;
    if (mCb != NULL) {
        mCb->OnTaskDataLoaded();
    }
    return NOERROR;
}

/** Notifies the callback listeners that this task has been unloaded */
ECode Task::NotifyTaskDataUnloaded(
    /* [in] */ IBitmap* defaultThumbnail,
    /* [in] */ IDrawable* defaultApplicationIcon)
{
    mApplicationIcon = defaultApplicationIcon;
    mThumbnail = defaultThumbnail;
    if (mCb != NULL) {
        mCb->OnTaskDataUnloaded();
    }
    return NOERROR;
}

// @Override
ECode Task::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Check that the id matches
    if (!ITask::Probe(o)) {
        *result = FALSE;
        return NOERROR;
    }
    Task* t = (Task*)ITask::Probe(o);
    return mKey->Equals((IComponentNameKey*)t->mKey.Get(), result);
}

// @Override
ECode Task::ToString(
    /* [out] */ String* str)
{
     VALIDATE_NOT_NULL(str);
    String groupAffiliation("no group");
    if (mGroup != NULL) {
        groupAffiliation = StringUtils::ToString(mGroup->mAffiliation);
    }
    StringBuilder sb;
    sb += "Task (";
    sb += groupAffiliation;
    sb += "): ";
    AutoPtr<IComponentName> component;
    mKey->mBaseIntent->GetComponent((IComponentName**)&component);
    String pkgName;
    component->GetPackageName(&pkgName);
    sb += pkgName;
    sb += " [";
    sb += (Int32)this;
    sb += "]";
    return sb.ToString(str);
}

} // namespace Model
} // namespace Recents
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
