
#include "widget/RelativeLayout.h"
#include <R.h>
#include "view/Gravity.h"
#include "view/ViewGroup.h"
#include <elastos/Math.h>
#include <elastos/Logger.h>
#include <elastos/Slogger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IComparator;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::EIID_IPoolable;
using Elastos::Droid::Utility::EIID_IPoolableManager;
using Elastos::Droid::View::Gravity;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Widget {

// {9cf99c13-56c5-4194-ab03-a862cee2ab06}
extern "C" const InterfaceID EIID_NODE =
    { 0x9cf99c13, 0x56c5, 0x4194, { 0xab, 0x03, 0xa8, 0x62, 0xce, 0xe2, 0xab, 0x06 } };

static String TAG = String("RelativeLayout");

//==============================================================================
//  TopToBottomLeftToRightComparator
//==============================================================================

CAR_INTERFACE_IMPL(TopToBottomLeftToRightComparator, IComparator)

ECode TopToBottomLeftToRightComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    IView* first = IView::Probe(lhs);
    IView* second = IView::Probe(rhs);

    VALIDATE_NOT_NULL(first);
    VALIDATE_NOT_NULL(second);

    Int32 iFirst, iSecond;

    // top - bottom
    first->GetTop(&iFirst);
    second->GetTop(&iSecond);
    Int32 topDifference = iFirst - iSecond;
    if (topDifference != 0) {
        *result = topDifference;
        return NOERROR;
    }
    // left - right
    first->GetLeft(&iFirst);
    second->GetLeft(&iSecond);
    Int32 leftDifference = iFirst - iSecond;
    if (leftDifference != 0) {
        *result = leftDifference;
        return NOERROR;
    }
    // break tie by height
    first->GetHeight(&iFirst);
    second->GetHeight(&iSecond);
    Int32 heightDiference = iFirst - iSecond;
    if (heightDiference != 0) {
        *result = heightDiference;
        return NOERROR;
    }
    // break tie by width
    first->GetWidth(&iFirst);
    second->GetWidth(&iSecond);
    Int32 widthDiference = iFirst - iSecond;
    if (widthDiference != 0) {
        *result = widthDiference;
        return NOERROR;
    }

    return NOERROR;
}

//==============================================================================
//  Node::NodePoolableManager
//==============================================================================

CAR_INTERFACE_IMPL(Node::NodePoolableManager, IPoolableManager)

ECode Node::NodePoolableManager::NewInstance(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = new Node();
    if (*element == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    INTERFACE_ADDREF(*element);
    return NOERROR;
}

ECode Node::NodePoolableManager::OnAcquired(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

ECode Node::NodePoolableManager::OnReleased(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

//==============================================================================
//  Node
//==============================================================================
static AutoPtr<IPool> InitPool()
{
    AutoPtr<IPoolableManager> pm = (IPoolableManager*)new Node::NodePoolableManager();
    AutoPtr<IPool> pool = Pools::AcquireFinitePool(pm, Node::POOL_LIMIT);
    return Pools::AcquireSynchronizedPool(pool);
}

const Int32 Node::POOL_LIMIT;
const AutoPtr<IPool> Node::sPool = InitPool();

PInterface Node::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IPoolable*)this;
    }
    else if (riid == EIID_IPoolable) {
        return (IPoolable*)this;
    }
    else if (riid == EIID_NODE) {
        return reinterpret_cast<IInterface*>(this);
    }
    return NULL;
}

ECode Node::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IPoolable*)this) {
        *iid = EIID_IPoolable;
    }
    return NOERROR;
}

UInt32 Node::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Node::Release()
{
    return ElRefBase::Release();
}

Node::Node()
    : mIsPooled(FALSE)
{
}

ECode Node::SetNextPoolable(
    /* [in] */ IPoolable* element)
{
    mNext = element;
    return NOERROR;
}

ECode Node::GetNextPoolable(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = mNext;
    INTERFACE_ADDREF(*element);
    return NOERROR;
}

ECode Node::IsPooled(
    /* [out */ Boolean* isPooled)
{
    VALIDATE_NOT_NULL(isPooled);
    *isPooled = mIsPooled;
    return NOERROR;
}

ECode Node::SetPooled(
    /* [in] */ Boolean isPooled)
{
    mIsPooled = isPooled;
    return NOERROR;
}

Boolean Node::IsPooled()
{
    return mIsPooled;
}

AutoPtr<Node> Node::Acquire(
    /* [in] */ IView* view)
{
    AutoPtr<IPoolable> tmp;
    sPool->Acquire((IPoolable**)&tmp);
    AutoPtr<Node> node = reinterpret_cast<Node*>(tmp->Probe(EIID_NODE));
    node->mView = view;
    return node;
}

void Node::Clear()
{
    mView = NULL;
    mDependents.Clear();
    mDependencies.Clear();

    sPool->ReleaseElement((IPoolable*)this);
}

//==============================================================================
//  DependencyGraph
//==============================================================================

DependencyGraph::DependencyGraph()
{
}

void DependencyGraph::Clear()
{
    List< AutoPtr<Node> >::Iterator it;
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        (*it)->Clear();
    }
    mNodes.Clear();

    mKeyNodes.Clear();
    mRoots.Clear();
}

void DependencyGraph::Add(
    /* [in] */ IView* view)
{
    assert(view != NULL);

    Int32 id;
    view->GetId(&id);
    AutoPtr<Node> node = Node::Acquire(view);

    if (id != IView::NO_ID) {
        mKeyNodes[id] = node;
    }

    mNodes.PushBack(node);
}

ECode DependencyGraph::GetSortedViews(
    /* [in] */ ArrayOf<IView*>* sorted,
    /* [in] */ ArrayOf<Int32>* rules)
{
    VALIDATE_NOT_NULL(sorted);

    List< AutoPtr<Node> >& roots = FindRoots(rules);
    Int32 index = 0;
    while (!roots.IsEmpty()) {
        AutoPtr<Node> node = roots.GetBack();
        roots.PopBack();

        IView* view = node->mView;
        Int32 key;
        view->GetId(&key);
        assert(view);
        sorted->Set(index, view);
        index++;

        Node::DependentsIterator it = node->mDependents.Begin();
        for (; it != node->mDependents.End(); ++it) {
            AutoPtr<Node> dependent = it->mFirst;
            HashMap<Int32, Node* >* dependencies = &dependent->mDependencies;
            dependencies->Erase(key);
            if (dependencies->Begin() == dependencies->End()) {
                roots.PushBack(dependent);
            }
        }
    }

    if (index < sorted->GetLength()) {
        Slogger::E(TAG, "Circular dependencies cannot exist in RelativeLayout");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

/**
 * Finds the roots of the graph. A root is a node with no dependency and
 * with [0..n] dependents.
 *
 * @param rulesFilter The list of rules to consider when building the
 *        dependencies
 *
 * @return A list of node, each being a root of the graph
 */
List< AutoPtr<Node> >& DependencyGraph::FindRoots(
    /* [in] */ ArrayOf<Int32>* rulesFilter)
{
    assert(rulesFilter != NULL);
    List< AutoPtr<Node> >::Iterator it;

    // Find roots can be invoked several times, so make sure to clear
    // all dependents and dependencies before running the algorithm
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        AutoPtr<Node> node = *it;
        node->mDependents.Clear();
        node->mDependencies.Clear();
    }

    // Builds up the dependents and dependencies for each node of the graph
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        AutoPtr<Node> node = *it;

        AutoPtr<IRelativeLayoutLayoutParams> layoutParams;
        node->mView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        AutoPtr<ArrayOf<Int32> > rules;
        layoutParams->GetRules((ArrayOf<Int32>**)&rules);
        Int32 rulesCount = rulesFilter->GetLength();

        // Look only the the rules passed in parameter, this way we build only the
        // dependencies for a specific set of rules
        for (Int32 j = 0; j < rulesCount; j++) {
            Int32 rule = (*rules)[(*rulesFilter)[j]];
            if (rule > 0) {
                // The node this node depends on
                HashMap<Int32, AutoPtr<Node> >::Iterator it = mKeyNodes.Find(rule);
                AutoPtr<Node> dependency;
                if (it != mKeyNodes.End())
                   dependency = it->mSecond;

                // Skip unknowns and self dependencies
                if (dependency == NULL || dependency == node) {
                    continue;
                }
                // Add the current node as a dependent
                dependency->mDependents.Insert(Node::DependentsValueType(node, this));
                // Add a dependency to the current node
                node->mDependencies[rule] = dependency;
            }
        }
    }

    mRoots.Clear();

    // Finds all the roots in the graph: all nodes with no dependencies
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        AutoPtr<Node> node = *it;
        if (node->mDependencies.IsEmpty()) {
            mRoots.PushBack(node);
        }
    }
    return mRoots;
}

/**
 * Prints the dependency graph for the specified rules.
 *
 * @param resources The context's resources to print the ids.
 * @param rules The list of rules to take into account.
 */
void DependencyGraph::Log(
    /* [in] */ IResources* resources,
    /* [in] */ ArrayOf<Int32>* rules)
{
    List< AutoPtr<Node> >& roots = FindRoots(rules);
    List< AutoPtr<Node> >::Iterator it = roots.Begin();
    for (; it != roots.End(); ++it) {
        AutoPtr<Node> node = *it;
        PrintNode(resources, node);
    }
}

void DependencyGraph::PrintViewId(
    /* [in] */ IResources* resources,
    /* [in] */ IView* view)
{
    assert(view != NULL && resources != NULL);

    Int32 viewId;
    view->GetId(&viewId);
    if (viewId != IView::NO_ID) {
        String name;
        resources->GetResourceEntryName(viewId, &name);
        Logger::D(TAG, name.string());
    } else {
        Logger::D(TAG, "NO_ID");
    }
}

void DependencyGraph::AppendViewId(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node,
    /* [in] */ StringBuilder& buffer)
{
    assert(node != NULL);

    Int32 id;
    node->mView->GetId(&id);
    if (id != IView::NO_ID) {
        assert(resources != NULL);

        String name;
        resources->GetResourceEntryName(id, &name);
        buffer += name;
    }
    else {
        buffer += "NO_ID";
    }
}

void DependencyGraph::PrintNode(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node)
{
    assert(node != NULL);

   if (node->mDependents.IsEmpty()) {
       PrintViewId(resources, node->mView);
   }
   else {
        Node::DependentsIterator it = node->mDependents.Begin();
        for (; it != node->mDependents.End(); ++it) {
            AutoPtr<Node> dependent = it->mFirst;
            StringBuilder buffer;
            AppendViewId(resources, node, buffer);
            Printdependents(resources, dependent, buffer);
        }
   }
}

void DependencyGraph::Printdependents(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node,
    /* [in] */ StringBuilder& buffer)
{
    buffer += " -> ";
    AppendViewId(resources, node, buffer);

    if (node->mDependents.IsEmpty()) {
        Logger::D(TAG, buffer.ToString().string());
    }
    else {
        Node::DependentsIterator it = node->mDependents.Begin();
        for (; it != node->mDependents.End(); ++it) {
            AutoPtr<Node> dependent = it->mFirst;
            StringBuilder subBuffer(buffer.ToString());
            Printdependents(resources, dependent, subBuffer);
        }
   }
}

//==============================================================================
//  DependencyGraph
//==============================================================================

static AutoPtr<ArrayOf<Int32> > InitRULES_VERTICAL()
{
    AutoPtr<ArrayOf<Int32> > vRules = ArrayOf<Int32>::Alloc(5);
    (*vRules)[0] = IRelativeLayout::ABOVE;
    (*vRules)[1] = IRelativeLayout::BELOW;
    (*vRules)[2] = IRelativeLayout::ALIGN_BASELINE;
    (*vRules)[3] = IRelativeLayout::ALIGN_TOP;
    (*vRules)[4] = IRelativeLayout::ALIGN_BOTTOM;
    return vRules;
}

static AutoPtr<ArrayOf<Int32> > InitRULES_HORIZONTAL()
{
    AutoPtr<ArrayOf<Int32> > hRules = ArrayOf<Int32>::Alloc(8);
    (*hRules)[0] = IRelativeLayout::LEFT_OF;
    (*hRules)[1] = IRelativeLayout::RIGHT_OF;
    (*hRules)[2] = IRelativeLayout::ALIGN_LEFT;
    (*hRules)[3] = IRelativeLayout::ALIGN_RIGHT;
    (*hRules)[4] = IRelativeLayout::START_OF;
    (*hRules)[5] = IRelativeLayout::END_OF;
    (*hRules)[6] = IRelativeLayout::ALIGN_START;
    (*hRules)[7] = IRelativeLayout::ALIGN_END;
    return hRules;
}

String RelativeLayout::LOGTAG = String("RelativeLayout");
Boolean RelativeLayout::DEBUG_GRAPH = FALSE;

const AutoPtr<ArrayOf<Int32> > RelativeLayout::RULES_VERTICAL = InitRULES_VERTICAL();
const AutoPtr<ArrayOf<Int32> > RelativeLayout::RULES_HORIZONTAL = InitRULES_HORIZONTAL();

RelativeLayout::RelativeLayout()
    : mHasBaselineAlignedChild(FALSE)
    , mGravity(IGravity::START | IGravity::TOP)
    , mIgnoreGravity(0)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf<IView*>::Alloc(0);
    mSortedVerticalChildren = ArrayOf<IView*>::Alloc(0);
    mGraph = new DependencyGraph();
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mHasBaselineAlignedChild(FALSE)
    , mGravity(IGravity::START | IGravity::TOP)
    , mIgnoreGravity(0)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf<IView*>::Alloc(0);
    mSortedVerticalChildren = ArrayOf<IView*>::Alloc(0);
    mGraph = new DependencyGraph();
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
    , mHasBaselineAlignedChild(FALSE)
    , mGravity(IGravity::START | IGravity::TOP)
    , mIgnoreGravity(0)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf<IView*>::Alloc(0);
    mSortedVerticalChildren = ArrayOf<IView*>::Alloc(0);
    mGraph = new DependencyGraph();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mHasBaselineAlignedChild(FALSE)
    , mGravity(IGravity::START | IGravity::TOP)
    , mIgnoreGravity(0)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf<IView*>::Alloc(0);
    mSortedVerticalChildren = ArrayOf<IView*>::Alloc(0);
    mGraph = new DependencyGraph();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

RelativeLayout::~RelativeLayout()
{
    mTopToBottomLeftToRightSet.Clear();
    mBaselineView = NULL;
    mContentBounds = NULL;
    mSelfBounds = NULL;
    mSortedHorizontalChildren = NULL;
    mSortedVerticalChildren = NULL;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context));
    return NOERROR;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
    return NOERROR;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
    return NOERROR;
}

ECode RelativeLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    VALIDATE_NOT_NULL(context);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::RelativeLayout),
            ARRAY_SIZE(R::styleable::RelativeLayout));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx2(
            attrs, attrIds, (ITypedArray**)&a));

    a->GetResourceId(R::styleable::RelativeLayout_ignoreGravity, IView::NO_ID, &mIgnoreGravity);
    a->GetInt32(R::styleable::RelativeLayout_gravity, mGravity, &mGravity);
    a->Recycle();

    return NOERROR;
}

Boolean RelativeLayout::ShouldDelayChildPressedState()
{
    return FALSE;
}

ECode RelativeLayout::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    mIgnoreGravity = viewId;
    return NOERROR;
}

Int32 RelativeLayout::GetGravity()
{
    return mGravity;
}

ECode RelativeLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        if ((gravity & IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK) == 0) {
            gravity |= IGravity::START;
        }

        if ((gravity & IGravity::VERTICAL_GRAVITY_MASK) == 0) {
            gravity |= IGravity::TOP;
        }

        mGravity = gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    Int32 gravity = horizontalGravity & IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK;
    if ((mGravity & IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK) | gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    Int32 gravity = verticalGravity & IGravity::VERTICAL_GRAVITY_MASK;
    if ((mGravity & IGravity::VERTICAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~IGravity::VERTICAL_GRAVITY_MASK) | gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::GetBaseline(
    /* [out] */ Int32* baseline)
{
    VALIDATE_NOT_NULL(baseline);
    return mBaselineView != NULL ?
            mBaselineView->GetBaseline(baseline) : ViewGroup::GetBaseline(baseline);
}

ECode RelativeLayout::RequestLayout()
{
    FAIL_RETURN(ViewGroup::RequestLayout());
    mDirtyHierarchy = TRUE;
    return NOERROR;
}

void RelativeLayout::SortChildren()
{
    Int32 count = GetChildCount();
    if (mSortedVerticalChildren->GetLength() != count) {
        mSortedVerticalChildren = ArrayOf<IView*>::Alloc(count);
    }
    if (mSortedHorizontalChildren->GetLength() != count) {
        mSortedHorizontalChildren = ArrayOf<IView*>::Alloc(count);
    }

    AutoPtr<DependencyGraph> graph = mGraph;
    graph->Clear();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        graph->Add(child);
    }

    if (DEBUG_GRAPH) {
        Logger::D(LOGTAG, "=== Sorted vertical children");
        graph->Log(GetResources(), RULES_VERTICAL);

        Logger::D(LOGTAG, "=== Sorted horizontal children");
        graph->Log(GetResources(), RULES_HORIZONTAL);
    }

    mGraph->GetSortedViews(mSortedVerticalChildren, RULES_VERTICAL);
    mGraph->GetSortedViews(mSortedHorizontalChildren, RULES_HORIZONTAL);

    if (DEBUG_GRAPH) {
        Logger::D(LOGTAG, "=== Ordered list of vertical children");
        for (Int32 i = 0; i < mSortedVerticalChildren->GetLength(); ++i) {
            DependencyGraph::PrintViewId(GetResources(), (*mSortedVerticalChildren)[i]);
        }
        Logger::D(LOGTAG, "=== Ordered list of horizontal children");
        for (Int32 i = 0; i < mSortedHorizontalChildren->GetLength(); ++i) {
            DependencyGraph::PrintViewId(GetResources(), (*mSortedHorizontalChildren)[i]);
        }
    }
}

//TODO: we need to find another way to implement RelativeLayout
// This implementation cannot handle every case
void RelativeLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mDirtyHierarchy) {
        mDirtyHierarchy = FALSE;
        SortChildren();
    }

    Int32 myWidth = -1;
    Int32 myHeight = -1;

    Int32 width = 0;
    Int32 height = 0;

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    // Record our dimensions if they are known;
    if (widthMode != MeasureSpec::UNSPECIFIED) {
        myWidth = widthSize;
    }

    if (heightMode != MeasureSpec::UNSPECIFIED) {
        myHeight = heightSize;
    }

    if (widthMode == MeasureSpec::EXACTLY) {
        width = myWidth;
    }

    if (heightMode == MeasureSpec::EXACTLY) {
        height = myHeight;
    }

    mHasBaselineAlignedChild = FALSE;

    AutoPtr<IView> ignore;
    Int32 gravity = mGravity & IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK;
    Boolean horizontalGravity = gravity != IGravity::START && gravity != 0;
    gravity = mGravity & IGravity::VERTICAL_GRAVITY_MASK;
    Boolean verticalGravity = gravity != IGravity::TOP && gravity != 0;

    Int32 left = Elastos::Core::Math::INT32_MAX_VALUE;
    Int32 top = Elastos::Core::Math::INT32_MAX_VALUE;
    Int32 right = Elastos::Core::Math::INT32_MIN_VALUE;
    Int32 bottom = Elastos::Core::Math::INT32_MIN_VALUE;

    Boolean offsetHorizontalAxis = FALSE;
    Boolean offsetVerticalAxis = FALSE;

    if ((horizontalGravity || verticalGravity) && mIgnoreGravity != IView::NO_ID) {
        ignore = FindViewById(mIgnoreGravity);
    }

    Boolean isWrapContentWidth = widthMode != MeasureSpec::EXACTLY;
    Boolean isWrapContentHeight = heightMode != MeasureSpec::EXACTLY;

    AutoPtr<ArrayOf<IView*> > views = mSortedHorizontalChildren;
    Int32 count = views->GetLength();

    // We need to know our size for doing the correct computation of positioning in RTL mode
    if (IsLayoutRtl() && (myWidth == -1 || isWrapContentWidth)) {
        Int32 w = GetPaddingStart() + GetPaddingEnd();
        Int32 childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = (*views)[i];
            Int32 visibility;
            child->GetVisibility(&visibility);
            if (visibility != IView::GONE) {
                AutoPtr<IRelativeLayoutLayoutParams> params;
                child->GetLayoutParams((IViewGroupLayoutParams**)&params);
                // Would be similar to a call to measureChildHorizontal(child, params, -1, myHeight)
                // but we cannot change for now the behavior of measureChildHorizontal() for
                // taking care or a "-1" for "mywidth" so use here our own version of that code.
                Int32 childHeightMeasureSpec;
                Int32 width;
                params->GetWidth(&width);
                if (width == IViewGroupLayoutParams::MATCH_PARENT) {
                    childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::EXACTLY);
                } else {
                    childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::AT_MOST);
                }
                child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);

                child->GetMeasuredWidth(&width);
                w += width;
                Int32 leftMargin, rightMargin;
                params->GetLeftMargin(&leftMargin);
                params->GetRightMargin(&rightMargin);
                w += leftMargin + rightMargin;
            }
        }
        if (myWidth == -1) {
            // Easy case: "myWidth" was undefined before so use the width we have just computed
            myWidth = w;
        } else {
            // "myWidth" was defined before, so take the min of it and the computed width if it
            // is a non null one
            if (w > 0) {
                myWidth = Elastos::Core::Math::Min(myWidth, w);
            }
        }
    }

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = (*views)[i];
        assert(child);

        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != IView::GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
            ApplyHorizontalSizeRules(lp, myWidth);
            MeasureChildHorizontal(child, lp, myWidth, myHeight);
            if (PositionChildHorizontal(child, lp, myWidth, isWrapContentWidth)) {
                offsetHorizontalAxis = TRUE;
            }
        }
    }

    views = mSortedVerticalChildren;
    count = views->GetLength();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = (*views)[i];
        assert(child);

        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != IView::GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
            assert(lp);
            ApplyVerticalSizeRules(lp, myHeight);
            MeasureChild(child, lp, myWidth, myHeight);
            if (PositionChildVertical(child, lp, myHeight, isWrapContentHeight)) {
                offsetVerticalAxis = TRUE;
            }

            if (isWrapContentWidth) {
                width = Elastos::Core::Math::Max(width, lp->mRight);
            }

            if (isWrapContentHeight) {
                height = Elastos::Core::Math::Max(height, lp->mBottom);
            }

            if (child != ignore || verticalGravity) {
                left = Elastos::Core::Math::Min(left, lp->mLeft - lp->mLeftMargin);
                top = Elastos::Core::Math::Min(top, lp->mTop - lp->mTopMargin);
            }

            if (child != ignore || horizontalGravity) {
                right = Elastos::Core::Math::Max(right, lp->mRight + lp->mRightMargin);
                bottom = Elastos::Core::Math::Max(bottom, lp->mBottom + lp->mBottomMargin);
            }
        }
    }

    if (mHasBaselineAlignedChild) {
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = (*views)[i];
            Int32 visibility;
            child->GetVisibility(&visibility);
            if (visibility != IView::GONE) {
                AutoPtr<IRelativeLayoutLayoutParams> params;
                child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                AlignBaseline(child, lp);
                if (child != ignore || verticalGravity) {
                    left = Elastos::Core::Math::Min(left, lp->mLeft - lp->mLeftMargin);
                    top = Elastos::Core::Math::Min(top, lp->mTop - lp->mTopMargin);
                }

                if (child != ignore || horizontalGravity) {
                    right = Elastos::Core::Math::Max(right, lp->mRight + lp->mRightMargin);
                    bottom = Elastos::Core::Math::Max(bottom, lp->mBottom + lp->mBottomMargin);
                }
            }
        }
    }

    Int32 layoutDirection = GetLayoutDirection();

    if (isWrapContentWidth) {
        // Width already has left padding in it since it was calculated by looking at
        // the right of each child view
        width += mPaddingRight;

        Int32 lpWidth;
        mLayoutParams->GetWidth(&lpWidth);
        if (lpWidth >= 0) {
            width = Elastos::Core::Math::Max(width, lpWidth);
        }

        width = Elastos::Core::Math::Max(width, GetSuggestedMinimumWidth());
        width = ResolveSize(width, widthMeasureSpec);

        if (offsetHorizontalAxis) {
            for (Int32 i = 0; i < count; ++i) {
                AutoPtr<IView> child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != IView::GONE) {
                    AutoPtr<IRelativeLayoutLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                    CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                    AutoPtr<ArrayOf<Int32> > rules;
                    lp->GetRulesEx(layoutDirection, (ArrayOf<Int32>**)&rules);
                    if ((*rules)[IRelativeLayout::CENTER_IN_PARENT] != 0 ||
                            (*rules)[IRelativeLayout::CENTER_HORIZONTAL] != 0) {
                        CenterHorizontal(child, lp, width);
                    }
                    else if ((*rules)[IRelativeLayout::ALIGN_PARENT_RIGHT] != 0) {
                        Int32 childWidth;
                        child->GetMeasuredWidth(&childWidth);
                        lp->mLeft = width - mPaddingRight - childWidth;
                        lp->mRight = lp->mLeft + childWidth;
                    }
                }
            }
        }
    }

    if (isWrapContentHeight) {
        // Height already has top padding in it since it was calculated by looking at
        // the bottom of each child view
        height += mPaddingBottom;

        Int32 lpHeight;
        mLayoutParams->GetHeight(&lpHeight);
        if (lpHeight >= 0) {
            height = Elastos::Core::Math::Max(height, lpHeight);
        }

        height = Elastos::Core::Math::Max(height, GetSuggestedMinimumHeight());
        height = ResolveSize(height, heightMeasureSpec);

        if (offsetVerticalAxis) {
            for (Int32 i = 0; i < count; i++) {
                AutoPtr<IView> child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != IView::GONE) {
                    AutoPtr<IRelativeLayoutLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                    CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                    AutoPtr<ArrayOf<Int32> > rules;
                    lp->GetRulesEx(layoutDirection, (ArrayOf<Int32>**)&rules);
                    if ((*rules)[IRelativeLayout::CENTER_IN_PARENT] != 0 ||
                            (*rules)[IRelativeLayout::CENTER_VERTICAL] != 0) {
                        CenterVertical(child, lp, height);
                    }
                    else if ((*rules)[IRelativeLayout::ALIGN_PARENT_BOTTOM] != 0) {
                        Int32 childHeight;
                        child->GetMeasuredHeight(&childHeight);
                        lp->mTop = height - mPaddingBottom - childHeight;
                        lp->mBottom = lp->mTop + childHeight;
                    }
                }
            }
        }
    }

    if (horizontalGravity || verticalGravity) {
        mSelfBounds->Set(mPaddingLeft, mPaddingTop, width - mPaddingRight,
                height - mPaddingBottom);

        Gravity::Apply(mGravity, right - left, bottom - top,
            mSelfBounds, mContentBounds, layoutDirection);

        Int32 horizontalOffset = ((CRect*)mContentBounds.Get())->mLeft - left;
        Int32 verticalOffset = ((CRect*)mContentBounds.Get())->mTop - top;
        if (horizontalOffset != 0 || verticalOffset != 0) {
            for (Int32 i = 0; i < count; ++i) {
                AutoPtr<IView> child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != IView::GONE && child != ignore) {
                    AutoPtr<IRelativeLayoutLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
                    CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                    if (horizontalGravity) {
                        lp->mLeft += horizontalOffset;
                        lp->mRight += horizontalOffset;
                    }
                    if (verticalGravity) {
                        lp->mTop += verticalOffset;
                        lp->mBottom += verticalOffset;
                    }
                }
            }
        }
    }

    SetMeasuredDimension(width, height);
}

void RelativeLayout::AlignBaseline(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params)
{
    assert(child != NULL && params != NULL);

    Int32 layoutDirection = GetLayoutDirection();

    AutoPtr<ArrayOf<Int32> > rules;
    params->GetRulesEx(layoutDirection, (ArrayOf<Int32>**)&rules);
    Int32 anchorBaseline = GetRelatedViewBaseline(rules, IRelativeLayout::ALIGN_BASELINE);

    if (anchorBaseline != -1) {
        AutoPtr<CRelativeLayoutLayoutParams> anchorParams =
                GetRelatedViewParams(rules, IRelativeLayout::ALIGN_BASELINE);
        if (anchorParams != NULL) {
            Int32 offset = anchorParams->mTop + anchorBaseline;
            Int32 baseline;
            child->GetBaseline(&baseline);
            if (baseline != -1) {
                offset -= baseline;
            }
            Int32 height = params->mBottom - params->mTop;
            params->mTop = offset;
            params->mBottom = params->mTop + height;
        }
    }

    if (mBaselineView == NULL) {
        mBaselineView = child;
    }
    else {
        AutoPtr<IRelativeLayoutLayoutParams> layoutParams;
        mBaselineView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)layoutParams.Get();
        if (params->mTop < lp->mTop || (params->mTop == lp->mTop && params->mLeft < lp->mLeft)) {
            mBaselineView = child;
        }
    }
}

void RelativeLayout::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(params->mLeft,
            params->mRight, params->mWidth,
            params->mLeftMargin, params->mRightMargin,
            mPaddingLeft, mPaddingRight, myWidth);
    Int32 childHeightMeasureSpec = GetChildMeasureSpec(params->mTop,
            params->mBottom, params->mHeight,
            params->mTopMargin, params->mBottomMargin,
            mPaddingTop, mPaddingBottom, myHeight);
    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void RelativeLayout::MeasureChildHorizontal(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(params->mLeft,
            params->mRight, params->mWidth,
            params->mLeftMargin, params->mRightMargin,
            mPaddingLeft, mPaddingRight, myWidth);
    Int32 childHeightMeasureSpec;
    if (params->mWidth == IViewGroupLayoutParams::MATCH_PARENT) {
        childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::EXACTLY);
    }
    else {
        childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::AT_MOST);
    }
    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

Int32 RelativeLayout::GetChildMeasureSpec(
    /* [in] */ Int32 childStart,
    /* [in] */ Int32 childEnd,
    /* [in] */ Int32 childSize,
    /* [in] */ Int32 startMargin,
    /* [in] */ Int32 endMargin,
    /* [in] */ Int32 startPadding,
    /* [in] */ Int32 endPadding,
    /* [in] */ Int32 mySize)
{
    Int32 childSpecMode = 0;
    Int32 childSpecSize = 0;

    // Figure out start and end bounds.
    Int32 tempStart = childStart;
    Int32 tempEnd = childEnd;

    // If the view did not express a layout constraint for an edge, use
    // view's margins and our padding
    if (tempStart < 0) {
        tempStart = startPadding + startMargin;
    }
    if (tempEnd < 0) {
        tempEnd = mySize - endPadding - endMargin;
    }

    // Figure out maximum size available to this view
    Int32 maxAvailable = tempEnd - tempStart;

    if (childStart >= 0 && childEnd >= 0) {
        // Constraints fixed both edges, so child must be an exact size
        childSpecMode = MeasureSpec::EXACTLY;
        childSpecSize = maxAvailable;
    }
    else {
        if (childSize >= 0) {
            // Child wanted an exact size. Give as much as possible
            childSpecMode = MeasureSpec::EXACTLY;

            if (maxAvailable >= 0) {
                // We have a maxmum size in this dimension.
                childSpecSize = Elastos::Core::Math::Min(maxAvailable, childSize);
            }
            else {
                // We can grow in this dimension.
                childSpecSize = childSize;
            }
        }
        else if (childSize == IViewGroupLayoutParams::MATCH_PARENT) {
            // Child wanted to be as big as possible. Give all availble
            // space
            childSpecMode = MeasureSpec::EXACTLY;
            childSpecSize = maxAvailable;
        }
        else if (childSize == IViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to wrap content. Use AT_MOST
            // to communicate available space if we know
            // our max size
            if (maxAvailable >= 0) {
                // We have a maxmum size in this dimension.
                childSpecMode = MeasureSpec::AT_MOST;
                childSpecSize = maxAvailable;
            }
            else {
                // We can grow in this dimension. Child can be as big as it
                // wants
                childSpecMode = MeasureSpec::UNSPECIFIED;
                childSpecSize = 0;
            }
        }
    }

    return MeasureSpec::MakeMeasureSpec(childSpecSize, childSpecMode);
}

Boolean RelativeLayout::PositionChildHorizontal(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Boolean wrapContent)
{
    assert(child != NULL && params != NULL);

    Int32 layoutDirection = GetLayoutDirection();

    AutoPtr<ArrayOf<Int32> > rules;
    params->GetRulesEx(layoutDirection, (ArrayOf<Int32>**)&rules);

    Int32 width;
    child->GetMeasuredWidth(&width);

    if (params->mLeft < 0 && params->mRight >= 0) {
        // Right is fixed, but left varies
        params->mLeft = params->mRight - width;
    }
    else if (params->mLeft >= 0 && params->mRight < 0) {
        // Left is fixed, but right varies
        params->mRight = params->mLeft + width;
    }
    else if (params->mLeft < 0 && params->mRight < 0) {
        // Both left and right vary
        if ((*rules)[IRelativeLayout::CENTER_IN_PARENT] != 0
                || (*rules)[IRelativeLayout::CENTER_HORIZONTAL] != 0) {
            if (!wrapContent) {
                CenterHorizontal(child, params, myWidth);
            }
            else {
                params->mLeft = mPaddingLeft + params->mLeftMargin;
                params->mRight = params->mLeft + width;
            }
            return TRUE;
        }
        else {
            // This is the default case. For RTL we start from the right and for LTR we start
            // from the left. This will give LEFT/TOP for LTR and RIGHT/TOP for RTL.
            if (IsLayoutRtl()) {
                params->mRight = myWidth - mPaddingRight- params->mRightMargin;
                params->mLeft = params->mRight - width;
            } else {
                params->mLeft = mPaddingLeft + params->mLeftMargin;
                params->mRight = params->mLeft + width;
            }
        }
    }
    return (*rules)[IRelativeLayout::ALIGN_PARENT_END] != 0;
}

Boolean RelativeLayout::PositionChildVertical(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myHeight,
    /* [in] */ Boolean wrapContent)
{
    assert(child != NULL && params != NULL);

    Int32 height;
    child->GetMeasuredHeight(&height);

    AutoPtr<ArrayOf<Int32> > rules;
    params->GetRules((ArrayOf<Int32>**)&rules);

    if (params->mTop < 0 && params->mBottom >= 0) {
        // Bottom is fixed, but top varies
        params->mTop = params->mBottom - height;
    }
    else if (params->mTop >= 0 && params->mBottom < 0) {
        // Top is fixed, but bottom varies
        params->mBottom = params->mTop + height;
    }
    else if (params->mTop < 0 && params->mBottom < 0) {
        // Both top and bottom vary
        if ((*rules)[IRelativeLayout::CENTER_IN_PARENT] != 0 || (*rules)[IRelativeLayout::CENTER_VERTICAL] != 0) {
            if (!wrapContent) {
                CenterVertical(child, params, myHeight);
            }
            else {
                params->mTop = mPaddingTop + params->mTopMargin;
                params->mBottom = params->mTop + height;
            }
            return TRUE;
        }
        else {
            params->mTop = mPaddingTop + params->mTopMargin;
            params->mBottom = params->mTop + height;
        }
    }
    return (*rules)[IRelativeLayout::ALIGN_PARENT_BOTTOM] != 0;
}

void RelativeLayout::ApplyHorizontalSizeRules(
    /* [in] */ CRelativeLayoutLayoutParams* childParams,
    /* [in] */ Int32 myWidth)
{
    assert(childParams != NULL);

    Int32 layoutDirection = GetLayoutDirection();

    AutoPtr<ArrayOf<Int32> > rules;
    childParams->GetRulesEx(layoutDirection, (ArrayOf<Int32>**)&rules);
    AutoPtr<CRelativeLayoutLayoutParams> anchorParams;

    // -1 indicated a "soft requirement" in that direction. For example:
    // left=10, right=-1 means the view must start at 10, but can go as far as it wants to the right
    // left =-1, right=10 means the view must end at 10, but can go as far as it wants to the left
    // left=10, right=20 means the left and right ends are both fixed
    childParams->mLeft = -1;
    childParams->mRight = -1;

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::LEFT_OF);
    if (anchorParams != NULL) {
        childParams->mRight =
                anchorParams->mLeft - (anchorParams->mLeftMargin + childParams->mRightMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::LEFT_OF] != 0) {
        if (myWidth >= 0) {
            childParams->mRight =
                    myWidth - mPaddingRight - childParams->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::RIGHT_OF);
    if (anchorParams != NULL) {
        childParams->mLeft =
                anchorParams->mRight + (anchorParams->mRightMargin + childParams->mLeftMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::RIGHT_OF] != 0) {
        childParams->mLeft = mPaddingLeft + childParams->mLeftMargin;
    }

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::ALIGN_LEFT);
    if (anchorParams != NULL) {
        childParams->mLeft = anchorParams->mLeft + childParams->mLeftMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::ALIGN_LEFT] != 0) {
        childParams->mLeft = mPaddingLeft + childParams->mLeftMargin;
    }

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::ALIGN_RIGHT);
    if (anchorParams != NULL) {
        childParams->mRight = anchorParams->mRight - childParams->mRightMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::ALIGN_RIGHT] != 0) {
        if (myWidth >= 0) {
            childParams->mRight = myWidth - mPaddingRight - childParams->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if (0 != (*rules)[IRelativeLayout::ALIGN_PARENT_LEFT]) {
        childParams->mLeft = mPaddingLeft + childParams->mLeftMargin;
    }

    if (0 != (*rules)[IRelativeLayout::ALIGN_PARENT_RIGHT]) {
        if (myWidth >= 0) {
            childParams->mRight = myWidth - mPaddingRight - childParams->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }
}

void RelativeLayout::ApplyVerticalSizeRules(
    /* [in] */ CRelativeLayoutLayoutParams* childParams,
    /* [in] */ Int32 myHeight)
{
    assert(childParams != NULL);

    AutoPtr<ArrayOf<Int32> > rules;
    childParams->GetRules((ArrayOf<Int32>**)&rules);
    AutoPtr<CRelativeLayoutLayoutParams> anchorParams;

    childParams->mTop = -1;
    childParams->mBottom = -1;

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::ABOVE);
    if (anchorParams != NULL) {
        childParams->mBottom =
                anchorParams->mTop - (anchorParams->mTopMargin + childParams->mBottomMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::ABOVE] != 0) {
        if (myHeight >= 0) {
            childParams->mBottom = myHeight - mPaddingBottom - childParams->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::BELOW);
    if (anchorParams != NULL) {
        childParams->mTop =
                anchorParams->mBottom + (anchorParams->mBottomMargin + childParams->mTopMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::BELOW] != 0) {
        childParams->mTop = mPaddingTop + childParams->mTopMargin;
    }

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::ALIGN_TOP);
    if (anchorParams != NULL) {
        childParams->mTop = anchorParams->mTop + childParams->mTopMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::ALIGN_TOP] != 0) {
        childParams->mTop = mPaddingTop + childParams->mTopMargin;
    }

    anchorParams = GetRelatedViewParams(rules, IRelativeLayout::ALIGN_BOTTOM);
    if (anchorParams != NULL) {
        childParams->mBottom = anchorParams->mBottom - childParams->mBottomMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[IRelativeLayout::ALIGN_BOTTOM] != 0) {
        if (myHeight >= 0) {
            childParams->mBottom = myHeight - mPaddingBottom - childParams->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if (0 != (*rules)[IRelativeLayout::ALIGN_PARENT_TOP]) {
        childParams->mTop = mPaddingTop + childParams->mTopMargin;
    }

    if (0 != (*rules)[IRelativeLayout::ALIGN_PARENT_BOTTOM]) {
        if (myHeight >= 0) {
            childParams->mBottom = myHeight - mPaddingBottom - childParams->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if ((*rules)[IRelativeLayout::ALIGN_BASELINE] != 0) {
        mHasBaselineAlignedChild = TRUE;
    }
}

AutoPtr<IView> RelativeLayout::GetRelatedView(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    assert(rules != NULL);

    AutoPtr<IView> v;
    Int32 id = (*rules)[relation];
    if (id != 0) {
        HashMap<Int32, AutoPtr<Node> >::Iterator it = mGraph->mKeyNodes.Find(id);
        Node* node = it == mGraph->mKeyNodes.End() ? NULL : it->mSecond;
        if (node == NULL)
            return v;

        v = node->mView;

        // Find the first non-GONE view up the chain
        Int32 visibility;
        v->GetVisibility(&visibility);
        while (visibility == IView::GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            v->GetLayoutParams((IViewGroupLayoutParams**)&params);
            AutoPtr<ArrayOf<Int32> > rules;
            Int32 direction;
            v->GetLayoutDirection(&direction);
            params->GetRulesEx(direction, (ArrayOf<Int32>**)&rules);

            it = mGraph->mKeyNodes.Find((*rules)[relation]);
            node = it == mGraph->mKeyNodes.End() ? NULL : it->mSecond;
            if (node == NULL) return v;
            v = node->mView;
            v->GetVisibility(&visibility);
        }

        return v;
    }

    return v;
}

AutoPtr<CRelativeLayoutLayoutParams> RelativeLayout::GetRelatedViewParams(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    AutoPtr<CRelativeLayoutLayoutParams> lp;
    AutoPtr<IView> v = GetRelatedView(rules, relation);
    if (v != NULL) {
        AutoPtr<IViewGroupLayoutParams> params;
        v->GetLayoutParams((IViewGroupLayoutParams**)&params);
        if (params != NULL && params->Probe(EIID_IRelativeLayoutLayoutParams) != NULL) {
             lp = (CRelativeLayoutLayoutParams*)IRelativeLayoutLayoutParams::Probe(params);
        }
    }

    return lp;
}

Int32 RelativeLayout::GetRelatedViewBaseline(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    assert(rules != NULL);

    AutoPtr<IView> v = GetRelatedView(rules, relation);
    if (v != NULL) {
        Int32 baseline;
        v->GetBaseline(&baseline);
        return baseline;
    }
    return -1;
}

void RelativeLayout::CenterHorizontal(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth)
{
    assert(child != NULL);

    Int32 childWidth;
    child->GetMeasuredWidth(&childWidth);
    Int32 left = (myWidth - childWidth) / 2;

    params->mLeft = left;
    params->mRight = left + childWidth;
}

void RelativeLayout::CenterVertical(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childHeight;
    child->GetMeasuredHeight(&childHeight);
    Int32 top = (myHeight - childHeight) / 2;

    params->mTop = top;
    params->mBottom = top + childHeight;
}

void RelativeLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    //  The layout has actually already been performed and the positions
    //  cached.  Apply the cached values to the children.
    Int32 count = GetChildCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != IView::GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);
            CRelativeLayoutLayoutParams* st = (CRelativeLayoutLayoutParams*)params.Get();
            child->Layout(st->mLeft, st->mTop, st->mRight, st->mBottom);
        }
    }
}

ECode RelativeLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    assert(attrs != NULL);
    AutoPtr<IRelativeLayoutLayoutParams> lp;
    FAIL_RETURN(CRelativeLayoutLayoutParams::New(GetContext(), attrs, (IRelativeLayoutLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    INTERFACE_ADDREF(*params);
    return NOERROR;
}

ECode RelativeLayout::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** lp)
{
    VALIDATE_NOT_NULL(lp);
    return CRelativeLayoutLayoutParams::New(
                IViewGroupLayoutParams::WRAP_CONTENT,
                IViewGroupLayoutParams::WRAP_CONTENT,
                (IRelativeLayoutLayoutParams**)lp);
}

Boolean RelativeLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p != NULL && IRelativeLayoutLayoutParams::Probe(p) != NULL;
}

AutoPtr<IViewGroupLayoutParams> RelativeLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    assert(p != NULL);

    AutoPtr<IViewGroupLayoutParams> lp;
    ASSERT_SUCCEEDED(CRelativeLayoutLayoutParams::New(p,
            (IRelativeLayoutLayoutParams**)&lp));

    return lp;
}

Boolean RelativeLayout::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* p)
{
    // sort children top-to-bottom and left-to-right
    Int32 childCount = GetChildCount();
    for (Int32 i = 0; i < childCount; i++) {
        mTopToBottomLeftToRightSet.Insert(GetChildAt(i));
    }

    Int32 visibility;
    Set<AutoPtr<IView> >::Iterator it = mTopToBottomLeftToRightSet.Begin();
    for (; it != mTopToBottomLeftToRightSet.End(); ++it) {
        AutoPtr<IView> view = *it;
        Boolean result;
        view->DispatchPopulateAccessibilityEvent(p, &result);
        view->GetVisibility(&visibility);
        if (visibility == IView::VISIBLE && result) {
            mTopToBottomLeftToRightSet.Clear();
            return TRUE;
        }
    }

    mTopToBottomLeftToRightSet.Clear();
    return FALSE;
}

ECode RelativeLayout::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewGroup::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CRelativeLayout"), (ICharSequence**)&seq);
    return event->SetClassName(seq);
}

//@Override
ECode RelativeLayout::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewGroup::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CRelativeLayout"), (ICharSequence**)&seq);
    return info->SetClassName(seq);
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
