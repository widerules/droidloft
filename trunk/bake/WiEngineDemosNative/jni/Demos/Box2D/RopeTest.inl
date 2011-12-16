class wyRopeTestLayer : public wyBox2DTestLayer {
private:
	b2RopeJointDef m_ropeDef;
	b2Joint* m_rope;

	wyBitmapFontLabel* m_label;

public:
	wyRopeTestLayer() {
		// get world
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

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
			shape.SetAsBox(0.5f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;
			fd.friction = 0.2f;
			fd.filter.categoryBits = 0x0001;
			fd.filter.maskBits = 0xFFFF & ~0x0002;

			b2RevoluteJointDef jd;
			jd.collideConnected = false;

			const int32 N = 10;
			const float32 y = 15.0f;
			m_ropeDef.localAnchorA.Set(0.0f, y);

			b2Body* prevBody = ground;
			for (int32 i = 0; i < N; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.5f + 1.0f * i, y);
				if (i == N - 1)
				{
					shape.SetAsBox(1.5f, 1.5f);
					fd.density = 100.0f;
					fd.filter.categoryBits = 0x0002;
					bd.position.Set(1.0f * i, y);
					bd.angularDamping = 0.4f;
				}

				b2Body* body = world->CreateBody(&bd);

				body->CreateFixture(&fd);

				b2Vec2 anchor(float32(i), y);
				jd.Initialize(prevBody, body, anchor);
				world->CreateJoint(&jd);

				prevBody = body;
			}

			m_ropeDef.localAnchorB.SetZero();

			float32 extraLength = 0.01f;
			m_ropeDef.maxLength = N - 1.0f + extraLength;
			m_ropeDef.bodyB = prevBody;
		}

		{
			m_ropeDef.bodyA = ground;
			m_rope = world->CreateJoint(&m_ropeDef);
		}

		setKeyEnabled(true);

		// toggle button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(300), DP(44));
		pressed->setContentSize(DP(300), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for toggle button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		m_label = wyBitmapFontLabel::make(font, "Toggle Rope Joint ON");
		m_label->setColor(wyc3b(255, 255, 0));
		m_label->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30));
		addChildLocked(m_label, 1);

		startUpdateWorld();
	}

	virtual ~wyRopeTestLayer() {
	}

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_MENU:
			{
				// get world
				b2World* world = m_box2d->getWorld();

				if(m_rope) {
					world->DestroyJoint(m_rope);
					m_rope = NULL;
					m_label->setText("(Menu)Toggle the rope joint (Rope OFF)");
				} else {
					m_rope = world->CreateJoint(&m_ropeDef);
					m_label->setText("(Menu)Toggle the rope joint (Rope ON)");
				}
				break;
			}
			default:
				return false;
		}

		return true;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
			{
				// get world
				b2World* world = m_box2d->getWorld();

				if(m_rope) {
					world->DestroyJoint(m_rope);
					m_rope = NULL;
					m_label->setText("Toggle Rope Joint OFF");
				} else {
					m_rope = world->CreateJoint(&m_ropeDef);
					m_label->setText("Toggle Rope Joint ON");
				}
				break;
			}
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
