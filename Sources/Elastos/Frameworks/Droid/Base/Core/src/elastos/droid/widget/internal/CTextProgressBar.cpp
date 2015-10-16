#include "elastos/droid/widget/internal/CTextProgressBar.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IRELATIVELAYOUT_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IVIEW_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IVIEWGROUP_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IVIEWPARENT_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IVIEWMANAGER_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IDRAWABLECALLBACK_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IKEYEVENTCALLBACK_METHODS_IMPL(CTextProgressBar, TextProgressBar)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CTextProgressBar, TextProgressBar)

CTextProgressBar::CTextProgressBar()
{}

PInterface CTextProgressBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTextProgressBar::Probe(riid);
}

ECode CTextProgressBar::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TextProgressBar::Init(context, attrs, defStyle);
}

ECode CTextProgressBar::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TextProgressBar::Init(context, attrs);
}

ECode CTextProgressBar::constructor(
    /* [in] */ IContext* context)
{
    return TextProgressBar::Init(context);
}

ECode CTextProgressBar::SetDurationBase(
    /* [in] */ Int64 durationBase)
{
    return TextProgressBar::SetDurationBase(durationBase);
}

ECode CTextProgressBar::OnChronometerTick(
    /* [in] */ IChronometer* chronometer)
{
    return TextProgressBar::OnChronometerTick(chronometer);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos