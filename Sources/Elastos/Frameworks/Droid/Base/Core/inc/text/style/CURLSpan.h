
#ifndef __CURLSPAN_H__
#define __CURLSPAN_H__

#include "_CURLSpan.h"
#include "text/style/URLSpan.h"
#include "text/style/CharacterStyleMacro.h"
#include "text/ParcelableSpanMacro.h"

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

#endif // __CURLSPAN_H__
