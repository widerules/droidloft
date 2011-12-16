#include "common.h"
#include "SoundDemos.h"
#include "WiEngine.h"
#include "WiEngine-WiSound.h"
#include <stdio.h>

namespace Sound {

class wyMP3TestLayer : public wyLayer {
public:
	wyLabel* m_hint;
	float m_volume;

public:
	wyMP3TestLayer(){
        wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 1, NULL))->autoRelease(),
				 wyLabel::make("Play", SP(32), NORMAL));
        wyMenuItemLabel* item2 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 2, NULL))->autoRelease(),
				 wyLabel::make("Stop", SP(32), NORMAL));
        wyMenuItemLabel* item3 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 3, NULL))->autoRelease(),
				 wyLabel::make("Effect(mono)", SP(32), NORMAL));
        wyMenuItemLabel* item4 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 4, NULL))->autoRelease(),
				 wyLabel::make("Effect(stereo)", SP(32), NORMAL));
        wyMenuItemLabel* item5 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 5, NULL))->autoRelease(),
				 wyLabel::make("Volume Down", SP(32), NORMAL));
        wyMenuItemLabel* item6 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 6, NULL))->autoRelease(),
				 wyLabel::make("Volume Up", SP(32), NORMAL));

        wyMenu* menu = new wyMenu((wyMenuItem*)item1->autoRelease(),
									item2->autoRelease(),
									item3->autoRelease(),
									item4->autoRelease(),
									item5->autoRelease(),
									item6->autoRelease(),
									NULL);
        menu->alignItemsVertically();

        addChildLocked(menu);
        wyObjectRelease(menu);

		// create a hint label
        m_volume = 1.0f;
		m_hint = wyLabel::make("background&effect volume: 1.0", SP(20));
		m_hint->setPosition(wyDevice::winWidth / 2, 100);
		addChildLocked(m_hint);

		// 不指定格式, 缺省是mp3
        wyAudioManager::getInstance()->preloadEffect(RES("R.raw.effect_mp3_mono"));
        wyAudioManager::getInstance()->preloadEffect(RES("R.raw.effect_mp3_stereo"));
	}

	virtual ~wyMP3TestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				wyAudioManager::getInstance()->playBackgroundMusic(RES("R.raw.bg_mp3"), FORMAT_MP3, -1);
				break;
			case 2:
				wyAudioManager::getInstance()->stopBackgroundMusic();
				break;
			case 3:
				wyAudioManager::getInstance()->playEffect(RES("R.raw.effect_mp3_mono"));
				break;
			case 4:
				wyAudioManager::getInstance()->playEffect(RES("R.raw.effect_mp3_stereo"));
				break;
			case 5:
			{
				m_volume -= 0.1f;
				m_volume = MAX(0, m_volume);
				wyAudioManager::getInstance()->setBackgroundVolume(m_volume);
				wyAudioManager::getInstance()->setEffectVolume(m_volume);
				char buf[64];
				sprintf(buf, "background&effect volume: %.1f", m_volume);
				m_hint->setText(buf);
				break;
			}
			case 6:
			{
				m_volume += 0.1f;
				m_volume = MIN(1.0f, m_volume);
				wyAudioManager::getInstance()->setBackgroundVolume(m_volume);
				wyAudioManager::getInstance()->setEffectVolume(m_volume);
				char buf[64];
				sprintf(buf, "background&effect volume: %.1f", m_volume);
				m_hint->setText(buf);
				break;
			}
		}
	}
};

class wyOGGTestLayer : public wyLayer {
public:
	wyLabel* m_hint;
	float m_volume;

public:
	wyOGGTestLayer(){
        wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 1, NULL))->autoRelease(),
				 wyLabel::make("Play", SP(32), NORMAL));
        wyMenuItemLabel* item2 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 2, NULL))->autoRelease(),
				 wyLabel::make("Stop", SP(32), NORMAL));
        wyMenuItemLabel* item3 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 3, NULL))->autoRelease(),
				 wyLabel::make("Effect(mono)", SP(32), NORMAL));
        wyMenuItemLabel* item4 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 4, NULL))->autoRelease(),
				 wyLabel::make("Effect(stereo)", SP(32), NORMAL));
        wyMenuItemLabel* item5 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 5, NULL))->autoRelease(),
				 wyLabel::make("Volume Down", SP(32), NORMAL));
        wyMenuItemLabel* item6 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 6, NULL))->autoRelease(),
				 wyLabel::make("Volume Up", SP(32), NORMAL));

        wyMenu* menu = new wyMenu((wyMenuItem*)item1->autoRelease(),
									item2->autoRelease(),
									item3->autoRelease(),
									item4->autoRelease(),
									item5->autoRelease(),
									item6->autoRelease(),
									NULL);
        menu->alignItemsVertically();

        addChildLocked(menu);
        wyObjectRelease(menu);

		// create a hint label
        m_volume = 1.0f;
		m_hint = wyLabel::make("background&effect volume: 1.0", SP(20));
		m_hint->setPosition(wyDevice::winWidth / 2, 100);
		addChildLocked(m_hint);

		// 不指定格式, 缺省是mp3
        wyAudioManager::getInstance()->preloadEffect(RES("R.raw.effect_ogg_mono"), FORMAT_OGG);
        wyAudioManager::getInstance()->preloadEffect(RES("R.raw.effect_ogg_stereo"), FORMAT_OGG);
	}

	virtual ~wyOGGTestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				wyAudioManager::getInstance()->playBackgroundMusic(RES("R.raw.bg_ogg"), FORMAT_OGG, -1);
				break;
			case 2:
				wyAudioManager::getInstance()->stopBackgroundMusic();
				break;
			case 3:
				wyAudioManager::getInstance()->playEffect(RES("R.raw.effect_ogg_mono"));
				break;
			case 4:
				wyAudioManager::getInstance()->playEffect(RES("R.raw.effect_ogg_stereo"));
				break;
			case 5:
			{
				m_volume -= 0.1f;
				m_volume = MAX(0, m_volume);
				wyAudioManager::getInstance()->setBackgroundVolume(m_volume);
				wyAudioManager::getInstance()->setEffectVolume(m_volume);
				char buf[64];
				sprintf(buf, "background&effect volume: %.1f", m_volume);
				m_hint->setText(buf);
				break;
			}
			case 6:
			{
				m_volume += 0.1f;
				m_volume = MIN(1.0f, m_volume);
				wyAudioManager::getInstance()->setBackgroundVolume(m_volume);
				wyAudioManager::getInstance()->setEffectVolume(m_volume);
				char buf[64];
				sprintf(buf, "background&effect volume: %.1f", m_volume);
				m_hint->setText(buf);
				break;
			}
		}
	}
};

class wyWAVTestLayer : public wyLayer {
public:
	wyLabel* m_hint;
	float m_volume;

public:
	wyWAVTestLayer(){
        wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 1, NULL))->autoRelease(),
				 wyLabel::make("Play", SP(32), NORMAL));
        wyMenuItemLabel* item2 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 2, NULL))->autoRelease(),
				 wyLabel::make("Stop", SP(32), NORMAL));
        wyMenuItemLabel* item3 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 3, NULL))->autoRelease(),
				 wyLabel::make("Effect(16bit, mono)", SP(32), NORMAL));
        wyMenuItemLabel* item4 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 4, NULL))->autoRelease(),
				 wyLabel::make("Effect(16bit, stereo)", SP(32), NORMAL));
        wyMenuItemLabel* item5 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 5, NULL))->autoRelease(),
				 wyLabel::make("Volume Down", SP(32), NORMAL));
        wyMenuItemLabel* item6 = new wyMenuItemLabel(NULL,
        		(wyTargetSelector*)(new wyTargetSelector(this, 6, NULL))->autoRelease(),
				 wyLabel::make("Volume Up", SP(32), NORMAL));

        wyMenu* menu = new wyMenu((wyMenuItem*)item1->autoRelease(),
									item2->autoRelease(),
									item3->autoRelease(),
									item4->autoRelease(),
									item5->autoRelease(),
									item6->autoRelease(),
									NULL);
        menu->alignItemsVertically();

        addChildLocked(menu);
        wyObjectRelease(menu);

		// create a hint label
        m_volume = 1.0f;
		m_hint = wyLabel::make("background&effect volume: 1.0", SP(20));
		m_hint->setPosition(wyDevice::winWidth / 2, 100);
		addChildLocked(m_hint);

        wyAudioManager::getInstance()->preloadEffect(RES("R.raw.effect_wav_mono_16"), FORMAT_WAV);
        wyAudioManager::getInstance()->preloadEffect(RES("R.raw.effect_wav_stereo_16"), FORMAT_WAV);
	}

	virtual ~wyWAVTestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				wyAudioManager::getInstance()->playBackgroundMusic(RES("R.raw.bg_wav_mono_16"), FORMAT_WAV, -1);
				break;
			case 2:
				wyAudioManager::getInstance()->stopBackgroundMusic();
				break;
			case 3:
				wyAudioManager::getInstance()->playEffect(RES("R.raw.effect_wav_mono_16"));
				break;
			case 4:
				wyAudioManager::getInstance()->playEffect(RES("R.raw.effect_wav_stereo_16"));
				break;
			case 5:
			{
				m_volume -= 0.1f;
				m_volume = MAX(0, m_volume);
				wyAudioManager::getInstance()->setBackgroundVolume(m_volume);
				wyAudioManager::getInstance()->setEffectVolume(m_volume);
				char buf[64];
				sprintf(buf, "background&effect volume: %.1f", m_volume);
				m_hint->setText(buf);
				break;
			}
			case 6:
			{
				m_volume += 0.1f;
				m_volume = MIN(1.0f, m_volume);
				wyAudioManager::getInstance()->setBackgroundVolume(m_volume);
				wyAudioManager::getInstance()->setEffectVolume(m_volume);
				char buf[64];
				sprintf(buf, "background&effect volume: %.1f", m_volume);
				m_hint->setText(buf);
				break;
			}
		}
	}
};

}

using namespace Sound;

#if ANDROID
	#define DEMO_ENTRY_IMPL(CLASSNAME) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_sound_##CLASSNAME##_nativeStart \
		(JNIEnv *env, jobject thiz) { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _sound_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(MP3Test);
DEMO_ENTRY_IMPL(OGGTest);
DEMO_ENTRY_IMPL(WAVTest);
