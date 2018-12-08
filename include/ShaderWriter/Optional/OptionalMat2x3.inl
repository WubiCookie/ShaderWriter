/*
See LICENSE file in root folder
*/
namespace sdw
{
	template< typename ValueT >
	Optional< Mat2x3T< ValueT > >::Optional( Shader * shader
		, expr::ExprPtr expr
		, bool enabled )
		: Mat2x3T< ValueT >{ shader, std::move( expr ) }
		, m_enabled{ enabled }
	{
	}

	template< typename ValueT >
	Optional< Mat2x3T< ValueT > >::Optional( Mat2x3T< ValueT > const & rhs
		, bool enabled )
		: Mat2x3T< ValueT >{ rhs }
		, m_enabled{ enabled }
	{
	}

	template< typename ValueT >
	Optional< Mat2x3T< ValueT > >::Optional( Optional< Mat2x3T< ValueT > > const & rhs )
		: Mat2x3T< ValueT >{ rhs }
		, m_enabled{ rhs.m_enabled }
	{
	}

	template< typename ValueT >
	Optional< Mat2x3T< ValueT > > & Optional< Mat2x3T< ValueT > >::operator=( Optional< Mat2x3T< ValueT > > const & rhs )
	{
		if ( this->getContainer() )
		{
			addStmt( *findShader( *this, rhs )
				, sdw::makeSimple( sdw::makeAssign( Mat2x3T< ValueT >::makeType( findTypesCache( *this, rhs ) )
					, makeExpr( *this )
					, makeExpr( rhs ) ) ) );
		}
		else
		{
			Value::operator=( rhs );
		}

		return *this;
	}

	template< typename ValueT >
	template< typename IndexT >
	Optional< Vec3T< ValueT > > Optional< Mat2x3T< ValueT > >::operator[]( IndexT const & rhs )const
	{
		return Optional< Vec3T< ValueT > >{ findShader( *this, rhs )
			, sdw::makeArrayAccess( Vec3T< ValueT >::makeType( findTypesCache( *this, rhs ) )
				, makeExpr( *this )
				, makeExpr( rhs ) )
			, areOptionalEnabled( *this, rhs ) };
	}

	template< typename ValueT >
	inline bool Optional< Mat2x3T< ValueT > >::isEnabled()const
	{
		return m_enabled;
	}
}
