
#ifndef __ELASTOS_DROID_OS_SYSTEMPROPERTIES_H
#define __ELASTOS_DROID_OS_SYSTEMPROPERTIES_H

#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Privacy::IPrivacySettingsManager;
using Elastos::Droid::Content::Pm::IIPackageManager;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Gives access to the system properties store.  The system properties
 * store contains a list of string key-value pairs.
 *
 * {@hide}
 */
class SystemProperties
{
public:
    /**
     * Get the value for the given key.
     * @return an empty string if the key isn't found
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI Get(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    /**
     * Get the value for the given key.
     * @return if the key isn't found, return def if it isn't null, or an empty string otherwise
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI Get(
        /* [in] */ const String& key,
        /* [in] */ const String& def,
        /* [out] */ String* value);

    /**
     * Get the value for the given key, and return as an integer.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as an integer, or def if the key isn't found or
     *         cannot be parsed
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Int32) GetInt32(
        /* [in] */ const String& key,
        /* [in] */ Int32 def);

    /**
     * Get the value for the given key, and return as a long.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as a long, or def if the key isn't found or
     *         cannot be parsed
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Int64) GetInt64(
        /* [in] */ const String& key,
        /* [in] */ Int64 def);

    /**
     * Get the value for the given key, returned as a boolean.
     * Values 'n', 'no', '0', 'false' or 'off' are considered false.
     * Values 'y', 'yes', '1', 'true' or 'on' are considered true.
     * (case insensitive).
     * If the key does not exist, or has any other value, then the default
     * result is returned.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as a boolean, or def if the key isn't found or is
     *         not able to be parsed as a boolean.
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Boolean) GetBoolean(
        /* [in] */ const String& key,
        /* [in] */ Boolean def);

    /**
     * Set the value for the given key.
     * @throws IllegalArgumentException if the key exceeds 32 characters
     * @throws IllegalArgumentException if the value exceeds 92 characters
     */
    static CARAPI Set(
        /* [in] */ const String& key,
        /* [in] */ const String& val);

    static CARAPI AddChangeCallback(
        /* [in] */ IRunnable* callback);

    static CARAPI_(void) CallChangeCallbacks();

private:
    static CARAPI_(String) NativeGet(
        /* [in] */ const String& key);

    static CARAPI_(String) NativeGet(
        /* [in] */ const String& key,
        /* [in] */ const String& def);

    static CARAPI_(Int32) NativeGetInt32(
        /* [in] */ const String& key,
        /* [in] */ Int32 def);

    static CARAPI_(Int64) NativeGetInt64(
        /* [in] */ const String& key,
        /* [in] */ Int64 def);

    static CARAPI_(Boolean) NativeGetBoolean(
        /* [in] */ const String& key,
        /* [in] */ Boolean def);

    static CARAPI_(void) NativeSet(
        /* [in] */ const String& key,
        /* [in] */ const String& val);

    static CARAPI_(void) NativeAddChangeCallback();

private:
        /**
     * {@hide}
     * @return package names of current process which is using this object or null if something went wrong
     */
    static AutoPtr<ArrayOf<String> > GetPackageName();

    /**
     * This method returns the fake image which should be in system folder!
     * @return byte array of jpeg fake image or null if something went wrong
     * {@hide}
     */
    static AutoPtr<ArrayOf<Byte> > GetFakeImage();

    /**
     * {@hide}
     * This method sets up all variables which are needed for privacy mode! It also writes to privacyMode, if everything was successfull or not!
     * -> privacyMode = true ok! otherwise false!
     * CALL THIS METHOD IN CONSTRUCTOR!
     */
    static void Initiate();

    /**
     * {@hide}
     * This method should be used, because in some devices the uid has more than one package within!
     * @return IS_ALLOWED (-1) if all packages allowed, IS_NOT_ALLOWED(-2) if one of these packages not allowed, GOT_ERROR (-3) if something went wrong
     */
    static Int32 CheckIfPackagesAllowed();

    /**
     * Loghelper method, true = access successful, false = blocked access
     * {@hide}
     */
    static void DataAccess(
        /* [in] */ Boolean success);

public:
    static const Int32 PROP_NAME_MAX = 31;
    static const Int32 PROP_VALUE_MAX = 91;

private:
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //BEGIN PRIVACY

    static List< AutoPtr<IRunnable> > sChangeCallbacks;
    static Object sChangeCallbacksLock;

    static const Int32 IS_ALLOWED;// = -1;
    static const Int32 IS_NOT_ALLOWED;// = -2;
    static const Int32 GOT_ERROR;// = -3;

    static const String PRIVACY_TAG;// = "PM,Camera";

    static AutoPtr<IContext> mContext;

    static AutoPtr<IPrivacySettingsManager> mPrivacySettingsManager;

    static Boolean mPrivacyMode;

    static AutoPtr<IIPackageManager> mPackageManager;

    //END PRIVACY
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_SYSTEMPROPERTIES_H

