#ifndef OBJ_REF_H
#define OBJ_REF_H

#include <cassert>
#include <memory>

template <class _Ty>
class obj_ptr : public std::_Ptr_base<_Ty> { // class for pointer to reference counted resource
private:
    using _Mybase = std::_Ptr_base<_Ty>;
public:
    using typename _Mybase::element_type;
public:
    constexpr obj_ptr() noexcept {}

    constexpr obj_ptr(nullptr_t) noexcept {}

    obj_ptr(const obj_ptr& _Other) noexcept {
        this->_Weakly_construct_from(_Other); // same type, no conversion
    }

    // template <class _Ty2>
    // obj_ptr(const obj_ptr<_Ty2>& _Right, element_type* _Px) noexcept {
    //     // construct shared_ptr object that aliases _Right
    //     // this->_Alias_construct_from(_Right, _Px);
    //
    //     this->_Weakly_construct_from(_Right);
    //
    //     // this->_Ptr = _Px;
    //     // this->_Rep = _Right._Rep;
    // }
    //
    // template <class _Ty2>
    // obj_ptr(obj_ptr<_Ty2>&& _Right, element_type* _Px) noexcept {
    //     // move construct shared_ptr object that aliases _Right
    //     // this->_Alias_move_construct_from(_STD move(_Right), _Px);
    //
    //     this->_Weakly_construct_from(_Right);
    //
    //     // this->_Ptr = _Px;
    //     // this->_Rep = _Right._Rep;
    //     //
    //     // _Right._Ptr = nullptr;
    //     // _Right._Rep = nullptr;
    // }

    // template <class _Ty2>
    // explicit obj_ptr(const obj_ptr<_Ty2>& _Other) noexcept {
    //     this->_Weakly_construct_from(_Other); // shared_ptr keeps resource alive during conversion
    // }

    template <class _Ty2, std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
    obj_ptr(const std::shared_ptr<_Ty2>& _Other) noexcept {
        this->_Weakly_construct_from(_Other); // shared_ptr keeps resource alive during conversion
    }

    template <class _Ty2, std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
    obj_ptr(const obj_ptr<_Ty2>& _Other) noexcept {
        this->_Weakly_convert_lvalue_avoiding_expired_conversions(_Other);
    }

    obj_ptr(obj_ptr&& _Other) noexcept {
        this->_Move_construct_from(_STD move(_Other));
    }

    template <class _Ty2, std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
    obj_ptr(obj_ptr<_Ty2>&& _Other) noexcept {
        this->_Weakly_convert_rvalue_avoiding_expired_conversions(_STD move(_Other));
    }

    ~obj_ptr() noexcept {
        this->_Decwref();
    }

    obj_ptr& operator=(const obj_ptr& _Right) noexcept {
        obj_ptr(_Right).swap(*this);
        return *this;
    }

    template <class _Ty2>
    obj_ptr& operator=(const obj_ptr<_Ty2>& _Right) noexcept {
        obj_ptr(_Right).swap(*this);
        return *this;
    }

    obj_ptr& operator=(obj_ptr&& _Right) noexcept {
        obj_ptr(_STD move(_Right)).swap(*this);
        return *this;
    }

    template <class _Ty2>
    obj_ptr& operator=(obj_ptr<_Ty2>&& _Right) noexcept {
        obj_ptr(_STD move(_Right)).swap(*this);
        return *this;
    }

    template <class _Ty2>
    obj_ptr& operator=(const std::shared_ptr<_Ty2>& _Right) noexcept {
        obj_ptr(_Right).swap(*this);
        return *this;
    }

    void reset() noexcept { // release resource, convert to null obj_ptr object
        obj_ptr{}.swap(*this);
    }

    void swap(obj_ptr& _Other) noexcept {
        this->_Swap(_Other);
    }

    _NODISCARD bool expired() const noexcept {
        return this->use_count() == 0;
    }

    operator bool() const noexcept {
        // return this->_Ptr_base<_Ty>::get() != nullptr; // ToDo use_count?
        return this->use_count() != 0;
    }

    _NODISCARD std::shared_ptr<_Ty> lock() const noexcept { // convert to shared_ptr
        std::shared_ptr<_Ty> _Ret;
        (void) _Ret._Construct_from_weak(*this);
        return _Ret;
    }

    template <class _Ty2 = _Ty, std::enable_if_t<!std::disjunction_v<std::is_array<_Ty2>, std::is_void<_Ty2>>, int> = 0>
    _NODISCARD _Ty2& operator*() const {
        assert(("Object null reference occured", !expired())); // ToDo
        return *this->_Mybase::get();
    }

    template <class _Ty2 = _Ty, std::enable_if_t<!std::is_array_v<_Ty2>, int> = 0>
    _NODISCARD _Ty2* operator->() const {
        assert(("Object null reference occured", !expired())); // ToDo
        return this->_Mybase::get();
    }

    bool operator==(const obj_ptr& other)
    {
        bool leftNotNull = *this;
        bool rightNotNull = other;

        if (leftNotNull && rightNotNull)
        {
            // return false; // ToDo
            return this->_Mybase::get() == other._Mybase::get();
        }

        if (leftNotNull || rightNotNull)
        {
            return false;
        }

        return true;
    }

    bool operator!=(const obj_ptr& other)
    {
        return !(*this == other);
    }

    bool operator==(nullptr_t)
    {
        return !*this;
    }

    bool operator!=(nullptr_t)
    {
        return *this;
    }

    // bool operator==(const obj_ptr& _Left, const obj_ptr& _Right)
    // {
    //     bool leftNotNull = _Left;
    //     bool rightNotNull = _Right;
    //
    //     if (leftNotNull && rightNotNull)
    //     {
    //         return _Left._Ptr == _Right._Ptr;
    //     }
    //
    //     if (leftNotNull || rightNotNull)
    //     {
    //         return false;
    //     }
    //
    //     return true;
    // }

    // bool operator!=(const obj_ptr& _Left, const obj_ptr& _Right)
    // {
    //     return !(_Left == _Right);
    // }
    //
    // bool operator==(const obj_ptr& _Left, nullptr_t)
    // {
    //     return !_Left;
    // }
    //
    // bool operator!=(const obj_ptr& _Left, nullptr_t)
    // {
    //     return _Left;
    // }

    using _Mybase::get;
};

#ifdef _CPPRTTI
template <class _Ty1, class _Ty2>
_NODISCARD obj_ptr<_Ty1> dynamic_pointer_cast(const obj_ptr<_Ty2>& _Other) noexcept {
    // dynamic_cast for shared_ptr that properly respects the reference count control block
    const auto _Ptr = dynamic_cast<typename obj_ptr<_Ty1>::element_type*>(_Other.get());

    if (_Ptr) {
        // return obj_ptr<_Ty1>(_Other, _Ptr);
        return obj_ptr<_Ty1>(_Other);
    }

    return {};
}

template <class _Ty1, class _Ty2>
_NODISCARD obj_ptr<_Ty1> dynamic_pointer_cast(obj_ptr<_Ty2>&& _Other) noexcept {
    // dynamic_cast for shared_ptr that properly respects the reference count control block
    const auto _Ptr = dynamic_cast<typename obj_ptr<_Ty1>::element_type*>(_Other.get());

    if (_Ptr) {
        // return obj_ptr<_Ty1>(_STD move(_Other), _Ptr);
        return obj_ptr<_Ty1>(_STD move(_Other));
    }

    return {};
}
#else // _CPPRTTI
template <class _Ty1, class _Ty2>
obj_ptr<_Ty1> dynamic_pointer_cast(const obj_ptr<_Ty2>&) noexcept = delete; // requires /GR option
template <class _Ty1, class _Ty2>
obj_ptr<_Ty1> dynamic_pointer_cast(obj_ptr<_Ty2>&&) noexcept = delete; // requires /GR option
#endif // _CPPRTTI

#endif //OBJ_REF_H
