
#include "text/method/CArrowKeyMovementMethod.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

AutoPtr<IArrowKeyMovementMethod> CArrowKeyMovementMethod::sInstance;

IBASEMOVEMENTMETHOD_METHODS_IMPL(CArrowKeyMovementMethod, ArrowKeyMovementMethod, ArrowKeyMovementMethod)

AutoPtr<IMovementMethod> CArrowKeyMovementMethod::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CArrowKeyMovementMethod> akmm;
        CArrowKeyMovementMethod::NewByFriend((CArrowKeyMovementMethod**)&akmm);
        sInstance = (IArrowKeyMovementMethod*)(akmm.Get());
    }
    AutoPtr<IMovementMethod> ret = IMovementMethod::Probe(sInstance);
    return ret;
}

/*
void CArrowKeyMovementMethod::RefShow()
{
    printf("==== CArrowKeyMovementMethod Object 's mRef Value:%d\n", m_cRef);
}
*/

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

