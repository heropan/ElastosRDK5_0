
#ifndef __CWEBVIEWDATABASEHELPER_H__
#define __CWEBVIEWDATABASEHELPER_H__

#include "_CWebViewDatabaseHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewDatabaseHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IWebViewDatabase** instance);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CWEBVIEWDATABASEHELPER_H__
