
#include "PreferredComponent.h"
#include "util/XmlUtils.h"
#include <elastos/StringUtils.h>
#include <elastos/Slogger.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Content::IComponentNameHelper;
using Elastos::Droid::Content::CComponentNameHelper;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {

PreferredComponent::PreferredComponent(
    /* [in] */ ICallbacks* callbacks,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* component)
{
    mCallbacks = callbacks;
    mMatch = match & IIntentFilter::MATCH_CATEGORY_MASK;
    mComponent = component;
    component->FlattenToShortString(&mShortComponent);
    if (set != NULL) {
        const Int32 N = set->GetLength();
        AutoPtr<ArrayOf<String> > myPackages = ArrayOf<String>::Alloc(N);
        AutoPtr<ArrayOf<String> > myClasses = ArrayOf<String>::Alloc(N);
        AutoPtr<ArrayOf<String> > myComponents = ArrayOf<String>::Alloc(N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IComponentName> cn = (*set)[i];
            if (cn == NULL) {
                mSetPackages = NULL;
                mSetClasses = NULL;
                mSetComponents = NULL;
                return;
            }
            String packageName, className, shortStr;
            cn->GetPackageName(&packageName);
            cn->GetClassName(&className);
            cn->FlattenToShortString(&shortStr);
            (*myPackages)[i] = packageName;
            (*myClasses)[i] = className;
            (*myComponents)[i] = shortStr;
        }
        mSetPackages = myPackages;
        mSetClasses = myClasses;
        mSetComponents = myComponents;
    }
}

PreferredComponent::PreferredComponent(
    /* [in] */ ICallbacks* callbacks,
    /* [in] */ IXmlPullParser* parser)
{
    mCallbacks = callbacks;
    parser->GetAttributeValueEx(String(NULL), String("name"), &mShortComponent);
    AutoPtr<IComponentNameHelper> componentNameHelper;
    CComponentNameHelper::AcquireSingleton((IComponentNameHelper**)&componentNameHelper);
    componentNameHelper->UnflattenFromString(mShortComponent, (IComponentName**)&mComponent);
    if (mComponent == NULL) {
        mParseError = String("Bad activity name ") + mShortComponent;
    }
    String matchStr;
    parser->GetAttributeValueEx(String(NULL), String("match"), &matchStr);
    mMatch = matchStr != NULL ? StringUtils::ParseInt32(matchStr, 16) : 0;
    String setCountStr;
    parser->GetAttributeValueEx(String(NULL), String("set"), &setCountStr);
    Int32 setCount = setCountStr != NULL ? StringUtils::ParseInt32(setCountStr) : 0;

    AutoPtr<ArrayOf<String> > myPackages = setCount > 0 ? ArrayOf<String>::Alloc(setCount) : NULL;
    AutoPtr<ArrayOf<String> > myClasses = setCount > 0 ? ArrayOf<String>::Alloc(setCount) : NULL;
    AutoPtr<ArrayOf<String> > myComponents = setCount > 0 ? ArrayOf<String>::Alloc(setCount) : NULL;

    Int32 setPos = 0;

    Int32 outerDepth, depth;
    parser->GetDepth(&outerDepth);
    Int32 type;
    Boolean res;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
        && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG
            || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        //Log.i(TAG, "Parse outerDepth=" + outerDepth + " depth="
        //        + parser.getDepth() + " tag=" + tagName);
        if (tagName.Equals("set")) {
            String name;
            parser->GetAttributeValueEx(String(NULL), String("name"), &name);
            if (name.IsNull()) {
                if (mParseError.IsNull()) {
                    mParseError = String("No name in set tag in preferred activity ")
                        + mShortComponent;
                }
            }
            else if (setPos >= setCount) {
                if (mParseError.IsNull()) {
                    mParseError = String("Too many set tags in preferred activity ")
                        + mShortComponent;
                }
            }
            else {
                AutoPtr<IComponentName> cn;
                componentNameHelper->UnflattenFromString(name, (IComponentName**)&cn);
                if (cn == NULL) {
                    if (mParseError.IsNull()) {
                        mParseError = String("Bad set name ") + name + " in preferred activity "
                            + mShortComponent;
                    }
                }
                else {
                    String packageName, className;
                    cn->GetPackageName(&packageName);
                    cn->GetClassName(&className);
                    (*myPackages)[setPos] = packageName;
                    (*myClasses)[setPos] = className;
                    (*myComponents)[setPos] = name;
                    setPos++;
                }
            }
            XmlUtils::SkipCurrentTag(parser);
        }
        else if (!(mCallbacks->OnReadTag(tagName, parser, &res), res)) {
            String name;
            parser->GetName(&name);
            Slogger::W("PreferredComponent", String("Unknown element: ") + name);
            XmlUtils::SkipCurrentTag(parser);
        }
    }

    if (setPos != setCount) {
        if (mParseError.IsNull()) {
            mParseError.AppendFormat("Not enough set tags (expected %d"
                " but found %d ) in %s", setCount, setPos, mShortComponent.string());
        }
    }

    mSetPackages = myPackages;
    mSetClasses = myClasses;
    mSetComponents = myComponents;
}

String PreferredComponent::GetParseError()
{
    return mParseError;
}

ECode PreferredComponent::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    assert(serializer != NULL);
    String nullStr;
    String setStr("set");
    String nameStr("name");
    Int32 NS = mSetClasses != NULL ? mSetClasses->GetLength() : 0;
    serializer->WriteAttribute(nullStr, nameStr, mShortComponent);
    if (mMatch != 0) {
        serializer->WriteAttribute(nullStr, String("match"), StringUtils::Int32ToHexString(mMatch));
    }
    serializer->WriteAttribute(nullStr, setStr, StringUtils::Int32ToString(NS));
    for (Int32 s = 0; s < NS; s++) {
       serializer->WriteStartTag(nullStr, setStr);
       serializer->WriteAttribute(nullStr, nameStr, (*mSetComponents)[s]);
       serializer->WriteEndTag(nullStr, setStr);
    }
    return NOERROR;
}

Boolean PreferredComponent::SameSet(
    /* [in] */ IObjectContainer* query,
    /* [in] */ Int32 priority)
{
    if (mSetPackages == NULL)
        return FALSE;
    const Int32 NS = mSetPackages->GetLength();
    Int32 numMatch = 0;
    Boolean hasNext = FALSE;
    AutoPtr<IObjectEnumerator> enumerator;
    query->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> item;
        enumerator->Current((IInterface**)&item);

        AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(item);
        Int32 riPriority;
        ri->GetPriority(&riPriority);
        if (riPriority != priority)
            continue;
        AutoPtr<IActivityInfo> ai;
        ri->GetActivityInfo((IActivityInfo**)&ai);
        String packageName, name;
        ai->GetPackageName(&packageName);
        ai->GetName(&name);
        Boolean good = FALSE;
        for (Int32 j = 0; j < NS; j++) {
            if ((*mSetPackages)[j].Equals(packageName)
                && (*mSetClasses)[j].Equals(name)) {
                numMatch++;
                good = TRUE;
                break;
            }
        }
        if (!good)
            return FALSE;
    }

    return numMatch == NS;
}

void PreferredComponent::Dump(
    /* [in] */ IPrintWriter* out,
    /* [in] */ const String& prefix,
    /* [in] */ IInterface* ident)
{
    out->PrintString(prefix);
    out->PrintString(StringUtils::Int32ToString((Int32)ident/*System.identityHashCode(ident)*/, 16));
    out->PrintChar(' ');
    String str;
    mComponent->FlattenToShortString(&str);
    out->PrintString(str);
    out->PrintString(String(" match=0x"));
    out->PrintStringln(StringUtils::Int32ToString(mMatch, 16));
    if (mSetComponents != NULL) {
        out->PrintString(prefix);
        out->PrintStringln(String("  Selected from:"));
        Int32 length = mSetComponents->GetLength();
        for (Int32 i = 0; i < length; i++) {
            out->PrintString(prefix);
            out->PrintString(String("    "));
            out->PrintStringln((*mSetComponents)[i]);
        }
    }
}

} // namespace Server
} // namespace Droid
} // namespace Elastos
