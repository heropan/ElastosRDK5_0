
#include "content/CIntentFilter.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CIntentFilter::SetPriority(
    /* [in] */ Int32 priority)
{
    return IntentFilter::SetPriority(priority);
}

ECode CIntentFilter::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority)
    *priority = IntentFilter::GetPriority();
    return NOERROR;
}

ECode CIntentFilter::AddAction(
    /* [in] */ const String& action)
{
    return IntentFilter::AddAction(action);
}

ECode CIntentFilter::CountActions(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountActions();
    return NOERROR;
}

ECode CIntentFilter::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String* action)
{
    VALIDATE_NOT_NULL(action)
    *action = IntentFilter::GetAction(index);
    return NOERROR;
}

ECode CIntentFilter::HasAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* hasAction)
{
    VALIDATE_NOT_NULL(hasAction)
    *hasAction = IntentFilter::HasAction(action);
    return NOERROR;
}

ECode CIntentFilter::MatchAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* isMatched)
{
    VALIDATE_NOT_NULL(isMatched)
    *isMatched = IntentFilter::MatchAction(action);
    return NOERROR;
}

ECode CIntentFilter::GetActions(
    /* [out, callee] */ ArrayOf<String>** actions)
{
    VALIDATE_NOT_NULL(actions)
    AutoPtr< ArrayOf<String> > acts = IntentFilter::GetActions();
    *actions = acts;
    REFCOUNT_ADD(*actions);
    return NOERROR;
}

ECode CIntentFilter::AddDataType(
    /* [in] */ const String& type)
{
    return IntentFilter::AddDataType(type);
}

ECode CIntentFilter::HasDataType(
    /* [in] */ const String& type,
    /* [out] */ Boolean* hasDataType)
{
    VALIDATE_NOT_NULL(hasDataType)
    *hasDataType = IntentFilter::HasDataType(type);
    return NOERROR;
}

ECode CIntentFilter::CountDataTypes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataTypes();
    return NOERROR;
}

ECode CIntentFilter::GetDataType(
    /* [in] */ Int32 index,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)
    *type = IntentFilter::GetDataType(index);
    return NOERROR;
}

ECode CIntentFilter::GetTypes(
    /* [out, callee] */ ArrayOf<String>** types)
{
    VALIDATE_NOT_NULL(types)
    AutoPtr< ArrayOf<String> > typs = IntentFilter::GetTypes();
    *types = typs;
    REFCOUNT_ADD(*types);
    return NOERROR;
}

ECode CIntentFilter::AddDataScheme(
    /* [in] */ const String& scheme)
{
    return IntentFilter::AddDataScheme(scheme);
}

ECode CIntentFilter::CountDataSchemes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataSchemes();
    return NOERROR;
}

ECode CIntentFilter::GetDataScheme(
    /* [in] */ Int32 index,
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme)
    *scheme = IntentFilter::GetDataScheme(index);
    return NOERROR;
}

ECode CIntentFilter::HasDataScheme(
    /* [in] */ const String& scheme,
    /* [out] */ Boolean* hasDataScheme)
{
    VALIDATE_NOT_NULL(hasDataScheme)
    *hasDataScheme = IntentFilter::HasDataScheme(scheme);
    return NOERROR;
}

ECode CIntentFilter::GetSchemes(
    /* [out, callee] */ ArrayOf<String>** schemes)
{
    VALIDATE_NOT_NULL(schemes)
    AutoPtr< ArrayOf<String> > schs = IntentFilter::GetSchemes();
    *schemes = schs;
    REFCOUNT_ADD(*schemes);
    return NOERROR;
}

ECode CIntentFilter::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    return IntentFilter::AddDataAuthority(host, port);
}

ECode CIntentFilter::CountDataAuthorities(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataAuthorities();
    return NOERROR;
}

ECode CIntentFilter::GetDataAuthority(
    /* [in] */ Int32 index,
    /* [out] */ IIntentFilterAuthorityEntry** authority)
{
    VALIDATE_NOT_NULL(authority)
    AutoPtr<IIntentFilterAuthorityEntry> tmp = IntentFilter::GetDataAuthority(index);
    *authority = tmp.Get();
    REFCOUNT_ADD(*authority)
    return NOERROR;
}

ECode CIntentFilter::HasDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Boolean* hasDataAuthority)
{
    VALIDATE_NOT_NULL(hasDataAuthority)
    *hasDataAuthority = IntentFilter::HasDataAuthority(data);
    return NOERROR;
}

ECode CIntentFilter::GetAuthorities(
    /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities)
{
    VALIDATE_NOT_NULL(authorities)
    AutoPtr< ArrayOf<IIntentFilterAuthorityEntry*> > auths = IntentFilter::GetAuthorities();
    *authorities = auths;
    REFCOUNT_ADD(*authorities)
    return NOERROR;
}

ECode CIntentFilter::AddDataPath(
    /* [in] */ const String& path,
    /* [in] */ Int32 type)
{
    return IntentFilter::AddDataPath(path, type);
}

ECode CIntentFilter::CountDataPaths(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountDataPaths();
    return NOERROR;
}

ECode CIntentFilter::GetDataPath(
    /* [in] */ Int32 index,
    /* [out] */ IPatternMatcher** path)
{
    VALIDATE_NOT_NULL(path)
    AutoPtr<IPatternMatcher> tmp = IntentFilter::GetDataPath(index);
    *path = tmp;
    REFCOUNT_ADD(*path)
    return NOERROR;
}

ECode CIntentFilter::HasDataPath(
    /* [in] */ const String& data,
    /* [out] */ Boolean* hasDataPath)
{
    VALIDATE_NOT_NULL(hasDataPath)
    *hasDataPath = IntentFilter::HasDataPath(data);
    return NOERROR;
}

ECode CIntentFilter::GetPaths(
    /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths)
{
    VALIDATE_NOT_NULL(paths)
    AutoPtr< ArrayOf<IPatternMatcher*> > pas = IntentFilter::GetPaths();
    *paths = pas;
    REFCOUNT_ADD(*paths)
    return NOERROR;
}

ECode CIntentFilter::MatchDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::MatchDataAuthority(data);
    return NOERROR;
}

ECode CIntentFilter::MatchData(
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::MatchData(type, scheme, data);
    return NOERROR;
}

ECode CIntentFilter::AddCategory(
    /* [in] */ const String& category)
{
    return IntentFilter::AddCategory(category);
}

ECode CIntentFilter::CountCategories(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = IntentFilter::CountCategories();
    return NOERROR;
}

ECode CIntentFilter::GetCategory(
    /* [in] */ Int32 index,
    /* [out] */ String* category)
{
    VALIDATE_NOT_NULL(category)
    *category = IntentFilter::GetCategory(index);
    return NOERROR;
}

ECode CIntentFilter::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* hasCategory)
{
    VALIDATE_NOT_NULL(hasCategory)
    *hasCategory = IntentFilter::HasCategory(category);
    return NOERROR;
}

ECode CIntentFilter::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    VALIDATE_NOT_NULL(categories)
    AutoPtr< ArrayOf<String> > cats = IntentFilter::GetCategories();
    *categories = cats;
    REFCOUNT_ADD(*categories);
    return NOERROR;
}

ECode CIntentFilter::MatchCategories(
    /* [in] */ ArrayOf<String>* categories,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = IntentFilter::MatchCategories(categories);
    return NOERROR;
}

ECode CIntentFilter::Match(
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

ECode CIntentFilter::Match(
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

ECode CIntentFilter::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    return IntentFilter::WriteToXml(serializer);
}

ECode CIntentFilter::ReadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    return IntentFilter::ReadFromXml(parser);
}

ECode CIntentFilter::Dump(
    /* [in] */ IPrinter* du,
    /* [in] */ const String& prefix)
{
    return IntentFilter::Dump(du, prefix);
}

ECode CIntentFilter::HasPartialTypes(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has)
    *has = mHasPartialTypes;
    return NOERROR;
}

ECode CIntentFilter::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return IntentFilter::ReadFromParcel(source);
}

ECode CIntentFilter::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return IntentFilter::WriteToParcel(dest);
}

ECode CIntentFilter::constructor()
{
    return IntentFilter::Init();
}

ECode CIntentFilter::constructor(
    /* [in] */ const String& action)
{
    return IntentFilter::Init(action);
}

ECode CIntentFilter::constructor(
    /* [in] */ const String& action,
    /* [in] */ const String& dataType)
{
    return IntentFilter::Init(action, dataType);
}

ECode CIntentFilter::constructor(
    /* [in] */ IIntentFilter* other)
{
    return IntentFilter::Init(other);
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
