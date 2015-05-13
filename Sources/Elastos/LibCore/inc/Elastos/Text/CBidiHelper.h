
#ifndef __CBIDIHELPER_H__
#define __CBIDIHELPER_H__

#include "_CBidiHelper.h"

namespace Elastos {
namespace Text {

CarClass(CBidiHelper)
{
public:

    CARAPI ReorderVisually(
        /* [in] */ ArrayOf<Byte>* levels,
        /* [in] */ Int32 levelStart,
        /* [in] */ ArrayOf<IInterface*>* objects,
        /* [in] */ Int32 objectStart,
        /* [in] */ Int32 count);

    CARAPI RequiresBidi(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [out] */ Boolean * result);
};

} // namespace Text
} // namespace Elastos

#endif // __CBIDIHELPER_H__
