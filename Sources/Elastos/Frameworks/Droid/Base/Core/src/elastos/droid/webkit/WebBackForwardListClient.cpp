
#include "elastos/droid/webkit/WebBackForwardListClient.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

void WebBackForwardListClient::OnNewHistoryItem(
    /* [in] */ IWebHistoryItem* item)
{
}

void WebBackForwardListClient::OnIndexChanged(
    /* [in] */ IWebHistoryItem* item,
    /* [in] */ Int32 index)
{
}

ECode WebBackForwardListClient::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebBackForwardListClient";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
