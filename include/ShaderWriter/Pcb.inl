namespace sdw
{
	template< typename T >
	inline T Pcb::declMember( std::string const & name
		, uint32_t location )
	{
		using type = typename TypeOf< T >::Type;
		registerName( m_container, name, TypeTraits< T >::TypeEnum );
		registerUniform( m_container, name, location, TypeTraits< T >::TypeEnum, 1u );

		if ( hasPushConstants( m_container ) )
		{
			m_stream << type().m_type << name << cuT( ";" ) << std::endl;
		}
		else
		{
			m_stream << cuT( "layout( location = " ) << location << cuT( " ) uniform " ) << type().m_type << name << cuT( ";" ) << std::endl;
		}

		m_count++;
		return T( &m_container, name );
	}

	template< typename T >
	inline Array< T > Pcb::declMember( std::string const & name
		, uint32_t dimension
		, uint32_t location )
	{
		using type = typename TypeOf< T >::Type;
		registerName( m_container, name, TypeTraits< T >::TypeEnum );
		registerUniform( m_container, name, location, TypeTraits< T >::TypeEnum, dimension );

		if ( hasPushConstants( m_container ) )
		{
			m_stream << type().m_type << name << cuT( "[" ) << dimension << cuT( "];" ) << std::endl;
		}
		else
		{
			m_stream << cuT( "layout( location = " ) << location << cuT( " ) uniform " ) << type().m_type << name << cuT( "[" ) << dimension << cuT( "];" ) << std::endl;
		}

		m_count++;
		return Array< T >( &m_container, name, dimension );
	}

	template< typename T >
	inline Array< T > Pcb::declMemberArray( std::string const & name
		, uint32_t location )
	{
		using type = typename TypeOf< T >::Type;
		registerName( m_container, name, TypeTraits< T >::TypeEnum );
		registerUniform( m_container, name, location, TypeTraits< T >::TypeEnum, 1u );

		if ( hasPushConstants( m_container ) )
		{
			m_stream << type().m_type << name << cuT( "[];" ) << std::endl;
		}
		else
		{
			m_stream << cuT( "layout( location = " ) << location << cuT( " ) uniform " ) << type().m_type << name << cuT( "[];" ) << std::endl;
		}

		m_count++;
		return Array< T >( &m_container, name, 0xFFFFFFFF );
	}

	template< typename T >
	inline Optional< T > Pcb::declMember( std::string const & name
		, uint32_t location
		, bool enabled )
	{
		using type = typename TypeOf< T >::Type;
		registerName( m_container, name, TypeTraits< T >::TypeEnum );
		registerUniform( m_container, name, location, TypeTraits< T >::TypeEnum, 1u, enabled );

		if ( enabled )
		{
			if ( hasPushConstants( m_container ) )
			{
				m_stream << type().m_type << name << cuT( ";" ) << std::endl;
			}
			else
			{
				m_stream << cuT( "layout( location = " ) << location << cuT( " ) uniform " ) << type().m_type << name << cuT( ";" ) << std::endl;
			}

			m_count++;
		}

		return Optional< T >( &m_container, name, enabled );
	}

	template< typename T >
	inline Optional< Array< T > > Pcb::declMember( std::string const & name
		, uint32_t dimension
		, uint32_t location
		, bool enabled )
	{
		using type = typename TypeOf< T >::Type;
		registerName( m_container, name, TypeTraits< T >::TypeEnum );
		registerUniform( m_container, name, location, TypeTraits< T >::TypeEnum, dimension, enabled );

		if ( enabled )
		{
			if ( hasPushConstants( m_container ) )
			{
				m_stream << type().m_type << name << cuT( "[" ) << dimension << cuT( "]" ) << cuT( ";" ) << std::endl;
			}
			else
			{
				m_stream << cuT( "layout( location = " ) << location << cuT( " ) uniform " ) << type().m_type << name << cuT( "[" ) << dimension << cuT( "]" ) << cuT( ";" ) << std::endl;
			}

			m_count++;
		}

		return Optional< Array< T > >( &m_container, name, dimension, enabled );
	}

	template< typename T >
	inline Optional< Array< T > > Pcb::declMemberArray( std::string const & name
		, uint32_t location
		, bool enabled )
	{
		using type = typename TypeOf< T >::Type;
		registerName( m_container, name, TypeTraits< T >::TypeEnum );
		registerUniform( m_container, name, location, TypeTraits< T >::TypeEnum, 1u, enabled );

		if ( enabled )
		{
			if ( hasPushConstants( m_container ) )
			{
				m_stream << type().m_type << name << cuT( "[]" ) << cuT( ";" ) << std::endl;
			}
			else
			{
				m_stream << cuT( "layout( location = " ) << location << cuT( " ) uniform " ) << type().m_type << name << cuT( "[]" ) << cuT( ";" ) << std::endl;
			}

			m_count++;
		}

		return Optional< Array< T > >( &m_container, name, 0xFFFFFFFF, enabled );
	}

	template< typename T >
	inline T Pcb::getMember( std::string const & name
		, uint32_t location )
	{
		checkNameExists( m_container, name, TypeTraits< T >::TypeEnum );
		return T( &m_container, name );
	}

	template< typename T >
	inline Array< T > Pcb::getMember( std::string const & name
		, uint32_t dimension
		, uint32_t location )
	{
		checkNameExists( m_container, name, TypeTraits< T >::TypeEnum );
		return Array< T >( &m_container, name, dimension );
	}

	template< typename T >
	inline Array< T > Pcb::getMemberArray( std::string const & name
		, uint32_t location )
	{
		checkNameExists( m_container, name, TypeTraits< T >::TypeEnum );
		return Array< T >( &m_container, name, 0xFFFFFFFF );
	}

	template< typename T >
	inline Optional< T > Pcb::getMember( std::string const & name
		, uint32_t location
		, bool enabled )
	{
		checkNameExists( m_container, name, TypeTraits< T >::TypeEnum );
		return Optional< T >( &m_container, name, enabled );
	}

	template< typename T >
	inline Optional< Array< T > > Pcb::getMember( std::string const & name
		, uint32_t dimension
		, uint32_t location
		, bool enabled )
	{
		checkNameExists( m_container, name, TypeTraits< T >::TypeEnum );
		return Optional< Array< T > >( &m_container, name, dimension, enabled );
	}

	template< typename T >
	inline Optional< Array< T > > Pcb::getMemberArray( std::string const & name
		, uint32_t location
		, bool enabled )
	{
		checkNameExists( m_container, name, TypeTraits< T >::TypeEnum );
		return Optional< Array< T > >( &m_container, name, 0xFFFFFFFF, enabled );
	}
}