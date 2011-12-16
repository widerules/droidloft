class wyVaryingRestitutionTestLayer : public wyBox2DTestLayer {
public:
	wyVaryingRestitutionTestLayer() {
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

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;

			float32 restitution[7] = {0.0f, 0.1f, 0.3f, 0.5f, 0.75f, 0.9f, 1.0f};

			for (int32 i = 0; i < 7; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10.0f + 3.0f * i, 20.0f);

				b2Body* body = world->CreateBody(&bd);

				fd.restitution = restitution[i];
				body->CreateFixture(&fd);
			}
		}

		startUpdateWorld();
	}

	virtual ~wyVaryingRestitutionTestLayer() {
	}
};
