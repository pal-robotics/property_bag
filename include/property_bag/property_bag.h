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
  std::is_same<typename std::decay<T>::type, const char*>::value>;

#define ASSERT_NAMED_PROPERTIES(NAME, ARGS) \
  static_assert(is_string<NAME>::value, \
    "Error PropertyBag::setValue :\nparameter name must be a string."); \
  static_assert(sizeof...(Args)%2==0, \
  "Error PropertyBag::setValue :\nparameters work by pair, a name (std::string) and a value."); \

#define ASSERT_NAMED_DOCED_PROPERTIES(NAME, DOC, ARGS) \
  static_assert(is_string<NAME>::value, \
    "Error PropertyBag::setValue :\nparameter name must be a string."); \
  static_assert(is_string<DOC>::value, \
    "Error PropertyBag::setValue :\nparameter description must be a string."); \
  static_assert(sizeof...(ARGS)%3==0, \
    "Error PropertyBag::setValue :\nparameters work by pair, a name (std::string) and a value."); \

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

class PropertyBag
{
  using PropertyMap = std::map<std::string, Property>;

public:

  using iterator = PropertyMap::iterator;
  using const_iterator = PropertyMap::const_iterator;

  /**
   * @brief 'pimpl' struct to enable access to
   * private members during serialization
   */
  struct serialization_accessor;

  struct WithDoc {};

  PropertyBag()          = default;
  virtual ~PropertyBag() = default;

  PropertyBag(const PropertyBag& rhs);
  PropertyBag(PropertyBag&& rhs);

  PropertyBag& operator=(const PropertyBag& rhs);
  PropertyBag& operator=(PropertyBag&& rhs);

  bool operator==(const PropertyBag& rhs);

  template <typename T>
  bool addProperty(const std::string &name, T&& value, const std::string& doc = "")
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
    ASSERT_NAMED_DOCED_PROPERTIES(Name, Doc, Args);

    addProperty(std::forward<Name>(name), std::forward<T>(value),
                std::forward<Doc>(description));

    addPropertiesWithDoc(std::forward<Args>(args)...);
  }

  template <typename Name, typename T, typename... Args>
  void addProperties(Name&& name, T&& value, Args&&... args)
  {
    ASSERT_NAMED_PROPERTIES(Name, Args);

    addProperty(std::forward<Name>(name), std::forward<T>(value));
    addProperties(std::forward<Args>(args)...);
  }

  template <typename... Args, typename =
            typename enable_if_none_is_same_as<PropertyBag, Args...>::type>
  PropertyBag(Args&&... args) /*:
                                // figure-out unrolling in map
                                // Should be something along the line
                                // build 2 indexes sequences : odd & pair
                                // and std::get<seq...>(args)
    properties_{{(typename PropertyMap::value_type(key, val))...}}*/
  {
    addProperties(std::forward<Args>(args)...);
  }

  template <typename... Args, typename =
            typename enable_if_none_is_same_as<PropertyBag, Args...>::type>
  PropertyBag(const WithDoc /*withdoc*/, Args&&... args)
  {
    addPropertiesWithDoc(std::forward<Args>(args)...);
  }

  Property& getProperty(const std::string &name);

  const Property& getProperty(const std::string &name) const;

  template <typename T>
  bool getPropertyValue(const std::string &name, T& value,
                        const RetrievalHandling handling) const
  {
    auto it = properties_.find(name);

    if (it != properties_.end()){
      if (handling == RetrievalHandling::QUIET){
        try{
          value = it->second.get<T>();
        }
        catch (PropertyException& e){
          //std::cerr << "While retrieving property "
          //          << name << " got " << e.what() << std::endl;
          return false;
        }
      }
      else{
        value = it->second.get<T>();
      }
    }
    else{
      if (handling == RetrievalHandling::THROW){
        std::stringstream ss;
        ss << "Variable '" << name
           << "' not found in property bag."
           << "\nAvailable variables:";
        auto variales = listProperties();
        for (auto it = variales.begin(); it != variales.end(); ++it){
          ss << "\n\t" << *it << std::endl;
        }
        throw PropertyException(ss.str());
      }
      return false;
    }

    return true;
  }

  template <typename T>
  bool getPropertyValue(const std::string &name, T& value) const
  {
    return getPropertyValue(name, value, default_handling_);
  }

  template <typename T>
  bool getPropertyValue(const std::string &name, T& value,
                        T&& default_value) const
  {
    bool got = getPropertyValue(name, value, RetrievalHandling::QUIET);

    if (!got) value = std::forward<T>(default_value);

    return got;
  }

  template <typename T>
  bool updateProperty(const std::string &name, T&& value)
  {
    auto it = properties_.find(name);

    if (it != properties_.end())
      /*value = */it->second.set<T>(std::forward<T>(value));
    else
      return false;

    return true;
  }

  bool removeProperty(const std::string &name);

  std::list<std::string> listProperties() const;

  bool exists(const std::string& name) const;

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

} //namespace property_bag

#endif //PROPERTY_BAG_PROPERTY_BAG_H
