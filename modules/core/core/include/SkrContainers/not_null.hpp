#pragma once
#include <functional>
#include <type_traits>
#include "SkrBase/misc/debug.h" 
namespace skr
{
namespace details
{
template <typename T, typename = void>
struct is_comparable_to_nullptr : std::false_type {
};

template <typename T>
struct is_comparable_to_nullptr<
T,
std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>>
    : std::true_type {
};
// Resolves to the more efficient of `const T` or `const T&`, in the context of returning a const-qualified value
// of type T.
//
// Copied from cppfront's implementation of the CppCoreGuidelines F.16 (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-in)
template <typename T>
using value_or_reference_return_t = std::conditional_t<
sizeof(T) < 2 * sizeof(void*) && std::is_trivially_copy_constructible<T>::value,
const T,
const T&>;
} // namespace details

//
// not_null
//
// Restricts a pointer or smart pointer to only hold non-null values.
//
// Has zero size overhead over T.
//
// If T is a pointer (i.e. T == U*) then
// - allow construction from U*
// - disallow construction from nullptr_t
// - disallow default construction
// - ensure construction from null U* fails
// - allow implicit conversion to U*
//
template <class T>
class not_null
{
public:
    static_assert(details::is_comparable_to_nullptr<T>::value, "T cannot be compared to nullptr.");

    template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
    constexpr not_null(U&& u)
        : ptr_(std::forward<U>(u))
    {
        SKR_ASSERT(ptr_ != nullptr);
    }

    constexpr not_null(T u)
        : ptr_(std::move(u))
    {
        SKR_ASSERT(ptr_ != nullptr);
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
    constexpr not_null(const not_null<U>& other)
        : not_null(other.ptr_)
    {
    }

    template <typename U, typename = std::enable_if_t<std::is_constructible_v<U*, T>>>
    constexpr not_null(U& u)
        : not_null(&u)
    {
    }

    not_null(const not_null& other)            = default;
    not_null& operator=(const not_null& other) = default;

    constexpr details::value_or_reference_return_t<T> get() const
    {
        return ptr_;
    }

    constexpr operator T() const { return ptr_; }
    constexpr decltype(auto) operator->() const { return ptr_; }
    constexpr decltype(auto) operator*() const { return *ptr_; }

    // unwanted operators...pointers only point to single objects!
    not_null& operator++()                = delete;
    not_null& operator--()                = delete;
    not_null  operator++(int)             = delete;
    not_null  operator--(int)             = delete;
    not_null& operator+=(std::ptrdiff_t)  = delete;
    not_null& operator-=(std::ptrdiff_t)  = delete;
    void operator[](std::ptrdiff_t) const = delete;

private:
    T ptr_;
};

template <class T, class U>
auto operator==(const not_null<T>& lhs,
                const not_null<U>& rhs) noexcept(noexcept(lhs.ptr_ == rhs.ptr_))
-> decltype(lhs.ptr_ == rhs.ptr_)
{
    return lhs.ptr_ == rhs.ptr_;
}

template <class T, class U>
auto operator!=(const not_null<T>& lhs,
                const not_null<U>& rhs) noexcept(noexcept(lhs.ptr_ != rhs.ptr_))
-> decltype(lhs.ptr_ != rhs.ptr_)
{
    return lhs.ptr_ != rhs.ptr_;
}

template <class T, class U>
auto operator<(const not_null<T>& lhs,
               const not_null<U>& rhs) noexcept(noexcept(std::less<>{}(lhs.ptr_, rhs.ptr_)))
-> decltype(std::less<>{}(lhs.ptr_, rhs.ptr_))
{
    return std::less<>{}(lhs.ptr_, rhs.ptr_);
}

template <class T, class U>
auto operator<=(const not_null<T>& lhs,
                const not_null<U>& rhs) noexcept(noexcept(std::less_equal<>{}(lhs.ptr_, rhs.ptr_)))
-> decltype(std::less_equal<>{}(lhs.ptr_, rhs.ptr_))
{
    return std::less_equal<>{}(lhs.ptr_, rhs.ptr_);
}

template <class T, class U>
auto operator>(const not_null<T>& lhs,
               const not_null<U>& rhs) noexcept(noexcept(std::greater<>{}(lhs.ptr_, rhs.ptr_)))
-> decltype(std::greater<>{}(lhs.ptr_, rhs.ptr_))
{
    return std::greater<>{}(lhs.ptr_, rhs.ptr_);
}

template <class T, class U>
auto operator>=(const not_null<T>& lhs,
                const not_null<U>& rhs) noexcept(noexcept(std::greater_equal<>{}(lhs.ptr_, rhs.ptr_)))
-> decltype(std::greater_equal<>{}(lhs.ptr_, rhs.ptr_))
{
    return std::greater_equal<>{}(lhs.ptr_, rhs.ptr_);
}

// more unwanted operators
template <class T, class U>
std::ptrdiff_t operator-(const not_null<T>&, const not_null<U>&) = delete;
template <class T>
not_null<T> operator-(const not_null<T>&, std::ptrdiff_t) = delete;
template <class T>
not_null<T> operator+(const not_null<T>&, std::ptrdiff_t) = delete;
template <class T>
not_null<T> operator+(std::ptrdiff_t, const not_null<T>&) = delete;

template <class T, class U = decltype(std::declval<const T&>().ptr_), bool = std::is_default_constructible<std::hash<U>>::value>
struct not_null_hash {
    std::size_t operator()(const T& value) const { return std::hash<U>{}(value.ptr_); }
};

template <class T, class U>
struct not_null_hash<T, U, false> {
    not_null_hash()                                = delete;
    not_null_hash(const not_null_hash&)            = delete;
    not_null_hash& operator=(const not_null_hash&) = delete;
};
} // namespace skr