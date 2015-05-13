

#ifndef __CLAYOUTDIRECTIONS_H__
#define __CLAYOUTDIRECTIONS_H__

#include "_CLayoutDirections.h"

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

#endif //__CLAYOUTDIRECTIONS_H__
