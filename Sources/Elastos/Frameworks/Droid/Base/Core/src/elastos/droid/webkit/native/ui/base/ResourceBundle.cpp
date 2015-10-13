
#include "elastos/droid/webkit/native/ui/base/ResourceBundle.h"

using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;

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
    //try {
        AutoPtr<IAssetManager> am;
        ctx->GetAssets((IAssetManager**)&am);

        AutoPtr<IAssetFileDescriptor> afd;
        am->OpenFd(filename, (IAssetFileDescriptor**)&afd);
        afd->Close();
        return TRUE;
    //} catch (IOException e) {
    //    return false;
    //}
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


