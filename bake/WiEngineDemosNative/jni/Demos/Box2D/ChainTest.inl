class wyChainTestLayer : public wyBox2DTestLayer {
public:
	wyChainTestLayer() {
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
			b2PolygonShape shape;
			shape.SetAsBox(0.6f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;
			fd.friction = 0.2f;

			b2RevoluteJointDef jd;
			jd.collideConnected = false;

			const float32 y = 25.0f;
			b2Body* prevBody = ground;
			for (int32 i = 0; i < 30; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.5f + i, y);
				b2Body* body = world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(float32(i), y);
				jd.Initialize(prevBody, body, anchor);
				world->CreateJoint(&jd);

				prevBody = body;
			}
		}

		startUpdateWorld();
	}

	virtual ~wyChainTestLayer() {
	}
};
