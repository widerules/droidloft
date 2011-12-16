#include "common.h"
#include "PersistenceDemos.h"
#include "WiEngine.h"
#include <stdio.h>
#include <stdlib.h>

namespace Persistence {

#if ANDROID
	#define TEST_FONT_NAME "DroidSans"
#elif IOS
	#define TEST_FONT_NAME "Verdana"
#endif

#define TARGETSELECTOR(obj, id, userData) (wyTargetSelector*)(new wyTargetSelector(obj, id, userData))->autoRelease()

static void createButton(const char* label, wyLayer* layer, float x, float y, int tsId) {
	wyNinePatchSprite* normal1 = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
	wyNinePatchSprite* pressed1 = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
	normal1->setContentSize(DP(300), DP(44));
	pressed1->setContentSize(DP(300), DP(44));

	wyButton* button1 = wyButton::make(normal1, pressed1, NULL, NULL, NULL, TARGETSELECTOR(layer, tsId, NULL));
	button1->setPosition(x, y);

	wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
    wyBitmapFontLabel* label1 = wyBitmapFontLabel::make(font, label);
    label1->setColor(wyc3b(255, 255, 0));
    label1->setPosition(x, y);

	layer->addChildLocked(button1);
    layer->addChildLocked(label1);
}

class wyCreateDatabaseTestLayer : public wyLayer {
private:
	wyLabel* m_value;

public:
	wyCreateDatabaseTestLayer() {
		// value label
		m_value = wyLabel::make("value is");
		m_value->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight * 3 / 4);
		addChildLocked(m_value);

		// create buttons
		createButton("Create Database", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2 + DP(40), 1);
		createButton("Delete Database", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2 - DP(40), 2);

        // if database exist, update hint label now
		const char* path = "/sdcard/WiEngine/create_database_test.db";
		if(wyUtils::isPathExists(path)) {
			// create database and open it
			wyDatabase* db = wyDatabase::make(path);
			db->open();

			// query the value inserted
			wyResultSet* rs = db->executeQuery("SELECT * FROM test");
			if(rs != NULL && rs->next()) {
				int value = rs->intForColumn("test_column");
				char buf[64];
				sprintf(buf, "value is %d", value);
				m_value->setText(buf);
			}
		}
	}

	virtual ~wyCreateDatabaseTestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		const char* path = "/sdcard/WiEngine/create_database_test.db";
		switch(ts->getId()) {
			case 1:
			{
				// check existence of database
				if(wyUtils::isPathExists(path)) {
					wyUtils::showToast("database already exists, so won't create it again");
				} else {
					// create database and open it
					wyDatabase* db = wyDatabase::make(path);
					db->open();

					// create a test table into it
					int value = wyMath::randMax(1000);
					db->executeUpdate("CREATE TABLE test (_id INTEGER PRIMARY KEY autoincrement, test_column INTEGER)");
					db->executeUpdate("INSERT INTO test (test_column) VALUES (%d)", value);

					// the database is auto released before make returns, and it will be closed when it is deconstructed
					// so no need to call close here, just update hint label
					char buf[64];
					sprintf(buf, "value is %d", value);
					m_value->setText(buf);
				}
				break;
			}
			case 2:
			{
				if(wyUtils::deleteFile(path)) {
					wyUtils::showToast("database file deleted, you can create it again");
					m_value->setText("value is");
				} else
					wyUtils::showToast("failed to delete database file");
				break;
			}
		}
	}
};

class wyPreferenceTestLayer : public wyLayer {
public:
	wyPreferenceTestLayer() {
		// label for text box
		wyLabel* label = wyLabel::make("", SP(18), BOLD, 0, WY_DEFAULT_FONT);
		label->setColor(wyc3bBlack);

        // create state sprite for text box
		wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.textbox_normal")), wyr(DP(13), DP(18), DP(1), DP(1)));
		wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.textbox_pressed")), wyr(DP(13), DP(18), DP(1), DP(1)));
		wyNinePatchSprite* focused = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.textbox_focused")), wyr(DP(13), DP(18), DP(1), DP(1)));
		normal->setContentSize(DP(280), DP(32));
		pressed->setContentSize(DP(280), DP(32));
		focused->setContentSize(DP(280), DP(32));

		// create text box
        wyTextBox* box = wyTextBox::make(normal, pressed, NULL, focused, label);
        box->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
        box->setTitle("Input something!");
        box->setMessage("试试中文的提示怎么样");
        box->setPositiveButton("想修改就点我");
        box->setNegativeButton("Oh No!");
        box->setPadding(DP(10), DP(3), DP(10), DP(6));
        static wyTextBoxCallback callback = {
        		NULL,
        		NULL,
        		onEndEditing
        };
        box->setCallback(&callback, this);
        addChildLocked(box);

        // label of hint
        wyLabel* hint = wyLabel::make("Input number and \nit will be saved", SP(18), BOLD, 0, WY_DEFAULT_FONT);
        hint->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2 + DP(44));
        addChildLocked(hint);

        // load value from preference, default value is -1
        char buf[32];
        int v = wyPrefs::getInt("test.int.pref", -1);
		sprintf(buf, "%d", v);
        box->setText(buf);
	}

	virtual ~wyPreferenceTestLayer() {
	}

	static void onEndEditing(wyTextBox* box, void* data) {
		wyPrefs::setInt("test.int.pref", atoi(box->getText()));
	}
};

class wySQLFileTestLayer : public wyLayer {
public:
	wySQLFileTestLayer() {
		createButton("Execute SQL File", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2, 1);
		createButton("Delete Database", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2 - DP(80), 2);
	}

	virtual ~wySQLFileTestLayer() {
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		const char* path = "/sdcard/WiEngine/sqlfile_test.db";
		switch(ts->getId()) {
			case 1:
			{
				if(!wyUtils::isPathExists(path)) {
					// create database and open it
					wyDatabase* db = wyDatabase::make(path);
					db->open();

					// execute sql file now
					if(db->executeSQL(RES("R.raw.cd")))
						wyUtils::showToast("SQL file executed successful, go to command line to verify the database");
					else
						wyUtils::showToast("SQL file is failed to be executed, nothing applied to database");
				}
				break;
			}
			case 2:
			{
				if(wyUtils::deleteFile(path))
					wyUtils::showToast("database file deleted, you can create it again");
				else
					wyUtils::showToast("failed to delete database file");
				break;
			}
		}
	}
};

class wyTransactionTestLayer : public wyLayer {
private:
	wyLabel* m_hint;
	wyDatabase* m_db;

public:
	wyTransactionTestLayer() : m_db(NULL) {
		const char* path = "/sdcard/WiEngine/transaction_test.db";

		// value label
		m_hint = wyLabel::make("row count: 0");
		m_hint->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight * 3 / 4);
		addChildLocked(m_hint);

		// create buttons
		createButton("Insert One Row", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2, 1);
		createButton("Commit", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2 - DP(60), 2);
		createButton("Rollback", this, wyDevice::winWidth / 2, wyDevice::winHeight / 2 - DP(120), 3);

		// open database, if it is not there, will create it
		m_db = wyDatabase::make(path);
		m_db->open();
		m_db->retain();

		// if no test table, create it
		if(!m_db->tableExists("test"))
			m_db->executeUpdate("CREATE TABLE test (_id INTEGER PRIMARY KEY autoincrement, test_column INTEGER)");

		// query the value inserted
		int rowCount = m_db->intForQuery("SELECT count() FROM test");
		char buf[64];
		sprintf(buf, "row count: %d", rowCount);
		m_hint->setText(buf);
	}

	virtual ~wyTransactionTestLayer() {
		m_db->release();
	}

	virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
		switch(ts->getId()) {
			case 1:
			{
				// if not in transaction, start transaction
				if(!m_db->isInTransaction())
					m_db->beginTransaction();

				// execute insertion
				m_db->executeUpdate("INSERT INTO test (test_column) VALUES (%d)", wyMath::randMax(1000));
				break;
			}
			case 2:
			{
				// commit
				if(m_db->commit()) {
					// query the value inserted
					int rowCount = m_db->intForQuery("SELECT count() from test");
					char buf[64];
					sprintf(buf, "row count: %d", rowCount);
					m_hint->setText(buf);
				} else {
					wyUtils::showToast("commit transaction failed");
				}
				break;
			}
			case 3:
			{
				// rollback
				if(m_db->rollback()) {
					wyUtils::showToast("rollback transaction successful");
				} else {
					wyUtils::showToast("rollback transaction failed");
				}
				break;
			}
		}
	}
};

}

using namespace Persistence;

#if ANDROID
	#define DEMO_ENTRY_IMPL(CLASSNAME) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_persistence_##CLASSNAME##_nativeStart \
		(JNIEnv *env, jobject thiz) { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _persistence_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(CreateDatabaseTest);
DEMO_ENTRY_IMPL(PreferenceTest);
DEMO_ENTRY_IMPL(SQLFileTest);
DEMO_ENTRY_IMPL(TransactionTest);
