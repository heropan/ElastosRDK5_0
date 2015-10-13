#include "elastos/droid/webkit/native/android_webview/AwWebContentsDelegate.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

// Call in response to a prior runFileChooser call.
void AwWebContentsDelegate::NativeFilesSelectedInChooser(
    /* [in] */ Int32 processId,
    /* [in] */ Int32 renderId,
    /* [in] */ Int32 mode_flags,
    /* [in] */ ArrayOf<String>* filePath,
    /* [in] */ ArrayOf<String>* displayName)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
