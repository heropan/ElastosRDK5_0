
#include "PreferenceInflater.h"
#include "content/Intent.h"
#include "utility/XmlUtils.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::Intent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Utility::XmlUtils;

namespace Elastos {
namespace Droid {
namespace Preference {

const String PreferenceInflater::TAG("PreferenceInflater");
const String PreferenceInflater::INTENT_TAG_NAME("intent");
const String PreferenceInflater::EXTRA_TAG_NAME("extra");

PreferenceInflater::PreferenceInflater(
    /* [in] */ IContext* context,
    /* [in] */ IPreferenceManager* preferenceManager)
    : GenericInflater(context)
{
    Init(preferenceManager);
}

PreferenceInflater::PreferenceInflater(
    /* [in] */ GenericInflater* original,
    /* [in] */ IPreferenceManager* preferenceManager,
    /* [in] */ IContext* newContext)
    : GenericInflater(original, newContext)
{
   Init(preferenceManager);
}

AutoPtr<GenericInflater> PreferenceInflater::CloneInContext(
    /* [in] */ IContext* newContext)
{
    return new PreferenceInflater(this, mPreferenceManager, newContext);
}

void PreferenceInflater::Init(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    mPreferenceManager = preferenceManager;
    SetDefaultPackage(String("Elastos.Droid.Preference."));
}

ECode PreferenceInflater::OnCreateCustomFromTag(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IInterface* _parentPreference,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;

    AutoPtr<IPreference> parentPreference = IPreference::Probe(_parentPreference);
    assert(parentPreference != NULL);
    String tag;
    parser->GetName(&tag);
    if (tag.Equals(INTENT_TAG_NAME)) {
        AutoPtr<IIntent> intent;

        AutoPtr<IContext> ctx = GetContext();
        AutoPtr<IResources> resource;
        ctx->GetResources((IResources**)&resource);
        if (Intent::ParseIntent(resource, parser, attrs, (IIntent**)&intent) == (ECode)E_IO_EXCEPTION) {
            Slogger::D("PreferenceInflater", "Error parsing preference");
            return E_XML_PULL_PARSER_EXCEPTION;
        }

        if (intent != NULL) {
            parentPreference->SetIntent(intent);
        }

        *res = TRUE;
        return NOERROR;
    }
    else if (tag.Equals(EXTRA_TAG_NAME)) {
        AutoPtr<IContext> ctx = GetContext();
        AutoPtr<IResources> resource;
        ctx->GetResources((IResources**)&resource);
        AutoPtr<IBundle> data;
        parentPreference->GetExtras((IBundle**)&data);
        resource->ParseBundleExtra(EXTRA_TAG_NAME, attrs, data);
        if (XmlUtils::SkipCurrentTag(parser) == (ECode)E_IO_EXCEPTION) {
            Slogger::D("PreferenceInflater", "Error parsing preference");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        *res = TRUE;
        return NOERROR;
    }

    return NOERROR;
}

ECode PreferenceInflater::OnMergeRoots(
    /* [in] */ IInterface* givenRoot,
    /* [in] */ Boolean attachToGivenRoot,
    /* [in] */ IInterface* xmlRoot,
    /* [out] */ IInterface** p)
{
    VALIDATE_NOT_NULL(p)
    // If we were given a Preferences, use it as the root (ignoring the root
    // Preferences from the XML file).
    if (givenRoot == NULL) {
        assert(IPreferenceGroup::Probe(xmlRoot) != NULL);
        IPreference::Probe(xmlRoot)->OnAttachedToHierarchy(mPreferenceManager);
        *p = xmlRoot;
    }
    else {
        *p = givenRoot;
    }

    REFCOUNT_ADD(*p)
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
