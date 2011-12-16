class wyCarTestLayer : public wyBox2DTestLayer {
private:
	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	float m_hz;
	float m_zeta;
	float m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;

public:
	wyCarTestLayer() {
		m_hz = 4.0f;
		m_zeta = 0.7f;
		m_speed = 50.0f;

		// get world
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		// create ground
		b2Body* ground = NULL;
		{
			// create ground body
			b2BodyDef bd;
			ground = world->CreateBody(&bd);

			// create ground shape
			b2EdgeShape shape;
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 0.0f;
			fd.friction = 0.6f;
			shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
			ground->CreateFixture(&fd);

			float hs[10] = {0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f};
			float x = 20.0f, y1 = 0.0f, dx = 5.0f;
			for (int i = 0; i < 10; ++i)
			{
				float y2 = hs[i];
				shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
				ground->CreateFixture(&fd);
				y1 = y2;
				x += dx;
			}
			for (int i = 0; i < 10; ++i)
			{
				float y2 = hs[i];
				shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
				ground->CreateFixture(&fd);
				y1 = y2;
				x += dx;
			}

			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
			ground->CreateFixture(&fd);

			x += 80.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
			ground->CreateFixture(&fd);

			x += 40.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
			ground->CreateFixture(&fd);

			x += 20.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
			ground->CreateFixture(&fd);

			x += 40.0f;
			shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
			ground->CreateFixture(&fd);
		}

		// Teeter
		{
			b2BodyDef bd;
			bd.position.Set(140.0f, 1.0f);
			bd.type = b2_dynamicBody;
			b2Body* body = world->CreateBody(&bd);

			b2PolygonShape box;
			box.SetAsBox(10.0f, 0.25f);
			body->CreateFixture(&box, 1.0f);

			b2RevoluteJointDef jd;
			jd.Initialize(ground, body, body->GetPosition());
			jd.lowerAngle = -8.0f * b2_pi / 180.0f;
			jd.upperAngle = 8.0f * b2_pi / 180.0f;
			jd.enableLimit = true;
			world->CreateJoint(&jd);

			body->ApplyAngularImpulse(100.0f);
		}

		// Bridge
		{
			int N = 20;
			b2PolygonShape shape;
			shape.SetAsBox(1.0f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.6f;

			b2RevoluteJointDef jd;

			b2Body* prevBody = ground;
			for (int i = 0; i < N; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(161.0f + 2.0f * i, -0.125f);
				b2Body* body = world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(160.0f + 2.0f * i, -0.125f);
				jd.Initialize(prevBody, body, anchor);
				world->CreateJoint(&jd);

				prevBody = body;
			}

			b2Vec2 anchor(160.0f + 2.0f * N, -0.125f);
			jd.Initialize(prevBody, ground, anchor);
			world->CreateJoint(&jd);
		}

		// Boxes
		{
			b2PolygonShape box;
			box.SetAsBox(0.5f, 0.5f);

			b2Body* body = NULL;
			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			bd.position.Set(230.0f, 0.5f);
			body = world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);

			bd.position.Set(230.0f, 1.5f);
			body = world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);

			bd.position.Set(230.0f, 2.5f);
			body = world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);

			bd.position.Set(230.0f, 3.5f);
			body = world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);

			bd.position.Set(230.0f, 4.5f);
			body = world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);
		}

		// Car
		{
			b2PolygonShape chassis;
			b2Vec2 vertices[8];
			vertices[0].Set(-1.5f, -0.5f);
			vertices[1].Set(1.5f, -0.5f);
			vertices[2].Set(1.5f, 0.0f);
			vertices[3].Set(0.0f, 0.9f);
			vertices[4].Set(-1.15f, 0.9f);
			vertices[5].Set(-1.5f, 0.2f);
			chassis.Set(vertices, 6);

			b2CircleShape circle;
			circle.m_radius = 0.4f;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 1.0f);
			m_car = world->CreateBody(&bd);
			m_car->CreateFixture(&chassis, 1.0f);

			b2FixtureDef fd;
			fd.shape = &circle;
			fd.density = 1.0f;
			fd.friction = 0.9f;

			bd.position.Set(-1.0f, 0.35f);
			m_wheel1 = world->CreateBody(&bd);
			m_wheel1->CreateFixture(&fd);

			bd.position.Set(1.0f, 0.4f);
			m_wheel2 = world->CreateBody(&bd);
			m_wheel2->CreateFixture(&fd);

			b2WheelJointDef jd;
			b2Vec2 axis(0.0f, 1.0f);

			jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
			jd.motorSpeed = 0.0f;
			jd.maxMotorTorque = 20.0f;
			jd.enableMotor = true;
			jd.frequencyHz = m_hz;
			jd.dampingRatio = m_zeta;
			m_spring1 = (b2WheelJoint*)world->CreateJoint(&jd);

			jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
			jd.motorSpeed = 0.0f;
			jd.maxMotorTorque = 10.0f;
			jd.enableMotor = false;
			jd.frequencyHz = m_hz;
			jd.dampingRatio = m_zeta;
			m_spring2 = (b2WheelJoint*)world->CreateJoint(&jd);
		}

		// backward button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(100), DP(44));
		pressed->setContentSize(DP(100), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(DP(55), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for backward button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Backward");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(55), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// stop button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
		button->setPosition(DP(160), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for stop button
		label = wyBitmapFontLabel::make(font, "Stop");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(160), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// forward button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(3, NULL));
		button->setPosition(DP(265), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for forward button
		label = wyBitmapFontLabel::make(font, "Forward");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(265), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// freq. down button
		normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(150), DP(44));
		pressed->setContentSize(DP(150), DP(44));
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(4, NULL));
		button->setPosition(DP(80), wyDevice::winHeight - DP(80));
		addChildLocked(button, 1);
		button->release();

		// label for freq. down button
		label = wyBitmapFontLabel::make(font, "Freq. Down");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(80), wyDevice::winHeight - DP(80));
		addChildLocked(label, 1);

		// freq. up button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(5, NULL));
		button->setPosition(DP(240), wyDevice::winHeight - DP(80));
		addChildLocked(button, 1);
		button->release();

		// label for freq. up button
		label = wyBitmapFontLabel::make(font, "Freq. Up");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(240), wyDevice::winHeight - DP(80));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyCarTestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				m_spring1->SetMotorSpeed(m_speed);
				break;
			case 2:
				m_spring1->SetMotorSpeed(0.0f);
				break;
			case 3:
				m_spring1->SetMotorSpeed(-m_speed);
				break;
			case 4:
				m_hz = b2Max(0.0f, m_hz - 1.0f);
				m_spring1->SetSpringFrequencyHz(m_hz);
				m_spring2->SetSpringFrequencyHz(m_hz);
				break;
			case 5:
				m_hz += 1.0f;
				m_spring1->SetSpringFrequencyHz(m_hz);
				m_spring2->SetSpringFrequencyHz(m_hz);
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}

	virtual void updateWorld(float delta) {
		wyBox2DTestLayer::updateWorld(delta);

		// move the scene, keep the car center
		const b2Vec2& carPos = m_car->GetPosition();
		float pX = m_box2d->meter2Pixel(carPos.x);
		m_box2d->setPosition(-pX + wyDevice::winWidth / 2, DP(100));
	}
};
