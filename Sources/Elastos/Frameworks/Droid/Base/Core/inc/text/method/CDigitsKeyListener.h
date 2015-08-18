
#ifndef __ELASTOS_DROID_TEXT_METHOD_CDIGITSKEYLISTENER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CDIGITSKEYLISTENER_H__

#include "_Elastos_Droid_Text_Method_CDigitsKeyListener.h"

#include "text/method/DigitsKeyListener.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDigitsKeyListener)
    , public DigitsKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()
    IInputFilter_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean sign,
        /* [in] */ Boolean decimal);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CDIGITSKEYLISTENER_H__
