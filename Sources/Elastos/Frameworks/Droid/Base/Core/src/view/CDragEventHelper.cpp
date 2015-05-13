#include "ext/frameworkext.h"
#include "view/CDragEventHelper.h"
#include "view/CDragEvent.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CDragEventHelper::Obtain(
    /* [out] */ IDragEvent** event)
{
    CDragEvent* ev;
    FAIL_RETURN(CDragEvent::Obtain(&ev));
    *event = (IDragEvent*)ev;
    return NOERROR;
}

ECode CDragEventHelper::ObtainEx(
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IInterface* localState,
    /* [in] */ IClipDescription* description,
    /* [in] */ IClipData* data,
    /* [in] */ Boolean result,
    /* [out] */ IDragEvent** event)
{
    AutoPtr<IDragEvent> ev;
    FAIL_RETURN(CDragEvent::ObtainEx(
        action, x, y, localState, description,
        data, result, (CDragEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CDragEventHelper::ObtainEx2(
    /* [in] */ IDragEvent* source,
    /* [out] */ IDragEvent** event)
{
    AutoPtr<IDragEvent> ev;
    FAIL_RETURN(CDragEvent::ObtainEx2((CDragEvent*)source, (CDragEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

} //view
} //Droid
} //Elastos

