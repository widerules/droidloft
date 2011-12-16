/*
 * Copyright (c) 2010 WiYun Inc.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package com.wiyun.engine.tests.node;

import android.os.SystemClock;
import android.view.MotionEvent;

import com.wiyun.engine.R;
import com.wiyun.engine.WiEngineTestActivity;
import com.wiyun.engine.nodes.Director;
import com.wiyun.engine.nodes.Layer;
import com.wiyun.engine.nodes.Sprite;
import com.wiyun.engine.opengl.Texture2D;
import com.wiyun.engine.types.WYPoint;
import com.wiyun.engine.types.WYSize;

public class PVRSpriteTest extends WiEngineTestActivity {
	@Override
	protected Layer createLayer() {
		return new MyLayer();
	}

    class MyLayer extends Layer {
    	int[] IDS = {
    		R.raw.grossini_rgba_8888,	
    		R.raw.grossini_rgba_5551,
    		R.raw.grossini_rgba_4444,
    		R.raw.grossini_rgb_565,
    		R.raw.grossini_i_8,	
    		R.raw.grossini_a_8,	
    		R.raw.grossini_ai_8	
    	};
    	
        public MyLayer() {
            float x, y;
            WYSize s = Director.getInstance().getWindowSize();
            x = s.width;
            y = s.height;
            addNewSprite(WYPoint.make(x / 2, y / 2));
            
            // enable touch
            setTouchEnabled(true);
        }
        
        private void addNewSprite(WYPoint pos) {
        	int index = (int)SystemClock.uptimeMillis() % IDS.length;
        	Sprite sprite = Sprite.make(Texture2D.makePVR(IDS[index]));
            addChild(sprite);
            sprite.setPosition(pos.x, pos.y);
            sprite.autoRelease();
        }
        
        @Override
        public boolean wyTouchesBegan(MotionEvent event) {
            WYPoint location = Director.getInstance().convertToGL(event.getX(), event.getY());

            addNewSprite(location);

            return true;
        }
    }
}
