
#include "elastos/droid/internal/widget/multiwaveview/Ease.h"

#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {
namespace Multiwaveview {

const Float Ease::DOMAIN = 1.0f;
const Float Ease::DURATION = 1.0f;
const Float Ease::START = 0.0f;

//=====================================================================
//               Ease::Linear::TimeInterpolator::
//=====================================================================
Float Ease::Linear::TimeInterpolator::GetInterpolation(
    /* [in] */ Float input)
{
    return input;
}

//=====================================================================
//               Ease::Linear::
//=====================================================================
AutoPtr<Ease::Linear::TimeInterpolator> Ease::Linear::mEaseNone =
    new Ease::Linear::TimeInterpolator();

//=====================================================================
//               Ease::Cubic::TimeInterpolatorIn::
//=====================================================================
Float Ease::Cubic::TimeInterpolatorIn::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION;
    return DOMAIN * input * input * input + START;
}

//=====================================================================
//               Ease::Cubic::TimeInterpolatorOut::
//=====================================================================
Float Ease::Cubic::TimeInterpolatorOut::GetInterpolation(
    /* [in] */ Float input)
{
    input = input / DURATION - 1;
    return DOMAIN * (input * input * input + 1) + START;
}

//=====================================================================
//               Ease::Cubic::TimeInterpolatorInOut::
//=====================================================================
Float Ease::Cubic::TimeInterpolatorInOut::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION / 2;
    if (input < 1.0f) {
        return DOMAIN / 2 * input * input * input + START;
    }
    else {
        input -= 2;
        return DOMAIN / 2 * (input * input * input + 2) + START;
    }
}

//=====================================================================
//               Ease::Cubic::
//=====================================================================
AutoPtr<Ease::Cubic::TimeInterpolatorIn> Ease::Cubic::mEaseIn =
    new Ease::Cubic::TimeInterpolatorIn();

AutoPtr<Ease::Cubic::TimeInterpolatorOut> Ease::Cubic::mEaseOut =
    new Ease::Cubic::TimeInterpolatorOut();

AutoPtr<Ease::Cubic::TimeInterpolatorInOut> Ease::Cubic::mEaseInOut =
    new Ease::Cubic::TimeInterpolatorInOut();

//=====================================================================
//               Ease::Quad::TimeInterpolatorIn::
//=====================================================================
Float Ease::Quad::TimeInterpolatorIn::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION;
    return DOMAIN * input * input + START;
}

//=====================================================================
//               Ease::Quad::TimeInterpolatorOut::
//=====================================================================
Float Ease::Quad::TimeInterpolatorOut::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION;
    return -DOMAIN * input * (input - 2) + START;
}

//=====================================================================
//               Ease::Quad::TimeInterpolatorInOut::
//=====================================================================
Float Ease::Quad::TimeInterpolatorInOut::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION / 2;
    if (input < 1) {
        return DOMAIN / 2 * input * input + START;
    }
    else {
        Float x = input - 2;
        Float y = --input;
        return -DOMAIN / 2 * (y * x - 1) + START;
    }
}

//=====================================================================
//               Ease::Quad::
//=====================================================================
AutoPtr<Ease::Quad::TimeInterpolatorIn> Ease::Quad::mEaseIn =
    new Ease::Quad::TimeInterpolatorIn();

AutoPtr<Ease::Quad::TimeInterpolatorOut> Ease::Quad::mEaseOut =
    new Ease::Quad::TimeInterpolatorOut();

AutoPtr<Ease::Quad::TimeInterpolatorInOut> Ease::Quad::mEaseInOut =
    new Ease::Quad::TimeInterpolatorInOut();

//=====================================================================
//               Ease::Quart::TimeInterpolatorIn::
//=====================================================================
Float Ease::Quart::TimeInterpolatorIn::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION;
    return DOMAIN * input * input * input * input + START;
}

//=====================================================================
//               Ease::Quart::TimeInterpolatorOut::
//=====================================================================
Float Ease::Quart::TimeInterpolatorOut::GetInterpolation(
    /* [in] */ Float input)
{
    input = input / DURATION - 1;
    return -DOMAIN * (input * input * input * input - 1) + START;
}

//=====================================================================
//               Ease::Quart::TimeInterpolatorInOut::
//=====================================================================
Float Ease::Quart::TimeInterpolatorInOut::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION / 2;
    if (input < 1) {
        return DOMAIN / 2 * input * input * input * input + START;
    }
    else {
        input -= 2;
        return -DOMAIN / 2 * (input * input * input * input - 2) + START;
    }
}

//=====================================================================
//               Ease::Quart::
//=====================================================================
AutoPtr<Ease::Quart::TimeInterpolatorIn> Ease::Quart::mEaseIn =
    new Ease::Quart::TimeInterpolatorIn();

AutoPtr<Ease::Quart::TimeInterpolatorOut> Ease::Quart::mEaseOut =
    new Ease::Quart::TimeInterpolatorOut();

AutoPtr<Ease::Quart::TimeInterpolatorInOut> Ease::Quart::mEaseInOut =
    new Ease::Quart::TimeInterpolatorInOut();

//=====================================================================
//               Ease::Quint::TimeInterpolatorIn::
//=====================================================================
Float Ease::Quint::TimeInterpolatorIn::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION;
    return DOMAIN * input * input * input * input * input + START;
}

//=====================================================================
//               Ease::Quint::TimeInterpolatorOut::
//=====================================================================
Float Ease::Quint::TimeInterpolatorOut::GetInterpolation(
    /* [in] */ Float input)
{
    input = input / DURATION - 1;
    return DOMAIN * (input * input * input * input * input + 1) + START;
}

//=====================================================================
//               Ease::Quint::TimeInterpolatorInOut::
//=====================================================================
Float Ease::Quint::TimeInterpolatorInOut::GetInterpolation(
    /* [in] */ Float input)
{
    input /= DURATION / 2;
    if (input < 1) {
        return DOMAIN / 2 * input * input * input * input * input + START;
    }
    else {
        input -= 2;
        return DOMAIN / 2 * (input * input * input * input * input + 2) + START;
    }
}

//=====================================================================
//               Ease::Quint::
//=====================================================================
AutoPtr<Ease::Quint::TimeInterpolatorIn> Ease::Quint::mEaseIn =
    new Ease::Quint::TimeInterpolatorIn();

AutoPtr<Ease::Quint::TimeInterpolatorOut> Ease::Quint::mEaseOut =
    new Ease::Quint::TimeInterpolatorOut();

AutoPtr<Ease::Quint::TimeInterpolatorInOut> Ease::Quint::mEaseInOut =
    new Ease::Quint::TimeInterpolatorInOut();

//=====================================================================
//               Ease::Sine::TimeInterpolatorIn::
//=====================================================================
Float Ease::Sine::TimeInterpolatorIn::GetInterpolation(
    /* [in] */ Float input)
{
    return -DOMAIN * (Float) Elastos::Core::Math::Cos(input / DURATION * (Elastos::Core::Math::PI / 2)) + DOMAIN + START;
}

//=====================================================================
//               Ease::Sine::TimeInterpolatorOut::
//=====================================================================
Float Ease::Sine::TimeInterpolatorOut::GetInterpolation(
    /* [in] */ Float input)
{
    return DOMAIN * (Float) Elastos::Core::Math::Sin(input / DURATION * (Elastos::Core::Math::PI / 2)) + START;
}

//=====================================================================
//               Ease::Sine::TimeInterpolatorInOut::
//=====================================================================
Float Ease::Sine::TimeInterpolatorInOut::GetInterpolation(
    /* [in] */ Float input)
{
    return -DOMAIN / 2 * ((Float)Elastos::Core::Math::Cos(Elastos::Core::Math::PI * input / DURATION) - 1.0f) + START;
}

//=====================================================================
//               Ease::Sine::
//=====================================================================
AutoPtr<Ease::Sine::TimeInterpolatorIn> Ease::Sine::mEaseIn =
    new Ease::Sine::TimeInterpolatorIn();

AutoPtr<Ease::Sine::TimeInterpolatorOut> Ease::Sine::mEaseOut =
    new Ease::Sine::TimeInterpolatorOut();

AutoPtr<Ease::Sine::TimeInterpolatorInOut> Ease::Sine::mEaseInOut =
    new Ease::Sine::TimeInterpolatorInOut();

} // namespace Multiwaveview
} // namespace Widget
} // namespace Internal
} // namespace Droid
} // namespace Elastos
