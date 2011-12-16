class wyChainShapeTestLayer : public wyBox2DTestLayer {
public:
	wyChainShapeTestLayer() {
		b2World* world = m_box2d->getWorld();

		// place box2d in center
		m_box2d->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);

		// create body
		b2Body* body = NULL;
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 0.0f);
			bd.angularVelocity = 0.2f;
			body = world->CreateBody(&bd);
		}

		// create chain shape
		{
			b2Vec2 vertices[] = {
					b2Vec2(-2.0f, 0.0f),
					b2Vec2(0.0f, 2.0f),
					b2Vec2(2.0f, 2.0f),
					b2Vec2(2.0f, -2.0f),
					b2Vec2(-2.0f, -2.0f)
			};
			b2ChainShape* ls = new b2ChainShape();
			ls->CreateLoop(vertices, 5);
			b2FixtureDef fixDef;
			fixDef.shape = ls;
			body->CreateFixture(&fixDef);
		}

		// start update world
		startUpdateWorld();
	}

	virtual ~wyChainShapeTestLayer() {
	}
};
