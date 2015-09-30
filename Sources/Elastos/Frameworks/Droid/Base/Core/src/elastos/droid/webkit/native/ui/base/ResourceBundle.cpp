
#include "elastos/droid/webkit/native/ui/base/ResourceBundle.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                            ResourceBundle
//=====================================================================
Boolean ResourceBundle::AssetContainedInApk(
    /* [in] */ IContext* ctx,
    /* [in] */ const String& filename)
{
    // ==================before translated======================
    // try {
    //     AssetManager am = ctx.getAssets();
    //     AssetFileDescriptor afd = am.openFd(filename);
    //     afd.close();
    //     return true;
    // } catch (IOException e) {
    //     return false;
    // }
    assert(0);
    return FALSE;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


