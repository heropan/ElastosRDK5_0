// wuweizuo automatic build .cpp file from .java file.

#include "PepperPluginManager.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                         PepperPluginManager
//=====================================================================
const String PepperPluginManager::PEPPER_PLUGIN_ACTION("org.chromium.intent.PEPPERPLUGIN");
const String PepperPluginManager::PEPPER_PLUGIN_ROOT("/system/lib/pepperplugin/");
const String PepperPluginManager::LOGTAG("PepperPluginManager");
const String PepperPluginManager::FILENAME("filename");
const String PepperPluginManager::MIMETYPE("mimetype");
const String PepperPluginManager::NAME("name");
const String PepperPluginManager::DESCRIPTION("description");
const String PepperPluginManager::VERSION("version");

String PepperPluginManager::GetPlugins(
    /* [in] */ const IContext* context)
{
    // ==================before translated======================
    // StringBuffer ret = new StringBuffer();
    // PackageManager pm = context.getPackageManager();
    // List<ResolveInfo> plugins = pm.queryIntentServices(
    //         new Intent(PEPPER_PLUGIN_ACTION),
    //         PackageManager.GET_SERVICES | PackageManager.GET_META_DATA);
    // for (ResolveInfo info : plugins) {
    //     // Retrieve the plugin's service information.
    //     ServiceInfo serviceInfo = info.serviceInfo;
    //     if (serviceInfo == null || serviceInfo.metaData == null ||
    //             serviceInfo.packageName == null) {
    //         Log.e(LOGTAG, "Can't get service information from " + info);
    //         continue;
    //     }
    //
    //     // Retrieve the plugin's package information.
    //     PackageInfo pkgInfo;
    //     try {
    //         pkgInfo = pm.getPackageInfo(serviceInfo.packageName, 0);
    //     } catch (NameNotFoundException e) {
    //         Log.e(LOGTAG, "Can't find plugin: " + serviceInfo.packageName);
    //         continue;
    //     }
    //     if (pkgInfo == null ||
    //             (pkgInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0) {
    //         continue;
    //     }
    //     Log.i(LOGTAG, "The given plugin package is preloaded: " + serviceInfo.packageName);
    //
    //     String plugin = getPluginDescription(serviceInfo.metaData);
    //     if (plugin == null) {
    //         continue;
    //     }
    //     if (ret.length() > 0) {
    //         ret.append(',');
    //     }
    //     ret.append(plugin);
    // }
    // return ret.toString();
    assert(0);
    return String("");
}

String PepperPluginManager::GetPluginDescription(
    /* [in] */ IBundle* metaData)
{
    // ==================before translated======================
    // // Find the name of the plugin's shared library.
    // String filename = metaData.getString(FILENAME);
    // if (filename == null || filename.isEmpty()) {
    //     return null;
    // }
    // // Find the mimetype of the plugin. Flash is handled in getFlashPath.
    // String mimetype = metaData.getString(MIMETYPE);
    // if (mimetype == null || mimetype.isEmpty()) {
    //     return null;
    // }
    // // Assemble the plugin info, according to the format described in
    // // pepper_plugin_list.cc.
    // // (eg. path<#name><#description><#version>;mimetype)
    // StringBuffer plugin = new StringBuffer(PEPPER_PLUGIN_ROOT);
    // plugin.append(filename);
    //
    // // Find the (optional) name/description/version of the plugin.
    // String name = metaData.getString(NAME);
    // String description = metaData.getString(DESCRIPTION);
    // String version = metaData.getString(VERSION);
    //
    // if (name != null && !name.isEmpty()) {
    //     plugin.append("#");
    //     plugin.append(name);
    //     if (description != null && !description.isEmpty()) {
    //         plugin.append("#");
    //         plugin.append(description);
    //         if (version != null && !version.isEmpty()) {
    //             plugin.append("#");
    //             plugin.append(version);
    //         }
    //     }
    // }
    // plugin.append(';');
    // plugin.append(mimetype);
    //
    // return plugin.toString();
    assert(0);
    return String("");
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


