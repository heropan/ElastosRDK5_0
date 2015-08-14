#ifndef __ELASTOS_DROID_TEXT_FORMAT_CFormatter_H__
#define __ELASTOS_DROID_TEXT_FORMAT_CFormatter_H__

#include "_CFormatter.h"
#include "Elastos.Droid.Core_server.h"
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

CarClass(CFormatter)
{
public:
    CARAPI FormatFileSize(
        /* [in] */ IContext* context,
        /* [in] */ Int64 number,
        /* [out] */ String* ret);

    CARAPI FormatIpAddress(
        /* [in] */ Int32 ipv4Address,
        /* [out] */ String* ret);

    CARAPI FormatShortFileSize(
        /* [in] */ IContext* context,
        /* [in] */ Int64 number,
        /* [out] */ String* ret);
};

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_FORMAT_CFormatter_H__
