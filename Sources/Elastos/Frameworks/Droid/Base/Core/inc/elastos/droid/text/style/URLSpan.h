#ifndef __ELASTOS_DROID_TEXT_STYLE_URLSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_URLSpan_H__

#include "elastos/droid/text/style/ClickableSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class URLSpan
    : public ClickableSpan
    , public IParcelableSpan
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    URLSpan();

    CARAPI constructor(
        /* [in] */ const String& url);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* id);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(String) GetURL();

    //@Override
    CARAPI OnClick(
        /* [in] */ IView* widget);

protected:
    URLSpan();

private:
    String mURL;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_URLSpan_H__
