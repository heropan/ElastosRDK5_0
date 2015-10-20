
#include "elastos/droid/preference/CSeekBarDialogPreferenceHelper.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Preference {

CAR_INTERFACE_IMPL(CSeekBarDialogPreferenceHelper, Singleton, ISeekBarDialogPreferenceHelper)

CAR_SINGLETON_IMPL(CSeekBarDialogPreferenceHelper)


ECode CSeekBarDialogPreferenceHelper::GetSeekBar(
        /* [in] */ IView* dialogView,
        /* [out] */ ISeekBar** bar)
{
    VALIDATE_NOT_NULL(bar)
    AutoPtr<IView> v;
    dialogView->FindViewById(R::id::seekbar, (IView**)&v);
    AutoPtr<ISeekBar> seekBar = ISeekBar::Probe(v);
    *bar = seekBar;
    REFCOUNT_ADD(*bar)
    return NOERROR;
}

} // namespace Preference
} // namespace Droid
} // namespace Elastos
