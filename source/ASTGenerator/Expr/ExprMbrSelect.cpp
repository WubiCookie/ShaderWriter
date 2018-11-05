/*
See LICENSE file in root folder
*/
#include "ASTGenerator/Expr/ExprMbrSelect.hpp"

#include "ASTGenerator/Expr/ExprVisitor.hpp"

namespace ast::expr
{
	MbrSelect::MbrSelect( type::TypePtr type
		, ExprPtr outer
		, uint32_t memberIndex
		, IdentifierPtr member )
		: Unary{ std::move( type )
			, std::move( member )
			, Kind::eMbrSelect }
		, m_outer{ std::move( outer ) }
		, m_memberIndex{ memberIndex }
	{
	}

	void MbrSelect::accept( VisitorPtr vis )
	{
		vis->visitMbrSelectExpr( this );
	}
}
