
#ifndef __ELASTOS_DROID_WEBKIT_CDATESORTER_H__
#define __ELASTOS_DROID_WEBKIT_CDATESORTER_H__

#include "_Elastos_Droid_Webkit_CDateSorter.h"
#include "webkit/DateSorter.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CDateSorter), public DateSorter
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI GetIndex(
        /* [in] */ Int64 time,
        /* [out] */ Int32* index);

    CARAPI GetLabel(
        /* [in] */ Int32 index,
        /* [out] */ String* lable);

    CARAPI GetBoundary(
        /* [in] */ Int32 index,
        /* [out] */ Int64* boundary);

};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CDATESORTER_H__
