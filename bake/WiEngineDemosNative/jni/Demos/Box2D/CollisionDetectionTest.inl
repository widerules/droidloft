class wyCollisionDetectionTestLayer : public wyLayer {
private:
	wySprite* m_sprite1;
	wySprite* m_sprite2;
	wyBox2DCollisionDetector* m_detector;
	wyBox2DCDResult m_result;

public:
	wyCollisionDetectionTestLayer() {
		// create sprite1
		float size = DP(32.0f);
		wyTexture2D* tex = wyTexture2D::makePNG(RES("R.drawable.blocks"));
		m_sprite1 = wySprite::make(tex, wyr(wyMath::randMax(2) * size, wyMath::randMax(2) * size, size, size));
		addChildLocked(m_sprite1, -1);

		// create sprite2
		m_sprite2 = wySprite::make(tex, wyr(wyMath::randMax(2) * size, wyMath::randMax(2) * size, size, size));
		addChildLocked(m_sprite2, -1);

		// init and begin
		reset();

		// create collision detector
		m_detector = wyBox2DCollisionDetector::make();
		m_detector->retain();

		// schedule a timer to check collision
		wyTargetSelector* ts = wyTargetSelector::make(this, SEL(wyCollisionDetectionTestLayer::checkCollision), NULL);
		wyTimer* t = wyTimer::make(ts);
		scheduleLocked(t);

		// touch enable
		setTouchEnabled(true);
	}

	virtual ~wyCollisionDetectionTestLayer() {
		m_detector->release();
	}

	void reset() {
		// clear result
		memset(&m_result, 0, sizeof(wyBox2DCDResult));

		// relocate sprite
		m_sprite1->setRotation(wyMath::randMax(360));
		m_sprite1->setPosition(0, 0);
		m_sprite2->setRotation(wyMath::randMax(360));
		m_sprite2->setPosition(wyDevice::winWidth, wyDevice::winHeight);

		// re-run actions
		wyMoveBy* move1 = wyMoveBy::make(4, wyDevice::winWidth, wyDevice::winHeight);
		wyRotateBy* rotate1 = wyRotateBy::make(4, wyMath::randMax(360));
		m_sprite1->runAction(move1);
		m_sprite1->runAction(rotate1);
		wyMoveBy* move2 = wyMoveBy::make(4, -wyDevice::winWidth, -wyDevice::winHeight);
		m_sprite2->runAction(move2);
		wyRotateBy* rotate2 = wyRotateBy::make(4, wyMath::randMax(360));
		m_sprite2->runAction(rotate2);
	}

	void checkCollision() {
		if(m_result.pointCount == 0) {
			if(m_detector->isCollided(m_sprite1, m_sprite2, &m_result)) {
				m_sprite1->stopAllActions();
				m_sprite2->stopAllActions();
			}
		}
	}

	virtual void draw() {
		if(m_result.pointCount > 0) {
			// draw collision point
        	glColor4f(1, 0, 0, 1);
        	glPointSize(5);
			wyDrawPoints((float*)m_result.points, m_result.pointCount * 2);

        	// draw normal
			wyDrawLine(m_result.points[0].x, m_result.points[0].y,
					   m_result.points[0].x + m_result.normal.x * 50, m_result.points[0].y + m_result.normal.y * 50);

        	glColor4f(1, 1, 1, 1);
		}
	}

	virtual bool touchesBegan(wyMotionEvent& e) {
		if(m_result.pointCount > 0) {
			reset();
		}
		return wyLayer::touchesBegan(e);
	}
};
