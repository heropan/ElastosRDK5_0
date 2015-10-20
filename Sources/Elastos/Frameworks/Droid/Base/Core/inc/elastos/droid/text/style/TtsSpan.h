#ifndef __ELASTOS_DROID_TEXT_STYLE_TTSSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_TTSSPAN_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Os::IPersistableBundle;
using Elastos::Droid::Text::IParcelableSpan;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * A span that supplies additional meta-data for the associated text intended
 * for text-to-speech engines. If the text is being processed by a
 * text-to-speech engine, the engine may use the data in this span in addition
 * to or instead of its associated text.
 *
 * Each instance of a TtsSpan has a type, for example {@link #TYPE_DATE}
 * or {@link #TYPE_MEASURE}. And a list of arguments, provided as
 * key-value pairs in a bundle.
 *
 * The inner classes are there for convenience and provide builders for each
 * TtsSpan type.
 */
class TtsSpan
    : public Object
    , public ITtsSpan
    , public IParcelableSpan
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    TtsSpan();

    virtual ~TtsSpan();

    CARAPI constructor();


    CARAPI constructor(
        /* [in] */ const String& type,
        /* [in] */ IPersistableBundle* args);

    /**
     * Returns the type.
     * @return The type of this instance.
     */
    CARAPI GetType(
        /* [out] */ String* type);

    /**
     * Returns a bundle of the arguments set.
     * @return The bundle of the arguments set.
     */
    CARAPI GetArgs(
        /* [out] */ IPersistableBundle** source);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* id);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    String mType;
    AutoPtr<IPersistableBundle> mArgs;

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_TTSSPAN_H__
