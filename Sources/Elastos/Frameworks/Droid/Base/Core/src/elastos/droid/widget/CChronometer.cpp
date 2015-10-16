
#include "elastos/droid/widget/CChronometer.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CChronometer, Chronometer)

IDRAWABLECALLBACK_METHODS_IMPL(CChronometer, Chronometer)

IKEYEVENTCALLBACK_METHODS_IMPL(CChronometer, Chronometer)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CChronometer, Chronometer)

ITEXTVIEW_METHODS_IMPL(CChronometer, Chronometer)

PInterface CChronometer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CChronometer::Probe(riid);
}

ECode CChronometer::SetBase(
    /* [in] */ Int64 base)
{
    // TODO: Add your code here
    return Chronometer::SetBase(base);
}

ECode CChronometer::GetBase(
    /* [out] */ Int64 * pBase)
{
    // TODO: Add your code here
    *pBase =Chronometer::GetBase();
    return NOERROR;
}

ECode CChronometer::SetFormat(
    /* [in] */ const String& format)
{
    // TODO: Add your code here
    return Chronometer::SetFormat(format);
}

ECode CChronometer::GetFormat(
    /* [out] */ String * pFormat)
{
    // TODO: Add your code here
    *pFormat = Chronometer::GetFormat();
    return NOERROR;
}

ECode CChronometer::SetOnChronometerTickListener(
    /* [in] */ IOnChronometerTickListener * pListener)
{
    // TODO: Add your code here
    return Chronometer::SetOnChronometerTickListener(pListener);
}

ECode CChronometer::GetOnChronometerTickListener(
    /* [out] */ IOnChronometerTickListener ** ppListener)
{
    // TODO: Add your code here
    AutoPtr<IOnChronometerTickListener> temp;
    temp = Chronometer::GetOnChronometerTickListener();
    *ppListener = temp;
    REFCOUNT_ADD(*ppListener);
    return NOERROR;
}

ECode CChronometer::Start()
{
    // TODO: Add your code here
    return Chronometer::Start();
}

ECode CChronometer::Stop()
{
    // TODO: Add your code here
    return Chronometer::Stop();
}

ECode CChronometer::SetStarted(
    /* [in] */ Boolean started)
{

    return Chronometer::SetStarted(started);
}

ECode CChronometer::OnPreDraw(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    *pResult = Chronometer::OnPreDraw();
    return NOERROR;
}

ECode CChronometer::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return Chronometer::Init(pCtx);
}

ECode CChronometer::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return Chronometer::Init(pCtx, pAttrs);
}

ECode CChronometer::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return Chronometer::Init(pCtx, pAttrs, defStyle);
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
