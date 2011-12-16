#include "common.h"
#include "Box2DDemos.h"
#include "WiEngine.h"
#include "WiEngine-Box2D.h"
#include <math.h>

namespace Box2D {

#define TS_UPDATE 100

#define TARGETSELECTOR(id, userData) (wyTargetSelector*)(new wyTargetSelector(this, id, userData))->autoRelease()

class QueryCallback: public b2QueryCallback {
public:
	b2Vec2 m_point;
	b2Fixture* m_fixture;

public:
	QueryCallback(const b2Vec2& point) {
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) {
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody) {
			bool inside = fixture->TestPoint(m_point);
			if (inside) {
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}
};

class wyBox2DTestLayer: public wyLayer {
protected:
	wyBox2D* m_box2d;
	b2Vec2 m_mouseWorld;
	b2MouseJoint* m_mouseJoint;
	b2Body* m_groundBody;
	int m_stepCount;

public:
	wyBox2DTestLayer() :
			wyLayer(),
			m_mouseJoint(NULL),
			m_stepCount(0) {
		m_box2d = new wyBox2D();
		m_box2d->setDebugDraw(true);
		addChildLocked(m_box2d);

		b2BodyDef bodyDef;
		m_groundBody = m_box2d->getWorld()->CreateBody(&bodyDef);

		setTouchEnabled(true);
	}

	virtual ~wyBox2DTestLayer() {
		m_box2d->release();
		m_box2d = NULL;
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch (ts->getId()) {
			case TS_UPDATE:
				updateWorld(ts->getDelta());
				break;
		}
	}

	virtual void updateWorld(float delta) {
		m_box2d->getWorld()->Step(1.f / 60.f, 10, 10);
		m_box2d->getWorld()->ClearForces();
		m_stepCount++;
	}

	void startUpdateWorld() {
		wyTargetSelector* ts = new wyTargetSelector(this, TS_UPDATE, NULL);
		wyTimer* t = new wyTimer(ts);
		scheduleLocked(t);
		t->release();
		ts->release();
	}

	virtual bool touchesBegan(wyMotionEvent& e) {
		wyPoint loc = m_box2d->worldToNodeSpace(wyp(e.x[0], e.y[0]));
		b2Vec2 p = b2Vec2(m_box2d->pixel2Meter(loc.x), m_box2d->pixel2Meter(loc.y));
		m_mouseWorld = p;

		if (m_mouseJoint != NULL) {
			return false;
		}

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;

		// Query the world for overlapping shapes.
		QueryCallback callback(p);
		m_box2d->getWorld()->QueryAABB(&callback, aabb);

		if (callback.m_fixture) {
			b2Body* body = callback.m_fixture->GetBody();
			b2MouseJointDef md;
			md.bodyA = m_groundBody;
			md.bodyB = body;
			md.target = p;
			md.maxForce = 1000.0f * body->GetMass();
			m_mouseJoint = (b2MouseJoint*)m_box2d->getWorld()->CreateJoint(&md);
			body->SetAwake(true);

			return true;
		}

		return false;
	}

	virtual bool touchesMoved(wyMotionEvent& e) {
		wyPoint loc = m_box2d->worldToNodeSpace(wyp(e.x[0], e.y[0]));
		b2Vec2 p = b2Vec2(m_box2d->pixel2Meter(loc.x), m_box2d->pixel2Meter(loc.y));
		m_mouseWorld = p;

		if (m_mouseJoint) {
			m_mouseJoint->SetTarget(p);
		}

		return true;
	}

	virtual bool touchesEnded(wyMotionEvent& e) {
		if (m_mouseJoint) {
			m_box2d->getWorld()->DestroyJoint(m_mouseJoint);
			m_mouseJoint = NULL;
		}

		return true;
	}

	virtual bool touchesCancelled(wyMotionEvent& e) {
		if (m_mouseJoint) {
			m_box2d->getWorld()->DestroyJoint(m_mouseJoint);
			m_mouseJoint = NULL;
		}

		return true;
	}

	static float frand() {
		return (float) rand() / RAND_MAX;
	}

	static float frand_unit() {
		return 2.0f * rand() / RAND_MAX - 1.0f;
	}

	static float RandomFloat() {
		float r = (float)rand();
		r /= RAND_MAX;
		r = 2.0f * r - 1.0f;
		return r;
	}

	static float RandomFloat(float lo, float hi) {
		float r = (float) rand();
		r /= RAND_MAX;
		r = (hi - lo) * r + lo;
		return r;
	}
};

#include "ApplyForceTest.inl"
#include "BodyTypesTest.inl"
#include "BounceTest.inl"
#include "BreakableTest.inl"
#include "BridgeTest.inl"
#include "BulletTest.inl"
#include "BuoyancyTest.inl"
#include "CantileverTest.inl"
#include "CarTest.inl"
#include "ChainTest.inl"
#include "CollisionDetectionTest.inl"
#include "CollisionFilteringTest.inl"
#include "CompoundShapesTest.inl"
#include "ConfinedTest.inl"
#include "DominosTest.inl"
#include "EdgeShapesTest.inl"
#include "FixtureAnimation.inl"
#include "GearsTest.inl"
#include "ChainShapeTest.inl"
#include "OneWayTest.inl"
#include "PulleysTest.inl"
#include "PyramidTest.inl"
#include "RopeTest.inl"
#include "SensorTest.inl"
#include "ShapeEditingTest.inl"
#include "SimpleTest.inl"
#include "SliderCrankTest.inl"
#include "SphereStackTest.inl"
#include "TensorDampingTest.inl"
#include "TexturedBounceTest.inl"
#include "TexturedEdgeShapesTest.inl"
#include "TexturedChainShapeTest.inl"
#include "TexturedOneWayTest.inl"
#include "VaryingFrictionTest.inl"
#include "VaryingRestitutionTest.inl"
#include "WebTest.inl"
#include "PhysicsEditorTest.inl"

}

using namespace Box2D;

#if ANDROID
	#define DEMO_ENTRY_IMPL(CLASSNAME) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_box2d_##CLASSNAME##_nativeStart \
		(JNIEnv *, jobject) { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _box2d_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(ApplyForceTest);
DEMO_ENTRY_IMPL(BodyTypesTest);
DEMO_ENTRY_IMPL(BounceTest);
DEMO_ENTRY_IMPL(BreakableTest);
DEMO_ENTRY_IMPL(BridgeTest);
DEMO_ENTRY_IMPL(BulletTest);
DEMO_ENTRY_IMPL(BuoyancyTest);
DEMO_ENTRY_IMPL(CantileverTest);
DEMO_ENTRY_IMPL(CarTest);
DEMO_ENTRY_IMPL(ChainTest);
DEMO_ENTRY_IMPL(ChainShapeTest);
DEMO_ENTRY_IMPL(CollisionDetectionTest);
DEMO_ENTRY_IMPL(CollisionFilteringTest);
DEMO_ENTRY_IMPL(CompoundShapesTest);
DEMO_ENTRY_IMPL(ConfinedTest);
DEMO_ENTRY_IMPL(DominosTest);
DEMO_ENTRY_IMPL(EdgeShapesTest);
DEMO_ENTRY_IMPL(FixtureAnimationTest);
DEMO_ENTRY_IMPL(GearsTest);
DEMO_ENTRY_IMPL(OneWayTest);
DEMO_ENTRY_IMPL(PulleysTest);
DEMO_ENTRY_IMPL(PyramidTest);
DEMO_ENTRY_IMPL(RopeTest);
DEMO_ENTRY_IMPL(SensorTest);
DEMO_ENTRY_IMPL(ShapeEditingTest);
DEMO_ENTRY_IMPL(SliderCrankTest);
DEMO_ENTRY_IMPL(SphereStackTest);
DEMO_ENTRY_IMPL(TensorDampingTest);
DEMO_ENTRY_IMPL(TexturedBounceTest);
DEMO_ENTRY_IMPL(TexturedEdgeShapesTest);
DEMO_ENTRY_IMPL(TexturedChainShapeTest);
DEMO_ENTRY_IMPL(TexturedOneWayTest);
DEMO_ENTRY_IMPL(VaryingFrictionTest);
DEMO_ENTRY_IMPL(VaryingRestitutionTest);
DEMO_ENTRY_IMPL(WebTest);
DEMO_ENTRY_IMPL(SimpleTest);
DEMO_ENTRY_IMPL(PhysicsEditorTest);
