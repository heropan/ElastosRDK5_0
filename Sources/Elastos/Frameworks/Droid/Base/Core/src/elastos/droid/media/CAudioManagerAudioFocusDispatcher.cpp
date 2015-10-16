
#include "elastos/droid/media/CAudioManagerAudioFocusDispatcher.h"
#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/media/CAudioManager.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Utility::Logging::Logger;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Media {

ECode CAudioManagerAudioFocusDispatcher::constructor(
    /* [in] */ IAudioManager* owner)
{
    IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(owner);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mOwnerWeak);
    return NOERROR;
}

ECode CAudioManagerAudioFocusDispatcher::DispatchAudioFocusChange(
    /* [in] */ Int32 focusChange,
    /* [in] */ const String& id)
{
    AutoPtr<IAudioManager> am;
    mOwnerWeak->Resolve(EIID_IAudioManager, (IInterface**)&am);
    if (am == NULL)
        return NOERROR;

    AutoPtr<ICharSequence> seq;
    CString::New(id, (ICharSequence**)&seq);

    AutoPtr<IHandler> handler = ((CAudioManager*)am.Get())->mAudioFocusEventHandlerDelegate->GetHandler();
    AutoPtr<IMessage> msg;
    handler->ObtainMessage(focusChange, seq, (IMessage**)&msg);
    Boolean result;
    return handler->SendMessage(msg, &result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
