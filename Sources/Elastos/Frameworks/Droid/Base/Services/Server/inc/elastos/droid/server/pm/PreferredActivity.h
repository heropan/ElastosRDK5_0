#ifndef __ELASTOS_DROID_SERVER_PM_PREFERREDACTIVITY_H__
#define __ELASTOS_DROID_SERVER_PM_PREFERREDACTIVITY_H__

#include "elastos/droid/content/IntentFilter.h"
#include "elastos/droid/server/pm/PreferredComponent.h"

using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;
using Elastos::Droid::Content::IntentFilter;
using Elastos::Droid::Content::IComponentName;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class PreferredActivity
    : public IntentFilter
    , public PreferredComponent::ICallbacks
{
public:
    PreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* activity,
        /* [in] */ Boolean always);

    PreferredActivity(
        /* [in] */ IXmlPullParser* parser);

    CARAPI WriteToXml(
        /* [in] */ IXmlSerializer* serializer,
        /* [in] */ Boolean full);

    CARAPI OnReadTag(
        /* [in] */ const String& tagName,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    AutoPtr<PreferredComponent> mPref;

private:
    static const String TAG;

    static const Boolean DEBUG_FILTERS = FALSE;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

template <>
struct Conversion<Elastos::Droid::Server::Pm::PreferredActivity*, IInterface*>
{
    enum { exists = TRUE, exists2Way = FALSE, sameType = FALSE };
};

#endif //__ELASTOS_DROID_SERVER_PM_PREFERREDACTIVITY_H__
