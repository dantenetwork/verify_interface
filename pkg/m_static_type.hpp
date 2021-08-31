#pragma once

template <bool b, typename T>
struct if_T_Type_Any{
	
};

template <typename T>
struct  if_T_Type_Any<false, T>
{
	//故意触发编译错误，当T为class时，可以在编译信息中打印出下面这句话以及T的真正类型名
	T::导致以下编译错误的类型名称为：;
};

template <bool b, typename T>
struct m_static_type : public if_T_Type_Any<b, T>
{
	static const bool value = b;
};

template <typename T1, typename T2>
struct is_type_equal
{
	static const bool value = false;
};

template <typename T1>
struct is_type_equal<T1, T1>
{
	static const bool value = true;
};

const unsigned long Floating_Point_Min_Precision = 1000000;

template<typename T>
bool isEqual(const T& d1, const T& d2)
{
	static_assert(is_type_equal<T, double>::value || is_type_equal<T, float>::value, "isEqual is just for double or float!");

	T dis = d1 - d2;

	T T_Precision = 1 / (T)Floating_Point_Min_Precision;

	if ((dis < -T_Precision) || (dis > T_Precision))
	{
		return false;
	}

	return true;
}

