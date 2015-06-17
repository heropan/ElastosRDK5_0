#ifndef __UTILITY_PREFS_FILEPREFERENCESIMPL_H__
#define __UTILITY_PREFS_FILEPREFERENCESIMPL_H__

#include "Object.h"

namespace Elastos {
namespace Utility {
namespace Prefs {

class FilePreferencesImpl
    : public AbstractPreferences
{
public:
    CAR_INTERFACE_DECL();

    CARAPI ToString(
        /* [out] */ String* info);

    /*
     * --------------------------------------------------------------
     * Constructors
     * --------------------------------------------------------------
     */

    /**
     * Construct root <code>FilePreferencesImpl</code> instance rooted
     * at the given path.
     *
     * @hide
     */
    FilePreferencesImpl(
        /* [in] */ const String& path,
        /* [in] */ Boolean isUserNode);

    CARAPI_(void) InitPrefs();

protected:
    // @Override
    CARAPI ChildrenNamesSpi(
        /* [out, callee] */ ArrayOf<String>** list) /*throws BackingStoreException*/;

    // @Override
    CARAPI_(AutoPtr<IAbstractPreferences>) ChildSpi(
        /* [in] */ const String& name);

    // @Override
    CARAPI FlushSpi() /*throws BackingStoreException*/;

    // @Override
    CARAPI_(String) GetSpi(
        /* [in] */ const String& key);

    // @Override
    CARAPI_(AutoPtr<ArrayOf<IInterface*> >) KeysSpi()/* throws BackingStoreException*/;

    // @Override
    CARAPI PutSpi(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    // @Override
    CARAPI RemoveNodeSpi() /*throws BackingStoreException */;

    // @Override
    CARAPI RemoveSpi(
        /* [in] */ const String& key);

    // @Override
    CARAPI SyncSpi()/* throws BackingStoreException */;

private:
    /**
     * Construct a prefs using given parent and given name
     */
    FilePreferencesImpl(
        /* [in] */ IAbstractPreferences* parent,
        /* [in] */ const String& name);

private:
    //prefs file name
    static const String PREFS_FILE_NAME;

    //file path for this preferences node
    String mPath;

    //internal cache for prefs key-value pair
    AutoPtr<IProperties> mPrefs;

    //file represents this preferences node
    AutoPtr<IFile> mPrefsFile;

    //parent dir for this preferences node
    AutoPtr<IFile> mDir;

    //cache for removed prefs key-value pair
    AutoPtr<ISet> mRemoved = new HashSet<String>();

    //cache for updated prefs key-value pair
    AutoPtr<ISet> mUpdated = new HashSet<String>();
};

} // namespace Prefs
} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_PREFS_FILEPREFERENCESIMPL_H__