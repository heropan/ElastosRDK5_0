
#ifndef __ELASTOS_DROID_TEXT_METHOD_CMULTITAPKEYLISTENER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CMULTITAPKEYLISTENER_H__

#include "_CMultiTapKeyListener.h"
#include "text/method/MultiTapKeyListener.h"



using Elastos::Droid::Text::Method::Capitalize;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CMultiTapKeyListener), public MultiTapKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()

    CARAPI OnSpanAdded(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanRemoved(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 ostart,
        /* [in] */ Int32 oend,
        /* [in] */ Int32 nstart,
        /* [in] */ Int32 nend);

    CARAPI constructor(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

    /**
     * Returns a new or existing instance with the specified capitalization
     * and correction properties.
     */
    static AutoPtr<IMultiTapKeyListener> GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap);

private:
    static const Int32 CAPITALIZELENGTH = 4;
    static AutoPtr< IMultiTapKeyListener > sInstance[CAPITALIZELENGTH*2];// = new MultiTapKeyListener[Capitalize.values().length * 2];
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CMULTITAPKEYLISTENER_H__
