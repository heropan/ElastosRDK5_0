#include "os/Build.h"
#ifdef DROID_CORE
#include "os/CSystemProperties.h"
#endif
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Os {

static String GetINCREMENTAL()
{
    return Build::GetString("ro.build.version.incremental");
}

static String GetRELEASE()
{
    return Build::GetString("ro.build.version.release");
}

static String GetSDK()
{
    return Build::GetString("ro.build.version.sdk");
}

static Int32 GetSDKInt()
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    Int32 value;
    sysProp->GetInt32(String("ro.build.version.sdk"), 0, &value);
    return value;
}

static String GetCodeName()
{
    return Build::GetString("ro.build.version.codename");
}

static Int32 GetResourcesSDKInt()
{
    String codeName = GetCodeName();
    return GetSDKInt() + ((codeName == "REL") ? 0 : 1);
}

const String Build::VERSION::INCREMENTAL = GetINCREMENTAL();
const String Build::VERSION::RELEASE = GetRELEASE();
const String Build::VERSION::SDK = GetSDK();
const Int32 Build::VERSION::SDK_INT = GetSDKInt();
const String Build::VERSION::CODENAME = GetCodeName();
const Int32 Build::VERSION::RESOURCES_SDK_INT = GetResourcesSDKInt();

const Int32 Build::VERSION_CODES::CUR_DEVELOPMENT;
const Int32 Build::VERSION_CODES::BASE;
const Int32 Build::VERSION_CODES::BASE_1_1;
const Int32 Build::VERSION_CODES::CUPCAKE;
const Int32 Build::VERSION_CODES::DONUT;
const Int32 Build::VERSION_CODES::ECLAIR;
const Int32 Build::VERSION_CODES::ECLAIR_0_1;
const Int32 Build::VERSION_CODES::ECLAIR_MR1;
const Int32 Build::VERSION_CODES::FROYO;
const Int32 Build::VERSION_CODES::GINGERBREAD;
const Int32 Build::VERSION_CODES::GINGERBREAD_MR1;
const Int32 Build::VERSION_CODES::HONEYCOMB;
const Int32 Build::VERSION_CODES::HONEYCOMB_MR1;
const Int32 Build::VERSION_CODES::HONEYCOMB_MR2;
const Int32 Build::VERSION_CODES::ICE_CREAM_SANDWICH;
const Int32 Build::VERSION_CODES::ICE_CREAM_SANDWICH_MR1;
const Int32 Build::VERSION_CODES::JELLY_BEAN;
const Int32 Build::VERSION_CODES::JELLY_BEAN_MR1;

const String Build::UNKNOWN = String("unknown");
const String Build::ID = GetString("ro.build.id");
const String Build::DISPLAY = GetString("ro.build.display.id");
const String Build::FIRMWARE = GetString("ro.product.firmware");
const String Build::PRODUCT = GetString("ro.product.name");
const String Build::DEVICE = GetString("ro.product.device");
const String Build::BOARD = GetString("ro.product.board");
const String Build::CPU_ABI = GetString("ro.product.cpu.abi");
const String Build::CPU_ABI2 = GetString("ro.product.cpu.abi2");
const String Build::MANUFACTURER = GetString("ro.product.manufacturer");
const String Build::BRAND = GetString("ro.product.brand");
const String Build::MODEL = GetString("ro.product.model");
const String Build::BOOTLOADER = GetString("ro.bootloader");
const String Build::RADIO = GetString("gsm.version.baseband");
const String Build::HARDWARE = GetString("ro.hardware");
const String Build::SERIAL = GetString("ro.serialno");
const String Build::TYPE = String("user"); //GetString("ro.build.type");
const String Build::TAGS = GetString("ro.build.tags");
const String Build::FINGERPRINT = GetString("ro.build.fingerprint");
const Int64 Build::TIME = GetLong("ro.build.date.utc") * 1000;
const String Build::USER = GetString("ro.build.user");
const String Build::HOST = GetString("ro.build.host");
const String Build::TABLETUI = GetString("ro.property.tabletUI");
const String Build::DEFAULT_FONTSCALE = GetString("ro.property.fontScale");

static Boolean InitISDEBUGGABLE()
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    Int32 value;
    sysProp->GetInt32(String("ro.debuggable"), 0, &value);
    return value == 1;
}
const Boolean Build::IS_DEBUGGABLE = InitISDEBUGGABLE();

String Build::GetString(
    /* [in] */ const char* property)
{
    String str(property);
    return GetString(str);
}

Int64 Build::GetLong(
    /* [in] */ const char* property)
{
    String str(property);
    return GetLong(str);
}

String Build::GetString(
    /* [in] */ const String& property)
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String value;
    sysProp->Get(property, UNKNOWN, &value);
    return value;
}

Int64 Build::GetLong(
    /* [in] */ const String& property)
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String retStr;
    sysProp->Get(property, UNKNOWN, &retStr);
    if (retStr.Equals(UNKNOWN))
        return -1;

    return StringUtils::ParseInt64(retStr);
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
