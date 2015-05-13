#ifndef __SPANNEDSTRING_H__
#define __SPANNEDSTRING_H__

#include "text/SpannableStringInternal.h"

namespace Elastos {
namespace Droid {
namespace Text {

extern "C" const InterfaceID EIID_SpannedString;

class SpannedString : public SpannableStringInternal
{
public:
    SpannedString(
        /* [in] */ ICharSequence* source);

    virtual CARAPI_(AutoPtr<ICharSequence>) SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:
    SpannedString(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Int32) GetLen(
        /* [in] */ ICharSequence* source);

protected:
    SpannedString();

    Init(
        /* [in] */ ICharSequence* source);

    Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif
