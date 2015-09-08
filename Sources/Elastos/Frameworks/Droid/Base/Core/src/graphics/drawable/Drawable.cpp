
#include "ext/frameworkext.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CPorterDuffColorFilter.h"
#include "graphics/Insets.h"
#include "graphics/NinePatch.h"
#include "graphics/drawable/CBitmapDrawable.h"
#include "graphics/drawable/CColorDrawable.h"
#include "graphics/drawable/CNinePatchDrawable.h"
#include "graphics/drawable/CStateListDrawable.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"
#include "graphics/drawable/CLayerDrawable.h"
#include "graphics/drawable/CGradientDrawable.h"
#include "graphics/drawable/CClipDrawable.h"
#include "graphics/drawable/CAnimationDrawable.h"
#include "graphics/drawable/CLevelListDrawable.h"
#include "graphics/drawable/CTransitionDrawable.h"
#include "graphics/drawable/CScaleDrawable.h"
#include "graphics/drawable/CRotateDrawable.h"
#include "graphics/drawable/CInsetDrawable.h"
#include "util/StateSet.h"
#include "util/CDisplayMetrics.h"
#include "util/Xml.h"
#include "R.h"

using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_INTERFACE_IMPL_2(Drawable::ConstantState, IDrawableConstantState, IWeakReferenceSource)

ECode Drawable::ConstantState::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode Drawable::ConstantState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return NewDrawable(drawable);
}

AutoPtr<IRect> Init_ZERO_BOUNDS_RECT()
{
    AutoPtr<CRect> rect;
    CRect::NewByFriend((CRect**)&rect);
    return (IRect*)rect.Get();
}

CAR_INTERFACE_IMPL(Drawable, Object, IDrawable);
AutoPtr<IRect> Drawable::ZERO_BOUNDS_RECT = Init_ZERO_BOUNDS_RECT();
Drawable::Drawable()
    : mStateSet(const_cast<ArrayOf<Int32>*>(StateSet::WILD_CARD.Get()))
    , mLevel(0)
    , mChangingConfigurations(0)
    , mBounds(ZERO_BOUNDS_RECT)
    , mVisible(TRUE)
    , mLayoutDirection(0)
    , mResId(-1)
{}

Drawable::~Drawable()
{
    mStateSet = NULL;
    mBounds = NULL;
    mCallback = NULL;
}

ECode Drawable::SetBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IRect> oldBounds = mBounds;

    if (oldBounds == ZERO_BOUNDS_RECT) {
        mBounds = NULL;
        FAIL_RETURN(CRect::New((IRect**)&mBounds));
        oldBounds = mBounds;
    }

    CRect* oldBoundsObj = (CRect*)oldBounds.Get();
    if (oldBoundsObj->mLeft != left || oldBoundsObj->mTop != top ||
            oldBoundsObj->mRight != right || oldBoundsObj->mBottom != bottom) {
        mBounds->Set(left, top, right, bottom);
        OnBoundsChange(mBounds);
    }
    return NOERROR;
}

ECode Drawable::SetBounds(
    /* [in] */ IRect* bounds)
{
    CRect* boundsObj = (CRect*)bounds;
    return SetBounds(boundsObj->mLeft, boundsObj->mTop, boundsObj->mRight, boundsObj->mBottom);
}

ECode Drawable::CopyBounds(
    /* [in] */ IRect* bounds)
{
    bounds->Set(mBounds);
    return NOERROR;
}

ECode Drawable::CopyBounds(
    /* [out] */ IRect** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    return CRect::New(mBounds.Get(), bounds);
}

ECode Drawable::GetBounds(
    /* [out] */ IRect** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    if (mBounds == ZERO_BOUNDS_RECT) {
        mBounds = NULL;
        ASSERT_SUCCEEDED(CRect::New((IRect**)&mBounds));
    }

    *bounds = mBounds;
    REFCOUNT_ADD(*bounds);
    return NOERROR;
}

ECode Drawable::SetChangingConfigurations(
    /* [in] */ Int32 configs)
{
    mChangingConfigurations = configs;
    return NOERROR;
}

ECode Drawable::GetChangingConfigurations(
    /* [out] */ Int32* configs)
{
    VALIDATE_NOT_NULL(configs);
    *configs = mChangingConfigurations;
    return NOERROR;
}

ECode Drawable::SetDither(
    /* [in] */ Boolean dither)
{
    return NOERROR;
}

ECode Drawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return NOERROR;
}

ECode Drawable::SetCallback(
    /* [in] */ IDrawableCallback* cb)
{
    mCallback = NULL;
    if (cb != NULL) {
        IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(cb);
        assert(wrs != NULL && "IWeakReferenceSource was not implemented by DrawableCallback!");
        wrs->GetWeakReference((IWeakReference**)&mCallback);
    }

    return NOERROR;
}

ECode Drawable::GetCallback(
    /* [out] */ IDrawableCallback** callback)
{
    VALIDATE_NOT_NULL(callback);
    *callback = NULL;
    if (mCallback != NULL) {
        mCallback->Resolve(EIID_IDrawableCallback, (IInterface**)&callback);
    }
    return NOERROR;
}

ECode Drawable::InvalidateSelf()
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode Drawable::ScheduleSelf(
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->ScheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable),
                what, when);
    }
    return NOERROR;
}

ECode Drawable::UnscheduleSelf(
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable),
                what);
    }
    return NOERROR;
}

ECode Drawable::GetLayoutDirection(
    /* [out] */ Int32* dir)
{
    VALIDATE_NOT_NULL(dir);
    *dir = mLayoutDirection;
    return NOERROR;
}

ECode Drawable::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    if (GetLayoutDirection() != layoutDirection) {
        mLayoutDirection = layoutDirection;
    }

    return NOERROR;
}

ECode Drawable::SetColorFilter(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    AutoPtr<IPorterDuffColorFilter> filter;
    CPorterDuffColorFilter::New(color, mode, (IPorterDuffColorFilter**)&filter);
    return SetColorFilter(filter.Get());
}

ECode Drawable::ClearColorFilter()
{
    return SetColorFilter(NULL);
}

ECode Drawable::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    *isStateful = FALSE;
    return NOERROR;
}

ECode Drawable::SetState(
    /* [in] */ ArrayOf<Int32>* stateSet,
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    Boolean isEqual = TRUE;

    if (mStateSet.Get() == stateSet) {
        isEqual = TRUE;
    }
    else if (mStateSet == NULL || stateSet == NULL
            || mStateSet->GetLength() != stateSet->GetLength()) {
        isEqual = FALSE;
    }
    else {
        Int32 len = mStateSet->GetLength();
        for (Int32 i = 0; i < len; i++) {
            if ((*mStateSet)[i] != (*stateSet)[i]) {
                isEqual = FALSE;
            }
        }
    }

    if (!isEqual) {
        mStateSet = stateSet;
        *isStateful = OnStateChange(stateSet);
        return NOERROR;
    }
    *isStateful = FALSE;
    return NOERROR;
}

ECode Drawable::GetState(
    /* [out, callee] */ ArrayOf<Int32>** stateSet)
{
    VALIDATE_NOT_NULL(stateSet);
    *stateSet = mStateSet;
    REFCOUNT_ADD(*stateSet);
    return NOERROR;
}

ECode Drawable::JumpToCurrentState()
{
    return NOERROR;
}

ECode Drawable::GetCurrent(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode Drawable::SetLevel(
    /* [in] */ Int32 level,
    /* [out] */ Boolean* change)
{
    VALIDATE_NOT_NULL(change);
    if (mLevel != level) {
        mLevel = level;
        *change = OnLevelChange(level);
        return NOERROR;
    }

    *change = FALSE;
    return NOERROR;
}

ECode Drawable::GetLevel(
    /* [out] */ Int32* level)
{
    VALIDATE_NOT_NULL(level);
    *level = mLevel;
    return NOERROR;
}

ECode Drawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart,
    /* [out] */ Boolean* isDifferent)
{
    VALIDATE_NOT_NULL(isDifferent);
    Boolean changed = mVisible != visible;
    if (changed) {
        mVisible = visible;
        InvalidateSelf();
    }
    *isDifferent = changed;
    return NOERROR;
}

ECode Drawable::IsVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = mVisible;
    return NOERROR;
}

ECode Drawable::ResolveOpacity(
    /* [in] */ Int32 op1,
    /* [in] */ Int32 op2,
    /* [out] */ Int32* opacity)
{
    VALIDATE_NOT_NULL(opacity);
    if (op1 == op2) {
        *opacity = op1;
        return NOERROR;
    }
    if (op1 == IPixelFormat::UNKNOWN || op2 == IPixelFormat::UNKNOWN) {
        *opacity = IPixelFormat::UNKNOWN;
        return NOERROR;
    }
    if (op1 == IPixelFormat::TRANSLUCENT || op2 == IPixelFormat::TRANSLUCENT) {
        *opacity = IPixelFormat::TRANSLUCENT;
        return NOERROR;
    }
    if (op1 == IPixelFormat::TRANSPARENT || op2 == IPixelFormat::TRANSPARENT) {
        *opacity = IPixelFormat::TRANSPARENT;
        return NOERROR;
    }
    *opacity = IPixelFormat::OPAQUE;
    return NOERROR;
}

ECode Drawable::GetTransparentRegion(
    /* [out] */ IRegion** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    *bounds = NULL;
    return NOERROR;
}

Boolean Drawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    return FALSE;
}

Boolean Drawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    return FALSE;
}

void Drawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
}

ECode Drawable::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = -1;
    return NOERROR;
}

ECode Drawable::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = -1;
    return NOERROR;
}

ECode Drawable::GetMinimumWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    Int32 intrinsicWidth = 0;
    GetIntrinsicWidth(&intrinsicWidth);
    *width = intrinsicWidth > 0 ? intrinsicWidth : 0;
    return NOERROR;
}

ECode Drawable::GetMinimumHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    Int32 intrinsicHeight = 0;
    GetIntrinsicHeight(&intrinsicHeight);
    *height = intrinsicHeight > 0 ? intrinsicHeight : 0;
    return NOERROR;
}

ECode Drawable::GetPadding(
    /* [in, out] */ IRect* padding,
    /* [out] */ Boolean* isPadding)
{
    assert(padding != NULL);
    VALIDATE_NOT_NULL(isPadding);
    padding->Set(0, 0, 0, 0);
    *isPadding = FALSE;
    return NOERROR;
}

AutoPtr<IInsets> Drawable::GetLayoutInsets()
{
    return Insets::NONE;
}

ECode Drawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode Drawable::CreateFromStream(
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CreateFromResourceStream(NULL, NULL, is, srcName, NULL, drawable);
}

ECode Drawable::CreateFromResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CreateFromResourceStream(res, value, is, srcName, NULL, drawable);
}

ECode Drawable::CreateFromResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ const String& srcName,
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = NULL;

    if (is == NULL) {
        return NOERROR;
    }

    /*  ugh. The decodeStream contract is that we have already allocated
        the pad rect, but if the bitmap does not had a ninepatch chunk,
        then the pad will be ignored. If we could change this to lazily
        alloc/assign the rect, we could avoid the GC churn of making new
        Rects only to drop them on the floor.
    */
    AutoPtr<IRect> pad;
    CRect::New((IRect**)&pad);

    // Special stuff for compatibility mode: if the target density is not
    // the same as the display density, but the resource -is- the same as
    // the display density, then don't scale it down to the target density.
    // This allows us to load the system's density-correct resources into
    // an application in compatibility mode, without scaling those down
    // to the compatibility density only to have them scaled back up when
    // drawn to the screen.
    AutoPtr<IBitmapFactoryOptions> opts = _opts;
    if (opts == NULL) {
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&opts);
    }

    if (res != NULL) {
        AutoPtr<IDisplayMetrics> dis;
        res->GetDisplayMetrics((IDisplayMetrics**)&dis);
        assert(dis != NULL);
        Int32 dpi;
        dis->GetNoncompatDensityDpi(&dpi);
        opts->SetInScreenDensity(dpi);
    }
    else {
        opts->SetInScreenDensity(CDisplayMetrics::DENSITY_DEVICE);
    }

    AutoPtr<IBitmap> bm;
    AutoPtr<IBitmapFactory> factory;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    FAIL_RETURN(factory->DecodeResourceStream(
            res, value, is, pad, opts, (IBitmap**)&bm));
    if (bm != NULL) {
        AutoPtr< ArrayOf<Byte> > np;
        FAIL_RETURN(bm->GetNinePatchChunk((ArrayOf<Byte>**)&np));
        if (np == NULL || !NinePatch::IsNinePatchChunk(np)) {
            np = NULL;
            pad = NULL;
        }
        AutoPtr< ArrayOf<Int32> > layoutBounds;
        bm->GetLayoutBounds((ArrayOf<Int32>**)&layoutBounds);
        AutoPtr<IRect> layoutBoundsRect;
        if (layoutBounds != NULL) {
            CRect::New((*layoutBounds)[0], (*layoutBounds)[1], (*layoutBounds)[2],
                (*layoutBounds)[3], (IRect**)&layoutBoundsRect);
        }
        return DrawableFromBitmap(res, bm, np, pad, layoutBoundsRect, srcName, drawable);
    }

    return NOERROR;
}

ECode Drawable::CreateFromXml(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = NULL;

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type;
    FAIL_RETURN(parser->Next(&type));
    while (type != IXmlPullParser::START_TAG && type != IXmlPullParser::END_DOCUMENT) {
        // Empty loop
        FAIL_RETURN(parser->Next(&type));
    }

    if (type != IXmlPullParser::START_TAG) {
//        throw new XmlPullParserException("No start tag found");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    FAIL_RETURN(CreateFromXmlInner(r, parser, attrs, drawable));

    if (*drawable == NULL) {
//        throw new RuntimeException("Unknown initial tag: " + parser.getName());
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode Drawable::CreateFromXmlInner(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = NULL;

    String name;
    FAIL_RETURN(parser->GetName((String*)&name));

    if (name.Equals("selector")) {
        FAIL_RETURN(CStateListDrawable::New((IStateListDrawable**)drawable));
    }
    else if (name.Equals("level-list")) {
        FAIL_RETURN(CLevelListDrawable::New((ILevelListDrawable**)drawable));
    }
    /* Probably not doing this.
    } else if (name.equals("mipmap")) {
        drawable = new MipmapDrawable();
    }*/
    else if (name.Equals("layer-list")) {
        FAIL_RETURN(CLayerDrawable::New((ILayerDrawable**)drawable));
    }
    else if (name.Equals("transition")) {
        FAIL_RETURN(CTransitionDrawable::New((ITransitionDrawable**)drawable));
    }
    else if (name.Equals("color")) {
        FAIL_RETURN(CColorDrawable::New((IColorDrawable**)drawable));
    }
    else if (name.Equals("shape")) {
        FAIL_RETURN(CGradientDrawable::New((IGradientDrawable**)drawable));
    }
    else if (name.Equals("scale")) {
        FAIL_RETURN(CScaleDrawable::New((IScaleDrawable**)drawable));
    }
    else if (name.Equals("clip")) {
        FAIL_RETURN(CClipDrawable::New((IClipDrawable**)drawable));
    }
    else if (name.Equals("rotate")) {
        FAIL_RETURN(CRotateDrawable::New((IRotateDrawable**)drawable));;
    }
    else if (name.Equals("animated-rotate")) {
        FAIL_RETURN(CAnimatedRotateDrawable::New((IAnimatedRotateDrawable**)drawable));
    }
    else if (name.Equals("animation-list")) {
        FAIL_RETURN(CAnimationDrawable::New((IAnimationDrawable**)drawable));
    }
    else if (name.Equals("inset")) {
        FAIL_RETURN(CInsetDrawable::New((IInsetDrawable**)drawable));
    }
    else if (name.Equals("bitmap")) {
        FAIL_RETURN(CBitmapDrawable::New(r, (IBitmapDrawable**)drawable));
        if (r != NULL) {
            AutoPtr<IDisplayMetrics> metrics;
            r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            IBitmapDrawable::Probe(*drawable)->SetTargetDensity(metrics);
        }
    }
    else if (name.Equals("nine-patch")) {
        FAIL_RETURN(CNinePatchDrawable::New((INinePatchDrawable**)drawable));
        if (r != NULL) {
            AutoPtr<IDisplayMetrics> metrics;
            r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            INinePatchDrawable::Probe(*drawable)->SetTargetDensity(metrics);
        }
    }
     else {
//        throw new XmlPullParserException(parser.getPositionDescription() +
//                ": invalid drawable tag " + name);
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    return (*drawable)->Inflate(r, parser, attrs);
}

ECode Drawable::CreateFromPath(
    /* [in] */ const String& pathName,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = NULL;

    if (pathName.IsNull()) {
        return NOERROR;
    }

    AutoPtr<IBitmapFactory> factory;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    AutoPtr<IBitmap> bm;
    FAIL_RETURN(factory->DecodeFile(pathName, (IBitmap**)&bm));
    if (bm != NULL) {
        return DrawableFromBitmap(NULL, bm, NULL, NULL, NULL, pathName, drawable);
    }

    return NOERROR;
}

ECode Drawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Drawable),
            ARRAY_SIZE(R::styleable::Drawable));

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, attrIds, (ITypedArray**)&a));
    FAIL_RETURN(InflateWithAttributes(r, parser, a, R::styleable::Drawable_visible));
    return a->Recycle();
}

ECode Drawable::InflateWithAttributes(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ ITypedArray* attrs,
    /* [in] */ Int32 visibleAttr)
{
    return attrs->GetBoolean(visibleAttr, mVisible, &mVisible);
}

ECode Drawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    *state = NULL;
    return NOERROR;
}

ECode Drawable::DrawableFromBitmap(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bm,
    /* [in] */ ArrayOf<Byte>* np,
    /* [in] */ IRect* pad,
    /* [in] */ IRect* layoutBounds,
    /* [in] */ const String& srcName,
    /* [out] */ IDrawable** drawable)
{
    assert(drawable != NULL);
    if (np != NULL) {
        return CNinePatchDrawable::New(
            res, bm, np, pad, layoutBounds, srcName, (INinePatchDrawable**)drawable);
    }

    return CBitmapDrawable::New(res, bm, (IBitmapDrawable**)drawable);
}
ECode Drawable::SetResId(
    /* [in] */ Int32 resId)
{
    mResId = resId;
    return NOERROR;
}

ECode Drawable::GetResId(
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId);
    *resId = mResId;
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
