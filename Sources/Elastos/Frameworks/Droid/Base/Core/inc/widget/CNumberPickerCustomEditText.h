#ifndef __CNUMBERPICKERCUSTOMEDITTEXT__H_
#define __CNUMBERPICKERCUSTOMEDITTEXT__H_

#include "_CNumberPickerCustomEditText.h"
#include "widget/NumberPickerCustomEditText.h"



namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CNumberPickerCustomEditText) , public NumberPickerCustomEditText
{
public:
    IVIEW_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    CARAPI SetSelection(
        /* [in] */ Int32 index);

    CARAPI SelectAll();

    CARAPI ExtendSelection(
        /* [in] */ Int32 index);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CNUMBERPICKERCUSTOMEDITTEXT__H_