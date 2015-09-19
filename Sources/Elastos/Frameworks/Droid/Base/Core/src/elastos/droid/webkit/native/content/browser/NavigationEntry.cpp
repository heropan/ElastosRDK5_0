// wuweizuo automatic build .cpp file from .java file.

#include "NavigationEntry.h"

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
    // ==================before translated======================
    // return mIndex;
    assert(0);
    return 0;
}

String NavigationEntry::GetUrl()
{
    // ==================before translated======================
    // return mUrl;
    assert(0);
    return String("");
}

String NavigationEntry::GetVirtualUrl()
{
    // ==================before translated======================
    // return mVirtualUrl;
    assert(0);
    return String("");
}

String NavigationEntry::GetOriginalUrl()
{
    // ==================before translated======================
    // return mOriginalUrl;
    assert(0);
    return String("");
}

String NavigationEntry::GetTitle()
{
    // ==================before translated======================
    // return mTitle;
    assert(0);
    return String("");
}

AutoPtr<IBitmap> NavigationEntry::GetFavicon()
{
    // ==================before translated======================
    // return mFavicon;
    assert(0);
    AutoPtr<IBitmap> empty;
    return empty;
}

ECode NavigationEntry::UpdateFavicon(
    /* [in] */ IBitmap* favicon)
{
    VALIDATE_NOT_NULL(favicon);
    // ==================before translated======================
    // mFavicon = favicon;
    assert(0);
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
    // ==================before translated======================
    // mIndex = index;
    // mUrl = url;
    // mVirtualUrl = virtualUrl;
    // mOriginalUrl = originalUrl;
    // mTitle = title;
    // mFavicon = favicon;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


