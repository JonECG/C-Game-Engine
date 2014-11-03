#include "OcculusCameraComponent.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Renderer\Shader.h"

#include "Entity.h"
#include "TransformComponent.h"
#include <OVRMAIN\OVR.h>
#include <Src\OVR_CAPI_GL.h>
#include "Stage.h"
#include "Game.h"
#include "Window\Graphics.h"
#include "Renderer\Texture.h"

#include <Renderer\FrameBufferObject.h>
#include <Src\Util\Util_Interface.h>

ovrEyeRenderDesc	EyeRenderDesc[2];
ovrFrameTiming		frameTiming;
ovrRecti			EyeRenderViewport[2];
ovrGLTexture		EyeTexture[2];
ovrPosef			headPoses[2];

FrameBufferObject *renderTarget, *leftEye, *rightEye;

void OcculusCameraComponent::init()
{
	ovr_Initialize();
	parent->getStage()->getGame()->getGraphicsHandle()->setAutoBufferSwap( false );

	hmd = ovrHmd_Create(0);
	if (hmd)
	{
		ovrSizei resolution = hmd->Resolution;
		resolution;
	}
	else
	{
		hmd = ovrHmd_CreateDebug( ovrHmdType::ovrHmd_DK2 );
	}
	// Start the sensor which provides the Rift’s pose and motion. 
	//ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);
	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
	ovrHmd_RecenterPose( hmd );

	ovrFovPort eyeFov[2] = { hmd->DefaultEyeFov[0], hmd->DefaultEyeFov[1] } ;

	ovrGLConfig oglConfig;
	oglConfig.OGL.Header.API         = ovrRenderAPI_OpenGL;
    oglConfig.OGL.Header.RTSize      = OVR::Sizei(hmd->Resolution.w, hmd->Resolution.h);
    oglConfig.OGL.Header.Multisample = 1;
	oglConfig.OGL.Window = parent->getStage()->getGame()->getGraphicsHandle()->getHandle();
	oglConfig.OGL.DC = parent->getStage()->getGame()->getGraphicsHandle()->getHDC();
	#pragma comment(lib,"libovrd.lib")
	ovrBool result = ovrHmd_ConfigureRendering( hmd, &oglConfig.Config,
		                           ovrDistortionCap_Chromatic | ovrDistortionCap_Vignette |
                                   ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive,
								   eyeFov, EyeRenderDesc);

	result;

	ovrHmd_AttachToWindow(hmd, oglConfig.OGL.Window, NULL, NULL);

	//Sets up FBOS
	// Configure Stereo settings. 
	OVR::Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
	OVR::Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);

	OVR::Sizei renderTargetSize;
	renderTargetSize.w = recommenedTex0Size.w + recommenedTex1Size.w;
	renderTargetSize.h = max ( recommenedTex0Size.h, recommenedTex1Size.h );

	renderTarget = FrameBufferObject::createFrameBuffer( renderTargetSize.w, renderTargetSize.h );

	//Set up viewports
	EyeRenderViewport[0].Pos  = OVR::Vector2i(0,0);
	EyeRenderViewport[0].Size = OVR::Sizei(renderTarget->width / 2, renderTarget->height);
    EyeRenderViewport[1].Pos  = OVR::Vector2i((renderTarget->width + 1) / 2, 0);
    EyeRenderViewport[1].Size = EyeRenderViewport[0].Size;

	
	EyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	EyeTexture[0].OGL.Header.TextureSize = renderTargetSize;
	EyeTexture[0].OGL.Header.RenderViewport = EyeRenderViewport[0];
	EyeTexture[0].OGL.TexId = renderTarget->colorTexture->textureID;

	EyeTexture[1].OGL.Header.API = ovrRenderAPI_OpenGL;
	EyeTexture[1].OGL.Header.TextureSize = renderTargetSize;
	EyeTexture[1].OGL.Header.RenderViewport = EyeRenderViewport[1];
	EyeTexture[1].OGL.TexId = renderTarget->colorTexture->textureID;
}
void OcculusCameraComponent::destroy()
{
	ovrHmd_Destroy(hmd); 
	ovr_Shutdown();
}

glm::quat fromOVR( OVR::Quatf quat )
{
	OVR::Vector3f OVRaxis;
	float angle;
	quat.GetAxisAngle(&OVRaxis, &angle);

	//convert axis type
	glm::vec3 axis = glm::vec3(OVRaxis.x, OVRaxis.y, OVRaxis.z);

	//construct glm orientation quaternion
	glm::quat orientation = glm::angleAxis(glm::degrees(angle), glm::normalize(axis));

	return orientation;
}

glm::vec3 fromOVR( OVR::Vector3f vec )
{
	return glm::vec3( vec.x, vec.y, vec.z );
}

void OcculusCameraComponent::update( float dt )
{
	//Occulus warning
	// Health and Safety Warning display state.
	ovrHSWDisplayState hswDisplayState;
	ovrHmd_GetHSWDisplayState(hmd, &hswDisplayState);
	if (hswDisplayState.Displayed) 
	{ 
		// Dismiss the warning if the user pressed the appropriate key or if the user 
		// is tapping the side of the HMD. 
		// If the user has requested to dismiss the warning via keyboard or controller input... 
		//if (Util_GetAndResetHSWDismissedState()) 
		//	ovrHmd_DismissHSWDisplay(hmd); 
		//else 
		{ 
			// Detect a moderate tap on the side of the HMD. 
			ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
			if (ts.StatusFlags & ovrStatus_OrientationTracked) 
			{ 
				const OVR::Vector3f v(ts.RawSensorData.Accelerometer.x, ts.RawSensorData.Accelerometer.y, ts.RawSensorData.Accelerometer.z);

				// Arbitrary value and representing moderate tap on the side of the DK2 Rift.
				if (v.LengthSq() > 250.f) ovrHmd_DismissHSWDisplay(hmd);
			}
		}
	}


	dt;
	// Query the HMD for the current tracking state.
	
	ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
	{
		ovrPosef pose = ts.HeadPose.ThePose;

		//convert to mat4 if desired
		glm::mat4 transformMatrix = glm::mat4_cast( fromOVR( pose.Orientation ) );

		//parent->gc<TransformComponent>()->setRotation( glm::mat3(transformMatrix) );
	}
	parent->getStage()->subscribeRender( this );
}

int OcculusCameraComponent::getNumberOfRuns()
{
	return 2;
}

void OcculusCameraComponent::setUp()
{
	ovrHmd_BeginFrame(hmd, 0); 
	//frameTiming = ovrHmd_BeginFrameTiming(hmd, 0);
	
	//Set render target		pRender->SetRenderTarget ( pRendertargetTexture );
	//clear render target	pRender->Clear();

	Graphics * g = parent->getStage()->getGame()->getGraphicsHandle();
	g->setFrameBuffer( renderTarget );
	g->clear();

	ovrVector3f hmdToEyeViewOffset[2] = { EyeRenderDesc[0].HmdToEyeViewOffset, EyeRenderDesc[1].HmdToEyeViewOffset };

	static ovrTrackingState HmdState;
	ovrHmd_GetEyePoses(hmd, 0, hmdToEyeViewOffset, headPoses, &HmdState);
}

void OcculusCameraComponent::startRun( int run )
{
	//OcculusStuff
	ovrEyeType eye = hmd->EyeRenderOrder[run]; 

	ovrPosef eyePose = headPoses[ eye ];

	OVR::Quatf orientation = OVR::Quatf(eyePose.Orientation); 
	glm::quat orient = fromOVR( orientation );

	glm::mat4 orientTransform = glm::mat4_cast( orient );

	// * Test code * 
	// Assign quaternion result directly to view (translation is ignored). 
	Graphics * g = parent->getStage()->getGame()->getGraphicsHandle();
	g->setFrameBuffer( renderTarget );
	g->setViewport( EyeRenderViewport[eye].Pos.x, EyeRenderViewport[eye].Pos.y, EyeRenderViewport[eye].Size.w, EyeRenderViewport[eye].Size.h );
	//OVR::Matrix4f view = OVR::Matrix4f(orientation.Inverted()) * OVR::Matrix4f::Translation(-WorldEyePosition);
	//pRender->SetViewport(); pRender->SetProjection(proj);
	//pRoomScene->Render(pRender, Matrix4f::Translation(EyeRenderDesc[eye].ViewAdjust) * view);

	TransformComponent * trans = parent->getComponent<TransformComponent>();

	glm::vec3 eyepos = 20.0f*fromOVR(eyePose.Position);
	//OVR::Vector3f shiftedEyePos      = OVR::Vector3f( trans->getTranslation().x, trans->getTranslation().y, trans->getTranslation().z ) + eyePose.Position;
    
	glm::vec3 from = glm::vec3();
	glm::vec3 to = glm::vec3(0,0,-1);
	glm::vec3 up = glm::vec3(0,1,0);
	//TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
	{
		from = glm::vec3( trans->getTransformation() * orientTransform * glm::vec4( from, 1 ) );
		to = glm::vec3( trans->getTransformation() * orientTransform * glm::vec4( to, 1 ) );
		up = glm::vec3( trans->getRotationMatrix() * orientTransform * glm::vec4( up, 1 ) );
	}
	glm::mat4 camcam = glm::perspective(fov, aspect, camNear, camFar) * glm::lookAt( from + eyepos, to + eyepos, up );
	Shader::setGlobalUniformParameter( "mvp", ParameterType::PT_MAT4, &camcam );
}

void OcculusCameraComponent::endRun( int run )
{
	run;
}

void OcculusCameraComponent::breakDown()
{
	parent->getStage()->getGame()->getGraphicsHandle()->setFrameBuffer();
	ovrHmd_EndFrame(hmd, headPoses, &EyeTexture[0].Texture);
}