
#ifndef  __CSOUNDMANAGERHELPER_H__
#define  __CSOUNDMANAGERHELPER_H__

#include "_CSoundManagerHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Inputmethods::PinyinIME::ISoundManager;

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

CarClass(CSoundManagerHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ ISoundManager** sm);

private:
    static AutoPtr<ISoundManager> mInstance;
};

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos

#endif  //__CSOUNDMANAGERHELPER_H__
