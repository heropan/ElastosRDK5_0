
#include "AbstractPreferences.h"
#include "CLinkedList.h"

namespace Elastos {
namespace Utility {
namespace Prefs {

const Int64 AbstractPreferences::NodeAddEvent::serialVersionUID = 1L;
AbstractPreferences::NodeAddEvent::NodeAddEvent(
    /* [in] */ Preferences* p,
    /* [in] */ Preferences* c)
    : NodeChangeEvent(p, c)
{}

const Int64 AbstractPreferences::NodeRemoveEvent::serialVersionUID = 1L;
AbstractPreferences::NodeRemoveEvent::NodeRemoveEvent(
    /* [in] */ Preferences* p,
    /* [in] */ Preferences* c)
    : NodeChangeEvent(p, c)
{}

Boolean AbstractPreferences::sStaticInit = AbstractPreferences::staticInit();
AutoPtr<IList> AbstractPreferences::sEvents;
// AutoPtr<EventDispatcher> AbstractPreferences::sDispatcher;

Boolean AbstractPreferences::staticInit()
{
    CLinkedList::New((IList**) &sEvents);
    //TODO
    assert(0);
    // sDispatcher = new EventDispatcher("Preference Event Dispatcher");

    /*
     * -----------------------------------------------------------
     * Class initializer
     * -----------------------------------------------------------
     */
    // static {
    //     sDispatcher.setDaemon(true);
    //     sDispatcher.start();
    //     Runtime.getRuntime().addShutdownHook(new Thread() {
    //         @Override
    //         public void run() {
    //             Preferences uroot = Preferences.userRoot();
    //             Preferences sroot = Preferences.systemRoot();
    //             try {
    //                 uroot.flush();
    //             } catch (BackingStoreException e) {
    //                 // ignore
    //             }
    //             try {
    //                 sroot.flush();
    //             } catch (BackingStoreException e) {
    //                 // ignore
    //             }
    //         }
    //     });
    // }

    return TRUE;
}

AbstractPreferences::AbstractPreferences(
    /* [in] */ AbstractPreferences* parent,
    /* [in] */ const String& name)
{
    if ((parent == NULL ^ name.GetLength() == 0) || name.IndexOf("/") >= 0) {
        // throw new IllegalArgumentException();
        assert(0);
    }
    if (parent == NULL) {
        mRoot = this;
    } else {
        mRoot = parent->mRoot;
    }
    CLinkedList::New((ILinkedList**)&mNodeChangeListeners);
    CLinkedList::New((ILinkedList**)&mPreferenceChangeListeners);
    mIsRemoved = FALSE;
    mNodeName = name;
    mParentPref = parent;
    mUserNode = mRoot->mUserNode;
}

AutoPtr<ArrayOf<AutoPtr<AbstractPreferences> > > AbstractPreferences::CachedChildren()
{
    AutoPtr<ArrayOf<AutoPtr<AbstractPreferences> > > absps
        = ArrayOf<AutoPtr<AbstractPreferences> >::Alloc(mCachedNode.GetSize());

    HashMap<String, AutoPtr<AbstractPreferences> >::Iterator iter = mCachedNode.Begin();
    for (Int32 i = 0; iter != mCachedNode.End(); ++iter, i++) {
        absps->Set(i, iter->mSecond);
    }

    return absps;
}

ECode AbstractPreferences::GetChild(
    /* [in] */ const String& name,
    /* [out] */ AbstractPreferences** apfs)
{
    VALIDATE_NOT_NULL(apfs);
    ECode ec = NOERROR;
    *apfs = NULL;
    synchronized (mLock) {
        FAIL_RETURN(CheckState());
        AutoPtr<AbstractPreferences> result;
        AutoPtr<ArrayOf<String> > childrenNames;
        FAIL_RETURN(ChildrenNames((ArrayOf<String>**)&childrenNames));
        for (Int32 i = 0; i < childrenNames->GetLength(); i++) {
            if ((*childrenNames)[i].Equals(name)) {
                result = ChildSpi(name);
                break;
            }
        }

        *apfs = result;
        REFCOUNT_ADD(*apfs);
    }

    return NOERROR;
}

Boolean AbstractPreferences::IsRemoved()
{
    synchronized (mLock) {
        return mIsRemoved;
    }
}

ECode AbstractPreferences::AbsolutePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    if (mParentPref == NULL) {
        *path = String("/");
        return NOERROR;
    } else if (mParentPref == mRoot) {
        *path = String("/") + mNodeName;
        return NOERROR;
    }

    String tmp;
    mParentPref->AbsolutePath(&tmp);
    *path = tmp + String("/") + mNodeName
    return NOERROR;
}

ECode AbstractPreferences::ChildrenNames(
    /* [out, callee] */ ArrayOf<String>** names) /*throws BackingStoreException*/
{
    VALIDATE_NOT_NULL(names);
    synchronized (mLock) {
        FAIL_RETURN(CheckState());
        AutoPtr<ITreeSet> result;
        CTreeSet::New((ITreeSet**)&result);

        HashMap<String, AutoPtr<AbstractPreferences> >::Iterator iter = mCachedNode.Begin();
        Boolean modified = FALSE;
        for (Int32 i = 0; iter != mCachedNode.End(); ++iter, i++) {
            AutoPtr<ICharSequence> name;
            CStringWrapper::New(iter->mFirst, (ICharSequence**)&name);
            result->Add(name, &modified);
        }

        AutoPtr<ArrayOf<String> > names;
        FAIL_RETURN(ChildrenNamesSpi((ArrayOf<String>**)&names));
        for (Int32 i = 0; i < names->GetLength(); i++) {
            AutoPtr<ICharSequence> name;
            CStringWrapper::New((*names)[i], (ICharSequence**)&name);
            result->Add(name, &modified);
        }

        for (Int32 i = 0; i < names.length; i++) {
            result.add(names[i]);
        }
        return result.toArray(new String[result.size()]);
    }
}

ECode AbstractPreferences::Clear() /*throws BackingStoreException */
{
    synchronized (lock) {
        for (String key : keys()) {
            remove(key);
        }
    }
}

ECode AbstractPreferences::ExportNode(
    /* [in] */ IOutputStream* ostream) /*throws IOException, BackingStoreException */
{
    if (ostream == NULL) {
        // throw new NullPointerException("ostream == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    checkState();
    XMLParser.exportPrefs(this, ostream, false);
}

ECode AbstractPreferences::ExportSubtree(
    /* [in] */ IOutputStream* ostream) /*throws IOException, BackingStoreException*/
{
    if (ostream == NULL) {
        // throw new NullPointerException("ostream == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    checkState();
    XMLParser.exportPrefs(this, ostream, true);
}

ECode AbstractPreferences::Flush() /*throws BackingStoreException */
{
    synchronized (lock) {
        flushSpi();
    }
    AbstractPreferences[] cc = cachedChildren();
    Int32 i;
    for (i = 0; i < cc.length; i++) {
        cc[i].flush();
    }
}

ECode AbstractPreferences::Get(
    /* [in] */ const String& key,
    /* [in] */ const String& deflt,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    if (key == NULL) {
        // throw new NullPointerException("key == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    String result = NULL;
    synchronized (lock) {
        checkState();
        try {
            result = getSpi(key);
        } catch (Exception e) {
            // ignored
        }
    }
    return (result == NULL ? deflt : result);
}

ECode AbstractPreferences::GetBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean deflt,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    String result = get(key, NULL);
    if (result == NULL) {
        return deflt;
    }
    if ("true".equalsIgnoreCase(result)) {
        return true;
    } else if ("false".equalsIgnoreCase(result)) {
        return false;
    } else {
        return deflt;
    }
}

ECode AbstractPreferences::GetByteArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* deflt,
    /* [out] */ ArrayOf<Byte>** values)
{
    VALIDATE_NOT_NULL(values);
    String svalue = get(key, NULL);
    if (svalue == NULL) {
        return deflt;
    }
    if (svalue.length() == 0) {
        return EmptyArray.BYTE;
    }
    try {
        byte[] bavalue = svalue.getBytes(StandardCharsets.US_ASCII);
        if (bavalue.length % 4 != 0) {
            return deflt;
        }
        return Base64.decode(bavalue);
    } catch (Exception e) {
        return deflt;
    }
}

ECode AbstractPreferences::GetDouble(
    /* [in] */ const String& key,
    /* [in] */ Double deflt,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    String result = get(key, NULL);
    if (result == NULL) {
        return deflt;
    }
    try {
        return Double.parseDouble(result);
    } catch (NumberFormatException e) {
        return deflt;
    }
}

ECode AbstractPreferences::GetFloat(
    /* [in] */ const String& key,
    /* [in] */ Float deflt,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    String result = get(key, NULL);
    if (result == NULL) {
        return deflt;
    }
    try {
        return Float.parseFloat(result);
    } catch (NumberFormatException e) {
        return deflt;
    }
}

ECode AbstractPreferences::GetInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 deflt,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    String result = get(key, NULL);
    if (result == NULL) {
        return deflt;
    }
    try {
        return Integer.parseInt(result);
    } catch (NumberFormatException e) {
        return deflt;
    }
}

ECode AbstractPreferences::GetInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 deflt,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    String result = get(key, NULL);
    if (result == NULL) {
        return deflt;
    }
    try {
        return Long.parseLong(result);
    } catch (NumberFormatException e) {
        return deflt;
    }
}

ECode AbstractPreferences::IsUserNode(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    return root == Preferences.userRoot();
}

ECode AbstractPreferences::Keys(
    /* [out, callee] */ ArrayOf<String>** keys) /*throws BackingStoreException*/
{
    VALIDATE_NOT_NULL(keys);
    synchronized (lock) {
        checkState();
        return keysSpi();
    }
}

ECode AbstractPreferences::Name(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mNodeName;
    return NOERROR;
}

ECode AbstractPreferences::Node(
    /* [in] */ const String& name,
    /* [out] */ IPreferences** pfs)
{
    VALIDATE_NOT_NULL(pfs);
    AbstractPreferences startNode = NULL;
    synchronized (lock) {
        checkState();
        validateName(name);
        if (name.isEmpty()) {
            return this;
        } else if ("/".equals(name)) {
            return root;
        }
        if (name.startsWith("/")) {
            startNode = root;
            name = name.substring(1);
        } else {
            startNode = this;
        }
    }
    try {
        return startNode.nodeImpl(name, true);
    } catch (BackingStoreException e) {
        // should not happen
        return NULL;
    }
}

ECode AbstractPreferences::ValidateName(
    /* [in] */ const String& name)
{
    if (name.endsWith("/") && name.length() > 1) {
        throw new IllegalArgumentException("Name cannot end with '/'");
    }
    if (name.indexOf("//") >= 0) {
        throw new IllegalArgumentException("Name cannot contain consecutive '/' characters");
    }
}

ECode AbstractPreferences::NodeImpl(
    /* [in] */ const String& path,
    /* [in] */ Boolean createNew,
    /* [out] */ AbstractPreferences** apfs)
{
    String[] names = path.split("/");
    AbstractPreferences currentNode = this;
    AbstractPreferences temp;
    for (String name : names) {
        synchronized (currentNode.lock) {
            temp = currentNode.mCachedNode.get(name);
            if (temp == NULL) {
                temp = getNodeFromBackend(createNew, currentNode, name);
            }
        }
        currentNode = temp;
        if (currentNode == NULL) {
            break;
        }
    }
    return currentNode;
}

ECode AbstractPreferences::GetNodeFromBackend(
    /* [in] */ Boolean createNew,
    /* [in] */ AbstractPreferences* currentNode,
    /* [in] */ const String& name,
    /* [out] */ AbstractPreferences** apfs)
{
    if (name.length() > MAX_NAME_LENGTH) {
        throw new IllegalArgumentException("Name '" + name + "' too long");
    }
    AbstractPreferences temp;
    if (createNew) {
        temp = currentNode.childSpi(name);
        currentNode.mCachedNode.put(name, temp);
        if (temp.newNode && currentNode.nodeChangeListeners.size() > 0) {
            currentNode.notifyChildAdded(temp);
        }
    } else {
        temp = currentNode.getChild(name);
    }
    return temp;
}

ECode AbstractPreferences::NodeExists(
    /* [in] */ const String& name,
    /* [out] */ Boolean* exist)
{
    VALIDATE_NOT_NULL(exist);
    if (name == NULL) {
        // throw new NullPointerException("name == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    AbstractPreferences startNode = NULL;
    synchronized (lock) {
        if (isRemoved()) {
            if (name.isEmpty()) {
                return false;
            }
            throw new IllegalStateException("This node has been removed");
        }
        validateName(name);
        if (name.isEmpty() || "/".equals(name)) {
            return true;
        }
        if (name.startsWith("/")) {
            startNode = root;
            name = name.substring(1);
        } else {
            startNode = this;
        }
    }
    try {
        Preferences result = startNode.nodeImpl(name, false);
        return (result != NULL);
    } catch(IllegalArgumentException e) {
        return false;
    }
}

ECode AbstractPreferences::Parent(
    /* [out] */IPreferences** pf)
{
    checkState();
    return parentPref;
}

ECode AbstractPreferences::CheckState()
{
    if (IsRemoved()) {
        throw new IllegalStateException("This node has been removed");
    }
}

ECode AbstractPreferences::Put(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    if (key == NULL) {
        // throw new NullPointerException("key == NULL");
        return E_NULL_POINTER_EXCEPTION;
    } else if (value == NULL) {
        // throw new NullPointerException("value == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (key.length() > MAX_KEY_LENGTH || value.length() > MAX_VALUE_LENGTH) {
        throw new IllegalArgumentException();
    }
    synchronized (lock) {
        checkState();
        putSpi(key, value);
    }
    notifyPreferenceChange(key, value);
}

ECode AbstractPreferences::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    put(key, String.valueOf(value));
}

ECode AbstractPreferences::PutByteArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* value)
{
    put(key, Base64.encode(value));
}

ECode AbstractPreferences::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ Double value)
{
    put(key, Double.toString(value));
}

ECode AbstractPreferences::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    put(key, Float.toString(value));
}

ECode AbstractPreferences::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    put(key, Integer.toString(value));
}

ECode AbstractPreferences::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    put(key, Long.toString(value));
}

ECode AbstractPreferences::Remove(
    /* [in] */ const String& key)
{
    synchronized (lock) {
        checkState();
        removeSpi(key);
    }
    notifyPreferenceChange(key, NULL);
}

ECode AbstractPreferences::RemoveNode() /*throws BackingStoreException */
{
    if (root == this) {
        throw new UnsupportedOperationException("Cannot remove root node");
    }
    synchronized (parentPref.lock) {
        removeNodeImpl();
    }
}

ECode AbstractPreferences::RemoveNodeImpl()/* throws BackingStoreException */
{
    synchronized (lock) {
        FAIL_RETURN(CheckState());
        AutoPtr<ArrayOf<String> > childrenNames;
        FAIL_RETURN(ChildrenNamesSpi((ArrayOf<String>**)&childrenNames));
        for (String childrenName : childrenNames) {
            if (mCachedNode.get(childrenName) == NULL) {
                AbstractPreferences child = ChildSpi(childrenName);
                mCachedNode.put(childrenName, child);
            }
        }

        final Collection<AbstractPreferences> values = mCachedNode.values();
        final AbstractPreferences[] children = values.toArray(new AbstractPreferences[values.size()]);
        for (AbstractPreferences child : children) {
            child.removeNodeImpl();
        }
        FAIL_RETURN(RemoveNodeSpi());
        isRemoved = true;
        parentPref.mCachedNode.remove(nodeName);
    }
    if (parentPref.nodeChangeListeners.size() > 0) {
        FAIL_RETURN(parentPref.notifyChildRemoved(this));
    }
    return NOERROR;
}

ECode AbstractPreferences::AddNodeChangeListener(
    /* [in] */ INodeChangeListener* ncl)
{
    if (ncl == NULL) {
        // throw new NullPointerException("ncl == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    checkState();
    synchronized (nodeChangeListeners) {
        nodeChangeListeners.add(ncl);
    }
}

ECode AbstractPreferences::AddPreferenceChangeListener(
    /* [in] */ IPreferenceChangeListener* pcl)
{
    if (pcl == NULL) {
        // throw new NullPointerException("pcl == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    checkState();
    synchronized (preferenceChangeListeners) {
        preferenceChangeListeners.add(pcl);
    }
}

ECode AbstractPreferences::RemoveNodeChangeListener(
    /* [in] */ INodeChangeListener* ncl)
{
    checkState();
    synchronized (nodeChangeListeners) {
        Int32 pos;
        if ((pos = nodeChangeListeners.indexOf(ncl)) == -1) {
            throw new IllegalArgumentException();
        }
        nodeChangeListeners.remove(pos);
    }
}

ECode AbstractPreferences::RemovePreferenceChangeListener(
    /* [in] */ IPreferenceChangeListener* pcl)
{
    checkState();
    synchronized (preferenceChangeListeners) {
        Int32 pos;
        if ((pos = preferenceChangeListeners.indexOf(pcl)) == -1) {
            throw new IllegalArgumentException();
        }
        preferenceChangeListeners.remove(pos);
    }
}

ECode AbstractPreferences::Sync() /*throws BackingStoreException*/
{
    synchronized (lock) {
        checkState();
        syncSpi();
    }
    for (AbstractPreferences child : cachedChildren()) {
        child.sync();
    }
}

ECode AbstractPreferences::ToString(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    Boolean userNode = FALSE;
    IsUserNode(&userNode);
    String path;
    AbsolutePath(&path);
    if (userNode) {
        *value = String("User") + String(" Preference Node: ") + path;
    } else {
        *value = String("System") + String(" Preference Node: ") + path;
    }
    return NOERROR;
}

void AbstractPreferences::NotifyChildAdded(
    /* [in] */ Preferences* child)
{
    NodeChangeEvent nce = new NodeAddEvent(this, child);
    synchronized (events) {
        events.add(nce);
        events.notifyAll();
    }
}

void AbstractPreferences::NotifyChildRemoved(
    /* [in] */ Preferences* child)
{
    NodeChangeEvent nce = new NodeRemoveEvent(this, child);
    synchronized (events) {
        events.add(nce);
        events.notifyAll();
    }
}

void AbstractPreferences::NotifyPreferenceChange(
    /* [in] */ const String& key,
    /* [in] */ const String& newValue)
{
    PreferenceChangeEvent pce = new PreferenceChangeEvent(this, key, newValue);
    synchronized (events) {
        events.add(pce);
        events.notifyAll();
    }
}

} // namespace Prefs
} // namespace Utility
} // namespace Elastos
