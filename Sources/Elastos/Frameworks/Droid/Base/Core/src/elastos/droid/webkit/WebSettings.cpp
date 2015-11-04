
#include "elastos/droid/webkit/WebSettings.h"
#include "elastos/droid/webkit/WebViewFactory.h"
#include <elastos/core/AutoLock.h>
#include <elastos/core/Math.h>

using Elastos::Core::AutoLock;

namespace Elastos {
namespace Droid {
namespace Webkit {

WebSettings::WebSettings()
{
}

WebSettings::~WebSettings()
{
}

TextSize WebSettings::GetTextSize()
{
    AutoLock lock(this);
    TextSize closestSize = -1;
    Int32 smallestDelta = Elastos::Core::Math::INT32_MAX_VALUE;
    Int32 textSize = GetTextZoom();
    Int32 TEXT_SZIE[5] = { TextSize_SMALLEST, TextSize_SMALLER, TextSize_NORMAL,
            TextSize_LARGER, TextSize_LARGEST };
    for (Int32 i = 0; i < 5; i++) {
        TextSize size = TEXT_SZIE[i];
        Int32 delta = Elastos::Core::Math::Abs(textSize - size);
        if (delta == 0) {
            return size;
        }
        if (delta < smallestDelta) {
            smallestDelta = delta;
            closestSize = size;
        }
    }
    return closestSize != -1 ? closestSize : TextSize_NORMAL;
}

String WebSettings::GetDefaultUserAgent(
    /* [in] */ IContext* context)
{
    AutoPtr<IWebViewFactoryProvider::IStatics> stat;
    WebViewFactory::GetProvider()->GetStatics((IWebViewFactoryProvider::IStatics**)&stat);
    String userAgent;
    stat->GetDefaultUserAgent(context, &userAgent);
    return userAgent;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
