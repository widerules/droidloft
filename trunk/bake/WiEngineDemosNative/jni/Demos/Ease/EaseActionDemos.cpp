#include "common.h"
#include "EaseActionDemos.h"
#include "WiEngine.h"
#include <stdio.h>

namespace Ease_Action {
    class wyEaseTestLayer : public wyLayer{
    protected:
        wySprite* m_Sprite;
    public:
        wyEaseTestLayer(){
            m_Sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
            m_Sprite->retain();
            addChildLocked(m_Sprite);
            m_Sprite->setPosition(60, wyDevice::winHeight/ 2);
        };
        
        virtual ~wyEaseTestLayer(){
            wyObjectRelease(m_Sprite);
            m_Sprite = NULL;
        };
    };
    
    class wyEaseBackInOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseBackInOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackInOut*)(new wyEaseBackInOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackInOut*)(new wyEaseBackInOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    class wyEaseBackInTestLayer : public wyEaseTestLayer {
    public:
        wyEaseBackInTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackInOut*)(new wyEaseBackIn((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackInOut*)(new wyEaseBackIn((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    class wyEaseBackOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseBackOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseBackIn((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseBackIn((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            
            wyObjectRelease(action);
        };
    };

    class wyEaseBounceInOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseBounceInOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseBounceInOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseBounceInOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            
            wyObjectRelease(action);
        };
    };

    class wyEaseBounceInTestLayer : public wyEaseTestLayer {
    public:
        wyEaseBounceInTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseBounceIn((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseBounceIn((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            
            wyObjectRelease(action);
        };
    };

    class wyEaseBounceOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseBounceOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseBounceOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseBounceOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    class wyEaseElasticInOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseElasticInOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseElasticInOut(0.3f, (wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseElasticInOut(0.3f, (wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            
            wyObjectRelease(action);
        };
    };

    class wyEaseElasticInTestLayer : public wyEaseTestLayer {
    public:
        wyEaseElasticInTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseElasticIn(0.3f, (wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseElasticIn(0.3f, (wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    
   class wyEaseElasticOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseElasticOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseElasticOut(0.3f, (wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseElasticOut(0.3f, (wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

  class wyEaseExponentialInOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseExponentialInOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseExponentialInOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseExponentialInOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    class wyEaseExponentialInTestLayer : public wyEaseTestLayer {
    public:
        wyEaseExponentialInTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseExponentialIn((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseExponentialIn((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };


   class wyEaseExponentialOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseExponentialOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseExponentialOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseExponentialOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

 class wyEaseInOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseInOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseInOut(2, (wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseInOut(2, (wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    class wyEaseInTestLayer : public wyEaseTestLayer {
    public:
        wyEaseInTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseIn(2, (wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseIn(2, (wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };


   class wyEaseOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseOut(2, (wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseOut(2, (wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

 class wyEaseSineInOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseSineInOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseSineInOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseSineInOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };

    class wyEaseSineInTestLayer : public wyEaseTestLayer {
    public:
        wyEaseSineInTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseSineIn((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseSineIn((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };


   class wyEaseSineOutTestLayer : public wyEaseTestLayer {
    public:
        wyEaseSineOutTestLayer(){
            wyIntervalAction* a = new wyMoveBy(2, wyDevice::winWidth - 120, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make((wyEaseBackOut*)(new wyEaseSineOut((wyIntervalAction*)a->autoRelease()))->autoRelease(),
												(wyEaseBackOut*)(new wyEaseSineOut((wyIntervalAction*)r->autoRelease()))->autoRelease(),
 												NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);

            wyObjectRelease(action);
        };
    };
}

using namespace Ease_Action;

#if ANDROID
	#define DEMO_ENTRY_IMPL(CLASSNAME) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_ease_##CLASSNAME##_nativeStart \
	(JNIEnv *, jobject){ \
		wyLayer* layer = new wy##CLASSNAME##Layer(); \
		runDemo(layer, NULL); \
		wyObjectRelease(layer); \
	}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _ease_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(EaseBackInOutTest);
DEMO_ENTRY_IMPL(EaseBackInTest);
DEMO_ENTRY_IMPL(EaseBackOutTest);
DEMO_ENTRY_IMPL(EaseBounceInOutTest);
DEMO_ENTRY_IMPL(EaseBounceInTest);
DEMO_ENTRY_IMPL(EaseBounceOutTest);
DEMO_ENTRY_IMPL(EaseElasticInOutTest);
DEMO_ENTRY_IMPL(EaseElasticInTest);
DEMO_ENTRY_IMPL(EaseElasticOutTest);
DEMO_ENTRY_IMPL(EaseExponentialInOutTest);
DEMO_ENTRY_IMPL(EaseExponentialInTest);
DEMO_ENTRY_IMPL(EaseExponentialOutTest);
DEMO_ENTRY_IMPL(EaseInOutTest);
DEMO_ENTRY_IMPL(EaseInTest);
DEMO_ENTRY_IMPL(EaseOutTest);
DEMO_ENTRY_IMPL(EaseSineInOutTest);
DEMO_ENTRY_IMPL(EaseSineInTest);
DEMO_ENTRY_IMPL(EaseSineOutTest);
