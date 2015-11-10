
#include <elastos/core/StringBuffer.h>
#include <elastos/utility/logging/Slogger.h>
//TODO #include "elastos/droid/content/CIntent.h"
#include "elastos/droid/webkit/native/content/browser/PepperPluginManager.h"

using Elastos::Core::StringBuffer;
//TODO using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IPackageItemInfo;
using Elastos::Droid::Content::Pm::EIID_IPackageItemInfo;
using Elastos::Utility::IList;
using Elastos::Utility::IIterator;
using Elastos::Utility::IIterable;
using Elastos::Utility::EIID_IIterable;
using Elastos::Utility::Logging::Slogger;

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
    /* [in] */ /*const*/ IContext* context)
{
    AutoPtr<StringBuffer> ret = new StringBuffer();
    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IList> plugins;
    AutoPtr<IIntent> intent;
    assert(0);
    // TODO
    // CIntent::New(PEPPER_PLUGIN_ACTION, (IIntent**)&intent);
    pm->QueryIntentServices(intent,
            IPackageManager::GET_SERVICES | IPackageManager::GET_META_DATA,
            (IList**)&plugins);

    AutoPtr<IIterable> iterable = IIterable::Probe(plugins);
    AutoPtr<IIterator> iter;
    iterable->GetIterator((IIterator**)&iter);
    Boolean bNext = FALSE;
    for (iter->HasNext(&bNext); bNext; iter->HasNext(&bNext)) {
        AutoPtr<IResolveInfo> info;
        iter->GetNext((IInterface**)&info);
        // Retrieve the plugin's service information.
        AutoPtr<IServiceInfo> serviceInfo;
        info->GetServiceInfo((IServiceInfo**)&serviceInfo);
        AutoPtr<IBundle> metaData;
        String packageName;
        AutoPtr<IPackageItemInfo> packageItemInfo = IPackageItemInfo::Probe(serviceInfo);
        if (serviceInfo == NULL || (packageItemInfo->GetMetaData((IBundle**)&metaData), metaData == NULL) ||
                (packageItemInfo->GetPackageName(&packageName), packageName == NULL)) {
            String strLog("Can't get service information from ");
            String infoStr;
            info->ToString(&infoStr);
            strLog += infoStr;
            Slogger::E(LOGTAG, strLog);
            continue;
        }

        // Retrieve the plugin's package information.
        AutoPtr<IPackageInfo> pkgInfo;
    //     try {
            packageItemInfo->GetPackageName(&packageName);
            pm->GetPackageInfo(packageName, 0, (IPackageInfo**)&pkgInfo);
    //     } catch (NameNotFoundException e) {
    //         Log.e(LOGTAG, "Can't find plugin: " + serviceInfo.packageName);
    //         continue;
    //     }

        assert(0);
        // TODO
        // if (pkgInfo == NULL ||
        //         (pkgInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0) {
        //     continue;
        // }

        packageItemInfo->GetPackageName(&packageName);
        String strLog("The given plugin package is preloaded: ");
        strLog += packageName;
        Slogger::I(LOGTAG, strLog);

        packageItemInfo->GetMetaData((IBundle**)&metaData);
        String plugin = GetPluginDescription(metaData);
        if (plugin == NULL) {
           continue;
        }

        if (ret->GetLength() > 0) {
            ret->Append(',');
        }

        ret->Append(plugin);
    }

    return ret->ToString();
}

String PepperPluginManager::GetPluginDescription(
    /* [in] */ IBundle* metaData)
{
    // Find the name of the plugin's shared library.
    String filename;
    metaData->GetString(FILENAME, &filename);
    if (filename == NULL || filename.IsEmpty()) {
        return String(NULL);
    }

    // Find the mimetype of the plugin. Flash is handled in getFlashPath.
    String mimetype;
    metaData->GetString(MIMETYPE, &mimetype);
    if (mimetype == NULL || mimetype.IsEmpty()) {
        return String(NULL);
    }

    // Assemble the plugin info, according to the format described in
    // pepper_plugin_list.cc.
    // (eg. path<#name><#description><#version>;mimetype)
    AutoPtr<StringBuffer> plugin = new StringBuffer(PEPPER_PLUGIN_ROOT);
    plugin->Append(filename);

    // Find the (optional) name/description/version of the plugin.
    String name;
    metaData->GetString(NAME, &name);
    String description;
    metaData->GetString(DESCRIPTION, &description);
    String version;
    metaData->GetString(VERSION, &version);

    if (name != NULL && !name.IsEmpty()) {
        plugin->Append("#");
        plugin->Append(name);
        if (description != NULL && !description.IsEmpty()) {
            plugin->Append("#");
            plugin->Append(description);
            if (version != NULL && !version.IsEmpty()) {
                plugin->Append("#");
                plugin->Append(version);
            }
        }
    }

    plugin->Append(';');
    plugin->Append(mimetype);

    return plugin->ToString();
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
