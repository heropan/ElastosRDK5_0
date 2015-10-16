#ifndef __ELASTOS_DROID_WIDGET_CSCROLLBARDRAWABLE_H__
#define __ELASTOS_DROID_WIDGET_CSCROLLBARDRAWABLE_H__

#include "_Elastos_Droid_Widget_CScrollBarDrawable.h"

#include "elastos/droid/widget/ScrollBarDrawable.h"

using Elastos::Droid::Graphics::Drawable::IDrawableConstantState;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CScrollBarDrawable), public ScrollBarDrawable
{
public :
    IDRAWABLE_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    /**
     * Indicate whether the horizontal scrollbar track should always be drawn regardless of the
     * extent. Defaults to false.
     *
     * @param alwaysDrawTrack Set to true if the track should always be drawn
     */
    CARAPI SetAlwaysDrawHorizontalTrack(
        /* [in] */ Boolean alwaysDrawTrack);

    /**
     * Indicate whether the vertical scrollbar track should always be drawn regardless of the
     * extent. Defaults to false.
     *
     * @param alwaysDrawTrack Set to true if the track should always be drawn
     */
    CARAPI SetAlwaysDrawVerticalTrack(
        /* [in] */ Boolean alwaysDrawTrack);

    /**
     * Indicates whether the horizontal scrollbar track should always be drawn regardless of the
     * extent.
     */
    CARAPI GetAlwaysDrawHorizontalTrack(
        /* [out] */ Boolean* alwaysDrawTrack);

    /**
     * Indicates whether the vertical scrollbar track should always be drawn regardless of the
     * extent.
     */
    CARAPI GetAlwaysDrawVerticalTrack(
        /* [out] */ Boolean* alwaysDrawTrack);

    CARAPI SetParameters(
        /* [in] */ Int32 range,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 extent,
        /* [in] */ Boolean vertical);

    CARAPI SetVerticalThumbDrawable(
        /* [in] */ IDrawable* thumb);

    CARAPI SetVerticalTrackDrawable(
        /* [in] */ IDrawable* track);

    CARAPI SetHorizontalThumbDrawable(
        /* [in] */ IDrawable* thumb);

    CARAPI SetHorizontalTrackDrawable(
        /* [in] */ IDrawable* track);

    CARAPI GetSize(
        /* [in] */ Boolean vertical,
        /* [out] */ Int32* size);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CSCROLLBARDRAWABLE_H__
