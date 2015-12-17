
#include "elastos/droid/server/am/CActivityManagerService.h"
#include "elastos/droid/server/am/BroadcastFilter.h"
#include "elastos/droid/server/am/ReceiverList.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Droid::Content::EIID_IIntentFilter;
//TODO using Elastos::Droid::Content::CIntentFilter;

#include <elastos/utility/logging/Slogger.h>
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// {65796e4b-488d-46ba-86c0-79002facc6a2}
extern const InterfaceID EIID_BroadcastFilter =
        { 0x65796e4b, 0x488d, 0x46ba, { 0x86, 0xc0, 0x79, 0x00, 0x2f, 0xac, 0xc6, 0xa2 } };

BroadcastFilter::BroadcastFilter()
    : mReceiverList(NULL)
    , mOwningUid(0)
    , mOwningUserId(0)
{
    IntentFilter::Init();
}

BroadcastFilter::BroadcastFilter(
    /* [in] */ IIntentFilter* intentFilter,
    /* [in] */ ReceiverList* receiverList,
    /* [in] */ const String& packageName,
    /* [in] */ const String& requiredPermission,
    /* [in] */ Int32 owningUid,
    /* [in] */ Int32 userId)
    : IntentFilter(intentFilter)
    , mReceiverList(receiverList)
    , mPackageName(packageName)
    , mRequiredPermission(requiredPermission)
    , mOwningUid(owningUid)
    , mOwningUserId(userId)
{}

BroadcastFilter::~BroadcastFilter()
{
}

PInterface BroadcastFilter::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if ( riid == EIID_BroadcastFilter) {
        return reinterpret_cast<PInterface>(this);
    }
    else if (riid == EIID_IIntentFilter) {
        return (IIntentFilter*)this;
    }
    return NULL;
}

UInt32 BroadcastFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BroadcastFilter::Release()
{
    return ElRefBase::Release();
}

ECode BroadcastFilter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

String BroadcastFilter::ToString()
{
    StringBuilder sb(128);
    sb += ("BroadcastFilter{");
    sb += (StringUtils::Int32ToHexString(Int32(this)));
    sb += ("userId=");
    sb += (mOwningUserId);

    sb += ", actions={";
    AutoPtr<ArrayOf<String> > actions;
    GetActions((ArrayOf<String>**)&actions);
    if (actions) {
        for (Int32 i = 0; i < actions->GetLength(); ++i) {
            if (i != 0) {
                sb += ", ";
            }
            sb += (*actions)[i];

        }
    }
    sb += "}, receiverList={";

    if (mReceiverList != NULL)
        sb += mReceiverList->ToString();
    sb += "}}";
    return sb.ToString();
}

ECode BroadcastFilter::SetPriority(
    /* [in] */ Int32 priority)
{
    return IntentFilter::SetPriority(priority);
}

ECode BroadcastFilter::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority)
    *priority = IntentFilter::GetPriority();
    return NOERROR;
}

ECode BroadcastFilter::AddAction(
    /* [in] */ const String& action)
{
    return IntentFilter::AddAction(action);
}

ECode BroadcastFilter::CountActions(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountActions();
    return NOERROR;
}

ECode BroadcastFilter::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String* action)
{
    VALIDATE_NOT_NULL(action)
    *action = IntentFilter::GetAction(index);
    return NOERROR;
}

ECode BroadcastFilter::HasAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* hasAction)
{
    VALIDATE_NOT_NULL(hasAction)
    *hasAction = IntentFilter::HasAction(action);
    return NOERROR;
}

ECode BroadcastFilter::MatchAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* isMatched)
{
    VALIDATE_NOT_NULL(isMatched)
    *isMatched = IntentFilter::MatchAction(action);
    return NOERROR;
}

ECode BroadcastFilter::GetActions(
    /* [out, callee] */ ArrayOf<String>** actions)
{
    VALIDATE_NOT_NULL(actions)
    AutoPtr< ArrayOf<String> > acts = IntentFilter::GetActions();
    *actions = acts;
    REFCOUNT_ADD(*actions);
    return NOERROR;
}

ECode BroadcastFilter::AddDataType(
    /* [in] */ const String& type)
{
    return IntentFilter::AddDataType(type);
}

ECode BroadcastFilter::HasDataType(
    /* [in] */ const String& type,
    /* [out] */ Boolean* hasDataType)
{
    VALIDATE_NOT_NULL(hasDataType)
    *hasDataType = IntentFilter::HasDataType(type);
    return NOERROR;
}

ECode BroadcastFilter::CountDataTypes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataTypes();
    return NOERROR;
}

ECode BroadcastFilter::GetDataType(
    /* [in] */ Int32 index,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)
    *type = IntentFilter::GetDataType(index);
    return NOERROR;
}

ECode BroadcastFilter::GetTypes(
    /* [out, callee] */ ArrayOf<String>** types)
{
    VALIDATE_NOT_NULL(types)
    AutoPtr< ArrayOf<String> > typs = IntentFilter::GetTypes();
    *types = typs;
    REFCOUNT_ADD(*types);
    return NOERROR;
}

ECode BroadcastFilter::AddDataScheme(
    /* [in] */ const String& scheme)
{
    return IntentFilter::AddDataScheme(scheme);
}

ECode BroadcastFilter::CountDataSchemes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataSchemes();
    return NOERROR;
}

ECode BroadcastFilter::GetDataScheme(
    /* [in] */ Int32 index,
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme)
    *scheme = IntentFilter::GetDataScheme(index);
    return NOERROR;
}

ECode BroadcastFilter::HasDataScheme(
    /* [in] */ const String& scheme,
    /* [out] */ Boolean* hasDataScheme)
{
    VALIDATE_NOT_NULL(hasDataScheme)
    *hasDataScheme = IntentFilter::HasDataScheme(scheme);
    return NOERROR;
}

ECode BroadcastFilter::GetSchemes(
    /* [out, callee] */ ArrayOf<String>** schemes)
{
    VALIDATE_NOT_NULL(schemes)
    AutoPtr< ArrayOf<String> > schs = IntentFilter::GetSchemes();
    *schemes = schs;
    REFCOUNT_ADD(*schemes);
    return NOERROR;
}

ECode BroadcastFilter::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    return IntentFilter::AddDataAuthority(host, port);
}

ECode BroadcastFilter::CountDataAuthorities(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataAuthorities();
    return NOERROR;
}

ECode BroadcastFilter::GetDataAuthority(
    /* [in] */ Int32 index,
    /* [out] */ IIntentFilterAuthorityEntry** authority)
{
    VALIDATE_NOT_NULL(authority)
    AutoPtr<IIntentFilterAuthorityEntry> tmp = IntentFilter::GetDataAuthority(index);
    *authority = tmp.Get();
    REFCOUNT_ADD(*authority)
    return NOERROR;
}

ECode BroadcastFilter::HasDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Boolean* hasDataAuthority)
{
    VALIDATE_NOT_NULL(hasDataAuthority)
    *hasDataAuthority = IntentFilter::HasDataAuthority(data);
    return NOERROR;
}

ECode BroadcastFilter::GetAuthorities(
    /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities)
{
    VALIDATE_NOT_NULL(authorities)
    AutoPtr< ArrayOf<IIntentFilterAuthorityEntry*> > auths = IntentFilter::GetAuthorities();
    *authorities = auths;
    REFCOUNT_ADD(*authorities)
    return NOERROR;
}

ECode BroadcastFilter::AddDataPath(
    /* [in] */ const String& path,
    /* [in] */ Int32 type)
{
    return IntentFilter::AddDataPath(path, type);
}

ECode BroadcastFilter::CountDataPaths(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataPaths();
    return NOERROR;
}

ECode BroadcastFilter::GetDataPath(
    /* [in] */ Int32 index,
    /* [out] */ IPatternMatcher** path)
{
    VALIDATE_NOT_NULL(path)
    AutoPtr<IPatternMatcher> tmp = IntentFilter::GetDataPath(index);
    *path = tmp;
    REFCOUNT_ADD(*path)
    return NOERROR;
}

ECode BroadcastFilter::HasDataPath(
    /* [in] */ const String& data,
    /* [out] */ Boolean* hasDataPath)
{
    VALIDATE_NOT_NULL(hasDataPath)
    *hasDataPath = IntentFilter::HasDataPath(data);
    return NOERROR;
}

ECode BroadcastFilter::GetPaths(
    /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths)
{
    VALIDATE_NOT_NULL(paths)
    AutoPtr< ArrayOf<IPatternMatcher*> > pas = IntentFilter::GetPaths();
    *paths = pas;
    REFCOUNT_ADD(*paths)
    return NOERROR;
}

ECode BroadcastFilter::MatchDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::MatchDataAuthority(data);
    return NOERROR;
}

ECode BroadcastFilter::MatchData(
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::MatchData(type, scheme, data);
    return NOERROR;
}

ECode BroadcastFilter::AddCategory(
    /* [in] */ const String& category)
{
    return IntentFilter::AddCategory(category);
}

ECode BroadcastFilter::CountCategories(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountCategories();
    return NOERROR;
}

ECode BroadcastFilter::GetCategory(
    /* [in] */ Int32 index,
    /* [out] */ String* category)
{
    VALIDATE_NOT_NULL(category)
    *category = IntentFilter::GetCategory(index);
    return NOERROR;
}

ECode BroadcastFilter::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* hasCategory)
{
    VALIDATE_NOT_NULL(hasCategory)
    *hasCategory = IntentFilter::HasCategory(category);
    return NOERROR;
}

ECode BroadcastFilter::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    VALIDATE_NOT_NULL(categories)
    AutoPtr< ArrayOf<String> > cats = IntentFilter::GetCategories();
    *categories = cats;
    REFCOUNT_ADD(*categories);
    return NOERROR;
}

ECode BroadcastFilter::MatchCategories(
    /* [in] */ ArrayOf<String>* categories,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::MatchCategories(categories);
    return NOERROR;
}

ECode BroadcastFilter::Match(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean resolve,
    /* [in] */ const String& logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::Match(resolver, intent, resolve, logTag);
    return NOERROR;
}

ECode BroadcastFilter::Match(
    /* [in] */ const String& action,
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ const String& logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::Match(action, type, scheme, data, categories, logTag);
    return NOERROR;
}

ECode BroadcastFilter::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    return IntentFilter::WriteToXml(serializer);
}

ECode BroadcastFilter::ReadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    return IntentFilter::ReadFromXml(parser);
}

ECode BroadcastFilter::Dump(
    /* [in] */ IPrinter* du,
    /* [in] */ const String& prefix)
{
    return IntentFilter::Dump(du, prefix);
}

ECode BroadcastFilter::HasPartialTypes(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has)
    *has = mHasPartialTypes;
    return NOERROR;
}

Boolean BroadcastFilter::HasCategory(
    /* [in] */ const String& category)
{
    return IntentFilter::HasCategory(category);
}

Int32 BroadcastFilter::Match(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean resolve,
    /* [in] */ const String& logTag)
{
    return IntentFilter::Match(resolver, intent, resolve, logTag);
}

Int32 BroadcastFilter::Match(
    /* [in] */ const String& action,
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ const String& logTag)
{
    return IntentFilter::Match(action, type, scheme, data, categories, logTag);
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
