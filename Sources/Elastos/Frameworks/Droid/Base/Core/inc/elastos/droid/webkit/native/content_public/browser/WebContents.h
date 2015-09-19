
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_PUBLIC_BROWSER_WEBCONTENTS_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_PUBLIC_BROWSER_WEBCONTENTS_H__

#include "ext/frameworkext.h"
#include "webkit/native/content_public/browser/NavigationController.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content_Public {
namespace Browser {

/**
 * The WebContents Java wrapper to allow communicating with the native WebContents object.
 */
class WebContents
{
public:
    /**
     * @return The navigation controller associated with this WebContents.
     */
    virtual CARAPI_(AutoPtr<NavigationController>) GetNavigationController() = 0;

    /**
     * @return The title for the current visible page.
     */
    virtual CARAPI_(String) GetTitle() = 0;

    /**
     * @return The URL for the current visible page.
     */
    virtual CARAPI_(String) GetVisibleUrl() = 0;

    /**
     * Stop any pending navigation.
     */
    virtual CARAPI_(void) Stop() = 0;
};

} // namespace Browser
} // namespace Content_Public
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_PUBLIC_BROWSER_WEBCONTENTS_H__
