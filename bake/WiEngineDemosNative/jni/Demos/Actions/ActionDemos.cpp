#include "common.h"
#include "ActionDemos.h"
#include "WiEngine.h"
#include <stdio.h>

namespace Action {
	class wyActionTestLayer : public wyLayer{
	protected:
		wySprite* m_Sprite;
	public:
		wyActionTestLayer(){
			m_Sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
			m_Sprite->retain();
			addChildLocked(m_Sprite);
			m_Sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight/ 2);
		};

		virtual ~wyActionTestLayer(){
			wyObjectRelease(m_Sprite);
			m_Sprite = NULL;
		};
	};
    
    class wyAnimationTestLayer : public wyActionTestLayer {
    public:
    	wyLabel* m_hint;

    public:
        wyAnimationTestLayer(){
            // create animation
            wyAnimation* anim = new wyAnimation(0);
            for (int i = 0; i < 14; i++){
                char name[256];
                sprintf(name, "grossini_dance_%.2d", i + 1);
                anim->addFrame(0.2f, wyTexture2D::makePNG(wyUtils::getResId(name, "drawable", NULL)));
            }
            wyAnimationCallback callback = {
            		onAnimationFrameChanged,
            		onAnimationEnded
            };
            anim->setCallback(&callback, this);
        
            wyAnimate* a = new wyAnimate(anim);
            
            m_Sprite->runAction(a);
            
            wyObjectRelease(anim);
            wyObjectRelease(a);

    		// create a hint label
    		m_hint = wyLabel::make("callback display label", SP(20));
    		m_hint->setPosition(wyDevice::winWidth / 2, 100);
    		addChildLocked(m_hint);
        }

    	static void onAnimationFrameChanged(wyAnimation* anim, int index, void* data) {
    		wyAnimationTestLayer* layer = (wyAnimationTestLayer*)data;
    		char buf[64];
    		sprintf(buf, "frame changed: %d", index);
    		layer->m_hint->setText(buf);
    	}

    	static void onAnimationEnded(wyAnimation* anim, void* data) {
    		wyAnimationTestLayer* layer = (wyAnimationTestLayer*)data;
    		layer->m_hint->setText("animation ended");
    	}
    };
    
    class wyAtlasAnimationTestLayer : public wyLayer {
    private:
		int ITEM_WIDTH;
		int ITEM_HEIGHT;

    public:
		wyLabel* m_hint;

    private:
		wyRect frameAt(int x, int y){
		   return wyr(x * ITEM_WIDTH, y * ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT);
		}

    public:
        wyAtlasAnimationTestLayer(){
            // set width and height
            ITEM_WIDTH = DP(85);
            ITEM_HEIGHT = DP(121);
            
            wyTexture2D* tex = wyTexture2D::makePNG(RES("R.drawable.grossini_dance_atlas"));
            // add sprite
            wySprite* sprite = new wySprite(tex, wyr(0, 0, ITEM_WIDTH, ITEM_HEIGHT));
            sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
            addChildLocked(sprite);
            
            // create animation and add it to atlas sprite
            wyAnimation* anim = new wyAnimation(0);
            anim->addFrame(0.2f, frameAt(0, 0));
            anim->addFrame(0.2f, frameAt(1, 0));
            anim->addFrame(0.2f, frameAt(2, 0));
            anim->addFrame(0.2f, frameAt(3, 0));
            anim->addFrame(0.2f, frameAt(4, 0));
            anim->addFrame(0.2f, frameAt(0, 1));
            anim->addFrame(0.2f, frameAt(1, 1));
            anim->addFrame(0.2f, frameAt(2, 1));
            anim->addFrame(0.2f, frameAt(3, 1));
            anim->addFrame(0.2f, frameAt(4, 1));
            anim->addFrame(0.2f, frameAt(0, 2));
            anim->addFrame(0.2f, frameAt(1, 2));
            anim->addFrame(0.2f, frameAt(2, 2));
            anim->addFrame(0.2f, frameAt(3, 2));
            wyAnimationCallback callback = {
            		onAnimationFrameChanged,
            		onAnimationEnded
            };
            anim->setCallback(&callback, this);
            
            wyAnimate* a = new wyAnimate(anim);
            
            sprite->runAction(a);
            
            wyObjectRelease(anim);
            wyObjectRelease(a);
			wyObjectRelease(sprite);

    		// create a hint label
    		m_hint = wyLabel::make("callback display label", SP(20));
    		m_hint->setPosition(wyDevice::winWidth / 2, 100);
    		addChildLocked(m_hint);
        };

    	static void onAnimationFrameChanged(wyAnimation* anim, int index, void* data) {
    		wyAtlasAnimationTestLayer* layer = (wyAtlasAnimationTestLayer*)data;
    		char buf[64];
    		sprintf(buf, "frame changed: %d", index);
    		layer->m_hint->setText(buf);
    	}

    	static void onAnimationEnded(wyAnimation* anim, void* data) {
    		wyAtlasAnimationTestLayer* layer = (wyAtlasAnimationTestLayer*)data;
    		layer->m_hint->setText("animation ended");
    	}
    };
    
    class wyBlinkTestLayer : public wyActionTestLayer {
    public:
        wyBlinkTestLayer(){
            wyIntervalAction* t = wyBlink::make(1,2);
            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        };
    };

    class wyCallFuncTestLayer : public wyLayer {
    public:
        wyCallFuncTestLayer(){
        	wyCallFunc* call1 = wyCallFunc::make(wyTargetSelector::make(this, 1, NULL));
        	wyCallFunc* call2 = wyCallFunc::make(wyTargetSelector::make(this, 2, NULL));

        	wyAction* seq = wySequence::make(call1, call2, NULL);
        	runAction(seq);
        };

		virtual void onTargetSelectorInvoked(wyTargetSelector* ts){
			switch(ts->getId()){
				case 1:
					onFun1();
					break;
				case 2:
					onFun2();
					break;
			}
		}

        void onFun1() {
			wyUtils::showToast("func1 is called");
        }

        void onFun2() {
        	wyUtils::showToast("func2 is called");
        }
    };

    class wyDelayShowHideTestLayer : public wyActionTestLayer {
    public:
        wyDelayShowHideTestLayer() {
            wyIntervalAction* t = wySequence::make(wyDelayTime::make(1),
												   wyHide::make(),
												   wyDelayTime::make(1),
												   wyShow::make(),
												   NULL);
           
            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        };
    };

    class wyFadeInOutTestLayer : public wyActionTestLayer {
    public:
        wyFadeInOutTestLayer() {
            wyIntervalAction* a = wyFadeOut::make(2);
            wyAction* r = (wyAction*)a->reverse()->autoRelease();
            wyIntervalAction* t = wySequence::make(a, (wyFiniteTimeAction*)r, NULL);

            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);        
        };
    };

    class wyFadeToTestLayer : public wyActionTestLayer {
    public:
        wyFadeToTestLayer() {
            wyIntervalAction* a = wyFadeTo::make(2, 255, 0);
            wyIntervalAction* r = (wyIntervalAction*)a->reverse()->autoRelease();
            wyIntervalAction* t = wySequence::make(a, r, NULL);

            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        };
    };

    class wyFollowTestLayer : public wyActionTestLayer {
	public:
    	wyFollowTestLayer(){
    		float startx = DP(60);
    		float y = wyDevice::winHeight / 2;
    		float endx = wyDevice::winWidth + DP(100);

    		m_Sprite->setPosition(startx, y);
			wyIntervalAction* a = new wyMoveTo(3, startx, y, endx, y);
			wyAction* r = a->reverse();
			wyIntervalAction* t = wySequence::make(
					(wyFiniteTimeAction*)a->autoRelease(),
					(wyFiniteTimeAction*)r->autoRelease(),
					NULL);

			wyTiledSprite* ts = wyTiledSprite::make(wyTexture2D::makePNG(RES("R.drawable.tiled_bg")));
			ts->setContentSize(wyDevice::winWidth, DP(115));
			ts->setTileDirection(true, false);
			ts->setAnchorPercent(0, 0);
			ts->setPosition(0, 100);
			addChildLocked(ts);

			wyAction* action = new wyRepeatForever(t);
			m_Sprite->runAction(action);
			wyObjectRelease(action);

			wyRect rect = wyr(0, 0, wyDevice::winWidth + DP(160), wyDevice::winHeight);
			wyFollow* f = new wyFollow(m_Sprite, rect);
			runAction(f);
			f->release();
		};
	};

    class wyJumpByTestLayer : public wyActionTestLayer {
    public:
        wyJumpByTestLayer(){
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);
    
            wyIntervalAction* a = new wyJumpBy(3, wyDevice::winWidth - 120, 0, 200, 3);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            
            wyObjectRelease(action);
        }
    };

    class wyJumpToTestLayer : public wyActionTestLayer {
    public:
        wyJumpToTestLayer(){
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);

            wyIntervalAction* a = new wyJumpTo(3, m_Sprite->getPositionX(), m_Sprite->getPositionY(),
                                               wyDevice::winWidth - 60, wyDevice::winHeight/2, 200, 3);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            
            wyObjectRelease(action);
        }
    };

    class wyHypotrochoidTestLayer : public wyActionTestLayer {
    private:
    	wyHypotrochoidConfig m_c;
    	wyHypotrochoidConfig m_circle;
    	wyHypotrochoidConfig m_ellipse;

	public:
    	wyHypotrochoidTestLayer(){
    		m_c = wyhcQuad(DP(100), DP(40), DP(100), 0, 720, wyDevice::winWidth / 2, wyDevice::winHeight / 2);
    		addTrace(m_c);

    		m_ellipse = wyhcEllipse(DP(100), DP(30), 0, 360, wyDevice::winWidth / 2, wyDevice::winHeight / 2);
    		addTrace(m_ellipse);

    		m_circle = wyhcCircle(DP(80), 0, 360, wyDevice::winWidth / 2, wyDevice::winHeight / 2);
    		addTrace(m_circle);

    		m_Sprite->setVisible(false);
    	}

    	void addTrace(wyHypotrochoidConfig c) {
    		wySprite* r = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
    		r->setScale(0.2f);
			addChildLocked(r);
			r->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			wyHypotrochoid* a = wyHypotrochoid::make(6, c);
			a->setAutoRotate(true, 90);
			/*
			 * 如果不启用auto rotate, 可以指定pin point, 注意pin point只在auto rotate为false的时候生效
			 * 下面两句取消注释时必须把a->setAutoRotate(true, 90);注释掉才行
			 */
//	        a->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
//	        a->setPinAngleDelta(90);
			wyAction* action = wyRepeatForever::make(a);
			r->runAction(action);
    	}

    	virtual void draw() {
			// draw bezier curve so we can see the node is follow the track
			glColor4f(0, 1, 0, 1);
			wyDrawHypotrochoid(m_c, 100);

			glColor4f(1, 0, 0, 1);
			wyDrawHypotrochoid(m_ellipse, 100);

			glColor4f(0, 0, 1, 1);
			wyDrawHypotrochoid(m_circle, 100);

			glColor4f(1, 1, 1, 1);
		}
	};

    class wyLagrangeTestLayer : public wyActionTestLayer {
    private:
    	wyLagrangeConfig m_config;

    public:
    	wyLagrangeTestLayer(){
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);

	        m_config = wylcCubic(m_Sprite->getPositionX(),
	        		m_Sprite->getPositionY(),
	        		wyDevice::winWidth - 60,
	        		wyDevice::winHeight / 2,
	        		wyDevice::winWidth / 2,
	        		wyDevice::winHeight * 3 / 4,
	        		wyDevice::winWidth / 2,
	        		wyDevice::winHeight / 4);
            wyLagrange* a = wyLagrange::make(3, m_config);
			a->setAutoRotate(true, 90);
			/*
			 * 如果不启用auto rotate, 可以指定pin point, 注意pin point只在auto rotate为false的时候生效
			 * 下面两句取消注释时必须把a->setAutoRotate(true, 90);注释掉才行
			 */
//	        a->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
//	        a->setPinAngleDelta(90);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(a,
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        }

        virtual void draw() {
        	// draw lagrange curve so we can see the node is follow the track
        	glColor4f(0, 1, 0, 1);
        	wyDrawLagrange(m_config, 30);

        	// draw lagrange start, end, and control points
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
        	wyDrawPoint(m_config.startX, m_config.startY);
        	wyDrawPoint(m_config.cp1X, m_config.cp1Y);
        	wyDrawPoint(m_config.cp2X, m_config.cp2Y);
        	wyDrawPoint(m_config.endX, m_config.endY);

        	glColor4f(1, 1, 1, 1);
        }
    };

    class wyMoveByTestLayer : public wyActionTestLayer {
    public:
        wyMoveByTestLayer() {
        	reorderChild(m_Sprite, -1);
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);

	        wyMoveBy* m = wyMoveBy::make(2, wyDevice::winWidth - 120, 0);
	        m->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
	        m->setPinAngleDelta(90);
	        wyMoveBy* r = (wyMoveBy*)m->reverse()->autoRelease();
            wyIntervalAction* t = wySequence::make(m, r, NULL);

            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        }

        virtual void draw() {
        	// draw pin point
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
        	wyDrawPoint(DP(100), wyDevice::winHeight - DP(100));

        	// draw anchor point
        	wyPoint anchor = wyp(m_Sprite->getAnchorX(), m_Sprite->getAnchorY());
        	anchor = m_Sprite->nodeToWorldSpace(anchor);
        	wyDrawPoint(anchor.x, anchor.y);

        	// draw line between pin point and anchor point
        	glColor4f(0, 1, 0, 1);
        	wyDrawDashLine(DP(100), wyDevice::winHeight - DP(100), anchor.x, anchor.y, 5);
        }
    };

    class wyMoveByAngleTestLayer : public wyActionTestLayer {
    public:
    	wyMoveByAngleTestLayer() {
    		reorderChild(m_Sprite, -1);
			m_Sprite->setPosition(60, wyDevice::winHeight / 2);
			int velocity = (int)(wyDevice::winWidth - 60 * 2) / 2;

			wyMoveByAngle* a = new wyMoveByAngle(2, 0, velocity);
	        a->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
	        a->setPinAngleDelta(90);
			wyMoveByAngle* r = (wyMoveByAngle*)a->reverse()->autoRelease();
			wyIntervalAction* t = wySequence::make(a, r, NULL);

			wyAction* action = wyRepeatForever::make(t);
			m_Sprite->runAction(action);
		}

        virtual void draw() {
        	// draw pin point
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
        	wyDrawPoint(DP(100), wyDevice::winHeight - DP(100));

        	// draw anchor point
        	wyPoint anchor = wyp(m_Sprite->getAnchorX(), m_Sprite->getAnchorY());
        	anchor = m_Sprite->nodeToWorldSpace(anchor);
        	wyDrawPoint(anchor.x, anchor.y);

        	// draw line between pin point and anchor point
        	glColor4f(0, 1, 0, 1);
        	wyDrawDashLine(DP(100), wyDevice::winHeight - DP(100), anchor.x, anchor.y, 5);
        }
	};

    class wyMoveByPathTestLayer : public wyActionTestLayer {
    private:
    	wyMoveByPath* m_path;

    public:
    	wyMoveByPathTestLayer() {
    		m_Sprite->setPosition(DP(30), wyDevice::winHeight / 2);

    		wyMoveByPath* a = wyMoveByPath::make();
    		m_path = a;
			a->setAutoRotate(true, 90);
			/*
			 * 如果不启用auto rotate, 可以指定pin point, 注意pin point只在auto rotate为false的时候生效
			 * 下面两句取消注释时必须把a->setAutoRotate(true, 90);注释掉才行
			 */
//	        a->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
//	        a->setPinAngleDelta(90);
			a->addPoint(DP(30), wyDevice::winHeight / 2, 0);
			a->addPoint(wyDevice::winWidth / 2, wyDevice::winHeight / 2, 0.5f);
			a->addPoint(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30), 1.f);
			a->addPoint(wyDevice::winWidth - DP(30), wyDevice::winHeight - DP(30), 2.f);
			a->addPoint(wyDevice::winWidth - DP(30), DP(30), 3.f);
			wyAction* r = a->reverse();
			wyIntervalAction* t = wySequence::make(a,
					(wyFiniteTimeAction*)r->autoRelease(),
					NULL);

			wyAction* action = wyRepeatForever::make(t);
			m_Sprite->runAction(action);
		}

        virtual void draw() {
        	// draw bezier curve so we can see the node is follow the track
        	glColor4f(0, 1, 0, 1);
        	float* points = m_path->getPoints();
        	int count = m_path->getPointCount();
        	wyDrawDashPath(points, count * 2, 5);

        	// draw bezier start, end, and control points
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
        	wyDrawPoints(points, count * 2);

        	glColor4f(1, 1, 1, 1);
        }
	};

    class wyMoveToTestLayer : public wyActionTestLayer {
    public:
        wyMoveToTestLayer(){
        	reorderChild(m_Sprite, -1);
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);

	        wyMoveTo* a = new wyMoveTo(2, m_Sprite->getPositionX(), m_Sprite->getPositionY(),
                                               wyDevice::winWidth - 60, wyDevice::winHeight/2);
	        a->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
	        a->setPinAngleDelta(90);
	        wyMoveTo* r = (wyMoveTo*)a->reverse()->autoRelease();
            wyIntervalAction* t = wySequence::make(a, r, NULL);

            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        }

        virtual void draw() {
        	// draw pin point
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
        	wyDrawPoint(DP(100), wyDevice::winHeight - DP(100));

        	// draw anchor point
        	wyPoint anchor = wyp(m_Sprite->getAnchorX(), m_Sprite->getAnchorY());
        	anchor = m_Sprite->nodeToWorldSpace(anchor);
        	wyDrawPoint(anchor.x, anchor.y);

        	// draw line between pin point and anchor point
        	glColor4f(0, 1, 0, 1);
        	wyDrawDashLine(DP(100), wyDevice::winHeight - DP(100), anchor.x, anchor.y, 5);
        }
    };

    class wyScaleByTestLayer : public wyActionTestLayer {
    public:
        wyScaleByTestLayer(){
            wyIntervalAction* a = new wyScaleBy(2, 2);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            wyObjectRelease(action);
        }
    };

    class wyScaleToTestLayer : public wyActionTestLayer {
    public:
        wyScaleToTestLayer(){
            wyIntervalAction* a = new wyScaleTo(2, 1.0f, 0.0f);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            wyObjectRelease(action);
        }
    };

    class wyShakeTestLayer : public wyActionTestLayer {
    public:
    	wyShakeTestLayer() {
            wyIntervalAction* a = new wyShake(5, 10);
            m_Sprite->runAction(a);
            wyObjectRelease(a);
        }
    };

    class wyRotateByTestLayer : public wyActionTestLayer {
    public:
        wyRotateByTestLayer() {
            wyIntervalAction* rotate = new wyRotateBy(2, 360);
            wyAction* action = new wyRepeatForever(rotate);
            m_Sprite->runAction(action);
            wyObjectRelease(rotate);
            wyObjectRelease(action);
        }
    };

    class wyRotateToTestLayer : public wyActionTestLayer {
    public:
        wyRotateToTestLayer() {
            wyIntervalAction* rotate = new wyRotateTo(2, 0, 360);
            wyAction* action = new wyRepeatForever(rotate);
            m_Sprite->runAction(action);
            wyObjectRelease(rotate);
            wyObjectRelease(action);
        }
    };

    class wyPauseResumeActionTestLayer : public wyActionTestLayer {
    public:
    	wyPauseResumeActionTestLayer() {
    		/*
    		 * 我们同时在sprite上运行两个动作, 点击屏幕时暂停旋转动作, 再点击恢复
    		 * 为什么不用wySpawn来包装两个动作呢? 因为暂停动作对于组合类中的动作
    		 * 是无效的
    		 */

    		// run a rotate action
            wyIntervalAction* rotate = wyRotateBy::make(2, 360);
            wyAction* action1 = wyRepeatForever::make(rotate);
			action1->setTag(100); // for pause action, assign a tag to action
            m_Sprite->runAction(action1);

            // run a move action
	        wyMoveBy* m = wyMoveBy::make(2, wyDevice::winWidth - 120, 0);
	        wyMoveBy* r = (wyMoveBy*)m->reverse()->autoRelease();
            wyIntervalAction* t = wySequence::make(m, r, NULL);
            wyAction* action2 = wyRepeatForever::make(t);
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);
            m_Sprite->runAction(action2);

            // enable touch
            setTouchEnabled(true);
        }

    	virtual ~wyPauseResumeActionTestLayer() {
    	}

    	virtual bool touchesBegan(wyMotionEvent& e) {
    		wyAction* action = m_Sprite->getAction(100);
    		if(action->isPaused())
    			m_Sprite->resumeAction(100);
    		else
    			m_Sprite->pauseAction(100);

    		return true;
    	}
    };

    class wyProgressByTestLayer : public wyLayer {
    private:
    	float ITEM_WIDTH;
    	float ITEM_HEIGHT;

    public:
        wyProgressByTestLayer(){
        	wyProgressTimer* left = new wyProgressTimer(wyTexture2D::makePNG(RES("R.drawable.grossini")));
            left->setStyle(RADIAL_CW);
        	left->setPosition(100, wyDevice::winHeight / 2);
        	wyProgressBy* action = new wyProgressBy(2, 100);
        	addChildLocked((wyNode*)left->autoRelease());
        	left->runAction(action);
        	wyObjectRelease(action);

    		ITEM_WIDTH = 85;
    		ITEM_HEIGHT = 121;
    		int idx = rand() % 14;
    		float x = (idx % 5) * ITEM_WIDTH;
    		float y = (idx / 5) * ITEM_HEIGHT;
    		wyTexture2D* tex = wyTexture2D::makePNG(RES("R.drawable.grossini_dance_atlas"));
    		wySprite* sprite = wySprite::make(tex, wyUtils::resolveRect(wyr(x, y, ITEM_WIDTH, ITEM_HEIGHT)));
    		wyProgressTimer* right = new wyProgressTimer(sprite);
            right->setStyle(VERTICAL_BAR_BT);
        	right->setPosition(wyDevice::winWidth - 100, wyDevice::winHeight / 2);
        	action = new wyProgressBy(2, 100);
        	addChildLocked((wyNode*)right->autoRelease());
        	right->runAction(action);
        	wyObjectRelease(action);
        }
    };

    class wyProgressToTestLayer : public wyLayer {
    private:
    	float ITEM_WIDTH;
    	float ITEM_HEIGHT;

    public:
        wyProgressToTestLayer(){
        	wyProgressTimer* left = new wyProgressTimer(wyTexture2D::makePNG(RES("R.drawable.grossini")));
            left->setStyle(RADIAL_CW);
        	left->setPosition(100, wyDevice::winHeight / 2);
        	wyProgressTo* action = new wyProgressTo(2, 0, 100);
        	wyRepeatForever* r = new wyRepeatForever(action);
        	left->runAction(r);
        	addChildLocked((wyNode*)left->autoRelease());
        	wyObjectRelease(action);
        	wyObjectRelease(r);
            
    		ITEM_WIDTH = 85;
    		ITEM_HEIGHT = 121;
    		int idx = rand() % 14;
    		float x = (idx % 5) * ITEM_WIDTH;
    		float y = (idx / 5) * ITEM_HEIGHT;
    		wyTexture2D* tex = wyTexture2D::makePNG(RES("R.drawable.grossini_dance_atlas"));
    		wySprite* sprite = wySprite::make(tex, wyUtils::resolveRect(wyr(x, y, ITEM_WIDTH, ITEM_HEIGHT)));
    		wyProgressTimer* right = new wyProgressTimer(sprite);
            right->setStyle(VERTICAL_BAR_BT);
        	right->setPosition(wyDevice::winWidth - 100, wyDevice::winHeight / 2);
        	action = new wyProgressTo(2, 0, 100);
        	r = new wyRepeatForever(action);
        	addChildLocked((wyNode*)right->autoRelease());
        	right->runAction(action);
        	wyObjectRelease(action);
        	wyObjectRelease(r);
        };
    };

    class wyTintByTestLayer : public wyActionTestLayer {
    public:
        wyTintByTestLayer(){
            wyIntervalAction* a = new wyTintBy(2, 255, 0, 0);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyTintToTestLayer : public wyActionTestLayer {
    public:
        wyTintToTestLayer(){
            wyIntervalAction* a = new wyTintTo(2, 255, 0, 0, 0, 0, 255);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(
            		(wyFiniteTimeAction*)a->autoRelease(),
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);

            wyAction* action = new wyRepeatForever(t);
            m_Sprite->runAction(action);
            wyObjectRelease(action);
        };
    };

    class wyOrbitCameraFlipXTestLayer : public wyActionTestLayer {
    public:
        wyOrbitCameraFlipXTestLayer() {
        	m_Sprite->setScale(4);

            wyIntervalAction* action = new wyOrbitCamera(3, 1, 0, 0, 360, 0, 0);
            m_Sprite->runAction(action);
            wyObjectRelease(action);

        	wySprite* bg = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.background")));
        	bg->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
        	addChildLocked(bg, -1);
        };
    };

    class wyBezierTestLayer : public wyActionTestLayer {
    private:
    	wyBezierConfig m_config;

    public:
        wyBezierTestLayer(){
	        m_Sprite->setPosition(60, wyDevice::winHeight / 2);

	        m_config = wybcCubic(m_Sprite->getPositionX(),
	        		m_Sprite->getPositionY(),
	        		wyDevice::winWidth - 60,
	        		wyDevice::winHeight / 2,
	        		wyDevice::winWidth / 2,
	        		wyDevice::winHeight * 3 / 4,
	        		wyDevice::winWidth / 2,
	        		wyDevice::winHeight / 4);
	        wyBezier* a = wyBezier::make(3, m_config);
	        a->setAutoRotate(true, 90);
			/*
			 * 如果不启用auto rotate, 可以指定pin point, 注意pin point只在auto rotate为false的时候生效
			 * 下面两句取消注释时必须把a->setAutoRotate(true, 90);注释掉才行
			 */
//	        a->setPinPoint(DP(100), wyDevice::winHeight - DP(100));
//	        a->setPinAngleDelta(90);
            wyAction* r = a->reverse();
            wyIntervalAction* t = wySequence::make(a,
            		(wyFiniteTimeAction*)r->autoRelease(),
            		NULL);
    
            wyAction* action = wyRepeatForever::make(t);
            m_Sprite->runAction(action);
        }

        virtual void draw() {
        	// draw bezier curve so we can see the node is follow the track
        	glColor4f(0, 1, 0, 1);
        	wyDrawBezier(m_config, 30);

        	// draw bezier start, end, and control points
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
        	wyDrawPoint(m_config.startX, m_config.startY);
        	wyDrawPoint(m_config.cp1X, m_config.cp1Y);
        	wyDrawPoint(m_config.cp2X, m_config.cp2Y);
        	wyDrawPoint(m_config.endX, m_config.endY);

        	glColor4f(1, 1, 1, 1);
        }
    };
    
}

using namespace Action;

#if ANDROID
	#define DEMO_ENTRY_IMPL(CLASSNAME) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_action_##CLASSNAME##_nativeStart \
		(JNIEnv *env, jobject thiz){ \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _action_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(AnimationTest);
DEMO_ENTRY_IMPL(AtlasAnimationTest);
DEMO_ENTRY_IMPL(BezierTest);
DEMO_ENTRY_IMPL(BlinkTest);
DEMO_ENTRY_IMPL(CallFuncTest);
DEMO_ENTRY_IMPL(DelayShowHideTest);
DEMO_ENTRY_IMPL(FadeInOutTest);
DEMO_ENTRY_IMPL(FadeToTest);
DEMO_ENTRY_IMPL(FollowTest);
DEMO_ENTRY_IMPL(JumpByTest);
DEMO_ENTRY_IMPL(JumpToTest);
DEMO_ENTRY_IMPL(HypotrochoidTest);
DEMO_ENTRY_IMPL(LagrangeTest);
DEMO_ENTRY_IMPL(MoveByTest);
DEMO_ENTRY_IMPL(MoveByAngleTest);
DEMO_ENTRY_IMPL(MoveByPathTest);
DEMO_ENTRY_IMPL(MoveToTest);
DEMO_ENTRY_IMPL(OrbitCameraFlipXTest);
DEMO_ENTRY_IMPL(PauseResumeActionTest);
DEMO_ENTRY_IMPL(ProgressByTest);
DEMO_ENTRY_IMPL(ProgressToTest);
DEMO_ENTRY_IMPL(ScaleByTest);
DEMO_ENTRY_IMPL(ScaleToTest);
DEMO_ENTRY_IMPL(ShakeTest);
DEMO_ENTRY_IMPL(RotateByTest);
DEMO_ENTRY_IMPL(RotateToTest);
DEMO_ENTRY_IMPL(TintByTest);
DEMO_ENTRY_IMPL(TintToTest);
