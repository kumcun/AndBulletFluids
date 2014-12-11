/* ScreenSpaceFluidRendererGL.cpp
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
#include "ScreenSpaceFluidRendererGL.h"

#include <cstdlib> 	//exit()
#include <cstdio>


#define STRINGIFY(A) #A
#define PI 3.1415926535897932384626433832795f

void esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz)
{
	result->m[0][0] *= sx;
	result->m[0][1] *= sx;
	result->m[0][2] *= sx;
	result->m[0][3] *= sx;

	result->m[1][0] *= sy;
	result->m[1][1] *= sy;
	result->m[1][2] *= sy;
	result->m[1][3] *= sy;

	result->m[2][0] *= sz;
	result->m[2][1] *= sz;
	result->m[2][2] *= sz;
	result->m[2][3] *= sz;
}

void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
	result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
	result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
	result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
	result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat sinAngle, cosAngle;
	GLfloat mag = sqrtf(x * x + y * y + z * z);

	sinAngle = sinf(angle * PI / 180.0f);
	cosAngle = cosf(angle * PI / 180.0f);

	if (mag > 0.0f)
	{
		GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
		GLfloat oneMinusCos;
		ESMatrix rotMat;

		x /= mag;
		y /= mag;
		z /= mag;

		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * sinAngle;
		ys = y * sinAngle;
		zs = z * sinAngle;
		oneMinusCos = 1.0f - cosAngle;

		rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
		rotMat.m[0][1] = (oneMinusCos * xy) - zs;
		rotMat.m[0][2] = (oneMinusCos * zx) + ys;
		rotMat.m[0][3] = 0.0F;

		rotMat.m[1][0] = (oneMinusCos * xy) + zs;
		rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
		rotMat.m[1][2] = (oneMinusCos * yz) - xs;
		rotMat.m[1][3] = 0.0F;

		rotMat.m[2][0] = (oneMinusCos * zx) - ys;
		rotMat.m[2][1] = (oneMinusCos * yz) + xs;
		rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
		rotMat.m[2][3] = 0.0F;

		rotMat.m[3][0] = 0.0F;
		rotMat.m[3][1] = 0.0F;
		rotMat.m[3][2] = 0.0F;
		rotMat.m[3][3] = 1.0F;

		esMatrixMultiply(result, &rotMat, result);
	}
}

void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;
	ESMatrix    frust;

	if ((nearZ <= 0.0f) || (farZ <= 0.0f) ||
		(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f))
	{
		return;
	}

	frust.m[0][0] = 2.0f * nearZ / deltaX;
	frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

	frust.m[1][1] = 2.0f * nearZ / deltaY;
	frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

	frust.m[2][0] = (right + left) / deltaX;
	frust.m[2][1] = (top + bottom) / deltaY;
	frust.m[2][2] = -(nearZ + farZ) / deltaZ;
	frust.m[2][3] = -1.0f;

	frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
	frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

	esMatrixMultiply(result, &frust, result);
}

void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ)
{
	GLfloat frustumW, frustumH;

	frustumH = tanf(fovy / 360.0f * PI) * nearZ;
	frustumW = frustumH * aspect;

	esFrustum(result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

void esOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;
	ESMatrix    ortho;

	if ((deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f))
	{
		return;
	}

	esMatrixLoadIdentity(&ortho);
	ortho.m[0][0] = 2.0f / deltaX;
	ortho.m[3][0] = -(right + left) / deltaX;
	ortho.m[1][1] = 2.0f / deltaY;
	ortho.m[3][1] = -(top + bottom) / deltaY;
	ortho.m[2][2] = -2.0f / deltaZ;
	ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

	esMatrixMultiply(result, &ortho, result);
}

void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB)
{
	ESMatrix    tmp;
	int         i;

	for (i = 0; i < 4; i++)
	{
		tmp.m[i][0] = (srcA->m[i][0] * srcB->m[0][0]) +
			(srcA->m[i][1] * srcB->m[1][0]) +
			(srcA->m[i][2] * srcB->m[2][0]) +
			(srcA->m[i][3] * srcB->m[3][0]);

		tmp.m[i][1] = (srcA->m[i][0] * srcB->m[0][1]) +
			(srcA->m[i][1] * srcB->m[1][1]) +
			(srcA->m[i][2] * srcB->m[2][1]) +
			(srcA->m[i][3] * srcB->m[3][1]);

		tmp.m[i][2] = (srcA->m[i][0] * srcB->m[0][2]) +
			(srcA->m[i][1] * srcB->m[1][2]) +
			(srcA->m[i][2] * srcB->m[2][2]) +
			(srcA->m[i][3] * srcB->m[3][2]);

		tmp.m[i][3] = (srcA->m[i][0] * srcB->m[0][3]) +
			(srcA->m[i][1] * srcB->m[1][3]) +
			(srcA->m[i][2] * srcB->m[2][3]) +
			(srcA->m[i][3] * srcB->m[3][3]);
	}

	memcpy(result, &tmp, sizeof (ESMatrix));
}

void esMatrixLoadIdentity(ESMatrix *result)
{
	memset(result, 0x0, sizeof (ESMatrix));
	result->m[0][0] = 1.0f;
	result->m[1][1] = 1.0f;
	result->m[2][2] = 1.0f;
	result->m[3][3] = 1.0f;
}

void esMatrixLookAt(ESMatrix *result,
	float posX, float posY, float posZ,
	float lookAtX, float lookAtY, float lookAtZ,
	float upX, float upY, float upZ)
{
	float axisX[3], axisY[3], axisZ[3];
	float length;

	// axisZ = lookAt - pos
	axisZ[0] = lookAtX - posX;
	axisZ[1] = lookAtY - posY;
	axisZ[2] = lookAtZ - posZ;

	// normalize axisZ
	length = sqrtf(axisZ[0] * axisZ[0] + axisZ[1] * axisZ[1] + axisZ[2] * axisZ[2]);

	if (length != 0.0f)
	{
		axisZ[0] /= length;
		axisZ[1] /= length;
		axisZ[2] /= length;
	}

	// axisX = up X axisZ
	axisX[0] = upY * axisZ[2] - upZ * axisZ[1];
	axisX[1] = upZ * axisZ[0] - upX * axisZ[2];
	axisX[2] = upX * axisZ[1] - upY * axisZ[0];

	// normalize axisX
	length = sqrtf(axisX[0] * axisX[0] + axisX[1] * axisX[1] + axisX[2] * axisX[2]);

	if (length != 0.0f)
	{
		axisX[0] /= length;
		axisX[1] /= length;
		axisX[2] /= length;
	}

	// axisY = axisZ x axisX
	axisY[0] = axisZ[1] * axisX[2] - axisZ[2] * axisX[1];
	axisY[1] = axisZ[2] * axisX[0] - axisZ[0] * axisX[2];
	axisY[2] = axisZ[0] * axisX[1] - axisZ[1] * axisX[0];

	// normalize axisY
	length = sqrtf(axisY[0] * axisY[0] + axisY[1] * axisY[1] + axisY[2] * axisY[2]);

	if (length != 0.0f)
	{
		axisY[0] /= length;
		axisY[1] /= length;
		axisY[2] /= length;
	}

	memset(result, 0x0, sizeof (ESMatrix));

	result->m[0][0] = -axisX[0];
	result->m[0][1] = axisY[0];
	result->m[0][2] = -axisZ[0];

	result->m[1][0] = -axisX[1];
	result->m[1][1] = axisY[1];
	result->m[1][2] = -axisZ[1];

	result->m[2][0] = -axisX[2];
	result->m[2][1] = axisY[2];
	result->m[2][2] = -axisZ[2];

	// translate (-posX, -posY, -posZ)
	result->m[3][0] = axisX[0] * posX + axisX[1] * posY + axisX[2] * posZ;
	result->m[3][1] = -axisY[0] * posX - axisY[1] * posY - axisY[2] * posZ;
	result->m[3][2] = axisZ[0] * posX + axisZ[1] * posY + axisZ[2] * posZ;
	result->m[3][3] = 1.0f;
}




ScreenSpaceFluidRendererGL::ScreenSpaceFluidRendererGL(int screenWidth, int screenHeight)
{
	//
	GLuint shader_count = 0;

	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	m_basicShader = compileProgram(vShaderStr, fShaderStr);
	m_generateDepthShader = compileProgram(generateDepthVertexShader, generateDepthFragmentShader);
	m_blurDepthShader = compileProgram(fullScreenTextureVertexShader, bilateralFilter1dFragmentShader_depth);
	m_curvatureFlowShader = compileProgram(fullScreenTextureVertexShader, curvatureFlowShader);

	m_blurThickShader = compileProgram(fullScreenTextureVertexShader, bilateralFilter1dFragmentShader_alpha);
	m_absorptionAndTransparencyShader = compileProgram(fullScreenTextureVertexShader, absorptionAndTransparencyFragmentShader);

	m_generateSurfaceShader = compileProgram(fullScreenTextureVertexShader, generateSurfaceFragmentShader);
	m_blitShader = compileProgram(fullScreenTextureVertexShader, blitFragmentShader);

	if( (m_generateDepthShader != 0) &&
				(m_blurDepthShader != 0) &&
				(m_curvatureFlowShader != 0) &&
				(m_blurThickShader != 0) &&
				(m_absorptionAndTransparencyShader != 0) &&
				(m_blitShader != 0) &&
				(m_generateDepthShader != 0))
		LOGI("shader compile success");
	//
	glGenBuffers(1, &m_positionVertexBuffer);
	//
	LOGI("Width:%d, Height:%d\n ",screenWidth, screenHeight);
	m_frameBuffer.initialize(screenWidth, screenHeight);
	m_tempDepthTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_DEPTH_TEXTURE);
	m_depthTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_RGBA_TEXTURE);
	m_blurredDepthTexturePass1 = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_RGBA_TEXTURE);
	m_blurredDepthTexturePass2 = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_RGBA_TEXTURE);
	m_surfaceDepthTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_DEPTH_TEXTURE);

	m_thickTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_ALPHA_TEXTURE);
	m_blurredThickTexturePass1 = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_ALPHA_TEXTURE);
	m_blurredThickTexturePass2 = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_ALPHA_TEXTURE);
	m_absorptionAndTransparencyTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_RGBA_TEXTURE);
	m_tempColorTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_RGBA_TEXTURE);
	m_surfaceColorTexture = m_frameBuffer.createFrameBufferTexture(FrameBufferGL::FBT_RGBA_TEXTURE);
}
ScreenSpaceFluidRendererGL::~ScreenSpaceFluidRendererGL()
{
	glDeleteShader(m_basicShader);
	glDeleteShader(m_generateDepthShader);
	glDeleteShader(m_blurDepthShader);

	glDeleteShader(m_blurThickShader);
	glDeleteShader(m_absorptionAndTransparencyShader);

	glDeleteShader(m_generateSurfaceShader);
	glDeleteShader(m_blitShader);

	//
	glDeleteBuffers(1, &m_positionVertexBuffer);

	//
	m_frameBuffer.deactivate();
}

void ScreenSpaceFluidRendererGL::render(const btAlignedObjectArray<btVector3>& particlePositions, float sphereRadius,
										float r, float g, float b, float absorptionR, float absorptionG, float absorptionB,
										ESMatrix* _projMat, ESMatrix* _viewMat)
{
	projMat = _projMat;
	viewMat = _viewMat;
/*
	btAssert( sizeof(btVector3) == 16 );

	if( !particlePositions.size() ) return;
//*/
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	int numParticles = particlePositions.size();

	float screenWidth = static_cast<float>( m_frameBuffer.getWidth() );
	float screenHeight = static_cast<float>( m_frameBuffer.getHeight() );
	float lesserDistance = (screenWidth > screenHeight) ?  screenHeight : screenWidth;

	bool renderingResolutionDiffers = ( m_windowWidth != m_frameBuffer.getWidth() || m_windowHeight != m_frameBuffer.getHeight() );
	if(renderingResolutionDiffers)
		glViewport( 0, 0, m_frameBuffer.getWidth(), m_frameBuffer.getHeight() );
/*
	glUseProgram(m_basicShader);
	renderFullScreenTexture(m_blurredThickTexturePass2, 0, 0);
//*/
	render_stage1_generateDepthTexture(particlePositions, sphereRadius);

	const bool USE_CURVATURE_FLOW = true;
	if(USE_CURVATURE_FLOW) render_stage2_blurDepthTextureCurvatureFlow();
	else render_stage2_blurDepthTextureBilateral();

	render_stage3_generateThickTexture(particlePositions, sphereRadius);
	render_stage4_blurThickTexture();

	render_stage5_generateAbsorptionAndTransparencyTexture(absorptionR, absorptionG, absorptionB);
	render_stage6_generateSurfaceTexture();

	//Blit results to the main/window frame buffer
	if(renderingResolutionDiffers) glViewport(0, 0, m_windowWidth, m_windowHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(m_blitShader);
	glUniform1i( glGetUniformLocation(m_blitShader, "rgbaTexture"), 0 );
	glUniform1i( glGetUniformLocation(m_blitShader, "depthTexture"), 1 );
		renderFullScreenTexture(m_surfaceColorTexture, m_surfaceDepthTexture, 0);
	glDisable(GL_BLEND);
	//*/
	glUseProgram(0);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ScreenSpaceFluidRendererGL::render_stage1_generateDepthTexture(const btAlignedObjectArray<btVector3>& particlePositions, float sphereRadius)
{
	glUseProgram(m_generateDepthShader);
	{
		int numParticles = particlePositions.size();

		float screenWidth = static_cast<float>( m_frameBuffer.getWidth() );
		float screenHeight = static_cast<float>( m_frameBuffer.getHeight() );
		float lesserDistance = (screenWidth > screenHeight) ?  screenHeight : screenWidth;
		glUniform1f( glGetUniformLocation(m_generateDepthShader, "pointScale"), lesserDistance );
		glUniform1f( glGetUniformLocation(m_generateDepthShader, "pointRadius"), sphereRadius );
		glUniformMatrix4fv(glGetUniformLocation(m_generateDepthShader, "mvMat"), 1, false, (GLfloat*)viewMat);
		glUniformMatrix4fv(glGetUniformLocation(m_generateDepthShader, "pMat"), 1, false, (GLfloat*)projMat);

		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, &particlePositions[0]);
		glEnableVertexAttribArray(0);

		m_frameBuffer.attachAndSetRenderTargets(m_depthTexture, m_tempDepthTexture);
			glDrawArrays(GL_POINTS, 0, numParticles);
		m_frameBuffer.detachAndUseDefaultFrameBuffer();

		glDisableVertexAttribArray(0);
	}
	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::render_stage2_blurDepthTextureCurvatureFlow()
{
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_curvatureFlowShader);

	const float HORIZONTAL_FOV_RADIANS = SIMD_RADS_PER_DEG * 90.0f;
	const float VERTICAL_FOV_RADIANS = SIMD_RADS_PER_DEG * 75.0f;

	const float SCREEN_WIDTH = 1.0f;
	const float SCREEN_HEIGHT = 0.75f;

	//	focal length probably incorrect
	//const float FOCAL_LENGTH_X = (SCREEN_WIDTH * 0.5f) / btTan(HORIZONTAL_FOV_RADIANS * 0.5f);
	//const float FOCAL_LENGTH_Y = (SCREEN_HEIGHT * 0.5f) / btTan(VERTICAL_FOV_RADIANS * 0.5f);

	const float FOCAL_LENGTH_X = 0.5f;
	const float FOCAL_LENGTH_Y = 0.5f;

	float texelSizeX;
	float texelSizeY;
	texelSizeX = 0.1;
	texelSizeY = 0.1;
	//texelSizeX = 1.0f / static_cast<float>( m_frameBuffer.getWidth());
	//texelSizeY = 1.0f / static_cast<float>( m_frameBuffer.getHeight());

	glUniform2f( glGetUniformLocation(m_curvatureFlowShader, "focalLength"), FOCAL_LENGTH_X, FOCAL_LENGTH_Y );
	glUniform2f( glGetUniformLocation(m_curvatureFlowShader, "texelSize"), texelSizeX, texelSizeY );
	//glUniform1f( glGetUniformLocation(m_curvatureFlowShader, "timeStep"), 0.001f );
	glUniform1f( glGetUniformLocation(m_curvatureFlowShader, "timeStep"), 0.05f );
	glUniform1i( glGetUniformLocation(m_curvatureFlowShader, "depthTexture"), 0 );

	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Near"), 0.0f);
	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Far"), 1.0f);

	GLuint depthTextures[2] = { m_blurredDepthTexturePass2, m_depthTexture };

	//Since textures are swapped, apply an odd number of iterations to ensure that the final result is in depthTextures[0]
	const int ITERATIONS = 4;
	const int ACTUAL_ITERATIONS = (ITERATIONS % 2) ? ITERATIONS : ITERATIONS + 1;
	for(int i = 0; i < ACTUAL_ITERATIONS; ++i)
	{
		m_frameBuffer.attachAndSetRenderTargets(depthTextures[0], m_tempDepthTexture);
			renderFullScreenTexture(depthTextures[1], 0, 0);
		m_frameBuffer.detachAndUseDefaultFrameBuffer();

		//Final output will be in depthTextures[0] == m_blurredDepthTexturePass2
		GLuint swap = depthTextures[0];
		depthTextures[0] = depthTextures[1];
		depthTextures[1] = swap;
	}

	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::render_stage2_blurDepthTextureBilateral()
{
	glDepthFunc(GL_ALWAYS);
	glUseProgram(m_blurDepthShader);

	//First pass blurs along the x-axis
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "texelSize"), 1.0f / static_cast<float>( m_frameBuffer.getWidth() ) );
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "filterRadiusPixels"), 32.0f );
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "blurScale"), 0.17f );
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "blurDepthFalloff"), 24.0f );
	glUniform2f( glGetUniformLocation(m_blurDepthShader, "blurDirection"), 1.0f, 0.0f );
	glUniform1i( glGetUniformLocation(m_blurDepthShader, "depthTexture"), 0 );

	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Near"), 0.0f);
	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Far"), 1.0f);

	m_frameBuffer.attachAndSetRenderTargets(m_tempColorTexture, m_blurredDepthTexturePass1);
		renderFullScreenTexture(m_depthTexture, 0, 0);
	m_frameBuffer.detachAndUseDefaultFrameBuffer();

	//Second pass blurs along the y-axis
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "texelSize"), 1.0f / static_cast<float>( m_frameBuffer.getHeight() ) );
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "filterRadiusPixels"), 32.0f );
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "blurScale"), 0.17f );
	glUniform1f( glGetUniformLocation(m_blurDepthShader, "blurDepthFalloff"), 24.0f );
	glUniform2f( glGetUniformLocation(m_blurDepthShader, "blurDirection"), 0.0f, 1.0f );
	glUniform1i( glGetUniformLocation(m_blurDepthShader, "depthTexture"), 0 );

	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Near"), 0.0f);
	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Far"), 1.0f);

	m_frameBuffer.attachAndSetRenderTargets(m_tempColorTexture, m_blurredDepthTexturePass2);
		renderFullScreenTexture(m_blurredDepthTexturePass1, 0, 0);
	m_frameBuffer.detachAndUseDefaultFrameBuffer();

	glDepthFunc(GL_LESS);
	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::render_stage3_generateThickTexture(const btAlignedObjectArray<btVector3>& particlePositions, float sphereRadius)
{
	glUseProgram(m_generateDepthShader);

	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	{
		int numParticles = particlePositions.size();

		float screenWidth = static_cast<float>( m_frameBuffer.getWidth() );
		float screenHeight = static_cast<float>( m_frameBuffer.getHeight() );
		float lesserDistance = (screenWidth > screenHeight) ?  screenHeight : screenWidth;

		glUniform1f( glGetUniformLocation(m_generateDepthShader, "pointScale"), lesserDistance );
		glUniform1f( glGetUniformLocation(m_generateDepthShader, "pointRadius"), sphereRadius );
		glUniformMatrix4fv(glGetUniformLocation(m_generateDepthShader, "mvMat"), 1, false, (GLfloat*)viewMat);
		glUniformMatrix4fv(glGetUniformLocation(m_generateDepthShader, "pMat"), 1, false, (GLfloat*)projMat);

		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, &particlePositions[0]);
		glEnableVertexAttribArray(0);
		m_frameBuffer.attachAndSetRenderTargets(m_thickTexture, m_tempDepthTexture);
			glDrawArrays(GL_POINTS, 0, numParticles);
		m_frameBuffer.detachAndUseDefaultFrameBuffer();
		glDisableVertexAttribArray(0);
	}
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::render_stage4_blurThickTexture()
{
	glDepthFunc(GL_ALWAYS);
	glUseProgram(m_blurThickShader);

	//First pass blurs along the x-axis
	glUniform1f( glGetUniformLocation(m_blurThickShader, "texelSize"), 1.0f / static_cast<float>( m_frameBuffer.getWidth() ) );
	glUniform1f( glGetUniformLocation(m_blurThickShader, "filterRadiusPixels"), 32.0f );
	glUniform1f( glGetUniformLocation(m_blurThickShader, "blurScale"), 0.1f );
	glUniform1f( glGetUniformLocation(m_blurThickShader, "blurDepthFalloff"), 6.0f );
	glUniform2f( glGetUniformLocation(m_blurThickShader, "blurDirection"), 1.0f, 0.0f );
	glUniform1i( glGetUniformLocation(m_blurThickShader, "alphaTexture"), 0 );

	m_frameBuffer.attachAndSetRenderTargets(m_blurredThickTexturePass1, m_tempDepthTexture);
		renderFullScreenTexture(m_thickTexture, 0, 0);
	m_frameBuffer.detachAndUseDefaultFrameBuffer();

	//Second pass blurs along the y-axis
	glUniform1f( glGetUniformLocation(m_blurThickShader, "texelSize"), 1.0f / static_cast<float>( m_frameBuffer.getHeight() ) );
	glUniform1f( glGetUniformLocation(m_blurThickShader, "filterRadiusPixels"), 32.0f );
	glUniform1f( glGetUniformLocation(m_blurThickShader, "blurScale"), 0.1f );
	glUniform1f( glGetUniformLocation(m_blurThickShader, "blurDepthFalloff"), 6.0f );
	glUniform2f( glGetUniformLocation(m_blurThickShader, "blurDirection"), 0.0f, 1.0f );
	glUniform1i( glGetUniformLocation(m_blurThickShader, "alphaTexture"), 0 );

	m_frameBuffer.attachAndSetRenderTargets(m_blurredThickTexturePass2, m_tempDepthTexture);
		renderFullScreenTexture(m_blurredThickTexturePass1, 0, 0);
	m_frameBuffer.detachAndUseDefaultFrameBuffer();

	glDepthFunc(GL_LESS);
	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::render_stage5_generateAbsorptionAndTransparencyTexture(float absorptionR, float absorptionG, float absorptionB)
{
	glDepthFunc(GL_ALWAYS);
	glUseProgram(m_absorptionAndTransparencyShader);

	glUniform3f( glGetUniformLocation(m_absorptionAndTransparencyShader, "absorption"), absorptionR, absorptionG, absorptionB);
	glUniform1i( glGetUniformLocation(m_absorptionAndTransparencyShader, "thicknessTexture"), 0 );

	m_frameBuffer.attachAndSetRenderTargets(m_absorptionAndTransparencyTexture, m_tempDepthTexture);
		renderFullScreenTexture(m_blurredThickTexturePass2, 0, 0);
	m_frameBuffer.detachAndUseDefaultFrameBuffer();

	glDepthFunc(GL_LESS);
	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::render_stage6_generateSurfaceTexture()
{
	float texelSize_x = 1.0f / static_cast<float>( m_frameBuffer.getWidth() );
	float texelSize_y = 1.0f / static_cast<float>( m_frameBuffer.getHeight() );

	glUseProgram(m_generateSurfaceShader);
	glUniformMatrix4fv( glGetUniformLocation(m_generateSurfaceShader, "depthProjectionMatrix"), 1, false, m_depthProjectionMatrix );
	glUniform2f( glGetUniformLocation(m_generateSurfaceShader, "texelSize"), texelSize_x, texelSize_y );
	glUniform1i( glGetUniformLocation(m_generateSurfaceShader, "depthTextureBlurred"), 0 );
	glUniform1i( glGetUniformLocation(m_generateSurfaceShader, "absorptionAndTransparencyTexture"), 1 );

	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Near"), 0.0f);
	glUniform1f(glGetUniformLocation(m_curvatureFlowShader, "DepthRange_Far"), 1.0f);
	m_frameBuffer.attachAndSetRenderTargets(m_surfaceColorTexture, m_surfaceDepthTexture);
		renderFullScreenTexture(m_blurredDepthTexturePass2, m_absorptionAndTransparencyTexture, 0);
		//renderFullScreenTexture(m_blurredDepthTexturePass2, m_tempColorTexture, 0);		//For display of curvature
	m_frameBuffer.detachAndUseDefaultFrameBuffer();

	glUseProgram(0);
}
void ScreenSpaceFluidRendererGL::renderFullScreenTexture(GLuint texture2d_0, GLuint texture2d_1, GLuint texture2d_2)
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture2d_2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2d_1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2d_0);

	//Render
	{
		//Arranged for GL_TRIANGLE_STRIP; order: Upper Left, Upper Right, Lower Left, Lower Right
		GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,  // Position 0
			                           0.0f,  1.0f,        // TexCoord 0
			                           -1.0f, -1.0f, 0.0f,  // Position 1
			                            0.0f,  0.0f,        // TexCoord 1
			                            1.0f, -1.0f, 0.0f,  // Position 2
			                            1.0f,  0.0f,        // TexCoord 2
			                            1.0f,  1.0f, 0.0f,  // Position 3
			                            1.0f,  1.0f         // TexCoord 3
			                   };
		static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
		glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );
		glEnableVertexAttribArray ( 0 );
		glEnableVertexAttribArray ( 1 );

		glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	}

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint ScreenSpaceFluidRendererGL::compileProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	static  GLuint shader_count = 0;

	++shader_count;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//
	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	const int MAX_STRING_LENGTH = 65536;

	btAlignedObjectArray<char> string;
	string.resize(MAX_STRING_LENGTH);
	char* stringStart = &string[0];

	LOGI("GLProgram %d State :\n",shader_count);
	if (!success){
		glGetProgramInfoLog(program, MAX_STRING_LENGTH, 0, stringStart);
		LOGI("GL Program Build Log:\n");
		LOGI("%s\n",stringStart);

		glGetShaderInfoLog(vertexShader, MAX_STRING_LENGTH, 0, stringStart);
		LOGI("Vertex Shader Build Log:\n");
		LOGI("%s\n", stringStart);

		glGetShaderInfoLog(fragmentShader, MAX_STRING_LENGTH, 0, stringStart);
		LOGI("Fragment Shader Build Log:\n");
		LOGI("%s\n", stringStart);

		glDeleteProgram(program);
		program = 0;
	}
	//If program was compiled successfully, marks shaders for deletion(not deleted until program is deleted)
	else
		LOGI("GL Program Build Complete\n");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
