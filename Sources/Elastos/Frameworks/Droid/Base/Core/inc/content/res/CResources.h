
#ifndef __CRESOURCES_H__
#define __CRESOURCES_H__

#include "_CResources.h"
#include "content/res/XmlBlock.h"
#include "content/res/CAssetManager.h"
#include "content/res/CConfiguration.h"
#include "content/res/CCompatibilityInfo.h"
#include "content/res/CTypedArray.h"
#include "util/CDisplayMetrics.h"
#include "util/CTypedValue.h"

using Elastos::Core::Mutex;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Graphics::IMovie;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawableConstantState;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Utility::CDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/**
 * Class for accessing an application's resources.  This sits on top of the
 * asset manager of the application (accessible through {@link #getAssets}) and
 * provides a high-level API for getting typed data from the assets.
 *
 * <p>The Android resource system keeps track of all non-code assets associated with an
 * application. You can use this class to access your application's resources. You can generally
 * acquire the {@link android.content.res.Resources} instance associated with your application
 * with {@link android.content.Context#getResources getResources()}.</p>
 *
 * <p>The Android SDK tools compile your application's resources into the application binary
 * at build time.  To use a resource, you must install it correctly in the source tree (inside
 * your project's {@code res/} directory) and build your application.  As part of the build
 * process, the SDK tools generate symbols for each resource, which you can use in your application
 * code to access the resources.</p>
 *
 * <p>Using application resources makes it easy to update various characteristics of your
 * application without modifying code, and&mdash;by providing sets of alternative
 * resources&mdash;enables you to optimize your application for a variety of device configurations
 * (such as for different languages and screen sizes). This is an important aspect of developing
 * Android applications that are compatible on different types of devices.</p>
 *
 * <p>For more information about using resources, see the documentation about <a
 * href="{@docRoot}guide/topics/resources/index.html">Application Resources</a>.</p>
 */
CarClass(CResources)
{
public:
    class Theme
        : public ElRefBase
        , public IResourcesTheme
    {
    public:
        Theme(
            /* [in] */ CResources* host);

        ~Theme();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI ApplyStyle(
            /* [in] */ Int32 resid,
            /* [in] */ Boolean force);

        CARAPI SetTo(
            /* [in] */ IResourcesTheme* other);

        CARAPI ObtainStyledAttributes(
            /* [in] */ ArrayOf<Int32>* attrs,
            /* [out] */ ITypedArray** styles);

        CARAPI ObtainStyledAttributes(
            /* [in] */ Int32 resid,
            /* [in] */ ArrayOf<Int32>* attrs,
            /* [out] */ ITypedArray** styles);

        CARAPI ObtainStyledAttributes(
            /* [in] */ IAttributeSet* set,
            /* [in] */ ArrayOf<Int32>* attrs,
            /* [in] */ Int32 defStyleAttr,
            /* [in] */ Int32 defStyleRes,
            /* [out] */ ITypedArray** styles);

        CARAPI ResolveAttribute(
            /* [in] */ Int32 resid,
            /* [in, out] */ ITypedValue* outValue,
            /* [in] */ Boolean resolveRefs,
            /* [out] */ Boolean* isFounded);

        CARAPI Dump(
            /* [in] */ Int32 priority,
            /* [in] */ const String& tag,
            /* [in] */ const String& prefix);

    private:
        AutoPtr<CResources> mHost;
        AutoPtr<CAssetManager> mAssets;
        Int32 mTheme;
    };

public:
    CResources();

    ~CResources();

    static CARAPI_(Int32) SelectDefaultTheme(
        /* [in] */ Int32 curTheme,
        /* [in] */ Int32 targetSdkVersion);

    static CARAPI_(Int32) SelectSystemTheme(
        /* [in] */ Int32 curTheme,
        /* [in] */ Int32 targetSdkVersion,
        /* [in] */ Int32 orig,
        /* [in] */ Int32 holo,
        /* [in] */ Int32 deviceDefault);

    /**
     * Return a global shared Resources object that provides access to only
     * system resources (no application resources), and is not configured for
     * the current screen (can not use dimension units, does not change based
     * on orientation, etc).
     */
    static CARAPI_(AutoPtr<IResources>) GetSystem();

    /**
     * Return the string value associated with a particular resource ID.  The
     * returned object will be a String if this is a plain string; it will be
     * some other type of CharSequence if it is styled.
     * {@more}
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return CharSequence The string data associated with the resource, plus
     *         possibly styled text information.
     */
    CARAPI GetText(
        /* [in] */ Int32 id,
        /* [out] */ ICharSequence** seq);

    /**
     * Return the character sequence associated with a particular resource ID for a particular
     * numerical quantity.
     *
     * <p>See <a href="{@docRoot}guide/topics/resources/string-resource.html#Plurals">String
     * Resources</a> for more on quantity strings.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     * @param quantity The number used to get the correct string for the current language's
     *           plural rules.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return CharSequence The string data associated with the resource, plus
     *         possibly styled text information.
     */
    CARAPI GetQuantityText(
        /* [in] */ Int32 id,
        /* [in] */ Int32 quantity,
        /* [out] */ ICharSequence** seq);

    /**
     * Return the string value associated with a particular resource ID.  It
     * will be stripped of any styled text information.
     * {@more}
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return String The string data associated with the resource,
     * stripped of styled text information.
     */
    CARAPI GetString(
        /* [in] */ Int32 id,
        /* [out] */ String* str);

    /**
     * Return the string value associated with a particular resource ID,
     * substituting the format arguments as defined in {@link java.util.Formatter}
     * and {@link java.lang.String#format}. It will be stripped of any styled text
     * information.
     * {@more}
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @param formatArgs The format arguments that will be used for substitution.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return String The string data associated with the resource,
     * stripped of styled text information.
     */
    CARAPI GetString(
        /* [in] */ Int32 id,
        /* [in] */ ArrayOf<IInterface*>* formatArgs,
        /* [out] */ String* str);

    /**
     * Return the string value associated with a particular resource ID for a particular
     * numerical quantity, substituting the format arguments as defined in
     * {@link java.util.Formatter} and {@link java.lang.String#format}. It will be
     * stripped of any styled text information.
     * {@more}
     *
     * <p>See <a href="{@docRoot}guide/topics/resources/string-resource.html#Plurals">String
     * Resources</a> for more on quantity strings.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     * @param quantity The number used to get the correct string for the current language's
     *           plural rules.
     * @param formatArgs The format arguments that will be used for substitution.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return String The string data associated with the resource,
     * stripped of styled text information.
     */
    CARAPI GetQuantityString(
        /* [in] */ Int32 id,
        /* [in] */ Int32 quantity,
        /* [in] */ ArrayOf<IInterface*>* formatArgs,
        /* [out] */ String* str);

    /**
     * Return the string value associated with a particular resource ID for a particular
     * numerical quantity.
     *
     * <p>See <a href="{@docRoot}guide/topics/resources/string-resource.html#Plurals">String
     * Resources</a> for more on quantity strings.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     * @param quantity The number used to get the correct string for the current language's
     *           plural rules.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return String The string data associated with the resource,
     * stripped of styled text information.
     */
    CARAPI GetQuantityString(
        /* [in] */ Int32 id,
        /* [in] */ Int32 quantity,
        /* [out] */ String* str);

    /**
     * Return the string value associated with a particular resource ID.  The
     * returned object will be a String if this is a plain string; it will be
     * some other type of CharSequence if it is styled.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @param def The default CharSequence to return.
     *
     * @return CharSequence The string data associated with the resource, plus
     *         possibly styled text information, or def if id is 0 or not found.
     */
    CARAPI GetText(
        /* [in] */ Int32 id,
        /* [in] */ ICharSequence* def,
        /* [out] */ ICharSequence** seq);

    /**
     * Return the styled text array associated with a particular resource ID.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return The styled text array associated with the resource.
     */
    CARAPI GetTextArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<ICharSequence*>** seqs);

    /**
     * Return the string array associated with a particular resource ID.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return The string array associated with the resource.
     */
    CARAPI GetStringArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<String>** strs);

    /**
     * Return the int array associated with a particular resource ID.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return The int array associated with the resource.
     */
    CARAPI GetIntArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<Int32>** ints);

    /**
     * Return an array of heterogeneous values.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return Returns a TypedArray holding an array of the array values.
     * Be sure to call {@link TypedArray#recycle() TypedArray.recycle()}
     * when done with it.
     */
    CARAPI ObtainTypedArray(
        /* [in] */ Int32 id,
        /* [out] */ ITypedArray** array);

    /**
     * Retrieve a dimensional for a particular resource ID.  Unit
     * conversions are based on the current {@link DisplayMetrics} associated
     * with the resources.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @return Resource dimension value multiplied by the appropriate
     * metric.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getDimensionPixelOffset
     * @see #getDimensionPixelSize
     */
    CARAPI GetDimension(
        /* [in] */ Int32 id,
        /* [out] */ Float* dim);

    /**
     * Retrieve a dimensional for a particular resource ID for use
     * as an offset in raw pixels.  This is the same as
     * {@link #getDimension}, except the returned value is converted to
     * integer pixels for you.  An offset conversion involves simply
     * truncating the base value to an integer.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @return Resource dimension value multiplied by the appropriate
     * metric and truncated to integer pixels.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getDimension
     * @see #getDimensionPixelSize
     */
    CARAPI GetDimensionPixelOffset(
        /* [in] */ Int32 id,
        /* [out] */ Int32* offset);

    /**
     * Retrieve a dimensional for a particular resource ID for use
     * as a size in raw pixels.  This is the same as
     * {@link #getDimension}, except the returned value is converted to
     * integer pixels for use as a size.  A size conversion involves
     * rounding the base value, and ensuring that a non-zero base value
     * is at least one pixel in size.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @return Resource dimension value multiplied by the appropriate
     * metric and truncated to integer pixels.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getDimension
     * @see #getDimensionPixelOffset
     */
    CARAPI GetDimensionPixelSize(
        /* [in] */ Int32 id,
        /* [out] */ Int32* size);

    /**
     * Retrieve a fractional unit for a particular resource ID.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     * @param base The base value of this fraction.  In other words, a
     *             standard fraction is multiplied by this value.
     * @param pbase The parent base value of this fraction.  In other
     *             words, a parent fraction (nn%p) is multiplied by this
     *             value.
     *
     * @return Attribute fractional value multiplied by the appropriate
     * base value.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     */
    CARAPI GetFraction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 base,
        /* [in] */ Int32 pbase,
        /* [out] */ Float* fraction);

    /**
     * Return a drawable object associated with a particular resource ID.
     * Various types of objects will be returned depending on the underlying
     * resource -- for example, a solid color, PNG image, scalable image, etc.
     * The Drawable API hides these implementation details.
     *
     * <p class="note"><strong>Note:</strong> Prior to
     * {@link android.os.Build.VERSION_CODES#JELLY_BEAN}, this function
     * would not correctly retrieve the final configuration density when
     * the resource ID passed here is an alias to another Drawable resource.
     * This means that if the density configuration of the alias resource
     * is different than the actual resource, the density of the returned
     * Drawable would be incorrect, resulting in bad scaling.  To work
     * around this, you can instead retrieve the Drawable through
     * {@link TypedArray#getDrawable TypedArray.getDrawable}.  Use
     * {@link android.content.Context#obtainStyledAttributes(int[])
     * Context.obtainStyledAttributes} with
     * an array containing the resource ID of interest to create the TypedArray.</p>
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return Drawable An object that can be used to draw this resource.
     */
    CARAPI GetDrawable(
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    /**
     * Return a drawable object associated with a particular resource ID for the
     * given screen density in DPI. This will set the drawable's density to be
     * the device's density multiplied by the ratio of actual drawable density
     * to requested density. This allows the drawable to be scaled up to the
     * correct size if needed. Various types of objects will be returned
     * depending on the underlying resource -- for example, a solid color, PNG
     * image, scalable image, etc. The Drawable API hides these implementation
     * details.
     *
     * @param id The desired resource identifier, as generated by the aapt tool.
     *            This integer encodes the package, type, and resource entry.
     *            The value 0 is an invalid identifier.
     * @param density the desired screen density indicated by the resource as
     *            found in {@link DisplayMetrics}.
     * @throws NotFoundException Throws NotFoundException if the given ID does
     *             not exist.
     * @return Drawable An object that can be used to draw this resource.
     */
    CARAPI GetDrawableForDensity(
        /* [in] */ Int32 id,
        /* [in] */ Int32 density,
        /* [out] */ IDrawable** drawable);

    /**
     * Return a movie object associated with the particular resource ID.
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     */
    CARAPI GetMovie(
        /* [in] */ Int32 id,
        /* [out] */ IMovie** movie);

    /**
     * Return a color integer associated with a particular resource ID.
     * If the resource holds a complex
     * {@link android.content.res.ColorStateList}, then the default color from
     * the set is returned.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return Returns a single color value in the form 0xAARRGGBB.
     */
    CARAPI GetColor(
        /* [in] */ Int32 id,
        /* [out] */ Int32* color);

    /**
     * Return a color state list associated with a particular resource ID.  The
     * resource may contain either a single raw color value, or a complex
     * {@link android.content.res.ColorStateList} holding multiple possible colors.
     *
     * @param id The desired resource identifier of a {@link ColorStateList},
     *        as generated by the aapt tool. This integer encodes the package, type, and resource
     *        entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return Returns a ColorStateList object containing either a single
     * solid color or multiple colors that can be selected based on a state.
     */
    CARAPI GetColorStateList(
        /* [in] */ Int32 id,
        /* [out] */ IColorStateList** list);

    /**
     * Return a boolean associated with a particular resource ID.  This can be
     * used with any integral resource value, and will return true if it is
     * non-zero.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return Returns the boolean value contained in the resource.
     */
    CARAPI GetBoolean(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* b);

    /**
     * Return an integer associated with a particular resource ID.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return Returns the integer value contained in the resource.
     */
    CARAPI GetInteger(
        /* [in] */ Int32 id,
        /* [out] */ Int32* i);

    /**
     * Return an XmlResourceParser through which you can read a view layout
     * description for the given resource ID.  This parser has limited
     * functionality -- in particular, you can't change its input, and only
     * the high-level events are available.
     *
     * <p>This function is really a simple wrapper for calling
     * {@link #getXml} with a layout resource.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return A new parser object through which you can read
     *         the XML data.
     *
     * @see #getXml
     */
    CARAPI GetLayout(
        /* [in] */ Int32 id,
        /* [out] */ IXmlResourceParser** parser);

    /**
     * Return an XmlResourceParser through which you can read an animation
     * description for the given resource ID.  This parser has limited
     * functionality -- in particular, you can't change its input, and only
     * the high-level events are available.
     *
     * <p>This function is really a simple wrapper for calling
     * {@link #getXml} with an animation resource.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return A new parser object through which you can read
     *         the XML data.
     *
     * @see #getXml
     */
    CARAPI GetAnimation(
        /* [in] */ Int32 id,
        /* [out] */ IXmlResourceParser** animation);

    /**
     * Return an XmlResourceParser through which you can read a generic XML
     * resource for the given resource ID.
     *
     * <p>The XmlPullParser implementation returned here has some limited
     * functionality.  In particular, you can't change its input, and only
     * high-level parsing events are available (since the document was
     * pre-parsed for you at build time, which involved merging text and
     * stripping comments).
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @return A new parser object through which you can read
     *         the XML data.
     *
     * @see android.util.AttributeSet
     */
    CARAPI GetXml(
        /* [in] */ Int32 id,
        /* [out] */ IXmlResourceParser** xml);

    /**
     * Open a data stream for reading a raw resource.  This can only be used
     * with resources whose value is the name of an asset files -- that is, it can be
     * used to open drawable, sound, and raw resources; it will fail on string
     * and color resources.
     *
     * @param id The resource identifier to open, as generated by the appt
     *           tool.
     *
     * @return InputStream Access to the resource data.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     */
    CARAPI OpenRawResource(
        /* [in] */ Int32 id,
        /* [out] */ IInputStream** res);

    /**
     * Open a data stream for reading a raw resource.  This can only be used
     * with resources whose value is the name of an asset file -- that is, it can be
     * used to open drawable, sound, and raw resources; it will fail on string
     * and color resources.
     *
     * @param id The resource identifier to open, as generated by the appt tool.
     * @param value The TypedValue object to hold the resource information.
     *
     * @return InputStream Access to the resource data.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     */
    CARAPI OpenRawResource(
        /* [in] */ Int32 id,
        /* [in] */ ITypedValue* value,
        /* [out] */ IInputStream** res);

    /**
     * Open a file descriptor for reading a raw resource.  This can only be used
     * with resources whose value is the name of an asset files -- that is, it can be
     * used to open drawable, sound, and raw resources; it will fail on string
     * and color resources.
     *
     * <p>This function only works for resources that are stored in the package
     * as uncompressed data, which typically includes things like mp3 files
     * and png images.
     *
     * @param id The resource identifier to open, as generated by the appt
     *           tool.
     *
     * @return AssetFileDescriptor A new file descriptor you can use to read
     * the resource.  This includes the file descriptor itself, as well as the
     * offset and length of data where the resource appears in the file.  A
     * null is returned if the file exists but is compressed.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     */
    CARAPI OpenRawResourceFd(
        /* [in] */ Int32 id,
        /* [out] */ IAssetFileDescriptor** des);

    /**
     * Return the raw data associated with a particular resource ID.
     *
     * @param id The desired resource identifier, as generated by the aapt
     *           tool. This integer encodes the package, type, and resource
     *           entry. The value 0 is an invalid identifier.
     * @param outValue Object in which to place the resource data.
     * @param resolveRefs If true, a resource that is a reference to another
     *                    resource will be followed so that you receive the
     *                    actual final resource data.  If false, the TypedValue
     *                    will be filled in with the reference itself.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     */
    CARAPI GetValue(
        /* [in] */ Int32 id,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    /**
     * Get the raw value associated with a resource with associated density.
     *
     * @param id resource identifier
     * @param density density in DPI
     * @param resolveRefs If true, a resource that is a reference to another
     *            resource will be followed so that you receive the actual final
     *            resource data. If false, the TypedValue will be filled in with
     *            the reference itself.
     * @throws NotFoundException Throws NotFoundException if the given ID does
     *             not exist.
     * @see #getValue(String, TypedValue, boolean)
     */
    CARAPI GetValueForDensity(
        /* [in] */ Int32 id,
        /* [in] */ Int32 density,
        /* [in] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    /**
     * Return the raw data associated with a particular resource ID.
     * See getIdentifier() for information on how names are mapped to resource
     * IDs, and getString(int) for information on how string resources are
     * retrieved.
     *
     * <p>Note: use of this function is discouraged.  It is much more
     * efficient to retrieve resources by identifier than by name.
     *
     * @param name The name of the desired resource.  This is passed to
     *             getIdentifier() with a default type of "string".
     * @param outValue Object in which to place the resource data.
     * @param resolveRefs If true, a resource that is a reference to another
     *                    resource will be followed so that you receive the
     *                    actual final resource data.  If false, the TypedValue
     *                    will be filled in with the reference itself.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     */
    CARAPI GetValue(
        /* [in] */ const String& name,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    /**
     * Generate a new Theme object for this set of Resources.  It initially
     * starts out empty.
     *
     * @return Theme The newly created Theme container.
     */
    CARAPI NewTheme(
        /* [out] */ IResourcesTheme** theme);

    /**
     * Retrieve a set of basic attribute values from an AttributeSet, not
     * performing styling of them using a theme and/or style resources.
     *
     * @param set The current attribute values to retrieve.
     * @param attrs The specific attributes to be retrieved.
     * @return Returns a TypedArray holding an array of the attribute values.
     * Be sure to call {@link TypedArray#recycle() TypedArray.recycle()}
     * when done with it.
     *
     * @see Theme#obtainStyledAttributes(AttributeSet, int[], int, int)
     */
    CARAPI ObtainAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** array);

    /**
     * Store the newly updated configuration.
     */
    CARAPI UpdateConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics);

    /**
     * @hide
     */
    CARAPI UpdateConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ ICompatibilityInfo* compat);

    /**
     * Update the system resources configuration if they have previously
     * been initialized.
     *
     * @hide
     */
    static CARAPI_(void) UpdateSystemConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ ICompatibilityInfo* compat);

    /**
     * @hide
     */
    static CARAPI_(void) UpdateSystemConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics);

    /**
     * Return the current display metrics that are in effect for this resource
     * object.  The returned object should be treated as read-only.
     *
     * @return The resource's current display metrics.
     */
    CARAPI GetDisplayMetrics(
        /* [out] */ IDisplayMetrics** metrics);

    /**
     * Return the current configuration that is in effect for this resource
     * object.  The returned object should be treated as read-only.
     *
     * @return The resource's current configuration.
     */
    CARAPI GetConfiguration(
        /* [out] */ IConfiguration** config);

    /**
     * Return the compatibility mode information for the application.
     * The returned object should be treated as read-only.
     *
     * @return compatibility info.
     * @hide
     */
    CARAPI GetCompatibilityInfo(
        /* [out] */ ICompatibilityInfo** ci);

    /**
     * This is just for testing.
     * @hide
     */
    CARAPI SetCompatibilityInfo(
        /* [in] */ ICompatibilityInfo* ci);

    /**
     * Return a resource identifier for the given resource name.  A fully
     * qualified resource name is of the form "package:type/entry".  The first
     * two components (package and type) are optional if defType and
     * defPackage, respectively, are specified here.
     *
     * <p>Note: use of this function is discouraged.  It is much more
     * efficient to retrieve resources by identifier than by name.
     *
     * @param name The name of the desired resource.
     * @param defType Optional default resource type to find, if "type/" is
     *                not included in the name.  Can be null to require an
     *                explicit type.
     * @param defPackage Optional default package to find, if "package:" is
     *                   not included in the name.  Can be null to require an
     *                   explicit package.
     *
     * @return int The associated resource identifier.  Returns 0 if no such
     *         resource was found.  (0 is not a valid resource ID.)
     */
    CARAPI GetIdentifier(
        /* [in] */ const String& name,
        /* [in] */ const String& defType,
        /* [in] */ const String& defCapsule,
        /* [out] */ Int32* id);

    /**
     * Return the full name for a given resource identifier.  This name is
     * a single string of the form "package:type/entry".
     *
     * @param resid The resource identifier whose name is to be retrieved.
     *
     * @return A string holding the name of the resource.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getResourcePackageName
     * @see #getResourceTypeName
     * @see #getResourceEntryName
     */
    CARAPI GetResourceName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    CARAPI GetDrawableResourceName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    /**
     * Return the package name for a given resource identifier.
     *
     * @param resid The resource identifier whose package name is to be
     * retrieved.
     *
     * @return A string holding the package name of the resource.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getResourceName
     */
    CARAPI GetResourcePackageName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    /**
     * Return the type name for a given resource identifier.
     *
     * @param resid The resource identifier whose type name is to be
     * retrieved.
     *
     * @return A string holding the type name of the resource.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getResourceName
     */
    CARAPI GetResourceTypeName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    /**
     * Return the entry name for a given resource identifier.
     *
     * @param resid The resource identifier whose entry name is to be
     * retrieved.
     *
     * @return A string holding the entry name of the resource.
     *
     * @throws NotFoundException Throws NotFoundException if the given ID does not exist.
     *
     * @see #getResourceName
     */
    CARAPI GetResourceEntryName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    /**
     * Parse a series of {@link android.R.styleable#Extra &lt;extra&gt;} tags from
     * an XML file.  You call this when you are at the parent tag of the
     * extra tags, and it will return once all of the child tags have been parsed.
     * This will call {@link #parseBundleExtra} for each extra tag encountered.
     *
     * @param parser The parser from which to retrieve the extras.
     * @param outBundle A Bundle in which to place all parsed extras.
     * @throws XmlPullParserException
     * @throws IOException
     */
    CARAPI ParseBundleExtras(
        /* [in] */ IXmlResourceParser* parser,
        /* [in, out] */ IBundle* outBundle);

    /**
     * Parse a name/value pair out of an XML tag holding that data.  The
     * AttributeSet must be holding the data defined by
     * {@link android.R.styleable#Extra}.  The following value types are supported:
     * <ul>
     * <li> {@link TypedValue#TYPE_STRING}:
     * {@link Bundle#putCharSequence Bundle.putCharSequence()}
     * <li> {@link TypedValue#TYPE_INT_BOOLEAN}:
     * {@link Bundle#putCharSequence Bundle.putBoolean()}
     * <li> {@link TypedValue#TYPE_FIRST_INT}-{@link TypedValue#TYPE_LAST_INT}:
     * {@link Bundle#putCharSequence Bundle.putBoolean()}
     * <li> {@link TypedValue#TYPE_FLOAT}:
     * {@link Bundle#putCharSequence Bundle.putFloat()}
     * </ul>
     *
     * @param tagName The name of the tag these attributes come from; this is
     * only used for reporting error messages.
     * @param attrs The attributes from which to retrieve the name/value pair.
     * @param outBundle The Bundle in which to place the parsed value.
     * @throws XmlPullParserException If the attributes are not valid.
     */
    CARAPI ParseBundleExtra(
        /* [in] */ const String& tagName,
        /* [in] */ IAttributeSet* attrs,
        /* [in, out] */ IBundle* outBundle);

    /**
     * Retrieve underlying AssetManager storage for these resources.
     */
    CARAPI GetAssets(
        /* [out] */ IAssetManager** manager);

    /**
     * Call this to remove all cached loaded layout resources from the
     * Resources object.  Only intended for use with performance testing
     * tools.
     */
    CARAPI FlushLayoutCache();

    /**
     * Start preloading of resource data using this Resources object.  Only
     * for use by the zygote process for loading common system resources.
     * {@hide}
     */
    CARAPI StartPreloading();

    /**
     * Called by zygote when it is done preloading resources, to change back
     * to normal Resources operation.
     */
    CARAPI FinishPreloading();

    /**
     * Create a new Resources object on top of an existing set of assets in an
     * AssetManager.
     *
     * @param assets Previously created AssetManager.
     * @param metrics Current display metrics to consider when
     *                selecting/computing resource values.
     * @param config Desired device configuration to consider when
     *               selecting/computing resource values (optional).
     */
    CARAPI constructor(
        /* [in] */ IAssetManager* assets,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ IConfiguration* config);

    /**
     * Creates a new Resources object with CompatibilityInfo.
     *
     * @param assets Previously created AssetManager.
     * @param metrics Current display metrics to consider when
     *                selecting/computing resource values.
     * @param config Desired device configuration to consider when
     *               selecting/computing resource values (optional).
     * @param compInfo this resource's compatibility info. It will use the default compatibility
     *  info when it's null.
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IAssetManager* assets,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ IConfiguration* config,
        /* [in] */ ICompatibilityInfo* compInfo);

    CARAPI constructor();

    CARAPI LoadDrawable(
        /* [in] */ ITypedValue* value,
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    CARAPI LoadColorStateList(
        /* [in] */ ITypedValue* value,
        /* [in] */ Int32 id,
        /* [out] */ IColorStateList** stateList);

    CARAPI LoadXmlResourceParser(
        /* [in] */ Int32 id,
        /* [in] */ const String& type,
        /* [out] */ IXmlResourceParser** resourceParser);

    CARAPI LoadXmlResourceParser(
        /* [in] */ const String& file,
        /* [in] */ Int32 id,
        /* [in] */ Int32 assetCookie,
        /* [in] */ const String& type,
        /* [out] */ IXmlResourceParser** resourceParser);

private:
    // CARAPI_(AutoPtr<NativePluralRules>) GetPluralRule();

    static CARAPI_(Int32) AttrForQuantityCode(
        /* [in] */ Int32 quantityCode);

    static CARAPI_(String) StringForQuantityCode(
        /* [in] */ Int32 quantityCode);

    CARAPI_(void) ClearDrawableCache(
        /* [in] */ HashMap<Int64, AutoPtr<IWeakReference> >& cache,
        /* [in] */ Int32 configChanges);

    CARAPI_(Boolean) VerifyPreloadConfig(
        /* [in] */ ITypedValue* value,
        /* [in] */ const String& name);

    CARAPI_(AutoPtr<IDrawable>) GetCachedDrawable(
        /* [in] */ HashMap<Int64, AutoPtr<IWeakReference> >& drawableCache,
        /* [in] */ Int64 key);

    CARAPI_(AutoPtr<IColorStateList>) GetCachedColorStateList(
        /* [in] */ Int64 key);

    CARAPI_(AutoPtr<CTypedArray>) GetCachedStyledAttributes(
        /* [in] */ Int32 len);

public: /*package*/
    static const String TAG;
    static const String WIDGET_SUFFIX;

    static AutoPtr<IResources> mSystem;

    AutoPtr<CTypedValue> mTmpValue;
    Mutex mTmpValueLock;
    AutoPtr<CConfiguration> mTmpConfig;
    AutoPtr<CTypedArray> mCachedStyledAttributes;
    AutoPtr<CAssetManager> mAssets;
    AutoPtr<CDisplayMetrics> mMetrics;

private:
    static const Boolean DEBUG_LOAD;
    static const Boolean DEBUG_CONFIG;
    static const Boolean DEBUG_ATTRIBUTES_CACHE;
    static const Boolean TRACE_FOR_PRELOAD;
    static const Boolean TRACE_FOR_MISS_PRELOAD;
    static const Int32   ID_OTHER;

    // Information about preloaded resources.  Note that they are not
    // protected by a lock, because while preloading in zygote we are all
    // single-threaded, and after that these are immutable.
    // private final LongSparseArray<WeakReference<Drawable.ConstantState> > mDrawableCache
    // private final LongSparseArray<WeakReference<ColorStateList> > mColorStateListCache
    // private final LongSparseArray<WeakReference<Drawable.ConstantState> > mColorDrawableCache
    typedef HashMap<Int64, AutoPtr<IWeakReference> > DrawableMap;
    typedef typename HashMap<Int64, AutoPtr<IWeakReference> >::Iterator DrawableMapIterator;
    typedef HashMap<Int64, AutoPtr<IWeakReference> > ColorStateMap;
    typedef typename HashMap<Int64, AutoPtr<IWeakReference> >::Iterator ColorStateIterator;

    static DrawableMap sPreloadedDrawables;
    static ColorStateMap sPreloadedColorStateLists;
    static DrawableMap sPreloadedColorDrawables;
    static Boolean sPreloaded;
    static Int32 sPreloadedDensity;

    static Mutex mSyncLock;

    // These are protected by the mTmpValue lock.
    DrawableMap mDrawableCache;
    ColorStateMap mColorStateListCache;
    DrawableMap mColorDrawableCache;

    Boolean mPreloading;

    Int32 mLastCachedXmlBlockIndex;
    AutoPtr< ArrayOf<Int32> > mCachedXmlBlockIds;
    Mutex mCachedXmlBlockIdsLock;
    AutoPtr< ArrayOf<XmlBlock*> > mCachedXmlBlocks;
    AutoPtr<CConfiguration> mConfiguration;
    // AutoPtr<NativePluralRules> mPluralRule;

    AutoPtr<ICompatibilityInfo> mCompatibilityInfo;
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CRESOURCES_H__
