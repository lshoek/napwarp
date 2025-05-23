/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once

// External Includes
#include <nap/resourceptr.h>
#include <componentptr.h>
#include <glm/glm.hpp>
#include <texture.h>

namespace nap
{
    // Forward Declares
    class WarpComponentInstance;

	/**
	 * WarpComponent
	 */
    class NAPAPI WarpComponent : public Component
    {
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(WarpComponent, WarpComponentInstance)
    public:
		glm::vec2 mTopLeft		= { 0.0f, 1080.0f };
		glm::vec2 mTopRight		= { 1920.0f, 1080.0f };
		glm::vec2 mBottomRight	= { 1920.0f, 0.0f };
		glm::vec2 mBottomLeft	= { 0.0f, 0.0f };

		float mSourceWidth		= 1920.0f;
		float mSourceHeight		= 1080.0f;
		ResourcePtr<Texture2D> mReferenceTexture;
    };

	/**
	 * WarpComponentInstance
	 */
    class NAPAPI WarpComponentInstance : public ComponentInstance
    {
		RTTI_ENABLE(ComponentInstance)
    public:
        WarpComponentInstance(EntityInstance& entity, Component& resource) :
			ComponentInstance(entity, resource) { }

		/**
		 * Initialize this component
		 * @param errorState the error state if initialization fails
		 * @return whether initialization succeeded
		 */
        virtual bool init(utility::ErrorState& errorState) override;

		/**
		 * Update this component
		 * @param deltaTime the time in between cooks in seconds
		 */
        virtual void update(double deltaTime) override;

		/**
		 * Sets the top left corner of the quad to warp, in absolute coordinates
		 * @param topLeft the position of the top left corner in absolute coordinates
		 */
        void setTopLeft(const glm::vec2& topLeft)				{ mTopLeft = topLeft; mDirty = true; }

		/**
		 * @return the position of the top left corner in absolute coordinates
		 */
        const glm::vec2& getTopLeft() const						{ return mTopLeft; }

		/**
		 * Sets the top right corner of the quad to warp
		 * @param topRight the position of the top right corner in absolute coordinates
		 */
        void setTopRight(const glm::vec2& topRight)				{ mTopRight = topRight; mDirty = true; }

		/**
		 * @return the position of the top right corner in absolute coordinates
		 */
        const glm::vec2& getTopRight() const					{ return mTopRight; }

		/**
		 * Sets the bottom right corner of the quad to warp, in absolute coordinates
		 * @param bottomRight the position of the bottom right corner in absolute coordinates
		 */
        void setBottomRight(const glm::vec2& bottomRight)		{ mBottomRight = bottomRight; mDirty = true; }

		/**
		 * @return the position of the bottom right corner in absolute coordinates
		 */
        const glm::vec2& getBottomRight() const					{ return mBottomRight; }

		/**
		 * Sets the bottom left corner of the quad to warp, in absolute coordinates
		 * @param bottomLeft the position of the bottom left corner in absolute coordinates
		 */
        void setBottomLeft(const glm::vec2& bottomLeft)			{ mBottomLeft = bottomLeft; mDirty = true; }

		/**
		 * @return the position of the bottom left corner in absolute coordinates
		 */
        const glm::vec2& getBottomLeft() const					{ return mBottomLeft; }

		/**
		 * @return the homography matrix used to warp the image
		 */
		const glm::mat3& getHomographyMatrix() const			{ return mHomographyMatrix; }

		/**
		 * @return the inverse of the homography matrix
		 */
		const glm::mat3& getInverseHomographyMatrix() const		{ return mInverseHomographyMatrix; }

    private:
		glm::vec2 mTopLeft;
		glm::vec2 mTopRight;
		glm::vec2 mBottomRight;
		glm::vec2 mBottomLeft;

		glm::vec2 mSourceSize;

		glm::mat3 mHomographyMatrix;
		glm::mat3 mInverseHomographyMatrix;

		bool mDirty = false;					//< Dirty flag for the homography matrix
	};
}
