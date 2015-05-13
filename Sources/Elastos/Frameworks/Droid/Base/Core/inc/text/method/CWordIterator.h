
#ifndef __CWORDITERATOR_H__
#define __CWORDITERATOR_H__

#include "_CWordIterator.h"
#include "text/method/WordIterator.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CWordIterator), public WordIterator
{
public:
    CARAPI SetCharSequence(
        /* [in] */ ICharSequence* charSequence,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetBeginning(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* ret);

    CARAPI GetEnd(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* ret);

    CARAPI Preceding(
        /* [in] */ Int32 position,
        /* [out] */ Int32* ret);

    CARAPI Following(
        /* [in] */ Int32 position,
        /* [out] */ Int32* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale* locale);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CWORDITERATOR_H__
