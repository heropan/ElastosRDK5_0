
#ifndef __ELASTOS_DROID_SERVER_AM_BROADCASTFILTER_H__
#define __ELASTOS_DROID_SERVER_AM_BROADCASTFILTER_H__

#include <Elastos.Droid.Core.h>
#include "elastos/droid/content/IntentFilter.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IntentFilter;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilterAuthorityEntry;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern const InterfaceID EIID_BroadcastFilter;

class ReceiverList;

class BroadcastFilter
    : public IIntentFilter
    , public IntentFilter
    , public Object
{
public:
    BroadcastFilter();

    BroadcastFilter(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ ReceiverList* receiverList,
        /* [in] */ const String& packageName,
        /* [in] */ const String& requiredPermission,
        /* [in] */ Int32 owningUid,
        /* [in] */ Int32 userId);

    ~BroadcastFilter();

    CAR_INTERFACE_DECL()

    // public void dump(PrintWriter pw, String prefix);

    // public void dumpBrief(PrintWriter pw, String prefix);

    // public void dumpInReceiverList(PrintWriter pw, Printer pr, String prefix);

    // void dumpBroadcastFilterState(PrintWriter pw, String prefix);

    CARAPI_(String) ToString();

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

    CARAPI WriteToXml(
        /* [in] */ IXmlSerializer* serializer);

    CARAPI ReadFromXml(
        /* [in] */ IXmlPullParser* parser);

    CARAPI Dump(
        /* [in] */ IPrinter* du,
        /* [in] */ const String& prefix);

    CARAPI HasPartialTypes(
        /* [out] */ Boolean* has);

    CARAPI_(Boolean) HasCategory(
        /* [in] */ const String& category);

    CARAPI_(Int32) Match(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean resolve,
        /* [in] */ const String& logTag);

    CARAPI_(Int32) Match(
        /* [in] */ const String& action,
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [in] */ ArrayOf<String>* categories,
        /* [in] */ const String& logTag);

public:
    // Back-pointer to the list this filter is in.
    ReceiverList* mReceiverList; // BroadcastFilter will be PushBack to ReceiverList
    String mPackageName;
    String mRequiredPermission;
    Int32 mOwningUid;
    Int32 mOwningUserId;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

template <>
struct Conversion<Elastos::Droid::Server::Am::BroadcastFilter*, IInterface*>
{
    enum { exists = TRUE, exists2Way = FALSE, sameType = FALSE };
};

#endif //__ELASTOS_DROID_SERVER_AM_BROADCASTFILTER_H__
