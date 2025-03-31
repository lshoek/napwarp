// Local Includes
#include "warpapp.h"

// External Includes
#include <inputrouter.h>
#include <rendercomponent.h>
#include <orthocameracomponent.h>

namespace nap 
{    
    bool nap::WarpApp::init(utility::ErrorState& error)
    {
		// Retrieve services
		mRenderService	= getCore().getService<nap::RenderService>();
		mSceneService	= getCore().getService<nap::SceneService>();
		mInputService	= getCore().getService<nap::InputService>();
		mGuiService		= getCore().getService<nap::IMGuiService>();

		// Fetch the resource manager
        mResourceManager = getCore().getResourceManager();

		// Get the render window
		mRenderWindow = mResourceManager->findObject<nap::RenderWindow>("Window");
		if (!error.check(mRenderWindow != nullptr, "unable to find render window with name: %s", "Window"))
			return false;

		// Get the scene that contains our entities and components
		mScene = mResourceManager->findObject<Scene>("Scene");
		if (!error.check(mScene != nullptr, "unable to find scene with name: %s", "Scene"))
			return false;

		// Get the scene that contains our entities and components
		mParameterGUI = mResourceManager->findObject<ParameterGUI>("ParameterGUI");
		if (!error.check(mParameterGUI != nullptr, "unable to find scene with name: %s", "ParameterGUI"))
			return false;

		// Get the camera entity
		mCameraEntity = mScene->findEntity("CameraEntity");
		if (!error.check(mCameraEntity != nullptr, "unable to find entity with name: %s", "CameraEntity"))
			return false;

		// Get the warp entity
		mWarpEntity = mScene->findEntity("WarpEntity");
		if (!error.check(mWarpEntity != nullptr, "unable to find entity with name: %s", "WarpEntity"))
			return false;

		// All done!
        return true;
    }


    // Render app
    void WarpApp::render()
    {
		// Signal the beginning of a new frame, allowing it to be recorded.
		// The system might wait until all commands that were previously associated with the new frame have been processed on the GPU.
		// Multiple frames are in flight at the same time, but if the graphics load is heavy the system might wait here to ensure resources are available.
		mRenderService->beginFrame();

		// Begin recording the render commands for the main render window
		if (mRenderService->beginRecording(*mRenderWindow))
		{
			// Begin render pass
			mRenderWindow->beginRendering();

			// Get camera to render with
			auto& cam = mCameraEntity->getComponent<OrthoCameraComponentInstance>();

			// Add Gnomon
			std::vector<RenderableComponentInstance*> comps;
			mWarpEntity->getComponentsOfTypeRecursive(comps);

			// Render components
			mRenderService->renderObjects(*mRenderWindow, cam, comps);

			// Draw GUI elements
			mGuiService->draw();

			// Stop render pass
			mRenderWindow->endRendering();

			// End recording
			mRenderService->endRecording();
		}

		// Proceed to next frame
		mRenderService->endFrame();
    }


    void WarpApp::windowMessageReceived(WindowEventPtr windowEvent)
    {
		mRenderService->addEvent(std::move(windowEvent));
    }


    void WarpApp::inputMessageReceived(InputEventPtr inputEvent)
    {
		// If we pressed escape, quit the loop
		if (inputEvent->get_type().is_derived_from(RTTI_OF(nap::KeyPressEvent)))
		{
			auto* event = static_cast<nap::KeyPressEvent*>(inputEvent.get());
			if (event->mKey == nap::EKeyCode::KEY_ESCAPE)
				quit();

			else if (event->mKey == nap::EKeyCode::KEY_f)
				mRenderWindow->toggleFullscreen();

			else if (event->mKey == nap::EKeyCode::KEY_h)
				mHideGUI = !mHideGUI;
		}
		mInputService->addEvent(std::move(inputEvent));
    }


    int WarpApp::shutdown()
    {
		return 0;
    }


    void WarpApp::update(double deltaTime)
    {
		// Use a default input router to forward input events (recursively) to all input components in the scene
		// This is explicit because we don't know what entity should handle the events from a specific window.
		nap::DefaultInputRouter input_router(true);
		mInputService->processWindowEvents(*mRenderWindow, input_router, { &mScene->getRootEntity() });

		// Record parameter GUI
		if (!mHideGUI)
			mParameterGUI->show(false);
    }
}
