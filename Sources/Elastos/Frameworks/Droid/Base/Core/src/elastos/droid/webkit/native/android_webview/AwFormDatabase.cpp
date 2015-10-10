#include "elastos/droid/webkit/native/android_webview/AwFormDatabase.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

Boolean AwFormDatabase::HasFormData()
{
    return NativeHasFormData();
}

void AwFormDatabase::ClearFormData()
{
    NativeClearFormData();
}

//--------------------------------------------------------------------------------------------
//  Native methods
//--------------------------------------------------------------------------------------------
Boolean AwFormDatabase::NativeHasFormData()
{
    return FALSE;
}

void AwFormDatabase::NativeClearFormData()
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
