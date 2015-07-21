
#ifndef __ELASTOS_DROID_DROIDRUNTIME_H__
#define __ELASTOS_DROID_DROIDRUNTIME_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {

class DroidRuntime : public ElLightRefBase
{
public:
    DroidRuntime();

    virtual ~DroidRuntime();

    CARAPI CallMain(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className,
        /* [in] */ ArrayOf<String>* args);

    CARAPI_(void) Start(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className,
        /* [in] */ const String& options);

    static CARAPI_(AutoPtr<DroidRuntime>) GetRuntime();

    /**
     * This gets called after the JavaVM has initialized.  Override it
     * with the system's native entry point.
     */
    virtual CARAPI_(void) OnStarted() = 0;
};

} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_DROIDRUNTIME_H__
