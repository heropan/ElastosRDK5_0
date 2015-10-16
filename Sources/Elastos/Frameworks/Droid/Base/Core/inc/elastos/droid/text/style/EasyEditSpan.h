#ifndef __ELASTOS_DROID_TEXT_STYLE_EasyEditSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_EasyEditSpan_H__

#include "Elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

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

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPendingIntent* intent);

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI GetSpanTypeId(
        /* [out] */ Int32* id);

    /**
     * @return True if the {@link TextView} should offer the ability to delete the text.
     *
     * @hide
     */
    public boolean isDeleteEnabled() {
        return mDeleteEnabled;
    }

    /**
     * Enables or disables the deletion of the text.
     *
     * @hide
     */
    public void setDeleteEnabled(boolean value) {
        mDeleteEnabled = value;
    }

    /**
     * @return the pending intent to send when the wrapped text is deleted or modified.
     *
     * @hide
     */
    public PendingIntent getPendingIntent() {
        return mPendingIntent;
    }

private:
    AutoPtr<IPendingIntent> mPendingIntent;

    Boolean mDeleteEnabled;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_EasyEditSpan_H__
