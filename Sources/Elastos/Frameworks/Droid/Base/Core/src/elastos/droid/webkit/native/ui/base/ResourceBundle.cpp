
#include "elastos/droid/webkit/native/ui/base/ResourceBundle.h"
#include "elastos/droid/content/res/CAssetManager.h"
#include "elastos/droid/content/res/CAssetFileDescriptor.h"

using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::CAssetManager;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Content::Res::CAssetFileDescriptor;

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
        CAssetManager::New((IAssetManager**)&am);
        ctx->GetAssets((IAssetManager**)&am);

        AutoPtr<IAssetFileDescriptor> afd;
        CAssetFileDescriptor::New((IAssetFileDescriptor**)&afd);
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


