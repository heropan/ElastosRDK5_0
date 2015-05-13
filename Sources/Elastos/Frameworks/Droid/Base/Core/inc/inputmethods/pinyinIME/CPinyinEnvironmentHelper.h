
#ifndef  __CPINYINENVIRONMENTHELPER_H__
#define  __CPINYINENVIRONMENTHELPER_H__

#include "_CPinyinEnvironmentHelper.h"

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

CarClass(CPinyinEnvironmentHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IPinyinEnvironment** env);

private:
    /**
     * The configurations are managed in a singleton.
     */
    static AutoPtr<IPinyinEnvironment> mInstance;
};

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos

#endif  // __CPINYINENVIRONMENTHELPER_H__
