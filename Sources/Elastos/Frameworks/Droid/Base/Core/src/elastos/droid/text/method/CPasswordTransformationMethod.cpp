
#include "ext/frameworkext.h"
#include "elastos/droid/text/method/CPasswordTransformationMethod.h"
#include "elastos/droid/text/method/CTextKeyListener.h"
#include "elastos/droid/text/TextUtils.h"
#include "view/View.h"
#include "os/Handler.h"
#include "os/SystemClock.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::CString;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::Text::Style::EIID_IUpdateLayout;
using Elastos::Droid::Text::Style::EIID_IUpdateAppearance;
using Elastos::Droid::Text::Method::CTextKeyListener;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

AutoPtr<IPasswordTransformationMethod> CPasswordTransformationMethod::sInstance;
Char32 CPasswordTransformationMethod::DOT = 0x2022/*'\u2022'*/;

/*****************************CPasswordTransformationMethod::PasswordCharSequence*****************************/
PInterface CPasswordTransformationMethod::PasswordCharSequence::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IGetChars*)this;
    }
    else if (riid == EIID_IGetChars) {
        return (IGetChars*)this;
    }
    return NULL;
}

UInt32 CPasswordTransformationMethod::PasswordCharSequence::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPasswordTransformationMethod::PasswordCharSequence::Release()
{
    return ElRefBase::Release();
}

ECode CPasswordTransformationMethod::PasswordCharSequence::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IGetChars*)this) {
        *iID = EIID_IGetChars;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CPasswordTransformationMethod::PasswordCharSequence::GetLength(
    /* [out] */ Int32* number)
{
    mSource->GetLength(number);
    return NOERROR;
}

ECode CPasswordTransformationMethod::PasswordCharSequence::GetCharAt(
    /* [in] */ Int32 i,
    /* [out] */ Char32* c)
{
    AutoPtr<ISpanned> sp;
    if((sp = ISpanned::Probe(mSource), sp)!=NULL)
    {
        Int32 st;
        sp->GetSpanStart(CTextKeyListener::ACTIVE, &st);
        Int32 en;
        sp->GetSpanEnd(CTextKeyListener::ACTIVE, &en);

        if (i >= st && i < en) {
            mSource->GetCharAt(i, c);
            return NOERROR;
        }

        AutoPtr< ArrayOf< IPasswordTransformationMethodVisible* > > visible;
        Int32 spLen;
        sp->GetSpans(0, (sp->GetLength(&spLen), spLen), EIID_IPasswordTransformationMethodVisible, (ArrayOf< IInterface* >**)&visible);

        for (Int32 a = 0; a < visible->GetLength(); a++) {
            Int32 start;
            if ((sp->GetSpanStart( (((Visible*)(*visible)[a]))->mTransformer, &start), start) >= 0) {
                sp->GetSpanStart((*visible)[a], &st);
                sp->GetSpanEnd((*visible)[a], &en);

                if (i >= st && i < en) {
                    mSource->GetCharAt(i, c);
                    return NOERROR;
                }
            }
        }
    }
    *c=DOT;
    return NOERROR;
}

ECode CPasswordTransformationMethod::PasswordCharSequence::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    //Java:    char[] buf = new char[end - start]; getChars(start, end, buf, 0); return new String(buf);
    AutoPtr< ArrayOf<Char32> > buf = ArrayOf<Char32>::Alloc(end - start);
    GetChars(start, end, buf, 0);
    StringBuilder sb;
    sb.AppendChars(*buf);
    AutoPtr<ICharSequence> cs;
    CString::New(sb.ToString(), (ICharSequence**)&cs);
    *ret = cs;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CPasswordTransformationMethod::PasswordCharSequence::ToString(
    /* [out] */ String* str)
{
    //Java:return subSequence(0, length()).toString();
    Int32 len;
    AutoPtr<ICharSequence> cs;
    SubSequence(0, (GetLength(&len), len), (ICharSequence**)&cs);
    cs->ToString(str);
    return NOERROR;
}

ECode CPasswordTransformationMethod::PasswordCharSequence::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 off)
{
    TextUtils::GetChars(mSource, start, end, dest, off);

    Int32 st = -1, en = -1;
    Int32 nvisible = 0;
    AutoPtr< ArrayOf<Int32> > starts = NULL, ends = NULL;

    AutoPtr<ISpanned> sp;
    if((sp = ISpanned::Probe(mSource), sp)!=NULL){
        sp->GetSpanStart(CTextKeyListener::ACTIVE, &st);
        sp->GetSpanEnd(CTextKeyListener::ACTIVE, &en);

        AutoPtr< ArrayOf< IPasswordTransformationMethodVisible* > > visible;
        Int32 spLen;
        AutoPtr<ArrayOf<IInterface*> > arrayTemp;

        sp->GetSpans(0, (sp->GetLength(&spLen), spLen), EIID_IPasswordTransformationMethodVisible, (ArrayOf<IInterface*>**)&arrayTemp);
        visible = ArrayOf< IPasswordTransformationMethodVisible* >::Alloc(arrayTemp->GetLength());
        for (Int32 len = 0; len < arrayTemp->GetLength(); len++) {
            visible->Set(len, IPasswordTransformationMethodVisible::Probe((*arrayTemp)[len]));
        }

        nvisible = visible->GetLength();
        starts = ArrayOf<Int32>::Alloc(nvisible);
        ends = ArrayOf<Int32>::Alloc(nvisible);

        for (Int32 i = 0; i < nvisible; i++) {
            Int32 startP;
            sp->GetSpanStart(((Visible*)((*visible)[i]))->mTransformer, &startP);
            if (startP >= 0) {
                sp->GetSpanStart((IInterface*)(((*visible)[i])->Probe(EIID_IInterface)), &((*starts)[i]));
                sp->GetSpanEnd((IInterface*)(((*visible)[i]->Probe(EIID_IInterface))), &((*ends)[i]));
             }
        }
    }

    for (Int32 i = start; i < end; i++) {
        if (! (i >= st && i < en)) {
            Boolean visible = FALSE;

            for (Int32 a = 0; a < nvisible; a++) {
                if (i >= (*starts)[a] && i < (*ends)[a]) {
                    visible = TRUE;
                    break;
                }
            }

            if (!visible) {
                (*dest)[i - start + off] = DOT;
            }
        }
    }
    return NOERROR;
}

CPasswordTransformationMethod::PasswordCharSequence::PasswordCharSequence(
    /* [in] */ ICharSequence* source)
{
    mSource = source;
}

/*****************************CPasswordTransformationMethod::Visible*****************************/
IHANDLER_METHODS_IMPL(CPasswordTransformationMethod::Visible, Handler)

PInterface CPasswordTransformationMethod::Visible::Probe(
    /* [in] */ REIID riid)
{// IHandler, IUpdateLayout, IRunnable, IUpdateAppearance
    if (riid == EIID_IInterface) {
        return (IInterface*)(IHandler*)this;
    }
    else if (riid == EIID_IHandler) {
        return (IHandler*)this;
    }
    else if (riid == EIID_IUpdateLayout) {
        return (IUpdateLayout*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    else if (riid == EIID_IUpdateAppearance) {
        return (IUpdateAppearance*)this;
    }
    else if (riid == EIID_IPasswordTransformationMethodVisible) {
        return (IPasswordTransformationMethodVisible*)this;
    }
    return NULL;
}

UInt32 CPasswordTransformationMethod::Visible::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPasswordTransformationMethod::Visible::Release()
{
    return ElRefBase::Release();
}

ECode CPasswordTransformationMethod::Visible::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IHandler*)this) {
        *iID = EIID_IHandler;
    }
    else if (object == (IInterface*)(IUpdateLayout*)this) {
        *iID = EIID_IUpdateLayout;
    }
    else if (object == (IInterface*)(IRunnable*)this) {
        *iID = EIID_IRunnable;
    }
    else if (object == (IInterface*)(IUpdateAppearance*)this) {
        *iID = EIID_IUpdateAppearance;
    }
    else if (object == (IInterface*)(IPasswordTransformationMethodVisible*)this) {
        *iID = EIID_IPasswordTransformationMethodVisible;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CPasswordTransformationMethod::Visible::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

ECode CPasswordTransformationMethod::Visible::Run()
{
    mText->RemoveSpan(this->Probe(EIID_IInterface));
    return NOERROR;
}

CPasswordTransformationMethod::Visible::Visible(
    /* [in] */ ISpannable* sp,
    /* [in] */ IPasswordTransformationMethod* ptm)
    : Handler(FALSE)
    , mText(sp)
    , mTransformer(ptm)
{
    Boolean result;
    this->PostDelayed(this, 1500, &result);
}

/*****************************CPasswordTransformationMethod*****************************/
ECode CPasswordTransformationMethod::GetTransformation(
    /* [in] */ ICharSequence* source,
    /* [in] */ IView* view,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);

    AutoPtr<ISpannable> sp = ISpannable::Probe(source);
    if (sp != NULL){
        /*
         * Remove any references to other views that may still be
         * attached.  This will happen when you flip the screen
         * while a password field is showing; there will still
         * be references to the old EditText in the text.
         */
        //Java:    ViewReference[] vr = sp.getSpans(0, sp.length(), ViewReference.class);
        AutoPtr< ArrayOf< IView* > > vr;
        Int32 spLen;
        sp->GetSpans(0, (sp->GetLength(&spLen), spLen), EIID_IView, (ArrayOf< IInterface* >**)&vr);

        for (Int32 i = 0; i < vr->GetLength(); i++) {
            sp->RemoveSpan((*vr)[i]);
        }

        RemoveVisibleSpans(sp);

        sp->SetSpan(view, 0, 0, ISpannable::SPAN_POINT_POINT);
    }

    AutoPtr<ICharSequence> cs = (ICharSequence*)(IGetChars*)(new PasswordCharSequence(source));
    *ret = cs;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

AutoPtr<IPasswordTransformationMethod> CPasswordTransformationMethod::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CPasswordTransformationMethod> ptm;
        CPasswordTransformationMethod::NewByFriend((CPasswordTransformationMethod**)&ptm);
        sInstance = (IPasswordTransformationMethod*)(ptm.Get());
    }
    return sInstance;
}

ECode CPasswordTransformationMethod::BeforeTextChanged(
    /* [in] */ ICharSequence * s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    // This callback isn't used.
    return NOERROR;
}

ECode CPasswordTransformationMethod::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    AutoPtr<ISpannable> sp = ISpannable::Probe(s);
    if(sp!=NULL){
        //Java:    ViewReference[] vr = sp.getSpans(0, s.length(), ViewReference.class);
        AutoPtr< ArrayOf< IView* > > vr;
        Int32 spLen;
        sp->GetSpans(0, (sp->GetLength(&spLen), spLen), EIID_IView, (ArrayOf< IInterface* >**)&vr);
        if (vr->GetLength() == 0) {
            return NOERROR;
        }

        /*
         * There should generally only be one ViewReference in the text,
         * but make sure to look through all of them if necessary in case
         * something strange is going on.  (We might still end up with
         * multiple ViewReferences if someone moves text from one password
         * field to another.)
         */
        AutoPtr<IView> v = NULL;
        for (Int32 i = 0; v == NULL && i < vr->GetLength(); i++) {
            v = ((*vr)[i]);
        }

        if (v == NULL) {
            return NOERROR;
        }

        AutoPtr<IContext> context;
        Int32 pref;
        pref = ((CTextKeyListener*)((CTextKeyListener::GetInstance()).Get()))->GetPrefs((v->GetContext((IContext**)&context), context));
        if ((pref & CTextKeyListener::SHOW_PASSWORD) != 0) {
            if (count > 0) {
                RemoveVisibleSpans(sp);

                if (count == 1) {
                    AutoPtr<IPasswordTransformationMethod> tpm = (IPasswordTransformationMethod*)this->Probe(EIID_IPasswordTransformationMethod);
                    AutoPtr<IPasswordTransformationMethodVisible> visible = new Visible(sp, tpm);

                    sp->SetSpan((IInterface*)( visible->Probe(EIID_IInterface)), start, start + count, ISpannable::SPAN_EXCLUSIVE_EXCLUSIVE);
                }
            }
        }
    }
    return NOERROR;
}

ECode CPasswordTransformationMethod::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    // This callback isn't used.
    return NOERROR;
}

ECode CPasswordTransformationMethod::OnFocusChanged(
    /* [in] */ IView* view,
    /* [in] */ ICharSequence* sourceText,
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (!focused) {
        AutoPtr<ISpannable> sp = ISpannable::Probe(sourceText);
        if(sp!=NULL){
            RemoveVisibleSpans(sp);
        }
    }
    return NOERROR;
}

void CPasswordTransformationMethod::RemoveVisibleSpans(
    /* [in] */ ISpannable* sp)
{
    //Java:    Visible[] old = sp.getSpans(0, sp.length(), Visible.class);
    AutoPtr< ArrayOf< IPasswordTransformationMethodVisible* > > old;
    Int32 spLen;
    sp->GetSpans(0, (sp->GetLength(&spLen), spLen), EIID_IPasswordTransformationMethodVisible, (ArrayOf< IInterface* >**)&old);
    for (Int32 i = 0; i < old->GetLength(); i++) {
        sp->RemoveSpan((*old)[i]);
    }
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

