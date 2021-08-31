#pragma once

//C++11ò��û�����Ԫ�������Լ�ʵ�֣�����������boost
template <bool bFlag, typename T1, typename T2>
struct m_if_c
{
	using type = T1;
};

template <typename T1, typename T2>
struct m_if_c<false, T1, T2>
{
	using type = T2;
};

template <bool bFlag, typename T1, typename T2>
struct m_eval_if_c : public m_if_c<bFlag, T1, T2>::type
{

};
