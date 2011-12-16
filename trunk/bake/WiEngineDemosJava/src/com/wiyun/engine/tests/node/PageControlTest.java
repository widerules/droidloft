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

import android.os.Build;

import com.wiyun.engine.R;
import com.wiyun.engine.WiEngineTestActivity;
import com.wiyun.engine.nodes.Director;
import com.wiyun.engine.nodes.DotPageIndicator;
import com.wiyun.engine.nodes.Label;
import com.wiyun.engine.nodes.Layer;
import com.wiyun.engine.nodes.PageControl;
import com.wiyun.engine.nodes.Sprite;
import com.wiyun.engine.nodes.PageControl.IPageControlCallback;
import com.wiyun.engine.opengl.Texture2D;
import com.wiyun.engine.types.WYSize;
import com.wiyun.engine.utils.ResolutionIndependent;

public class PageControlTest extends WiEngineTestActivity {
	@Override
	protected Layer createLayer() {
		return new MyLayer();
	}
	
    @Override
    protected String checkPrecondition() {
    	return Build.VERSION.SDK_INT > 4 ? null : "This demo requires Android 2.0 or higher version";
    }

    class MyLayer extends Layer implements IPageControlCallback {
    	Label m_hint;
    	
    	public MyLayer() {
    		// create pages
    		Sprite page1 = Sprite.make(Texture2D.makePNG(R.drawable.powered));
    		Sprite page2 = Sprite.make(Texture2D.makePNG(R.drawable.atlastest));
    		Sprite page3 = Sprite.make(Texture2D.makeJPG(R.drawable.test_jpg));

    		// page control
    		PageControl pageControl = PageControl.make();
    		pageControl.setPageSpacing(DP(25));
    		pageControl.addPage(page1);
    		pageControl.addPage(page2);
    		pageControl.addPage(page3);
    		pageControl.setInitialPage(1);
    		pageControl.setCallback(this);
    		// 可以设置为垂直排列
//    		pageControl.setVertical(true);
    		addChild(pageControl);

    		// set page indicator
    		WYSize s = Director.getInstance().getWindowSize();
    		DotPageIndicator indicator = DotPageIndicator.make(
    				Texture2D.makePNG(R.drawable.thumb),
    				Texture2D.makePNG(R.drawable.joystick_rocker));
    		indicator.setPosition(s.width / 2, s.height - DP(60));
    		pageControl.setPageIndicator(indicator);
    		
    		// create a hint label
    		m_hint = Label.make("try scroll and click");
    		m_hint.setPosition(s.width / 2, 100);
    		addChild(m_hint);
		}
    	
    	private float DP(float v) {
    		return ResolutionIndependent.resolveDp(v);
    	}

		public void onPageChanged(int pageControlPointer, int index) {
			m_hint.setText("page changed: " + index);
		}

		public void onPageClicked(int pageControlPointer, int index) {
			m_hint.setText("page clicked: " + index);
		}
    }
}
