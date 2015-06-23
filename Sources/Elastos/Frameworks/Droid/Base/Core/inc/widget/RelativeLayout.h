
#ifndef __RELATIVELAYOUT_H__
#define __RELATIVELAYOUT_H__

#include "ext/frameworkext.h"
#include "view/ViewGroup.h"
#include "widget/CRelativeLayoutLayoutParams.h"
#include <elastos/Set.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/core/StringBuilder.h>
#include "util/Pools.h"

using Elastos::String;
using Elastos::Core::IComparator;
using Elastos::Utility::Set;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Utility::IPool;
using Elastos::Droid::Utility::IPoolable;
using Elastos::Droid::Utility::IPoolableManager;
using Elastos::Droid::Utility::Pools;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
class Node;
}}}

#ifndef HASH_FUNC_FOR_AUTOPTR_USING_ADDR_NODE
#define HASH_FUNC_FOR_AUTOPTR_USING_ADDR_NODE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Widget::Node)
#endif

namespace Elastos {
namespace Droid {
namespace Widget {

extern "C" const InterfaceID EIID_NODE;

/**
 * Compares two views in left-to-right and top-to-bottom fashion.
 */
class TopToBottomLeftToRightComparator : public ElRefBase, public IComparator
{
public:
    CAR_INTERFACE_DECL()

    CARAPI Compare(
        /* [in] */ IInterface* lhs,
        /* [in] */ IInterface* rhs,
        /* [out] */ Int32* result);
};

class DependencyGraph : public ElRefBase
{
public:
    DependencyGraph();

    /**
     * Clears the graph.
     */
    CARAPI_(void) Clear();

    /**
     * Adds a view to the graph.
     *
     * @param view The view to be added as a node to the graph.
     */
    CARAPI_(void) Add(
        /* [in] */ IView* view);

    /**
     * Builds a sorted list of views. The sorting order depends on the dependencies
     * between the view. For instance, if view C needs view A to be processed first
     * and view A needs view B to be processed first, the dependency graph
     * is: B -> A -> C. The sorted array will contain views B, A and C in this order.
     *
     * @param sorted The sorted list of views. The length of this array must
     *        be equal to getChildCount().
     * @param rules The list of rules to take into account.
     */
    CARAPI GetSortedViews(
        /* [in] */ ArrayOf<IView*>* sorted,
        /* [in] */ ArrayOf<Int32>* rules);

    /**
     * Prints the dependency graph for the specified rules.
     *
     * @param resources The context's resources to print the ids.
     * @param rules The list of rules to take into account.
     */
    CARAPI_(void) Log(
        /* [in] */ IResources* resources,
        /* [in] */ ArrayOf<Int32>* rules);

    static CARAPI_(void) PrintViewId(
        /* [in] */ IResources* resources,
        /* [in] */ IView* view);

private:
    /**
     * Finds the roots of the graph. A root is a node with no dependency and
     * with [0..n] dependents.
     *
     * @param rulesFilter The list of rules to consider when building the
     *        dependencies
     *
     * @return A list of node, each being a root of the graph
     */
    CARAPI_(List< AutoPtr<Node> > &) FindRoots(
        /* [in] */ ArrayOf<Int32>* rulesFilter);

    static CARAPI_(void) AppendViewId(
        /* [in] */ IResources* resources,
        /* [in] */ Node* node,
        /* [in] */ StringBuilder& buffer);

    static CARAPI_(void) PrintNode(
        /* [in] */ IResources* resources,
        /* [in] */ Node* node);

    static CARAPI_(void) Printdependents(
        /* [in] */ IResources* resources,
        /* [in] */ Node* node,
        /* [in] */ StringBuilder& buffer);

private:
    friend class RelativeLayout;

    /**
     * List of all views in the graph.
     */
    List< AutoPtr<Node> > mNodes;

    /**
     * List of nodes in the graph. Each node is identified by its
     * view id (see View#getId()).
     */
    HashMap<Int32, AutoPtr<Node> > mKeyNodes;

    /**
     * Temporary data structure used to build the list of roots
     * for this graph.
     */
    List< AutoPtr<Node> > mRoots;
};

/**
 * A node in the dependency graph. A node is a view, its list of dependencies
 * and its list of dependents.
 *
 * A node with no dependent is considered a root of the graph.
 */
class Node : public ElRefBase, public IPoolable
{
public:

    CAR_INTERFACE_DECL()

    Node();

    virtual CARAPI SetNextPoolable(
        /* [in] */ IPoolable* element);

    virtual CARAPI GetNextPoolable(
        /* [out] */ IPoolable** element);

    virtual CARAPI IsPooled(
        /* [out] */ Boolean* result);

    virtual CARAPI SetPooled(
        /* [in] */ Boolean isPooled);

    CARAPI_(Boolean) IsPooled();

    static CARAPI_(AutoPtr<Node>) Acquire(
        /* [in] */ IView* view);

    // Method name Release is conflic with IInterface->Release, rename Release to Clear
    CARAPI_(void) Clear();

public:
    /**
     * The view representing this node in the layout.
     */
    AutoPtr<IView> mView;

    /**
     * The list of dependents for this node; a dependent is a node
     * that needs this node to be processed first.
     */
    HashMap<AutoPtr<Node>, DependencyGraph*> mDependents;
    typedef typename HashMap<AutoPtr<Node>, DependencyGraph*>::Iterator DependentsIterator;
    typedef typename HashMap<AutoPtr<Node>, DependencyGraph*>::ValueType DependentsValueType;

    /**
     * The list of dependencies for this node.
     */
    HashMap<Int32, Node*> mDependencies;

    // The pool is static, so all nodes instances are shared across
    // activities, that's why we give it a rather high limit
    const static Int32 POOL_LIMIT = 100;

    static const AutoPtr<IPool> sPool;

    AutoPtr<IPoolable> mNext;
    Boolean mIsPooled;

/*
 * START POOL IMPLEMENTATION
 */
public:
    class NodePoolableManager : public ElRefBase, public IPoolableManager
    {
    public:
        CAR_INTERFACE_DECL()

        NewInstance(
            /* [out] */ IPoolable** element);

        CARAPI OnAcquired(
            /* [in] */ IPoolable* element);

        CARAPI OnReleased(
            /* [in] */ IPoolable* element);
    };
};

class RelativeLayout : public ViewGroup
{
public:
    RelativeLayout();

    RelativeLayout(
        /* [in] */ IContext* context);

    RelativeLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    RelativeLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    ~RelativeLayout();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI SetIgnoreGravity(
        /* [in] */ Int32 viewId);

    virtual CARAPI_(Int32) GetGravity();

    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    virtual CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    virtual CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);

    virtual CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    virtual CARAPI RequestLayout();

    virtual CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    virtual CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(Boolean) ShouldDelayChildPressedState();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    //@Override
    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    //TODO: we need to find another way to implement RelativeLayout
    // This implementation cannot handle every case
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    /**
     * Returns a set of layout parameters with a width of
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT},
     * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT} and no spanning.
     */
    virtual CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** lp);

    // Override to allow type-checking of LayoutParams.
    virtual CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

private:
    CARAPI_(void) SortChildren();

    CARAPI_(void) AlignBaseline(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params);

    /**
     * Measure a child. The child should have left, top, right and bottom information
     * stored in its LayoutParams. If any of these values is -1 it means that the view
     * can extend up to the corresponding edge.
     *
     * @param child Child to measure
     * @param params LayoutParams associated with child
     * @param myWidth Width of the the RelativeLayout
     * @param myHeight Height of the RelativeLayout
     */
    CARAPI_(void) MeasureChild(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params,
        /* [in] */ Int32 myWidth,
        /* [in] */ Int32 myHeight);

    CARAPI_(void) MeasureChildHorizontal(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params,
        /* [in] */ Int32 myWidth,
        /* [in] */ Int32 myHeight);

    /**
     * Get a measure spec that accounts for all of the constraints on this view.
     * This includes size contstraints imposed by the RelativeLayout as well as
     * the View's desired dimension.
     *
     * @param childStart The left or top field of the child's layout params
     * @param childEnd The right or bottom field of the child's layout params
     * @param childSize The child's desired size (the width or height field of
     *        the child's layout params)
     * @param startMargin The left or top margin
     * @param endMargin The right or bottom margin
     * @param startPadding mPaddingLeft or mPaddingTop
     * @param endPadding mPaddingRight or mPaddingBottom
     * @param mySize The width or height of this view (the RelativeLayout)
     * @return MeasureSpec for the child
     */
    CARAPI_(Int32) GetChildMeasureSpec(
        /* [in] */ Int32 childStart,
        /* [in] */ Int32 childEnd,
        /* [in] */ Int32 childSize,
        /* [in] */ Int32 startMargin,
        /* [in] */ Int32 endMargin,
        /* [in] */ Int32 startPadding,
        /* [in] */ Int32 endPadding,
        /* [in] */ Int32 mySize);

    CARAPI_(Boolean) PositionChildHorizontal(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params,
        /* [in] */ Int32 myWidth,
        /* [in] */ Boolean wrapContent);

    CARAPI_(Boolean) PositionChildVertical(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params,
        /* [in] */ Int32 myHeight,
        /* [in] */ Boolean wrapContent);

    CARAPI_(void) ApplyHorizontalSizeRules(
        /* [in] */ CRelativeLayoutLayoutParams* childParams,
        /* [in] */ Int32 myWidth);

    CARAPI_(void) ApplyVerticalSizeRules(
        /* [in] */ CRelativeLayoutLayoutParams* childParams,
        /* [in] */ Int32 myHeight);

    CARAPI_(AutoPtr<IView>) GetRelatedView(
        /* [in] */ ArrayOf<Int32>* rules,
        /* [in] */ Int32 relation);

    CARAPI_(AutoPtr<CRelativeLayoutLayoutParams>) GetRelatedViewParams(
        /* [in] */ ArrayOf<Int32>* rules,
        /* [in] */ Int32 relation);

    CARAPI_(Int32) GetRelatedViewBaseline(
        /* [in] */ ArrayOf<Int32>* rules,
        /* [in] */ Int32 relation);

    CARAPI_(void) CenterHorizontal(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params,
        /* [in] */ Int32 myWidth);

    CARAPI_(void) CenterVertical(
        /* [in] */ IView* child,
        /* [in] */ CRelativeLayoutLayoutParams* params,
        /* [in] */ Int32 myHeight);

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    friend class RelativeLayoutLayoutParams;

    static String LOGTAG;
    static Boolean DEBUG_GRAPH;

    static const AutoPtr<ArrayOf<Int32> > RULES_VERTICAL;
    static const AutoPtr<ArrayOf<Int32> > RULES_HORIZONTAL;

    AutoPtr<IView> mBaselineView;
    Boolean mHasBaselineAlignedChild;
    Int32 mGravity;
    AutoPtr<IRect> mContentBounds;
    AutoPtr<IRect> mSelfBounds;
    Int32 mIgnoreGravity;

    Set< AutoPtr<IView> > mTopToBottomLeftToRightSet;
    Boolean mDirtyHierarchy;
    AutoPtr<ArrayOf<IView*> > mSortedHorizontalChildren;
    AutoPtr<ArrayOf<IView*> > mSortedVerticalChildren;
    AutoPtr<DependencyGraph> mGraph;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__RelativeLayout_H__
