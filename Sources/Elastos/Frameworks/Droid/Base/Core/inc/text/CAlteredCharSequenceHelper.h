
#ifndef __ELASTOS_DROID_TEXT_CALTEREDCHARSEQUENCEHELPER_H__
#define __ELASTOS_DROID_TEXT_CALTEREDCHARSEQUENCEHELPER_H__

#include "_Elastos_Droid_Text_CAlteredCharSequenceHelper.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAlteredCharSequenceHelper)
{
public:
    CARAPI Make(
        /* [in] */ ICharSequence* source,
        /* [in] */ ArrayOf<Char32>* sub,
        /* [in] */ Int32 substart,
        /* [in] */ Int32 subend,
        /* [out] */ IAlteredCharSequence** ret);

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_CALTEREDCHARSEQUENCEHELPER_H__
