
#ifndef __ELASTOS_DROID_APP_CFASTBITMAPDRAWABLE_H__
#define __ELASTOS_DROID_APP_CFASTBITMAPDRAWABLE_H__

#include "_CFastBitmapDrawable.h"
#include "ext/frameworkdef.h"
#include "app/FastBitmapDrawable.h"


using Elastos::Droid::Graphics::IRect;

namespace Elastos{
namespace Droid{
namespace App{

/**
 * Special drawable that draws a wallpaper as fast as possible.  Assumes
 * no scaling or placement off (0,0) of the wallpaper (this should be done
 * at the time the bitmap is loaded).
 */
CarClass(CFastBitmapDrawable), public FastBitmapDrawable
{
public:
    IDRAWABLE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap);

    //@Override
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CFASTBITMAPDRAWABLE_H__
