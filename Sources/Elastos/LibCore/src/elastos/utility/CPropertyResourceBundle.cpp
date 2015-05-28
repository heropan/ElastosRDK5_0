
#include "CPropertyResourceBundle.h"
#include "CProperties.h"
#include "CStringWrapper.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Utility {

//==========================================================
//       CPropertyResourceBundle::_Enumeration
//==========================================================

CPropertyResourceBundle::_Enumeration::_Enumeration(
    /* [in] */ CPropertyResourceBundle* host)
    : mHost(host)
{
    mLocal = mHost->GetLocalKeys();

    mHost->mParent->GetKeys((IEnumeration**)&mPEnum);
}

CAR_INTERFACE_IMPL(CPropertyResourceBundle::_Enumeration, IEnumeration);

ECode CPropertyResourceBundle::_Enumeration::HasMoreElements(
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value)

    Boolean isflag = FALSE;
    if (mLocal->HasMoreElements(&isflag), isflag) {
        *value = TRUE;
        return NOERROR;
    }
    *value = FindNext();
    return NOERROR;
}

ECode CPropertyResourceBundle::_Enumeration::NextElement(
    /* [out] */ IInterface ** inter)
{
    VALIDATE_NOT_NULL(inter)
    *inter = NULL;

    Boolean isflag = FALSE;
    if (mLocal->HasMoreElements(&isflag), isflag) {
        return mLocal->NextElement(inter);
    }
    if (FindNext()) {
        String result = mNextElement;
        mNextElement = String(NULL);
        AutoPtr<ICharSequence> sq;
        CStringWrapper::New(result, (ICharSequence**)&sq);
        *inter = sq;
        INTERFACE_ADDREF(*inter)
        return NOERROR;
    }
    // Cause an exception
    return mPEnum->NextElement(inter);
}

Boolean CPropertyResourceBundle::_Enumeration::FindNext()
{
    if (!mNextElement.IsNull()) {
        return TRUE;
    }
    Boolean isflag = FALSE;
    while (mPEnum->HasMoreElements(&isflag), isflag) {
        AutoPtr<IInterface> next;
        mPEnum->NextElement((IInterface**)&next);
        if (IMap::Probe(mHost->mResources)->ContainsKey(next, &isflag), !isflag) {
            AutoPtr<ICharSequence> sq = ICharSequence::Probe(next);
            if (sq != NULL) {
                sq->ToString(&mNextElement);
            }
            return TRUE;
        }
    }
    return FALSE;
}

//==========================================================
//       CPropertyResourceBundle
//==========================================================

ECode CPropertyResourceBundle::constructor()
{
}

ECode CPropertyResourceBundle::constructor(
    /* [in] */ IInputStream* stream)
{
    if (stream == NULL) {
        // throw new NullPointerException("stream == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    FAIL_RETURN(CProperties::New((IProperties**)&mResources));
    return mResources->Load(stream);
}

ECode CPropertyResourceBundle::constructor(
    /* [in] */ IReader* reader)
{
    FAIL_RETURN(CProperties::New((IProperties**)&mResources));
    return mResources->Load(reader);
}

ECode CPropertyResourceBundle::GetKeys(
    /* [out] */ IEnumeration** enu)
{
    VALIDATE_NOT_NULL(enu)
    *enu = NULL;

    if (mParent == NULL) {
        AutoPtr<IEnumeration> outenu = GetLocalKeys();
        *enu = outenu;
        INTERFACE_ADDREF(*enu)
        return NOERROR;
    }
    AutoPtr<IEnumeration> outenu = (IEnumeration*) new _Enumeration(this);
    *enu = outenu;
    INTERFACE_ADDREF(*enu)
    return NOERROR;
}

ECode CPropertyResourceBundle::GetLocale(
    /* [out] */ ILocale** loc)
{
    return ResourceBundle::GetLocale(loc);
}

ECode CPropertyResourceBundle::GetObject(
    /* [in] */ const String& key,
    /* [out] */ IInterface** outface)
{
    return ResourceBundle::GetObject(key, outface);
}

ECode CPropertyResourceBundle::GetString(
    /* [in] */ const String& key,
    /* [out] */ String* str)
{
    return ResourceBundle::GetString(key, str);
}

ECode CPropertyResourceBundle::GetStringArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** arrstr)
{
    return ResourceBundle::GetStringArray(key, arrstr);
}

ECode CPropertyResourceBundle::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    return ResourceBundle::ContainsKey(key, value);
}

ECode CPropertyResourceBundle::HandleGetObject(
    /* [in] */ const String& key,
    /* [out] */ IInterface** outface)
{
    AutoPtr<ICharSequence> sq;
    CStringWrapper::New(key, (ICharSequence**)&sq);
    return mResources->Get(sq, outface);
}

ECode CPropertyResourceBundle::HandleKeySet(
    /* [out] */ ISet** outset)
{
    return ResourceBundle::HandleKeySet(outset);
}

AutoPtr<IEnumeration> CPropertyResourceBundle::GetLocalKeys()
{
    AutoPtr<IInterface> outenu;

    mResources->PropertyNames((IInterface**)&outenu);
    return IEnumeration::Probe(outenu);
}

} // namespace Utility
} // namespace Elastos
