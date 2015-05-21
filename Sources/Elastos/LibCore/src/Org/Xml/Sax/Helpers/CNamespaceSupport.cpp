
#include "CNamespaceSupport.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CObjectContainer;

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

AutoPtr<IObjectContainer> CNamespaceSupport::EMPTY_ENUMERATION;

CAR_INTERFACE_IMPL(CNamespaceSupport, Object, INamespaceSupport)
CAR_OBJECT_IMPL(CNamespaceSupport)

ECode CNamespaceSupport::Reset()
{
    mContexts = ArrayOf<Context*>::Alloc(32);
    mNamespaceDeclUris = false;
    mContextPos = 0;
    mCurrentContext = new Context(this);
    mContexts->Set(mContextPos, mCurrentContext);
    mCurrentContext->DeclarePrefix(String("xml"), XMLNS);

    return NOERROR;
}

ECode CNamespaceSupport::PushContext()
{
    Int32 max = mContexts->GetLength();

    (*mContexts)[mContextPos]->mDeclsOK = FALSE;
    mContextPos++;

    // Extend the array if necessary
    if (mContextPos >= max) {
        AutoPtr<ArrayOf<Context*> > newContexts = ArrayOf<Context*>::Alloc(max*2);
        newContexts->Copy(0, mContexts, 0, max);
        max *= 2;
        mContexts = newContexts;
    }

    // Allocate the context if necessary.
    mCurrentContext = (*mContexts)[mContextPos];
    if (mCurrentContext == NULL) {
        mCurrentContext = new Context(this);
        mContexts->Set(mContextPos, mCurrentContext);
    }

    // Set the parent, if any.
    if (mContextPos > 0) {
        mCurrentContext->SetParent((*mContexts)[mContextPos - 1]);
    }
    return NOERROR;
}

ECode CNamespaceSupport::PopContext()
{
    (*mContexts)[mContextPos]->Clear();
    mContextPos--;
    if (mContextPos < 0) {
        // throw new EmptyStackException();
        return E_SAX_EMPTYSTACK_EXCEPTION;
    }
    mCurrentContext = (*mContexts)[mContextPos];
    return NOERROR;
}

ECode CNamespaceSupport::DeclarePrefix(
    /* [in] */ const String& prefix,
    /* [in] */ const String& uri,
    /* [out] */ Boolean* isPrefixLegal)
{
    VALIDATE_NOT_NULL(isPrefixLegal)

    if (prefix.Equals("xml") || prefix.Equals("xmlns")) {
        *isPrefixLegal = FALSE;
    }
    else {
        mCurrentContext->DeclarePrefix(prefix, uri);
        *isPrefixLegal = TRUE;
    }
    return NOERROR;
}

ECode CNamespaceSupport::ProcessName(
    /* [in] */ const String& qName,
    /* [in] */ const ArrayOf<String> & parts,
    /* [in] */ Boolean isAttribute,
    /* [out, callee] */ ArrayOf<String>** nName)
{
    VALIDATE_NOT_NULL(nName)

    AutoPtr< ArrayOf<String> > myParts = mCurrentContext->ProcessName(qName, isAttribute);
    if (myParts == NULL) {
        *nName = NULL;
    }
    else {
        *nName = myParts;
        REFCOUNT_ADD(*nName)
    }
    return NOERROR;
}

ECode CNamespaceSupport::GetURI(
    /* [in] */ const String& prefix,
    /* [out] */ String* URI)
{
    VALIDATE_NOT_NULL(URI)

    *URI = mCurrentContext->GetURI(prefix);
    return NOERROR;
}

ECode CNamespaceSupport::GetPrefixes(
    /* [out] */ IObjectContainer** prefixes)
{
    VALIDATE_NOT_NULL(prefixes)

    AutoPtr<IObjectContainer> res = mCurrentContext->GetPrefixes();
    *prefixes = res;
    REFCOUNT_ADD(*prefixes)

    return NOERROR;
}

ECode CNamespaceSupport::GetPrefix(
    /* [in] */ const String& uri,
    /* [out] */ String* prefix)
{
    VALIDATE_NOT_NULL(prefix)

    *prefix = mCurrentContext->GetPrefix(uri);
    return NOERROR;
}

ECode CNamespaceSupport::GetPrefixes(
    /* [in] */ const String& uri,
    /* [out] */ IObjectContainer** prefixes)
{
    VALIDATE_NOT_NULL(prefixes)

    AutoPtr<IObjectContainer> prefixes1;
    CObjectContainer::New((IObjectContainer**)&prefixes1);
    AutoPtr<IObjectContainer> allPrefixes;
    GetPrefixes((IObjectContainer**)&allPrefixes);
    AutoPtr<IObjectEnumerator> enumerator;
    allPrefixes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean isflag = FALSE;
    enumerator->MoveNext(&isflag);
    while(isflag) {
        AutoPtr<ICharSequence> outcsq;
        enumerator->Current((IInterface**)&outcsq);
        String prefix;
        outcsq->ToString(&prefix);
        String outuri;
        GetURI(prefix,&outuri);
        if (uri.Equals(outuri))
        {
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(prefix, (ICharSequence**)&csq);
            prefixes1->Add(csq);
        }
        enumerator->MoveNext(&isflag);
    }
    *prefixes = prefixes1;
    REFCOUNT_ADD(*prefixes)

    return NOERROR;
}

ECode CNamespaceSupport::GetDeclaredPrefixes(
    /* [out] */ IObjectContainer** prefixes)
{
    VALIDATE_NOT_NULL(prefixes)

    AutoPtr<IObjectContainer> res = mCurrentContext->GetDeclaredPrefixes();
    *prefixes = res;
    REFCOUNT_ADD(*prefixes)

    return NOERROR;
}

ECode CNamespaceSupport::SetNamespaceDeclUris(
    /* [in] */ Boolean value)
{
    if (mContextPos != 0) {
        // throw new IllegalStateException ();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (value == mNamespaceDeclUris)
        return NOERROR;
    mNamespaceDeclUris = value;
    if (value)
        mCurrentContext->DeclarePrefix(String("xmlns"), NSDECL);
    else {
        mCurrentContext = new Context(this);
        (*mContexts)[mContextPos] = mCurrentContext;
        mCurrentContext->DeclarePrefix(String("xml"), XMLNS);
    }
    return NOERROR;
}

ECode CNamespaceSupport::IsNamespaceDeclUris(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mNamespaceDeclUris;
    return NOERROR;
}

ECode CNamespaceSupport::constructor()
{
    CObjectContainer::New((IObjectContainer**)&EMPTY_ENUMERATION);
    return Reset();
}

CNamespaceSupport::Context::Context(
    /* [in] */ CNamespaceSupport* parent)
    : mDeclarations()
{
    mParentHolder = parent;
    mDeclSeen = FALSE;
    mParentContext = NULL;

    mDefaultNS = NULL;
    mDeclsOK = TRUE;

    CopyTables();
}

CNamespaceSupport::Context::~Context()
{
}

PInterface CNamespaceSupport::Context::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CNamespaceSupport::Context::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CNamespaceSupport::Context::Release()
{
    return ElRefBase::Release();
}

ECode CNamespaceSupport::Context::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);

    if (object == (IInterface*)this) {
        *iid = EIID_IInterface;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

void CNamespaceSupport::Context::SetParent (
    /* [in] */ Context* parent)
{
    mParentContext = parent;
    mPrefixTable = parent->mPrefixTable;
    mUriTable = parent->mUriTable;
    mElementNameTable = parent->mElementNameTable;
    mAttributeNameTable = parent->mAttributeNameTable;
    mDefaultNS = parent->mDefaultNS;
    mDeclSeen = FALSE;
    mDeclsOK = TRUE;
}

void CNamespaceSupport::Context::Clear ()
{
    mParentContext = NULL;
    mDefaultNS = String(NULL);
}


void CNamespaceSupport::Context::DeclarePrefix(
    /* [in] */ const String& prefix,
    /* [in] */ const String& uri)
{
    // Lazy processing...
    if (!mDeclsOK) {
        // throw new IllegalStateException ("can't declare any more prefixes in this context");
        return;
    }
    if (!mDeclSeen) {
       CopyTables();
    }

    // prefix = prefix.intern();
    // uri = uri.intern();
    if (String("").Equals(prefix)) {
        if (String("").Equals(uri)) {
            mDefaultNS = String(NULL);
        }
        else {
            mDefaultNS = uri;
        }
    }
    else {
        mPrefixTable[prefix] = uri;
        mUriTable[uri] = prefix; // may wipe out another prefix
    }
    mDeclarations.PushBack(prefix);
}

AutoPtr< ArrayOf<String> > CNamespaceSupport::Context::ProcessName (
    /* [in] */ const String& qName,
    /* [in] */ Boolean isAttribute)
{
    AutoPtr< ArrayOf<String> > name;
    HashMap<String, AutoPtr< ArrayOf<String> > >* table;

    // detect errors in call sequence
    mDeclsOK = FALSE;

    // Select the appropriate table.
    if (isAttribute) {
        table = &mAttributeNameTable;
    }
    else {
        table = &mElementNameTable;
    }

    // Start by looking in the cache, and
    // return immediately if the name
    // is already known in this content
    name = (*table)[qName];
    if (name != NULL) {
        return name;
    }

    // We haven't seen this name in this
    // context before.  Maybe in the parent
    // context, but we can't assume prefix
    // bindings are the same.
    name = ArrayOf<String>::Alloc(3);
    (*name)[2] = qName;// qName.intern();
    Int32 index = qName.IndexOf(':');


           // No prefix.
    if (index == -1) {
        if (isAttribute) {
            if (qName == "xmlns" && mParentHolder->mNamespaceDeclUris)
                (*name)[0] = NSDECL;
            else
                (*name)[0] = "";
        }
        else if (mDefaultNS.IsNull()) {
            (*name)[0] = "";
        }
        else {
            (*name)[0] = mDefaultNS;
        }
        (*name)[1] = (*name)[2];
    }
    else { // Prefix
        String prefix = qName.Substring(0, index);
        String local = qName.Substring(index+1);
        String uri;
        if (String("").Equals(prefix)) {
            uri = mDefaultNS;
        }
        else {
            uri = mPrefixTable[prefix];
        }
        if (uri.IsNull() || (!isAttribute && String("xmlns").Equals(prefix))) {
           return NULL;
        }
        (*name)[0] = uri;
        (*name)[1] = local; // local.intern();
    }

    // Save in the cache for future use.
    // (Could be shared with parent context...)
    (*table)[(*name)[2]] = name;
    return name;
}


String CNamespaceSupport::Context::GetURI (
    /* [in] */ const String& prefix)
{
    if (String("").Equals(prefix)) {
        return mDefaultNS;
    }
    else if (mPrefixTable.IsEmpty()) {
        return String(NULL);
    }
    else {
        return mPrefixTable[prefix];
    }
    return String("");
}

String CNamespaceSupport::Context::GetPrefix (
    /* [in] */ const String& uri)
{
    if (mUriTable.IsEmpty()) {
        return String(NULL);
    } else {
        return mUriTable[uri];
    }
    return String("");
}

AutoPtr<IObjectContainer> CNamespaceSupport::Context::GetDeclaredPrefixes() {
    // return (declarations == null) ? EMPTY_ENUMERATION : Collections.enumeration(declarations);
    if (mDeclarations.IsEmpty()) {
        return EMPTY_ENUMERATION;
    }
    else {
        AutoPtr<IObjectContainer> obj;
        CObjectContainer::New((IObjectContainer**)&obj);
        for (Int32 i = 0; i < mDeclarations.GetSize(); ++i)
        {
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(mDeclarations[i], (ICharSequence**)&csq);
            obj->Add(csq);
        }
        return obj;
    }
}

AutoPtr<IObjectContainer> CNamespaceSupport::Context::GetPrefixes ()
{
    if (mPrefixTable.IsEmpty()) {
        return EMPTY_ENUMERATION;
    } else {
        // return mPrefixTable.keys();
        AutoPtr<IObjectContainer> obj;
        CObjectContainer::New((IObjectContainer**)&obj);
        HashMap<String, String>::Iterator iter = mPrefixTable.Begin();
        while (iter != mPrefixTable.End()) {
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(iter->mFirst, (ICharSequence**)&csq);
            obj->Add(csq);
            iter++;
        }
        return obj;
    }
}

void CNamespaceSupport::Context::CopyTables ()
{
    if (!mPrefixTable.IsEmpty()) {
        assert(0 && "TODO");
        // mPrefixTable = (Hashtable)mPrefixTable.clone();
    }
    else {
        // mPrefixTable = new Hashtable();
    }
    if (!mUriTable.IsEmpty()) {
        // mUriTable = (Hashtable)mUriTable.clone();
    }
    else {
        // mUriTable = new Hashtable();
    }
    mElementNameTable.Clear();
    mAttributeNameTable.Clear();
    mDeclSeen = TRUE;
}

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org
