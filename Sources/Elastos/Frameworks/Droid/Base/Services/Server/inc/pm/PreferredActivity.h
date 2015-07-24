#ifndef __PREFERREDACTIVITY_H__
#define __PREFERREDACTIVITY_H__

#include "Elastos.Droid.Server_server.h"
#include "content/IntentFilter.h"
#include "PreferredComponent.h"

using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;

using Elastos::Droid::Content::IntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IIntentFilterAuthorityEntry;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class PreferredActivity
    : public ElRefBase
    , public IIntentFilter
    , public PreferredComponent::ICallbacks
    , public IntentFilter
{
public:
    PreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* activity);

    PreferredActivity(
        /* [in] */ IXmlPullParser* parser);

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID);

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI AddAction(
        /* [in] */ const String& action);

    CARAPI CountActions(
        /* [out] */ Int32* count);

    CARAPI GetAction(
        /* [in] */ Int32 index,
        /* [out] */ String* action);

    CARAPI HasAction(
        /* [in] */ const String& action,
        /* [out] */ Boolean* hasAction);

    CARAPI MatchAction(
        /* [in] */ const String& action,
        /* [out] */ Boolean* isMatched);

    CARAPI GetActions(
        /* [out, callee] */ ArrayOf<String>** actions);

    CARAPI AddDataType(
        /* [in] */ const String& type);

    CARAPI HasDataType(
        /* [in] */ const String& type,
        /* [out] */ Boolean* hasDataType);

    CARAPI CountDataTypes(
        /* [out] */ Int32* Count);

    CARAPI GetDataType(
        /* [in] */ Int32 index,
        /* [out] */ String* type);

    CARAPI GetTypes(
        /* [out, callee] */ ArrayOf<String>** types);

    CARAPI AddDataScheme(
        /* [in] */ const String& scheme);

    CARAPI CountDataSchemes(
        /* [out] */ Int32* count);

    CARAPI GetDataScheme(
        /* [in] */ Int32 index,
        /* [out] */ String* scheme);

    CARAPI HasDataScheme(
        /* [in] */ const String& scheme,
        /* [out] */ Boolean* hasDataScheme);

    CARAPI GetSchemes(
        /* [out, callee] */ ArrayOf<String>** schemes);

    CARAPI AddDataAuthority(
        /* [in] */ const String& host,
        /* [in] */ const String& port);

    CARAPI CountDataAuthorities(
        /* [out] */ Int32* count);

    CARAPI GetDataAuthority(
        /* [in] */ Int32 index,
        /* [out] */ IIntentFilterAuthorityEntry** authority);

    CARAPI HasDataAuthority(
        /* [in] */ IUri* Data,
        /* [out] */ Boolean* hasDataAuthority);

    CARAPI GetAuthorities(
        /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities);

    CARAPI AddDataPath(
        /* [in] */ const String& path,
        /* [in] */ Int32 type);

    CARAPI CountDataPaths(
        /* [out] */ Int32* count);

    CARAPI GetDataPath(
        /* [in] */ Int32 index,
        /* [out] */ IPatternMatcher** path);

    CARAPI HasDataPath(
        /* [in] */ const String& data,
        /* [out] */ Boolean* hasDataPath);

    CARAPI GetPaths(
        /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths);

    CARAPI MatchDataAuthority(
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    CARAPI MatchData(
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    CARAPI AddCategory(
        /* [in] */ const String& category);

    CARAPI CountCategories(
        /* [out] */ Int32* count);

    CARAPI GetCategory(
        /* [in] */ Int32 index,
        /* [out] */ String* category);

    CARAPI HasCategory(
        /* [in] */ const String& category,
        /* [out] */ Boolean* hasCategory);

    CARAPI GetCategories(
        /* [out, callee] */ ArrayOf<String>** categories);

    CARAPI MatchCategories(
        /* [in] */ ArrayOf<String>* categories,
        /* [out] */ String* result);

    CARAPI Match(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean resolve,
        /* [in] */ const String& logTag,
        /* [out] */ Int32* result);

    CARAPI Match(
        /* [in] */ const String& action,
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [in] */ ArrayOf<String>* categories,
        /* [in] */ const String& logTag,
        /* [out] */ Int32* result);

    CARAPI Dump(
        /* [in] */ IPrinter* du,
        /* [in] */ const String& prefix);

    CARAPI HasPartialTypes(
        /* [out] */ Boolean* has);

    CARAPI ReadFromXml(
        /* [in] */ IXmlPullParser* parser);

    CARAPI WriteToXml(
        /* [in] */ IXmlSerializer* serializer);

    CARAPI OnReadTag(
        /* [in] */ const String& tagName,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ Boolean* result);

    using IntentFilter::Match;
    using IntentFilter::HasCategory;

public:
    static const String ATTR_USER_ID;

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

#endif //__PREFERREDACTIVITY_H__
