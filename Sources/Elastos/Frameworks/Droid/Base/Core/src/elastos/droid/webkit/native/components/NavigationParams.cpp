
#include "elastos/droid/webkit/native/components/NavigationParams.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//                           NavigationParams
//=====================================================================
NavigationParams::NavigationParams(
    /* [in] */ const String& url1,
    /* [in] */ Boolean isPost1,
    /* [in] */ Boolean hasUserGesture1,
    /* [in] */ Int32 pageTransitionType1,
    /* [in] */ Boolean isRedirect1)
    : url(url1)
    , isPost(isPost1)
    , hasUserGesture(hasUserGesture1)
    , pageTransitionType(pageTransitionType1)
    , isRedirect(isRedirect1)
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

    AutoPtr<NavigationParams> result = new NavigationParams(url, isPost, hasUserGesture, pageTransitionType, isRedirect);;
    return result;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

