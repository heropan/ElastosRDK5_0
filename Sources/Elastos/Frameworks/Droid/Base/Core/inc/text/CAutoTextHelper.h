
#ifndef __CAUTOTEXTHELPER_H__
#define __CAUTOTEXTHELPER_H__

#include "_CAutoTextHelper.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAutoTextHelper)
{
public:
    CARAPI Get(
        /* [in] */ ICharSequence* src,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IView* view,
        /* [out] */ String* ret);

    CARAPI GetSize(
        /* [in] */ IView* view,
        /* [out] */ Int32* ret);

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CAUTOTEXTHELPER_H__
