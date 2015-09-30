
#ifndef __ELASTOS_DROID_TEXT_STYLE_CURLSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CURLSPAN_H__

#include "_Elastos_Droid_Text_Style_CURLSpan.h"
#include "elastos/droid/text/style/URLSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CURLSpan),public URLSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnClick(
        /* [in] */ IView* widget);

    CARAPI GetURL(
        /* [out] */ String* url);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& url);

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CURLSPAN_H__
