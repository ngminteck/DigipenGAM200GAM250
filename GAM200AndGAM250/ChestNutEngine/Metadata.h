/******************************************************************************/
/*!
\file  Metadata.h

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for Metadata class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <tuple>
#include <map>
#include <functional>
#include <array>
#include <memory>
#include "TypeErasedObj.h"

template<unsigned N, typename ... Rest>
struct GenNameTupleType : GenNameTupleType<N - 1, const char *, Rest...>
{

};

template<typename ... List>
struct GenNameTupleType<0, List...>
{
	using type = std::tuple<List...>;
};

template<typename T, typename ... PtrToMems>
class Metadata
{
    using NameTupleType 
       = typename GenNameTupleType<sizeof...(PtrToMems)>::type;
    using PtrMemTuple = std::tuple<PtrToMems...>;
    const char *class_name;    
	PtrMemTuple ptrs; 
    NameTupleType names;
    
    template<typename Container>
    using DataMap = std::map<std::string, Container>;

    template<typename TypeErasedObj, size_t ... Indices>
    auto reflect_aux(T& rhs, std::index_sequence<Indices...>) const
    {
        return DataMap<TypeErasedObj>
               {
                   std::make_pair(std::string(std::get<Indices>(names)), 
                                  TypeErasedObj{std::invoke(std::get<Indices>(ptrs), rhs)})...
               };
    }

public:
       
    template<typename TypeErasedObj>
    auto reflect(T& rhs) const
    {
        return reflect_aux<TypeErasedObj>(rhs, std::make_index_sequence<sizeof...(PtrToMems)>{});
    }
	
	const auto& GetPtrToMembers() const
	{
		return ptrs; 
	}

    constexpr Metadata(const char *cn, const PtrMemTuple&  ptr_tuple, 
                       const NameTupleType & name_tuple)
       : class_name{cn}, ptrs{ptr_tuple}, names{name_tuple}
    {

    }
};