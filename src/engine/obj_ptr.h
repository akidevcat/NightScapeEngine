#ifndef OBJ_REF_H
#define OBJ_REF_H

#include <cassert>
#include <memory>

// That's extremely dirty
template <class _Ty>
class _Ptr_base_Hack
{
public:
    using element_type = std::remove_extent_t<_Ty>;

//     _NODISCARD long use_count() const noexcept {
//         return _Rep ? _Rep->_Use_count() : 0;
//     }
//
//     template <class _Ty2>
//     _NODISCARD bool owner_before(const _Ptr_base_Hack<_Ty2>& _Right) const noexcept { // compare addresses of manager objects
//         return _Rep < _Right._Rep;
//     }
//
//     _Ptr_base_Hack(const _Ptr_base_Hack&) = delete;
//     _Ptr_base_Hack& operator=(const _Ptr_base_Hack&) = delete;
//
// public:
//     _NODISCARD element_type* get() const noexcept {
//         return _Ptr;
//     }
//
//     constexpr _Ptr_base_Hack() noexcept = default;
//
//     ~_Ptr_base_Hack() = default;
//
//     template <class _Ty2>
//     void _Move_construct_from(_Ptr_base_Hack<_Ty2>&& _Right) noexcept {
//         // implement shared_ptr's (converting) move ctor and weak_ptr's move ctor
//         _Ptr = _Right._Ptr;
//         _Rep = _Right._Rep;
//
//         _Right._Ptr = nullptr;
//         _Right._Rep = nullptr;
//     }
//
//     template <class _Ty2>
//     void _Copy_construct_from(const std::shared_ptr<_Ty2>& _Other) noexcept {
//         // implement shared_ptr's (converting) copy ctor
//         _Other._Incref();
//
//         _Ptr = _Other._Ptr;
//         _Rep = _Other._Rep;
//     }
//
//     template <class _Ty2>
//     void _Alias_construct_from(const std::shared_ptr<_Ty2>& _Other, element_type* _Px) noexcept {
//         // implement shared_ptr's aliasing ctor
//         _Other._Incref();
//
//         _Ptr = _Px;
//         _Rep = _Other._Rep;
//     }
//
//     template <class _Ty2>
//     void _Alias_move_construct_from(std::shared_ptr<_Ty2>&& _Other, element_type* _Px) noexcept {
//         // implement shared_ptr's aliasing move ctor
//         _Ptr = _Px;
//         _Rep = _Other._Rep;
//
//         _Other._Ptr = nullptr;
//         _Other._Rep = nullptr;
//     }
//
//     template <class _Ty0>
//     friend class std::weak_ptr; // specifically, weak_ptr::lock()
//
//     template <class _Ty2>
//     bool _Construct_from_weak(const std::weak_ptr<_Ty2>& _Other) noexcept {
//         // implement shared_ptr's ctor from weak_ptr, and weak_ptr::lock()
//         if (_Other._Rep && _Other._Rep->_Incref_nz()) {
//             _Ptr = _Other._Ptr;
//             _Rep = _Other._Rep;
//             return true;
//         }
//
//         return false;
//     }
//
//     void _Incref() const noexcept {
//         if (_Rep) {
//             _Rep->_Incref();
//         }
//     }
//
//     void _Decref() noexcept { // decrement reference count
//         if (_Rep) {
//             _Rep->_Decref();
//         }
//     }
//
//     void _Swap(_Ptr_base_Hack& _Right) noexcept { // swap pointers
//         _STD swap(_Ptr, _Right._Ptr);
//         _STD swap(_Rep, _Right._Rep);
//     }
//
//     template <class _Ty2>
//     void _Weakly_construct_from(const std::_Ptr_base<_Ty2>& _Other) noexcept { // implement weak_ptr's ctors
//         if (_Other._Rep) {
//             _Ptr = _Other._Ptr;
//             _Rep = _Other._Rep;
//             _Rep->_Incwref();
//         } else {
//             _STL_INTERNAL_CHECK(!_Ptr && !_Rep);
//         }
//     }
//
//     template <class _Ty2>
//     void _Weakly_convert_lvalue_avoiding_expired_conversions(const std::_Ptr_base<_Ty2>& _Other) noexcept {
//         // implement weak_ptr's copy converting ctor
//         if (_Other._Rep) {
//             _Rep = _Other._Rep; // always share ownership
//             _Rep->_Incwref();
//
//             if (_Rep->_Incref_nz()) {
//                 _Ptr = _Other._Ptr; // keep resource alive during conversion, handling virtual inheritance
//                 _Rep->_Decref();
//             } else {
//                 _STL_INTERNAL_CHECK(!_Ptr);
//             }
//         } else {
//             _STL_INTERNAL_CHECK(!_Ptr && !_Rep);
//         }
//     }
//
//     template <class _Ty2>
//     void _Weakly_convert_rvalue_avoiding_expired_conversions(std::_Ptr_base<_Ty2>&& _Other) noexcept {
//         // implement weak_ptr's move converting ctor
//         _Rep        = _Other._Rep; // always transfer ownership
//         _Other._Rep = nullptr;
//
//         if (_Rep && _Rep->_Incref_nz()) {
//             _Ptr = _Other._Ptr; // keep resource alive during conversion, handling virtual inheritance
//             _Rep->_Decref();
//         } else {
//             _STL_INTERNAL_CHECK(!_Ptr);
//         }
//
//         _Other._Ptr = nullptr;
//     }
//
//     void _Incwref() const noexcept {
//         if (_Rep) {
//             _Rep->_Incwref();
//         }
//     }
//
//     void _Decwref() noexcept { // decrement weak reference count
//         if (_Rep) {
//             _Rep->_Decwref();
//         }
//     }

public:
    element_type* _Ptr{nullptr};
    std::_Ref_count_base* _Rep{nullptr};

//     template <class _Ty0>
//     friend class std::_Ptr_base;
//
//     friend std::shared_ptr<_Ty>;
//
//     template <class _Ty0>
//     friend struct std::atomic;
//
//     friend std::_Exception_ptr_access;
//
// #if _HAS_STATIC_RTTI
//     template <class _Dx, class _Ty0>
//     friend _Dx* get_deleter(const std::shared_ptr<_Ty0>& _Sx) noexcept;
// #endif // _HAS_STATIC_RTTI
};

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

    template <class _Ty2>
    obj_ptr(const obj_ptr<_Ty2>& _Right, element_type* _Px) noexcept {
        // construct shared_ptr object that aliases _Right
        // this->_Alias_construct_from(_Right, _Px);

        // this->_Weakly_convert_lvalue_avoiding_expired_conversions(_Right);

        auto baseHack = reinterpret_cast<_Ptr_base_Hack<_Ty>*>(this);
        auto rightBaseHack = reinterpret_cast<const _Ptr_base_Hack<_Ty>&>(_Right);

        baseHack->_Ptr = _Px;
        baseHack->_Rep = rightBaseHack._Rep;
        baseHack->_Rep->_Incwref();
        // this->_Ptr = _Px;
        // this->_Rep = _Right._Rep;
    }
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
        return this->use_count() != 0;
    }

    _NODISCARD std::shared_ptr<_Ty> lock() const noexcept { // convert to shared_ptr
        std::shared_ptr<_Ty> _Ret;
        (void) _Ret._Construct_from_weak(*this);
        return _Ret;
    }

    template <class _Ty2 = _Ty, std::enable_if_t<!std::disjunction_v<std::is_array<_Ty2>, std::is_void<_Ty2>>, int> = 0>
    _NODISCARD _Ty2& operator*() const {
        assert(("Object null reference occured", !expired()));
        return *this->_Mybase::get();
    }

    template <class _Ty2 = _Ty, std::enable_if_t<!std::is_array_v<_Ty2>, int> = 0>
    _NODISCARD _Ty2* operator->() const {
        assert(("Object null reference occured", !expired()));
        return this->_Mybase::get();
    }

    bool operator==(const obj_ptr& other) const
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
        return obj_ptr<_Ty1>(_Other, _Ptr);
    }

    return {};
}

template <class _Ty1, class _Ty2>
_NODISCARD obj_ptr<_Ty1> dynamic_pointer_cast(obj_ptr<_Ty2>&& _Other) noexcept {
    // dynamic_cast for shared_ptr that properly respects the reference count control block
    const auto _Ptr = dynamic_cast<typename obj_ptr<_Ty1>::element_type*>(_Other.get());

    if (_Ptr) {
        return obj_ptr<_Ty1>(_STD move(_Other), _Ptr);
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
