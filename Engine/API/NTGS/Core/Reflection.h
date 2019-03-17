#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <functional>
#include <any>
#include <map>
#include <utility>

namespace NTGS {
    class Object;
    template<typename Type>
    class ClassProperty;
    template<typename T>
    class ClassMethod;




    template<typename T>
    struct HasReference : std::false_type {};

    template<typename T>
    struct HasReference<T&> :std::true_type {};

    template<typename T>
    struct HasReference<T&&> :std::true_type {};




    class IProperty {
    public:
        virtual std::any Get(std::any pInstance) = 0;
    };
    



    class IMethod {
    public:
        virtual std::any Invoke(std::any pInstance, std::any cArgsTuple) = 0;
    };




    template<typename A, typename Class>
    class ClassProperty<A(Class::*)> final : public IProperty {
    public:
        using PropertyType = A(Class::*);
        using ClassType = Class;

        ClassProperty(A(Class::*t)) : mProperty(t) {}

        std::any Get(std::any pInstance) {
            Object* pObject = std::any_cast<Object*>(pInstance);
            ClassType* pClass = dynamic_cast<ClassType*>(pObject);
            return std::any(&(pClass->*mProperty));
        }
    private:
        PropertyType mProperty;
    };


   

    template<typename ReturnType, typename Class, typename...ArgsType>
    class ClassMethod<ReturnType(Class::*)(ArgsType...)> final : public IMethod {
    public:
        using FunctionType = ReturnType(Class::*)(ArgsType...);
        using ClassType = Class;

        ClassMethod(FunctionType pFunc) :mFunction(pFunc) {}
 
        std::any Invoke(std::any pInstance, std::any cArgsTuple) {
            try {
                auto cTuple = std::any_cast<std::tuple<ArgsType...>>(cArgsTuple);
                return InvokeHelp(std::make_index_sequence<sizeof...(ArgsType)>{}, cTuple, pInstance);
            }
            catch (std::bad_any_cast& cError) {
                auto Str = std::string("the invoke args's type is not as the same as method's");
                std::cout << Str << std::endl;
                throw Str;
            }
        }

    private:
        template<std::size_t...Indexs, typename TupleType>
        std::any InvokeHelp(std::index_sequence<Indexs...>, TupleType cTuple, std::any pInstance) {
            return Call(pInstance, std::get<Indexs>(cTuple)...);
        }

        std::any Call(std::any pInstance, ArgsType...cArgs) {
            Object* pObject = std::any_cast<Object*>(pInstance);
            return CallHelp<ReturnType>(dynamic_cast<ClassType*>(pObject), cArgs...);
        }

        template<typename RType>
        std::enable_if_t<std::is_void_v<RType>, std::any> CallHelp(ClassType* pObject, ArgsType...cArgs) {
            (pObject->*mFunction)(cArgs...);
            return std::any(0);
        }

        template<typename RType>
        std::enable_if_t<!std::is_void_v<RType>, std::any> CallHelp(ClassType* pObject, ArgsType...cArgs) {
            if (HasReference<ReturnType>::value) {
                return &(pObject->*mFunction)(cArgs...);
            }
            return (pObject->*mFunction)(cArgs...);
        }

    private:
        FunctionType mFunction;
    };




    class Object {
    public:
        Object() = default;
        virtual ~Object() = default;
    
        template<typename Type>
        void AddProperty(const std::string& cName, Type cData) {
            auto cPointer = std::make_shared<ClassProperty<Type>>(cData);
            mPropertyMap.emplace(cName, cPointer);
        }
        template<typename FunctionType>
        void AddMethod(const std::string& cName, FunctionType cFunc) {
            auto cPointer = std::make_shared<ClassMethod<FunctionType>>(cFunc);
            mMethodMap.emplace(cName, cPointer);
        }

        template<typename ConvertType>
        ConvertType& Get(const std::string& cName) {
            auto cIter = mPropertyMap.find(cName);
            if (cIter != mPropertyMap.end()) {
                ConvertType* pData = std::any_cast<ConvertType*>(cIter->second->Get(this));
                return *pData;
            }
            return (ConvertType&) (*this);  //bad data
        }

        template<typename ConvertType, typename...ArgsType>
        std::enable_if_t<!std::is_void_v<ConvertType>, ConvertType> Invoke(const std::string& cName, ArgsType&&...cArgs) {
            auto cIter = mMethodMap.find(cName);
            if (cIter != mMethodMap.end()) {
                auto cTuple = std::tuple<ArgsType...>(std::forward<ArgsType>(cArgs)...);
                if (HasReference<ConvertType>::value) {
                    using pointType = typename std::remove_reference<ConvertType>::type*;
                    auto cPointer = std::any_cast<pointType>(cIter->second->Invoke(this, cTuple));
                    return *cPointer;
                }
                return std::any_cast<ConvertType>(cIter->second->Invoke(this, cTuple));
            }
            return (ConvertType&) (*this);
        };

        template<typename ConvertType, typename...ArgsType>
        std::enable_if_t<std::is_void_v<ConvertType>> Invoke(const std::string& cName, ArgsType&&...cArgs) {
            Invoke(cName, std::forward<ArgsType>(cArgs)...);
        };

        template<typename...ArgsType>
        void Invoke(const std::string& cName, ArgsType&&...cArgs) {
            auto cIter = mMethodMap.find(cName);
            if (cIter != mMethodMap.end()) {
                auto cTuple = std::tuple<ArgsType...>(std::forward<ArgsType>(cArgs)...);
                cIter->second->Invoke(this, cTuple);
            }
        };

    private:
        std::map<std::string, std::shared_ptr<IProperty>> mPropertyMap;
        std::map<std::string, std::shared_ptr<IMethod>> mMethodMap;
    };
}