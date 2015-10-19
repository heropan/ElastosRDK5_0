
#include "HttpDateGenerator.h"
#include "CSystem.h"
#include "elastos/text/CSimpleDateFormat.h"
#include "CDate.h"
#include "CTimeZoneHelper.h"
#include "AutoLock.h"
#include "Logger.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

static AutoPtr<ITimeZone> InitGMT()
{
    AutoPtr<CTimeZoneHelper> helper;
    CTimeZoneHelper::AcquireSingletonByFriend((CTimeZoneHelper**)&helper);
    AutoPtr<ITimeZone> tz;
    helper->GetTimeZone(String("GMT"), (ITimeZone**)&tz);
    return tz;
}
const AutoPtr<ITimeZone> HttpDateGenerator::GMT = InitGMT();

CAR_INTERFACE_IMPL(HttpDateGenerator, Object, IHttpDateGenerator)

HttpDateGenerator::HttpDateGenerator()
    : mDateAsLong(0)
    , mDateAsText(String(NULL))
{}

ECode HttpDateGenerator::Init()
{
    AutoPtr<ISimpleDateFormat> sdf;
    AutoPtr<ILocale> l/*ILocale::US*/;
    CSimpleDateFormat::New(PATTERN_RFC1123, l, (ISimpleDateFormat**)&sdf);
    mDateformat = IDateFormat::Probe(sdf);
    mDateformat->SetTimeZone(GMT);
    return NOERROR;
}

ECode HttpDateGenerator::GetCurrentDate(
    /* [out] */ String* date)
{
    VALIDATE_NOT_NULL(date)
    synchronized(this)
    {
        Int64 now;
        AutoPtr<ISystem> system;
        CSystem::AcquireSingleton((ISystem**)&system);
        system->GetCurrentTimeMillis(&now);
        if (now - mDateAsLong > 1000) {
            // Generate new date string
            AutoPtr<IDate> d;
            CDate::New(now, (IDate**)&d);
            mDateformat->Format(d, &mDateAsText);
            mDateAsLong = now;
        }
        *date = mDateAsText;
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
