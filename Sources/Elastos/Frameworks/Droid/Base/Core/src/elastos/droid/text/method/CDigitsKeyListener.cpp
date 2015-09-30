
#include "elastos/droid/text/method/CDigitsKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListener_METHODS_IMPL(CDigitsKeyListener, DigitsKeyListener, DigitsKeyListener)
IKeyListener_METHODS_IMPL(CDigitsKeyListener, DigitsKeyListener, DigitsKeyListener)
IInputFilter_METHODS_IMPL(CDigitsKeyListener, DigitsKeyListener, DigitsKeyListener)


PInterface CDigitsKeyListener::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_DigitsKeyListener) {
        return reinterpret_cast<PInterface>((DigitsKeyListener*)this);
    }
    return _CDigitsKeyListener::Probe(riid);
}

ECode CDigitsKeyListener::constructor()
{
    Init();
    return NOERROR;
}

ECode CDigitsKeyListener::constructor(
    /* [in] */ Boolean sign,
    /* [in] */ Boolean decimal)
{
    Init(sign, decimal);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

