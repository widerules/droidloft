class wySimpleTestLayer : public wyBox2DTestLayer {
public:
	wySimpleTestLayer() {
		b2World* world = m_box2d->getWorld();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// create ground body
		b2BodyDef bd;
		bd.position.Set(0.0f, 0.0f);
		b2Body* ground = world->CreateBody(&bd);

		// create bottom edge
		b2EdgeShape* es = new b2EdgeShape();
		b2FixtureDef fixDef;
		fixDef.shape = es;
		fixDef.density = 0.0f;
		fixDef.restitution = 0.5f;
		es->Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(wyDevice::winWidth), 0));
		ground->CreateFixture(&fixDef);

		// create circle body
		bd.type = b2_dynamicBody;
		bd.position.Set(m_box2d->pixel2Meter(wyDevice::winWidth / 2),
				m_box2d->pixel2Meter(wyDevice::winHeight));
		b2Body* ball = world->CreateBody(&bd);

		// create circle shape for ball
		b2CircleShape* cs = new b2CircleShape();
		fixDef.shape = cs;
		fixDef.restitution = 0.5f;
		fixDef.density = 1.0f;
		cs->m_radius = 0.5f;
		ball->CreateFixture(&fixDef);

		// start update world
		startUpdateWorld();
	}

	virtual ~wySimpleTestLayer() {
	}
};
