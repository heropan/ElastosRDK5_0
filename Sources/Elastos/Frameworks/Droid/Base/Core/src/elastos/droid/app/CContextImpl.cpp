#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/res/CCompatibilityInfo.h"
#include "elastos/droid/content/res/CResources.h"
#include "elastos/droid/app/ReceiverRestrictedContext.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/app/CInstrumentation.h"
#include "elastos/droid/app/CContextImpl.h"
#include "elastos/droid/app/CReceiverRestrictedContext.h"
#include "elastos/droid/app/CActivityManager.h"
#include "elastos/droid/app/CUiModeManager.h"
#include "elastos/droid/app/NotificationManager.h"
#include "elastos/droid/app/ApplicationPackageManager.h"
#include "elastos/droid/app/CWallpaperManager.h"
#include "elastos/droid/app/CAlarmManager.h"
#include "elastos/droid/app/backup/CBackupManager.h"
#include "elastos/droid/app/CStatusBarManager.h"
#include "elastos/droid/app/CKeyguardManager.h"
#include "elastos/droid/app/SharedPreferencesImpl.h"
#include "elastos/droid/app/admin/CDevicePolicyManager.h"
#include "elastos/droid/hardware/display/CDisplayManager.h"
#include "elastos/droid/hardware/CSystemSensorManager.h"
#include "elastos/droid/hardware/CSerialManager.h"
#include "elastos/droid/hardware/usb/CUsbManager.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/os/Environment.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/FileUtils.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/CPowerManager.h"
#include "elastos/droid/os/CDropBoxManager.h"
#include "elastos/droid/os/CUserManager.h"
#include "elastos/droid/os/storage/CStorageManager.h"
#include "elastos/droid/privacy/surrogate/CPrivacyLocationManager.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CClipboardManager.h"
#include "elastos/droid/impl/CPolicyManager.h"
#include "elastos/droid/net/CConnectivityManager.h"
#include "elastos/droid/net/wifi/CWifiManager.h"
#include "elastos/droid/net/wifi/p2p/CWifiP2pManager.h"
#include "elastos/droid/view/accessibility/CAccessibilityManager.h"
#include "elastos/droid/view/inputmethod/CInputMethodManager.h"
#include "elastos/droid/view/CContextThemeWrapper.h"
#include "elastos/droid/view/WindowManagerImpl.h"
#include "elastos/droid/view/CDisplayManagerAw.h"
#include "elastos/droid/media/CAudioManager.h"
#include "elastos/droid/database/sqlite/SQLiteDatabase.h"
#include "elastos/droid/accounts/CAccountManager.h"
#include "elastos/droid/privacy/CPrivacySettingsManager.h"
#include <elastos/utility/logging/Slogger.h>
#include "elastos/droid/R.h"

using Elastos::IO::CFile;
using Elastos::IO::CFileInputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Content::IClipboardManager;
using Elastos::Droid::Content::CClipboardManager;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Hardware::ISystemSensorManager;
using Elastos::Droid::Hardware::CSystemSensorManager;
using Elastos::Droid::Hardware::CSerialManager;
using Elastos::Droid::Hardware::ISerialManager;
using Elastos::Droid::Hardware::IISerialManager;
using Elastos::Droid::Hardware::EIID_IISerialManager;
using Elastos::Droid::Hardware::Display::IDisplayManager;
using Elastos::Droid::Hardware::Display::CDisplayManager;
using Elastos::Droid::Hardware::Usb::IIUsbManager;
using Elastos::Droid::Hardware::Usb::IUsbManager;
using Elastos::Droid::Hardware::Usb::CUsbManager;
using Elastos::Droid::Location::IILocationManager;
using Elastos::Droid::Location::EIID_IILocationManager;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IVibratorService;
using Elastos::Droid::Os::CDropBoxManager;
using Elastos::Droid::Os::CUserManager;
using Elastos::Droid::Os::Storage::IStorageManager;
using Elastos::Droid::Os::Storage::CStorageManager;
using Elastos::Droid::Privacy::Surrogate::IPrivacyLocationManager;
using Elastos::Droid::Privacy::Surrogate::CPrivacyLocationManager;
using Elastos::Droid::View::WindowManagerImpl;
using Elastos::Droid::View::IContextThemeWrapper;
using Elastos::Droid::View::CContextThemeWrapper;
using Elastos::Droid::View::IDisplayManagerAw;
using Elastos::Droid::View::CDisplayManagerAw;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::App::CUiModeManager;
using Elastos::Droid::App::NotificationManager;
using Elastos::Droid::App::CStatusBarManager;
using Elastos::Droid::App::CKeyguardManager;
using Elastos::Droid::App::Admin::IDevicePolicyManager;
using Elastos::Droid::App::Admin::CDevicePolicyManager;
using Elastos::Droid::App::Backup::IBackupManager;
using Elastos::Droid::App::Backup::CBackupManager;
using Elastos::Droid::Net::IIConnectivityManager;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::CConnectivityManager;
using Elastos::Droid::Net::Wifi::IWifiManager;
using Elastos::Droid::Net::Wifi::CWifiManager;
using Elastos::Droid::Net::Wifi::IIWifiManager;
using Elastos::Droid::Net::Wifi::EIID_IIWifiManager;
using Elastos::Droid::Net::Wifi::P2p::IWifiP2pManager;
using Elastos::Droid::Net::Wifi::P2p::CWifiP2pManager;
using Elastos::Droid::Net::Wifi::P2p::IIWifiP2pManager;
using Elastos::Droid::Net::Wifi::P2p::EIID_IIWifiP2pManager;
using Elastos::Droid::Net::Ethernet::EIID_IIEthernetManager;
using Elastos::Droid::Internal::Policy::IPolicyManager;
using Elastos::Droid::Internal::Policy::CPolicyManager;
using Elastos::Droid::Internal::Os::IDropBoxManagerService;
using Elastos::Droid::Internal::Os::EIID_IDropBoxManagerService;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::CAudioManager;
using Elastos::Droid::Accounts::CAccountManager;
using Elastos::Droid::Accounts::IIAccountManager;
using Elastos::Droid::Accounts::IAccountManager;
using Elastos::Droid::Accounts::EIID_IIAccountManager;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Droid::Privacy::IPrivacySettingsManager;
using Elastos::Droid::Privacy::CPrivacySettingsManager;

namespace Elastos {
namespace Droid {
namespace App {

CContextImpl::ApplicationContentResolver::ApplicationContentResolver(
    /* [in] */ IContext* context,
    /* [in] */ CActivityThread* mainThread,
    /* [in] */ IUserHandle* user)
    : ContentResolver(context)
    , mMainThread(mainThread)
    , mUser(user)
{
    assert(mainThread != NULL && mUser != NULL);
}

ECode CContextImpl::ApplicationContentResolver::AcquireProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& auth,
    /* [out] */ IIContentProvider** provider)
{
    Int32 identifier;
    mUser->GetIdentifier(&identifier);
    return mMainThread->AcquireProvider(context, auth, identifier, TRUE, provider);
}

ECode CContextImpl::ApplicationContentResolver::AcquireExistingProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& auth,
    /* [out] */ IIContentProvider** provider)
{
    Int32 identifier = 0;
    mUser->GetIdentifier(&identifier);
    return mMainThread->AcquireExistingProvider(context, auth, identifier, TRUE, provider);
}

ECode CContextImpl::ApplicationContentResolver::ReleaseProvider(
    /* [in] */ IIContentProvider* provider,
    /* [out] */ Boolean* result)
{
   return mMainThread->ReleaseProvider(provider, TRUE, result);
}

ECode CContextImpl::ApplicationContentResolver::AcquireUnstableProvider(
    /* [in] */ IContext* c,
    /* [in] */ const String& auth,
    /* [out] */ IIContentProvider** provider)
{
    Int32 identifier = 0;
    mUser->GetIdentifier(&identifier);
    return mMainThread->AcquireProvider(c, auth, identifier, FALSE, provider);
}

ECode CContextImpl::ApplicationContentResolver::ReleaseUnstableProvider(
    /* [in] */ IIContentProvider* icp,
    /* [out] */ Boolean* result)
{
    return mMainThread->ReleaseProvider(icp, FALSE, result);
}

ECode CContextImpl::ApplicationContentResolver::UnstableProviderDied(
    /* [in] */ IIContentProvider* icp)
{
    return mMainThread->HandleUnstableProviderDied(IBinder::Probe(icp), TRUE);
}

const String CContextImpl::TAG("ContextImpl");
const Boolean CContextImpl::DEBUG = FALSE;
HashMap<String, AutoPtr<SharedPreferencesImpl> > CContextImpl::sSharedPrefs;
Mutex CContextImpl::sSharedPrefsLock;
AutoPtr< ArrayOf<String> > CContextImpl::EMPTY_FILE_LIST = ArrayOf<String>::Alloc(0);

AutoPtr<IWallpaperManager> CContextImpl::sWallpaperManager;

CContextImpl::CContextImpl()
    : mThemeResource(0)
    , mRestricted(FALSE)
{
}

CContextImpl::~CContextImpl()
{
    mContentResolver = NULL;
}

ECode CContextImpl::constructor()
{
    AutoPtr<IWeakReferenceSource> wrs = THIS_PROBE(IWeakReferenceSource);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mOuterContext);
    return NOERROR;
}

ECode CContextImpl::constructor(
    /* [in] */ IContextImpl* context)
{
    mPackageInfo = ((CContextImpl*)context)->mPackageInfo;
    mBasePackageName = ((CContextImpl*)context)->mBasePackageName;
    mResources = ((CContextImpl*)context)->mResources;
    mMainThread = ((CContextImpl*)context)->mMainThread;
    mContentResolver = ((CContextImpl*)context)->mContentResolver;
    mUser = ((CContextImpl*)context)->mUser;
    mDisplay = ((CContextImpl*)context)->mDisplay;

    AutoPtr<IWeakReferenceSource> wrs = THIS_PROBE(IWeakReferenceSource);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mOuterContext);
    return NOERROR;
}

AutoPtr<CContextImpl> CContextImpl::GetImpl(
    /* [in] */ IContext* _context)
{
    AutoPtr<IContext> context = _context;
    AutoPtr<IContext> nextContext;
    while ((IContextWrapper::Probe(context) != NULL) &&
            (IContextWrapper::Probe(context)->GetBaseContext((IContext**)&nextContext), nextContext != NULL)) {
        context = nextContext;
        nextContext = NULL;
    }
    return (CContextImpl*)IContextImpl::Probe(context);
}

ECode CContextImpl::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetAssets(assetManager);
    return NOERROR;
}

ECode CContextImpl::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources);
    *resources = mResources;
    REFCOUNT_ADD(*resources);
    return NOERROR;
}

ECode CContextImpl::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    VALIDATE_NOT_NULL(packageManager);
    *packageManager = NULL;

    if (mPackageManager != NULL) {
        *packageManager = mPackageManager;
        REFCOUNT_ADD(*packageManager);
        return NOERROR;
    }

    AutoPtr<IIPackageManager> pm = CActivityThread::GetPackageManager();
    if (pm != NULL) {
        // Doesn't matter if we make more than one instance.
        mPackageManager = new ApplicationPackageManager(this, pm);
        *packageManager = mPackageManager;
        REFCOUNT_ADD(*packageManager);
        return NOERROR;
    }

    return NOERROR;
}

ECode CContextImpl::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    VALIDATE_NOT_NULL(resolver)
    *resolver = (IContentResolver*)mContentResolver.Get();
    REFCOUNT_ADD(*resolver);
    return NOERROR;
}

ECode CContextImpl::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    mMainThread->GetLooper(looper);
    return NOERROR;
}

ECode CContextImpl::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);

    AutoPtr<IApplication> app;
    if (mPackageInfo != NULL) {
        app = mPackageInfo->GetApplication();
    }
    else {
        mMainThread->GetApplication((IApplication**)&app);
    }

    *ctx = IContext::Probe(app);
    REFCOUNT_ADD(*ctx);
    return NOERROR;
}

ECode CContextImpl::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    VALIDATE_NOT_NULL(componentCallback);

    AutoPtr<IContext> ctx;
    GetApplicationContext((IContext**)&ctx);
    ctx->RegisterComponentCallbacks(componentCallback);
    return NOERROR;
}

ECode CContextImpl::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    VALIDATE_NOT_NULL(componentCallback);

    AutoPtr<IContext> ctx;
    GetApplicationContext((IContext**)&ctx);
    ctx->UnregisterComponentCallbacks(componentCallback);
    return NOERROR;
}

ECode CContextImpl::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetText(resId, text);
    return NOERROR;
}

ECode CContextImpl::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetString(resId, str);
    return NOERROR;
}

ECode CContextImpl::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetString(resId, formatArgs, str);
    return NOERROR;
}

ECode CContextImpl::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return NOERROR;
}

ECode CContextImpl::GetThemeResId(
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId);
    *resId = mThemeResource;
    return NOERROR;
}

ECode CContextImpl::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    VALIDATE_NOT_NULL(theme);

    if (mTheme == NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        GetOuterContext()->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 version;
        appInfo->GetTargetSdkVersion(&version);
        mThemeResource = CResources::SelectDefaultTheme(mThemeResource,
                version);
        mResources->NewTheme((IResourcesTheme**)&mTheme);
        mTheme->ApplyStyle(mThemeResource, TRUE);
    }

    *theme = mTheme;
    REFCOUNT_ADD(*theme);
    return NOERROR;
}

ECode CContextImpl::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    theme->ObtainStyledAttributes(attrs, styles);
    return NOERROR;
}

ECode CContextImpl::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    theme->ObtainStyledAttributes(resid, attrs, styles);
    return NOERROR;
}

ECode CContextImpl::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    theme->ObtainStyledAttributes(set, attrs, 0, 0, styles);
    return NOERROR;
}

ECode CContextImpl::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    theme->ObtainStyledAttributes(
            set, attrs, defStyleAttr, defStyleRes, styles);
    return NOERROR;
}

ECode CContextImpl::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);
    *loader = NULL;

    if (mPackageInfo != NULL) {
        mPackageInfo->GetClassLoader(loader);
    }
//    else ClassLoader.getSystemClassLoader();
    return NOERROR;
}

ECode CContextImpl::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = String(NULL);

    if (mPackageInfo != NULL) {
        return mPackageInfo->GetPackageName(packageName);
    }
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    if (mPackageInfo != NULL) {
        return mPackageInfo->GetApplicationInfo(info);
    }
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetPackageResourcePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = String(NULL);

    if (mPackageInfo != NULL) {
        return mPackageInfo->GetResDir(path);
    }
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    VALIDATE_NOT_NULL(codePath);
    *codePath = String(NULL);

    if (mPackageInfo != NULL) {
        return mPackageInfo->GetAppDir(codePath);
    }
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return MakeFilename(GetPreferencesDir(), name + ".xml", file);
}

ECode CContextImpl::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    VALIDATE_NOT_NULL(prefs);

    AutoPtr<SharedPreferencesImpl> sp;
    {
        AutoLock lock(sSharedPrefsLock);
        HashMap<String, AutoPtr<SharedPreferencesImpl> >::Iterator ator = sSharedPrefs.Find(name);
        if (ator == sSharedPrefs.End()) {
            AutoPtr<IFile> prefsFile;
            GetSharedPrefsFile(name, (IFile**)&prefsFile);
            sp = new SharedPreferencesImpl(prefsFile, mode);
            sSharedPrefs[name] = sp;
            *prefs = (ISharedPreferences*)sp->Probe(EIID_ISharedPreferences);
            REFCOUNT_ADD(*prefs);
            return NOERROR;
        }
        sp = ator->mSecond;
    }
    AutoPtr<IApplicationInfo> appInfo;
    GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 version;
    appInfo->GetTargetSdkVersion(&version);
    if ((mode & IContext::MODE_MULTI_PROCESS) != 0 ||
        version < Elastos::Droid::Os::Build::VERSION_CODES::HONEYCOMB) {
        // If somebody else (some other process) changed the prefs
        // file behind our back, we reload it.  This has been the
        // historical (if undocumented) behavior.
        sp->StartReloadIfChangedUnexpectedly();
    }

    *prefs = sp;
    REFCOUNT_ADD(*prefs);
    return NOERROR;
}

AutoPtr<IFile> CContextImpl::GetPreferencesDir()
{
    AutoLock lock(mSyncLock);
    if (mPreferencesDir == NULL) {
        AutoPtr<IFile> file;
        GetDataDirFile((IFile**)&file);
        CFile::New(file, String("shared_prefs"), (IFile**)&mPreferencesDir);
    }
    return mPreferencesDir;
}

ECode CContextImpl::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    AutoPtr<IFile> filesDir;
    GetFilesDir((IFile**)&filesDir);
    AutoPtr<IFile> f;
    MakeFilename(filesDir, name, (IFile**)&f);
    return CFileInputStream::New(f, fileInputStream);
}

ECode CContextImpl::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */IFileOutputStream** fileOutputStream)
{
    Boolean append = (mode&MODE_APPEND) != 0;
    AutoPtr<IFile> filesDir;
    GetFilesDir((IFile**)&filesDir);
    AutoPtr<IFile> f;
    MakeFilename(filesDir, name, (IFile**)&f);
    // try {
    CFileOutputStream::New(f, append, fileOutputStream);
    String path;
    f->GetPath(&path);
    SetFilePermissionsFromMode(path, mode, 0);
    return NOERROR;
    // } catch (FileNotFoundException e) {
    // }

    AutoPtr<IFile> parent;
    f->GetParentFile((IFile**)&parent);
    Boolean res;
    parent->Mkdir(&res);
    String parentPath;
    parent->GetPath(&parentPath);
    FileUtils::SetPermissions(
        parentPath,
        FileUtils::sS_IRWXU|FileUtils::sS_IRWXG|FileUtils::sS_IXOTH,
        -1, -1);
    CFileOutputStream::New(f, append, fileOutputStream);
    SetFilePermissionsFromMode(path, mode, 0);
    return NOERROR;
}

ECode CContextImpl::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    AutoPtr<IFile> filesDir;
    GetFilesDir((IFile**)&filesDir);
    AutoPtr<IFile> f;
    MakeFilename(filesDir, name, (IFile**)&f);
    return f->Delete(succeeded);
}

ECode CContextImpl::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    AutoPtr<IFile> filesDir;
    GetFilesDir((IFile**)&filesDir);
    MakeFilename(filesDir, name, file);
    return NOERROR;
}

ECode CContextImpl::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    AutoLock lock(mSyncLock);
    if (mFilesDir == NULL) {
        AutoPtr<IFile> file;
        GetDataDirFile((IFile**)&file);
        CFile::New(file, String("files"), (IFile**)&mFilesDir);
    }
    Boolean isExist;
    mFilesDir->Exists(&isExist);
    if (!isExist) {
        String path;
        mFilesDir->GetPath(&path);
        Boolean maked;
        if(mFilesDir->Mkdirs(&maked), !maked) {
            Slogger::W(TAG, "Unable to create files directory %s", path.string());
            *filesDir = NULL;
            return NOERROR;
        }
        FileUtils::SetPermissions(
                path,
                FileUtils::sS_IRWXU|FileUtils::sS_IRWXG|FileUtils::sS_IXOTH,
                -1, -1);
    }
    *filesDir = mFilesDir;
    REFCOUNT_ADD(*filesDir);
    return NOERROR;
}

ECode CContextImpl::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    AutoLock lock(mSyncLock);
    if (mExternalFilesDir == NULL) {
        String packageName;
        GetPackageName(&packageName);
        mExternalFilesDir = Environment::GetExternalStorageAppFilesDirectory(
                packageName);
    }
    Boolean isExist;
    if (mExternalFilesDir->Exists(&isExist), !isExist) {
//        try {
        AutoPtr<IFile> file;
        CFile::New(Environment::GetExternalStorageElastosDataDir(),
                String(".nomedia"), (IFile**)&file);
        Boolean succeeded;
        file->CreateNewFile(&succeeded);
//        } catch (IOException e) {
//        }
        Boolean isMk;
        if (mExternalFilesDir->Mkdirs(&isMk), !isMk) {
            Slogger::W(TAG, "Unable to create external files directory");
            *filesDir = NULL;
        }
    }
    if (type.IsNull()) {
        *filesDir = mExternalFilesDir;
        REFCOUNT_ADD(*filesDir);
        return NOERROR;
    }
    AutoPtr<IFile> dir;
    CFile::New(mExternalFilesDir, type, (IFile**)&dir);
    Boolean dirExist;
    if (dir->Exists(&dirExist), !dirExist) {
        Boolean dirMks;
        if (dir->Mkdirs(&dirMks), !dirMks) {
            Slogger::W(TAG, "Unable to create external media directory %p", dir.Get());
            *filesDir = NULL;
            return NOERROR;
        }
    }
    *filesDir = dir;
    REFCOUNT_ADD(*filesDir);
    return NOERROR;
}

ECode CContextImpl::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    AutoLock lock(mSyncLock);
    if (mObbDir == NULL) {
        String pkgName;
        GetPackageName(&pkgName);
        mObbDir = Environment::GetExternalStorageAppObbDirectory(pkgName);
    }
    *obbDir = mObbDir;
    REFCOUNT_ADD(*obbDir);
    return NOERROR;
}

ECode CContextImpl::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    {
        AutoLock lock(mSyncLock);
        if (mCacheDir == NULL) {
            AutoPtr<IFile> file;
            GetDataDirFile((IFile**)&file);
            CFile::New(file, String("cache"), (IFile**)&mCacheDir);
        }
        Boolean isExist;
        if (mCacheDir->Exists(&isExist), !isExist) {
            Boolean isMk;
            if(mCacheDir->Mkdirs(&isMk), !isMk) {
                String aPath;
                mCacheDir->GetAbsolutePath(&aPath);
                Slogger::W(TAG, "Unable to create cache directory %s", aPath.string());
                *cacheDir = NULL;
                return NOERROR;
            }
            String path;
            mCacheDir->GetPath(&path);
            FileUtils::SetPermissions(
                    path,
                    FileUtils::sS_IRWXU|FileUtils::sS_IRWXG|FileUtils::sS_IXOTH,
                    -1, -1);
        }
    }
    *cacheDir = mCacheDir;
    REFCOUNT_ADD(*cacheDir);
    return NOERROR;
}

ECode CContextImpl::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    AutoLock lock(mSyncLock);
    if (mExternalCacheDir == NULL) {
        String pkgName;
        GetPackageName(&pkgName);
        mExternalCacheDir = Environment::GetExternalStorageAppCacheDirectory(
                pkgName);
    }
    Boolean isExist;
    if (mExternalCacheDir->Exists(&isExist), !isExist) {
//        try {
        AutoPtr<IFile> file;
        CFile::New(Environment::GetExternalStorageElastosDataDir(),
                String(".nomedia"), (IFile**)&file);
        Boolean succeeded;
        file->CreateNewFile(&succeeded);
//        } catch (IOException e) {
//        }
        Boolean isMk;
        if (mExternalCacheDir->Mkdirs(&isMk), !isMk) {
            Slogger::W(TAG, "Unable to create external cache directory");
            *externalDir = NULL;
            return NOERROR;
        }
    }
    *externalDir = mExternalCacheDir;
    REFCOUNT_ADD(*externalDir);
    return NOERROR;
}

ECode CContextImpl::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    VALIDATE_NOT_NULL(fileList);

    AutoPtr<IFile> filesDir;
    GetFilesDir((IFile**)&filesDir);
    AutoPtr< ArrayOf<String> > list;
    filesDir->List((ArrayOf<String>**)&list);
    *fileList = (list != NULL) ? list: EMPTY_FILE_LIST;
    REFCOUNT_ADD(*fileList);
    return NOERROR;
}

ECode CContextImpl::GetDataDirFile(
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);
    *file = NULL;

    if (mPackageInfo != NULL) {
        mPackageInfo->GetDataDirFile(file);
        return NOERROR;
    }
    return E_RUNTIME_EXCEPTION;
}

ECode CContextImpl::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir);

    String lname = String("app_") + name;
    AutoPtr<IFile> ff;
    GetDataDirFile((IFile**)&ff);
    AutoPtr<IFile> file;
    MakeFilename(ff, lname, (IFile**)&file);
    Boolean isExist;
    if (file->Exists(&isExist), !isExist) {
        Boolean isMk;
        file->Mkdir(&isMk);
        String path;
        file->GetPath(&path);
        SetFilePermissionsFromMode(path, mode,
                FileUtils::sS_IRWXU|FileUtils::sS_IRWXG|FileUtils::sS_IXOTH);
    }
    *dir = file;
    REFCOUNT_ADD(*dir);
    return NOERROR;
}

ECode CContextImpl::GetUserId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    if (mUser != NULL) {
        return mUser->GetIdentifier(id);
    }
    else {
        *id = 0;
    }
    return NOERROR;
}

ECode CContextImpl::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return OpenOrCreateDatabase(name, mode, factory, NULL, sqliteDB);
}

ECode CContextImpl::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    VALIDATE_NOT_NULL(sqliteDB);

    AutoPtr<IFile> f = ValidateFilePath(name, TRUE);
    assert(f != NULL);
    Int32 flags = ISQLiteDatabase::CREATE_IF_NECESSARY;
    if ((mode & IContext::MODE_ENABLE_WRITE_AHEAD_LOGGING) != 0) {
        flags |= ISQLiteDatabase::ENABLE_WRITE_AHEAD_LOGGING;
    }
    String path;
    f->GetPath(&path);
    AutoPtr<ISQLiteDatabase> db;
    SQLiteDatabase::OpenDatabase(path, factory, flags, errorHandler, (ISQLiteDatabase**)&db);
    SetFilePermissionsFromMode(path, mode, 0);
    *sqliteDB = db;
    REFCOUNT_ADD(*sqliteDB);
    return NOERROR;
}

ECode CContextImpl::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

//    try {
    AutoPtr<IFile> f = ValidateFilePath(name, FALSE);
    return SQLiteDatabase::DeleteDatabase(f, succeeded);
//    } catch (Exception e) {
//    }
}

ECode CContextImpl::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    VALIDATE_NOT_NULL(path);
    AutoPtr<IFile> f = ValidateFilePath(name, FALSE);
    *path = f;
    REFCOUNT_ADD(*path);
    return NOERROR;
}

ECode CContextImpl::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    VALIDATE_NOT_NULL(databaseList);

    AutoPtr< ArrayOf<String> > list;
    GetDatabasesDir()->List((ArrayOf<String>**)&list);
    *databaseList = (list != NULL) ? list: EMPTY_FILE_LIST;
    REFCOUNT_ADD(*databaseList);
    return NOERROR;
}

AutoPtr<IFile> CContextImpl::GetDatabasesDir()
{
    AutoLock lock(mSyncLock);
    if (mDatabasesDir == NULL) {
        AutoPtr<IFile> file;
        GetDataDirFile((IFile**)&file);
        CFile::New(file, String("databases"), (IFile**)&mDatabasesDir);
    }

    String path;
    mDatabasesDir->GetPath(&path);
    if (path.Equals("databases")) {
        mDatabasesDir = NULL;
        CFile::New(String("/data/system"), (IFile**)&mDatabasesDir);
    }
    return mDatabasesDir;
}

ECode CContextImpl::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return GetWallpaperManager()->GetDrawable(drawable);
}

ECode CContextImpl::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return GetWallpaperManager()->PeekDrawable(drawable);
}

ECode CContextImpl::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    return GetWallpaperManager()->GetDesiredMinimumWidth(minWidth);
}

ECode CContextImpl::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    return GetWallpaperManager()->GetDesiredMinimumHeight(minHeight);
}

ECode CContextImpl::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return GetWallpaperManager()->SetBitmap(bitmap);
}

ECode CContextImpl::SetWallpaper(
    /* [in] */ IInputStream* data)
{
    return GetWallpaperManager()->SetStream(data);
}

ECode CContextImpl::ClearWallpaper()
{
    return GetWallpaperManager()->Clear();
}

ECode CContextImpl::StartActivity(
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent);

    WarnIfCallingFromSystemProcess();
    StartActivity(intent, NULL);
    return NOERROR;
}

ECode CContextImpl::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    VALIDATE_NOT_NULL(intent);

    WarnIfCallingFromSystemProcess();
    Int32 flags;
    intent->GetFlags(&flags);
    if ((flags&IIntent::FLAG_ACTIVITY_NEW_TASK) == 0) {
        return E_RUNTIME_EXCEPTION;
//        throw new AndroidRuntimeException(
//                "Calling startActivity() from outside of an Activity "
//                + " context requires the FLAG_ACTIVITY_NEW_TASK flag."
//                + " Is this really what you want?");
    }
    AutoPtr<IInstrumentation> instrumentation;
    mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    AutoPtr<IContext> ctx = GetOuterContext();
    AutoPtr<IInstrumentationActivityResult> instrumentationResult;
    instrumentation->ExecStartActivity(
        ctx, IBinder::Probe(appThread), NULL,
        NULL, intent, -1, options, (IInstrumentationActivityResult**)&instrumentationResult);
    return NOERROR;
}

ECode CContextImpl::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return StartActivityAsUser(intent, NULL, user);
}

ECode CContextImpl::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* user)
{
    VALIDATE_NOT_NULL(intent);
    VALIDATE_NOT_NULL(user);

//    try {
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    String type;
    AutoPtr<IContentResolver> resolver;
    GetContentResolver((IContentResolver**)&resolver);
    intent->ResolveTypeIfNeeded(resolver, &type);
    Int32 id, result;
    user->GetIdentifier(&id);
    ActivityManagerNative::GetDefault()->StartActivityAsUser(
        appThread, intent, type,
        NULL, String(NULL), 0, IIntent::FLAG_ACTIVITY_NEW_TASK, String(NULL), NULL, options,
        id, &result);
    return NOERROR;
//    } catch (RemoteException re) {
//    }
}

ECode CContextImpl::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    VALIDATE_NOT_NULL(intents);

    WarnIfCallingFromSystemProcess();
    StartActivities(intents, NULL);
    return NOERROR;
}

ECode CContextImpl::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    VALIDATE_NOT_NULL(intents);
    VALIDATE_NOT_NULL(options);

    WarnIfCallingFromSystemProcess();
    Int32 flags;
    (*intents)[0]->GetFlags(&flags);
    if ((flags&IIntent::FLAG_ACTIVITY_NEW_TASK) == 0) {
        return E_RUNTIME_EXCEPTION;
//        throw new AndroidRuntimeException(
//                "Calling startActivities() from outside of an Activity "
//                + " context requires the FLAG_ACTIVITY_NEW_TASK flag on first Intent."
//                + " Is this really what you want?");
    }
    AutoPtr<IInstrumentation> instrumentation;
    mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    AutoPtr<IContext> ctx = GetOuterContext();
    instrumentation->ExecStartActivities(
        ctx, IBinder::Probe(appThread.Get()), NULL, NULL, intents, options);
    return NOERROR;
}

ECode CContextImpl::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    Int32 flags;
    (*intents)[0]->GetFlags(&flags);
    if ((flags&IIntent::FLAG_ACTIVITY_NEW_TASK) == 0) {
        return E_RUNTIME_EXCEPTION;
//        throw new AndroidRuntimeException(
//                "Calling startActivities() from outside of an Activity "
//                + " context requires the FLAG_ACTIVITY_NEW_TASK flag on first Intent."
//                + " Is this really what you want?");
    }
    AutoPtr<IInstrumentation> instrumentation;
    mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    userHandle->GetIdentifier(&id);
    AutoPtr<IContext> ctx = GetOuterContext();
    instrumentation->ExecStartActivitiesAsUser(
        ctx, IBinder::Probe(appThread.Get()), NULL,
        NULL, intents, options, id);
    return NOERROR;
}

ECode CContextImpl::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return StartIntentSender(intent, fillInIntent, flagsMask, flagsValues, extraFlags, NULL);
}

ECode CContextImpl::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
//    try {
    String resolvedType = String(NULL);
    if (fillInIntent != NULL) {
        fillInIntent->SetAllowFds(FALSE);
        AutoPtr<IContentResolver> contentResolver;
        GetContentResolver((IContentResolver**)&contentResolver);
        fillInIntent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
    }
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 result;
    ActivityManagerNative::GetDefault()->
        StartActivityIntentSender(appThread, intent,
                fillInIntent, resolvedType, NULL, String(NULL),
                0, flagsMask, flagsValues, options, &result);
    if (result == IActivityManager::START_CANCELED) {
        return E_SEND_INTENT_EXCEPTION;
//        throw new IntentSender.SendIntentException();
    }
    CInstrumentation::CheckStartActivityResult(result, NULL);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    WarnIfCallingFromSystemProcess();
    String resolvedType;
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    GetUserId(&id);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, NULL, IActivity::RESULT_OK,
        String(NULL), NULL, String(NULL), FALSE, FALSE, id, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    WarnIfCallingFromSystemProcess();
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//     try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    GetUserId(&id);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, NULL,
        IActivity::RESULT_OK, String(NULL), NULL, receiverPermission, FALSE, FALSE,
        id, &result);
//     } catch (RemoteException e) {
//     }
    return NOERROR;
}

ECode CContextImpl::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    WarnIfCallingFromSystemProcess();
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    GetUserId(&id);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, NULL,
        IActivity::RESULT_OK, String(NULL), NULL, receiverPermission, TRUE, FALSE,
        id, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    WarnIfCallingFromSystemProcess();
    AutoPtr<IIntentReceiver> rd;
    if (resultReceiver != NULL) {
        if (mPackageInfo != NULL) {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IInstrumentation> instrumentation;
            mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
            AutoPtr<IContext> ctx = GetOuterContext();
            mPackageInfo->GetReceiverDispatcher(
                resultReceiver, ctx, scheduler,
                instrumentation, FALSE, (IIntentReceiver**)&rd);
        } else {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IContext> ctx = GetOuterContext();
            LoadedPkg::ReceiverDispatcher* dispatcher = new LoadedPkg::ReceiverDispatcher(
                    resultReceiver, ctx, scheduler, NULL, FALSE);
            rd = dispatcher->GetIIntentReceiver();
        }
    }
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    GetUserId(&id);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, rd,
        initialCode, initialData, initialExtras, receiverPermission,
        TRUE, FALSE, id, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
        intent->SetAllowFds(FALSE);
        AutoPtr<IApplicationThread> appThread;
        mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
        Int32 identifier;
        user->GetIdentifier(&identifier);
        Int32 result;
        ActivityManagerNative::GetDefault()->BroadcastIntent(appThread,
                intent, resolvedType, NULL, IActivity::RESULT_OK, String(NULL),
                NULL, String(NULL), FALSE, FALSE, identifier, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
        intent->SetAllowFds(FALSE);
        AutoPtr<IApplicationThread> appThread;
        mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
        Int32 identifier = 0;
        user->GetIdentifier(&identifier);
        Int32 result;
        ActivityManagerNative::GetDefault()->BroadcastIntent(
            appThread, intent, resolvedType, NULL,
            IActivity::RESULT_OK, String(NULL), NULL, receiverPermission, FALSE, FALSE,
            identifier, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    AutoPtr<IIntentReceiver> rd;
    if (resultReceiver != NULL) {
        if (mPackageInfo != NULL) {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IInstrumentation> instrumentation;
            mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
            AutoPtr<IContext> ctx = GetOuterContext();
            mPackageInfo->GetReceiverDispatcher(
                resultReceiver, ctx, scheduler,
                instrumentation, FALSE, (IIntentReceiver**)&rd);
        } else {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IContext> ctx = GetOuterContext();
            LoadedPkg::ReceiverDispatcher* dispatcher = new LoadedPkg::ReceiverDispatcher(
                    resultReceiver, ctx, scheduler, NULL, FALSE);
            rd = dispatcher->GetIIntentReceiver();
        }
    }
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 identifier = 0;
    user->GetIdentifier(&identifier);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, rd,
        initialCode, initialData, initialExtras, receiverPermission,
        TRUE, FALSE, identifier, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    WarnIfCallingFromSystemProcess();
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id = 0;
    GetUserId(&id);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, NULL,
        IActivity::RESULT_OK, String(NULL), NULL, String(NULL), FALSE, TRUE,
        id, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendStickyOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    WarnIfCallingFromSystemProcess();
    AutoPtr<IIntentReceiver> rd;
    if (resultReceiver != NULL) {
        if (mPackageInfo != NULL) {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IInstrumentation> instrumentation;
            mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
            AutoPtr<IContext> ctx = GetOuterContext();
            mPackageInfo->GetReceiverDispatcher(
                resultReceiver, ctx, scheduler,
                instrumentation, FALSE, (IIntentReceiver**)&rd);
        } else {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IContext> ctx = GetOuterContext();
            LoadedPkg::ReceiverDispatcher* dispatcher = new LoadedPkg::ReceiverDispatcher(
                    resultReceiver, ctx, scheduler, NULL, FALSE);
            rd = dispatcher->GetIIntentReceiver();
        }
    }
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    GetUserId(&id);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, rd,
        initialCode, initialData, initialExtras, String(NULL),
        TRUE, TRUE, id, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::RemoveStickyBroadcast(
    /* [in] */ IIntent* inIntent)
{
    AutoPtr<IIntent> intent = inIntent;
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
    if (!resolvedType.IsNull()) {
        intent = NULL;
        CIntent::New(inIntent, (IIntent**)&intent);
        AutoPtr<IUri> data;
        intent->GetData((IUri**)&data);
        intent->SetDataAndType(data, resolvedType);
    }
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 id;
    GetUserId(&id);
    ActivityManagerNative::GetDefault()->UnbroadcastIntent(
            appThread, intent, id);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
        intent->SetAllowFds(FALSE);
        AutoPtr<IApplicationThread> appThread;
        mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
        Int32 identifier;
        user->GetIdentifier(&identifier);
        Int32 result;
        ActivityManagerNative::GetDefault()->BroadcastIntent(
            appThread, intent, resolvedType, NULL,
            IActivity::RESULT_OK, String(NULL), NULL, String(NULL), FALSE, TRUE, identifier, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::SendStickyOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    AutoPtr<IIntentReceiver> rd;
    if (resultReceiver != NULL) {
        if (mPackageInfo != NULL) {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IInstrumentation> instrumentation;
            mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
            AutoPtr<IContext> ctx = GetOuterContext();
            mPackageInfo->GetReceiverDispatcher(
                resultReceiver, ctx, scheduler,
                instrumentation, FALSE, (IIntentReceiver**)&rd);
        } else {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IContext> ctx = GetOuterContext();
            LoadedPkg::ReceiverDispatcher* dispatcher = new LoadedPkg::ReceiverDispatcher(
                    resultReceiver, ctx, scheduler, NULL, FALSE);
            rd = dispatcher->GetIIntentReceiver();
        }
    }
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
//    try {
    intent->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    Int32 identifier;
    user->GetIdentifier(&identifier);
    Int32 result;
    ActivityManagerNative::GetDefault()->BroadcastIntent(
        appThread, intent, resolvedType, rd,
        initialCode, initialData, initialExtras, String(NULL),
        TRUE, TRUE, identifier, &result);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* inIntent,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IIntent> intent = inIntent;
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String resolvedType;
    intent->ResolveTypeIfNeeded(contentResolver, &resolvedType);
    if (resolvedType != String(NULL)) {
        intent = NULL;
        CIntent::New(inIntent, (IIntent**)&intent);
        AutoPtr<IUri> data;
        intent->GetData((IUri**)&data);
        intent->SetDataAndType(data, resolvedType);
    }
//    try {
        intent->SetAllowFds(FALSE);
        AutoPtr<IApplicationThread> appThread;
        mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
        Int32 identifier;
        user->GetIdentifier(&identifier);
        ActivityManagerNative::GetDefault()->UnbroadcastIntent(
                appThread, intent, identifier);
//    } catch (RemoteException e) {
//    }
    return NOERROR;
}

ECode CContextImpl::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** intent)
{
    return RegisterReceiver(receiver, filter, String(NULL), NULL, intent);
}

ECode CContextImpl::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** intent)
{
    Int32 id = 0;
    GetUserId(&id);
    AutoPtr<IContext> ctx = GetOuterContext();
    return RegisterReceiverInternal(receiver, id,
            filter, broadcastPermission, scheduler, ctx, intent);
}

ECode CContextImpl::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    Int32 identifier;
    user->GetIdentifier(&identifier);
    AutoPtr<IContext> ctx = GetOuterContext();
    return RegisterReceiverInternal(receiver, identifier,
            filter, broadcastPermission, scheduler, ctx, stickyIntent);
}

ECode CContextImpl::RegisterReceiverInternal(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ Int32 userId,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [in] */ IContext* context,
    /* [out] */ IIntent** intent)
{
    AutoPtr<IIntentReceiver> rd;
    if (receiver != NULL) {
        if (mPackageInfo != NULL && context != NULL) {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<IInstrumentation> instrumentation;
            mMainThread->GetInstrumentation((IInstrumentation**)&instrumentation);
            mPackageInfo->GetReceiverDispatcher(
                receiver, context, scheduler,
                instrumentation, TRUE, (IIntentReceiver**)&rd);
        }
        else {
            if (scheduler == NULL) {
                scheduler = mMainThread->GetHandler();
            }
            AutoPtr<LoadedPkg::ReceiverDispatcher> dispatcher = new LoadedPkg::ReceiverDispatcher(
                    receiver, context, scheduler, NULL, TRUE);
            rd = dispatcher->GetIIntentReceiver();
        }
    }
//     try {
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    ActivityManagerNative::GetDefault()->RegisterReceiver(
            appThread, mBasePackageName,
            rd, filter, broadcastPermission, userId, intent);
//     } catch (RemoteException e) {
//         return null;
//     }
    return NOERROR;
}

ECode CContextImpl::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    if (mPackageInfo != NULL) {
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IIntentReceiver> rd;
        mPackageInfo->ForgetReceiverDispatcher(ctx, receiver, (IIntentReceiver**)&rd);
//        try {
        ActivityManagerNative::GetDefault()->UnregisterReceiver(rd);
//        } catch (RemoteException e) {
//        }
        return NOERROR;
    } else {
        return E_RUNTIME_EXCEPTION;
//        throw new RuntimeException("Not supported in system context");
    }
}

ECode CContextImpl::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    WarnIfCallingFromSystemProcess();
    return StartServiceAsUser(service, mUser, name);
}

ECode CContextImpl::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    WarnIfCallingFromSystemProcess();
    return StopServiceAsUser(service, mUser, succeeded);
}

ECode CContextImpl::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = NULL;
    VALIDATE_NOT_NULL(user);

//     try {
        service->SetAllowFds(FALSE);
        AutoPtr<IApplicationThread> appThread;
        mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
        AutoPtr<IContentResolver> contentResolver;
        GetContentResolver((IContentResolver**)&contentResolver);
        String type;
        service->ResolveTypeIfNeeded(contentResolver, &type);
        Int32 identifier = 0;
        user->GetIdentifier(&identifier);
        AutoPtr<IComponentName> cn;
        ActivityManagerNative::GetDefault()->StartService(
            appThread, service, type, identifier, (IComponentName**)&cn);
        if (cn != NULL) {
            String pkgName, className;
            cn->GetPackageName(&pkgName);
            cn->GetClassName(&className);
            if (pkgName.Equals("!")) {
                Slogger::E(TAG, "Not allowed to start service without permission: %s", className.string());
                return E_SECURITY_EXCEPTION;
            }
            else if (pkgName.Equals("!!")) {
                Slogger::E(TAG, "Unable to start service: %s", className.string());
                return E_SECURITY_EXCEPTION;
            }
        }
        *name = cn;
        REFCOUNT_ADD(*name);
        return NOERROR;
//     } catch (RemoteException e) {
//         return null;
//     }
}

ECode CContextImpl::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = FALSE;
    VALIDATE_NOT_NULL(user);
    VALIDATE_NOT_NULL(service);

//     try {
    service->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    AutoPtr<IContentResolver> contentResolver;
    GetContentResolver((IContentResolver**)&contentResolver);
    String type;
    service->ResolveTypeIfNeeded(contentResolver, &type);
    Int32 identifier;
    user->GetIdentifier(&identifier);
    Int32 res;
    ActivityManagerNative::GetDefault()->StopService(
        appThread, service, type, identifier, &res);
    if (res < 0) {
        String intentStr;
        service->ToString(&intentStr);
        Slogger::E(TAG, "Not allowed to stop service : %s", intentStr.string());
        return E_SECURITY_EXCEPTION;
    }
    *succeeded = res != 0;
    return NOERROR;
//     } catch (RemoteException e) {
//         return false;
//     }
}

ECode CContextImpl::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    WarnIfCallingFromSystemProcess();
    return BindService(service, conn, flags, UserHandle::GetUserId(Process::MyUid()), succeeded);
}

ECode CContextImpl::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = FALSE;

    AutoPtr<IIServiceConnection> sd;
    if (conn == NULL) {
        Slogger::E(TAG, "connection is null");
//        throw new IllegalArgumentException("connection is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mPackageInfo != NULL) {
        AutoPtr<IContext> ctx = GetOuterContext();
        mPackageInfo->GetServiceDispatcher(conn, ctx,
                mMainThread->GetHandler(), flags, (IIServiceConnection**)&sd);
    }
    else {
        Slogger::E(TAG, "Not supported in system context");
//        throw new RuntimeException("Not supported in system context");
        return E_RUNTIME_EXCEPTION;
    }
//    try {
    AutoPtr<IBinder> token = GetActivityToken();
    AutoPtr<IApplicationInfo> appInfo;
    mPackageInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 version;
    appInfo->GetTargetSdkVersion(&version);
    if (token == NULL && (flags & IContext::BIND_AUTO_CREATE) == 0 && mPackageInfo != NULL
           && version < Elastos::Droid::Os::Build::VERSION_CODES::ICE_CREAM_SANDWICH) {
       flags |= BIND_WAIVE_PRIORITY;
    }
    service->SetAllowFds(FALSE);
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    AutoPtr<IContentResolver> cResolver;
    GetContentResolver((IContentResolver**)&cResolver);
    String type;
    service->ResolveTypeIfNeeded(cResolver, &type);
    Int32 res;
    ActivityManagerNative::GetDefault()->BindService(
        appThread, GetActivityToken(),
        service, type, sd, flags, userHandle, &res);
    if (res < 0) {
        String info;
        service->ToString(&info);
        Slogger::E(TAG, "Not allowed to bind to service %s", info.string());
//            throw new SecurityException(
//                    "Not allowed to bind to service " + service);
        return E_SECURITY_EXCEPTION;
    }
    *succeeded = res != 0;
    return NOERROR;
//    } catch (RemoteException e) {
//        return false;
//    }
}

ECode CContextImpl::UnbindService(
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn)
{
    if (conn == NULL) {
        Slogger::E(TAG, "connection is null");
//        throw new IllegalArgumentException("connection is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mPackageInfo != NULL) {
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IIServiceConnection> sd;
        mPackageInfo->ForgetServiceDispatcher(ctx, conn, (IIServiceConnection**)&sd);
//        try {
        Boolean unbinded;
        ActivityManagerNative::GetDefault()->UnbindService(sd, &unbinded);
//        } catch (RemoteException e) {
//        }
    }
    else {
        Slogger::E(TAG, "Not supported in system context");
//        throw new RuntimeException("Not supported in system context");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CContextImpl::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

//    try {
    if (arguments != NULL) {
        arguments->SetAllowFds(FALSE);
    }
    Int32 id;
    GetUserId(&id);
    return ActivityManagerNative::GetDefault()->StartInstrumentation(
            className, profileFile, 0, arguments, NULL, id, succeeded);
//    } catch (RemoteException e) {
//        // System has crashed, nothing we can do.
//    }
    *succeeded = FALSE;
    return NOERROR;
}

ECode CContextImpl::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = NULL;

    HashMap< String, AutoPtr<IInterface> >::Iterator it = mServiceCache.Find(name);
    if (it != mServiceCache.End()) {
        *object = it->mSecond;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }

    if (IContext::PRIVACY_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        // BEGIN privacy-added
        // Log.d(TAG, "PDroid:ContextImpl: Creating static privacy service");
        // IBinder b = ServiceManager.getService("privacy");
        // IPrivacySettingsManager service = IPrivacySettingsManager.Stub.asInterface(b);
        AutoPtr<IInterface> b = ServiceManager::GetService(PRIVACY_SERVICE);
        AutoPtr<IIPrivacySettingsManager> service = IIPrivacySettingsManager::Probe(b);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IPrivacySettingsManager> privacySettingsManager;
        CPrivacySettingsManager::New(ctx, service, (IPrivacySettingsManager**)&privacySettingsManager);
        *object = privacySettingsManager.Get();
        REFCOUNT_ADD(*object);
        // END privacy-added
        return NOERROR;
    }
    else if (IContext::WINDOW_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IDisplay> display = mDisplay;
        if (display == NULL) {
            AutoPtr<IDisplayManager> dm;
            GetOuterContext()->GetSystemService(
                    IContext::DISPLAY_SERVICE, (IInterface**)&dm);
            dm->GetDisplay(IDisplay::DEFAULT_DISPLAY, (IDisplay**)&display);
        }
        *object = new WindowManagerImpl(display);
        mServiceCache[name] = *object;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::LAYOUT_INFLATER_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IPolicyManager> pm;
        FAIL_RETURN(CPolicyManager::AcquireSingleton((IPolicyManager**)&pm));
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<ILayoutInflater> inflater;
        FAIL_RETURN(pm->MakeNewLayoutInflater(ctx, (ILayoutInflater**)&inflater));
        *object = inflater.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::ACCOUNT_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IInterface> service = ServiceManager::GetService(IContext::ACCOUNT_SERVICE);
        AutoPtr<IIAccountManager> accountService = (IIAccountManager*)service->Probe(EIID_IIAccountManager);
        AutoPtr<IAccountManager> accountManager;
        AutoPtr<IContext> ctx = GetOuterContext();
        CAccountManager::New(ctx, accountService, (IAccountManager**)&accountManager);
        *object = accountManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::ACTIVITY_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IActivityManager> activityManager;
        FAIL_RETURN(CActivityManager::New(ctx, mMainThread->GetHandler(),
                (IActivityManager**)&activityManager));
        *object = activityManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::INPUT_METHOD_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IInputMethodManager> iManager =
        CInputMethodManager::GetInstance(this);
        mServiceCache[name] = iManager;
        *object = iManager;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::ALARM_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IInterface> service = ServiceManager::GetService(IContext::ALARM_SERVICE);
        AutoPtr<IIAlarmManager> alarmService = IIAlarmManager::Probe(service.Get());
        assert(alarmService != NULL);
        AutoPtr<IAlarmManager> alarmManager;
        CAlarmManager::New(alarmService , (IAlarmManager**)&alarmManager);
        *object = alarmManager.Get();
        mServiceCache[name] = *object;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::BACKUP_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IBackupManager> backupManager;
        CBackupManager::New(ctx, (IBackupManager**)&backupManager);
        *object = backupManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::ACCOUNT_SERVICE.Equals(name)) {
        Slogger::E(TAG, " >>> TODO: Service %s is not ready!", name.string());
        return E_NOT_IMPLEMENTED;
    }
    else if (IContext::POWER_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IInterface> service = ServiceManager::GetService(IContext::POWER_SERVICE);
        AutoPtr<IIPowerManager> powerService = (IIPowerManager*)service->Probe(EIID_IIPowerManager);
        AutoPtr<IPowerManager> powerManager;
        AutoPtr<IContext> ctx = GetOuterContext();
        CPowerManager::New(ctx, powerService, mMainThread->GetHandler(), (IPowerManager**)&powerManager);
        *object = powerManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::CONNECTIVITY_SERVICE.Equals(name)) {
        AutoPtr<IIConnectivityManager> service =
                (IIConnectivityManager*)ServiceManager::GetService(IContext::CONNECTIVITY_SERVICE).Get();
        AutoPtr<IConnectivityManager> connManager;
        CConnectivityManager::New(service , (IConnectivityManager**)&connManager);
        *object = connManager.Get();
        mServiceCache[name] = *object;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::DISPLAY_SERVICE.Equals(name)) {
        AutoPtr<IContext> ctx = GetOuterContext();
        return CDisplayManager::New(ctx, (IDisplayManager**)object);
    }
    else if (IContext::THROTTLE_SERVICE.Equals(name)) {
        Slogger::E(TAG, " >>> TODO: Service %s is not ready!", name.string());
        return E_NOT_IMPLEMENTED;
    }
    else if (IContext::WIFI_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IInterface> b = ServiceManager::GetService(IContext::WIFI_SERVICE);
        AutoPtr<IIWifiManager> service = (IIWifiManager*)b->Probe(EIID_IIWifiManager);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IWifiManager> wifiManager;
        CWifiManager::New(ctx, service, (IWifiManager**)&wifiManager);
        *object = wifiManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::WIFI_P2P_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IInterface> b = ServiceManager::GetService(IContext::WIFI_P2P_SERVICE);
        assert(b != NULL);
        AutoPtr<IIWifiP2pManager> service = (IIWifiP2pManager*)b->Probe(EIID_IIWifiP2pManager);
        assert(service != NULL);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IWifiP2pManager> wifiP2pManager;
        CWifiP2pManager::New((IWifiP2pManager**)&wifiP2pManager);
        wifiP2pManager->SetService(service);
        *object = wifiP2pManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::NOTIFICATION_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IApplicationInfo> appInfo;
        ctx->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 version;
        appInfo->GetTargetSdkVersion(&version);
        Int32 value = CResources::SelectSystemTheme(0,
            version,
            R::style::Theme_Dialog,
            R::style::Theme_Holo_Dialog,
            R::style::Theme_DeviceDefault_Dialog);
        AutoPtr<IContextThemeWrapper> wrapper;
        CContextThemeWrapper::New(ctx, value, (IContextThemeWrapper**)&wrapper);
        *object = new NotificationManager(wrapper, mMainThread->GetHandler());
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::KEYGUARD_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IKeyguardManager> kgManager;
        CKeyguardManager::New((IKeyguardManager**)&kgManager);
        mServiceCache[name] = kgManager.Get();
        *object = kgManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::ACCESSIBILITY_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IAccessibilityManager> aManager;
        CAccessibilityManager::GetInstance(this, (IAccessibilityManager**)&aManager);
        *object = aManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::LOCATION_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IILocationManager> locationManager;
        AutoPtr<IInterface> lm = ServiceManager::GetService(IContext::LOCATION_SERVICE);
        if(lm != NULL)
            locationManager = (IILocationManager*)(lm->Probe(EIID_IILocationManager));

        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IPrivacyLocationManager> privacyLocationManager;
        // TODO Mike： Change GetOuterContext() to GetStaticOuterContext()
        CPrivacyLocationManager::New(locationManager, ctx/*GetStaticOuterContext()*/, (IPrivacyLocationManager**)&privacyLocationManager);

        *object = privacyLocationManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::SEARCH_SERVICE.Equals(name)) {
        Slogger::E(TAG, " >>> TODO: Service %s is not ready!", name.string());
        return E_NOT_IMPLEMENTED;
    }
    else if (IContext::SENSOR_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        //return new SystemSensorManager(ctx.getOuterContext(),ctx.mMainThread.getHandler().getLooper());
        AutoPtr<ILooper> looper;
        mMainThread->GetHandler()->GetLooper((ILooper**)&looper);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<ISystemSensorManager> managersensor;
        CSystemSensorManager::New(ctx, looper, (ISystemSensorManager**)&managersensor);
        *object = managersensor.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::STORAGE_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        // try {
        AutoPtr<ILooper> looper;
        mMainThread->GetHandler()->GetLooper((ILooper**)&looper);
        AutoPtr<IStorageManager> sManager;
        CStorageManager::New(looper, (IStorageManager**)&sManager);
        assert(sManager != NULL);
        *object = sManager.Get();
        mServiceCache[name] = *object;
        REFCOUNT_ADD(*object);
        // } catch (RemoteException rex) {
        //     Log.e(TAG, "Failed to create StorageManager", rex);
        //     return null;
        // }
        return NOERROR;
    }
    else if (IContext::USB_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IIUsbManager> service = (IIUsbManager*)ServiceManager::GetService(IContext::USB_SERVICE).Get();
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IUsbManager> usbManager;
        CUsbManager::New(ctx, service, (IUsbManager**)&usbManager);
        *object = usbManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;

    }
    else if (IContext::SERIAL_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IInterface> service = ServiceManager::GetService(IContext::SERIAL_SERVICE);
        assert(service != NULL);
        AutoPtr<IISerialManager> serialService = (IISerialManager*)service->Probe(EIID_IISerialManager);
        assert(serialService != NULL);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<ISerialManager> serialManager;
        CSerialManager::New(ctx, serialService , (ISerialManager**)&serialManager);
        *object = serialManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::VIBRATOR_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IVibratorService> service = IVibratorService::Probe(ServiceManager::GetService(IContext::VIBRATOR_SERVICE).Get());
        mServiceCache[name] = service.Get();
        *object = service.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::STATUS_BAR_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IStatusBarManager> statusBarManager;
        ASSERT_SUCCEEDED(CStatusBarManager::New(ctx, (IStatusBarManager**)&statusBarManager));
        *object = statusBarManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::AUDIO_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IAudioManager> aManager;
        CAudioManager::New(this, (IAudioManager**)&aManager);
        *object = aManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::TELEPHONY_SERVICE.Equals(name)) {
        Slogger::E(TAG, " >>> TODO: Service %s is not ready!", name.string());
        return E_NOT_IMPLEMENTED;
    }
    else if (IContext::CLIPBOARD_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IClipboardManager> cbm;
        AutoPtr<IContext> ctx = GetOuterContext();
        FAIL_RETURN(CClipboardManager::New(ctx, (IClipboardManager**)&cbm))
        *object = cbm.Get();
        REFCOUNT_ADD(*object)
        return NOERROR;
    }
    else if (IContext::WALLPAPER_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        *object = GetWallpaperManager().Get();
        mServiceCache[name] = GetWallpaperManager().Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::DROPBOX_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IInterface> b = ServiceManager::GetService(IContext::DROPBOX_SERVICE);
        AutoPtr<IDropBoxManagerService> service = IDropBoxManagerService::Probe(b);
        if (service == NULL) {
            // Don't return a DropBoxManager that will NPE upon use.
            // This also avoids caching a broken DropBoxManager in
            // getDropBoxManager during early boot, before the
            // DROPBOX_SERVICE is registered.
            mServiceCache[name] = NULL;
            *object = NULL;
            return NOERROR;
        }
        AutoPtr<IDropBoxManager> dbm;
        CDropBoxManager::New(service, (IDropBoxManager**)&dbm);
        mServiceCache[name] = dbm.Get();
        *object = dbm.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::DEVICE_POLICY_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IDevicePolicyManager> devicePolicy;
        AutoPtr<IHandler> handler = mMainThread->GetHandler();
        CDevicePolicyManager::New(ctx, handler, (IDevicePolicyManager**)&devicePolicy);
        *object = devicePolicy.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::UI_MODE_SERVICE.Equals(name)) {
        AutoLock lock(mCacheLock);

        AutoPtr<IUiModeManager> uManager;
        CUiModeManager::New((IUiModeManager**)&uManager);
        *object = uManager.Get();
        mServiceCache[name] = uManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::DOWNLOAD_SERVICE.Equals(name)) {
        AutoPtr<IContext> ctx = GetOuterContext();
        AutoPtr<IContentResolver> resolver;
        ctx->GetContentResolver((IContentResolver**)&resolver);
        String pkgName;
        ctx->GetPackageName(&pkgName);
        AutoPtr<IDownloadManager> downloadManager;
        CDownloadManager::New(resolver, pkgName, (IDownloadManager**)&downloadManager);
        *object = downloadManager.Get();
        mServiceCache[name] = downloadManager.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::NFC_SERVICE.Equals(name)) {
        Slogger::E(TAG, " >>> TODO: Service %s is not ready!", name.string());
        return E_NOT_IMPLEMENTED;
    }
    else if (IContext::USER_SERVICE.Equals(name)) {
        AutoPtr<IInterface> b = ServiceManager::GetService(IContext::USER_SERVICE);
        AutoPtr<IIUserManager> service = IIUserManager::Probe(b);
        AutoPtr<IUserManager> userMgr;
        CUserManager::New(this, service, (IUserManager**)&userMgr);
        *object = userMgr.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::DISPLAY_SERVICE_AW.Equals(name)) {
        AutoPtr<IDisplayManagerAw> dmAw;
        CDisplayManagerAw::New((IDisplayManagerAw**)&dmAw);
        *object = dmAw.Get();
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    return NOERROR;
}

AutoPtr<IWallpaperManager> CContextImpl::GetWallpaperManager()
{
    if (sWallpaperManager == NULL) {
        CWallpaperManager::New(GetOuterContext(), NULL, (IWallpaperManager**)&sWallpaperManager);
    }

    return sWallpaperManager;
}

// AutoPtr<IDropBoxManager> CreateDropBoxManager()
// {
//     IBinder b = ServiceManager.getService(DROPBOX_SERVICE);
//     IDropBoxManagerService service = IDropBoxManagerService.Stub.asInterface(b);
//     if (service == null) {
//         // Don't return a DropBoxManager that will NPE upon use.
//         // This also avoids caching a broken DropBoxManager in
//         // getDropBoxManager during early boot, before the
//         // DROPBOX_SERVICE is registered.
//         return null;
//     }
//     return new DropBoxManager(service);
// }

ECode CContextImpl::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    if (permission.IsNull()) {
//        throw new IllegalArgumentException("permission is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    try {
    ECode ec = ActivityManagerNative::GetDefault()->CheckPermission(
            permission, pid, uid, result);
    if (FAILED(ec)) *result = IPackageManager::PERMISSION_DENIED;
    return ec;
//    } catch (RemoteException e) {
//        return PackageManager.PERMISSION_DENIED;
//    }
}

ECode CContextImpl::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;

    if (permission.IsNull()) {
//        throw new IllegalArgumentException("permission is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 pid = Binder::GetCallingPid();
    if (pid != Process::MyPid()) {
        return CheckPermission(permission, pid, Binder::GetCallingUid(), value);
    }
    *value = IPackageManager::PERMISSION_DENIED;
    return NOERROR;
}

ECode CContextImpl::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    VALIDATE_NOT_NULL(perm);
    *perm = 0;

    if (permission.IsNull()) {
//        throw new IllegalArgumentException("permission is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return CheckPermission(permission, Binder::GetCallingPid(),
            Binder::GetCallingUid(), perm);
}

ECode CContextImpl::Enforce(
    /* [in] */ const String& permission,
    /* [in] */ Int32 resultOfCheck,
    /* [in] */ Boolean selfToo,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    if (resultOfCheck != IPackageManager::PERMISSION_GRANTED) {
        StringBuilder sb("");
        if (!message.IsNull()) {
            sb += message;
            sb += ": ";
        }
        if (selfToo) {
            sb += "Neither user ";
            sb += uid;
            sb += " nor current process has ";
        }
        else {
            sb += "uid ";
            sb += uid;
            sb += " does not have ";
        }
        sb += permission;
        sb += ".";
        Slogger::E(TAG, sb.ToString());
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}

ECode CContextImpl::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    Int32 res;
    CheckPermission(permission, pid, uid, &res);
    return Enforce(permission,
            res,
            FALSE,
            uid,
            message);
}

ECode CContextImpl::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    Int32 res;
    CheckCallingPermission(permission, &res);
    return Enforce(permission,
            res,
            false,
            Binder::GetCallingUid(),
            message);
}

ECode CContextImpl::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    Int32 res;
    CheckCallingOrSelfPermission(permission, &res);
    return Enforce(permission,
            res,
            TRUE,
            Binder::GetCallingUid(),
            message);
}

ECode CContextImpl::GrantUriPermission(
    /* [in] */ const String& toPackage,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
//     try {
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    return ActivityManagerNative::GetDefault()->GrantUriPermission(
            appThread, toPackage, uri,
            modeFlags);
//    } catch (RemoteException e) {
//    }
}

ECode CContextImpl::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
//     try {
    AutoPtr<IApplicationThread> appThread;
    mMainThread->GetApplicationThread((IApplicationThread**)&appThread);
    return ActivityManagerNative::GetDefault()->RevokeUriPermission(
            appThread, uri,
            modeFlags);
//    } catch (RemoteException e) {
//    }
}

ECode CContextImpl::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
//    try {
    return ActivityManagerNative::GetDefault()->CheckUriPermission(
            uri, pid, uid, modeFlags, result);
//    } catch (RemoteException e) {
//        return PackageManager.PERMISSION_DENIED;
//    }
}

ECode CContextImpl::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    Int32 pid = Binder::GetCallingPid();
    if (pid != Process::MyPid()) {
        return CheckUriPermission(uri, pid,
                Binder::GetCallingUid(), modeFlags, result);
    }
    *result = IPackageManager::PERMISSION_DENIED;
    return NOERROR;
}

ECode CContextImpl::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return CheckUriPermission(uri, Binder::GetCallingPid(),
            Binder::GetCallingUid(), modeFlags, result);
}

ECode CContextImpl::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    if (DEBUG) {
        Slogger::I("foo", "checkUriPermission: uri=%preadPermission=%s writePermission=%s pid=%d uid=%d mode%d"
            , uri, readPermission.string(), writePermission.string()
            , pid, uid, modeFlags);
    }
    Int32 checked;
    CheckPermission(readPermission, pid, uid, &checked);
    if ((modeFlags&IIntent::FLAG_GRANT_READ_URI_PERMISSION) != 0) {
        if (readPermission == NULL
                || checked
                == IPackageManager::PERMISSION_GRANTED) {
            *result = IPackageManager::PERMISSION_GRANTED;
            return NOERROR;
        }
    }
    Int32 checked2;
    CheckPermission(writePermission, pid, uid, &checked2);
    if ((modeFlags&IIntent::FLAG_GRANT_WRITE_URI_PERMISSION) != 0) {
        if (writePermission == NULL
                || checked2
                == IPackageManager::PERMISSION_GRANTED) {
            *result = IPackageManager::PERMISSION_GRANTED;
            return NOERROR;
        }
    }
    Int32 checkedUri;
    CheckUriPermission(uri, pid, uid, modeFlags, &checkedUri);
    *result = uri != NULL ? checkedUri
            : IPackageManager::PERMISSION_DENIED;
    return NOERROR;
}

ECode CContextImpl::UriModeFlagToString(
    /* [in] */ Int32 uriModeFlags,
    /* [out] */ String* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = String(NULL);

    switch (uriModeFlags) {
        case IIntent::FLAG_GRANT_READ_URI_PERMISSION |
                IIntent::FLAG_GRANT_WRITE_URI_PERMISSION:
            *mode = String("read and write");
            return NOERROR;
        case IIntent::FLAG_GRANT_READ_URI_PERMISSION:
            *mode = String("read");
            return NOERROR;
        case IIntent::FLAG_GRANT_WRITE_URI_PERMISSION:
            *mode = String("write");
            return NOERROR;
    }
//     throw new IllegalArgumentException(
//             "Unknown permission mode flags: " + uriModeFlags);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CContextImpl::EnforceForUri(
    /* [in] */ Int32 modeFlags,
    /* [in] */ Int32 resultOfCheck,
    /* [in] */ Boolean selfToo,
    /* [in] */ Int32 uid,
    /* [in] */ IUri* uri,
    /* [in] */ const String& message)
{
    if (resultOfCheck != IPackageManager::PERMISSION_GRANTED) {
        StringBuilder sb("");
        if (message != NULL) sb += (message + ": ");
        if (selfToo) {
            sb += "Neither user ";
            sb += uid;
            sb += " nor current process has ";
        }
        else {
            sb += "User ";
            sb += uid;
            sb += " does not have ";
        }
        String mode;
        UriModeFlagToString(modeFlags, &mode);
        sb += mode;
        sb += " permission on ";
        sb += uri;
        sb += ".";
        Slogger::E(TAG, sb.ToString());
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}

ECode CContextImpl::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    Int32 checked;
    CheckUriPermission(uri, pid, uid, modeFlags, &checked);
    return EnforceForUri(
            modeFlags, checked,
            FALSE, uid, uri, message);
}

ECode CContextImpl::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    Int32 checked;
    CheckCallingUriPermission(uri, modeFlags, &checked);
    return EnforceForUri(
            modeFlags, checked,
            FALSE,
            Binder::GetCallingUid(), uri, message);
}

ECode CContextImpl::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    Int32 checked;
    CheckCallingOrSelfUriPermission(uri, modeFlags, &checked);
    return EnforceForUri(
            modeFlags,
            checked, TRUE,
            Binder::GetCallingUid(), uri, message);
}

ECode CContextImpl::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    Int32 checked;
    CheckUriPermission(
            uri, readPermission, writePermission, pid, uid,
            modeFlags, &checked);
    return EnforceForUri(modeFlags, checked, FALSE, uid, uri, message);
}

void CContextImpl::WarnIfCallingFromSystemProcess()
{
    if (Process::MyUid() == IProcess::SYSTEM_UID) {
        Slogger::W(TAG, "Calling a method in the system process without a qualified user: "
                /*+ Debug.getCallers(5)*/);
    }
}

ECode CContextImpl::CreatePackageContext(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    AutoPtr<IUserHandle> userHandle;
    Process::MyUserHandle((IUserHandle**)&userHandle);
    return CreatePackageContextAsUser(packageName, flags,
            mUser != NULL ? mUser : userHandle, ctx);
}

ECode CContextImpl::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    *ctx = NULL;

    if (packageName.Equals("system") || packageName.Equals("android")) {
        AutoPtr<CContextImpl> sysContext;
        mMainThread->GetSystemContext((IContextImpl**)&sysContext);
        AutoPtr<CContextImpl> context;
        CContextImpl::New(sysContext, (IContextImpl**)&context);
        context->mRestricted = (flags & CONTEXT_RESTRICTED) == CONTEXT_RESTRICTED;
        context->Init(mPackageInfo, NULL, mMainThread, mResources, mBasePackageName, user);
        *ctx = context;
        REFCOUNT_ADD(*ctx);
        return NOERROR;
    }

    Int32 identifier = 0;
    user->GetIdentifier(&identifier);
    AutoPtr<ICompatibilityInfo> comInfo;
    mResources->GetCompatibilityInfo((ICompatibilityInfo**)&comInfo);
    AutoPtr<LoadedPkg> pi =
        mMainThread->GetPackageInfo(packageName, (CCompatibilityInfo*)comInfo.Get(), flags,
                identifier);
    if (pi != NULL) {
        AutoPtr<CContextImpl> c;
        CContextImpl::New((IContextImpl**)&c);
        c->mRestricted = (flags & CONTEXT_RESTRICTED) == CONTEXT_RESTRICTED;
        c->Init(pi, NULL, mMainThread, mResources, mBasePackageName, user);
        if (c->mResources != NULL) {
            *ctx = c;
            REFCOUNT_ADD(*ctx);
            return NOERROR;
        }
    }

    // Should be a better exception.
//    throw new PackageManager.NameNotFoundException(
//        "Application package " + packageName + " not found");
    return E_NAME_NOT_FOUND_EXCEPTION;

}

ECode CContextImpl::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    *ctx = NULL;

    if (overrideConfiguration == NULL) {
//        throw new IllegalArgumentException("overrideConfiguration must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<CContextImpl> c;
    CContextImpl::New((IContextImpl**)&c);
    c->Init(mPackageInfo, NULL, mMainThread);
    String resDir;
    mPackageInfo->GetResDir(&resDir);
    AutoPtr<ICompatibilityInfo> comInfo;
    mResources->GetCompatibilityInfo((ICompatibilityInfo**)&comInfo);
    c->mResources = NULL;
    mMainThread->GetTopLevelResources(
            resDir, GetDisplayId(), overrideConfiguration,
            (CCompatibilityInfo*)comInfo.Get(), (IResources**)&c->mResources);
    *ctx = c;
    REFCOUNT_ADD(*ctx);
    return NOERROR;

}

ECode CContextImpl::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    *ctx = NULL;

    if (display == NULL) {
//         throw new IllegalArgumentException("display must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 displayId;
    display->GetDisplayId(&displayId);
    AutoPtr<ICompatibilityInfo> ci = CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO;
    AutoPtr<ICompatibilityInfoHolder> cih;
    GetCompatibilityInfo(displayId, (ICompatibilityInfoHolder**)&cih);
    if (cih != NULL) {
        ci = NULL;
        cih->Get((ICompatibilityInfo**)&ci);
    }

    AutoPtr<CContextImpl> context;
    CContextImpl::New((IContextImpl**)&context);
    context->Init(mPackageInfo, NULL, mMainThread);
    context->mDisplay = display;
    String resDir;
    mPackageInfo->GetResDir(&resDir);
    context->mResources = NULL;
    mMainThread->GetTopLevelResources(
            resDir, displayId, NULL, (CCompatibilityInfo*)ci.Get()
            , (IResources**)&context->mResources);
    *ctx = context;
    REFCOUNT_ADD(*ctx);
    return NOERROR;

}

Int32 CContextImpl::GetDisplayId()
{
    Int32 displayId;
    mDisplay->GetDisplayId(&displayId);
    return mDisplay != NULL ? displayId : IDisplay::DEFAULT_DISPLAY;
}

ECode CContextImpl::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    VALIDATE_NOT_NULL(isRestricted);

    *isRestricted = mRestricted;
    return NOERROR;
}

ECode CContextImpl::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    VALIDATE_NOT_NULL(infoHolder);
    *infoHolder = NULL;

    if (displayId == IDisplay::DEFAULT_DISPLAY) {
        *infoHolder = mPackageInfo->mCompatibilityInfo;
        REFCOUNT_ADD(*infoHolder);
    }

    return NOERROR;
}

AutoPtr<CContextImpl> CContextImpl::CreateSystemContext(
    /* [in] */ IActivityThread* mainThread)
{
    AutoPtr<CContextImpl> context;
    ASSERT_SUCCEEDED(CContextImpl::NewByFriend((CContextImpl**)&context));
    AutoPtr<IUserHandle> user;
    Process::MyUserHandle((IUserHandle**)&user);
    context->Init(CResources::GetSystem(), (CActivityThread*)mainThread, user);
    return context;
}

ECode CContextImpl::Init(
    /* [in] */ LoadedPkg* packageInfo,
    /* [in] */ IBinder* activityToken,
    /* [in] */ CActivityThread* mainThread)
 {
    AutoPtr<IUserHandle> userHandle;
    Process::MyUserHandle((IUserHandle**)&userHandle);
    return Init(packageInfo, activityToken, mainThread, NULL, String(NULL), userHandle);
}

ECode CContextImpl::Init(
    /* [in] */ LoadedPkg* packageInfo,
    /* [in] */ IBinder* activityToken,
    /* [in] */ CActivityThread* mainThread,
    /* [in] */ IResources* container,
    /* [in] */ const String& basePackageName,
    /* [in] */ IUserHandle* user)
{
    mPackageInfo = packageInfo;
    mBasePackageName = !basePackageName.IsNull() ? basePackageName : packageInfo->mPackageName;
    mPackageInfo->GetResources(mainThread, (IResources**)&mResources);

    if (mResources != NULL && container != NULL) {
        AutoPtr<ICompatibilityInfo> ci,rci;
        Float as, ras;
        container->GetCompatibilityInfo((ICompatibilityInfo**)&ci);
        ci->GetApplicationScale(&as);
        mResources->GetCompatibilityInfo((ICompatibilityInfo**)&rci);
        rci->GetApplicationScale(&ras);
        if (as != ras) {
            if (DEBUG) {
                AutoPtr<IDisplayMetrics> dm;
                container->GetDisplayMetrics((IDisplayMetrics**)&dm);
                String displayMetrics;
                IObject::Probe(dm.Get())->ToString(&displayMetrics);
                Slogger::D(TAG, "loaded context has different scaling. Using container's"
                    " compatiblity info: %s", displayMetrics.string());
            }
            String dir;
            mPackageInfo->GetResDir(&dir);
            mResources = NULL;
            mainThread->GetTopLevelResources(dir, IDisplay::DEFAULT_DISPLAY,
                NULL, (CCompatibilityInfo*)ci.Get(), (IResources**)&mResources);
        }
    }

    mMainThread = mainThread;
    mActivityToken = activityToken;
    mContentResolver = new ApplicationContentResolver(this, mainThread, user);
    mUser = user;
    return NOERROR;
}

ECode CContextImpl::Init(
    /* [in] */ IResources* resources,
    /* [in] */ CActivityThread* mainThread,
    /* [in] */ IUserHandle* user)
{
    mPackageInfo = NULL;
    mBasePackageName = NULL;
    mResources = resources;
    mMainThread = mainThread;
    mContentResolver = new ApplicationContentResolver(this, mainThread, user);
    mUser = user;
    return NOERROR;
}

ECode CContextImpl::ScheduleFinalCleanup(
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    return mMainThread->ScheduleContextCleanup(this, who, what);
}

ECode CContextImpl::PerformFinalCleanup(
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    //Log.i(TAG, "Cleanup up context: " + this);
    AutoPtr<IContext> ctx = GetOuterContext();
    return mPackageInfo->RemoveContextRegistrations(ctx, who, what);
}

ECode CContextImpl::GetReceiverRestrictedContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    *ctx = NULL;

    if (mReceiverRestrictedContext != NULL) {
        *ctx = mReceiverRestrictedContext;
        REFCOUNT_ADD(*ctx);
        return NOERROR;
    }
    AutoPtr<IContext> outCtx = GetOuterContext();
    CReceiverRestrictedContext::New(outCtx, (IReceiverRestrictedContext**)&mReceiverRestrictedContext);
    *ctx = mReceiverRestrictedContext;
    REFCOUNT_ADD(*ctx);
    return NOERROR;
 }

void CContextImpl::SetOuterContext(
    /* [in] */ IContext* context)
{
    mOuterContext = NULL;
    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)context->Probe(EIID_IWeakReferenceSource);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mOuterContext);
}

AutoPtr<IContext> CContextImpl::GetOuterContext()
{
    AutoPtr<IContext> context;
    if (mOuterContext != NULL) {
        mOuterContext->Resolve(EIID_IContext, (IInterface**)&context);
    }
    return context;
}

AutoPtr<IBinder> CContextImpl::GetActivityToken()
{
    return mActivityToken;
}

void CContextImpl::SetFilePermissionsFromMode(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 extraPermissions)
{
    Int32 perms = FileUtils::sS_IRUSR|FileUtils::sS_IWUSR
        |FileUtils::sS_IRGRP|FileUtils::sS_IWGRP
        |extraPermissions;
    if ((mode&IContext::MODE_WORLD_READABLE) != 0) {
        perms |= FileUtils::sS_IROTH;
    }
    if ((mode&IContext::MODE_WORLD_WRITEABLE) != 0) {
        perms |= FileUtils::sS_IWOTH;
    }
    if (DEBUG) {
        Slogger::I(TAG, "File %s: mode=0x%x, perms=0x%x", name.string(), mode, perms);
    }
    FileUtils::SetPermissions(name, perms, -1, -1);
}

AutoPtr<IFile> CContextImpl::ValidateFilePath(
    /* [in] */ const String& name,
    /* [in] */ Boolean createDirectory)
{
    AutoPtr<IFile> dir;
    AutoPtr<IFile> f;
    Char32 separatorChar;
    AutoPtr<IFileHelper> helper;
    CFileHelper::AcquireSingleton((IFileHelper**)&helper);
    helper->GetSeparatorChar(&separatorChar);

    if (name.GetChar(0) == separatorChar) {
        String dirPath = name.Substring(0, name.LastIndexOf(separatorChar));
        CFile::New(dirPath, (IFile**)&dir);
        String _name = name.Substring(name.LastIndexOf(separatorChar));
        CFile::New(dir, _name, (IFile**)&f);
    } else {
        dir = GetDatabasesDir();
        MakeFilename(dir, name, (IFile**)&f);
    }

    Boolean isDir, isMk;
    if (createDirectory && (dir->IsDirectory(&isDir), !isDir) && (dir->Mkdir(&isMk), isMk)) {
        String path;
        dir->GetPath(&path);
        FileUtils::SetPermissions(path,
            FileUtils::sS_IRWXU|FileUtils::sS_IRWXG|FileUtils::sS_IXOTH,
            -1, -1);
    }

    return f;
}

ECode CContextImpl::MakeFilename(
    /* [in] */ IFile* base,
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);
    *file = NULL;

    Char32 separatorChar;
    AutoPtr<IFileHelper> helper;
    CFileHelper::AcquireSingleton((IFileHelper**)&helper);
    helper->GetSeparatorChar(&separatorChar);
    if (name.IndexOf(separatorChar) < 0) {
        return CFile::New(base, name, file);
    }
    //throw new IllegalArgumentException(
    //        "File " + name + " contains a path separator");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
