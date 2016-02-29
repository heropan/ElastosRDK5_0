#ifndef  __ELASTOS_DROID_PACKAGES_LAUNCHER2_ALLAPPSLIST_H__
#define  __ELASTOS_DROID_PACKAGES_LAUNCHER2_ALLAPPSLIST_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

/**
 * Stores the list of all applications for the all apps view.
 */
class AllAppsList
    : public Object
{
public:
    /**
     * Boring constructor.
     */
    AllAppsList(
        /* [in] */ IconCache* iconCache);

    /**
     * Add the supplied ApplicationInfo objects to the list, and enqueue it into the
     * list to broadcast when notify() is called.
     *
     * If the app is already in the list, doesn't add it.
     */
    Add(
        /* [in] */ ApplicationInfo* info);

    Clear();

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ ApplicationInfo** info);

    /**
     * Add the icons for the supplied apk called packageName.
     */
    CARAPI AddPackage(
        /* [in] */ IContext* context,
        /* [in] */ const String& packageName,
        /* [in] */ IUserHandle* user);

    /**
     * Remove the apps for the given apk identified by packageName.
     */
    CARAPI RemovePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IUserHandle* user);

    /**
     * Add and remove icons for this package which has been updated.
     */
    CARAPI UpdatePackage(
        /* [in] */ IContext* context,
        /* [in] */ const String& packageName,
        /* [in] */ IUserHandle* user);

private:
    /**
     * Returns whether <em>apps</em> contains <em>component</em> for a specific
     * user profile.
     */
    static CARAPI_(Boolean) FindActivity(
        /* [in] */ IList* apps,
        /* [in] */ IComponentName* component,
        /* [in] */ IUserHandle* user);

    /**
     * Returns whether <em>apps</em> contains <em>component</em> for a specific
     * user profile.
     */
    static CARAPI_(Boolean) FindActivity(
        /* [in] */ IArrayList* apps,
        /* [in] */ IComponentName* component,
        /* [in] */ IUserHandle* user);

    /**
     * Find an ApplicationInfo object for the given packageName, className and
     * user profile.
     */
    CARAPI_(AutoPtr<ApplicationInfo>) FindApplicationInfoLocked(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className,
        /* [in] */ IUserHandle* user);

public:
    static const Int32 DEFAULT_APPLICATIONS_NUMBER = 42;

    /** The list off all apps. */
    AutoPtr<IArrayList> mData;
    /** The list of apps that have been added since the last notify() call. */
    AutoPtr<IArrayList> mAdded;
    /** The list of apps that have been removed since the last notify() call. */
    AutoPtr<IArrayList> mRemoved;
    /** The list of apps that have been modified since the last notify() call. */
    AutoPtr<IArrayList> mModified;

private:
    AutoPtr<IconCache> mIconCache;

};

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_LAUNCHER2_ALLAPPSLIST_H__