
#ifndef __DROIDRUNTIME_H__
#define __DROIDRUNTIME_H__

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
        /* [in] */ const char* moduleName,
        /* [in] */ const char* className,
        /* [in] */ const char* options);

    static CARAPI_(AutoPtr<DroidRuntime>) GetRuntime();

    /**
     * This gets called after the JavaVM has initialized.  Override it
     * with the system's native entry point.
     */
    virtual CARAPI_(void) OnStarted() = 0;
};

} // namespace Droid
} // namespace Elastos

#endif //__DROIDRUNTIME_H__
