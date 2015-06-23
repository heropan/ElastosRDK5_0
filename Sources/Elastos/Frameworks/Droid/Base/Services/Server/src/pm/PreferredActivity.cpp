
#include "pm/PreferredActivity.h"
#include "pm/CPackageManagerService.h"
#include "util/XmlUtils.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

using Elastos::Droid::Utility::XmlUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

const String PreferredActivity::ATTR_USER_ID = String("userId");
const String PreferredActivity::TAG = String("PreferredActivity");

const Boolean PreferredActivity::DEBUG_FILTERS;

PreferredActivity::PreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity)
    : IntentFilter(filter)
{
    mPref = new PreferredComponent(this, match, set, activity);
}

PreferredActivity::PreferredActivity(
    /* [in] */ IXmlPullParser* parser)
{
    IntentFilter::Init();
    mPref = new PreferredComponent(this, parser);
}

PInterface PreferredActivity::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IIntentFilter*)this;
    }
    else if (riid == Elastos::Droid::Content::EIID_IIntentFilter) {
        return (IIntentFilter*)this;
    }

    return NULL;
}

UInt32 PreferredActivity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PreferredActivity::Release()
{
    return ElRefBase::Release();
}

ECode PreferredActivity::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode PreferredActivity::SetPriority(
    /* [in] */ Int32 priority)
{
    return IntentFilter::SetPriority(priority);
}

ECode PreferredActivity::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);

    *priority = IntentFilter::GetPriority();
    return NOERROR;
}

ECode PreferredActivity::AddAction(
    /* [in] */ const String& action)
{
    return IntentFilter::AddAction(action);
}

ECode PreferredActivity::CountActions(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountActions();
    return NOERROR;
}

ECode PreferredActivity::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String* action)
{
    VALIDATE_NOT_NULL(action);

    *action = IntentFilter::GetAction(index);
    return NOERROR;
}

ECode PreferredActivity::HasAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* hasAction)
{
    VALIDATE_NOT_NULL(hasAction);

    *hasAction = IntentFilter::HasAction(action);
    return NOERROR;
}

ECode PreferredActivity::MatchAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* isMatched)
{
    VALIDATE_NOT_NULL(isMatched);

    *isMatched = IntentFilter::MatchAction(action);
    return NOERROR;
}

ECode PreferredActivity::GetActions(
    /* [out, callee] */ ArrayOf<String>** actions)
{
    VALIDATE_NOT_NULL(actions);

    AutoPtr< ArrayOf<String> > acts = IntentFilter::GetActions();
    *actions = acts;
    INTERFACE_ADDREF(*actions);
    return NOERROR;
}

ECode PreferredActivity::AddDataType(
    /* [in] */ const String& type)
{
    return IntentFilter::AddDataType(type);
}

ECode PreferredActivity::HasDataType(
    /* [in] */ const String& type,
    /* [out] */ Boolean* hasDataType)
{
    VALIDATE_NOT_NULL(hasDataType);

    *hasDataType = IntentFilter::HasDataType(type);
    return NOERROR;
}

ECode PreferredActivity::CountDataTypes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataTypes();
    return NOERROR;
}

ECode PreferredActivity::GetDataType(
    /* [in] */ Int32 index,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);

    *type = IntentFilter::GetDataType(index);
    return NOERROR;
}

ECode PreferredActivity::GetTypes(
    /* [out, callee] */ ArrayOf<String>** types)
{
    VALIDATE_NOT_NULL(types);

    AutoPtr< ArrayOf<String> > typs = IntentFilter::GetTypes();
    *types = typs;
    INTERFACE_ADDREF(*types);
    return NOERROR;
}

ECode PreferredActivity::AddDataScheme(
    /* [in] */ const String& scheme)
{
    return IntentFilter::AddDataScheme(scheme);
}

ECode PreferredActivity::CountDataSchemes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataSchemes();
    return NOERROR;
}

ECode PreferredActivity::GetDataScheme(
    /* [in] */ Int32 index,
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);

    *scheme = IntentFilter::GetDataScheme(index);
    return NOERROR;
}

ECode PreferredActivity::HasDataScheme(
    /* [in] */ const String& scheme,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataScheme(scheme);
    return NOERROR;
}

ECode PreferredActivity::GetSchemes(
    /* [out, callee] */ ArrayOf<String>** schemes)
{
    VALIDATE_NOT_NULL(schemes);

    AutoPtr< ArrayOf<String> > schs = IntentFilter::GetSchemes();
    *schemes = schs;
    INTERFACE_ADDREF(*schemes);
    return NOERROR;
}

ECode PreferredActivity::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    return IntentFilter::AddDataAuthority(host, port);
}

ECode PreferredActivity::CountDataAuthorities(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataAuthorities();
    return NOERROR;
}

ECode PreferredActivity::GetDataAuthority(
    /* [in] */ Int32 index,
    /* [out] */ IIntentFilterAuthorityEntry** authority)
{
    VALIDATE_NOT_NULL(authority);

    AutoPtr<IIntentFilterAuthorityEntry> tmp = IntentFilter::GetDataAuthority(index);
    *authority = tmp.Get();
    INTERFACE_ADDREF(*authority);
    return NOERROR;
}

ECode PreferredActivity::HasDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataAuthority(data);
    return NOERROR;
}

ECode PreferredActivity::GetAuthorities(
    /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities)
{
    VALIDATE_NOT_NULL(authorities)

    AutoPtr< ArrayOf<IIntentFilterAuthorityEntry*> > auths = IntentFilter::GetAuthorities();
    *authorities = auths;
    INTERFACE_ADDREF(*authorities);
    return NOERROR;
}

ECode PreferredActivity::AddDataPath(
    /* [in] */ const String& path,
    /* [in] */ Int32 type)
{
    return IntentFilter::AddDataPath(path, type);
}

ECode PreferredActivity::CountDataPaths(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataPaths();
    return NOERROR;
}

ECode PreferredActivity::GetDataPath(
    /* [in] */ Int32 index,
    /* [out] */ IPatternMatcher** path)
{
    VALIDATE_NOT_NULL(path);

    AutoPtr<IPatternMatcher> tmp = IntentFilter::GetDataPath(index);
    *path = tmp;
    INTERFACE_ADDREF(*path);
    return NOERROR;
}

ECode PreferredActivity::HasDataPath(
    /* [in] */ const String& data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataPath(data);
    return NOERROR;
}

ECode PreferredActivity::GetPaths(
    /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths)
{
    VALIDATE_NOT_NULL(paths)

    AutoPtr< ArrayOf<IPatternMatcher*> > pas = IntentFilter::GetPaths();
    *paths = pas;
    INTERFACE_ADDREF(*paths);
    return NOERROR;
}

ECode PreferredActivity::MatchDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchDataAuthority(data);
    return NOERROR;
}

ECode PreferredActivity::MatchData(
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchData(type, scheme, data);
    return NOERROR;
}

ECode PreferredActivity::AddCategory(
    /* [in] */ const String& category)
{
    return IntentFilter::AddCategory(category);
}

ECode PreferredActivity::CountCategories(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountCategories();
    return NOERROR;
}

ECode PreferredActivity::GetCategory(
    /* [in] */ Int32 index,
    /* [out] */ String* category)
{
    VALIDATE_NOT_NULL(category);

    *category = IntentFilter::GetCategory(index);
    return NOERROR;
}

ECode PreferredActivity::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* hasCategory)
{
    VALIDATE_NOT_NULL(hasCategory);

    *hasCategory = IntentFilter::HasCategory(category);
    return NOERROR;
}

ECode PreferredActivity::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    VALIDATE_NOT_NULL(categories)

    AutoPtr< ArrayOf<String> > cats = IntentFilter::GetCategories();
    *categories = cats;
    INTERFACE_ADDREF(*categories);
    return NOERROR;
}

ECode PreferredActivity::MatchCategories(
    /* [in] */ ArrayOf<String>* categories,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchCategories(categories);
    return NOERROR;
}

ECode PreferredActivity::Match(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean resolve,
    /* [in] */ const String& logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::Match(resolver, intent, resolve, logTag);
    return NOERROR;
}

ECode PreferredActivity::Match(
    /* [in] */ const String& action,
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ const String& logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::Match(action, type, scheme, data, categories, logTag);
    return NOERROR;
}

ECode PreferredActivity::ReadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    return IntentFilter::ReadFromXml(parser);
}

ECode PreferredActivity::Dump(
    /* [in] */ IPrinter* du,
    /* [in] */ const String& prefix)
{
    return IntentFilter::Dump(du, prefix);
}

ECode PreferredActivity::GetHasPartialTypes(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has)
    *has = mHasPartialTypes;
    return NOERROR;
}

ECode PreferredActivity::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    FAIL_RETURN(mPref->WriteToXml(serializer));
    FAIL_RETURN(serializer->WriteStartTag(String(NULL), String("filter")));
    FAIL_RETURN(IntentFilter::WriteToXml(serializer));
    return serializer->WriteEndTag(String(NULL), String("filter"));
}

ECode PreferredActivity::OnReadTag(
    /* [in] */ const String& tagName,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    if (tagName.Equals("filter")) {
        if (DEBUG_FILTERS) {
            Slogger::I(TAG, "Starting to parse filter...");
        }
        FAIL_RETURN(IntentFilter::ReadFromXml(parser));
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
        CPackageManagerService::ReportSettingsProblem(5 /*TODO: Log.WARN*/,
                String("Unknown element under <preferred-activities>: ") + name);
        XmlUtils::SkipCurrentTag(parser);
    }
    *result = TRUE;
    return NOERROR;
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
