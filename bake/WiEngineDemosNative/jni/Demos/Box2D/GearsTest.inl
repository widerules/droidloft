class wyGearsTestLayer : public wyBox2DTestLayer {
private:
	b2RevoluteJoint* m_joint1;
	b2RevoluteJoint* m_joint2;
	b2PrismaticJoint* m_joint3;
	b2GearJoint* m_joint4;
	b2GearJoint* m_joint5;

public:
	wyGearsTestLayer() {
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
			shape.Set(b2Vec2(50.0f, 0.0f), b2Vec2(-50.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2CircleShape circle1;
			circle1.m_radius = 1.0f;

			b2CircleShape circle2;
			circle2.m_radius = 2.0f;

			b2PolygonShape box;
			box.SetAsBox(0.5f, 5.0f);

			b2BodyDef bd1;
			bd1.type = b2_dynamicBody;
			bd1.position.Set(-3.0f, 12.0f);
			b2Body* body1 = world->CreateBody(&bd1);
			body1->CreateFixture(&circle1, 5.0f);

			b2RevoluteJointDef jd1;
			jd1.bodyA = ground;
			jd1.bodyB = body1;
			jd1.localAnchorA = ground->GetLocalPoint(bd1.position);
			jd1.localAnchorB = body1->GetLocalPoint(bd1.position);
			jd1.referenceAngle = body1->GetAngle() - ground->GetAngle();
			m_joint1 = (b2RevoluteJoint*)world->CreateJoint(&jd1);

			b2BodyDef bd2;
			bd2.type = b2_dynamicBody;
			bd2.position.Set(0.0f, 12.0f);
			b2Body* body2 = world->CreateBody(&bd2);
			body2->CreateFixture(&circle2, 5.0f);

			b2RevoluteJointDef jd2;
			jd2.Initialize(ground, body2, bd2.position);
			m_joint2 = (b2RevoluteJoint*)world->CreateJoint(&jd2);

			b2BodyDef bd3;
			bd3.type = b2_dynamicBody;
			bd3.position.Set(2.5f, 12.0f);
			b2Body* body3 = world->CreateBody(&bd3);
			body3->CreateFixture(&box, 5.0f);

			b2PrismaticJointDef jd3;
			jd3.Initialize(ground, body3, bd3.position, b2Vec2(0.0f, 1.0f));
			jd3.lowerTranslation = -5.0f;
			jd3.upperTranslation = 5.0f;
			jd3.enableLimit = true;

			m_joint3 = (b2PrismaticJoint*)world->CreateJoint(&jd3);

			b2GearJointDef jd4;
			jd4.bodyA = body1;
			jd4.bodyB = body2;
			jd4.joint1 = m_joint1;
			jd4.joint2 = m_joint2;
			jd4.ratio = circle2.m_radius / circle1.m_radius;
			m_joint4 = (b2GearJoint*)world->CreateJoint(&jd4);

			b2GearJointDef jd5;
			jd5.bodyA = body2;
			jd5.bodyB = body3;
			jd5.joint1 = m_joint2;
			jd5.joint2 = m_joint3;
			jd5.ratio = -1.0f / circle2.m_radius;
			m_joint5 = (b2GearJoint*)world->CreateJoint(&jd5);
		}

		startUpdateWorld();
	}

	virtual ~wyGearsTestLayer() {
	}
};
