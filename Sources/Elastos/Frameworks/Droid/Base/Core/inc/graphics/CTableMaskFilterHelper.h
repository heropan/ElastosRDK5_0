
#ifndef __CTABLEMASKFILTERHELPER_H__
#define __CTABLEMASKFILTERHELPER_H__

#include "_CTableMaskFilterHelper.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CTableMaskFilterHelper)
{
public:
    CARAPI CreateClipTable(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max,
        /* [out] */ ITableMaskFilter** filter);

    CARAPI CreateGammaTable(
        /* [in] */ Float gamma,
        /* [out] */ ITableMaskFilter** filter);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CTABLEMASKFILTERHELPER_H__
