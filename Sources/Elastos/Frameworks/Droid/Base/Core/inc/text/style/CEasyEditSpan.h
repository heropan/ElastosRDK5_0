
#ifndef __CEASYEDITSPAN_H__
#define __CEASYEDITSPAN_H__

#include "_CEasyEditSpan.h"
#include "text/style/EasyEditSpan.h"
#include "text/ParcelableSpanMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CEasyEditSpan), public EasyEditSpan
{
public:
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI constructor();
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CEASYEDITSPAN_H__
