class wyConfinedTestLayer : public wyBox2DTestLayer {
public:
	wyConfinedTestLayer() {
		b2World* world = m_box2d->getWorld();

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		{
			b2BodyDef bd;
			b2Body* ground = world->CreateBody(&bd);

			b2EdgeShape shape;

			// Floor
			shape.Set(b2Vec2(-10.0f, 0.0f), b2Vec2(10.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);

			// Left wall
			shape.Set(b2Vec2(-10.0f, 0.0f), b2Vec2(-10.0f, 20.0f));
			ground->CreateFixture(&shape, 0.0f);

			// Right wall
			shape.Set(b2Vec2(10.0f, 0.0f), b2Vec2(10.0f, 20.0f));
			ground->CreateFixture(&shape, 0.0f);

			// Roof
			shape.Set(b2Vec2(-10.0f, 20.0f), b2Vec2(10.0f, 20.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		setKeyEnabled(true);

		// create circle button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(200), DP(44));
		pressed->setContentSize(DP(200), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(1, NULL));
		button->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for create circle button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Create Circle");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyConfinedTestLayer() {
	}

	void CreateCircle() {
		float32 radius = 2.0f;
		b2CircleShape shape;
		shape.m_p.SetZero();
		shape.m_radius = radius;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1.0f;
		fd.friction = 0.0f;

		b2Vec2 p(RandomFloat(), 3.0f + RandomFloat());
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = p;
		b2Body* body = m_box2d->getWorld()->CreateBody(&bd);

		body->CreateFixture(&fd);
	}

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_MENU:
				CreateCircle();
				break;
			default:
				return false;
		}

		return true;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				CreateCircle();
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
