#ifndef __UTILITY_PREFS_FILEPREFERENCESFACTORYIMPL_H__
#define __UTILITY_PREFS_FILEPREFERENCESFACTORYIMPL_H__

#include "Object.h"

namespace Elastos {
namespace Utility {
namespace Prefs {

class FilePreferencesFactoryImpl
    : public Object
    , public IPreferencesFactory
{
public:
    CAR_INTERFACE_DECL();

    FilePreferencesFactoryImpl();

    CARAPI UserRoot(
        /* [out] */ IPreferences** outpre);

    CARAPI SystemRoot(
        /* [out] */ IPreferences** outpre);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    //  user root preferences
    static final Preferences USER_ROOT = new FilePreferencesImpl(
            System.getProperty("user.home") + "/.java/.userPrefs", true);

    //  system root preferences
    static final Preferences SYSTEM_ROOT = new FilePreferencesImpl(
            System.getProperty("java.home") + "/.systemPrefs", false);

};

} // namespace Prefs
} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_PREFS_FILEPREFERENCESFACTORYIMPL_H__
