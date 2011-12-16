class wyBulletTestLayer : public wyBox2DTestLayer {
private:
	b2Body* m_body;
	b2Body* m_bullet;
	float32 m_x;

public:
	wyBulletTestLayer() {
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, 0);

		{
			b2BodyDef bd;
			bd.position.Set(0.0f, 0.0f);
			b2Body* body = world->CreateBody(&bd);

			b2EdgeShape edge;

			edge.Set(b2Vec2(-10.0f, 0.0f), b2Vec2(10.0f, 0.0f));
			body->CreateFixture(&edge, 0.0f);

			b2PolygonShape shape;
			shape.SetAsBox(0.2f, 1.0f, b2Vec2(0.5f, 1.0f), 0.0f);
			body->CreateFixture(&shape, 0.0f);
		}

		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 4.0f);

			b2PolygonShape box;
			box.SetAsBox(2.0f, 0.1f);

			m_body = world->CreateBody(&bd);
			m_body->CreateFixture(&box, 1.0f);

			box.SetAsBox(0.25f, 0.25f);

			m_x = 0.20352793f;
			bd.position.Set(m_x, 10.0f);
			bd.bullet = true;

			m_bullet = world->CreateBody(&bd);
			m_bullet->CreateFixture(&box, 100.0f);

			m_bullet->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
		}

		startUpdateWorld();
	}

	virtual ~wyBulletTestLayer() {
	}

	void Launch() {
		m_body->SetTransform(b2Vec2(0.0f, 4.0f), 0.0f);
		m_body->SetLinearVelocity(b2Vec2_zero);
		m_body->SetAngularVelocity(0.0f);

		m_x = frand_unit();
		m_bullet->SetTransform(b2Vec2(m_x, 10.0f), 0.0f);
		m_bullet->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
		m_bullet->SetAngularVelocity(0.0f);
	}

	virtual void updateWorld(float delta) {
		wyBox2DTestLayer::updateWorld(delta);

		if(m_stepCount % 60 == 0) {
			Launch();
		}
	}
};
