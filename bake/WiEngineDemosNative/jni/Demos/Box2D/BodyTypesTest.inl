class wyBodyTypesTestLayer : public wyBox2DTestLayer {
private:
	b2Body* m_attachment;
	b2Body* m_platform;
	float32 m_speed;

public:
	wyBodyTypesTestLayer() {
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
			shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));

			b2FixtureDef fd;
			fd.shape = &shape;

			ground->CreateFixture(&fd);
		}

		// Define attachment
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 3.0f);
			m_attachment = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 2.0f);
			m_attachment->CreateFixture(&shape, 2.0f);
		}

		// Define platform
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-4.0f, 5.0f);
			m_platform = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 4.0f, b2Vec2(4.0f, 0.0f), 0.5f * b2_pi);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.friction = 0.6f;
			fd.density = 2.0f;
			m_platform->CreateFixture(&fd);

			b2RevoluteJointDef rjd;
			rjd.Initialize(m_attachment, m_platform, b2Vec2(0.0f, 5.0f));
			rjd.maxMotorTorque = 50.0f;
			rjd.enableMotor = true;
			world->CreateJoint(&rjd);

			b2PrismaticJointDef pjd;
			pjd.Initialize(ground, m_platform, b2Vec2(0.0f, 5.0f), b2Vec2(1.0f, 0.0f));

			pjd.maxMotorForce = 1000.0f;
			pjd.enableMotor = true;
			pjd.lowerTranslation = -10.0f;
			pjd.upperTranslation = 10.0f;
			pjd.enableLimit = true;

			(b2PrismaticJoint*)world->CreateJoint(&pjd);

			m_speed = 3.0f;
		}

		// Create a payload
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 8.0f);
			b2Body* body = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.75f, 0.75f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.friction = 0.6f;
			fd.density = 2.0f;

			body->CreateFixture(&fd);
		}

		setKeyEnabled(true);

		// dynamic button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(100), DP(44));
		pressed->setContentSize(DP(100), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(DP(55), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for dynamic button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Dynamic");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(55), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// static button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
		button->setPosition(DP(160), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for static button
		label = wyBitmapFontLabel::make(font, "Static");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(160), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// kinematic button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(3, NULL));
		button->setPosition(DP(265), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for kinematic button
		label = wyBitmapFontLabel::make(font, "Kinematic");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(265), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyBodyTypesTestLayer() {
	}

	virtual void updateWorld(float delta) {
		// Drive the kinematic body.
		if (m_platform->GetType() == b2_kinematicBody)
		{
			b2Vec2 p = m_platform->GetTransform().p;
			b2Vec2 v = m_platform->GetLinearVelocity();

			if ((p.x < -10.0f && v.x < 0.0f) ||
				(p.x > 10.0f && v.x > 0.0f))
			{
				v.x = -v.x;
				m_platform->SetLinearVelocity(v);
			}
		}

		wyBox2DTestLayer::updateWorld(delta);
	}

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_DPAD_UP:
				m_platform->SetType(b2_dynamicBody);
				break;
			case KEYCODE_DPAD_LEFT:
				m_platform->SetType(b2_staticBody);
				break;
			case KEYCODE_DPAD_RIGHT:
				m_platform->SetType(b2_kinematicBody);
				m_platform->SetLinearVelocity(b2Vec2(-m_speed, 0.0f));
				m_platform->SetAngularVelocity(0.0f);
				break;
			case KEYCODE_BACK:
				wyDirector::getInstance()->popScene();
				break;
			default:
				return false;
		}

		return true;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				m_platform->SetType(b2_dynamicBody);
				break;
			case 2:
				m_platform->SetType(b2_staticBody);
				break;
			case 3:
				m_platform->SetType(b2_kinematicBody);
				m_platform->SetLinearVelocity(b2Vec2(-m_speed, 0.0f));
				m_platform->SetAngularVelocity(0.0f);
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
