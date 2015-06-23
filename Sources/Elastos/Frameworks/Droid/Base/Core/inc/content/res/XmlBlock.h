
#ifndef __XMLBLOCK_H__
#define __XMLBLOCK_H__

#include "content/res/StringBlock.h"

using Elastos::Core::Mutex;
using Elastos::IO::IInputStream;
using Elastos::IO::IReader;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/**
 * Wrapper around a compiled XML file.
 *
 * {@hide}
 */
class XmlBlock : public ElRefBase
{
public:
    class Parser :
        public ElRefBase,
        public IXmlResourceParser,
        public IAttributeSet
    {
    public:
        Parser(
            /* [in] */ Int32 parseState,
            /* [in] */ XmlBlock* block,
            /* [in] */ const String& path);

        ~Parser();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI SetFeature(
            /* [in] */ const String& name,
            /* [in] */ Boolean state);

        CARAPI GetFeature(
            /* [in] */ const String& name,
            /* [out] */ Boolean* value);

        CARAPI SetProperty(
            /* [in] */ const String& name,
            /* [in] */ IInterface* value);

        CARAPI GetProperty(
            /* [in] */ const String& name,
            /* [out] */ IInterface** value);

        CARAPI SetInput(
            /* [in]*/ IReader* reader);

        CARAPI SetInput(
            /* [in] */ IInputStream* pInputStream,
            /* [in] */ const String& inputEncoding);

        CARAPI DefineEntityReplacementText(
            /* [in] */ const String& entityName,
            /* [in] */ const String& replacementText);

        CARAPI GetNamespacePrefix(
            /* [in] */ Int32 pos,
            /* [out] */ String* prefix);

        CARAPI GetInputEncoding(
            /* [out] */ String* inputEncoding);

        CARAPI GetNamespace(
            /* [in] */ const String& prefix,
            /* [out] */ String* ns);

        CARAPI GetNamespaceCount(
            /* [in] */ Int32 depth,
            /* [out] */ Int32* count);

        CARAPI GetPositionDescription(
            /* [out] */ String* des);

        CARAPI GetNamespaceUri(
            /* [in] */ Int32 pos,
            /* [out] */ String* uri);

        CARAPI GetColumnNumber(
            /* [out] */ Int32* number);

        CARAPI GetDepth(
            /* [out] */ Int32* depth);

        CARAPI GetText(
            /* [out] */ String* text);

        CARAPI GetLineNumber(
            /* [out] */ Int32* number);

        CARAPI GetEventType(
            /* [out] */ Int32* type);

        CARAPI IsWhitespace(
            /* [out] */ Boolean* result);

        CARAPI GetPrefix(
            /* [out] */ String* prefix);

        CARAPI GetTextCharacters(
            /* [out] */ ArrayOf<Int32>* holderForStartAndLength,
            /* [out, callee] */ ArrayOf<Char32>** textChars);

        CARAPI GetNamespace(
            /* [out] */ String* ns);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetXmlPath(
            /* [out] */ String* name);

        CARAPI GetAttributeNamespace(
            /* [in] */ Int32 index,
            /* [out] */ String* attrNamespace);

        CARAPI GetAttributeName(
            /* [in] */ Int32 index,
            /* [out] */ String* attrName);

        CARAPI GetAttributePrefix(
            /* [in] */ Int32 index,
            /* [out] */ String* attrPrefix);

        CARAPI IsEmptyElementTag(
            /* [out] */ Boolean* result);

        CARAPI GetAttributeCount(
            /* [out] */ Int32* attrCount);

        CARAPI GetAttributeValue(
            /* [in] */ Int32 index,
            /* [out] */ String* attrValue);

        CARAPI GetAttributeType(
            /* [in] */ Int32 index,
            /* [out] */ String* attrType);

        CARAPI IsAttributeDefault(
            /* [in] */ Int32 index,
            /* [out] */ Boolean* result);

        CARAPI NextToken(
            /* [out] */ Int32* token);

        CARAPI GetAttributeValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& name,
            /* [out] */ String* attrValue);

        CARAPI Next(
            /* [out] */ Int32* event);

        CARAPI Require(
            /* [in] */ Int32 type,
            /* [in] */ const String& ns,
            /* [in] */ const String& name);

        CARAPI NextText(
            /* [out] */ String* text);

        CARAPI NextTag(
            /* [out] */ Int32* tag);

        CARAPI GetAttributeNameResource(
            /* [in] */ Int32 index,
            /* [out] */ Int32* resId);

        CARAPI GetAttributeListValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& attribute,
            /* [in] */ const ArrayOf<String>& options,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeBooleanValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& attribute,
            /* [in] */ Boolean defaultValue,
            /* [out] */ Boolean* value);

        CARAPI GetAttributeResourceValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& attribute,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeIntValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& attribute,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeUnsignedIntValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& attribute,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeFloatValue(
            /* [in] */ const String& ns,
            /* [in] */ const String& attribute,
            /* [in] */ Float defaultValue,
            /* [out] */ Float* value);

        CARAPI GetAttributeListValue(
            /* [in] */ Int32 idx,
            /* [in] */ const ArrayOf<String>& options,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeBooleanValue(
            /* [in] */ Int32 idx,
            /* [in] */ Boolean defaultValue,
            /* [out] */ Boolean* value);

        CARAPI GetAttributeResourceValue(
            /* [in] */ Int32 idx,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeIntValue(
            /* [in] */ Int32 idx,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeUnsignedIntValue(
            /* [in] */ Int32 idx,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetAttributeFloatValue(
            /* [in] */ Int32 idx,
            /* [in] */ Float defaultValue,
            /* [out] */ Float* value);

        CARAPI GetIdAttribute(
            /* [out] */ String* attr);

        CARAPI GetClassAttribute(
            /* [out] */ String* attr);

        CARAPI GetIdAttributeResourceValue(
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32* value);

        CARAPI GetStyleAttribute(
            /* [out] */ Int32* value);

        CARAPI Close();

        CARAPI GetPooledString(
            /* [in] */ Int32 id,
            /* [out] */ ICharSequence** csq);

    public:
        Int32 mParseState;

    private:
        AutoPtr<XmlBlock> mHost;
        Boolean mStarted;
        Boolean mDecNextDepth;
        Int32 mDepth;
        Int32 mEventType;
        String mXmlPath;
    };

public:
    XmlBlock(
        /* [in] */ const ArrayOf<Byte>& data);

    XmlBlock(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size);

    /**
     * Create from an existing xml block native object.  This is
     * -extremely- dangerous -- only use it if you absolutely know what you
     *  are doing!  The given native object must exist for the entire lifetime
     *  of this newly creating XmlBlock.
     */
    XmlBlock(
        /* [in] */ IAssetManager* assets,
        /* [in] */ Int32 xmlBlock);

    ~XmlBlock();

    CARAPI_(void) Close();

    CARAPI_(AutoPtr<IXmlResourceParser>) NewParser(
        /* [in] */ const String& filename);

    CARAPI_(Int32) GetHashCode();

    static CARAPI_(Int32) NativeNext(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetName(
        /* [in] */ Int32 parser);

private:
    CARAPI_(void) DecOpenCountLocked();

    static CARAPI_(Int32) NativeCreate(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size);

    static CARAPI_(Int32) NativeGetStringBlock(
        /* [in] */ Int32 xmlTree);

    static CARAPI_(Int32) NativeCreateParseState(
        /* [in] */ Int32 xmlTree);

    static CARAPI_(Int32) NativeGetNamespace(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetText(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetLineNumber(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetAttributeCount(
        /* [in] */ Int32 parsere);

    static CARAPI_(Int32) NativeGetAttributeNamespace(
        /* [in] */ Int32 parser,
        /* [in] */ Int32 idx);

    static CARAPI_(Int32) NativeGetAttributeName(
        /* [in] */ Int32 parser,
        /* [in] */ Int32 idx);

    static CARAPI_(Int32) NativeGetAttributeResource(
        /* [in] */ Int32 parser,
        /* [in] */ Int32 idx);

    static CARAPI_(Int32) NativeGetAttributeDataType(
        /* [in] */ Int32 parser,
        /* [in] */ Int32 idx);

    static CARAPI_(Int32) NativeGetAttributeData(
        /* [in] */ Int32 parser,
        /* [in] */ Int32 idx);

    static CARAPI_(Int32) NativeGetAttributeStringValue(
        /* [in] */ Int32 parser,
        /* [in] */ Int32 idx);

    static CARAPI_(Int32) NativeGetIdAttribute(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetClassAttribute(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetStyleAttribute(
        /* [in] */ Int32 parser);

    static CARAPI_(Int32) NativeGetAttributeIndex(
        /* [in] */ Int32 parser,
        /* [in] */ const String& ns,
        /* [in] */ const String& name);

    static CARAPI_(void) NativeDestroyParseState(
        /* [in] */ Int32 parser);

    static CARAPI_(void) NativeDestroy(
        /* [in] */ Int32 obj);

protected:
    AutoPtr<StringBlock> mStrings;

private:
    static const String TAG;
    static const Boolean DEBUG;

    AutoPtr<IAssetManager> mAssets;
    Int32 mNative;
    Int32 mOpenCount;
    Mutex mSyncLock;
    Boolean mOpen;
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __XMLBLOCK_H__
