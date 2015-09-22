
#ifndef __ELASTOS_DROID_SERVER_PREFERREDCOMPONENT_H__
#define __ELASTOS_DROID_SERVER_PREFERREDCOMPONENT_H__

#include "Elastos.Droid.Server_server.h"

using Elastos::IO::IPrintWriter;
using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;

using Elastos::Droid::Content::IComponentName;

namespace Elastos {
namespace Droid {
namespace Server {

class PreferredComponent : public ElRefBase
{
public:
    interface ICallbacks : public IInterface
    {
    public:
        virtual OnReadTag(
            /* [in] */ const String& tagName,
            /* [in] */ IXmlPullParser* parser,
            /* [out] */ Boolean* result) = 0;
    };

public:
    PreferredComponent(
        /* [in] */ ICallbacks* callbacks,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* component);

    PreferredComponent(
        /* [in] */ ICallbacks* callbacks,
        /* [in] */ IXmlPullParser* parser);

    CARAPI_(String) GetParseError();

    CARAPI WriteToXml(
        /* [in] */ IXmlSerializer* serializer);

    CARAPI_(Boolean) SameSet(
        /* [in] */ IObjectContainer* query,
        /* [in] */ Int32 priority);

    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* out,
        /* [in] */ const String& prefix,
        /* [in] */ IInterface* ident);

public:
    Int32 mMatch;
    AutoPtr<IComponentName> mComponent;

private:
    AutoPtr<ArrayOf<String> > mSetPackages;
    AutoPtr<ArrayOf<String> > mSetClasses;
    AutoPtr<ArrayOf<String> > mSetComponents;
    String mShortComponent;
    String mParseError;
    AutoPtr<ICallbacks> mCallbacks;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PREFERREDCOMPONENT_H__
