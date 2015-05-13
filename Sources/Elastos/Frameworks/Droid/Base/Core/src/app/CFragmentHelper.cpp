
#include "app/CFragmentHelper.h"

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

ECode CFragmentHelper::InstantiateEx(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname,
    /* [in] */ IBundle* args,
    /* [out] */ IFragment** fragment)
{
    return Fragment::InstantiateEx(context, fname, args, fragment);
}

} // namespace App
} // namespace Droid
} // namespace Elastos