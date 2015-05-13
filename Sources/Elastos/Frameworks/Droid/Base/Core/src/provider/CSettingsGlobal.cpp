
#include "ext/frameworkdef.h"
#include "provider/CSettingsGlobal.h"
#include "provider/Settings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CSettingsGlobal::GetUriFor(
    /* [in] */ IUri* u,
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    return Settings::Global::GetUriFor(u, name, uri);
}

ECode CSettingsGlobal::GetContentUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = Settings::Global::CONTENT_URI;
    INTERFACE_ADDREF(*uri)
    return NOERROR;
}

ECode CSettingsGlobal::GetSettingsToBackup(
    /* [out] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array)
    *array = Settings::Global::SETTINGS_TO_BACKUP;
    ARRAYOF_ADDREF(*array)
    return NOERROR;
}

ECode CSettingsGlobal::GetBluetoothHeadsetPriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)
    *key = Settings::Global::GetBluetoothHeadsetPriorityKey(address);
    return NOERROR;
}

ECode CSettingsGlobal::GetBluetoothA2dpSinkPriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)
    *key = Settings::Global::GetBluetoothA2dpSinkPriorityKey(address);
    return NOERROR;
}

ECode CSettingsGlobal::GetBluetoothInputDevicePriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)
    *key = Settings::Global::GetBluetoothInputDevicePriorityKey(address);
    return NOERROR;
}

ECode CSettingsGlobal::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetString(resolver, name, value);
}

ECode CSettingsGlobal::GetStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetStringForUser(resolver, name, userHandle, value);
}

ECode CSettingsGlobal::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Global::PutString(resolver, name, value, result);
}

ECode CSettingsGlobal::PutStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::PutStringForUser(resolver, name, value, userHandle, result);
}

ECode CSettingsGlobal::GetUriForEx(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return Settings::Global::GetUriForEx(name, uri);
}

ECode CSettingsGlobal::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetInt32(cr, name, def, value);
}

ECode CSettingsGlobal::GetInt32Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetInt32(cr, name, value);
}

ECode CSettingsGlobal::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Global::PutInt32(cr, name, value, result);
}

ECode CSettingsGlobal::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetInt64(cr, name, def, value);
}

ECode CSettingsGlobal::GetInt64Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetInt64(cr, name, value);
}

ECode CSettingsGlobal::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Global::PutInt64(cr, name, value, result);
}

ECode CSettingsGlobal::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetFloat(cr, name, def, value);
}

ECode CSettingsGlobal::GetFloatEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Global::GetFloat(cr, name, value);
}

ECode CSettingsGlobal::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Global::PutFloat(cr, name, value, result);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos