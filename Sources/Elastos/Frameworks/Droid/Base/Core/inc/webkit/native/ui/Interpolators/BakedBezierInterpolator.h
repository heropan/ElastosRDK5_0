// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_BAKEDBEZIERINTERPOLATOR_H_
#define _ELASTOS_DROID_WEBKIT_UI_BAKEDBEZIERINTERPOLATOR_H_

//package org.chromium.ui.interpolators;
//import android.view.animation.Interpolator;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * A pre-baked bezier-curved interpolator for quantum-paper transitions.
 * TODO(dtrainor): Move to the API Compatability version iff that supports the curves we need and
 * once we move to that SDK.
 */
class BakedBezierInterpolator : public Interpolator
{
public:
    //@Override
    CARAPI_(Float) GetInterpolation(
        /* in */ Float input);
private:
    /**
     * Use the INSTANCE variable instead of instantiating.
     */
    BakedBezierInterpolator(
        /* in */ ArrayOf<Float>* values);

    static CARAPI_(AutoPtr< ArrayOf<Float> >) InitTransformValus();

    static CARAPI_(AutoPtr< ArrayOf<Float> >) InitFadeOutValus();

    static CARAPI_(AutoPtr< ArrayOf<Float> >) InitFadeInValus();

    static CARAPI_(AutoPtr< ArrayOf<Float> >) InitTransformThroughValus();

public:
    /**
     * 0.4 to 0.2 bezier curve.  Should be used for general movement.
     */
    const BakedBezierInterpolator TRANSFORM_CURVE;

    /**
     * 0.4 to 1.0 bezier curve.  Should be used for fading out.
     */
    const BakedBezierInterpolator FADE_OUT_CURVE;

    /**
     * 0.0 to 0.2 bezier curve.  Should be used for fading in.
     */
    const BakedBezierInterpolator FADE_IN_CURVE;

    /**
     * 0.0 to 0.13 by 0.84 to 0.99 bezier curve.  Should be used for very quick transforms.
     */
    const BakedBezierInterpolator TRANSFORM_FOLLOW_THROUGH_CURVE;

private:
    /**
     * Lookup table values.
     * Generated using a Bezier curve from (0,0) to (1,1) with control points:
     * P0 (0.0, 0.0)
     * P1 (0.4, 0.0)
     * P2 (0.2, 1.0)
     * P3 (1.0, 1.0)
     *
     * Values sampled with x at regular intervals between 0 and 1.
     */
    static AutoPtr< ArrayOf<Float> > TRANSFORM_VALUES;

    /**
     * Lookup table values.
     * Generated using a Bezier curve from (0,0) to (1,1) with control points:
     * P0 (0.0, 0.0)
     * P1 (0.4, 0.0)
     * P2 (1.0, 1.0)
     * P3 (1.0, 1.0)
     *
     * Values sampled with x at regular intervals between 0 and 1.
     */
    static AutoPtr< ArrayOf<Float> > FADE_OUT_VALUES;

    /**
     * Lookup table values.
     * Generated using a Bezier curve from (0,0) to (1,1) with control points:
     * P0 (0.0, 0.0)
     * P1 (0.0, 0.0)
     * P2 (0.2, 1.0)
     * P3 (1.0, 1.0)
     *
     * Values sampled with x at regular intervals between 0 and 1.
     */
    static AutoPtr< ArrayOf<Float> > FADE_IN_VALUES;

    /**
     * Lookup table values.
     * Generated using a Bezier curve from (0,0) to (1,1) with control points:
     * P0 (0.0, 0.0)
     * P1 (0.0, 0.84)
     * P2 (0.13, 0.99)
     * P3 (1.0, 1.0)
     */
    static AutoPtr< ArrayOf<Float> > TRANSFORM_FOLLOW_THROUGH_VALUES;

    const AutoPtr< ArrayOf<Float> > mValues;
    const Float mStepSize;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_BAKEDBEZIERINTERPOLATOR_H_

