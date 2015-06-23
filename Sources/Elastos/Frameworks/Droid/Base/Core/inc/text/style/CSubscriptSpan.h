
#ifndef __CSUBSCRIPTSPAN_H__
#define __CSUBSCRIPTSPAN_H__

#include "_CSubscriptSpan.h"
#include "text/style/SubscriptSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CSubscriptSpan), public SubscriptSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CSUBSCRIPTSPAN_H__
