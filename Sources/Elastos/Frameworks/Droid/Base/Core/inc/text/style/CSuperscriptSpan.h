
#ifndef __CSUPERSCRIPTSPAN_H__
#define __CSUPERSCRIPTSPAN_H__

#include "_CSuperscriptSpan.h"
#include "text/style/SuperscriptSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CSuperscriptSpan)
    , public SuperscriptSpan
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

#endif // __CSUPERSCRIPTSPAN_H__
