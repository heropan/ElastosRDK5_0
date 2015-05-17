
#include "cmdef.h"
#include "CThreadGroup.h"
#include "Thread.h"

namespace Elastos {
namespace Core {
namespace Threading {

AutoPtr<IThreadGroup> CThreadGroup::sSystem;
AutoPtr<IThreadGroup> CThreadGroup::sMain;

ECode CThreadGroup::constructor()
{
    return ThreadGroup::Init();
}

ECode CThreadGroup::constructor(
    /* [in] */ const String& name)
{
    AutoPtr<IThreadGroup> threadGroup;
    Thread::GetCurrentThread()->GetThreadGroup((IThreadGroup**)&threadGroup);
    return constructor(threadGroup, name);
}

ECode CThreadGroup::constructor(
    /* [in] */ IThreadGroup* parent,
    /* [in] */ const String& name)
{
    return ThreadGroup::Init(parent, name);
}

PInterface CThreadGroup::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ThreadGroup) {
        return reinterpret_cast<PInterface>((ThreadGroup*)this);
    }
    else
        return _CThreadGroup::Probe(riid);
}

ECode CThreadGroup::ActiveCount(
    /* [out] */ Int32* number)
{
    return ThreadGroup::ActiveCount(number);
}

ECode CThreadGroup::ActiveGroupCount(
    /* [out] */ Int32* number)
{
    return ThreadGroup::ActiveGroupCount(number);
}

ECode CThreadGroup::AllowThreadSuspension(
    /* [in] */ Boolean b,
    /* [out] */ Boolean* result)
{
    return ThreadGroup::AllowThreadSuspension(b, result);
}

ECode CThreadGroup::CheckAccess()
{
    return ThreadGroup::CheckAccess();
}

ECode CThreadGroup::Destroy()
{
    return ThreadGroup::Destroy();
}

ECode CThreadGroup::EnumerateThread(
    /* [out] */ ArrayOf<IThread*>* threads,
    /* [out] */ Int32* number)
{
    return ThreadGroup::EnumerateThread(threads, number);
}

ECode CThreadGroup::EnumerateThreadEx(
    /* [in] */ Boolean recurse,
    /* [out] */ ArrayOf<IThread*>* threads,
    /* [out] */ Int32* number)
{
    return ThreadGroup::EnumerateThreadEx(recurse, threads, number);
}

ECode CThreadGroup::EnumerateThreadGroup(
    /* [out] */ ArrayOf<IThreadGroup*>* groups,
    /* [out] */ Int32* number)
{
    return ThreadGroup::EnumerateThreadGroup(groups, number);
}

ECode CThreadGroup::EnumerateThreadGroupEx(
    /* [in] */ Boolean recurse,
    /* [out] */ ArrayOf<IThreadGroup*>* groups,
    /* [out] */ Int32* number)
{
    return ThreadGroup::EnumerateThreadGroupEx(recurse, groups, number);
}

ECode CThreadGroup::GetMaxPriority(
    /* [out] */ Int32* maxPriority)
{
    return ThreadGroup::GetMaxPriority(maxPriority);
}

ECode CThreadGroup::GetName(
    /* [out] */ String* name)
{
    return ThreadGroup::GetName(name);
}

ECode CThreadGroup::GetParent(
    /* [out] */ IThreadGroup** parent)
{
    return ThreadGroup::GetParent(parent);
}

ECode CThreadGroup::Interrupt()
{
    return ThreadGroup::Interrupt();
}

ECode CThreadGroup::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return ThreadGroup::IsDaemon(isDaemon);
}

ECode CThreadGroup::IsDestroyed(
    /* [out] */ Boolean* isDestroyed)
{
    return ThreadGroup::IsDestroyed(isDestroyed);
}

ECode CThreadGroup::List()
{
    return ThreadGroup::List();
}

ECode CThreadGroup::IsParentOf(
    /* [in] */ IThreadGroup* g,
    /* [out] */ Boolean* result)
{
    return ThreadGroup::IsParentOf(g, result);
}

ECode CThreadGroup::Resume()
{
    return ThreadGroup::Resume();
}

ECode CThreadGroup::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return ThreadGroup::SetDaemon(isDaemon);
}

ECode CThreadGroup::SetMaxPriority(
    /* [in] */ Int32 newMax)
{
    return ThreadGroup::SetMaxPriority(newMax);
}

ECode CThreadGroup::Stop()
{
    return ThreadGroup::Stop();
}

ECode CThreadGroup::Suspend()
{
    return ThreadGroup::Suspend();
}

AutoPtr<IThreadGroup> CThreadGroup::GetSystemThreadGroup()
{
    if (sSystem == NULL) {
        AutoPtr<CThreadGroup> group;
        ASSERT_SUCCEEDED(CThreadGroup::NewByFriend((CThreadGroup**)&group));
        sSystem = (IThreadGroup*)group.Get();
    }
    return sSystem;
}

AutoPtr<IThreadGroup> CThreadGroup::GetMainThreadGroup()
{
    if (sMain == NULL) {
        if (sSystem == NULL) {
            AutoPtr<CThreadGroup> group;
            ASSERT_SUCCEEDED(CThreadGroup::NewByFriend((CThreadGroup**)&group));
            sSystem = (IThreadGroup*)group.Get();
        }
        AutoPtr<CThreadGroup> group;
        ASSERT_SUCCEEDED(CThreadGroup::NewByFriend(sSystem, String("main"), (CThreadGroup**)&group));
        sMain = (IThreadGroup*)group.Get();
    }
    return sMain;
}

} // namespace Threading
} // namespace Core
} // namespace Elastos
