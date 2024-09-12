#ifndef _PROPERTY_BAG_UTILS_H_
#define _PROPERTY_BAG_UTILS_H_

#include <typeinfo>
#include <map>

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

// http://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname

namespace property_bag
{
namespace details
{
inline std::string demangle(const char* name)
{
  int status = -4; // some arbitrary value to eliminate the compiler warning

  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void(*)(void*)> res {
    abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free};

  return (status==0) ? res.get() : name ;
}
} //namespace details
} //namespace property_bag

#else
namespace property_bag
{
namespace details
{
// does nothing if not g++
inline std::string demangle(const char* name) { return name; }
} //namespace details
} //namespace property_bag
#endif

namespace
{
class TypeMapper
{
  // <mangled_type, demangled_type>
  typedef std::map<std::string, std::string> TypeMap;

public:

  TypeMapper()          = default;
  virtual ~TypeMapper() = default;

  TypeMapper(TypeMapper&)      = delete;
  void operator=(TypeMapper&)  = delete;

  const std::string& lookup(const std::type_info& ti)
  {
    return lookup(ti.name());
  }

protected:

  const std::string& lookup(const std::string& mangled)
  {
    std::string mangled_tmp(mangled);

    if (mangled_tmp == typeid(mangled).name())
      mangled_tmp = "std::string";

    TypeMap::iterator iter = type_map_.find(mangled_tmp);

    if (iter != type_map_.end()) return iter->second;

    std::string& registered = type_map_[mangled_tmp];

    registered = property_bag::details::demangle(mangled_tmp.c_str());

    return registered;
  }

  TypeMap type_map_;
};

} // namespace

namespace property_bag
{
namespace details
{
static TypeMapper type_mapper_inst;

inline const std::string& name_of(const std::type_info& ti)
{
  return type_mapper_inst.lookup(ti);
}
} // namespace details

template<typename T>
inline const std::string& name_of()
{
  static const std::string& name_cache = details::name_of(typeid(T));
  return name_cache;
}

} // namespace property_bag

namespace {

#define PROPERTY_BAG_REGISTER_NAME_OF(...) \
  /*const std::string& dummy = */property_bag::name_of<__VA_ARGS__>();

}

#endif /* _PROPERTY_BAG_UTILS_H_ */
