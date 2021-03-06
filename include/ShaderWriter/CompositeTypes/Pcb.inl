/*
See LICENSE file in root folder
*/
namespace sdw
{
	template< typename T >
	inline T Pcb::declMember( std::string const & name )
	{
		auto type = m_info.registerMember< typeEnum< T > >( name );
		auto var = registerMember( m_shader, m_var, name, type );
		m_stmt->add( stmt::makeVariableDecl( var ) );
		return T{ &m_shader
			, makeExpr( m_shader, var ) };
	}

	template< typename T >
	inline Array< T > Pcb::declMember( std::string const & name
		, uint32_t dimension )
	{
		auto type = m_info.registerMember< typeEnum< T > >( name, dimension );
		auto var = registerMember( m_shader, m_var, name, type );
		m_stmt->add( stmt::makeVariableDecl( var ) );
		return Array< T >{ &m_shader
			, makeExpr( m_shader, var ) };
	}

	template< typename T >
	inline Optional< T > Pcb::declMember( std::string const & name
		, bool enabled )
	{
		auto type = m_info.registerMember< typeEnum< T > >( name );
		auto var = registerMember( m_shader, m_var, name, type );

		if ( enabled )
		{
			m_stmt->add( stmt::makeVariableDecl( var ) );
		}

		return Optional< T >{ &m_shader
			, makeExpr( m_shader, var )
			, enabled };
	}

	template< typename T >
	inline Optional< Array< T > > Pcb::declMember( std::string const & name
		, uint32_t dimension
		, bool enabled )
	{
		auto type = m_info.registerMember< typeEnum< T > >( name, dimension );
		auto var = registerMember( m_shader, m_var, name, type );

		if ( enabled )
		{
			m_stmt->add( stmt::makeVariableDecl( var ) );
		}

		return Optional< Array< T > >{ &m_shader
			, makeExpr( m_shader, var )
			, enabled };
	}

	template< typename T >
	inline T Pcb::getMember( std::string const & name )
	{
		auto var = getVar( m_shader, name );
		return T{ &m_shader
			, makeExpr( m_shader, var ) };
	}

	template< typename T >
	inline Array< T > Pcb::getMemberArray( std::string const & name )
	{
		auto var = getVar( m_shader, name );
		return Array< T >{ &m_shader
			, makeExpr( m_shader, var ) };
	}

	template< typename T >
	inline Optional< T > Pcb::getMember( std::string const & name
		, bool enabled )
	{
		auto var = getVar( m_shader, name );
		return Optional< T >{ &m_shader
			, makeExpr( m_shader, var )
			, enabled };
	}

	template< typename T >
	inline Optional< Array< T > > Pcb::getMemberArray( std::string const & name
		, bool enabled )
	{
		auto var = getVar( m_shader, name );
		return Optional< Array< T > >{ &m_shader
			, makeExpr( m_shader, var )
			, enabled };
	}
}
