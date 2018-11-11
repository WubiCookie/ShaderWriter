/*
See LICENSE file in root folder
*/
#include "ShaderWriter/HLSL/HlslExprAdapter.hpp"

#include "ShaderWriter/HLSL/HlslHelpers.hpp"
#include "ShaderWriter/HLSL/HlslVariableReplacer.hpp"
#include "ShaderWriter/HLSL/HlslImageAccessConfig.hpp"
#include "ShaderWriter/HLSL/HlslIntrinsicConfig.hpp"
#include "ShaderWriter/HLSL/HlslTextureAccessConfig.hpp"

#include "ShaderWriter/Intrinsics.hpp"

namespace sdw::hlsl
{
	namespace
	{
		static std::map< std::string, std::string > const TypeCtorNames
		{
			{ "bool", "bool" },
			{ "bvec2", "bool2" },
			{ "bvec3", "bool3" },
			{ "bvec4", "bool4" },
			{ "int", "int" },
			{ "ivec2", "int2" },
			{ "ivec3", "int3" },
			{ "ivec4", "int4" },
			{ "uint", "uint" },
			{ "uvec2", "uint2" },
			{ "uvec3", "uint3" },
			{ "uvec4", "uint4" },
			{ "float", "float" },
			{ "vec2", "float2" },
			{ "vec3", "float3" },
			{ "vec4", "float4" },
			{ "mat2", "float2x2" },
			{ "mat2x3", "float2x3" },
			{ "mat2x4", "float2x4" },
			{ "mat3x2", "float3x2" },
			{ "mat3", "float3x3" },
			{ "mat3x4", "float3x4" },
			{ "mat4x2", "float4x2" },
			{ "mat4x3", "float4x3" },
			{ "mat4", "float4x4" },
			{ "double", "double" },
			{ "dvec2", "double2" },
			{ "dvec3", "double3" },
			{ "dvec4", "double4" },
		};

		var::VariablePtr adaptName( var::VariablePtr var )
		{
			auto it = TypeCtorNames.find( var->getName() );

			if ( it != TypeCtorNames.end() )
			{
				return var::makeVariable( var->getType(), it->second );
			}

			return var;
		}

		expr::IdentifierPtr adaptName( expr::Identifier const & ident )
		{
			return makeIdent( adaptName( ident.getVariable() ) );
		}

		bool isMatrix( type::Kind kind )
		{
			return kind >= type::Kind::eMat2x2F
				&& kind <= type::Kind::eMat4x4D;
		}

		expr::ExprPtr writeProjectTexCoords2( expr::ExprPtr texcoords )
		{
			expr::ExprList args;
			args.emplace_back( std::move( texcoords ) );
			return expr::makeFnCall( type::getFloat()
				, expr::makeIdentifier( var::makeFunction( "SDW_projectTexCoords2" ) )
				, std::move( args ) );
		}

		expr::ExprPtr writeProjectTexCoords3( expr::ExprPtr texcoords )
		{
			expr::ExprList args;
			args.emplace_back( std::move( texcoords ) );
			return expr::makeFnCall( type::getVec2F()
				, expr::makeIdentifier( var::makeFunction( "SDW_projectTexCoords3" ) )
				, std::move( args ) );
		}

		expr::ExprPtr writeProjectTexCoords4To1( expr::ExprPtr texcoords )
		{
			expr::ExprList args;
			args.emplace_back( std::move( texcoords ) );
			return expr::makeFnCall( type::getFloat()
				, expr::makeIdentifier( var::makeFunction( "SDW_projectTexCoords4To1" ) )
				, std::move( args ) );
		}

		expr::ExprPtr writeProjectTexCoords4To2( expr::ExprPtr texcoords )
		{
			expr::ExprList args;
			args.emplace_back( std::move( texcoords ) );
			return expr::makeFnCall( type::getVec2F()
				, expr::makeIdentifier( var::makeFunction( "SDW_projectTexCoords4To2" ) )
				, std::move( args ) );
		}

		expr::ExprPtr writeProjectTexCoords4( expr::ExprPtr texcoords )
		{
			expr::ExprList args;
			args.emplace_back( std::move( texcoords ) );
			return expr::makeFnCall( type::getVec3F()
				, expr::makeIdentifier( var::makeFunction( "SDW_projectTexCoords4" ) )
				, std::move( args ) );
		}

		expr::ExprPtr writeProjTexCoords( expr::TextureAccess access
			, expr::ExprPtr texcoords )
		{
			switch ( access )
			{
			case ast::expr::TextureAccess::eTextureProj1DF2:
			case ast::expr::TextureAccess::eTextureProj1DShadowF:
			case ast::expr::TextureAccess::eTextureProj1DI2:
			case ast::expr::TextureAccess::eTextureProj1DU2:
			case ast::expr::TextureAccess::eTextureProj1DF2Bias:
			case ast::expr::TextureAccess::eTextureProj1DShadowFBias:
			case ast::expr::TextureAccess::eTextureProj1DI2Bias:
			case ast::expr::TextureAccess::eTextureProj1DU2Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DF2:
			case ast::expr::TextureAccess::eTextureProjOffset1DShadowF:
			case ast::expr::TextureAccess::eTextureProjOffset1DI2:
			case ast::expr::TextureAccess::eTextureProjOffset1DU2:
			case ast::expr::TextureAccess::eTextureProjOffset1DF2Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DShadowFBias:
			case ast::expr::TextureAccess::eTextureProjOffset1DI2Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DU2Bias:
			case ast::expr::TextureAccess::eTextureProjLod1DF2:
			case ast::expr::TextureAccess::eTextureProjLod1DShadowF:
			case ast::expr::TextureAccess::eTextureProjLod1DI2:
			case ast::expr::TextureAccess::eTextureProjLod1DU2:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DF2:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DShadowF:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DI2:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DU2:
			case ast::expr::TextureAccess::eTextureProjGrad1DF2:
			case ast::expr::TextureAccess::eTextureProjGrad1DShadowF:
			case ast::expr::TextureAccess::eTextureProjGrad1DI2:
			case ast::expr::TextureAccess::eTextureProjGrad1DU2:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DF2:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DShadowF:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DI2:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DU2:
				return writeProjectTexCoords2( std::move( texcoords ) );

			case ast::expr::TextureAccess::eTextureProj1DF4:
			case ast::expr::TextureAccess::eTextureProj1DI4:
			case ast::expr::TextureAccess::eTextureProj1DU4:
			case ast::expr::TextureAccess::eTextureProj1DF4Bias:
			case ast::expr::TextureAccess::eTextureProj1DI4Bias:
			case ast::expr::TextureAccess::eTextureProj1DU4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DF4:
			case ast::expr::TextureAccess::eTextureProjOffset1DI4:
			case ast::expr::TextureAccess::eTextureProjOffset1DU4:
			case ast::expr::TextureAccess::eTextureProjOffset1DF4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DI4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DU4Bias:
			case ast::expr::TextureAccess::eTextureProjLod1DF4:
			case ast::expr::TextureAccess::eTextureProjLod1DI4:
			case ast::expr::TextureAccess::eTextureProjLod1DU4:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DF4:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DI4:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DU4:
			case ast::expr::TextureAccess::eTextureProjGrad1DF4:
			case ast::expr::TextureAccess::eTextureProjGrad1DI4:
			case ast::expr::TextureAccess::eTextureProjGrad1DU4:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DF4:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DI4:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DU4:
				return writeProjectTexCoords4To1( std::move( texcoords ) );

			case ast::expr::TextureAccess::eTextureProj2DF3:
			case ast::expr::TextureAccess::eTextureProj2DShadowF:
			case ast::expr::TextureAccess::eTextureProj2DRectF3:
			case ast::expr::TextureAccess::eTextureProj2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProj2DI3:
			case ast::expr::TextureAccess::eTextureProj2DRectI3:
			case ast::expr::TextureAccess::eTextureProj2DU3:
			case ast::expr::TextureAccess::eTextureProj2DRectU3:
			case ast::expr::TextureAccess::eTextureProj2DF3Bias:
			case ast::expr::TextureAccess::eTextureProj2DShadowFBias:
			case ast::expr::TextureAccess::eTextureProj2DI3Bias:
			case ast::expr::TextureAccess::eTextureProj2DU3Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DF3:
			case ast::expr::TextureAccess::eTextureProjOffset2DShadowF:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectF3:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProjOffset2DI3:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectI3:
			case ast::expr::TextureAccess::eTextureProjOffset2DU3:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectU3:
			case ast::expr::TextureAccess::eTextureProjOffset2DF3Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DShadowFBias:
			case ast::expr::TextureAccess::eTextureProjOffset2DI3Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DU3Bias:
			case ast::expr::TextureAccess::eTextureProjLod2DF3:
			case ast::expr::TextureAccess::eTextureProjLod2DShadowF:
			case ast::expr::TextureAccess::eTextureProjLod2DI3:
			case ast::expr::TextureAccess::eTextureProjLod2DU3:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DF3:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DShadowF:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DI3:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DU3:
			case ast::expr::TextureAccess::eTextureProjGrad2DF3:
			case ast::expr::TextureAccess::eTextureProjGrad2DShadowF:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectF3:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProjGrad2DI3:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectI3:
			case ast::expr::TextureAccess::eTextureProjGrad2DU3:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectU3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DF3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DShadowF:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectF3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DI3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectI3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DU3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectU3:
				return writeProjectTexCoords3( std::move( texcoords ) );

			case ast::expr::TextureAccess::eTextureProj2DF4:
			case ast::expr::TextureAccess::eTextureProj2DRectF4:
			case ast::expr::TextureAccess::eTextureProj2DI4:
			case ast::expr::TextureAccess::eTextureProj2DRectI4:
			case ast::expr::TextureAccess::eTextureProj2DU4:
			case ast::expr::TextureAccess::eTextureProj2DRectU4:
			case ast::expr::TextureAccess::eTextureProj2DF4Bias:
			case ast::expr::TextureAccess::eTextureProj2DI4Bias:
			case ast::expr::TextureAccess::eTextureProj2DU4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DF4:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectF4:
			case ast::expr::TextureAccess::eTextureProjOffset2DI4:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectI4:
			case ast::expr::TextureAccess::eTextureProjOffset2DU4:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectU4:
			case ast::expr::TextureAccess::eTextureProjOffset2DF4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DI4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DU4Bias:
			case ast::expr::TextureAccess::eTextureProjLod2DF4:
			case ast::expr::TextureAccess::eTextureProjLod2DI4:
			case ast::expr::TextureAccess::eTextureProjLod2DU4:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DF4:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DI4:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DU4:
			case ast::expr::TextureAccess::eTextureProjGrad2DF4:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectF4:
			case ast::expr::TextureAccess::eTextureProjGrad2DI4:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectI4:
			case ast::expr::TextureAccess::eTextureProjGrad2DU4:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectU4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DF4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectF4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DI4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectI4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DU4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectU4:
				return writeProjectTexCoords4To2( std::move( texcoords ) );

			case ast::expr::TextureAccess::eTextureProj3DF:
			case ast::expr::TextureAccess::eTextureProj3DI:
			case ast::expr::TextureAccess::eTextureProj3DU:
			case ast::expr::TextureAccess::eTextureProj3DFBias:
			case ast::expr::TextureAccess::eTextureProj3DIBias:
			case ast::expr::TextureAccess::eTextureProj3DUBias:
			case ast::expr::TextureAccess::eTextureProjOffset3DF:
			case ast::expr::TextureAccess::eTextureProjOffset3DI:
			case ast::expr::TextureAccess::eTextureProjOffset3DU:
			case ast::expr::TextureAccess::eTextureProjOffset3DFBias:
			case ast::expr::TextureAccess::eTextureProjOffset3DIBias:
			case ast::expr::TextureAccess::eTextureProjOffset3DUBias:
			case ast::expr::TextureAccess::eTextureProjLod3DF:
			case ast::expr::TextureAccess::eTextureProjLod3DI:
			case ast::expr::TextureAccess::eTextureProjLod3DU:
			case ast::expr::TextureAccess::eTextureProjLodOffset3DF:
			case ast::expr::TextureAccess::eTextureProjLodOffset3DI:
			case ast::expr::TextureAccess::eTextureProjLodOffset3DU:
			case ast::expr::TextureAccess::eTextureProjGrad3DF:
			case ast::expr::TextureAccess::eTextureProjGrad3DI:
			case ast::expr::TextureAccess::eTextureProjGrad3DU:
			case ast::expr::TextureAccess::eTextureProjGradOffset3DF:
			case ast::expr::TextureAccess::eTextureProjGradOffset3DI:
			case ast::expr::TextureAccess::eTextureProjGradOffset3DU:
				return writeProjectTexCoords4( std::move( texcoords ) );

			default:
				assert( false && "Expected a textureProj access function" );
				return nullptr;
			}
		}

		bool requiresProjTexCoords( expr::TextureAccess access )
		{
			bool result = false;

			switch ( access )
			{
			case ast::expr::TextureAccess::eTextureProj1DF2:
			case ast::expr::TextureAccess::eTextureProj1DShadowF:
			case ast::expr::TextureAccess::eTextureProj1DI2:
			case ast::expr::TextureAccess::eTextureProj1DU2:
			case ast::expr::TextureAccess::eTextureProj1DF2Bias:
			case ast::expr::TextureAccess::eTextureProj1DShadowFBias:
			case ast::expr::TextureAccess::eTextureProj1DI2Bias:
			case ast::expr::TextureAccess::eTextureProj1DU2Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DF2:
			case ast::expr::TextureAccess::eTextureProjOffset1DShadowF:
			case ast::expr::TextureAccess::eTextureProjOffset1DI2:
			case ast::expr::TextureAccess::eTextureProjOffset1DU2:
			case ast::expr::TextureAccess::eTextureProjOffset1DF2Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DShadowFBias:
			case ast::expr::TextureAccess::eTextureProjOffset1DI2Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DU2Bias:
			case ast::expr::TextureAccess::eTextureProjLod1DF2:
			case ast::expr::TextureAccess::eTextureProjLod1DShadowF:
			case ast::expr::TextureAccess::eTextureProjLod1DI2:
			case ast::expr::TextureAccess::eTextureProjLod1DU2:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DF2:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DShadowF:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DI2:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DU2:
			case ast::expr::TextureAccess::eTextureProjGrad1DF2:
			case ast::expr::TextureAccess::eTextureProjGrad1DShadowF:
			case ast::expr::TextureAccess::eTextureProjGrad1DI2:
			case ast::expr::TextureAccess::eTextureProjGrad1DU2:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DF2:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DShadowF:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DI2:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DU2:
			case ast::expr::TextureAccess::eTextureProj1DF4:
			case ast::expr::TextureAccess::eTextureProj1DI4:
			case ast::expr::TextureAccess::eTextureProj1DU4:
			case ast::expr::TextureAccess::eTextureProj1DF4Bias:
			case ast::expr::TextureAccess::eTextureProj1DI4Bias:
			case ast::expr::TextureAccess::eTextureProj1DU4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DF4:
			case ast::expr::TextureAccess::eTextureProjOffset1DI4:
			case ast::expr::TextureAccess::eTextureProjOffset1DU4:
			case ast::expr::TextureAccess::eTextureProjOffset1DF4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DI4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset1DU4Bias:
			case ast::expr::TextureAccess::eTextureProjLod1DF4:
			case ast::expr::TextureAccess::eTextureProjLod1DI4:
			case ast::expr::TextureAccess::eTextureProjLod1DU4:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DF4:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DI4:
			case ast::expr::TextureAccess::eTextureProjLodOffset1DU4:
			case ast::expr::TextureAccess::eTextureProjGrad1DF4:
			case ast::expr::TextureAccess::eTextureProjGrad1DI4:
			case ast::expr::TextureAccess::eTextureProjGrad1DU4:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DF4:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DI4:
			case ast::expr::TextureAccess::eTextureProjGradOffset1DU4:
			case ast::expr::TextureAccess::eTextureProj2DF3:
			case ast::expr::TextureAccess::eTextureProj2DShadowF:
			case ast::expr::TextureAccess::eTextureProj2DRectF3:
			case ast::expr::TextureAccess::eTextureProj2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProj2DI3:
			case ast::expr::TextureAccess::eTextureProj2DRectI3:
			case ast::expr::TextureAccess::eTextureProj2DU3:
			case ast::expr::TextureAccess::eTextureProj2DRectU3:
			case ast::expr::TextureAccess::eTextureProj2DF3Bias:
			case ast::expr::TextureAccess::eTextureProj2DShadowFBias:
			case ast::expr::TextureAccess::eTextureProj2DI3Bias:
			case ast::expr::TextureAccess::eTextureProj2DU3Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DF3:
			case ast::expr::TextureAccess::eTextureProjOffset2DShadowF:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectF3:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProjOffset2DI3:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectI3:
			case ast::expr::TextureAccess::eTextureProjOffset2DU3:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectU3:
			case ast::expr::TextureAccess::eTextureProjOffset2DF3Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DShadowFBias:
			case ast::expr::TextureAccess::eTextureProjOffset2DI3Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DU3Bias:
			case ast::expr::TextureAccess::eTextureProjLod2DF3:
			case ast::expr::TextureAccess::eTextureProjLod2DShadowF:
			case ast::expr::TextureAccess::eTextureProjLod2DI3:
			case ast::expr::TextureAccess::eTextureProjLod2DU3:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DF3:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DShadowF:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DI3:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DU3:
			case ast::expr::TextureAccess::eTextureProjGrad2DF3:
			case ast::expr::TextureAccess::eTextureProjGrad2DShadowF:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectF3:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProjGrad2DI3:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectI3:
			case ast::expr::TextureAccess::eTextureProjGrad2DU3:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectU3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DF3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DShadowF:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectF3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectShadowF:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DI3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectI3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DU3:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectU3:
			case ast::expr::TextureAccess::eTextureProj2DF4:
			case ast::expr::TextureAccess::eTextureProj2DRectF4:
			case ast::expr::TextureAccess::eTextureProj2DI4:
			case ast::expr::TextureAccess::eTextureProj2DRectI4:
			case ast::expr::TextureAccess::eTextureProj2DU4:
			case ast::expr::TextureAccess::eTextureProj2DRectU4:
			case ast::expr::TextureAccess::eTextureProj2DF4Bias:
			case ast::expr::TextureAccess::eTextureProj2DI4Bias:
			case ast::expr::TextureAccess::eTextureProj2DU4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DF4:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectF4:
			case ast::expr::TextureAccess::eTextureProjOffset2DI4:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectI4:
			case ast::expr::TextureAccess::eTextureProjOffset2DU4:
			case ast::expr::TextureAccess::eTextureProjOffset2DRectU4:
			case ast::expr::TextureAccess::eTextureProjOffset2DF4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DI4Bias:
			case ast::expr::TextureAccess::eTextureProjOffset2DU4Bias:
			case ast::expr::TextureAccess::eTextureProjLod2DF4:
			case ast::expr::TextureAccess::eTextureProjLod2DI4:
			case ast::expr::TextureAccess::eTextureProjLod2DU4:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DF4:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DI4:
			case ast::expr::TextureAccess::eTextureProjLodOffset2DU4:
			case ast::expr::TextureAccess::eTextureProjGrad2DF4:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectF4:
			case ast::expr::TextureAccess::eTextureProjGrad2DI4:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectI4:
			case ast::expr::TextureAccess::eTextureProjGrad2DU4:
			case ast::expr::TextureAccess::eTextureProjGrad2DRectU4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DF4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectF4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DI4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectI4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DU4:
			case ast::expr::TextureAccess::eTextureProjGradOffset2DRectU4:
			case ast::expr::TextureAccess::eTextureProj3DF:
			case ast::expr::TextureAccess::eTextureProj3DI:
			case ast::expr::TextureAccess::eTextureProj3DU:
			case ast::expr::TextureAccess::eTextureProj3DFBias:
			case ast::expr::TextureAccess::eTextureProj3DIBias:
			case ast::expr::TextureAccess::eTextureProj3DUBias:
			case ast::expr::TextureAccess::eTextureProjOffset3DF:
			case ast::expr::TextureAccess::eTextureProjOffset3DI:
			case ast::expr::TextureAccess::eTextureProjOffset3DU:
			case ast::expr::TextureAccess::eTextureProjOffset3DFBias:
			case ast::expr::TextureAccess::eTextureProjOffset3DIBias:
			case ast::expr::TextureAccess::eTextureProjOffset3DUBias:
			case ast::expr::TextureAccess::eTextureProjLod3DF:
			case ast::expr::TextureAccess::eTextureProjLod3DI:
			case ast::expr::TextureAccess::eTextureProjLod3DU:
			case ast::expr::TextureAccess::eTextureProjLodOffset3DF:
			case ast::expr::TextureAccess::eTextureProjLodOffset3DI:
			case ast::expr::TextureAccess::eTextureProjLodOffset3DU:
			case ast::expr::TextureAccess::eTextureProjGrad3DF:
			case ast::expr::TextureAccess::eTextureProjGrad3DI:
			case ast::expr::TextureAccess::eTextureProjGrad3DU:
			case ast::expr::TextureAccess::eTextureProjGradOffset3DF:
			case ast::expr::TextureAccess::eTextureProjGradOffset3DI:
			case ast::expr::TextureAccess::eTextureProjGradOffset3DU:
				result = true;
				break;
			}

			return result;
		}

		expr::TextureAccess getSampleCmp( expr::TextureAccess value )
		{
			assert( value >= expr::TextureAccess::eTextureGrad2DRectShadowF
				&& value <= expr::TextureAccess::eTextureProjGradOffset2DRectShadowF );
			expr::TextureAccess result;

			switch ( value )
			{
			case expr::TextureAccess::eTextureGrad2DRectShadowF:
				result = expr::TextureAccess::eTexture2DRectShadowF;
				break;
			case expr::TextureAccess::eTextureGrad1DShadowF:
				result = expr::TextureAccess::eTexture1DShadowF;
				break;
			case expr::TextureAccess::eTextureGrad2DShadowF:
				result = expr::TextureAccess::eTexture2DShadowF;
				break;
			case expr::TextureAccess::eTextureGrad1DArrayShadowF:
				result = expr::TextureAccess::eTexture1DArrayShadowF;
				break;
			case expr::TextureAccess::eTextureGradOffset2DRectShadowF:
				result = expr::TextureAccess::eTextureOffset2DRectShadowF;
				break;
			case expr::TextureAccess::eTextureGradOffset1DShadowF:
				result = expr::TextureAccess::eTextureOffset1DShadowF;
				break;
			case expr::TextureAccess::eTextureGradOffset2DShadowF:
				result = expr::TextureAccess::eTextureOffset2DShadowF;
				break;
			case expr::TextureAccess::eTextureGradOffset1DArrayShadowF:
				result = expr::TextureAccess::eTextureOffset1DArrayShadowF;
				break;
			case expr::TextureAccess::eTextureGradOffset2DArrayShadowF:
				result = expr::TextureAccess::eTextureOffset2DArrayShadowF;
				break;
			case expr::TextureAccess::eTextureProjGrad1DShadowF:
				result = expr::TextureAccess::eTextureProj1DShadowF;
				break;
			case expr::TextureAccess::eTextureProjGrad2DShadowF:
				result = expr::TextureAccess::eTextureProj2DShadowF;
				break;
			case expr::TextureAccess::eTextureProjGrad2DRectShadowF:
				result = expr::TextureAccess::eTextureProj2DRectShadowF;
				break;
			case expr::TextureAccess::eTextureProjGradOffset1DShadowF:
				result = expr::TextureAccess::eTextureProjOffset1DShadowF;
				break;
			case expr::TextureAccess::eTextureProjGradOffset2DShadowF:
				result = expr::TextureAccess::eTextureProjOffset2DShadowF;
				break;
			case expr::TextureAccess::eTextureProjGradOffset2DRectShadowF:
				result = expr::TextureAccess::eTextureProjOffset2DRectShadowF;
				break;
			}

			return result;
		}
	}

	expr::ExprPtr ExprAdapter::submit( expr::Expr * expr
		, IntrinsicsConfig const & config
		, LinkedVars const & linkedVars
		, VariableExprMap const & inputMembers
		, VariableExprMap const & outputMembers )
	{
		expr::ExprPtr result;
		ExprAdapter vis{ result, config, linkedVars, inputMembers, outputMembers };
		expr->accept( &vis );
		return result;
	}
			
	expr::ExprPtr ExprAdapter::submit( expr::ExprPtr const & expr
		, IntrinsicsConfig const & config
		, LinkedVars const & linkedVars
		, VariableExprMap const & inputMembers
		, VariableExprMap const & outputMembers )
	{
		return submit( expr.get()
			, config
			, linkedVars
			, inputMembers
			, outputMembers );
	}

	ExprAdapter::ExprAdapter( expr::ExprPtr & result
		, IntrinsicsConfig const & config
		, LinkedVars const & linkedVars
		, VariableExprMap const & inputMembers
		, VariableExprMap const & outputMembers )
		: ExprCloner{ result }
		, m_config{ config }
		, m_linkedVars{ linkedVars }
		, m_inputMembers{ inputMembers }
		, m_outputMembers{ outputMembers }
	{
	}

	ast::expr::ExprPtr ExprAdapter::doSubmit( ast::expr::Expr * expr )
	{
		expr::ExprPtr result;
		ExprAdapter vis{ result, m_config, m_linkedVars, m_inputMembers, m_outputMembers };
		expr->accept( &vis );
		return result;
	}

	void ExprAdapter::visitIdentifierExpr( expr::Identifier * expr )
	{
		auto itInputs = m_inputMembers.find( expr->getVariable() );
		auto itOutputs = m_outputMembers.find( expr->getVariable() );

		if ( m_inputMembers.end() != itInputs )
		{
			m_result = makeExpr( itInputs->second );
		}
		else if ( m_outputMembers.end() != itOutputs )
		{
			m_result = makeExpr( itOutputs->second );
		}
		else
		{
			m_result = expr::makeIdentifier( expr->getVariable() );
		}
	}

	void ExprAdapter::visitFnCallExpr( expr::FnCall * expr )
	{
		expr::ExprList args;

		for ( auto & arg : expr->getArgList() )
		{
			auto ident = findIdentifier( arg );

			if ( ident )
			{
				auto it = m_linkedVars.find( ident->getVariable() );

				if ( m_linkedVars.end() != it )
				{
					args.emplace_back( VariableReplacer::submit( arg, ident->getVariable(), it->second.first ) );
					args.emplace_back( VariableReplacer::submit( arg, ident->getVariable(), it->second.second ) );
				}
				else
				{
					args.emplace_back( doSubmit( arg.get() ) );
				}
			}
			else
			{
				args.emplace_back( doSubmit( arg.get() ) );
			}
		}

		m_result = expr::makeFnCall( expr->getType()
			, adaptName( *expr->getFn() )
			, std::move( args ) );
	}

	void ExprAdapter::visitImageAccessCallExpr( expr::ImageAccessCall * expr )
	{
		expr::ExprList args;

		for ( auto & arg : expr->getArgList() )
		{
			args.emplace_back( doSubmit( arg.get() ) );
		}

		m_result = expr::makeImageAccessCall( expr->getType()
			, expr->getImageAccess()
			, std::move( args ) );
	}

	void ExprAdapter::visitIntrinsicCallExpr( expr::IntrinsicCall * expr )
	{
		auto instantChange = getInstantChange( expr->getIntrinsic() );

		if ( instantChange.toOperator )
		{
			switch ( instantChange.operatorKind )
			{
			case expr::Kind::eLess:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeLess( doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;
				
			case expr::Kind::eLessEqual:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeLessEqual( doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;

			case expr::Kind::eGreater:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeGreater( doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;

			case expr::Kind::eGreaterEqual:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeGreaterEqual( doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;

			case expr::Kind::eEqual:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeEqual( doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;

			case expr::Kind::eNotEqual:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeNotEqual( doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;

			case expr::Kind::eLogNot:
				assert( expr->getArgList().size() == 1u );
				m_result = expr::makeLogNot( doSubmit( expr->getArgList()[0].get() ) );
				break;

			case expr::Kind::eTimes:
				assert( expr->getArgList().size() == 2u );
				m_result = expr::makeTimes( expr->getArgList()[0].get()->getType()
					, doSubmit( expr->getArgList()[0].get() )
					, doSubmit( expr->getArgList()[1].get() ) );
				break;

			default:
				assert( false && "Unexpected operator type." );
				break;
			}
		}
		else
		{
			expr::ExprList args;

			for ( auto & arg : expr->getArgList() )
			{
				args.emplace_back( doSubmit( arg.get() ) );
			}

			m_result = expr::makeIntrinsicCall( expr->getType()
				, expr->getIntrinsic()
				, std::move( args ) );
		}
	}

	void ExprAdapter::visitTextureAccessCallExpr( expr::TextureAccessCall * expr )
	{
		if ( ( expr->getTextureAccess() >= expr::TextureAccess::eTextureSize1DF
				&& expr->getTextureAccess() <= expr::TextureAccess::eTextureSizeBufferU )
			|| ( expr->getTextureAccess() >= expr::TextureAccess::eTextureQueryLevels1DF
				&& expr->getTextureAccess() <= expr::TextureAccess::eTextureQueryLevelsCubeArrayU ) )
		{
			doProcessTextureQueries( expr );
		}
		else if ( expr->getTextureAccess() >= expr::TextureAccess::eTexelFetch1DF
			&& expr->getTextureAccess() <= expr::TextureAccess::eTexelFetchOffset2DArrayU )
		{
			doProcessTexelFetch( expr );
		}
		else if ( expr->getTextureAccess() >= expr::TextureAccess::eTextureGrad2DRectShadowF
			&& expr->getTextureAccess() <= expr::TextureAccess::eTextureProjGradOffset2DRectShadowF
			&& m_config.requiresShadowSampler )
		{
			doProcessTextureGradShadow( expr );
		}
		else
		{
			expr::ExprList args;

			uint32_t index = 0u;
			uint32_t sampler = 0u;

			for ( auto & arg : expr->getArgList() )
			{
				if ( doProcessSampledImageArg( *arg, true, args ) )
				{
					sampler = index;
				}
				else if ( index == sampler + 1
					&& requiresProjTexCoords( expr->getTextureAccess() ) )
				{
					args.emplace_back( writeProjTexCoords( expr->getTextureAccess(), doSubmit( arg.get() ) ) );
				}
				else
				{
					args.emplace_back( doSubmit( arg.get() ) );
				}

				++index;
			}

			m_result = expr::makeTextureAccessCall( expr->getType()
				, expr->getTextureAccess()
				, std::move( args ) );
		}
	}

	void ExprAdapter::visitTimesExpr( expr::Times * expr )
	{
		if ( isMatrix( expr->getType()->getKind() ) )
		{
			expr::ExprList argsList;
			argsList.emplace_back( doSubmit( expr->getLHS() ) );
			argsList.emplace_back( doSubmit( expr->getRHS() ) );
			m_result = expr::makeFnCall( expr->getType()
				, expr::makeIdentifier( var::makeVariable( type::getFunction(), "mul" ) )
				, std::move( argsList ) );
		}
		else
		{
			m_result = expr::makeTimes( expr->getType()
				, doSubmit( expr->getLHS() )
				, doSubmit( expr->getRHS() ) );
		}
	}

	bool ExprAdapter::doProcessSampledImageArg( expr::Expr & arg
		, bool writeSampler
		, expr::ExprList & args )
	{
		bool result = arg.getKind() == expr::Kind::eIdentifier;

		if ( result )
		{
			auto ident = findIdentifier( &arg );
			auto it = m_linkedVars.find( ident->getVariable() );

			if ( m_linkedVars.end() != it )
			{
				args.emplace_back( makeIdent( it->second.first ) );

				if ( writeSampler )
				{
					args.emplace_back( makeIdent( it->second.second ) );
				}
			}
			else
			{
				args.emplace_back( doSubmit( &arg ) );
			}
		}

		return result;
	}

	void ExprAdapter::doProcessTextureQueries( expr::TextureAccessCall * expr )
	{
		expr::ExprList args;

		for ( auto & arg : expr->getArgList() )
		{
			if ( !doProcessSampledImageArg( *arg, false, args ) )
			{
				args.emplace_back( doSubmit( arg.get() ) );
			}
		}

		m_result = expr::makeTextureAccessCall( expr->getType()
			, expr->getTextureAccess()
			, std::move( args ) );
	}

	void ExprAdapter::doProcessTexelFetch( expr::TextureAccessCall * expr )
	{
		expr::ExprList args;
		// First parameter should be sampled image
		auto isImage = doProcessSampledImageArg( *expr->getArgList()[0], false, args );
		assert( isImage );

		if ( expr->getTextureAccess() == expr::TextureAccess::eTexelFetchBufferF
			|| expr->getTextureAccess() == expr::TextureAccess::eTexelFetchBufferI
			|| expr->getTextureAccess() == expr::TextureAccess::eTexelFetchBufferU )
		{
			// For those texel fetch functions, no lod, and none needed.
			assert( expr->getArgList().size() >= 2u );
			args.emplace_back( doSubmit( expr->getArgList()[1].get() ) );
		}
		else
		{
			expr::ExprList merged;

			if ( expr->getTextureAccess() == expr::TextureAccess::eTexelFetch2DRectF
				|| expr->getTextureAccess() == expr::TextureAccess::eTexelFetch2DRectI
				|| expr->getTextureAccess() == expr::TextureAccess::eTexelFetch2DRectU )
			{
				// For those texel fetch functions, no lod, hence create a 0 lod.
				assert( expr->getArgList().size() >= 2u );
				// Merge second and literal(0) parameters to the appropriate vector type (int=>ivec2, ivec2=>ivec3, ivec3=>ivec4).
				merged.emplace_back( doSubmit( expr->getArgList()[1].get() ) );
				merged.emplace_back( expr::makeLiteral( 0 ) );
			}
			else
			{
				// For those texel fetch functions, lod is put inside the coords parameter, instead of being aside.
				assert( expr->getArgList().size() >= 3u );
				// Merge second and third parameters to the appropriate vector type (int=>ivec2, ivec2=>ivec3, ivec3=>ivec4).
				merged.emplace_back( doSubmit( expr->getArgList()[1].get() ) );
				merged.emplace_back( doSubmit( expr->getArgList()[2].get() ) );
			}

			switch ( merged[0]->getType()->getKind() )
			{
			case type::Kind::eInt:
				args.emplace_back( sdw::makeFnCall( type::makeType( typeEnum< IVec2 > )
					, sdw::makeIdent( var::makeFunction( "int2" ) )
					, std::move( merged ) ) );
				break;
			case type::Kind::eVec2I:
				args.emplace_back( sdw::makeFnCall( type::makeType( typeEnum< IVec3 > )
					, sdw::makeIdent( var::makeFunction( "int3" ) )
					, std::move( merged ) ) );
				break;
			case type::Kind::eVec3I:
				args.emplace_back( sdw::makeFnCall( type::makeType( typeEnum< IVec4 > )
					, sdw::makeIdent( var::makeFunction( "int4" ) )
					, std::move( merged ) ) );
				break;
			}
		}

		// Other parameters remain unchanged.
		for ( size_t i = 3u; i < expr->getArgList().size(); ++i )
		{
			args.emplace_back( doSubmit( expr->getArgList()[i].get() ) );
		}

		m_result = expr::makeTextureAccessCall( expr->getType()
			, expr->getTextureAccess()
			, std::move( args ) );
	}

	void ExprAdapter::doProcessTextureGradShadow( expr::TextureAccessCall * expr )
	{
		// Sample grad doesn't support SampleComparisonState, replace it with a SampleCmp.
		expr::ExprList args;
		// First parameter should be sampled image
		auto isImage = doProcessSampledImageArg( *expr->getArgList()[0], false, args );
		assert( isImage );
		assert( expr->getArgList().size() >= 5u );
		// Second param is texcoord
		args.emplace_back( doSubmit( expr->getArgList()[1].get() ) );
		// Third param is dref value
		args.emplace_back( doSubmit( expr->getArgList()[2].get() ) );
		// Fourth and fifth params ard dPdx and dPdy, drop them

		// Other parameters remain unchanged.
		for ( size_t i = 5u; i < expr->getArgList().size(); ++i )
		{
			args.emplace_back( doSubmit( expr->getArgList()[i].get() ) );
		}

		auto result = expr::makeTextureAccessCall( expr->getType()
			, getSampleCmp( expr->getTextureAccess() )
			, std::move( args ) );

		// Reparse the created expression, textureProj cases.
		visitTextureAccessCallExpr( result.get() );
	}
}