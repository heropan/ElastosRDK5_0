
#include "elastos/droid/graphics/drawable/CMipmapDrawable.h"
#include "elastos/droid/ext/frameworkext.h"

IDRAWABLE_METHODS_IMPL(
    CMipmapDrawable, MipmapDrawable);


PInterface CMipmapDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CMipmapDrawable::Probe(riid);
}

ECode CMipmapDrawable::AddDrawable(
    /* [in] */ IDrawable* drawable)
{
    return MipmapDrawable::AddDrawable(drawable);
}

ECode CMipmapDrawable::constructor()
{
    return NOERROR;
}
