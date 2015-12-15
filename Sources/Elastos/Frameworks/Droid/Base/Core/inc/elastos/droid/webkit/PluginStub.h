
#ifndef __ELASTOS_DROID_WEBKIT_PLUGINSTUB_H__
#define __ELASTOS_DROID_WEBKIT_PLUGINSTUB_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * This interface is used to implement plugins in a WebView. A plugin
 * package may extend this class and implement the abstract functions to create
 * embedded or fullscreeen views displayed in a WebView. The PluginStub
 * implementation will be provided the same NPP instance that is created
 * through the native interface.
 */
class PluginStub : public Object
{
public:
    /**
     * Return a custom embedded view to draw the plugin.
     * @param NPP The native NPP instance.
     * @param context The current application's Context.
     * @return A custom View that will be managed by WebView.
     */
    virtual CARAPI_(AutoPtr<IView>) GetEmbeddedView(
        /* [in] */ Int32 NPP,
        /* [in] */ IContext* context) = 0;

    /**
     * Return a custom full-screen view to be displayed when the user requests
     * a plugin display as full-screen. Note that the application may choose not
     * to display this View as completely full-screen.
     * @param NPP The native NPP instance.
     * @param context The current application's Context.
     * @return A custom View that will be managed by the application.
     */
    virtual CARAPI_(AutoPtr<IView>) GetFullScreenView(
        /* [in] */ Int32 NPP,
        /* [in] */ IContext* context) = 0;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_PLUGINSTUB_H__
