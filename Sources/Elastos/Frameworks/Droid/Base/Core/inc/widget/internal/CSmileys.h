
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_SMILEYS_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_SMILEYS_H__

#include "ext/frameworkext.h"
#include "R.h"
#include "_CSmileys.h"

using Elastos::Droid::R;

/**
 * Temp stupidity until we have a real emoticon API.
 */
namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CSmileys)
{
private:
    static const Int32 sIconIds[17];

public:
    CARAPI GetSmileyResource(
        /* [in] */ Int32 which,
        /* [out] */ Int32* rs);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
