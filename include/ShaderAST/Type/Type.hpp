/*
See LICENSE file in root folder
*/
#ifndef ___AST_Type_H___
#define ___AST_Type_H___
#pragma once

#include "ShaderAST/ShaderASTPrerequisites.hpp"

namespace ast::type
{
	static uint32_t constexpr NotMember = ~( 0u );
	enum class Kind
		: uint8_t
	{
		eUndefined,
		eVoid,
		eFunction,
		eStruct,
		eBoolean,
		eInt,
		eUInt,
		eFloat,
		eDouble,
		eVec2B,
		eVec3B,
		eVec4B,
		eVec2I,
		eVec3I,
		eVec4I,
		eVec2U,
		eVec3U,
		eVec4U,
		eVec2F,
		eVec3F,
		eVec4F,
		eVec2D,
		eVec3D,
		eVec4D,
		eMat2x2F,
		eMat2x3F,
		eMat2x4F,
		eMat3x2F,
		eMat3x3F,
		eMat3x4F,
		eMat4x2F,
		eMat4x3F,
		eMat4x4F,
		eMat2x2D,
		eMat2x3D,
		eMat2x4D,
		eMat3x2D,
		eMat3x3D,
		eMat3x4D,
		eMat4x2D,
		eMat4x3D,
		eMat4x4D,
		eConstantsBuffer,
		eShaderBuffer,
		eSampler,
		eImage,
		eSampledImage,
		eArray,
		eCount,
		eHalf,// Internal only, never use this !!!
		eVec2H,// Internal only, never use this !!!
		eVec3H,// Internal only, never use this !!!
		eVec4H,// Internal only, never use this !!!
		eMin = eUndefined,
		eMax = eSampledImage,
	};

	class Type
	{
	private:
		template< typename TypeT
			, typename CreatorT
			, typename HasherT >
		friend class TypeCache;
		friend class Struct;

	public:
		Type( TypesCache * cache
			, Kind kind );
		Type( TypesCache * cache
			, Struct * parent
			, uint32_t index
			, Kind kind );
		Type( Struct & parent
			, uint32_t index
			, Kind kind );
		virtual TypePtr getMemberType( Struct & parent, uint32_t index )const;

		virtual ~Type();

		inline Kind getKind()const
		{
			return m_kind;
		}

		inline TypesCache & getCache()const
		{
			return *m_cache;
		}

		inline bool isMember()const
		{
			return m_index != NotMember;
		}

		inline uint32_t getIndex()const
		{
			return m_index;
		}

		inline Struct * getParent()const
		{
			return m_parent;
		}

	private:
		Kind m_kind;
		TypesCache * m_cache;
		Struct * m_parent;
		uint32_t m_index;
	};

	bool operator==( Type const & lhs, Type const & rhs );

	bool isBoolType( Kind kind );
	bool isUnsignedIntType( Kind kind );
	bool isSignedIntType( Kind kind );
	bool isHalfType( Kind kind );
	bool isFloatType( Kind kind );
	bool isDoubleType( Kind kind );
	bool isScalarType( Kind kind );
	bool isVectorType( Kind kind );
	bool isMatrixType( Kind kind );
	bool isArrayType( Kind kind );
	bool isStructType( Kind kind );
	bool isSamplerType( Kind kind );
	bool isImageType( Kind kind );
	bool isSampledImageType( Kind kind );
	uint32_t getComponentCount( Kind kind );
	/**
	*\remarks
	*	Returns \p kind if it is not a matrix or vector type.
	*/
	Kind getComponentType( Kind kind );
	Kind getScalarType( Kind kind );
	expr::CompositeType getCompositeType( Kind kind );
	Kind getNonArrayKindRec( Type const & type );
	Kind getNonArrayKindRec( TypePtr type );
	Kind getNonArrayKind( Type const & type );
	Kind getNonArrayKind( TypePtr type );
	uint32_t getArraySize( Type const & type );
	uint32_t getArraySize( TypePtr type );
}

#endif
