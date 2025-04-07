/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once

// External Includes
#include <nap/resourceptr.h>
#include <parametervec.h>

// Local includes
#include "warpcomponent.h"

namespace nap
{
    // Forward Declares
    class UpdateHomographyComponentInstance;
	class WarpComponentInstance;

	/**
	 * UpdateHomographyComponent
	 */
    class NAPAPI UpdateHomographyComponent : public Component
    {
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(UpdateHomographyComponent, UpdateHomographyComponentInstance)
    public:
		void getDependentComponents(std::vector<rtti::TypeInfo>& components) const override;

		ResourcePtr<ParameterVec2> mTopLeft;
		ResourcePtr<ParameterVec2> mTopRight;
		ResourcePtr<ParameterVec2> mBottomRight;
		ResourcePtr<ParameterVec2> mBottomLeft;
	};

	/**
	 * UpdateHomographyComponentInstance
	 */
    class NAPAPI UpdateHomographyComponentInstance : public ComponentInstance
    {
		RTTI_ENABLE(ComponentInstance)
    public:
        UpdateHomographyComponentInstance(EntityInstance& entity, Component& resource) :
			ComponentInstance(entity, resource) { }

        virtual bool init(utility::ErrorState& errorState) override;

        virtual void update(double deltaTime) override;

		UpdateHomographyComponent* mResource = nullptr;
		WarpComponentInstance* mWarper = nullptr;
	};
}
