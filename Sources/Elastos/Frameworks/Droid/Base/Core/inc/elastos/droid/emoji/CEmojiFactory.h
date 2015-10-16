
#ifndef __ELASTOS_DROID_EMOJI_EMOJIFACTORY_H__
#define __ELASTOS_DROID_EMOJI_EMOJIFACTORY_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/Map.h>
#include "elastos/droid/graphics/CBitmap.h"


namespace Elastos {
namespace Droid {
namespace Emoji {
/**
 * A class for the factories which produce Emoji (pictgram) images.
 * This is intended to be used by IME, Email app, etc.
 * There's no plan to make this public for now.
 * @hide
 */
CarClass (CEmojiFactory)
{
public:
    /**
     * Returns Bitmap object corresponding to the AndroidPua.
     *
     * Note that each Bitmap is cached by this class, which means that, if you modify a
     * Bitmap object (using setPos() method), all same emoji Bitmap will be modified.
     * If it is unacceptable, please copy the object before modifying it.
     *
     * @param pua A unicode codepoint.
     * @return Bitmap object when this factory knows the Bitmap relevant to the codepoint.
     * Otherwise null is returned.
     */
    CARAPI GetBitmapFromAndroidPua(
        /* [in] */ Int32 pua,
        /* [out] */ IBitmap** RetBitmap);

    /**
     * Constructs an instance of EmojiFactory corresponding to the name.
     *
     * @param class_name Name of the factory. This must include complete package name.
     * @return A concrete EmojiFactory instance corresponding to factory_name.
     * If factory_name is invalid, null is returned.
     */
    static CARAPI NewInstance(
        /* [in] */ const String& class_name
        /* [out] */ IEmojiFactory** EmojiFactory);


    /**
     * Constructs an instance of available EmojiFactory.
     *
     * @return A concrete EmojiFactory instance. If there are several available
     * EmojiFactory class, preferred one is chosen by the system. If there isn't, null
     * is returned.
     */
    static CARAPI NewAvailableInstance(
        /* [out] */ IEmojiFactory** EmojiFactory);

    /**
     * Returns the lowest code point corresponding to an Android
     * emoji character.
     */
    CARAPI GetMinimumAndroidPua(
        /* [out] */ Int32* RetValue);

    /**
     * Returns the highest code point corresponding to an Android
     * emoji character.
     */
    CARAPI GetMaximumAndroidPua(
        /* [out] */ Int32* RetValue);

    CARAPI GetName(
        /* [out] */ String* RetValue);

   /**
    * Returns Bitmap object corresponding to the vendor specified sjis.
    *
    * See comments in getBitmapFromAndroidPua().
    *
    * @param sjis sjis code specific to each career(vendor)
    * @return Bitmap object when this factory knows the Bitmap relevant to the code. Otherwise
    * null is returned.
    */
    static CARAPI GetBitmapFromVendorSpecificSjis(
        /* [in] */char sjis,
        /* [out] */ IBitmap** RetBitmap);

   /**
    * Returns Bitmap object corresponding to the vendor specific Unicode.
    *
    * See comments in getBitmapFromAndroidPua().
    *
    * @param vsp vendor specific PUA.
    * @return Bitmap object when this factory knows the Bitmap relevant to the code. Otherwise
    * null is returned.
    */
    static CARAPI GetBitmapFromVendorSpecificPua(
        /* [in] */ Int32 vsp,
        /* [out] */ IBitmap** RetBitmap);

   /**
    * Returns Unicode PUA for Android corresponding to the vendor specific sjis.
    *
    * @param sjis vendor specific sjis
    * @return Unicode PUA for Android, or -1 if there's no map for the sjis.
    */
    static CARAPI GetAndroidPuaFromVendorSpecificSjis(
        /* [in] */ char sjis,
        /* [out] */ Int32* RetValue);

   /**
    * Returns vendor specific sjis corresponding to the Unicode AndroidPua.
    *
    * @param pua Unicode PUA for Android,
    * @return vendor specific sjis, or -1 if there's no map for the AndroidPua.
    */
    static CARAPI GetVendorSpecificSjisFromAndroidPua(
        /* [in] */ Int32 pua,
        /* [out] */ Int32* RetValue);

   /**
    * Returns Unicode PUA for Android corresponding to the vendor specific Unicode.
    *
    * @param vsp vendor specific PUA.
    * @return Unicode PUA for Android, or -1 if there's no map for the
    * Unicode.
    */
    static CARAPI GetAndroidPuaFromVendorSpecificPua(
        /* [in] */ Int32 vsp,
        /* [out] */ Int32* RetValue);

    static CARAPI GetAndroidPuaFromVendorSpecificPua(
        /* [in] */ const String& vspString,
        /* [out] */ String* RetValue);

    /**
     * Returns vendor specific Unicode corresponding to the Unicode AndroidPua.
     *
     * @param pua Unicode PUA for Android,
     * @return vendor specific sjis, or -1 if there's no map for the AndroidPua.
     */
    static CARAPI GetVendorSpecificPuaFromAndroidPua(
        /* [in] */ Int32 pua
        /* [out] */ Int32* RetValue);

    static CARAPI GetVendorSpecificPuaFromAndroidPua(
        /* [in] */ const String& puaString,
        /* [out] */ String* RetValue);


private:
    Int32 sCacheSize = 100;
    Int32 mNativeEmojiFactory;
    String mName;


     IMap<Integer, Bitmap> mCache;

    constructor(
        /* [in] */ Int32 nativeEmojiFactory,
        /* [in] */ const String& name);

    void NativeDestructor(
        /* [in] */ Int32 factory);
    Int32 NativeGetMinimumAndroidPua(
        /* [in] */ Int32 nativeEmojiFactory);
    Int32 NativeGetMaximumAndroidPua(
       /* [in] */ Int32 nativeEmojiFactory);
    Bitmap NativeGetBitmapFromAndroidPua(
       /* [in] */ Int32 nativeEmojiFactory,
       /* [in] */ Int32 AndroidPua);
    Int32 NativeGetAndroidPuaFromVendorSpecificSjis(
       /* [in] */ Int32 nativeEmojiFactory,
       /* [in] */ char sjis);
    Int32 NativeGetVendorSpecificSjisFromAndroidPua(
       /* [in] */ Int32 nativeEmojiFactory,
       /* [in] */ Int32 pua);
    Int32 NativeGetAndroidPuaFromVendorSpecificPua(
       /* [in] */ Int32 nativeEmojiFactory,
       /* [in] */ Int32 vsp);
    Int32 NativeGetVendorSpecificPuaFromAndroidPua(
       /* [in] */ Int32 nativeEmojiFactory,
       /* [in] */ Int32 pua);
    Int32 NativeGetMaximumVendorSpecificPua(
       /* [in] */ Int32 nativeEmojiFactory);
    Int32 NativeGetMinimumVendorSpecificPua(
       /* [in] */ Int32 nativeEmojiFactory);

    class CustomLinkedHashMap<K, V> : HashMap<K, V>
    {
        public:
            CustomLinkedHashMap();
        protected:
            Boolean removeEldestEntry(Map.Entry<K, V> eldest);
    };


};

} // namespace Emoji
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_EMOJI_EMOJIFACTORY_H__
