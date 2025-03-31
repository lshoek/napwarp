/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Local Includes
#include "renderwarptexture.h"
#include "warpcomponent.h"
#include "homographyshader.h"

// External Includes
#include <entity.h>
#include <nap/core.h>
#include <rendertarget.h>
#include <renderservice.h>
#include <renderglobals.h>
#include <uniforminstance.h>
#include <textureutils.h>

// nap::RenderWarpTexture run time class definition
RTTI_BEGIN_CLASS(nap::RenderWarpTexture)
	RTTI_PROPERTY("Texture", &nap::RenderWarpTexture::mTexture, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("RenderBorder", &nap::RenderWarpTexture::mRenderBorder, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("RenderGrid", &nap::RenderWarpTexture::mRenderGrid, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

// nap::RenderWarpTextureInstance run time class definition
RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::RenderWarpTextureInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

//////////////////////////////////////////////////////////////////////////


namespace nap
{
	//////////////////////////////////////////////////////////////////////////
	// RenderWarpTexture
	//////////////////////////////////////////////////////////////////////////

	void RenderWarpTexture::getDependentComponents(std::vector<rtti::TypeInfo>& components) const
	{
		components.emplace_back(RTTI_OF(WarpComponent));
	}


	//////////////////////////////////////////////////////////////////////////
	// RenderWarpTextureInstance
	//////////////////////////////////////////////////////////////////////////

	RenderWarpTextureInstance::RenderWarpTextureInstance(EntityInstance& entity, Component& resource) :
		RenderableComponentInstance(entity, resource),
		mRenderService(*entity.getCore()->getService<RenderService>()),
		mEmptyMesh(std::make_unique<EmptyMesh>(*entity.getCore()))
	{ }


	bool RenderWarpTextureInstance::init(utility::ErrorState& errorState)
	{
		if (!RenderableComponentInstance::init(errorState))
			return false;

		// Get resource
		mResource = getComponent<RenderWarpTexture>();

		// Get homography component
		mWarpComponent = getEntityInstance()->findComponent<WarpComponentInstance>();
		if (!errorState.check(mWarpComponent != nullptr, "%s: missing warp component", mID.c_str()))
			return false;

		// Create empty mesh
		if (!mEmptyMesh->init(errorState))
			return false;

		// Initialize material based on resource
		mMaterial = mRenderService.getOrCreateMaterial<HomographyShader>(errorState); assert(mMaterial != nullptr);
		if (!errorState.check(mMaterial != nullptr, "%s: unable to get or create homography material", mResource->mID.c_str()))
			return false;

		mBorderConstant.mName = "BORDER";
		mBorderConstant.mValue = mResource->mRenderBorder ? 1 : 0;
		mMaterialInstanceResource.mConstants.emplace_back(&mBorderConstant);

		mGridConstant.mName = "GRID";
		mGridConstant.mValue = mResource->mRenderGrid ? 1 : 0;
		mMaterialInstanceResource.mConstants.emplace_back(&mGridConstant);

		mMaterialInstanceResource.mMaterial = mMaterial;
		mMaterialInstanceResource.mBlendMode = EBlendMode::Opaque;
		mMaterialInstanceResource.mDepthMode = EDepthMode::NoReadWrite;
		if (!mMaterialInstance.init(mRenderService, mMaterialInstanceResource, errorState))
			return false;

		// Get UBO struct
		auto* ubo = mMaterialInstance.getOrCreateUniform("UBO");
		if (!errorState.check(ubo != nullptr, "%s: Unable to find uniform UBO struct: %s in material: %s", mID.c_str(), "UBO", mMaterialInstance.getMaterial().mID.c_str()))
			return false;

		// Get frame buffer size uniform
		mHomographyMatrixUniform = ubo->getOrCreateUniform<UniformMat4Instance>("homographyMatrix");
		if (!errorState.check(mHomographyMatrixUniform != nullptr, "Missing uniform mat4 'homographyMatrix' in uniform UBO"))
			return false;

		mColorTextureSampler = mMaterialInstance.getOrCreateSampler<Sampler2DInstance>("colorTexture");
		if (!errorState.check(mColorTextureSampler != nullptr, "Missing sampler2D 'colorTexture' in shader"))
			return false;

		if (mResource->mTexture != nullptr)
			mColorTextureSampler->setTexture(*mResource->mTexture);

		return true;
	}


	void RenderWarpTextureInstance::onDraw(IRenderTarget& renderTarget, VkCommandBuffer commandBuffer, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		if (!isVisible())
			return;

		mHomographyMatrixUniform->setValue(mWarpComponent->getInverseHomographyMatrix());

		utility::ErrorState error_state;
		RenderService::Pipeline pipeline = mRenderService.getOrCreatePipeline(renderTarget, *mEmptyMesh, mMaterialInstance, error_state);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.mPipeline);

		const auto& descriptor_set = mMaterialInstance.update();
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.mLayout, 0, 1, &descriptor_set.mSet, 0, nullptr);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
}
