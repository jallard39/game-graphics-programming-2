#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "BufferStructs.h"

#include <DirectXMath.h>

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Called once per program, after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
void Game::Initialize()
{
	// Initialize ray tracing
	RayTracing::Initialize(
		Window::Width(),
		Window::Height(),
		FixPath(L"RayTracing.cso"));

	// Set up the viewport and scissor rectangle
	{
		// Set up the viewport so we render into the correct
		// portion of the render target
		viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)Window::Width();
		viewport.Height = (float)Window::Height();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		// Define a scissor rectangle that defines a portion of
		// the render target for clipping.  This is different from
		// a viewport in that it is applied after the pixel shader.
		// We need at least one of these, but we're rendering to 
		// the entire window, so it'll be the same size.
		scissorRect = {};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = Window::Width();
		scissorRect.bottom = Window::Height();
	}
	
	CreateCameras();
	CreateGeometry();
	CreateMaterials();
	CreateEntities();
	CreateLights();

	// Create a TLAS for our "scene"
	RayTracing::CreateTopLevelAccelerationStructureForScene(entities);

	// Finalize any initialization and wait for the GPU before
	// proceeding to the game loop
	Graphics::CloseAndExecuteCommandList();
	Graphics::WaitForGPU();
	Graphics::ResetAllocatorAndCommandList();
}


// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// Wait for the GPU before we shut down
	Graphics::WaitForGPU();

}


// --------------------------------------------------------
// Set up all the cameras in the scene and set initial active camera
// --------------------------------------------------------
void Game::CreateCameras()
{
	// Initialize camera field
	DirectX::XMFLOAT3 camInitPos = { 0.04f, 0.0f, -3.92f };
	DirectX::XMFLOAT3 camInitRot = { 0.0f, 0.0f, -1.0f };
	cameras.push_back(std::make_shared<Camera>((float)Window::Width() / (float)Window::Height(), camInitPos, camInitRot, (float)XM_PI / 3));

	activeCameraIndex = 0;
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Load meshes
	meshes.push_back(std::make_shared<Mesh>("Cube", FixPath(L"../../Assets/Models/cube.obj").c_str()));
	meshes.push_back(std::make_shared<Mesh>("Cylinder", FixPath(L"../../Assets/Models/cylinder.obj").c_str()));
	meshes.push_back(std::make_shared<Mesh>("Helix", FixPath(L"../../Assets/Models/helix.obj").c_str()));
	meshes.push_back(std::make_shared<Mesh>("Quad", FixPath(L"../../Assets/Models/quad.obj").c_str()));
	meshes.push_back(std::make_shared<Mesh>("Quad_Double_Sided", FixPath(L"../../Assets/Models/quad_double_sided.obj").c_str()));
	meshes.push_back(std::make_shared<Mesh>("Sphere", FixPath(L"../../Assets/Models/sphere.obj").c_str()));
	meshes.push_back(std::make_shared<Mesh>("Torus", FixPath(L"../../Assets/Models/torus.obj").c_str()));
}

// --------------------------------------------------------
// Creates the materials for the game objects
// --------------------------------------------------------
void Game::CreateMaterials()
{
	materials.push_back(std::make_shared<Material>(1.0, 0.0, 0.0));
	/*for (int i = 0; i < 10; i++)
	{
		materials.push_back(std::make_shared<Material>(
			(float)(rand()) / (float)(RAND_MAX), 
			(float)(rand()) / (float)(RAND_MAX), 
			(float)(rand()) / (float)(RAND_MAX)));
	} */
}


// --------------------------------------------------------
// Creates the GameEntities that will be drawn to the screen
// --------------------------------------------------------
void Game::CreateEntities()
{
	entities.push_back(std::make_shared<GameEntity>(meshes[0], materials[0])); // Cube
	entities[0]->GetTransform()->SetPosition(-2.0f, -0.3f, 1.0f);
	entities[0]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);

	entities.push_back(std::make_shared<GameEntity>(meshes[5], materials[0])); // Sphere
	entities[1]->GetTransform()->SetPosition(0.0f, 0.5f, 0.0f);
	entities[1]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);

	entities.push_back(std::make_shared<GameEntity>(meshes[2], materials[0])); // Helix
	entities[2]->GetTransform()->SetPosition(2.0f, 0.2f, 0.0f);
	entities[2]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);
}


// --------------------------------------------------------
// Creates all the lights in the scene
// --------------------------------------------------------
void Game::CreateLights()
{
	// Main directional
	lights.push_back(Light{});
	lights[0].Direction = XMFLOAT3(0.0f, -2.0f, 1.0f);
	lights[0].Color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	lights[0].Intensity = 1.0f;
	lightCount++;

	//// Added directional
	//lights.push_back(Light{});
	//lights[1].Direction = XMFLOAT3(0.0f, -1.0f, 0.3f);
	//lights[1].Color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//lights[1].Intensity = 1.0f;
	//lightCount++;

	//// Added directional
	//lights.push_back(Light{});
	//lights[2].Direction = XMFLOAT3(1.0f, 1.0f, 2.3f);
	//lights[2].Color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//lights[2].Intensity = 1.0f;
	//lightCount++;
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Resize the viewport and scissor rectangle
	{
		// Set up the viewport so we render into the correct
		// portion of the render target
		viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)Window::Width();
		viewport.Height = (float)Window::Height();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		// Define a scissor rectangle that defines a portion of
		// the render target for clipping.  This is different from
		// a viewport in that it is applied after the pixel shader.
		// We need at least one of these, but we're rendering to 
		// the entire window, so it'll be the same size.
		scissorRect = {};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = Window::Width();
		scissorRect.bottom = Window::Height();

		// Resize raytracing output texture
		RayTracing::ResizeOutputUAV(Window::Width(), Window::Height());

		// Update cameras
		for (int i = 0; i < cameras.size(); i++) {
			cameras[i]->UpdateProjectionMatrix((float)Window::Width() / (float)Window::Height());
		}
	}
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();

	// Move objects
	entities[0]->GetTransform()->MoveAbsolute(0.0f, 0.0006f * cosf(totalTime + ((4*XM_PI)/3)), 0.0f);
	entities[1]->GetTransform()->MoveAbsolute(0.0f, 0.0006f * cosf(totalTime + ((2*XM_PI)/3)), 0.0f);
	entities[2]->GetTransform()->MoveAbsolute(0.0f, 0.0006f * cosf(totalTime), 0.0f);

	entities[0]->GetTransform()->Rotate(0.0f, 0.0f, -1.0f * deltaTime);

	// Update cameras
	cameras[activeCameraIndex]->Update(deltaTime);
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Grab the current back buffer for this frame
	Microsoft::WRL::ComPtr<ID3D12Resource> currentBackBuffer =
		Graphics::BackBuffers[Graphics::SwapChainIndex()];

	// Update TLAS to contain the latest transformations from this frame
	//RayTracing::CreateTopLevelAccelerationStructureForScene(entities);

	// Perform ray trace (which also copies the results to the back buffer)
	RayTracing::Raytrace(cameras[activeCameraIndex], currentBackBuffer);

	// Present
	{
		// Must occur BEFORE present
		Graphics::CloseAndExecuteCommandList();

		// Present the current back buffer and move to the next one
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);
		Graphics::AdvanceSwapChainIndex();

		// Wait for the GPU to be done and then reset the command list & allocator
		Graphics::WaitForGPU();
		Graphics::ResetAllocatorAndCommandList();
	}

}



