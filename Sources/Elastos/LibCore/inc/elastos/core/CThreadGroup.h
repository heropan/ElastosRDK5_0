
#ifndef __CTHREADGROUP_H__
#define __CTHREADGROUP_H__

#include "_Elastos_Core_Threading_CThreadGroup.h"
#include "ThreadGroup.h"

namespace Elastos {
namespace Core {
namespace Threading {

CarClass(CThreadGroup), ThreadGroup
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ IThreadGroup* parent,
        /* [in] */ const String& name);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI ActiveCount(
        /* [out] */ Int32* number);

    CARAPI ActiveGroupCount(
        /* [out] */ Int32* number);

    CARAPI AllowThreadSuspension(
        /* [in] */ Boolean b,
        /* [out] */ Boolean* result);

    CARAPI CheckAccess();

    CARAPI Destroy();

    CARAPI EnumerateThread(
        /* [out] */ ArrayOf<IThread*>* threads,
        /* [out] */ Int32* number);

    CARAPI EnumerateThreadEx(
        /* [in] */ Boolean recurse,
        /* [out] */ ArrayOf<IThread*>* threads,
        /* [out] */ Int32* number);

    CARAPI EnumerateThreadGroup(
        /* [out] */ ArrayOf<IThreadGroup*>* groups,
        /* [out] */ Int32* number);

    CARAPI EnumerateThreadGroupEx(
        /* [in] */ Boolean recurse,
        /* [out] */ ArrayOf<IThreadGroup*>* groups,
        /* [out] */ Int32* number);

    CARAPI GetMaxPriority(
        /* [out] */ Int32* maxPriority);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetParent(
        /* [out] */ IThreadGroup** parent);

    CARAPI Interrupt();

    CARAPI IsDaemon(
        /* [out] */ Boolean* isDaemon);

    CARAPI IsDestroyed(
        /* [out] */ Boolean* isDestroyed);

    CARAPI List();

    CARAPI IsParentOf(
        /* [in] */ IThreadGroup* g,
        /* [out] */ Boolean* result);

    CARAPI Resume();

    CARAPI SetDaemon(
        /* [in] */ Boolean isDaemon);

    CARAPI SetMaxPriority(
        /* [in] */ Int32 newMax);

    CARAPI Stop();

    CARAPI Suspend();

    static CARAPI_(AutoPtr<IThreadGroup>) GetSystemThreadGroup();

    static CARAPI_(AutoPtr<IThreadGroup>) GetMainThreadGroup();

private:
    // BEGIN android-added
    /* the VM uses these directly; do not rename */
    static AutoPtr<IThreadGroup> sSystem;
    static AutoPtr<IThreadGroup> sMain;
    // END android-added
};

} // namespace Threading
} // namespace Core
} // namespace Elastos

#endif //__CTHREADGROUP_H__
