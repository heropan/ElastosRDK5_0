
#ifndef __PACKAGEMANAGER_H__
#define __PACKAGEMANAGER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Droid::Content::IIntentFilter;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Class for retrieving various kinds of information related to the application
 * packages that are currently installed on the device.
 *
 * You can find this class through {@link Context#getPackageManager}.
 */
class PackageManager
{
public:
    /**
     * Retrieve overall information about an application package defined
     * in a package archive file
     *
     * @param archiveFilePath The path to the archive file
     * @param flags Additional option flags. Use any combination of
     * {@link #GET_ACTIVITIES},
     * {@link #GET_GIDS},
     * {@link #GET_CONFIGURATIONS},
     * {@link #GET_INSTRUMENTATION},
     * {@link #GET_PERMISSIONS},
     * {@link #GET_PROVIDERS},
     * {@link #GET_RECEIVERS},
     * {@link #GET_SERVICES},
     * {@link #GET_SIGNATURES}, to modify the data returned.
     *
     * @return Returns the information about the package. Returns
     * null if the package could not be successfully parsed.
     *
     * @see #GET_ACTIVITIES
     * @see #GET_GIDS
     * @see #GET_CONFIGURATIONS
     * @see #GET_INSTRUMENTATION
     * @see #GET_PERMISSIONS
     * @see #GET_PROVIDERS
     * @see #GET_RECEIVERS
     * @see #GET_SERVICES
     * @see #GET_SIGNATURES
     *
     */
    CARAPI GetPackageArchiveInfo(
        /* [in] */ const String& archiveFilePath,
        /* [in] */ Int32 flags,
        /* [out] */ IPackageInfo** info);

    /**
     * Like {@link #getPackageSizeInfo(String, int, IPackageStatsObserver)}, but
     * returns the size for the calling user.
     *
     * @hide
     */
    CARAPI GetPackageSizeInfo(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageStatsObserver* observer);

    virtual CARAPI GetPackageSizeInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userHandle,
        /* [in] */ IPackageStatsObserver* observer) = 0;

    /**
     * Same as {@link #addPreferredActivity(IntentFilter, int,
            ComponentName[], ComponentName)}, but with a specific userId to apply the preference
            to.
     * @hide
     */
    CARAPI AddPreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* activity,
        /* [in] */ Int32 userId);

    /**
     * Returns the data directory for a particular user and package, given the uid of the package.
     * @param uid uid of the package, including the userId and appId
     * @param packageName name of the package
     * @return the user-specific data directory for the package
     * @hide
     */
    static CARAPI_(String) GetDataDirForUser(
        /* [in] */ Int32 userId,
        /* [in] */ const String& packageName);
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __PACKAGEMANAGER_H__
