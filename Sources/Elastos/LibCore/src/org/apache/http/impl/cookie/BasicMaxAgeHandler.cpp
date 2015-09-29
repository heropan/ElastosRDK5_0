
#include "BasicMaxAgeHandler.h"
#include "StringUtils.h"
#include "CSystem.h"
#include "CDate.h"
#include "Logger.h"

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

ECode BasicMaxAgeHandler::Parse(
    /* [in] */ ISetCookie* cookie,
    /* [in] */ const String& value)
{
    if (cookie == NULL) {
        Logger::E("BasicMaxAgeHandler", "Cookie may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (value.IsNull()) {
        Logger::E("BasicMaxAgeHandler", "Missing value for max-age attribute");
        return E_MALFORMED_COOKIE_EXCEPTION;
    }
    Int32 age;
    // try {
    age = StringUtils::ParseInt32(value);
    // } catch (NumberFormatException e) {
    //     throw new MalformedCookieException ("Invalid max-age attribute: "
    //             + value);
    // }
    if (age < 0) {
        Logger::E("BasicMaxAgeHandler", "Negative max-age attribute: %s", value.string());
        return E_MALFORMED_COOKIE_EXCEPTION;
    }
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int64 current;
    system->GetCurrentTimeMillis(&current);
    AutoPtr<IDate> date;
    CDate::New(current + age * 1000L, (IDate**)&date);
    return cookie->SetExpiryDate(date);
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org