#if 0
#include "ShaderWriter/PerVertex.hpp"

namespace sdw
{
	gl_PerVertex::gl_PerVertex()
		: Value{ nullptr, "gl_PerVertex " }
	{
	}

	gl_PerVertex::gl_PerVertex( Shader & shader, Source source )
		: Value( &shader, "gl_PerVertex " ) )
	{
		switch ( source )
		{
		case eVertexOutput:
			writer << cuT( "out gl_PerVertex\n" )
				<< cuT( "{\n" )
				<< cuT( "    vec4 gl_Position;\n" )
				<< cuT( "    float gl_PointSize;\n" )
				<< cuT( "    float gl_ClipDistance[];\n" )
				<< cuT( "};\n" );
			break;
		case eTessellationControlInput:
		case eTessellationEvaluationInput:
			writer << cuT( "in gl_PerVertex\n" )
				<< cuT( "{\n" )
				<< cuT( "    vec4 gl_Position;\n" )
				<< cuT( "    float gl_PointSize;\n" )
				<< cuT( "    float gl_ClipDistance[];\n" )
				<< cuT( "} gl_in[gl_MaxPatchVertices];\n" );
			break;
		case eTessellationControlOutput:
			writer << cuT( "out gl_PerVertex\n" )
				<< cuT( "{\n" )
				<< cuT( "    vec4 gl_Position;\n" )
				<< cuT( "    float gl_PointSize;\n" )
				<< cuT( "    float gl_ClipDistance[];\n" )
				<< cuT( "} gl_out[];\n" );
			break;
		case eTessellationEvaluationOutput:
			writer << cuT( "out gl_PerVertex\n" )
				<< cuT( "{\n" )
				<< cuT( "    vec4 gl_Position;\n" )
				<< cuT( "    float gl_PointSize;\n" )
				<< cuT( "    float gl_ClipDistance[];\n" )
				<< cuT( "};\n" );
			break;
		case eGeometryInput:
			writer << cuT( "in gl_PerVertex\n" )
				<< cuT( "{\n" )
				<< cuT( "    vec4 gl_Position;\n" )
				<< cuT( "    float gl_PointSize;\n" )
				<< cuT( "    float gl_ClipDistance[];\n" )
				<< cuT( "} gl_in[];\n" );
			break;
		case eGeometryOutput:
			writer << cuT( "out gl_PerVertex\n" )
				<< cuT( "{\n" )
				<< cuT( "    vec4 gl_Position;\n" )
				<< cuT( "    float gl_PointSize;\n" )
				<< cuT( "    float gl_ClipDistance[];\n" )
				<< cuT( "};\n" );
			break;
		}

	}

	Vec4 gl_PerVertex::gl_Position()const
	{
		return Vec4( m_container, m_name + cuT( "gl_Position" ) );
	}

	Float gl_PerVertex::gl_PointSize()const
	{
		return Float( m_container, m_name + cuT( "gl_PointSize" ) );
	}

	Float gl_PerVertex::gl_ClipDistance()const
	{
		return Array< Float >( m_container, m_name + cuT( "gl_ClipDistance" ), 8 );
	}
}
#endif