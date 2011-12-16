class wyWebTestLayer : public wyBox2DTestLayer, public b2DestructionListener {
private:
	b2Body* m_bodies[4];
	b2Joint* m_joints[8];

public:
	wyWebTestLayer() {
		// get world
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// set self as destruction listener
		world->SetDestructionListener(this);

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			bd.position.Set(-5.0f, 5.0f);
			m_bodies[0] = world->CreateBody(&bd);
			m_bodies[0]->CreateFixture(&shape, 5.0f);

			bd.position.Set(5.0f, 5.0f);
			m_bodies[1] = world->CreateBody(&bd);
			m_bodies[1]->CreateFixture(&shape, 5.0f);

			bd.position.Set(5.0f, 15.0f);
			m_bodies[2] = world->CreateBody(&bd);
			m_bodies[2]->CreateFixture(&shape, 5.0f);

			bd.position.Set(-5.0f, 15.0f);
			m_bodies[3] = world->CreateBody(&bd);
			m_bodies[3]->CreateFixture(&shape, 5.0f);

			b2DistanceJointDef jd;
			b2Vec2 p1, p2, d;

			jd.frequencyHz = 4.0f;
			jd.dampingRatio = 0.5f;

			jd.bodyA = ground;
			jd.bodyB = m_bodies[0];
			jd.localAnchorA.Set(-10.0f, 0.0f);
			jd.localAnchorB.Set(-0.5f, -0.5f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[0] = world->CreateJoint(&jd);

			jd.bodyA = ground;
			jd.bodyB = m_bodies[1];
			jd.localAnchorA.Set(10.0f, 0.0f);
			jd.localAnchorB.Set(0.5f, -0.5f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[1] = world->CreateJoint(&jd);

			jd.bodyA = ground;
			jd.bodyB = m_bodies[2];
			jd.localAnchorA.Set(10.0f, 20.0f);
			jd.localAnchorB.Set(0.5f, 0.5f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[2] = world->CreateJoint(&jd);

			jd.bodyA = ground;
			jd.bodyB = m_bodies[3];
			jd.localAnchorA.Set(-10.0f, 20.0f);
			jd.localAnchorB.Set(-0.5f, 0.5f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[3] = world->CreateJoint(&jd);

			jd.bodyA = m_bodies[0];
			jd.bodyB = m_bodies[1];
			jd.localAnchorA.Set(0.5f, 0.0f);
			jd.localAnchorB.Set(-0.5f, 0.0f);;
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[4] = world->CreateJoint(&jd);

			jd.bodyA = m_bodies[1];
			jd.bodyB = m_bodies[2];
			jd.localAnchorA.Set(0.0f, 0.5f);
			jd.localAnchorB.Set(0.0f, -0.5f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[5] = world->CreateJoint(&jd);

			jd.bodyA = m_bodies[2];
			jd.bodyB = m_bodies[3];
			jd.localAnchorA.Set(-0.5f, 0.0f);
			jd.localAnchorB.Set(0.5f, 0.0f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[6] = world->CreateJoint(&jd);

			jd.bodyA = m_bodies[3];
			jd.bodyB = m_bodies[0];
			jd.localAnchorA.Set(0.0f, -0.5f);
			jd.localAnchorB.Set(0.0f, 0.5f);
			p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
			p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
			d = p2 - p1;
			jd.length = d.Length();
			m_joints[7] = world->CreateJoint(&jd);
		}

		setKeyEnabled(true);

		// delete body button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(150), DP(44));
		pressed->setContentSize(DP(150), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(DP(80), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for delete body button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Delete Body");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(80), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// delete joint button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
		button->setPosition(DP(240), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for delete joint button
		label = wyBitmapFontLabel::make(font, "Delete Joint");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(240), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyWebTestLayer() {
	}

	void JointDestroyed(b2Joint* joint) {
		for(int32 i = 0; i < 8; ++i) {
			if(m_joints[i] == joint) {
				m_joints[i] = NULL;
				break;
			}
		}
	}

	/// Called when any joint is about to be destroyed due
	/// to the destruction of one of its attached bodies.
	virtual void SayGoodbye(b2Joint* joint) {
		if(m_mouseJoint == joint) {
			m_mouseJoint = NULL;
		} else {
			JointDestroyed(joint);
		}
	}

	/// Called when any fixture is about to be destroyed due
	/// to the destruction of its parent body.
	virtual void SayGoodbye(b2Fixture* fixture) {
	}

	virtual bool keyDown(wyKeyEvent& event) {
		// get world
		b2World* world = m_box2d->getWorld();

		switch(event.keyCode) {
			case KEYCODE_MENU:
				for(int32 i = 0; i < 4; ++i) {
					if(m_bodies[i]) {
						world->DestroyBody(m_bodies[i]);
						m_bodies[i] = NULL;
						break;
					}
				}
				break;
			case KEYCODE_SEARCH:
				for(int32 i = 0; i < 8; ++i) {
					if(m_joints[i]) {
						world->DestroyJoint(m_joints[i]);
						m_joints[i] = NULL;
						break;
					}
				}
				break;
			default:
				return false;
		}

		return true;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		// get world
		b2World* world = m_box2d->getWorld();

		switch(ts->getId()) {
			case 1:
				for(int32 i = 0; i < 4; ++i) {
					if(m_bodies[i]) {
						world->DestroyBody(m_bodies[i]);
						m_bodies[i] = NULL;
						break;
					}
				}
				break;
			case 2:
				for(int32 i = 0; i < 8; ++i) {
					if(m_joints[i]) {
						world->DestroyJoint(m_joints[i]);
						m_joints[i] = NULL;
						break;
					}
				}
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
