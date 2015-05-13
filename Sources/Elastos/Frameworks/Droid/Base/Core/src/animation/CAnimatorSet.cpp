
#include "animation/CAnimatorSet.h"

namespace Elastos {
namespace Droid {
namespace Animation {

IANIMATOR_METHOD_IMPL(CAnimatorSet, AnimatorSet)

PInterface CAnimatorSet::Probe(
        /* [in] */ REIID riid)
{
    if(riid == EIID_Animator)
    {
        return reinterpret_cast<PInterface>((Animator*)this);
    } else if(riid == EIID_AnimatorSet) {
        return reinterpret_cast<PInterface>((AnimatorSet*)this);
    }
    return _CAnimatorSet::Probe(riid);
}

/**
 * Sets up this AnimatorSet to play all of the supplied animations at the same time.
 *
 * @param items The animations that will be started simultaneously.
 */
ECode CAnimatorSet::PlayTogether(
    /* [in] */ ArrayOf<IAnimator*>* items)
{
    return AnimatorSet::PlayTogether(items);
}

/**
 * Sets up this AnimatorSet to play all of the supplied animations at the same time.
 *
 * @param items The animations that will be started simultaneously.
 */
ECode CAnimatorSet::PlayTogetherEx(
    /* [in] */ IObjectContainer* items)
{
    return AnimatorSet::PlayTogether(items);
}

/**
 * Sets up this AnimatorSet to play each of the supplied animations when the
 * previous animation ends.
 *
 * @param items The animations that will be started one after another.
 */
ECode CAnimatorSet::PlaySequentially(
    /* [in] */ ArrayOf<IAnimator*>* items)
{
    return AnimatorSet::PlaySequentially(items);
}

/**
 * Sets up this AnimatorSet to play each of the supplied animations when the
 * previous animation ends.
 *
 * @param items The animations that will be started one after another.
 */
ECode CAnimatorSet::PlaySequentiallyEx(
    /* [in] */ IObjectContainer* items)
{
    return AnimatorSet::PlaySequentially(items);
}

/**
 * Returns the current list of child Animator objects controlled by this
 * AnimatorSet. This is a copy of the internal list; modifications to the returned list
 * will not affect the AnimatorSet, although changes to the underlying Animator objects
 * will affect those objects being managed by the AnimatorSet.
 *
 * @return ArrayList<Animator> The list of child animations of this AnimatorSet.
 */
ECode CAnimatorSet::GetChildAnimations(
    /* [out, callee] */ ArrayOf<IAnimator*>** childAnimations)
{
    return AnimatorSet::GetChildAnimations(childAnimations);
}

ECode CAnimatorSet::GetChildAnimationsEx(
    /* [out] */ IObjectContainer** childAnimations)
{
    return AnimatorSet::GetChildAnimations(childAnimations);
}

/**
 * This method creates a <code>Builder</code> object, which is used to
 * set up playing constraints. This initial <code>play()</code> method
 * tells the <code>Builder</code> the animation that is the dependency for
 * the succeeding commands to the <code>Builder</code>. For example,
 * calling <code>play(a1).with(a2)</code> sets up the AnimatorSet to play
 * <code>a1</code> and <code>a2</code> at the same time,
 * <code>play(a1).before(a2)</code> sets up the AnimatorSet to play
 * <code>a1</code> first, followed by <code>a2</code>, and
 * <code>play(a1).after(a2)</code> sets up the AnimatorSet to play
 * <code>a2</code> first, followed by <code>a1</code>.
 *
 * <p>Note that <code>play()</code> is the only way to tell the
 * <code>Builder</code> the animation upon which the dependency is created,
 * so successive calls to the various functions in <code>Builder</code>
 * will all refer to the initial parameter supplied in <code>play()</code>
 * as the dependency of the other animations. For example, calling
 * <code>play(a1).before(a2).before(a3)</code> will play both <code>a2</code>
 * and <code>a3</code> when a1 ends; it does not set up a dependency between
 * <code>a2</code> and <code>a3</code>.</p>
 *
 * @param anim The animation that is the dependency used in later calls to the
 * methods in the returned <code>Builder</code> object. A null parameter will result
 * in a null <code>Builder</code> return value.
 * @return Builder The object that constructs the AnimatorSet based on the dependencies
 * outlined in the calls to <code>play</code> and the other methods in the
 * <code>Builder</code object.
 */
ECode CAnimatorSet::Play(
    /* [in] */ IAnimator* anim,
    /* [out] */ IAnimatorSetBuilder** builder)
{
    VALIDATE_NOT_NULL(builder);
    AutoPtr<IAnimatorSetBuilder> b = AnimatorSet::Play(anim);
    *builder = b;
    INTERFACE_ADDREF(*builder);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
