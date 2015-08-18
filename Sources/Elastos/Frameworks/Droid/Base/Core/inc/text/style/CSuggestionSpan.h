
#ifndef __ELASTOS_DROID_TEXT_STYLE_CSUGGESTIONSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CSUGGESTIONSPAN_H__

#include "_Elastos_Droid_Text_Style_CSuggestionSpan.h"
#include "ext/frameworkext.h"
#include "text/style/SuggestionSpan.h"


#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CSuggestionSpan)
    , public SuggestionSpan
{
public:
    class ParcelableCreatorSuggestionSpan
        : public ElRefBase
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [out] */ InterfaceID* IID);
    public:
        //@Override
        CARAPI CreateFromParcel(
            /* [in] */ IParcel* source,
            /* [out] */ ISuggestionSpan** ret);

        //@Override
        CARAPI NewArray(
            /* [in] */ Int32 size,
            /* [out] */ ArrayOf<ISuggestionSpan*>** ret);

    };

public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSuggestions(
        /* [out, callee] */ ArrayOf<String>** ret);

    CARAPI GetLocale(
        /* [out] */ String* ret);

    CARAPI GetNotificationTargetClassName(
        /* [out] */ String* ret);

    CARAPI GetFlags(
        /* [out] */ Int32* ret);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* ret);

    CARAPI GetHashCode(
        /* [out] */ Int32* ret);

    CARAPI GetUnderlineColor(
        /* [out] */ Int32* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ ILocale* pLocale,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILocale* locale,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags,
        /* [in] */ Handle32 notificationTargetClass);

    CARAPI constructor(
        /* [in] */ IParcel* src);

public:
    static const AutoPtr<ParcelableCreatorSuggestionSpan> CREATOR;// = new ParcelableCreatorSuggestionSpan();

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CSUGGESTIONSPAN_H__
