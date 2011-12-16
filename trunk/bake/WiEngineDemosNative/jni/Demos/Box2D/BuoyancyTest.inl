class wyBuoyancyTestLayer : public wyBox2DTestLayer {
private:
    enum
	{
		e_maxBodies = 256,
	};

	int32 m_bodyIndex;
	b2Body* m_bodies[e_maxBodies];
    b2PolygonShape m_polygons[4];
	b2CircleShape m_circle;
    b2BuoyancyController* m_bc;

public:
	wyBuoyancyTestLayer() {
		// get world
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

        // create shapes
        {
			b2Vec2 vertices[3];
			vertices[0].Set(-0.5f, 0.0f);
			vertices[1].Set(0.5f, 0.0f);
			vertices[2].Set(0.0f, 1.5f);
			m_polygons[0].Set(vertices, 3);
		}
		{
			b2Vec2 vertices[3];
			vertices[0].Set(-0.1f, 0.0f);
			vertices[1].Set(0.1f, 0.0f);
			vertices[2].Set(0.0f, 1.5f);
			m_polygons[1].Set(vertices, 3);
		}
		{
			float32 w = 1.0f;
			float32 b = w / (2.0f + b2Sqrt(2.0f));
			float32 s = b2Sqrt(2.0f) * b;

			b2Vec2 vertices[8];
			vertices[0].Set(0.5f * s, 0.0f);
			vertices[1].Set(0.5f * w, b);
			vertices[2].Set(0.5f * w, b + s);
			vertices[3].Set(0.5f * s, w);
			vertices[4].Set(-0.5f * s, w);
			vertices[5].Set(-0.5f * w, b + s);
			vertices[6].Set(-0.5f * w, b);
			vertices[7].Set(-0.5f * s, 0.0f);

			m_polygons[2].Set(vertices, 8);
		}
		{
			m_polygons[3].SetAsBox(0.5f, 0.5f);
		}
		{
			m_circle.m_radius = 0.5f;
		}
		m_bodyIndex = 0;
		memset(m_bodies, 0, sizeof(m_bodies));

        // create controller
        b2BuoyancyControllerDef cd;
        cd.normal = b2Vec2(0, 1);
        cd.offset = m_box2d->pixel2Meter(wyDevice::winHeight / 2);
        cd.density = 2;
        cd.useWorldGravity = true;
        cd.useDensity = true;
        cd.linearDrag = 2;
        cd.angularDrag = 1;
        m_bc = (b2BuoyancyController*)world->CreateController(&cd);

        // add a layer for water surface
        wyColorLayer* waterLayer = wyColorLayer::make(wyc4b(0, 0, 255, 100));
        waterLayer->setContentSize(wyDevice::winWidth, wyDevice::winHeight / 2);
        addChildLocked(waterLayer, 1);

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

	virtual ~wyBuoyancyTestLayer() {
	}

    void Create(int32 index)
	{
		// get world
		b2World* world = m_box2d->getWorld();

		if (m_bodies[m_bodyIndex] != NULL)
		{
			world->DestroyBody(m_bodies[m_bodyIndex]);
			m_bodies[m_bodyIndex] = NULL;
		}

		b2BodyDef bd;

		float32 x = RandomFloat(-10.0f, 10.0f);
		float32 y = RandomFloat(10.0f, 20.0f);
		bd.position.Set(x, y);
		bd.angle = RandomFloat(-b2_pi, b2_pi);
		bd.type = b2_dynamicBody;

		if (index == 4)
		{
			bd.angularDamping = 0.02f;
		}

		m_bodies[m_bodyIndex] = world->CreateBody(&bd);

		if (index < 4)
		{
			b2FixtureDef fd;
			fd.shape = m_polygons + index;
			fd.friction = 0.3f;
			fd.density = 1;
			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}
		else
		{
			b2FixtureDef fd;
			fd.shape = &m_circle;
			fd.friction = 0.3f;
			fd.density = 1;
			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}

        // add to controller
        m_bc->AddBody(m_bodies[m_bodyIndex]);

		m_bodyIndex = (m_bodyIndex + 1) % e_maxBodies;
	}

	void DestroyBody()
	{
		for (int32 i = 0; i < e_maxBodies; ++i)
		{
			if (m_bodies[i] != NULL)
			{
				m_box2d->getWorld()->DestroyBody(m_bodies[i]);
				m_bodies[i] = NULL;
				return;
			}
		}
	}

    virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				Create(rand() % 5);
				break;
			case 2:
				DestroyBody();
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}
};
