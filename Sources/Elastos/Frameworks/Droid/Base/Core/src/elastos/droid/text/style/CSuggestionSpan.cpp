
#include "elastos/droid/text/style/CSuggestionSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/*****************************CSuggestionSpan::ParcelableCreatorSuggestionSpan*****************************/
PInterface CSuggestionSpan::ParcelableCreatorSuggestionSpan::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 CSuggestionSpan::ParcelableCreatorSuggestionSpan::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSuggestionSpan::ParcelableCreatorSuggestionSpan::Release()
{
    return ElRefBase::Release();
}

ECode CSuggestionSpan::ParcelableCreatorSuggestionSpan::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CSuggestionSpan::ParcelableCreatorSuggestionSpan::CreateFromParcel(
    /* [in] */ IParcel* source,
    /* [out] */ ISuggestionSpan** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ISuggestionSpan> ss;
    ECode ec = CSuggestionSpan::New(source, (ISuggestionSpan**)&ss);
    *ret = ss;
    REFCOUNT_ADD(*ret);
    return ec;
}

ECode CSuggestionSpan::ParcelableCreatorSuggestionSpan::NewArray(
    /* [in] */ Int32 size,
    /* [out, callee] */ ArrayOf<ISuggestionSpan*>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<ISuggestionSpan*> > o = ArrayOf<ISuggestionSpan*>::Alloc(size);
    *ret = o;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

/*****************************CSuggestionSpan*****************************/

ICHARACTERSTYLE_METHODS_IMPL(CSuggestionSpan, SuggestionSpan, SuggestionSpan)
IPARCELABLESPAN_METHODS_IMPL(CSuggestionSpan, SuggestionSpan, SuggestionSpan)

PInterface CSuggestionSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CSuggestionSpan::Probe(riid);
}

ECode CSuggestionSpan::GetSuggestions(
    /* [out, callee] */ ArrayOf<String>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<String> > o = SuggestionSpan::GetSuggestions();
    *ret = o;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CSuggestionSpan::GetLocale(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SuggestionSpan::GetLocale();
    return NOERROR;
}

ECode CSuggestionSpan::GetNotificationTargetClassName(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SuggestionSpan::GetNotificationTargetClassName();
    return NOERROR;
}

ECode CSuggestionSpan::GetFlags(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SuggestionSpan::GetFlags();
    return NOERROR;
}

ECode CSuggestionSpan::SetFlags(
    /* [in] */ Int32 flags)
{
    SuggestionSpan::SetFlags(flags);
    return NOERROR;
}

ECode CSuggestionSpan::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SuggestionSpan::Equals(o);
    return NOERROR;
}

ECode CSuggestionSpan::GetHashCode(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SuggestionSpan::GetHashCode();
    return NOERROR;
}

ECode CSuggestionSpan::GetUnderlineColor(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SuggestionSpan::GetUnderlineColor();
    return NOERROR;
}

ECode CSuggestionSpan::constructor()
{
    return NOERROR;
}

ECode CSuggestionSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags)
{
    Init(context, suggestions, flags);
    return NOERROR;
}

ECode CSuggestionSpan::constructor(
    /* [in] */ ILocale* locale,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags)
{
    Init(locale, suggestions, flags);
    return NOERROR;
}

ECode CSuggestionSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags,
    /* [in] */ Handle32 notificationTargetClass)
{
    Init(context, locale, suggestions, flags, notificationTargetClass);
    return NOERROR;
}

ECode CSuggestionSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

