
#ifndef __ELREFWRAPPER_H__
#define __ELREFWRAPPER_H__

#include <elastos.h>
#include <cutils/atomic.h>

using namespace Elastos;

#define LOG_ASSERT(cond, ...)

template<typename T> class WeakPtr;

// ---------------------------------------------------------------------------

#define COMPARE_WEAK(_op_)                                      \
inline Boolean operator _op_ (const StrongPtr<T>& o) const {              \
    return mPtr _op_ o.mPtr;                                  \
}                                                               \
inline Boolean operator _op_ (const T* o) const {                  \
    return mPtr _op_ o;                                        \
}                                                               \
template<typename U>                                            \
inline Boolean operator _op_ (const StrongPtr<U>& o) const {              \
    return mPtr _op_ o.mPtr;                                  \
}                                                               \
template<typename U>                                            \
inline Boolean operator _op_ (const U* o) const {                  \
    return mPtr _op_ o;                                        \
}

#define COMPARE(_op_)                                           \
COMPARE_WEAK(_op_)                                              \
inline Boolean operator _op_ (const WeakPtr<T>& o) const {              \
    return mPtr _op_ o.mPtr;                                  \
}                                                               \
template<typename U>                                            \
inline Boolean operator _op_ (const WeakPtr<U>& o) const {              \
    return mPtr _op_ o.mPtr;                                  \
}

class RefWrapper
{
public:
    class WeakRefType
    {
    public:
        RefWrapper* GetRefWrapper() const;

        void IncWeak(const void* id);
        void DecWeak(const void* id);

        Boolean AttemptIncStrong(const void* id);

        //! This is only safe if you have set OBJECT_LIFETIME_FOREVER.
        Boolean AttemptIncWeak(const void* id);

        //! DEBUGGING ONLY: Get current weak ref count.
        Int32 GetWeakCount() const;

        //! DEBUGGING ONLY: Print references held on object.
        void PrintRefs() const;

        //! DEBUGGING ONLY: Enable tracking for this object.
        // enable -- enable/disable tracking
        // retain -- when tracking is enable, if true, then we save a stack trace
        //           for each reference and dereference; when retain == false, we
        //           match up references and dereferences and keep only the
        //           outstanding ones.
        void TrackMe(Boolean enable, Boolean retain) const;
    };

    // used to override the RefWrapper destruction.
    class Destroyer
    {
        friend class RefWrapper;
    public:
        virtual ~Destroyer();
    private:
        virtual void Destroy(RefWrapper const* base) = 0;
    };

public:
    RefWrapper();
    RefWrapper(IInterface* obj);

    ~RefWrapper();

    void IncStrong(const void* id) const;
    void DecStrong(const void* id) const;

    void ForceIncStrong(const void* id) const;

    //! DEBUGGING ONLY: Get current strong ref count.
    Int32 GetStrongCount() const;

    WeakRefType* CreateWeak(const void* id) const;

    WeakRefType* GetWeakRefs() const;

    inline IInterface* Get() const { return mObj; }

    //! DEBUGGING ONLY: Print references held on object.
    inline void PrintRefs() const { GetWeakRefs()->PrintRefs(); }

    //! DEBUGGING ONLY: Enable tracking of object.
    inline void TrackMe(Boolean enable, Boolean retain)
    {
        GetWeakRefs()->TrackMe(enable, retain);
    }

    // Make sure to never acquire a strong reference from this function. The
    // same restrictions than for destructors apply.
    void SetDestroyer(Destroyer* destroyer);

protected:
    // RefWrapper();
    // virtual ~RefWrapper();

    //! Flags for extendObjectLifetime()
    enum {
        OBJECT_LIFETIME_WEAK    = 0x0001,
        OBJECT_LIFETIME_FOREVER = 0x0003
    };

    void ExtendObjectLifetime(Int32 mode);

    //! Flags for onIncStrongAttempted()
    enum {
        FIRST_INC_STRONG = 0x0001
    };

    virtual void OnFirstRef();
    virtual void OnLastStrongRef(const void* id);
    virtual Boolean OnIncStrongAttempted(uint32_t flags, const void* id);
    virtual void OnLastWeakRef(const void* id);

private:
    friend class WeakRefType;
    class WeakRefImpl;

    RefWrapper(const RefWrapper& o);
    RefWrapper& operator = (const RefWrapper& o);

    WeakRefImpl* const mRefs;
    IInterface* const mObj;
};

// ---------------------------------------------------------------------------

template <typename T>
class StrongPtr
{
public:
    typedef typename T::WeakRefType WeakRefType;

    inline StrongPtr() : mPtr(0) { }

    StrongPtr(T* other);
    StrongPtr(const StrongPtr<T>& other);
    template<typename U> StrongPtr(U* other);
    template<typename U> StrongPtr(const StrongPtr<U>& other);

    ~StrongPtr();

    // Assignment

    StrongPtr& operator = (T* other);
    StrongPtr& operator = (const StrongPtr<T>& other);

    template<typename U> StrongPtr& operator = (const StrongPtr<U>& other);
    template<typename U> StrongPtr& operator = (U* other);

    //! Special optimization for use by ProcessState (and nobody else).
    void ForceSet(T* other);

    // Reset

    void Clear();

    // Accessors

    inline T& operator* () const { return *mPtr; }
    inline T* operator-> () const { return mPtr;  }
    inline T* Get() const { return mPtr; }

    // Operators

    COMPARE(==)
    COMPARE(!=)
    COMPARE(>)
    COMPARE(<)
    COMPARE(<=)
    COMPARE(>=)

private:
    template<typename Y> friend class StrongPtr;
    template<typename Y> friend class WeakPtr;

    // Optimization for WeakPtr::Promote().
    StrongPtr(T* p, WeakRefType* refs);

    T* mPtr;
};

// ---------------------------------------------------------------------------

template <typename T>
class WeakPtr
{
public:
    typedef typename T::WeakRefType WeakRefType;

    inline WeakPtr() : mPtr(0) {}

    WeakPtr(T* other);
    WeakPtr(const WeakPtr<T>& other);
    WeakPtr(const StrongPtr<T>& other);
    template<typename U> WeakPtr(U* other);
    template<typename U> WeakPtr(const StrongPtr<U>& other);
    template<typename U> WeakPtr(const WeakPtr<U>& other);

    ~WeakPtr();

    // Assignment

    WeakPtr& operator = (T* other);
    WeakPtr& operator = (const WeakPtr<T>& other);
    WeakPtr& operator = (const StrongPtr<T>& other);

    template<typename U> WeakPtr& operator = (U* other);
    template<typename U> WeakPtr& operator = (const WeakPtr<U>& other);
    template<typename U> WeakPtr& operator = (const StrongPtr<U>& other);

    void SetObjectAndRefs(T* other, WeakRefType* refs);

    // promotion to StrongPtr

    StrongPtr<T> Promote() const;

    // Reset

    void Clear();

    // Accessors

    inline WeakRefType* GetRefs() const { return mRefs; }

    inline T* UnsafeGet() const { return mPtr; }

    // Operators

    COMPARE_WEAK(==)
    COMPARE_WEAK(!=)
    COMPARE_WEAK(>)
    COMPARE_WEAK(<)
    COMPARE_WEAK(<=)
    COMPARE_WEAK(>=)

    inline Boolean operator == (const WeakPtr<T>& o) const
    {
        return (mPtr == o.mPtr) && (mRefs == o.mRefs);
    }
    template<typename U>
    inline Boolean operator == (const WeakPtr<U>& o) const
    {
        return mPtr == o.mPtr;
    }

    inline Boolean operator > (const WeakPtr<T>& o) const
    {
        return (mPtr == o.mPtr) ? (mRefs > o.mRefs) : (mPtr > o.mPtr);
    }
    template<typename U>
    inline Boolean operator > (const WeakPtr<U>& o) const
    {
        return (mPtr == o.mPtr) ? (mRefs > o.mRefs) : (mPtr > o.mPtr);
    }

    inline Boolean operator < (const WeakPtr<T>& o) const
    {
        return (mPtr == o.mPtr) ? (mRefs < o.mRefs) : (mPtr < o.mPtr);
    }
    template<typename U>
    inline Boolean operator < (const WeakPtr<U>& o) const
    {
        return (mPtr == o.mPtr) ? (mRefs < o.mRefs) : (mPtr < o.mPtr);
    }
    inline Boolean operator != (const WeakPtr<T>& o) const { return mRefs != o.mRefs; }
    template<typename U>
    inline Boolean operator != (const WeakPtr<U>& o) const { return !operator == (o); }
    inline Boolean operator <= (const WeakPtr<T>& o) const { return !operator > (o); }
    template<typename U>
    inline Boolean operator <= (const WeakPtr<U>& o) const { return !operator > (o); }
    inline Boolean operator >= (const WeakPtr<T>& o) const { return !operator < (o); }
    template<typename U>
    inline Boolean operator >= (const WeakPtr<U>& o) const { return !operator < (o); }

private:
    template<typename Y> friend class StrongPtr;
    template<typename Y> friend class WeakPtr;

    T* mPtr;
    WeakRefType* mRefs;
};

#undef COMPARE
#undef COMPARE_WEAK

// ---------------------------------------------------------------------------
// No user serviceable parts below here.

// ---------------------------------------------------------------------------
#define INITIAL_STRONG_VALUE (1<<28)

RefWrapper::Destroyer::~Destroyer()
{}


// ---------------------------------------------------------------------------
class RefWrapper::WeakRefImpl : public RefWrapper::WeakRefType
{
public:
    volatile Int32    mStrong;
    volatile Int32    mWeak;
    RefWrapper* const   mBase;
    volatile Int32    mFlags;
    Destroyer*        mDestroyer;

#if !DEBUG_REFS

    WeakRefImpl(RefWrapper* base)
        : mStrong(INITIAL_STRONG_VALUE)
        , mWeak(0)
        , mBase(base)
        , mFlags(0)
        , mDestroyer(0)
    {}

    void AddStrongRef(const void* /*id*/) { }
    void RemoveStrongRef(const void* /*id*/) { }
    void AddWeakRef(const void* /*id*/) { }
    void RemoveWeakRef(const void* /*id*/) { }
    void PrintRefs() const { }
    void TrackMe(Boolean, Boolean) const { }

#else

    WeakRefImpl(RefWrapper* base)
        : mStrong(INITIAL_STRONG_VALUE)
        , mWeak(0)
        , mBase(base)
        , mFlags(0)
        , mStrongRefs(NULL)
        , mWeakRefs(NULL)
        , mTrackEnabled(!!DEBUG_REFS_ENABLED_BY_DEFAULT)
        , mRetain(FALSE)
    {
        //LOGI("NEW WeakRefImpl %p for RefWrapper %p", this, base);
    }

    ~WeakRefImpl()
    {
        LOG_ALWAYS_FATAL_IF(!mRetain && mStrongRefs != NULL, "Strong references remain!");
        LOG_ALWAYS_FATAL_IF(!mRetain && mWeakRefs != NULL, "Weak references remain!");
    }

    void AddStrongRef(const void* id)
    {
        AddRef(&mStrongRefs, id, mStrong);
    }

    void RemoveStrongRef(const void* id)
    {
        if (!mRetain) {
            RemoveRef(&mStrongRefs, id);
        }
        else {
            AddRef(&mStrongRefs, id, -mStrong);
        }
    }

    void AddWeakRef(const void* id)
    {
        AddRef(&mWeakRefs, id, mWeak);
    }

    void RemoveWeakRef(const void* id)
    {
        if (!mRetain) {
            RemoveRef(&mWeakRefs, id);
        }
        else {
            AddRef(&mWeakRefs, id, -mWeak);
        }
    }

    void TrackMe(Boolean track, Boolean retain) const
    {
        mTrackEnabled = track;
        mRetain = retain;
    }

    void PrintRefs() const
    {
        String text;

        {
            AutoMutex _l(const_cast<WeakRefImpl*>(this)->mMutex);

            char buf[128];
            sprintf(buf, "Strong references on RefWrapper %p (WeakRefType %p):\n", mBase, this);
            text.append(buf);
            PrintRefsLocked(&text, mStrongRefs);
            sprintf(buf, "Weak references on RefWrapper %p (WeakRefType %p):\n", mBase, this);
            text.append(buf);
            PrintRefsLocked(&text, mWeakRefs);
        }

        {
            char name[100];
            snprintf(name, 100, "/data/%p.stack", this);
            int rc = open(name, O_RDWR | O_CREAT | O_APPEND);
            if (rc >= 0) {
                write(rc, text.string(), text.length());
                close(rc);
                LOGD("STACK TRACE for %p saved in %s", this, name);
            }
            else LOGE("FAILED TO PRINT STACK TRACE for %p in %s: %s", this,
                      name, strerror(errno));
        }
    }

private:
    struct RefEntry
    {
        RefEntry* next;
        const void* id;
#if DEBUG_REFS_CALLSTACK_ENABLED
        CallStack stack;
#endif
        Int32 ref;
    };

    void AddRef(RefEntry** refs, const void* id, Int32 mRef)
    {
        if (mTrackEnabled) {
            AutoMutex _l(mMutex);
            RefEntry* ref = new RefEntry;
            // Reference count at the time of the snapshot, but before the
            // update.  Positive value means we increment, negative--we
            // decrement the reference count.
            ref->ref = mRef;
            ref->id = id;
#if DEBUG_REFS_CALLSTACK_ENABLED
            ref->stack.update(2);
#endif

            ref->next = *refs;
            *refs = ref;
        }
    }

    void RemoveRef(RefEntry** refs, const void* id)
    {
        if (mTrackEnabled) {
            AutoMutex _l(mMutex);

            RefEntry* ref = *refs;
            while (ref != NULL) {
                if (ref->id == id) {
                    *refs = ref->next;
                    delete ref;
                    return;
                }

                refs = &ref->next;
                ref = *refs;
            }

            LOG_ALWAYS_FATAL("RefWrapper: removing id %p on RefWrapper %p (WeakRefType %p) that doesn't exist!",
                             id, mBase, this);
        }
    }

    void PrintRefsLocked(String* out, const RefEntry* refs) const
    {
        char buf[128];
        while (refs) {
            char inc = refs->ref >= 0 ? '+' : '-';
            sprintf(buf, "\t%c ID %p (ref %d):\n",
                    inc, refs->id, refs->ref);
            out->append(buf);
#if DEBUG_REFS_CALLSTACK_ENABLED
            out->append(refs->stack.toString("\t\t"));
#else
            out->append("\t\t(call stacks disabled)");
#endif
            refs = refs->next;
        }
    }

    Mutex mMutex;
    RefEntry* mStrongRefs;
    RefEntry* mWeakRefs;

    Boolean mTrackEnabled;
    // Collect stack traces on addref and removeref, instead of deleting the stack references
    // on removeref that match the address ones.
    Boolean mRetain;

#if 0
    void addRef(KeyedVector<const void*, int32_t>* refs, const void* id)
    {
        AutoMutex _l(mMutex);
        ssize_t i = refs->indexOfKey(id);
        if (i >= 0) {
            ++(refs->editValueAt(i));
        } else {
            i = refs->add(id, 1);
        }
    }

    void removeRef(KeyedVector<const void*, int32_t>* refs, const void* id)
    {
        AutoMutex _l(mMutex);
        ssize_t i = refs->indexOfKey(id);
        LOG_ALWAYS_FATAL_IF(i < 0, "RefWrapper: removing id %p that doesn't exist!", id);
        if (i >= 0) {
            int32_t val = --(refs->editValueAt(i));
            if (val == 0) {
                refs->removeItemsAt(i);
            }
        }
    }

    void printRefs(const KeyedVector<const void*, int32_t>& refs)
    {
        const size_t N=refs.size();
        for (size_t i=0; i<N; i++) {
            printf("\tID %p: %d remain\n", refs.keyAt(i), refs.valueAt(i));
        }
    }

    mutable Mutex mMutex;
    KeyedVector<const void*, int32_t> mStrongRefs;
    KeyedVector<const void*, int32_t> mWeakRefs;
#endif

#endif
};

// ---------------------------------------------------------------------------

void RefWrapper::IncStrong(const void* id) const
{
    WeakRefImpl* const refs = mRefs;
    refs->AddWeakRef(id);
    refs->IncWeak(id);

    refs->AddStrongRef(id);
    const Int32 c = android_atomic_inc(&refs->mStrong);
    LOG_ASSERT(c > 0, "incStrong() called on %p after last strong ref", refs);
#if PRINT_REFS
    LOGD("incStrong of %p from %p: cnt=%d\n", this, id, c);
#endif
    if (c != INITIAL_STRONG_VALUE)  {
        return;
    }

    android_atomic_add(-INITIAL_STRONG_VALUE, &refs->mStrong);
    const_cast<RefWrapper*>(this)->OnFirstRef();
}

void RefWrapper::DecStrong(const void* id) const
{
    WeakRefImpl* const refs = mRefs;
    refs->RemoveStrongRef(id);
    const Int32 c = android_atomic_dec(&refs->mStrong);
#if PRINT_REFS
    LOGD("decStrong of %p from %p: cnt=%d\n", this, id, c);
#endif
    LOG_ASSERT(c >= 1, "decStrong() called on %p too many times", refs);
    if (c == 1) {
        const_cast<RefWrapper*>(this)->OnLastStrongRef(id);
        if ((refs->mFlags & OBJECT_LIFETIME_WEAK) != OBJECT_LIFETIME_WEAK) {
            if (refs->mDestroyer) {
                refs->mDestroyer->Destroy(this);
            }
            else {
                delete this;
            }
        }
    }
    refs->RemoveWeakRef(id);
    refs->DecWeak(id);
}

void RefWrapper::ForceIncStrong(const void* id) const
{
    WeakRefImpl* const refs = mRefs;
    refs->AddWeakRef(id);
    refs->IncWeak(id);

    refs->AddStrongRef(id);
    const Int32 c = android_atomic_inc(&refs->mStrong);
    LOG_ASSERT(c >= 0, "forceIncStrong called on %p after ref count underflow",
               refs);
#if PRINT_REFS
    LOGD("forceIncStrong of %p from %p: cnt=%d\n", this, id, c);
#endif

    switch (c) {
    case INITIAL_STRONG_VALUE:
        android_atomic_add(-INITIAL_STRONG_VALUE, &refs->mStrong);
        // fall through...
    case 0:
        const_cast<RefWrapper*>(this)->OnFirstRef();
    }
}

Int32 RefWrapper::GetStrongCount() const
{
    return mRefs->mStrong;
}

void RefWrapper::SetDestroyer(RefWrapper::Destroyer* destroyer) {
    mRefs->mDestroyer = destroyer;
}

RefWrapper* RefWrapper::WeakRefType::GetRefWrapper() const
{
    return static_cast<const WeakRefImpl*>(this)->mBase;
}

void RefWrapper::WeakRefType::IncWeak(const void* id)
{
    WeakRefImpl* const impl = static_cast<WeakRefImpl*>(this);
    impl->AddWeakRef(id);
    const Int32 c = android_atomic_inc(&impl->mWeak);
    LOG_ASSERT(c >= 0, "incWeak called on %p after last weak ref", this);
}

void RefWrapper::WeakRefType::DecWeak(const void* id)
{
    WeakRefImpl* const impl = static_cast<WeakRefImpl*>(this);
    impl->RemoveWeakRef(id);
    const Int32 c = android_atomic_dec(&impl->mWeak);
    LOG_ASSERT(c >= 1, "decWeak called on %p too many times", this);
    if (c != 1) return;

    if ((impl->mFlags & OBJECT_LIFETIME_WEAK) != OBJECT_LIFETIME_WEAK) {
        if (impl->mStrong == INITIAL_STRONG_VALUE) {
            if (impl->mBase) {
                if (impl->mDestroyer) {
                    impl->mDestroyer->Destroy(impl->mBase);
                }
                else {
                    delete impl->mBase;
                }
            }
        }
        else {
            // LOGV("Freeing refs %p of old RefWrapper %p\n", this, impl->mBase);
            delete impl;
        }
    }
    else {
        impl->mBase->OnLastWeakRef(id);
        if ((impl->mFlags & OBJECT_LIFETIME_FOREVER) != OBJECT_LIFETIME_FOREVER) {
            if (impl->mBase) {
                if (impl->mDestroyer) {
                    impl->mDestroyer->Destroy(impl->mBase);
                }
                else {
                    delete impl->mBase;
                }
            }
        }
    }
}

Boolean RefWrapper::WeakRefType::AttemptIncStrong(const void* id)
{
    IncWeak(id);

    WeakRefImpl* const impl = static_cast<WeakRefImpl*>(this);

    Int32 curCount = impl->mStrong;
    LOG_ASSERT(curCount >= 0, "attemptIncStrong called on %p after underflow",
               this);
    while (curCount > 0 && curCount != INITIAL_STRONG_VALUE) {
        if (android_atomic_cmpxchg(curCount, curCount + 1, &impl->mStrong) == 0) {
            break;
        }
        curCount = impl->mStrong;
    }

    if (curCount <= 0 || curCount == INITIAL_STRONG_VALUE) {
        Boolean allow;
        if (curCount == INITIAL_STRONG_VALUE) {
            // Attempting to acquire first strong reference...  this is allowed
            // if the object does NOT have a longer lifetime (meaning the
            // implementation doesn't need to see this), or if the implementation
            // allows it to happen.
            allow = (impl->mFlags & OBJECT_LIFETIME_WEAK) != OBJECT_LIFETIME_WEAK
                  || impl->mBase->OnIncStrongAttempted(FIRST_INC_STRONG, id);
        }
        else {
            // Attempting to revive the object...  this is allowed
            // if the object DOES have a longer lifetime (so we can safely
            // call the object with only a weak ref) and the implementation
            // allows it to happen.
            allow = (impl->mFlags & OBJECT_LIFETIME_WEAK) == OBJECT_LIFETIME_WEAK
                  && impl->mBase->OnIncStrongAttempted(FIRST_INC_STRONG, id);
        }
        if (!allow) {
            DecWeak(id);
            return FALSE;
        }
        curCount = android_atomic_inc(&impl->mStrong);

        // If the strong reference count has already been incremented by
        // someone else, the implementor of onIncStrongAttempted() is holding
        // an unneeded reference.  So call onLastStrongRef() here to remove it.
        // (No, this is not pretty.)  Note that we MUST NOT do this if we
        // are in fact acquiring the first reference.
        if (curCount > 0 && curCount < INITIAL_STRONG_VALUE) {
            impl->mBase->OnLastStrongRef(id);
        }
    }

    impl->AddWeakRef(id);
    impl->AddStrongRef(id);

#if PRINT_REFS
    LOGD("attemptIncStrong of %p from %p: cnt=%d\n", this, id, curCount);
#endif

    if (curCount == INITIAL_STRONG_VALUE) {
        android_atomic_add(-INITIAL_STRONG_VALUE, &impl->mStrong);
        impl->mBase->OnFirstRef();
    }

    return TRUE;
}

Boolean RefWrapper::WeakRefType::AttemptIncWeak(const void* id)
{
    WeakRefImpl* const impl = static_cast<WeakRefImpl*>(this);

    Int32 curCount = impl->mWeak;
    LOG_ASSERT(curCount >= 0, "attemptIncWeak called on %p after underflow",
               this);
    while (curCount > 0) {
        if (android_atomic_cmpxchg(curCount, curCount+1, &impl->mWeak) == 0) {
            break;
        }
        curCount = impl->mWeak;
    }

    if (curCount > 0) {
        impl->AddWeakRef(id);
    }

    return curCount > 0;
}

Int32 RefWrapper::WeakRefType::GetWeakCount() const
{
    return static_cast<const WeakRefImpl*>(this)->mWeak;
}

void RefWrapper::WeakRefType::PrintRefs() const
{
    static_cast<const WeakRefImpl*>(this)->PrintRefs();
}

void RefWrapper::WeakRefType::TrackMe(Boolean enable, Boolean retain) const
{
    static_cast<const WeakRefImpl*>(this)->TrackMe(enable, retain);
}

typename RefWrapper::WeakRefType* RefWrapper::CreateWeak(const void* id) const
{
    mRefs->IncWeak(id);
    return mRefs;
}

typename RefWrapper::WeakRefType* RefWrapper::GetWeakRefs() const
{
    return mRefs;
}

RefWrapper::RefWrapper()
    : mRefs(new WeakRefImpl(this))
    , mObj(NULL)
{
//    LOGV("Creating refs %p with RefWrapper %p\n", mRefs, this);
}

RefWrapper::RefWrapper(IInterface* obj)
    : mRefs(new WeakRefImpl(this))
    , mObj(obj)
{
//    LOGV("Creating refs %p with RefWrapper %p\n", mRefs, this);
    if (mObj) mObj->AddRef();
}

RefWrapper::~RefWrapper()
{
    if ((mRefs->mFlags & OBJECT_LIFETIME_WEAK) == OBJECT_LIFETIME_WEAK) {
        if (mRefs->mWeak == 0) {
            delete mRefs;
        }
    }
    if (mObj) mObj->Release();
}

void RefWrapper::ExtendObjectLifetime(Int32 mode)
{
    android_atomic_or(mode, &mRefs->mFlags);
}

void RefWrapper::OnFirstRef()
{}

void RefWrapper::OnLastStrongRef(const void* /*id*/)
{}

Boolean RefWrapper::OnIncStrongAttempted(uint32_t flags, const void* id)
{
    return (flags&FIRST_INC_STRONG) ? TRUE : FALSE;
}

void RefWrapper::OnLastWeakRef(const void* /*id*/)
{}


// ---------------------------------------------------------------------------
// StrongPtr
template<typename T>
StrongPtr<T>::StrongPtr(T* other)
    : mPtr(other)
{
    if (other) other->IncStrong(this);
}

template<typename T>
StrongPtr<T>::StrongPtr(const StrongPtr<T>& other)
    : mPtr(other.mPtr)
{
    if (mPtr) mPtr->IncStrong(this);
}

template<typename T> template<typename U>
StrongPtr<T>::StrongPtr(U* other) : mPtr(other)
{
    if (other) other->IncStrong(this);
}

template<typename T> template<typename U>
StrongPtr<T>::StrongPtr(const StrongPtr<U>& other)
    : mPtr(other.mPtr)
{
    if (mPtr) mPtr->IncStrong(this);
}

template<typename T>
StrongPtr<T>::~StrongPtr()
{
    if (mPtr) mPtr->DecStrong(this);
}

template<typename T>
StrongPtr<T>& StrongPtr<T>::operator = (const StrongPtr<T>& other)
{
    T* otherPtr(other.mPtr);
    if (otherPtr) otherPtr->IncStrong(this);
    if (mPtr) mPtr->DecStrong(this);
    mPtr = otherPtr;
    return *this;
}

template<typename T>
StrongPtr<T>& StrongPtr<T>::operator = (T* other)
{
    if (other) other->IncStrong(this);
    if (mPtr) mPtr->DecStrong(this);
    mPtr = other;
    return *this;
}

template<typename T> template<typename U>
StrongPtr<T>& StrongPtr<T>::operator = (const StrongPtr<U>& other)
{
    U* otherPtr(other.mPtr);
    if (otherPtr) otherPtr->IncStrong(this);
    if (mPtr) mPtr->DecStrong(this);
    mPtr = otherPtr;
    return *this;
}

template<typename T> template<typename U>
StrongPtr<T>& StrongPtr<T>::operator = (U* other)
{
    if (other) other->IncStrong(this);
    if (mPtr) mPtr->DecStrong(this);
    mPtr = other;
    return *this;
}

template<typename T>
void StrongPtr<T>::ForceSet(T* other)
{
    other->ForceIncStrong(this);
    mPtr = other;
}

template<typename T>
void StrongPtr<T>::Clear()
{
    if (mPtr) {
        mPtr->DecStrong(this);
        mPtr = 0;
    }
}

template<typename T>
StrongPtr<T>::StrongPtr(T* p, WeakRefType* refs)
    : mPtr((p && refs->AttemptIncStrong(this)) ? p : 0)
{}

// ---------------------------------------------------------------------------

template<typename T>
WeakPtr<T>::WeakPtr(T* other)
    : mPtr(other)
{
    if (other) mRefs = other->CreateWeak(this);
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
    : mPtr(other.mPtr)
    , mRefs(other.mRefs)
{
    if (mPtr) mRefs->IncWeak(this);
}

template<typename T>
WeakPtr<T>::WeakPtr(const StrongPtr<T>& other)
    : mPtr(other.mPtr)
{
    if (mPtr) {
        mRefs = mPtr->CreateWeak(this);
    }
}

template<typename T> template<typename U>
WeakPtr<T>::WeakPtr(U* other)
    : mPtr(other)
{
    if (other) mRefs = other->CreateWeak(this);
}

template<typename T> template<typename U>
WeakPtr<T>::WeakPtr(const WeakPtr<U>& other)
    : mPtr(other.mPtr)
{
    if (mPtr) {
        mRefs = other.mRefs;
        mRefs->IncWeak(this);
    }
}

template<typename T> template<typename U>
WeakPtr<T>::WeakPtr(const StrongPtr<U>& other)
    : mPtr(other.mPtr)
{
    if (mPtr) {
        mRefs = mPtr->CreateWeak(this);
    }
}

template<typename T>
WeakPtr<T>::~WeakPtr()
{
    if (mPtr) mRefs->DecWeak(this);
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator = (T* other)
{
    WeakRefType* newRefs =
        other ? other->CreateWeak(this) : 0;
    if (mPtr) mRefs->DecWeak(this);
    mPtr = other;
    mRefs = newRefs;
    return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator = (const WeakPtr<T>& other)
{
    WeakRefType* otherRefs(other.mRefs);
    T* otherPtr(other.mPtr);
    if (otherPtr) otherRefs->IncWeak(this);
    if (mPtr) mRefs->DecWeak(this);
    mPtr = otherPtr;
    mRefs = otherRefs;
    return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator = (const StrongPtr<T>& other)
{
    WeakRefType* newRefs =
        other != NULL ? other->CreateWeak(this) : 0;
    T* otherPtr(other.mPtr);
    if (mPtr) mRefs->DecWeak(this);
    mPtr = otherPtr;
    mRefs = newRefs;
    return *this;
}

template<typename T> template<typename U>
WeakPtr<T>& WeakPtr<T>::operator = (U* other)
{
    WeakRefType* newRefs =
        other ? other->CreateWeak(this) : 0;
    if (mPtr) mRefs->DecWeak(this);
    mPtr = other;
    mRefs = newRefs;
    return *this;
}

template<typename T> template<typename U>
WeakPtr<T>& WeakPtr<T>::operator = (const WeakPtr<U>& other)
{
    WeakRefType* otherRefs(other.mRefs);
    U* otherPtr(other.mPtr);
    if (otherPtr) otherRefs->IncWeak(this);
    if (mPtr) mRefs->DecWeak(this);
    mPtr = otherPtr;
    mRefs = otherRefs;
    return *this;
}

template<typename T> template<typename U>
WeakPtr<T>& WeakPtr<T>::operator = (const StrongPtr<U>& other)
{
    WeakRefType* newRefs =
        other != NULL ? other->CreateWeak(this) : 0;
    U* otherPtr(other.mPtr);
    if (mPtr) mRefs->DecWeak(this);
    mPtr = otherPtr;
    mRefs = newRefs;
    return *this;
}

template<typename T>
void WeakPtr<T>::SetObjectAndRefs(T* other, WeakRefType* refs)
{
    if (other) refs->IncWeak(this);
    if (mPtr) mRefs->DecWeak(this);
    mPtr = other;
    mRefs = refs;
}

template<typename T>
StrongPtr<T> WeakPtr<T>::Promote() const
{
    return StrongPtr<T>(mPtr, mRefs);
}

template<typename T>
void WeakPtr<T>::Clear()
{
    if (mPtr) {
        mRefs->DecWeak(this);
        mPtr = 0;
    }
}

#endif //__ELREFWRAPPER_H__
