#include "widget/CScrollBarDrawable.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"

using Elastos::Droid::Graphics::Drawable::CAnimatedRotateDrawable;

namespace Elastos {
namespace Droid {
namespace Widget {

IDRAWABLE_METHODS_IMPL(CScrollBarDrawable, ScrollBarDrawable)
IDRAWABLECALLBACK_METHODS_IMPL(CScrollBarDrawable, ScrollBarDrawable);

PInterface CScrollBarDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CScrollBarDrawable::Probe(riid);
}

ECode CScrollBarDrawable::constructor()
{
    return NOERROR;
}

/**
 * Indicate whether the horizontal scrollbar track should always be drawn regardless of the
 * extent. Defaults to false.
 *
 * @param alwaysDrawTrack Set to true if the track should always be drawn
 */
ECode CScrollBarDrawable::SetAlwaysDrawHorizontalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    return ScrollBarDrawable::SetAlwaysDrawHorizontalTrack(alwaysDrawTrack);
}

/**
 * Indicate whether the vertical scrollbar track should always be drawn regardless of the
 * extent. Defaults to false.
 *
 * @param alwaysDrawTrack Set to true if the track should always be drawn
 */
ECode CScrollBarDrawable::SetAlwaysDrawVerticalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    return ScrollBarDrawable::SetAlwaysDrawVerticalTrack(alwaysDrawTrack);
}

/**
 * Indicates whether the horizontal scrollbar track should always be drawn regardless of the
 * extent.
 */
ECode CScrollBarDrawable::GetAlwaysDrawHorizontalTrack(
    /* [out] */ Boolean* alwaysDrawTrack)
{
    VALIDATE_NOT_NULL(alwaysDrawTrack);
    *alwaysDrawTrack = ScrollBarDrawable::GetAlwaysDrawHorizontalTrack();
    return NOERROR;
}

/**
 * Indicates whether the vertical scrollbar track should always be drawn regardless of the
 * extent.
 */
ECode CScrollBarDrawable::GetAlwaysDrawVerticalTrack(
    /* [out] */ Boolean* alwaysDrawTrack)
{
    VALIDATE_NOT_NULL(alwaysDrawTrack);
    *alwaysDrawTrack = ScrollBarDrawable::GetAlwaysDrawVerticalTrack();
    return NOERROR;
}

ECode CScrollBarDrawable::SetParameters(
    /* [in] */ Int32 range,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 extent,
    /* [in] */ Boolean vertical)
{
    return ScrollBarDrawable::SetParameters(
            range, offset, extent, vertical);
}

ECode CScrollBarDrawable::SetVerticalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    return ScrollBarDrawable::SetVerticalThumbDrawable(thumb);
}

ECode CScrollBarDrawable::SetVerticalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    return ScrollBarDrawable::SetVerticalTrackDrawable(track);
}

ECode CScrollBarDrawable::SetHorizontalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    return ScrollBarDrawable::SetHorizontalThumbDrawable(thumb);
}

ECode CScrollBarDrawable::SetHorizontalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    return ScrollBarDrawable::SetHorizontalTrackDrawable(track);
}

ECode CScrollBarDrawable::GetSize(
    /* [in] */ Boolean vertical,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = ScrollBarDrawable::GetSize(vertical);
    return NOERROR;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
