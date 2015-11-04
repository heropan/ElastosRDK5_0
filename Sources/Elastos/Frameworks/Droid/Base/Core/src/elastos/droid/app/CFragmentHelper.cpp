
#include "elastos/droid/app/CFragmentHelper.h"
#include "elastos/droid/app/Fragment.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CFragmentHelper::Instantiate(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname,
    /* [out] */ IFragment** fragment)
{
    return Fragment::Instantiate(context, fname, fragment);
}

ECode CFragmentHelper::Instantiate(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname,
    /* [in] */ IBundle* args,
    /* [out] */ IFragment** fragment)
{
    return Fragment::Instantiate(context, fname, args, fragment);
}

} // namespace App
} // namespace Droid
} // namespace Elastos