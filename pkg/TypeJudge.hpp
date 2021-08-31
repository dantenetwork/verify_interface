#pragma once

#include "m_eval_if_c.hpp"
#include "m_static_type.hpp"
#include <type_traits>

template <typename T>
struct ObjectTypeJudge
{
	using type = T;

	//static_assert((!std::is_trivially_move_assignable<T>::value), "no user defined move assign");
	//static_assert((!std::is_trivially_move_constructible<T>::value), "no user defined move constructor");
	//static_assert((!std::is_trivially_copy_assignable<T>::value), "no user defined copy assign");
	//static_assert((!std::is_trivially_copy_constructible<T>::value), "no user defined copy constructor");

	static_assert(m_static_type<!std::is_trivially_move_assignable<T>::value, T>::value, "trivially move assign");
	static_assert(m_static_type<std::is_move_assignable<T>::value, T>::value, "can not move assign");

	static_assert(m_static_type<!std::is_trivially_move_constructible<T>::value, T>::value, "trivially move constructor");
	static_assert(m_static_type<std::is_move_constructible<T>::value, T>::value, "can not move constructor");

	static_assert(m_static_type<!std::is_trivially_copy_assignable<T>::value, T>::value, "trivially copy assign");
	static_assert(m_static_type<std::is_copy_assignable<T>::value, T>::value, "can not copy assign");

	static_assert(m_static_type<!std::is_trivially_copy_constructible<T>::value, T>::value, "trivially copy constructor");
	static_assert(m_static_type<std::is_copy_constructible<T>::value, T>::value, "can not copy constructor");
};

template <typename T>
struct DefaultTypeJudge
{
	using type = T;
	//以下静态断言纯粹为了测试元条件能够到此分支
//	static_assert((std::is_class<T>::value), "default type is not class");
};

template <typename T>
struct TypeJudge: public m_eval_if_c<std::is_class<T>::value, ObjectTypeJudge<T>, DefaultTypeJudge<T>>
{

};
