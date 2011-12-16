class EdgeShapesCallback : public b2RayCastCallback
{
public:
	b2Fixture* m_fixture;
	b2Vec2 m_point;
	b2Vec2 m_normal;

public:
	EdgeShapesCallback() {
		m_fixture = NULL;
	}

	virtual ~EdgeShapesCallback() {
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;

		return fraction;
	}
};

class wyEdgeShapesTestLayer : public wyBox2DTestLayer {
private:
	enum
	{
		e_maxBodies = 256,
	};

	int32 m_bodyIndex;
	b2Body* m_bodies[e_maxBodies];
	b2PolygonShape m_polygons[4];
	b2CircleShape m_circle;

	float32 m_angle;

public:
	wyEdgeShapesTestLayer() {
		// get world
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 4);

		// Ground body
		{
			b2BodyDef bd;
			b2Body* ground = world->CreateBody(&bd);

			float32 x1 = -20.0f;
			float32 y1 = 2.0f * cosf(x1 / 10.0f * b2_pi);
			for (int32 i = 0; i < 80; ++i)
			{
				float32 x2 = x1 + 0.5f;
				float32 y2 = 2.0f * cosf(x2 / 10.0f * b2_pi);

				b2EdgeShape shape;
				shape.Set(b2Vec2(x1, y1), b2Vec2(x2, y2));
				ground->CreateFixture(&shape, 0.0f);

				x1 = x2;
				y1 = y2;
			}
		}

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

		m_angle = 0.0f;

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

	virtual ~wyEdgeShapesTestLayer() {
	}

	virtual void updateWorld(float delta) {
		wyBox2DTestLayer::updateWorld(delta);

		float32 L = 25.0f;
		b2Vec2 point1(0.0f, 10.0f);
		b2Vec2 d(L * cosf(m_angle), -L * b2Abs(sinf(m_angle)));
		b2Vec2 point2 = point1 + d;

		EdgeShapesCallback callback;

		m_box2d->getWorld()->RayCast(&callback, point1, point2);

		glPushMatrix();
		{
			m_box2d->transformAncestors();
			m_box2d->transform();

			b2Draw* dd = m_box2d->getDebugDrawImpl();
			if(callback.m_fixture) {
				dd->DrawPoint(callback.m_point, 5.0f, b2Color(0.4f, 0.9f, 0.4f));

				dd->DrawSegment(point1, callback.m_point, b2Color(0.8f, 0.8f, 0.8f));

				b2Vec2 head = callback.m_point + 0.5f * callback.m_normal;
				dd->DrawSegment(callback.m_point, head, b2Color(0.9f, 0.9f, 0.4f));
			} else {
				dd->DrawSegment(point1, point2, b2Color(0.8f, 0.8f, 0.8f));
			}
		}
		glPopMatrix();

		m_angle += 0.25f * b2_pi / 180.0f;
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
			fd.density = 20.0f;
			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}
		else
		{
			b2FixtureDef fd;
			fd.shape = &m_circle;
			fd.friction = 0.3f;
			fd.density = 20.0f;
			m_bodies[m_bodyIndex]->CreateFixture(&fd);
		}

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

	virtual bool keyDown(wyKeyEvent& event) {
		switch(event.keyCode) {
			case KEYCODE_MENU:
				Create(rand() % 5);
				break;
			case KEYCODE_SEARCH:
				DestroyBody();
				break;
			default:
				return false;
		}

		return true;
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
