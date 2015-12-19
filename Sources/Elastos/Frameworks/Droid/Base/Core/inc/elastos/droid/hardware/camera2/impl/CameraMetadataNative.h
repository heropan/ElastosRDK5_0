
#ifndef __ELASTOS_DROID_HARDWARE_CAMERA2_IMPL_CAMERAMETADATANATIVE_H__
#define __ELASTOS_DROID_HARDWARE_CAMERA2_IMPL_CAMERAMETADATANATIVE_H__

#include "Elastos.Droid.Hardware.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Hardware::Camera2::Utils::ITypeReference;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Impl {

class CameraMetadataNative
    : public Object
    , public ICameraMetadataNative
    , public IParcelable
{
public:
    class Key
        : public Object
        , public ICameraMetadataNativeKey
    {
    public:
        CAR_INTERFACE_DECL()

        /**
         * Visible for testing only.
         *
         * <p>Use the CameraCharacteristics.Key, CaptureResult.Key, or CaptureRequest.Key
         * for application code or vendor-extended keys.</p>
         */
        Key(
            /* [in] */ const String& name,
            /* [in] */ ClassID type);


        /**
         * Visible for testing only.
         *
         * <p>Use the CameraCharacteristics.Key, CaptureResult.Key, or CaptureRequest.Key
         * for application code or vendor-extended keys.</p>
         */
        //@SuppressWarnings("unchecked")
        Key(
            /* [in] */ const String& name,
            /* [in] */ ITypeReference* typeReference);

        /**
         * Return a camelCase, period separated name formatted like:
         * {@code "root.section[.subsections].name"}.
         *
         * <p>Built-in keys exposed by the Android SDK are always prefixed with {@code "android."};
         * keys that are device/platform-specific are prefixed with {@code "com."}.</p>
         *
         * <p>For example, {@code CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP} would
         * have a name of {@code "android.scaler.streamConfigurationMap"}; whereas a device
         * specific key might look like {@code "com.google.nexus.data.private"}.</p>
         *
         * @return String representation of the key name
         */
        CARAPI GetName(
            /* [out] */ String* name);

        /**
         * {@inheritDoc}
         */
        //@Override
        CARAPI GetHashCode(
            /* [out] */ Int32* code);

       /**
         * Compare this key against other native keys, request keys, result keys, and
         * characteristics keys.
         *
         * <p>Two keys are considered equal if their name and type reference are equal.</p>
         *
         * <p>Note that the equality against non-native keys is one-way. A native key may be equal
         * to a result key; but that same result key will not be equal to a native key.</p>
         */
        //@SuppressWarnings("rawtypes")
        //@Override
        CARAPI Equals(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* e);

        /**
         * <p>
         * Get the tag corresponding to this key. This enables insertion into the
         * native metadata.
         * </p>
         *
         * <p>This value is looked up the first time, and cached subsequently.</p>
         *
         * @return The tag numeric value corresponding to the string
         */
        CARAPI GetTag(
            /* [out] */ Int32* tag);

        /**
         * Get the raw class backing the type {@code T} for this key.
         *
         * <p>The distinction is only important if {@code T} is a generic, e.g.
         * {@code Range<Integer>} since the nested type will be erased.</p>
         */
        CARAPI GetType(
            /* [out] */ ClassID* type);

        /**
         * Get the type reference backing the type {@code T} for this key.
         *
         * <p>The distinction is only important if {@code T} is a generic, e.g.
         * {@code Range<Integer>} since the nested type will be retained.</p>
         */
        CARAPI GetTypeReference(
            /* [out] */ ITypeReference** ref);

    private:
        Boolean mHasTag;
        Int32 mTag;
        ClassID mType;
        AutoPtr<ITypeReference> mTypeReference;
        String mName;
        Int32 mHash;
    };

public:
    CAR_INTERFACE_DECL();

    CameraMetadataNative();

    virtual ~CameraMetadataNative() {}

    CARAPI constructor();

    /**
     * Copy constructor - clone metadata
     */
    CARAPI constructor(
        /* [in] */ ICameraMetadataNative* requestId);

    /**
     * Set a camera metadata field to a value. The field definitions can be
     * found in {@link CameraCharacteristics}, {@link CaptureResult}, and
     * {@link CaptureRequest}.
     *
     * @param key The metadata field to write.
     * @param value The value to set the field to, which must be of a matching
     * type to the key.
     */
    CARAPI Set(
        /* [in] */ ICameraMetadataNativeKey* key,
        /* [in] */ IInterface* value);

    CARAPI Set(
        /* [in] */ ICaptureRequestKey* key,
        /* [in] */ IInterface* value);

    CARAPI Set(
        /* [in] */ ICaptureResultKey* key,
        /* [in] */ IInterface* value);

    CARAPI Set(
        /* [in] */ ICameraCharacteristicsKey* key,
        /* [in] */ IInterface* value);

    /**
     * <p>Perform a 0-copy swap of the internal metadata with another object.</p>
     *
     * <p>Useful to convert a CameraMetadata into e.g. a CaptureRequest.</p>
     *
     * @param other Metadata to swap with
     * @throws NullPointerException if other was null
     * @hide
     */
    CARAPI Swap(
        /* [in] */ ICameraMetadataNative* other);

    /**
     * Dumps the native metadata contents to logcat.
     *
     * <p>Visibility for testing/debugging only. The results will not
     * include any synthesized keys, as they are invisible to the native layer.</p>
     *
     * @hide
     */
    CARAPI DumpToLog();

    /**
     * @hide
     */
    CARAPI Get(
        /* [in] */ ICameraCharacteristicsKey* key,
        /* [out] */ IInterface** result);

    /**
     * @hide
     */
    CARAPI Get(
        /* [in] */ ICaptureResultKey* key,
        /* [out] */ IInterface** result);

    /**
     * @hide
     */
    CARAPI Get(
        /* [in] */ ICaptureRequestKey* key,
        /* [out] */ IInterface** result);

    /**
     * Look-up a metadata field value by its key.
     *
     * @param key a non-{@code null} key instance
     * @return the field corresponding to the {@code key}, or {@code null} if no value was set
     */
    CARAPI Get(
        /* [in] */ ICameraMetadataNativeKey* key,
        /* [out] */ IInterface** result);

    /**
     * Does this metadata contain at least 1 entry?
     *
     * @hide
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Convert a key string into the equivalent native tag.
     *
     * @throws IllegalArgumentException if the key was not recognized
     * @throws NullPointerException if the key was null
     *
     * @hide
     */
    static CARAPI GetTag(
        /* [in] */ const String& key,
        /* [out] */ Int32* tag);

    /**
     * Move the contents from {@code other} into a new camera metadata instance.</p>
     *
     * <p>After this call, {@code other} will become empty.</p>
     *
     * @param other the previous metadata instance which will get pilfered
     * @return a new metadata instance with the values from {@code other} moved into it
     */
    static CARAPI Move(
        /* [in] */ ICameraMetadataNative* other,
        /* [out] */ ICameraMetadataNative** result);

private:
    static const String TAG;
    static const Boolean VERBOSE;

    static const String CELLID_PROCESS;
    static const String GPS_PROCESS;
    static const Int32 FACE_LANDMARK_SIZE;
};

} // namespace Impl
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_HARDWARE_CAMERA2_IMPL_CAMERAMETADATANATIVE_H__
