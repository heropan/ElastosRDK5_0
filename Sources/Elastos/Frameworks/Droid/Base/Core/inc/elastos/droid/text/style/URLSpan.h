#ifndef __ELASTOS_DROID_TEXT_STYLE_URLSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_URLSpan_H__

#include "text/style/ClickableSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class URLSpan : public ClickableSpan
{
public:
    URLSpan(
        /* [in] */ const String& url);

    URLSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ const String& url);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

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
