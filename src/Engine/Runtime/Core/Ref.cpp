#include "Ref.h"

std::unordered_set<void*> NSE::RefCounted::_references;
std::mutex NSE::RefCounted::_referencesMutex;

void NSE::RefCounted::RegRef(void *ptr)
{
    std::scoped_lock lock(_referencesMutex);
    assert(ptr);
    _references.insert(ptr);
}

void NSE::RefCounted::UnregRef(void *ptr)
{
    assert(ptr);
    std::scoped_lock lock(_referencesMutex);
    assert(_references.find(ptr) != _references.end());
    _references.erase(ptr);
}

bool NSE::RefCounted::ContainsRef(void *ptr)
{
    assert(ptr);
    return _references.find(ptr) != _references.end();
}
