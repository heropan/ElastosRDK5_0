
#include "elastos/droid/text/method/CTextKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

AutoPtr<ArrayOf<ITextKeyListener*> > CTextKeyListener::sInstance = ArrayOf<ITextKeyListener*>::Alloc(CAPITALIZELENGTH * 2);

IBaseKeyListener_METHODS_IMPL(CTextKeyListener, TextKeyListener, TextKeyListener)
IKeyListener_METHODS_IMPL(CTextKeyListener, TextKeyListener, TextKeyListener)

ECode CTextKeyListener::OnSpanAdded(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return TextKeyListener::OnSpanAdded(text, what, start, end);
}

ECode CTextKeyListener::OnSpanRemoved(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return TextKeyListener::OnSpanRemoved(text, what, start, end);
}

ECode CTextKeyListener::OnSpanChanged(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 ostart,
    /* [in] */ Int32 oend,
    /* [in] */ Int32 nstart,
    /* [in] */ Int32 nend)
{
    return TextKeyListener::OnSpanChanged(text, what, ostart, oend, nstart, nend);
}

ECode CTextKeyListener::ReleaseListener()
{
    return TextKeyListener::ReleaseListener();
}

ECode CTextKeyListener::constructor(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    Init(cap, autotext);
    _m_syncLock.mMutex = (TextKeyListener::mLock).mMutex;
    return NOERROR;
}

AutoPtr<ITextKeyListener> CTextKeyListener::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap)
{
    Int32 off = cap * 2 + (autotext ? 1 : 0);

    if ((*sInstance)[off] == NULL) {
        AutoPtr<ITextKeyListener> listener;
        CTextKeyListener::New(cap, autotext, (ITextKeyListener**)&listener);
        sInstance->Set(off, listener);
    }

    return (*sInstance)[off];
}

AutoPtr<ITextKeyListener> CTextKeyListener::GetInstance()
{
    return GetInstance(FALSE, Capitalize_NONE);
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
