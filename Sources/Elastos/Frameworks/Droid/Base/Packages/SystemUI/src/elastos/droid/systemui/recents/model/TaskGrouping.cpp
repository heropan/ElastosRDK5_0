
#include "elastos/droid/systemui/recents/model/TaskGrouping.h"

using Elastos::Utility::CArrayList;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recents {
namespace Model {

CAR_INTERFACE_IMPL(TaskGrouping, Object, ITaskGrouping)

/** Creates a group with a specified affiliation. */
TaskGrouping::TaskGrouping(
    /* [in] */ Int32 affiliation)
    : mAffiliation(affiliation)
    , mLatestActiveTimeInGroup(0)
{
    CArrayList::New((IList**)&mTaskKeys);
}

/** Adds a new task to this group. */
void TaskGrouping::AddTask(
    /* [in] */ Task* t)
{
    mTaskKeys->Add((ITaskKey*)t->mKey);
    if (t->mKey->mLastActiveTime > mLatestActiveTimeInGroup) {
        mLatestActiveTimeInGroup = t->mKey->mLastActiveTime;
    }
    t->SetGroup(this);
    UpdateTaskIndices();
}

/** Removes a task from this group. */
void TaskGrouping::RemoveTask(
    /* [in] */ Task* t)
{
    mTaskKeys->Remove((ITaskKey*)t->mKey);
    mLatestActiveTimeInGroup = 0;
    Int32 taskCount;
    mTaskKeys->GetSize(&taskCount);
    for (Int32 i = 0; i < taskCount; i++) {
        AutoPtr<IInterface> item;
        mTaskKeys->Get(i, (IInterface**)&item);
        Int64 lastActiveTime = ((Task::TaskKey*)ITaskKey::Probe(item))->mLastActiveTime;
        if (lastActiveTime > mLatestActiveTimeInGroup) {
            mLatestActiveTimeInGroup = lastActiveTime;
        }
    }
    t->SetGroup(NULL);
    UpdateTaskIndices();
}

/** Returns the key of the next task in the group. */
AutoPtr<Task::TaskKey> TaskGrouping::GetNextTaskInGroup(
    /* [in] */ Task* t)
{
    Int32 i = IndexOf(t);
    if ((i + 1) < GetTaskCount()) {
        AutoPtr<IInterface> item;
        mTaskKeys->Get(i + 1, (IInterface**)&item);
        return (Task::TaskKey*)ITaskKey::Probe(item);
    }
    return NULL;
}

/** Returns the key of the previous task in the group. */
AutoPtr<Task::TaskKey> TaskGrouping::GetPrevTaskInGroup(
    /* [in] */ Task* t)
{
    Int32 i = IndexOf(t);
    if ((i - 1) >= 0) {
        AutoPtr<IInterface> item;
        mTaskKeys->Get(i - 1, (IInterface**)&item);
        return (Task::TaskKey*)ITaskKey::Probe(item);
    }
    return NULL;
}

/** Gets the front task */
Boolean TaskGrouping::IsFrontMostTask(
    /* [in] */ Task* t)
{
    return (t->mKey == mFrontMostTaskKey);
}

/** Finds the index of a given task in a group. */
Int32 TaskGrouping::IndexOf(
    /* [in] */ Task* t)
{
    HashMap<AutoPtr<ITaskKey>, Int32>::Iterator find = mTaskKeyIndices.Find(t->mKey);
    return find != mTaskKeyIndices.End() ? find->mSecond : -1;
}

/** Returns whether a task is in this grouping. */
Boolean TaskGrouping::ContainsTask(
    /* [in] */ Task* t)
{
    return mTaskKeyIndices.Find(t->mKey) != mTaskKeyIndices.End();
}

/** Returns whether one task is above another in the group.  If they are not in the same group,
 * this returns false. */
Boolean TaskGrouping::IsTaskAboveTask(
    /* [in] */ Task* t,
    /* [in] */ Task* below)
{
    return ContainsTask(t) && ContainsTask(below) && IndexOf(t) > IndexOf(below);
}

/** Returns the number of tasks in this group. */
Int32 TaskGrouping::GetTaskCount()
{
    Int32 size;
    mTaskKeys->GetSize(&size);
    return size;
}

/** Updates the mapping of tasks to indices. */
void TaskGrouping::UpdateTaskIndices()
{
    Int32 taskCount;
    mTaskKeys->GetSize(&taskCount);
    if (taskCount == 0) {
        mFrontMostTaskKey = NULL;
        mTaskKeyIndices.Clear();
        return;
    }

    AutoPtr<IInterface> item;
    mTaskKeys->Get(taskCount - 1, (IInterface**)&item);
    mFrontMostTaskKey = (Task::TaskKey*)ITaskKey::Probe(item);
    mTaskKeyIndices.Clear();
    for (Int32 i = 0; i < taskCount; i++) {
        item = NULL;
        mTaskKeys->Get(i, (IInterface**)&item);
        mTaskKeyIndices[ITaskKey::Probe(item)] = i;
    }
}

} // namespace Model
} // namespace Recents
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
