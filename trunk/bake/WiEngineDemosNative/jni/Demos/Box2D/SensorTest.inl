class wySensorTestLayer : public wyBox2DTestLayer, public b2ContactListener {
private:
	enum
	{
		e_count = 7
	};

	b2Fixture* m_sensor;
	b2Body* m_bodies[e_count];
	bool m_touching[e_count];

public:
	wySensorTestLayer() {
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// set contact listener
		world->SetContactListener(this);

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		{
			b2BodyDef bd;
			b2Body* ground = world->CreateBody(&bd);

			{
				b2EdgeShape shape;
				shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
				ground->CreateFixture(&shape, 0.0f);
			}

			{
				b2CircleShape shape;
				shape.m_radius = 5.0f;
				shape.m_p.Set(0.0f, 10.0f);

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.isSensor = true;
				m_sensor = ground->CreateFixture(&fd);
			}
		}

		{
			b2CircleShape shape;
			shape.m_radius = 1.0f;

			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10.0f + 3.0f * i, 20.0f);
				bd.userData = m_touching + i;

				m_touching[i] = false;
				m_bodies[i] = world->CreateBody(&bd);

				m_bodies[i]->CreateFixture(&shape, 1.0f);
			}
		}

		startUpdateWorld();
	}

	virtual ~wySensorTestLayer() {
	}

	// Implement contact listener.
	virtual void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA == m_sensor)
		{
			void* userData = fixtureB->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}

		if (fixtureB == m_sensor)
		{
			void* userData = fixtureA->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}
	}

	// Implement contact listener.
	virtual void EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA == m_sensor)
		{
			void* userData = fixtureB->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = false;
			}
		}

		if (fixtureB == m_sensor)
		{
			void* userData = fixtureA->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = false;
			}
		}
	}

	virtual void updateWorld(float delta)
	{
		wyBox2DTestLayer::updateWorld(delta);

		// Traverse the contact results. Apply a force on shapes
		// that overlap the sensor.
		for (int32 i = 0; i < e_count; ++i)
		{
			if (m_touching[i] == false)
			{
				continue;
			}

			b2Body* body = m_bodies[i];
			b2Body* ground = m_sensor->GetBody();

			b2CircleShape* circle = (b2CircleShape*)m_sensor->GetShape();
			b2Vec2 center = ground->GetWorldPoint(circle->m_p);

			b2Vec2 position = body->GetPosition();

			b2Vec2 d = center - position;
			if (d.LengthSquared() < FLT_EPSILON * FLT_EPSILON)
			{
				continue;
			}

			d.Normalize();
			b2Vec2 F = 100.0f * d;
			body->ApplyForce(F, position);
		}
	}
};
