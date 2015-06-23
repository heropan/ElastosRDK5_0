
#ifndef __STRINGBLOCK_H__
#define __STRINGBLOCK_H__

#include "ext/frameworkext.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/etl/HashMap.h>


using Elastos::Core::ICharSequence;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Graphics::IPaintFontMetricsInt;
using Elastos::Droid::Text::ITextPaint;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::Style::ILineHeightSpanWithDensity;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/**
 * Conveniences for retrieving data out of a compiled string resource.
 *
 * {@hide}
 */
class StringBlock : public ElRefBase
{
public:
    class StyleIDs : public ElRefBase
    {
    public:
        StyleIDs();

    public:
        Int32 boldId;
        Int32 italicId;
        Int32 underlineId;
        Int32 ttId;
        Int32 bigId;
        Int32 smallId;
        Int32 subId;
        Int32 supId;
        Int32 strikeId;
        Int32 listItemId;
        Int32 marqueeId;
    };

    class Height
        : public ElRefBase
        , public ILineHeightSpanWithDensity
    {
    public:
        Height(
            /* [in] */ Int32 size);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI ChooseHeight(
            /* [in] */ ICharSequence* text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Int32 spanstartv,
            /* [in] */ Int32 v,
            /* [in] */ IPaintFontMetricsInt* fm);

        CARAPI ChooseHeight(
            /* [in] */ ICharSequence* text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Int32 spanstartv,
            /* [in] */ Int32 v,
            /* [in] */ IPaintFontMetricsInt* fm,
            /* [in] */ ITextPaint* paint);
    private:
        Int32 mSize;
        static Float sProportion;
    };

public:
    StringBlock(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Boolean useSparse);

    StringBlock(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size,
        /* [in] */ Boolean useSparse);

    /**
     * Create from an existing string block native object.  This is
     * -extremely- dangerous -- only use it if you absolutely know what you
     *  are doing!  The given native object must exist for the entire lifetime
     *  of this newly creating StringBlock.
     */
    StringBlock(
        /* [in] */ Int32 obj,
        /* [in] */ Boolean useSparse);

    ~StringBlock();

    CARAPI_(AutoPtr<ICharSequence>) Get(
        /* [in] */ Int32 idx);

private:
    CARAPI_(AutoPtr<ICharSequence>) ApplyStyles(
            /* [in] */ const String& str,
            /* [in] */ ArrayOf<Int32>* style,
            /* [in] */ StyleIDs* ids);

    /**
     * If a translator has messed up the edges of paragraph-level markup,
     * fix it to actually cover the entire paragraph that it is attached to
     * instead of just whatever range they put it on.
     */
    static CARAPI_(void) AddParagraphSpan(
            /* [in] */ ISpannable* buffer,
            /* [in] */ IInterface* what,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end);

    static CARAPI_(String) Subtag(
            /* [in] */ const String& full,
            /* [in] */ const String& attribute);

    static CARAPI_(Int32) NativeCreate(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size);

    static CARAPI_(Int32) NativeGetSize(
        /* [in] */ Int32 obj);

    static CARAPI_(String) NativeGetString(
        /* [in] */ Int32 obj,
        /* [in] */ Int32 idx);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) NativeGetStyle(
        /* [in] */ Int32 obj,
        /* [in] */ Int32 idx);

    static CARAPI_(void) NativeDestroy(
        /* [in] */ Int32 obj);

private:
    static const String TAG;
    static const Boolean sLocalLOGV;

    Int32 mNative;
    AutoPtr< ArrayOf<ICharSequence*> > mStrings;
    AutoPtr< HashMap<Int32, AutoPtr<ICharSequence> > > mSparseStrings;
    AutoPtr<StyleIDs> mStyleIDs;
    Mutex mSyncLock;
    Boolean mUseSparse;
    Boolean mOwnsNative;
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __STRINGBLOCK_H__
