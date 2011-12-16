// control variables
static const float HORSEPOWER = 40;
static const float SIDEWAY_FRICTION_FORCE = 10;
static const float STEER_SPEED = 1.5f;
static const float MAX_STEER_ANGLE = b2_pi / 3;

class wyTensorDampingTestLayer : public wyBox2DTestLayer {
private:
	b2Body* m_car;
	b2Body* m_leftRearWheel;
	b2Body* m_leftFrontWheel;
	b2Body* m_rightRearWheel;
	b2Body* m_rightFrontWheel;
	b2RevoluteJoint* m_leftJoint;
	b2RevoluteJoint* m_rightJoint;

	float m_engineSpeed;
	float m_steeringAngle;

public:
	wyTensorDampingTestLayer() {
		// get world
		b2World* world = m_box2d->getWorld();

		// don't draw solid
		world->SetNoSolidDraw(true);

		// place box2d to center of bottom edge
		m_box2d->setPosition(0, 0);

		// init
		m_engineSpeed = 0;
		m_steeringAngle = 0;

		// position
		b2Vec2 carPos = b2Vec2(10, 10);
		b2Vec2 leftRearPos = b2Vec2(-1.5f, -1.9f);
		b2Vec2 rightRearPos = b2Vec2(1.5f, -1.9f);
		b2Vec2 leftFrontPos = b2Vec2(-1.5f, 1.9f);
		b2Vec2 rightFrontPos = b2Vec2(1.5f, 1.9f);

		// create controller for wheel
		b2TensorDampingControllerDef cd;
		cd.SetAxisAligned(10, 5);
		b2TensorDampingController* tdc = (b2TensorDampingController*)world->CreateController(&cd);

		// car
		{
			b2BodyDef bd;
			bd.linearDamping = 1;
			bd.angularDamping = 1;
			bd.type = b2_dynamicBody;
			bd.position = carPos;
			m_car = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(1.5f, 2.5f);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1;
			m_car->CreateFixture(&fd);
		}

		// left-rear wheel
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position = carPos;
			bd.position += leftRearPos;
			m_leftRearWheel = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.2f, 0.5f);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1;
			m_leftRearWheel->CreateFixture(&fd);

			b2PrismaticJointDef jd;
			jd.Initialize(m_car, m_leftRearWheel, m_leftRearWheel->GetWorldCenter(), b2Vec2(1, 0));
			jd.enableLimit = true;
			jd.lowerTranslation = jd.upperTranslation = 0;
			world->CreateJoint(&jd);

			tdc->AddBody(m_leftRearWheel);
		}

		// right-rear wheel
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position = carPos;
			bd.position += rightRearPos;
			m_rightRearWheel = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.2f, 0.5f);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1;
			m_rightRearWheel->CreateFixture(&fd);

			b2PrismaticJointDef jd;
			jd.Initialize(m_car, m_rightRearWheel, m_rightRearWheel->GetWorldCenter(), b2Vec2(1, 0));
			jd.enableLimit = true;
			jd.lowerTranslation = jd.upperTranslation = 0;
			world->CreateJoint(&jd);

			tdc->AddBody(m_rightRearWheel);
		}

		// left-front wheel
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position = carPos;
			bd.position += leftFrontPos;
			m_leftFrontWheel = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.2f, 0.5f);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1;
			m_leftFrontWheel->CreateFixture(&fd);

			b2RevoluteJointDef jd;
			jd.Initialize(m_car, m_leftFrontWheel, m_leftFrontWheel->GetWorldCenter());
			jd.enableMotor = true;
			jd.maxMotorTorque = 100;
			m_leftJoint = (b2RevoluteJoint*)world->CreateJoint(&jd);

			tdc->AddBody(m_leftFrontWheel);
		}

		// right-front wheel
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position = carPos;
			bd.position += rightFrontPos;
			m_rightFrontWheel = world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(0.2f, 0.5f);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1;
			m_rightFrontWheel->CreateFixture(&fd);

			b2RevoluteJointDef jd;
			jd.Initialize(m_car, m_rightFrontWheel, m_rightFrontWheel->GetWorldCenter());
			jd.enableMotor = true;
			jd.maxMotorTorque = 100;
			m_rightJoint = (b2RevoluteJoint*)world->CreateJoint(&jd);

			tdc->AddBody(m_rightFrontWheel);
		}

		// left button
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal->setContentSize(DP(80), DP(44));
		pressed->setContentSize(DP(80), DP(44));
		wyButton* button = new wyButton(normal, pressed, NULL, NULL, TARGETSELECTOR(1, NULL), TARGETSELECTOR(5, NULL));
		button->setPosition(DP(40), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for left button
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Left");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(40), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// up button
		button = new wyButton(normal, pressed, NULL, NULL, TARGETSELECTOR(2, NULL), TARGETSELECTOR(6, NULL));
		button->setPosition(DP(120), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for up button
		label = wyBitmapFontLabel::make(font, "Up");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(120), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// down button
		button = new wyButton(normal, pressed, NULL, NULL, TARGETSELECTOR(3, NULL), TARGETSELECTOR(7, NULL));
		button->setPosition(DP(200), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for down button
		label = wyBitmapFontLabel::make(font, "Down");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(200), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		// right button
		button = new wyButton(normal, pressed, NULL, NULL, TARGETSELECTOR(4, NULL), TARGETSELECTOR(8, NULL));
		button->setPosition(DP(280), wyDevice::winHeight - DP(30));
		addChildLocked(button, 1);
		button->release();

		// label for right button
		label = wyBitmapFontLabel::make(font, "Right");
		label->setColor(wyc3b(255, 255, 0));
		label->setPosition(DP(280), wyDevice::winHeight - DP(30));
		addChildLocked(label, 1);

		startUpdateWorld();
	}

	virtual ~wyTensorDampingTestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
				m_steeringAngle = MAX_STEER_ANGLE;
				break;
			case 2:
				m_car->SetAwake(false);
				m_engineSpeed = HORSEPOWER;
				break;
			case 3:
				m_engineSpeed = -HORSEPOWER;
				break;
			case 4:
				m_steeringAngle = -MAX_STEER_ANGLE;
				break;
			case 5:
			case 8:
				m_steeringAngle = 0;
				break;
			case 6:
			case 7:
				m_engineSpeed = 0;
				break;
			default:
				wyBox2DTestLayer::onTargetSelectorInvoked(ts);
				break;
		}
	}

	virtual void updateWorld(float delta) {
		wyBox2DTestLayer::updateWorld(delta);

		// driving
		const b2Transform& tl = m_leftFrontWheel->GetTransform();
		b2Vec2 ldirection = b2Vec2(-tl.q.s, tl.q.c);
		ldirection *= m_engineSpeed;
		const b2Transform& tr = m_rightFrontWheel->GetTransform();
		b2Vec2 rdirection = b2Vec2(-tr.q.s, tr.q.c);
		rdirection *= m_engineSpeed;
		m_leftFrontWheel->ApplyForce(ldirection, m_leftFrontWheel->GetPosition());
		m_rightFrontWheel->ApplyForce(rdirection, m_rightFrontWheel->GetPosition());

		// steering
		float mspeed = m_steeringAngle - m_leftJoint->GetJointAngle();
		m_leftJoint->SetMotorSpeed(mspeed * STEER_SPEED);
		mspeed = m_steeringAngle - m_rightJoint->GetJointAngle();
		m_rightJoint->SetMotorSpeed(mspeed * STEER_SPEED);
	}
};
