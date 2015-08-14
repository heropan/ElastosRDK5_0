
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

const String AnimatorInflater::TAG = String("AnimatorInflater");
const Int32 AnimatorInflater::TOGETHER = 0;
const Int32 AnimatorInflater::SEQUENTIALLY = 1;
const Int32 AnimatorInflater::VALUE_TYPE_FLOAT       = 0;
const Int32 AnimatorInflater::VALUE_TYPE_INT         = 1;
const Int32 AnimatorInflater::VALUE_TYPE_PATH        = 2;
const Int32 AnimatorInflater::VALUE_TYPE_COLOR       = 4;
const Int32 AnimatorInflater::VALUE_TYPE_CUSTOM      = 5;
const Boolean AnimatorInflater::DBG_ANIMATOR_INFLATER = FALSE;

CAR_INTERFACE_IMPL(AnimatorInflater::PathDataEvaluator, Object, ITypeEvaluator);
AnimatorInflater::PathDataEvaluator::PathDataEvaluator(
    /* [in] */ IPathParserPathDataNode* nodeArray)
    : mNodeArray(nodeArray)
{
}

ECode AnimatorInflater::PathDataEvaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* startPathData,
    /* [in] */ IInterface* endPathData,
    /* [out] */ IInterface** result)
{
    if (!PathParser::CanMorph(startPathData, endPathData)) {
        // throw new IllegalArgumentException("Can't interpolate between"
        //         + " two incompatible pathData");
        *result = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mNodeArray == NULL || !PathParser::CanMorph(mNodeArray, startPathData)) {
        mNodeArray = PathParser::DeepCopyNodes(startPathData);
    }

    for (Int32 i = 0; i < startPathData.length; i++) {
        mNodeArray[i].interpolatePathDataNode(startPathData[i],
                endPathData[i], fraction);
    }

    *result = mNodeArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode AnimatorInflater::LoadAnimator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IAnimator** animator) /*throws NotFoundException*/
{
    VALIDATE_NOT_NULL(*animator);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IResourcesTheme> rt;
    context->GetTheme((IResourcesTheme**)&rt);
    return LoadAnimator(res, rt, id, animator);
}

ECode AnimatorInflater::LoadAnimator(
    /* [in] */ IResources* resources,
    /* [in] */ ITheme* theme,
    /* [in] */ Int32 id,
    /* [out] */ IAnimator** animator) /*throws NotFoundException*/
{
    VALIDATE_NOT_NULL(*animator);
    return LoadAnimator(resources, theme, id, 1, animator);
}

ECode AnimatorInflater::LoadAnimator(
    /* [in] */ IResources* resources,
    /* [in] */ ITheme* theme,
    /* [in] */ Int32 id,
    /* [in] */ Float pathErrorScale,
    /* [out] */ IAnimator** animator) /*throws NotFoundException*/
{
    VALIDATE_NOT_NULL(*animator);
    AutoPtr<IXmlResourceParser> parser;
    ECode ec = NOERROR;
    // try {
    ec = resources->GetAnimation(id, (IXmlResourceParser**)&parser);
    FAIL_GOTO(ec, error);
    ec = CreateAnimatorFromXml(resources, theme, parser, pathErrorScale, animator);
    // } catch (XmlPullParserException ex) {
    //     Resources.NotFoundException rnf =
    //             new Resources.NotFoundException("Can't load animation resource ID #0x" +
    //                     Integer.toHexString(id));
    //     rnf.initCause(ex);
    //     throw rnf;
    // } catch (IOException ex) {
    //     Resources.NotFoundException rnf =
    //             new Resources.NotFoundException("Can't load animation resource ID #0x" +
    //                     Integer.toHexString(id));
    //     rnf.initCause(ex);
    //     throw rnf;
    // } finally {
    //     if (parser != null) parser.close();
    // }

error:
    if (parser != NULL) {
        parser->Close();
    }

    if (ec != NOERROR) {
        *animator = NULL;
        return E_RESOURCES_NOT_FOUND_EXCEPTION;
    }

    return NOERROR;
}

ECode AnimatorInflater::LoadStateListAnimator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IStateListAnimator** animator) /*throws NotFoundException*/
{
    VALIDATE_NOT_NULL(*animator);
    AutoPtr<IXmlResourceParser> parser;
    ECode ec = NOERROR;
    // try {
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    ec = res->GetAnimation(id, (IXmlResourceParser**)&parser);
    FAIL_GOTO(ec, error);
    ec = CreateStateListAnimatorFromXml(context, parser, Xml.asAttributeSet(parser), animator);
    // } catch (XmlPullParserException ex) {
    //     Resources.NotFoundException rnf =
    //             new Resources.NotFoundException(
    //                     "Can't load state list animator resource ID #0x" +
    //                             Integer.toHexString(id)
    //             );
    //     rnf.initCause(ex);
    //     throw rnf;
    // } catch (IOException ex) {
    //     Resources.NotFoundException rnf =
    //             new Resources.NotFoundException(
    //                     "Can't load state list animator resource ID #0x" +
    //                             Integer.toHexString(id)
    //             );
    //     rnf.initCause(ex);
    //     throw rnf;
    // } finally {
    //     if (parser != null) {
    //         parser.close();
    //     }
    // }
error:
    if (parser != NULL) {
        parser->Close();
    }

    if (ec != NOERROR) {
        *animator = NULL;
        return E_RESOURCES_NOT_FOUND_EXCEPTION;
    }

    return NOERROR;
}

ECode AnimatorInflater::CreateStateListAnimatorFromXml(
    /* [in] */ IContext* context,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attributeSet,
    /* [out] */ IStateListAnimator** animator) /*throws IOException, XmlPullParserException*/
{
    VALIDATE_NOT_NULL(animator);
    Int32 type;
    AutoPtr<IStateListAnimator> stateListAnimator;
    CStateListAnimator::New((IStateListAnimator**)&stateListAnimator);

    while (TRUE) {
        FAIL_RETURN(parser->Next(&type));
        switch (type) {
            case IXmlPullParser::END_DOCUMENT:
            case IXmlPullParser::END_TAG: {
                *animator = stateListAnimator;
                REFCOUNT_ADD(*animator);
                return NOERROR;
            }

            case IXmlPullParser::START_TAG:
                // parse item
                AutoPtr<IAnimator> animator;
                String name;
                FAIL_RETURN(parser->GetName(&name));
                if (name.Equals("item")) {
                    Int32 attributeCount = 0;
                    FAIL_RETURN(parser->GetAttributeCount(&attributeCount));
                    AutoPtr<ArrayOf<Int32> > states = ArrayOf<Int32>::Alloc(attributeCount);
                    Int32 stateIndex = 0;
                    for (Int32 i = 0; i < attributeCount; i++) {
                        Int32 attrName = 0;
                        FAIL_RETURN(attributeSet->GetAttributeNameResource(i, &attrName));
                        if (attrName == R::attr::animation) {
                            Int32 v = 0;
                            FAIL_RETURN(attributeSet->GetAttributeResourceValue(i, 0, &v));
                            FAIL_RETURN(LoadAnimator(context, v, animator));
                        } else {
                            Boolean v = FALSE;
                            FAIL_RETURN(attributeSet->GetAttributeBooleanValue(i, FALSE, &v));
                            (*states)[stateIndex++] = v ? attrName : -attrName;
                        }

                    }
                    if (animator == NULL) {
                        AutoPtr<IResources> res;
                        context->GetResources((IResources**)&res);
                        AutoPtr<IResourcesTheme> theme;
                        context->GetTheme((IResourcesTheme**)&theme);
                        FAIL_RETURN(CreateAnimatorFromXml(res, theme, parser, 1f, animator));
                    }

                    if (animator == NULL) {
                        return E_RESOURCES_NOT_FOUND_EXCEPTION;
                        // throw new Resources.NotFoundException(
                        //         "animation state item must have a valid animation");
                    }
                    stateListAnimator->AddState(StateSet::TrimStateSet(states, stateIndex), animator);

                }
                break;
        }
    }

    return NOERROR;
}

void AnimatorInflater::ParseAnimatorFromTypeArray(
    /* [in] */ IValueAnimator* anim,
    /* [in] */ ITypedArray* arrayAnimator,
    /* [in] */ ITypedArray* arrayObjectAnimator,
    /* [in] */ Float pixelSize)
{
    Int64 duration = 0;
    arrayAnimator->GetInt32(R::styleable::Animator_duration, 300, &duration);

    Int64 startDelay = 0;
    arrayAnimator->GetInt32(R::styleable::Animator_startOffset, 0, &startDelay);

    Int32 valueType = 0;
    arrayAnimator->GetInt32(R::styleable::Animator_valueType, VALUE_TYPE_FLOAT, &valueType);

    AutoPtr<ITypeEvaluator> evaluator;

    Boolean getFloats = (valueType == VALUE_TYPE_FLOAT);

    AutoPtr<ITypedValue> tvFrom;
    arrayAnimator->PeekValue(R::styleable::Animator_valueFrom, (ITypedValue**)&tvFrom);
    Boolean hasFrom = (tvFrom != NULL);
    Int32 fromType = 0;
    if (hasFrom) {
        tvFrom->GetType(&fromType);
    }

    AutoPtr<ITypedValue> tvTo;
    arrayAnimator->PeekValue(R::styleable::Animator_valueTo, (ITypedValue**)&tvTo);
    Boolean hasTo = (tvTo != NULL);
    Int32 toType = 0;
    if (hasTo) {
        tvTo->GetType(&toType);
    }

    // TODO: Further clean up this part of code into 4 types : path, color,
    // integer and float.
    if (valueType == VALUE_TYPE_PATH) {
        evaluator = SetupAnimatorForPath(anim, arrayAnimator);
    } else {
        // Integer and float value types are handled here.
        if ((hasFrom && (fromType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                (fromType <= ITypedValue::TYPE_LAST_COLOR_INT)) ||
                (hasTo && (toType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                        (toType <= ITypedValue::TYPE_LAST_COLOR_INT))) {
            // special case for colors: ignore valueType and get ints
            getFloats = FALSE;
            evaluator = CArgbEvaluator::GetInstance();
        }
        SetupValues(anim, arrayAnimator, getFloats, hasFrom, fromType, hasTo, toType);
    }

    anim->SetDuration(duration);
    anim->SetStartDelay(startDelay);

    Boolean hasValue = FALSE;
    if (arrayAnimator->HasValue(R::styleable::Animator_repeatCount, &hasValue), hasValue) {
        Int32 v = 0;
        arrayAnimator->GetInt32(R::styleable::Animator_repeatCount, 0, &v);
        anim->SetRepeatCount(v;);
    }
    if (arrayAnimator->HasValue(R.styleable.Animator_repeatMode, &hasValue), hasValue) {
        Int32 v = 0;
        arrayAnimator->GetInt32(R::styleable::Animator_repeatMode, IValueAnimator::RESTART, &v);
        anim->SetRepeatMode(v);
    }
    if (evaluator != NULL) {
        anim->SetEvaluator(evaluator);
    }

    if (arrayObjectAnimator != NULL) {
        SetupObjectAnimator(anim, arrayObjectAnimator, getFloats, pixelSize);
    }
}

ECode AnimatorInflater::SetupAnimatorForPath(
    /* [in] */ IValueAnimator* anim,
    /* [in] */ ITypedArray* arrayAnimator,
    /* [out] */ ITypeEvaluator** te)
{
    VALIDATE_NOT_NULL(te);
    AutoPtr<ITypeEvaluator> evaluator;
    String fromString;
    arrayAnimator->GetString(R::styleable::Animator_valueFrom, &fromString);
    String toString;
    arrayAnimator->GetString(R::styleable::Animator_valueTo, &toString);
    AutoPtr<ArrayOf<IPathParserPathDataNode> > nodesFrom = PathParser::CreateNodesFromPathData(fromString);
    AutoPtr<ArrayOf<IPathParserPathDataNode> > nodesTo = PathParser::CreateNodesFromPathData(toString);

    if (nodesFrom != NULL) {
        if (nodesTo != NULL) {
            anim->SetObjectValues(nodesFrom, nodesTo);
            if (!PathParser::CanMorph(nodesFrom, nodesTo)) {
                // throw new InflateException(arrayAnimator.getPositionDescription()
                //         + " Can't morph from " + fromString + " to " + toString);
                *te = NULL;
                return E_INFLATE_EXCEPTION;
            }
        } else {
            anim->SetObjectValues((Object)nodesFrom);
        }
        CPathDataEvaluator::New(PathParser::DeepCopyNodes(nodesFrom), te);
    } else if (nodesTo != NULL) {
        anim->SetObjectValues(nodesTo);
        CPathDataEvaluator::New(PathParser::DeepCopyNodes(nodesTo), te);
    }

    if (DBG_ANIMATOR_INFLATER && evaluator != NULL) {
        Log::V(TAG, "create a new PathDataEvaluator here");
    }

    return NOERROR;
}

ECode AnimatorInflater::SetupObjectAnimator(
    /* [in] */ IValueAnimator* anim,
    /* [in] */ ITypedArray* arrayObjectAnimator,
    /* [in] */ Boolean getFloats,
    /* [in] */ Float pixelSize)
{
    AutoPtr<IObjectAnimator> oa = IObjectAnimator::Probe(anim);
    String pathData;
    arrayObjectAnimator->GetString(R::styleable::PropertyAnimator_pathData, &pathData);

    // Note that if there is a pathData defined in the Object Animator,
    // valueFrom / valueTo will be ignored.
    if (pathData != NULL) {
        String propertyXName;
        arrayObjectAnimator->GetString(R::styleable::PropertyAnimator_propertyXName, &propertyXName);
        String propertyYName;
        arrayObjectAnimator->GetString(R::styleable::PropertyAnimator_propertyYName, &propertyYName);

        if (propertyXName == NULL && propertyYName == NULL) {
            // throw new InflateException(arrayObjectAnimator.getPositionDescription()
            //         + " propertyXName or propertyYName is needed for PathData");
            return E_INFLATE_EXCEPTION;
        } else {
            AutoPtr<IPath> path = PathParser::CreatePathFromPathData(pathData);
            Float error = 0.5f * pixelSize; // max half a pixel error
            AutoPtr<IPathKeyframes> keyframeSet = KeyframeSet::OfPath(path, error);
            AutoPtr<IKeyframes> xKeyframes;
            AutoPtr<IKeyframes> yKeyframes;
            if (getFloats) {
                keyframeSet->CreateXFloatKeyframes((IKeyframes**)&xKeyframes);
                keyframeSet->CreateYFloatKeyframes((IKeyframes**)&yKeyframes);
            } else {
                keyframeSet->CreateXIntKeyframes((IKeyframes**)&xKeyframes);
                keyframeSet->CreateYIntKeyframes((IKeyframes**)&yKeyframes);
            }
            AutoPtr<IPropertyValuesHolder> x;
            AutoPtr<IPropertyValuesHolder> y;
            if (propertyXName != NULL) {
                x = PropertyValuesHolder::OfKeyframes(propertyXName, xKeyframes);
            }
            if (propertyYName != NULL) {
                y = PropertyValuesHolder::OfKeyframes(propertyYName, yKeyframes);
            }
            if (x == NULL) {
                oa->SetValues(y);
            } else if (y == NULL) {
                oa->SetValues(x);
            } else {
                oa->SetValues(x, y);
            }
        }
    } else {
        String propertyName;
        arrayObjectAnimator->GetString(R::styleable::PropertyAnimator_propertyName, &propertyName);
        oa->SetPropertyName(propertyName);
    }

    return NOERROR;
}

void AnimatorInflater::SetupValues(
    /* [in] */ IValueAnimator* anim,
    /* [in] */ ITypedArray* arrayAnimator,
    /* [in] */ Boolean getFloats,
    /* [in] */ Boolean hasFrom,
    /* [in] */ Int32 fromType,
    /* [in] */ Boolean hasTo,
    /* [in] */ Int32 toType)
{
    Int32 valueFromIndex = R::styleable::Animator_valueFrom;
    Int32 valueToIndex = R::styleable::Animator_valueTo;
    if (getFloats) {
        Float valueFrom;
        Float valueTo;
        if (hasFrom) {
            if (fromType == ITypedValue::TYPE_DIMENSION) {
                arrayAnimator->GetDimension(valueFromIndex, 0f, &valueFrom);
            } else {
                arrayAnimator->GetFloat(valueFromIndex, 0f, &valueFrom);
            }
            if (hasTo) {
                if (toType == ITypedValue::TYPE_DIMENSION) {
                    arrayAnimator->GetDimension(valueToIndex, 0f, &valueTo);
                } else {
                    arrayAnimator->GetFloat(valueToIndex, 0f, &valueTo);
                }
                anim->SetFloatValues(valueFrom, valueTo);
            } else {
                anim->SetFloatValues(valueFrom);
            }
        } else {
            if (toType == ITypedValue::TYPE_DIMENSION) {
                arrayAnimator->GetDimension(valueToIndex, 0f, &valueTo);
            } else {
                arrayAnimator->GetFloat(valueToIndex, 0f, &valueTo);
            }
            anim->SetFloatValues(valueTo);
        }
    } else {
        Int32 valueFrom;
        Int32 valueTo;
        if (hasFrom) {
            Float tmp = 0f;
            if (fromType == ITypedValue::TYPE_DIMENSION) {
                arrayAnimator->GetDimension(valueFromIndex, 0f, &tmp);
                valueFrom = tmp;
            } else if ((fromType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                    (fromType <= ITypedValue::TYPE_LAST_COLOR_INT)) {
                arrayAnimator->GetColor(valueFromIndex, 0, &valueFrom);
            } else {
                arrayAnimator->GetInt32(valueFromIndex, 0, &valueFrom);
            }
            if (hasTo) {
                if (toType == ITypedValue::TYPE_DIMENSION) {
                    arrayAnimator->GetDimension(valueToIndex, 0f, &tmp);
                    valueTo = tmp;
                } else if ((toType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                        (toType <= ITypedValue::TYPE_LAST_COLOR_INT)) {
                    arrayAnimator->GetColor(valueToIndex, 0, &valueTo);
                } else {
                    arrayAnimator->GetInt32(valueToIndex, 0, &valueTo);
                }
                anim->SetIntValues(valueFrom, valueTo);
            } else {
                anim->SetIntValues(valueFrom);
            }
        } else {
            if (hasTo) {
                if (toType == ITypedValue::TYPE_DIMENSION) {
                    arrayAnimator->GetDimension(valueToIndex, 0f, &tmp);
                    valueTo = tmp;
                } else if ((toType >= ITypedValue::TYPE_FIRST_COLOR_INT) &&
                        (toType <= ITypedValue::TYPE_LAST_COLOR_INT)) {
                    arrayAnimator->GetColor(valueToIndex, 0, &valueTo);
                } else {
                    arrayAnimator->GetInt32(valueToIndex, 0, &valueTo);
                }
                anim->SetIntValues(valueTo);
            }
        }
    }
}

ECode AnimatorInflater::CreateAnimatorFromXml(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Float pixelSize,
    /* [out] */ IAnimator** animator) /*throws XmlPullParserException, IOException */
{
    VALIDATE_NOT_NULL(animator);
    AutoPtr<IAttributeSet> set;
    Xml::AsAttributeSet(parser, (IAttributeSet**)&set);
    return CreateAnimatorFromXml(res, theme, parser, set, NULL, 0,
            pixelSize, animator);
}

ECode AnimatorInflater::CreateAnimatorFromXml(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IAnimatorSet* parent,
    /* [in] */ Int32 sequenceOrdering,
    /* [in] */ Float pixelSize,
    /* [out] */ IAnimator** animator) /*throws XmlPullParserException, IOException*/
{
    VALIDATE_NOT_NULL(animator);
    *animator = NULL;
    // ArrayList<Animator> childAnims = null;
    List<AutoPtr<IAnimator> > childAnims;

    // Make sure we are on a start tag.
    Int32 type;
    Int32 depth = 0, tmpD = 0;
    parser->GetDepth(&depth);
    ECode ec = NOERROR;

    while (((parser->Next(&type), type) != IXmlPullParser::END_TAG || (parser->GetDepth(&tmpD) ,tmpD) > depth)
            && type != IXmlPullParser::END_DOCUMENT) {

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);

        if (name.Equals(String("objectAnimator"))) {
            FAIL_RETURN(LoadObjectAnimator(res, theme, attrs, pixelSize, animator));
        } else if (name.Equals(String("animator"))) {
            FAIL_RETURN(LoadAnimator(res, theme, attrs, null, pixelSize, animator));
        } else if (name.Equals(String("set"))) {
            CAnimatorSet::New((IAnimator**)&animator);
            AutoPtr<ITypedArray> a;
            if (theme != NULL) {
                theme->ObtainStyledAttributes(attrs, R::styleable::AnimatorSet, 0, 0, (ITypedArray**)&a);
            } else {
                res->ObtainAttributes(attrs, R::styleable::AnimatorSet, (ITypedArray**)&a);
            }
            Int32 ordering = 0;
            a->GetInt32(R::styleable::AnimatorSet_ordering, TOGETHER, &ordering);
            ec = CreateAnimatorFromXml(res, theme, parser, attrs, IAnimatorSet::Probe(*animator), ordering,
                    pixelSize);
            a->Recycle();
            if (FAILED(ec)) {
                return ec;
            }
        } else {
            // throw new RuntimeException("Unknown animator name: " + parser.getName());
            return E_RUNTIME_EXCEPTION;
        }

        if (parent != NULL) {
            // if (childAnims == null) {
            //     childAnims = new ArrayList<Animator>();
            // }
            childAnims.PushBack(*animator);
        }
    }
    if (parent != NULL/* && childAnims != null*/) {
        AutoPtr<ArrayOf<IAnimator*> > animsArray = ArrayOf<IAnimator*>::Alloc(childAnims.GetSize());
        Int32 index = 0;
        List<AutoPtr<IAnimator> >::Iterator ator = childAnims.Begin();
        for (; ator != childAnims.End(); ++ator) {
            animsArray->Set(index++, *ator);
        }
        if (sequenceOrdering == TOGETHER) {
            parent->PlayTogether(animsArray);
        } else {
            parent->PlaySequentially(animsArray);
        }
    }

    return NOERROR;

}

ECode AnimatorInflater::LoadObjectAnimator(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Float pathErrorScale,
    /* [out] */ IObjectAnimator** animator) /*throws NotFoundException*/
{
    VALIDATE_NOT_NULL(animator);
    CObjectAnimator::New((IObjectAnimator**)&animator);

    AutoPtr<IAnimator> tmp;
    return LoadAnimator(res, theme, attrs, *animator, pathErrorScale, (IAnimator**)&tmp);
}

ECode AnimatorInflater::LoadAnimator(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IValueAnimator* anim,
    /* [in] */ Float pathErrorScale,
    /* [out] */ IValueAnimator** animator) /*throws NotFoundException*/
{
    VALIDATE_NOT_NULL(animator);
    *animator = NULL;
    Int32 resID = 0;
    AutoPtr<ITypedArray> arrayAnimator;
    AutoPtr<ITypedArray> arrayObjectAnimator;
    ECode ec = NOERROR;
    if (theme != NULL) {
        ec = theme->ObtainStyledAttributes(attrs, R::styleable::Animator, 0, 0, (ITypedArray**)&arrayAnimator);
        FAIL_GOTO(ec, error);
    } else {
        ec = res->ObtainAttributes(attrs, R::styleable::Animator, (ITypedArray**)&arrayAnimator);
        FAIL_GOTO(ec, error);
    }

    // If anim is not null, then it is an object animator.
    if (anim != NULL) {
        if (theme != NULL) {
            ec = theme->ObtainStyledAttributes(attrs,
                    R::styleable::PropertyAnimator, 0, 0, (ITypedArray**)&arrayObjectAnimator);
            FAIL_GOTO(ec, error);
        } else {
            ec = res->ObtainAttributes(attrs, R::styleable::PropertyAnimator, (ITypedArray**)&arrayObjectAnimator);
            FAIL_GOTO(ec, error);
        }
    }

    if (anim == NULL) {
        CValueAnimator::New((IAnimator**)&anim);
    }

    ParseAnimatorFromTypeArray(anim, arrayAnimator, arrayObjectAnimator, pathErrorScale);

    ec = arrayAnimator->GetResourceId(R::styleable::Animator_interpolator, 0, &resID);
    FAIL_GOTO(ec, error);
    if (resID > 0) {
        ec = anim->SetInterpolator(AnimationUtils::LoadInterpolator(res, theme, resID));
        FAIL_GOTO(ec, error);
    }

error:
    arrayAnimator->Recycle();
    if (arrayObjectAnimator != NULL) {
        arrayObjectAnimator->Recycle();
    }

    *animator = anim;
    REFCOUNT_ADD(*animator);
    return ec;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
