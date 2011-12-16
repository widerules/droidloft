class wyPhysicsEditorTestLayer : public wyLayer {
private:
    wyBox2D* m_box2d;
    wyBox2DPELoader* m_bodyLoader;

    wyArray* m_arrayBodies;    

public:
    wyPhysicsEditorTestLayer() {
    	m_arrayBodies = wyArrayNew(20);
	    setTouchEnabled(true);

	    m_bodyLoader = new wyBox2DPELoader(RES("R.raw.shapedefs_box2d"));
	    m_box2d = new wyBox2D();
	    m_box2d->setMeterPixels(m_bodyLoader->getMeterPixels());
	    m_box2d->setDebugDraw(false);

	    m_box2d->getWorld()->SetGravity(b2Vec2(0, -10.0));
	    m_box2d->autoRelease();
	
	    b2BodyDef bd;
	    bd.type = b2_staticBody;
	    bd.position = b2Vec2(m_box2d->pixel2Meter(wyDevice::winWidth / 2), m_box2d->pixel2Meter(10.0f));
	    b2FixtureDef fd;
	    b2PolygonShape* shape = new b2PolygonShape();
	    shape->SetAsBox(m_box2d->pixel2Meter(wyDevice::winWidth), m_box2d->pixel2Meter(2.0f));
	    fd.shape = shape;
	    b2Body* body = m_box2d->getWorld()->CreateBody(&bd);
	    body->CreateFixture(&fd);
	    delete shape;
	
	    addChildLocked(m_box2d);
	  
		wyTargetSelector* ts = new wyTargetSelector(this, 0, NULL);
		wyTimer* t = new wyTimer(ts);
		scheduleLocked(t);
		t->release();
		ts->release();

		wyUtils::showToast("touch to add body");
	}
	
    virtual ~wyPhysicsEditorTestLayer() {
	    wyArrayDestroy(m_arrayBodies);
	    wyObjectRelease(m_bodyLoader);
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector * ts) {
	    m_box2d->getWorld()->Step(1.0f / 60, 10, 10);
	    m_box2d->getWorld()->ClearForces();
	
	    for(int i = 0; i < m_arrayBodies->num; ++i) {
	        b2Body* body = (b2Body*)m_arrayBodies->arr[i];
	        if(body) {
			    b2Vec2 pos = body->GetPosition();
			    float angle = body->GetAngle();
			
				wySprite* sprite = (wySprite*)body->GetUserData();
			    sprite->setPosition(m_box2d->meter2Pixel(pos.x), m_box2d->meter2Pixel(pos.y));
			    sprite->setRotation(-wyMath::r2d(angle));
			    sprite->setVisible(true);
			
			    if(pos.y < 0) {
				    m_box2d->getWorld()->DestroyBody(body);
				    m_box2d->removeChildLocked(sprite, NULL);
				    m_arrayBodies->arr[i] = NULL;
			    }
	        }
	    }
	
	    for(int i = m_arrayBodies->num - 1; i >= 0; --i) {
	        if(m_arrayBodies->arr[i] == NULL) {
	            wyArrayDeleteIndex(m_arrayBodies, i);
	        }
	    }
	}

    virtual bool touchesBegan(wyMotionEvent& e) {	
	    int randNum = rand() % 7;
	    float radian = wyMath::d2r(wyMath::randMinusOneToOne() * 360);
	
	    b2Body* body = NULL;
	    wySprite* sprite = NULL;
	    wyPoint anchorPercent;
	    switch(randNum) {
	        case 0:
	            body = m_bodyLoader->createBodyByName(m_box2d, "drink");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("drink", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("drink");
	            break;
	        case 1:
	            body = m_bodyLoader->createBodyByName(m_box2d, "hamburger");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("hamburger", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("hamburger");
	            break;            
	        case 2:
	            body = m_bodyLoader->createBodyByName(m_box2d, "hotdog");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("hotdog", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("hotdog");
	            break;
	        case 3:
	            body = m_bodyLoader->createBodyByName(m_box2d, "icecream");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("icecream", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("icecream");
	            break;            
	        case 4:
	            body = m_bodyLoader->createBodyByName(m_box2d, "icecream2");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("icecream2", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("icecream2");
	            break;
	        case 5:
	            body = m_bodyLoader->createBodyByName(m_box2d, "icecream3");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("icecream3", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("icecream3");
	            break;
	        case 6:
	            body = m_bodyLoader->createBodyByName(m_box2d, "orange");
	            sprite = wySprite::make(wyTexture2D::makePNG(wyUtils::getResId("orange", "drawable", NULL)));
	            anchorPercent = m_bodyLoader->getAnchorPercent("orange");
	            break;	        
	    } 

	    body->SetTransform(b2Vec2(m_box2d->pixel2Meter(e.x[0]), m_box2d->pixel2Meter(e.y[0])), radian);
	
	    sprite->setAnchorPercent(anchorPercent.x, anchorPercent.y);
	    m_box2d->addChildLocked(sprite);
	    sprite->setVisible(false);
	    body->SetUserData(sprite);
	    
	    wyArrayPush(m_arrayBodies, body); 
        return true;
	}
};
