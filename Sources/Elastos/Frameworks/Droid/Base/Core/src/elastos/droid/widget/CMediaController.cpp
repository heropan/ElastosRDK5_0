#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CMediaController.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CMediaController, MediaController)
IVIEWGROUP_METHODS_IMPL(CMediaController, MediaController)
IVIEWPARENT_METHODS_IMPL(CMediaController, MediaController)
IVIEWMANAGER_METHODS_IMPL(CMediaController, MediaController)
IDRAWABLECALLBACK_METHODS_IMPL(CMediaController, MediaController)
IKEYEVENTCALLBACK_METHODS_IMPL(CMediaController, MediaController)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CMediaController, MediaController)
IFRAMELAYOUT_METHODS_IMPL(CMediaController, MediaController)

PInterface CMediaController::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CMediaController::Probe(riid);
}

ECode CMediaController::constructor(
        /* [in] */ IContext* context)
{
    return MediaController::Init(context);
}

ECode CMediaController::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return MediaController::Init(context, attrs);
}

ECode CMediaController::constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForWard)
{
    return MediaController::Init(context, useFastForWard);
}

ECode CMediaController::SetMediaPlayer(
        /* [in] */ IMediaPlayerControl* player)
{
    return MediaController::SetMediaPlayer(player);
}

ECode CMediaController::SetAnchorView(
        /* [in] */ IView* view)
{
    return MediaController::SetAnchorView(view);
}

ECode CMediaController::Show()
{
    return MediaController::Show();
}

ECode CMediaController::Show(
        /* [in] */ Int32 timeout)
{
    return MediaController::Show(timeout);
}

ECode CMediaController::IsShowing(
        /* [out] */ Boolean* isShowing)
{
    VALIDATE_NOT_NULL(isShowing);
    *isShowing = MediaController::IsShowing();
    return NOERROR;
}

ECode CMediaController::Hide()
{
    return MediaController::Hide();
}

ECode CMediaController::SetPrevNextListeners(
        /* [in] */ IViewOnClickListener* next,
        /* [in] */ IViewOnClickListener* prev)
{
    return MediaController::SetPrevNextListeners(next, prev);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
