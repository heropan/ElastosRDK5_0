// wuweizuo automatic build .cpp file from .java file.

#include "NavigationParams.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//                           NavigationParams
//=====================================================================
NavigationParams::NavigationParams(
    /* [in] */ const String& url,
    /* [in] */ Boolean isPost,
    /* [in] */ Boolean hasUserGesture,
    /* [in] */ Int32 pageTransitionType,
    /* [in] */ Boolean isRedirect)
{
    // ==================before translated======================
    // this.url = url;
    // this.isPost = isPost;
    // this.hasUserGesture = hasUserGesture;
    // this.pageTransitionType = pageTransitionType;
    // this.isRedirect = isRedirect;
}

AutoPtr<NavigationParams> NavigationParams::Create(
    /* [in] */ const String& url,
    /* [in] */ Boolean isPost,
    /* [in] */ Boolean hasUserGesture,
    /* [in] */ Int32 pageTransitionType,
    /* [in] */ Boolean isRedirect)
{
    // ==================before translated======================
    // return new NavigationParams(url, isPost, hasUserGesture, pageTransitionType,
    //         isRedirect);
    assert(0);
    AutoPtr<NavigationParams> empty;
    return empty;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


