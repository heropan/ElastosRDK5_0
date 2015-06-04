
#include "CConcurrentSkipListMap.h"
#include <Math.h>
#include "CRandom.h"
#include "CArrayList.h"
#include "CCollections.h"

using Elastos::Core::Math;
using Elastos::Core::CRandom;
using Elastos::Core::EIID_IComparable;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CConcurrentSkipListMap::
//====================================================================

//static long serialVersionUID = -8627078645895051609L;

AutoPtr<IRandom> CConcurrentSkipListMap::sSeedGenerator;

AutoPtr<IInterface> CConcurrentSkipListMap::sBASE_HEADER;// = new CObject();

ECode CConcurrentSkipListMap::Initialize()
{
    CRandom::New((IRandom**)&sSeedGenerator);
    mKeySet = NULL;
    mEntrySet = NULL;
    mValues = NULL;
    mDescendingMap = NULL;
    Int32 num = 0;
    sSeedGenerator->NextInt32(&num);
    mRandomSeed = num | 0x0100; // ensure nonzero
    mHead = new HeadIndex(new Node(NULL, sBASE_HEADER, NULL),
                        NULL, NULL, 1);
    return NOERROR;
}

Boolean CConcurrentSkipListMap::CasHead(
    /* [in] */ HeadIndex* cmp,
    /* [in] */ HeadIndex* val)
{
    assert(0 && "TODO");
//    return UNSAFE.compareAndSwapObject(this, headOffset, cmp, val);
    return FALSE;
}

//====================================================================
// CConcurrentSkipListMap::Node::
//====================================================================

CConcurrentSkipListMap::Node::Node(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [in] */ Node* next)
{
    mKey = key;
    mValue = value;
    mNext = next;
}

CConcurrentSkipListMap::Node::Node(
    /* [in] */ Node* next)
{
    mKey = NULL;
    mValue = this;
    mNext = next;
}

PInterface CConcurrentSkipListMap::Node::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return THIS_PROBE(IInterface);
    }
    return NULL;
}

UInt32 CConcurrentSkipListMap::Node::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CConcurrentSkipListMap::Node::Release()
{
    return ElRefBase::Release();
}

ECode CConcurrentSkipListMap::Node::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (Object::Equals(object, THIS_PROBE(IInterface))) {
        *iid = EIID_IInterface;
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

Boolean CConcurrentSkipListMap::Node::CasValue(
    /* [in] */ IInterface* cmp,
    /* [in] */ IInterface* val)
{
    assert(0 && "TODO");
//    return UNSAFE.compareAndSwapObject(this, valueOffset, cmp, val);
    return FALSE;
}

Boolean CConcurrentSkipListMap::Node::CasNext(
    /* [in] */ Node* cmp,
    /* [in] */ Node* val)
{
    assert(0 && "TODO");
//    return UNSAFE.compareAndSwapObject(this, nextOffset, cmp, val);
    return FALSE;
}

Boolean CConcurrentSkipListMap::Node::IsMarker()
{
    return Object::Equals(mValue, THIS_PROBE(IInterface));
}

Boolean CConcurrentSkipListMap::Node::IsBaseHeader()
{
    return Object::Equals(mValue, sBASE_HEADER);
}

Boolean CConcurrentSkipListMap::Node::AppendMarker(
    /* [in] */ Node* f)
{
    return CasNext(f, new Node(f));
}

void CConcurrentSkipListMap::Node::HelpDelete(
    /* [in] */ Node* b,
    /* [in] */ Node* f)
{
    /*
     * Rechecking links and then doing only one of the
     * help-out stages per call tends to minimize CAS
     * interference among helping threads.
     */
    if (Object::Equals(f->Probe(EIID_IInterface), mNext)
        && Object::Equals(THIS_PROBE(IInterface), b->mNext)) {
        if (f == NULL || !Object::Equals(f->mValue, f->Probe(EIID_IInterface))) // not already marked
            AppendMarker(f);
        else
            b->CasNext(this, f->mNext);
    }
}

AutoPtr<IInterface> CConcurrentSkipListMap::Node::GetValidValue()
{
    AutoPtr<IInterface> v = mValue;
    if (Object::Equals(v, THIS_PROBE(IInterface))
        || Object::Equals(v, sBASE_HEADER) )
        return NULL;
    return v;
}

AutoPtr<AbstractMap::SimpleImmutableEntry> CConcurrentSkipListMap::Node::CreateSnapshot()
{
    AutoPtr<IInterface> v = GetValidValue();
    if (v == NULL)
        return NULL;
    AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(mKey, v);
    return p;
}

// UNSAFE mechanics

// private static final sun.misc.Unsafe UNSAFE;
// private static final long valueOffset;
// private static final long nextOffset;

// static {
//     try {
//         UNSAFE = sun.misc.Unsafe.getUnsafe();
//         Class<?> k = Node.class;
//         valueOffset = UNSAFE.objectFieldOffset
//             (k.getDeclaredField("value"));
//         nextOffset = UNSAFE.objectFieldOffset
//             (k.getDeclaredField("next"));
//     } catch (Exception e) {
//         throw new Error(e);
//     }
// }

//====================================================================
// CConcurrentSkipListMap::Index::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::Index, IInterface);

CConcurrentSkipListMap::Index::Index(
    /* [in] */ Node* node,
    /* [in] */ Index* down,
    /* [in] */ Index* right)
{
    mNode = node;
    mDown = down;
    mRight = right;
}

Boolean CConcurrentSkipListMap::Index::CasRight(
    /* [in] */ Index* cmp,
    /* [in] */ Index* val)
{
    assert(0 && "TODO");
//    return UNSAFE.compareAndSwapObject(this, rightOffset, cmp, val);
    return FALSE;
}

Boolean CConcurrentSkipListMap::Index::IndexesDeletedNode()
{
    return mNode->mValue == NULL;
}

Boolean CConcurrentSkipListMap::Index::Link(
    /* [in] */ Index* succ,
    /* [in] */ Index* newSucc)
{
    AutoPtr<Node> n = mNode;
    newSucc->mRight = succ;
    return n->mValue != NULL && CasRight(succ, newSucc);
}

Boolean CConcurrentSkipListMap::Index::Unlink(
    /* [in] */ Index* succ)
{
    return !IndexesDeletedNode() && CasRight(succ, succ->mRight);
}

// Unsafe mechanics
// private static final sun.misc.Unsafe UNSAFE;
// private static final long rightOffset;
// static {
//     try {
//         UNSAFE = sun.misc.Unsafe.getUnsafe();
//         Class<?> k = Index.class;
//         rightOffset = UNSAFE.objectFieldOffset
//             (k.getDeclaredField("right"));
//     } catch (Exception e) {
//         throw new Error(e);
//     }
// }

//====================================================================
// CConcurrentSkipListMap::HeadIndex::
//====================================================================

CConcurrentSkipListMap::HeadIndex::HeadIndex(
    /* [in] */ Node* node,
    /* [in] */ Index* down,
    /* [in] */ Index* right,
    /* [in] */ Int32 level) : Index(node, down, right)
{
    mLevel = level;
}

//====================================================================
// CConcurrentSkipListMap::ComparableUsingComparator::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::ComparableUsingComparator, IComparable)

CConcurrentSkipListMap::ComparableUsingComparator::ComparableUsingComparator(
    /* [in] */ IInterface* key,
    /* [in] */ IComparator* cmp)
{
    mActualKey = key;
    mCmp = cmp;
}

ECode CConcurrentSkipListMap::ComparableUsingComparator::CompareTo(
    /* [in] */ IInterface* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mCmp->Compare(mActualKey, another, result);
}

//====================================================================
// CConcurrentSkipListMap::
//====================================================================

AutoPtr<IComparable> CConcurrentSkipListMap::Comparable(
    /* [in] */ PInterface key)
{
    if (key == NULL)
        return NULL;
//        throw new NullPointerException();
    AutoPtr<IComparable> p;
    if (mComparator != NULL) {
        p = new ComparableUsingComparator(key, mComparator);
        return p;
    }
    else {
        p = (IComparable*)key->Probe(EIID_IComparable);
        return p;
    }
}

Int32 CConcurrentSkipListMap::Compare(
    /* [in] */ PInterface k1,
    /* [in] */ PInterface k2)
{
    AutoPtr<IComparator> cmp = mComparator;
    if (cmp != NULL) {
        Int32 res;
        cmp->Compare(k1, k2, &res);
        return res;
    }
    else {
        AutoPtr<IComparable> p = (IComparable*)k1->Probe(EIID_IComparable);
        if (p != NULL) {
            Int32 res;
            p->CompareTo(k2, &res);
            return res;
        }
    }
    return -1;
}

Boolean CConcurrentSkipListMap::InHalfOpenRange(
    /* [in] */ PInterface key,
    /* [in] */ PInterface least,
    /* [in] */ PInterface fence)
{
    if (key == NULL)
        return FALSE;
//        throw new NullPointerException();
    return ((least == NULL || Compare(key, least) >= 0) &&
            (fence == NULL || Compare(key, fence) <  0));
}

Boolean CConcurrentSkipListMap::InOpenRange(
    /* [in] */ PInterface key,
    /* [in] */ PInterface least,
    /* [in] */ PInterface fence)
{
    if (key == NULL)
        return FALSE;
//        throw new NullPointerException();
    return ((least == NULL || Compare(key, least) >= 0) &&
            (fence == NULL || Compare(key, fence) <= 0));
}

/* ---------------- Traversal -------------- */

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::FindPredecessor(
    /* [in] */ IComparable* key)
{
    if (key == NULL)
        return NULL;
//        throw new NullPointerException(); // don't postpone errors
    for (;;) {
        AutoPtr<Index> q = mHead;
        AutoPtr<Index> r = q->mRight;
        for (;;) {
            if (r != NULL) {
                AutoPtr<Node> n = r->mNode;
                AutoPtr<IInterface> k = n->mKey;
                if (n->mValue == NULL) {
                    if (!q->Unlink(r))
                        break;           // restart
                    r = q->mRight;         // reread r
                    continue;
                }
                Int32 res;
                if ((key->CompareTo(k, &res), res) > 0) {
                    q = r;
                    r = r->mRight;
                    continue;
                }
            }
            AutoPtr<Index> d = q->mDown;
            if (d != NULL) {
                q = d;
                r = d->mRight;
            }
            else
                return q->mNode;
        }
    }
    return NULL;
}

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::FindNode(
    /* [in] */ IComparable* key)
{
    for (;;) {
        AutoPtr<Node> b = FindPredecessor(key);
        AutoPtr<Node> n = b->mNext;
        for (;;) {
            if (n == NULL)
                return NULL;
            AutoPtr<Node> f = n->mNext;
            if (n != b->mNext)                // inconsistent read
                break;
            AutoPtr<IInterface> v = n->mValue;
            if (v == NULL) {                // n is deleted
                n->HelpDelete(b, f);
                break;
            }
            if (Object::Equals(v, n->Probe(EIID_IInterface))
                || b->mValue == NULL)  // b is deleted
                break;
            Int32 c = 0;
            key->CompareTo(n->mKey, &c);
            if (c == 0)
                return n;
            if (c < 0)
                return NULL;
            b = n;
            n = f;
        }
    }
}

AutoPtr<IInterface> CConcurrentSkipListMap::DoGet(
    /* [in] */ PInterface okey)
{
    AutoPtr<IComparable> key = Comparable(okey);
    /*
     * Loop needed here and elsewhere in case value field goes
     * null just as it is about to be returned, in which case we
     * lost a race with a deletion, so must retry.
     */
    for (;;) {
        AutoPtr<Node> n = FindNode(key);
        if (n == NULL)
            return NULL;
        AutoPtr<IInterface> v = n->mValue;
        if (v != NULL)
            return v;
    }
}

/* ---------------- Insertion -------------- */

AutoPtr<IInterface> CConcurrentSkipListMap::DoPut(
    /* [in] */ PInterface kkey,
    /* [in] */ PInterface value,
    /* [in] */ Boolean onlyIfAbsent)
{
    AutoPtr<IComparable> key = Comparable(kkey);
    for (;;) {
        AutoPtr<Node> b = FindPredecessor(key);
        AutoPtr<Node> n = b->mNext;
        for (;;) {
            if (n != NULL) {
                AutoPtr<Node> f = n->mNext;
                if (n != b->mNext)               // inconsistent read
                    break;
                AutoPtr<IInterface> v = n->mValue;
                if (v == NULL) {               // n is deleted
                    n->HelpDelete(b, f);
                    break;
                }
                if (Object::Equals(v, n->Probe(EIID_IInterface))
                    || b->mValue == NULL) // b is deleted
                    break;
                Int32 c = 0;
                key->CompareTo(n->mKey, &c);
                if (c > 0) {
                    b = n;
                    n = f;
                    continue;
                }
                if (c == 0) {
                    if (onlyIfAbsent || n->CasValue(v, value))
                        return v;
                    else
                        break; // restart if lost race to replace value
                }
                // else c < 0; fall through
            }

            AutoPtr<Node> z = new Node(kkey, value, n);
            if (!b->CasNext(n, z))
                break;         // restart if lost race to append to b
            Int32 level = RandomLevel();
            if (level > 0)
                InsertIndex(z, level);
            return NULL;
        }
    }
}

Int32 CConcurrentSkipListMap::RandomLevel()
{
    Int32 x = mRandomSeed;
    x ^= x << 13;
    x ^= x >> 17;
    mRandomSeed = x ^= x << 5;
    if ((x & 0x80000001) != 0) // test highest and lowest bits
        return 0;
    Int32 level = 1;
    while (((x >>= 1) & 1) != 0) ++level;
    return level;
}

void CConcurrentSkipListMap::InsertIndex(
    /* [in] */ Node* z,
    /* [in] */ Int32 level)
{
    AutoPtr<HeadIndex> h = mHead;
    Int32 max = h->mLevel;

    if (level <= max) {
        AutoPtr<Index> idx = NULL;
        for (Int32 i = 1; i <= level; ++i)
            idx = new Index(z, idx, NULL);
        AddIndex(idx, h, level);
    }
    else { // Add a new level
        /*
         * To reduce interference by other threads checking for
         * empty levels in tryReduceLevel, new levels are added
         * with initialized right pointers. Which in turn requires
         * keeping levels in an array to access them while
         * creating new head index nodes from the opposite
         * direction.
         */
        level = max + 1;
        AutoPtr<ArrayOf<Index*> > idxs = ArrayOf<Index*>::Alloc(level+1);
        AutoPtr<Index> idx = NULL;
        for (Int32 i = 1; i <= level; ++i) {
            idx = new Index(z, idx, NULL);
            idxs->Set(i, idx);
        }

        AutoPtr<HeadIndex> oldh;
        Int32 k;
        for (;;) {
            oldh = mHead;
            Int32 oldLevel = oldh->mLevel;
            if (level <= oldLevel) { // lost race to add level
                k = level;
                break;
            }
            AutoPtr<HeadIndex> newh = oldh;
            AutoPtr<Node> oldbase = oldh->mNode;
            for (Int32 j = oldLevel+1; j <= level; ++j)
                newh = new HeadIndex(oldbase, newh, (*idxs)[j], j);
            if (CasHead(oldh, newh)) {
                k = oldLevel;
                break;
            }
        }
        AddIndex((*idxs)[k], oldh, k);
    }
}

void CConcurrentSkipListMap::AddIndex(
    /* [in] */ Index* idx,
    /* [in] */ HeadIndex* h,
    /* [in] */ Int32 indexLevel)
{
    // Track next level to insert in case of retries
    Int32 insertionLevel = indexLevel;
    AutoPtr<IComparable> key = Comparable(idx->mNode->mKey);
    if (key == NULL) return; //throw new NullPointerException();

    // Similar to findPredecessor, but adding index nodes along
    // path to key.
    for (;;) {
        Int32 j = h->mLevel;
        AutoPtr<Index> q = h;
        AutoPtr<Index> r = q->mRight;
        AutoPtr<Index> t = idx;
        for (;;) {
            if (r != NULL) {
                AutoPtr<Node> n = r->mNode;
                // compare before deletion check avoids needing recheck
                Int32 c = 0;
                key->CompareTo(n->mKey, &c);
                if (n->mValue == NULL) {
                    if (!q->Unlink(r))
                        break;
                    r = q->mRight;
                    continue;
                }
                if (c > 0) {
                    q = r;
                    r = r->mRight;
                    continue;
                }
            }

            if (j == insertionLevel) {
                // Don't insert index if node already deleted
                if (t->IndexesDeletedNode()) {
                    FindNode(key); // cleans up
                    return;
                }
                if (!q->Link(r, t))
                    break; // restart
                if (--insertionLevel == 0) {
                    // need final deletion check before return
                    if (t->IndexesDeletedNode())
                        FindNode(key);
                    return;
                }
            }

            if (--j >= insertionLevel && j < indexLevel)
                t = t->mDown;
            q = q->mDown;
            r = q->mRight;
        }
    }
}

/* ---------------- Deletion -------------- */

AutoPtr<IInterface> CConcurrentSkipListMap::DoRemove(
    /* [in] */ PInterface okey,
    /* [in] */ PInterface value)
{
    AutoPtr<IComparable> key = Comparable(okey);
    for (;;) {
        AutoPtr<Node> b = FindPredecessor(key);
        AutoPtr<Node> n = b->mNext;
        for (;;) {
            if (n == NULL)
                return NULL;
            AutoPtr<Node> f = n->mNext;
            if (n != b->mNext)                    // inconsistent read
                break;
            AutoPtr<IInterface> v = n->mValue;
            if (v == NULL) {                    // n is deleted
                n->HelpDelete(b, f);
                break;
            }
            if (Object::Equals(v, n->Probe(EIID_IInterface)) || b->mValue == NULL)      // b is deleted
                break;
            Int32 c = 0;
            key->CompareTo(n->mKey, &c);
            if (c < 0)
                return NULL;
            if (c > 0) {
                b = n;
                n = f;
                continue;
            }
            if (value != NULL && !Object::Equals(value, v))
                return NULL;
            if (!n->CasValue(v, NULL))
                break;
            if (!n->AppendMarker(f) || !b->CasNext(n, f))
                FindNode(key);                  // Retry via findNode
            else {
                FindPredecessor(key);           // Clean index
                if (mHead->mRight == NULL)
                    TryReduceLevel();
            }
            return v;
        }
    }
}

void CConcurrentSkipListMap::TryReduceLevel()
{
    AutoPtr<HeadIndex> h = mHead;
    AutoPtr<HeadIndex> d;
    AutoPtr<HeadIndex> e;
    if (h->mLevel > 3 &&
        (d = h->mDown) != NULL &&
        (e = d->mDown) != NULL &&
        e->mRight == NULL &&
        d->mRight == NULL &&
        h->mRight == NULL &&
        CasHead(h, d) && // try to set
        h->mRight != NULL) // recheck
        CasHead(d, h);   // try to backout
}

/* ---------------- Finding and removing first element -------------- */

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::FindFirst()
{
    for (;;) {
        AutoPtr<Node> b = mHead->mNode;
        AutoPtr<Node> n = b->mNext;
        if (n == NULL)
            return NULL;
        if (n->mValue != NULL)
            return n;
        n->HelpDelete(b, n->mNext);
    }
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::DoRemoveFirstEntry()
{
    for (;;) {
        AutoPtr<Node> b = mHead->mNode;
        AutoPtr<Node> n = b->mNext;
        if (n == NULL)
            return NULL;
        AutoPtr<Node> f = n->mNext;
        if (!Object::Equals(n->Probe(EIID_IInterface), b->mNext))
            continue;
        AutoPtr<IInterface> v = n->mValue;
        if (v == NULL) {
            n->HelpDelete(b, f);
            continue;
        }
        if (!n->CasValue(v, NULL))
            continue;
        if (!n->AppendMarker(f) || !b->CasNext(n, f))
            FindFirst(); // retry
        ClearIndexToFirst();
        AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(n->mKey, v);
        AutoPtr<IMapEntry> res = (IMapEntry*)p->Probe(EIID_IMapEntry);
        return res;
    }
}

void CConcurrentSkipListMap::ClearIndexToFirst()
{
    for (;;) {
        AutoPtr<Index> q = mHead;
        for (;;) {
            AutoPtr<Index> r = q->mRight;
            if (r != NULL && r->IndexesDeletedNode() && !q->Unlink(r))
                break;
            if ((q = q->mDown) == NULL) {
                if (mHead->mRight == NULL)
                    TryReduceLevel();
                return;
            }
        }
    }
}

/* ---------------- Finding and removing last element -------------- */

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::FindLast()
{
    /*
     * findPredecessor can't be used to traverse index level
     * because this doesn't use comparisons.  So traversals of
     * both levels are folded together.
     */
    AutoPtr<Index> q = mHead;
    for (;;) {
        AutoPtr<Index> d, r;
        if ((r = q->mRight) != NULL) {
            if (r->IndexesDeletedNode()) {
                q->Unlink(r);
                q = mHead; // restart
            }
            else
                q = r;
        }
        else if ((d = q->mDown) != NULL) {
            q = d;
        }
        else {
            AutoPtr<Node> b = q->mNode;
            AutoPtr<Node> n = b->mNext;
            for (;;) {
                if (n == NULL)
                    return b->IsBaseHeader() ? NULL : b;
                AutoPtr<Node> f = n->mNext;            // inconsistent read
                if (n != b->mNext)
                    break;
                AutoPtr<IInterface> v = n->mValue;
                if (v == NULL) {                 // n is deleted
                    n->HelpDelete(b, f);
                    break;
                }
                if (Object::Equals(v, n->Probe(EIID_IInterface))
                    || b->mValue == NULL)   // b is deleted
                    break;
                b = n;
                n = f;
            }
            q = mHead; // restart
        }
    }
}

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::FindPredecessorOfLast()
{
    for (;;) {
        AutoPtr<Index> q = mHead;
        for (;;) {
            AutoPtr<Index> d, r;
            if ((r = q->mRight) != NULL) {
                if (r->IndexesDeletedNode()) {
                    q->Unlink(r);
                    break;    // must restart
                }
                // proceed as far across as possible without overshooting
                if (r->mNode->mNext != NULL) {
                    q = r;
                    continue;
                }
            }
            if ((d = q->mDown) != NULL)
                q = d;
            else
                return q->mNode;
        }
    }
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::DoRemoveLastEntry()
{
    for (;;) {
        AutoPtr<Node> b = FindPredecessorOfLast();
        AutoPtr<Node> n = b->mNext;
        if (n == NULL) {
            if (b->IsBaseHeader())               // empty
                return NULL;
            else
                continue; // all b's successors are deleted; retry
        }
        for (;;) {
            AutoPtr<Node> f = n->mNext;
            if (n != b->mNext)                    // inconsistent read
                break;
            AutoPtr<IInterface> v = n->mValue;
            if (v == NULL) {                    // n is deleted
                n->HelpDelete(b, f);
                break;
            }
            if (Object::Equals(v, n->Probe(EIID_IInterface)) || b->mValue == NULL)      // b is deleted
                break;
            if (f != NULL) {
                b = n;
                n = f;
                continue;
            }
            if (!n->CasValue(v, NULL))
                break;
            AutoPtr<IInterface> key = n->mKey;
            AutoPtr<IComparable> ck = Comparable(key);
            if (!n->AppendMarker(f) || !b->CasNext(n, f))
                FindNode(ck);                  // Retry via findNode
            else {
                FindPredecessor(ck);           // Clean index
                if (mHead->mRight == NULL)
                    TryReduceLevel();
            }
            AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(key, v);
            AutoPtr<IMapEntry> res = (IMapEntry*)p->Probe(EIID_IMapEntry);
            return res;
        }
    }
}

/* ---------------- Relational operations -------------- */

// Control values OR'ed as arguments to findNear

Int32 CConcurrentSkipListMap::sEQ = 1;
Int32 CConcurrentSkipListMap::sLT = 2;
Int32 CConcurrentSkipListMap::sGT = 0; // Actually checked as !sLT

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::FindNear(
    /* [in] */ PInterface kkey,
    /* [in] */ Int32 rel)
{
    AutoPtr<IComparable> key = Comparable(kkey);
    for (;;) {
        AutoPtr<Node> b = FindPredecessor(key);
        AutoPtr<Node> n = b->mNext;
        for (;;) {
            if (n == NULL)
                return ((rel & sLT) == 0 || b->IsBaseHeader()) ? NULL : b;
            AutoPtr<Node> f = n->mNext;
            if (n != b->mNext)                  // inconsistent read
                break;
            AutoPtr<IInterface> v = n->mValue;
            if (v == NULL) {                  // n is deleted
                n->HelpDelete(b, f);
                break;
            }
            if (Object::Equals(v, n->Probe(EIID_IInterface)) || b->mValue == NULL)    // b is deleted
                break;
            Int32 c = 0;
            key->CompareTo(n->mKey, &c);
            if ((c == 0 && (rel & sEQ) != 0) ||
                (c <  0 && (rel & sLT) == 0))
                return n;
            if ( c <= 0 && (rel & sLT) != 0)
                return b->IsBaseHeader() ? NULL : b;
            b = n;
            n = f;
        }
    }
}

AutoPtr<AbstractMap::SimpleImmutableEntry> CConcurrentSkipListMap::GetNear(
    /* [in] */ PInterface key,
    /* [in] */ Int32 rel)
{
    for (;;) {
        AutoPtr<Node> n = FindNear(key, rel);
        if (n == NULL)
            return NULL;
        AutoPtr<SimpleImmutableEntry> e = n->CreateSnapshot();
        if (e != NULL)
            return e;
    }
}

/* ---------------- Constructors -------------- */

ECode CConcurrentSkipListMap::constructor()
{
    mComparator = NULL;
    return Initialize();
}

ECode CConcurrentSkipListMap::constructor(
    /* [in] */ IComparator* comparator)
{
    mComparator = comparator;
    return Initialize();
}

ECode CConcurrentSkipListMap::constructor(
    /* [in] */ IMap* m)
{
    mComparator = NULL;
    Initialize();
    return AbstractMap::PutAll(m);
}

ECode CConcurrentSkipListMap::constructor(
    /* [in] */ ISortedMap* m)
{
    m->Comparator((IComparator**)&mComparator);
    Initialize();
    BuildFromSorted(m);
    return NOERROR;
}

PInterface CConcurrentSkipListMap::Probe(
    /* [in] */ REIID riid)
{
    return _CConcurrentSkipListMap::Probe(riid);
}

ECode CConcurrentSkipListMap::Clone(
    /* [out] */ IInterface** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> clone;
    CConcurrentSkipListMap::New((INavigableMap**)&clone);
    CConcurrentSkipListMap* cRes = (CConcurrentSkipListMap*)clone.Get();
    cRes->Initialize();
    cRes->BuildFromSorted(this);
    *res = clone->Probe(EIID_IInterface);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

void CConcurrentSkipListMap::BuildFromSorted(
    /* [in] */ ISortedMap* map)
{
    if (map == NULL)
        return;
    //    throw new NullPointerException();

    AutoPtr<HeadIndex> h = mHead;
    AutoPtr<Node> basepred = h->mNode;

    // Track the current rightmost node at each level. Uses an
    // ArrayList to avoid committing to initial or maximum level.
    AutoPtr<IArrayList> preds;
    CArrayList::New((IArrayList**)&preds);

    // initialize
    for (Int32 i = 0; i <= h->mLevel; ++i) {
        Boolean b;
        preds->Add(NULL, &b);
    }
    AutoPtr<Index> q = h;
    AutoPtr<IInterface> pre;
    for (Int32 i = h->mLevel; i > 0; --i) {
        preds->Set(i, q, (IInterface**)&pre);
        q = q->mDown;
    }

    AutoPtr<ISet> s;
    map->EntrySet((ISet**)&s);
    AutoPtr<IIterator> it;
    s->GetIterator((IIterator**)&it);
    Boolean bNext;
    while ((it->HasNext(&bNext), bNext)) {
        AutoPtr<IMapEntry> e;
        it->Next((IInterface**)&e);
        Int32 j = RandomLevel();
        if (j > h->mLevel) j = h->mLevel + 1;
        AutoPtr<IInterface> k;
        e->GetKey((IInterface**)&k);
        AutoPtr<IInterface> v;
        e->GetValue((IInterface**)&v);
        if (k == NULL || v == NULL)
            return;
//            throw new NullPointerException();
        AutoPtr<Node> z = new Node(k, v, NULL);
        basepred->mNext = z;
        basepred = z;
        if (j > 0) {
            AutoPtr<Index> idx = NULL;
            for (Int32 i = 1; i <= j; ++i) {
                idx = new Index(z, idx, NULL);
                if (i > h->mLevel)
                    h = new HeadIndex(h->mNode, h, idx, i);
                Int32 size = 0;
                preds->GetSize(&size);
                if (i < size) {
                    AutoPtr<Index> ar;
                    preds->Get(i, (IInterface**)&ar);
                    ar->mRight = idx;
                    AutoPtr<IInterface> pre;
                    preds->Set(i, idx, (IInterface**)&pre);
                }
                else {
                    Boolean b;
                    preds->Add(idx, &b);
                }
            }
        }
    }
    mHead = h;
}

/* ---------------- Serialization -------------- */

void CConcurrentSkipListMap::WriteObject(
    /* [in] */ IObjectOutputStream* s)
{
    assert(0 && "TODO");
    // Write out the Comparator and any hidden stuff
    s->DefaultWriteObject();

    // Write out keys and values (alternating)
    for (AutoPtr<Node> n = FindFirst(); n != NULL; n = n->mNext) {
        AutoPtr<IInterface> v = n->GetValidValue();
        if (v != NULL) {
//            s->WriteObject(n->mKey);
//            s->WriteObject(v);
        }
    }
//    s->WriteObject(NULL);
}

void CConcurrentSkipListMap::ReadObject(
    /* [in] */ IObjectInputStream* s)
{
    // Read in the Comparator and any hidden stuff
    s->DefaultReadObject();
    // Reset transients
    Initialize();

    /*
     * This is nearly identical to buildFromSorted, but is
     * distinct because readObject calls can't be nicely adapted
     * as the kind of iterator needed by buildFromSorted. (They
     * can be, but doing so requires type cheats and/or creation
     * of adaptor classes.) It is simpler to just adapt the code.
     */

    AutoPtr<HeadIndex> h = mHead;
    AutoPtr<Node> basepred = h->mNode;
    AutoPtr<IArrayList> preds;
    CArrayList::New((IArrayList**)&preds);
    for (Int32 i = 0; i <= h->mLevel; ++i) {
        Boolean b;
        preds->Add(NULL, &b);
    }
    AutoPtr<Index> q = h;
    for (Int32 i = h->mLevel; i > 0; --i) {
        AutoPtr<IInterface> pre;
        preds->Set(i, q, (IInterface**)&pre);
        q = q->mDown;
    }

    for (;;) {
        assert(0 && "TODO");
        AutoPtr<IInterface> k;// = s->ReadObject();
        if (k == NULL)
            break;
        AutoPtr<IInterface> v;// = s->ReadObject();
        if (v == NULL)
            return;
        //    throw new NullPointerException();
        AutoPtr<IInterface> key = k;
        AutoPtr<IInterface> val = v;
        Int32 j = RandomLevel();
        if (j > h->mLevel) j = h->mLevel + 1;
        AutoPtr<Node> z = new Node(key, val, NULL);
        basepred->mNext = z;
        basepred = z;
        if (j > 0) {
            AutoPtr<Index> idx = NULL;
            for (Int32 i = 1; i <= j; ++i) {
                idx = new Index(z, idx, NULL);
                if (i > h->mLevel)
                    h = new HeadIndex(h->mNode, h, idx, i);
                Int32 size = 0;
                preds->GetSize(&size);
                if (i < size) {
                    AutoPtr<Index> ar;
                    preds->Get(i, (IInterface**)&ar);
                    ar->mRight = idx;
                    AutoPtr<IInterface> pre;
                    preds->Set(i, idx, (IInterface**)&pre);
                }
                else {
                    Boolean b;
                    preds->Add(idx, &b);
                }
            }
        }
    }
    mHead = h;
}

/* ------ Map API methods ------ */

ECode CConcurrentSkipListMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = DoGet(key) != NULL;
    return NOERROR;
}

ECode CConcurrentSkipListMap::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = DoGet(key);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CConcurrentSkipListMap::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** oldValue)
{
    VALIDATE_NOT_NULL(oldValue);
    if (value == NULL)
        return E_NULL_POINTER_EXCEPTION;
    *oldValue = DoPut(key, value, FALSE);
    REFCOUNT_ADD(*oldValue);
    return NOERROR;
}

ECode CConcurrentSkipListMap::Remove(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = DoRemove(key, NULL);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CConcurrentSkipListMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (value == NULL)
        return E_NULL_POINTER_EXCEPTION;
    for (AutoPtr<Node> n = FindFirst(); n != NULL; n = n->mNext) {
        AutoPtr<IInterface> v = n->GetValidValue();
        if (v != NULL && Object::Equals(value, v)) {
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode CConcurrentSkipListMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    Int64 count = 0;
    for (AutoPtr<Node> n = FindFirst(); n != NULL; n = n->mNext) {
        if (n->GetValidValue() != NULL)
            ++count;
    }
    *size = (count >= Elastos::Core::Math::INT32_MAX_VALUE) ? Elastos::Core::Math::INT32_MAX_VALUE : (Int32) count;
    return NOERROR;
}

ECode CConcurrentSkipListMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FindFirst() == NULL;
    return NOERROR;
}

ECode CConcurrentSkipListMap::Clear()
{
    return Initialize();
}

/* ---------------- View methods -------------- */

ECode CConcurrentSkipListMap::KeySet(
    /* [out] */ ISet** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<_KeySet> ks = mKeySet;
    *res = (ks != NULL) ? ks : (mKeySet = new _KeySet(this));
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::NavigableKeySet(
    /* [out] */ INavigableSet** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<_KeySet> ks = mKeySet;
    *res = (ks != NULL) ? ks : (mKeySet = new _KeySet(this));
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<_Values> vs = mValues;
    *value = (vs != NULL) ? vs : (mValues = new _Values(this));
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CConcurrentSkipListMap::EntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries);
    AutoPtr<_EntrySet> es = mEntrySet;
    *entries = (es != NULL) ? es : (mEntrySet = new _EntrySet(this));
    REFCOUNT_ADD(*entries);
    return NOERROR;
}

ECode CConcurrentSkipListMap::DescendingMap(
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> dm = mDescendingMap;
    *res = (dm != NULL) ? dm : (mDescendingMap = new _SubMap(this, NULL, FALSE, NULL, FALSE, TRUE));
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::DescendingKeySet(
    /* [out] */ INavigableSet** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> nm;
    DescendingMap((INavigableMap**)&nm);
    return nm->NavigableKeySet(res);
}

/* ---------------- AbstractMap Overrides -------------- */

ECode CConcurrentSkipListMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (Object::Equals(object, THIS_PROBE(IInterface))) {
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<IMap> m = (IMap*)object->Probe(EIID_IMap);
    if (m == NULL) {
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<ISet> entries;
    this->EntrySet((ISet**)&entries);
    AutoPtr<ArrayOf<IInterface*> > arr;
    entries->ToArray((ArrayOf<IInterface*>**)&arr);
    for (Int32 i = 0;i < arr->GetLength();i++) {
        AutoPtr<IInterface> o = (*arr)[i];
        AutoPtr<IMapEntry> e = (IMapEntry*)o->Probe(EIID_IMapEntry);
        AutoPtr<IInterface> k;
        e->GetKey((IInterface**)&k);
        AutoPtr<IInterface> v;
        m->Get(k, (IInterface**)&v);
        AutoPtr<IInterface> v2;
        e->GetValue((IInterface**)&v2);
        if (!Object::Equals(v2, v)) {
            *result = FALSE;
            return NOERROR;
        }
    }
    AutoPtr<ISet> entries2;
    m->EntrySet((ISet**)&entries2);
    AutoPtr<ArrayOf<IInterface*> > arr2;
    entries2->ToArray((ArrayOf<IInterface*>**)&arr2);
    for (Int32 i = 0;i < arr2->GetLength();i++) {
        AutoPtr<IInterface> o = (*arr2)[i];
        AutoPtr<IMapEntry> e = (IMapEntry*)o->Probe(EIID_IMapEntry);
        AutoPtr<IInterface> k;
        e->GetKey((IInterface**)&k);
        AutoPtr<IInterface> v;
        e->GetValue((IInterface**)&v);
        AutoPtr<IInterface> v2;
        Get(k, (IInterface**)&v2);
        if (k == NULL || v == NULL || !Object::Equals(v2, v)) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

/* ------ ConcurrentMap API methods ------ */

ECode CConcurrentSkipListMap::PutIfAbsent(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* out)
{
    VALIDATE_NOT_NULL(out);
    if (value == NULL)
        return E_NULL_POINTER_EXCEPTION;
    *out = DoPut(key, value, TRUE);
    REFCOUNT_ADD(*out);
    return NOERROR;
}

ECode CConcurrentSkipListMap::Remove(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    if (key == NULL)
        return E_NULL_POINTER_EXCEPTION;
    if (value == NULL) {
        *res = FALSE;
        return NOERROR;
    }
    *res = DoRemove(key, value) != NULL;
    return NOERROR;
}

ECode CConcurrentSkipListMap::Replace(
    /* [in] */ PInterface key,
    /* [in] */ PInterface oldValue,
    /* [in] */ PInterface newValue,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    if (oldValue == NULL || newValue == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IComparable> k = Comparable(key);
    for (;;) {
        AutoPtr<Node> n = FindNode(k);
        if (n == NULL) {
            *res = FALSE;
            return NOERROR;
        }
        AutoPtr<IInterface> v = n->mValue;
        if (v != NULL) {
            if (! Object::Equals(oldValue, v)) {
                *res = FALSE;
                return NOERROR;
            }
            if (n->CasValue(v, newValue)) {
                *res = TRUE;
                return NOERROR;
            }
        }
    }
}

ECode CConcurrentSkipListMap::Replace(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* res)
{
    VALIDATE_NOT_NULL(res);
    if (value == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IComparable> k = Comparable(key);
    for (;;) {
        AutoPtr<Node> n = FindNode(k);
        if (n == NULL)
            return E_NULL_POINTER_EXCEPTION;
        AutoPtr<IInterface> v = n->mValue;
        if (v != NULL && n->CasValue(v, value)) {
            *res = v;
            REFCOUNT_ADD(*res);
            return NOERROR;
        }
    }
}

/* ------ SortedMap API methods ------ */

ECode CConcurrentSkipListMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp);
    *comp = mComparator;
    REFCOUNT_ADD(*comp);
    return NOERROR;
}

ECode CConcurrentSkipListMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<Node> n = FindFirst();
    if (n == NULL)
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    *outface = n->mKey;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<Node> n = FindLast();
    if (n == NULL)
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    *outface = n->mKey;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::SubMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ PInterface toKey,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    if (fromKey == NULL || toKey == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<_SubMap> p = new _SubMap(this, fromKey, fromInclusive, toKey, toInclusive, FALSE);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::HeadMap(
    /* [in] */ PInterface toKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    if (toKey == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<_SubMap> p = new _SubMap(this, NULL, FALSE, toKey, inclusive, FALSE);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::TailMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    if (fromKey == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<_SubMap> p = new _SubMap(this, fromKey, inclusive, NULL, FALSE, FALSE);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::SubMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ PInterface toKey,
    /* [out] */ ISortedMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> p;
    ECode ec = SubMap(fromKey, TRUE, toKey, FALSE, (INavigableMap**)&p);
    if (p != NULL) {
        *res = (ISortedMap*)p->Probe(EIID_ISortedMap);
        REFCOUNT_ADD(*res);
    }
    return ec;
}

ECode CConcurrentSkipListMap::HeadMap(
    /* [in] */ PInterface toKey,
    /* [out] */ ISortedMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> p;
    ECode ec = HeadMap(toKey, FALSE, (INavigableMap**)&p);
    if (p != NULL) {
        *res = (ISortedMap*)p->Probe(EIID_ISortedMap);
        REFCOUNT_ADD(*res);
    }
    return ec;
}

ECode CConcurrentSkipListMap::TailMap(
    /* [in] */ PInterface fromKey,
    /* [out] */ ISortedMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> p;
    ECode ec = TailMap(fromKey, TRUE, (INavigableMap**)&p);
    if (p != NULL) {
        *res = (ISortedMap*)p->Probe(EIID_ISortedMap);
        REFCOUNT_ADD(*res);
    }
    return ec;
}

/* ---------------- Relational operations -------------- */

ECode CConcurrentSkipListMap::LowerEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    *outent = GetNear(key, sLT);
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::LowerKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<Node> n = FindNear(key, sLT);
    *outface = (n == NULL) ? NULL : n->mKey;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::FloorEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    *outent = GetNear(key, sLT|sEQ);
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::FloorKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<Node> n = FindNear(key, sLT|sEQ);
    *outface = (n == NULL) ? NULL : n->mKey;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::CeilingEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    *outent = GetNear(key, sGT|sEQ);
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::CeilingKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<Node> n = FindNear(key, sGT|sEQ);
    *outface = (n == NULL) ? NULL : n->mKey;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::HigherEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    *outent = GetNear(key, sGT);
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::HigherKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<Node> n = FindNear(key, sGT);
    *outface = (n == NULL) ? NULL : n->mKey;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::FirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    for (;;) {
        AutoPtr<Node> n = FindFirst();
        if (n == NULL) {
            *outent = NULL;
            return NOERROR;
        }
        AutoPtr<SimpleImmutableEntry> e = n->CreateSnapshot();
        if (e != NULL) {
            *outent = e;
            REFCOUNT_ADD(*outent);
            return NOERROR;
        }
    }
}

ECode CConcurrentSkipListMap::LastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    for (;;) {
        AutoPtr<Node> n = FindLast();
        if (n == NULL) {
            *outent = NULL;
            return NOERROR;
        }
        AutoPtr<SimpleImmutableEntry> e = n->CreateSnapshot();
        if (e != NULL) {
            *outent = e;
            REFCOUNT_ADD(*outent);
            return NOERROR;
        }
    }
}

ECode CConcurrentSkipListMap::PollFirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    *outent = DoRemoveFirstEntry();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::PollLastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    *outent = DoRemoveLastEntry();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::Iter::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::Iter, IIterator);

CConcurrentSkipListMap::Iter::Iter(
    /* [in] */ CConcurrentSkipListMap* owner)
{
    mOwner = owner;
    for (;;) {
        mNext = mOwner->FindFirst();
        if (mNext == NULL)
            break;
        AutoPtr<IInterface> x = mNext->mValue;
        if (x != NULL && !Object::Equals(x, mNext->Probe(EIID_IInterface))) {
            mNextValue = x;
            break;
        }
    }
}

ECode CConcurrentSkipListMap::Iter::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNext != NULL;
    return NOERROR;
}

/** Advances next to higher entry. */
ECode CConcurrentSkipListMap::Iter::Advance()
{
    if (mNext == NULL)
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    mLastReturned = mNext;
    for (;;) {
        mNext = mNext->mNext;
        if (mNext == NULL)
            break;
        AutoPtr<IInterface> x = mNext->mValue;
        if (x != NULL && !Object::Equals(x, mNext)) {
            mNextValue = x;
            break;
        }
    }
    return NOERROR;
}

ECode CConcurrentSkipListMap::Iter::Remove()
{
    AutoPtr<Node> l = mLastReturned;
    if (l == NULL)
        return E_ILLEGAL_STATE_EXCEPTION;
    // It would not be worth all of the overhead to directly
    // unlink from here. Using remove is fast enough.
    AutoPtr<IInterface> pre;
    mOwner->Remove(l->mKey, (IInterface**)&pre);
    mLastReturned = NULL;
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::_ValueIterator::
//====================================================================
CConcurrentSkipListMap::_ValueIterator::_ValueIterator(
    /* [in] */ CConcurrentSkipListMap* owner) : Iter(owner)
{
}

ECode CConcurrentSkipListMap::_ValueIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IInterface> v = mNextValue;
    Advance();
    *object = v;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::_KeyIterator::
//====================================================================
CConcurrentSkipListMap::_KeyIterator::_KeyIterator(
    /* [in] */ CConcurrentSkipListMap* owner) : Iter(owner)
{
}

ECode CConcurrentSkipListMap::_KeyIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<Node> n = mNext;
    Advance();
    *object = n->mKey;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::_EntryIterator::
//====================================================================
CConcurrentSkipListMap::_EntryIterator::_EntryIterator(
    /* [in] */ CConcurrentSkipListMap* owner) : Iter(owner)
{
}

ECode CConcurrentSkipListMap::_EntryIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<Node> n = mNext;
    AutoPtr<IInterface> v = mNextValue;
    Advance();
    AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(n->mKey, v);
    *object = p->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::
//====================================================================

// Factory methods for iterators needed by ConcurrentSkipListSet etc

AutoPtr<IIterator> CConcurrentSkipListMap::KeyIterator()
{
    AutoPtr<_KeyIterator> cRes = new _KeyIterator(this);
    AutoPtr<IIterator> res = (IIterator*)cRes->Probe(EIID_IIterator);
    return res;
}

AutoPtr<IIterator> CConcurrentSkipListMap::ValueIterator()
{
    AutoPtr<_ValueIterator> cRes = new _ValueIterator(this);
    AutoPtr<IIterator> res = (IIterator*)cRes->Probe(EIID_IIterator);
    return res;
}

AutoPtr<IIterator> CConcurrentSkipListMap::EntryIterator()
{
    AutoPtr<_EntryIterator> cRes = new _EntryIterator(this);
    AutoPtr<IIterator> res = (IIterator*)cRes->Probe(EIID_IIterator);
    return res;
}

/* ---------------- View Classes -------------- */

AutoPtr<IList> CConcurrentSkipListMap::ToList(
    /* [in] */ ICollection* c)
{
    // Using size() here would be a pessimization.
    AutoPtr<IArrayList> list;
    CArrayList::New((IArrayList**)&list);
    AutoPtr<ArrayOf<IInterface*> > arr;
    c->ToArray((ArrayOf<IInterface*>**)&arr);
    for (Int32 i = 0;i < arr->GetLength();i++) {
        AutoPtr<IInterface> e = (*arr)[i];
        Boolean b;
        list->Add(e, &b);
    }
    return (IList*)list->Probe(EIID_IList);
}

ECode CConcurrentSkipListMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

//====================================================================
// CConcurrentSkipListMap::_KeySet::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::_KeySet, INavigableSet);

CConcurrentSkipListMap::_KeySet::_KeySet(
    /* [in] */ INavigableMap* map)
{
    mM = map;
}

ECode CConcurrentSkipListMap::_KeySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode CConcurrentSkipListMap::_KeySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode CConcurrentSkipListMap::_KeySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsKey(object, result);
}

ECode CConcurrentSkipListMap::_KeySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    AutoPtr<IInterface> res;
    mM->Remove(object, (IInterface**)&res);
    *modified = res != NULL;
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::Clear()
{
    return mM->Clear();
}

ECode CConcurrentSkipListMap::_KeySet::Lower(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->LowerKey(e, outface);
}

ECode CConcurrentSkipListMap::_KeySet::Floor(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->FloorKey(e, outface);
}

ECode CConcurrentSkipListMap::_KeySet::Ceiling(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->CeilingKey(e, outface);
}

ECode CConcurrentSkipListMap::_KeySet::Higher(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->HigherKey(e, outface);
}

ECode CConcurrentSkipListMap::_KeySet::Comparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom);
    return mM->Comparator(outcom);
}

ECode CConcurrentSkipListMap::_KeySet::First(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->FirstKey(outface);
}

ECode CConcurrentSkipListMap::_KeySet::Last(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->LastKey(outface);
}

ECode CConcurrentSkipListMap::_KeySet::PollFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IMapEntry> e;
    mM->PollFirstEntry((IMapEntry**)&e);
    if (e == NULL) {
        *outface = NULL;
        return NOERROR;
    }
    else
        return e->GetKey(outface);
}

ECode CConcurrentSkipListMap::_KeySet::PollLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IMapEntry> e;
    mM->PollLastEntry((IMapEntry**)&e);
    if (e == NULL) {
        *outface = NULL;
        return NOERROR;
    }
    else
        return e->GetKey(outface);
}

ECode CConcurrentSkipListMap::_KeySet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<INavigableMap> p = (INavigableMap*)mM->Probe(EIID_INavigableMap);
    if (p != NULL) {
        AutoPtr<CConcurrentSkipListMap> res = (CConcurrentSkipListMap*)p.Get();
        *it = res->KeyIterator();
        REFCOUNT_ADD(*it);
        return NOERROR;
    }
    else {
        AutoPtr<_SubMap> res = (_SubMap*)mM.Get();
        *it = res->KeyIterator();
        REFCOUNT_ADD(*it);
        return NOERROR;
    }
}

ECode CConcurrentSkipListMap::_KeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (Object::Equals(object, THIS_PROBE(IInterface))) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_ISet) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<ICollection> c = (ICollection*)object->Probe(EIID_ICollection);
    if (c == NULL) {
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean a = FALSE, b = FALSE;
    ContainsAll(c, &a);
    c->ContainsAll(this, &b);
    *result = a && b;
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return ToList(this)->ToArray(array);
}

ECode CConcurrentSkipListMap::_KeySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return ToList(this)->ToArray(inArray, outArray);
}

ECode CConcurrentSkipListMap::_KeySet::DescendingIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter);
    AutoPtr<INavigableSet> s;
    DescendingSet((INavigableSet**)&s);
    return s->GetIterator(outiter);
}

ECode CConcurrentSkipListMap::_KeySet::SubSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> m;
    mM->SubMap(fromElement, fromInclusive,
                toElement, toInclusive, (INavigableMap**)&m);
    *outnav = new _KeySet(m);
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::HeadSet(
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> m;
    mM->HeadMap(toElement, inclusive, (INavigableMap**)&m);
    *outnav = new _KeySet(m);
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::TailSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> m;
    mM->TailMap(fromElement, inclusive, (INavigableMap**)&m);
    *outnav = new _KeySet(m);
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<INavigableSet> p;
    SubSet(start, TRUE, end, FALSE, (INavigableSet**)&p);
    AutoPtr<ISortedSet> res = (ISortedSet*)p->Probe(EIID_ISortedSet);
    *outsort = res;
    REFCOUNT_ADD(*outsort);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<INavigableSet> p;
    HeadSet(end, FALSE, (INavigableSet**)&p);
    AutoPtr<ISortedSet> res = (ISortedSet*)p->Probe(EIID_ISortedSet);
    *outsort = res;
    REFCOUNT_ADD(*outsort);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<INavigableSet> p;
    TailSet(start, TRUE, (INavigableSet**)&p);
    AutoPtr<ISortedSet> res = (ISortedSet*)p->Probe(EIID_ISortedSet);
    *outsort = res;
    REFCOUNT_ADD(*outsort);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::DescendingSet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> m;
    mM->DescendingMap((INavigableMap**)&m);
    AutoPtr<INavigableMap> p = (INavigableMap*)m->Probe(EIID_INavigableMap);
    AutoPtr<_KeySet> res = new _KeySet(p);
    *outnav = (INavigableSet*)res->Probe(EIID_INavigableSet);
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_KeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CConcurrentSkipListMap::_KeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CConcurrentSkipListMap::_KeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CConcurrentSkipListMap::_KeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CConcurrentSkipListMap::_KeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CConcurrentSkipListMap::_KeySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

//====================================================================
// CConcurrentSkipListMap::_Values::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::_Values, ICollection);

CConcurrentSkipListMap::_Values::_Values(
    /* [in] */ INavigableMap* map)
{
    mM = map;
}

ECode CConcurrentSkipListMap::_Values::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<INavigableMap> p = (INavigableMap*)mM->Probe(EIID_INavigableMap);
    if (p != NULL) {
        AutoPtr<CConcurrentSkipListMap> cp = (CConcurrentSkipListMap*)p.Get();
        AutoPtr<IIterator> res = cp->ValueIterator();
        *it = res;
        REFCOUNT_ADD(*it);
        return NOERROR;
    }
    else {
        AutoPtr<_SubMap> p2 = (_SubMap*)mM.Get();
        AutoPtr<IIterator> res = p2->ValueIterator();
        *it = res;
        REFCOUNT_ADD(*it);
        return NOERROR;
    }
}

ECode CConcurrentSkipListMap::_Values::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode CConcurrentSkipListMap::_Values::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode CConcurrentSkipListMap::_Values::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsValue(object, result);
}

ECode CConcurrentSkipListMap::_Values::Clear()
{
    return mM->Clear();
}

ECode CConcurrentSkipListMap::_Values::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return ToList(this)->ToArray(array);
}

ECode CConcurrentSkipListMap::_Values::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return ToList(this)->ToArray(inArray, outArray);
}

ECode CConcurrentSkipListMap::_Values::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::Add(object, modified);
}

ECode CConcurrentSkipListMap::_Values::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::AddAll(collection, modified);
}

ECode CConcurrentSkipListMap::_Values::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::ContainsAll(collection, result);
}

ECode CConcurrentSkipListMap::_Values::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::RemoveAll(collection, modified);
}

ECode CConcurrentSkipListMap::_Values::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::RetainAll(collection, modified);
}

ECode CConcurrentSkipListMap::_Values::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return E_NO_SUCH_METHOD_EXCEPTION;
}

ECode CConcurrentSkipListMap::_Values::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::Remove(object, modified);
}

ECode CConcurrentSkipListMap::_Values::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return E_NO_SUCH_METHOD_EXCEPTION;
}

//====================================================================
// CConcurrentSkipListMap::_EntrySet::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::_EntrySet, ISet);

CConcurrentSkipListMap::_EntrySet::_EntrySet(
    /* [in] */ INavigableMap* map)
{
    mM = map;
}

ECode CConcurrentSkipListMap::_EntrySet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<INavigableMap> p = (INavigableMap*)mM->Probe(EIID_INavigableMap);
    if (p != NULL) {
        AutoPtr<CConcurrentSkipListMap> cp = (CConcurrentSkipListMap*)p.Get();
        AutoPtr<IIterator> res = cp->EntryIterator();
        *it = res;
        REFCOUNT_ADD(*it);
        return NOERROR;
    }
    else {
        AutoPtr<_SubMap> p2 = (_SubMap*)mM.Get();
        AutoPtr<IIterator> res = p2->EntryIterator();
        *it = res;
        REFCOUNT_ADD(*it);
        return NOERROR;
    }
}

ECode CConcurrentSkipListMap::_EntrySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IMapEntry> e = (IMapEntry*)object->Probe(EIID_IMapEntry);
    if (e == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IInterface> k;
    e->GetKey((IInterface**)&k);
    AutoPtr<IInterface> v;
    mM->Get(k, (IInterface**)&v);
    if (v != NULL) {
        AutoPtr<IInterface> v2;
        e->GetValue((IInterface**)&v2);
        *result = Object::Equals(v, v2);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CConcurrentSkipListMap::_EntrySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    if (object->Probe(EIID_IMapEntry) == NULL) {
        *modified = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = (IMapEntry*)object->Probe(EIID_IMapEntry);
    AutoPtr<IInterface> k;
    e->GetKey((IInterface**)&k);
    AutoPtr<IInterface> v;
    e->GetValue((IInterface**)&v);
    AutoPtr<IConcurrentMap> res = (IConcurrentMap*)mM->Probe(EIID_IConcurrentMap);
    return res->Remove(k, v, modified);
}

ECode CConcurrentSkipListMap::_EntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode CConcurrentSkipListMap::_EntrySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode CConcurrentSkipListMap::_EntrySet::Clear()
{
    return mM->Clear();
}

ECode CConcurrentSkipListMap::_EntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (Object::Equals(object, THIS_PROBE(IInterface))) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_ISet) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<ICollection> c = (ICollection*)object->Probe(EIID_ICollection);
    if (c == NULL) {
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean a = FALSE, b = FALSE;
    ContainsAll(c, &a);
    c->ContainsAll(this, &b);
    *result = a && b;
    return NOERROR;
}

ECode CConcurrentSkipListMap::_EntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return ToList(this)->ToArray(array);
}

ECode CConcurrentSkipListMap::_EntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return ToList(this)->ToArray(inArray, outArray);
}

ECode CConcurrentSkipListMap::_EntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CConcurrentSkipListMap::_EntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CConcurrentSkipListMap::_EntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CConcurrentSkipListMap::_EntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CConcurrentSkipListMap::_EntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CConcurrentSkipListMap::_EntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

//====================================================================
// CConcurrentSkipListMap::_SubMap::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::_SubMap, INavigableMap);

CConcurrentSkipListMap::_SubMap::_SubMap(
    /* [in] */ CConcurrentSkipListMap* map,
    /* [in] */ IInterface* fromKey,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toKey,
    /* [in] */ Boolean toInclusive,
    /* [in] */ Boolean isDescending)
{
    if (fromKey != NULL && toKey != NULL &&
        map->Compare(fromKey, toKey) > 0)
        return;

    mM = map;
    mLo = fromKey;
    mHi = toKey;
    mLoInclusive = fromInclusive;
    mHiInclusive = toInclusive;
    mIsDescending = isDescending;
}

/* ----------------  Utilities -------------- */

Boolean CConcurrentSkipListMap::_SubMap::TooLow(
    /* [in] */ IInterface* key)
{
    if (mLo != NULL) {
        Int32 c = mM->Compare(key, mLo);
        if (c < 0 || (c == 0 && !mLoInclusive))
            return TRUE;
    }
    return FALSE;
}

Boolean CConcurrentSkipListMap::_SubMap::TooHigh(
    /* [in] */ PInterface key)
{
    if (mHi != NULL) {
        Int32 c = mM->Compare(key, mHi);
        if (c > 0 || (c == 0 && !mHiInclusive))
            return TRUE;
    }
    return FALSE;
}

Boolean CConcurrentSkipListMap::_SubMap::InBounds(
    /* [in] */ PInterface key)
{
    return !TooLow(key) && !TooHigh(key);
}

ECode CConcurrentSkipListMap::_SubMap::CheckKeyBounds(
    /* [in] */ PInterface key)
{
    if (key == NULL)
        return E_NULL_POINTER_EXCEPTION;
    if (!InBounds(key))
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    return NOERROR;
}

Boolean CConcurrentSkipListMap::_SubMap::IsBeforeEnd(
    /* [in] */ Node* n)
{
    if (n == NULL)
        return FALSE;
    if (mHi == NULL)
        return TRUE;
    AutoPtr<IInterface> k = n->mKey;
    if (k == NULL) // pass by markers and headers
        return TRUE;
    Int32 c = mM->Compare(k, mHi);
    if (c > 0 || (c == 0 && !mHiInclusive))
        return FALSE;
    return TRUE;
}

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::_SubMap::LoNode()
{
    if (mLo == NULL)
        return mM->FindFirst();
    else if (mLoInclusive)
        return mM->FindNear(mLo, sGT|sEQ);
    else
        return mM->FindNear(mLo, sGT);
}

AutoPtr<CConcurrentSkipListMap::Node> CConcurrentSkipListMap::_SubMap::HiNode()
{
    if (mHi == NULL)
        return mM->FindLast();
    else if (mHiInclusive)
        return mM->FindNear(mHi, sLT|sEQ);
    else
        return mM->FindNear(mHi, sLT);
}

AutoPtr<IInterface> CConcurrentSkipListMap::_SubMap::LowestKey()
{
    AutoPtr<Node> n = LoNode();
    if (IsBeforeEnd(n))
        return n->mKey;
    else
        return NULL;
    //    throw new NoSuchElementException();
}

AutoPtr<IInterface> CConcurrentSkipListMap::_SubMap::HighestKey()
{
    AutoPtr<Node> n = HiNode();
    if (n != NULL) {
        AutoPtr<IInterface> last = n->mKey;
        if (InBounds(last))
            return last;
    }
    return NULL;
//    throw new NoSuchElementException();
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::_SubMap::LowestEntry()
{
    for (;;) {
        AutoPtr<Node> n = LoNode();
        if (!IsBeforeEnd(n))
            return NULL;
        AutoPtr<IMapEntry> e = n->CreateSnapshot();
        if (e != NULL)
            return e;
    }
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::_SubMap::HighestEntry()
{
    for (;;) {
        AutoPtr<Node> n = HiNode();
        if (n == NULL || !InBounds(n->mKey))
            return NULL;
        AutoPtr<IMapEntry> e = n->CreateSnapshot();
        if (e != NULL)
            return e;
    }
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::_SubMap::RemoveLowest()
{
    for (;;) {
        AutoPtr<Node> n = LoNode();
        if (n == NULL)
            return NULL;
        AutoPtr<IInterface> k = n->mKey;
        if (!InBounds(k))
            return NULL;
        AutoPtr<IInterface> v = mM->DoRemove(k, NULL);
        if (v != NULL) {
            AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(k, v);
            AutoPtr<IMapEntry> res = (IMapEntry*)p->Probe(EIID_IMapEntry);
            return res;
        }
    }
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::_SubMap::RemoveHighest()
{
    for (;;) {
        AutoPtr<Node> n = HiNode();
        if (n == NULL)
            return NULL;
        AutoPtr<IInterface> k = n->mKey;
        if (!InBounds(k))
            return NULL;
        AutoPtr<IInterface> v = mM->DoRemove(k, NULL);
        if (v != NULL) {
            AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(k, v);
            AutoPtr<IMapEntry> res = (IMapEntry*)p->Probe(EIID_IMapEntry);
            return res;
        }
    }
}

AutoPtr<IMapEntry> CConcurrentSkipListMap::_SubMap::GetNearEntry(
    /* [in] */ PInterface key,
    /* [in] */ Int32 rel)
{
    if (mIsDescending) { // adjust relation for direction
        if ((rel & sLT) == 0)
            rel |= sLT;
        else
            rel &= ~sLT;
    }
    if (TooLow(key))
        return ((rel & sLT) != 0) ? NULL : LowestEntry();
    if (TooHigh(key))
        return ((rel & sLT) != 0) ? HighestEntry() : NULL;
    for (;;) {
        AutoPtr<Node> n = mM->FindNear(key, rel);
        if (n == NULL || !InBounds(n->mKey))
            return NULL;
        AutoPtr<IInterface> k = n->mKey;
        AutoPtr<IInterface> v = n->GetValidValue();
        if (v != NULL) {
            AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(k, v);
            AutoPtr<IMapEntry> res = (IMapEntry*)p->Probe(EIID_IMapEntry);
            return res;
        }
    }
}

// Almost the same as getNearEntry, except for keys
AutoPtr<IInterface> CConcurrentSkipListMap::_SubMap::GetNearKey(
    /* [in] */ PInterface key,
    /* [in] */ Int32 rel)
{
    if (mIsDescending) { // adjust relation for direction
        if ((rel & sLT) == 0)
            rel |= sLT;
        else
            rel &= ~sLT;
    }
    if (TooLow(key)) {
        if ((rel & sLT) == 0) {
            AutoPtr<Node> n = LoNode();
            if (IsBeforeEnd(n))
                return n->mKey;
        }
        return NULL;
    }
    if (TooHigh(key)) {
        if ((rel & sLT) != 0) {
            AutoPtr<Node> n = HiNode();
            if (n != NULL) {
                AutoPtr<IInterface> last = n->mKey;
                if (InBounds(last))
                    return last;
            }
        }
        return NULL;
    }
    for (;;) {
        AutoPtr<Node> n = mM->FindNear(key, rel);
        if (n == NULL || !InBounds(n->mKey))
            return NULL;
        AutoPtr<IInterface> k = n->mKey;
        AutoPtr<IInterface> v = n->GetValidValue();
        if (v != NULL)
            return k;
    }
}

/* ----------------  Map API methods -------------- */

ECode CConcurrentSkipListMap::_SubMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (key == NULL) return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IInterface> k = key;
    Boolean b = FALSE;
    *result = InBounds(k) && (mM->ContainsKey(k, &b), b);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    if (key == NULL) return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IInterface> k = key;
    if (!InBounds(k)) {
        *value = NULL;
        return NOERROR;
    }
    else {
        return mM->Get(k, value);
    }
}

ECode CConcurrentSkipListMap::_SubMap::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** oldValue)
{
    VALIDATE_NOT_NULL(oldValue);
    CheckKeyBounds(key);
    return mM->Put(key, value, oldValue);
}

ECode CConcurrentSkipListMap::_SubMap::Remove(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IInterface> k = key;
    if (!InBounds(k)) {
        *value = NULL;
        return NOERROR;
    }
    else {
        return mM->Remove(k, value);
    }
}

ECode CConcurrentSkipListMap::_SubMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    Int64 count = 0;
    for (AutoPtr<Node> n = LoNode();
         IsBeforeEnd(n);
         n = n->mNext) {
        if (n->GetValidValue() != NULL)
            ++count;
    }
    *size = count >= Elastos::Core::Math::INT32_MAX_VALUE ? Elastos::Core::Math::INT32_MAX_VALUE : (Int32)count;
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = !IsBeforeEnd(LoNode());
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (value == NULL) {
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }
    for (AutoPtr<Node> n = LoNode();
         IsBeforeEnd(n);
         n = n->mNext) {
        AutoPtr<IInterface> v = n->GetValidValue();
        if (v != NULL && Object::Equals(value, v)) {
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::Clear()
{
    for (AutoPtr<Node> n = LoNode();
         IsBeforeEnd(n);
         n = n->mNext) {
        if (n->GetValidValue() != NULL) {
            AutoPtr<IInterface> pre;
            mM->Remove(n->mKey, (IInterface**)&pre);
        }
    }
    return NOERROR;
}

/* ----------------  ConcurrentMap API methods -------------- */

ECode CConcurrentSkipListMap::_SubMap::PutIfAbsent(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* out)
{
    VALIDATE_NOT_NULL(out);
    CheckKeyBounds(key);
    return mM->PutIfAbsent(key, value, out);
}

ECode CConcurrentSkipListMap::_SubMap::Remove(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IInterface> k = key;
    Boolean b = FALSE;
    *res = InBounds(k) && (mM->Remove(k, value, &b), b);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::Replace(
            /* [in] */ PInterface key,
            /* [in] */ PInterface oldValue,
            /* [in] */ PInterface newValue,
            /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    CheckKeyBounds(key);
    return mM->Replace(key, oldValue, newValue, res);
}

ECode CConcurrentSkipListMap::_SubMap::Replace(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* res)
{
    VALIDATE_NOT_NULL(res);
    CheckKeyBounds(key);
    return mM->Replace(key, value, res);
}

/* ----------------  SortedMap API methods -------------- */

ECode CConcurrentSkipListMap::_SubMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp);
    AutoPtr<IComparator> cmp;
    mM->Comparator((IComparator**)&cmp);
    if (mIsDescending) {
        return CCollections::_ReverseOrder(cmp, comp);
    }
    else {
        *comp = cmp;
        REFCOUNT_ADD(*comp);
        return NOERROR;
    }
}

AutoPtr<CConcurrentSkipListMap::_SubMap> CConcurrentSkipListMap::_SubMap::NewSubMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ PInterface toKey,
    /* [in] */ Boolean toInclusive)
{
    if (mIsDescending) { // flip senses
        AutoPtr<IInterface> tk = fromKey;
        fromKey = toKey;
        toKey = tk;
        Boolean ti = fromInclusive;
        fromInclusive = toInclusive;
        toInclusive = ti;
    }
    if (mLo != NULL) {
        if (fromKey == NULL) {
            fromKey = mLo;
            fromInclusive = mLoInclusive;
        }
        else {
            Int32 c = mM->Compare(fromKey, mLo);
            if (c < 0 || (c == 0 && !mLoInclusive && fromInclusive))
                return NULL;
        }
    }
    if (mHi != NULL) {
        if (toKey == NULL) {
            toKey = mHi;
            toInclusive = mHiInclusive;
        }
        else {
            Int32 c = mM->Compare(toKey, mHi);
            if (c > 0 || (c == 0 && !mHiInclusive && toInclusive))
                return NULL;
        }
    }
    AutoPtr<_SubMap> res = new _SubMap(mM, fromKey, fromInclusive,
                                    toKey, toInclusive, mIsDescending);
    return res;
}

ECode CConcurrentSkipListMap::_SubMap::SubMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ PInterface toKey,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    if (fromKey == NULL || toKey == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<_SubMap> p = NewSubMap(fromKey, fromInclusive, toKey, toInclusive);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::HeadMap(
    /* [in] */ PInterface toKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    if (toKey == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<_SubMap> p = NewSubMap(NULL, FALSE, toKey, inclusive);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::TailMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    if (fromKey == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<_SubMap> p = NewSubMap(fromKey, inclusive, NULL, FALSE);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::SubMap(
    /* [in] */ PInterface fromKey,
    /* [in] */ PInterface toKey,
    /* [out] */ ISortedMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> p;
    ECode ec = SubMap(fromKey, TRUE, toKey, FALSE, (INavigableMap**)&p);
    if (p != NULL) {
        *res = (ISortedMap*)p->Probe(EIID_ISortedMap);
        REFCOUNT_ADD(*res);
    }
    return ec;
}

ECode CConcurrentSkipListMap::_SubMap::HeadMap(
    /* [in] */ PInterface toKey,
    /* [out] */ ISortedMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> p;
    ECode ec = HeadMap(toKey, FALSE, (INavigableMap**)&p);
    if (p != NULL) {
        *res = (ISortedMap*)p->Probe(EIID_ISortedMap);
        REFCOUNT_ADD(*res);
    }
    return ec;
}

ECode CConcurrentSkipListMap::_SubMap::TailMap(
    /* [in] */ PInterface fromKey,
    /* [out] */ ISortedMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> p;
    ECode ec = TailMap(fromKey, TRUE, (INavigableMap**)&p);
    if (p != NULL) {
        *res = (ISortedMap*)p->Probe(EIID_ISortedMap);
        REFCOUNT_ADD(*res);
    }
    return ec;
}

ECode CConcurrentSkipListMap::_SubMap::DescendingMap(
    /* [out] */ INavigableMap** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<_SubMap> p = new _SubMap(mM, mLo, mLoInclusive,
                        mHi, mHiInclusive, !mIsDescending);
    *res = (INavigableMap*)p->Probe(EIID_INavigableMap);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

/* ----------------  Relational methods -------------- */

ECode CConcurrentSkipListMap::_SubMap::CeilingEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = GetNearEntry(key, sGT|sEQ);
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::CeilingKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IInterface> p = GetNearKey(key, sGT|sEQ);
    *outface = p.Get();
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::LowerEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = GetNearEntry(key, sLT);
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::LowerKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IInterface> p = GetNearKey(key, sLT);
    *outface = p.Get();
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::FloorEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = GetNearEntry(key, sLT|sEQ);
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::FloorKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IInterface> p = GetNearKey(key, sLT|sEQ);
    *outface = p.Get();
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::HigherEntry(
    /* [in] */ IInterface* key,
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = GetNearEntry(key, sGT);
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::HigherKey(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IInterface> p = GetNearKey(key, sGT);
    *outface = p.Get();
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IInterface> p = mIsDescending ? HighestKey() : LowestKey();
    *outface = p.Get();
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IInterface> p = mIsDescending ? LowestKey() : HighestKey();
    *outface = p.Get();
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::FirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = mIsDescending ? HighestEntry() : LowestEntry();
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::LastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = mIsDescending ? LowestEntry() : HighestEntry();
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::PollFirstEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = mIsDescending ? RemoveHighest() : RemoveLowest();
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::PollLastEntry(
    /* [out] */ IMapEntry** outent)
{
    VALIDATE_NOT_NULL(outent);
    AutoPtr<IMapEntry> p = mIsDescending ? RemoveLowest() : RemoveHighest();
    *outent = p.Get();
    REFCOUNT_ADD(*outent);
    return NOERROR;
}

/* ---------------- _SubMap Views -------------- */

ECode CConcurrentSkipListMap::_SubMap::KeySet(
    /* [out] */ ISet** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<_KeySet> ks = mKeySetView;
    *res = (ks != NULL) ? ks : (mKeySetView = new _KeySet(this));
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::NavigableKeySet(
    /* [out] */ INavigableSet** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<_KeySet> ks = mKeySetView;
    *res = (ks != NULL) ? ks : (mKeySetView = new _KeySet(this));
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<ICollection> vs = mValuesView;
    *value = (vs != NULL) ? vs : (mValuesView = new _Values(this));
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::EntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries);
    AutoPtr<ISet> es = mEntrySetView;
    *entries = (es != NULL) ? es : (mEntrySetView = new _EntrySet(this));
    REFCOUNT_ADD(*entries);
    return NOERROR;
}

ECode CConcurrentSkipListMap::_SubMap::DescendingKeySet(
    /* [out] */ INavigableSet** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableMap> m;
    DescendingMap((INavigableMap**)&m);
    return m->NavigableKeySet(res);
}

AutoPtr<IIterator> CConcurrentSkipListMap::_SubMap::KeyIterator()
{
    AutoPtr<SubMapKeyIterator> p = new SubMapKeyIterator(this);
    return (IIterator*)p->Probe(EIID_IIterator);
}

AutoPtr<IIterator> CConcurrentSkipListMap::_SubMap::ValueIterator()
{
    AutoPtr<SubMapValueIterator> p = new SubMapValueIterator(this);
    return (IIterator*)p->Probe(EIID_IIterator);
}

AutoPtr<IIterator> CConcurrentSkipListMap::_SubMap::EntryIterator()
{
    AutoPtr<SubMapEntryIterator> p = new SubMapEntryIterator(this);
    return (IIterator*)p->Probe(EIID_IIterator);
}

ECode CConcurrentSkipListMap::_SubMap::Equals(
    /* [in] */ IInterface* entry,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(entry, result);
}

ECode CConcurrentSkipListMap::_SubMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CConcurrentSkipListMap::_SubMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

//====================================================================
// CConcurrentSkipListMap::_SubMap::SubMapIter::
//====================================================================
CAR_INTERFACE_IMPL(CConcurrentSkipListMap::_SubMap::SubMapIter, IIterator);

CConcurrentSkipListMap::_SubMap::SubMapIter::SubMapIter(
    /* [in] */ _SubMap* owner)
{
    mOwner = owner;
    for (;;) {
        mNext = mOwner->mIsDescending ? mOwner->HiNode() : mOwner->LoNode();
        if (mNext == NULL)
            break;
        AutoPtr<IInterface> x = mNext->mValue;
        if (x != NULL && !Object::Equals(x, mNext)) {
            if (!mOwner->InBounds(mNext->mKey))
                mNext = NULL;
            else
                mNextValue = x;
            break;
        }
    }
}

ECode CConcurrentSkipListMap::_SubMap::SubMapIter::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNext != NULL;
    return NOERROR;
}

void CConcurrentSkipListMap::_SubMap::SubMapIter::Advance()
{
    if (mNext == NULL)
        return;
//        throw new NoSuchElementException();
    mLastReturned = mNext;
    if (mOwner->mIsDescending)
        Descend();
    else
        Ascend();
}

void CConcurrentSkipListMap::_SubMap::SubMapIter::Ascend()
{
    for (;;) {
        mNext = mNext->mNext;
        if (mNext == NULL)
            break;
        AutoPtr<IInterface> x = mNext->mValue;
        if (x != NULL && !Object::Equals(x, mNext)) {
            if (mOwner->TooHigh(mNext->mKey))
                mNext = NULL;
            else
                mNextValue = x;
            break;
        }
    }
}

void CConcurrentSkipListMap::_SubMap::SubMapIter::Descend()
{
    for (;;) {
        mNext = mOwner->mM->FindNear(mLastReturned->mKey, sLT);
        if (mNext == NULL)
            break;
        AutoPtr<IInterface> x = mNext->mValue;
        if (x != NULL && !Object::Equals(x, mNext)) {
            if (mOwner->TooLow(mNext->mKey))
                mNext = NULL;
            else
                mNextValue = x;
            break;
        }
    }
}

ECode CConcurrentSkipListMap::_SubMap::SubMapIter::Remove()
{
    AutoPtr<Node> l = mLastReturned;
    if (l == NULL)
        return E_ILLEGAL_STATE_EXCEPTION;
    AutoPtr<IInterface> val;
    mOwner->mM->Remove(l->mKey, (IInterface**)&val);
    mLastReturned = NULL;
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::_SubMap::SubMapValueIterator::
//====================================================================
CConcurrentSkipListMap::_SubMap::SubMapValueIterator::SubMapValueIterator(
    /* [in] */ _SubMap* owner) : SubMapIter(owner)
{
}

ECode CConcurrentSkipListMap::_SubMap::SubMapValueIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IInterface> v = mNextValue;
    Advance();
    *object = v;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::_SubMap::SubMapKeyIterator::
//====================================================================
CConcurrentSkipListMap::_SubMap::SubMapKeyIterator::SubMapKeyIterator(
    /* [in] */ _SubMap* owner) : SubMapIter(owner)
{
}

ECode CConcurrentSkipListMap::_SubMap::SubMapKeyIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<Node> n = mNext;
    Advance();
    *object = n->mKey;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

//====================================================================
// CConcurrentSkipListMap::_SubMap::SubMapEntryIterator::
//====================================================================
CConcurrentSkipListMap::_SubMap::SubMapEntryIterator::SubMapEntryIterator(
    /* [in] */ _SubMap* owner) : SubMapIter(owner)
{
}

ECode CConcurrentSkipListMap::_SubMap::SubMapEntryIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<Node> n = mNext;
    AutoPtr<IInterface> v = mNextValue;
    Advance();
    AutoPtr<SimpleImmutableEntry> p = new SimpleImmutableEntry(n->mKey, v);
    *object = p->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object);
    return NOERROR;
}

    // Unsafe mechanics
    // private static final sun.misc.Unsafe UNSAFE;
    // private static final long headOffset;
    // static {
    //     try {
    //         UNSAFE = sun.misc.Unsafe.getUnsafe();
    //         Class<?> k = ConcurrentSkipListMap.class;
    //         headOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("head"));
    //     } catch (Exception e) {
    //         throw new Error(e);
    //     }
    // }

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
