#include "NetworkDemos.h"
#include "common.h"
#include "WiEngine.h"
#include "WiEngine-Network.h"

namespace Network {
    #define TARGETSELECTOR(id, userData) (wyTargetSelector*)(new wyTargetSelector(this, id, userData))->autoRelease()
    
	class wyDownloadImageTestLayer : public wyLayer {
    private:
        wyProgressTimer* m_progressTimer;
        wySprite* m_bg;
        wyTexture2D* m_bgTex;
        
	public:
		wyDownloadImageTestLayer() :
                m_progressTimer(NULL),
                m_bg(NULL),
                m_bgTex(NULL) {
            // create button
            wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
            wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
            normal->setContentSize(DP(150), DP(44));
            pressed->setContentSize(DP(150), DP(44));
            wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
            button->setPosition(DP(80), wyDevice::winHeight - DP(30));
            addChildLocked(button, 1);
            button->release();
            
            // label for create button
            wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
            wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Style 1 BG");
            label->setColor(wyc3b(255, 255, 0));
            label->setPosition(DP(80), wyDevice::winHeight - DP(30));
            addChildLocked(label, 1);
            
            // destroy button
            button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
            button->setPosition(DP(240), wyDevice::winHeight - DP(30));
            addChildLocked(button, 1);
            button->release();
            
            // label for destroy button
            label = wyBitmapFontLabel::make(font, "Style 2 BG");
            label->setColor(wyc3b(255, 255, 0));
            label->setPosition(DP(240), wyDevice::winHeight - DP(30));
            addChildLocked(label, 1);
            
            // init network, must be called
			wyNetwork::init();
		}

		virtual ~wyDownloadImageTestLayer() {
            // cleanup, must be called
			wyNetwork::cleanup();
		}
        
        void createProgressTimer() {
            m_progressTimer = new wyProgressTimer(wyTexture2D::makePNG(RES("R.drawable.bar")));
            m_progressTimer->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
            m_progressTimer->setStyle(HORIZONTAL_BAR_LR);
            addChildLocked(m_progressTimer);
            m_progressTimer->release();
        }
        
        void deleteProgressTimer() {
            removeChildLocked(m_progressTimer, true);
            m_progressTimer = NULL;
        }
        
        virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
            static wyDownloadCallback dc = {
                NULL,
                onDownloadProgress,
                onDownloadCompleted,
                onDownloadFailed
            };
            
            switch(ts->getId()) {
                case 1:
                    if(m_progressTimer == NULL) {
                        createProgressTimer();
                        wyNetwork::download("http://wiengine.wiyun.com/bg_style1.png", "/sdcard/WiEngine/bg_style1.png", &dc, this);
                    } else {
                        wyUtils::showToast("Already downloading, please wait...");
                    }
                    break;
                case 2:
                    if(m_progressTimer == NULL) {
                        createProgressTimer();
                        wyNetwork::download("http://wiengine.wiyun.com/bg_style2.png", "/sdcard/WiEngine/bg_style2.png", &dc, this);
                    } else {
                        wyUtils::showToast("Already downloading, please wait...");
                    }
                    break;
            }
        }
        
        static void onDownloadProgress(const char* url, const char* savePath, int totalBytes, int downloadedBytes, void* data) {
            wyDownloadImageTestLayer* layer = (wyDownloadImageTestLayer*)data;
            layer->m_progressTimer->setPercentage(totalBytes == 0 ? 0 : (downloadedBytes * 100 / totalBytes));
        }
        
        static void onDownloadCompleted(const char* url, const char* savePath, void* data) {
            wyDownloadImageTestLayer* layer = (wyDownloadImageTestLayer*)data;
            layer->deleteProgressTimer();
            
            // remove old bg
            if(layer->m_bg != NULL) {
                layer->removeChildLocked(layer->m_bg, true);
                layer->m_bg = NULL;
            }
            
            // remove old bg texture
            if(layer->m_bgTex != NULL) {
                wyTextureManager::getInstance()->removeTexture(layer->m_bgTex);
                layer->m_bgTex = NULL;
            }
            
            // create new bg
            layer->m_bgTex = wyTexture2D::makeFilePNG(savePath);
            layer->m_bg = wySprite::make(layer->m_bgTex);
            layer->m_bg->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
            layer->m_bg->setContentSize(wyDevice::winWidth, wyDevice::winHeight);
            layer->m_bg->setAutoFit(true);
            layer->addChildLocked(layer->m_bg, -1);
        }
        
        static void onDownloadFailed(const char* url, const char* savePath, void* data) {
            wyDownloadImageTestLayer* layer = (wyDownloadImageTestLayer*)data;
            layer->deleteProgressTimer();
            wyUtils::showToast("Failed to download background");
        }
	};

	class wyUploadScreenshotTestLayer : public wyLayer {
	private:
		wySprite* m_screenshotSprite;
		wyProgressTimer* m_progressTimer;

	public:
		wyUploadScreenshotTestLayer() : m_screenshotSprite(NULL), m_progressTimer(NULL) {
			// create a sprite
			wySprite* sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
			sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(100));
			addChildLocked(sprite);

			// run action on sprite
	        wyIntervalAction* rotate = new wyRotateTo(2, 0, 360);
	        wyAction* action = new wyRepeatForever(rotate);
	        sprite->runAction(action);
	        wyObjectRelease(rotate);
	        wyObjectRelease(action);

	        // create menu for screenshot
			wyLabel* label1 = wyLabel::make("Make Screenshot");
	        wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL, (wyTargetSelector*)(new wyTargetSelector(this, 0, NULL))->autoRelease(), label1);
			wyLabel* label2 = wyLabel::make("Upload Screenshot");
	        wyMenuItemLabel* item2 = new wyMenuItemLabel(NULL, (wyTargetSelector*)(new wyTargetSelector(this, 1, NULL))->autoRelease(), label2);
	        wyMenu* menu = new wyMenu(item1, item2, NULL);
	        menu->alignItemsVertically();
	        addChildLocked(menu);
			menu->release();
			item1->release();
			item2->release();

	        // 添加director生命周期监听,我们只需要处理截图事件
	        static wyDirectorLifecycleListener dll = {
	        	NULL,
	        	NULL,
	        	NULL,
	        	NULL,
	        	NULL,
	        	NULL,
	        	onScreenCaptured
	        };
	        wyDirector::getInstance()->addLifecycleListener(&dll, this);

            // init network, must be called
			wyNetwork::init();
		}

		virtual ~wyUploadScreenshotTestLayer() {
			// cleanup, must be called when not using libcurl
			wyNetwork::cleanup();
		}

        void createProgressTimer() {
            m_progressTimer = new wyProgressTimer(wyTexture2D::makePNG(RES("R.drawable.bar")));
            m_progressTimer->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
            m_progressTimer->setStyle(HORIZONTAL_BAR_LR);
            addChildLocked(m_progressTimer);
            m_progressTimer->release();
        }

        void deleteProgressTimer() {
            removeChildLocked(m_progressTimer, true);
            m_progressTimer = NULL;
        }

		virtual void onTargetSelectorInvoked(wyTargetSelector* ts){
			switch(ts->getId()){
				case 0:
					onMakeScreenshot();
					break;
				case 1:
                    if(m_progressTimer == NULL) {
                        createProgressTimer();
						onUploadScreenshot();
                    } else {
                        wyUtils::showToast("Already uploading, please wait...");
                    }
					break;
			}
		}

		void onUploadScreenshot() {
			/*
			 * 上传地址, 以及用户名密码, 用户名密码用冒号隔开. 填写后重新编译示例代码运行即可.
			 */
			const char* uploadUrl = "please fill it";
			const char* userpwd = "please fill it";

			// 检查是否填写了上传信息
			if(!strcmp(uploadUrl, "please fill it")) {
				wyUtils::showToast("please fill upload info and recompile");
				return;
			}
			
			// check file
			if(!wyUtils::isPathExists("/sdcard/WiEngine/screenshot.png")) {
				wyUtils::showToast("no screenshot file");
				return;
			}

			// start upload
            static wyUploadCallback uc = {
                NULL,
                onUploadProgress,
                onUploadCompleted,
                onUploadFailed
            };
            wyNetwork::upload(uploadUrl, "/sdcard/WiEngine/screenshot.png", userpwd, &uc, this);
		}

		void onMakeScreenshot() {
	    	// 如果之前的截图预览还在, 删除掉, 这样的话, 就不会被截进去了
	    	if(m_screenshotSprite != NULL) {
	    		removeChild(m_screenshotSprite, true);
	    		m_screenshotSprite = NULL;
	    	}

	    	// 截屏, 截屏操作是异步的, 可以在onDirectorScreenCaptured中处理截屏成功事件
			wyDirector::getInstance()->makeScreenshot("/sdcard/WiEngine/screenshot.png");
		}

		static void onScreenCaptured(const char* path, void* data) {
	    	// 之前截屏过的贴图应该已经在缓冲中, 所以先删掉缓冲中的贴图, 由于这个贴图是从文件系统中载入的, 它的路径就是它的key
			wyTextureManager::getInstance()->removeTexture(path);

			// 从新的截图中创建贴图对象
	    	wyTexture2D* tex = wyTexture2D::makeFilePNG(path);

	    	// 创建新的截图sprite, 添加到屏幕下方
	    	// 这个sprite设置了自动拉伸, 因此看起来是缩小了, 为了看的清楚一些, 这个截图渲染时用红色
	    	wyUploadScreenshotTestLayer* layer = (wyUploadScreenshotTestLayer*)data;
	    	layer->m_screenshotSprite = wySprite::make(tex);
	    	layer->m_screenshotSprite->setContentSize(wyDevice::winWidth * 0.4f, wyDevice::winHeight * 0.4f);
	    	layer->m_screenshotSprite->setAutoFit(true);
	    	layer->m_screenshotSprite->setAnchorPercent(0.5f, 0.f);
	    	layer->m_screenshotSprite->setPosition(wyDevice::winWidth / 2, 0);
	    	layer->m_screenshotSprite->setColor(wyc3bRed);
	    	layer->addChildLocked(layer->m_screenshotSprite);
		}

		static void onUploadProgress(const char* url, const char* readPath, int totalBytes, int uploadedBytes, void* data) {
			wyUploadScreenshotTestLayer* layer = (wyUploadScreenshotTestLayer*)data;
            layer->m_progressTimer->setPercentage(totalBytes == 0 ? 0 : (uploadedBytes * 100 / totalBytes));
		}

		static void onUploadCompleted(const char* url, const char* readPath, void* data) {
			wyUploadScreenshotTestLayer* layer = (wyUploadScreenshotTestLayer*)data;
            layer->deleteProgressTimer();
            wyUtils::showToast("Screenshot uploaded");
		}

		static void onUploadFailed(const char* url, const char* readPath, void* data) {
			wyUploadScreenshotTestLayer* layer = (wyUploadScreenshotTestLayer*)data;
            layer->deleteProgressTimer();
            wyUtils::showToast("Failed to upload screenshot");
		}
	};
}

using namespace Network;

#if ANDROID
	#define DEMO_ENTRY_IMPL(testname) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_network_##testname##_nativeStart \
		(JNIEnv *, jobject){ \
			wyLayer* layer = new wy##testname##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _network_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(DownloadImageTest);
DEMO_ENTRY_IMPL(UploadScreenshotTest);
