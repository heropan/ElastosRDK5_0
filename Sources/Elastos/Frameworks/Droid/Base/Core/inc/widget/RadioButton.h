
#ifndef __ELASTOS_DROID_WIDGET_RADIOBUTTON_H__
#define __ELASTOS_DROID_WIDGET_RADIOBUTTON_H__

#include "widget/CompoundButton.h"

namespace Elastos {
namespace Droid {
namespace Widget {

class RadioButton : public CompoundButton
{
public:

    RadioButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::radioButtonStyle);

    //@Override
    virtual CARAPI Toggle();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    RadioButton();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::radioButtonStyle);
};


} // namespace Widget
} // namepsace Droid
} // namespace Elastos

#endif //__BUTTON_H__
