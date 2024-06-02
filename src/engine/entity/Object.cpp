#include "Object.h"

size_t NSE::Object::_uidCount = 0;

NSE::Object::Object()
{
    // Assign next UID
    _uid = _uidCount++;
}

NSE::Object::~Object()
{

}
