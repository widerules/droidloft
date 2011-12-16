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
package com.wiyun.engine.tests.other;

import android.view.Gravity;

import com.wiyun.engine.R;
import com.wiyun.engine.WiEngineTestActivity;
import com.wiyun.engine.nodes.Layer;
import com.wiyun.engine.nodes.Sprite;
import com.wiyun.engine.utils.LayoutUtil;

public class XMLLayoutTest extends WiEngineTestActivity {
	@Override
	protected Layer createLayer() {
		return new MyLayer();
	}

    class MyLayer extends Layer {
        public MyLayer() {
        	Sprite sprite1 = Sprite.make(R.drawable.grossini);
        	Sprite sprite2 = Sprite.make(R.drawable.grossini);
        	Sprite sprite3 = Sprite.make(R.drawable.grossini);
        	sprite3.setAnchorPercent(1, 1);
        	Sprite sprite4 = Sprite.make(R.drawable.grossini);
        	sprite4.setAnchorPercent(0, 1);
        	addChild(sprite1);
        	addChild(sprite2);
        	addChild(sprite3);
        	addChild(sprite4);
        	
        	LayoutUtil.loadLayout(R.layout.xml_layout);
        	LayoutUtil.bindNode(sprite1, R.id.upcenter);
        	LayoutUtil.bindNode(sprite2, R.id.downcenter);
        	LayoutUtil.bindNode(sprite3, R.id.upcenter, Gravity.LEFT | Gravity.BOTTOM, false);
        	LayoutUtil.bindNode(sprite4, R.id.upcenter, Gravity.RIGHT | Gravity.BOTTOM, false);
        	LayoutUtil.releaseView();
        }
    }
}
