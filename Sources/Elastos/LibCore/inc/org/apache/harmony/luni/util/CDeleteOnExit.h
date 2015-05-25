
#ifndef __CDELETEONEXIT_H__
#define __CDELETEONEXIT_H__

#include "_CDeleteOnExit.h"
#include <elastos/Mutex.h>
#include <elastos/ThreadBase.h>

using Elastos::Core::IClassLoader;
using Elastos::Core::Mutex;
using Elastos::Core::IRunnable;
using Elastos::Core::ThreadState;
using Elastos::Core::IThreadGroup;
using Elastos::Core::IThreadUncaughtExceptionHandler;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Core::ThreadBase;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Luni {
namespace Util {

CarClass(CDeleteOnExit), public ThreadBase
{
public:
    static CARAPI GetInstance(
    /* [out] */ IDeleteOnExit** instance);

    CARAPI AddFile(
        /* [in] */ const String& filename);

    CDeleteOnExit();

    CARAPI Run();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid);

private:
    static Mutex sLockLock;
    Mutex mLockLock;
    /**
     * Our singleton instance.
     */
    static AutoPtr<IDeleteOnExit> sInstance;

    /**
     * Our list of files scheduled for deletion.
     */
    AutoPtr<IArrayList> mFiles;// = new ArrayList<String>();
};

} // namespace Util
} // namespace Luni
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CDELETEONEXIT_H__
