
#include "elastos/droid/hardware/camera2/impl/CameraMetadataNative.h"
#include "elastos/droid/hardware/camera2/impl/CCameraMetadataNative.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Hardware::Camera2::Impl::CCameraMetadataNative;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Impl {

CAR_INTERFACE_IMPL(CameraMetadataNative::Key, Object, ICaptureRequestKey)

CameraMetadataNative::Key::Key(
    /* [in] */ const String& name,
    /* [in] */ ClassID type)
{
    if (name.IsNull()) {
        //throw new NullPointerException("Key needs a valid name");
        Slogger::E("CameraMetadataNative::Key", "Key needs a valid name");
    }
    // else if (type == null) {
    //     throw new NullPointerException("Type needs to be non-null");
    // }

    mName = name;
    mType = type;
    assert(0);
    //mTypeReference = TypeReference::CreateSpecializedTypeReference(type);
    Int32 code;
    IObject::Probe(mTypeReference)->GetHashCode(&code);
    mHash = mName.GetHashCode() ^ code;
}

CameraMetadataNative::Key::Key(
    /* [in] */ const String& name,
    /* [in] */ ITypeReference* typeReference)
{
    if (name.IsNull()) {
        //throw new NullPointerException("Key needs a valid name");
        Slogger::E("CameraMetadataNative::Key", "Key needs a valid name");;
    }
    else if (typeReference == NULL) {
        //throw new NullPointerException("TypeReference needs to be non-null");
        Slogger::E("CameraMetadataNative::Key", "TypeReference needs to be non-null");
    }
    mName = name;
    ClassID id;
    AutoPtr<IClassInfo> info;
    typeReference->GetRawType((IClassInfo**)&info);
    info->GetId(&id);
    mType = id;
    mTypeReference = typeReference;
    Int32 code;
    IObject::Probe(mTypeReference)->GetHashCode(&code);
    mHash = mName.GetHashCode() ^ code;
}

ECode CameraMetadataNative::Key::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode CameraMetadataNative::Key::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);

    *code = mHash;
    return NOERROR;
}

ECode CameraMetadataNative::Key::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equal)
{
    VALIDATE_NOT_NULL(equal);
    *equal = FALSE;

    if (THIS_PROBE(IInterface) == TO_IINTERFACE(obj)) {
        *equal = TRUE;
        return NOERROR;
    }

    if (obj == NULL) {
        *equal = FALSE;
        return NOERROR;
    }

    Int32 code;
    this->GetHashCode(&code);
    Int32 code2;
    IObject::Probe(obj)->GetHashCode(&code2);
    if (code != code2) {
        *equal = FALSE;
        return NOERROR;
    }

    AutoPtr<ICameraMetadataNativeKey> lhs;
    if (ICaptureResultKey::Probe(obj) != NULL) {
        ICaptureResultKey::Probe(obj)->GetNativeKey((ICameraMetadataNativeKey**)&lhs);
    }
    else if (ICaptureRequestKey::Probe(obj) != NULL) {
        ICaptureRequestKey::Probe(obj)->GetNativeKey((ICameraMetadataNativeKey**)&lhs);
    }
    else if (ICameraCharacteristicsKey::Probe(obj) != NULL) {
        ICameraCharacteristicsKey::Probe(obj)->GetNativeKey((ICameraMetadataNativeKey**)&lhs);
    }
    else if (ICameraMetadataNativeKey::Probe(obj) != NULL) {
        lhs = ICameraMetadataNativeKey::Probe(obj);
    }
    else {
        *equal = FALSE;
        return NOERROR;
    }

    String name;
    lhs->GetName(&name);
    AutoPtr<ITypeReference> ref;
    lhs->GetTypeReference((ITypeReference**)&ref);
    Boolean result2;
    IObject::Probe(mTypeReference)->Equals(ref, &result2);
    *equal = mName.Equals(name) && result2;
    return NOERROR;
}

ECode CameraMetadataNative::Key::GetTag(
    /* [out] */ Int32* tag)
{
    VALIDATE_NOT_NULL(tag);

    if (!mHasTag) {
        CameraMetadataNative::GetTag(mName, &mTag);
        mHasTag = TRUE;
    }
    *tag = mTag;
    return NOERROR;
}

ECode CameraMetadataNative::Key::GetType(
    /* [out] */ ClassID* type)
{
    VALIDATE_NOT_NULL(type);
    // TODO: remove this; other places should use #getTypeReference() instead
    *type = mType;
    return NOERROR;
}

ECode CameraMetadataNative::Key::GetTypeReference(
    /* [out] */ ITypeReference** ref)
{
    VALIDATE_NOT_NULL(ref);

    *ref = mTypeReference;
    REFCOUNT_ADD(*ref);
    return NOERROR;
}

const String CameraMetadataNative::TAG("CameraMetadataJV");
const Boolean CameraMetadataNative::VERBOSE = FALSE; //Log.isLoggable(TAG, Log.VERBOSE);

const String CameraMetadataNative::CELLID_PROCESS("CELLID");
const String CameraMetadataNative::GPS_PROCESS("GPS");
const Int32 CameraMetadataNative::FACE_LANDMARK_SIZE = 6;

CAR_INTERFACE_IMPL_2(CameraMetadataNative, Object, ICameraMetadataNative, IParcelable)

CameraMetadataNative::CameraMetadataNative()
{

}

ECode CameraMetadataNative::constructor()
{
    // //super();
    // mMetadataPtr = nativeAllocate();
    // if (mMetadataPtr == 0) {
    //     throw new OutOfMemoryError("Failed to allocate native CameraMetadata");
    // }
    return NOERROR;
}

ECode CameraMetadataNative::constructor(
    /* [in] */ ICameraMetadataNative* requestId)
{
    // //super();
    // mMetadataPtr = nativeAllocateCopy(other);
    // if (mMetadataPtr == 0) {
    //     throw new OutOfMemoryError("Failed to allocate native CameraMetadata");
    // }
    return NOERROR;
}

ECode CameraMetadataNative::Set(
    /* [in] */ ICameraMetadataNativeKey* key,
    /* [in] */ IInterface* value)
{
    // SetCommand s = sSetCommandMap.get(key);
    // if (s != null) {
    //     s.setValue(this, value);
    //     return;
    // }

    // setBase(key, value);
    return NOERROR;
}

ECode CameraMetadataNative::Set(
    /* [in] */ ICaptureRequestKey* key,
    /* [in] */ IInterface* value)
{
    AutoPtr<ICameraMetadataNativeKey> _key;
    key->GetNativeKey((ICameraMetadataNativeKey**)&_key);
    return Set(_key, value);
}

ECode CameraMetadataNative::Set(
    /* [in] */ ICaptureResultKey* key,
    /* [in] */ IInterface* value)
{
    AutoPtr<ICameraMetadataNativeKey> _key;
    key->GetNativeKey((ICameraMetadataNativeKey**)&_key);
    return Set(_key, value);
}

ECode CameraMetadataNative::Set(
    /* [in] */ ICameraCharacteristicsKey* key,
    /* [in] */ IInterface* value)
{
    AutoPtr<ICameraMetadataNativeKey> _key;
    key->GetNativeKey((ICameraMetadataNativeKey**)&_key);
    return Set(_key, value);
}

ECode CameraMetadataNative::Swap(
    /* [in] */ ICameraMetadataNative* other)
{
    //nativeSwap(other)
    return NOERROR;
}

ECode CameraMetadataNative::DumpToLog()
{
    // try {
    //     nativeDump();
    // } catch (IOException e) {
    //     Log.wtf(TAG, "Dump logging failed", e);
    // }
    return NOERROR;
}

ECode CameraMetadataNative::Get(
    /* [in] */ ICameraCharacteristicsKey* key,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICameraMetadataNativeKey> _key;
    key->GetNativeKey((ICameraMetadataNativeKey**)&_key);
    return Get(_key, result);
}

ECode CameraMetadataNative::Get(
    /* [in] */ ICaptureResultKey* key,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICameraMetadataNativeKey> _key;
    key->GetNativeKey((ICameraMetadataNativeKey**)&_key);
    return Get(_key, result);
}

ECode CameraMetadataNative::Get(
    /* [in] */ ICaptureRequestKey* key,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICameraMetadataNativeKey> _key;
    key->GetNativeKey((ICameraMetadataNativeKey**)&_key);
    return Get(_key, result);
}

ECode CameraMetadataNative::Get(
    /* [in] */ ICameraMetadataNativeKey* key,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    assert(0);
    // Preconditions.checkNotNull(key, "key must not be null");

    // // Check if key has been overridden to use a wrapper class on the java side.
    // GetCommand g = sGetCommandMap.get(key);
    // if (g != null) {
    //     return g.getValue(this, key);
    // }
    // return getBase(key);
    return NOERROR;
}

ECode CameraMetadataNative::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    assert(0);
    //return nativeIsEmpty();
    return NOERROR;
}

ECode CameraMetadataNative::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(0);
    //return nativeWriteToParcel(dest);
    return NOERROR;
}

ECode CameraMetadataNative::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(0);
    return NOERROR;
}

ECode CameraMetadataNative::GetTag(
    /* [in] */ const String& key,
    /* [out] */ Int32* tag)
{
    assert(0);
    //return nativeGetTagFromKey(key);
    return NOERROR;
}

ECode CameraMetadataNative::Move(
    /* [in] */ ICameraMetadataNative* other,
    /* [out] */ ICameraMetadataNative** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICameraMetadataNative> newObject;
    CCameraMetadataNative::New((ICameraMetadataNative**)&newObject);
    newObject->Swap(other);
    *result = newObject;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

} // namespace Impl
} // namespace Camera2
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos