#include "GraphicsHandle.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include "Camera.h"
#include <QtGui\QMouseEvent>
#include "ShapeGenerator.h"

#include "DebugMenus.h"
#include <Qt\qapplication.h>

#include "noise\noise.h"

Camera camera;
float tightness;
glm::vec3 shipPos;
float fps, delta;
float timey;

GeneralGlWindow::ShaderInfo *passShad;

const char * stringy = "This is a string";

GeneralGlWindow::ShaderInfo *questionShad;
GeneralGlWindow::Renderable * character, *bushRend, *groundRend, *skyRend, *questionBlock;

GeneralGlWindow::ShaderInfo *alphaPotShad;
GeneralGlWindow::Renderable * pot;

GeneralGlWindow::ShaderInfo *worldNormalMapShader;
GeneralGlWindow::Renderable * worldPlane, *lightCube;
glm::vec3 lightPos;
float specAmount;
float normalFade;

GeneralGlWindow::ShaderInfo *tangentNormalMapShader;
GeneralGlWindow::Renderable * datCubeTangent;
glm::vec3 rotateTangentCube;

GeneralGlWindow::ShaderInfo *ogreShader;
GeneralGlWindow::Renderable * ogre;
glm::vec3 rotateOgre;
bool useDiffuse = true, useNormal = true, useAmbient = true;
float useDiffuseF, useNormalF, useAmbientF;

GeneralGlWindow::TextureInfo *perlinMap;
GeneralGlWindow::Renderable * perlinShow, *starrySky;
GeneralGlWindow::ShaderInfo *perlinShad, *starShad;
float perlinOctaves = 11, perlinFrequency = 10, perlinPersistence = 0.5, perlinSeed = 22, perlinLacunarity = 3.0, perlinZ = 0;
//glm::vec3 standingPosition = glm::vec3( 10, 0, 0 );
bool goOnSurface = false;
QImage * img = new QImage();

GeneralGlWindow::FrameBufferInfo *frameBuffer;
GeneralGlWindow::Renderable *plane1, *plane2;

GeneralGlWindow::ShaderInfo *hudShad, *hudDepthShad;

GeneralGlWindow::FrameBufferInfo *shadowBuffer;
GeneralGlWindow::Renderable *plane3, *backPlane;
GeneralGlWindow::ShaderInfo *perlinShadWithShadow, *textShadWithShadow;
Camera lightCamera;
bool useLightCamera = false;

GeneralGlWindow::TextureInfo *cubeMapSimple, *cubeMapFancy;
GeneralGlWindow::ShaderInfo *environmentMapper, *skyBoxShad;
GeneralGlWindow::Renderable *skybox, *environmentTarget;
bool useFancyEnvironment = false;

float orbitRate = 0.2, currentRot = 0;
GeneralGlWindow::ShaderInfo *litStarrySky;
GeneralGlWindow::Renderable *sun;

bool showFirstLab, showSecondLab, showThirdLab, showFourthLab, showFifthLab, showSixthLab, showSeventhLab, showEighthLab, showNinthLab, showCoolLab;

Neumont::ShapeData makeCube()
{
	Neumont::ShapeData result = Neumont::ShapeGenerator::makeCube();

	int i = 0;

	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );

	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].color = glm::vec4( 1, 0, 0, -1 );

	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, 1, -1 );
	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, 1, -1 );
	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, 1, -1 );
	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, 1, -1 );

	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, -1, -1 );
	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, -1, -1 );
	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, -1, -1 );
	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].color = glm::vec4( 0, 0, -1, -1 );

	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, -1 );
	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, -1 );
	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, -1 );
	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, -1 );

	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, 1 );
	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, 1 );
	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, 1 );
	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].color = glm::vec4( -1, 0, 0, 1 );

	return result;
}

Neumont::ShapeData makeCoolPlane()
{
	Neumont::ShapeData result = Neumont::ShapeData();

	result.numVerts = 4;
	result.numIndices = 6;

	result.indices = new ushort[result.numIndices];
	result.verts = new Neumont::Vertex[result.numVerts];

	result.indices[0] = 0;
	result.indices[1] = 1;
	result.indices[2] = 2;

	result.indices[3] = 0;
	result.indices[4] = 2;
	result.indices[5] = 3;

	int i = 0; 
	result.verts[i].uv = glm::vec2( 0, 0 );
	result.verts[i].position = glm::vec3( 0, 0, 0.9f );
	result.verts[i].normal = glm::vec3( 0, 1, 0 );
	i++;

	result.verts[i].uv = glm::vec2( 1, 0 );
	result.verts[i].position = glm::vec3( 1, 0, 0.9f );
	result.verts[i].normal = glm::vec3( 0, 1, 0 );
	i++;

	result.verts[i].uv = glm::vec2( 1, 1 );
	result.verts[i].position = glm::vec3( 1, 1, 0.9f );
	result.verts[i].normal = glm::vec3( 0, 1, 0 );
	i++;

	result.verts[i].uv = glm::vec2( 0, 1 );
	result.verts[i].position = glm::vec3( 0, 1, 0.9f );
	result.verts[i].normal = glm::vec3( 0, 1, 0 );

	return result;
}

//Ogre lab
void calculateTangents(Neumont::ShapeData * shapeData)
{
	glm::vec3 *tan1 = new glm::vec3[shapeData->numVerts * 2];
    glm::vec3 *tan2 = tan1 + shapeData->numVerts;
    ZeroMemory(tan1, shapeData->numVerts * sizeof(glm::vec3) * 2);
    
	for (long a = 0; a < shapeData->numIndices/3; a++)
    {
		long i1 = shapeData->indices[a*3+0];
        long i2 = shapeData->indices[a*3+1];
        long i3 = shapeData->indices[a*3+2];
        
		const Neumont::Vertex& v1 = shapeData->verts[i1];
        const Neumont::Vertex& v2 = shapeData->verts[i2];
        const Neumont::Vertex& v3 = shapeData->verts[i3];
        
        /*const Point2D& w1 = texcoord[i1];
        const Point2D& w2 = texcoord[i2];
        const Point2D& w3 = texcoord[i3];*/
        
        float x1 = v2.normal.x - v1.normal.x;
        float x2 = v3.normal.x - v1.normal.x;
        float y1 = v2.normal.y - v1.normal.y;
        float y2 = v3.normal.y - v1.normal.y;
        float z1 = v2.normal.z - v1.normal.z;
        float z2 = v3.normal.z - v1.normal.z;
        
		float s1 = v2.uv.x - v1.uv.x;
        float s2 = v3.uv.x - v1.uv.x;
        float t1 = v2.uv.y - v1.uv.y;
        float t2 = v3.uv.y - v1.uv.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);
        
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }
    
    for (long a = 0; a < shapeData->numVerts; a++)
    {
		const glm::vec3& n = shapeData->verts[a].normal;
        const glm::vec3& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
		shapeData->verts[a].color = glm::vec4( glm::normalize(t - n * glm::dot(n, t)), 0 );
        
        // Calculate handedness
		shapeData->verts[a].color.w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		//std::cout << shapeData->verts[a].normal.x << "; " << shapeData->verts[a].normal.y << "; " << shapeData->verts[a].normal.z << ";;; " << shapeData->verts[a].color.x << "; " << shapeData->verts[a].color.y << "; " << shapeData->verts[a].color.z << "; " << shapeData->verts[a].color.w << std::endl;
    }
    
    delete[] tan1;
}

glm::vec4 difLight = glm::vec4(1.0f,1.0f,1.0f,1), colInfluence = glm::vec4(0.8f,0.8f,0.8f,1), specColor = glm::vec4(0.8f,0.8f,0.8f,1);

void GraphicsHandle::init()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	showFirstLab = false;
	showSecondLab = false;
	showThirdLab = false;
	showFourthLab = false;
	showFifthLab = false;
	showSixthLab = false;
	showSeventhLab = false;
	showEighthLab = false;
	showNinthLab = false;
	showCoolLab = true;

	specAmount = 100;
	lightPos = glm::vec3(-1.0f,1.0f,3.0f);
	normalFade = 0;

	//DebugMenus::inject( layout() );
	//DebugMenus::menu->show();

	passShad = addShaderInfo( "res/passThrough.vert", "res/passThrough.frag" );
	textShad = addShaderInfo( "res/texture.vert", "res/texture.frag" );
	questionShad = addShaderInfo( "res/texture.vert", "res/question.frag" );
	alphaPotShad = addShaderInfo( "res/texture.vert", "res/teapotAlpha.frag" );
	worldNormalMapShader = addShaderInfo( "res/texture.vert", "res/worldNormal.frag" );
	tangentNormalMapShader = addShaderInfo( "res/texture.vert", "res/tangentNormal.frag" );
	ogreShader = addShaderInfo( "res/texture.vert", "res/ogre.frag" );
	perlinShad = addShaderInfo( "res/perlin.vert", "res/perlin.frag" );
	starShad = addShaderInfo( "res/texture.vert", "res/stars.frag" );
	hudShad = addShaderInfo( "res/noCamera.vert", "res/noCamera.frag" );
	hudDepthShad = addShaderInfo( "res/noCamera.vert", "res/depthPlz.frag" );
	perlinShadWithShadow = addShaderInfo( "res/perlin.vert", "res/perlinWithShad.frag" );
	textShadWithShadow = addShaderInfo( "res/texture.vert", "res/textureWithShad.frag" );
	environmentMapper = addShaderInfo( "res/texture.vert", "res/enviroMapped.frag" );
	skyBoxShad = addShaderInfo( "res/texture.vert", "res/skyBox.frag" );
	litStarrySky = addShaderInfo( "res/texture.vert", "res/starLit.frag" );

	GeneralGlWindow::TextureInfo * marioAndWeegee = addTexture( "res/marioAndLuigi.png" );
	GeneralGlWindow::TextureInfo * marioAndWeegeeTrans = addTexture( "res/marioAndLuigiTrans.png" );

	GeneralGlWindow::TextureInfo * bush = addTexture( "res/bush.png" );
	GeneralGlWindow::TextureInfo * bushTrans = addTexture( "res/bushTrans.png" );

	GeneralGlWindow::TextureInfo * ground = addTexture( "res/grass.png" );
	GeneralGlWindow::TextureInfo * sky = addTexture( "res/sky.png" );

	GeneralGlWindow::TextureInfo * question = addTexture( "res/question.png" );
	GeneralGlWindow::TextureInfo * noise = addTexture( "res/seamless.png" );

	GeneralGlWindow::TextureInfo * worldNormalMapText = addTexture( "res/Shapes.png" );

	GeneralGlWindow::TextureInfo * ogreColor = addTexture( "res/ogreColor.png" );
	GeneralGlWindow::TextureInfo * ogreAmbientOcclusion = addTexture( "res/ogreAmbientOcclusion.png" );
	GeneralGlWindow::TextureInfo * ogreNormal = addTexture( "res/ogreNormal.png" );

	cubeMapSimple = addTextureCubeMap( "res/simpleCubeMap/posx.png", "res/simpleCubeMap/posy.png", "res/simpleCubeMap/posz.png", "res/simpleCubeMap/negx.png", "res/simpleCubeMap/negy.png", "res/simpleCubeMap/negz.png" );
	cubeMapFancy = addTextureCubeMap( "res/fancyCubeMap/posx.png", "res/fancyCubeMap/posy.png", "res/fancyCubeMap/posz.png", "res/fancyCubeMap/negx.png", "res/fancyCubeMap/negy.png", "res/fancyCubeMap/negz.png" );

	GeneralGlWindow::TextureInfo * sunText = addTexture( "res/sun.png" );

	frameBuffer = createFrameBuffer( 512, 512 );
	shadowBuffer = createFrameBuffer( 1024, 1024 );

	Neumont::ShapeData charData = Neumont::ShapeGenerator::makePlane(2);
	GeneralGlWindow::GeometryInfo * charGeo = addGeometry( charData.verts, charData.numVerts, charData.indices, charData.numIndices, GL_TRIANGLES );

	Neumont::ShapeData fancyPlane = makeCoolPlane();
	GeneralGlWindow::GeometryInfo * planeGeo = addGeometry( fancyPlane.verts, fancyPlane.numVerts, fancyPlane.indices, fancyPlane.numIndices, GL_TRIANGLES );

	Neumont::ShapeData cubeData = makeCube();
	GeneralGlWindow::GeometryInfo * cubeGeo = addGeometry( cubeData.verts, cubeData.numVerts, cubeData.indices, cubeData.numIndices, GL_TRIANGLES );

	Neumont::ShapeData potData = Neumont::ShapeGenerator::makeTeapot(8,glm::mat4());
	GeneralGlWindow::GeometryInfo * potGeo = addGeometry( potData.verts, potData.numVerts, potData.indices, potData.numIndices, GL_TRIANGLES );

	Neumont::ShapeData sphereData = Neumont::ShapeGenerator::makeSphere( 250 );
	for( int i = 0; i < sphereData.numVerts; i++ )
	{
		Neumont::Vertex * vert = &sphereData.verts[i];
		vert->uv = glm::vec2( ( std::atan2( vert->position.y, vert->position.x ) ) / 6.28 + 0.5, 0.5 - std::asin( vert->position.z ) / 3.14 );
	}
	GeneralGlWindow::GeometryInfo * sphereGeo = addGeometry( sphereData.verts, sphereData.numVerts, sphereData.indices, sphereData.numIndices, GL_TRIANGLES );

	GeneralGlWindow::GeometryInfo * ogreGeo = loadFile( "res/bs_ears.mod" );
	
	setUpAttribs( charGeo );
	setUpAttribs( planeGeo );
	setUpAttribs( cubeGeo );
	setUpAttribs( potGeo );
	setUpAttribs( ogreGeo );
	setUpAttribs( sphereGeo );
	character = addRenderable( charGeo, glm::translate( glm::vec3( -2, 0, 2 ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) ) * glm::scale( glm::vec3( 1, 1 , -1 ) ), textShad, marioAndWeegee, marioAndWeegeeTrans );
	bushRend = addRenderable( charGeo, glm::translate( glm::vec3( -3, 0, 1 ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) ) * glm::scale( glm::vec3( 1, 1 , -1 ) ), textShad, bush, bushTrans );
	groundRend = addRenderable( charGeo, glm::translate( glm::vec3( 0, -1, 4 ) )*glm::scale( glm::vec3( 4.0f ) ), textShad, ground );
	skyRend = addRenderable( charGeo, glm::translate( glm::vec3( 0, 3, 0 ) )*glm::scale( glm::vec3( 4.0f ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) ) * glm::scale( glm::vec3( 1, 1 , -1 ) ), textShad, sky );

	questionBlock = addRenderable( cubeGeo, glm::translate( glm::vec3( -1, 0.5f, 2 ) )*glm::scale( glm::vec3( 0.2f ) ), questionShad, question, noise );
	/*character->trans = flagByTexture;*/

	pot = addRenderable( potGeo, glm::translate( glm::vec3( -2, -1, 2 ) )*glm::rotate( -90.0f, glm::vec3( 1,0,0 ) )*glm::scale( glm::vec3( 0.5f ) ), alphaPotShad, noise );

	worldPlane = addRenderable( charGeo, glm::translate( glm::vec3( -0.1f, -1, 2 ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) )*glm::scale( glm::vec3( 2.0f ) ), worldNormalMapShader, worldNormalMapText, worldNormalMapText );
	lightCube = addRenderable( cubeGeo, glm::mat4(), worldNormalMapShader, sky );

	datCubeTangent = addRenderable( cubeGeo, glm::mat4(), tangentNormalMapShader, worldNormalMapText, worldNormalMapText );
	rotateTangentCube = glm::vec3();

	ogre = addRenderable( ogreGeo, glm::mat4(), ogreShader, ogreColor, ogreNormal, ogreAmbientOcclusion );
	rotateOgre = glm::vec3();


	QImage perlin( 512, 512, QImage::Format::Format_ARGB32 );
	perlinMap = addTexture( &perlin );
	perlinShow = addRenderable( sphereGeo, glm::rotate( glm::mat4(), 90.0f, glm::vec3( 1,0, 0 ) ), perlinShad, perlinMap, shadowBuffer->depthTexture );
	starrySky = addRenderable( sphereGeo, glm::mat4(), starShad, perlinMap );

	plane1 = addRenderable( planeGeo, glm::translate( glm::vec3( 0.5, 0.5, 0 ) ) * glm::scale( glm::vec3( 0.5 ) ), hudShad, frameBuffer->colorTexture );
	plane2 = addRenderable( planeGeo, glm::translate( glm::vec3( 0.5, 0, 0 ) ) * glm::scale( glm::vec3( 0.5 ) ), hudDepthShad, frameBuffer->depthTexture );

	plane3 = addRenderable( planeGeo, glm::translate( glm::vec3( 0.5, 0.5, 0 ) ) * glm::scale( glm::vec3( 0.5 ) ), hudDepthShad, shadowBuffer->depthTexture );
	backPlane = addRenderable( charGeo, glm::translate( glm::vec3( 0.5, -1.5, -3 ) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 1,0, 0 ) ) * glm::scale( glm::vec3( 4 ) ), textShadWithShadow, sky, shadowBuffer->depthTexture );
	
	skybox = addRenderable( cubeGeo, glm::scale( glm::vec3( 10 ) ), skyBoxShad, nullptr, cubeMapSimple );
	environmentTarget = addRenderable( potGeo, glm::translate( glm::vec3( -1, 0, 2 ) )*glm::rotate( -90.0f, glm::vec3( 1,0,0 ) )*glm::scale( glm::vec3( 0.25f ) ), environmentMapper, nullptr, cubeMapSimple );
	
	sun = addRenderable( sphereGeo, glm::mat4(), passShad, sunText );

	lightCamera = Camera( float(width())/height(), 0.01, 20 );
	lightCamera.setFrom( lightPos );
	lightCamera.setTo( glm::vec3(-1,0,0) );

	tightness = 40.0f;

	camera = Camera( float(width())/height(), 0.01, 20 );
	camera.setFrom( glm::vec3( -1, 0.5f, 5 ) );
	camera.setTo( glm::vec3(-2,0,0) );

	addUniformParameter( "mvp", PT_MAT4, (float*)&camera.mvp );
	setUniformParameter( "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );//set
	addUniformParameter( "diffpos", PT_VEC3, (float*)&lightPos );
	addUniformParameter( "normalFade", PT_FLOAT, (float*)&normalFade );
	addUniformParameter( "difflight", PT_VEC4, (float*)&difLight);//set
	addUniformParameter( "colorInfluence", PT_VEC4, (float*)&colInfluence);//set
	addUniformParameter( "specColor", PT_VEC4, (float*)&specColor);//set
	addUniformParameter( "tightness", PT_FLOAT, (float*)&(specAmount) );
	addUniformParameter( "eye", PT_VEC4, (float*)&camera.from);
	addUniformParameter( "timey", PT_FLOAT, &timey);


	addUniformParameter( ogreShader, "pleaseColor", PT_FLOAT, (float*)&(useDiffuseF) );
	addUniformParameter( ogreShader, "pleaseNormal", PT_FLOAT, (float*)&(useNormalF) );
	addUniformParameter( ogreShader, "pleaseAmbient", PT_FLOAT, (float*)&(useAmbientF) );

	DebugMenus::toggleBool( "Show Environment Mapping Lab", showNinthLab, "Environment Map" );
	DebugMenus::toggleBool( "Use Fancy Environment Map", useFancyEnvironment, "Environment Map" );

	DebugMenus::toggleBool( "Show Shadows for Planet", showEighthLab, "Shadow" );
	//DebugMenus::slideVector( "Light Position", lightPos, -5, 5, "Shadow" );
	DebugMenus::toggleBool( "Use light camera", useLightCamera, "Shadow" );

	DebugMenus::toggleBool( "Show Cool Lab", showCoolLab, "Cool Lab" );
	DebugMenus::slideFloat( "Orbit Rate", orbitRate, 0, 3.5f, "Cool Lab" );

	DebugMenus::toggleBool( "Show Color+Depth", showSeventhLab, "Frame Buffers" );

	DebugMenus::toggleBool( "Show Perlin Lab", showSixthLab, "Perlin" );
	DebugMenus::slideFloat( "Seed", perlinSeed, 0, 100, "Perlin" );
	DebugMenus::slideFloat( "Octaves", perlinOctaves, 1, 11, "Perlin" );
	DebugMenus::slideFloat( "Frequency", perlinFrequency, 0, 10, "Perlin" );
	DebugMenus::slideFloat( "Persistence", perlinPersistence, 0, 2, "Perlin" );
	DebugMenus::slideFloat( "Lacunarity", perlinLacunarity, 0, 10, "Perlin" );
	DebugMenus::slideFloat( "Sampled Z", perlinZ, 0, 1, "Perlin" );
	DebugMenus::toggleBool( "Planet Walk", goOnSurface, "Perlin" );

	DebugMenus::toggleBool( "Show Ogre Lab", showFifthLab, "RAGHR OGRE" );
	DebugMenus::slideVector( "Light Position", lightPos, -5, 5, "RAGHR OGRE" );
	DebugMenus::slideVector( "Ogre Rotation", rotateOgre, 0, 360, "RAGHR OGRE" );
	DebugMenus::toggleBool( "Use Diffuse", useDiffuse, "RAGHR OGRE" );
	DebugMenus::toggleBool( "Use Normal", useNormal, "RAGHR OGRE" );
	DebugMenus::toggleBool( "Use AO", useAmbient, "RAGHR OGRE" );

	/*DebugMenus::toggleBool( "Show Tangent Normal Lab", showFourthLab, "Tangent Normal" );
	DebugMenus::slideVector( "Light Position", lightPos, -5, 5, "Tangent Normal" );
	DebugMenus::slideVector( "Cube Rotation", rotateTangentCube, 0, 360, "Tangent Normal" );
	DebugMenus::slideFloat( "Spec Amount", specAmount, 0, 1000, "Tangent Normal" );
	DebugMenus::slideFloat( "Normal Fade", normalFade, 0, 1, "Tangent Normal" );*/

	/*DebugMenus::toggleBool( "Show World Normal Lab", showThirdLab, "World Normal" );
	DebugMenus::slideVector( "Light Position", lightPos, -5, 5, "World Normal" );
	DebugMenus::slideFloat( "Spec Amount", specAmount, 0, 1000, "World Normal" );
	DebugMenus::slideFloat( "Normal Fade", normalFade, 0, 1, "World Normal" );*/

	//DebugMenus::toggleBool( "Show Alpha Lab", showSecondLab, "Alpha" );

	//DebugMenus::toggleBool( "Show Binary Alpha Lab", showFirstLab, "Binary Alpha" );
	
}

float angle = 0;

void GraphicsHandle::paint()
{
	if( showSeventhLab )
	{
		setRenderTarget( frameBuffer );
		paintInner();
		resetRenderTarget();
	}
	if( showEighthLab )
	{
		glm::vec3 fro = camera.from;
		glm::vec3 toe = camera.to;

		camera.from = lightPos;
		lightCamera.from = lightPos;
		camera.to = lightCamera.to;

		camera.calcModelViewProjection();
		setUniformParameter( "lightMvp", ParameterType::PT_MAT4, (float*)&camera.mvp );

		perlinShow->howShader = perlinShadWithShadow;

		setRenderTarget( shadowBuffer );
		paintInner();
		resetRenderTarget();


		camera.from = fro;
		camera.to = toe;
		camera.calcModelViewProjection();
	}
	else
	{
		perlinShow->howShader = perlinShad;
	}
	glViewport( 0, 0, width(), height() );
	if( useLightCamera )
	{
		glm::vec3 fro = camera.from;
		glm::vec3 toe = camera.to;

		camera.from = lightCamera.from;
		camera.to = lightCamera.to;

		camera.calcModelViewProjection();

		paintInner();

		camera.from = fro;
		camera.to = toe;
		camera.calcModelViewProjection();
	}
	else
	{
		paintInner();
	}

	if( showSeventhLab )
	{
		plane1->draw();
		plane2->draw();
	}
	if( showEighthLab )
	{
		plane3->draw();
		setUniformParameter( "diffpos", PT_VEC4, (float*)&glm::vec4(lightPos,1) );
		//lightCube->where = glm::translate( lightPos )*glm::scale( glm::vec3( 0.05 ) );
		//lightCube->draw();
	}
}

void GraphicsHandle::paintInner()
{
	camera.setAspect( float(width())/height() );
	glClearColor( 0.5, 0.1, 0.1, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( showCoolLab )
	{
		goOnSurface = true;
	}

	setUniformParameter( "diffpos", PT_VEC4, (float*)&glm::vec4(lightPos,1) );

	if( showFirstLab )
	{
		character->draw();
		bushRend->draw();
		groundRend->draw();
		skyRend->draw();
		questionBlock->draw();
	}

	if( showSecondLab )
	{
		groundRend->draw();

		glEnable (GL_BLEND);
		//glEnable(GL_POLYGON_STIPPLE);
		glEnable(GL_CULL_FACE);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		pot->draw();
		//glDisable(GL_POLYGON_STIPPLE);
		glDisable(GL_CULL_FACE);
		glDisable (GL_BLEND);
	}

	if( showThirdLab )
	{
		setUniformParameter( worldNormalMapShader, "diffpos", PT_VEC4, (float*)&glm::vec4(lightPos,1) );
		lightCube->where = glm::translate( lightPos )*glm::scale( glm::vec3( 0.05 ) );
		lightCube->draw();
		worldPlane->draw();
	}

	if( showFourthLab )
	{
		setUniformParameter( tangentNormalMapShader, "diffpos", PT_VEC4, (float*)&glm::vec4(lightPos,1) );
		lightCube->where = glm::translate( lightPos )*glm::scale( glm::vec3( 0.05 ) );
		lightCube->draw();
		datCubeTangent->where = glm::translate( glm::vec3(-1, 0, 2) ) * glm::scale( 0.25f, 0.25f, 0.25f ) * glm::rotate( rotateTangentCube.x, glm::vec3( 1, 0, 0 ) ) * glm::rotate( rotateTangentCube.y, glm::vec3( 0, 1, 0 ) ) * glm::rotate( rotateTangentCube.z, glm::vec3( 0, 0, 1 ) );
		datCubeTangent->draw();
	}

	if( showFifthLab )
	{
		setUniformParameter( ogreShader, "diffpos", PT_VEC4, (float*)&glm::vec4(lightPos,1) );
		lightCube->where = glm::translate( lightPos )*glm::scale( glm::vec3( 0.05 ) );
		lightCube->draw();
		ogre->where = glm::translate( glm::vec3(-1, 0, 2) ) * glm::scale( glm::vec3( 1.0f ) ) * glm::rotate( rotateOgre.x, glm::vec3( 1, 0, 0 ) ) * glm::rotate( rotateOgre.y, glm::vec3( 0, 1, 0 ) ) * glm::rotate( rotateOgre.z, glm::vec3( 0, 0, 1 ) );
		//ogre->where = glm::scale( glm::vec3( 3.0f ) );
		ogre->draw();
		useDiffuseF = (useDiffuse) ? 1 : 0;
		useNormalF = (useNormal) ? 1 : 0;
		useAmbientF = (useAmbient) ? 1 : 0;
	}

	if( showSixthLab || showEighthLab || showCoolLab )
	{
		static float prevFreq, prevLac, prevSeed, prevZ, prevPers;
		static int prevOct;
		int dimensions = 256;

		if( prevFreq != perlinFrequency || prevLac != perlinLacunarity || prevSeed != perlinSeed || prevZ != perlinZ || prevOct != (int)perlinOctaves || prevPers != perlinPersistence )
		{
			noise::module::Perlin perlinModule;
			perlinModule.SetFrequency( perlinFrequency );
			perlinModule.SetPersistence( perlinPersistence );
			perlinModule.SetOctaveCount( (int) perlinOctaves );
			perlinModule.SetLacunarity( perlinLacunarity );
			perlinModule.SetSeed( perlinSeed );

			
			*img = QImage( dimensions, dimensions, QImage::Format::Format_ARGB32 );
			for( int x = 0; x < img->width(); x++ )
			{
				for( int y = 0; y < img->height(); y++ )
				{
					double value = ( perlinModule.GetValue( x/(float)(dimensions), y/(float)(dimensions), perlinZ/10 ) + 1 ) / 2;
					value = (value > 1) ? 1 : value;
					value = (value < 0) ? 0 : value;
					int amount = (int) (255 * value );
					img->setPixel( x, y, qRgb( amount, amount, amount ) );
				}
			}
			updateTexture( perlinMap, img );

			prevFreq = perlinFrequency;
			prevLac = perlinLacunarity;
			prevSeed = perlinSeed;
			prevZ = perlinZ;
			prevOct = (int)perlinOctaves;
			prevPers = perlinPersistence;
		}

		glm::vec3 worldPosition = glm::vec3( -1, 0, 0 );

		if( goOnSurface )
		{
			/*noise::module::Perlin perlinModule;
			perlinModule.SetFrequency( perlinFrequency );
			perlinModule.SetPersistence( perlinPersistence );
			perlinModule.SetOctaveCount( (int) perlinOctaves );
			perlinModule.SetLacunarity( perlinLacunarity );
			perlinModule.SetSeed( perlinSeed );*/

			camera.from = glm::normalize( camera.from - worldPosition );
			//glm::vec2 uvShould = glm::vec2( ( std::atan2( camera.from.y, camera.from.x ) ) / 6.28 + 0.5, 0.5 - std::asin( camera.from.z ) / 3.14 );

			//float value = ( perlinModule.GetValue( uvShould.x, uvShould.y, perlinZ/10 ) + 1 ) / 2;
			//		value = (value > 1) ? 1 : value;
			//		value = (value < 0) ? 0 : value;
			//		//int amount = (int) (255 * value );

			////float value = qRed( img->pixel( (int)(img->width()*uvShould.x), (int)(img->height()*uvShould.y) ) ) / 255.0f;


			//float dist = 2 * std::max( 0.0f, std::min( 0.5f, 0.6f-glm::length( uvShould - glm::vec2( 0.5f, 0.5f ) ) ) );
			//float xMult = 2, yMult = 3;
			//float cutOff = 0.35 + sin(timey+uvShould.x*xMult+uvShould.y*yMult)*0.02;
			camera.from = camera.from * ( 1.05f );//+ std::pow( std::max( cutOff, value*dist) , 2.0f )/10.0f );

			
			//camera.from = camera.from * 1.05f + worldPosition;
			camera.from += worldPosition;
			camera.up = glm::normalize( camera.from - worldPosition );
			//camera.from = standingPosition;
		}
		else
		{
			camera.up = glm::vec3( 0, 1, 0 );
		}

		camera.calcModelViewProjection();

		perlinShow->where = glm::translate( glm::vec3(-1, 0, 0) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 1,0, 0 ) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 0, 0, 1 ) );
		starrySky->where = glm::translate( camera.from ) * glm::scale( -glm::vec3( 10 ) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 1,0, 0 ) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 0, 0, 1 ) );
		starrySky->draw();

		perlinShow->draw();
	}
	if( showEighthLab )
	{
		backPlane->draw();
	}
	if( showNinthLab )
	{
		skybox->trans = ( useFancyEnvironment ) ? cubeMapFancy : cubeMapSimple;
		environmentTarget->trans = ( useFancyEnvironment ) ? cubeMapFancy : cubeMapSimple;

		skybox->where = glm::translate( camera.from ) * glm::scale( glm::vec3(10) );
		skybox->draw();
		environmentTarget->draw();
	}
	if( showCoolLab )
	{
		sun->where = glm::translate( lightPos ) * glm::scale( glm::vec3( 0.5f ) );
		sun->draw();
	}
}

glm::vec2 mousePosition = glm::vec2();
bool mouseTracking = false;


void GraphicsHandle::mousePressEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{

		if( !mouseTracking )
		{
			mousePosition.x = mEvent->x()/float(width());
			mousePosition.y = mEvent->y()/float(height());
		}
		mouseTracking = true; 
	}

}

void GraphicsHandle::mouseReleaseEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{
		mouseTracking = false;
	}
}

void GraphicsHandle::mouseMoveEvent( QMouseEvent * mEvent )
{
	if ( mouseTracking )
	{
		Camera* cam = (useLightCamera) ? &lightCamera : &camera;

		glm::vec2 currentPosition = glm::vec2( mEvent->x()/float(width()), mEvent->y()/float(height()));

		glm::vec2 delta = currentPosition-mousePosition;

		glm::vec3 camNorm = glm::normalize( cam->getTo()-cam->getFrom() );

		glm::vec3 rotUp = glm::cross( cam->up, camNorm );
		glm::vec3 rotRight = glm::cross( rotUp, camNorm );


		cam->setTo( cam->getFrom() + glm::rotate( glm::rotate( camNorm, delta.y*100, rotUp ), delta.x*100, rotRight ) );
		//camera.setTo( camera.getFrom() + glm::rotate( camNorm, delta.y*100, rotUp ) );
		//camera.setTo( glm::vec3( mousePosition.x, mousePosition.y, 0 ) );

		mousePosition.x = currentPosition.x;
		mousePosition.y = currentPosition.y;

		lightPos = lightCamera.from;
	}
}

void GraphicsHandle::update( float dt )
{
	timey += dt;

	fps = 1/dt;
	delta = dt;

	if( showCoolLab )
	{
		currentRot += orbitRate * dt;
		lightPos = camera.from + glm::vec3( sin( currentRot )*9, 0, cos(currentRot)*9 );
		lightCamera.from = lightPos;
		starrySky->howShader = litStarrySky;
	}
	else
	{
		starrySky->howShader = starShad;
	}

	if( QApplication::activeWindow() != 0 )
	{
		static glm::vec2 mousePosition = glm::vec2();

		Camera* cam = (useLightCamera) ? &lightCamera : &camera;

		glm::vec3 camNorm = glm::normalize( cam->getTo()-cam->getFrom() );
		glm::vec3 strafe = glm::normalize( -glm::cross( cam->up, camNorm ) );
		glm::vec3 up = glm::normalize( glm::cross( strafe, camNorm ) );

		glm::mat3 camSpace = glm::mat3( strafe, camNorm, up );

		glm::vec3 movement;
		/*if ( GetAsyncKeyState(VK_SHIFT) )
			movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), 0, (GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );
		else*/
			movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), (GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0),(GetAsyncKeyState('R')?1:0) - (GetAsyncKeyState('F')?1:0) );

			glm::vec3 worldMove = camSpace*movement * dt * ( (goOnSurface)? 0.1f : 1.0f );

		cam->setTo( cam->getTo() + worldMove);
		cam->setFrom( cam->getFrom() + worldMove);

		
	}
	
	lightPos = lightCamera.from;

	camera.calcModelViewProjection();

	repaint();
}