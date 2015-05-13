#ifndef __CACTIVITYCHOOSERMODELHELPER_H__
#define __CACTIVITYCHOOSERMODELHELPER_H__

#include "_CActivityChooserModelHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CActivityChooserModelHelper)
{
public:
    CARAPI constructor();

    CARAPI Get(
        /* [in] */ IContext* context,
        /* [in] */ const String& historyFileName,
        /* [out] */ IActivityChooserModel** result);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif