class wyTexturedBounceTestLayer: public wyBox2DTestLayer {
public:
	wyTexturedBounceTestLayer() {
		b2World* world = m_box2d->getWorld();
		m_box2d->setDebugDraw(false);
		wyBox2DRender* render = new wyBox2DRender();
		m_box2d->setBox2DRender(render);
		render->release();

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
		wyTexture2D* tex = wyTexture2D::makePNG(RES("R.drawable.blocks"));
		float size = DP(32.0f) / 2;
		float meterSize = m_box2d->pixel2Meter(size);
		for (int i = 0; i < 10; i++) {
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(frand_unit() * meterWidth / 2, frand_unit() * meterHeight / 2);
			bd.linearVelocity = b2Vec2(5 * frand_unit(), 5 * frand_unit());
			b2Body* body = world->CreateBody(&bd);
			body->SetTransform(bd.position, frand() * 2 * M_PI);

			b2PolygonShape box;
			box.SetAsBox(meterSize, meterSize);
			b2FixtureDef fd;
			fd.shape = &box;
			fd.restitution = 1.0f;
			fd.friction = 0.0f;
			fd.density = 1.0f;
			b2Fixture* f = body->CreateFixture(&fd);

			// bind texture
			render->bindTexture(f, tex, wyr(wyMath::randMax(2) * size * 2, wyMath::randMax(2) * size * 2, size * 2, size * 2));
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
			b2Fixture* f = stickBody->CreateFixture(&fd);

			b2RevoluteJointDef jd;
			jd.bodyA = ground;
			jd.bodyB = stickBody;
			jd.localAnchorA = b2Vec2(0, 0);
			jd.localAnchorB = b2Vec2(0, 0);
			world->CreateJoint(&jd);

			// bind texture
			tex = wyTexture2D::makePNG(RES("R.drawable.bar"));
			render->bindTexture(f, tex);
		}

		startUpdateWorld();
	}

	virtual ~wyTexturedBounceTestLayer() {
	}
};
