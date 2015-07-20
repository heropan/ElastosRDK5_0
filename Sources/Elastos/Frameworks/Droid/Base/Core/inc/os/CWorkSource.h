
#ifndef __CWORKSOURCE_H__
#define __CWORKSOURCE_H__

#include "_CWorkSource.h"

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Describes the source of some work that may be done by someone else.
 * Currently the public representation of what a work source is is not
 * defined; this is an opaque container.
 */
CarClass(CWorkSource)
{
public:
    CWorkSource();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWorkSource* orig);

    CARAPI constructor(
        /* [in] */ Int32 uid);

    CARAPI GetSize(
        /* [out] */ Int32* value);

    /** @hide */
    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    /**
     * Clear this WorkSource to be empty.
     */
    CARAPI Clear();

    CARAPI Equals(
        /* [in] */ IWorkSource* o,
        /* [out] */ Boolean* equal);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* equal);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    /**
     * Compare this WorkSource with another.
     * @param other The WorkSource to compare against.
     * @return If there is a difference, true is returned.
     */
    CARAPI Diff(
        /* [in] */ IWorkSource* other,
        /* [out] */ Boolean* difference);

    /**
     * Replace the current contents of this work source with the given
     * work source.  If <var>other</var> is null, the current work source
     * will be made empty.
     */
    CARAPI Set(
        /* [in] */ IWorkSource* other);

    /** @hide */
    CARAPI Set(
        /* [in] */ Int32 uid);

    /** @hide */
    CARAPI SetReturningDiffs(
        /* [in] */ IWorkSource* other,
        /* [out, callee] */ ArrayOf<IWorkSource*>** result);

    /**
     * Merge the contents of <var>other</var> WorkSource in to this one.
     *
     * @param other The other WorkSource whose contents are to be merged.
     * @return Returns true if any new sources were added.
     */
    CARAPI Add(
        /* [in] */ IWorkSource* other,
        /* [out] */ Boolean* added);

    /** @hide */
    CARAPI AddReturningNewbs(
        /* [in] */ IWorkSource* other,
        /* [out] */ IWorkSource** result);

    /** @hide */
    CARAPI Add(
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* added);

    /** @hide */
    CARAPI AddReturningNewbs(
        /* [in] */ Int32 uid,
        /* [out] */ IWorkSource** result);

    CARAPI Remove(
        /* [in] */ IWorkSource* other,
        /* [out] */ Boolean* successed);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    CARAPI_(Boolean) UpdateLocked(
        /* [in] */ IWorkSource* other,
        /* [in] */ Boolean set,
        /* [in] */ Boolean returnNewbs);

    CARAPI_(void) AddLocked(
        /* [in] */ Int32 uid);

public:
    Int32 mNum;
    AutoPtr< ArrayOf<Int32> > mUids;

    /**
     * Internal statics to avoid object allocations in some operations.
     * The WorkSource object itself is not thread safe, but we need to
     * hold sTmpWorkSource lock while working with these statics.
     */
    static const AutoPtr<IWorkSource> sTmpWorkSource;
    static Object sTmpWorkSourceLock;
    /**
     * For returning newbie work from a modification operation.
     */
    static AutoPtr<IWorkSource> sNewbWork;
    /**
     * For returning gone work form a modification operation.
     */
    static AutoPtr<IWorkSource> sGoneWork;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CWORKSOURCE_H__
