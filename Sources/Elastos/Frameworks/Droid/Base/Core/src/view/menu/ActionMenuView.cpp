
#include "view/menu/ActionMenuView.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/CActionMenuItemView.h"
#include "view/menu/CActionMenuViewLayoutParams.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include <elastos/core/Math.h>
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Widget::EIID_ILinearLayoutLayoutParams;
using Elastos::Droid::Widget::CLinearLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

// {7cac439f-9b11-4735-8ee1-d6b252b3b29f}
extern "C" const InterfaceID EIID_ActionMenuView =
        { 0x7cac439f, 0x9b11, 0x4735, { 0x8e, 0xe1, 0xd6, 0xb2, 0x52, 0xb3, 0xb2, 0x9f } };

static Int32 BitCount(Int64 v) {
    // Combines techniques from several sources
    if (v < 0)
        v = -v;
    v -=  (v >> 1) & 0x5555555555555555L;
    v = (v & 0x3333333333333333L) + ((v >> 2) & 0x3333333333333333L);
    Int32 i =  ((Int32)(v >> 32)) + (Int32) v;
    if (i < 0)
        i = -i;
    i = (i & 0x0F0F0F0F) + ((i >> 4) & 0x0F0F0F0F);
    i += i >> 8;
    i += i >> 16;
    return i  & 0x0000007F;
}

String ActionMenuView::TAG("ActionMenuView");
const Int32 ActionMenuView::MIN_CELL_SIZE = 56; // dips
const Int32 ActionMenuView::GENERATED_ITEM_PADDING = 4; // dips

ActionMenuView::ActionMenuView()
    : mReserveOverflow(FALSE)
    , mFormatItems(FALSE)
    , mFormatItemsWidth(0)
    , mMinCellSize(0)
    , mGeneratedItemPadding(0)
    , mMeasuredExtraWidth(0)
    , mMaxItemHeight(0)
{
}

ECode ActionMenuView::Init(
    /* [in] */ IContext* context)
{
    InitActionMenu(context, NULL);
    return NOERROR;
}

ECode ActionMenuView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    InitActionMenu(context, attrs);
    return NOERROR;
}

ECode ActionMenuView::SetPresenter(
    /* [in] */ IActionMenuPresenter* presenter)
{
    mPresenter = NULL;
    if (presenter != NULL) {
        IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(presenter);
        wrs->GetWeakReference((IWeakReference**)&mPresenter);
    }
    return NOERROR;
}

Boolean ActionMenuView::IsExpandedFormat()
{
    return mFormatItems;
}

ECode ActionMenuView::SetMaxItemHeight(
    /* [in] */ Int32 maxItemHeight)
{
    mMaxItemHeight = maxItemHeight;
    RequestLayout();
    return NOERROR;
}

void ActionMenuView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    LinearLayout::OnConfigurationChanged(newConfig);
    AutoPtr<IActionMenuPresenter> presenter = GetPresenter();
    if (presenter != NULL)
        presenter->UpdateMenuView(FALSE);

    Boolean rst = FALSE;
    if (presenter != NULL && (presenter->IsOverflowMenuShowing(&rst), rst)) {
        presenter->HideOverflowMenu(&rst);
        presenter->ShowOverflowMenu(&rst);
    }
}

void ActionMenuView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // If we've been given an exact size to match, apply special formatting during layout.
    const Boolean wasFormatted = mFormatItems;
    mFormatItems = MeasureSpec::GetMode(widthMeasureSpec) == MeasureSpec::EXACTLY;

    if (wasFormatted != mFormatItems) {
        mFormatItemsWidth = 0; // Reset this when switching modes
    }

    // Special formatting can change whether items can fit as action buttons.
    // Kick the menu and update presenters when this changes.
    const Int32 widthSize = MeasureSpec::GetMode(widthMeasureSpec);
    if (mFormatItems && mMenu != NULL && widthSize != mFormatItemsWidth) {
        mFormatItemsWidth = widthSize;
        mMenu->OnItemsChanged(TRUE);
    }

    if (mFormatItems) {
        OnMeasureExactFormat(widthMeasureSpec, heightMeasureSpec);
    } else {
        // Previous measurement at exact format may have set margins - reset them.
        const Int32 childCount = GetChildCount();
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);

            AutoPtr<IActionMenuViewLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            lp->SetLeftMargin(0);
            lp->SetRightMargin(0);
        }

        LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    }
}

void ActionMenuView::OnMeasureExactFormat(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // We already know the width mode is EXACTLY if we're here.
    const Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    const Int32 widthPadding = GetPaddingLeft() + GetPaddingRight();
    const Int32 heightPadding = GetPaddingTop() + GetPaddingBottom();

    const Int32 itemHeightSpec = heightMode == MeasureSpec::EXACTLY
            ? MeasureSpec::MakeMeasureSpec(heightSize - heightPadding, MeasureSpec::EXACTLY)
            : MeasureSpec::MakeMeasureSpec(
                Elastos::Core::Math::Min(mMaxItemHeight, heightSize - heightPadding), MeasureSpec::AT_MOST);

    widthSize -= widthPadding;

    // Divide the view into cells.
    const Int32 cellCount = widthSize / mMinCellSize;
    const Int32 cellSizeRemaining = widthSize % mMinCellSize;

    if (cellCount == 0) {
        // Give up, nothing fits.
        SetMeasuredDimension(widthSize, 0);
        return;
    }

    const Int32 cellSize = mMinCellSize + cellSizeRemaining / cellCount;

    Int32 cellsRemaining = cellCount;
    Int32 maxChildHeight = 0;
    Int32 maxCellsUsed = 0;
    Int32 expandableItemCount = 0;
    Int32 visibleItemCount = 0;
    Boolean hasOverflow = FALSE;

    // This is used as a bitfield to locate the smallest items present. Assumes childCount < 64.
    Int64 smallestItemsAt = 0;

    const Int32 childCount = GetChildCount();
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = GetChildAt(i);

        Int32 visiable = 0;
        if ((child->GetVisibility(&visiable), visiable) == IView::GONE) continue;

        const Boolean isGeneratedItem = IActionMenuItemView::Probe(child) != NULL;
        visibleItemCount++;

        if (isGeneratedItem) {
            // Reset padding for generated menu item views; it may change below
            // and views are recycled.
            child->SetPadding(mGeneratedItemPadding, 0, mGeneratedItemPadding, 0);
        }

        AutoPtr<IActionMenuViewLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

        lp->SetExpanded(FALSE);
        lp->SetExtraPixels(0);
        lp->SetCellsUsed(0);
        lp->SetExpandable(FALSE);
        lp->SetLeftMargin(0);
        lp->SetRightMargin(0);

        Boolean hasText;
        IActionMenuItemView::Probe(child)->HasText(&hasText);


        lp->SetPreventEdgeOffset(isGeneratedItem && hasText);
        // Overflow always gets 1 cell. No more, no less.
        Boolean isOverflow;
        lp->GetIsOverflowButton(&isOverflow);
        const Int32 cellsAvailable = isOverflow ? 1 : cellsRemaining;

        const Int32 cellsUsed = MeasureChildForCells(child, cellSize, cellsAvailable,
                itemHeightSpec, heightPadding);

        maxCellsUsed = Elastos::Core::Math::Max(maxCellsUsed, cellsUsed);
        Boolean expandable;
        lp->GetExpandable(&expandable);
        if (expandable) expandableItemCount++;
        if (isOverflow) hasOverflow = TRUE;

        cellsRemaining -= cellsUsed;

        Int32 measureHeight = 0;
        child->GetMeasuredHeight(&measureHeight);
        maxChildHeight = Elastos::Core::Math::Max(maxChildHeight, measureHeight);
        if (cellsUsed == 1) smallestItemsAt |= (1 << i);
    }

    // When we have overflow and a single expanded (text) item, we want to try centering it
    // visually in the available space even though overflow consumes some of it.
    const Boolean centerSingleExpandedItem = hasOverflow && visibleItemCount == 2;

    // Divide space for remaining cells if we have items that can expand.
    // Try distributing whole leftover cells to smaller items first.

    Boolean needsExpansion = FALSE;
    while (expandableItemCount > 0 && cellsRemaining > 0) {
        Int32 minCells = Elastos::Core::Math::INT32_MAX_VALUE;
        Int64 minCellsAt = 0; // Bit locations are indices of relevant child views
        Int32 minCellsItemCount = 0;
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);

            AutoPtr<IActionMenuViewLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            // Don't try to expand items that shouldn't.
            Boolean expandable;
            lp->GetExpandable(&expandable);
            if (!expandable) continue;

            // Mark indices of children that can receive an extra cell.
            Int32 cellsUsed;
            lp->GetCellsUsed(&cellsUsed);
            if (cellsUsed < minCells) {
                minCells = cellsUsed;
                minCellsAt = 1 << i;
                minCellsItemCount = 1;
            } else if (cellsUsed == minCells) {
                minCellsAt |= 1 << i;
                minCellsItemCount++;
            }
        }

        // Items that get expanded will always be in the set of smallest items when we're done.
        smallestItemsAt |= minCellsAt;

        if (minCellsItemCount > cellsRemaining) break; // Couldn't expand anything evenly. Stop.

        // We have enough cells, all minimum size items will be incremented.
        minCells++;

        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);

            AutoPtr<IActionMenuViewLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            if ((minCellsAt & (1 << i)) == 0) {
                // If this item is already at our small item count, mark it for later.
                Int32 cellsUsed;
                lp->GetCellsUsed(&cellsUsed);
                if (cellsUsed == minCells) smallestItemsAt |= 1 << i;
                continue;
            }

            Boolean preventEdgeOffset;
            lp->GetPreventEdgeOffset(&preventEdgeOffset);
            if (centerSingleExpandedItem && preventEdgeOffset && cellsRemaining == 1) {
                // Add padding to this item such that it centers.
                child->SetPadding(mGeneratedItemPadding + cellSize, 0, mGeneratedItemPadding, 0);
            }

            Int32 cellsUsed;
            lp->GetCellsUsed(&cellsUsed);
            lp->SetCellsUsed(++cellsUsed);
            lp->SetExpanded(TRUE);
            cellsRemaining--;
        }

        needsExpansion = TRUE;
    }

    // Divide any space left that wouldn't divide along cell boundaries
    // evenly among the smallest items

    const Boolean singleItem = !hasOverflow && visibleItemCount == 1;
    if (cellsRemaining > 0 && smallestItemsAt != 0 &&
            (cellsRemaining < visibleItemCount - 1 || singleItem || maxCellsUsed > 1)) {
        Float expandCount = (Int32)BitCount(smallestItemsAt);

        if (!singleItem) {
            // The items at the far edges may only expand by half in order to pin to either side.
            if ((smallestItemsAt & 1) != 0) {
                AutoPtr<IActionMenuViewLayoutParams> lp;
                GetChildAt(0)->GetLayoutParams((IViewGroupLayoutParams**)&lp);

                Boolean preventEdgeOffset;
                lp->GetPreventEdgeOffset(&preventEdgeOffset);
                if (!preventEdgeOffset) expandCount -= 0.5f;
            }
            if ((smallestItemsAt & (1 << (childCount - 1))) != 0) {
                AutoPtr<IActionMenuViewLayoutParams> lp;
                GetChildAt(childCount - 1)->GetLayoutParams((IViewGroupLayoutParams**)&lp);

                Boolean preventEdgeOffset;
                lp->GetPreventEdgeOffset(&preventEdgeOffset);
                if (!preventEdgeOffset) expandCount -= 0.5f;
            }
        }

        const Int32 extraPixels = expandCount > 0 ?
                (Int32) (cellsRemaining * cellSize / expandCount) : 0;

        for (Int32 i = 0; i < childCount; i++) {
            if ((smallestItemsAt & (1 << i)) == 0) continue;
            AutoPtr<IView> child = GetChildAt(i);
            AutoPtr<IActionMenuViewLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Boolean isOverflow;
            if (IActionMenuItemView::Probe(child) != NULL) {
                // If this is one of our views, expand and measure at the larger size.
                lp->SetExtraPixels(extraPixels);
                lp->SetExpanded(TRUE);
                Boolean preventEdgeOffset;
                lp->GetPreventEdgeOffset(&preventEdgeOffset);
                if (i == 0 && !preventEdgeOffset) {
                    // First item getsAutoPtr<IView> child = GetChildAt(i); part of its new padding pushed out of sight.
                    // The last item will get this implicitly from layout.
                    lp->SetLeftMargin(-extraPixels / 2);
                }

                needsExpansion = TRUE;

            } else if (lp->GetIsOverflowButton(&isOverflow), isOverflow) {
                lp->SetExtraPixels(extraPixels);
                lp->SetExpanded(TRUE);
                lp->SetRightMargin(-extraPixels / 2);
                needsExpansion = TRUE;
            } else {
                // If we don't know what it is, give it some margins instead
                // and let it center within its space. We still want to pin
                // against the edges.
                if (i != 0) {
                    lp->SetLeftMargin(extraPixels / 2);
                }
                if (i != childCount - 1) {
                    lp->SetRightMargin(extraPixels / 2);
                }
            }
        }

        cellsRemaining = 0;
    }

    // Remeasure any items that have had extra space allocated to them.
    if (needsExpansion) {
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            AutoPtr<IActionMenuViewLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Boolean expanded;
            lp->GetExpanded(&expanded);
            if (!expanded) continue;

            Int32 useds;
            lp->GetCellsUsed(&useds);
            Int32 extraPixels;
            lp->GetExtraPixels(&extraPixels);
            const Int32 width = useds * cellSize + extraPixels;
            child->Measure(MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY),
                    itemHeightSpec);
        }
    }

    if (heightMode != MeasureSpec::EXACTLY) {
        heightSize = maxChildHeight;
    }

    SetMeasuredDimension(widthSize, heightSize);
    mMeasuredExtraWidth = cellsRemaining * cellSize;
}

Int32 ActionMenuView::MeasureChildForCells(
    /* [in] */ IView* child,
    /* [in] */ Int32 cellSize,
    /* [in] */ Int32 cellsRemaining,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 parentHeightPadding)
{
    AutoPtr<IActionMenuViewLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    const Int32 childHeightSize = MeasureSpec::GetSize(parentHeightMeasureSpec) -
            parentHeightPadding;
    const Int32 childHeightMode = MeasureSpec::GetMode(parentHeightMeasureSpec);
    const Int32 childHeightSpec = MeasureSpec::MakeMeasureSpec(childHeightSize, childHeightMode);

    AutoPtr<IActionMenuItemView> itemView = IActionMenuItemView::Probe(child) != NULL ?
            IActionMenuItemView::Probe(child) : NULL;

    Boolean hasText = FALSE;
    if(itemView != NULL)
        itemView->HasText(&hasText);

    Int32 cellsUsed = 0;
    if (cellsRemaining > 0 && (!hasText || cellsRemaining >= 2)) {
        const Int32 childWidthSpec = MeasureSpec::MakeMeasureSpec(
                cellSize * cellsRemaining, MeasureSpec::AT_MOST);
        child->Measure(childWidthSpec, childHeightSpec);

        Int32 measuredWidth = 0;
        child->GetMeasuredWidth(&measuredWidth);
        cellsUsed = measuredWidth / cellSize;
        if (measuredWidth % cellSize != 0) cellsUsed++;
        if (hasText && cellsUsed < 2) cellsUsed = 2;
    }

    Boolean overflow;
    const Boolean expandable = !(lp->GetIsOverflowButton(&overflow), overflow) && hasText;
    lp->SetExpandable(expandable);

    lp->SetCellsUsed(cellsUsed);
    const Int32 targetWidth = cellsUsed * cellSize;
    child->Measure(MeasureSpec::MakeMeasureSpec(targetWidth, MeasureSpec::EXACTLY),
            childHeightSpec);
    return cellsUsed;
}

void ActionMenuView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if (!mFormatItems) {
        LinearLayout::OnLayout(changed, left, top, right, bottom);
        return;
    }

    const Int32 childCount = GetChildCount();
    const Int32 midVertical = (top + bottom) / 2;
    const Int32 dividerWidth = GetDividerWidth();
    Int32 overflowWidth = 0;
    Int32 nonOverflowWidth = 0;
    Int32 nonOverflowCount = 0;
    Int32 widthRemaining = right - left - GetPaddingRight() - GetPaddingLeft();
    Boolean hasOverflow = FALSE;
    Boolean isLayoutRtl = IsLayoutRtl();
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> v = GetChildAt(i);

        Int32 visiable = 0;
        if ((v->GetVisibility(&visiable), visiable) == IView::GONE) {
            continue;
        }

        AutoPtr<IActionMenuViewLayoutParams> p;
        v->GetLayoutParams((IViewGroupLayoutParams**)&p);

        Boolean isOverflow;
        p->GetIsOverflowButton(&isOverflow);
        if (isOverflow) {
            v->GetMeasuredWidth(&overflowWidth);
            if (HasDividerBeforeChildAt(i)) {
                overflowWidth += dividerWidth;
            }

            Int32 height = 0;
            v->GetMeasuredHeight(&height);
            Int32 r;
            Int32 l;
            if (isLayoutRtl) {
                Int32 leftMargin;
                p->GetLeftMargin(&leftMargin);
                l = GetPaddingLeft() + leftMargin;
                r = l + overflowWidth;
            } else {
                Int32 rightMargin;
                p->GetRightMargin(&rightMargin);
                r = GetWidth() - GetPaddingRight() - rightMargin;
                l = r - overflowWidth;
            }

            Int32 t = midVertical - (height / 2);
            Int32 b = t + height;
            v->Layout(l, t, r, b);

            widthRemaining -= overflowWidth;
            hasOverflow = TRUE;
        } else {
            Int32 mW = 0;
            v->GetMeasuredWidth(&mW);
            Int32 leftMargin, rightMargin;
            p->GetLeftMargin(&leftMargin);
            p->GetRightMargin(&rightMargin);
            const Int32 size = mW + leftMargin + rightMargin;
            nonOverflowWidth += size;
            widthRemaining -= size;
            if (HasDividerBeforeChildAt(i)) {
                nonOverflowWidth += dividerWidth;
            }
            nonOverflowCount++;
        }
    }

    if (childCount == 1 && !hasOverflow) {
        // Center a single child
        AutoPtr<IView> v = GetChildAt(0);
        Int32 width = 0;
        v->GetMeasuredWidth(&width);

        Int32 height = 0;
        v->GetMeasuredHeight(&height);

        const Int32 midHorizontal = (right - left) / 2;
        const Int32 l = midHorizontal - width / 2;
        const Int32 t = midVertical - height / 2;
        v->Layout(l, t, l + width, t + height);
        return;
    }

    const Int32 spacerCount = nonOverflowCount - (hasOverflow ? 0 : 1);
    const Int32 spacerSize = Elastos::Core::Math::Max(0, spacerCount > 0 ? widthRemaining / spacerCount : 0);

    if (isLayoutRtl) {
        Int32 startRight = GetWidth() - GetPaddingRight();
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v = GetChildAt(i);
            AutoPtr<IActionMenuViewLayoutParams> lp;
            v->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Int32 visiable = 0;
            Boolean isOverflow;
            lp->GetIsOverflowButton(&isOverflow);
            if ((v->GetVisibility(&visiable), visiable) == IView::GONE || isOverflow) {
                continue;
            }

            Int32 rightMargin;
            lp->GetRightMargin(&rightMargin);
            startRight -= rightMargin;
            Int32 width = 0;
            v->GetMeasuredWidth(&width);

            Int32 height = 0;
            v->GetMeasuredHeight(&height);

            Int32 t = midVertical - height / 2;
            v->Layout(startRight - width, t, startRight, t + height);
            Int32 leftMargin;
            lp->GetRightMargin(&leftMargin);
            startRight -= width + leftMargin + spacerSize;
        }
    } else {
        Int32 startLeft = GetPaddingLeft();
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v = GetChildAt(i);
            AutoPtr<IActionMenuViewLayoutParams> lp;
            v->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Int32 visiable = 0;
            Boolean isOverflow;
            lp->GetIsOverflowButton(&isOverflow);
            if ((v->GetVisibility(&visiable), visiable) == IView::GONE || isOverflow) {
                continue;
            }

            Int32 leftMargin;
            lp->GetRightMargin(&leftMargin);
            startLeft += leftMargin;
            Int32 width = 0;
            v->GetMeasuredWidth(&width);

            Int32 height = 0;
            v->GetMeasuredHeight(&height);

            Int32 t = midVertical - height / 2;
            v->Layout(startLeft, t, startLeft + width, t + height);
            Int32 rightMargin;
            lp->GetRightMargin(&rightMargin);
            startLeft += width + rightMargin + spacerSize;
        }
    }
}

ECode ActionMenuView::OnDetachedFromWindow()
{
    LinearLayout::OnDetachedFromWindow();
    Boolean rst;
    AutoPtr<IActionMenuPresenter> presenter = GetPresenter();
    if (presenter != NULL)
        presenter->DismissPopupMenus(&rst);
    return NOERROR;
}

Boolean ActionMenuView::IsOverflowReserved()
{
    return mReserveOverflow;
}

ECode ActionMenuView::SetOverflowReserved(
    /* [in] */ Boolean reserveOverflow)
{
    mReserveOverflow = reserveOverflow;
    return NOERROR;
}

ECode ActionMenuView::GenerateDefaultLayoutParams(
    /* [out] */ ILinearLayoutLayoutParams** lp)
{
    assert(lp != NULL);
    AutoPtr<IActionMenuViewLayoutParams> amvlp;
    CActionMenuViewLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
        IViewGroupLayoutParams::WRAP_CONTENT, (IActionMenuViewLayoutParams**)&amvlp);
    amvlp->SetGravity(IGravity::CENTER_VERTICAL);
    *lp = amvlp.Get();
    INTERFACE_ADDREF(*lp);
    return NOERROR;
}

ECode ActionMenuView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IActionMenuViewLayoutParams> lp;
    FAIL_RETURN(CActionMenuViewLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT, (IActionMenuViewLayoutParams**)&lp));

    *params = IViewGroupLayoutParams::Probe(lp);
    INTERFACE_ADDREF(*params);
    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> ActionMenuView::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    if (p != NULL) {
        AutoPtr<IActionMenuViewLayoutParams> result;
        if(IActionMenuViewLayoutParams::Probe(p) != NULL)
        {
            CActionMenuViewLayoutParams::New(IActionMenuViewLayoutParams::Probe(p), (IActionMenuViewLayoutParams**)&result);
        } else {
            CActionMenuViewLayoutParams::New(p, (IActionMenuViewLayoutParams**)&result);
        }

        Int32 gravity;
        result->GetGravity(&gravity);
        if (gravity <= IGravity::NO_GRAVITY) {
            result->SetGravity(IGravity::CENTER_VERTICAL);
        }
        return result;
    }

    AutoPtr<IViewGroupLayoutParams> ret;
    GenerateDefaultLayoutParams((ILinearLayoutLayoutParams**)&ret);
    return ret;
}

Boolean ActionMenuView::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p != NULL && IActionMenuViewLayoutParams::Probe(p) != NULL;
}

AutoPtr<IActionMenuViewLayoutParams> ActionMenuView::GenerateOverflowButtonLayoutParams()
{
    AutoPtr<IActionMenuViewLayoutParams> ret;
    GenerateDefaultLayoutParams((ILinearLayoutLayoutParams**)&ret);
    ret->SetIsOverflowButton(TRUE);
    return ret;
}

ECode ActionMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    return mMenu->PerformItemAction(item, 0, result);
}

ECode ActionMenuView::GetWindowAnimations(
    /* [out] */ Int32* animations)
{
    assert(animations != NULL);
    *animations = 0;
    return NOERROR;
}

ECode ActionMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu)
{
    mMenu = menu;
    return NOERROR;
}

Boolean ActionMenuView::HasDividerBeforeChildAt(
    /* [in] */ Int32 childIndex)
{
    if (childIndex == 0) {
        return FALSE;
    }

    AutoPtr<IView> childBefore = GetChildAt(childIndex - 1);
    AutoPtr<IView> child = GetChildAt(childIndex);
    Boolean result = FALSE;
    if (childIndex < GetChildCount() && IActionMenuChildView::Probe(childBefore) != NULL) {
        Boolean need = FALSE;
        IActionMenuChildView::Probe(childBefore)->NeedsDividerAfter(&need);
        result |= need;
    }
    if (childIndex > 0 && IActionMenuChildView::Probe(child) != NULL) {
        Boolean need = FALSE;
        IActionMenuChildView::Probe(child)->NeedsDividerBefore(&need);
        result |= need;
    }

    return result;
}

Boolean ActionMenuView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    return FALSE;
}

void ActionMenuView::InitActionMenu(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    LinearLayout::Init(context, attrs);
    SetBaselineAligned(FALSE);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    assert(metrics != NULL);

    Float density = 0.f;
    metrics->GetDensity(&density);

    mMinCellSize = (Int32) (MIN_CELL_SIZE * density);
    mGeneratedItemPadding = (Int32) (GENERATED_ITEM_PADDING * density);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ActionBar),
        ARRAY_SIZE(R::styleable::ActionBar));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds,
            R::attr::actionBarStyle, 0, (ITypedArray**)&a);

    a->GetDimensionPixelSize(R::styleable::ActionBar_height, 0, &mMaxItemHeight);
    a->Recycle();
}

AutoPtr<IActionMenuPresenter> ActionMenuView::GetPresenter()
{
    AutoPtr<IActionMenuPresenter> presenter;
    if (mPresenter != NULL) {
        mPresenter->Resolve(EIID_IActionMenuPresenter, (IInterface**)&presenter);
    }
    return presenter;
}
} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
