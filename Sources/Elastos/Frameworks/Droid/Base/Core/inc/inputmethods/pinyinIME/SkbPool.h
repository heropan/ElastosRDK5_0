
#ifndef  __SKBPOOL_H__
#define  __SKBPOOL_H__

#include "ext/frameworkdef.h"
#include <elastos/List.h>
#include "SkbTemplate.h"

using Elastos::Utility::List;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

class SoftKeyboard;

/**
 * Class used to cache previously loaded soft keyboard layouts.
 */
class SkbPool: public ElRefBase
{
public:
    static AutoPtr<SkbPool> GetInstance();

    void ResetCachedSkb();

    AutoPtr<SkbTemplate> GetSkbTemplate(
        /* [in] */ Int32 skbTemplateId,
        /* [in] */ IContext* context);

    // Try to find the keyboard in the pool with the cache id. If there is no
    // keyboard found, try to load it with the given xml id.
    AutoPtr<SoftKeyboard> GetSoftKeyboard(
        /* [in] */ Int32 skbCacheId,
        /* [in] */ Int32 skbXmlId,
        /* [in] */ Int32 skbWidth,
        /* [in] */ Int32 skbHeight,
        /* [in] */ IContext* context);

private:
    SkbPool();

private:
    static AutoPtr<SkbPool> mInstance;

    List<AutoPtr<SkbTemplate> > mSkbTemplates; // = new Vector<SkbTemplate>();
    List<AutoPtr<SoftKeyboard> > mSoftKeyboards; // = new Vector<SoftKeyboard>();
};

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos

#endif  //__SKBPOOL_H__
