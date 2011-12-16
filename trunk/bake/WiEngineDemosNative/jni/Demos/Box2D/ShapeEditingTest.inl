class wyShapeEditingTestLayer : public wyBox2DTestLayer {
private:
	b2Body* m_body;
	b2Fixture* m_fixture1;
	b2Fixture* m_fixture2;

public:
	wyShapeEditingTestLayer() {
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		{
			b2BodyDef bd;
			b2Body* ground = world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(0.0f, 10.0f);
		m_body = world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(4.0f, 4.0f, b2Vec2(0.0f, 0.0f), 0.0f);
		m_fixture1 = m_body->CreateFixture(&shape, 10.0f);

		m_fixture2 = NULL;

		setKeyEnabled(true);

		// create button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(150), DP(44));
		pressed->setContentSize(DP(150), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(DP(80), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for create button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Create Shape");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(80), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// destroy button
		button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(2, NULL));
		button->setPosition(DP(240), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for destroy button
		label = wyBitmapFontLabel::make(font, "Destroy Shape");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(240), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyShapeEditingTestLayer() {
	}

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_MENU:
				if (m_fixture2 == NULL)
				{
					b2CircleShape shape;
					shape.m_radius = 3.0f;
					shape.m_p.Set(0.5f, -4.0f);
					m_fixture2 = m_body->CreateFixture(&shape, 10.0f);
					m_body->SetAwake(true);
				}
				break;
			case KEYCODE_SEARCH:
				if (m_fixture2 != NULL)
				{
					m_body->DestroyFixture(m_fixture2);
					m_fixture2 = NULL;
					m_body->SetAwake(true);
				}
				break;
			default:
				return false;
		}

		return true;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				if (m_fixture2 == NULL)
				{
					b2CircleShape shape;
					shape.m_radius = 3.0f;
					shape.m_p.Set(0.5f, -4.0f);
					m_fixture2 = m_body->CreateFixture(&shape, 10.0f);
					m_body->SetAwake(true);
				}
				break;
			case 2:
				if (m_fixture2 != NULL)
				{
					m_body->DestroyFixture(m_fixture2);
					m_fixture2 = NULL;
					m_body->SetAwake(true);
				}
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
