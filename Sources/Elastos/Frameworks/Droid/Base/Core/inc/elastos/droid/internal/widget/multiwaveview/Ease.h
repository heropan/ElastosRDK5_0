
#ifndef __ELASTOS_DROID_INTERNAL_WIDGET_EASE_H__
#define __ELASTOS_DROID_INTERNAL_WIDGET_EASE_H__

#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {
namespace Multiwaveview {

class Ease
    : public Object
{
public:
    class Linear
        : public Object
    {
    public:
        class TimeInterpolator
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

    public:
        static AutoPtr<TimeInterpolator> mEaseNone;
    };

    class Cubic
        : public Object
    {
    public:
        class TimeInterpolatorIn
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorInOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

    public:
        static AutoPtr<TimeInterpolatorIn> mEaseIn;

        static AutoPtr<TimeInterpolatorOut> mEaseOut;

        static AutoPtr<TimeInterpolatorInOut> mEaseInOut;
    };

    class Quad
        : public Object
    {
    public:
        class TimeInterpolatorIn
            : public Object
        {
        public:
            Float GetInterpolation (
                /* [in] */ Float input);
        };

        class TimeInterpolatorOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorInOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

    public:
        static AutoPtr<TimeInterpolatorIn> mEaseIn;

        static AutoPtr<TimeInterpolatorOut> mEaseOut;

        static AutoPtr<TimeInterpolatorInOut> mEaseInOut;
    };

    class Quart
        : public Object
    {
    public:
        class TimeInterpolatorIn
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorInOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

    public:
        static AutoPtr<TimeInterpolatorIn> mEaseIn;

        static AutoPtr<TimeInterpolatorOut> mEaseOut;

        static AutoPtr<TimeInterpolatorInOut> mEaseInOut;
    };

    class Quint
        : public Object
    {
    public:
        class TimeInterpolatorIn
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorInOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

    public:
        static AutoPtr<TimeInterpolatorIn> mEaseIn;

        static AutoPtr<TimeInterpolatorOut> mEaseOut;

        static AutoPtr<TimeInterpolatorInOut> mEaseInOut;
    };

    class Sine
        : public Object
    {
    public:
        class TimeInterpolatorIn
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

        class TimeInterpolatorInOut
            : public Object
        {
        public:
            Float GetInterpolation(
                /* [in] */ Float input);
        };

    public:
        static AutoPtr<TimeInterpolatorIn> mEaseIn;

        static AutoPtr<TimeInterpolatorOut> mEaseOut;

        static AutoPtr<TimeInterpolatorInOut> mEaseInOut;
    };

public:
    static const Float DOMAIN;
    static const Float DURATION;
    static const Float START;
};

} // namespace Multiwaveview
} // namespace Widget
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_WIDGET_EASE_H__
