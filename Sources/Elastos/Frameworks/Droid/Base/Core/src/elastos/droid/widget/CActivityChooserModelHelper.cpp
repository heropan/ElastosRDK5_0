#include "elastos/droid/widget/CActivityChooserModelHelper.h"
#include "elastos/droid/widget/ActivityChooserModel.h"

namespace Elastos {
namespace Droid {
namespace Widget {

ECode CActivityChooserModelHelper::constructor()
{
    return NOERROR;
}

ECode CActivityChooserModelHelper::Get(
    /* [in] */ IContext* context,
    /* [in] */ const String& historyFileName,
    /* [out] */ IActivityChooserModel** result)
{
    AutoPtr<IActivityChooserModel> activity =
        ActivityChooserModel::Get(context, historyFileName);
    *result = activity;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos