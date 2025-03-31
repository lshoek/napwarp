/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

// External Includes
#include <rendercomponent.h>
#include <componentptr.h>
#include <perspcameracomponent.h>
#include <materialinstance.h>
#include <renderablemesh.h>
#include <emptymesh.h>

namespace nap
{
	// Forward Declares
	class RenderWarpTextureInstance;
	class WarpComponentInstance;

	/**
	 * RenderWarpTexture
	 */
	class NAPAPI RenderWarpTexture : public RenderableComponent
	{
		RTTI_ENABLE(RenderableComponent)
		DECLARE_COMPONENT(RenderWarpTexture, RenderWarpTextureInstance)
	public:
		void getDependentComponents(std::vector<rtti::TypeInfo>& components) const override;

		ResourcePtr<Texture2D> mTexture; 		//< Property: 'Texture' The texture to sample
		bool mRenderBorder = false;				//< Property: 'RenderBorder' Whether to render a border around the quad
		bool mRenderGrid = false;				//< Property: 'RenderGrid' Whether to render a grid within the quad
	};


	/**
	 * RenderWarpTextureInstance
	 */
	class NAPAPI RenderWarpTextureInstance : public  RenderableComponentInstance
	{
		RTTI_ENABLE(RenderableComponentInstance)
	public:
		RenderWarpTextureInstance(EntityInstance& entity, Component& resource);

		/**
		 * Initialize RenderWarpTextureInstance based on the RenderWarpTexture resource.
		 * @param errorState should hold the error message when initialization fails
		 * @return if the RenderWarpTextureInstance initialized successfully
		 */
		virtual bool init(utility::ErrorState& errorState) override;

	protected:
		/**
		 * Draws the effect full screen to the currently active render target
		 * @param renderTarget the target to render to.
		 * @param commandBuffer the currently active command buffer.
		 * @param viewMatrix ignored
		 * @param projectionMatrix ignored
		 */
		virtual void onDraw(IRenderTarget& renderTarget, VkCommandBuffer commandBuffer, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

	private:
		RenderService&					mRenderService;						///< Render service

		RenderWarpTexture*				mResource = nullptr;
		WarpComponentInstance*			mWarpComponent = nullptr;

		Material*						mMaterial = nullptr;				///< The Material
		MaterialInstance				mMaterialInstance;					///< The MaterialInstance as created from the resource
		MaterialInstanceResource		mMaterialInstanceResource;			///< The MaterialInstance resource

		ShaderConstant					mBorderConstant;
		ShaderConstant					mGridConstant;
		UniformMat4Instance*			mHomographyMatrixUniform = nullptr; ///< Homography matrix uniform
		Sampler2DInstance*				mColorTextureSampler = nullptr; 	///< Texture sampler

		std::unique_ptr<EmptyMesh>		mEmptyMesh;							///< Empty mesh
	};
}
