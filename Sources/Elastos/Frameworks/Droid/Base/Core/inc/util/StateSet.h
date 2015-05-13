
#ifndef __STATESET_H__
#define __STATESET_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Utility {

class StateSet
{
public:
    static CARAPI_(Boolean) IsWildCard(
        /* [in] */ const ArrayOf<Int32>* stateSetOrSpec);

    static CARAPI_(Boolean) StateSetMatches(
        /* [in] */ const ArrayOf<Int32>* stateSpec,
        /* [in] */ const ArrayOf<Int32>* stateSet);

    static CARAPI_(Boolean) StateSetMatches(
        /* [in] */ const ArrayOf<Int32>* stateSpec,
        /* [in] */ Int32 state);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) TrimStateSet(
        /* [in] */ const ArrayOf<Int32>* states,
        /* [in] */ Int32 newSize);

    //static String dump(int[] states);

public:
    static const AutoPtr< ArrayOf<Int32> > WILD_CARD;
    static const AutoPtr< ArrayOf<Int32> > NOTHING;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__STATESET_H__
