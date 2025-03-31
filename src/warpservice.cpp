/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Local Includes
#include "warpservice.h"

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::WarpService)
	RTTI_CONSTRUCTOR(nap::ServiceConfiguration*)
RTTI_END_CLASS

namespace nap
{
	WarpService::WarpService(ServiceConfiguration* configuration) :
		Service(configuration)
	{
	}

	WarpService::~WarpService()
	{
	}


	bool WarpService::init(utility::ErrorState& error)
	{
		return true;
	}
}