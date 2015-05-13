
#include "graphics/Typeface.h"
#include "graphics/CTypeface.h"
#include "content/res/CAssetManager.h"
#include <skia/core/SkStream.h>
#include <skia/core/SkTypeface.h>
#include <androidfw/AssetManager.h>

using Elastos::Droid::Content::Res::CAssetManager;

extern void skia_set_text_gamma(float, float);

namespace Elastos {
namespace Droid {
namespace Graphics {

// {CCCF6009-21EE-411A-BC20-EA156193742D}
extern const InterfaceID EIID_Typeface =
    { 0xcccf6009, 0x21ee, 0x411a, { 0xbc, 0x20, 0xea, 0x15, 0x61, 0x93, 0x74, 0x2d } };

AutoPtr<ITypeface> Typeface::DEFAULT;
AutoPtr<ITypeface> Typeface::DEFAULT_BOLD;
AutoPtr<ITypeface> Typeface::SANS_SERIF;
AutoPtr<ITypeface> Typeface::SERIF;
AutoPtr<ITypeface> Typeface::MONOSPACE;
AutoPtr< ArrayOf<ITypeface*> > Typeface::sDefaults = StaticInit();
HashMap<Int32, AutoPtr<Typeface::TypefaceMap> > Typeface::sTypefaceCache(3);

AutoPtr< ArrayOf<ITypeface*> > Typeface::StaticInit()
{
    Typeface::Create(String(NULL), 0, (ITypeface**)&Typeface::DEFAULT);
    Typeface::Create(String(NULL), ITypeface::BOLD, (ITypeface**)&Typeface::DEFAULT_BOLD);
    Typeface::Create(String("sans-serif"), 0, (ITypeface**)&Typeface::SANS_SERIF);
    Typeface::Create(String("serif"), 0, (ITypeface**)&Typeface::SERIF);
    Typeface::Create(String("monospace"), 0, (ITypeface**)&Typeface::MONOSPACE);

    AutoPtr<ITypeface> italic, boldItalic;
    Typeface::Create(String(NULL), ITypeface::ITALIC, (ITypeface**)&italic);
    Typeface::Create(String(NULL), ITypeface::BOLD_ITALIC, (ITypeface**)&boldItalic);

    AutoPtr< ArrayOf<ITypeface*> > typefaces = ArrayOf<ITypeface*>::Alloc(4);
    typefaces->Set(0, Typeface::DEFAULT);
    typefaces->Set(1, Typeface::DEFAULT_BOLD);
    typefaces->Set(2, italic);
    typefaces->Set(3, boldItalic);
    return typefaces;
}

Typeface::Typeface()
    : mNativeInstance(0)
    , mStyle(0)
{}

Typeface::~Typeface()
{
    NativeUnref(mNativeInstance);
}

Int32 Typeface::GetStyle()
{
    return mStyle;
}

Boolean Typeface::IsBold()
{
    return (mStyle & ITypeface::BOLD) != 0;
}

Boolean Typeface::IsItalic()
{
    return (mStyle & ITypeface::ITALIC) != 0;
}

ECode Typeface::Create(
    /* [in] */ const String& familyName,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);

    return CTypeface::New(NativeCreate(familyName, style), typeface);
}

ECode Typeface::Create(
    /* [in] */ ITypeface* family,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);

    Int32 ni = 0;
    if (family != NULL) {
        // Return early if we're asked for the same face/style
        Int32 familyStyle;
        family->GetStyle(&familyStyle);
        if (familyStyle == style) {
            *typeface = family;
            INTERFACE_ADDREF(*typeface);
            return NOERROR;
        }

        ni = ((Typeface*)family->Probe(EIID_Typeface))->mNativeInstance;
    }

    AutoPtr<ITypeface> tmpTypeface;
    AutoPtr<TypefaceMap> styles;
    HashMap<Int32, AutoPtr<TypefaceMap> >::Iterator it = sTypefaceCache.Find(ni);
    if (it != sTypefaceCache.End()) {
        styles = it->mSecond;
    }

    if (styles != NULL) {
        typename TypefaceMap::Iterator sit = styles->Find(style);
        if (sit != styles->End()) {
            tmpTypeface = sit->mSecond;
        }
        if (tmpTypeface != NULL) {
            *typeface = tmpTypeface;
            INTERFACE_ADDREF(*typeface);
            return NOERROR;
        }
    }

    CTypeface::New(NativeCreateFromTypeface(ni, style), typeface);
    if (styles == NULL) {
        styles = new TypefaceMap(4);
        sTypefaceCache[ni] = styles;
    }
    (*styles)[style] = *typeface;

    return NOERROR;
}

ECode Typeface::DefaultFromStyle(
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);

    *typeface = (*sDefaults)[style];
    INTERFACE_ADDREF(*typeface);
    return NOERROR;
}

ECode Typeface::CreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ const String& path,
    /* [out] */ ITypeface** typeface)
{
    return CTypeface::New(NativeCreateFromAsset(mgr, path), typeface);
}

ECode Typeface::CreateFromFile(
    /* [in] */ IFile* path,
    /* [out] */ ITypeface** typeface)
{
    String absPath;
    path->GetAbsolutePath(&absPath);
    return CTypeface::New(NativeCreateFromFile(absPath), typeface);
}

ECode Typeface::CreateFromFile(
    /* [in]*/ const String& path,
    /* [out] */ ITypeface** typeface)
{
    return CTypeface::New(NativeCreateFromFile(path), typeface);
}

ECode Typeface::Init(
    /* [in] */ Int32 ni)
{
    if (ni == 0) {
//        throw new RuntimeException("native typeface cannot be made");
        return E_RUNTIME_EXCEPTION;
    }
    mNativeInstance = ni;
    mStyle = NativeGetStyle(ni);
    return NOERROR;
}

Int32 Typeface::NativeCreate(
    /* [in] */ const String& familyName,
    /* [in] */ Int32 style)
{
    SkTypeface* face;

    if (familyName.IsNull()) {
        face = SkTypeface::CreateFromName(NULL, (SkTypeface::Style)style);
    }
    else {
        face = SkTypeface::CreateFromName(familyName.string(), (SkTypeface::Style)style);
    }
    return (Int32)face;
}

Int32 Typeface::NativeCreateFromTypeface(
    /* [in] */ Int32 nOther,
    /* [in] */ Int32 style)
{
    return (Int32)SkTypeface::CreateFromTypeface((SkTypeface*)nOther, (SkTypeface::Style)style);
}

void Typeface::NativeUnref(
    /* [in] */ Int32 nObj)
{
    SkSafeUnref((SkTypeface*)nObj);
}

Int32 Typeface::NativeGetStyle(
    /* [in] */ Int32 nObj)
{
    return ((SkTypeface*)nObj)->style();
}

class AssetStream : public SkStream
{
public:
    AssetStream(android::Asset* asset, bool hasMemoryBase) : fAsset(asset)
    {
        fMemoryBase = hasMemoryBase ? fAsset->getBuffer(false) : NULL;
    }

    virtual ~AssetStream()
    {
        delete fAsset;
    }

    virtual const void* getMemoryBase()
    {
        return fMemoryBase;
    }

    virtual bool rewind()
    {
        off_t pos = fAsset->seek(0, SEEK_SET);
        return pos != (off_t)-1;
    }

    virtual size_t read(void* buffer, size_t size)
    {
        ssize_t amount;

        if (NULL == buffer)
        {
            if (0 == size)  // caller is asking us for our total length
                return fAsset->getLength();

            // asset->seek returns new total offset
            // we want to return amount that was skipped

            off_t oldOffset = fAsset->seek(0, SEEK_CUR);
            if (-1 == oldOffset)
                return 0;
            off_t newOffset = fAsset->seek(size, SEEK_CUR);
            if (-1 == newOffset)
                return 0;

            amount = newOffset - oldOffset;
        }
        else
        {
            amount = fAsset->read(buffer, size);
        }

        if (amount < 0)
            amount = 0;
        return amount;
    }

private:
    android::Asset* fAsset;
    const void* fMemoryBase;
};

Int32 Typeface::NativeCreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ const String& path)
{
    if (mgr == NULL || path.IsNull()) return 0;

    android::AssetManager* amgr = (android::AssetManager*)((CAssetManager*)mgr)->Ni();
    if (NULL == mgr) {
        return 0;
    }

    android::Asset* asset = amgr->open(
            path.string(), android::Asset::ACCESS_BUFFER);
    if (NULL == asset) {
        return 0;
    }

    SkStream* stream = new AssetStream(asset, true);
    SkTypeface* face = SkTypeface::CreateFromStream(stream);
    // SkTypeFace::CreateFromStream calls ref() on the stream, so we
    // need to unref it here or it won't be freed later on
    stream->unref();

    return (Int32)face;
}

Int32 Typeface::NativeCreateFromFile(
    /* [in] */ const String& path)
{
    if (path.IsNull()) return 0;

    return (Int32)SkTypeface::CreateFromFile(path.string());
}

#define MIN_GAMMA   (0.1f)
#define MAX_GAMMA   (10.0f)
static float pinGamma(float gamma)
{
    if (gamma < MIN_GAMMA) {
        gamma = MIN_GAMMA;
    }
    else if (gamma > MAX_GAMMA) {
        gamma = MAX_GAMMA;
    }
    return gamma;
}

void Typeface::SetGammaForText(
        /* [in] */ Float blackGamma,
        /* [in] */ Float whiteGamma)
{
    // Comment this out for release builds. This is only used during development
    skia_set_text_gamma(pinGamma(blackGamma), pinGamma(whiteGamma));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
