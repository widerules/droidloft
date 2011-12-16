#include "GridActionDemos.h"
#include "common.h"
#include "WiEngine.h"
#include <stdio.h>

namespace Grid_Action {
    class wyActionTestLayer : public wyLayer{
    protected:
        wySprite* m_Sprite;
    public:
        wyActionTestLayer(){
            m_Sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
            m_Sprite->retain();
            addChildLocked(m_Sprite, 2);
            m_Sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight/ 2);
			m_Sprite->setScale(4);
        };

        virtual ~wyActionTestLayer(){
            wyObjectRelease(m_Sprite);
            m_Sprite = NULL;
        };
    };
    
    class wyTilesActionTestLayer : public wyColorLayer{
    protected:
        wySprite* m_Sprite;
    public:
        wyTilesActionTestLayer():wyColorLayer(wyc4b(0, 0, 255, 255)){
            m_Sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
            m_Sprite->retain();
            addChildLocked(m_Sprite, 2);
            m_Sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight/ 2);
			m_Sprite->setScale(4);
        };

        virtual ~wyTilesActionTestLayer(){
            wyObjectRelease(m_Sprite);
            m_Sprite = NULL;
        };
    };

    class wyGridFlipXTestLayer : public wyActionTestLayer {
    public:
        wyGridFlipXTestLayer(){
            wyIntervalAction* action = wySequence::make(
            		(wyFiniteTimeAction*)(new wyGridFlipX(4.0f))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            m_Sprite->runAction(action);

        	wySprite* bg = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.background")));
        	bg->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
        	addChildLocked(bg, -1);
        };
    };

    class wyGridFlipYTestLayer : public wyActionTestLayer {
    public:
        wyGridFlipYTestLayer(){
            wyIntervalAction* action = wySequence::make(
            		(wyFiniteTimeAction*)(new wyGridFlipY(4.0f))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            m_Sprite->runAction(action);

        	wySprite* bg = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.background")));
        	bg->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
        	addChildLocked(bg, -1);
        };
    };

    class wyJumpTiles3DTestLayer : public wyTilesActionTestLayer {
    public:
        wyJumpTiles3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyJumpTiles3D(4, 12, 20, 20.0f, 3))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(1))->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyLens3DTestLayer : public wyActionTestLayer {
    public:
        wyLens3DTestLayer(){
			wyIntervalAction* a = new wyLens3D(12, 12, 20, 100, 100, wyDevice::winWidth - 100, wyDevice::winHeight - 100, 200, 200, 0.7f);
			wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyLineShrinkOutTestLayer : public wyTilesActionTestLayer {
    public:
        wyLineShrinkOutTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyBottomLineShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(3))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyTopLineShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyLeftLineShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyRightLineShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyLiquidTestLayer : public wyActionTestLayer {
    public:
        wyLiquidTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyLiquid(12, 12, 20, 20, -20, 10))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyPageTurn3DTestLayer : public wyTilesActionTestLayer {
    public:
        wyPageTurn3DTestLayer(){
			wyIntervalAction* a = new wyPageTurn3D(2.0f, 12, 20);
			wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(2))->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyRipple3DTestLayer : public wyActionTestLayer {
    public:
        wyRipple3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyRipple3D(4, 12, 20, wyDevice::winWidth / 2, wyDevice::winHeight / 2, 0, 0, 400, 0, 200, -200, 2))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyShaky3DTestLayer : public wyActionTestLayer {
    public:
        wyShaky3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyShaky3D(1200, 12, 20, 5, 0, false))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(1))->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyShakyTiles3DTestLayer : public wyTilesActionTestLayer {
    public:
        wyShakyTiles3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyShakyTiles3D(4, 12, 20, 5, 0, false))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(1))->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyShatterTiles3DTestLayer : public wyTilesActionTestLayer {
    public:
        wyShatterTiles3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyShatteredTiles3D(4, 12, 20, 20, false))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(1))->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyShuffleTilesTestLayer : public wyTilesActionTestLayer {
    public:
        wyShuffleTilesTestLayer(){
			wyIntervalAction* t =  new wyShuffleTiles(4, 12, 20);
            runAction(t);
            wyObjectRelease(t);
        };
    };

    class wySplitRowsColsTestLayer : public wyTilesActionTestLayer {
    public:
        wySplitRowsColsTestLayer(){
			wyIntervalAction* ar = new wySplitRows(2, 4, 8);
			wyIntervalAction* ac = new wySplitCols(2, 4, 8);
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)ar->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(4))->autoRelease(),
            		(wyFiniteTimeAction*)ar->reverse()->autoRelease(),
            		(wyFiniteTimeAction*)ac->autoRelease(),
            		(wyFiniteTimeAction*)ac->reverse()->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyTilesShrinkOutTestLayer : public wyTilesActionTestLayer {
    public:
        wyTilesShrinkOutTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyLeftBottomTilesShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(3))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyRightTopTilesShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyBottomTilesShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyTopTilesShrinkOut(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyTurnOffTilesTestLayer : public wyTilesActionTestLayer {
    public:
        wyTurnOffTilesTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyTurnOffTiles(4, 12, 20))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyTwirlTestLayer : public wyActionTestLayer {
    public:
        wyTwirlTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyTwirl(12, 12, 20, wyDevice::winWidth/2, wyDevice::winHeight/2, 0, 0, 3, 0, 1))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyWaves3DTestLayer : public wyActionTestLayer {
    public:
        wyWaves3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyWaves3D(4, 12, 20, 20, 3))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyWavesTestLayer : public wyActionTestLayer {
    public:
        wyWavesTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyWaves(4, 12, 20, 20, 3, false, true))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyStopGrid())->autoRelease(),
            		NULL);
            runAction(t);
        };
    };

    class wyWavesTiles3DTestLayer : public wyTilesActionTestLayer {
    public:
        wyWavesTiles3DTestLayer(){
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)(new wyWavesTiles3D(4, 12, 20, 20, 3))->autoRelease(),
            		(wyFiniteTimeAction*)(new wyReuseGrid(1))->autoRelease(),
            		NULL);
  			wyAction* action = new wyRepeatForever(t);
            runAction(action);
            wyObjectRelease(action);
        };
    };

    class wySuckTestLayer : public wyActionTestLayer {
    private:
    	wySprite* powered;
	public:
    	wySuckTestLayer(){
    		powered = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.background")));
			powered->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight/ 2);
			addChildLocked(powered, 2);
			removeChildLocked(m_Sprite, true);
			setTouchEnabled(true);
		};

    	virtual bool touchesBegan(wyMotionEvent& event) {
    		// powered
    		if (powered->getGrid() == NULL || !powered->getGrid()->isActive()) {
				wyIntervalAction* t = wySequence::make(
						wySuck::make(0.6f, 20, 20, event.x[0], event.y[0]),
						wyStopGrid::make(),
						NULL);
				powered->runAction(t);
    		}
			return true;
		}
	};

}

using namespace Grid_Action;

#if ANDROID
	#define DEMO_ENTRY_IMPL(testname) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_gridaction_##testname##_nativeStart \
		(JNIEnv *, jobject){ \
			wyLayer* layer = new wy##testname##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _gridaction_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(SuckTest);
DEMO_ENTRY_IMPL(GridFlipXTest);
DEMO_ENTRY_IMPL(GridFlipYTest);
DEMO_ENTRY_IMPL(JumpTiles3DTest);
DEMO_ENTRY_IMPL(Lens3DTest);
DEMO_ENTRY_IMPL(LineShrinkOutTest);
DEMO_ENTRY_IMPL(LiquidTest);
DEMO_ENTRY_IMPL(PageTurn3DTest);
DEMO_ENTRY_IMPL(Ripple3DTest);
DEMO_ENTRY_IMPL(Shaky3DTest);
DEMO_ENTRY_IMPL(ShakyTiles3DTest);
DEMO_ENTRY_IMPL(ShatterTiles3DTest);
DEMO_ENTRY_IMPL(ShuffleTilesTest);
DEMO_ENTRY_IMPL(SplitRowsColsTest);
DEMO_ENTRY_IMPL(TilesShrinkOutTest);
DEMO_ENTRY_IMPL(TurnOffTilesTest);
DEMO_ENTRY_IMPL(TwirlTest);
DEMO_ENTRY_IMPL(Waves3DTest);
DEMO_ENTRY_IMPL(WavesTest);
DEMO_ENTRY_IMPL(WavesTiles3DTest);
