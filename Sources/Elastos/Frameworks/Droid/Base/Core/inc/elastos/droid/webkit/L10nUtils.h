
#ifndef __ELASTOS_DROID_WEBKIT_L10NUTILS_H__
#define __ELASTOS_DROID_WEBKIT_L10NUTILS_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Content::IContext;
using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * @hide
 */
class L10nUtils
{
public:
    static CARAPI_(void) SetApplicationContext(
        /* [in] */ IContext* applicationContext);

    static CARAPI_(String) GetLocalisedString(
        /* [in] */ Int32 id);

private:
    static CARAPI_(String) LoadString(
        /* [in] */ Int32 id);

    // These array elements must be kept in sync with those defined in
    // external/chromium/android/app/l10n_utils.h
    static Int32 mIdsArray[];

    static AutoPtr<IContext> mApplicationContext;
    //static AutoPtr<IObjectStringMap> mStrings;
    // Don't use SoftReference for value of HashMap
    static HashMap<Int32, String> mStrings;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_L10NUTILS_H__
