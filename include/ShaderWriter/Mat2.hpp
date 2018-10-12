﻿/*
See LICENSE file in root folder
*/
#ifndef ___Writer_Mat2_H___
#define ___Writer_Mat2_H___
#pragma once

#include "Vec2.hpp"

namespace sdw
{
	template< typename ValueT >
	struct Mat2T
		: public Value
	{
		using ValueType = ValueT;
		using my_vec = Vec2T< ValueT >;
		using my_mat = Mat2T< ValueT >;

		inline Mat2T( stmt::Container * container
			, expr::ExprPtr expr );
		inline Mat2T< ValueT > & operator=( Mat2T< ValueT > const & rhs );
		template< typename RhsT >
		inline Mat2T< ValueT > & operator=( RhsT const & rhs );
		template< typename IndexT >
		inline Vec2T< ValueT > operator[]( IndexT const & rhs )const;
	};
}

#include "Mat2.inl"

#endif