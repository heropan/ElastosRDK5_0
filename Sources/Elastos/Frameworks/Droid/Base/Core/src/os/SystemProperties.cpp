
#include "os/SystemProperties.h"
#include "os/ServiceManager.h"
#include "os/Process.h"
#include "graphics/CBitmapFactory.h"
#include "privacy/CPrivacySettingsManager.h"
#include <cutils/properties.h>
#include "utils/misc.h"
#include <stdio.h>

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Core::IRunnable;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::BitmapCompressFormat_JPEG;
using Elastos::Droid::Privacy::CPrivacySettingsManager;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Droid::Privacy::IPrivacySettings;

namespace Elastos {
namespace Droid {
namespace Os {


const Int32 SystemProperties::PROP_NAME_MAX;
const Int32 SystemProperties::PROP_VALUE_MAX;

List< AutoPtr<IRunnable> > SystemProperties::sChangeCallbacks;
Mutex SystemProperties::sChangeCallbacksLock;

const Int32 SystemProperties::IS_ALLOWED = -1;
const Int32 SystemProperties::IS_NOT_ALLOWED = -2;
const Int32 SystemProperties::GOT_ERROR = -3;

const String SystemProperties::PRIVACY_TAG = String("PM,Camera");

AutoPtr<IContext> SystemProperties::mContext;
AutoPtr<IPrivacySettingsManager> SystemProperties::mPrivacySettingsManager;
Boolean SystemProperties::mPrivacyMode = FALSE;
AutoPtr<IIPackageManager> SystemProperties::mPackageManager;


ECode SystemProperties::Get(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    if (key.IsNull() || key.GetLength() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *value = NativeGet(key);
    return NOERROR;
}

ECode SystemProperties::Get(
    /* [in] */ const String& key,
    /* [in] */ const String& def,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    if (key.IsNull() || key.GetLength() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *value = NativeGet(key, def);
    return NOERROR;
}

Int32 SystemProperties::GetInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 def)
{
    if (key.IsNull() || key.GetLength() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGetInt32(key, def);
}

Int64 SystemProperties::GetInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 def)
{
    if (key.IsNull() || key.GetLength() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGetInt64(key, def);
}

Boolean SystemProperties::GetBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean def)
{
    if (key.IsNull() || key.GetLength() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGetBoolean(key, def);
}

void SystemProperties::Set(
    /* [in] */ const String& key,
    /* [in] */ const String& val)
{
    if (key.IsNull() || key.GetLength() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    if (!val.IsNull() && val.GetLength() > PROP_VALUE_MAX) {
//        throw new IllegalArgumentException("val.length > " +
//            PROP_VALUE_MAX);
        assert(0);
    }
    NativeSet(key, val);
}

String SystemProperties::NativeGet(
    /* [in] */ const String& key)
{
    return NativeGet(key, String());
}

String SystemProperties::NativeGet(
    /* [in] */ const String& key,
    /* [in] */ const String& def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];

    len = property_get((const char*)key, buf, "");
    if ((len <= 0) && (!def.IsNull())) {
        return def;
    }
    else if (len >= 0) {
        return String(buf);
    }
    else {
        return String("");
    }
}

Int32 SystemProperties::NativeGetInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];
    Int32 result = def;

    len = property_get(key, buf, "");
    if (len > 0) {
        Int32 temp;
        if (sscanf(buf, "%d", &temp) == 1) {
            result = temp;
        }
    }

    return result;
}

Int64 SystemProperties::NativeGetInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];
    Int64 result = def;

    len = property_get(key, buf, "");
    if (len > 0) {
        Int64 temp;
        if (sscanf(buf, "%lld", &temp) == 1)
            result = temp;
    }

    return result;
}

Boolean SystemProperties::NativeGetBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];
    Boolean result = def;

    len = property_get(key, buf, "");
    if (len == 1) {
        char ch = buf[0];
        if (ch == '0' || ch == 'n') {
            result = FALSE;
        }
        else if (ch == '1' || ch == 'y') {
            result = TRUE;
        }
    }
    else if (len > 1) {
        if (!strcmp(buf, "no") || !strcmp(buf, "false") || !strcmp(buf, "off")) {
            result = FALSE;
        }
        else if (!strcmp(buf, "yes") || !strcmp(buf, "true") || !strcmp(buf, "on")) {
            result = TRUE;
        }
    }

    return result;
}

void SystemProperties::NativeSet(
    /* [in] */ const String& key,
    /* [in] */ const String& val)
{
    Int32 err;
    String value = val;
    if (value.IsNull()) {
        value = String("");       /* NULL pointer not allowed here */
    }

    err = property_set(key, val);
}

void SystemProperties::NativeAddChangeCallback()
{
    // This is called with the Java lock held.
    android::add_sysprop_change_callback(SystemProperties::CallChangeCallbacks, -10000);
}

AutoPtr<ArrayOf<String> > SystemProperties::GetPackageName()
{
    // try{
    if (mPackageManager == NULL) {
        AutoPtr<IInterface> b = ServiceManager::GetService(String("package"));
        assert(b != NULL);
        mPackageManager = IIPackageManager::Probe(b);
    }

    AutoPtr<ArrayOf<String> > packageNames;
    Int32 uid = Process::MyUid();
    mPackageManager->GetPackagesForUid(uid, (ArrayOf<String>**)&packageNames);
    return packageNames;
    // }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG,"something went wrong with getting package name");
    //     return NULL;
    // }
}

AutoPtr<ArrayOf<Byte> > SystemProperties::GetFakeImage()
{
    // try{
    String filepath("/system/media/PDroid.jpeg");
    AutoPtr<IFile> imagefile;
    AutoPtr<IFileInputStream> fis;
    AutoPtr<IBitmap> bm;
    AutoPtr<IByteArrayOutputStream> helper;
    AutoPtr<ArrayOf<Byte> > bytes;
    Boolean bval;
    AutoPtr<IBitmapFactory> fact;

    FAIL_GOTO(CFile::New(filepath, (IFile**)&imagefile), _EXIT_);
    FAIL_GOTO(CFileInputStream::New(imagefile, (IFileInputStream**)&fis), _EXIT_);

    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&fact);
    FAIL_GOTO(fact->DecodeStream(fis, (IBitmap**)&bm), _EXIT_);

    FAIL_GOTO(CByteArrayOutputStream::New((IByteArrayOutputStream**)&helper), _EXIT_);
    FAIL_GOTO(bm->Compress(BitmapCompressFormat_JPEG, 100 , helper, &bval), _EXIT_);
    FAIL_GOTO(helper->ToByteArray((ArrayOf<Byte>**)&bytes), _EXIT_);

_EXIT_:
    if (fis != NULL) {
        fis->Close();
    }
    return bytes;
    // }
    // catch (Exception e){
    //     Log.e(PRIVACY_TAG,"something went wrong with getting the picture!");
    //     e.printStackTrace();
    //     return NULL;
    // }
}

void SystemProperties::Initiate()
{
    // try{
    mContext = NULL;

    AutoPtr<IInterface> b = ServiceManager::GetService(String("package"));
    assert(b != NULL);
    mPackageManager = IIPackageManager::Probe(b);

    b = ServiceManager::GetService(String("privacy"));
    assert(b != NULL);
    AutoPtr<IIPrivacySettingsManager> psm = IIPrivacySettingsManager::Probe(b);

    ECode ec = CPrivacySettingsManager::New(mContext, psm, (IPrivacySettingsManager**)&mPrivacySettingsManager);
    if (FAILED(ec)) {
        mPrivacyMode = FALSE;
    }
    else {
        mPrivacyMode = TRUE;
    }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG, "Something went wrong with initalize variables");
    //     privacyMode = false;
    // }
}

Int32 SystemProperties::CheckIfPackagesAllowed()
{
    // try{
    if (mPrivacySettingsManager == NULL) {
        // Log.e(PRIVACY_TAG,"return GOT_ERROR, because pSetMan is NULL");
        return GOT_ERROR;
    }

    AutoPtr<IPrivacySettings> settings;
    AutoPtr<ArrayOf<String> > packageNames = GetPackageName();
    Int32 uid = Process::MyUid();

    if (packageNames != NULL){
        Byte cs;
        for (Int32 i = 0; i < packageNames->GetLength(); i++) {
            settings = NULL;
            mPrivacySettingsManager->GetSettings((*packageNames)[i], uid, (IPrivacySettings**)&settings);
            //if settings is NULL, we allow application to access to mic
            if (settings != NULL) {
                settings->GetCameraSetting(&cs);
                if (cs != IPrivacySettings::REAL) {
                    return IS_NOT_ALLOWED;
                }
            }
        }
        return IS_ALLOWED;
    }

    // Log.e(PRIVACY_TAG,"return GOT_ERROR, because package_names are NULL");
    return GOT_ERROR;

    // }
    // catch (Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG,"Got exception in checkIfPackagesAllowed");
    //     return GOT_ERROR;
    // }
}

void SystemProperties::DataAccess(
    /* [in] */ Boolean success)
{
    // AutoPtr<ArrayOf<String> > packageNames = GetPackageName();
    // if (success && packageNames != NULL){
    //     for(Int32 i = 0; i < packageNames->GetLength(); i++)
    //         Log.i(PRIVACY_TAG,"Allowed Package: -" + package_names[i] + "- accessing camera.");
    // }
    // else if (packageNames != NULL){
    //     for(Int32 i = 0; i < packageNames->GetLength(); i++)
    //         Log.i(PRIVACY_TAG,"Blocked Package: -" + package_names[i] + "- accessing camera.");
    // }
}

void SystemProperties::AddChangeCallback(
    /* [in] */ IRunnable* callback)
{
    AutoLock lock(sChangeCallbacksLock);
    if (sChangeCallbacks.Begin() == sChangeCallbacks.End()) {
        NativeAddChangeCallback();
    }
    sChangeCallbacks.PushBack(callback);
}

void SystemProperties::CallChangeCallbacks()
{
    AutoLock lock(sChangeCallbacksLock);
    //Log.i("foo", "Calling " + sChangeCallbacks.size() + " change callbacks!");
    if (sChangeCallbacks.Begin() == sChangeCallbacks.End()) {
        return;
    }
    AutoPtr< List< AutoPtr<IRunnable> > > callbacks = new List< AutoPtr<IRunnable> >(sChangeCallbacks);
    List< AutoPtr<IRunnable> >::Iterator it = callbacks->Begin();
    for (; it != callbacks->End(); ++it) {
        (*it)->Run();
    }
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
