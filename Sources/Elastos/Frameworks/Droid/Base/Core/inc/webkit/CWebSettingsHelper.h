
#ifndef __CWEBSETTINGSHELPER_H__
#define __CWEBSETTINGSHELPER_H__

#include "_CWebSettingsHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebSettingsHelper)
{
public:
    CARAPI GetDefaultUserAgent(
        /* [in] */ IContext* context,
        /* [out] */ String* ua);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CWEBSETTINGSHELPER_H__
