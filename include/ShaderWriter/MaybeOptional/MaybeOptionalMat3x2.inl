/*
See LICENSE file in root folder
*/
#include "MaybeOptionalMat3x2.hpp"

namespace sdw
{
	template< typename ValueT >
	MaybeOptional< Mat3x2T< ValueT > >::MaybeOptional( Shader * shader
		, expr::ExprPtr expr )
		: Mat3x2T< ValueT >{ shader, std::move( expr ) }
		, m_optional{ false }
		, m_enabled{ true }
	{
	}

	template< typename ValueT >
	MaybeOptional< Mat3x2T< ValueT > >::MaybeOptional( Shader * shader
		, expr::ExprPtr expr
		, bool enabled )
		: Mat3x2T< ValueT >{ shader, std::move( expr ) }
		, m_optional{ true }
		, m_enabled{ enabled }
	{
	}

	template< typename ValueT >
	inline MaybeOptional< Mat3x2T< ValueT > >::MaybeOptional( Mat3x2T< ValueT > const & rhs )
		: Mat3x2T< ValueT >{ rhs }
		, m_optional{ false }
		, m_enabled{ true }
	{
	}

	template< typename ValueT >
	inline MaybeOptional< Mat3x2T< ValueT > >::MaybeOptional( Optional< Mat3x2T< ValueT > > const & rhs )
		: Mat3x2T< ValueT >{ rhs }
		, m_optional{ true }
		, m_enabled{ rhs.isEnabled() }
	{
	}

	template< typename ValueT >
	MaybeOptional< Mat3x2T< ValueT > >::MaybeOptional( MaybeOptional< Mat3x2T< ValueT > > const & rhs )
		: Mat3x2T< ValueT >{ rhs }
		, m_optional{ rhs.isOptional() }
		, m_enabled{ rhs.isEnabled() }
	{
	}

	template< typename ValueT >
	MaybeOptional< Mat3x2T< ValueT > > & MaybeOptional< Mat3x2T< ValueT > >::operator=( MaybeOptional< Mat3x2T< ValueT > > const & rhs )
	{
		writeAssignOperator< Mat3x2T< ValueT > >( *this, rhs, sdw::makeAssign );
		return *this;
	}

	template< typename ValueT >
	template< typename IndexT >
	MaybeOptional< Vec2T< ValueT > > MaybeOptional< Mat3x2T< ValueT > >::operator[]( IndexT const & offset )const
	{
		return writeBinOperator< Vec2T< ValueT > >( *this, offset, sdw::makeArrayAccess );
	}

	template< typename ValueT >
	inline MaybeOptional< Mat3x2T< ValueT > >::operator Mat3x2T< ValueT >()const
	{
		return *this;
	}

	template< typename ValueT >
	inline MaybeOptional< Mat3x2T< ValueT > >::operator Optional< Mat3x2T< ValueT > >()const
	{
		return Optional< Mat3x2T< ValueT > >{ *this
			, m_enabled };
	}

	template< typename ValueT >
	inline bool MaybeOptional< Mat3x2T< ValueT > >::isOptional()const
	{
		return m_optional;
	}

	template< typename ValueT >
	inline bool MaybeOptional< Mat3x2T< ValueT > >::isEnabled()const
	{
		return m_enabled;
	}
}
