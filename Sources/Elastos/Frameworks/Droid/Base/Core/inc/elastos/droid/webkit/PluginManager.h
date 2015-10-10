
#ifndef __ELASTOS_DROID_WEBKIT_PLUGINMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_PLUGINMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::ISignature;
using Elastos::Droid::Content::Pm::IPackageInfo;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CPluginManager;

/**
 * Class for managing the relationship between the {@link WebViewClassic} and installed
 * plugins in the system. You can find this class through
 * {@link PluginManager#getInstance}.
 *
 * @hide pending API solidification
 */
class PluginManager
{
    friend class CPluginManagerHelper;
public:
    /**
     * Service Action: A plugin wishes to be loaded in the WebView must provide
     * {@link android.content.IntentFilter IntentFilter} that accepts this
     * action in their AndroidManifest.xml.
     * <p>
     * TODO: we may change this to a new PLUGIN_ACTION if this is going to be
     * public.
     */
    static const String PLUGIN_ACTION;

    /**
     * A plugin wishes to be loaded in the WebView must provide this permission
     * in their AndroidManifest.xml.
     */
    static const String PLUGIN_PERMISSION;


    /**
     * Signal the WebCore thread to refresh its list of plugins. Use this if the
     * directory contents of one of the plugin directories has been modified and
     * needs its changes reflecting. May cause plugin load and/or unload.
     *
     * @param reloadOpenPages Set to true to reload all open pages.
     */
    virtual CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    virtual CARAPI_(AutoPtr< ArrayOf<String> >) GetPluginDirectories();

    /* package */
    virtual CARAPI_(Boolean) ContainsPluginPermissionAndSignatures(
        /* [in] */ const String& pluginAPKName);

    /* package */
    virtual CARAPI_(String) GetPluginsAPKName(
        /* [in] */ const String& pluginLib);

    virtual CARAPI_(String) GetPluginSharedDataDirectory();

    /* package */
    /*Class<?>*/virtual CARAPI_(AutoPtr<IInterface>) GetPluginClass(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className);

protected:
    PluginManager();
    void SetContext(IContext* context);

private:
    PluginManager(
        /* [in] */ IContext* context);

    static CARAPI_(Boolean) ContainsPluginPermissionAndSignatures(
        /* [in] */ IPackageInfo* pkgInfo);

    static CARAPI_(AutoPtr<CPluginManager>) GetInstance(
        /* [in] */ IContext* context);

private:
    // Only plugin matches one of the signatures in the list can be loaded
    // inside the WebView process
    static const String SIGNATURE_1;

    static const AutoPtr< ArrayOf<ISignature*> > SIGNATURES;

    static const String LOGTAG;

    static const String PLUGIN_SYSTEM_LIB;

    static const String PLUGIN_TYPE;
    static const String TYPE_NATIVE;

    AutoPtr<IContext> mContext;

    List< AutoPtr<IPackageInfo> > mPackageInfoCache;
    Object mPackageInfoCacheLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_PLUGINMANAGER_H__
