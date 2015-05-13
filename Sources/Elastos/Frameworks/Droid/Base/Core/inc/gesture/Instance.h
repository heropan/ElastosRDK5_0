#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Gesture::IGesture;

namespace Elastos {
namespace Droid {
namespace Gesture {
/**
 * An instance represents a sample if the label is available or a query if the
 * label is null.
 */
class Instance
    : public ElRefBase
{
public:
    /**
     * create a learning instance for a single stroke gesture
     *
     * @param gesture
     * @param label
     * @return the instance
     */
    static CARAPI_(AutoPtr<Instance>) CreateInstance(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ IGesture *gesture,
        /* [in] */ const String& label);

private:
    Instance(
        /* [in] */ Int64 id,
        /* [in] */ ArrayOf<Float> *sample,
        /* [in] */ const String& sampleName);

    CARAPI Normalize();

    static CARAPI SpatialSampler(
        /* [in] */ IGesture *gesture,
        /* [out, callee] */ ArrayOf<Float> **sampler);

    static CARAPI_(AutoPtr<ArrayOf<Float> >) TemporalSampler(
        /* [in] */ Int32 orientationType,
        /* [in] */ IGesture *gesture);

public:
    // the feature vector
    const AutoPtr<ArrayOf<Float> > mVector;

    // the label can be null
    const String mLabel;

    // the id of the instance
    const Int64 mId;

private:
    const static Int32 SEQUENCE_SAMPLE_SIZE = 16;

    const static Int32 PATCH_SAMPLE_SIZE = 16;

    const static AutoPtr<ArrayOf<Float> > ORIENTATIONS;


};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
#endif //__INSTANCE_H__
