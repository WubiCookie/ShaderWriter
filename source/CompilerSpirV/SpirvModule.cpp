/*
See LICENSE file in root folder
*/
#include "SpirvModule.hpp"

#include "SpirvHelpers.hpp"

#include <ShaderAST/Type/TypeImage.hpp>
#include <ShaderAST/Type/TypeSampledImage.hpp>
#include <ShaderAST/Type/TypeArray.hpp>

#include <algorithm>

namespace spirv
{
	//*************************************************************************

	namespace
	{
		std::vector< uint32_t > const & packString( std::string const & name )
		{
			static std::map < std::string, std::vector< uint32_t > > cache;
			auto it = cache.find( name );

			if ( it == cache.end() )
			{
				std::vector< uint32_t > packed;
				uint32_t word{ 0u };
				uint32_t offset{ 0u };
				size_t i = 0u;

				while ( i < name.size() )
				{
					if ( i != 0 && ( i % 4u ) == 0u )
					{
						packed.push_back( word );
						word = 0u;
						offset = 0u;
					}

					word |= ( uint32_t( name[i] ) & 0x000000FF ) << offset;
					++i;
					offset += 8u;
				}

				if ( word )
				{
					packed.push_back( word );
				}

				if ( i != 0 && ( i % 4u ) == 0u )
				{
					packed.push_back( 0u );
				}

				it = cache.emplace( name, packed ).first;
			}

			return it->second;
		}

		spv::BuiltIn getBuiltin( std::string const & name )
		{
			auto result = spv::BuiltIn( -1 );

			if ( name == "gl_Position" )
			{
				result = spv::BuiltIn::Position;
			}
			else if ( name == "gl_PointSize" )
			{
				result = spv::BuiltIn::PointSize;
			}
			else if ( name == "gl_ClipDistance" )
			{
				result = spv::BuiltIn::ClipDistance;
			}
			else if ( name == "gl_CullDistance" )
			{
				result = spv::BuiltIn::CullDistance;
			}
			else if ( name == "gl_VertexID" )
			{
				result = spv::BuiltIn::VertexId;
			}
			else if ( name == "gl_InstanceID" )
			{
				result = spv::BuiltIn::InstanceId;
			}
			else if ( name == "gl_PrimitiveID" )
			{
				result = spv::BuiltIn::PrimitiveId;
			}
			else if ( name == "gl_InvocationID" )
			{
				result = spv::BuiltIn::InvocationId;
			}
			else if ( name == "gl_Layer" )
			{
				result = spv::BuiltIn::Layer;
			}
			else if ( name == "gl_ViewportIndex" )
			{
				result = spv::BuiltIn::ViewportIndex;
			}
			else if ( name == "gl_TessLevelOuter" )
			{
				result = spv::BuiltIn::TessLevelOuter;
			}
			else if ( name == "gl_TessLevelInner" )
			{
				result = spv::BuiltIn::TessLevelInner;
			}
			else if ( name == "gl_TessCoord" )
			{
				result = spv::BuiltIn::TessCoord;
			}
			else if ( name == "gl_PatchVertices" )
			{
				result = spv::BuiltIn::PatchVertices;
			}
			else if ( name == "gl_FragCoord" )
			{
				result = spv::BuiltIn::FragCoord;
			}
			else if ( name == "gl_PointCoord" )
			{
				result = spv::BuiltIn::PointCoord;
			}
			else if ( name == "gl_FrontFacing" )
			{
				result = spv::BuiltIn::FrontFacing;
			}
			else if ( name == "gl_SampleID" )
			{
				result = spv::BuiltIn::SampleId;
			}
			else if ( name == "gl_SamplePosition" )
			{
				result = spv::BuiltIn::SamplePosition;
			}
			else if ( name == "gl_SampleMask" )
			{
				result = spv::BuiltIn::SampleMask;
			}
			else if ( name == "gl_FragDepth" )
			{
				result = spv::BuiltIn::FragDepth;
			}
			else if ( name == "gl_HelperInvocation" )
			{
				result = spv::BuiltIn::HelperInvocation;
			}
			else if ( name == "gl_NumWorkgroups" )
			{
				result = spv::BuiltIn::NumWorkgroups;
			}
			else if ( name == "gl_WorkgroupSize" )
			{
				result = spv::BuiltIn::WorkgroupSize;
			}
			else if ( name == "gl_WorkgroupID" )
			{
				result = spv::BuiltIn::WorkgroupId;
			}
			else if ( name == "gl_LocalInvocationID" )
			{
				result = spv::BuiltIn::LocalInvocationId;
			}
			else if ( name == "gl_GlobalInvocationID" )
			{
				result = spv::BuiltIn::GlobalInvocationId;
			}
			else if ( name == "gl_LocalInvocationIndex" )
			{
				result = spv::BuiltIn::LocalInvocationIndex;
			}
			else if ( name == "gl_WorkDim" )
			{
				result = spv::BuiltIn::WorkDim;
			}
			else if ( name == "gl_GlobalSize" )
			{
				result = spv::BuiltIn::GlobalSize;
			}
			else if ( name == "gl_EnqueuedWorkgroupSize" )
			{
				result = spv::BuiltIn::EnqueuedWorkgroupSize;
			}
			else if ( name == "gl_GlobalOffset" )
			{
				result = spv::BuiltIn::GlobalOffset;
			}
			else if ( name == "gl_GlobalLinearID" )
			{
				result = spv::BuiltIn::GlobalLinearId;
			}
			else if ( name == "gl_SubgroupSize" )
			{
				result = spv::BuiltIn::SubgroupSize;
			}
			else if ( name == "gl_SubgroupMaxSize" )
			{
				result = spv::BuiltIn::SubgroupMaxSize;
			}
			else if ( name == "gl_NumSubgroups" )
			{
				result = spv::BuiltIn::NumSubgroups;
			}
			else if ( name == "gl_NumEnqueuedSubgroups" )
			{
				result = spv::BuiltIn::NumEnqueuedSubgroups;
			}
			else if ( name == "gl_SubgroupID" )
			{
				result = spv::BuiltIn::SubgroupId;
			}
			else if ( name == "gl_SubgroupLocalInvocationID" )
			{
				result = spv::BuiltIn::SubgroupLocalInvocationId;
			}
			else if ( name == "gl_VertexIndex" )
			{
				result = spv::BuiltIn::VertexIndex;
			}
			else if ( name == "gl_InstanceIndex" )
			{
				result = spv::BuiltIn::InstanceIndex;
			}
			else if ( name == "gl_SubgroupEqMaskKHR" )
			{
				result = spv::BuiltIn::SubgroupEqMaskKHR;
			}
			else if ( name == "gl_SubgroupGeMaskKHR" )
			{
				result = spv::BuiltIn::SubgroupGeMaskKHR;
			}
			else if ( name == "gl_SubgroupGtMaskKHR" )
			{
				result = spv::BuiltIn::SubgroupGtMaskKHR;
			}
			else if ( name == "gl_SubgroupLeMaskKHR" )
			{
				result = spv::BuiltIn::SubgroupLeMaskKHR;
			}
			else if ( name == "gl_SubgroupLtMaskKHR" )
			{
				result = spv::BuiltIn::SubgroupLtMaskKHR;
			}
			else if ( name == "gl_BaseVertex" )
			{
				result = spv::BuiltIn::BaseVertex;
			}
			else if ( name == "gl_BaseInstance" )
			{
				result = spv::BuiltIn::BaseInstance;
			}
			else if ( name == "gl_DrawIndex" )
			{
				result = spv::BuiltIn::DrawIndex;
			}
			else if ( name == "gl_DeviceIndex" )
			{
				result = spv::BuiltIn::DeviceIndex;
			}
			else if ( name == "gl_ViewIndex" )
			{
				result = spv::BuiltIn::ViewIndex;
			}
			else if ( name == "gl_BaryCoordNoPerspAMD" )
			{
				result = spv::BuiltIn::BaryCoordNoPerspAMD;
			}
			else if ( name == "gl_BaryCoordNoPerspCentroidAMD" )
			{
				result = spv::BuiltIn::BaryCoordNoPerspCentroidAMD;
			}
			else if ( name == "gl_BaryCoordNoPerspSampleAMD" )
			{
				result = spv::BuiltIn::BaryCoordNoPerspSampleAMD;
			}
			else if ( name == "gl_BaryCoordSmoothAMD" )
			{
				result = spv::BuiltIn::BaryCoordSmoothAMD;
			}
			else if ( name == "gl_BaryCoordSmoothCentroidAMD" )
			{
				result = spv::BuiltIn::BaryCoordSmoothCentroidAMD;
			}
			else if ( name == "gl_BaryCoordSmoothSampleAMD" )
			{
				result = spv::BuiltIn::BaryCoordSmoothSampleAMD;
			}
			else if ( name == "gl_BaryCoordPullModelAMD" )
			{
				result = spv::BuiltIn::BaryCoordPullModelAMD;
			}
			else if ( name == "gl_FragStencilRefEXT" )
			{
				result = spv::BuiltIn::FragStencilRefEXT;
			}
			else if ( name == "gl_ViewportMaskNV" )
			{
				result = spv::BuiltIn::ViewportMaskNV;
			}
			else if ( name == "gl_SecondaryPositionNV" )
			{
				result = spv::BuiltIn::SecondaryPositionNV;
			}
			else if ( name == "gl_SecondaryViewportMaskNV" )
			{
				result = spv::BuiltIn::SecondaryViewportMaskNV;
			}
			else if ( name == "gl_PositionPerViewNV" )
			{
				result = spv::BuiltIn::PositionPerViewNV;
			}
			else if ( name == "gl_ViewportMaskPerViewNV" )
			{
				result = spv::BuiltIn::ViewportMaskPerViewNV;
			}

			return result;
		}

		ast::type::Kind getComponentType( ast::type::ImageFormat format )
		{
			ast::type::Kind result;

			switch ( format )
			{
			case ast::type::ImageFormat::eRgba32f:
			case ast::type::ImageFormat::eRgba16f:
			case ast::type::ImageFormat::eRg32f:
			case ast::type::ImageFormat::eRg16f:
			case ast::type::ImageFormat::eR32f:
			case ast::type::ImageFormat::eR16f:
				return ast::type::Kind::eFloat;

			case ast::type::ImageFormat::eRgba32i:
			case ast::type::ImageFormat::eRgba16i:
			case ast::type::ImageFormat::eRgba8i:
			case ast::type::ImageFormat::eRg32i:
			case ast::type::ImageFormat::eRg16i:
			case ast::type::ImageFormat::eRg8i:
			case ast::type::ImageFormat::eR32i:
			case ast::type::ImageFormat::eR16i:
			case ast::type::ImageFormat::eR8i:
				return ast::type::Kind::eInt;

			case ast::type::ImageFormat::eRgba32u:
			case ast::type::ImageFormat::eRgba16u:
			case ast::type::ImageFormat::eRgba8u:
			case ast::type::ImageFormat::eRg32u:
			case ast::type::ImageFormat::eRg16u:
			case ast::type::ImageFormat::eRg8u:
			case ast::type::ImageFormat::eR32u:
			case ast::type::ImageFormat::eR16u:
			case ast::type::ImageFormat::eR8u:
				return ast::type::Kind::eUInt;

			default:
				assert( false && "Unsupported ast::type::ImageFormat." );
				return ast::type::Kind::eFloat;
			}

			return result;
		}

		ast::type::TypePtr getUnqualifiedType( ast::type::TypePtr qualified );

		ast::type::StructPtr getUnqualifiedType( ast::type::Struct const & qualified )
		{
			auto result = qualified.getCache().getStruct( qualified.getMemoryLayout(), qualified.getName() );
			assert( result->empty() || ( result->size() == qualified.size() ) );

			if ( result->empty() && !qualified.empty() )
			{
				for ( auto & member : qualified )
				{
					auto type = getUnqualifiedType( member.type );

					if ( type->getKind() == ast::type::Kind::eArray )
					{
						result->declMember( member.name
							, std::static_pointer_cast< ast::type::Array >( type ) );
					}
					else if ( type->getKind() == ast::type::Kind::eStruct )
					{
						result->declMember( member.name
							, std::static_pointer_cast< ast::type::Struct >( type ) );
					}
					else
					{
						result->declMember( member.name
							, type );
					}
				}
			}

			return result;
		}

		ast::type::ArrayPtr getUnqualifiedType( ast::type::Array const & qualified )
		{
			return qualified.getCache().getArray( getUnqualifiedType( qualified.getType() ), qualified.getArraySize() );
		}

		ast::type::SamplerPtr getUnqualifiedType( ast::type::Sampler const & qualified )
		{
			return qualified.getCache().getSampler( qualified.isComparison() );
		}

		ast::type::SampledImagePtr getUnqualifiedType( ast::type::SampledImage const & qualified )
		{
			return qualified.getCache().getSampledImage( qualified.getConfig() );
		}

		ast::type::ImagePtr getUnqualifiedType( ast::type::Image const & qualified )
		{
			return qualified.getCache().getImage( qualified.getConfig() );
		}

		ast::type::TypePtr getUnqualifiedType( ast::type::Type const & qualified )
		{
			ast::type::TypePtr result;

			if ( qualified.getKind() == ast::type::Kind::eArray )
			{
				result = getUnqualifiedType( static_cast< ast::type::Array const & >( qualified ) );
			}
			else if ( qualified.getKind() == ast::type::Kind::eStruct )
			{
				result = getUnqualifiedType( static_cast< ast::type::Struct const & >( qualified ) );
			}
			else if ( qualified.getKind() == ast::type::Kind::eImage )
			{
				result = getUnqualifiedType( static_cast< ast::type::Image const & >( qualified ) );
			}
			else if ( qualified.getKind() == ast::type::Kind::eSampledImage )
			{
				result = getUnqualifiedType( static_cast< ast::type::SampledImage const & >( qualified ) );
			}
			else if ( qualified.getKind() == ast::type::Kind::eSampler )
			{
				result = getUnqualifiedType( static_cast< ast::type::Sampler const & >( qualified ) );
			}
			else if ( qualified.isMember() )
			{
				result = qualified.getCache().makeType( qualified.getKind() );
			}

			return result;
		}

		ast::type::TypePtr getUnqualifiedType( ast::type::TypePtr qualified )
		{
			ast::type::TypePtr result = getUnqualifiedType( *qualified );
			return result
				? result
				: qualified;
		}

		ast::type::MemoryLayout getMemoryLayout( ast::type::Type const & type )
		{
			ast::type::MemoryLayout result{ ast::type::MemoryLayout::eStd430 };
			auto kind = type.getKind();

			if ( kind == ast::type::Kind::eArray )
			{
				if ( type.isMember() )
				{
					result = getMemoryLayout( *type.getParent() );
				}
				else
				{
					result = getMemoryLayout( *static_cast< ast::type::Array const & >( type ).getType() );
				}
			}
			else if ( kind == ast::type::Kind::eStruct )
			{
				auto & structType = static_cast< ast::type::Struct const & >( type );
				result = structType.getMemoryLayout();
			}
			else if ( type.isMember() )
			{
				result = getMemoryLayout( *type.getParent() );
			}

			return result;
		}

		void writeArrayStride( Module & module
			, ast::type::TypePtr type
			, uint32_t typeId
			, uint32_t arrayStride )
		{
			auto kind = getNonArrayKind( type );

			if ( kind != ast::type::Kind::eImage
				&& kind != ast::type::Kind::eSampledImage
				&& kind != ast::type::Kind::eSampler )
			{
				module.decorate( typeId
					, IdList
					{
						uint32_t( spv::Decoration::ArrayStride ),
						arrayStride
					} );
			}
		}
	}

	//*************************************************************************

	size_t IdListHasher::operator()( IdList const & list )const
	{
		assert( !list.empty() );
		auto hash = std::hash< spv::Id >{}( list[0] );

		std::for_each( list.begin() + 1u
			, list.end()
			, [&hash]( spv::Id id )
			{
				ast::type::hashCombine( hash, id );
			} );

		return hash;
	}

	//*************************************************************************

	Instruction::Instruction( Config const & config
		, spv::Op op
		, std::optional< spv::Id > returnTypeId
		, std::optional< spv::Id > resultId
		, IdList operands
		, std::optional< std::string > name
		, std::optional< std::map< int32_t, spv::Id > > labels )
		: returnTypeId{ returnTypeId }
		, resultId{ resultId }
		, operands{ operands }
		, packedName{ std::nullopt }
		, config{ config }
		, name{ name }
		, labels{ labels }
	{
		if ( bool( this->name ) )
		{
			packedName = packString( this->name.value() );
		}

		this->op.op = op;
		this->op.opCount = uint16_t( 1u
			+ ( bool( this->returnTypeId ) ? 1u : 0u )
			+ ( bool( this->resultId ) ? 1u : 0u )
			+ this->operands.size()
			+ ( bool( this->packedName ) ? this->packedName.value().size() : 0u ) );

		assertType( *this, config );
	}

	void Instruction::serialize( UInt32List & buffer
		, Instruction const & instruction )
	{
		assertType( instruction, instruction.config );

		auto pushValue = [&buffer]( uint32_t value )
		{
			buffer.push_back( value );
		};

		pushValue( instruction.op.opValue );
		//pushValue( uint32_t( instruction.op.op ) );

		if ( instruction.returnTypeId )
		{
			pushValue( instruction.returnTypeId.value() );
		}
		
		if ( instruction.resultId )
		{
			pushValue( instruction.resultId.value() );
		}
		
		if ( !instruction.operands.empty() )
		{
			for ( auto & operand : instruction.operands )
			{
				pushValue( operand );
			}
		}

		if ( instruction.packedName )
		{
			for ( auto & c : instruction.packedName.value() )
			{
				pushValue( c );
			}
		}
		else if ( instruction.labels )
		{
			for ( auto & label : instruction.labels.value() )
			{
				pushValue( label.first );
				pushValue( label.second );
			}
		}
	}

	InstructionPtr Instruction::deserialize( UInt32ListCIt & buffer
		, bool returnTypeId
		, bool resultId
		, uint32_t operandsCount
		, bool name
		, bool labels )
	{
		Config config{};
		auto result = std::make_unique< Instruction >( config );
		auto index = 0u;
		auto popValue = [&buffer, &index]()
		{
			auto result = *buffer;
			++buffer;
			++index;
			return result;
		};
		result->op.opValue = popValue();

		if ( returnTypeId )
		{
			result->returnTypeId = popValue();
		}

		if ( resultId )
		{
			result->resultId = popValue();
		}

		if ( operandsCount )
		{
			result->operands.resize( operandsCount );

			for ( auto & operand : result->operands )
			{
				operand = popValue();
			}
		}

		if ( name )
		{
			result->packedName = UInt32List( result->op.opCount - 1u - index );

			for ( auto & c : result->packedName.value() )
			{
				c = popValue();
			}
		}
		else if ( labels )
		{
			auto count = ( result->op.opCount - 1u - index ) / 2u;
			result->labels = std::map< int32_t, spv::Id >{};

			for ( auto i = 0u; i < count; ++i )
			{
				auto label = popValue();
				result->labels.value()[label] = popValue();
			}
		}

		return result;
	}

	Instruction::~Instruction()
	{
	}

	//*************************************************************************

	Module::Module( ast::type::TypesCache & cache
		, spv::MemoryModel memoryModel
		, spv::ExecutionModel executionModel )
		: variables{ &globalDeclarations }
		, memoryModel{ makeInstruction< MemoryModelInstruction >( spv::Id( spv::AddressingModel::Logical ), spv::Id( memoryModel ) ) }
		, m_cache{ cache }
		, m_model{ executionModel }
	{
		header.push_back( spv::MagicNumber );
		header.push_back( spv::Version );
		header.push_back( 0x00100001 );
		header.push_back( 0u ); // Bound IDs.
		header.push_back( 0u ); // Schema.
		m_currentId = &header[3];
		auto id = getIntermediateResult();
		extensions.push_back( makeInstruction< ExtInstImportInstruction >( id
			, "GLSL.std.450" ) );
		debug.push_back( makeInstruction< SourceInstruction >( spv::Id( spv::SourceLanguage::GLSL ), 450u ) );

		switch ( m_model )
		{
		case spv::ExecutionModel::Vertex:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Shader ) ) );
			break;
		case spv::ExecutionModel::TessellationControl:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Shader ) ) );
			break;
		case spv::ExecutionModel::TessellationEvaluation:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Shader ) ) );
			break;
		case spv::ExecutionModel::Geometry:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Shader ) ) );
			break;
		case spv::ExecutionModel::Fragment:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Shader ) ) );
			break;
		case spv::ExecutionModel::GLCompute:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Shader ) ) );
			break;
		case spv::ExecutionModel::Kernel:
			capabilities.push_back( makeInstruction< CapabilityInstruction >( spv::Id( spv::Capability::Kernel ) ) );
			break;
		default:
			assert( false && "Unsupported ExecutionModel" );
			break;
		}
	}

	spv::Id Module::registerType( ast::type::TypePtr type )
	{
		return registerType( type
			, ast::type::NotMember
			, 0u
			, 0u );
	}

	spv::Id Module::registerPointerType( spv::Id type, spv::StorageClass storage )
	{
		uint64_t key = ( uint64_t( type ) << 32 ) | uint64_t( storage );
		auto it = m_registeredPointerTypes.find( key );

		if ( it == m_registeredPointerTypes.end() )
		{
			spv::Id id{ ++*m_currentId };
			it = m_registeredPointerTypes.emplace( key, id ).first;
			globalDeclarations.push_back( makeInstruction< PointerTypeInstruction >( id
				, spv::Id( storage ), type ) );
		}

		return it->second;
	}

	void Module::decorate( spv::Id id, spv::Decoration decoration )
	{
		decorate( id, IdList{ spv::Id( decoration ) } );
	}

	void Module::decorate( spv::Id id
		, IdList const & decoration )
	{
		IdList operands;
		operands.push_back( id );
		operands.insert( operands.end(), decoration.begin(), decoration.end() );
		decorations.push_back( makeInstruction< DecorateInstruction >( operands ) );
	}

	void Module::decorateMember( spv::Id id
		, uint32_t index
		, spv::Decoration decoration )
	{
		decorateMember( id, index, IdList{ spv::Id( decoration ) } );
	}

	void Module::decorateMember( spv::Id id
		, uint32_t index
		, IdList const & decoration )
	{
		IdList operands;
		operands.push_back( id );
		operands.push_back( index );
		operands.insert( operands.end(), decoration.begin(), decoration.end() );
		decorations.push_back( makeInstruction< MemberDecorateInstruction >( operands ) );
	}

	spv::Id Module::loadVariable( spv::Id variable
		, ast::type::TypePtr type
		, Block & currentBlock )
	{
		auto result = getIntermediateResult();
		currentBlock.instructions.push_back( makeInstruction< LoadInstruction >( registerType( type )
			, result
			, variable ) );
		lnkIntermediateResult( result, variable );
		return result;
	}

	void Module::bindVariable( spv::Id varId
		, uint32_t bindingPoint
		, uint32_t descriptorSet )
	{
		decorate( varId, { spv::Id( spv::Decoration::Binding ), bindingPoint } );
		decorate( varId, { spv::Id( spv::Decoration::DescriptorSet ), descriptorSet } );
	}

	void Module::bindBufferVariable( std::string const & name
		, uint32_t bindingPoint
		, uint32_t descriptorSet
		, spv::Decoration structDecoration )
	{
		auto varIt = m_registeredVariables.find( name );

		if ( varIt != m_registeredVariables.end() )
		{
			auto varId = varIt->second;
			decorate( varId, { spv::Id( spv::Decoration::Binding ), bindingPoint } );
			decorate( varId, { spv::Id( spv::Decoration::DescriptorSet ), descriptorSet } );

			auto typeIt = m_registeredVariablesTypes.find( varId );

			if ( typeIt != m_registeredVariablesTypes.end() )
			{
				auto typeId = typeIt->second;
				decorate( typeId, structDecoration );
			}
		}
	}

	spv::Id Module::registerVariable( std::string const & name
		, spv::StorageClass storage
		, ast::type::TypePtr type )
	{
		auto it = m_registeredVariables.find( name );

		if ( it == m_registeredVariables.end() )
		{
			spv::Id id{ ++*m_currentId };
			it = m_registeredVariables.emplace( name, id ).first;

			if ( type->getKind() != ast::type::Kind::eStruct
				|| std::static_pointer_cast< ast::type::Struct >( type )->getName() != name )
			{
				debug.push_back( makeInstruction< NameInstruction >( id, name ) );
			}
			else if ( type->getKind() == ast::type::Kind::eStruct
				|| std::static_pointer_cast< ast::type::Struct >( type )->getName() == name )
			{
				debug.push_back( makeInstruction< NameInstruction >( id, name + "Inst" ) );
			}

			auto builtin = getBuiltin( name );

			if ( builtin != spv::BuiltIn( -1 ) )
			{
				decorate( id, { spv::Id( spv::Decoration::BuiltIn ), spv::Id( builtin ) } );
			}

			auto rawTypeId = registerType( type );
			auto varTypeId = registerPointerType( rawTypeId, storage );

			if ( storage == spv::StorageClass::Function
				&& m_currentFunction )
			{
				m_currentFunction->variables.push_back( makeInstruction< VariableInstruction >( varTypeId
					, id
					, spv::Id( storage ) ) );
			}
			else
			{
				globalDeclarations.push_back( makeInstruction< VariableInstruction >( varTypeId
					, id
					, spv::Id( storage ) ) );
			}

			m_registeredVariablesTypes.emplace( id, rawTypeId );
		}

		return it->second;
	}

	spv::Id Module::registerSpecConstant( std::string name
		, uint32_t location
		, ast::type::TypePtr type
		, ast::expr::Literal const & value )
	{
		auto it = m_registeredVariables.find( name );

		if ( it == m_registeredVariables.end() )
		{
			spv::Id id{ ++*m_currentId };
			it = m_registeredVariables.emplace( name, id ).first;
			auto rawTypeId = registerType( type );
			IdList operands;
			debug.push_back( makeInstruction< NameInstruction >( id, name ) );

			if ( value.getLiteralType() == ast::expr::LiteralType::eBool )
			{
				if ( value.getValue< ast::expr::LiteralType::eBool >() )
				{
					globalDeclarations.emplace_back( makeInstruction< SpecConstantTrueInstruction >( rawTypeId, id ) );
				}
				else
				{
					globalDeclarations.emplace_back( makeInstruction< SpecConstantFalseInstruction >( rawTypeId, id ) );
				}
			}
			else
			{
				switch ( value.getLiteralType() )
				{
				case ast::expr::LiteralType::eInt:
					operands.emplace_back( uint32_t( value.getValue< ast::expr::LiteralType::eInt >() ) );
					break;
				case ast::expr::LiteralType::eUInt:
					operands.emplace_back( value.getValue< ast::expr::LiteralType::eUInt >() );
					break;
				case ast::expr::LiteralType::eFloat:
				{
					operands.resize( sizeof( float ) / sizeof( uint32_t ) );
					auto dst = reinterpret_cast< float * >( operands.data() );
					*dst = value.getValue< ast::expr::LiteralType::eFloat >();
				}
				break;
				case ast::expr::LiteralType::eDouble:
				{
					operands.resize( sizeof( double ) / sizeof( uint32_t ) );
					auto dst = reinterpret_cast< double * >( operands.data() );
					*dst = value.getValue< ast::expr::LiteralType::eDouble >();
				}
				break;
				}

				globalDeclarations.emplace_back( makeInstruction< SpecConstantInstruction >( rawTypeId
					, id
					, operands ) );
			}

			decorate( id, { spv::Id( spv::Decoration::SpecId ), spv::Id( location ) } );
			m_registeredVariablesTypes.emplace( id, rawTypeId );
			m_registeredConstants.emplace( id, value.getType() );
		}

		return it->second;
	}

	spv::Id Module::registerParameter( ast::type::TypePtr type )
	{
		auto typeId = registerType( type );
		auto paramId = ++*m_currentId;
		return paramId;
	}

	spv::Id Module::registerMemberVariableIndex( ast::type::TypePtr type )
	{
		assert( type->isMember() );
		return registerLiteral( type->getIndex() );
	}

	spv::Id Module::registerMemberVariable( spv::Id outer
		, std::string name
		, ast::type::TypePtr type )
	{
		auto it = std::find_if( m_registeredVariables.begin()
			, m_registeredVariables.end()
			, [outer]( std::map< std::string, spv::Id >::value_type const & pair )
			{
				return pair.second == outer;
			} );
		assert( it != m_registeredVariables.end() );
		assert( type->isMember() );
		auto fullName = it->first + "::" + name;
		auto outerId = it->second;
		it = m_registeredVariables.find( fullName );

		if ( it == m_registeredVariables.end() )
		{
			spv::Id id{ ++*m_currentId };
			m_registeredMemberVariables.insert( { fullName, { outer, id } } );
			it = m_registeredVariables.emplace( fullName, id ).first;
			registerLiteral( type->getIndex() );
		}

		return it->second;
	}

	ast::type::Kind Module::getLiteralType( spv::Id litId )const
	{
		auto it = m_registeredConstants.find( litId );
		assert( it != m_registeredConstants.end() );
		return it->second->getKind();
	}

	spv::Id Module::getOuterVariable( spv::Id mbrId )const
	{
		auto itInner = std::find_if( m_registeredMemberVariables.begin()
			, m_registeredMemberVariables.end()
			, [mbrId]( std::map< std::string, std::pair< spv::Id, spv::Id > >::value_type const & pair )
			{
				return pair.second.second == mbrId;
			} );
		assert( itInner != m_registeredMemberVariables.end() );
		
		auto result = itInner->second.first;
		auto itOuter = m_registeredMemberVariables.end();

		while ( ( itOuter = std::find_if( m_registeredMemberVariables.begin()
				, m_registeredMemberVariables.end()
				, [result]( std::map< std::string, std::pair< spv::Id, spv::Id > >::value_type const & pair )
				{
					return pair.second.second == result;
				} ) ) != m_registeredMemberVariables.end() )
		{
			result = itOuter->second.first;
		}

		auto itOutermost = std::find_if( m_registeredVariables.begin()
			, m_registeredVariables.end()
			, [result]( std::map< std::string, spv::Id >::value_type const & pair )
			{
					return pair.second == result;
			} );
		assert( itOutermost != m_registeredVariables.end() );
		result = itOutermost->second;
		return result;
	}

	spv::Id Module::registerLiteral( bool value )
	{
		auto it = m_registeredBoolConstants.find( value );

		if ( it == m_registeredBoolConstants.end() )
		{
			spv::Id result{ ++*m_currentId };
			auto type = registerType( m_cache.getBool() );

			if ( value )
			{
				globalDeclarations.push_back( makeInstruction< ConstantTrueInstruction >( type, result ) );
			}
			else
			{
				globalDeclarations.push_back( makeInstruction< ConstantFalseInstruction >( type, result ) );
			}

			it = m_registeredBoolConstants.emplace( value, result ).first;
			m_registeredConstants.emplace( result, m_cache.getBool() );
		}

		return it->second;
	}

	spv::Id Module::registerLiteral( int32_t value )
	{
		auto it = m_registeredIntConstants.find( value );

		if ( it == m_registeredIntConstants.end() )
		{
			spv::Id result{ ++*m_currentId };
			auto type = registerType( m_cache.getInt() );
			globalDeclarations.push_back( makeInstruction< ConstantInstruction >( type
				, result
				, IdList{ uint32_t( value ) } ) );
			it = m_registeredIntConstants.emplace( value, result ).first;
			m_registeredConstants.emplace( result, m_cache.getInt() );
		}

		return it->second;
	}

	spv::Id Module::registerLiteral( uint32_t value )
	{
		auto it = m_registeredUIntConstants.find( value );

		if ( it == m_registeredUIntConstants.end() )
		{
			spv::Id result{ ++*m_currentId };
			auto type = registerType( m_cache.getUInt() );
			globalDeclarations.push_back( makeInstruction< ConstantInstruction >( type
				, result
				, IdList{ value } ) );
			it = m_registeredUIntConstants.emplace( value, result ).first;
			m_registeredConstants.emplace( result, m_cache.getUInt() );
		}

		return it->second;
	}

	spv::Id Module::registerLiteral( float value )
	{
		auto it = m_registeredFloatConstants.find( value );

		if ( it == m_registeredFloatConstants.end() )
		{
			spv::Id result{ ++*m_currentId };
			auto type = registerType( m_cache.getFloat() );
			globalDeclarations.push_back( makeInstruction< ConstantInstruction >( type
				, result
				, IdList{ *reinterpret_cast< uint32_t * >( &value ) } ) );
			it = m_registeredFloatConstants.emplace( value, result ).first;
			m_registeredConstants.emplace( result, m_cache.getFloat() );
		}

		return it->second;
	}

	spv::Id Module::registerLiteral( double value )
	{
		auto it = m_registeredDoubleConstants.find( value );

		if ( it == m_registeredDoubleConstants.end() )
		{
			spv::Id result{ ++*m_currentId };
			auto type = registerType( m_cache.getDouble() );
			IdList list;
			list.resize( 2u );
			auto dst = reinterpret_cast< double * >( list.data() );
			*dst = value;
			globalDeclarations.push_back( makeInstruction< ConstantInstruction >( type
				, result
				, list ) );
			it = m_registeredDoubleConstants.emplace( value, result ).first;
			m_registeredConstants.emplace( result, m_cache.getDouble() );
		}

		return it->second;
	}

	spv::Id Module::registerLiteral( IdList const & initialisers
		, ast::type::TypePtr type )
	{
		auto typeId = registerType( type );
		auto it = std::find_if( m_registeredCompositeConstants.begin()
			, m_registeredCompositeConstants.end()
			, [initialisers]( std::pair< IdList, spv::Id > const & lookup )
			{
				return lookup.first == initialisers;
			} );

		if ( it == m_registeredCompositeConstants.end() )
		{
			spv::Id result{ ++*m_currentId };
			globalDeclarations.push_back( makeInstruction< ConstantCompositeInstruction >( typeId
				, result
				, initialisers ) );
			m_registeredCompositeConstants.emplace_back( initialisers, result );
			it = m_registeredCompositeConstants.begin() + m_registeredCompositeConstants.size() - 1u;
			m_registeredConstants.emplace( result, type );
		}

		return it->second;
	}

	void Module::registerExtension( std::string const & name )
	{
		extensions.push_back( makeInstruction< ExtensionInstruction >( name ) );
	}

	void Module::registerEntryPoint( spv::Id functionId
		, std::string const & name
		, IdList const & inputs
		, IdList const & outputs )
	{
		IdList operands;
		operands.push_back( spv::Id( m_model ) );
		operands.push_back( functionId );
		operands.insert( operands.end(), inputs.begin(), inputs.end() );
		operands.insert( operands.end(), outputs.begin(), outputs.end() );
		entryPoint = makeInstruction< EntryPointInstruction >( operands
			, name );

		switch ( m_model )
		{
		case spv::ExecutionModel::Vertex:
			break;
		case spv::ExecutionModel::TessellationControl:
			break;
		case spv::ExecutionModel::TessellationEvaluation:
			break;
		case spv::ExecutionModel::Geometry:
			break;
		case spv::ExecutionModel::Fragment:
			registerExecutionMode( spv::ExecutionMode::OriginLowerLeft );
			break;
		case spv::ExecutionModel::GLCompute:
			break;
		case spv::ExecutionModel::Kernel:
			break;
		case spv::ExecutionModel::Max:
			break;
		default:
			break;
		}

		for ( auto & executionMode : m_pendingExecutionModes )
		{
			executionModes.emplace_back( std::move( executionMode ) );
			executionModes.back()->operands[0u] = functionId;
		}

		m_pendingExecutionModes.clear();
	}

	void Module::registerExecutionMode( spv::ExecutionMode mode )
	{
		registerExecutionMode( mode, {} );
	}

	void Module::registerExecutionMode( spv::ExecutionMode mode, IdList const & operands )
	{
		if ( !entryPoint || entryPoint->operands.empty() )
		{
			IdList ops;
			ops.push_back( spv::Id( 0u ) );
			ops.push_back( spv::Id( mode ) );
			ops.insert( ops.end(), operands.begin(), operands.end() );
			m_pendingExecutionModes.push_back( makeInstruction< ExecutionModeInstruction >( ops ) );
		}
		else
		{
			IdList ops;
			ops.push_back( spv::Id( entryPoint->operands[1] ) );
			ops.push_back( spv::Id( mode ) );
			ops.insert( ops.end(), operands.begin(), operands.end() );
			executionModes.push_back( makeInstruction< ExecutionModeInstruction >( ops ) );
		}
	}

	spv::Id Module::getIntermediateResult()
	{
		spv::Id result{};

		if ( m_freeIntermediates.empty() )
		{
			result = ++*m_currentId;
			m_intermediates.insert( result );
			m_freeIntermediates.insert( result );
		}

		result = *m_freeIntermediates.begin();
		m_freeIntermediates.erase( m_freeIntermediates.begin() );
		return result;
	}

	void Module::lnkIntermediateResult( spv::Id intermediate, spv::Id var )
	{
		if ( m_intermediates.end() != m_intermediates.find( intermediate ) )
		{
			m_busyIntermediates.emplace( intermediate, var );
		}
	}

	void Module::putIntermediateResult( spv::Id id )
	{
		//if ( m_intermediates.end() != m_intermediates.find( id ) )
		//{
		//	m_freeIntermediates.insert( id );
		//	auto it = m_busyIntermediates.begin();

		//	while ( it != m_busyIntermediates.end() )
		//	{
		//		if ( it->first == id
		//			|| it->second == id )
		//		{
		//			it = m_busyIntermediates.erase( it );
		//		}
		//		else
		//		{
		//			++it;
		//		}
		//	}
		//}
	}

	spv::Id Module::getNonIntermediate( spv::Id id )
	{
		while ( m_intermediates.end() != m_intermediates.find( id ) )
		{
			id = m_busyIntermediates.find( id )->second;
		}

		return id;
	}

	Function * Module::beginFunction( std::string const & name
		, spv::Id retType
		, ast::var::VariableList const & params )
	{
		IdList funcTypes;
		IdList funcParams;
		funcTypes.push_back( retType );

		for ( auto & param : params )
		{
			funcTypes.push_back( registerType( param->getType() ) );
			spv::Id paramId{ ++*m_currentId };
			funcParams.push_back( paramId );
			debug.push_back( makeInstruction< NameInstruction >( paramId, param->getName() ) );
			m_registeredVariables.emplace( param->getName(), funcParams.back() );
			m_registeredVariablesTypes.emplace( funcParams.back(), funcTypes.back() );
		}

		spv::Id funcType{ ++*m_currentId };
		globalDeclarations.push_back( makeInstruction< FunctionTypeInstruction >( funcType
			, funcTypes ) );
		spv::Id result{ ++*m_currentId };
		InstructionList declaration;
		declaration.emplace_back( makeInstruction< FunctionInstruction >( retType
			, result
			, spv::Id( spv::FunctionControlMask::MaskNone )
			, funcType ) );
		auto itType = funcTypes.begin() + 1u;
		auto itParam = funcParams.begin();

		for ( auto & param : params )
		{
			declaration.emplace_back( makeInstruction< FunctionParameterInstruction >( *itType
				, *itParam ) );
			++itType;
			++itParam;
		}

		m_registeredVariables.emplace( name, result );
		m_registeredVariablesTypes.emplace( result, funcType );
		functions.emplace_back( Function
			{
				std::move( declaration ),
				{},
				{},
				m_currentId,
			} );
		debug.push_back( makeInstruction< NameInstruction >( result, name ) );
		m_currentFunction = &functions.back();
		variables = &m_currentFunction->variables;

		return m_currentFunction;
	}

	Block Module::newBlock()
	{
		Block result
		{
			++*m_currentId
		};
		result.instructions.push_back( makeInstruction< LabelInstruction >( result.label ) );
		return result;
	}

	void Module::endFunction()
	{
		if ( m_currentFunction
			&& !m_currentFunction->cfg.blocks.empty()
			&& !m_currentFunction->variables.empty() )
		{
			auto & instructions = m_currentFunction->cfg.blocks.begin()->instructions;
			auto variables = std::move( m_currentFunction->variables );
			std::reverse( variables.begin(), variables.end() );

			for ( auto & variable : variables )
			{
				instructions.emplace( instructions.begin() + 1u
					, std::move( variable ) );
			}

			m_currentFunction->variables.clear();
		}

		variables = &globalDeclarations;
		m_currentFunction = nullptr;
	}

	spv::Id Module::doRegisterNonArrayType( ast::type::TypePtr type
		, uint32_t mbrIndex
		, spv::Id parentId )
	{
		spv::Id result;

		auto unqualifiedType = getUnqualifiedType( type );
		auto it = m_registeredTypes.find( unqualifiedType );

		if ( it == m_registeredTypes.end() )
		{
			result = registerBaseType( unqualifiedType
				, mbrIndex
				, parentId
				, 0u );
			m_registeredTypes.emplace( unqualifiedType, result );
		}
		else
		{
			result = it->second;
		}

		return result;
	}

	spv::Id Module::registerType( ast::type::TypePtr type
		, uint32_t mbrIndex
		, spv::Id parentId
		, uint32_t arrayStride )
	{
		spv::Id result;

		if ( type->getKind() == ast::type::Kind::eArray )
		{
			auto arrayedType = std::static_pointer_cast< ast::type::Array >( type )->getType();
			auto arraySize = getArraySize( type );
			auto elementTypeId = registerType( arrayedType
				, mbrIndex
				, parentId
				, arrayStride );

			auto unqualifiedType = getUnqualifiedType( type );
			auto it = m_registeredTypes.find( unqualifiedType );

			if ( it == m_registeredTypes.end() )
			{
				if ( arraySize != ast::type::UnknownArraySize )
				{
					auto lengthId = registerLiteral( arraySize );
					result = ++*m_currentId;
					globalDeclarations.push_back( makeInstruction< ArrayTypeInstruction >( result
						, elementTypeId
						, lengthId ) );
				}
				else
				{
					result = ++*m_currentId;
					globalDeclarations.push_back( makeInstruction< RuntimeArrayTypeInstruction >( result
						, elementTypeId ) );
				}

				writeArrayStride( *this
					, unqualifiedType
					, result
					, arrayStride );
				m_registeredTypes.emplace( unqualifiedType, result );
			}
			else
			{
				result = it->second;
			}
		}
		else
		{
			result = doRegisterNonArrayType( type
				, mbrIndex
				, parentId );
		}

		return result;
	}

	spv::Id Module::registerBaseType( ast::type::Kind kind
		, uint32_t mbrIndex
		, spv::Id parentId
		, uint32_t arrayStride )
	{
		assert( kind != ast::type::Kind::eStruct );
		assert( kind != ast::type::Kind::eImage );
		assert( kind != ast::type::Kind::eSampledImage );

		spv::Id result{};
		auto type = m_cache.makeType( kind );

		if ( isVectorType( kind )
			|| isMatrixType( kind ) )
		{
			auto componentType = registerType( m_cache.makeType( getComponentType( kind ) ) );
			result = ++*m_currentId;

			if ( isMatrixType( kind ) )
			{
				globalDeclarations.push_back( makeInstruction< MatrixTypeInstruction >( result
					, componentType
					, getComponentCount( kind ) ) );

				if ( mbrIndex != ast::type::NotMember )
				{
					decorateMember( parentId
						, mbrIndex
						, spv::Decoration::ColMajor );
				}
				else
				{
					decorate( result
						, spv::Decoration::ColMajor );
				}
			}
			else
			{
				globalDeclarations.push_back( makeInstruction< VectorTypeInstruction >( result
					, componentType
					, getComponentCount( kind ) ) );
			}
		}
		else
		{
			result = ++*m_currentId;
			globalDeclarations.push_back( makeBaseTypeInstruction( kind, result ) );
		}

		return result;
	}

	spv::Id Module::registerBaseType( ast::type::SampledImagePtr type
		, uint32_t mbrIndex
		, spv::Id parentId )
	{
		auto imgTypeId = registerType( std::static_pointer_cast< ast::type::SampledImage >( type )->getImageType() );
		auto result = ++*m_currentId;
		globalDeclarations.push_back( makeInstruction< SampledImageTypeInstruction >( result
			, imgTypeId ) );
		return result;
	}

	spv::Id Module::registerBaseType( ast::type::ImagePtr type
		, uint32_t mbrIndex
		, spv::Id parent )
	{
		// The Sampled Type.
		auto sampledTypeId = registerType( m_cache.makeType( type->getConfig().sampledType ) );
		// The Image Type.
		auto result = ++*m_currentId;
		globalDeclarations.push_back( makeImageTypeInstruction( type->getConfig(), result, sampledTypeId ) );
		return result;
	}

	spv::Id Module::registerBaseType( ast::type::StructPtr type
		, uint32_t
		, spv::Id )
	{
		spv::Id result{ ++*m_currentId };
		IdList subTypes;

		for ( auto & member : *type )
		{
			subTypes.push_back( registerType( member.type
				, member.type->getIndex()
				, result
				, member.arrayStride ) );
		}

		globalDeclarations.push_back( makeInstruction< StructTypeInstruction >( result, subTypes ) );
		debug.push_back( makeInstruction< NameInstruction >( result, type->getName() ) );

		for ( auto & member : *type )
		{
			auto index = member.type->getIndex();
			debug.push_back( makeInstruction< MemberNameInstruction >( result, index, member.name ) );
			decorateMember( result
				, index
				, makeOperands( uint32_t( spv::Decoration::Offset ), member.offset ) );

			if ( isMatrixType( member.type->getKind() ) )
			{
				auto colType = getComponentType( member.type->getKind() );
				auto size = getSize( *member.type->getCache().makeType( colType )
					, ast::type::MemoryLayout::eStd430 );
				decorateMember( result
					, index
					, makeOperands( uint32_t( spv::Decoration::MatrixStride ), size ) );
			}
		}

		return result;
	}

	spv::Id Module::registerBaseType( ast::type::TypePtr type
		, uint32_t mbrIndex
		, spv::Id parentId
		, uint32_t arrayStride )
	{
		spv::Id result{};

		if ( type->getKind() == ast::type::Kind::eArray )
		{
			type = std::static_pointer_cast< ast::type::Array >( type )->getType();
		}

		auto kind = type->getKind();

		if ( kind == ast::type::Kind::eSampledImage )
		{
			result = registerBaseType( std::static_pointer_cast< ast::type::SampledImage >( type )
				, mbrIndex
				, parentId );
		}
		else if ( kind == ast::type::Kind::eImage )
		{
			result = registerBaseType( std::static_pointer_cast< ast::type::Image >( type )
				, mbrIndex
				, parentId );
		}
		else if ( kind == ast::type::Kind::eStruct )
		{
			result = registerBaseType( std::static_pointer_cast< ast::type::Struct >( type )
				, mbrIndex
				, parentId );
		}
		else
		{
			result = registerBaseType( kind
				, mbrIndex
				, parentId
				, arrayStride );
		}

		return result;
	}

	//*************************************************************************
}
