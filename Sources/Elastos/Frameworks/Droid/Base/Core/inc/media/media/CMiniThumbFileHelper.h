
#ifndef __CMINITHUMBFILEHELPER_H__
#define __CMINITHUMBFILEHELPER_H__

#include "_CMiniThumbFileHelper.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMiniThumbFileHelper)
{
public:
    /**
     * We store different types of thumbnails in different files. To remain backward compatibility,
     * we should hashcode of content://media/external/images/media remains the same.
     */
    CARAPI Reset();

    CARAPI Instance(
        /* [in] */ IUri* uri,
        /* [out] */ IMiniThumbFile** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMINITHUMBFILEHELPER_H__
