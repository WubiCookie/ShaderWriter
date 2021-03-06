/*
See LICENSE file in root folder
*/
namespace sdw
{
#if Writer_UseSwizzle
	//*****************************************************************************************

	namespace
	{
		template< typename T, typename U > struct are_equal : public std::false_type
		{
		};

		template< typename T > struct are_equal< T, T > : public std::true_type
		{
		};

		template< typename TInput, typename TOutput, typename UInput, typename UOutput, typename Enable = void >
		struct SwizzleOperator
		{
		};

		template< typename TInput, typename TOutput, typename UInput, typename UOutput >
		struct SwizzleOperator < TInput, TOutput, UInput, UOutput, typename std::enable_if < are_equal< TOutput, UOutput >::value && !are_equal< TOutput, Float >::value && !are_equal< TOutput, Int >::value >::type >
		{
			Swizzle< TInput, TOutput > & operator()( std::string const & p_operator, Swizzle< TInput, TOutput > & p_lhs, Swizzle< UInput, TOutput > const & rhs )
			{
				p_lhs.updateContainer( *p_lhs.m_input );
				*p_lhs.m_container << p_lhs.m_value.str() << p_operator << std::string( static_cast< TOutput >( rhs ) ) << cuT( ";" ) << sdw::endl;
				return p_lhs;
			}
		};

		template< typename TInput, typename TOutput, typename UInput, typename UOutput >
		struct SwizzleOperator< TInput, TOutput, UInput, UOutput, typename std::enable_if< are_equal< TOutput, Float >::value >::type >
		{
			Swizzle< TInput, TOutput > & operator()( std::string const & p_operator, Swizzle< TInput, TOutput > & p_lhs, Swizzle< UInput, Float > const & rhs )
			{
				p_lhs.updateContainer( *p_lhs.m_input );
				*p_lhs.m_container << p_lhs.m_value.str() << p_operator << std::string( static_cast< Float >( rhs ) ) << cuT( ";" ) << sdw::endl;
				return p_lhs;
			}
		};

		template< typename TInput, typename TOutput, typename UInput, typename UOutput >
		struct SwizzleOperator< TInput, TOutput, UInput, UOutput, typename std::enable_if< are_equal< TOutput, Int >::value >::type >
		{
			Swizzle< TInput, TOutput > & operator()( std::string const & p_operator, Swizzle< TInput, TOutput > & p_lhs, Swizzle< UInput, Int > const & rhs )
			{
				p_lhs.updateContainer( *p_lhs.m_input );
				*p_lhs.m_container << p_lhs.m_value.str() << p_operator << std::string( static_cast< Int >( rhs ) ) << cuT( ";" ) << sdw::endl;
				return p_lhs;
			}
		};

		template< typename Input, typename Output, typename T >
		struct SwizzleOperator< Input, Output, T, T >
		{
			Swizzle< Input, Output > & operator()( std::string const & p_operator, Swizzle< Input, Output > & p_lhs, T rhs )
			{
				p_lhs.updateContainer( *p_lhs.m_input );
				*p_lhs.m_container << p_lhs.m_value.str() << p_operator << castor::string::toString( rhs, Expr::getLocale() ) << cuT( ";" ) << sdw::endl;
				return p_lhs;
			}
		};
	}

	//*****************************************************************************************

	template< typename Input, typename Output >
	Swizzle< Input, Output >::Swizzle( std::string const & p_name, Input * p_input )
		: Expr( p_input->m_container, p_name )
		, m_input( p_input )
	{
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator=( Swizzle const & rhs )
	{
		if ( m_container )
		{
			m_container->writeAssign( *this, rhs );
		}
		else
		{
			Expr::operator=( rhs );
			m_container = rhs.m_container;
		}

		return *this;
	}

	template< typename Input, typename Output >
	template< typename T >
	inline Swizzle< Input, Output > & Swizzle< Input, Output >::operator=( T const & rhs )
	{
		updateContainer( rhs );
		m_container->writeAssign( *this, rhs );
		return *this;
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output >::operator Output()const
	{
		std::string result = m_input->m_value.str();

		if ( result.empty() )
		{
			if ( m_input->m_name.empty() )
			{
				result = m_input->m_type;
			}
			else
			{
				result = m_input->m_name;
			}
		}

		Output output( m_input->m_container, result );
		output.m_value << output.m_name << "." << m_value.rdbuf();
		return output;
	}

	template< typename Input, typename Output >
	template< typename UInput, typename UOutput >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator+=( Swizzle< UInput, UOutput > const & p_type )
	{
		return SwizzleOperator< Input, Output, UInput, UOutput >()( cuT( " += " ), *this, p_type );
	}

	template< typename Input, typename Output >
	template< typename UInput, typename UOutput >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator-=( Swizzle< UInput, UOutput > const & p_type )
	{
		return SwizzleOperator< Input, Output, UInput, UOutput >()( cuT( " -= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	template< typename UInput, typename UOutput >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator*=( Swizzle< UInput, UOutput > const & p_type )
	{
		return SwizzleOperator< Input, Output, UInput, UOutput >()( cuT( " *= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	template< typename UInput, typename UOutput >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator/=( Swizzle< UInput, UOutput > const & p_type )
	{
		return SwizzleOperator< Input, Output, UInput, UOutput >()( cuT( " /= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator+=( float p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " += " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator-=( float p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " -= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator*=( float p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " *= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator/=( float p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " /= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator+=( int p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " += " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator-=( int p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " -= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator*=( int p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " *= " ), *this, p_type );
	}

	template< typename Input, typename Output >
	Swizzle< Input, Output > & Swizzle< Input, Output >::operator/=( int p_type )
	{
		return SwizzleOperator< Input, Output, float, float >()( cuT( " /= " ), *this, p_type );
	}

	//*****************************************************************************************

	template< typename TInput, typename UInput, typename Output >
	Output operator+( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Output > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " + " ) << std::string( static_cast< Output >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator-( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Output > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " - " ) << std::string( static_cast< Output >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator*( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Output > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " * " ) << std::string( static_cast< Output >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator/( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Output > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " / " ) << std::string( static_cast< Output >( p_b ) );
		return result;
	}
	template< typename TInput, typename UInput, typename Output >
	Output operator+( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Float > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " + " ) << std::string( static_cast< Float >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator-( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Float > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " - " ) << std::string( static_cast< Float >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator*( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Float > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " * " ) << std::string( static_cast< Float >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator/( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Float > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " / " ) << std::string( static_cast< Float >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator+( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Int > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " + " ) << std::string( static_cast< Int >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator-( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Int > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " - " ) << std::string( static_cast< Int >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator*( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Int > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " * " ) << std::string( static_cast< Int >( p_b ) );
		return result;
	}

	template< typename TInput, typename UInput, typename Output >
	Output operator/( Swizzle< TInput, Output > const & p_a, Swizzle< UInput, Int > const & p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " / " ) << std::string( static_cast< Int >( p_b ) );
		return result;
	}

	template< typename Input, typename Output >
	Output operator+( Swizzle< Input, Output > const & p_a, float p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " + " ) << castor::string::toString( p_b, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator-( Swizzle< Input, Output > const & p_a, float p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " - " ) << castor::string::toString( p_b, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator*( Swizzle< Input, Output > const & p_a, float p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " * " ) << castor::string::toString( p_b, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator/( Swizzle< Input, Output > const & p_a, float p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " / " ) << castor::string::toString( p_b, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator+( Swizzle< Input, Output > const & p_a, int p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " + " ) << castor::string::toString( p_b, 10, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator-( Swizzle< Input, Output > const & p_a, int p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " - " ) << castor::string::toString( p_b, 10, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator*( Swizzle< Input, Output > const & p_a, int p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " * " ) << castor::string::toString( p_b, 10, Expr::getLocale() );
		return result;
	}

	template< typename Input, typename Output >
	Output operator/( Swizzle< Input, Output > const & p_a, int p_b )
	{
		Output result( p_a.m_input->m_container, p_a.m_input->m_type );
		result.m_value << std::string( static_cast< Output >( p_a ) ) << cuT( " / " ) << castor::string::toString( p_b, 10, Expr::getLocale() );
		return result;
	}

	//*****************************************************************************************
#endif
}
