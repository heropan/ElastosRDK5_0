
#include "elastos/droid/webkit/native/net/GURLUtils.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//                              GURLUtils
//=====================================================================
String GURLUtils::GetOrigin(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // return nativeGetOrigin(url);

    return NativeGetOrigin(url);
}

String GURLUtils::GetScheme(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // return nativeGetScheme(url);

    return NativeGetScheme(url);
}

String GURLUtils::NativeGetOrigin(
    /* [in] */ const String& url)
{
    assert(0);
    return String("");
}

String GURLUtils::NativeGetScheme(
    /* [in] */ const String& url)
{
    assert(0);
    return String("");
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


