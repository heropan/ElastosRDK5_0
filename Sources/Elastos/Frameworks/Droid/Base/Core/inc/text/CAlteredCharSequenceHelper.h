
#ifndef __CALTEREDCHARSEQUENCEHELPER_H__
#define __CALTEREDCHARSEQUENCEHELPER_H__

#include "_CAlteredCharSequenceHelper.h"

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

#endif // __CALTEREDCHARSEQUENCEHELPER_H__
