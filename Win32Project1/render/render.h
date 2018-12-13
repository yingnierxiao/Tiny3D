/*
 * render.h
 *
 *  Created on: 2017-4-7
 *      Author: a
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "../shader/shadermanager.h"
#include "../framebuffer/framebuffer.h"
#include "drawcall.h"

#define TEXTURE_2D 1
#define TEXTURE_2D_ARRAY 2
#define TEXTURE_CUBE 3

class Render {
public:
	static float MaxAniso;
private:
	void initEnvironment();
public: // Global render state
	bool enableCull;
	int cullMode;
	bool drawLine;
	bool enableDepthTest;
	int depthTestMode;
	bool enableAlphaTest;
	int alphaTestMode;
	float alphaThreshold;
	bool enableBlend;
	COLOR clearColor;
	Shader* currentShader;
public:
	int viewWidth,viewHeight;
	ShaderManager* shaders;
	std::map<uint, std::map<uint, uint>*> textureTypeSlots;
public:
	Render();
	~Render();
	void clearFrame(bool clearColor,bool clearDepth,bool clearStencil);
	void setState(const RenderState* state);
	void setDepthTest(bool enable,int testMode);
	void setAlphaTest(bool enable, int testMode, float threshold);
	void setCullState(bool enable);
	void setCullMode(int mode);
	void setDrawLine(bool line);
	void setBlend(bool enable);
	void setClearColor(float r,float g,float b,float a);
	void setViewPort(int width,int height);
	void resize(int width, int height, Camera* mainCamera, Camera* reflectCamera);
	Shader* findShader(const char* shader);
	void useShader(Shader* shader);
	void draw(Camera* camera, Drawcall* drawcall, RenderState* state);
	void finishDraw();
	void setFrameBuffer(FrameBuffer* framebuffer);
	void setColorMask(bool r, bool g, bool b, bool a);
	void useTexture(uint type, uint slot, uint texid);
	void clearTextureSlots();

	void setShaderInt(Shader* shader, const char* param, int value) { useShader(shader); shader->setInt(param, value); }
	void setShaderSampler(Shader* shader, const char* param, int value) { useShader(shader); shader->setSampler(param, value); }
	void setShaderFloat(Shader* shader, const char* param, float value) { useShader(shader); shader->setFloat(param, value); }
	void setShaderVec2(Shader* shader, const char* param, float x, float y) { useShader(shader); shader->setVector2(param, x, y); }
	void setShaderVec3(Shader* shader, const char* param, float x, float y, float z) { useShader(shader); shader->setVector3(param, x, y, z); }
	void setShaderVec4(Shader* shader, const char* param, float x, float y, float z, float w) { useShader(shader); shader->setVector4(param, x, y, z, w); }
	void setShaderMat4(Shader* shader, const char* param, float* matrix) { useShader(shader); shader->setMatrix4(param, matrix); }
	void setShaderMat4(Shader* shader, const char* param, int count, float* matrices) { useShader(shader); shader->setMatrix4(param, count, matrices); }
	void setShaderMat3x4(Shader* shader, const char* param, int count, float* matrices) { useShader(shader); shader->setMatrix3x4(param, count, matrices); }
	void setShaderMat3(Shader* shader, const char* param, float* matrix) { useShader(shader); shader->setMatrix3(param, matrix); }
	void setShaderMat3(Shader* shader, const char* param, int count, float* matrices) { useShader(shader); shader->setMatrix3(param, count, matrices); }
};


#endif /* RENDER_H_ */
