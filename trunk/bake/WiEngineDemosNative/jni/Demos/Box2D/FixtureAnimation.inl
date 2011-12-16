class wyFixtureAnimationTestLayer : public wyBox2DTestLayer {
public:
	wyFixtureAnimationTestLayer() {
		b2World* world = m_box2d->getWorld();
		m_box2d->setDebugDraw(false);
		wyBox2DRender* render = new wyBox2DRender();
		m_box2d->setBox2DRender(render);
		render->release();

		// set gravity
		world->SetGravity(b2Vec2(0, -10));

		// create ground body
		b2BodyDef* bd = new b2BodyDef();
		bd->position.Set(0.0f, 0.0f);
		b2Body* ground = world->CreateBody(bd);
		delete bd;
		bd = NULL;

		// create bottom edge
		b2EdgeShape* es = new b2EdgeShape();
		b2FixtureDef* fixDef = new b2FixtureDef();
		fixDef->shape = es;
		fixDef->density = 0.0f;
		fixDef->restitution = 0.5f;
		es->Set(b2Vec2(0, 0), b2Vec2(m_box2d->pixel2Meter(wyDevice::winWidth), 0));
		ground->CreateFixture(fixDef);
		delete fixDef;
		fixDef = NULL;

		// create circle body
		bd = new b2BodyDef();
		bd->type = b2_dynamicBody;
		bd->position.Set(m_box2d->pixel2Meter(wyDevice::winWidth / 2),
				m_box2d->pixel2Meter(wyDevice::winHeight));
		b2Body* ball = world->CreateBody(bd);
		delete bd;
		bd = NULL;

		// create circle shape for ball
		b2PolygonShape* ps = new b2PolygonShape();
		ps->SetAsBox(m_box2d->pixel2Meter(DP(85) / 2), m_box2d->pixel2Meter(DP(121) / 2));
		fixDef = new b2FixtureDef();
		fixDef->shape = ps;
		fixDef->restitution = 0.5f;
		fixDef->density = 1.0f;
		b2Fixture* fixture = ball->CreateFixture(fixDef);
		delete fixDef;
		fixDef = NULL;

		// create fixture animation
		wyFixtureAnimation* anim = wyFixtureAnimation::make(1.f,
			RES("R.drawable.grossini_dance_01"),
			RES("R.drawable.grossini_dance_02"),
			RES("R.drawable.grossini_dance_03"),
			RES("R.drawable.grossini_dance_04"),
			RES("R.drawable.grossini_dance_05"),
			RES("R.drawable.grossini_dance_06"),
			RES("R.drawable.grossini_dance_07"),
			RES("R.drawable.grossini_dance_08"),
			RES("R.drawable.grossini_dance_09"),
			RES("R.drawable.grossini_dance_10"),
			RES("R.drawable.grossini_dance_11"),
			RES("R.drawable.grossini_dance_12"),
			RES("R.drawable.grossini_dance_13"),
			RES("R.drawable.grossini_dance_14"),
			NULL);
		anim->setLoop(true);
		anim->start(fixture);

		// start update world
		startUpdateWorld();
	}

	virtual ~wyFixtureAnimationTestLayer() {
	}
};
