#ifndef __EasyEditSpan_H__
#define __EasyEditSpan_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * Provides an easy way to edit a portion of text.
 * <p>
 * The {@link TextView} uses this span to allow the user to delete a chuck of text in one click.
 * the text. {@link TextView} removes this span as soon as the text is edited, or the cursor moves.
 */
class EasyEditSpan
{
public:
    EasyEditSpan();

    //@Override
    CARAPI_(Int32) DescribeContents();

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI_(Int32) GetSpanTypeId();
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __EasyEditSpan_H__
