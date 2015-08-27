// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                           NavigationEntry
//=====================================================================
Int32 NavigationEntry::GetIndex()
{
    /*
    return mIndex;
    */
}

String NavigationEntry::GetUrl()
{
    /*
    return mUrl;
    */
}

String NavigationEntry::GetVirtualUrl()
{
    /*
    return mVirtualUrl;
    */
}

String NavigationEntry::GetOriginalUrl()
{
    /*
    return mOriginalUrl;
    */
}

String NavigationEntry::GetTitle()
{
    /*
    return mTitle;
    */
}

AutoPtr<IBitmap> NavigationEntry::GetFavicon()
{
    /*
    return mFavicon;
    */
}

ECode NavigationEntry::UpdateFavicon(
    /* [in] */ IBitmap* favicon)
{
    VALIDATE_NOT_NULL(favicon);
    /*
    mFavicon = favicon;
    */
    return NOERROR;
}

NavigationEntry::NavigationEntry(
    /* [in] */ Int32 index,
    /* [in] */ String url,
    /* [in] */ String virtualUrl,
    /* [in] */ String originalUrl,
    /* [in] */ String title,
    /* [in] */ IBitmap* favicon)
{
    /*
    mIndex = index;
    mUrl = url;
    mVirtualUrl = virtualUrl;
    mOriginalUrl = originalUrl;
    mTitle = title;
    mFavicon = favicon;
    */
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


