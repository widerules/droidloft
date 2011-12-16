class wySliderCrankTestLayer : public wyBox2DTestLayer {
private:
	b2RevoluteJoint* m_joint1;
	b2PrismaticJoint* m_joint2;

public:
	wySliderCrankTestLayer() {
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
			b2Body* prevBody = ground;

			// Define crank.
			{
				b2PolygonShape shape;
				shape.SetAsBox(0.5f, 2.0f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 7.0f);
				b2Body* body = world->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 5.0f));
				rjd.motorSpeed = 1.0f * b2_pi;
				rjd.maxMotorTorque = 10000.0f;
				rjd.enableMotor = true;
				m_joint1 = (b2RevoluteJoint*)world->CreateJoint(&rjd);

				prevBody = body;
			}

			// Define follower.
			{
				b2PolygonShape shape;
				shape.SetAsBox(0.5f, 4.0f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 13.0f);
				b2Body* body = world->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 9.0f));
				rjd.enableMotor = false;
				world->CreateJoint(&rjd);

				prevBody = body;
			}

			// Define piston
			{
				b2PolygonShape shape;
				shape.SetAsBox(1.5f, 1.5f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.fixedRotation = true;
				bd.position.Set(0.0f, 17.0f);
				b2Body* body = world->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 17.0f));
				world->CreateJoint(&rjd);

				b2PrismaticJointDef pjd;
				pjd.Initialize(ground, body, b2Vec2(0.0f, 17.0f), b2Vec2(0.0f, 1.0f));

				pjd.maxMotorForce = 1000.0f;
				pjd.enableMotor = true;

				m_joint2 = (b2PrismaticJoint*)world->CreateJoint(&pjd);
			}

			// Create a payload
			{
				b2PolygonShape shape;
				shape.SetAsBox(1.5f, 1.5f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 23.0f);
				b2Body* body = world->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);
			}
		}

		setKeyEnabled(true);

		// toggle friction button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(150), DP(44));
		pressed->setContentSize(DP(150), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL,  NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(DP(80), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for toggle friction button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Toggle friction");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(80), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// toggle motor button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
		button->setPosition(DP(240), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for toggle motor button
		label = wyBitmapFontLabel::make(font, "Toggle motor");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(240), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wySliderCrankTestLayer() {
	}

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_MENU:
				m_joint2->EnableMotor(!m_joint2->IsMotorEnabled());
				m_joint2->GetBodyB()->SetAwake(true);
				break;
			case KEYCODE_SEARCH:
				m_joint1->EnableMotor(!m_joint1->IsMotorEnabled());
				m_joint1->GetBodyB()->SetAwake(true);
				break;
			default:
				return false;
		}

		return true;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				m_joint2->EnableMotor(!m_joint2->IsMotorEnabled());
				m_joint2->GetBodyB()->SetAwake(true);
				break;
			case 2:
				m_joint1->EnableMotor(!m_joint1->IsMotorEnabled());
				m_joint1->GetBodyB()->SetAwake(true);
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
