#include "common.h"
#include "OtherDemos.h"
#include "WiEngine.h"
#include "wyZwoptexManager.h"
#include <vector>
#include <stdio.h>
#include "memory_file.h"

namespace Other {

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

	static const GLenum ALPHA_FUNC[] = {
		GL_NEVER,
		GL_LESS,
		GL_LEQUAL,
		GL_EQUAL,
		GL_GEQUAL,
		GL_GREATER,
		GL_NOTEQUAL
	};
	
	static const char* ALPHA_FUNC_TEXT[] = {
		"NEVER",
		"LESS",
		"LEQUAL",
		"EQUAL",
		"GEQUAL",
		"GREATER",
		"NOTEQUAL"
	};
	
	class wyAlphaTestTestLayer : public wyLayer {
	private:
		wySprite* m_sprite;
		wySlider* m_slider;
		wyLabel* m_hint;
		wyBitmapFontLabel* m_funcLabel;
		GLenum m_alphaFunc;
		
		int m_currentFunc;
		
	public:
		wyAlphaTestTestLayer() {
			// init member
			m_currentFunc = 4;
			m_alphaFunc = ALPHA_FUNC[m_currentFunc];
			
			// create
			m_sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.alpha_test")));
			m_sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2 + DP(80));
			addChildLocked(m_sprite);
			
			// create a hint label
			m_hint = wyLabel::make("drag slider", SP(18));
			m_hint->setPosition(wyDevice::winWidth / 2, DP(80));
			addChildLocked(m_hint);
			
			// alpha value slider
			wySprite* bar = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.bar")));
			wySprite* thumb = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.thumb")));
			m_slider = wySlider::make(NULL, bar, thumb);
			m_slider->setValue(0);
			m_slider->setMax(255);
			m_slider->setShowFullBar(true);
			m_slider->setPosition(wyDevice::winWidth / 2, DP(50));
			wySliderCallback callback = {
				onValueChanged
			};
			m_slider->setCallback(&callback, this);
			addChildLocked(m_slider);
			
			// alpha func button
			wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
			wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
			normal->setContentSize(DP(280), DP(44));
			pressed->setContentSize(DP(280), DP(44));
			wyButton* button = wyButton::make(normal, pressed, NULL, NULL, NULL,
											  wyTargetSelector::make(this, SEL(wyAlphaTestTestLayer::onSelectAlphaFunc), NULL));
			button->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30));
			addChildLocked(button, 1);
			
			// label for alpha func button
			wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
			m_funcLabel = wyBitmapFontLabel::make(font, ALPHA_FUNC_TEXT[4]);
			m_funcLabel->setColor(wyc3b(255, 255, 0));
			m_funcLabel->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(30));
			addChildLocked(m_funcLabel, 1);
		}
		
		virtual ~wyAlphaTestTestLayer() {
		}
		
		void onSelectAlphaFunc() {
			m_currentFunc++;
			m_currentFunc %= 7;
			m_alphaFunc = ALPHA_FUNC[m_currentFunc];
			m_funcLabel->setText(ALPHA_FUNC_TEXT[m_currentFunc]);
			m_sprite->setAlphaFunc(m_alphaFunc, m_slider->getValue() / 255.f);
		}
		
		static void onValueChanged(wySlider* slider, void* data) {
			wyAlphaTestTestLayer* layer = (wyAlphaTestTestLayer*)data;
			layer->m_sprite->setAlphaFunc(layer->m_alphaFunc, slider->getValue() / 255.f);
			
			char buf[8];
			sprintf(buf, "%d", (int)slider->getValue());
			layer->m_hint->setText(buf);
		}
	};

	static const int TILE_WIDTH = 32;
	static const int TILE_HEIGHT = 32;
	
	class wyAStarTestLayer : public wyLayer {
	private:
		bool m_isRunning;
		float m_tile_width;
		float m_tile_height;
		int m_tile_x;
		int m_tile_y;
		
		int m_endx;
		int m_endy;
		int m_findStartx;
		int m_findStarty;
		int m_startx;
		int m_starty;
		
		wyRect m_rect_free;
		wyRect m_rect_block;
		wyRect m_rect_r1;
		
		wyArray* m_bgs;
		
		wyAStarMap* m_map;
		
		wyAStar* m_astar;
		
		wyArray* m_paths;
		
		wySprite* m_moveSprite;
		
		static bool releaseNode(wyArray* arr, void* ptr, int index, void* data) {
			wyObjectRelease((wyObject*) ptr);
			return true;
		}
		
		static void onMoveEnded(wyAction* action, void* data) {
			wyAStarTestLayer* layer = (wyAStarTestLayer*) data;
			if (layer->canRunning()) {
				if (layer->isEndPosition()) {
					layer->setStartPosition();
					layer->runPath();
				} else {
					layer->runNextMove();
				}
			}
		}
		
	public:
		wyAStarTestLayer() {
			m_isRunning = false;
			m_paths = NULL;
			m_tile_width = DP(TILE_WIDTH);
			m_tile_height = DP(TILE_HEIGHT);
			m_tile_x = wyDevice::winWidth / m_tile_width;
			m_tile_y = wyDevice::winHeight / m_tile_height;
			
			wyTexture2D* obj = wyTexture2D::makePNG(RES("R.drawable.astar"));
			wyZwoptexManager::getInstance()->addZwoptex("astar", RES("R.raw.astar"));
			
			m_findStartx = m_startx = m_tile_x / 2;
			m_endx = m_tile_x / 2;
			m_findStarty = m_starty = m_tile_y - 1;
			m_endy = 0;
			
			m_rect_free = wyr(0, m_tile_width, m_tile_width, m_tile_width);
			m_rect_block = wyr(m_tile_width, 0, m_tile_width, m_tile_width);
			m_rect_r1 = wyr(0, 0, m_tile_width, m_tile_width);
			
			wySpriteBatchNode* batchNode = wySpriteBatchNode::make(obj);
			addChildLocked(batchNode);
			
			m_bgs = wyArrayNew(m_tile_x * m_tile_y);
			for (int x = 0; x < m_tile_x; x++) {
				for (int y = 0; y < m_tile_y; y++) {
					wySpriteEx* sprite = wyZwoptexManager::getInstance()->makeSpriteEx("astar", "tile_free.png", obj);
					sprite->setPosition(x * m_tile_width + m_tile_width / 2, y * m_tile_height + m_tile_height / 2);
					batchNode->addChild(sprite, 0 , 0);
					wyArrayPush(m_bgs, sprite);
				}
			}
			
			m_moveSprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.r1")));
			setStartPosition();
			addChildLocked( m_moveSprite);
			
			m_map = new wyRectAStarMap(m_tile_x, m_tile_y, false);
			m_astar = new wyAStar(m_map);
			m_map->release();
			runPath();
			setTouchEnabled(true);
			
			// start
			wySpriteEx* sprite = (wySpriteEx*) wyArrayGet(m_bgs, m_startx * m_tile_y + m_starty);
			sprite->setTextureRect(m_rect_r1);
			
			// end
			sprite = (wySpriteEx*) wyArrayGet(m_bgs, m_endx * m_tile_y + m_endy);
			sprite->setTextureRect(m_rect_r1);
		}
		
		virtual ~wyAStarTestLayer() {
			m_astar->release();
		}
		
		bool canRunning() { return m_isRunning; }
		
		void runPath() {
			m_isRunning = false;
			m_moveSprite->stopAllActions();
			m_findStartx = m_moveSprite->getPositionX() / m_tile_width;
			m_findStarty = m_moveSprite->getPositionY() / m_tile_height;
			
			m_astar->findPath(m_findStartx, m_findStarty, m_endx, m_endy, false);
			m_paths = m_astar->getPath();
			if (m_paths != NULL && m_paths->num > 0) {
				m_isRunning = true;
				runNextMove();
			}
		}
		
		void runNextMove() {
			wyAStarStep* step = (wyAStarStep*) wyArrayPop(m_paths);
			if (step != NULL) {
				wyMoveTo* move = new wyMoveTo(0.618f, m_moveSprite->getPositionX(), m_moveSprite->getPositionY(),
											  step->getX() * m_tile_width + m_tile_width / 2,
											  step->getY() * m_tile_height + m_tile_height / 2);
				wyObjectRelease(step);
				wyActionCallback moveCallback = { NULL, onMoveEnded, NULL };
				move->setCallback(&moveCallback, this);
				m_moveSprite->runAction(move);
				wyObjectRelease(move);
			}
		}
		
		bool isEndPosition() {
			if (m_moveSprite->getPositionX() == m_endx * m_tile_width + m_tile_width / 2
                && m_moveSprite->getPositionY() == m_endy * m_tile_height + m_tile_height / 2) {
				return true;
			}
			return false;
		}
		
		void setStartPosition() {
			m_moveSprite->setPosition(m_startx * m_tile_width + m_tile_width / 2, m_starty * m_tile_height + m_tile_height / 2);
			m_findStartx = m_startx;
			m_findStarty = m_starty;
		}
		
		virtual bool touchesEnded(wyMotionEvent& event) {
			float e_x = event.x[0];
			float e_y = event.y[0];
			
			int tile_x = e_x / m_tile_width;
			int tile_y = e_y / m_tile_height;
			
			if (tile_x == m_startx && tile_y == m_starty) {
				return true;
			}
			
			if (tile_x == m_endx && tile_y == m_endy) {
				return true;
			}
			
			int s_x = m_moveSprite->getPositionX() / m_tile_width;
			int s_y = m_moveSprite->getPositionY() / m_tile_height;
			
			if (tile_x == s_x && tile_y == s_y) {
				return true;
			}
			
			wySpriteEx* sprite = (wySpriteEx*) wyArrayGet(m_bgs, tile_x * m_tile_y + tile_y);
			
			if (m_map->isBlockedAt(tile_x, tile_y)) {
				m_map->freeTile(tile_x, tile_y);
				sprite->setTextureRect(m_rect_free);
			} else {
				m_map->blockTile(tile_x, tile_y);
				sprite->setTextureRect(m_rect_block);
			}
			
			if (s_x != m_endx || s_y != m_endy) {
				runPath();
			}
			return false;
		}
	};

	class wyColorFilterTestLayer : public wyLayer {
		wyTexture2D* m_tex;
		int m_nextFilter;
		
	public:
		wyColorFilterTestLayer() {
			m_nextFilter = 0;
			
			// create texture
			m_tex = wyTexture2D::makePNG(RES("R.drawable.grossini"));
			wySprite* sprite = wySprite::make(m_tex);
			sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			addChildLocked(sprite);
			
			// sprite for button
			wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
			wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
			normal->setContentSize(DP(300), DP(44));
			pressed->setContentSize(DP(300), DP(44));
			
			// button 1
			wyButton* button = wyButton::make(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(this, 1, NULL));
			button->setPosition(wyDevice::winWidth / 2, DP(70));
			
			// label for button 1
			wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
			wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Change Color Filter");
			label->setColor(wyc3b(255, 255, 0));
			label->setPosition(wyDevice::winWidth / 2, DP(70));
			
			// add
			addChildLocked(button);
			addChildLocked(label);
		}
		
		virtual ~wyColorFilterTestLayer() {
		}
		
		static void customApply(void* imageData, int width, int height, void* userData) {
			// 上边框
			unsigned char* p = (unsigned char*)imageData;
			for(int x = 0; x < width; x++) {
				p[0] = 0xFF;
				p[1] = 0;
				p[2] = 0;
				p[3] = 0xFF;
				p += 4;
			}
			
			// 下边框
			p = (unsigned char*)imageData + (height - 1) * width * 4;
			for(int x = 0; x < width; x++) {
				p[0] = 0xFF;
				p[1] = 0;
				p[2] = 0;
				p[3] = 0xFF;
				p += 4;
			}
			
			// 左边框
			p = (unsigned char*)imageData;
			for(int y = 0; y < height; y++) {
				p[0] = 0xFF;
				p[1] = 0;
				p[2] = 0;
				p[3] = 0xFF;
				p += width * 4;
			}
			
			// 右边框
			p = (unsigned char*)imageData + (width - 1) * 4;
			for(int y = 0; y < height; y++) {
				p[0] = 0xFF;
				p[1] = 0;
				p[2] = 0;
				p[3] = 0xFF;
				p += width * 4;
			}
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
			switch(ts->getId()) {
				case 1:
				{
					switch(m_nextFilter) {
						case 0:
						{
							// 将贴图变为黑白
							wyColorMatrix* m = wyColorMatrix::make();
							m->setSaturation(0);
							wyColorMatrixColorFilter* filter = wyColorMatrixColorFilter::make(m);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 1:
						{
							// 提高贴图的对比度
							wyColorMatrix* m = wyColorMatrix::make();
							m->setScale(2, 2, 2, 1);
							wyColorMatrixColorFilter* filter = wyColorMatrixColorFilter::make(m);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 2:
						{
							// 增强红色
							wyColorMatrix* m = wyColorMatrix::make();
							m->setScale(2, 1, 1, 1);
							wyColorMatrixColorFilter* filter = wyColorMatrixColorFilter::make(m);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 3:
						{
							// 底片效果
							wyColorMatrix* m = wyColorMatrix::make();
							m->setNegative();
							wyColorMatrixColorFilter* filter = wyColorMatrixColorFilter::make(m);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 4:
						{
							// 消除所有红色和绿色, 只留下蓝色
							wyLightingColorFilter* filter = wyLightingColorFilter::make(0xff0000ff, 0x0);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 5:
						{
							// 对图片的轮廓做一个红色的剪影
							wyLightingColorFilter* filter = wyLightingColorFilter::make(0xff000000, 0xffff0000);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 6:
						{
							// 给它换一身颜色的衣服, 预先已经知道衣服颜色是0xff330033
							wyMappingColorFilter* filter = wyMappingColorFilter::make();
							filter->mapColor(0xff330033, 0xff319c7a);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 7:
						{
							// 通过组合式过滤器, 先把图片变黑白, 再做底片, 相当于得到图像的黑白底片
							
							// 变成黑白
							wyColorMatrix* m1 = wyColorMatrix::make();
							m1->setSaturation(0);
							wyColorMatrixColorFilter* filter1 = wyColorMatrixColorFilter::make(m1);
							
							// 变成底片
							wyColorMatrix* m2 = wyColorMatrix::make();
							m2->setNegative();
							wyColorMatrixColorFilter* filter2 = wyColorMatrixColorFilter::make(m2);
							
							// 组合
							wyCombineColorFilter* filter = wyCombineColorFilter::make(filter1, filter2, NULL);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							
							break;
						}
						case 8:
						{
							// 对图片做浮雕效果
							wyEmbossColorFilter* filter = wyEmbossColorFilter::make(50);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 9:
						{
							// 模糊效果
							wyBlurColorFilter* filter = wyBlurColorFilter::make(1);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 10:
						{
							// 图片周围产生一个光环
							wyGlowColorFilter* filter = wyGlowColorFilter::make(wyc3bBlue, DP(10), DP(5), 128);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 11:
						{
							/*
							 * 给图片做一个背景阴影, 同时背景色改为红色, 以便让图片看起来有边框形成立体感
							 * 背景色是0x00990033, 这是预先知道的, 由于必须判断和替换alpha值, 但是我们不能设置掩码为0xffffffff
							 *
							 * 这是因为在Android和iOS上, 载入图片后, 纯透明的区域像素值有差异.
							 * 在Android上, 目前即使alpha为全0, rgb值也保持和原始数据相同
							 * 而在iOS上, 目前如果alpha为0, 则rgb也是0
							 * 因此为了平台一致, 当你需要替换纯透明区域时, 应该把fromMask设置为0xff000000, 这样只比较alpha值, 可以
							 * 得到平台一致的结果
							 */
							wyMappingColorFilter* filter1 = wyMappingColorFilter::make();
							filter1->mapColor(0x00990033, 0xffff0000, 0xff000000, 0xffffffff);
							wyDropShadowColorFilter* filter2 = wyDropShadowColorFilter::make(wyc3bBlue, DP(5), DP(2), 128);
							wyCombineColorFilter* filter = wyCombineColorFilter::make(filter1, filter2, NULL);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 12:
						{
							// 在图像上加了一个红色的框
							wyAreaColorFilter* filter = wyAreaColorFilter::make();
							filter->mapRect(10, 10, 30, 40, 0, 0xffff0000, 0, 0xffffffff);
							filter->removeRect(15, 15, 20, 30);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 13:
						{
							// 自定义的滤镜, 给图片加个红色边界
							static wyColorFilterDelegate delegate = {
								customApply
							};
							wyCustomColorFilter* filter = wyCustomColorFilter::make(&delegate, NULL);
							m_tex->setColorFilter(filter);
							m_tex->applyFilter();
							break;
						}
						case 14:
						{
							m_nextFilter = -1;
							m_tex->setColorFilter(NULL);
							m_tex->applyFilter();
							break;
						}
					}
					
					m_nextFilter++;
					break;
				}
			}
		}
	};

	class wyDrawPrimitivesTestLayer : public wyLayer {
	public:
		wyDrawPrimitivesTestLayer() {
			wyRotateBy* rotate = new wyRotateBy(4, -360);
			runAction(rotate);
			wyObjectRelease(rotate);
		}
		
		virtual ~wyDrawPrimitivesTestLayer() {
		}
		
		virtual void draw(){
			// 画简单线段
			// 默认参数: 宽度 1, 颜色 RGB(255,255,255,255)
			// Anti-Aliased
			glEnable(GL_LINE_SMOOTH);
			
			wyDrawLine(0, 0, wyDevice::winWidth, wyDevice::winHeight);
			
			// 关掉反锯齿
			glDisable(GL_LINE_SMOOTH);
			// 设置线段宽度, 宽度大于1时，无法使用GL_LINE_SMOOTH。
			glLineWidth(5.0f);
			// 设置颜色
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			wyDrawLine(0, wyDevice::winHeight, wyDevice::winWidth, 0);
			
			// 记住：OpenGL是一个状态机.如果你不再次设置画图参数，当前属性不会改变。
			
			// 在中心画一个大点（半径为64)
			glPointSize(64);
			glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
			wyDrawPoint(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			
			// 画四个小点（半径为4）
			float points[] = {
				60, 60, 70, 70, 60, 70, 70, 60
			};
			glPointSize(4);
			glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
			wyDrawPoints(points, sizeof(points) / sizeof(float));
			
			// 画一个绿色圆圈，圆圈被分成10段
			glLineWidth(16);
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			wyDrawCircle(wyDevice::winWidth / 2, wyDevice::winHeight / 2, 100, 0, 10, false);
			
			// 画一个青色圆圈，圆圈被分成50段。并且带一条中心到圆周的直线。
			// draw a green circle with 50 segments with line to center
			glLineWidth(2);
			glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
			wyDrawCircle(wyDevice::winWidth / 2, wyDevice::winHeight / 2, 50, 90, 50, true);
			
			// 画一个黄色的多边形
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glLineWidth(10);
			float vertices[] = {
				0, 0, 50, 50, 100, 50, 100, 100, 50, 100
			};
			wyDrawPoly(vertices, sizeof(vertices) / sizeof(float), false);
			
			// 画闭合多边形
			glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
			glLineWidth(2);
			float vertices2[] = {
				30, 130, 30, 230, 50, 200
			};
			wyDrawPoly(vertices2, sizeof(vertices2) / sizeof(float), true);
			
			// 画"二次贝赛尔曲线"
			wyBezierConfig c = wybcQuad(0, wyDevice::winHeight, wyDevice::winWidth / 2, wyDevice::winHeight / 2, wyDevice::winWidth, wyDevice::winHeight);
			wyDrawBezier(c, 50);
			
			// 画"三次贝塞尔曲线"
			c = wybcCubic(wyDevice::winWidth / 2,
						  wyDevice::winHeight / 2,
						  wyDevice::winWidth / 2 + 30,
						  wyDevice::winHeight / 2 + 50,
						  wyDevice::winWidth / 2 + 60,
						  wyDevice::winHeight / 2 - 50,
						  wyDevice::winWidth,
						  wyDevice::winHeight / 2);
			wyDrawBezier(c, 100);
			
			//画填充矩形
			float vertices_fill[] = {
				400, 400, 500, 400, 500, 500, 400, 500, 350, 450
			};
			wyDrawSolidPoly(vertices_fill, 10, wyc4bRed);
			
			// 重置
			glLineWidth(1);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glPointSize(1);
		}
	};

	class wyMemoryFileSystemTestLayer: public wyLayer {
	public:
		wyMemoryFileSystemTestLayer() {
			setTouchEnabled(true);
			addNewSprite(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
		}
		
		virtual ~wyMemoryFileSystemTestLayer() {}
		
		virtual bool touchesBegan(wyMotionEvent& event) {
			addNewSprite(event.x[0], event.y[0]);
			return true;
		}
		
		void addNewSprite(float posx, float posy) {
			char buf[128];
			int idx = rand() % 14 + 1;
			sprintf(buf, "grossini_dance_%02d.png", idx);
			wyTexture2D* tex = wyTexture2D::makeMemoryPNG(buf);
			wySprite* sprite = wySprite::make(tex);
			sprite->setFlipX(rand()%2);
			sprite->setFlipY(rand()%2);
			sprite->setRotation(rand() % 360);
			sprite->setPosition(posx, posy);
			addChildLocked(sprite);
		}
	};

	class wyNodePhysicsTestLayer : public wyLayer {
	private:
		wyLabel* m_hint;
		
	public:
		wyNodePhysicsTestLayer() {
			wySprite* sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
			sprite->setVelocity(100, 100);
			sprite->setAcceleration(100, 100);
			static wyNodePositionListener listener = {
				onPositionChanged
			};
			sprite->setPositionListener(&listener, this);
			addChildLocked(sprite);
			
			// create a hint label
			m_hint = wyLabel::make("callback display label", SP(20));
			m_hint->setPosition(wyDevice::winWidth / 2, 100);
			addChildLocked(m_hint);
		}
		
		virtual ~wyNodePhysicsTestLayer() {
		}
		
		static void onPositionChanged(wyNode* node, void* data) {
			float px = node->getPositionX();
			float py = node->getPositionY();
			if(px > wyDevice::winWidth || py > wyDevice::winHeight) {
				node->setVelocity(-100, -100);
				node->setAcceleration(-100, -100);
			} else if(px < 0 || py < 0) {
				node->setVelocity(100, 100);
				node->setAcceleration(100, 100);
			}
			
			char buf[32];
			sprintf(buf, "pos: %.1f, %.1f", px, py);
			wyNodePhysicsTestLayer* layer = (wyNodePhysicsTestLayer*)data;
			layer->m_hint->setText(buf);
		}
	};

	class wyPauseResumeGameTestLayer : public wyLayer {
	private:
		wyLabel* m_label;
		
	public:
		wyPauseResumeGameTestLayer() {
			wySprite* sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
			addChildLocked(sprite);
			sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight/ 2);
			
			// run a rotate action
			wyIntervalAction* rotate = wyRotateBy::make(2, 360);
			wyAction* action1 = wyRepeatForever::make(rotate);
			action1->setTag(100); // for pause action, assign a tag to action
			sprite->runAction(action1);
			
			// run a move action
			wyMoveBy* m = wyMoveBy::make(2, wyDevice::winWidth - 120, 0);
			wyMoveBy* r = (wyMoveBy*)m->reverse()->autoRelease();
			wyIntervalAction* t = wySequence::make(m, r, NULL);
			wyAction* action2 = wyRepeatForever::make(t);
			sprite->setPosition(60, wyDevice::winHeight / 2);
			sprite->runAction(action2);
			
			// pause hint
			m_label = wyLabel::make("Paused");
			m_label->setPosition(wyDevice::winWidth / 2, DP(100));
			m_label->setVisible(false);
			addChildLocked(m_label);
			
			// enable touch
			setTouchEnabled(true);
		}
		
		virtual ~wyPauseResumeGameTestLayer() {
		}
		
		virtual bool touchesBegan(wyMotionEvent& e) {
			wyDirector* d = wyDirector::getInstance();
			if(d->isUIPaused()) {
				m_label->setVisible(false);
				d->resumeUI();
			} else {
				m_label->setVisible(true);
				d->pauseUI();
			}
			
			return true;
		}
	};

	class wyPickFromAlbumTestLayer : public wyLayer {
	private:
		wySprite* m_bg;
		wyTexture2D* m_tex;
		
	public:
		wyPickFromAlbumTestLayer() :
		m_bg(NULL),
		m_tex(NULL) {
			// the background sprite
			m_bg = wySprite::make(NULL);
			m_bg->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			m_bg->setBlend(false);
			addChildLocked(m_bg);
			
			// buttons
			createButton("From Photo Album", this, wyDevice::winWidth / 2, wyDevice::winHeight - DP(40), 1);
		}
		
		virtual ~wyPickFromAlbumTestLayer() {
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
			static wyImagePickerCallback callback = {
				onImagePicked,
				onImagePickCancelled
			};
			
			switch(ts->getId()) {
				case 1:
				{
					wyImagePicker::pickFromAlbum(&callback, this, wyDevice::winWidth, wyDevice::winHeight, false);
					break;
				}
			}
		}
		
		static void onImagePicked(const char* raw, int w, int h, void* data) {
			wyUtils::showToast("image picked");
			
			// get layer
			wyPickFromAlbumTestLayer* layer = (wyPickFromAlbumTestLayer*)data;
			
			// release old texture
			wyTextureManager::getInstance()->removeTexture(layer->m_tex, true);
			
			// update background sprite
			layer->m_tex = wyTexture2D::makeRaw(raw, w, h);
			layer->m_bg->setTexture(layer->m_tex);
			
			// free data
			free((void*)raw);
		}
		
		static void onImagePickCancelled(void* data) {
			wyUtils::showToast("cancelled");
		}
	};

	class wyPickFromCameraTestLayer : public wyLayer {
	private:
		wySprite* m_bg;
		wyTexture2D* m_tex;
		
	public:
		wyPickFromCameraTestLayer() :
		m_bg(NULL),
		m_tex(NULL) {
			// the background sprite
			m_bg = wySprite::make(NULL);
			m_bg->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			m_bg->setBlend(false);
			addChildLocked(m_bg);
			
			// buttons
			createButton("From Rear Camera", this, wyDevice::winWidth / 2, wyDevice::winHeight - DP(40), 1);
			createButton("From Front Camera", this, wyDevice::winWidth / 2, wyDevice::winHeight - DP(100), 2);
		}
		
		virtual ~wyPickFromCameraTestLayer() {
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
			static wyImagePickerCallback callback = {
				onImagePicked,
				onImagePickCancelled
			};
			
			switch(ts->getId()) {
				case 1:
				{
					if(wyImagePicker::hasCamera()) {
						wyImagePicker::pickFromCamera(&callback, this, wyDevice::winWidth, wyDevice::winHeight, false);
					} else {
						wyUtils::showToast("This device doesn't have camera");
					}
					break;
				}
				case 2:
				{
					if(wyImagePicker::hasFrontCamera()) {
						wyImagePicker::pickFromFrontCamera(&callback, this, wyDevice::winWidth, wyDevice::winHeight, false);
					} else {
						wyUtils::showToast("This device doesn't have front camera");
					}
					break;
				}
			}
		}
		
		static void onImagePicked(const char* raw, int w, int h, void* data) {
			wyUtils::showToast("image picked");
			
			// get layer
			wyPickFromCameraTestLayer* layer = (wyPickFromCameraTestLayer*)data;
			
			// release old texture
			wyTextureManager::getInstance()->removeTexture(layer->m_tex, true);
			
			// update background sprite
			layer->m_tex = wyTexture2D::makeRaw(raw, w, h);
			layer->m_bg->setTexture(layer->m_tex);
			
			// free data
			free((void*)raw);
		}
		
		static void onImagePickCancelled(void* data) {
			wyUtils::showToast("cancelled");
		}
	};

	class wyPixelOperationTestLayer : public wyLayer {
	private:
		char* m_raw;
		wyTexture2D* m_tex;
		size_t m_line;
		size_t m_rawWidth;
		size_t m_rawHeight;
		
	public:
		wyPixelOperationTestLayer() {
			// load raw data and create texture from raw data
			m_line = 0;
			m_raw = wyUtils::loadPNG(RES("R.drawable.grossini"), &m_rawWidth, &m_rawHeight, false);
			m_tex = wyTexture2D::makeRaw(m_raw, m_rawWidth, m_rawHeight);
			wySprite* sprite = wySprite::make(m_tex);
			sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			addChildLocked(sprite);
			
			// sprite for button
			wyNinePatchSprite* normal = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
			wyNinePatchSprite* pressed = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
			normal->setContentSize(DP(300), DP(44));
			pressed->setContentSize(DP(300), DP(44));
			
			// button 1
			wyButton* button = wyButton::make(normal, pressed, NULL, NULL, NULL, TARGETSELECTOR(this, 1, NULL));
			button->setPosition(wyDevice::winWidth / 2, DP(70));
			
			// label for button 1
			wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
			wyBitmapFontLabel* label = wyBitmapFontLabel::make(font, "Make a Line Blue");
			label->setColor(wyc3b(255, 255, 0));
			label->setPosition(wyDevice::winWidth / 2, DP(70));
			
			// add
			addChildLocked(button);
			addChildLocked(label);
		}
		
		virtual ~wyPixelOperationTestLayer() {
		}
		
		virtual void onExit() {
			wyLayer::onExit();
			
			free(m_raw);
			wyTextureManager::getInstance()->removeTexture(m_tex, true);
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
			switch(ts->getId()) {
				case 1:
				{
					// change line to blue
					int start = m_line * m_rawWidth * 4;
					for(int x = 0; x < m_rawWidth * 4; x += 4) {
						m_raw[start + x] = 0x00; // R
						m_raw[start + x + 1] = 0x00; // G
						m_raw[start + x + 2] = 0xFF; // B
						m_raw[start + x + 3] = 0xFF; // A
					}
					
					m_line++;
					m_tex->updateRaw(m_raw);
					break;
				}
			}
		}
	};

	class wySchedulerTestLayer : public wyLayer{
		wyLabel* m_Label1;
		wyLabel* m_Label2;
		wyLabel* m_Label3;
		int label1Count;
		int label2Count;
		
		wyTimer* timer3;
		
	public:
		wySchedulerTestLayer(){
			label1Count = 0;
			label2Count = 0;
			
			m_Label1 = new wyLabel();
			m_Label1->setString("Lable1: 0");
			m_Label1->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2 + 80);
			addChildLocked(m_Label1);
			
			
			m_Label2 = new wyLabel();
			m_Label2->setString("Lable2: 0");
			m_Label2->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			addChildLocked(m_Label2);
			
			m_Label3 = new wyLabel();
			m_Label3->setString("Lable3: 0");
			m_Label3->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2 - 80);
			addChildLocked(m_Label3);
			
			wyTimer* timer1 = wyTimer::make((wyTargetSelector*)(new wyTargetSelector(this, 0, &label1Count))->autoRelease(), 1.0f);
			wyScheduler::getInstance()->scheduleLocked(timer1);
			
			// 每10帧调用一次updateLabel2，来更新第二个label
			wyTimer* timer2 = wyTimer::make((wyTargetSelector*)(new wyTargetSelector(this, 1, &label2Count))->autoRelease(), 10);
			wyScheduler::getInstance()->scheduleLocked(timer2);
			
			// 只更新Label 3一次, 延迟2秒
			timer3 = wyTimer::make((wyTargetSelector*)(new wyTargetSelector(this, 2, NULL))->autoRelease(), 2.0f);
			timer3->setOneShot(true);
			m_Label3->scheduleLocked(timer3);
			
			setTouchEnabled(true);
		}
		
		virtual ~wySchedulerTestLayer(){
			wyObjectRelease(m_Label1);
			wyObjectRelease(m_Label2);
			wyObjectRelease(m_Label3);
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts){
			char buf[128];
			int *count = (int*) ts->getData();
			switch(ts->getId()){
				case 0:
					sprintf(buf, "Label1: %.1f, %d", ts->getDelta(), (*count)++);
					m_Label1->setString(buf);
					break;
				case 1:
					sprintf(buf, "Label2: %.1f, %d", ts->getDelta(), (*count)++);
					m_Label2->setString(buf);
					break;
				case 2:
					m_Label3->setString("Label3: Once");
					break;
			}
		}
		
		virtual bool touchesBegan(wyMotionEvent& e) {
			m_Label3->setString("Label3:");
			m_Label3->scheduleLocked(timer3);
			return true;
		}
	};

	class wyScreenshotTestLayer : public wyLayer {
	private:
		wySprite* m_screenshotSprite;
		
	public:
		wyScreenshotTestLayer() : m_screenshotSprite(NULL) {
			// create a sprite
			wySprite* sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.grossini")));
			sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight - DP(100));
			addChildLocked(sprite);
			
			// run action on sprite
			wyIntervalAction* rotate = new wyRotateTo(2, 0, 360);
			wyAction* action = new wyRepeatForever(rotate);
			sprite->runAction(action);
			wyObjectRelease(rotate);
			wyObjectRelease(action);
			
			// create menu for screenshot
			wyLabel* label1 = wyLabel::make("Make Screenshot");
			wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL, (wyTargetSelector*)(new wyTargetSelector(this, 0, NULL))->autoRelease(), label1);
			wyMenu* menu = new wyMenu(item1, NULL);
			menu->alignItemsVertically();
			addChildLocked(menu);
			menu->release();
			item1->release();
			
			// 添加director生命周期监听,我们只需要处理截图事件
			static wyDirectorLifecycleListener dll = {
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				onScreenCaptured
			};
			wyDirector::getInstance()->addLifecycleListener(&dll, this);
		}
		
		virtual ~wyScreenshotTestLayer() {
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts){
			switch(ts->getId()){
				case 0:
					onMakeScreenshot();
					break;
			}
		}
		
		void onMakeScreenshot() {
			// 如果之前的截图预览还在, 删除掉, 这样的话, 就不会被截进去了
			if(m_screenshotSprite != NULL) {
				removeChild(m_screenshotSprite, true);
				m_screenshotSprite = NULL;
			}
			
			// 截屏, 截屏操作是异步的, 可以在onDirectorScreenCaptured中处理截屏成功事件
			// 截屏可以生成png或者jpg格式, 这是根据传入的路径来自动决定的, 如果想存为jpg, 就
			// 改成screenshot.jpg或者screenshot.jpeg即可
			wyDirector::getInstance()->makeScreenshot("/sdcard/WiEngine/screenshot.png");
		}
		
		static void onScreenCaptured(const char* path, void* data) {
			// 之前截屏过的贴图应该已经在缓冲中, 所以先删掉缓冲中的贴图, 由于这个贴图是从文件系统中载入的, 它的路径就是它的key
			wyTextureManager::getInstance()->removeTexture(path);
			
			// 从新的截图中创建贴图对象
			wyTexture2D* tex = wyTexture2D::makeFilePNG(path);
			
			// 创建新的截图sprite, 添加到屏幕下方
			// 这个sprite设置了自动拉伸, 因此看起来是缩小了, 为了看的清楚一些, 这个截图渲染时用红色
			wyScreenshotTestLayer* layer = (wyScreenshotTestLayer*)data;
			layer->m_screenshotSprite = wySprite::make(tex);
			layer->m_screenshotSprite->setContentSize(wyDevice::winWidth * 0.4f, wyDevice::winHeight * 0.4f);
			layer->m_screenshotSprite->setAutoFit(true);
			layer->m_screenshotSprite->setAnchorPercent(0.5f, 0.f);
			layer->m_screenshotSprite->setPosition(wyDevice::winWidth / 2, 0);
			layer->m_screenshotSprite->setColor(wyc3bRed);
			layer->addChildLocked(layer->m_screenshotSprite);
		}
	};

	class wyTextureAtlasTestLayer : public wyLayer{
		wyTextureAtlas* m_Atlas;
	public:
		wyTextureAtlasTestLayer() {
			m_Atlas = (wyTextureAtlas*) wyTextureAtlas::make(wyTexture2D::makeJPG(RES("R.drawable.test_jpg")))->retain();
			
			float y = (wyDevice::winHeight - m_Atlas->getHeight()) / 2;
			float x = (wyDevice::winWidth - m_Atlas->getWidth()) / 2;
			
			// specify texture coordination and vertice
			wyQuad2D texCoords = {
	    		0.0f, m_Atlas->getHeightScale(), m_Atlas->getWidthScale(),
	    		m_Atlas->getHeightScale(), 0.0f, 0.0f, m_Atlas->getWidthScale(), 0.0f
			};
			
			wyQuad3D vertices = {
	            x, y, 0,
	            x + m_Atlas->getWidth(), y, 0,
	            x, y + m_Atlas->getHeight(), 0,
	            x + m_Atlas->getWidth(), y + m_Atlas->getHeight(), 0,
			};
			
			m_Atlas->updateQuad(texCoords, vertices, 0);
			m_Atlas->updateColor(wyc4bWhite, 0);
		}
		
		virtual ~wyTextureAtlasTestLayer(){
			wyObjectRelease(m_Atlas);
		}
		
		virtual void draw() {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			
			m_Atlas->drawAll();
			
			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	};

	class wyTextureLoadUnloadTestLayer : public wyColorLayer {
	private:
		wyLabel* m_hintLabel;
		
		// flag indicating thread is started
		bool m_busying;
		
		// thread callback structure
		wyThreadCallback m_callback;
		
	public:
		wyTextureLoadUnloadTestLayer() :
		wyColorLayer(wyc4b(0, 0, 255, 255)),
		m_busying(false) {
			wyLabel* label1 = wyLabel::make("Push Scene");
			wyLabel* label2 = wyLabel::make("Quit");
			
			wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL, (wyTargetSelector*)(new wyTargetSelector(this, 0, NULL))->autoRelease(), label1);
			wyMenuItemLabel* item2 = new wyMenuItemLabel(NULL, (wyTargetSelector*)(new wyTargetSelector(this, 1, NULL))->autoRelease(), label2);
			wyMenu* menu = new wyMenu(item1, item2, NULL);
			
			menu->alignItemsVertically();
			addChildLocked(menu);
			
			m_hintLabel = wyLabel::make("texture loaded: 0/20", SP(20));
			m_hintLabel->setPosition(wyDevice::winWidth / 2, 100);
			addChildLocked(m_hintLabel);
			
			wyObjectRelease(menu);
			wyObjectRelease(item1);
			wyObjectRelease(item2);
		}
		
		wyTextureLoadUnloadTestLayer(bool newLayer) :
		wyColorLayer(wyc4b(255, 0, 0, 255)),
		m_busying(false) {
			wyLabel* label1 = wyLabel::make("Go Back");
			wyMenuItemLabel* item1 = new wyMenuItemLabel(NULL, (wyTargetSelector*)(new wyTargetSelector(this, 2, NULL))->autoRelease(), label1);
			
			wyMenu* menu = new wyMenu(item1, NULL);
			menu->alignItemsVertically();
			addChildLocked(menu);
			
			m_hintLabel = wyLabel::make("texture unloaded: 0/20", SP(20));
			m_hintLabel->setPosition(wyDevice::winWidth / 2, 100);
			addChildLocked(m_hintLabel);
			
			wyObjectRelease(menu);
			wyObjectRelease(item1);
		};
		
		virtual ~wyTextureLoadUnloadTestLayer() {
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts){
			switch(ts->getId()){
				case 0:
					onPushScene();
					break;
				case 1:
					onExitScene();
					break;
				case 2:
					onPopScene();
					break;
			}
		}
		
		void updateLoadHintLabel(int index) {
			char buf[64];
			sprintf(buf, "texture loaded: %d/20", index);
			m_hintLabel->setString(buf);
		}
		
		void updateUnloadHintLabel(int index) {
			char buf[64];
			sprintf(buf, "texture unloaded: %d/20", index);
			m_hintLabel->setString(buf);
		}
		
		static void loadingThreadEntry(void* arg) {
			wyTextureLoadUnloadTestLayer* layer = (wyTextureLoadUnloadTestLayer*)arg;
			
			// load textures
			wyTexture2D::makePNG(RES("R.drawable.atlastest"))->load();
			layer->updateLoadHintLabel(1);
			wyTexture2D::makePNG(RES("R.drawable.background"))->load();
			layer->updateLoadHintLabel(2);
			wyTexture2D::makePNG(RES("R.drawable.bitmapfont"))->load();
			layer->updateLoadHintLabel(3);
			wyTexture2D::makePNG(RES("R.drawable.powered"))->load();
			layer->updateLoadHintLabel(4);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_01"))->load();
			layer->updateLoadHintLabel(5);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_02"))->load();
			layer->updateLoadHintLabel(6);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_03"))->load();
			layer->updateLoadHintLabel(7);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_04"))->load();
			layer->updateLoadHintLabel(8);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_05"))->load();
			layer->updateLoadHintLabel(9);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_06"))->load();
			layer->updateLoadHintLabel(10);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_07"))->load();
			layer->updateLoadHintLabel(11);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_08"))->load();
			layer->updateLoadHintLabel(12);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_09"))->load();
			layer->updateLoadHintLabel(13);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_10"))->load();
			layer->updateLoadHintLabel(14);
			wyTexture2D::makePNG(RES("R.drawable.ortho_test1"))->load();
			layer->updateLoadHintLabel(15);
			wyTexture2D::makePNG(RES("R.drawable.ortho_test1_bw"))->load();
			layer->updateLoadHintLabel(16);
			wyTexture2D::makeJPG(RES("R.drawable.test_jpg"))->load();
			layer->updateLoadHintLabel(17);
			wyTexture2D::makePNG(RES("R.drawable.tiles"))->load();
			layer->updateLoadHintLabel(18);
			wyTexture2D::makePNG(RES("R.drawable.grossini_dance_atlas"))->load();
			layer->updateLoadHintLabel(19);
			wyTexture2D::makePNG(RES("R.drawable.hexa_tiles"))->load();
			layer->updateLoadHintLabel(20);
			
			// texture are all loaded, now go to next scene
			wyScene* scene = new wyScene();
			scene->addChildLocked(
								  (wyLayer*) (new wyTextureLoadUnloadTestLayer(true))->autoRelease());
			wyDirector::getInstance()->replaceScene(
													(wyTransitionScene*) (new wyFlipXTransition(1.0f, scene, true))->autoRelease());
			wyObjectRelease(scene);
			layer->m_busying = false;
		}
		
		static void unloadingThreadEntry(void* arg) {
			wyTextureLoadUnloadTestLayer* layer = (wyTextureLoadUnloadTestLayer*)arg;
			
			// unload textures
			wyTextureManager* tm = wyTextureManager::getInstance();
			tm->removeTexture(RES("R.drawable.atlastest"));
			layer->updateUnloadHintLabel(1);
			tm->removeTexture(RES("R.drawable.background"));
			layer->updateUnloadHintLabel(2);
			tm->removeTexture(RES("R.drawable.bitmapfont"));
			layer->updateUnloadHintLabel(3);
			tm->removeTexture(RES("R.drawable.powered"));
			layer->updateUnloadHintLabel(4);
			tm->removeTexture(RES("R.drawable.grossini_dance_01"));
			layer->updateUnloadHintLabel(5);
			tm->removeTexture(RES("R.drawable.grossini_dance_02"));
			layer->updateUnloadHintLabel(6);
			tm->removeTexture(RES("R.drawable.grossini_dance_03"));
			layer->updateUnloadHintLabel(7);
			tm->removeTexture(RES("R.drawable.grossini_dance_04"));
			layer->updateUnloadHintLabel(8);
			tm->removeTexture(RES("R.drawable.grossini_dance_05"));
			layer->updateUnloadHintLabel(9);
			tm->removeTexture(RES("R.drawable.grossini_dance_06"));
			layer->updateUnloadHintLabel(10);
			tm->removeTexture(RES("R.drawable.grossini_dance_07"));
			layer->updateUnloadHintLabel(11);
			tm->removeTexture(RES("R.drawable.grossini_dance_08"));
			layer->updateUnloadHintLabel(12);
			tm->removeTexture(RES("R.drawable.grossini_dance_09"));
			layer->updateUnloadHintLabel(13);
			tm->removeTexture(RES("R.drawable.grossini_dance_10"));
			layer->updateUnloadHintLabel(14);
			tm->removeTexture(RES("R.drawable.ortho_test1"));
			layer->updateUnloadHintLabel(15);
			tm->removeTexture(RES("R.drawable.ortho_test1_bw"));
			layer->updateUnloadHintLabel(16);
			tm->removeTexture(RES("R.drawable.test_jpg"));
			layer->updateUnloadHintLabel(17);
			tm->removeTexture(RES("R.drawable.tiles"));
			layer->updateUnloadHintLabel(18);
			tm->removeTexture(RES("R.drawable.grossini_dance_atlas"));
			layer->updateUnloadHintLabel(19);
			tm->removeTexture(RES("R.drawable.hexa_tiles"));
			layer->updateUnloadHintLabel(20);
			
			// texture are all unloaded, now back to first scene
			wyScene* scene = new wyScene();
			scene->addChildLocked(
								  (wyLayer*) (new wyTextureLoadUnloadTestLayer())->autoRelease());
			wyDirector::getInstance()->replaceScene(
													(wyTransitionScene*) (new wyFlipXTransition(1.0f, scene, false))->autoRelease());
			wyObjectRelease(scene);
			layer->m_busying = false;
		}
		
		void onPushScene() {
			if(m_busying)
				return;
			m_busying = true;
			
			// create loading thread
			m_callback.callback_func = loadingThreadEntry;
			m_callback.arg = this;
			wyThread::runThread(&m_callback);
		}
		
		void onExitScene() {
			if(m_busying)
				return;
			
			wyDirector::getInstance()->popScene();
		}
		
		void onPopScene(){
			if(m_busying)
				return;
			m_busying = true;
			
			// create loading thread
			m_callback.callback_func = unloadingThreadEntry;
			m_callback.arg = this;
			wyThread::runThread(&m_callback);
		}
	};

#if ANDROID
	class wyTransparentDrawPrimitivesTestLayer : public wyLayer {
	public:
		wyTransparentDrawPrimitivesTestLayer(){
			wyRotateBy* rotate = new wyRotateBy(4, -360);
			runAction(rotate);
			wyObjectRelease(rotate);
		}
		
		virtual ~wyTransparentDrawPrimitivesTestLayer(){
		}
		
		virtual void draw(){
			// 画简单线段
			// 默认参数: 宽度 1, 颜色 RGB(255,255,255,255)
			// Anti-Aliased
			glEnable(GL_LINE_SMOOTH);
			
			wyDrawLine(0, 0, wyDevice::winWidth, wyDevice::winHeight);
			
			// 关掉反锯齿
			glDisable(GL_LINE_SMOOTH);
			// 设置线段宽度, 宽度大于1时，无法使用GL_LINE_SMOOTH。
			glLineWidth(5.0f);
			// 设置颜色
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			wyDrawLine(0, wyDevice::winHeight, wyDevice::winWidth, 0);
			
			// 记住：OpenGL是一个状态机.如果你不再次设置画图参数，当前属性不会改变。
			
			// 在中心画一个大点（半径为64)
			glPointSize(64);
			glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
			wyDrawPoint(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			
			// 画四个小点（半径为4）
			float points[] = {
				60, 60, 70, 70, 60, 70, 70, 60
			};
			glPointSize(4);
			glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
			wyDrawPoints(points, sizeof(points) / sizeof(float));
			
			// 画一个绿色圆圈，圆圈被分成10段
			glLineWidth(16);
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			wyDrawCircle(wyDevice::winWidth / 2, wyDevice::winHeight / 2, 100, 0, 10, false);
			
			// 画一个青色圆圈，圆圈被分成50段。并且带一条中心到圆周的直线。
			// draw a green circle with 50 segments with line to center
			glLineWidth(2);
			glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
			wyDrawCircle(wyDevice::winWidth / 2, wyDevice::winHeight / 2, 50, 90, 50, true);
			
			// 画一个黄色的多边形
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glLineWidth(10);
			float vertices[] = {
				0, 0, 50, 50, 100, 50, 100, 100, 50, 100
			};
			wyDrawPoly(vertices, sizeof(vertices) / sizeof(float), false);
			
			// 画闭合多边形
			glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
			glLineWidth(2);
			float vertices2[] = {
				30, 130, 30, 230, 50, 200
			};
			wyDrawPoly(vertices2, sizeof(vertices2) / sizeof(float), true);
			
			// 画"二次贝赛尔曲线"
			wyBezierConfig c = wybcQuad(0, wyDevice::winHeight, wyDevice::winWidth / 2, wyDevice::winHeight / 2, wyDevice::winWidth, wyDevice::winHeight);
			wyDrawBezier(c, 50);
			
			// 画"三次贝塞尔曲线"
			c = wybcCubic(wyDevice::winWidth / 2,
						  wyDevice::winHeight / 2,
						  wyDevice::winWidth / 2 + 30,
						  wyDevice::winHeight / 2 + 50,
						  wyDevice::winWidth / 2 + 60,
						  wyDevice::winHeight / 2 - 50,
						  wyDevice::winWidth,
						  wyDevice::winHeight / 2);
			wyDrawBezier(c, 100);
			
			// 重置
			glLineWidth(1);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glPointSize(1);
		}
	};
#endif
	
	class wyVerletRopeTestLayer : public wyLayer {
	private:
		wyVerletRope* m_rope;
		wyPoint m_a;
		wyPoint m_b;
		
	public:
		wyVerletRopeTestLayer() :
		m_a(wyp(wyDevice::winWidth / 2, wyDevice::winHeight - 100)),
		m_b(wyp(50, wyDevice::winHeight / 2)) {
			m_rope = new wyVerletRope(m_a, m_b, wyTexture2D::makePNG(RES("R.drawable.rope")));
			
			setTouchEnabled(true);
			
			wyTargetSelector* ts = new wyTargetSelector(this, 0, NULL);
			wyTimer* t = new wyTimer(ts);
			scheduleLocked(t);
			t->release();
			ts->release();
		}
		
		virtual ~wyVerletRopeTestLayer() {
			m_rope->release();
		}
		
		virtual void draw() {
			wyLayer::draw();
			m_rope->draw();
		}
		
		virtual bool touchesBegan(wyMotionEvent& e) {
			m_b = wyp(e.x[0], e.y[0]);
			return true;
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
			m_rope->update(m_a, m_b, 1.0f / 30.f);
		}
	};

	class wyVideoPlayingTestLayer : public wyLayer {
	public:
		wyVideoPlayingTestLayer() {
			wyMenuItemLabel* item = new wyMenuItemLabel(NULL,
														(wyTargetSelector*)(new wyTargetSelector(this, 1, NULL))->autoRelease(),
														wyLabel::make("Play Video", SP(32), NORMAL));
			
			wyMenu* menu = new wyMenu((wyMenuItem*)item->autoRelease(), NULL);
			menu->alignItemsVertically();
			
			addChildLocked(menu);
			wyObjectRelease(menu);
		}
		
		virtual ~wyVideoPlayingTestLayer() {
			
		}
		
		virtual void onTargetSelectorInvoked(wyTargetSelector* ts) {
			switch(ts->getId()){
				case 1:
					onPlayVideo();
					break;
			}
		}
		
		void onPlayVideo() {
			wyUtils::playVideo(RES("R.raw.test_video"));
		}
	};

#if ANDROID
	class wyXMLLayoutTestLayer : public wyLayer{
	public:
		wyXMLLayoutTestLayer() {
			int id = RES("R.drawable.grossini");
			wySprite* sprite1 = wySprite::make(wyTexture2D::makePNG(id));
			wySprite* sprite2 = wySprite::make(wyTexture2D::makePNG(id));
			wySprite* sprite3 = wySprite::make(wyTexture2D::makePNG(id));
			sprite3->setAnchorPercent(1, 1);
			wySprite* sprite4 = wySprite::make(wyTexture2D::makePNG(id));
			sprite4->setAnchorPercent(0, 1);
			addChildLocked(sprite1);
			addChildLocked(sprite2);
			addChildLocked(sprite3);
			addChildLocked(sprite4);
			
			wyLayoutUtil::loadLayout(RES("R.layout.xml_layout"));
			wyLayoutUtil::bindNode(sprite1, RES("R.id.upcenter"));
			wyLayoutUtil::bindNode(sprite2, RES("R.id.downcenter"));
			wyLayoutUtil::bindNode(sprite3, RES("R.id.upcenter"), LAYOUT_LEFT | LAYOUT_BOTTOM, false);
			wyLayoutUtil::bindNode(sprite4, RES("R.id.upcenter"), LAYOUT_RIGHT | LAYOUT_BOTTOM, false);
			wyLayoutUtil::releaseView();
		}
		
		virtual ~wyXMLLayoutTestLayer() {
		}
	};

#endif

	class wyZwoptexTestLayer : public wyLayer {
		wyTextureAtlas* m_Atlas;
		
	public:
		wyZwoptexTestLayer() {
			wyTexture2D* tex = wyTexture2D::makePNG(RES("R.drawable.grossini_dance_atlas"));
			wyZwoptexManager::getInstance()->addZwoptex("grossini", RES("R.raw.grossini_dance_atlas"), tex);
			
			addNewSprite(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			
			setTouchEnabled(true);
		}
		
		virtual ~wyZwoptexTestLayer() {
		}
		
		void addNewSprite(float x, float y) {
			int idx = abs(rand()) % 14 + 1;
			char buf[128];
			sprintf(buf, "grossini_dance_%02d.png", idx);
			wySprite* sprite = wyZwoptexManager::getInstance()->makeSprite(buf);
			if(sprite != NULL) {
				sprite->setFlipX(rand() % 2);
				sprite->setFlipY(rand() % 2);
				sprite->setRotation(rand() % 360);
				sprite->setPosition(x, y);
				addChildLocked(sprite);
			} else {
				LOGD("Sprite frame not exist");
			}
		}
		
		virtual bool touchesBegan(wyMotionEvent& event) {
			wyPoint loc = wyp(event.x[0], event.y[0]);
			addNewSprite(loc.x, loc.y);
			return true;
		}
	};

}

using namespace Other;
using namespace std;

#if ANDROID
	#define DEMO_ENTRY_IMPL(CLASSNAME) JNIEXPORT void JNICALL Java_com_wiyun_engine_tests_other_##CLASSNAME##_nativeStart \
		(JNIEnv *, jobject){ \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#elif IOS || MACOSX
	#define DEMO_ENTRY_IMPL(CLASSNAME) void _other_##CLASSNAME##Launcher() { \
			wyLayer* layer = new wy##CLASSNAME##Layer(); \
			runDemo(layer, NULL); \
			wyObjectRelease(layer); \
		}
#endif

DEMO_ENTRY_IMPL(AlphaTestTest);
DEMO_ENTRY_IMPL(AStarTest);
DEMO_ENTRY_IMPL(ColorFilterTest);
DEMO_ENTRY_IMPL(DrawPrimitivesTest);
DEMO_ENTRY_IMPL(MemoryFileSystemTest);
DEMO_ENTRY_IMPL(NodePhysicsTest);
DEMO_ENTRY_IMPL(PauseResumeGameTest);
DEMO_ENTRY_IMPL(PickFromAlbumTest);
DEMO_ENTRY_IMPL(PickFromCameraTest);
DEMO_ENTRY_IMPL(PixelOperationTest);
DEMO_ENTRY_IMPL(TextureAtlasTest);
DEMO_ENTRY_IMPL(TextureLoadUnloadTest);
#if ANDROID
	DEMO_ENTRY_IMPL(TransparentDrawPrimitivesTest);
#endif
DEMO_ENTRY_IMPL(SchedulerTest);
DEMO_ENTRY_IMPL(ScreenshotTest);
DEMO_ENTRY_IMPL(VerletRopeTest);
DEMO_ENTRY_IMPL(VideoPlayingTest);
#if ANDROID
	DEMO_ENTRY_IMPL(XMLLayoutTest);
#endif
DEMO_ENTRY_IMPL(ZwoptexTest);
