class wySphereStackTestLayer : public wyBox2DTestLayer {
private:
	enum
	{
		e_count = 10
	};

	b2Body* m_bodies[e_count];

public:
	wySphereStackTestLayer() {
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
			b2CircleShape shape;
			shape.m_radius = 1.0f;

			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0, 4.0f + 3.0f * i);

				m_bodies[i] = world->CreateBody(&bd);

				m_bodies[i]->CreateFixture(&shape, 1.0f);
			}
		}

		startUpdateWorld();
	}

	virtual ~wySphereStackTestLayer() {
	}
};
