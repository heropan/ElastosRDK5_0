
#include "elastos/droid/view/animation/AnimationUtils.h"
#include "elastos/droid/view/animation/CAccelerateDecelerateInterpolator.h"
#include "elastos/droid/view/animation/CAccelerateInterpolator.h"
#include "elastos/droid/view/animation/CAnticipateInterpolator.h"
#include "elastos/droid/view/animation/CAnticipateOvershootInterpolator.h"
#include "elastos/droid/view/animation/CBounceInterpolator.h"
#include "elastos/droid/view/animation/CCycleInterpolator.h"
#include "elastos/droid/view/animation/CDecelerateInterpolator.h"
#include "elastos/droid/view/animation/CLinearInterpolator.h"
#include "elastos/droid/view/animation/COvershootInterpolator.h"
#include "elastos/droid/view/animation/CAnimationSet.h"
#include "elastos/droid/view/animation/CAlphaAnimation.h"
#include "elastos/droid/view/animation/CRotateAnimation.h"
#include "elastos/droid/view/animation/CScaleAnimation.h"
#include "elastos/droid/view/animation/CTranslateAnimation.h"
#include "elastos/droid/view/animation/CLayoutAnimationController.h"
#include "elastos/droid/view/animation/CGridLayoutAnimationController.h"
#include "elastos/droid/os/SystemClock.h"
#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode AnimationUtils::CurrentAnimationTimeMillis(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = SystemClock::GetUptimeMillis();
    return NOERROR;
}

ECode AnimationUtils::LoadAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    *animation = NULL;

    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res));
    AutoPtr<IXmlResourceParser> parser;
    FAIL_RETURN(res->GetAnimation(id, (IXmlResourceParser**)&parser));
    ECode ec = CreateAnimationFromXml(context, parser, animation);

    if (parser != NULL) {
        parser->Close();
    }

    return ec;
}

ECode AnimationUtils::CreateAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IAnimation** animation)
{
    return CreateAnimationFromXml(
        c, parser, NULL, Xml::AsAttributeSet(parser), animation);
}

ECode AnimationUtils::CreateAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAnimationSet* parent,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    *animation = NULL;

    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth, curDepth;
    parser->GetDepth(&depth);
    String name;
    AutoPtr<IAnimation> anim;

    while (((parser->Next(&type), type) != IXmlPullParser::END_TAG
        || (parser->GetDepth(&curDepth), curDepth) > depth)
        && type != IXmlPullParser::END_DOCUMENT) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        anim = NULL;
        parser->GetName(&name);

        if (name.Equals("set")) {
            FAIL_RETURN(CAnimationSet::New(c, attrs, (IAnimationSet**)&anim));
            AutoPtr<IAnimation> temp;
            FAIL_RETURN(CreateAnimationFromXml(
                c, parser, IAnimationSet::Probe(anim), attrs, (IAnimation**)&temp));
        }
        else if (name.Equals("alpha")) {
            FAIL_RETURN(CAlphaAnimation::New(c, attrs, (IAlphaAnimation**)&anim));
        }
        else if (name.Equals("scale")) {
            FAIL_RETURN(CScaleAnimation::New(c, attrs, (IScaleAnimation**)&anim));
        }
        else if (name.Equals("rotate")) {
            FAIL_RETURN(CRotateAnimation::New(c, attrs, (IRotateAnimation**)&anim));
        }
        else if (name.Equals("translate")) {
            FAIL_RETURN(CTranslateAnimation::New(c, attrs, (ITranslateAnimation**)&anim));
        }
        else {
            Logger::E("AnimationUtils", "Unknown animation name: %s", name.string());
            return E_RUNTIME_EXCEPTION;
        }

        if (parent != NULL) {
            parent->AddAnimation(anim);
        }
    }

    *animation = anim;
    REFCOUNT_ADD(*animation);
    return NOERROR;
}

ECode AnimationUtils::LoadLayoutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ ILayoutAnimationController** controller)
{
    VALIDATE_NOT_NULL(controller);
    *controller = NULL;
    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res));
    AutoPtr<IXmlResourceParser> parser;
    FAIL_RETURN(res->GetAnimation(id, (IXmlResourceParser**)&parser));
    ECode ec = CreateLayoutAnimationFromXml(context, parser, controller);

    if (parser != NULL) {
        parser->Close();
    }

    return ec;
}

ECode AnimationUtils::CreateLayoutAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ ILayoutAnimationController** controller)
{
    return CreateLayoutAnimationFromXml(
        c, parser, Xml::AsAttributeSet(parser), controller);
}

ECode AnimationUtils::CreateLayoutAnimationFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ ILayoutAnimationController** controller)
{
    VALIDATE_NOT_NULL(controller);
    *controller = NULL;

    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth, curDepth;
    parser->GetDepth(&depth);
    String name;
    AutoPtr<ILayoutAnimationController> temp;

    while (((parser->Next(&type), type) != IXmlPullParser::END_TAG
        || (parser->GetDepth(&curDepth), curDepth) > depth)
        && type != IXmlPullParser::END_DOCUMENT)
    {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        temp = NULL;
        parser->GetName(&name);

        if (name.Equals("layoutAnimation")) {
            FAIL_RETURN(CLayoutAnimationController::New(c, attrs, (ILayoutAnimationController**)&temp));
        }
        else if (name.Equals("gridLayoutAnimation")) {
            FAIL_RETURN(CGridLayoutAnimationController::New(
                c, attrs, (IGridLayoutAnimationController**)&temp));
        }
        else {
            Logger::E("AnimationUtils", "Unknown layout animation name: %s", name.string());
            return E_RUNTIME_EXCEPTION;
        }
    }

    *controller = temp;
    REFCOUNT_ADD(*controller);
    return NOERROR;
}

ECode AnimationUtils::MakeInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean fromLeft,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    *animation = NULL;

    if (fromLeft) {
        FAIL_RETURN(AnimationUtils::LoadAnimation(
            context, R::anim::slide_in_left,
            animation));
    }
    else {
        FAIL_RETURN(AnimationUtils::LoadAnimation(
            context, R::anim::slide_in_right,
            animation));
    }

    AutoPtr<IInterpolator> interpolator;
    FAIL_RETURN(CDecelerateInterpolator::New((IDecelerateInterpolator**)&interpolator));
    (*animation)->SetInterpolator(interpolator);
    (*animation)->SetStartTime(CurrentAnimationTimeMillis());

    return NOERROR;
}

ECode AnimationUtils::MakeOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean toRight,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    *animation = NULL;

    if (toRight) {
        FAIL_RETURN(AnimationUtils::LoadAnimation(
            context, R::anim::slide_out_right,
            animation));
    }
    else {
        FAIL_RETURN(AnimationUtils::LoadAnimation(
            context, R::anim::slide_out_left,
            animation));
    }

    AutoPtr<IInterpolator> interpolator;
    FAIL_RETURN(CAccelerateInterpolator::New((IAccelerateInterpolator**)&interpolator));
    (*animation)->SetInterpolator(interpolator);
    (*animation)->SetStartTime(CurrentAnimationTimeMillis());

    return NOERROR;
}

ECode AnimationUtils::MakeInChildBottomAnimation(
    /* [in] */ IContext* context,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    *animation = NULL;

    FAIL_RETURN(AnimationUtils::LoadAnimation(
        context, R::anim::slide_in_left,
        animation));

    AutoPtr<IInterpolator> interpolator;
    FAIL_RETURN(CAccelerateInterpolator::New((IAccelerateInterpolator**)&interpolator));
    (*animation)->SetInterpolator(interpolator);
    (*animation)->SetStartTime(CurrentAnimationTimeMillis());

    return NOERROR;
}

ECode AnimationUtils::LoadInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NULL;

    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res));
    AutoPtr<IXmlResourceParser> parser;
    FAIL_RETURN(res->GetAnimation(id, (IXmlResourceParser**)&parser));
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);
    ECode ec = CreateInterpolatorFromXml(res, theme, parser, interpolator);

    if (parser != NULL) {
        parser->Close();
    }

    return ec;
}

ECode AnimationUtils::LoadInterpolator(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ Int32 id,
    /* [out] */ IInterpolator** interpolator) /*throws NotFoundException */
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NULL;

    AutoPtr<IXmlResourceParser> parser;
    // try {
    FAIL_RETURN(res->GetAnimation(id, (IXmlResourceParser**)&parser));
    ECode ec = CreateInterpolatorFromXml(res, theme, parser, interpolator);
    // } catch (XmlPullParserException ex) {
    //     NotFoundException rnf = new NotFoundException("Can't load animation resource ID #0x" +
    //             Integer.toHexString(id));
    //     rnf.initCause(ex);
    //     throw rnf;
    // } catch (IOException ex) {
    //     NotFoundException rnf = new NotFoundException("Can't load animation resource ID #0x" +
    //             Integer.toHexString(id));
    //     rnf.initCause(ex);
    //     throw rnf;
    // } finally {
    if (parser != NULL)
        parser->Close();
    // }
    return ec;
}

ECode AnimationUtils::CreateInterpolatorFromXml(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NULL;

    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth, curDepth;
    parser->GetDepth(&depth);
    String name;
    AutoPtr<IInterpolator> temp;
    AutoPtr<IAttributeSet> attrs;

    while (((parser->Next(&type), type) != IXmlPullParser::END_TAG
        || (parser->GetDepth(&curDepth), curDepth) > depth)
        && type != IXmlPullParser::END_DOCUMENT) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        temp = NULL;
        attrs = Xml::AsAttributeSet(parser);
        parser->GetName(&name);
        if (name.Equals("linearInterpolator")) {
            FAIL_RETURN(CLinearInterpolator::New((ILinearInterpolator**)&temp));
        }
        else if (name.Equals("accelerateInterpolator")) {
            FAIL_RETURN(CAccelerateInterpolator::New(res, theme, attrs, (IAccelerateInterpolator**)&temp));
        }
        else if (name.Equals("decelerateInterpolator")) {
            FAIL_RETURN(CDecelerateInterpolator::New(res, theme, attrs, (IDecelerateInterpolator**)&temp));
        }
        else if (name.Equals("accelerateDecelerateInterpolator")) {
            FAIL_RETURN(CAccelerateDecelerateInterpolator::New((IAccelerateDecelerateInterpolator**)&temp));
        }
        else if (name.Equals("cycleInterpolator")) {
            FAIL_RETURN(CCycleInterpolator::New(res, theme, attrs, (ICycleInterpolator**)&temp));
        }
        else if (name.Equals("anticipateInterpolator")) {
            FAIL_RETURN(CAnticipateInterpolator::New(res, theme, attrs, (IAnticipateInterpolator**)&temp));
        }
        else if (name.Equals("overshootInterpolator")) {
            FAIL_RETURN(COvershootInterpolator::New(res, theme, attrs, (IOvershootInterpolator**)&temp));
        }
        else if (name.Equals("anticipateOvershootInterpolator")) {
            FAIL_RETURN(CAnticipateOvershootInterpolator::New(res, theme, attrs, (IAnticipateOvershootInterpolator**)&temp));
        }
        else if (name.Equals("bounceInterpolator")) {
            FAIL_RETURN(CBounceInterpolator::New((IBounceInterpolator**)&temp));
        }
        else if (name.Equals("pathInterpolator")) {
            FAIL_RETURN(CPathInterpolator::New(res, theme, attrs, (IPathInterpolator**)&interpolator));
        }
        else {
            Logger::E("AnimationUtils", "Unknown interpolator name: %s", name.string());
            return E_RUNTIME_EXCEPTION;
        }
    }

    *interpolator = temp;
    REFCOUNT_ADD(*interpolator);
    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
