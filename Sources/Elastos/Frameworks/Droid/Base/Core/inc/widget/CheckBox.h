
#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "widget/CompoundButton.h"

namespace Elastos{
namespace Droid{
namespace Widget{

class CheckBox : public CompoundButton
{
public:
    CheckBox(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::checkboxStyle);

protected:
    CheckBox();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::checkboxStyle);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif //__CHECKBOXBUTTON_H__
