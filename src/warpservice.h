/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

// External Includes
#include <nap/service.h>

namespace nap
{
	/**
	 * WarpService. Grants access to module-specific data search paths.
	 */
	class NAPAPI WarpService : public Service
	{
		RTTI_ENABLE(Service)
	public:
		/**
		 * Default constructor. This service has no settings associated with it.
		 */
		WarpService(ServiceConfiguration* configuration);
		virtual ~WarpService() override;

	protected:
		/**
		 * Prints all the available network adapters to console.
		 * @param error contains the error if initialization fails.
		 * @return if initialization succeeded.
		 */
		virtual bool init(utility::ErrorState& error) override;
	};
}
