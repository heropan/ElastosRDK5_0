
#ifndef __ELASTOS_DROID_PREFERENCE_CPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_CPREFERENCE_H__

#include "Preference.h"
#include "_CPreference.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreference) , public Preference
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CPREFERENCE_H__
