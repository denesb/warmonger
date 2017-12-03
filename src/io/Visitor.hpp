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

template <typename C, typename T, typename TG, typename TS>
struct Member
{
    using Type = T;
    using Getter = std::function<TG(const C&)>;
    using Setter = std::function<void(C&, TS)>;

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

struct NoParent
{
};

template <typename Class, typename Parent, typename... Members>
class ClassDescription
{
    template <typename T>
    using StripType = typename std::remove_reference<typename std::decay<T>::type>::type;
public:

    using ThisClass = Class;
    using ParentClass = Parent;

    ClassDescription()
    {
    }

    ClassDescription(std::tuple<Members...>&& members)
        : members(std::forward<std::tuple<Members...>>(members))
    {
    }

    template <typename C, typename T>
    auto visitMember(const char* name, T (C::*getter)() const)
    {
        static_assert(std::is_same<C, Class>::value, "Member must belong to the visited class");
        return appendMember(Member<C, StripType<T>, T, NoSetter>(name, getter));
    }

    template <typename CG, typename TG, typename CS, typename TS>
    auto visitMember(const char* name, TG (CG::*getter)() const, void (CS::*setter)(TS))
    {
        static_assert(std::is_same<CG, CS>::value, "Getter and Setter must be a member of the same class");
        static_assert(
            std::is_same<StripType<TG>, StripType<TS>>::value, "Getter and Setter must operate on the same type");
        static_assert(std::is_same<CG, Class>::value, "Member must belong to the visited class");
        return appendMember(Member<CG, StripType<TG>, TG, TS>(name, getter, setter));
    }

    template <typename... Args>
    auto visitConstructor(Args&&...)
    {
        //TODO: implement
        return *this;
    }

    QJsonObject operator()(const Class&)
    {
        return QJsonObject();
    }

    std::tuple<Members...> getMembers()
    {
        return this->members;
    }

private:
    template <typename Member>
    auto appendMember(Member&& member)
    {
        auto newMember = std::make_tuple<Member>(std::forward<Member>(member));
        auto allMembers = std::tuple_cat(std::move(newMember), std::move(members));
        return ClassDescription<Class, Parent, Member, Members...>(std::move(allMembers));
    }

    std::tuple<Members...> members;
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
