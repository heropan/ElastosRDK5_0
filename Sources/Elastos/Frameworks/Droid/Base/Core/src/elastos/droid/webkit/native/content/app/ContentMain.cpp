
#include "elastos/droid/webkit/native/content/app/ContentMain.h"

// import android.content.Context;

// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace App {

/**
 * Initialize application context in native side.
 **/
void ContentMain::InitApplicationContext(
    /* [in] */ IContext* context)
{
    NativeInitApplicationContext(context);
}

/**
 * Start the ContentMainRunner in native side.
 **/
Int32 ContentMain::Start()
{
    return NativeStart();
}

void ContentMain::NativeInitApplicationContext(
    /* [in] */ IContext* context)
{
}

Int32 ContentMain::NativeStart()
{
}

} // namespace App
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
