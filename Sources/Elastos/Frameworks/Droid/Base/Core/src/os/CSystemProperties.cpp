
#include "ext/frameworkdef.h"
#include "os/CSystemProperties.h"
#include "os/SystemProperties.h"

namespace Elastos {
namespace Droid {
namespace Os {

CAR_INTERFACE_DECL(CSystemProperties, Singleton, ISystemProperties)

CAR_SINGLETON_DECL(CSystemProperties)

ECode CSystemProperties::Get(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    return SystemProperties::Get(key, value);
}

ECode CSystemProperties::Get(
    /* [in] */ const String& key,
    /* [in] */ const String& def,
    /* [out] */ String* value)
{
    return SystemProperties::Get(key, def, value);
}

ECode CSystemProperties::GetInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = SystemProperties::GetInt32(key, def);
    return NOERROR;
}

ECode CSystemProperties::GetInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = SystemProperties::GetInt64(key, def);
    return NOERROR;
}

ECode CSystemProperties::GetBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean def,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = SystemProperties::GetBoolean(key, def);
    return NOERROR;
}

ECode CSystemProperties::Set(
    /* [in] */ const String& key,
    /* [in] */ const String& val)
{
    return SystemProperties::Set(key, val);
}

ECode CSystemProperties::AddChangeCallback(
    /* [in] */ IRunnable* cb)
{
    return SystemProperties::AddChangeCallback(cb);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
