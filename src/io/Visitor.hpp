/**
 * \copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_IO_VISITOR_HPP
#define W_IO_VISITOR_HPP

#include <type_traits>

namespace warmonger {
namespace io {

struct NoSetter
{
};

template <typename T>
using StripType = typename std::remove_reference<typename std::decay<T>::type>::type;

template <typename T, typename U>
struct RebindContainer;

template <typename T, typename U>
struct RebindContainer<std::vector<T>, U>
{
    using Type = std::vector<U>;
};

template <typename C, typename TG, typename TS, typename TGetter, typename TSetter>
struct Member
{
    using TypeGet = TG;
    using TypeSet = TS;
    using Getter = TGetter;
    using Setter = TSetter;

    Member(const char* name, Getter getter, Setter setter = Setter())
        : name(name)
        , getter(std::forward<Getter>(getter))
        , setter(std::forward<Setter>(setter))
    {
    }

    const char* name;
    Getter getter;
    Setter setter;
};

template <typename C, typename TG, typename TS>
using NormalMember = Member<C, StripType<TG>, StripType<TS>, std::function<TG(const C&)>, std::function<void(C&, TS)>>;

template <typename C, typename TG, typename TS1, typename TS2>
using MemberWithPiecewiseSetter = Member<C,
      StripType<TG>,
      typename RebindContainer<StripType<TG>, std::unique_ptr<TS2>>::Type,
      std::function<TG(const C&)>,
      std::function<TS1(C&, std::unique_ptr<TS2>)>>;

struct NoParent
{
};

template <typename... MemberDefs>
class Members
{
public:
    Members() = default;

    Members(std::tuple<MemberDefs...>&& members)
        : members(std::forward<std::tuple<MemberDefs...>>(members))
    {
    }

    template <typename C, typename T>
    auto visitMember(const char* name, T (C::*getter)() const)
    {
        return appendMember(NormalMember<C, T, NoSetter>(name, getter));
    }

    template <typename CG, typename TG, typename CS, typename TS>
    auto visitMember(const char* name, TG (CG::*getter)() const, void (CS::*setter)(TS))
    {
        return appendMember(NormalMember<CG, TG, TS>(name, getter, setter));
    }

    template <typename CG, typename TG, typename CS, typename TS1, typename TS2>
    auto visitMember(const char* name, TG (CG::*getter)() const, TS1 (CS::*setter)(std::unique_ptr<TS2>))
    {
        return appendMember(MemberWithPiecewiseSetter<CG, TG, TS1, TS2>(name, getter, setter));
    }

    const std::tuple<MemberDefs...>& asTuple() const
    {
        return this->members;
    }

private:
    template <typename MemberDef>
    auto appendMember(MemberDef&& member)
    {
        auto newMember = std::make_tuple<MemberDef>(std::forward<MemberDef>(member));
        auto allMembers = std::tuple_cat(std::move(members), std::move(newMember));
        return Members<MemberDefs..., MemberDef>(std::move(allMembers));
    }

    std::tuple<MemberDefs...> members;
};

template <typename T>
struct ConstructorArg
{
    using Type = T;

    ConstructorArg(const char* name)
        : name(name)
    {
    }

    const char* name;
};

template <typename... ConstructorArgDefs>
class ConstructorArgs
{
public:
    ConstructorArgs(ConstructorArgDefs&&... constructorArgs)
        : constructorArgs(std::make_tuple(std::forward<ConstructorArgDefs>(constructorArgs)...))
    {
    }

    const std::tuple<ConstructorArgDefs...>& asTuple() const
    {
        return this->constructorArgs;
    }

private:
    std::tuple<ConstructorArgDefs...> constructorArgs;
};

template <typename Class, typename Parent, typename ClassMembers = Members<>, typename ClassConstructorArgs = ConstructorArgs<>>
class ClassDescription
{
public:
    using ThisClass = Class;
    using ParentClass = Parent;

    ClassDescription() = default;

    ClassDescription(ClassMembers&& members, ClassConstructorArgs&& constructorArgs)
        : members(std::forward<ClassMembers>(members))
        , constructorArgs(std::forward<ClassConstructorArgs>(constructorArgs))
    {
    }

    template <typename C, typename T>
    auto visitMember(const char* name, T (C::*getter)() const)
    {
        static_assert(std::is_same<C, Class>::value, "Member must belong to the visited class");
        auto newMembers = this->members.visitMember(name, getter);
        return ClassDescription<Class, Parent, decltype(newMembers), ClassConstructorArgs>(std::move(newMembers), std::move(this->constructorArgs));
    }

    template <typename CG, typename TG, typename CS, typename TS>
    auto visitMember(const char* name, TG (CG::*getter)() const, void (CS::*setter)(TS))
    {
        static_assert(std::is_same<CG, CS>::value, "Getter and Setter must be a member of the same class");
        static_assert(
            std::is_same<StripType<TG>, StripType<TS>>::value, "Getter and Setter must operate on the same type");
        static_assert(std::is_same<CG, Class>::value, "Member must belong to the visited class");
        auto newMembers = this->members.visitMember(name, getter, setter);
        return ClassDescription<Class, Parent, decltype(newMembers), ClassConstructorArgs>(std::move(newMembers), std::move(this->constructorArgs));
    }

    template <typename CG, typename TG, typename CS, typename TS1, typename TS2>
    auto visitMember(const char* name, TG (CG::*getter)() const, TS1 (CS::*setter)(std::unique_ptr<TS2>))
    {
        static_assert(std::is_same<CG, CS>::value, "Getter and Setter must be a member of the same class");
        static_assert(
            std::is_same<StripType<TS1>, StripType<TS2>*>::value, "Piecewise setter must take std::unique_ptr to the same type it returns");
        static_assert(
            std::is_same<typename StripType<TG>::value_type, StripType<TS1>>::value, "Getter and Setter must operate on the same type");
        static_assert(std::is_same<CG, Class>::value, "Member must belong to the visited class");
        auto newMembers = this->members.visitMember(name, getter, setter);
        return ClassDescription<Class, Parent, decltype(newMembers), ClassConstructorArgs>(std::move(newMembers), std::move(this->constructorArgs));
    }

    template <typename... ArgTypes>
    auto visitConstructor(ConstructorArg<ArgTypes>&&... args)
    {
        static_assert(std::tuple_size<StripType<decltype(this->constructorArgs.asTuple())>>::value == 0, "Constructor already defined");
        auto constructorArgs = ConstructorArgs<ConstructorArg<ArgTypes>...>(std::forward<ConstructorArg<ArgTypes>>(args)...);
        return ClassDescription<Class, Parent, ClassMembers, decltype(constructorArgs)>(std::move(this->members), std::move(constructorArgs));
    }

    ClassMembers getMembers()
    {
        return this->members;
    }

    ClassConstructorArgs getConstructorArgs()
    {
        return this->constructorArgs;
    }

private:
    ClassMembers members;
    ClassConstructorArgs constructorArgs;
};

template <class Class>
struct Visitor
{
    template <class Parent>
    ClassDescription<Class, Parent> visitParent()
    {
        return ClassDescription<Class, Parent>();
    }
};

} // namespace io
} // namespace warmonger

#endif // W_IO_VISITOR_HPP
