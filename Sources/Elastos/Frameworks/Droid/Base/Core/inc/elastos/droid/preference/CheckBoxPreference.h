
#ifndef __ELASTOS_DROID_PREFERENCE_CHECKBOXPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_CHECKBOXPREFERENCE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/preference/TwoStatePreference.h"

using Elastos::Droid::Preference::IPreference;
using Elastos::Droid::Widget::ICheckable;

namespace Elastos {
namespace Droid {
namespace Preference {

class CheckBoxPreference
    : public TwoStatePreference
    , public ICheckBoxPreference
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI OnBindView(
        /* [in]  */ IView* view);
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CHECKBOXPREFERENCE_H__
