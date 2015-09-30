
#include "ext/frameworkext.h"
#include "CVisualizerMeasurementPeakRms.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

CVisualizerMeasurementPeakRms::CVisualizerMeasurementPeakRms()
    : mPeak(0)
    , mRms(0)
{}

CAR_INTERFACE_IMPL(CVisualizerMeasurementPeakRms, Object, IVisualizerMeasurementPeakRms)

CAR_OBJECT_IMPL(CVisualizerMeasurementPeakRms)


} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
