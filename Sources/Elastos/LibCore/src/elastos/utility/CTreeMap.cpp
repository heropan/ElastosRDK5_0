
#include "CTreeMap.h"
#include "CCollections.h"
#include "elastos/Math.h"

using Elastos::Core::IComparable;
using Elastos::Core::EIID_IComparable;
using Elastos::Core::IComparator;
using Elastos::Core::EIID_IComparator;
using Elastos::IO::EIID_ISerializable;
using Elastos::IO::IObjectOutputStreamPutField;
using Elastos::IO::IObjectInputStreamGetField;

namespace Elastos {
namespace Utility {


//==========================================================
//       CTreeMap
//==========================================================
const AutoPtr<IComparator> CTreeMap::NATURAL_ORDER = (IComparator*) new OrderComparator();

CTreeMap::CTreeMap()
    : mSize(0)
    , mModCount(0)
{
}

CTreeMap::Relation CTreeMap::ForOrder(
    /* [in] */ Boolean ascending,
    /* [in] */ Relation src)
{
    if (ascending) {
        return src;
    }

    switch (src) {
        case LOWER:
            return HIGHER;
        case FLOOR:
            return CEILING;
        case EQUAL:
            return EQUAL;
        case CEILING:
            return FLOOR;
        case HIGHER:
            return LOWER;
        default:
            // throw new IllegalStateException();
            return -9999;
    }
}

PInterface CTreeMap::Probe(
    /* [in] */ REIID riid)
{
    return _CTreeMap::Probe(riid);
}

ECode CTreeMap::constructor()
{
    mComparator = NATURAL_ORDER;
    return NOERROR;
}

ECode CTreeMap::constructor(
    /* [in] */ IMap* copyFrommap)
{
    this->constructor();
    AutoPtr<ArrayOf<IInterface*> > entries;
    AutoPtr<ISet> outset;
    copyFrommap->GetEntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&entries);
    for (Int32 i = 0; i < entries->GetLength(); i++) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe((*entries)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        entry->GetKey((IInterface**)&keyface);
        entry->GetValue((IInterface**)&valueface);
        PutInternal(keyface, valueface);
    }
    return NOERROR;
}

ECode CTreeMap::constructor(
    /* [in] */ IComparator* comparator)
{
    if (comparator != NULL) {
        mComparator = comparator;
    }
    else {
        mComparator = NATURAL_ORDER;
    }
    return NOERROR;
}

ECode CTreeMap::constructor(
    /* [in] */ ISortedMap* copyFrom)
{
    AutoPtr<IComparator> sourceComparator;
    copyFrom->Comparator((IComparator**)&copyFrom);
    if (sourceComparator != NULL) {
        mComparator = sourceComparator;
    }
    else {
        mComparator = NATURAL_ORDER;
    }
    AutoPtr<ArrayOf<IInterface*> > entries;
    AutoPtr<ISet> outset;
    copyFrom->GetEntrySet((ISet**)&entries);
    outset->ToArray((ArrayOf<IInterface*>**)&entries);
    for (Int32 i = 0; i < entries->GetLength(); i++) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe((*entries)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        entry->GetKey((IInterface**)&keyface);
        entry->GetValue((IInterface**)&valueface);
        PutInternal(keyface, valueface);
    }
    return NOERROR;
}

ECode CTreeMap::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    // try {
    // @SuppressWarnings("unchecked") // super.clone() must return the same type
    AutoPtr<CTreeMap> map;
    FAIL_RETURN(CTreeMap::NewByFriend((CTreeMap**)&map));
    map->mRoot = mRoot != NULL ? mRoot->Copy(NULL) : NULL;
    map->mEntrySet = NULL;
    map->mKeySet = NULL;
    *object = map->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object)
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError();
    // }
    return NOERROR;
}

ECode CTreeMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mSize;
    return NOERROR;
}

ECode CTreeMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mSize == 0;
    return NOERROR;
}

ECode CTreeMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IMapEntry> entry = FindByObject(key);
    AutoPtr<IInterface> valueface;
    *value = entry != NULL ? (entry->GetValue((IInterface**)&valueface), valueface) : NULL;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CTreeMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = FindByObject(key) != NULL;
    return NOERROR;
}

ECode CTreeMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    if (oldValue) {
        AutoPtr<IInterface> res = PutInternal(key, value);
        *oldValue = res;
        REFCOUNT_ADD(*oldValue)
    }
    else {
        PutInternal(key, value);
    }
    return NOERROR;
}

ECode CTreeMap::Clear()
{
    mRoot = NULL;
    mSize = 0;
    mModCount++;
    return NOERROR;
}

ECode CTreeMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<Node> node = RemoveInternalByKey(key);
    *value = node != NULL ? node->mValue : NULL;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

AutoPtr<IInterface> CTreeMap::PutInternal(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    AutoPtr<Node> created = Find(key, CREATE);
    AutoPtr<IInterface> result = created->mValue;
    created->mValue = value;
    return result;
}

AutoPtr<CTreeMap::Node> CTreeMap::Find(
    /* [in] */ IInterface* key,
    /* [in] */ Relation relation)
{
    if (mRoot == NULL) {
        if (mComparator == NATURAL_ORDER && !(IComparable::Probe(key))) {
            // throw new ClassCastException(key.getClass().getName() + " is not Comparable"); // NullPointerException ok
            return NULL;
        }
        if (relation == CREATE) {
            mRoot = new Node(NULL, key);
            mSize = 1;
            mModCount++;
            return mRoot;
        }
        else {
            return NULL;
        }
    }

    /*
     * Micro-optimization: avoid polymorphic calls to Comparator.compare().
     * This is 10% faster for naturally ordered trees.
     */
    // @SuppressWarnings("unchecked") // will throw a ClassCastException below if there's trouble
    AutoPtr<IComparable> comparableKey = (mComparator == NATURAL_ORDER)
            ? (IComparable*) key->Probe(EIID_IComparable)
            : NULL;

    AutoPtr<Node> nearest = mRoot;
    while (TRUE) {
        Int32 comvalue = 0;
        Int32 comparison = (comparableKey != NULL)
                ? (comparableKey->CompareTo(nearest->mKey, &comvalue), comvalue)
                : (mComparator->Compare(key, nearest->mKey, &comvalue), comvalue);

        /*
         * We found the requested key.
         */
        if (comparison == 0) {
            switch (relation) {
                case LOWER:
                    return nearest->Prev();
                case FLOOR:
                case EQUAL:
                case CREATE:
                case CEILING:
                    return nearest;
                case HIGHER:
                    return nearest->GetNext();
            }
        }

        AutoPtr<Node> child = (comparison < 0) ? nearest->mLeft : nearest->mRight;
        if (child != NULL) {
            nearest = child;
            continue;
        }

        /*
         * We found a nearest node. Every key not in the tree has up to two
         * nearest nodes, one lower and one higher.
         */
        if (comparison < 0) { // nearest.key is higher
            switch (relation) {
                case LOWER:
                case FLOOR:
                    return nearest->Prev();
                case CEILING:
                case HIGHER:
                    return nearest;
                case EQUAL:
                    return NULL;
                case CREATE:
                    AutoPtr<Node> created = new Node(nearest, key);
                    nearest->mLeft = created;
                    mSize++;
                    mModCount++;
                    Rebalance(nearest, TRUE);
                    return created;
            }
        }
        else { // comparison > 0, nearest.key is lower
            switch (relation) {
                case LOWER:
                case FLOOR:
                    return nearest;
                case CEILING:
                case HIGHER:
                    return nearest->GetNext();
                case EQUAL:
                    return NULL;
                case CREATE:
                    AutoPtr<Node> created = new Node(nearest, key);
                    nearest->mRight = created;
                    mSize++;
                    mModCount++;
                    Rebalance(nearest, TRUE);
                    return created;
            }
        }
    }
    return NULL;
}

AutoPtr<CTreeMap::Node> CTreeMap::FindByObject(
    /* [in] */ IInterface* key)
{
    AutoPtr<Node> res = Find(key, EQUAL);
    return res;
}

AutoPtr<CTreeMap::Node> CTreeMap::FindByEntry(
    /* [in] */ IMapEntry* entry)
{
    assert(entry);
    AutoPtr<IInterface> keyface;
    entry->GetKey((IInterface**)&keyface);
    AutoPtr<Node> mine = FindByObject(keyface);
    AutoPtr<IInterface> valueface;
    Boolean valuesEqual = mine != NULL && (Object::Equals(mine->mValue,
        (entry->GetValue((IInterface**)&valueface), valueface)));
    return valuesEqual ? mine : NULL;
}

ECode CTreeMap::RemoveInternal(
    /* [in] */ Node* node)
{
    AutoPtr<Node> left = node->mLeft;
    AutoPtr<Node> right = node->mRight;
    AutoPtr<Node> originalParent = node->mParent;
    if (left != NULL && right != NULL) {

        /*
         * To remove a node with both left and right subtrees, move an
         * adjacent node from one of those subtrees into this node's place.
         *
         * Removing the adjacent node may change this node's subtrees. This
         * node may no longer have two subtrees once the adjacent node is
         * gone!
         */

        AutoPtr<Node> adjacent = (left->mHeight > right->mHeight) ? left->Last() : right->First();
        RemoveInternal(adjacent); // takes care of rebalance and size--

        Int32 leftHeight = 0;
        left = node->mLeft;
        if (left != NULL) {
            leftHeight = left->mHeight;
            adjacent->mLeft = left;
            left->mParent = adjacent;
            node->mLeft = NULL;
        }
        Int32 rightHeight = 0;
        right = node->mRight;
        if (right != NULL) {
            rightHeight = right->mHeight;
            adjacent->mRight = right;
            right->mParent = adjacent;
            node->mRight = NULL;
        }
        adjacent->mHeight = Elastos::Core::Math::Max(leftHeight, rightHeight) + 1;
        return ReplaceInParent(node, adjacent);
    }
    else if (left != NULL) {
        ReplaceInParent(node, left);
        node->mLeft = NULL;
    }
    else if (right != NULL) {
        ReplaceInParent(node, right);
        node->mRight = NULL;
    }
    else {
        ReplaceInParent(node, NULL);
    }

    Rebalance(originalParent, FALSE);
    mSize--;
    mModCount++;
    return NOERROR;
}

AutoPtr<CTreeMap::Node> CTreeMap::RemoveInternalByKey(
    /* [in] */ IInterface* key)
{
    AutoPtr<Node> node = FindByObject(key);
    if (node != NULL) {
        RemoveInternal(node);
    }
    return node;
}

ECode CTreeMap::ReplaceInParent(
    /* [in] */ Node* node,
    /* [in] */ Node* replacement)
{
    AutoPtr<Node> parent = node->mParent;
    node->mParent = NULL;
    if (replacement != NULL) {
        replacement->mParent = parent;
    }

    if (parent != NULL) {
        if (parent->mLeft.Get() == node) {
            parent->mLeft = replacement;
        }
        else {
            assert(parent->mRight.Get() == node);
            parent->mRight = replacement;
        }
    }
    else {
        mRoot = replacement;
    }
    return NOERROR;
}

ECode CTreeMap::Rebalance(
    /* [in] */ Node* unbalanced,
    /* [in] */ Boolean insert)
{
    for (AutoPtr<Node> node = unbalanced; node != NULL; node = node->mParent) {
        AutoPtr<Node> left = node->mLeft;
        AutoPtr<Node> right = node->mRight;
        Int32 leftHeight = left != NULL ? left->mHeight : 0;
        Int32 rightHeight = right != NULL ? right->mHeight : 0;

        Int32 delta = leftHeight - rightHeight;
        if (delta == -2) {
            AutoPtr<Node> rightLeft = right->mLeft;
            AutoPtr<Node> rightRight = right->mRight;
            Int32 rightRightHeight = rightRight != NULL ? rightRight->mHeight : 0;
            Int32 rightLeftHeight = rightLeft != NULL ? rightLeft->mHeight : 0;

            Int32 rightDelta = rightLeftHeight - rightRightHeight;
            if (rightDelta == -1 || (rightDelta == 0 && !insert)) {
                RotateLeft(node); // AVL right right
            }
            else {
                assert (rightDelta == 1);
                RotateRight(right); // AVL right left
                RotateLeft(node);
            }
            if (insert) {
                break; // no further rotations will be necessary
            }

        }
        else if (delta == 2) {
            AutoPtr<Node> leftLeft = left->mLeft;
            AutoPtr<Node> leftRight = left->mRight;
            Int32 leftRightHeight = leftRight != NULL ? leftRight->mHeight : 0;
            Int32 leftLeftHeight = leftLeft != NULL ? leftLeft->mHeight : 0;

            Int32 leftDelta = leftLeftHeight - leftRightHeight;
            if (leftDelta == 1 || (leftDelta == 0 && !insert)) {
                RotateRight(node); // AVL left left
            }
            else {
                assert (leftDelta == -1);
                RotateLeft(left); // AVL left right
                RotateRight(node);
            }
            if (insert) {
                break; // no further rotations will be necessary
            }

        }
        else if (delta == 0) {
            node->mHeight = leftHeight + 1; // leftHeight == rightHeight
            if (insert) {
                break; // the insert caused balance, so rebalancing is done!
            }

        }
        else {
            assert (delta == -1 || delta == 1);
            node->mHeight = Elastos::Core::Math::Max(leftHeight, rightHeight) + 1;
            if (!insert) {
                break; // the height hasn't changed, so rebalancing is done!
            }
        }
    }
    return NOERROR;
}

ECode CTreeMap::RotateLeft(
    /* [in] */ Node* root)
{
    AutoPtr<Node> left = root->mLeft;
    AutoPtr<Node> pivot = root->mRight;
    AutoPtr<Node> pivotLeft = pivot->mLeft;
    AutoPtr<Node> pivotRight = pivot->mRight;

    // move the pivot's left child to the root's right
    root->mRight = pivotLeft;
    if (pivotLeft != NULL) {
        pivotLeft->mParent = root;
    }

    ReplaceInParent(root, pivot);

    // move the root to the pivot's left
    pivot->mLeft = root;
    root->mParent = pivot;

    // fix heights
    root->mHeight = Elastos::Core::Math::Max(left != NULL ? left->mHeight : 0, pivotLeft != NULL ? pivotLeft->mHeight : 0) + 1;
    pivot->mHeight = Elastos::Core::Math::Max(root->mHeight, pivotRight != NULL ? pivotRight->mHeight : 0) + 1;
    return NOERROR;
}

ECode CTreeMap::RotateRight(
    /* [in] */ Node* root)
{
    AutoPtr<Node> pivot = root->mLeft;
    AutoPtr<Node> right = root->mRight;
    AutoPtr<Node> pivotLeft = pivot->mLeft;
    AutoPtr<Node> pivotRight = pivot->mRight;

    // move the pivot's right child to the root's left
    root->mLeft = pivotRight;
    if (pivotRight != NULL) {
        pivotRight->mParent = root;
    }

    ReplaceInParent(root, pivot);

    // move the root to the pivot's right
    pivot->mRight = root;
    root->mParent = pivot;

    // fixup heights
    root->mHeight = Elastos::Core::Math::Max(right != NULL ? right->mHeight : 0,
            pivotRight != NULL ? pivotRight->mHeight : 0) + 1;
    pivot->mHeight = Elastos::Core::Math::Max(root->mHeight,
            pivotLeft != NULL ? pivotLeft->mHeight : 0) + 1;
    return NOERROR;
}

AutoPtr<CTreeMap::SimpleImmutableEntry> CTreeMap::ImmutableCopy(
    /* [in] */ IMapEntry* entry)
{
    AutoPtr<SimpleImmutableEntry> res;
    return entry == NULL ? NULL : (res = new SimpleImmutableEntry(entry));
}

ECode CTreeMap::FirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = mRoot == NULL ? NULL : mRoot->First();
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

AutoPtr<IMapEntry> CTreeMap::InternalPollFirstEntry()
{
    if (mRoot == NULL) {
        return NULL;
    }
    AutoPtr<Node> result = mRoot->First();
    RemoveInternal(result);
    return result;
}

ECode CTreeMap::PollFirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<IMapEntry> res = InternalPollFirstEntry();
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy(res);
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    if (mRoot == NULL) {
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    AutoPtr<Node> res = mRoot->First();
    return res->GetKey(outface);
}

ECode CTreeMap::LastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = mRoot == NULL ? NULL : mRoot->Last();
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

AutoPtr<IMapEntry> CTreeMap::InternalPollLastEntry()
{
    if (mRoot == NULL) {
        return NULL;
    }
    AutoPtr<Node> result = mRoot->Last();
    RemoveInternal(result);
    return result;
}

ECode CTreeMap::PollLastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<IMapEntry> res = InternalPollLastEntry();
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy(res);
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    if (mRoot == NULL) {
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    AutoPtr<Node> res = mRoot->Last();
    return res->GetKey(outface);
}

ECode CTreeMap::LowerEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = Find(key, LOWER);
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::LowerKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = Find(key, LOWER);
    AutoPtr<IInterface> resint;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&resint), resint) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::FloorEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = Find(key, FLOOR);
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::FloorKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = Find(key, FLOOR);
    AutoPtr<IInterface> keyface;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&keyface), keyface) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::CeilingEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = Find(key, CEILING);
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::CeilingKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = Find(key, CEILING);
    AutoPtr<IInterface> keyface;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&keyface), keyface) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::HigherEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = Find(key, HIGHER);
    AutoPtr<SimpleImmutableEntry> outsim = ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)outsim.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::HigherKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = Find(key, HIGHER);
    AutoPtr<IInterface> keyface;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&keyface), keyface) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp)

    *comp = mComparator != NATURAL_ORDER ? mComparator : NULL;
    REFCOUNT_ADD(*comp)
    return NOERROR;
}

ECode CTreeMap::GetEntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)

    if (mEntrySet == NULL) {
        mEntrySet = new _EntrySet(this);
    }
    *entries = mEntrySet.Get();
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CTreeMap::GetKeySet(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    if (mKeySet == NULL) {
        mKeySet = new _KeySet(this);
    }
    *keySet = mKeySet.Get();
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode CTreeMap::NavigableKeySet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<_KeySet> result = mKeySet;
    *outnav = result != NULL ? result : (mKeySet = new _KeySet(this));
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::SubMap(
    /* [in] */ IInterface* fromKey,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toKey,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    Bound fromBound = fromInclusive ? INCLUSIVE : EXCLUSIVE;
    Bound toBound = toInclusive ? INCLUSIVE : EXCLUSIVE;
    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, fromKey, fromBound, toKey, toBound, this);
    *outnav = (INavigableMap*)res.Get();
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::SubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)

    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, startKey, INCLUSIVE, endKey, EXCLUSIVE, this);
    AutoPtr<ISortedMap> outsort = (ISortedMap*)res.Get();
    *sortmap = outsort;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CTreeMap::HeadMap(
    /* [in] */ IInterface* toKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    Bound toBound = inclusive ? INCLUSIVE : EXCLUSIVE;
    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, NULL, NO_BOUND, toKey, toBound, this);
    *outnav = (INavigableMap*)res.Get();
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::HeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)

    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, NULL, NO_BOUND, endKey, EXCLUSIVE, this);
    AutoPtr<ISortedMap> outsort = (ISortedMap*)res.Get();
    *sortmap = outsort;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CTreeMap::TailMap(
    /* [in] */ IInterface* fromKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    Bound fromBound = inclusive ? INCLUSIVE : EXCLUSIVE;
    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, fromKey, fromBound, NULL, NO_BOUND, this);
    *outnav = (INavigableMap*)res.Get();
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::TailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)

    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, startKey, INCLUSIVE, NULL, NO_BOUND, this);
    AutoPtr<ISortedMap> outsort = (ISortedMap*)res.Get();
    *sortmap = outsort;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CTreeMap::DescendingMap(
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<BoundedMap> res = new BoundedMap(FALSE, NULL, NO_BOUND, NULL, NO_BOUND, this);
    *outnav = (INavigableMap*)res.Get();
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::DescendingKeySet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<BoundedMap> res = new BoundedMap(FALSE, NULL, NO_BOUND, NULL, NO_BOUND, this);
    return res->NavigableKeySet(outnav);
}

Int32 CTreeMap::Count(
    /* [in] */ IIterator* iterator)
{
    Int32 count = 0;
    Boolean isflag = FALSE;
    while (iterator->HasNext(&isflag), isflag) {
        AutoPtr<IInterface> nextface;
        iterator->GetNext((IInterface**)&nextface);
        count++;
    }
    return count;
}

ECode CTreeMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    AutoPtr<IObjectOutputStreamPutField> strfield;
    stream->PutFields((IObjectOutputStreamPutField**)&strfield);
    AutoPtr<IComparator> rescom;
    Comparator((IComparator**)&rescom);
    strfield->PutObject(String("comparator"), rescom);
    stream->WriteFields();
    stream->Write(mSize);

    AutoPtr<ArrayOf<IInterface*> > entries;
    AutoPtr<ISet> outset;
    GetEntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&entries);
    for (Int32 i = 0; i < entries->GetLength(); i++) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe((*entries)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        entry->GetKey((IInterface**)&keyface);
        entry->GetValue((IInterface**)&valueface);
        assert(0 && "TODO");
        // stream->WriteObject(keyface);
        // stream->WriteObject(valueface);
    }

    return NOERROR;
}

ECode CTreeMap::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    AutoPtr<IObjectInputStreamGetField> fields;
    stream->ReadFields((IObjectInputStreamGetField**)&fields);
    fields->GetObject(String("comparator"), NULL, (IInterface**)&mComparator);
    if (mComparator == NULL) {
        mComparator = NATURAL_ORDER;
    }
    Int32 size = 0;
    stream->Read(&size);
    for (Int32 i = 0; i < size; i++) {
        assert(0 && "TODO");
        // PutInternal((K) stream.readObject(), (V) stream.readObject());
    }
    return NOERROR;
}

ECode CTreeMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return AbstractMap::ContainsValue(value, result);
}

ECode CTreeMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CTreeMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CTreeMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CTreeMap::GetValues(
    /* [out] */ ICollection** value)
{
    return AbstractMap::GetValues(value);
}

//==========================================================
//       CTreeMap::Node
//==========================================================
CAR_INTERFACE_IMPL(CTreeMap::Node, IMapEntry);

CTreeMap::Node::Node(
    /* [in] */ Node* parent,
    /* [in] */ IInterface* key)
{
    mParent = parent;
    mKey = key;
    mHeight = 1;
}

AutoPtr<CTreeMap::Node> CTreeMap::Node::Copy(
    /* [in] */ Node* parent)
{
    AutoPtr<Node> result = new Node(parent, mKey);
    if (mLeft != NULL) {
        result->mLeft = mLeft->Copy(result);
    }
    if (mRight != NULL) {
        result->mRight = mRight->Copy(result);
    }
    result->mValue = mValue;
    result->mHeight = mHeight;
    return result;
}

ECode CTreeMap::Node::GetKey(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key)

    *key = mKey;
    REFCOUNT_ADD(*key)
    return NOERROR;
}

ECode CTreeMap::Node::GetValue(
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mValue;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CTreeMap::Node::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    VALIDATE_NOT_NULL(valueReplacee)

    AutoPtr<IInterface> oldValue = mValue;
    mValue = valueReplacer;
    *valueReplacee = oldValue;
    REFCOUNT_ADD(*valueReplacee)
    return NOERROR;
}

ECode CTreeMap::Node::Equals(
    /* [in] */ IInterface* entry,
    /* [out] */ Boolean* result)
{
    if (IMapEntry::Probe(entry)) {
        AutoPtr<IMapEntry> other = (IMapEntry*) entry->Probe(EIID_IMapEntry);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        other->GetKey((IInterface**)&keyface);
        other->GetValue((IInterface**)&valueface);
        *result = (mKey == NULL ? keyface == NULL : mKey == keyface)
                && (mValue == NULL ? valueface == NULL : mValue == valueface);
    }
    *result = FALSE;
    return NOERROR;
}

ECode CTreeMap::Node::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    *hashCode = (mKey == NULL ? 0 : Object::GetHashCode(mKey))
            ^ (mValue == NULL ? 0 : Object::GetHashCode(mValue));
    return NOERROR;
}

ECode CTreeMap::Node::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = Object::ToString(mKey) + String("=") + Object::ToString(mValue);
    return NOERROR;
}

AutoPtr<CTreeMap::Node> CTreeMap::Node::Next()
{
    if (mRight != NULL) {
        return mRight->First();
    }

    AutoPtr<Node> node = this;
    AutoPtr<Node> parent = node->mParent;
    while (parent != NULL) {
        if (parent->mLeft == node) {
            return parent;
        }
        node = parent;
        parent = node->mParent;
    }
    return NULL;
}

AutoPtr<CTreeMap::Node> CTreeMap::Node::Prev()
{
    if (mLeft != NULL) {
        return mLeft->Last();
    }

    AutoPtr<Node> node = this;
    AutoPtr<Node> parent = node->mParent;
    while (parent != NULL) {
        if (parent->mRight == node) {
            return parent;
        }
        node = parent;
        parent = node->mParent;
    }
    return NULL;
}

AutoPtr<CTreeMap::Node> CTreeMap::Node::First()
{
    AutoPtr<Node> node = this;
    AutoPtr<Node> child = node->mLeft;
    while (child != NULL) {
        node = child;
        child = node->mLeft;
    }
    return node;
}

AutoPtr<CTreeMap::Node> CTreeMap::Node::Last()
{
    AutoPtr<Node> node = this;
    AutoPtr<Node> child = node->mRight;
    while (child != NULL) {
        node = child;
        child = node->mRight;
    }
    return node;
}


//==========================================================
//       CTreeMap::MapIterator
//==========================================================
CAR_INTERFACE_IMPL(CTreeMap::MapIterator, IIterator)

CTreeMap::MapIterator::MapIterator(
    /* [in] */ Node* next,
    /* [in] */ CTreeMap* host)
{
    mNext = next;
    mHost = host;
    mExpectedModCount = mHost->mModCount;
}

ECode CTreeMap::MapIterator::HasNext(
    /* [out] */ Boolean* result)
{
    *result = mNext != NULL;
    return NOERROR;
}

AutoPtr<CTreeMap::Node> CTreeMap::MapIterator::StepForward()
{
    if (mNext == NULL) {
        // throw new NoSuchElementException();
        return NULL;
    }
    if (mHost->mModCount != mExpectedModCount) {
        // throw new ConcurrentModificationException();
        return NULL;
    }
    mLast = mNext;
    mNext = mNext->GetNext();
    return mLast;
}

AutoPtr<CTreeMap::Node> CTreeMap::MapIterator::StepBackward()
{
    if (mNext == NULL) {
        // throw new NoSuchElementException();
        return NULL;
    }
    if (mHost->mModCount != mExpectedModCount) {
        // throw new ConcurrentModificationException();
        return NULL;
    }
    mLast = mNext;
    mNext = mNext->Prev();
    return mLast;
}

ECode CTreeMap::MapIterator::Remove()
{
    if (mLast == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mHost->RemoveInternal(mLast);
    mExpectedModCount = mHost->mModCount;
    mLast = NULL;
    return NOERROR;
}


//==========================================================
//       CTreeMap::_EntrySet
//==========================================================
UInt32 CTreeMap::_EntrySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::_EntrySet::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::_EntrySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode CTreeMap::_EntrySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CTreeMap::_EntrySet::_EntrySet(
    /* [in] */ CTreeMap* host)
{
    mHost = host;
}

ECode CTreeMap::_EntrySet::GetSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mHost->mSize;
    return NOERROR;
}

ECode CTreeMap::_EntrySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterator> res = (IIterator*) new _EntrySetForwardMapIterator(mHost->mRoot == NULL ? NULL : mHost->mRoot->First(), mHost);
    *outiter = res;
    REFCOUNT_ADD(*outiter)
    return NOERROR;
}

ECode CTreeMap::_EntrySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IMapEntry> res = IMapEntry::Probe(o);
    AutoPtr<Node> outnode;
    *value = res != NULL && (outnode = mHost->FindByEntry(res), outnode != NULL);
    return NOERROR;
}

ECode CTreeMap::_EntrySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(IMapEntry::Probe(o))) {
        *value = FALSE;
        return NOERROR;
    }

    AutoPtr<Node> node = mHost->FindByEntry(IMapEntry::Probe(o));
    if (node == NULL) {
        *value = FALSE;
        return NOERROR;
    }
    mHost->RemoveInternal(node);
    *value = TRUE;
    return NOERROR;
}

ECode CTreeMap::_EntrySet::Clear()
{
    return mHost->Clear();
}

ECode CTreeMap::_EntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CTreeMap::_EntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CTreeMap::_EntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CTreeMap::_EntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CTreeMap::_EntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CTreeMap::_EntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CTreeMap::_EntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CTreeMap::_EntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CTreeMap::_EntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CTreeMap::_EntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

//==========================================================
//       CTreeMap::_KeySet
//==========================================================

UInt32 CTreeMap::_KeySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::_KeySet::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::_KeySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(INavigableSet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(INavigableSet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(INavigableSet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)(INavigableSet*)this;
    }
    else if (EIID_ISortedSet == riid) {
        return (ISortedSet*)(INavigableSet*)this;
    }
    else if (EIID_INavigableSet == riid) {
        return (INavigableSet*)this;
    }

    return NULL;
}

ECode CTreeMap::_KeySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(INavigableSet*)this) {
        *pIID = EIID_INavigableSet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CTreeMap::_KeySet::_KeySet(
    /* [in] */ CTreeMap* host)
{
    mHost = host;
}

ECode CTreeMap::_KeySet::GetSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mHost->mSize;
    return NOERROR;
}

ECode CTreeMap::_KeySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterator> res = (IIterator*) new _KeySetForwardMapIterator(mHost->mRoot == NULL ? NULL : mHost->mRoot->First(), mHost);
    *outiter = res;
    REFCOUNT_ADD(*outiter)
    return NOERROR;
}

ECode CTreeMap::_KeySet::DescendingIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterator> res = (IIterator*) new _KeySetBackwardMapIterator(mHost->mRoot == NULL ? NULL : mHost->mRoot->First(), mHost);
    *outiter = res;
    REFCOUNT_ADD(*outiter)
    return NOERROR;
}

ECode CTreeMap::_KeySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    return mHost->ContainsKey(o, value);
}

ECode CTreeMap::_KeySet::Remove(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<Node> res = mHost->RemoveInternalByKey(key);
    *value = res != NULL;
    return NOERROR;
}

ECode CTreeMap::_KeySet::Clear()
{
    return mHost->Clear();
}

ECode CTreeMap::_KeySet::Comparator(
    /* [out] */ IComparator** outcom)
{
    return mHost->Comparator(outcom);
}

ECode CTreeMap::_KeySet::First(
    /* [out] */ IInterface** outface)
{
    return mHost->FirstKey(outface);
}

ECode CTreeMap::_KeySet::Last(
    /* [out] */ IInterface** outface)
{
    return mHost->LastKey(outface);
}

ECode CTreeMap::_KeySet::Lower(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->LowerKey(e, outface);
}

ECode CTreeMap::_KeySet::Floor(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->FloorKey(e, outface);
}

ECode CTreeMap::_KeySet::Ceiling(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->CeilingKey(e, outface);
}

ECode CTreeMap::_KeySet::Higher(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->HigherKey(e, outface);
}

ECode CTreeMap::_KeySet::PollFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = mHost->InternalPollFirstEntry();
    AutoPtr<IInterface> res;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::_KeySet::PollLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = mHost->InternalPollLastEntry();
    AutoPtr<IInterface> res;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::_KeySet::SubSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<INavigableMap> res;
    mHost->SubMap(fromElement, fromInclusive, toElement, toInclusive, (INavigableMap**)&res);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::_KeySet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    AutoPtr<INavigableMap> res;
    mHost->SubMap(start, TRUE, end, FALSE, (INavigableMap**)&res);
    return res->NavigableKeySet((INavigableSet**)outsort);
}

ECode CTreeMap::_KeySet::HeadSet(
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<INavigableMap> res;
    mHost->HeadMap(toElement, inclusive, (INavigableMap**)&res);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::_KeySet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    AutoPtr<INavigableMap> res;
    mHost->HeadMap(end, FALSE, (INavigableMap**)&res);
    return res->NavigableKeySet((INavigableSet**)outsort);
}

ECode CTreeMap::_KeySet::TailSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<INavigableMap> res;
    mHost->TailMap(fromElement, inclusive, (INavigableMap**)&res);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::_KeySet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    AutoPtr<INavigableMap> res;
    mHost->TailMap(start, TRUE, (INavigableMap**)&res);
    return res->NavigableKeySet((INavigableSet**)outsort);
}

ECode CTreeMap::_KeySet::DescendingSet(
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<BoundedMap> res = new BoundedMap(FALSE, NULL, NO_BOUND, NULL, NO_BOUND, mHost);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::_KeySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CTreeMap::_KeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CTreeMap::_KeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CTreeMap::_KeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CTreeMap::_KeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CTreeMap::_KeySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CTreeMap::_KeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CTreeMap::_KeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CTreeMap::_KeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CTreeMap::_KeySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}


//==========================================================
//       CTreeMap::BoundedMap::BoundedIterator
//==========================================================
CTreeMap::BoundedMap::BoundedIterator::BoundedIterator(
    /* [in] */ Node* next,
    /* [in] */ BoundedMap* host)
    : MapIterator(next, host->mHost)
{
    mBoundParent = host;
}

AutoPtr<CTreeMap::Node> CTreeMap::BoundedMap::BoundedIterator::StepForward()
{
    AutoPtr<Node> result = MapIterator::StepForward();
    if (mNext != NULL && !mBoundParent->IsInBounds(mNext->mKey, NO_BOUND, mBoundParent->mToBound)) {
        mNext = NULL;
    }
    return result;
}

AutoPtr<CTreeMap::Node> CTreeMap::BoundedMap::BoundedIterator::StepBackward()
{
    AutoPtr<Node> result = MapIterator::StepBackward();
    if (mNext != NULL && ! mBoundParent->IsInBounds(mNext->mKey, mBoundParent->mFromBound, NO_BOUND)) {
        mNext = NULL;
    }
    return result;
}

//==========================================================
//       CTreeMap::BoundedMap::BoundedEntrySet::BoundedEntrySetIterator
//==========================================================

CTreeMap::BoundedMap::BoundedEntrySet::BoundedEntrySetIterator::BoundedEntrySetIterator(
    /* [in] */ Node* next,
    /* [in] */ BoundedMap* host)
    : BoundedIterator(next, host)
{
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::BoundedEntrySetIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = mBoundParent->mAscending ? StepForward() : StepBackward();
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//==========================================================
//       CTreeMap::BoundedMap::BoundedEntrySet
//==========================================================

UInt32 CTreeMap::BoundedMap::BoundedEntrySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::BoundedMap::BoundedEntrySet::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::BoundedMap::BoundedEntrySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CTreeMap::BoundedMap::BoundedEntrySet::BoundedEntrySet(
    /* [in] */ BoundedMap* host)
{
    mHost = host;
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::GetSize(
    /* [out] */ Int32* value)
{
    return mHost->GetSize(value);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::IsEmpty(
    /* [out] */ Boolean* value)
{
    return mHost->IsEmpty(value);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    *outiter = new BoundedEntrySetIterator(mHost->Endpoint(FALSE), mHost);
    REFCOUNT_ADD(*outiter);
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(IMapEntry::Probe(o))) {
        *value = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> entry = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    entry->GetKey((IInterface**)&keyface);

    *value = mHost->IsInBounds(keyface) && mHost->mHost->FindByEntry(entry) != NULL;
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(IMapEntry::Probe(o))) {
        *value = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> entry = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    entry->GetKey((IInterface**)&keyface);
    AutoPtr<ISet> entries;
    mHost->mHost->GetEntrySet((ISet**)&entries);
    Boolean isflag = FALSE;
    *value = mHost->IsInBounds(keyface) && (entries->Remove(entry, &isflag), isflag);
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::Clear()
{
    return AbstractSet::Clear();
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RemoveAll(collection, result);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RetainAll(collection, result);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(contents, outArray);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CTreeMap::BoundedMap::BoundedEntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

//==========================================================
//       CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySetIterator
//==========================================================

CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySetIterator::BoundedKeySetIterator(
    /* [in] */ Node* next,
    /* [in] */ BoundedMap* host)
    : BoundedIterator(next, host)
{
}

ECode CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySetIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = (mBoundParent->mAscending ? StepForward() : StepBackward())->mKey.Get();
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//==========================================================
//       CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySetDescendingIterator
//==========================================================

CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySetDescendingIterator::BoundedKeySetDescendingIterator(
    /* [in] */ Node* next,
    /* [in] */ BoundedMap* host)
    : BoundedIterator(next, host)
{
}

ECode CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySetDescendingIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = (mBoundParent->mAscending ? StepBackward() : StepForward())->mKey.Get();
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//==========================================================
//       CTreeMap::BoundedMap::BoundedKeySet
//==========================================================

UInt32 CTreeMap::BoundedMap::BoundedKeySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::BoundedMap::BoundedKeySet::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::BoundedMap::BoundedKeySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(INavigableSet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(INavigableSet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(INavigableSet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)(INavigableSet*)this;
    }
    else if (EIID_ISortedSet == riid) {
        return (ISortedSet*)(INavigableSet*)this;
    }
    else if (EIID_INavigableSet == riid) {
        return (INavigableSet*)this;
    }

    return NULL;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(INavigableSet*)this) {
        *pIID = EIID_INavigableSet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CTreeMap::BoundedMap::BoundedKeySet::BoundedKeySet(
    /* [in] */ BoundedMap* host)
{
    mHost = host;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::GetSize(
    /* [out] */ Int32* value)
{
    return mHost->GetSize(value);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::IsEmpty(
    /* [out] */ Boolean* value)
{
    return mHost->IsEmpty(value);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    *outiter = new BoundedKeySetIterator(mHost->Endpoint(TRUE), mHost);
    REFCOUNT_ADD(*outiter);;
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::DescendingIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    *outiter = new BoundedKeySetIterator(mHost->Endpoint(FALSE), mHost);
    REFCOUNT_ADD(*outiter);;
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mHost->IsInBounds(o) && mHost->mHost->FindByObject(o) != NULL;
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mHost->IsInBounds(o) && mHost->mHost->RemoveInternalByKey(o) != NULL;
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::First(
    /* [out] */ IInterface** outface)
{
    return mHost->FirstKey(outface);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::PollFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry;
    mHost->PollFirstEntry((IMapEntry**)&entry);
    AutoPtr<IInterface> keyface;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&keyface), keyface) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Last(
    /* [out] */ IInterface** outface)
{
    return mHost->LastKey(outface);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::PollLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry;
    mHost->PollLastEntry((IMapEntry**)&entry);
    AutoPtr<IInterface> keyface;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&keyface), keyface) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Lower(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->LowerKey(e, outface);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Floor(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->FloorKey(e, outface);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Ceiling(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->CeilingKey(e, outface);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Higher(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mHost->HigherKey(e, outface);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Comparator(
    /* [out] */ IComparator** outcom)
{
    return mHost->Comparator(outcom);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::SubSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<INavigableMap> res;
    mHost->SubMap(fromElement, fromInclusive, toElement, toInclusive, (INavigableMap**)&res);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    AutoPtr<ISortedMap> res;
    mHost->SubMap(start, end, (ISortedMap**)&res);
    return INavigableMap::Probe(res)->NavigableKeySet((INavigableSet**)outsort);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::HeadSet(
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<INavigableMap> res;
    mHost->HeadMap(toElement, inclusive, (INavigableMap**)&res);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    AutoPtr<ISortedMap> res;
    mHost->HeadMap(end, (ISortedMap**)&res);
    return INavigableMap::Probe(res)->NavigableKeySet((INavigableSet**)outsort);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::TailSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<INavigableMap> res;
    mHost->TailMap(fromElement, inclusive, (INavigableMap**)&res);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    AutoPtr<ISortedMap> res;
    mHost->TailMap(start, (ISortedMap**)&res);
    return INavigableMap::Probe(res)->NavigableKeySet((INavigableSet**)outsort);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::DescendingSet(
    /* [out] */ INavigableSet** outnav)
{
    AutoPtr<BoundedMap> res = new BoundedMap(
        !mHost->mAscending, mHost->mFrom, mHost->mFromBound,
        mHost->mTo, mHost->mToBound, mHost->mHost);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Clear()
{
    return AbstractSet::Clear();
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RemoveAll(collection, result);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RetainAll(collection, result);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(contents, outArray);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CTreeMap::BoundedMap::BoundedKeySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}


//==========================================================
//       CTreeMap::BoundedMap
//==========================================================

UInt32 CTreeMap::BoundedMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::BoundedMap::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::BoundedMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(INavigableMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(INavigableMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(INavigableMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)(INavigableMap*)this;
    }
    else if (EIID_ISortedMap == riid) {
        return (ISortedMap*)(INavigableMap*)this;
    }
    else if (EIID_INavigableMap == riid) {
        return (INavigableMap*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CTreeMap::BoundedMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(INavigableMap*)this) {
        *pIID = EIID_INavigableMap;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CTreeMap::BoundedMap::BoundedMap(
    /* [in] */ Boolean ascending,
    /* [in] */ IInterface* from,
    /* [in] */ Bound fromBound,
    /* [in] */ IInterface* to,
    /* [in] */ Bound toBound,
    /* [in] */ CTreeMap* host)
{
    /*
     * Validate the bounds. In addition to checking that from <= to, we
     * verify that the comparator supports our bound objects.
     */
    mHost = host;
    Int32 comvalue = 0;
    if (fromBound != NO_BOUND && toBound != NO_BOUND) {
        if ((mHost->mComparator->Compare(from, to, &comvalue), comvalue) > 0) {
            assert(0 && "TODO");
            // throw new IllegalArgumentException(from + " > " + to);
        }
    }
    else if (fromBound != NO_BOUND) {
        mHost->mComparator->Compare(from, from, &comvalue);
    }
    else if (toBound != NO_BOUND) {
        mHost->mComparator->Compare(to, to, &comvalue);
    }
    mAscending = ascending;
    mFrom = from;
    mFromBound = fromBound;
    mTo = to;
    mToBound = toBound;
}

ECode CTreeMap::BoundedMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    AutoPtr<ISet> outset;
    GetEntrySet((ISet**)&outset);
    AutoPtr<IIterator> it;
    outset->GetIterator((IIterator**)&it);
    *size = Count(it);
    return NOERROR;
}

ECode CTreeMap::BoundedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = Endpoint(TRUE) == NULL;
    return NOERROR;
}

ECode CTreeMap::BoundedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IInterface> keyface;
    *value = IsInBounds(key) ? (mHost->Get(key, (IInterface**)&keyface), keyface) : NULL;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Boolean isflag = FALSE;
    *result = IsInBounds(key) && (mHost->ContainsKey(key, &isflag), isflag);
    return NOERROR;
}

ECode CTreeMap::BoundedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    if (!IsInBounds(key)) {
        if (oldValue) {
            *oldValue = NULL;
        }
        return OutOfBounds(key, mFromBound, mToBound);
    }

    if (oldValue) {
        AutoPtr<IInterface> obj = mHost->PutInternal(key, value);
        *oldValue = obj;
        REFCOUNT_ADD(*oldValue)
    }
    else {
        mHost->PutInternal(key, value);
    }
    return NOERROR;
}

ECode CTreeMap::BoundedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IInterface> res;
    *value = IsInBounds(key) ? (mHost->Remove(key, (IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

Boolean CTreeMap::BoundedMap::IsInBounds(
    /* [in] */ IInterface* key)
{
    return IsInBounds(key, mFromBound, mToBound);
}

Boolean CTreeMap::BoundedMap::IsInBounds(
    /* [in] */ IInterface* key,
    /* [in] */ Bound fromBound,
    /* [in] */ Bound toBound)
{
    Int32 comvalue = 0;
    if (fromBound == INCLUSIVE) {
        if ((mHost->mComparator->Compare(key, mFrom, &comvalue), comvalue) < 0) {
            return FALSE; // less than from
        }
    }
    else if (fromBound == EXCLUSIVE) {
        if ((mHost->mComparator->Compare(key, mFrom, &comvalue), comvalue) <= 0) {
            return FALSE; // less than or equal to from
        }
    }

    if (toBound == INCLUSIVE) {
        if ((mHost->mComparator->Compare(key, mTo, &comvalue), comvalue) > 0) {
            return FALSE; // greater than 'to'
        }
    }
    else if (toBound == EXCLUSIVE) {
        if ((mHost->mComparator->Compare(key, mTo, &comvalue), comvalue) >= 0) {
            return FALSE; // greater than or equal to 'to'
        }
    }

    return TRUE;
}

AutoPtr<CTreeMap::Node> CTreeMap::BoundedMap::GetBound(
    /* [in] */ Node* node,
    /* [in] */ Bound fromBound,
    /* [in] */ Bound toBound)
{
    AutoPtr<IInterface> keyface;
    AutoPtr<Node> res = node != NULL && IsInBounds((node->GetKey((IInterface**)&keyface), keyface), fromBound, toBound) ? node : NULL;
    return res;
}

ECode CTreeMap::BoundedMap::FirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = Endpoint(TRUE);
    AutoPtr<SimpleImmutableEntry> res2 = mHost->ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)res2.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::PollFirstEntry(
    /* [out] */ IMapEntry** outent)
{
    AutoPtr<Node> result = Endpoint(TRUE);
    if (result != NULL) {
        mHost->RemoveInternal(result);
    }
    AutoPtr<SimpleImmutableEntry> res = mHost->ImmutableCopy(result);
    *outent = (IMapEntry*)res.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = Endpoint(TRUE);
    if (entry == NULL) {
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    return entry->GetKey(outface);
}

ECode CTreeMap::BoundedMap::LastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> res = Endpoint(FALSE);
    AutoPtr<SimpleImmutableEntry> res2 = mHost->ImmutableCopy((IMapEntry*)res.Get());
    *outent = (IMapEntry*)res2.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::PollLastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<Node> result = Endpoint(FALSE);
    if (result != NULL) {
        mHost->RemoveInternal(result);
    }
    AutoPtr<SimpleImmutableEntry> res = mHost->ImmutableCopy(result);
    *outent = (IMapEntry*)res.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = Endpoint(FALSE);
    if (entry == NULL) {
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    return entry->GetKey(outface);
}

AutoPtr<CTreeMap::Node> CTreeMap::BoundedMap::Endpoint(
    /* [in] */ Boolean first)
{
    AutoPtr<Node> node;
    if (mAscending == first) {
        switch (mFromBound) {
            case NO_BOUND:
                node = mHost->mRoot == NULL ? NULL : mHost->mRoot->First();
                break;
            case INCLUSIVE:
                node = mHost->Find(mFrom, CEILING);
                break;
            case EXCLUSIVE:
                node = mHost->Find(mFrom, HIGHER);
                break;
            default:
                // throw new AssertionError();
                return NULL;
        }
        return GetBound(node, NO_BOUND, mToBound);
    }
    else {
        switch (mToBound) {
            case NO_BOUND:
                node = mHost->mRoot == NULL ? NULL : mHost->mRoot->Last();
                break;
            case INCLUSIVE:
                node = mHost->Find(mTo, FLOOR);
                break;
            case EXCLUSIVE:
                node = mHost->Find(mTo, LOWER);
                break;
            default:
                // throw new AssertionError();
                return NULL;
        }
        return GetBound(node, mFromBound, NO_BOUND);
    }
}

AutoPtr<IMapEntry> CTreeMap::BoundedMap::FindBounded(
    /* [in] */ IInterface* key,
    /* [in] */ Relation relation)
{
    relation = mHost->ForOrder(mAscending, relation);
    Bound fromBoundForCheck = mFromBound;
    Bound toBoundForCheck = mToBound;

    if (mToBound != NO_BOUND && (relation == LOWER || relation == FLOOR)) {
        Int32 comparison = 0;
        mHost->mComparator->Compare(mTo, key, &comparison);
        if (comparison <= 0) {
            key = mTo;
            if (mToBound == EXCLUSIVE) {
                relation = LOWER; // 'to' is too high
            }
            else if (comparison < 0) {
                relation = FLOOR; // we already went lower
            }
        }
        toBoundForCheck = NO_BOUND; // we've already checked the upper bound
    }

    if (mFromBound != NO_BOUND && (relation == CEILING || relation == HIGHER)) {
        Int32 comparison = 0;
        mHost->mComparator->Compare(mFrom, key, &comparison);
        if (comparison >= 0) {
            key = mFrom;
            if (mFromBound == EXCLUSIVE) {
                relation = HIGHER; // 'from' is too low
            }
            else if (comparison > 0) {
                relation = CEILING; // we already went higher
            }
        }
        fromBoundForCheck = NO_BOUND; // we've already checked the lower bound
    }

    AutoPtr<Node> nodfind = mHost->Find(key, relation);
    return GetBound(nodfind, fromBoundForCheck, toBoundForCheck);
}

ECode CTreeMap::BoundedMap::LowerEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<IMapEntry> findentry = FindBounded(key, LOWER);
    AutoPtr<SimpleImmutableEntry> res = mHost->ImmutableCopy(findentry);
    *outent = (IMapEntry*)res.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::LowerKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = FindBounded(key, LOWER);
    AutoPtr<IInterface> res;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::FloorEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<IMapEntry> res = FindBounded(key, FLOOR);
    AutoPtr<SimpleImmutableEntry> res2 = mHost->ImmutableCopy(res);
    *outent = (IMapEntry*)res.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::FloorKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = FindBounded(key, FLOOR);
    AutoPtr<IInterface> res;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::CeilingEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<IMapEntry> res = FindBounded(key, CEILING);
    AutoPtr<SimpleImmutableEntry> res2 = mHost->ImmutableCopy(res);
    *outent = (IMapEntry*)res.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::CeilingKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = FindBounded(key, CEILING);
    AutoPtr<IInterface> res;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::HigherEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent)

    AutoPtr<IMapEntry> entry = FindBounded(key, HIGHER);
    AutoPtr<SimpleImmutableEntry> res = mHost->ImmutableCopy(entry);
    *outent = (IMapEntry*)res.Get();
    REFCOUNT_ADD(*outent)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::HigherKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry = FindBounded(key, HIGHER);
    AutoPtr<IInterface> res;
    *outface = entry != NULL ? (entry->GetKey((IInterface**)&res), res) : NULL;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp)

    AutoPtr<IComparator> forward;
    mHost->Comparator((IComparator**)&forward);
    if (mAscending) {
        *comp = forward;
        REFCOUNT_ADD(*comp)
        return NOERROR;
    }
    else {
        return CCollections::_ReverseOrder(forward, comp);
    }
}

ECode CTreeMap::BoundedMap::GetEntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)

    AutoPtr<BoundedEntrySet> result = mEntrySet;
    *entries = result != NULL ? result : (mEntrySet = new BoundedEntrySet(this));
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::GetKeySet(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    AutoPtr<INavigableSet> outnav;
    FAIL_RETURN(NavigableKeySet((INavigableSet**)&outnav));
    AutoPtr<ISet> outset = ISet::Probe(outnav);
    *keySet = outset;
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::NavigableKeySet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<BoundedKeySet> result = mKeySet;
    *outnav = result != NULL ? result : (mKeySet = new BoundedKeySet(this));
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::DescendingMap(
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<BoundedMap> res = new BoundedMap(!mAscending, mFrom, mFromBound, mTo, mToBound, mHost);
    *outnav = (INavigableMap*)res.Get();
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::DescendingKeySet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<BoundedMap> res = new BoundedMap(!mAscending, mFrom, mFromBound, mTo, mToBound, mHost);
    return res->NavigableKeySet(outnav);
}

ECode CTreeMap::BoundedMap::SubMap(
    /* [in] */ IInterface* fromKey,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toKey,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    Bound fromBound = fromInclusive ? INCLUSIVE : EXCLUSIVE;
    Bound toBound = toInclusive ? INCLUSIVE : EXCLUSIVE;
    AutoPtr<INavigableMap> res = SubMap(fromKey, fromBound, toKey, toBound);
    *outnav = res;
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::SubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)

    AutoPtr<INavigableMap> res = SubMap(startKey, INCLUSIVE, endKey, EXCLUSIVE);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap);
    return NOERROR;
}

ECode CTreeMap::BoundedMap::HeadMap(
    /* [in] */ IInterface* toKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    Bound toBound = inclusive ? INCLUSIVE : EXCLUSIVE;
    AutoPtr<INavigableMap> res = SubMap(NULL, NO_BOUND, toKey, toBound);
    *outnav = res;
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::HeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)

    AutoPtr<INavigableMap> res = SubMap(NULL, NO_BOUND, endKey, EXCLUSIVE);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::TailMap(
    /* [in] */ IInterface* fromKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    Bound fromBound = inclusive ? INCLUSIVE : EXCLUSIVE;
    AutoPtr<INavigableMap> res = SubMap(fromKey, fromBound, NULL, NO_BOUND);
    *outnav = res;
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeMap::BoundedMap::TailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)

    AutoPtr<INavigableMap> res = SubMap(startKey, INCLUSIVE, NULL, NO_BOUND);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

AutoPtr<INavigableMap> CTreeMap::BoundedMap::SubMap(
    /* [in] */ IInterface* from,
    /* [in] */ Bound fromBound,
    /* [in] */ IInterface* to,
    /* [in] */ Bound toBound)
{
    if (!mAscending) {
        AutoPtr<IInterface> fromTmp = from;
        Bound fromBoundTmp = fromBound;
        from = mTo;
        fromBound = toBound;
        mTo = fromTmp;
        toBound = fromBoundTmp;
    }

    /*
     * If both the current and requested bounds are exclusive, the isInBounds check must be
     * inclusive. For example, to create (C..F) from (A..F), the bound 'F' is in bounds.
     */

    if (fromBound == NO_BOUND) {
        from = mFrom;
        fromBound = mFromBound;
    }
    else {
        Bound fromBoundToCheck = fromBound == mFromBound ? INCLUSIVE : mFromBound;
        if (!IsInBounds(from, fromBoundToCheck, mToBound)) {
            // throw outOfBounds(to, fromBoundToCheck, this.toBound);
            return NULL;
        }
    }

    if (toBound == NO_BOUND) {
        to = mTo;
        toBound = mToBound;
    }
    else {
        Bound toBoundToCheck = toBound == mToBound ? INCLUSIVE : mToBound;
        if (!IsInBounds(to, mFromBound, toBoundToCheck)) {
            // throw outOfBounds(to, this.fromBound, toBoundToCheck);
            return NULL;
        }
    }

    AutoPtr<BoundedMap> res = new BoundedMap(mAscending, from, fromBound, to, toBound, mHost);
    return (INavigableMap*)res.Get();
}

ECode CTreeMap::BoundedMap::OutOfBounds(
    /* [in] */ IInterface* value,
    /* [in] */ Bound fromBound,
    /* [in] */ Bound toBound)
{
    // return new IllegalArgumentException(value + " not in range "
    //         + fromBound.leftCap(from) + ".." + toBound.rightCap(to));
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

AutoPtr<IInterface> CTreeMap::BoundedMap::WriteReplace()
{
    if (mAscending) {
        AutoPtr<AscendingSubMap> res = new AscendingSubMap(mHost, mFrom, mFromBound, mTo, mToBound);
        return res->Probe(EIID_IInterface);
    }
    else {
        AutoPtr<DescendingSubMap> res = new DescendingSubMap(mHost, mFrom, mFromBound, mTo, mToBound);
        return res->Probe(EIID_IInterface);
    }
}

ECode CTreeMap::BoundedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return AbstractMap::ContainsValue(value, result);
}

ECode CTreeMap::BoundedMap::Clear()
{
    return AbstractMap::Clear();
}

ECode CTreeMap::BoundedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CTreeMap::BoundedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CTreeMap::BoundedMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CTreeMap::BoundedMap::GetValues(
    /* [out] */ ICollection** value)
{
    return AbstractMap::GetValues(value);
}

//==========================================================
//       CTreeMap::NavigableSubMap
//==========================================================
UInt32 CTreeMap::NavigableSubMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::NavigableSubMap::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::NavigableSubMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }

    return NULL;
}

ECode CTreeMap::NavigableSubMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CTreeMap::NavigableSubMap::NavigableSubMap(
    /* [in] */ CTreeMap* delegate,
    /* [in] */ IInterface* from,
    /* [in] */ Bound fromBound,
    /* [in] */ IInterface* to,
    /* [in] */ Bound toBound,
    /* [in] */ Boolean isDescending)
{
    mM = delegate;
    mLo = from;
    mHi = to;
    mFromStart = fromBound == NO_BOUND;
    mToEnd = toBound == NO_BOUND;
    mLoInclusive = fromBound == INCLUSIVE;
    mHiInclusive = toBound == INCLUSIVE;
    mIsDescending = isDescending;
}

ECode CTreeMap::NavigableSubMap::GetEntrySet(
    /* [out] */ ISet** entries)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

AutoPtr<IInterface> CTreeMap::NavigableSubMap::ReadResolve()
{
    Bound fromBound = mFromStart ? NO_BOUND : (mLoInclusive ? INCLUSIVE : EXCLUSIVE);
    Bound toBound = mToEnd ? NO_BOUND : (mHiInclusive ? INCLUSIVE : EXCLUSIVE);
    Boolean ascending = !mIsDescending;
    AutoPtr<BoundedMap> res = new BoundedMap(ascending, mLo, fromBound, mHi, toBound, mM);
    return res->Probe(EIID_IInterface);
}

ECode CTreeMap::NavigableSubMap::Clear()
{
    return AbstractMap::Clear();
}

ECode CTreeMap::NavigableSubMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return AbstractMap::ContainsKey(key, result);
}

ECode CTreeMap::NavigableSubMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return AbstractMap::ContainsValue(value, result);
}

ECode CTreeMap::NavigableSubMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CTreeMap::NavigableSubMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Get(key, value);
}

ECode CTreeMap::NavigableSubMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CTreeMap::NavigableSubMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode CTreeMap::NavigableSubMap::GetKeySet(
    /* [out] */ ISet** keySet)
{
    return AbstractMap::GetKeySet(keySet);
}

ECode CTreeMap::NavigableSubMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return AbstractMap::Put(key, value, oldValue);
}

ECode CTreeMap::NavigableSubMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CTreeMap::NavigableSubMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Remove(key, value);
}

ECode CTreeMap::NavigableSubMap::GetSize(
    /* [out] */ Int32* size)
{
    return AbstractMap::GetSize(size);
}

ECode CTreeMap::NavigableSubMap::GetValues(
    /* [out] */ ICollection** value)
{
    return AbstractMap::GetValues(value);
}

//==========================================================
//       CTreeMap::DescendingSubMap
//==========================================================
CTreeMap::DescendingSubMap::DescendingSubMap(
    /* [in] */ CTreeMap* delegate,
    /* [in] */ IInterface* from,
    /* [in] */ Bound fromBound,
    /* [in] */ IInterface* to,
    /* [in] */ Bound toBound)
    : NavigableSubMap(delegate, from, fromBound, to, toBound, TRUE)
{
}


//==========================================================
//       CTreeMap::AscendingSubMap
//==========================================================
CTreeMap::AscendingSubMap::AscendingSubMap(
    /* [in] */ CTreeMap* delegate,
    /* [in] */ IInterface* from,
    /* [in] */ Bound fromBound,
    /* [in] */ IInterface* to,
    /* [in] */ Bound toBound)
    : NavigableSubMap(delegate, from, fromBound, to, toBound, FALSE)
{
}


//==========================================================
//       CTreeMap::SubMap
//==========================================================

UInt32 CTreeMap::SubMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CTreeMap::SubMap::Release()
{
    return ElRefBase::Release();
}

PInterface CTreeMap::SubMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }

    return NULL;
}

ECode CTreeMap::SubMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CTreeMap::SubMap::GetEntrySet(
    /* [out] */ ISet** entries)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

AutoPtr<IInterface> CTreeMap::SubMap::ReadResolve()
{
    Bound fromBound = mFromStart ? NO_BOUND : INCLUSIVE;
    Bound toBound = mToEnd ? NO_BOUND : EXCLUSIVE;
    AutoPtr<BoundedMap> res = new BoundedMap(TRUE, mFromKey, fromBound, mToKey, toBound, mHost);
    return res->Probe(EIID_IInterface);
}

ECode CTreeMap::SubMap::Clear()
{
    return AbstractMap::Clear();
}

ECode CTreeMap::SubMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return AbstractMap::ContainsKey(key, result);
}

ECode CTreeMap::SubMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return AbstractMap::ContainsValue(value, result);
}

ECode CTreeMap::SubMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CTreeMap::SubMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Get(key, value);
}

ECode CTreeMap::SubMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CTreeMap::SubMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode CTreeMap::SubMap::GetKeySet(
    /* [out] */ ISet** keySet)
{
    return AbstractMap::GetKeySet(keySet);
}

ECode CTreeMap::SubMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return AbstractMap::Put(key, value, oldValue);
}

ECode CTreeMap::SubMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CTreeMap::SubMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Remove(key, value);
}

ECode CTreeMap::SubMap::GetSize(
    /* [out] */ Int32* size)
{
    return AbstractMap::GetSize(size);
}

ECode CTreeMap::SubMap::GetValues(
    /* [out] */ ICollection** value)
{
    return AbstractMap::GetValues(value);
}

//==========================================================
//       CTreeMap::OrderComparator
//==========================================================
CAR_INTERFACE_IMPL(CTreeMap::OrderComparator, IComparator);

ECode CTreeMap::OrderComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IComparable> a = (IComparable*) lhs->Probe(EIID_IComparable);

    if (a == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return a->CompareTo(rhs, result);
}

} // namespace Utility
} // namespace Elastos
