
#ifndef  __ANIMATORINFLATER_H__
#define  __ANIMATORINFLATER_H__

#include "ext/frameworkext.h"
#include <Elastos.Droid.Core_server.h>

using Org::Xmlpull::V1::IXmlPullParser;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Animation {

class AnimatorInflater{
public:
    /**
     * Loads an {@link Animator} object from a resource
     *
     * @param context Application context used to access resources
     * @param id The resource id of the animation to load
     * @return The animator object reference by the specified id
     * @throws android.content.res.Resources.NotFoundException when the animation cannot be loaded
     */
    static CARAPI_(AutoPtr<IAnimator>) LoadAnimator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id);

private:
    static CARAPI_(AutoPtr<IAnimator>) CreateAnimatorFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser);

    static CARAPI_(AutoPtr<IAnimator>) CreateAnimatorFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ IAnimatorSet* parent,
        /* [in] */ Int32 sequenceOrdering);

    static CARAPI_(AutoPtr<IObjectAnimator>) LoadObjectAnimator(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Creates a new animation whose parameters come from the specified context and
     * attributes set.
     *
     * @param context the application environment
     * @param attrs the set of attributes holding the animation parameters
     */
    static CARAPI_(AutoPtr<IValueAnimator>) LoadAnimator(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ IValueAnimator* anim);

private:
    /**
     * These flags are used when parsing AnimatorSet objects
     */
    static const Int32 TOGETHER;
    static const Int32 SEQUENTIALLY;

    /**
     * Enum values used in XML attributes to indicate the value for mValueType
     */
    static const Int32 VALUE_TYPE_FLOAT;
    static const Int32 VALUE_TYPE_INT;
    static const Int32 VALUE_TYPE_COLOR;
    static const Int32 VALUE_TYPE_CUSTOM;
};


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
#endif  //__ANIMATORINFLATER_H__
