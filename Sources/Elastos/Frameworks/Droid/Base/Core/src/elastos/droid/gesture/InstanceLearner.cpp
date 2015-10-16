
#include "elastos/droid/gesture/CPrediction.h"
#include "elastos/droid/gesture/GestureUtils.h"
#include "elastos/droid/gesture/InstanceLearner.h"
#include <elastos/Map.h>
#include <elastos/core/Math.h>

using Elastos::Core::EIID_IComparator;
using Elastos::Utility::Map;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CCollections;
using Elastos::Utility::ICollections;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Gesture {

CAR_INTERFACE_IMPL(InstanceLearner::PredictionComparator, IComparator)

ECode InstanceLearner::PredictionComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    Double score1;
    IPrediction::Probe(object1)->GetScore(&score1);
    Double score2;
    IPrediction::Probe(object2)->GetScore(&score2);
    if (score1 > score2) {
        *result = -1;
    } else if (score1 < score2) {
        *result = 1;
    } else {
        *result = 0;
    }
    return NOERROR;
}

InstanceLearner::InstanceLearner()
{
    mComparator = new PredictionComparator();
}

AutoPtr<IArrayList> InstanceLearner::Classify(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ ArrayOf<Float> *vec)
{
    AutoPtr<IArrayList> predictions;
    CArrayList::New((IArrayList**)&predictions);

    AutoPtr<List<AutoPtr<Instance> > > instances = GetInstances();

    Map<String, Double> label2score;

    List<AutoPtr<Instance> >::Iterator it;
    for (it = instances->Begin(); it != instances->End(); ++it) {
        AutoPtr<Instance> sample = *it;
        if (sample->mVector->GetLength() != vec->GetLength()) {
            continue;
        }
        Double distance;
        if (sequenceType == IGestureStore::SEQUENCE_SENSITIVE) {
            distance = GestureUtils::MinimumCosineDistance(sample->mVector, vec, orientationType);
        } else {
            distance = GestureUtils::SquaredEuclideanDistance(sample->mVector, vec);
        }
        Double weight;
        if (distance == 0) {
            weight = Elastos::Core::Math::DOUBLE_MAX_VALUE;
        } else {
            weight = 1 / distance;
        }
        Double score = NULL;
        Map<String, Double>::Iterator mapit = label2score.Find(sample->mLabel);
        if (mapit != label2score.End()) score = mapit->mSecond;
        if (score == NULL || weight > score) {
            label2score[sample->mLabel] = weight;
        }
    }

    Map<String, Double>::Iterator iter;
    for (iter = label2score.Begin(); iter != label2score.End(); ++iter) {
        String name = iter->mFirst;
        Double score = iter->mSecond;
        AutoPtr<IPrediction> prediction;
        CPrediction::New(name, score, (IPrediction**)&prediction);
        Boolean result;
        predictions->Add((IPrediction*)prediction, &result);
    }

    AutoPtr<ICollections> collections;
    CCollections::AcquireSingleton((ICollections**)&collections);
    collections->Sort(IList::Probe(predictions), mComparator);

    return predictions;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
