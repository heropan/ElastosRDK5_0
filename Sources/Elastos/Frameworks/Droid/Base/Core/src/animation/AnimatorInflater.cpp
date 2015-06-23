#include "animation/AnimatorInflater.h"
#include <elastos/utility/etl/List.h>
#include "R.h"
#include "view/animation/AnimationUtils.h"
#include "animation/CAnimatorSet.h"
#include "animation/CValueAnimator.h"
#include "animation/CArgbEvaluator.h"
#include "animation/CObjectAnimator.h"

using Elastos::Utility::Etl::List;
using Elastos::Droid::R;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::View::Animation::AnimationUtils;

namespace Elastos {
namespace Droid {
namespace Animation {

const Int32 AnimatorInflater::TOGETHER = 0;
const Int32 AnimatorInflater::SEQUENTIALLY = 1;
const Int32 AnimatorInflater::VALUE_TYPE_FLOAT       = 0;
const Int32 AnimatorInflater::VALUE_TYPE_INT         = 1;
const Int32 AnimatorInflater::VALUE_TYPE_COLOR       = 4;
const Int32 AnimatorInflater::VALUE_TYPE_CUSTOM      = 5;


AutoPtr<IAnimator> AnimatorInflater::LoadAnimator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id)
{
    assert(context != NULL);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    assert(res != NULL);

    AutoPtr<IXmlResourceParser> parser;
    res->GetAnimation(id, (IXmlResourceParser**)&parser);
    AutoPtr<IAnimator> a = CreateAnimatorFromXml(context, parser);
    // } catch (XmlPullParserException ex) {
    //     Resources.NotFoundException rnf =
    //             new Resources.NotFoundException("Can't load animation resource ID #0x" +
    //             Integer.toHexString(id));
    //     rnf.initCause(ex);
    //     throw rnf;
    // } catch (IOException ex) {
    //     Resources.NotFoundException rnf =
    //             new Resources.NotFoundException("Can't load animation resource ID #0x" +
    //             Integer.toHexString(id));
    //     rnf.initCause(ex);
    //     throw rnf;
    // } finally {

    if (parser != NULL) {
        parser->Close();
    }

    return a;
}

AutoPtr<IAnimator> AnimatorInflater::CreateAnimatorFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser)
{
    return CreateAnimatorFromXml(c, parser, Xml::AsAttributeSet(parser), NULL, 0);
}

AutoPtr<IAnimator> AnimatorInflater::CreateAnimatorFromXml(
    /* [in] */ IContext* c,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IAnimatorSet* parent,
    /* [in] */ Int32 sequenceOrdering)
{
    AutoPtr<IAnimator> anim;
    List<AutoPtr<IAnimator> > childAnims;

    // Make sure we are on a start tag.
    Int32 type = 0;
    Int32 depth = 0, curDepth = 0;
    parser->GetDepth(&depth);
    String name;
    while (((parser->Next(&type), type) != IXmlPullParser::END_TAG
            || (parser->GetDepth(&curDepth), curDepth) > depth)
           && type != IXmlPullParser::END_DOCUMENT) {

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        parser->GetName(&name);

        if (name.Equals("objectAnimator")) {
            anim = LoadObjectAnimator(c, attrs);
        } else if (name.Equals("animator")) {
            anim = LoadAnimator(c, attrs, NULL);
        } else if (name.Equals("set")) {
            AutoPtr<IAnimatorSet> animatorSet;
            CAnimatorSet::New((IAnimatorSet**)&animatorSet);
            anim = IAnimator::Probe(animatorSet.Get());

            AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
                    const_cast<Int32 *>(R::styleable::AnimatorSet),
                    ARRAY_SIZE(R::styleable::AnimatorSet));

            AutoPtr<ITypedArray> a;
            c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
            Int32 ordering = 0;
            a->GetInt32(R::styleable::AnimatorSet_ordering, TOGETHER, &ordering);
            CreateAnimatorFromXml(c, parser, attrs, animatorSet,  ordering);
            a->Recycle();
        } else {
            //throw new RuntimeException("Unknown animator name: " + parser.getName());
            assert(0 && "Unknown animator name: ");
        }

        if (parent != NULL) {
            childAnims.PushBack(anim);
        }
    }

    Int32 size = childAnims.GetSize();
    if (parent != NULL && size > 0) {
        AutoPtr<ArrayOf<IAnimator*> > animsArray = ArrayOf<IAnimator *>::Alloc(size);
        List<AutoPtr<IAnimator> >::Iterator it = childAnims.Begin();
        for (Int32 i = 0; it != childAnims.End(); it++, i++) {
            AutoPtr<IAnimator> a = *it;
            animsArray->Set(i, a);
        }

        if (sequenceOrdering == TOGETHER) {
            parent->PlayTogether(animsArray);
        } else {
            parent->PlaySequentially(animsArray);
        }
    }

    return anim;
}

AutoPtr<IObjectAnimator> AnimatorInflater::LoadObjectAnimator(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IObjectAnimator> anim;
    CObjectAnimator::New((IObjectAnimator**)&anim);

    LoadAnimator(context, attrs, anim);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PropertyAnimator),
            ARRAY_SIZE(R::styleable::PropertyAnimator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    String propertyName;
    a->GetString(R::styleable::PropertyAnimator_propertyName, &propertyName);
    anim->SetPropertyName(propertyName);

    a->Recycle();

    return anim;
}

AutoPtr<IValueAnimator> AnimatorInflater::LoadAnimator(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IValueAnimator* _anim)
{
    AutoPtr<IValueAnimator> anim = _anim;

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Animator),
            ARRAY_SIZE(R::styleable::Animator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    Int32 value = 0;
    a->GetInt32(R::styleable::Animator_duration, 0, &value);
    Int64 duration = value;

    a->GetInt32(R::styleable::Animator_startOffset, 0, &value);
    Int64 startDelay = value;

    Int32 valueType = 0;
    a->GetInt32(R::styleable::Animator_valueType, VALUE_TYPE_FLOAT, &valueType);

    if (anim == NULL) {
        CValueAnimator::New((IValueAnimator**)&anim);
    }

    AutoPtr<ITypeEvaluator> evaluator;

    Int32 valueFromIndex = R::styleable::Animator_valueFrom;
    Int32 valueToIndex = R::styleable::Animator_valueTo;

    Boolean getFloats = (valueType == VALUE_TYPE_FLOAT);

    AutoPtr<ITypedValue> tvFrom;
    a->PeekValue(valueFromIndex, (ITypedValue**)&tvFrom);

    Boolean hasFrom = (tvFrom != NULL);
    Int32 fromType = 0;
    if (hasFrom) {
        tvFrom->GetType(&fromType);
    }

    AutoPtr<ITypedValue> tvTo;
    a->PeekValue(valueToIndex, (ITypedValue**)&tvTo);
    Boolean hasTo = (tvTo != NULL);
    Int32 toType = 0;
    if (hasTo) {
        tvTo->GetType(&toType);
    }

    if ((hasFrom && (fromType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
            (fromType <= ITypedValue::TYPE_LAST_COLOR_INT)) ||
        (hasTo && (toType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
            (toType <= ITypedValue::TYPE_LAST_COLOR_INT))) {
        // special case for colors: ignore valueType and get ints
        getFloats = FALSE;
        AutoPtr<IArgbEvaluator> argb;
        CArgbEvaluator::New((IArgbEvaluator**)&argb);
        anim->SetEvaluator(argb);
    }

    if (getFloats) {
        Float valueFrom = 0.0f;
        Float valueTo = 0.0f;
        if (hasFrom) {
            if (fromType == ITypedValue::TYPE_DIMENSION) {
                a->GetDimension(valueFromIndex, 0.0f, &valueFrom);
            } else {
                a->GetFloat(valueFromIndex, 0.0f, &valueFrom);
            }
            if (hasTo) {
                if (toType == ITypedValue::TYPE_DIMENSION) {
                    a->GetDimension(valueToIndex, 0.0f, &valueTo);
                } else {
                    a->GetFloat(valueToIndex, 0.0f, &valueTo);
                }

                AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(2);
                values->Set(0, valueFrom);
                values->Set(1, valueTo);
                anim->SetFloatValues(values);
            } else {
                AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
                values->Set(0, valueFrom);
                anim->SetFloatValues(values);
            }
        } else {
            if (toType == ITypedValue::TYPE_DIMENSION) {
                a->GetDimension(valueToIndex, 0.0f, &valueTo);
            } else {
                a->GetFloat(valueToIndex, 0.0f, &valueTo);
            }

            AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
            values->Set(0, valueTo);
            anim->SetFloatValues(values);
        }
    } else {
        Int32 valueFrom = 0;
        Int32 valueTo = 0;
        Float value;
        if (hasFrom) {
            if (fromType == ITypedValue::TYPE_DIMENSION) {
                a->GetDimension(valueFromIndex, 0.0f, &value);
                valueFrom = (Int32)value;
            } else if ((fromType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                    (fromType <= ITypedValue::TYPE_LAST_COLOR_INT)) {
                a->GetColor(valueFromIndex, 0, &valueFrom);
            } else {
                a->GetInt32(valueFromIndex, 0, &valueFrom);
            }
            if (hasTo) {
                if (toType == ITypedValue::TYPE_DIMENSION) {
                    a->GetDimension(valueToIndex, 0.0f, &value);
                    valueTo = (Int32)value;
                } else if ((toType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                        (toType <= ITypedValue::TYPE_LAST_COLOR_INT)) {
                    a->GetColor(valueToIndex, 0, &valueTo);
                } else {
                    a->GetInt32(valueToIndex, 0, &valueTo);
                }

                AutoPtr<ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(2);
                values->Set(0, valueFrom);
                values->Set(1, valueTo);
                anim->SetInt32Values(values);
            } else {
                AutoPtr<ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(1);
                values->Set(0, valueFrom);
                anim->SetInt32Values(values);
            }
        } else {
            if (hasTo) {
                if (toType == ITypedValue::TYPE_DIMENSION) {
                    a->GetDimension(valueToIndex, 0.0f, &value);
                    valueTo = (Int32)value;
                } else if ((toType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                    (toType <= ITypedValue::TYPE_LAST_COLOR_INT)) {
                    a->GetColor(valueToIndex, 0, &valueTo);
                } else {
                    a->GetInt32(valueToIndex, 0, &valueTo);
                }

                AutoPtr<ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(1);
                values->Set(0, valueTo);
                anim->SetInt32Values(values);
            }
        }
    }

    anim->SetDuration(duration);
    anim->SetStartDelay(startDelay);

    Boolean has = FALSE;
    a->HasValue(R::styleable::Animator_repeatCount, &has);
    if (has) {
        Int32 count = 0;
        a->GetInt32(R::styleable::Animator_repeatCount, 0, &count);
        anim->SetRepeatCount(count);
    }

    a->HasValue(R::styleable::Animator_repeatMode, &has);
    if (has) {
        Int32 mode = 0;
        a->GetInt32(R::styleable::Animator_repeatMode, IValueAnimator::ANIMATION_RESTART, &mode);
        anim->SetRepeatMode(mode);
    }

    if (evaluator != NULL) {
        anim->SetEvaluator(evaluator);
    }

    Int32 resID = 0;
    a->GetResourceId(R::styleable::Animator_interpolator, 0, &resID);
    if (resID > 0) {
        AutoPtr<Elastos::Droid::View::Animation::IInterpolator> iplt;
        AnimationUtils::LoadInterpolator(context, resID, (Elastos::Droid::View::Animation::IInterpolator**)&iplt);
        anim->SetInterpolator(iplt);
    }

    a->Recycle();

    return anim;
}


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
