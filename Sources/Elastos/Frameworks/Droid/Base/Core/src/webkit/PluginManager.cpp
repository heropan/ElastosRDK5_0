
#include "content/CIntent.h"
#include "content/pm/CSignature.h"
#include "os/CMessageHelper.h"
#include "os/SystemProperties.h"
#include "webkit/BrowserFrame.h"
#include "webkit/PluginManager.h"
#include "webkit/WebCoreElastosBridge.h"
#include "webkit/CPluginManager.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CBoolean;
using Elastos::Core::IBoolean;
using Elastos::Core::IClassLoader;
using Elastos::Core::Mutex;
using Elastos::IO::IFile;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::CSignature;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

static AutoPtr< ArrayOf<ISignature*> > SignatureCreate()
{
    AutoPtr< ArrayOf<ISignature*> > array = ArrayOf<ISignature*>::Alloc(1);
    AutoPtr<ISignature> signature;
    CSignature::New((ISignature**)&signature);
    array->Set(0, signature);
    return array;
}

const String PluginManager::PLUGIN_ACTION("android.webkit.PLUGIN");

/**
 * A plugin wishes to be loaded in the WebView must provide this permission
 * in their AndroidManifest.xml.
 */
const String PluginManager::PLUGIN_PERMISSION("android.webkit.permission.PLUGIN");

// Only plugin matches one of the signatures in the list can be loaded
// inside the WebView process
const String PluginManager::SIGNATURE_1(
             "308204c5308203ada003020102020"
             "900d7cb412f75f4887e300d06092a"
             "864886f70d010105050030819d310"
             "b3009060355040613025553311330"
             "110603550408130a43616c69666f7"
             "26e69613111300f06035504071308"
             "53616e204a6f73653123302106035"
             "5040a131a41646f62652053797374"
             "656d7320496e636f72706f7261746"
             "564311c301a060355040b1313496e"
             "666f726d6174696f6e20537973746"
             "56d73312330210603550403131a41"
             "646f62652053797374656d7320496"
             "e636f72706f7261746564301e170d"
             "3039313030313030323331345a170"
             "d3337303231363030323331345a30"
             "819d310b300906035504061302555"
             "3311330110603550408130a43616c"
             "69666f726e69613111300f0603550"
             "407130853616e204a6f7365312330"
             "21060355040a131a41646f6265205"
             "3797374656d7320496e636f72706f"
             "7261746564311c301a060355040b1"
             "313496e666f726d6174696f6e2053"
             "797374656d7331233021060355040"
             "3131a41646f62652053797374656d"
             "7320496e636f72706f72617465643"
             "0820120300d06092a864886f70d01"
             "010105000382010d0030820108028"
             "201010099724f3e05bbd78843794f"
             "357776e04b340e13cb1c9ccb30448"
             "65180d7d8fec8166c5bbd876da8b8"
             "0aa71eb6ba3d4d3455c9a8de162d2"
             "4a25c4c1cd04c9523affd06a279fc"
             "8f0d018f242486bdbb2dbfbf6fcb2"
             "1ed567879091928b876f7ccebc7bc"
             "cef157366ebe74e33ae1d7e937309"
             "1adab8327482154afc0693a549522"
             "f8c796dd84d16e24bb221f5dbb809"
             "ca56dd2b6e799c5fa06b6d9c5c09a"
             "da54ea4c5db1523a9794ed22a3889"
             "e5e05b29f8ee0a8d61efe07ae28f6"
             "5dece2ff7edc5b1416d7c7aad7f0d"
             "35e8f4a4b964dbf50ae9aa6d62015"
             "7770d974131b3e7e3abd6d163d657"
             "58e2f0822db9c88598b9db6263d96"
             "3d13942c91fc5efe34fc1e06e3020"
             "103a382010630820102301d060355"
             "1d0e041604145af418e419a639e16"
             "57db960996364a37ef20d403081d2"
             "0603551d230481ca3081c780145af"
             "418e419a639e1657db960996364a3"
             "7ef20d40a181a3a481a030819d310"
             "b3009060355040613025553311330"
             "110603550408130a43616c69666f7"
             "26e69613111300f06035504071308"
             "53616e204a6f73653123302106035"
             "5040a131a41646f62652053797374"
             "656d7320496e636f72706f7261746"
             "564311c301a060355040b1313496e"
             "666f726d6174696f6e20537973746"
             "56d73312330210603550403131a41"
             "646f62652053797374656d7320496"
             "e636f72706f7261746564820900d7"
             "cb412f75f4887e300c0603551d130"
             "40530030101ff300d06092a864886"
             "f70d0101050500038201010076c2a"
             "11fe303359689c2ebc7b2c398eff8"
             "c3f9ad545cdbac75df63bf7b5395b"
             "6988d1842d6aa1556d595b5692e08"
             "224d667a4c9c438f05e74906c53dd"
             "8016dde7004068866f01846365efd"
             "146e9bfaa48c9ecf657f87b97c757"
             "da11f225c4a24177bf2d7188e6cce"
             "2a70a1e8a841a14471eb51457398b"
             "8a0addd8b6c8c1538ca8f1e40b4d8"
             "b960009ea22c188d28924813d2c0b"
             "4a4d334b7cf05507e1fcf0a06fe94"
             "6c7ffc435e173af6fc3e340064371"
             "0acc806f830a14788291d46f2feed"
             "9fb5c70423ca747ed1572d752894a"
             "c1f19f93989766308579393fabb43"
             "649aa8806a313b1ab9a50922a44c2"
             "467b9062037f2da0d484d9ffd8fe6"
             "28eeea629ba637");

const AutoPtr< ArrayOf<ISignature*> > PluginManager::SIGNATURES = SignatureCreate();
const String PluginManager::LOGTAG("PluginManager");
const String PluginManager::PLUGIN_SYSTEM_LIB("/system/lib/plugins/");
const String PluginManager::PLUGIN_TYPE("type");
const String PluginManager::TYPE_NATIVE("native");

PluginManager::PluginManager()
{
}

PluginManager::PluginManager(
    /* [in] */ IContext* context)
    : mContext(context)
{
}

void PluginManager::SetContext(IContext* context)
{
    mContext = context;
}

AutoPtr<CPluginManager> PluginManager::GetInstance(
    /* [in] */ IContext* context)
{
    //return NULL;
    //*
    //Mutex::Autolock lock(sLock);

   // if (mInstance == NULL) {
   //     if (context == NULL)
   //     {
   //         Logger::E(LOGTAG, "First call to PluginManager need a valid context.");
   //         assert(0);
   //     }
   //     mInstance = new PluginManager(context->GetApplicationContext());
   // }
    return NULL;
    //*/
}

//synchronized
//PluginManager* PluginManager::GetInstance(
//    /* [in] */ IContext* context)
//{
//    Mutex::Autolock lock(sLock);
//
//    if (mInstance == NULL) {
//        if (context == NULL) {
//            //throw new IllegalStateException(
//            //        "First call to PluginManager need a valid context.");
//            assert(0);
//        }
//
//        AutoPtr<IContext> con;
//        context->GetApplicationContext((IContext**)&con);
//        mInstance = new PluginManager(con);
//    }
//
//    return mInstance;
//}

/**
 * Signal the WebCore thread to refresh its list of plugins. Use this if the
 * directory contents of one of the plugin directories has been modified and
 * needs its changes reflecting. May cause plugin load and/or unload.
 *
 * @param reloadOpenPages Set to true to reload all open pages.
 */
ECode PluginManager::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IBoolean> iReloadOpenPages;
    CBoolean::New(reloadOpenPages, (IBoolean**)&iReloadOpenPages);
    AutoPtr<IMessage> msg;
    mh->Obtain(BrowserFrame::sElastosBridge, WebCoreElastosBridge::REFRESH_PLUGINS, iReloadOpenPages,
        (IMessage**)&msg);
    return msg->SendToTarget();
}

AutoPtr< ArrayOf<String> > PluginManager::GetPluginDirectories()
{
    List<String> directories;
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IIntent> intent;
    CIntent::New(PLUGIN_ACTION, (IIntent**)&intent);
    AutoPtr<IObjectContainer> plugins;
    pm->QueryIntentServices(intent,
            IPackageManager::GET_SERVICES | IPackageManager::GET_META_DATA, (IObjectContainer**)&plugins);

    {
        Mutex::Autolock lock(mPackageInfoCacheLock);

        // clear the list of existing packageInfo objects
        mPackageInfoCache.Clear();

        AutoPtr<IObjectEnumerator> enumerator;
        plugins->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean bNext = FALSE;
        while(enumerator->MoveNext(&bNext), bNext) {
            AutoPtr<IResolveInfo> info;
            enumerator->Current((IInterface**)&info);

            // retrieve the plugin's service information
            AutoPtr<IServiceInfo> serviceInfo;
            info->GetServiceInfo((IServiceInfo**)&serviceInfo);
            if (serviceInfo == NULL) {
                //Log.w(LOGTAG, "Ignore bad plugin");
                continue;
            }

            // retrieve information from the plugin's manifest
            AutoPtr<IPackageInfo> pkgInfo;
            String packageName;
            serviceInfo->GetPackageName(&packageName);
            if (FAILED(pm->GetPackageInfo(packageName,
                                IPackageManager::GET_PERMISSIONS
                                | IPackageManager::GET_SIGNATURES, (IPackageInfo**)&pkgInfo))) {
                continue;
            }

            if (pkgInfo == NULL) {
                continue;
            }

            /*
             * find the location of the plugin's shared library. The default
             * is to assume the app is either a user installed app or an
             * updated system app. In both of these cases the library is
             * stored in the app's data directory.
             */
            AutoPtr<IApplicationInfo> appInfo;
            pkgInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String dataDir;
            appInfo->GetDataDir(&dataDir);
            String directory(dataDir);
            directory += "/lib";
            Int32 appFlags;
            appInfo->GetFlags(&appFlags);
            Int32 updatedSystemFlags = IApplicationInfo::FLAG_SYSTEM |
                                           IApplicationInfo::FLAG_UPDATED_SYSTEM_APP;
            // preloaded system app with no user updates
            if ((appFlags & updatedSystemFlags) == IApplicationInfo::FLAG_SYSTEM) {
                directory = PLUGIN_SYSTEM_LIB;
                String name;
                pkgInfo->GetPackageName(&name);
                directory += name;
            }

            // check if the plugin has the required permissions and
            // signatures
            if (!ContainsPluginPermissionAndSignatures(pkgInfo))
            {
                continue;
            }

            // determine the type of plugin from the manifest
            AutoPtr<IBundle> metaData;
            serviceInfo->GetMetaData((IBundle**)&metaData);
            if (metaData == NULL) {
                //Log.e(LOGTAG, "The plugin '" + serviceInfo.name + "' has no type defined");
                continue;
            }

            String pluginType;
            metaData->GetString(PLUGIN_TYPE, &pluginType);
            if (!TYPE_NATIVE.Equals(pluginType)) {
                //Log.e(LOGTAG, "Unrecognized plugin type: " + pluginType);
                continue;
            }

            //try {
                String name;
                serviceInfo->GetName(&name);
                AutoPtr<IInterface> cls = GetPluginClass(packageName, name);

                //TODO implement any requirements of the plugin class here!
                Boolean classFound = TRUE;

                if (!classFound) {
                    //Log.e(LOGTAG, "The plugin's class' " + serviceInfo.name + "' does not extend the appropriate class.");
                    continue;
                }

            //} catch (NameNotFoundException e) {
            //    Log.e(LOGTAG, "Can't find plugin: " + serviceInfo.packageName);
            //    continue;
            //} catch (ClassNotFoundException e) {
            //    Log.e(LOGTAG, "Can't find plugin's class: " + serviceInfo.name);
            //    continue;
            //}

            // if all checks have passed then make the plugin available
            mPackageInfoCache.PushBack(pkgInfo);
            directories.PushBack(directory);
        }
    }

    //Logger::V(LOGTAG, " GetPluginDirectories::%d", directories.GetSize());
    AutoPtr< ArrayOf<String> > retArray = ArrayOf<String>::Alloc(directories.GetSize());
    List<String>::Iterator iter = directories.Begin();
    for (Int32 i = 0; iter != directories.End(); ++iter, ++i) {
        //Logger::V(LOGTAG, " plugin dir,%dth::%s", i, (*iter).string());
        retArray->Set(i, *iter);
    }

    return retArray;
}

/* package */
Boolean PluginManager::ContainsPluginPermissionAndSignatures(
    /* [in] */ const String& pluginAPKName)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);

    // retrieve information from the plugin's manifest
    //try {
        AutoPtr<IPackageInfo> pkgInfo;
        pm->GetPackageInfo(pluginAPKName, IPackageManager::GET_PERMISSIONS
                | IPackageManager::GET_SIGNATURES,
                (IPackageInfo**)&pkgInfo);
        if (pkgInfo != NULL) {
            return ContainsPluginPermissionAndSignatures(pkgInfo);
        }
    //} catch (NameNotFoundException e) {
    //    Log.w(LOGTAG, "Can't find plugin: " + pluginAPKName);
    //}
    return FALSE;
}

/* package */
String PluginManager::GetPluginsAPKName(
    /* [in] */ const String& pluginLib)
{
    // basic error checking on input params
    if (pluginLib.IsNullOrEmpty()) {
        return String("");
    }

    // must be synchronized to ensure the consistency of the cache
    {
        Mutex::Autolock lock(mPackageInfoCacheLock);
        List< AutoPtr<IPackageInfo> >::Iterator iter = mPackageInfoCache.Begin();
        for (; iter != mPackageInfoCache.End(); ++iter) {
            AutoPtr<IPackageInfo> pkgInfo = *iter;
            String packageName;
            pkgInfo->GetPackageName(&packageName);
            if (pluginLib.Contains(packageName)) {
                return packageName;
            }
        }
    }

    // if no apk was found then return null
    return String(NULL);
}

String PluginManager::GetPluginSharedDataDirectory()
{
    AutoPtr<IFile> file;
    String path;
    mContext->GetDir(String("plugins"), 0, (IFile**)&file);
    file->GetPath(&path);
    return path;
}

/* package */
/*Class<?>*/AutoPtr<IInterface> PluginManager::GetPluginClass(
    /* [in] */ const String& packageName,
    /* [in] */ const String& className)
{
    AutoPtr<IContext> pluginContext;
    mContext->CreatePackageContext(packageName,
            IContext::CONTEXT_INCLUDE_CODE |
            IContext::CONTEXT_IGNORE_SECURITY,
            (IContext**)&pluginContext);

    AutoPtr<IClassLoader> pluginCL;
    pluginContext->GetClassLoader((IClassLoader**)&pluginCL);

    AutoPtr<IInterface> lClass;
//    pluginCL->LoadClass(className, (IInterface**)&lClass);

    return lClass;
}

Boolean PluginManager::ContainsPluginPermissionAndSignatures(
    /* [in] */ IPackageInfo* pkgInfo)
{
    // check if the plugin has the required permissions
    AutoPtr< ArrayOf<String> > permissions;
    pkgInfo->GetRequestedPermissions((ArrayOf<String>**)&permissions);

    if (permissions == NULL) {
        return FALSE;
    }

    Boolean permissionOk = FALSE;
    Int32 length = permissions->GetLength();
    for (Int32 i = 0; i < length; i++) {
        String permit = (*permissions)[i];
        if (PLUGIN_PERMISSION.Equals(permit)) {
            permissionOk = TRUE;
            break;
        }
    }

    if (!permissionOk) {
        return FALSE;
    }

    String packageName;
    pkgInfo->GetPackageName(&packageName);
    if ((SystemProperties::GetBoolean(String("ro.secure"), FALSE))) {
        // check to ensure the plugin is properly signed
        AutoPtr< ArrayOf<ISignature*> > signatures;
        pkgInfo->GetSignatures((ArrayOf<ISignature*>**)&signatures);
        if (signatures == NULL) {
            return FALSE;
        }
        Boolean signatureMatch = FALSE;
        Int32 length = signatures->GetLength();
        for (Int32 j = 0; j < length; j++) {
            AutoPtr<ISignature> signature = (*signatures)[j];
            for (Int32 i = 0; i < SIGNATURES->GetLength(); i++) {
                Boolean bFlag = FALSE;
                (*SIGNATURES)[i]->Equals(signature, &bFlag);
                if (bFlag) {
                    signatureMatch = TRUE;
                    break;
                }
            }
        }

        if (!signatureMatch) {
            return FALSE;
        }
    }

    return TRUE;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
