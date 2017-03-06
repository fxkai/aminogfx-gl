#ifndef _AMINOVIDEOS_H
#define _AMINOVIDEOS_H

#include "base_js.h"
#include "gfx.h"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
}

//cbx FIXME
#define DEBUG_VIDEOS true

class AminoVideoFactory;
class AminoTexture;

/**
 * Amino Video Loader.
 *
 */
class AminoVideo : public AminoJSObject {
public:
    AminoVideo();
    ~AminoVideo();

    bool hasVideo();

    bool isLocalFile();
    std::string getLocalFile();

    //creation
    static AminoVideoFactory* getFactory();

    //init
    static NAN_MODULE_INIT(Init);

private:
    //JS constructor
    static NAN_METHOD(New);
};

/**
 * AminoImage class factory.
 */
class AminoVideoFactory : public AminoJSObjectFactory {
public:
    AminoVideoFactory(Nan::FunctionCallback callback);

    AminoJSObject* create() override;
};

/**
 * Amino Video Player.
 */
class AminoVideoPlayer {
public:
    AminoVideoPlayer(AminoTexture *texture, AminoVideo *video);
    virtual ~AminoVideoPlayer();

    virtual bool initStream() = 0; //called on main thread
    virtual void init() = 0; //called on OpenGL thread
    virtual void initVideoTexture() = 0;
    virtual void destroy();
    void destroyAminoVideoPlayer();

    bool isReady();
    std::string getLastError();
    void getVideoDimension(int &w, int &h);

protected:
    AminoTexture *texture;
    AminoVideo *video;

    //state
    bool initDone = false;
    bool ready = false;
    bool playing = false;
    bool destroyed = false;
    std::string lastError;

    //settings
    bool loop = true;

    //video
    int videoW = 0;
    int videoH = 0;

    void handlePlaybackDone();
    void handlePlaybackError();

    void handleInitDone(bool ready);
};

/**
 * Demux a video container.
 */
class VideoDemuxer {
public:
    VideoDemuxer();
    virtual ~VideoDemuxer();

    bool init();
    bool loadFile(std::string filename);
    bool initStream();

    bool saveStream(std::string filename);
    bool readFrame();

    std::string getLastError();

private:
    std::string lastError;

    //context
    AVFormatContext *context = NULL;
    AVCodecContext *codecCtx = NULL;

    //stream info
    int videoStream = -1;
    size_t width = 0;
    size_t height = 0;
    float durationSecs = -1.f;
    bool isH264 = false;

    void close();
};

#endif