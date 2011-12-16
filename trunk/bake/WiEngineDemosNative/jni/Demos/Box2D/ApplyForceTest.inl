class wyApplyForceTestLayer : public wyBox2DTestLayer {
private:
	b2Body* m_body;

public:
	wyApplyForceTestLayer() {
		b2World* world = m_box2d->getWorld();

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		const float32 k_restitution = 0.4f;
		b2Body* ground;
		{
			b2BodyDef bd;
			ground = world->CreateBody(&bd);

			b2EdgeShape shape;

			b2FixtureDef sd;
			sd.shape = &shape;
			sd.density = 0.0f;
			sd.restitution = k_restitution;

			// Left vertical
			shape.Set(b2Vec2(-10.0f, 0.0f),
					b2Vec2(-10.0f, m_box2d->pixel2Meter(wyDevice::winHeight)));
			ground->CreateFixture(&sd);

			// Right vertical
			shape.Set(b2Vec2(10.0f, 0.0f),
					b2Vec2(10.0f, m_box2d->pixel2Meter(wyDevice::winHeight)));
			ground->CreateFixture(&sd);

			// Top horizontal
			shape.Set(b2Vec2(-10.0f, m_box2d->pixel2Meter(wyDevice::winHeight)),
					b2Vec2(10.0f, m_box2d->pixel2Meter(wyDevice::winHeight)));
			ground->CreateFixture(&sd);

			// Bottom horizontal
			shape.Set(b2Vec2(-10.0f, 0.0f), b2Vec2(10.0f, 0.0f));
			ground->CreateFixture(&sd);
		}

		{
			b2Transform xf1;
			xf1.q.Set(0.3524f * b2_pi);
			xf1.p = b2Mul(xf1.q, b2Vec2(1.0f, 0.0f));

			b2Vec2 vertices[3];
			vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
			vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
			vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));

			b2PolygonShape poly1;
			poly1.Set(vertices, 3);

			b2FixtureDef sd1;
			sd1.shape = &poly1;
			sd1.density = 4.0f;

			b2Transform xf2;
			xf2.q.Set(-0.3524f * b2_pi);
			xf2.p = b2Mul(xf2.q, b2Vec2(-1.0f, 0.0f));

			vertices[0] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
			vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
			vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));

			b2PolygonShape poly2;
			poly2.Set(vertices, 3);

			b2FixtureDef sd2;
			sd2.shape = &poly2;
			sd2.density = 2.0f;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.angularDamping = 5.0f;
			bd.linearDamping = 0.1f;

			bd.position.Set(0.0f, 2.0);
			bd.angle = b2_pi;
			bd.allowSleep = false;
			m_body = world->CreateBody(&bd);
			m_body->CreateFixture(&sd1);
			m_body->CreateFixture(&sd2);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.3f;

			for (int i = 0; i < 10; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;

				bd.position.Set(0.0f, 5.0f + 1.54f * i);
				b2Body* body = world->CreateBody(&bd);

				body->CreateFixture(&fd);

				float32 gravity = 10.0f;
				float32 I = body->GetInertia();
				float32 mass = body->GetMass();

				// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
				float32 radius = b2Sqrt(2.0f * I / mass);

				b2FrictionJointDef jd;
				jd.localAnchorA.SetZero();
				jd.localAnchorB.SetZero();
				jd.bodyA = ground;
				jd.bodyB = body;
				jd.collideConnected = true;
				jd.maxForce = mass * gravity;
				jd.maxTorque = mass * radius * gravity;

				world->CreateJoint(&jd);
			}
		}

		setKeyEnabled(true);

		// speed button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(100), DP(44));
		pressed->setContentSize(DP(100), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(DP(55), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for speed button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Speed");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(55), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// left button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
		button->setPosition(DP(160), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for left button
		label = wyBitmapFontLabel::make(font, "Left");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(160), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// right button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(3, NULL));
		button->setPosition(DP(265), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for right button
		label = wyBitmapFontLabel::make(font, "Right");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(265), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyApplyForceTestLayer() {
	}

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_DPAD_UP:
			{
				b2Vec2 f = m_body->GetWorldVector(b2Vec2(0.0f, -200.0f));
				b2Vec2 p = m_body->GetWorldPoint(b2Vec2(0.0f, 2.0f));
				m_body->ApplyForce(f, p);
				break;
			}
			case KEYCODE_DPAD_LEFT:
				m_body->ApplyTorque(50.0f);
				break;
			case KEYCODE_DPAD_RIGHT:
				m_body->ApplyTorque(-50.0f);
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
			{
				b2Vec2 f = m_body->GetWorldVector(b2Vec2(0.0f, -200.0f));
				b2Vec2 p = m_body->GetWorldPoint(b2Vec2(0.0f, 2.0f));
				m_body->ApplyForce(f, p);
				break;
			}
			case 2:
				m_body->ApplyTorque(50.0f);
				break;
			case 3:
				m_body->ApplyTorque(-50.0f);
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
