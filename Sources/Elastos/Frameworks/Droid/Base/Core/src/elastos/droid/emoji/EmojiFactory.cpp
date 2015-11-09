
#include "elastos/droid/emoji/EmojiFactory.h"

#include <dlfcn.h>
#include "EmojiFactory.h"
#include <utils/Log.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include "SkTypes.h"
#include "SkImageDecoder.h"

namespace Elastos {
namespace Droid {
namespace Emoji {

CAR_INTERFACE_IMPL(EmojiFactory, Object, IEmojiFactory);

//For navtive used class
class EmojiFactoryCaller
{
public:
    EmojiFactoryCaller() {}
    virtual ~EmojiFactoryCaller();

    Boolean Init();

    EmojiFactory * TryCallGetImplementation(const char* name);
    EmojiFactory * TryCallGetAvailableImplementation();

private:
    EmojiFactory * (*m_get_implementation)(const char*);
    EmojiFactory * (*m_get_available_implementation)();
};

//EmojiFactoryCaller function
Boolean EmojiFactoryCaller::Init()
{
    const char* error_msg = NULL;
    m_handle = dlopen("libemoji.so", RTLD_LAZY | RTLD_LOCAL);

    if (m_handle == NULL) {
        error_msg = "Failed to load libemoji.so";
        goto FAIL;
    }

    m_get_implementation =
        reinterpret_cast<EmojiFactory *(*)(const char*)>(dlsym(m_handle, "GetImplementation"));
    if (m_get_implementation == NULL) {
        error_msg = "Failed to get symbol of GetImplementation";
        goto FAIL;
    }

    m_get_available_implementation =
        reinterpret_cast<EmojiFactory *(*)()>(dlsym(m_handle,"GetAvailableImplementation"));
    if (m_get_available_implementation == NULL) {
        error_msg = "Failed to get symbol of GetAvailableImplementation";
        goto FAIL;
    }

    return true;

FAIL:
    const char* error_str = dlerror();
    if (error_str == NULL) {
        error_str = "unknown reason";
    }

    ALOGE("%s: %s", error_msg, error_str);
    if (m_handle != NULL) {
        dlclose(m_handle);
        m_handle = NULL;
    }
    return false;
}

EmojiFactoryCaller::~EmojiFactoryCaller()
{
    if (m_handle) {
        dlclose(m_handle);
    }
}

EmojiFactory * EmojiFactoryCaller::TryCallGetImplementation(const char* name)
{
    if (NULL == m_handle) {
        return NULL;
    }
    return m_get_implementation(name);
}

EmojiFactory * EmojiFactoryCaller::TryCallGetAvailableImplementation()
{
    if (NULL == m_handle) {
        return NULL;
    }
    return m_get_available_implementation();
}

static EmojiFactoryCaller* gCaller;
static pthread_once_t g_once = PTHREAD_ONCE_INIT;

static void InitializeCaller() {
    gCaller = new EmojiFactoryCaller();
    lib_emoji_factory_is_ready = gCaller->Init();
}


/*
 * class EmojiFactory
 */

//For navtive used class
EmojiFactory::EmojiFactory()
{}

EmojiFactory::~EmojiFactory()
{}

ECode EmojiFactory::GetBitmapFromAndroidPua(
    /* [in] */ Int32 pua,
    /* [out] */ IBitmap** retBitmap)
{
    VALIDATE_NOT_NULL(retBitmap);

    AutoPtr<Bitmap>* cache = mCache->Get(pua);
    if (cache == null) {
        Bitmap* ret = nativeGetBitmapFromAndroidPua(mNativeEmojiFactory, pua);
        cache = new Bitmap(ret);
        mCache->put(pua, cache);

    }

    *retBitmap = cache;
    REFCOUNT_ADD(*retBitmap);
    return NOERROR;
}

ECode EmojiFactory::GetBitmapFromVendorSpecificSjis(
    /* [in] */char sjis
    /* [out] */ IBitmap** retBitmap)
{
    return GetBitmapFromAndroidPua(getAndroidPuaFromVendorSpecificSjis(sjis), retBitmap);
}

ECode EmojiFactory::GetBitmapFromVendorSpecificPua(
    /* [in] */ Int32 vsp,
    /* [out] */ IBitmap** retBitmap)
{
    return GetBitmapFromAndroidPua(getAndroidPuaFromVendorSpecificPua(vsp), retBitmap);
}

ECode EmojiFactory::GetAndroidPuaFromVendorSpecificSjis(
    /* [in] */ char sjis,
    /* [out] */ Int32* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NativeGetAndroidPuaFromVendorSpecificSjis(mNativeEmojiFactory, sjis);
    return NOERROR;
}

ECode EmojiFactory::GetVendorSpecificSjisFromAndroidPua(
    /* [in] */ Int32 pua,
    /* [out] */ Int32* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NativeGetVendorSpecificSjisFromAndroidPua(mNativeEmojiFactory, pua);
    return NOERROR;
}

ECode EmojiFactory::GetAndroidPuaFromVendorSpecificPua(
    /* [in] */ Int32 vsp,
    /* [out] */ Int32* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NativeGetAndroidPuaFromVendorSpecificPua(mNativeEmojiFactory, vsp);
    return NOERROR;
}

ECode EmojiFactory::GetAndroidPuaFromVendorSpecificPua(
    /* [in] */ const String& vspString,
    /* [out] */ String* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NULL;

    if (vspString.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 minVsp = NativeGetMinimumVendorSpecificPua(mNativeEmojiFactory);
    Int32 maxVsp = NativeGetMaximumVendorSpecificPua(mNativeEmojiFactory);

    AutoPtr<ArrayOf<Int32> > codePoints = ArrayOf<Int32>::Alloc(vspString.GetCharCount(0, len));\
    Int32 new_len = 0;

    for (Int32 i=0; i < len; i++,new_len++)
        Int32 codePoint = (Int32)vspString.GetChar(i);
        if (minVsp <= codePoint && codePoint <= maxVsp) {
            Int32 newCodePoint = GetAndroidPuaFromVendorSpecificPua(codePoint);
            if (newCodePoint > 0) {
                    (*codePoints)[new_len] = newCodePoint;
                    continue;
            }
        }
        (*codePoints)[new_len] = codePoint;
    }

    *retValue = String(codePoints, 0, new_len);
    return NOERROR;
}

Int32 EmojiFactory::GetVendorSpecificPuaFromAndroidPua(
    /* [in] */ Int32 pua
    /* [out] */ Int32* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NativeGetVendorSpecificPuaFromAndroidPua(mNativeEmojiFactory, pua);
    return NOERROR;
}

String EmojiFactory::GetVendorSpecificPuaFromAndroidPua(
    /* [in] */ const String& puaString,
    /* [out] */ String* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NULL;

    if (puaString == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 minVsp = nativeGetMinimumAndroidPua(mNativeEmojiFactory);
    Int32 maxVsp = nativeGetMaximumAndroidPua(mNativeEmojiFactory);
    Int32 len = puaString.GetLength();
    AutoPtr<ArrayOf<Int32> > codePoints = ArrayOf<Int32>::Alloc(puaString.GetCharCount(0, len));

    Int32 new_len = 0;


    for (Int32 i=0; i < len; i++,new_len++)
    {
        Int32 codePoint = (Int32)puaString.GetChar(i);
        if (minVsp <= codePoint && codePoint <= maxVsp) {
        {
            Int32 newCodePoint = getVendorSpecificPuaFromAndroidPua(codePoint);
            if (newCodePoint > 0) {
                (*codePoints)[new_len] = newCodePoint;
                continue;
            }
        }
        (*codePoints)[new_len] = codePoint;

    }
    *retValue= String(codePoints, 0, new_len);
    return NOERROR;
}

Ecode EmojiFactory::NewInstance(
    /* [in] */ const String& class_name
    /* [out] */ IEmojiFactory** emojiFactory)
{
    VALIDATE_NOT_NULL(emojiFactory);
    *emojiFactory = NULL;

    if (NULL == name) {
        return E_NULL_POINTER_EXCEPTION;
    }

    pthread_once(&g_once, InitializeCaller);
    if (!lib_emoji_factory_is_ready) {
        return E_NULL_POINTER_EXCEPTION;
    }

    EemojiFactory *factory = gCaller->TryCallGetImplementation(name.string());
    if (NULL == factory) {
       return E_NULL_POINTER_EXCEPTION;
    }

    *emojiFactory = new EmojiFactory(factory,name);
    REFCOUNT_ADD(*emojiFactory);
    return NOERROR;
}

Ecode EmojiFactory::NewAvailableInstance(
    /* [out] */ IEmojiFactory** emojiFactory)
{
    VALIDATE_NOT_NULL(retValue);
    *emojiFactory=NULL;

    pthread_once(&g_once, InitializeCaller);
    if (!lib_emoji_factory_is_ready) {
        return E_NULL_POINTER_EXCEPTION;
    }

    emojiFactory *factory = gCaller->TryCallGetAvailableImplementation();
    if (NULL == factory) {
        return E_NULL_POINTER_EXCEPTION;
    }

    String tempname = factory->Name();
    if (NULL == tempname) {
      return E_NULL_POINTER_EXCEPTION;
    }

    *emojiFactory= new EmojiFactory(factory,tempname);
    REFCOUNT_ADD(*emojiFactory);
    return NOERROR;
}

Ecode EmojiFactory::GetMinimumAndroidPua(
    /* [out] */ Int32* retValue )
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NativeGetMinimumAndroidPua(mNativeEmojiFactory);
    return NOERROR;
}

Ecode EmojiFactory::GetMaximumAndroidPua(
    /* [out] */ Int32* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = NativeGetMaximumAndroidPua(mNativeEmojiFactory);
    return NOERROR;
}

Ecode EmojiFactory::GetName(
    /* [out] */ String* retValue)
{
    VALIDATE_NOT_NULL(retValue);
    *retValue = mName;
    return NOERROR;
}


ECode  EmojiFactory::constructor(
    /* [in] */ Int64 nativeEmojiFactory,
    /* [in] */ const String& name)
{
    mNativeEmojiFactory = nativeEmojiFactory;
    mName = name;
    mCache = new CustomLinkedHashMap<Integer, WeakReference<Bitmap>>();
}


vold EmojiFactory::CustomLinkedHashMap<K, V>:CustomLinkedHashMap()
{
    this.HashMap(16, 0.75f);
}

Boolean EmojiFactory::CustomLinkedHashMap<K, V>:removeEldestEntry(Map.Entry<K, V> eldest)
{
    return Map.GetSize() > sCacheSize;
}


static Int32 EmojiFactory::NativeGetMinimumAndroidPua(
    /* [in] */ Int64 nativeEmojiFactory)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetMinimumAndroidPua();
}

static Int32 EmojiFactory::NativeGetMaximumAndroidPua(
   /* [in] */ Int64 nativeEmojiFactory)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetMaximumAndroidPua();
}


static void EmojiFactory::NativeDestructor(
    /* [in] */ Int64 nativeEmojiFactory)
{
    //Don't implement
    return;
}

Bitmap EmojiFactory::NativeGetBitmapFromAndroidPua(
    /* [in] */ Int64 nativeEmojiFactory,
    /* [in] */ Int32 AndroidPua)
{
     EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);

     int size;
     const char *bytes = factory->GetImageBinaryFromAndroidPua(pua, &size);
     if (bytes == NULL) {
        return NULL;
     }

     SkBitmap *bitmap = new SkBitmap;
     if (!SkImageDecoder::DecodeMemory(bytes, size, bitmap)) {
        ALOGE("SkImageDecoder::DecodeMemory() failed.");
        return NULL;
     }

     Bitmap obj= new IBitmap(NULL, false, NULL, -1);
     if (obj == NULL) {
        return NULL;
     }

     return obj;
}

Int32 EmojiFactory::NativeGetAndroidPuaFromVendorSpecificSjis(
    /* [in] */ Int64 nativeEmojiFactory,
    /* [in] */ char sjis)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetAndroidPuaFromVendorSpecificSjis(sjis);
}

Int32 EmojiFactory::NativeGetVendorSpecificSjisFromAndroidPua(
    /* [in] */ Int64 nativeEmojiFactory,
    /* [in] */ Int32 pua)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetVendorSpecificSjisFromAndroidPua(pua);
}

Int32 EmojiFactory::NativeGetAndroidPuaFromVendorSpecificPua(
    /* [in] */ Int64 nativeEmojiFactory,
    /* [in] */ Int32 vsp)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetAndroidPuaFromVendorSpecificPua(vsu);
}

Int32 EmojiFactory::NativeGetVendorSpecificPuaFromAndroidPua(
    /* [in] */ Int64 nativeEmojiFactory,
    /* [in] */ Int32 pua)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetVendorSpecificPuaFromAndroidPua(pua);
}

Int32 EmojiFactory::NativeGetMaximumVendorSpecificPua(
    /* [in] */ Int64 nativeEmojiFactory)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetMaximumVendorSpecificPua();
}

Int32 EmojiFactory::NativeGetMinimumVendorSpecificPua(
    /* [in] */ Int64 nativeEmojiFactory)
{
    EmojiFactory *factory = reinterpret_cast<EmojiFactory *>(nativeEmojiFactory);
    return factory->GetMinimumVendorSpecificPua();
}

} // namespace Emoji
} // namepsace Droid
} // namespace Elastos
