/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "updatehomographycomponent.h"

// External Includes
#include <entity.h>
#include <rtti/typeinfo.h>

RTTI_BEGIN_CLASS(nap::UpdateHomographyComponent)
	RTTI_PROPERTY("TopLeft",		&nap::UpdateHomographyComponent::mTopLeft,		nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("TopRight",		&nap::UpdateHomographyComponent::mTopRight,		nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("BottomLeft",		&nap::UpdateHomographyComponent::mBottomLeft,	nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("BottomRight",	&nap::UpdateHomographyComponent::mBottomRight, 	nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::UpdateHomographyComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

//////////////////////////////////////////////////////////////////////////


namespace nap
{
	//////////////////////////////////////////////////////////////////////////
	// UpdateHomographyComponent
	//////////////////////////////////////////////////////////////////////////

	void UpdateHomographyComponent::getDependentComponents(std::vector<rtti::TypeInfo>& components) const
	{
		components.emplace_back(RTTI_OF(WarpComponent));
	}


	//////////////////////////////////////////////////////////////////////////
	// UpdateHomographyComponentInstance
	//////////////////////////////////////////////////////////////////////////

    bool UpdateHomographyComponentInstance::init(utility::ErrorState& errorState)
    {
		mResource = getComponent<UpdateHomographyComponent>();

		// Ensure there is a transform component
		mHomographyComponent = getEntityInstance()->findComponent<WarpComponentInstance>();
		if (!errorState.check(mHomographyComponent != nullptr, "%s: missing homography component", mID.c_str()))
			return false;

        return true;
    }


    void UpdateHomographyComponentInstance::update(double deltaTime)
    {
		mHomographyComponent->setTopLeft(mResource->mTopLeft->mValue);
		mHomographyComponent->setTopRight(mResource->mTopRight->mValue);
		mHomographyComponent->setBottomLeft(mResource->mBottomLeft->mValue);
		mHomographyComponent->setBottomRight(mResource->mBottomRight->mValue);
    }
}
