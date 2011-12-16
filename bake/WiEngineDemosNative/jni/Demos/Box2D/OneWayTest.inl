class wyOneWayTestLayer : public wyBox2DTestLayer, public b2ContactListener {
private:
	enum State
	{
		e_unknown,
		e_above,
		e_below,
	};

	float32 m_radius, m_top, m_bottom;
	State m_state;
	b2Fixture* m_platform;
	b2Fixture* m_character;

public:
	wyOneWayTestLayer() {
		// get world
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// set contact listener
		world->SetContactListener(this);

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		// Ground
		{
			b2BodyDef bd;
			b2Body* ground = world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		// Platform
		{
			b2BodyDef bd;
			bd.position.Set(0.0f, 10.0f);
			b2Body* body = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(3.0f, 0.5f);
			m_platform = body->CreateFixture(&shape, 0.0f);

			m_bottom = 10.0f - 0.5f;
			m_top = 10.0f + 0.5f;
		}

		// Actor
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 12.0f);
			b2Body* body = world->CreateBody(&bd);

			m_radius = 0.5f;
			b2CircleShape shape;
			shape.m_radius = m_radius;
			m_character = body->CreateFixture(&shape, 20.0f);

			body->SetLinearVelocity(b2Vec2(0.0f, -50.0f));

			m_state = e_unknown;
		}

		// add a hint label
		wyLabel* label = wyLabel::make("try drag ball through platform",
				SP(14));
		label->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(60));
		addChildLocked(label);

		startUpdateWorld();
	}

	virtual ~wyOneWayTestLayer() {
	}

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if(fixtureA != m_platform && fixtureA != m_character) {
			return;
		}

		if(fixtureB != m_platform && fixtureB != m_character) {
			return;
		}

		b2Vec2 position = m_character->GetBody()->GetPosition();

		if(position.y < m_top + m_radius - 3.0f * b2_linearSlop) {
			contact->SetEnabled(false);
		}
	}
};
