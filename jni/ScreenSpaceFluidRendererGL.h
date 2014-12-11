/* ScreenSpaceFluidRendererGL.h
	Copyright (C) 2012 Jackson Lee

	ZLib license
	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/
#ifndef SCREEN_SPACE_FLUID_RENDERER_GL_H
#define SCREEN_SPACE_FLUID_RENDERER_GL_H

#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"

#include "FrameBufferGL.h"

typedef struct
{
	GLfloat   m[4][4];
} ESMatrix;

void esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);
void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);
void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);
void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);
void esOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);
void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);
void esMatrixLoadIdentity(ESMatrix *result);
void esMatrixLookAt(ESMatrix *result,
	float posX, float posY, float posZ,
	float lookAtX, float lookAtY, float lookAtZ,
	float upX, float upY, float upZ);

//ScreenSpaceFluidRendererGL constructor may initialize GLEW; will fail if an OpenGL context does not exist
class ScreenSpaceFluidRendererGL
{
	int m_windowWidth;
	int m_windowHeight;

	GLuint m_positionVertexBuffer;
	GLfloat m_depthProjectionMatrix[16];	//Projection matrix when m_generateDepthShader is run

	GLuint m_basicShader;

	GLuint m_generateDepthShader;
	GLuint m_blurDepthShader;
	GLuint m_curvatureFlowShader;

	GLuint m_blurThickShader;
	GLuint m_absorptionAndTransparencyShader;

	GLuint m_generateSurfaceShader;
	GLuint m_blitShader;

	FrameBufferGL m_frameBuffer;

	//Textures are managed(and deleted) by m_frameBuffer
	GLuint m_tempColorTexture;
	GLuint m_tempDepthTexture;

	GLuint m_depthTexture;
	GLuint m_blurredDepthTexturePass1;
	GLuint m_blurredDepthTexturePass2;

	GLuint m_thickTexture;
	GLuint m_blurredThickTexturePass1;
	GLuint m_blurredThickTexturePass2;
	GLuint m_absorptionAndTransparencyTexture;

	GLuint m_surfaceColorTexture;
	GLuint m_surfaceDepthTexture;

	ESMatrix* projMat;
	ESMatrix* viewMat;

public:
	ScreenSpaceFluidRendererGL(int screenWidth, int screenHeight);
	~ScreenSpaceFluidRendererGL();

	void render(const btAlignedObjectArray<btVector3>& particlePositions, float sphereRadius,
				float r, float g, float b, float absorptionR, float absorptionG, float absorptionB,
				ESMatrix* _projMat, ESMatrix* _viewMat);


	void setWindowResolution(int width, int height)
	{
		if(width <= 0 || height <= 0) return;	//ScreenSpaceFluidRendererGL::render() crashes if dimension is 0

		m_windowWidth = width;
		m_windowHeight = height;
	}
	void setRenderingResolution(int width, int height)
	{
		if(width <= 0 || height <= 0) return;	//ScreenSpaceFluidRendererGL::render() crashes if dimension is 0
		m_frameBuffer.resizeTextures(width, height);
	}

private:
	void initialize();

	void render_stage1_generateDepthTexture(const btAlignedObjectArray<btVector3>& particlePositions, float sphereRadius);
	void render_stage2_blurDepthTextureCurvatureFlow();
	void render_stage2_blurDepthTextureBilateral();
	void render_stage3_generateThickTexture(const btAlignedObjectArray<btVector3>& particlePositions, float sphereRadius);
	void render_stage4_blurThickTexture();
	void render_stage5_generateAbsorptionAndTransparencyTexture(float absorptionR, float absorptionG, float absorptionB);
	void render_stage6_generateSurfaceTexture();

	void renderFullScreenTexture(GLuint texture2d_0, GLuint texture2d_1, GLuint texture2d_2);

	static GLuint compileProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
};

const char vShaderStr[] =
      "#version 300 es                            \n"
      "layout(location = 0) in vec4 a_position;   \n"
      "layout(location = 1) in vec2 a_texCoord;   \n"
      "out vec2 v_texCoord\n;                       \n"
      "void main()                                \n"
      "{                                          \n"
      "   gl_Position = a_position;               \n"
      "   v_texCoord = a_texCoord;                \n"
      "}                                          \n";

const char fShaderStr[] =
      "#version 300 es                                     \n"
      "precision mediump float;                            \n"
      "in vec2 v_texCoord;                                 \n"
      "layout(location = 0) out vec4 outColor;             \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
	//"outColor = texture( s_texture, v_texCoord ).r;\n"
	//"outColor =1.0f;\n"
	  "vec2 UV = v_texCoord;\n"
	  //"UV.x += 0.1; UV.y += 0.1;\n"
	  //"vec4 temp = texture( s_texture, v_texCoord ); "
	  "vec4 temp = texture( s_texture, UV ); "
	  "outColor = vec4(vec3(temp.x), 1.0f);"
	  //"outColor = texture( s_texture, v_texCoord );"
      "}                                                   \n";

const char generateDepthVertexShader[] =
	"#version 300 es\n"
	"layout(location = 0) in vec4 v_Pos;\n"
	"uniform float pointRadius\n;  	//Point size in world space\n"
	"uniform float pointScale\n;  	 	//Scale to calculate size in pixels\n"
	"uniform mat4 mvMat;\n"
	"uniform mat4 pMat;\n"
	"out vec3 eyePosition;\n"
	"out mat4 projectionMatrix;\n"
	"void main()\n"
	"{\n"
	"	 //Calculate window-space point size\n"
	"	eyePosition = (mvMat * vec4(v_Pos.xyz, 1.0)).xyz;\n"
	"	float distance = length(eyePosition);\n"
	"	gl_PointSize = pointRadius * (pointScale / distance);\n"
	"	projectionMatrix = pMat;\n"
	"	gl_Position = pMat * mvMat * vec4(v_Pos.xyz, 1.0);\n"
	"}";

const char generateDepthFragmentShader[] =
	"#version 300 es\n"
	"uniform float pointRadius;\n"
	"in vec3 eyePosition;\n"
	"in mat4 projectionMatrix;\n"
	//"out float Color;\n"
	"out vec4 Color;\n"
	"void main()\n"
	"{\n"
	"	vec3 normal;\n"
	"	normal.xy = gl_PointCoord.xy*2.0 - 1.0;\n"
	"	float r2 = dot(normal.xy, normal.xy);\n"
	"	if(r2 > 1.0) discard;\n"
	"	float nearness = sqrt(1.0 - r2);\n"
	"	normal.z = nearness;\n"
	"	vec4 pixelPosition = vec4(eyePosition + normal*pointRadius, 1.0);\n"
	"	vec4 clipSpacePosition = projectionMatrix * pixelPosition;\n"
	"	float depth = clipSpacePosition.z / clipSpacePosition.w;\n"
	"	float thickness = nearness * 0.03;\n"
	//"	Color = depth;\n"
	"	Color = vec4(vec3(depth),1.0f);\n"
	"	gl_FragDepth = depth;\n"
	"}";

const char fullScreenTextureVertexShader[] =
		"#version 300 es                            \n"
		      "layout(location = 0) in vec4 a_position;   \n"
		      "layout(location = 1) in vec2 a_texCoord;   \n"
		      "out vec2 Tex\n;                       \n"
		      "void main()                                \n"
		      "{                                          \n"
		      "   gl_Position = a_position;               \n"
		      "   Tex = a_texCoord;                \n"
		      "} ";

const char curvatureFlowShader[] =
	"#version 300 es\n"
	"uniform vec2 focalLength;\n"
	"uniform vec2 texelSize;\n"
	"uniform float timeStep;\n"
	"uniform sampler2D depthTexture;\n"
	"uniform float DepthRange_Far;\n"
	"uniform float DepthRange_Near;\n"
	"in vec2 Tex;\n"
	"out vec4 Frag;\n"
	"void main()\n"
	"{\n"
	"	float depth = texture(depthTexture, Tex).x;\n"
	"	if (depth == DepthRange_Far) discard;\n"
	"	vec2 upTexel = Tex.xy + vec2(0, texelSize.y);\n"
	"	vec2 downTexel = Tex.xy + vec2(0, -texelSize.y);\n"
	"	vec2 leftTexel = Tex.xy + vec2(texelSize.x, 0);\n"
	"	vec2 rightTexel = Tex.xy + vec2(-texelSize.x, 0);\n"
	"	float upDepth = texture(depthTexture, upTexel).x;\n"
	"	float downDepth = texture(depthTexture, downTexel).x;\n"
	"	float leftDepth = texture(depthTexture, leftTexel).x;\n"
	"	float rightDepth = texture(depthTexture, rightTexel).x;\n"
	"	float dz_dx = (leftDepth- rightDepth) * 0.5;\n"
	"	float dz_dy = (upDepth - downDepth) * 0.5;\n"
	"	float d2z_dx2 = (leftDepth - 2.0*depth + rightDepth);	//d2z_dx2 == d^2z / dx^2\n"
	"	float d2z_dy2 = (upDepth - 2.0*depth + downDepth);\n"
	"	float Cx = (2.0 / focalLength.x) * texelSize.x;		//texelSize.x == (1.0 / screenWidthInPixels)\n"
	"	float Cy = (2.0 / focalLength.y) * texelSize.y;		//texelSize.y == (1.0 / screenHeightInPixels)\n"
	"	float CxSquared = Cx*Cx;\n"
	"	float CySquared = Cy*Cy;\n"
	"	float D = CySquared*dz_dx*dz_dx + CxSquared*dz_dy*dz_dy + CxSquared*CySquared*depth*depth;\n"
	"	vec2 upLeftTexel = Tex.xy + vec2(texelSize.x, texelSize.y);\n"
	"	vec2 upRightTexel = Tex.xy + vec2(-texelSize.x, texelSize.y);\n"
	"	vec2 downLeftTexel = Tex.xy + vec2(texelSize.x, -texelSize.y);\n"
	"	vec2 downRightTexel = Tex.xy + vec2(-texelSize.x, -texelSize.y);\n"
	"	float upLeftDepth = texture(depthTexture, upLeftTexel).x;\n"
	"	float upRightDepth = texture(depthTexture, upRightTexel).x;\n"
	"	float downLeftDepth = texture(depthTexture, downLeftTexel).x;\n"
	"	float downRightDepth = texture(depthTexture, downRightTexel).x;\n"
	"	float d2z_dxdy = (upLeftDepth - downLeftDepth - upRightDepth + downRightDepth) * 0.25;\n"
	"	float d2z_dydx = d2z_dxdy;\n"
	"	float dD_dx;\n"
	"	float dD_dy;\n"
	"	dD_dx = 2.0*CySquared*d2z_dx2*dz_dx + 2.0*CxSquared*d2z_dxdy*dz_dy + 2.0*CxSquared*CySquared*dz_dx*depth;\n"
	"	dD_dy = 2.0*CySquared*d2z_dydx*dz_dx + 2.0*CxSquared*d2z_dy2*dz_dy + 2.0*CxSquared*CySquared*dz_dy*depth;\n"
	"	float Ex = 0.5*dz_dx*dD_dx - d2z_dx2*D;\n"
	"	float Ey = 0.5*dz_dy*dD_dy - d2z_dy2*D;\n"
	"	float doubleH = (Cy*Ex + Cx*Ey) / pow(D, 1.5);\n"
	"	float H = doubleH*0.5;	//H should be in [-1, 1]\n"
	"	float result;"
	"	result = depth - H*timeStep;\n"
	"	gl_FragDepth = result;\n"
	"	float curvature = H;\n"
	//"	float r = curvature;\n"
	//"	float g = curvature;\n"
	/*
	" 	if(curvature < 0.0f)\n"
	"		r =  abs(curvature);\n"
	"	else\n"
	"		r = 0.0;\n"
	" 	if(curvature < 0.0f)\n"
	"		g =  abs(curvature);\n"
	"	else\n"
	"		g = 0.0;\n"
	//*/
	//"	Frag = vec4( vec3( r, g, 0.0f ), 1.0 );\n"
	//"if(result < 0.0000001f) Frag = vec4( vec3(1.0f), 1.0 );"
	//"else Frag = vec4(vec3(1.0f), 1.0);"
	"	Frag = vec4( vec3(result), 1.0 );\n"
	"}";

const char bilateralFilter1dFragmentShader_depth[] =
	"#version 300 es\n"
	"uniform float texelSize;\n"
	"uniform float filterRadiusPixels;\n"
	"uniform float blurScale;\n"
	"uniform float blurDepthFalloff;\n"
	"uniform vec2 blurDirection;\n"
	"uniform sampler2D depthTexture;\n"
	"uniform float DepthRange_Far;\n"
	"uniform float DepthRange_Near;\n"
	"in vec2 Tex;\n"
	"out vec4 Frag;\n"
	"void main()\n"
	"{\n"
	"	float depth = texture(depthTexture, Tex).x;\n"
	"	if (depth == DepthRange_Far) discard;\n"
	"	float sum = 0.0;\n"
	"	float wsum = 0.0;\n"
	"	for(float x = -filterRadiusPixels; x <= filterRadiusPixels; x += 1.0)\n"
	"	{\n"
	"		float neighborDepth = texture(depthTexture, Tex.xy + blurDirection*texelSize*x).x;\n"
	"		float r = x * blurScale;\n"
	"		float w = exp(-r*r);\n"
	"		float r2 = (neighborDepth - depth) * blurDepthFalloff;\n"
	"		float g = exp(-r2*r2);\n"
	"		sum += neighborDepth * w * g;\n"
	"		wsum += w * g;\n"
	"	}\n"
	"	if(wsum > 0.0) sum /= wsum;\n"
	"	Frag = vec4(vec3(sum),1.0f);\n"
	"	gl_FragDepth = sum;\n"
	"}";

const char bilateralFilter1dFragmentShader_alpha[] =
	"#version 300 es\n"
	"uniform float texelSize;\n"
	"uniform float filterRadiusPixels;\n"
	"uniform float blurScale;\n"
	"uniform float blurDepthFalloff;\n"
	"uniform vec2 blurDirection;\n"
	"uniform sampler2D alphaTexture;\n"
	"in vec2 Tex;\n"
	"out vec4 Frag;\n"
	"void main()\n"
	"{\n"
	"	float depth = texture(alphaTexture, Tex).a;\n"
	"	float sum = 0.0;\n"
	"	float wsum = 0.0;\n"
	"	for(float x = -filterRadiusPixels; x <= filterRadiusPixels; x += 1.0)\n"
	"	{\n"
	"		float neighborDepth = texture(alphaTexture, Tex.xy + blurDirection*texelSize*x).a;\n"
	"		float r = x * blurScale;\n"
	"		float w = exp(-r*r);\n"
	"		float r2 = (neighborDepth - depth) * blurDepthFalloff;\n"
	"		float g = exp(-r2*r2);\n"
	"		sum += neighborDepth * w * g;\n"
	"		wsum += w * g;\n"
	"	}\n"
	"	if(wsum > 0.0) sum /= wsum;\n"
	"	Frag = vec4( vec3(sum), sum );\n"
	"}";


const char absorptionAndTransparencyFragmentShader[] =
	"#version 300 es\n"
	"uniform vec3 absorption;\n"
	"uniform sampler2D thicknessTexture;\n"
	"in vec2 Tex;\n"
	"out vec4 Frag;\n"
	"void main()\n"
	"{\n"
	"	float thickness = texture(thicknessTexture, Tex).a;\n"
	"	Frag = vec4( exp(-absorption.x * thickness), exp(-absorption.y * thickness), exp(-absorption.z * thickness), thickness );\n"
	"}";

const char generateSurfaceFragmentShader[] =
	"#version 300 es\n"
	"vec3 getEyePos(sampler2D depthTexture, vec2 texCoord, mat4 projectionMatrix)\n"
	"{\n"
	"	float depth = texture(depthTexture, texCoord).x;\n"
	"	vec4 unprojectedPosition = inverse(projectionMatrix) * vec4(texCoord.xy*2.0 - 1.0, depth, 1.0);\n"
	"	vec3 eyePosition = unprojectedPosition.xyz / unprojectedPosition.w;\n"
	"	return eyePosition;\n"
	"}\n"
	"uniform mat4 depthProjectionMatrix;\n"
	"uniform vec2 texelSize;\n"
	"uniform sampler2D depthTexture;\n"
	"uniform sampler2D absorptionAndTransparencyTexture;\n"
	"uniform float DepthRange_Far;\n"
	"uniform float DepthRange_Near;\n"
	"in vec2 Tex;\n"
	"in vec4 Color;\n"
	"out vec4 Frag;\n"
	"void main()\n"
	"{\n"
	"	float depth = texture(depthTexture, Tex).x;\n"
	"	if (depth == DepthRange_Far) Frag = vec4(0.0f,0.0f,0.0f,0.0f);\n"
	"	vec3 eyePosition = getEyePos(depthTexture, Tex.xy, depthProjectionMatrix);\n"
	"	vec3 ddx = getEyePos(depthTexture, Tex.xy + vec2(texelSize.x, 0), depthProjectionMatrix) - eyePosition;\n"
	"	vec3 ddx2 = eyePosition - getEyePos(depthTexture, Tex.xy + vec2(-texelSize.x, 0), depthProjectionMatrix);\n"
	"	if( abs(ddx.z) > abs(ddx2.z) ) ddx = ddx2;\n"
	"	vec3 ddy = getEyePos(depthTexture, Tex.xy + vec2(0, texelSize.y), depthProjectionMatrix) - eyePosition;\n"
	"	vec3 ddy2 = eyePosition - getEyePos(depthTexture, Tex.xy + vec2(0, -texelSize.y), depthProjectionMatrix);\n"
	"	if( abs(ddy.z) > abs(ddy2.z) ) ddy = ddy2;\n"
	"	vec3 normal = normalize( cross(ddx, ddy) );\n"
	"	const vec3 LIGHT_DIRECTION = vec3(0.577, 0.577, 0.577);\n"
	"	const float SHININESS = 40.0;\n"
	"	float diffuse = max( 0.0, dot(normal, LIGHT_DIRECTION) );\n"
	"	vec3 v = normalize(-eyePosition);\n"
	"	vec3 h = normalize(LIGHT_DIRECTION + v);\n"
	"	float specular = pow( max(0.0, dot(normal, h)), SHININESS );\n"
	"	vec4 absorptionAndTransparency = texture(absorptionAndTransparencyTexture, Tex);\n"
	"	const float MINIMUM_ALPHA = 0.70;\n"
	"	vec3 color = Color.xyz * absorptionAndTransparency.xyz * diffuse + specular;\n"
	"	float alpha = MINIMUM_ALPHA + absorptionAndTransparency.w * (1.0 - MINIMUM_ALPHA);\n"
	"	Frag = vec4(color, alpha);\n"
	"	gl_FragDepth = ((DepthRange_Far - DepthRange_Near)*depth + DepthRange_Near + DepthRange_Far) * 0.5;\n"
	"}";

const char blitFragmentShader[] =
	"#version 300 es	\n"
	"uniform sampler2D rgbaTexture;\n"
	"uniform sampler2D depthTexture;\n"
	"in vec2 Tex;\n"
	"in vec4 Color;\n"
	"out vec4 Frag;\n"
	"void main()\n"
	"{\n"
	"	Frag = texture(rgbaTexture, Tex);\n"
	"	gl_FragDepth = texture(depthTexture, Tex).x;\n"
	"}";

#endif
