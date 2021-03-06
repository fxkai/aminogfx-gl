#ifndef _AMINOIMAGES_H
#define _AMINOIMAGES_H

#include "base_js.h"
#include "gfx.h"
#include "videos.h"

class AminoImageFactory;

/**
 * Amino Image Loader.
 *
 * Convert image binary data to pixel array.
 */
class AminoImage : public AminoJSObject {
public:
    int w = 0;
    int h = 0;
    bool alpha = 0;
    int bpp = 0;

    AminoImage();
    ~AminoImage();

    bool hasImage();
    void destroy() override;
    void destroyAminoImage();
    GLuint createTexture(GLuint textureId);
    static GLuint createTexture(GLuint textureId, char *bufferData, size_t bufferLength, int w, int h, int bpp);

    void imageLoaded(v8::Local<v8::Object> &buffer, int w, int h, bool alpha, int bpp);

    //creation
    static AminoImageFactory* getFactory();

    //init
    static NAN_MODULE_INIT(Init);

private:
    Nan::Persistent<v8::Object> buffer;
    char *bufferData = NULL;
    size_t bufferLength = 0;

    //JS constructor
    static NAN_METHOD(New);

    //JS methods
    static NAN_METHOD(loadImage);
};

/**
 * AminoImage class factory.
 */
class AminoImageFactory : public AminoJSObjectFactory {
public:
    AminoImageFactory(Nan::FunctionCallback callback);

    AminoJSObject* create() override;
};

class AminoTextureFactory;

/**
 * Amino Texture class.
 */
class AminoTexture : public AminoJSObject {
public:
    GLuint *textureIds = NULL;
    int textureCount = 0;
    int activeTexture = -1;
    bool ownTexture = true;
    int w = 0;
    int h = 0;

    AminoTexture();
    ~AminoTexture();

    void destroy() override;
    void destroyAminoTexture(bool destructorCall);

    //creation
    static AminoTextureFactory* getFactory();

    //init
    static v8::Local<v8::FunctionTemplate> GetInitFunction();

    //texture
    GLuint getTexture();

    //video
    void initVideoTexture();
    void videoPlayerInitDone();
    void prepareTexture(GLContext *ctx);
    void fireVideoEvent(std::string event);

private:
    Nan::Callback *callback = NULL;

    //video
    AminoVideoPlayer *videoPlayer = NULL;
    uv_mutex_t videoLock;
    bool videoLockUsed = false;

    void preInit(Nan::NAN_METHOD_ARGS_TYPE info) override;

    //JS constructor
    static NAN_METHOD(New);

    //JS methods
    static NAN_METHOD(LoadTextureFromImage);
    static NAN_METHOD(LoadTextureFromVideo);
    static NAN_METHOD(LoadTextureFromBuffer);
    static NAN_METHOD(LoadTextureFromFont);
    static NAN_METHOD(Destroy);
    static NAN_METHOD(GetMediaTime);
    static NAN_METHOD(GetDuration);
    static NAN_METHOD(GetState);
    static NAN_METHOD(StopPlayback);
    static NAN_METHOD(PausePlayback);
    static NAN_METHOD(ResumePlayback);

    void createTexture(AsyncValueUpdate *update, int state);
    void createVideoTexture(AsyncValueUpdate *update, int state);
    void createTextureFromBuffer(AsyncValueUpdate *update, int state);
    void createTextureFromFont(AsyncValueUpdate *update, int state);

    void initVideoTextureHandler(AsyncValueUpdate *update, int state);
    void handleVideoPlayerInitDone(JSCallbackUpdate *update);
    void handleFireVideoEvent(JSCallbackUpdate *update);
};

/**
 * AminoTexture class factory.
 */
class AminoTextureFactory : public AminoJSObjectFactory {
public:
    AminoTextureFactory(Nan::FunctionCallback callback);

    AminoJSObject* create() override;
};

#endif