/*
See LICENSE file in root folder
*/
namespace sdw
{
	//*************************************************************************

	template< typename ValueT >
	inline Vec4T< ValueT >::Vec4T( Shader * shader
		, expr::ExprPtr expr )
		: Value{ shader, std::move( expr ) }
	{
	}

	template< typename ValueT >
	Vec4T< ValueT >::Vec4T( Vec4T const & rhs )
		: Value{ rhs.getShader(), makeExpr( rhs ) }
	{
	}

	template< typename ValueT >
	inline Vec4T< ValueT > & Vec4T< ValueT >::operator=( Vec4T< ValueT > const & rhs )
	{
		if ( getContainer() )
		{
			addStmt( *findShader( *this, rhs )
				, sdw::makeSimple( sdw::makeAssign( Vec4T< ValueT >::makeType( findTypesCache( *this, rhs ) )
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
	inline ValueT Vec4T< ValueT >::operator[]( IndexT const & rhs )const
	{
		return ValueT{ findShader( *this, rhs )
			, sdw::makeArrayAccess( ValueT::makeType( findTypesCache( *this, rhs ) )
				, makeExpr( *this )
				, makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	inline ValueT Vec4T< ValueT >::operator[]( int32_t offset )const
	{
		return ValueT{ findShader( *this, offset )
			, sdw::makeArrayAccess( ValueT::makeType( findTypesCache( *this ) )
				, makeExpr( *this )
				, makeExpr( offset ) ) };
	}

	template< typename ValueT >
	inline ValueT Vec4T< ValueT >::operator[]( uint32_t offset )const
	{
		return ValueT{ findShader( *this, offset )
			, sdw::makeArrayAccess( ValueT::makeType( findTypesCache( *this ) )
				, makeExpr( *this )
				, makeExpr( offset ) ) };
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator+=( Vec4T< ValueT > const & rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeAddAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator-=( Vec4T< ValueT > const & rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeMinusAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator*=( Vec4T< ValueT > const & rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeTimesAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator/=( Vec4T< ValueT > const & rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeDivideAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator+=( Optional< Vec4T< ValueT > > const & rhs )
	{
		if ( rhs.isEnabled() )
		{
			addStmt( *findShader( *this, rhs )
				, sdw::makeSimple( sdw::makeAddAssign( getType()
				, makeExpr( *this )
				, makeExpr( rhs ) ) ) );
		}
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator-=( Optional< Vec4T< ValueT > > const & rhs )
	{
		if ( rhs.isEnabled() )
		{
			addStmt( *findShader( *this, rhs )
				, sdw::makeSimple( sdw::makeMinusAssign( getType()
				, makeExpr( *this )
				, makeExpr( rhs ) ) ) );
		}
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator*=( Optional< Vec4T< ValueT > > const & rhs )
	{
		if ( rhs.isEnabled() )
		{
			addStmt( *findShader( *this, rhs )
				, sdw::makeSimple( sdw::makeTimesAssign( getType()
				, makeExpr( *this )
				, makeExpr( rhs ) ) ) );
		}
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator/=( Optional< Vec4T< ValueT > > const & rhs )
	{
		if ( rhs.isEnabled() )
		{
			addStmt( *findShader( *this, rhs )
				, sdw::makeSimple( sdw::makeDivideAssign( getType()
				, makeExpr( *this )
				, makeExpr( rhs ) ) ) );
		}
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator+=( ValueT rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeAddAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator-=( ValueT rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeMinusAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator*=( ValueT rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeTimesAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	Vec4T< ValueT > & Vec4T< ValueT >::operator/=( ValueT rhs )
	{
		addStmt( *findShader( *this, rhs )
			, sdw::makeSimple( sdw::makeDivideAssign( getType()
			, makeExpr( *this )
			, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< typename ValueT >
	inline ast::type::TypePtr Vec4T< ValueT >::makeType( ast::type::TypesCache & cache )
	{
		return sdw::makeType< Vec4T< ValueT > >( cache );
	}

	//*************************************************************************

	template< typename ValueT >
	Boolean operator==( Vec4T< ValueT > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Boolean{ findShader( lhs, rhs )
			, sdw::makeEqual( makeExpr( lhs ), makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	Boolean operator!=( Vec4T< ValueT > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Boolean{ findShader( lhs, rhs )
			, sdw::makeNEqual( makeExpr( lhs ), makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	Boolean operator<( Vec4T< ValueT > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Boolean{ findShader( lhs, rhs )
			, sdw::makeLess( makeExpr( lhs ), makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	Boolean operator<=( Vec4T< ValueT > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Boolean{ findShader( lhs, rhs )
			, sdw::makeLEqual( makeExpr( lhs ), makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	Boolean operator>( Vec4T< ValueT > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Boolean{ findShader( lhs, rhs )
			, sdw::makeGreater( makeExpr( lhs ), makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	Boolean operator>=( Vec4T< ValueT > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Boolean{ findShader( lhs, rhs )
			, sdw::makeGEqual( makeExpr( lhs ), makeExpr( rhs ) ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator==( Optional< Vec4T< ValueT > > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator!=( Optional< Vec4T< ValueT > > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeNEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator<( Optional< Vec4T< ValueT > > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeLess( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator<=( Optional< Vec4T< ValueT > > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeLEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator>( Optional< Vec4T< ValueT > > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeGreater( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator>=( Optional< Vec4T< ValueT > > const & lhs, Vec4T< ValueT > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeGEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator==( Vec4T< ValueT > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator!=( Vec4T< ValueT > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeNEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator<( Vec4T< ValueT > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeLess( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator<=( Vec4T< ValueT > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeLEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator>( Vec4T< ValueT > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeGreater( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator>=( Vec4T< ValueT > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeGEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator==( Optional< Vec4T< ValueT > > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator!=( Optional< Vec4T< ValueT > > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeNEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator<( Optional< Vec4T< ValueT > > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeLess( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator<=( Optional< Vec4T< ValueT > > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeLEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator>( Optional< Vec4T< ValueT > > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeGreater( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	template< typename ValueT >
	Optional< Boolean > operator>=( Optional< Vec4T< ValueT > > const & lhs, Optional< Vec4T< ValueT > > const & rhs )
	{
		return Optional< Boolean >{ findShader( lhs, rhs )
			, sdw::makeGEqual( makeExpr( lhs ), makeExpr( rhs ) )
			, areOptionalEnabled( lhs, rhs ) };
	}

	//*************************************************************************
}
