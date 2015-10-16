
#include "elastos/droid/graphics/Typeface.h"
#include "elastos/droid/graphics/CTypeface.h"
#include <skia/core/SkStream.h>
#include <skia/core/SkTypeface.h>
#include <androidfw/AssetManager.h>

using Elastos::IO::CFile;

extern void skia_set_text_gamma(float, float);

namespace Elastos {
namespace Droid {
namespace Graphics {

// {CCCF6009-21EE-411A-BC20-EA156193742D}
extern const InterfaceID EIID_Typeface =
    { 0xcccf6009, 0x21ee, 0x411a, { 0xbc, 0x20, 0xea, 0x15, 0x61, 0x93, 0x74, 0x2d } };

const String Typeface::TAG = String("Typeface");
const String Typeface::FONTS_CONFIG = String("fonts.xml");
AutoPtr<ITypeface> Typeface::DEFAULT;
AutoPtr<ITypeface> Typeface::DEFAULT_BOLD;
AutoPtr<ITypeface> Typeface::SANS_SERIF;
AutoPtr<ITypeface> Typeface::SERIF;
AutoPtr<ITypeface> Typeface::MONOSPACE;
AutoPtr< ArrayOf<ITypeface*> > Typeface::sDefaults = StaticInit();
HashMap<Int32, AutoPtr<Typeface::TypefaceMap> > Typeface::sTypefaceCache(3);
AutoPtr<ITypeface> Typeface::sDefaultTypeface;
AutoPtr<IMap> Typeface::sSystemFontMap;
AutoPtr<ArrayOf<AutoPtr<IFontFamily> > > Typeface::sFallbackFonts;

AutoPtr< ArrayOf<ITypeface*> > Typeface::StaticInit()
{
    Init();
    // Set up defaults and typefaces exposed in public API
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

CAR_INTERFACE_IMPL(Typeface, Object, ITypeface);
Typeface::Typeface()
    : mNativeInstance(0)
    , mStyle(0)
{}

Typeface::~Typeface()
{
    NativeUnref(mNativeInstance);
}

ECode Typeface::GetStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    *style = mStyle;
    return NOERROR;
}

ECode Typeface::IsBold(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mStyle & ITypeface::BOLD) != 0;
    return NOERROR;
}

ECode Typeface::IsItalic(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mStyle & ITypeface::ITALIC) != 0;
    return NOERROR;
}

ECode Typeface::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* e)
{
    assert(0 && "TODO");
    // if (this == o) return true;
    // if (o == null || getClass() != o.getClass()) return false;

    // Typeface typeface = (Typeface) o;

    // return mStyle == typeface.mStyle && native_instance == typeface.native_instance;
    return NOERROR;
}

ECode Typeface::GetHashCode(
    /* [out] */ Int32* code)
{
    assert(0 && "TODO");
    /*
     * Modified method for hashCode with long native_instance derived from
     * http://developer.android.com/reference/java/lang/Object.html
     */
    // int result = 17;
    // result = 31 * result + (int) (native_instance ^ (native_instance >>> 32));
    // result = 31 * result + mStyle;
    // return result;
     return NOERROR;
}

ECode Typeface::Create(
    /* [in] */ const String& familyName,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);

    assert(0 && "TODO");
    // if (sSystemFontMap != NULL) {
    //     return create(sSystemFontMap.get(familyName), style);
    // }
    *typeface = NULL;
    return NOERROR;
}

ECode Typeface::Create(
    /* [in] */ ITypeface* family,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);

    if (style < 0 || style > 3) {
        style = 0;
    }
    Int64 ni = 0;
    if (family != NULL) {
        // Return early if we're asked for the same face/style
        Int32 familyStyle;
        family->GetStyle(&familyStyle);
        if (familyStyle == style) {
            *typeface = family;
            REFCOUNT_ADD(*typeface);
            return NOERROR;
        }

        ni = ((Typeface*)(ITypeface*)family->Probe(EIID_Typeface))->mNativeInstance;
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
            REFCOUNT_ADD(*typeface);
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
    REFCOUNT_ADD(*typeface);
    return NOERROR;
}

ECode Typeface::CreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ const String& path,
    /* [out] */ ITypeface** typeface)
{
    assert(0 && "TODO");
    // if (sFallbackFonts != null) {
    //     FontFamily fontFamily = new FontFamily();
    //     if (fontFamily.addFontFromAsset(mgr, path)) {
    //         FontFamily[] families = { fontFamily };
    //         return createFromFamiliesWithDefault(families);
    //     }
    // }
    // throw new RuntimeException("Font asset not found " + path);
    return E_RUNTIME_EXCEPTION;
}

ECode Typeface::CreateFromFile(
    /* [in] */ IFile* path,
    /* [out] */ ITypeface** typeface)
{
    String absPath;
    path->GetAbsolutePath(&absPath);
    return CreateFromFile(absPath, typeface);
}

ECode Typeface::CreateFromFile(
    /* [in]*/ const String& path,
    /* [out] */ ITypeface** typeface)
{
    assert(0 && "TODO");
    // if (sFallbackFonts != null) {
    //     FontFamily fontFamily = new FontFamily();
    //     if (fontFamily.addFont(path)) {
    //         FontFamily[] families = { fontFamily };
    //         return createFromFamiliesWithDefault(families);
    //     }
    // }
    // throw new RuntimeException("Font not found " + path);
    return E_RUNTIME_EXCEPTION;
}

ECode Typeface::CreateFromFamilies(
    /* [in]*/ ArrayOf<IFontFamily>* families,
    /* [out]*/ ITypeface** typeface)
{
    assert(0 && "TODO");
    // long[] ptrArray = new long[families.length];
    // for (int i = 0; i < families.length; i++) {
    //     ptrArray[i] = families[i].mNativePtr;
    // }
    // return new Typeface(nativeCreateFromArray(ptrArray));
    return NOERROR;
}

ECode Typeface::CreateFromFamiliesWithDefault(
    /* [in]*/ ArrayOf<IFontFamily>* families,
    /* [out]*/ ITypeface** typeface)
{
    assert(0 && "TODO");
    // long[] ptrArray = new long[families.length + sFallbackFonts.length];
    // for (int i = 0; i < families.length; i++) {
    //     ptrArray[i] = families[i].mNativePtr;
    // }
    // for (int i = 0; i < sFallbackFonts.length; i++) {
    //     ptrArray[i + families.length] = sFallbackFonts[i].mNativePtr;
    // }
    // return new Typeface(nativeCreateFromArray(ptrArray));
    return NOERROR;
}

ECode Typeface::constructor(
    /* [in] */ Int64 ni)
{
    if (ni == 0) {
//        throw new RuntimeException("native typeface cannot be made");
        return E_RUNTIME_EXCEPTION;
    }
    mNativeInstance = ni;
    mStyle = NativeGetStyle(ni);
    return NOERROR;
}

void Typeface::SetDefault(
    /* [in] */ ITypeface* t)
{
    sDefaultTypeface = t;
    NativeSetDefault(((Typeface*)t)->mNativeInstance);
}

Int64 Typeface::NativeCreateFromTypeface(
    /* [in] */ Int64 native_instance,
    /* [in] */ Int32 style)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int64 Typeface::NativeCreateWeightAlias(
    /* [in] */ Int64 native_instance,
    /* [in] */ Int32 weight)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

void Typeface::NativeUnref(
    /* [in] */ Int64 native_instance)
{
    assert(0 && "TODO: need jni codes.");
}

Int32 Typeface::NativeGetStyle(
    /* [in] */ Int64 native_instance)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int64 Typeface::NativeCreateFromArray(
    /* [in] */ ArrayOf<Int64>* familyArray)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

void Typeface::NativeSetDefault(
    /* [in] */ Int64 native_instance)
{
    assert(0 && "TODO: need jni codes.");
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

void Typeface::Init()
{
    assert(0 && "TODO");
    // // Load font config and initialize Minikin state
    // File systemFontConfigLocation = getSystemFontConfigLocation();
    // File configFilename = new File(systemFontConfigLocation, FONTS_CONFIG);
    // try {
    //     FileInputStream fontsIn = new FileInputStream(configFilename);
    //     FontListParser.Config fontConfig = FontListParser.parse(fontsIn);

    //     List<FontFamily> familyList = new ArrayList<FontFamily>();
    //     // Note that the default typeface is always present in the fallback list;
    //     // this is an enhancement from pre-Minikin behavior.
    //     for (int i = 0; i < fontConfig.families.size(); i++) {
    //         Family f = fontConfig.families.get(i);
    //         if (i == 0 || f.name == null) {
    //             familyList.add(makeFamilyFromParsed(f));
    //         }
    //     }
    //     sFallbackFonts = familyList.toArray(new FontFamily[familyList.size()]);
    //     setDefault(Typeface.createFromFamilies(sFallbackFonts));

    //     Map<String, Typeface> systemFonts = new HashMap<String, Typeface>();
    //     for (int i = 0; i < fontConfig.families.size(); i++) {
    //         Typeface typeface;
    //         Family f = fontConfig.families.get(i);
    //         if (f.name != null) {
    //             if (i == 0) {
    //                 // The first entry is the default typeface; no sense in
    //                 // duplicating the corresponding FontFamily.
    //                 typeface = sDefaultTypeface;
    //             } else {
    //                 FontFamily fontFamily = makeFamilyFromParsed(f);
    //                 FontFamily[] families = { fontFamily };
    //                 typeface = Typeface.createFromFamiliesWithDefault(families);
    //             }
    //             systemFonts.put(f.name, typeface);
    //         }
    //     }
    //     for (FontListParser.Alias alias : fontConfig.aliases) {
    //         Typeface base = systemFonts.get(alias.toName);
    //         Typeface newFace = base;
    //         int weight = alias.weight;
    //         if (weight != 400) {
    //             newFace = new Typeface(nativeCreateWeightAlias(base.native_instance, weight));
    //         }
    //         systemFonts.put(alias.name, newFace);
    //     }
    //     sSystemFontMap = systemFonts;

    // } catch (RuntimeException e) {
    //     Log.w(TAG, "Didn't create default family (most likely, non-Minikin build)", e);
    //     // TODO: normal in non-Minikin case, remove or make error when Minikin-only
    // } catch (FileNotFoundException e) {
    //     Log.e(TAG, "Error opening " + configFilename);
    // } catch (IOException e) {
    //     Log.e(TAG, "Error reading " + configFilename);
    // } catch (XmlPullParserException e) {
    //     Log.e(TAG, "XML parse exception for " + configFilename);
    // }
}

AutoPtr<IFile> Typeface::GetSystemFontConfigLocation()
{
    AutoPtr<IFile> file;
    CFile::New(String("/system/etc/"), (IFile**)&file);
    return file;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
