
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "warpcomponent.h"

// External Includes
#include <entity.h>
#include <rtti/typeinfo.h>
#include <renderglobals.h>
#include <opencv2/opencv.hpp>
#include <glm/gtc/type_ptr.hpp>

RTTI_BEGIN_CLASS(nap::WarpComponent)
	RTTI_PROPERTY("TopLeft", &nap::WarpComponent::mTopLeft, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("TopRight", &nap::WarpComponent::mTopRight, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("BottomRight", &nap::WarpComponent::mBottomRight, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("BottomLeft", &nap::WarpComponent::mBottomLeft, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("SourceWidth", &nap::WarpComponent::mSourceWidth, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("SourceHeight", &nap::WarpComponent::mSourceHeight, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("ReferenceTexture", &nap::WarpComponent::mReferenceTexture, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::WarpComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

//////////////////////////////////////////////////////////////////////////


namespace nap
{
    bool WarpComponentInstance::init(utility::ErrorState& errorState)
    {
        auto* resource = getComponent<WarpComponent>();
        mTopLeft		= resource->mTopLeft;
        mTopRight		= resource->mTopRight;
        mBottomRight	= resource->mBottomRight;
        mBottomLeft		= resource->mBottomLeft;

		mSourceSize = (resource->mReferenceTexture != nullptr) ?
			resource->mReferenceTexture->getSize() :
			glm::vec2(resource->mSourceWidth, resource->mSourceHeight);

        return true;
    }


	void WarpComponentInstance::update(double deltaTime)
	{
		if (!mDirty)
			return;

		const std::vector<cv::Point2f> source_points =
		{
			{ 0.0f,				mSourceSize.y },
			{ mSourceSize.x,	mSourceSize.y },
			{ mSourceSize.x,	0.0f },
			{ 0.0f,				0.0f }
		};

		// Translated points of projection
		const std::vector<cv::Point2f> dest_points =
		{
			{ mTopLeft.x,		mTopLeft.y },
			{ mTopRight.x,		mTopRight.y },
			{ mBottomRight.x,	mBottomRight.y },
			{ mBottomLeft.x,	mBottomLeft.y }
		};

		// Compute perspective transformation matrix, returns CV_64F (double)
		auto xform_double = cv::findHomography(source_points, dest_points, cv::RANSAC);

		// Convert to 32-bit float
		cv::Mat xform;
		xform_double.convertTo(xform, CV_32F);
		cv::transpose(xform, xform);

		// Copy to glm matrix
		std::memcpy(glm::value_ptr(mHomographyMatrix), xform.data, xform.total() * xform.elemSize());

		// Invert and copy
		cv::invert(xform, xform);
		std::memcpy(glm::value_ptr(mInverseHomographyMatrix), xform.data, xform.total() * xform.elemSize());

		// Reset dirty flag
		mDirty = false;
	}
}
