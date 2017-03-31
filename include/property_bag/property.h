/**
 * \file property.h
 * \brief Generic Property holding any type.
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_PROPERTY_H
#define PROPERTY_BAG_PROPERTY_H

#include <bitset>
#include <sstream>

#include "property_bag/utils.h"

/*
 * Boost pre-1.56 has no serialization
 * for std shared_ptr.
 */

#include <boost/version.hpp>

#if BOOST_VERSION < 105600

#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

namespace
{

template<typename A, typename B>
using disable_if_same_or_derived = typename std::enable_if<
        !std::is_base_of<A,typename std::remove_reference<B>::type >::value>;

}

namespace property_bag
{

template <class T>
using shared_ptr = boost::shared_ptr<T>;

template <typename T, typename... Args>
shared_ptr<T> make_ptr(Args&&... args)
{
  return boost::make_shared<T>(std::forward<Args>(args)...);
}

template <typename Tout, typename Tin>
shared_ptr<Tout> dynamic_pointer_cast(Tin&& in)
{
  return boost::dynamic_pointer_cast<Tout>(std::forward<Tin>(in));
}
} // namespace property_bag

#else

#include <memory>

namespace property_bag
{

template <class T>
using shared_ptr = std::shared_ptr<T>;

template <typename T, typename... Args>
shared_ptr<T> make_ptr(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename Tout, typename Tin>
shared_ptr<Tout> dynamic_pointer_cast(Tin&& in)
{
  return std::dynamic_pointer_cast<Tout>(std::forward<Tin>(in));
}

} // namespace property_bag
#endif

namespace property_bag
{

template <typename T>
bool empty(const shared_ptr<T>& ptr)
{
  return ptr.get() == nullptr;
}

struct PropertyException : public std::exception
{
  PropertyException(const std::string &message);

  const char* what() const throw();

  const std::string message_;
};

namespace details
{
/**
 * @brief The PlaceHolder class.
 * A place holder base class.
 */
class PlaceHolder
{
public:

  PlaceHolder()          = default;
  virtual ~PlaceHolder() = default;

  virtual const std::type_info& type() = 0;
};

using PlaceHolderPtr = shared_ptr<PlaceHolder>;

// Forward declaration
class Any;

/**
 * @brief The PlaceHolderImpl class.
 * The actual place holder.
 */
template<typename T>
class PlaceHolderImpl : public PlaceHolder
{
public:

  /**
   * @brief 'pimpl' struct to enable access to
   * private members during serialization
   */
  struct serialization_accessor;

  /**
   * @brief PlaceHolderImpl. Default constructor
   */
  PlaceHolderImpl() = default;

  /**
   * @brief PlaceHolderImpl
   * copy (T) constructor
   * @param value
   */
  explicit PlaceHolderImpl(const T& value) :
    PlaceHolder(),
    value_(value) { }

  /**
   * @brief PlaceHolderImpl
   * move (T) constructor
   * @param value
   */
  explicit PlaceHolderImpl(T&& value) :
    PlaceHolder(),
    value_(std::move(value)) { }

  /**
   * @brief PlaceHolderImpl
   * copy (PlaceHolderImpl<T>) constructor
   * @param o
   */
  explicit PlaceHolderImpl(const PlaceHolderImpl<T>& o) :
    PlaceHolder(),
    value_(o.value_) { }

  /**
   * @brief PlaceHolderImpl
   * move (PlaceHolderImpl<T>) constructor
   * @param o
   */
  explicit PlaceHolderImpl(PlaceHolderImpl<T>&& o) :
    PlaceHolder(),
    value_(std::move(o.value_)) { }

  /**
   * @brief operator =
   * copy (PlaceHolderImpl<T>) assignment
   * @param o. Another const PlaceHolderImpl<T>
   * @return this PlaceHolderImpl<T>
   */
  PlaceHolderImpl<T>& operator=(const PlaceHolderImpl<T>& o)
  {
    value_ = o.value_;
    return *this;
  }

  /**
   * @brief operator =
   * move (PlaceHolderImpl<T>) assignment
   * @param o. Another movable PlaceHolderImpl<T>
   * @return this PlaceHolderImpl<T>
   */
  PlaceHolderImpl<T>& operator=(PlaceHolderImpl<T>&& o)
  {
    value_ = std::move(o.value_);
    return *this;
  }

  /**
   * @brief ~PlaceHolderImpl. Default destructor.
   */
  ~PlaceHolderImpl() = default;

  /**
   * @brief type
   * @return std::type_info. typeid(T)
   */
  inline const std::type_info& type() override { return typeid(T); }

protected:

  T value_;

  template<typename TT>
  friend TT& anycast(Any &val);

  template<typename TT>
  friend const TT& anycast(const Any &val);
};

template <typename T>
using PlaceHolderImplPtr = shared_ptr<PlaceHolderImpl<T>>;

/**
 * @brief The Any class.
 * A type-erasure based holder.
 */
class Any
{
public:

  /**
   * @brief 'pimpl' struct to enable access to
   * private members during serialization
   */
  struct serialization_accessor;

  Any()  = default;
  ~Any() = default;

  Any(const Any& o) = default;
  Any(Any&& o);

  Any& operator=(const Any& o) = default;
  Any& operator=(Any&& o);

  template<typename T,
           typename = typename disable_if_same_or_derived<Any,T>::type>
  Any(T&& value)
  {
    placeholder_ =
        make_ptr<PlaceHolderImpl<typename std::decay<T>::type>>(std::forward<T>(value));
  }

  template<typename T,
           typename = typename disable_if_same_or_derived<Any,T>::type>
  void operator=(T&& value)
  {
    placeholder_ =
        make_ptr<PlaceHolderImpl<typename std::decay<T>::type>>(std::forward<T>(value));
  }

  /**
   * @brief type. Return the type info of the holded value, typeid(T).
   * @return std::type_info.
   */
  inline const std::type_info& type() const
  {
    return placeholder_->type();
  }

  /**
   * @brief empty. Whether Any holds something or not.
   * @return true if holding, false otherwise.
   */
  inline bool empty() const noexcept
  {
    return placeholder_.get() == nullptr;
  }

protected:

  PlaceHolderPtr placeholder_;

  template<typename T>
  friend T& anycast(Any& val);

  template<typename T>
  friend const T& anycast(const Any& val);
};

template<typename T>
T& anycast(Any& val)
{
  PlaceHolderImplPtr<T> concrete =
      dynamic_pointer_cast<PlaceHolderImpl<T>>(val.placeholder_);

  if (empty(concrete))
    throw PropertyException(std::string("Could not convert from ") +
                            details::name_of(val.type()) +
                            std::string(" to ") +
                            details::name_of(typeid(T)));

  return concrete->value_;
}

template<typename T>
const T& anycast(const Any& val)
{
  PlaceHolderImplPtr<T> concrete =
      dynamic_pointer_cast<PlaceHolderImpl<T>>(val.placeholder_);

  if (empty(concrete))
  {
    throw PropertyException(std::string("Could not convert from ") +
                            details::name_of(val.type()) +
                            std::string(" to ") +
                            details::name_of(typeid(T)));
  }

  return concrete->value_;
}
} // namespace details

/**
 * \brief A Property
 */
class Property
{
public:

  /**
   * @brief 'pimpl' struct to enable access to
   * private members during serialization
   */
  struct serialization_accessor;

  enum
  {
    NONE = 0,
    DEFAULT_VALUE,
    PROVIDED_VALUE
  };

  /**
   * \brief Property default constructor.
   * Creates a Property that is initialized with the
   * Property::none type. This should be fairly cheap.
   */
  Property();
  ~Property() = default;

  /**
   * @brief Property. Copy constructor.
   * @param rhs, another Property.
   */
  Property(const Property& rhs);

  /**
   * @brief Property. Move constructor.
   * @param rhs, another Property.
   */
  Property(Property&& rhs);

  /**
   * @brief operator =
   * @param rhs
   * @return
   */
  Property& operator=(const Property& rhs);

  /**
   * @brief operator =
   * @param rhs
   * @return
   */
  Property& operator=(Property&& rhs);

  /**
   * \brief A convenience constructor for creating a Property
   * that holds the given type.
   * @param T The type to hide in this Property
   * @param t default value for t
   * @param doc a documentation string
   */
  template <typename T,
            typename = typename disable_if_same_or_derived<Property,T>::type>
  Property(T&& t, const std::string& doc = "") :
    description_{doc},
    flags_()
  {
    set_holder(std::forward<T>(t));

    if (!std::is_same<T, none>::value)
      flags_[DEFAULT_VALUE]=true;
    else
      flags_[NONE]=true;
  }

  /**
   * \brief type_name. Type name of whatever Property is holding.
   *
   * @return std::string. A name equivalent to typeid(T).name
   *
   * @see type().
   */
  std::string type_name() const noexcept;

  /**
   * @brief type. The type_info of whatever Property is holding.
   * @return std::type_info.
   */
  const std::type_info& type() const noexcept;

  inline bool is_defined()  const noexcept { return !flags_[NONE];           }
  inline bool is_default()  const noexcept { return  flags_[DEFAULT_VALUE];  }
  inline bool is_modified() const noexcept { return  flags_[PROVIDED_VALUE]; }

  /**
   * \brief A doc string for this Property, "foo is for the input
   * and will be mashed with spam."
   * @return A very descriptive human readable string of whatever
   * the Property is holding on to.
   */
  std::string description() const noexcept;

  /**
   * \brief The doc for this Property is runtime defined, so you may want to update it.
   * @param description A human readable description of the Property.
   */
  void description(const std::string& doc);

  /**
   * \brief runtime check if the Property is of the given type.
   * @return true if it is the type.
   */
  template<typename T>
  bool is_same() const
  {
    return (typeid(T) == holder_.type());
  }

  /**
   * \brief Test if the given Property is the same type as this one
   * @param rhs The Property to test against.
   * @return true if they are the same type.
   */
  bool is_same(const Property& rhs) const;

  /// @todo is_castable or such
  bool is_compatible(const Property& rhs) const;

  /// @todo is_castable or such
  template<typename T>
  bool is_compatible() const
  {
    return (typeid(T) == holder_.type())? true : flags_[NONE];
  }

  /**
   * \brief This sets the default value of the Property. This is a
   * @param val
   */
  template<typename T>
  void set(T&& val)
  {
    enforce_type_set<T>();

    if (flags_[NONE])
    {
      flags_[NONE]           = false;
      flags_[DEFAULT_VALUE]  = true;
      flags_[PROVIDED_VALUE] = false;
    }
    else if (flags_[DEFAULT_VALUE])
    {
      flags_[DEFAULT_VALUE]  = false;
      flags_[PROVIDED_VALUE] = true;
    }

    set_holder(std::forward<T>(val));
  }

  template<typename T>
  inline const T& get() const
  {
    enforce_type<T>();
    return unsafe_get<T>();
  }

  template<typename T>
  inline T& get()
  {
    enforce_type<T>();
    return unsafe_get<T>();
  }

  /**
   * \brief runtime check if the Property is of the given type, this will throw.
   */
  template<typename T>
  inline void enforce_type() const
  {
    if (!is_same<T>())
      throw PropertyException(
          "Property::enforce_type() Type missmatched !\nObject 'this' of type "
                        + type_name() + " whereas " + name_of<T>());
  }

  //! A none type for Property when the Property is uninitialized.
  struct none
  {
    none& operator=(const none&) { return *this; }
    const none& operator=(const none&) const { return *this; }
    friend bool operator==(const none&, const none&) { return true; }
    friend std::ostream& operator<<(std::ostream& os, const none&)
    {
      os << "Property::none"; return os;
    }
  };

protected:

  /**
   * \brief runtime check if the Property is of the given type, this will throw.
   */
  template<typename T>
  inline void enforce_type_set() const
  {
    if (!is_same<T>() && !flags_[NONE])
      throw PropertyException(
          "Property::enforce_type_set() Type missmatched !\nObject 'this' of type "
                        + type_name() + " whereas " + name_of<T>());
  }

  template<typename T>
  inline const T& unsafe_get() const
  {
    return details::anycast<T>(holder_);
  }

  template<typename T>
  inline T& unsafe_get()
  {
    return details::anycast<T>(holder_);
  }

  template <typename T>
  void set_holder(T&& t)
  {
    holder_ = std::forward<T>(t);
  }

  details::Any holder_;

  std::string description_;

  std::bitset<3> flags_;
};

} //namespace property_bag

#endif // PROPERTY_BAG_PROPERTY_H
