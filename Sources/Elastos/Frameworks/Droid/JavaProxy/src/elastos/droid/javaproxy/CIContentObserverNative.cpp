
#include "elastos/droid/javaproxy/CIContentObserverNative.h"
#include "elastos/droid/javaproxy/Util.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Database::EIID_IIContentObserver;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

const String CIContentObserverNative::TAG("CIContentObserverNative");

CAR_INTERFACE_IMPL_2(CIContentObserverNative, Object, IIContentObserver, IBinder)

CAR_OBJECT_IMPL(CIContentObserverNative)

CIContentObserverNative::~CIContentObserverNative()
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);
    env->DeleteGlobalRef(mJInstance);
}

ECode CIContentObserverNative::constructor(
    /* [in] */ Handle32 jVM,
    /* [in] */ Handle32 jInstance)
{
    mJVM = (JavaVM*)jVM;
    mJInstance = (jobject)jInstance;
    return NOERROR;
}

ECode CIContentObserverNative::OnChange(
    /* [in] */ Boolean selfUpdate,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 userId)
{
    // LOGGERD(TAG, "+ CIContentObserverNative::OnChangeEx()");

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject iuri = NULL;
    if (uri != NULL) {
        iuri = Util::ToJavaUri(env, uri);
        if (iuri == NULL) {
            LOGGERD(TAG, "OnChangeEx() iuri can not be NULL!");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    jclass c = env->FindClass("android/database/IContentObserver");
    Util::CheckErrorAndLog(env, TAG, "Fail FindClass: IContentObserver %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "onChange", "(ZLandroid/net/Uri;I)V");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: onChange %d", __LINE__);

    env->CallVoidMethod(mJInstance, m, (jboolean)selfUpdate, iuri, userId);
    Util::CheckErrorAndLog(env, TAG, "CallVoidMethod: onChange %d", __LINE__);

    env->DeleteLocalRef(c);

    if(iuri){
        env->DeleteLocalRef(iuri);
    }

    // LOGGERD(TAG, "- CIContentObserverNative::OnChangeEx()");
    return NOERROR;
}

ECode CIContentObserverNative::ToString(
    /* [out] */ String* str)
{
    // LOGGERD(TAG, "+ CIContentObserverNative::ToString()");
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jclass c = env->FindClass("java/lang/Object");
    Util::CheckErrorAndLog(env, "ToString", "FindClass: Object", __LINE__);

    jmethodID m = env->GetMethodID(c, "toString", "()Ljava/lang/String;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: toString", __LINE__);

    jstring jstr = (jstring)env->CallObjectMethod(mJInstance, m);
    Util::CheckErrorAndLog(env, TAG, "CallVoidMethod: toString", __LINE__);

    *str = Util::GetElString(env, jstr);

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(jstr);

    // LOGGERD(TAG, "- CIContentObserverNative::ToString()");
    return NOERROR;
}

}
}
}

