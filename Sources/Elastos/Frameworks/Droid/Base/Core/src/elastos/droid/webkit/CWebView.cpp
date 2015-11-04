
#include "elastos/droid/webkit/CWebView.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_OBJECT_IMPL(CWebView);

void CWebView::PostInvalidate(
    /* [in] */ IInterface* obj)
{
    assert(0);
    // TODO
    // ((CWebView*)IWebView::Probe(obj))->PostInvalidate();
}

} // Webkit
} // Droid
} // Elastos
