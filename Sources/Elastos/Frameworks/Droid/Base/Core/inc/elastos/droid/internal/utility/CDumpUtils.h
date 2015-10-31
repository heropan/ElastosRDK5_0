#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_CDUMPUTILS_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_CDUMPUTILS_H__

#include "_Elastos_Droid_Internal_Utility_CDumpUtils.h"
#include "elastos/droid/os/Runnable.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Core::Singleton;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IStringWriter;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

CarClass(CDumpUtils)
    , public Singleton
    , public IDumpUtils
{
private:
    class DumpRunnable : public Runnable
    {
    public:
        DumpRunnable(
            /* [in] */ IDump* dump,
            /* [in] */ IStringWriter* sw);

        CARAPI Run();

    private:
        AutoPtr<IDump> mDump;
        AutoPtr<IStringWriter> mSw;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Helper for dumping state owned by a handler thread.
     *
     * Because the caller might be holding an important lock that the handler is
     * trying to acquire, we use a short timeout to avoid deadlocks.  The process
     * is inelegant but this function is only used for debugging purposes.
     */
    static CARAPI_(void) _DumpAsync(
        /* [in] */ IHandler* handler,
        /* [in] */ IDump* dump,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int64 timeout);

    CARAPI DumpAsync(
        /* [in] */ IHandler* handler,
        /* [in] */ IDump* dump,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int64 timeout);
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_CDUMPUTILS_H__
