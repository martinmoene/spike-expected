// Anto Nonco. Extending expected<T> to deal with references. 27 May 2013.
// http://anto-nonco.blogspot.nl/2013/03/extending-expected-to-deal-with.html

#ifndef NONSTD_NONCO_EXPECTED_H_INCLUDED
#define NONSTD_NONCO_EXPECTED_H_INCLUDED

#include <functional>
#include <stdexcept>

template<typename>
class expected;

template<typename>
class expected_common;

template<typename T>
class expected_traits {
    friend class expected_common<T>;

    typedef T  storage;
    typedef T  value;
    typedef T* pointer;
    typedef T& reference;
}; //expected_traits

template<typename T>
class expected_traits<T&> {
    friend class expected_common<T&>;

    typedef std::reference_wrapper<T> storage;
    typedef T                         value;
    typedef T*                        pointer;
    typedef T&                        reference;
}; //expected_traits<T&>

template<typename T>
class expected_traits<const T&> {
    friend class expected_common<const T&>;

    typedef std::reference_wrapper<const T> storage;
    typedef T                               value;
    typedef const T*                        pointer;
    typedef const T&                        reference;
}; //expected_traits<const T&>

template<typename T>
class expected_common {
    friend class expected<T>;

    typedef typename expected_traits<T>::value     value;
    typedef typename expected_traits<T>::storage   storage;
    typedef typename expected_traits<T>::pointer   pointer;
    typedef typename expected_traits<T>::reference reference;

    expected_common() {};

    expected_common(const value& value) :
        m_valid  (true),
        m_storage(value)
    {};

    expected_common(value& value) :
        m_valid  (true),
        m_storage(value)
    {};

    expected_common(value&& value) :
        m_valid  (true),
        m_storage(std::move(value))
    {};

    template<typename... AA>
    explicit expected_common(AA&&... arguments) :
        m_valid  (true),
        m_storage(std::forward<AA>(arguments)...)
    {};

    expected_common(const expected_common& other) :
        m_valid(other.m_valid)
    {
        if(m_valid) new(&m_storage) storage(other.m_storage);
        else new(&m_exception) std::exception_ptr(other.m_exception);
    };

    expected_common(expected_common&& other) :
        m_valid(other.m_valid)
    {
        if(m_valid) new(&m_storage) storage(std::move(other.m_storage));
        else new(&m_exception) std::exception_ptr(std::move(other.m_exception));
    };

    ~expected_common() {
        if(m_valid) m_storage.~storage();
        else m_exception.~exception_ptr();
    };

    void swap(expected_common& other) {
        if(m_valid) {
            if(other.m_valid) std::swap(m_storage, other.m_storage);
            else {
                auto exception = std::move(other.m_exception);
                new(&other.m_storage) storage(std::move(m_storage));
                new(&m_exception) std::exception_ptr(exception);
                std::swap(m_valid, other.m_valid);
            }
        } else
            if(other.m_valid) other.swap(*this);
            else m_exception.swap(other.m_exception);
    };

public:
    template<class E>
    static expected<T> from_exception(const E& exception) {
        if (typeid (exception) != typeid (E))
        throw std::invalid_argument("slicing detected");

        return from_exception(std::make_exception_ptr(exception));
    }

    static expected<T> from_exception(std::exception_ptr exception) {
        expected<T> result;
        result.m_valid = false;
        new(&result.m_exception) std::exception_ptr(exception);
        return result;
    };

    static expected<T> from_exception() {
        return from_exception(std::current_exception());
    };

    template<class F, typename... AA>
    static expected<T> from_code(F function, AA&&... arguments) {
        try {
            return expected<T>(function(std::forward<AA>(arguments)...));
        }
        catch (...) {
            return from_exception();
        };
    };

    operator bool() const {
        return m_valid;
    };

    template<class E>
    bool exception_is() const {
        try {
            if(!m_valid) std::rethrow_exception(m_exception);
        } catch(const E&) {
            return true;
        } catch(...) {};

        return false;
    };

private:
    reference get() {
        if(!m_valid) std::rethrow_exception(m_exception);
        return m_storage;
    };

    const reference get() const {
        if(!m_valid) std::rethrow_exception(m_exception);
        return m_storage;
    };

public:
    reference operator * () {
        return get();
    };

    const reference operator * () const {
        return get();
    };

    pointer operator -> () {
        return &get();
    };

    const pointer operator -> () const {
        return &get();
    };

private:
    bool                   m_valid;
    union {
        storage            m_storage;
        std::exception_ptr m_exception;
    };
}; //expected_common

template<typename T>
class expected :
    public expected_common<T>
{
    friend class expected_common<T>;
    typedef expected_common<T> common;

    expected() {};

public:
    template<typename U>
    expected(expected<U>) = delete;

    template<class... AA>
    expected(AA&&... arguments) :
        common(std::forward<AA>(arguments)...)
    {};

    expected(const T& value) :
        common(value)
    {};

    expected(T&& value) :
        common(value)
    {};

    expected(const expected& other) :
        common(static_cast<const common&>(other))
    {};

    expected(expected&& other) :
        common(static_cast<common&&>(other))
    {};

    void swap(expected& other) {
        common::swap(static_cast<common&>(other));
    };
}; //expected

template<typename T>
class expected<T&> :
    public expected_common<T&>
{
    friend class expected_common<T&>;
    typedef expected_common<T&> common;

    expected() {};

public:
    expected(T& value) :
        common(value)
    {};

    expected(const expected& other) :
        common(static_cast<const common&>(other))
    {};

    expected(expected&& other) :
        common(static_cast<common&&>(other))
    {};

    void swap(expected& other) {
        common::swap(static_cast<common&>(other));
    };
}; //expected<T&>

template<typename T>
class expected<const T&> :
    public expected_common<const T&>
{
    friend class expected_common<const T&>;
    typedef expected_common<const T&> common;

    expected() {};

public:
    expected(const T& value) :
        common(value)
    {};

    expected(const expected& other) :
        common(static_cast<const common&>(other))
    {};

    expected(expected&& other) :
        common(static_cast<common&&>(other))
    {};

    void swap(expected& other) {
        common::swap(static_cast<common&>(other));
    };
}; //expected<const T&>

#endif // NONSTD_NONCO_EXPECTED_H_INCLUDED
