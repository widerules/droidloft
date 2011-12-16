class wyBounceTestLayer: public wyBox2DTestLayer {
public:
	wyBounceTestLayer() {
		b2World* world = m_box2d->getWorld();

		// place box2d to center of bottom edge
		m_box2d->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);

		// meter size of screen
		float meterWidth = m_box2d->pixel2Meter(wyDevice::winWidth);
		float meterHeight = m_box2d->pixel2Meter(wyDevice::winHeight);

		// border
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = world->CreateBody(&bd);

			// bottom edge
			b2EdgeShape bottom;
			bottom.Set(b2Vec2(-meterWidth / 2, -meterHeight / 2), b2Vec2(meterWidth / 2, -meterHeight / 2));
			b2FixtureDef fd;
			fd.shape = &bottom;
			ground->CreateFixture(&fd);

			// top edge
			b2EdgeShape top;
			top.Set(b2Vec2(-meterWidth / 2, meterHeight / 2), b2Vec2(meterWidth / 2, meterHeight / 2));
			fd.shape = &top;
			ground->CreateFixture(&fd);

			// left edge
			b2EdgeShape left;
			left.Set(b2Vec2(-meterWidth / 2, -meterHeight / 2), b2Vec2(-meterWidth / 2, meterHeight / 2));
			fd.shape = &left;
			ground->CreateFixture(&fd);

			// right edge
			b2EdgeShape right;
			right.Set(b2Vec2(meterWidth / 2, -meterHeight / 2), b2Vec2(meterWidth / 2, meterHeight / 2));
			fd.shape = &right;
			ground->CreateFixture(&fd);
		}

		// boxes
		for (int i = 0; i < 10; i++) {
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(frand_unit() * meterWidth / 2, frand_unit() * meterHeight / 2);
			bd.linearVelocity = b2Vec2(5 * frand_unit(), 5 * frand_unit());
			b2Body* body = world->CreateBody(&bd);
			body->SetTransform(bd.position, frand() * 2 * M_PI);

			b2PolygonShape box;
			box.SetAsBox(0.5, 0.5);
			b2FixtureDef fd;
			fd.shape = &box;
			fd.restitution = 1.0f;
			fd.friction = 0.0f;
			fd.density = 1.0f;
			body->CreateFixture(&fd);
		}

		// center stick
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0, 0);
			b2Body* stickBody = world->CreateBody(&bd);

			b2PolygonShape stick;
			stick.SetAsBox(meterWidth / 4, 0.4f);
			b2FixtureDef fd;
			fd.shape = &stick;
			fd.restitution = 1.0f;
			fd.friction = 0.0f;
			fd.density = 1.0f;
			stickBody->CreateFixture(&fd);

			b2RevoluteJointDef jd;
			jd.bodyA = ground;
			jd.bodyB = stickBody;
			jd.localAnchorA = b2Vec2(0, 0);
			jd.localAnchorB = b2Vec2(0, 0);
			world->CreateJoint(&jd);
		}

		startUpdateWorld();
	}

	virtual ~wyBounceTestLayer() {
	}
};
