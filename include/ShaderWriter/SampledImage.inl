/*
See LICENSE file in root folder
*/
namespace sdw
{
	//*************************************************************************

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::eBuffer, false >
	{
		using FetchType = sdw::Int;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::e1D, false >
	{
		using QueryLodType = sdw::Float;
		using SampleType = sdw::Float;
		using ProjType = sdw::Vec2;
		using OffsetType = sdw::Int;
		using FetchType = sdw::Int;
		using DerivativeType = sdw::Float;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::e2D, false >
	{
		using QueryLodType = sdw::Vec2;
		using SampleType = sdw::Vec2;
		using ProjType = sdw::Vec3;
		using OffsetType = sdw::IVec2;
		using FetchType = sdw::IVec2;
		using DerivativeType = sdw::Vec2;
		using GatherType = sdw::Vec2;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::e3D, false >
	{
		using QueryLodType = sdw::Vec3;
		using SampleType = sdw::Vec3;
		using ProjType = sdw::Vec4;
		using OffsetType = sdw::IVec3;
		using FetchType = sdw::IVec3;
		using DerivativeType = sdw::Vec3;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::eCube, false >
	{
		using QueryLodType = sdw::Vec3;
		using SampleType = sdw::Vec3;
		using DerivativeType = sdw::Vec3;
		using GatherType = sdw::Vec3;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::eRect, false >
	{
		using SampleType = sdw::Vec2;
		using ProjType = sdw::Vec3;
		using OffsetType = sdw::IVec2;
		using FetchType = sdw::IVec2;
		using DerivativeType = sdw::Vec2;
		using GatherType = sdw::Vec3;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::e1D, true >
	{
		using QueryLodType = sdw::Float;
		using SampleType = sdw::Vec2;
		using OffsetType = sdw::Int;
		using FetchType = sdw::IVec2;
		using DerivativeType = sdw::Float;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::e2D, true >
	{
		using QueryLodType = sdw::Vec2;
		using SampleType = sdw::Vec3;
		using OffsetType = sdw::IVec2;
		using FetchType = sdw::IVec3;
		using DerivativeType = sdw::Vec2;
		using GatherType = sdw::Vec3;
	};

	template<>
	struct SampledImageCoordsGetter< ast::type::ImageDim::eCube, true >
	{
		using QueryLodType = sdw::Vec3;
		using SampleType = sdw::Vec4;
		using DerivativeType = sdw::Vec3;
		using GatherType = sdw::Vec4;
	};

	//*************************************************************************

	template< typename T >
	SampledImage & SampledImage::operator=( T const & rhs )
	{
		this->updateContainer( rhs );
		addStmt( *findContainer( *this, rhs )
			, sdw::makeSimple( sdw::makeAssign( m_expr->getType()
				, makeExpr( m_expr )
				, makeExpr( rhs ) ) ) );
		return *this;
	}

	//*************************************************************************

	template< ast::type::Kind SampledT
		, ast::type::ImageDim DimT
		, bool ArrayedT
		, bool DepthT
		, bool MsT >
	SampledImageT< SampledT, DimT, ArrayedT, DepthT, MsT >::SampledImageT( Shader * shader
		, expr::ExprPtr expr
		, ast::type::ImageFormat format )
		: SampledImage{ shader, std::move( expr ), format }
	{
	}

	template< ast::type::Kind SampledT
		, ast::type::ImageDim DimT
		, bool ArrayedT
		, bool DepthT
		, bool MsT >
	template< typename T >
	SampledImageT< SampledT, DimT, ArrayedT, DepthT, MsT > & SampledImageT< SampledT, DimT, ArrayedT, DepthT, MsT >::operator=( T const & rhs )
	{
		this->updateContainer( rhs );
		addStmt( *findContainer( *this, rhs )
			, sdw::makeSimple( sdw::makeAssign( m_expr->getType()
				, makeExpr( m_expr )
				, makeExpr( rhs ) ) ) );
		return *this;
	}

	template< ast::type::Kind SampledT
		, ast::type::ImageDim DimT
		, bool ArrayedT
		, bool DepthT
		, bool MsT >
	SampledImageT< SampledT, DimT, ArrayedT, DepthT, MsT >::operator uint32_t()
	{
		return 0u;
	}

	template< ast::type::Kind SampledT
		, ast::type::ImageDim DimT
		, bool ArrayedT
		, bool DepthT
		, bool MsT >
	ast::type::ImageConfiguration SampledImageT< SampledT, DimT, ArrayedT, DepthT, MsT >::makeConfig( ast::type::ImageFormat format )
	{
		return ast::type::makeConfig< SampledT, DimT, ArrayedT, DepthT, MsT >( format, true, true );
	}

	//*************************************************************************
}