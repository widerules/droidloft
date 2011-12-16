class wyPulleysTestLayer : public wyBox2DTestLayer {
private:
	b2PulleyJoint* m_joint1;

public:
	wyPulleysTestLayer() {
		// get world
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
			float32 a = 2.0f;
			float32 b = 4.0f;
			float32 y = 10.0f;
			float32 L = 8.0f;

			b2PolygonShape shape;
			shape.SetAsBox(a, b);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			bd.position.Set(-4.0f, y);
			b2Body* body1 = world->CreateBody(&bd);
			body1->CreateFixture(&shape, 5.0f);

			bd.position.Set(4.0f, y);
			b2Body* body2 = world->CreateBody(&bd);
			body2->CreateFixture(&shape, 5.0f);

			b2PulleyJointDef pulleyDef;
			b2Vec2 anchor1(-4.0f, y + b);
			b2Vec2 anchor2(4.0f, y + b);
			b2Vec2 groundAnchor1(-4.0f, y + b + L);
			b2Vec2 groundAnchor2(4.0f, y + b + L);
			pulleyDef.Initialize(body1, body2, groundAnchor1, groundAnchor2, anchor1, anchor2, 2.0f);

			m_joint1 = (b2PulleyJoint*)world->CreateJoint(&pulleyDef);
		}

		startUpdateWorld();
	}

	virtual ~wyPulleysTestLayer() {
	}
};
