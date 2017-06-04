#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "include.h"
#include "texture.h"

class ShadowFrameBuffer
{

public:

	ShadowFrameBuffer();

	~ShadowFrameBuffer();

	void bindGL() const;

	void unbindGL() const;

	TexturePtr getDepthTexture() const;

	GLuint getDepthTextureRaw() const;

private:

	GLuint _FBO;

	// GLuint _depthRenderBuffer;

	GLuint _depthTexture;

};

#endif