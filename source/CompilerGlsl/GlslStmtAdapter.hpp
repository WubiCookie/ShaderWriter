/*
See LICENSE file in root folder
*/
#ifndef ___SDW_GlslStmtAdapter_H___
#define ___SDW_GlslStmtAdapter_H___
#pragma once

#include "CompilerGlsl/compileGlsl.hpp"
#include "GlslHelpers.hpp"

#include <ShaderAST/Visitors/CloneStmt.hpp>

namespace glsl
{
	class StmtAdapter
		: public ast::StmtCloner
	{
	public:
		static ast::stmt::ContainerPtr submit( sdw::Shader const & shader
			, GlslConfig const & writerConfig
			, IntrinsicsConfig const & intrinsicsConfig );

	private:
		StmtAdapter( sdw::Shader const & shader
			, GlslConfig const & writerConfig
			, IntrinsicsConfig const & intrinsicsConfig
			, ast::stmt::ContainerPtr & result );

		ast::expr::ExprPtr doSubmit( ast::expr::Expr * expr )override;

		void visitConstantBufferDeclStmt( ast::stmt::ConstantBufferDecl * stmt )override;
		void visitImageDeclStmt( ast::stmt::ImageDecl * stmt )override;
		void visitPushConstantsBufferDeclStmt( ast::stmt::PushConstantsBufferDecl * stmt )override;
		void visitSampledImageDeclStmt( ast::stmt::SampledImageDecl * stmt )override;
		void visitShaderBufferDeclStmt( ast::stmt::ShaderBufferDecl * stmt )override;
		void visitShaderStructBufferDeclStmt( ast::stmt::ShaderStructBufferDecl * stmt )override;
		void visitPreprocVersion( ast::stmt::PreprocVersion * preproc )override;

	private:
		GlslConfig const & m_writerConfig;
		IntrinsicsConfig const & m_intrinsicsConfig;
		ast::stmt::Container * m_intrinsics;
		sdw::ShaderType m_type;
	};
}

#endif
