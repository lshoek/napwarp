/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once

// External Includes
#include <shader.h>
#include <warpservice.h>

namespace nap
{
	// Forward declares
	class Core;
	class RenderService;

	// Video shader sampler names
	namespace uniform
	{
		namespace homography
		{
			inline constexpr const char* homographyMatrix = "homographyMatrix";	///< Name of the homography matrix

			namespace sampler
			{
				inline constexpr const char* colorTexture = "colorTexture";		///< Name of the color texture sampler
			}
		}
	}

	/**
	 * Homography shader.
	 *
	 * The homography shader exposes the following shader variables:
	 *
	 * ~~~~{.vert}
	 *		uniform UBO
	 *		{
	 *			uniform mat4 homographyMatrix;
	 *		} ubo;
	 * ~~~~
	 *
	 * ~~~~{.frag}
	 *		uniform sampler2D colorTexture;
	 * ~~~~
	 */
	class NAPAPI HomographyShader : public Shader
	{
		RTTI_ENABLE(Shader)
	public:
		HomographyShader(Core& core);

		/**
		 * Cross compiles the constant GLSL shader code to SPIR-V, creates the shader module and parses all the uniforms and samplers.
		 * @param errorState contains the error if initialization fails.
		 * @return if initialization succeeded.
		 */
		virtual bool init(utility::ErrorState& errorState) override;

	private:
		WarpService& mWarpService;
	};
}
