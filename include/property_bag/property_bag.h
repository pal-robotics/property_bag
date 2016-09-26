/**
 * \file property_bag.h
 * \brief Property container holding many different type.
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_PROPERTY_BAG_H
#define PROPERTY_BAG_PROPERTY_BAG_H

#include <list>

#include "property_bag/property.h"

namespace property_bag
{
class PropertyBag
{
  using PropertyMap = std::map<std::string, Property>;

public:

  PropertyBag()          = default;
  virtual ~PropertyBag() = default;

  PropertyBag(const PropertyBag& rhs);
  PropertyBag& operator=(const PropertyBag& rhs);

  template <typename... Args>
  PropertyBag(const Args&... args)
  {
    addProperties(args...);
  }

  template <typename Name, typename T, typename... Args>
  void addProperties(const Name &name, const T &value, const Args&... args)
  {
    // Assert that name is a string (char*)
    static_assert(is_string<Name>::value,
      "Error PropertyBag::setValue :\nparameter name must be a string.");

    // Assert that each name as an associated value and vice-versa
    static_assert(sizeof...(Args)%2==0,
    "Error PropertyBag::setValue :\nparameters work by pair, a name (std::string) and a value.");

    addProperty(std::forward<const std::string>(name), std::forward<const T>(value));
    addProperties(std::forward<const Args>(args)...);
  }

  template <typename Name, typename T, typename Doc, typename... Args>
  void addPropertiesWithDoc(const Name &name, const T &value, const Doc &description, const Args&... args)
  {
    // Assert that name is a string (char*)
    static_assert(is_string<Name>::value,
      "Error PropertyBag::setValue :\nparameter name must be a string.");

    // Assert that description is a string (char*)
    static_assert(is_string<Doc>::value,
      "Error PropertyBag::setValue :\nparameter description must be a string.");

    // Assert that each name as an associated value and vice-versa
    static_assert(sizeof...(Args)%3==0,
    "Error PropertyBag::setValue :\nparameters work by pair, a name (std::string) and a value.");

    addProperty(std::forward<const std::string>(name),
                std::forward<const T>(value),
                std::forward<const std::string>(description));

    addPropertiesWithDoc(std::forward<const Args>(args)...);
  }

  template <typename T>
  bool addProperty(const std::string &name, const T& value, const std::string& doc = "")
  {
    auto it = properties_.find(name);

    if (it == properties_.end())
      properties_.emplace(name, Property(std::forward<const T>(value),
                                         std::forward<const std::string>(doc)));
    else
      return false;

    return true;
  }

  Property& getProperty(const std::string &name);

  const Property& getProperty(const std::string &name) const;

  template <typename T>
  bool getPropertyValue(const std::string &name, T& value)
  {
    auto it = properties_.find(name);

    if (it != properties_.end())
      value = it->second.get<T>();
    else
      return false;

    return true;
  }

  template <typename T>
  bool updateProperty(const std::string &name, const T& value)
  {
    auto it = properties_.find(name);

    if (it != properties_.end())
      value = it->second.set<T>(value);
    else
      return false;

    return true;
  }

  bool removeProperty(const std::string &name);

  std::list<std::string> listProperties() const;

  bool exists(const std::string& name);

  inline size_t size()  const { return properties_.size(); }
  inline size_t empty() const { return properties_.empty(); }

private:

  Property none_;

  PropertyMap properties_;

  void addProperties();

  void addPropertiesWithDoc();

  template <typename T>
  using is_string =
  std::integral_constant<bool,
    std::is_same<typename std::decay<T>::type, char*>::value ||
    std::is_same<typename std::decay<T>::type, const char*>::value>;

  friend class boost::serialization::access;

  BOOST_SERIALIZATION_SPLIT_MEMBER()

  /** save support for SuperCounter */
  template <typename Archive>
  void save(Archive &ar, const unsigned int /*version*/) const
  {
    ar << BOOST_SERIALIZATION_NVP(properties_);
  }

  /** load support for SuperCounter */
  template <typename Archive>
  void load(Archive &ar, const unsigned int /*version*/)
  {
    // Who knows why I can't load directly in properties_ ...
    PropertyMap map;
    ar >> boost::serialization::make_nvp("properties_", map);
    properties_ = map;
  }
};

} //namespace property_bag

#endif //PROPERTY_BAG_PROPERTY_BAG_H