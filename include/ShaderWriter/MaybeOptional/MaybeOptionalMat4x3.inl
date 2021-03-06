/*
See LICENSE file in root folder
*/
#include "MaybeOptionalMat4x3.hpp"

namespace sdw
{
	template< typename ValueT >
	MaybeOptional< Mat4x3T< ValueT > >::MaybeOptional( Shader * shader
		, expr::ExprPtr expr )
		: Mat4x3T< ValueT >{ shader, std::move( expr ) }
		, m_optional{ false }
		, m_enabled{ true }
	{
	}

	template< typename ValueT >
	MaybeOptional< Mat4x3T< ValueT > >::MaybeOptional( Shader * shader
		, expr::ExprPtr expr
		, bool enabled )
		: Mat4x3T< ValueT >{ shader, std::move( expr ) }
		, m_optional{ true }
		, m_enabled{ enabled }
	{
	}

	template< typename ValueT >
	inline MaybeOptional< Mat4x3T< ValueT > >::MaybeOptional( Mat4x3T< ValueT > const & rhs )
		: Mat4x3T< ValueT >{ rhs }
		, m_optional{ false }
		, m_enabled{ true }
	{
	}

	template< typename ValueT >
	inline MaybeOptional< Mat4x3T< ValueT > >::MaybeOptional( Optional< Mat4x3T< ValueT > > const & rhs )
		: Mat4x3T< ValueT >{ rhs }
		, m_optional{ true }
		, m_enabled{ rhs.isEnabled() }
	{
	}

	template< typename ValueT >
	MaybeOptional< Mat4x3T< ValueT > >::MaybeOptional( MaybeOptional< Mat4x3T< ValueT > > const & rhs )
		: Mat4x3T< ValueT >{ rhs }
		, m_optional{ rhs.isOptional() }
		, m_enabled{ rhs.isEnabled() }
	{
	}

	template< typename ValueT >
	MaybeOptional< Mat4x3T< ValueT > > & MaybeOptional< Mat4x3T< ValueT > >::operator=( MaybeOptional< Mat4x3T< ValueT > > const & rhs )
	{
		writeAssignOperator< Mat4x3T< ValueT > >( *this, rhs, sdw::makeAssign );
		return *this;
	}

	template< typename ValueT >
	template< typename IndexT >
	MaybeOptional< Vec3T< ValueT > > MaybeOptional< Mat4x3T< ValueT > >::operator[]( IndexT const & offset )const
	{
		return writeBinOperator< Vec3T< ValueT > >( *this, offset, sdw::makeArrayAccess );
	}

	template< typename ValueT >
	inline MaybeOptional< Mat4x3T< ValueT > >::operator Mat4x3T< ValueT >()const
	{
		return *this;
	}

	template< typename ValueT >
	inline MaybeOptional< Mat4x3T< ValueT > >::operator Optional< Mat4x3T< ValueT > >()const
	{
		return Optional< Mat4x3T< ValueT > >{ *this
			, m_enabled };
	}

	template< typename ValueT >
	inline bool MaybeOptional< Mat4x3T< ValueT > >::isOptional()const
	{
		return m_optional;
	}

	template< typename ValueT >
	inline bool MaybeOptional< Mat4x3T< ValueT > >::isEnabled()const
	{
		return m_enabled;
	}
}
