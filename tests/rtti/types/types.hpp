#pragma once
#include "platform/configure.h"
#include <EASTL/string.h>
#include <EASTL/unique_ptr.h>

// TODO: Move To Core
struct SRuntimeAttribute
{

};

template<typename T>
void XXXInformation(int);
void PrintField();
inline void CreateBuffers()
{

}

using EnumValue = uint32_t;

namespace Types sreflect
{

sreflect_enum("guid" : "1a0b91c7-6690-41d6-acfd-0c2b61f181f3")
sattr("rtti" : true)
TestEnum : uint32_t
{
    Value0 = 0, 
    Value1 = 1, 
    Value2 = 2 
}
sstatic_ctor(1, XXXInformation<TestEnum>(1));

sreflect_struct("guid" : "25809bab-41e8-48c5-806b-1ae4af3edfef")
sattr("rtti" : true)
TestParent
{
    eastl::string buffer;
}
sstatic_ctor(0, CreateBuffers());

sreflect_struct("guid" : "d55175b2-9d7f-47b8-bccd-a06aeac55240")
sattr("rtti" : true)
TestSon : public TestParent
{
    eastl::string sex;
}
sstatic_ctor(0, XXXInformation<TestSon>(1));

}

template<typename T>
struct XXXInformationAttribute : public SRuntimeAttribute
{

};

template <>
struct eastl::default_delete<struct SRuntimeAttribue>
{
    void operator()(struct SRuntimeAttribue* p) const
    {

    }
};

sattr("target" : [ "field", "method" ])
inline void PrintField()
{

}

inline void DontCreateObject()
{

}

template<typename T>
void XXXInformation()
{
    // return nullptr;
    // return eastl::make_unique<XXXInformationAttribute<T>>();
    // ->
    // registry->add_runtime_attribute(eastl::make_unique<XXXInformationAttribute<T>>());
}