
#include "elastos/droid/text/method/CMultiTapKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Int32 CMultiTapKeyListener::CAPITALIZELENGTH;
AutoPtr< IMultiTapKeyListener > CMultiTapKeyListener::sInstance[CAPITALIZELENGTH*2];

IBaseKeyListener_METHODS_IMPL(CMultiTapKeyListener, MultiTapKeyListener, MultiTapKeyListener)
IKeyListener_METHODS_IMPL(CMultiTapKeyListener, MultiTapKeyListener, MultiTapKeyListener)

ECode CMultiTapKeyListener::OnSpanAdded(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    MultiTapKeyListener::OnSpanAdded(text, what, start, end);
    return NOERROR;
}

ECode CMultiTapKeyListener::OnSpanRemoved(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    MultiTapKeyListener::OnSpanRemoved(text, what, start, end);
    return NOERROR;
}

ECode CMultiTapKeyListener::OnSpanChanged(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 ostart,
    /* [in] */ Int32 oend,
    /* [in] */ Int32 nstart,
    /* [in] */ Int32 nend)
{
    MultiTapKeyListener::OnSpanChanged(text, what, ostart, oend, nstart, nend);
    return NOERROR;
}

ECode CMultiTapKeyListener::constructor(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    Init(cap, autotext);
    return NOERROR;
}

AutoPtr<IMultiTapKeyListener> CMultiTapKeyListener::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap)
{
    //Java:    int off = cap.ordinal() * 2 + (autotext ? 1 : 0);
    Int32 off = cap * 2 + (autotext ? 1 : 0);

    if (sInstance[off] == NULL) {
        AutoPtr<CMultiTapKeyListener> mtkl;
        CMultiTapKeyListener::NewByFriend(cap, autotext, (CMultiTapKeyListener**)&mtkl);
        sInstance[off] = (IMultiTapKeyListener*)(mtkl.Get());
    }

    return sInstance[off];
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

