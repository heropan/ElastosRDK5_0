
#ifndef __TYPEFACE_H__
#define __TYPEFACE_H__

#include "ext/frameworkext.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::IO::IFile;

using Elastos::Droid::Content::Res::IAssetManager;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_Typeface;

/**
 * The Typeface class specifies the typeface and intrinsic style of a font.
 * This is used in the paint, along with optionally Paint settings like
 * textSize, textSkewX, textScaleX to specify
 * how text appears when drawn (and measured).
 */
class Typeface
{
public:
    /** Returns the typeface's intrinsic style attributes */
    virtual CARAPI_(Int32) GetStyle();

    /** Returns true if getStyle() has the BOLD bit set. */
    CARAPI_(Boolean) IsBold();

    /** Returns true if getStyle() has the ITALIC bit set. */
    CARAPI_(Boolean) IsItalic();

    /**
     * Create a typeface object given a family name, and option style information.
     * If null is passed for the name, then the "default" font will be chosen.
     * The resulting typeface object can be queried (getStyle()) to discover what
     * its "real" style characteristics are.
     *
     * @param familyName May be null. The name of the font family.
     * @param style  The style (normal, bold, italic) of the typeface.
     *               e.g. NORMAL, BOLD, ITALIC, BOLD_ITALIC
     * @return The best matching typeface.
     */
    static CARAPI Create(
        /* [in] */ const String& familyName,
        /* [in] */ Int32 style,
        /* [out] */ ITypeface** typeface);

    /**
     * Create a typeface object that best matches the specified existing
     * typeface and the specified Style. Use this call if you want to pick a new
     * style from the same family of an existing typeface object. If family is
     * null, this selects from the default font's family.
     *
     * @param family May be null. The name of the existing type face.
     * @param style  The style (normal, bold, italic) of the typeface.
     *               e.g. NORMAL, BOLD, ITALIC, BOLD_ITALIC
     * @return The best matching typeface.
     */
    static CARAPI Create(
        /* [in] */ ITypeface* family,
        /* [in] */ Int32 style,
        /* [out] */ ITypeface** typeface);

    /**
     * Returns one of the default typeface objects, based on the specified style
     *
     * @return the default typeface that corresponds to the style
     */
    static CARAPI DefaultFromStyle(
        /* [in] */ Int32 style,
        /* [out] */ ITypeface** typeface);

    /**
     * Create a new typeface from the specified font data.
     * @param mgr The application's asset manager
     * @param path  The file name of the font data in the assets directory
     * @return The new typeface.
     */
    static CARAPI CreateFromAsset(
        /* [in] */ IAssetManager* mgr,
        /* [in] */ const String& path,
        /* [out] */ ITypeface** typeface);

    /**
     * Create a new typeface from the specified font file.
     *
     * @param path The path to the font data.
     * @return The new typeface.
     */
    static CARAPI CreateFromFile(
        /* [in] */ IFile* path,
        /* [out] */ ITypeface** typeface);

    /**
     * Create a new typeface from the specified font file.
     *
     * @param path The full path to the font data.
     * @return The new typeface.
     */
    static CARAPI CreateFromFile(
        /* [in]*/ const String& path,
        /* [out] */ ITypeface** typeface);

    /**
     * Set the global gamma coefficients for black and white text. This call is
     * usually a no-op in shipping products, and only exists for testing during
     * development.
     *
     * @param blackGamma gamma coefficient for black text
     * @param whiteGamma gamma coefficient for white text
     *
     * @hide - this is just for calibrating devices, not for normal apps
     */
    static CARAPI_(void) SetGammaForText(
        /* [in] */ Float blackGamma,
        /* [in] */ Float whiteGamma);

protected:
    Typeface();

    // don't allow clients to call this directly
    CARAPI Init(
        /* [in] */ Int32 ni);

    virtual ~Typeface();

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ const String& familyName,
        /* [in] */ Int32 style);

    static CARAPI_(Int32) NativeCreateFromTypeface(
        /* [in] */ Int32 nOther,
        /* [in] */ Int32 style);

    static CARAPI_(void) NativeUnref(
        /* [in] */ Int32 nObj);

    static CARAPI_(Int32) NativeGetStyle(
        /* [in] */ Int32 nObj);

    static CARAPI_(Int32) NativeCreateFromAsset(
        /* [in] */ IAssetManager* mgr,
        /* [in] */ const String& path);

    static CARAPI_(Int32) NativeCreateFromFile(
        /* [in] */ const String& path);

    static CARAPI_(AutoPtr< ArrayOf<ITypeface*> >) StaticInit();

public:
    /** The default NORMAL typeface object */
    static AutoPtr<ITypeface> DEFAULT;
    /**
     * The default BOLD typeface object. Note: this may be not actually be
     * bold, depending on what fonts are installed. Call getStyle() to know
     * for sure.
     */
    static AutoPtr<ITypeface> DEFAULT_BOLD;
    /** The NORMAL style of the default sans serif typeface. */
    static AutoPtr<ITypeface> SANS_SERIF;
    /** The NORMAL style of the default serif typeface. */
    static AutoPtr<ITypeface> SERIF;
    /** The NORMAL style of the default monospace typeface. */
    static AutoPtr<ITypeface> MONOSPACE;

    static AutoPtr< ArrayOf<ITypeface*> > sDefaults;

    typedef HashMap<Int32, AutoPtr<ITypeface> > TypefaceMap;
    static HashMap<Int32, AutoPtr<TypefaceMap> > sTypefaceCache;

    /* package */
    Int32 mNativeInstance;

private:
    Int32 mStyle;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__TYPEFACE_H__
