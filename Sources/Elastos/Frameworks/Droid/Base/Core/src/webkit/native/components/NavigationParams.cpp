// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//                           NavigationParams
//=====================================================================
NavigationParams::NavigationParams(
    /* [in] */ String url,
    /* [in] */ Boolean isPost,
    /* [in] */ Boolean hasUserGesture,
    /* [in] */ Int32 pageTransitionType,
    /* [in] */ Boolean isRedirect)
{
    /*
    this.url = url;
    this.isPost = isPost;
    this.hasUserGesture = hasUserGesture;
    this.pageTransitionType = pageTransitionType;
    this.isRedirect = isRedirect;
    */
}

AutoPtr<NavigationParams> NavigationParams::Create(
    /* [in] */ String url,
    /* [in] */ Boolean isPost,
    /* [in] */ Boolean hasUserGesture,
    /* [in] */ Int32 pageTransitionType,
    /* [in] */ Boolean isRedirect)
{
    /*
    return new NavigationParams(url, isPost, hasUserGesture, pageTransitionType,
            isRedirect);
    */
    return AutoPtr<NavigationParams>(this);
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


