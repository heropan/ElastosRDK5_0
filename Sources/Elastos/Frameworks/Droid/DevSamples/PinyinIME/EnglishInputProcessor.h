
#ifndef  __ENGLISHINPUTPROCESSOR_H__
#define  __ENGLISHINPUTPROCESSOR_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::InputMethod::IInputConnection;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace PinyinIME {

/**
 * Class to handle English input.
 */
class EnglishInputProcessor: public ElRefBase
{
public:
    EnglishInputProcessor();

    CARAPI_(Boolean) ProcessKey(
        /* [in] */ IInputConnection* inputContext,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Boolean upperCase,
        /* [in] */ Boolean realAction);

private:
    Int32 mLastKeyCode;
};

} // namespace PinyinIME
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif  // __ENGLISHINPUTPROCESSOR_H__
