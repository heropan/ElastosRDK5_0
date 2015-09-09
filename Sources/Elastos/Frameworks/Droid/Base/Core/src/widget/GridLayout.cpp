#include "widget/GridLayout.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include "graphics/Insets.h"
#include "graphics/CPaint.h"
#include "graphics/CColor.h"
#include "widget/CGridLayoutLayoutParams.h"
#include "view/ViewGroupLayoutParams.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::Graphics::IInsets;
using Elastos::Droid::Graphics::Insets;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::CColor;
using Elastos::Droid::Widget::IGridLayoutLayoutParams;
using Elastos::Droid::Widget::CGridLayoutLayoutParams;
using Elastos::Droid::Widget::GridLayout;
using Elastos::Droid::View::ViewGroupLayoutParams;

DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(GridLayout::Interval)

namespace Elastos {
namespace Droid {
namespace Widget {

// {06572623-710a-42c0-9b32-fab6e1a8a303}
extern "C" const InterfaceID EIID_GridLayout =
        { 0x06572623, 0x710a, 0x42c0, { 0x9b, 0x32, 0xfa, 0xb6, 0xe1, 0xa8, 0xa3, 0x03 } };

static AutoPtr<GridLayout::Alignment> InitUndefine()
{
    AutoPtr<GridLayout::Alignment> ment = new GridLayout::UndefinedAlignment();
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitLeading()
{
    AutoPtr<GridLayout::Alignment> ment = new GridLayout::LeadingAlignment();
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitTrailing()
{
    AutoPtr<GridLayout::Alignment> ment = new GridLayout::TrailingAlignment();
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitCenter()
{
    AutoPtr<GridLayout::Alignment> ment = new GridLayout::CenterAlignment();
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitBaseLine()
{
    AutoPtr<GridLayout::Alignment> ment = new GridLayout::BaseLineAlignment();
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitFill()
{
    AutoPtr<GridLayout::Alignment> ment = new GridLayout::FillAlignment();
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitLeft()
{
    AutoPtr<GridLayout::Alignment> ment = GridLayout::CreateSwitchingAlignment(GridLayout::START, GridLayout::END);
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitRight()
{
    AutoPtr<GridLayout::Alignment> ment = GridLayout::CreateSwitchingAlignment(GridLayout::END, GridLayout::START);
    return ment;
}

static AutoPtr<GridLayout::Alignment> InitTop()
{
    if (GridLayout::LEADING) {
        return GridLayout::LEADING;
    }
    return NULL;
}

static AutoPtr<GridLayout::Alignment> InitBottom()
{
    if (GridLayout::TRAILING) {
        return GridLayout::TRAILING;
    }
    return NULL;
}

const String GridLayout::TAG("GridLayout");
const Int32 GridLayout::HORIZONTAL;
const Int32 GridLayout::VERTICAL;
const Int32 GridLayout::UNDEFINED = Elastos::Core::Math::INT32_MIN_VALUE;
const Int32 GridLayout::ALIGN_BOUNDS;
const Int32 GridLayout::ALIGN_MARGINS;
const Int32 GridLayout::MAX_SIZE;
const Int32 GridLayout::DEFAULT_CONTAINER_MARGIN;
const Int32 GridLayout::UNINITIALIZED_HASH;
const Int32 GridLayout::DEFAULT_ORIENTATION;
const Int32 GridLayout::DEFAULT_COUNT = UNDEFINED;
const Boolean GridLayout::DEFAULT_USE_DEFAULT_MARGINS = FALSE;
const Boolean GridLayout::DEFAULT_ORDER_PRESERVED;
const Int32 GridLayout::DEFAULT_ALIGNMENT_MODE;
const Int32 GridLayout::ORIENTATION;
const Int32 GridLayout::ROW_COUNT;
const Int32 GridLayout::COLUMN_COUNT;
const Int32 GridLayout::USE_DEFAULT_MARGINS;
const Int32 GridLayout::ALIGNMENT_MODE;
const Int32 GridLayout::ROW_ORDER_PRESERVED;
const Int32 GridLayout::COLUMN_ORDER_PRESERVED;
const AutoPtr<GridLayout::Alignment> GridLayout::UNDEFINED_ALIGNMENT = InitUndefine(); // = UndefinedAlignment
const AutoPtr<GridLayout::Alignment> GridLayout::LEADING = InitLeading(); // = LeadingAlignment
const AutoPtr<GridLayout::Alignment> GridLayout::TRAILING = InitTrailing(); // = TrailingAlignment
const AutoPtr<GridLayout::Alignment> GridLayout::TOP = InitTop(); //= LEADING
const AutoPtr<GridLayout::Alignment> GridLayout::BOTTOM = InitBottom(); //= TRAILING
const AutoPtr<GridLayout::Alignment> GridLayout::START = InitTop(); //= LEADING
const AutoPtr<GridLayout::Alignment> GridLayout::END = InitBottom(); //= TRAILING
const AutoPtr<GridLayout::Alignment> GridLayout::LEFT = InitLeft(); //= createSwitchingAlignment(START, END);
const AutoPtr<GridLayout::Alignment> GridLayout::RIGHT = InitRight(); //= createSwitchingAlignment(END, START);
const AutoPtr<GridLayout::Alignment> GridLayout::CENTER = InitCenter(); // = CenterAlignment
const AutoPtr<GridLayout::Alignment> GridLayout::BASELINE = InitBaseLine(); // = BaseLineAlignment
const AutoPtr<GridLayout::Alignment> GridLayout::FILL = InitFill(); // = FillAlignment*/
const Int32 GridLayout::INFLEXIBLE;
const Int32 GridLayout::CAN_STRETCH;

GridLayout::GridLayout()
    : mOrientation(DEFAULT_ORIENTATION)
    , mUseDefaultMargins(DEFAULT_USE_DEFAULT_MARGINS)
    , mAlignmentMode(DEFAULT_ALIGNMENT_MODE)
    , mDefaultGap(0)
    , mLastLayoutParamsHashCode(UNINITIALIZED_HASH)
{
    mHorizontalAxis = new Axis(TRUE, this);
    mVerticalAxis = new Axis(FALSE, this);
}

GridLayout::GridLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mOrientation(DEFAULT_ORIENTATION)
    , mUseDefaultMargins(DEFAULT_USE_DEFAULT_MARGINS)
    , mAlignmentMode(DEFAULT_ALIGNMENT_MODE)
    , mDefaultGap(0)
    , mLastLayoutParamsHashCode(UNINITIALIZED_HASH)
{
    mHorizontalAxis = new Axis(TRUE, this);
    mVerticalAxis = new Axis(FALSE, this);
    Init(context, attrs, defStyle);
}

ECode GridLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ViewGroup::Init(context, attrs, defStyle);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDimensionPixelOffset(R::dimen::default_gap, &mDefaultGap);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::GridLayout),
            ARRAY_SIZE(R::styleable::GridLayout));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));

//    try {
    Int32 rowCount = 0;
    a->GetInt32(ROW_COUNT, DEFAULT_COUNT, &rowCount);
    SetRowCount(rowCount);
    Int32 columnCount = 0;
    a->GetInt32(COLUMN_COUNT, DEFAULT_COUNT, &columnCount);
    SetColumnCount(columnCount);
    Int32 orientation = 0;
    a->GetInt32(ORIENTATION, DEFAULT_ORIENTATION, &orientation);
    SetOrientation(orientation);
    Boolean margins = FALSE;
    a->GetBoolean(USE_DEFAULT_MARGINS, DEFAULT_USE_DEFAULT_MARGINS, &margins);
    SetUseDefaultMargins(margins);
    Int32 alignmentMode = 0;
    a->GetInt32(ALIGNMENT_MODE, DEFAULT_ALIGNMENT_MODE, &alignmentMode);
    SetAlignmentMode(alignmentMode);
    Boolean rowOrderPreserved = FALSE;
    a->GetBoolean(ROW_ORDER_PRESERVED, DEFAULT_ORDER_PRESERVED, &rowOrderPreserved);
    SetRowOrderPreserved(rowOrderPreserved);
    Boolean columnOrderPreserved = FALSE;
    a->GetBoolean(COLUMN_ORDER_PRESERVED, DEFAULT_ORDER_PRESERVED, &columnOrderPreserved);
    SetColumnOrderPreserved(columnOrderPreserved);
//        } finally {
    a->Recycle();
//        }
    return NOERROR;
}

Int32 GridLayout::GetOrientation()
{
   return mOrientation;
}

ECode GridLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    if (mOrientation != orientation) {
        mOrientation = orientation;
        InvalidateStructure();
        RequestLayout();
    }
    return NOERROR;
}

Int32 GridLayout::GetRowCount()
{
    return mVerticalAxis->GetCount();
}

ECode GridLayout::SetRowCount(
    /* [in] */ Int32 rowCount)
{
    mVerticalAxis->SetCount(rowCount);
    InvalidateStructure();
    RequestLayout();
    return NOERROR;
}

Int32 GridLayout::GetColumnCount()
{
    return mHorizontalAxis->GetCount();
}

ECode GridLayout::SetColumnCount(
    /* [in] */ Int32 columnCount)
{
    mHorizontalAxis->SetCount(columnCount);
    InvalidateStructure();
    RequestLayout();
    return NOERROR;
}

Boolean GridLayout::GetUseDefaultMargins()
{
    return mUseDefaultMargins;
}

ECode GridLayout::SetUseDefaultMargins(
    /* [in] */ Boolean useDefaultMargins)
{
    mUseDefaultMargins = useDefaultMargins;
    RequestLayout();
    return NOERROR;
}

Int32 GridLayout::GetAlignmentMode()
{
    return mAlignmentMode;
}

ECode GridLayout::SetAlignmentMode(
    /* [in] */ Int32 alignmentMode)
{
    mAlignmentMode = alignmentMode;
    RequestLayout();
    return NOERROR;
}

Boolean GridLayout::IsRowOrderPreserved()
{
    return mVerticalAxis->IsOrderPreserved();;
}

ECode GridLayout::SetRowOrderPreserved(
    /* [in] */ Boolean rowOrderPreserved)
{
    mVerticalAxis->SetOrderPreserved(rowOrderPreserved);
    InvalidateStructure();
    RequestLayout();
    return NOERROR;
}

Boolean GridLayout::IsColumnOrderPreserved()
{
    return mHorizontalAxis->IsOrderPreserved();
}

ECode GridLayout::SetColumnOrderPreserved(
    /* [in] */ Boolean columnOrderPreserved)
{
    mHorizontalAxis->SetOrderPreserved(columnOrderPreserved);
    InvalidateStructure();
    RequestLayout();
    return NOERROR;
}

Int32 GridLayout::Max2(
    /* [in] */ ArrayOf<Int32>* a,
    /* [in] */ Int32 valueIfEmpty)
{
    Int32 result = valueIfEmpty;
    for (Int32 i = 0, N = a->GetLength(); i < N; i++) {
        result = Elastos::Core::Math::Max(result, (*a)[i]);
    }
    return result;
}

AutoPtr<GridLayout::Alignment> GridLayout::GetAlignment(
    /* [in] */ Int32 gravity,
    /* [in] */ Boolean horizontal)
{
    Int32 mask = horizontal ? IGravity::HORIZONTAL_GRAVITY_MASK : IGravity::VERTICAL_GRAVITY_MASK;
    Int32 shift = horizontal ? IGravity::AXIS_X_SHIFT : IGravity::AXIS_Y_SHIFT;
    Int32 flags = (gravity & mask) >> shift;
    switch (flags) {
        case (IGravity::AXIS_SPECIFIED | IGravity::AXIS_PULL_BEFORE):
            return horizontal ? LEFT : TOP;
        case (IGravity::AXIS_SPECIFIED | IGravity::AXIS_PULL_AFTER):
            return horizontal ? RIGHT : BOTTOM;
        case (IGravity::AXIS_SPECIFIED | IGravity::AXIS_PULL_BEFORE | IGravity::AXIS_PULL_AFTER):
            return FILL;
        case IGravity::AXIS_SPECIFIED:
            return CENTER;
        case (IGravity::AXIS_SPECIFIED | IGravity::AXIS_PULL_BEFORE | IGravity::RELATIVE_LAYOUT_DIRECTION):
            return START;
        case (IGravity::AXIS_SPECIFIED | IGravity::AXIS_PULL_AFTER | IGravity::RELATIVE_LAYOUT_DIRECTION):
            return END;
        default:
            return UNDEFINED_ALIGNMENT;
    }
}

Int32 GridLayout::GetMargin1(
    /* [in] */ IView* view,
    /* [in] */ Boolean horizontal,
    /* [in] */ Boolean leading)
{
    AutoPtr<IViewGroupLayoutParams> lp = GetLayoutParams(view);
    AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp.Get();
    Int32 margin =  horizontal ?
            (leading ? glp->mLeftMargin : glp->mRightMargin) :
            (leading ? glp->mTopMargin : glp->mBottomMargin);
    return margin == UNDEFINED ? GetDefaultMargin(view, lp, horizontal, leading) : margin;
}

AutoPtr<IViewGroupLayoutParams> GridLayout::GetLayoutParams(
    /* [in] */ IView* c)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    c->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    return lp;
}

ECode GridLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IGridLayoutLayoutParams> lp;
    FAIL_RETURN(CGridLayoutLayoutParams::New(GetContext(), attrs, (IGridLayoutLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

Int32 GridLayout::GetMeasurementIncludingMargin(
    /* [in] */ IView* c,
    /* [in] */ Boolean horizontal)
{
    Int32 visibility = 0;
    c->GetVisibility(&visibility);
    if (visibility == IView::GONE) {
        return 0;
    }
    return GetMeasurement(c, horizontal) + GetTotalMargin(c, horizontal);
}

ECode GridLayout::RequestLayout()
{
    ViewGroup::RequestLayout();
    InvalidateValues();
    return NOERROR;
}

AutoPtr<GridLayout::Alignment> GridLayout::GetAlignment(
    /* [in] */ Alignment* alignment,
    /* [in] */ Boolean horizontal)
{
    if (alignment != UNDEFINED_ALIGNMENT) {
        return alignment;
    } else {
        if (horizontal) {
            return START;
        } else {
            return BASELINE;
        }
    }
}

ECode GridLayout::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewGroup::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("GridLayout"), (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode GridLayout::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewGroup::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("GridLayout"), (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

void GridLayout::OnSetLayoutParams(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* layoutParams)
{
    ViewGroup::OnSetLayoutParams(child, layoutParams);

    if (!CheckLayoutParams(layoutParams)) {
        String wt = String("supplied LayoutParams are of the wrong type");
        HandleInvalidParams(wt);
    }
    InvalidateStructure();
}

Boolean GridLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    if (!IGridLayoutLayoutParams::Probe(p)) {
        return FALSE;
    }
    AutoPtr<IGridLayoutLayoutParams> lp = (IGridLayoutLayoutParams*)p;

    CheckLayoutParams(lp, TRUE);
    CheckLayoutParams(lp, FALSE);
    return TRUE;
}

AutoPtr<IViewGroupLayoutParams> GridLayout::GenerateDefaultLayoutParams()
{
    AutoPtr<IGridLayoutLayoutParams> lp;
    CGridLayoutLayoutParams::New((IGridLayoutLayoutParams**)&lp);
    return lp;
}

AutoPtr<IViewGroupLayoutParams> GridLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IGridLayoutLayoutParams> lp;
    CGridLayoutLayoutParams::New(p, (IGridLayoutLayoutParams**)&lp);
    return lp;
}

void GridLayout::OnDebugDrawMargins(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IGridLayoutLayoutParams> lp;
    CGridLayoutLayoutParams::New((IGridLayoutLayoutParams**)&lp);
    for (Int32 i = 0; i < GetChildCount(); i++) {
        AutoPtr<IView> c = GetChildAt(i);
        AutoPtr<IInsets> insets;
        if (GetLayoutMode() == OPTICAL_BOUNDS) {
            c->GetOpticalInsets((IInsets**)&insets);
        } else {
            insets = Insets::NONE;
        }
        AutoPtr<Insets> in = (Insets*)insets.Get();
        lp->SetMargins(
                GetMargin1(c, TRUE, TRUE) - in->mLeft,
                GetMargin1(c, FALSE, TRUE) - in->mTop,
                GetMargin1(c, TRUE, FALSE) - in->mRight,
                GetMargin1(c, FALSE, FALSE) - in->mBottom);
        lp->OnDebugDraw(c, canvas);
    }
}

void GridLayout::OnDebugDraw(
    /* [in] */ ICanvas* canvas)
{
    Int32 height = GetHeight() - GetPaddingTop() - GetPaddingBottom();
    Int32 width = GetWidth() - GetPaddingLeft() - GetPaddingRight();

    AutoPtr<IPaint> paint;
    CPaint::New((IPaint**)&paint);
    paint->SetStyle(PaintStyle_STROKE);
    Int32 color = 0;
    AutoPtr<IColor> co;
    CColor::AcquireSingleton((IColor**)&co);
    co->Argb(50, 255, 255, 255, &color);
    paint->SetColor(color);

    AutoPtr< ArrayOf<Int32> > xs = mHorizontalAxis->mLocations;
    if (xs) {
        for (Int32 i = 0, length = xs->GetLength(); i < length; i++) {
            Int32 x = (*xs)[i];
            DrawLine(canvas, x, 0, x, height - 1, paint);
        }
    }

    AutoPtr< ArrayOf<Int32> > ys = mVerticalAxis->mLocations;
    if (ys) {
        for (Int32 i = 0, length = ys->GetLength(); i < length; i++) {
            Int32 y = (*ys)[i];
            DrawLine(canvas, 0, y, width - 1, y, paint);
        }
    }

    ViewGroup::OnDebugDraw(canvas);
}

void GridLayout::OnViewAdded(
    /* [in] */ IView* child)
{
    ViewGroup::OnViewAdded(child);
    InvalidateStructure();
}

void GridLayout::OnViewRemoved(
    /* [in] */ IView* child)
{
    ViewGroup::OnViewRemoved(child);
    InvalidateStructure();
}

void GridLayout::OnChildVisibilityChanged(
    /* [in] */ IView* child,
    /* [in] */ Int32 oldVisibility,
    /* [in] */ Int32 newVisibility)
{
    ViewGroup::OnChildVisibilityChanged(child, oldVisibility, newVisibility);
    if (oldVisibility == IView::GONE || newVisibility == IView::GONE) {
        InvalidateStructure();
    }
}

void GridLayout::OnMeasure(
    /* [in] */ Int32 widthSpec,
    /* [in] */ Int32 heightSpec)
{
    ConsistencyCheck();

    InvalidateValues();

    MeasureChildrenWithMargins(widthSpec, heightSpec, TRUE);

    Int32 width = 0, height = 0;

    if (mOrientation == HORIZONTAL) {
        width = mHorizontalAxis->GetMeasure(widthSpec);
        MeasureChildrenWithMargins(widthSpec, heightSpec, FALSE);
        height = mVerticalAxis->GetMeasure(heightSpec);
    } else {
        height = mVerticalAxis->GetMeasure(heightSpec);
        MeasureChildrenWithMargins(widthSpec, heightSpec, FALSE);
        width = mHorizontalAxis->GetMeasure(widthSpec);
    }

    Int32 hPadding = GetPaddingLeft() + GetPaddingRight();
    Int32 vPadding = GetPaddingTop() + GetPaddingBottom();

    Int32 measuredWidth = Elastos::Core::Math::Max(hPadding + width, GetSuggestedMinimumWidth());
    Int32 measuredHeight = Elastos::Core::Math::Max(vPadding + height, GetSuggestedMinimumHeight());

    SetMeasuredDimension(
            ResolveSizeAndState(measuredWidth, widthSpec, 0),
            ResolveSizeAndState(measuredHeight, heightSpec, 0));
}

void GridLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    ConsistencyCheck();

    Int32 targetWidth = mRight - mLeft;
    Int32 targetHeight = mBottom - mTop;

    Int32 paddingLeft = GetPaddingLeft();
    Int32 paddingTop = GetPaddingTop();
    Int32 paddingRight = GetPaddingRight();
    Int32 paddingBottom = GetPaddingBottom();

    mHorizontalAxis->Layout(targetWidth - paddingLeft - paddingRight);
    mVerticalAxis->Layout(targetHeight - paddingTop - paddingBottom);

    AutoPtr< ArrayOf<Int32> > hLocations = mHorizontalAxis->GetLocations();
    AutoPtr< ArrayOf<Int32> > vLocations = mVerticalAxis->GetLocations();

    for (Int32 i = 0, N = GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = GetChildAt(i);
        Int32 visibility = 0;
        c->GetVisibility(&visibility);
        if (visibility == IView::GONE) continue;
        AutoPtr<IViewGroupLayoutParams> lp = GetLayoutParams(c);
        AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)(IGridLayoutLayoutParams*)lp.Get();
        AutoPtr<Spec> columnSpec = (Spec*)glp->mColumnSpec.Get();
        AutoPtr<Spec> rowSpec = (Spec*)glp->mRowSpec.Get();

        AutoPtr<Interval> colSpan = columnSpec->mSpan;
        AutoPtr<Interval> rowSpan = rowSpec->mSpan;

        Int32 x1 = (*hLocations)[colSpan->mMin];
        Int32 y1 = (*vLocations)[rowSpan->mMin];

        Int32 x2 = (*hLocations)[colSpan->mMax];
        Int32 y2 = (*vLocations)[rowSpan->mMax];

        Int32 cellWidth = x2 - x1;
        Int32 cellHeight = y2 - y1;

        Int32 pWidth = GetMeasurement(c, TRUE);
        Int32 pHeight = GetMeasurement(c, FALSE);

        AutoPtr<Alignment> hAlign = GetAlignment(columnSpec->mAlignment, TRUE);
        AutoPtr<Alignment> vAlign = GetAlignment(rowSpec->mAlignment, FALSE);

        AutoPtr<Bounds> boundsX = mHorizontalAxis->GetGroupBounds()->GetValue(i);
        AutoPtr<Bounds> boundsY = mVerticalAxis->GetGroupBounds()->GetValue(i);

        Int32 gravityOffsetX = 0;
        hAlign->GetGravityOffset(c, cellWidth - boundsX->Size(TRUE), &gravityOffsetX);
        Int32 gravityOffsetY = 0;
        vAlign->GetGravityOffset(c, cellHeight - boundsY->Size(TRUE), &gravityOffsetY);

        Int32 leftMargin = GetMargin(c, TRUE, TRUE);
        Int32 topMargin = GetMargin(c, FALSE, TRUE);
        Int32 rightMargin = GetMargin(c, TRUE, FALSE);
        Int32 bottomMargin = GetMargin(c, FALSE, FALSE);

        Int32 sumMarginsX = leftMargin + rightMargin;
        Int32 sumMarginsY = topMargin + bottomMargin;

        Int32 alignmentOffsetX = boundsX->GetOffset((IGridLayout*)this->Probe(EIID_IGridLayout), c, hAlign, pWidth + sumMarginsX, TRUE);
        Int32 alignmentOffsetY = boundsY->GetOffset((IGridLayout*)this->Probe(EIID_IGridLayout), c, vAlign, pHeight + sumMarginsY, FALSE);

        Int32 width = 0;
        hAlign->GetSizeInCell(c, pWidth, cellWidth - sumMarginsX, &width);

        Int32 height = 0;
        vAlign->GetSizeInCell(c, pHeight, cellHeight - sumMarginsY, &height);


        Int32 dx = x1 + gravityOffsetX + alignmentOffsetX;

        Int32 cx = !IsLayoutRtl() ? paddingLeft + leftMargin + dx :
                targetWidth - width - paddingRight - rightMargin - dx;


        Int32 cy = paddingTop + y1 + gravityOffsetY + alignmentOffsetY + topMargin;
        Boolean useLayoutBounds = GetLayoutMode() == OPTICAL_BOUNDS;
        if (useLayoutBounds) {
            AutoPtr<IInsets> insets;
            c->GetOpticalInsets((IInsets**)&insets);
            AutoPtr<Insets> in = (Insets*)insets.Get();
            cx -= in->mLeft;
            cy -= in->mTop;
            width += (in->mLeft + in->mRight);
            height += (in->mTop + in->mBottom);
        }
        Int32 mw = 0, mh = 0;
        c->GetMeasuredWidth(&mw);
        c->GetMeasuredHeight(&mh);
        if (width != mw || height != mh) {
            c->Measure(View::MeasureSpec::MakeMeasureSpec(width, View::MeasureSpec::EXACTLY),
                View::MeasureSpec::MakeMeasureSpec(height, View::MeasureSpec::EXACTLY));
        }
        c->Layout(cx, cy, cx + width, cy + height);
    }
}

Int32 GridLayout::GetDefaultMargin(
    /* [in] */ IView* c,
    /* [in] */ Boolean horizontal,
    /* [in] */ Boolean leading)
{
    // is right?
    InterfaceID id;
    c->GetInterfaceID(c, &id);
    if (id == EIID_ISpace) {
        return 0;
    }
    return mDefaultGap / 2;
}

Int32 GridLayout::GetDefaultMargin(
    /* [in] */ IView* c,
    /* [in] */ Boolean isAtEdge,
    /* [in] */ Boolean horizontal,
    /* [in] */ Boolean leading)
{
    return isAtEdge ? DEFAULT_CONTAINER_MARGIN : GetDefaultMargin(c, horizontal, leading);
}

Int32 GridLayout::GetDefaultMargin(
    /* [in] */ IView* c,
    /* [in] */ IViewGroupLayoutParams* p,
    /* [in] */ Boolean horizontal,
    /* [in] */ Boolean leading)
{
    if (!mUseDefaultMargins) {
        return 0;
    }
    AutoPtr<CGridLayoutLayoutParams> lp = (CGridLayoutLayoutParams*)p;
    AutoPtr<Spec> spec = horizontal ? (Spec*)lp->mColumnSpec.Get() : (Spec*)lp->mRowSpec.Get();
    AutoPtr<Axis> axis = horizontal ? mHorizontalAxis : mVerticalAxis;
    AutoPtr<Interval> span = spec->mSpan;
    Boolean leading1 = (horizontal && IsLayoutRtl()) ? !leading : leading;
    Boolean isAtEdge = leading1 ? (span->mMin == 0) : (span->mMax == axis->GetCount());

    return GetDefaultMargin(c, isAtEdge, horizontal, leading);
}

Int32 GridLayout::GetMargin(
    /* [in] */ IView* view,
    /* [in] */ Boolean horizontal,
    /* [in] */ Boolean leading)
{
    if (mAlignmentMode == ALIGN_MARGINS) {
        return GetMargin1(view, horizontal, leading);
    } else {
        AutoPtr<Axis> axis = horizontal ? mHorizontalAxis : mVerticalAxis;
        AutoPtr< ArrayOf<Int32> > margins = leading ? axis->GetLeadingMargins() : axis->GetTrailingMargins();
        AutoPtr<IViewGroupLayoutParams> lp = GetLayoutParams(view);
        AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp.Get();
        AutoPtr<Spec> spec = horizontal ? (Spec*)glp->mColumnSpec.Get() : (Spec*)glp->mRowSpec.Get();
        Int32 index = leading ? spec->mSpan->mMin : spec->mSpan->mMax;
        return (*margins)[index];
    }
}

Int32 GridLayout::GetTotalMargin(
    /* [in] */ IView* child,
    /* [in] */ Boolean horizontal)
{
    return GetMargin(child, horizontal, TRUE) + GetMargin(child, horizontal, FALSE);
}

Boolean GridLayout::Fits(
    /* [in] */ ArrayOf<Int32>* a,
    /* [in] */ Int32 value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (end > a->GetLength()) {
        return FALSE;
    }
    for (Int32 i = start; i < end; i++) {
        if ((*a)[i] > value) {
            return FALSE;
        }
    }
    return TRUE;
}

void GridLayout::ProcrusteanFill(
    /* [in] */ ArrayOf<Int32>* a,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 value)
{
    Int32 length = a->GetLength();
    if(start < 0 || end > length) {
        /*throw new ArrayIndexOutOfBoundsException("start < 0 || end > len."
                    + " start=" + start + ", end=" + end + ", len=" + len);*/
    }
    if (start > end) {
        //throw new IllegalArgumentException("start > end: " + start + " > " + end);
    }

    Int32 newStart = Elastos::Core::Math::Min(start, length);
    Int32 newEnd = Elastos::Core::Math::Min(end, length);

    for(Int32 i = newStart; i < newEnd; i++) {
        (*a)[i] = value;
    }
}

void GridLayout::SetCellGroup(
    /* [in] */ IViewGroupLayoutParams* lp,
    /* [in] */ Int32 row,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 col,
    /* [in] */ Int32 colSpan)
{
    AutoPtr<Interval> rowItv = new Interval(row, row + rowSpan);
    AutoPtr<Interval> colItv = new Interval(col, col + colSpan);
    AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp;
    glp->SetRowSpecSpan(rowItv);
    glp->SetColumnSpecSpan(colItv);
}

Int32 GridLayout::Clip(
    /* [in] */ Interval* minorRange,
    /* [in] */ Boolean minorWasDefined,
    /* [in] */ Int32 count)
{
    Int32 size = minorRange->Size();
    if (count == 0) {
        return size;
    }
    Int32 min = minorWasDefined ? Elastos::Core::Math::Min(minorRange->mMin, count) : 0;
    return Elastos::Core::Math::Min(size, count - min);
}

void GridLayout::ValidateLayoutParams()
{
    Boolean horizontal = (mOrientation == HORIZONTAL);
    AutoPtr<Axis> axis = horizontal ? mHorizontalAxis : mVerticalAxis;
    Int32 count = (axis->mDefinedCount != UNDEFINED) ? axis->mDefinedCount : 0;

    Int32 major = 0;
    Int32 minor = 0;
    AutoPtr< ArrayOf<Int32> > maxSizes = ArrayOf<Int32>::Alloc(count);

    for (Int32 i = 0, N = GetChildCount(); i < N; i++) {
        AutoPtr<IView> v = GetChildAt(i);
        AutoPtr<IViewGroupLayoutParams> lp;
        v->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp.Get();
        AutoPtr<Spec> majorSpec = horizontal ? (Spec*)glp->mRowSpec.Get() : (Spec*)glp->mColumnSpec.Get();
        AutoPtr<Interval> majorRange = majorSpec->mSpan;
        Boolean majorWasDefined = majorSpec->mStartDefined;
        Int32 majorSpan = majorRange->Size();
        if (majorWasDefined) {
            major = majorRange->mMin;
        }

        AutoPtr<Spec> minorSpec = horizontal ? (Spec*)glp->mColumnSpec.Get() : (Spec*)glp->mRowSpec.Get();
        AutoPtr<Interval> minorRange = minorSpec->mSpan;
        Boolean minorWasDefined = minorSpec->mStartDefined;
        Int32 minorSpan = Clip(minorRange, minorWasDefined, count);
        if (minorWasDefined) {
            minor = minorRange->mMin;
        }

        if (count != 0) {
            if (!majorWasDefined || !minorWasDefined) {
                while (!Fits(maxSizes, major, minor, minor + minorSpan)) {
                    if (minorWasDefined) {
                        major++;
                    } else {
                        if (minor + minorSpan <= count) {
                            minor++;
                        } else {
                            minor = 0;
                            major++;
                        }
                    }
                }
            }
            ProcrusteanFill(maxSizes, minor, minor + minorSpan, major + majorSpan);
        }

        if (horizontal) {
            SetCellGroup(lp, major, majorSpan, minor, minorSpan);
        } else {
            SetCellGroup(lp, minor, minorSpan, major, majorSpan);
        }

        minor = minor + minorSpan;
    }
}

void GridLayout::InvalidateStructure()
{
    mLastLayoutParamsHashCode = UNINITIALIZED_HASH;
    mHorizontalAxis->InvalidateStructure();
    mVerticalAxis->InvalidateStructure();
    InvalidateValues();
}

void GridLayout::InvalidateValues()
{
    if (mHorizontalAxis && mVerticalAxis) {
        mHorizontalAxis->InvalidateValues();
        mVerticalAxis->InvalidateValues();
    }
}

ECode GridLayout::HandleInvalidParams(
    /* [in] */ const String& msg)
{
    //////////////////////////////// is right
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

void GridLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* lp,
    /* [in] */ Boolean horizontal)
{
    String groupName = horizontal ? String("column") : String("row");
    AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp;
    AutoPtr<Spec> spec = horizontal ? (Spec*)glp->mColumnSpec.Get() : (Spec*)glp->mRowSpec.Get();
    AutoPtr<Interval> span = spec->mSpan;
    if (span->mMin != UNDEFINED && span->mMin < 0) {
        String wt = groupName + String(" indices must be positive");
        HandleInvalidParams(wt);
    }
    AutoPtr<Axis> axis = horizontal ? mHorizontalAxis : mVerticalAxis;
    Int32 count = axis->mDefinedCount;
    if (count != UNDEFINED) {
        if (span->mMax > count) {
            String wt = groupName + String(" indices (start + span) mustn't exceed the ") + groupName + String(" count");
            HandleInvalidParams(wt);
        }
        if (span->Size() > count) {
            String wt = groupName + String(" span mustn't exceed the ") + groupName + String(" count");
            HandleInvalidParams(wt);
        }
    }
}

void GridLayout::DrawLine(
    /* [in] */ ICanvas* graphics,
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2,
    /* [in] */ IPaint* paint)
{
    Int32 dx = GetPaddingLeft();
    Int32 dy = GetPaddingTop();
    if (IsLayoutRtl()) {
        Int32 width = GetWidth();
        graphics->DrawLine(width - dx - x1, dy + y1, width - dx - x2, dy + y2, paint);
    } else {
        graphics->DrawLine(dx + x1, dy + y1, dx + x2, dy + y2, paint);
    }
}

Int32 GridLayout::ComputeLayoutParamsHashCode()
{
    Int32 result = 1;
    for (Int32 i = 0, N = GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = GetChildAt(i);
        Int32 visibility = 0;
        c->GetVisibility(&visibility);
        if (visibility == IView::GONE) continue;
        AutoPtr<IViewGroupLayoutParams> lp;
        c->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        IGridLayoutLayoutParams * glp = IGridLayoutLayoutParams::Probe(lp.Get());
        Int32 hash;
        glp->GetHashCode(&hash);
        result = 31 * result + hash;
    }
    return result;
}

void GridLayout::ConsistencyCheck()
{
    if (mLastLayoutParamsHashCode == UNINITIALIZED_HASH) {
        ValidateLayoutParams();
        mLastLayoutParamsHashCode = ComputeLayoutParamsHashCode();
    } else if (mLastLayoutParamsHashCode != ComputeLayoutParamsHashCode()) {
        /*Log.w(TAG, "The fields of some layout parameters were modified in between layout " +
                "operations. Check the javadoc for GridLayout.LayoutParams#rowSpec.");*/
        InvalidateStructure();
        ConsistencyCheck();
    }
}

void GridLayout::MeasureChildWithMargins2(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthSpec,
    /* [in] */ Int32 parentHeightSpec,
    /* [in] */ Int32 childWidth,
    /* [in] */ Int32 childHeight)
{
    Int32 childWidthSpec = GetChildMeasureSpec(parentWidthSpec,
            mPaddingLeft + mPaddingRight + GetTotalMargin(child, TRUE), childWidth);
    Int32 childHeightSpec = GetChildMeasureSpec(parentHeightSpec,
            mPaddingTop + mPaddingBottom + GetTotalMargin(child, FALSE), childHeight);

    child->Measure(childWidthSpec, childHeightSpec);
}

void GridLayout::MeasureChildrenWithMargins(
    /* [in] */ Int32 widthSpec,
    /* [in] */ Int32 heightSpec,
    /* [in] */ Boolean firstPass)
{
    for (Int32 i = 0, N = GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = GetChildAt(i);
        Int32 visibility = 0;
        c->GetVisibility(&visibility);
        if (visibility == IView::GONE) continue;
        AutoPtr<IViewGroupLayoutParams> lp = GetLayoutParams(c);
        AutoPtr<CGridLayoutLayoutParams> cp = (CGridLayoutLayoutParams*)lp.Get();
        if (firstPass) {
            MeasureChildWithMargins2(c, widthSpec, heightSpec, cp->mWidth, cp->mHeight);
        } else {
            Boolean horizontal = (mOrientation == HORIZONTAL);
            AutoPtr<Spec> spec = horizontal ? (Spec*)cp->mColumnSpec.Get() : (Spec*)cp->mRowSpec.Get();
            if (spec->mAlignment == FILL) {
                AutoPtr<Interval> span = spec->mSpan;
                AutoPtr<Axis> axis = horizontal ? mHorizontalAxis : mVerticalAxis;
                AutoPtr< ArrayOf<Int32> > locations = axis->GetLocations();
                Int32 cellSize = (*locations)[span->mMax] - (*locations)[span->mMin];
                Int32 viewSize = cellSize - GetTotalMargin(c, horizontal);
                if (horizontal) {
                    MeasureChildWithMargins2(c, widthSpec, heightSpec, viewSize, cp->mHeight);
                } else {
                    MeasureChildWithMargins2(c, widthSpec, heightSpec, cp->mWidth, viewSize);
                }
            }
        }
    }
}

Int32 GridLayout::GetMeasurement(
    /* [in] */ IView* c,
    /* [in] */ Boolean horizontal)
{
    Int32 result = 0;
    if (horizontal) {
        c->GetMeasuredWidth(&result);
    } else {
        c->GetMeasuredHeight(&result);
    }
    if (GetLayoutMode() == OPTICAL_BOUNDS) {
        AutoPtr<IInsets> insets;
        c->GetOpticalInsets((IInsets**)&insets);
        Insets* set = (Insets*)insets.Get();
        return result - (horizontal ? set->mLeft + set->mRight : set->mTop + set->mBottom);
    }
    return result;
}

AutoPtr<GridLayout::Alignment> GridLayout::CreateSwitchingAlignment(
    /* [in] */ Alignment* ltr,
    /* [in] */ Alignment* rtl)
{
    AutoPtr<Alignment> al = new SwitchingAlignment(ltr, rtl);
    return al;
}

Boolean GridLayout::CanStretch(
    /* [in] */ Int32 flexibility)
{
    return (flexibility & CAN_STRETCH) != 0;
}

AutoPtr<ISpec> GridLayout::NewInstance(
    /* [in] */ Int32 start,
    /* [in] */ Int32 size,
    /* [in] */ Alignment* alignment)
{
    AutoPtr<ISpec> sp = new Spec(start != UNDEFINED, start, size, alignment);
    return sp;
}

AutoPtr<ISpec> GridLayout::NewInstance(
    /* [in] */ Int32 start,
    /* [in] */ Alignment* alignment)
{
    AutoPtr<ISpec> sp = NewInstance(start, 1, alignment);
    return sp;
}

AutoPtr<ISpec> GridLayout::NewInstance(
    /* [in] */ Int32 start,
    /* [in] */ Int32 size)
{
    AutoPtr<ISpec> sp = NewInstance(start, size, UNDEFINED_ALIGNMENT);
    return sp;
}

AutoPtr<ISpec> GridLayout::NewInstance(
    /* [in] */ Int32 start)
{
    AutoPtr<ISpec> sp = NewInstance(start, 1);
    return sp;
}

//==================================================================
//                      GridLayout::Axis
//==================================================================
const Int32 GridLayout::Axis::NEW;
const Int32 GridLayout::Axis::PENDING;
const Int32 GridLayout::Axis::COMPLETE;

GridLayout::Axis::Axis(
    /* [in] */ Boolean horizontal,
    /* [in] */ GridLayout* host)
    : mHost(host)
    , mMaxIndex(GridLayout::UNDEFINED)
    , mHorizontal(horizontal)
    , mDefinedCount(GridLayout::UNDEFINED)
    , mGroupBoundsValid(FALSE)
    , mForwardLinksValid(FALSE)
    , mBackwardLinksValid(FALSE)
    , mLeadingMarginsValid(FALSE)
    , mTrailingMarginsValid(FALSE)
    , mArcsValid(FALSE)
    , mLocationsValid(FALSE)
    , mOrderPreserved(GridLayout::DEFAULT_ORDER_PRESERVED)
{
    mParentMin = new MutableInt(0);
    mParentMax = new MutableInt(-GridLayout::MAX_SIZE);
}

Int32 GridLayout::Axis::CalculateMaxIndex()
{
    Int32 result = -1;
    for (Int32 i = 0, N = mHost->GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = mHost->GetChildAt(i);
        AutoPtr<IViewGroupLayoutParams> params = mHost->GetLayoutParams(c);
        AutoPtr<CGridLayoutLayoutParams> lp = (CGridLayoutLayoutParams*)params.Get();
        AutoPtr<Spec> spec = mHorizontal ? (Spec*)lp->mColumnSpec.Get() : (Spec*)lp->mRowSpec.Get();
        AutoPtr<Interval> span = spec->mSpan;
        result = Elastos::Core::Math::Max(result, span->mMin);
        result = Elastos::Core::Math::Max(result, span->mMax);
        result = Elastos::Core::Math::Max(result, span->Size());
    }
    return result == -1 ? GridLayout::UNDEFINED : result;
}

Int32 GridLayout::Axis::GetMaxIndex()
{
    if (mMaxIndex == GridLayout::UNDEFINED) {
        mMaxIndex = Elastos::Core::Math::Max(0, CalculateMaxIndex());
    }
    return mMaxIndex;
}

AutoPtr< GridLayout::PackedMap<ISpec*, GridLayout::Bounds*> > GridLayout::Axis::CreateGroupBounds()
{
    Assoc<AutoPtr<ISpec>, AutoPtr<Bounds> >* assoc = Assoc<AutoPtr<ISpec>, AutoPtr<Bounds> >::Of(NULL, NULL);
    for (Int32 i = 0, N = mHost->GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = mHost->GetChildAt(i);
        AutoPtr<CGridLayoutLayoutParams> lp = (CGridLayoutLayoutParams*)(mHost->GetLayoutParams(c)).Get();
        AutoPtr<ISpec> spec = mHorizontal ? (ISpec*)lp->mColumnSpec.Get() : (ISpec*)lp->mRowSpec.Get();
        AutoPtr<Spec> sp = (Spec*)spec.Get();
        AutoPtr<Bounds> bounds = mHost->GetAlignment(sp->mAlignment, mHorizontal)->GetBounds();
        assoc->Put(spec, bounds);
    }
    AutoPtr<PackedMap<AutoPtr<ISpec>, AutoPtr<Bounds> > > map = assoc->Pack();
    delete assoc;
    return (PackedMap<ISpec*, GridLayout::Bounds*>*)(map.Get());
}

void GridLayout::Axis::ComputeGroupBounds()
{
    AutoPtr< ArrayOf<Bounds*> > values = mGroupBounds->mValues;
    for (Int32 i = 0; i < values->GetLength(); i++) {
        (*values)[i]->Reset();

    }
    for (Int32 i = 0, N = mHost->GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = mHost->GetChildAt(i);
        AutoPtr<IViewGroupLayoutParams> lp = mHost->GetLayoutParams(c);
        AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp.Get();
        AutoPtr<Spec> spec = mHorizontal ? (Spec*)glp->mColumnSpec.Get() : (Spec*)glp->mRowSpec.Get();
        mGroupBounds->GetValue(i)->Include((IGridLayout*)mHost->Probe(EIID_IGridLayout), c, spec, this);
    }
}

AutoPtr< GridLayout::PackedMap<GridLayout::Interval*, GridLayout::MutableInt*> > GridLayout::Axis::CreateLinks(
    /* [in] */ Boolean min)
{
    Assoc< AutoPtr<Interval>, AutoPtr<MutableInt> >* result = Assoc< AutoPtr<Interval>, AutoPtr<MutableInt> >::Of(NULL, NULL);
    AutoPtr< ArrayOf<ISpec*> > keys = GetGroupBounds()->mKeys;
    for (Int32 i = 0, N = keys->GetLength(); i < N; i++)
    {
        AutoPtr<Spec> sp = (Spec*)((*keys)[i]);
        AutoPtr<Interval> span = min ? sp->mSpan : sp->mSpan->Inverse();
        AutoPtr<MutableInt> mut = new MutableInt();
        result->Put(span, mut);
    }
    AutoPtr<PackedMap<AutoPtr<Interval>, AutoPtr<MutableInt> > > map = result->Pack();
    delete result;
    return (PackedMap<Interval*, GridLayout::MutableInt*>*)(map.Get());;
}

void GridLayout::Axis::ComputeLinks(
    /* [in] */ PackedMap<Interval*, MutableInt*>* links,
    /* [in] */ Boolean min)
{
    AutoPtr< ArrayOf<MutableInt*> > spans = links->mValues;
    for (Int32 i = 0; i < spans->GetLength(); i++) {
        (*spans)[i]->Reset();
    }

    AutoPtr< ArrayOf<Bounds*> > bounds = GetGroupBounds()->mValues;
    for (Int32 i = 0; i < bounds->GetLength(); i++) {
        Int32 size = (*bounds)[i]->Size(min);
        AutoPtr<MutableInt> valueHolder = links->GetValue(i);
        valueHolder->mValue = Elastos::Core::Math::Max(valueHolder->mValue, min ? size : -size);
    }
}

AutoPtr< GridLayout::PackedMap<GridLayout::Interval*, GridLayout::MutableInt*> > GridLayout::Axis::GetForwardLinks()
{
    if (!mForwardLinks) {
        mForwardLinks = CreateLinks(TRUE);
    }
    if (!mForwardLinksValid) {
        ComputeLinks(mForwardLinks, TRUE);
        mForwardLinksValid = TRUE;
    }
    return mForwardLinks;
}

AutoPtr< GridLayout::PackedMap<GridLayout::Interval*, GridLayout::MutableInt*> > GridLayout::Axis::GetBackwardLinks()
{
    if (!mBackwardLinks) {
        mBackwardLinks = CreateLinks(FALSE);
    }
    if (!mBackwardLinksValid) {
        ComputeLinks(mBackwardLinks, FALSE);
        mBackwardLinksValid = TRUE;
    }
    return mBackwardLinks;
}

void GridLayout::Axis::Include(
    /* [in] */ List< AutoPtr<Arc> >& arcs,
    /* [in] */ Interval* key,
    /* [in] */ MutableInt* size,
    /* [in] */ Boolean ignoreIfAlreadyPresent)
{
    if (key->Size() == 0) {
        return;
    }
    if (ignoreIfAlreadyPresent) {
        List< AutoPtr<Arc> >::Iterator it = arcs.Begin();
        for (; it != arcs.End(); it++)
        {
            AutoPtr<Interval> span = (*it)->mSpan;
            if (span->Equals(key)) {
                return;
            }
        }
    }
    AutoPtr<Arc> obj = new Arc(key, size);
    arcs.PushBack(obj);
}

void GridLayout::Axis::Include(
    /* [in] */ List< AutoPtr<Arc> >& arcs,
    /* [in] */ Interval* key,
    /* [in] */ MutableInt* size)
{
    Include(arcs, key, size, TRUE);
}

AutoPtr< ArrayOf<AutoPtr<ArrayOf<GridLayout::Arc*> > > > GridLayout::Axis::GroupArcsByFirstVertex(
    /* [in] */ AutoPtr< ArrayOf<Arc*> > arcs)
{
    Int32 N = GetCount() + 1;

    AutoPtr< ArrayOf<AutoPtr<ArrayOf<GridLayout::Arc*> > > > result = ArrayOf<AutoPtr<ArrayOf<GridLayout::Arc*> > >::Alloc(N);

    Int32 sizes[N];
    for (Int32 i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        sizes[i] = 0;
    }
    for (Int32 i = 0; i < arcs->GetLength(); i++) {
        AutoPtr<Arc> firstArc = (*arcs)[i];
        sizes[firstArc->mSpan->mMin]++;
    }
    for (Int32 i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        AutoPtr<ArrayOf<Arc*> > Arcn = ArrayOf<Arc*>::Alloc(sizes[i]);
        result->Set(i, Arcn);
    }
    for (Int32 i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        sizes[i] = 0;
    }
    for (Int32 i = 0; i < arcs->GetLength(); i++) {
        AutoPtr<Arc> secondArc = (*arcs)[i];
        Int32 index = secondArc->mSpan->mMin;
        (*result)[index]->Set(sizes[index]++, secondArc);
    }
    return result;
}

AutoPtr< ArrayOf<GridLayout::Arc*> > GridLayout::Axis::TopologicalSort(
    /* [in] */ ArrayOf<Arc*>* arcs)
{
    Int32 len = arcs->GetLength();
    AutoPtr< ArrayOf<Arc*> > result = ArrayOf<Arc*>::Alloc(len);
    Int32 cursor = len - 1;
    AutoPtr< ArrayOf<AutoPtr<ArrayOf<GridLayout::Arc*> > > > arcsByVertex = GroupArcsByFirstVertex(arcs);
    AutoPtr< ArrayOf<Int32> > visited = ArrayOf<Int32>::Alloc(GetCount() + 1);
    AutoPtr<TopoSort> sort = new TopoSort(result, cursor, arcsByVertex, visited);
    return sort->Sort();
    /*return new Object() {
        Arc[] result = new Arc[arcs.length];
        int cursor = result.length - 1;
        Arc[][] arcsByVertex = groupArcsByFirstVertex(arcs);
        int[] visited = new int[getCount() + 1];

        void walk(int loc) {
            switch (visited[loc]) {
                case NEW: {
                    visited[loc] = PENDING;
                    for (Arc arc : arcsByVertex[loc]) {
                        walk(arc.span.max);
                        result[cursor--] = arc;
                    }
                    visited[loc] = COMPLETE;
                    break;
                }
                case PENDING: {
                    // le singe est dans l'arbre
                    assert false;
                    break;
                }
                case COMPLETE: {
                    break;
                }
            }
        }

        Arc[] sort() {
            for (int loc = 0, N = arcsByVertex.length; loc < N; loc++) {
                walk(loc);
            }
            assert cursor == -1;
            return result;
        }
    }.sort();*/
}

AutoPtr< ArrayOf<GridLayout::Arc*> > GridLayout::Axis::TopologicalSort(
    /* [in] */ List< AutoPtr<Arc> >& arcs)
{
    AutoPtr< ArrayOf<Arc*> > result = ArrayOf<Arc*>::Alloc(arcs.GetSize());
    List< AutoPtr<Arc> >::Iterator it = arcs.Begin();
    for(Int32 i = 0; it != arcs.End(); it++, i++) {
        result->Set(i, *it);
    }
    return TopologicalSort(result);
}

void GridLayout::Axis::AddComponentSizes(
    /* [in] */ List< AutoPtr<Arc> >& result,
    /* [in] */ PackedMap<Interval*, MutableInt*>* links)
{
    for (Int32 i = 0; i < links->mKeys->GetLength(); i++) {
        AutoPtr<Interval> key = (*links->mKeys)[i];
        Include(result, key, (*links->mValues)[i], FALSE);
    }
}

AutoPtr< ArrayOf<GridLayout::Arc*> > GridLayout::Axis::CreateArcs()
{
    List< AutoPtr<Arc> > mins, maxs;

    AddComponentSizes(mins, GetForwardLinks());
    AddComponentSizes(maxs, GetBackwardLinks());

    if (mOrderPreserved) {
        for (Int32 i = 0; i < GetCount(); i++) {
            AutoPtr<Interval> val = new Interval(i, i + 1);
            AutoPtr<MutableInt> mut = new MutableInt(0);
            Include(mins, val, mut);
        }
    }

    Int32 N = GetCount();
    AutoPtr<Interval> firVal = new Interval(0, N);
    AutoPtr<Interval> secVal = new Interval(N, 0);
    Include(mins, firVal, mParentMin, FALSE);
    Include(maxs, secVal, mParentMax, FALSE);

    AutoPtr< ArrayOf<Arc*> > sMins = TopologicalSort(mins);
    AutoPtr< ArrayOf<Arc*> > sMaxs = TopologicalSort(maxs);
    /*AutoPtr< ArrayOf<Arc*> > test = mHost->Append(sMins.Get(), sMaxs.Get());
    return test;*/
    return mHost->Append(sMins.Get(), sMaxs.Get());
}

void GridLayout::Axis::ComputeArcs()
{
    GetForwardLinks();
    GetBackwardLinks();
}

Boolean GridLayout::Axis::Relax(
    /* [in] */ ArrayOf<Int32>* locations,
    /* [in] */ Arc* entry)
{
    if (!entry->mValid) {
        return FALSE;
    }
    AutoPtr<Interval> span = entry->mSpan;
    Int32 u = span->mMin;
    Int32 v = span->mMax;
    Int32 value = entry->mValue->mValue;
    Int32 candidate = (*locations)[u] + value;
    if (candidate > (*locations)[v]) {
        (*locations)[v] = candidate;
        return TRUE;
    }
    return FALSE;
}

void GridLayout::Axis::Init(
    /* [in] */ ArrayOf<Int32>* locations)
{
    for (Int32 i = 0; i < locations->GetLength(); i++) {
        (*locations)[i] = 0;
    }
}

String GridLayout::Axis::ArcsToString(
    /* [in] */ List< AutoPtr<Arc> >& arcs)
{
    String var = mHorizontal ? String("x") : String("y");
    StringBuilder result;
    Boolean first = TRUE;
    List< AutoPtr<Arc> >::Iterator it = arcs.Begin();
    for (; it != arcs.End(); it++) {
        if (first) {
            first = FALSE;
        } else {
            result.AppendString(String(", "));
        }
        Int32 src = (*it)->mSpan->mMin;
        Int32 dst = (*it)->mSpan->mMax;
        Int32 value = (*it)->mValue->mValue;
        if (src < dst) {
            result.AppendString(var);
            result.AppendInt32(dst);
            result.AppendString(String("-"));
            result.AppendString(var);
            result.AppendInt32(src);
            result.AppendString(String(">="));
            result.AppendInt32(value);
        } else {
            result.AppendString(var);
            result.AppendInt32(src);
            result.AppendString(String("-"));
            result.AppendString(var);
            result.AppendInt32(dst);
            result.AppendString(String("<="));
            result.AppendInt32(-value);
        }
    }
    String str;
    result.ToString(&str);
    return str;
}

void GridLayout::Axis::LogError(
    /* [in] */ const String& axisName,
    /* [in] */ ArrayOf<Arc*>* arcs,
    /* [in] */ ArrayOf<Boolean>* culprits0)
{
    List< AutoPtr<Arc> > culprits;
    List< AutoPtr<Arc> > removed;
    for (Int32 c = 0; c < arcs->GetLength(); c++) {
        AutoPtr<Arc> arc = (*arcs)[c];
        if ((*culprits0)[c]) {
            culprits.PushBack(arc);
        }
        if (!arc->mValid) {
            removed.PushBack(arc);
        }
    }
    /*Log.d(TAG, axisName + " constraints: " + arcsToString(culprits) + " are inconsistent; "
            + "permanently removing: " + arcsToString(removed) + ". ");*/
}

void GridLayout::Axis::Solve(
    /* [in] */ ArrayOf<Arc*>* arcs,
    /* [in] */ ArrayOf<Int32>* locations)
{

    String axisName = mHorizontal ? String("horizontal") : String("vertical");
    Int32 N = GetCount() + 1;
    AutoPtr< ArrayOf<Boolean> > originalCulprits = NULL;
    for (Int32 p = 0; p < arcs->GetLength(); p++) {
        Init(locations);

        for (Int32 i = 0; i < N; i++) {
            Boolean changed = FALSE;
            for (Int32 j = 0, length = arcs->GetLength(); j < length; j++) {
                changed |= Relax(locations, (*arcs)[j]);
            }
            if (!changed) {
                if (originalCulprits) {
                    LogError(axisName, arcs, originalCulprits);
                }
                return;
            }
        }

        AutoPtr< ArrayOf<Boolean> > culprits = ArrayOf<Boolean>::Alloc(arcs->GetLength());
        for (Int32 i = 0; i < N; i++) {
            for (Int32 j = 0, length = arcs->GetLength(); j < length; j++) {
                (*culprits)[j] |= Relax(locations, (*arcs)[j]);
            }
        }

        if (p == 0) {
            originalCulprits = culprits;
        }

        for (Int32 i = 0; i < arcs->GetLength(); i++) {
            if ((*culprits)[i]) {
                AutoPtr<Arc> arc = (*arcs)[i];
                if (arc->mSpan->mMin < arc->mSpan->mMax) {
                    continue;
                }
                arc->mValid = FALSE;
                break;
            }
        }
    }
}

void GridLayout::Axis::ComputeMargins(
    /* [in] */ Boolean leading)
{
    AutoPtr< ArrayOf<Int32> > margins = leading ? mLeadingMargins : mTrailingMargins;
    for (Int32 i = 0, N = mHost->GetChildCount(); i < N; i++) {
        AutoPtr<IView> c = mHost->GetChildAt(i);
        Int32 visibility = 0;
        c->GetVisibility(&visibility);
        if (visibility == IView::GONE) continue;
        AutoPtr<IViewGroupLayoutParams> lp = mHost->GetLayoutParams(c);
        AutoPtr<CGridLayoutLayoutParams> glp = (CGridLayoutLayoutParams*)lp.Get();
        AutoPtr<Spec> spec = mHorizontal ? (Spec*)glp->mColumnSpec.Get() : (Spec*)glp->mRowSpec.Get();
        AutoPtr<Interval> span = spec->mSpan;
        Int32 index = leading ? span->mMin : span->mMax;
        (*margins)[index] = Elastos::Core::Math::Max((*margins)[index], mHost->GetMargin1(c, mHorizontal, leading));
    }
}

void GridLayout::Axis::ComputeLocations(
    /* [in] */ ArrayOf<Int32>* a)
{
    /*AutoPtr< ArrayOf<Arc*> > arcs = GetArcs();
    Solve(arcs, a);*/
    Solve(GetArcs(), a);
    if (!mOrderPreserved) {
        Int32 a0 = (*a)[0];
        for (Int32 i = 0, N = a->GetLength(); i < N; i++) {
            (*a)[i] = (*a)[i] - a0;
        }
    }
}

Int32 GridLayout::Axis::Size(
    /* [in] */ ArrayOf<Int32>* locations)
{
    return (*locations)[GetCount()];
}

void GridLayout::Axis::SetParentConstraints(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    mParentMin->mValue = min;
    mParentMax->mValue = -max;
    mLocationsValid = FALSE;
}

Int32 GridLayout::Axis::GetMeasure(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    SetParentConstraints(min, max);
    return Size(GetLocations());
}

Int32 GridLayout::Axis::GetCount()
{
    return Elastos::Core::Math::Max(mDefinedCount, GetMaxIndex());
}

void GridLayout::Axis::SetCount(
    /* [in] */ Int32 count)
{
    if (count != GridLayout::UNDEFINED && count < GetMaxIndex()) {
        if (mHorizontal) {
            String wt = String("column") + String("Count must be greater than or equal to the maximum of all grid indices ")
                + String("(and spans) defined in the LayoutParams of each child");
            mHost->HandleInvalidParams(wt);
        } else {
            String wt = String("row") + String("Count must be greater than or equal to the maximum of all grid indices ")
                + String("(and spans) defined in the LayoutParams of each child");
            mHost->HandleInvalidParams(wt);
        }
    }
    mDefinedCount = count;
}

Boolean GridLayout::Axis::IsOrderPreserved()
{
    return mOrderPreserved;
}

void GridLayout::Axis::SetOrderPreserved(
    /* [in] */ Boolean orderPreserved)
{
    mOrderPreserved = orderPreserved;
    InvalidateStructure();
}

AutoPtr< GridLayout::PackedMap<ISpec*, GridLayout::Bounds*> > GridLayout::Axis::GetGroupBounds()
{
    if (!mGroupBounds) {
        mGroupBounds = CreateGroupBounds();
    }
    if (!mGroupBoundsValid) {
        ComputeGroupBounds();
        mGroupBoundsValid = true;
    }
    return mGroupBounds;
}

AutoPtr< ArrayOf<GridLayout::Arc*> > GridLayout::Axis::GetArcs()
{
    if (!mArcs) {
        mArcs = CreateArcs();
    }
    if (!mArcsValid) {
        ComputeArcs();
        mArcsValid = TRUE;
    }
    return mArcs;
}

AutoPtr< ArrayOf<Int32> > GridLayout::Axis::GetLeadingMargins()
{
    if (!mLeadingMargins) {
        mLeadingMargins = ArrayOf<Int32>::Alloc(GetCount() + 1);
    }
    if (!mLeadingMarginsValid) {
        ComputeMargins(TRUE);
        mLeadingMarginsValid = TRUE;
    }
    return mLeadingMargins;
}

AutoPtr< ArrayOf<Int32> > GridLayout::Axis::GetTrailingMargins()
{
    if (!mTrailingMargins) {
        mTrailingMargins = ArrayOf<Int32>::Alloc(GetCount() + 1);
    }
    if (!mTrailingMarginsValid) {
        ComputeMargins(FALSE);
        mTrailingMarginsValid = TRUE;
    }
    return mTrailingMargins;
}

AutoPtr< ArrayOf<Int32> > GridLayout::Axis::GetLocations()
{
    if (!mLocations) {
        Int32 N = GetCount() + 1;
        mLocations = ArrayOf<Int32>::Alloc(N);
    }
    if (!mLocationsValid) {
        ComputeLocations(mLocations);
        mLocationsValid = TRUE;
    }
    return mLocations;
}

Int32 GridLayout::Axis::GetMeasure(
    /* [in] */ Int32 measureSpec)
{
    Int32 mode = MeasureSpec::GetMode(measureSpec);
    Int32 size = MeasureSpec::GetSize(measureSpec);
    switch (mode) {
        case MeasureSpec::UNSPECIFIED: {
            return GetMeasure(0, MAX_SIZE);
        }
        case MeasureSpec::EXACTLY: {
            return GetMeasure(size, size);
        }
        case MeasureSpec::AT_MOST: {
            return GetMeasure(0, size);
        }
        default: {
            //assert false;
            return 0;
        }
    }
}

void GridLayout::Axis::Layout(
    /* [in] */ Int32 size)
{
    SetParentConstraints(size, size);
    GetLocations();
}

void GridLayout::Axis::InvalidateStructure()
{
    mMaxIndex = UNDEFINED;
    mGroupBounds = NULL;
    mForwardLinks = NULL;
    mBackwardLinks = NULL;
    mLeadingMargins = NULL;
    mTrailingMargins = NULL;
    mArcs = NULL;
    mLocations = NULL;
    InvalidateValues();
}

void GridLayout::Axis::InvalidateValues()
{
    mGroupBoundsValid = FALSE;
    mForwardLinksValid = FALSE;
    mBackwardLinksValid = FALSE;
    mLeadingMarginsValid = FALSE;
    mTrailingMarginsValid = FALSE;
    mArcsValid = FALSE;
    mLocationsValid = FALSE;
}

//==================================================================
//                      GridLayout::Arc
//==================================================================
GridLayout::Arc::Arc(
    /* [in] */ Interval* span,
    /* [in] */ MutableInt* value)
    : mValid(TRUE)
{
    mSpan = span;
    mValue = value;
}

String GridLayout::Arc::ToString()
{
    String str = mSpan->ToString();
    str += String(" ");
    String mut = mValue->ToString();
    if (!mValid) {
        str += String("+>");
    } else {
        str += String("->");
    }
    str += String(" ");
    str += mut;
    return str;
}

//==================================================================
//                      GridLayout::MutableInt
//==================================================================
GridLayout::MutableInt::MutableInt()
{
    Reset();
}

GridLayout::MutableInt::MutableInt(
    /* [in] */ Int32 value)
{
    mValue = value;
}

void GridLayout::MutableInt::Reset()
{
    mValue = Elastos::Core::Math::INT32_MIN_VALUE;
}

String GridLayout::MutableInt::ToString()
{
    return StringUtils::Int32ToString(mValue);
}

//==================================================================
//                      GridLayout::Bounds
//==================================================================
GridLayout::Bounds::Bounds()
{
    Reset();
}

void GridLayout::Bounds::Reset()
{
    mBefore = Elastos::Core::Math::INT32_MIN_VALUE;
    mAfter = Elastos::Core::Math::INT32_MIN_VALUE;
    mFlexibility = GridLayout::CAN_STRETCH;
}

void GridLayout::Bounds::Include(
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    mBefore = Elastos::Core::Math::Max(mBefore, before);
    mAfter = Elastos::Core::Math::Max(mAfter, after);
}

Int32 GridLayout::Bounds::Size(
    /* [in] */ Boolean min)
{
    if (!min) {
        if (CanStretch(mFlexibility)) {
            return GridLayout::MAX_SIZE;
        }
    }
    return mBefore + mAfter;
}

Int32 GridLayout::Bounds::GetOffset(
    /* [in] */ IGridLayout* gl,
    /* [in] */ IView* c,
    /* [in] */ Alignment* a,
    /* [in] */ Int32 size,
    /* [in] */ Boolean horizontal)
{
    Int32 mode = 0;
    gl->GetLayoutMode(&mode);
    Int32 value = 0;
    a->GetAlignmentValue(c, size, mode, &value);
    return mBefore -value;
}

void GridLayout::Bounds::Include(
    /* [in] */ IGridLayout* gl,
    /* [in] */ IView* c,
    /* [in] */ Spec* spec,
    /* [in] */ Axis* axis)
{
    mFlexibility &= spec->GetFlexibility();
    Boolean horizontal = axis->mHorizontal;
    Int32 size = 0;
    gl->GetMeasurementIncludingMargin(c, horizontal, &size);
    GridLayout* layout = reinterpret_cast<GridLayout*>(gl->Probe(EIID_GridLayout));
    AutoPtr<Alignment> alignment = layout->GetAlignment(spec->mAlignment, horizontal);
    Int32 mode = 0;
    gl->GetLayoutMode(&mode);
    Int32 before = 0;
    alignment->GetAlignmentValue(c, size, mode, &before);
    Include(before, size - before);
}

String GridLayout::Bounds::ToString()
{
    String before = StringUtils::ToString(mBefore);
    String after = StringUtils::ToString(mAfter);
    return String("Bounds{") + String("before=") + before + String(", after=") + after + String("}");
}

//==================================================================
//                      GridLayout::Interval
//==================================================================
GridLayout::Interval::Interval(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
    : mMin(min)
    , mMax(max)
{}

Int32 GridLayout::Interval::Size()
{
    return mMax - mMin;
}

AutoPtr<GridLayout::Interval> GridLayout::Interval::Inverse()
{
    AutoPtr<Interval> val = new Interval(mMax, mMin);
    return val;
}

Boolean GridLayout::Interval::Equals(
    /* [in] */ Interval* that)
{
    if (this == that) {
        return TRUE;
    }
    if (that == NULL) {
        return FALSE;
    }

    Interval* interval = (Interval*) that;

    if (mMax != interval->mMax) {
        return FALSE;
    }
    if (mMin != interval->mMin) {
        return FALSE;
    }

    return TRUE;
}

Int32 GridLayout::Interval::GetHashCode()
{
    Int32 result = mMin;
    result = 31 * result + mMax;
    return result;
}

String GridLayout::Interval::ToString()
{
    String min = StringUtils::ToString(mMin);
    String max = StringUtils::ToString(mMax);
    return String("[") + min + String(", ") + max + String("]");
}

//==================================================================
//                      GridLayout::Spec
//==================================================================
CAR_INTERFACE_IMPL(GridLayout::Spec, ISpec)

static AutoPtr<ISpec> InitStatic() {
    AutoPtr<ISpec> sp = GridLayout::NewInstance(GridLayout::UNDEFINED);
    return sp;
}

const AutoPtr<ISpec> GridLayout::Spec::UNDEFINED = InitStatic();

GridLayout::Spec::Spec(
    /* [in] */ Boolean startDefined,
    /* [in] */ Interval* span,
    /* [in] */ Alignment* alignment)
    : mStartDefined(startDefined)
    , mSpan(span)
    , mAlignment(alignment)
{}

GridLayout::Spec::Spec(
    /* [in] */ Boolean startDefined,
    /* [in] */ Int32 start,
    /* [in] */ Int32 size,
    /* [in] */ Alignment* alignment)
    : mStartDefined(startDefined)
    , mAlignment(alignment)
{
    AutoPtr<Interval> val = new Interval(start, start + size);
    mSpan = (Interval*)val;
}


AutoPtr<GridLayout::Spec> GridLayout::Spec::CopyWriteSpan(
    /* [in] */ Interval* span)
{
    AutoPtr<Spec> sp = new Spec(mStartDefined, span, mAlignment);
    return sp;
}

AutoPtr<GridLayout::Spec> GridLayout::Spec::CopyWriteAlignment(
    /* [in] */ Alignment* alignment)
{
    AutoPtr<Spec> sp = new Spec(mStartDefined, mSpan, alignment);
    return sp;
}

Int32 GridLayout::Spec::GetFlexibility()
{
    return (mAlignment == GridLayout::UNDEFINED_ALIGNMENT) ? GridLayout::INFLEXIBLE : GridLayout::CAN_STRETCH;
}

ECode GridLayout::Spec::Equals(
    /* [in] */ ISpec* that,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    VALIDATE_NOT_NULL(res);

    Spec* spec = (Spec*)that;
    if (mAlignment != spec->mAlignment) {
        *res = FALSE;
        return NOERROR;
    }
    if (!mSpan->Equals(spec->mSpan)) {
        *res = FALSE;
        return NOERROR;
    }
    *res = TRUE;
    return NOERROR;
}

ECode GridLayout::Spec::Equals(
    /* [in] */ IInterface* that,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    VALIDATE_NOT_NULL(that);
    return Equals(ISpec::Probe(that), res);
}

ECode GridLayout::Spec::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    Int32 result = mSpan->GetHashCode();
    result = 31 * result /*+ mAlignment->GetHashCode()*/;
    *code = result;
    return NOERROR;
}

//==================================================================
//                      GridLayout::Alignment
//==================================================================
CAR_INTERFACE_IMPL(GridLayout::Alignment, IGridLayoutAlignment)

GridLayout::Alignment::Alignment()
{}

AutoPtr<GridLayout::Bounds> GridLayout::Alignment::GetBounds()
{
    AutoPtr<Bounds> b = new Bounds();
    return b;
}

ECode GridLayout::Alignment::GetSizeInCell(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 cellSize,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = viewSize;
    return NOERROR;
}

//==================================================================
//                      GridLayout::UndefinedAlignment
//==================================================================
ECode GridLayout::UndefinedAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = UNDEFINED;
    return NOERROR;
}

ECode GridLayout::UndefinedAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = UNDEFINED;
    return NOERROR;
}

//==================================================================
//                      GridLayout::LeadingAlignment
//==================================================================
ECode GridLayout::LeadingAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = 0;
    return NOERROR;
}

ECode GridLayout::LeadingAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;
    return NOERROR;
}

//==================================================================
//                      GridLayout::TrailingAlignment
//==================================================================
ECode GridLayout::TrailingAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = cellDelta;
    return NOERROR;
}

ECode GridLayout::TrailingAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = viewSize;
    return NOERROR;
}

//==================================================================
//                      GridLayout::SwitchingAlignment
//==================================================================
GridLayout::SwitchingAlignment::SwitchingAlignment(
    /* [in] */ Alignment* ltr,
    /* [in] */ Alignment* rtl)
    : mLtr(ltr)
    , mRtl(rtl)
{}

ECode GridLayout::SwitchingAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    Boolean res = FALSE;
    view->IsLayoutRtl(&res);
    (!res ? mLtr : mRtl)->GetGravityOffset(view, cellDelta, offset);
    return NOERROR;
}

ECode GridLayout::SwitchingAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Boolean res = FALSE;
    view->IsLayoutRtl(&res);
    (!res ? mLtr : mRtl)->GetAlignmentValue(view, viewSize, mode, value);
    return NOERROR;
}

//==================================================================
//                      GridLayout::CenterAlignment
//==================================================================
ECode GridLayout::CenterAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = cellDelta >> 1;
    return NOERROR;
}

ECode GridLayout::CenterAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = viewSize >> 1;
    return NOERROR;
}

//==================================================================
//                      GridLayout::BaseLineAlignment
//==================================================================
ECode GridLayout::BaseLineAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = 0;
    return NOERROR;
}

ECode GridLayout::BaseLineAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 baseline = 0;
    view->GetBaseline(&baseline);
    if (baseline == -1) {
        *value = UNDEFINED;
        return NOERROR;
    } else {
        if (mode == OPTICAL_BOUNDS) {
            AutoPtr<IInsets> insets;
            view->GetOpticalInsets((IInsets**)&insets);
            AutoPtr<Insets> in = (Insets*)insets.Get();
            return baseline - in->mTop;
        }
        *value = baseline;
        return NOERROR;
    }
}

AutoPtr<GridLayout::Bounds> GridLayout::BaseLineAlignment::GetBounds()
{
    AutoPtr<AlignmentBounds> b = new AlignmentBounds();
    return b;
}

//==================================================================
//                      GridLayout::AlignmentBounds
//==================================================================

void GridLayout::AlignmentBounds::Reset()
{
    Bounds::Reset();
    mSize = Elastos::Core::Math::INT32_MIN_VALUE;
}

void GridLayout::AlignmentBounds::Include(
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    Bounds::Include(before, after);
    mSize = Elastos::Core::Math::Max(mSize, before + after);
}

Int32 GridLayout::AlignmentBounds::Size(
    /* [in] */ Boolean min)
{
    return Elastos::Core::Math::Max(Bounds::Size(min), mSize);
}

Int32 GridLayout::AlignmentBounds::GetOffset(
    /* [in] */ IGridLayout* gl,
    /* [in] */ IView* c,
    /* [in] */ Alignment* a,
    /* [in] */ Int32 size,
    /* [in] */ Boolean horizontal)
{
    return Elastos::Core::Math::Max(0, Bounds::GetOffset(gl, c, a, size, horizontal));
}

//==================================================================
//                      GridLayout::FillAlignment
//==================================================================
ECode GridLayout::FillAlignment::GetGravityOffset(
    /* [in] */ IView* view,
    /* [in] */ Int32 cellDelta,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = 0;
    return NOERROR;
}

ECode GridLayout::FillAlignment::GetAlignmentValue(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = UNDEFINED;
    return NOERROR;
}

ECode GridLayout::FillAlignment::GetSizeInCell(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSize,
    /* [in] */ Int32 cellSize,
    /* [in] */ Int32* cell)
{
    VALIDATE_NOT_NULL(cell);
    *cell = cellSize;
    return NOERROR;
}

//==================================================================
//                      GridLayout::TopoSort
//==================================================================
GridLayout::TopoSort::TopoSort(
    /* [in] */ ArrayOf<Arc*>* result,
    /* [in] */ Int32 cursor,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf< Arc*> > >* arcsByVertex,
    /* [in] */ ArrayOf<Int32>* visited)
    : mResult(result)
    , mCursor(cursor)
    , mArcsByVertex(arcsByVertex)
    , mvisited(visited)
{}

void GridLayout::TopoSort::Walk(
    /* [in] */ Int32 loc)
{
    switch((*mvisited)[loc]) {
        case Axis::NEW:
        {
            (*mvisited)[loc] = Axis::PENDING;
            AutoPtr<ArrayOf<GridLayout::Arc*> > a = (*mArcsByVertex)[loc];
            for (Int32 i = 0; i < a->GetLength(); i++) {
                AutoPtr<Arc> arc = (*a)[i];
                Walk(arc->mSpan->mMax);
                mResult->Set(mCursor--, arc);
            }
            (*mvisited)[loc] = Axis::COMPLETE;
            break;
        }
        case Axis::PENDING:
        //assert false;
            break;

        case Axis::COMPLETE:
            break;
    }
}

AutoPtr< ArrayOf<GridLayout::Arc*> > GridLayout::TopoSort::Sort()
{
    for(Int32 loc = 0, N = mArcsByVertex->GetLength(); loc < N; loc++) {
        Walk(loc);
    }
    assert(mCursor == -1);
    return mResult;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
