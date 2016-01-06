
#include "elastos/droid/server/pm/PreferredActivity.h"
#include "elastos/droid/server/pm/CPackageManagerService.h"
#include "elastos/droid/util/XmlUtils.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::Droid::Utility::ILogHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

const String PreferredActivity::TAG("PreferredActivity");
const Boolean PreferredActivity::DEBUG_FILTERS;

PreferredActivity::PreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity,
    /* [in] */ Boolean always)
    : IntentFilter(filter)
{
    mPref = new PreferredComponent(this, match, set, activity, always);
}

PreferredActivity::PreferredActivity(
    /* [in] */ IXmlPullParser* parser)
{
    mPref = new PreferredComponent(this, parser);
}

ECode PreferredActivity::WriteToXml(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ Boolean full)
{
    FAIL_RETURN(mPref->WriteToXml(serializer, full))
    FAIL_RETURN(serializer->WriteStartTag(String(NULL), String("filter")))
    FAIL_RETURN(IntentFilter::WriteToXml(serializer))
    return serializer->WriteEndTag(String(NULL), String("filter"));
}

ECode PreferredActivity::OnReadTag(
    /* [in] */ const String& tagName,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (tagName.Equals("filter")) {
        if (DEBUG_FILTERS) {
            Slogger::I(TAG, "Starting to parse filter...");
        }
        FAIL_RETURN(IntentFilter::ReadFromXml(parser))
        if (DEBUG_FILTERS) {
            Int32 depth;
            parser->GetDepth(&depth);
            String name;
            parser->GetName(&name);
            Slogger::I(TAG, "Finished filter: depth=%d tag=%s", depth, name.string());
        }
    }
    else {
        String name;
        parser->GetName(&name);
        CPackageManagerService::ReportSettingsProblem(ILogHelper::WARN,
                String("Unknown element under <preferred-activities>: ") + name);
        XmlUtils::SkipCurrentTag(parser);
    }
    *result = TRUE;
    return NOERROR;
}

ECode PreferredActivity::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 hashCode;
    sys->IdentityHashCode((IObject*)this, &hashCode);
    String str;
    mPref->mComponent->FlattenToShortString(&str);
    *str = String("PreferredActivity{0x") + StringUtils::ToHexString(hashCode)
            + " " + str + "}";
    return NOERROR;
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
