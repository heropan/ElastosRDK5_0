
#include "FilePreferencesFactoryImpl.h"

namespace Elastos {
namespace Utility {
namespace Prefs {

extern "C" const InterfaceID EIID_FilePreferencesFactoryImpl =
        { 0x153c5ecc, 0x3219, 0x4cfc, { 0x92, 0xeb, 0x33, 0xba, 0x9c, 0xed, 0x1d, 0x50 } };

CAR_INTERFACE_IMPL(FilePreferencesFactoryImpl, Object, IPreferencesFactory);

FilePreferencesFactoryImpl::FilePreferencesFactoryImpl()
{
}

ECode FilePreferencesFactoryImpl::UserRoot(
    /* [out] */ IPreferences** outpre)
{
    VALIDATE_NOT_NULL(outpre);
    *outpre = USER_ROOT;
    REFCOUNT_ADD(*outpre);
    return NOERROR;
}

ECode FilePreferencesFactoryImpl::SystemRoot(
    /* [out] */ IPreferences** outpre)
{
    VALIDATE_NOT_NULL(outpre);
    *outpre = SYSTEM_ROOT;
    REFCOUNT_ADD(*outpre);
    return NOERROR;
}

ECode FilePreferencesFactoryImpl::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = String("Elastos.Utility.Prefs.FilePreferencesFactoryImpl");
    return NOERROR;
}

} // namespace Prefs
} // namespace Utility
} // namespace Elastos
