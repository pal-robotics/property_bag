^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package property_bag
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Forthcoming
-----------
* added more propperty bag serialization types
* Contributors: Hilario Tome

0.0.6 (2019-03-18)
------------------
* Fix system include
* Add licence to package.xml
* Add LGPL v· license
* Contributors: Victor Lopez

0.0.5 (2018-01-11)
------------------
* Merge branch 'bug_fix' into 'dubnium-devel'
  fix bug return ptr to tmp obj
  See merge request common/property_bag!14
* fix bug return ptr to tmp obj
* Merge branch 'ros_seria' into 'dubnium-devel'
  ros seria use make_nvp & make_array
  See merge request common/property_bag!12
* ros seria use make_nvp & make_array
* added missing catkin include dirs
* Merge branch 'concatenate-bags' into 'dubnium-devel'
  Add concatenate bags
  See merge request common/property_bag!11
* Include property-bag.h from property_bag.hpp
* Rename to append
* Add concatenate bags
* Merge branch 'add-ros-types' into 'dubnium-devel'
  Add ros message serialization for some basic types
  See merge request common/property_bag!10
* Add ros message serialization for some basic types
* Contributors: Hilario Tome, Jeremie Deray, Victor Lopez

0.0.4 (2017-10-25)
------------------
* Merge branch 'update' into 'dubnium-devel'
  Update
  See merge request !7
* moar tests
* eigen seria missing template params
* rm comment
* eigen seria gtest
* moar gtest
* moar gtest
* gtest serialization bag name
* gtest serialization bag RetrievalHandling
* add bag name gtest
* serialize bag name
* serialize missing bag retrieval handling value
* typo property_bag.hpp
* fix test compilation
* add PropertyBag name
* fix getPropertyValue with default value
* catch Bag catch Property error and possibly re-throw with prop name
* WithDoc without instantiation brackets
* RetrievalHandling stream operator
* Merge branch 'fix-boost-v-1.56+' into 'dubnium-devel'
  mv traits outside boost guards
  See merge request !8
* Merge branch 'fix-header-guards' into 'dubnium-devel'
  add header guards to property_bag.hpp
  See merge request !9
* add header guards to property_bag.hpp
* mv traits outside boost guards
* Contributors: Hilario Tome, Jeremie Deray

0.0.3 (2017-07-24)
------------------
* Merge branch 'package-format-2' into 'dubnium-devel'
  Update to package format 2 and reorganize dependencies
  See merge request !6
* Update to package format 2 and reorganize dependencies
* Contributors: Victor Lopez

0.0.2 (2017-07-24)
------------------
* Merge branch 'template-key-type' into 'dubnium-devel'
  Make key type of property bag a template
  See merge request !5
* fix assert macro
* Make key type of property bag a template
* Merge branch 'extra_serialization_registration' into 'dubnium-devel'
  added extra registrations
  See merge request !4
* remove comments
* added extra registrations
* register seria in cpp
* Merge branch 'all_kind_of_fixes' into 'dubnium-devel'
  All kind of fixes
  See merge request !3
* add tests
* PropertyBag iterators
* PropertyBag fix forwarding, move, macro
* Property fix forwarding all the way long
* gtest dummy class
* mv seria related to sub folder
* Merge branch 'fix_ptr_bug_and_seria' into 'dubnium-devel'
  Fix ptr bug and seria
  See merge request !2
* missing eigen deps
* wip cmake cleanup
* rm boost seria from PropertyBag & use pimpl non-intrusive scheme & rm to_str
* rm boost seria from property & use pimpl non-intrusive scheme
* use utils demangling in type_name()
* add utils typeid demangling
* rm Formater.h
* a lot more test
* seria swap maps
* add to_str
* keywords
* fix getPropertyValue & add set/get retrievalHandling
* some doc & args forwards
* add nasty trick for constructor init with doc
* moar keywords
* Any use make_ptr & dynamic_pointer_cast & empty
* PlaceHolderImpl fix ref & use cpp11 keywords
* add dynamic_pointer_cast & empty(ptr)
* added missing cmake_modules to package
* added install rules
* add getPropertyValue(name, value, default_value) -> no throw
* format throw msg
* use RetrievalHandling
* universal ref
* add enum RetrievalHandling
* Added gitignore and const
* Added Eigen support
* Merge branch 'fix_cmake' into 'dubnium-devel'
  fix cmake tests
  fix cmake tests
  See merge request !1
* fix
* fix doc
* project upload
* init
* Contributors: Hilario Tome, Jeremie Deray, Victor Lopez
