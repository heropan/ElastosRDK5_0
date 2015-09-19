

#ifndef __ELASTOS_DROID_TEXT_CLAYOUTDIRECTIONS_H__
#define __ELASTOS_DROID_TEXT_CLAYOUTDIRECTIONS_H__

#include "_Elastos_Droid_Text_CLayoutDirections.h"

namespace Elastos {
namespace Droid {
namespace Text {


CarClass(CLayoutDirections)
{
public:

    CARAPI constructor(
        /* [in] */ ArrayOf<Int32>* directions);

    CARAPI SetDirections(
            /* [in] */ ArrayOf<Int32>* directions);

    CARAPI GetDirections(
        /* [out, callee] */ ArrayOf<Int32>** directions);

private:
    AutoPtr< ArrayOf<Int32> > mDirections;
};


} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CLAYOUTDIRECTIONS_H__
