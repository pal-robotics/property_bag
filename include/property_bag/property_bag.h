/**
 * \file property_bag.h
 * \brief Property container holding many different type.
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_PROPERTY_BAG_H
#define PROPERTY_BAG_PROPERTY_BAG_H

#include "property_bag/property.h"

#include <list>
#include <map>

namespace{

template <typename T>
using is_string =
std::integral_constant<bool,
  std::is_same<typename std::decay<T>::type, char*>::value ||
  std::is_same<typename std::decay<T>::type, const char*>::value ||
  std::is_same<typename std::decay<T>::type, std::string>::value>;

#define ASSERT_NAMED_PROPERTIES(KEY_TYPE, NAME, ARGS) \
  static_assert(sizeof...(Args)%2==0, \
    "Error : parameters work by pair, a name (std::string) and a value."); \
  static_assert(((is_string<KEY_TYPE>::value and is_string<NAME>::value) or \
                 std::is_same<KEY_TYPE, typename std::decay<NAME>::type >::value), \
    "Error : KeyType and provided key's type are not the same."); \

#define ASSERT_NAMED_DOCED_PROPERTIES(KEY_TYPE, NAME, DOC, ARGS) \
  static_assert(is_string<DOC>::value, \
    "Error : parameter description must be a string."); \
  static_assert(sizeof...(ARGS)%3==0, \
    "Error : parameters work by pair, a name KeyType and a value."); \
    static_assert(((is_string<KEY_TYPE>::value and is_string<NAME>::value) or \
                   std::is_same<KEY_TYPE, typename std::decay<NAME>::type >::value), \
    "Error : KeyType and provided key's type are not the same."); \

template<class...> struct disjunction : std::false_type { };
template<class B1> struct disjunction<B1> : B1 { };
template<class B1, class... Bn>
struct disjunction<B1, Bn...>
    : std::conditional<bool(B1::value), B1, disjunction<Bn...>>::type  { };

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

template<class B>
struct negation : bool_constant<!B::value> { };

template< typename T, typename... Ts >
using none_is_same_as = disjunction< negation<std::is_same< T, typename std::decay<Ts>::type >>... >;

template< class R, typename... Ts>
using enable_if_none_is_same_as = std::enable_if< none_is_same_as< R, Ts... >::value >;
}

namespace property_bag
{

enum class RetrievalHandling : std::size_t
{
  QUIET = 0,
  THROW
};

inline std::ostream& operator <<(std::ostream& s, const RetrievalHandling r)
{
  switch (r) {
  case RetrievalHandling::QUIET:
    s << "QUIET";
    break;
  case RetrievalHandling::THROW:
    s << "THROW";
    break;
  }
  return s;
}

template <typename KeyType = std::string>
class AbstractPropertyBag
{
  using PropertyMap = std::map<KeyType, Property>;

  struct WithDocHelper {};

public:

  using iterator = typename PropertyMap::iterator;
  using const_iterator = typename PropertyMap::const_iterator;

  /**
   * @brief 'pimpl' struct to enable access to
   * private members during serialization
   */
  struct serialization_accessor;

  static constexpr WithDocHelper WithDoc = {};

  AbstractPropertyBag()          = default;
  virtual ~AbstractPropertyBag() = default;

  AbstractPropertyBag(const AbstractPropertyBag& rhs);
  AbstractPropertyBag(AbstractPropertyBag&& rhs);

  AbstractPropertyBag& operator=(const AbstractPropertyBag& rhs);
  AbstractPropertyBag& operator=(AbstractPropertyBag&& rhs);

  bool operator==(const AbstractPropertyBag& rhs);

  template <typename T>
  bool addProperty(const KeyType &name, T&& value, const std::string& doc = "")
  {
    auto it = properties_.find(name);

    if (it == properties_.end())
      properties_.emplace(name, Property(std::forward<T>(value), doc));
    else
      return false;

    return true;
  }

  template <typename Name, typename T, typename Doc, typename... Args>
  void addPropertiesWithDoc(Name&& name, T&& value, Doc&& description, Args&&... args)
  {
    ASSERT_NAMED_DOCED_PROPERTIES(KeyType, Name, Doc, Args);

    addProperty(std::forward<Name>(name), std::forward<T>(value),
                std::forward<Doc>(description));

    addPropertiesWithDoc(std::forward<Args>(args)...);
  }

  template <typename Name, typename T, typename... Args>
  void addProperties(Name&& name, T&& value, Args&&... args)
  {
    ASSERT_NAMED_PROPERTIES(KeyType, Name, Args);

    addProperty(std::forward<Name>(name), std::forward<T>(value));
    addProperties(std::forward<Args>(args)...);
  }

  template <typename... Args, typename =
            typename enable_if_none_is_same_as<AbstractPropertyBag, Args...>::type>
  AbstractPropertyBag(Args&&... args) /*:
                                // figure-out unrolling in map
                                // Should be something along the line
                                // build 2 indexes sequences : odd & pair
                                // and std::get<seq...>(args)
    properties_{{(typename PropertyMap::value_type(key, val))...}}*/
  {
    addProperties(std::forward<Args>(args)...);
  }

  template <typename... Args, typename =
            typename enable_if_none_is_same_as<AbstractPropertyBag, Args...>::type>
  AbstractPropertyBag(const WithDocHelper /*withdoc*/, Args&&... args)
  {
    addPropertiesWithDoc(std::forward<Args>(args)...);
  }

  Property& getProperty(const KeyType &name);

  const Property& getProperty(const KeyType &name) const;

  template <typename T>
  bool getPropertyValue(const KeyType &name, T& value,
                        const RetrievalHandling handling) const
  {
    auto it = properties_.find(name);

    if (it != properties_.end())
    {
      try
      {
        value = it->second.template get<T>();
      }
      catch (const PropertyException& e)
      {
        switch (handling) {
        case RetrievalHandling::QUIET:
        {
          return false;
        }
        case RetrievalHandling::THROW:
        {
          std::stringstream ss;
          ss << "named '" << name << "':\n";
          ss << e.what();
          throw PropertyException(ss.str());
          break;
        }
        }
      }
    } else {

      switch (handling) {
      case RetrievalHandling::QUIET:
      {
        return false;
      }
      case RetrievalHandling::THROW:
      {
        std::stringstream ss;
        ss << "named '" << name
           << "' not found in property bag."
           << "\nAvailable properties:";

        for (const auto& n : listProperties())
          ss << "\n\t" << n << std::endl;

        throw PropertyException(ss.str());
      }
      }
    }
    return true;
  }

  template <typename T>
  bool getPropertyValue(const KeyType &name, T& value) const
  {
    return getPropertyValue(name, value, default_handling_);
  }

  template <typename T, typename TT>
  bool getPropertyValue(const KeyType &name, T& value,
                        TT&& default_value) const
  {
    static_assert(std::is_convertible<TT,T>::value,
                  "'default_value' type isn't convertible to "
                  "'value' type in 'value' assignation.");

    bool got = getPropertyValue(name, value, RetrievalHandling::QUIET);

    if (!got) value = std::forward<TT>(default_value);

    return got;
  }

  template <typename T>
  bool updateProperty(const KeyType &name, T&& value)
  {
    auto it = properties_.find(name);

    if (it != properties_.end())
    {
      try
      {
        it->second.template set<T>(std::forward<T>(value));
      }
      catch (const PropertyException& e)
      {
        switch (default_handling_) {
        case RetrievalHandling::QUIET:
        {
          return false;
        }
        case RetrievalHandling::THROW:
        {
          std::stringstream ss;
          ss << "Property '" << name << "':\n";
          ss << e.what();
          throw PropertyException(ss.str());
          break;
        }
        }
      }
    }
    else
      return false;

    return true;
  }

  bool removeProperty(const KeyType &name);

  std::list<KeyType> listProperties() const;

  bool exists(const KeyType& name) const;

  inline size_t size()  const noexcept { return properties_.size(); }
  inline size_t empty() const noexcept { return properties_.empty(); }

  inline void setRetrievalHandling(const RetrievalHandling h) noexcept
  { default_handling_ = h; }

  inline RetrievalHandling getRetrievalHandling() const noexcept
  { return default_handling_; }

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

private:

  Property none_;

  RetrievalHandling default_handling_ = RetrievalHandling::QUIET;

  PropertyMap properties_;

  void addProperties();

  void addPropertiesWithDoc();
};
using PropertyBag = AbstractPropertyBag<std::string>;

} //namespace property_bag

#include <property_bag/property_bag.hpp>
#endif //PROPERTY_BAG_PROPERTY_BAG_H
