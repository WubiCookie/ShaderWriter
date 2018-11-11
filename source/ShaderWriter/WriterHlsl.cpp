/*
See LICENSE file in root folder
*/
#include "ShaderWriter/WriterHlsl.hpp"

#include "ShaderWriter/HLSL/HlslStmtAdapter.hpp"
#include "ShaderWriter/HLSL/HlslStmtConfigFiller.hpp"
#include "ShaderWriter/HLSL/HlslStmtVisitor.hpp"

namespace sdw
{
	std::string writeHlsl( Shader const & shader, ShaderType type )
	{
		auto config = hlsl::StmtConfigFiller::submit( shader );
		auto dxStatements = hlsl::StmtAdapter::submit( shader, type, config );
		return hlsl::StmtVisitor::submit( dxStatements.get(), type );
	}
}