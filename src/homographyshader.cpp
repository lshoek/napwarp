/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Local includes
#include "homographyshader.h"
#include "renderservice.h"

// External includes
#include <nap/core.h>

// nap::HomographyShader run time class definition
RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::HomographyShader, "Shader program that warps a texture using a homography matrix.")
	RTTI_CONSTRUCTOR(nap::Core&)
RTTI_END_CLASS


//////////////////////////////////////////////////////////////////////////
// HomographyShader
//////////////////////////////////////////////////////////////////////////

namespace nap
{
	namespace shader
	{
		inline constexpr const char* homography = "homography";
	}

	HomographyShader::HomographyShader(Core& core) : Shader(core),
		mWarpService(*core.getService<WarpService>())
	{ }


	bool HomographyShader::init(utility::ErrorState& errorState)
	{
		std::string relative_path = utility::joinPath({ "shaders", utility::appendFileExtension(shader::homography, "vert") });
		const std::string vertex_shader_path = mWarpService.getModule().findAsset(relative_path);
		if (!errorState.check(!vertex_shader_path.empty(), "%s: Unable to find %s vertex shader %s", mWarpService.getModule().getName().c_str(), shader::homography, vertex_shader_path.c_str()))
			return false;

		relative_path = utility::joinPath({ "shaders", utility::appendFileExtension(shader::homography, "frag") });
		const std::string fragment_shader_path = mWarpService.getModule().findAsset(relative_path);
		if (!errorState.check(!vertex_shader_path.empty(), "%s: Unable to find %s fragment shader %s", mWarpService.getModule().getName().c_str(), shader::homography, fragment_shader_path.c_str()))
			return false;

		// Read vert shader file
		std::string vert_source;
		if (!errorState.check(utility::readFileToString(vertex_shader_path, vert_source, errorState), "Unable to read %s vertex shader file", shader::homography))
			return false;

		// Read frag shader file
		std::string frag_source;
		if (!errorState.check(utility::readFileToString(fragment_shader_path, frag_source, errorState), "Unable to read %s fragment shader file", shader::homography))
			return false;

		// Compile shader
		return load(shader::homography, {}, vert_source.data(), vert_source.size(), frag_source.data(), frag_source.size(), errorState);
	}
}
