
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
    INTERFACE_ADDREF(*weakReference)
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
    bounds->SetEx(mBounds);
    return NOERROR;
}

AutoPtr<IRect> Drawable::CopyBounds()
{
    AutoPtr<IRect> r;
    ASSERT_SUCCEEDED(CRect::New(mBounds.Get(), (IRect**)&r));
    return r;
}

AutoPtr<IRect> Drawable::GetBounds()
{
    if (mBounds == ZERO_BOUNDS_RECT) {
        mBounds = NULL;
        ASSERT_SUCCEEDED(CRect::New((IRect**)&mBounds));
    }

    return mBounds;
}

ECode Drawable::SetChangingConfigurations(
    /* [in] */ Int32 configs)
{
    mChangingConfigurations = configs;
    return NOERROR;
}

Int32 Drawable::GetChangingConfigurations()
{
    return mChangingConfigurations;
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

AutoPtr<IDrawableCallback> Drawable::GetCallback()
{
    AutoPtr<IDrawableCallback> callback;
    if (mCallback != NULL) {
        mCallback->Resolve(EIID_IDrawableCallback, (IInterface**)&callback);
    }
    return callback;
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

Int32 Drawable::GetLayoutDirection()
{
    return mLayoutDirection;
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

Boolean Drawable::IsStateful()
{
    return FALSE;
}

Boolean Drawable::SetState(
    /* [in] */ ArrayOf<Int32>* stateSet)
{
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
        return OnStateChange(stateSet);
    }
    return FALSE;
}

AutoPtr< ArrayOf<Int32> > Drawable::GetState()
{
    return mStateSet;
}

ECode Drawable::JumpToCurrentState()
{
    return NOERROR;
}

AutoPtr<IDrawable> Drawable::GetCurrent()
{
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

Boolean Drawable::SetLevel(
    /* [in] */ Int32 level)
{
    if (mLevel != level) {
        mLevel = level;
        return OnLevelChange(level);
    }
    return FALSE;
}

Int32 Drawable::GetLevel()
{
    return mLevel;
}

Boolean Drawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = mVisible != visible;
    if (changed) {
        mVisible = visible;
        InvalidateSelf();
    }
    return changed;
}

Boolean Drawable::IsVisible()
{
    return mVisible;
}

Int32 Drawable::ResolveOpacity(
    /* [in] */ Int32 op1,
    /* [in] */ Int32 op2)
{
    if (op1 == op2) {
        return op1;
    }
    if (op1 == IPixelFormat::UNKNOWN || op2 == IPixelFormat::UNKNOWN) {
        return IPixelFormat::UNKNOWN;
    }
    if (op1 == IPixelFormat::TRANSLUCENT || op2 == IPixelFormat::TRANSLUCENT) {
        return IPixelFormat::TRANSLUCENT;
    }
    if (op1 == IPixelFormat::TRANSPARENT || op2 == IPixelFormat::TRANSPARENT) {
        return IPixelFormat::TRANSPARENT;
    }
    return IPixelFormat::OPAQUE;
}

AutoPtr<IRegion> Drawable::GetTransparentRegion()
{
    return NULL;
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

Int32 Drawable::GetIntrinsicWidth()
{
    return -1;
}

Int32 Drawable::GetIntrinsicHeight()
{
    return -1;
}

Int32 Drawable::GetMinimumWidth()
{
    Int32 intrinsicWidth = GetIntrinsicWidth();
    return intrinsicWidth > 0 ? intrinsicWidth : 0;
}

Int32 Drawable::GetMinimumHeight()
{
    Int32 intrinsicHeight = GetIntrinsicHeight();
    return intrinsicHeight > 0 ? intrinsicHeight : 0;
}

Boolean Drawable::GetPadding(
    /* [in, out] */ IRect* padding)
{
    padding->Set(0, 0, 0, 0);
    return FALSE;
}

AutoPtr<IInsets> Drawable::GetLayoutInsets()
{
    return Insets::NONE;
}

AutoPtr<IDrawable> Drawable::Mutate()
{
    return (IDrawable*)this->Probe(EIID_IDrawable);
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
            IBitmapDrawable::Probe(*drawable)->SetTargetDensityEx(metrics);
        }
    }
    else if (name.Equals("nine-patch")) {
        FAIL_RETURN(CNinePatchDrawable::New((INinePatchDrawable**)drawable));
        if (r != NULL) {
            AutoPtr<IDisplayMetrics> metrics;
            r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            INinePatchDrawable::Probe(*drawable)->SetTargetDensityEx(metrics);
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
    FAIL_RETURN(factory->DecodeFileEx(pathName, (IBitmap**)&bm));
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

AutoPtr<IDrawableConstantState> Drawable::GetConstantState()
{
    return NULL;
}

/////////////////////////////////////////////////////////////////////

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

Int32 Drawable::GetResId()
{
    return mResId;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
