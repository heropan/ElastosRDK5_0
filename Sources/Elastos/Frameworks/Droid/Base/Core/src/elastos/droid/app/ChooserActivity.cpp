
#include "elastos/droid/app/ChooserActivity.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::EIID_IIntent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CAR_INTERFACE_IMPL(ChooserActivity, ResolverActivity, IChooserActivity)

ECode ChooserActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
    AutoPtr<IParcelable> targetParcelable;
    intent->GetParcelableExtra(IIntent::EXTRA_INTENT, (IParcelable**)&targetParcelable);
    if (!(targetParcelable->Probe(EIID_IIntent))) {
        Slogger::W("ChooseActivity", "Target is not an intent: %p", targetParcelable.Get());
        Finish();
        return NOERROR;
    }
    AutoPtr<IIntent> target = (IIntent*)targetParcelable->Probe(EIID_IIntent);
    AutoPtr<ICharSequence> title;
    intent->GetCharSequenceExtra(IIntent::EXTRA_TITLE, (ICharSequence**)&title);
    if (title == NULL) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->GetText(0/*com.android.internal.R.string.chooseActivity*/, (ICharSequence**)&title);
    }
    AutoPtr<ArrayOf<IParcelable*> > pa;
    intent->GetParcelableArrayExtra(IIntent::EXTRA_INITIAL_INTENTS, (ArrayOf<IParcelable*>**)&pa);
    AutoPtr<ArrayOf<IIntent*> > initialIntents;
    if (pa != NULL) {
        initialIntents = ArrayOf<IIntent*>::Alloc(pa->GetLength());
        for (Int32 i=0; i<pa->GetLength(); i++) {
            if (!((*pa)[i]->Probe(EIID_IIntent))) {
                Slogger::W("ChooseActivity", "Initial intent #%d not an Intent: %p", i,
                        (*pa)[i]);
                Finish();
                return NOERROR;
            }
            (*initialIntents)[i] = (IIntent*)(*pa)[i]->Probe(EIID_IIntent);
        }
    }
    ResolverActivity::OnCreate(savedInstanceState, target, title, initialIntents, NULL, FALSE);
    return NOERROR;
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
