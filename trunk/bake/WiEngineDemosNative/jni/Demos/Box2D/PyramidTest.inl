class wyPyramidTestLayer : public wyBox2DTestLayer {
public:
	enum
	{
		e_count = 15,
	};

	wyPyramidTestLayer() {
		// get world
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

		{
			float32 a = 0.5f;
			b2PolygonShape shape;
			shape.SetAsBox(a, a);

			b2Vec2 x(-7.0f, 0.75f);
			b2Vec2 y;
			b2Vec2 deltaX(0.5625f, 1.25f);
			b2Vec2 deltaY(1.125f, 0.0f);

			for (int32 i = 0; i < e_count; ++i)
			{
				y = x;

				for (int32 j = i; j < e_count; ++j)
				{
					b2BodyDef bd;
					bd.type = b2_dynamicBody;
					bd.position = y;
					b2Body* body = world->CreateBody(&bd);
					body->CreateFixture(&shape, 5.0f);

					y += deltaY;
				}

				x += deltaX;
			}
		}

		startUpdateWorld();
	}

	virtual ~wyPyramidTestLayer() {
	}
};
