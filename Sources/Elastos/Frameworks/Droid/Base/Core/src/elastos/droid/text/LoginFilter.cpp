#include "elastos/droid/text/LoginFilter.h"
#include "elastos/droid/text/CSpannableStringBuilder.h"

namespace Elastos {
namespace Droid {
namespace Text {

/*****************************LoginFilter*****************************/
PInterface LoginFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ILoginFilter*)this;
    }
    else if (riid == EIID_ILoginFilter) {
        return (ILoginFilter*)this;
    }
    else if (riid == EIID_IInputFilter) {
        return (IInputFilter*)this;
    }
    return NULL;
}

UInt32 LoginFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LoginFilter::Release()
{
    return ElRefBase::Release();
}

ECode LoginFilter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    if (IID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(ILoginFilter*)this) {
        *IID = EIID_ILoginFilter;
    }
    else if (object == (IInterface*)(IInputFilter*)this) {
        *IID = EIID_IInputFilter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

LoginFilter::LoginFilter(
    /* [in] */ Boolean appendInvalid)
{
    mAppendInvalid = appendInvalid;
}

LoginFilter::LoginFilter()
{
    mAppendInvalid = FALSE;
}

ECode LoginFilter::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /* [out] */ ICharSequence** ret)
{
    OnStart();

    // Scan through beginning characters in dest, calling onInvalidCharacter()
    // for each invalid character.
    for (Int32 i = 0; i < dstart; i++) {
        Char32 c;
        dest->GetCharAt(i, &c);
        Boolean bAllowed;
        if (!(IsAllowed(c, &bAllowed), bAllowed)) {
            OnInvalidCharacter(c);
        }
    }

    // Scan through changed characters rejecting disallowed chars
    AutoPtr<ISpannableStringBuilder> modification;
    Int32 modoff = 0;

    for (Int32 i = start; i < end; i++) {
        Char32 c;
        source->GetCharAt(i, &c);
        Boolean bAllowed;
        if ((IsAllowed(c, &bAllowed), bAllowed)) {
            // Character allowed.
            modoff++;
        }
        else {
            if (mAppendInvalid) {
                modoff++;
            }
            else {
                if (modification == NULL) {
                    CSpannableStringBuilder::New(source, start, end, (ISpannableStringBuilder**)&modification);
                    modoff = i - start;
                }

                modification->Delete(modoff, modoff + 1);
            }

            OnInvalidCharacter(c);
        }
    }

    // Scan through remaining characters in dest, calling onInvalidCharacter()
    // for each invalid character.
    Int32 cLen;
    for (Int32 i = dend; i < (dest->GetLength(&cLen), cLen); i++) {
        Char32 c;
        dest->GetCharAt(i, &c);
        Boolean bAllowed;
        if (!(IsAllowed(c, &bAllowed), bAllowed)) {
            OnInvalidCharacter(c);
        }
    }

    OnStop();

    // Either returns null if we made no changes,
    // or what we wanted to change it to if there were changes.
    *ret = modification;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode LoginFilter::OnStart()
{
    return NOERROR;
}

ECode LoginFilter::OnInvalidCharacter(
    /* [in] */ Char32 c)
{
    return NOERROR;
}

ECode LoginFilter::OnStop()
{
    return NOERROR;
}

/*****************************UsernameFilterGMail*****************************/
UsernameFilterGMail::UsernameFilterGMail() : LoginFilter(FALSE)
{
}

UsernameFilterGMail::UsernameFilterGMail(
    /* [in] */ Boolean appendInvalid) : LoginFilter(appendInvalid)
{
}

ECode UsernameFilterGMail::IsAllowed(
    /* [in] */ Char32 c,
    /* [out] */ Boolean* ret)
{
    // Allow [a-zA-Z0-9@.]
    if ('0' <= c && c <= '9') {
        *ret = TRUE;
        return NOERROR;
    }
    if ('a' <= c && c <= 'z') {
        *ret = TRUE;
        return NOERROR;
    }
    if ('A' <= c && c <= 'Z') {
        *ret = TRUE;
        return NOERROR;
    }
    if ('.' == c) {
        *ret = TRUE;
        return NOERROR;
    }
    *ret = FALSE;
    return NOERROR;
}

/*****************************UsernameFilterGeneric*****************************/
CString UsernameFilterGeneric::mAllowed = "@_-+."; // Additional characters

UsernameFilterGeneric::UsernameFilterGeneric() : LoginFilter(FALSE)
{
}

UsernameFilterGeneric::UsernameFilterGeneric(
    /* [in] */ Boolean appendInvalid) : LoginFilter(appendInvalid)
{
}

ECode UsernameFilterGeneric::IsAllowed(
    /* [in] */ Char32 c,
    /* [out] */ Boolean* ret)
{
    // Allow [a-zA-Z0-9@.]
    if ('0' <= c && c <= '9') {
        *ret = TRUE;
        return NOERROR;
    }
    if ('a' <= c && c <= 'z') {
        *ret = TRUE;
        return NOERROR;
    }
    if ('A' <= c && c <= 'Z') {
        *ret = TRUE;
        return NOERROR;
    }
    if ((String(mAllowed).IndexOf(c)) != -1) {
        *ret = TRUE;
        return NOERROR;
    }
    *ret = FALSE;
    return NOERROR;
}

/*****************************PasswordFilterGMail*****************************/
PasswordFilterGMail::PasswordFilterGMail() : LoginFilter(FALSE)
{
}

PasswordFilterGMail::PasswordFilterGMail(
    /* [in] */ Boolean appendInvalid) : LoginFilter(appendInvalid)
{
}

ECode PasswordFilterGMail::IsAllowed(
    /* [in] */ Char32 c,
    /* [out] */ Boolean* ret)
{
    if (32 <= c && c <= 127) {
        *ret = TRUE; // standard charset
        return NOERROR;
    }
    // if (128 <= c && c <= 159) return true;  // nonstandard (Windows(TM)(R)) charset
    if (160 <= c && c <= 255) {
        *ret = TRUE; // extended charset
        return NOERROR;
    }
    *ret = FALSE;
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos