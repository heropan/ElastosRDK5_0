
#ifndef __ELASTOS_DROID_PREFERENCE_PREFERENCEINFLATER_H__
#define __ELASTOS_DROID_PREFERENCE_PREFERENCEINFLATER_H__

#include "GenericInflater.h"

namespace Elastos {
namespace Droid {
namespace Preference {

class PreferenceInflater : public GenericInflater
{
public:
    PreferenceInflater(
        /* [in] */ IContext* context,
        /* [in] */ IPreferenceManager* preferenceManager);

    PreferenceInflater(
        /* [in] */ GenericInflater* original,
        /* [in] */ IPreferenceManager* preferenceManager,
        /* [in] */ IContext* newContext);

    CARAPI_(AutoPtr<GenericInflater>) CloneInContext(
        /* [in] */ IContext* newContext);

protected:
    CARAPI OnCreateCustomFromTag(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IInterface* parentPreference,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ Boolean* res);

    CARAPI OnMergeRoots(
        /* [in] */ IInterface* givenRoot,
        /* [in] */ Boolean attachToGivenRoot,
        /* [in] */ IInterface* xmlRoot,
        /* [out] */ IInterface** p);

private:
    CARAPI_(void) Init(
        /* [in] */ IPreferenceManager* preferenceManager);

private:
    static const String TAG;
    static const String INTENT_TAG_NAME;
    static const String EXTRA_TAG_NAME;

    AutoPtr<IPreferenceManager> mPreferenceManager;
};

} // Preference
} // Droid
} // Elastos

#endif // __CPREFERENCEINFLATER_H__
