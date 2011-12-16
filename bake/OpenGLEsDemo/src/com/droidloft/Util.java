package com.droidloft;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;

public class Util extends Activity {
	private Util(){
		
	}
	
	public static FloatBuffer convertToFloatBuffer(float[] data){
		ByteBuffer bb=ByteBuffer.allocateDirect(data.length*4);
		bb.order(ByteOrder.nativeOrder());
		FloatBuffer fb=bb.asFloatBuffer();
		fb.put(data);
		fb.position(0);
		return fb;
	}
	
	public static ShortBuffer convertToShortBuffer(short[] data){
		ByteBuffer bb=ByteBuffer.allocateDirect(data.length*2);
		bb.order(ByteOrder.nativeOrder());
		ShortBuffer sb=bb.asShortBuffer();
		sb.put(data);
		sb.position(0);
		return sb;
	}
	
	
	public static int pow2(int size) {
		int small = (int) (Math.log((double) size) / Math.log(2.0f));
		if ((1 << small) >= size)
			return 1 << small;
		else
			return 1 << (small + 1);
	}
	/*
	 * Texture use s sequence
	 */
	public static Bitmap createTextureBitmapFromFilePath(Activity atvt,String path,FloatBuffer texBuf2D) {
		
		int mWidth;
		int mHeight;
//		float _maxS;
//		float _maxT;
		float maxU = 1.0f;
		float maxV = 1.0f;
		Bitmap bmp=null;

		int mPow2Width;
		int mPow2Height;
		try {
			bmp = BitmapFactory
					.decodeStream(atvt.getAssets().open(path));
			//return bmp;
		} catch (IOException e) {
			e.printStackTrace();
			//return null;
		}
		
		if(bmp==null)return null;
		mWidth = bmp.getWidth();
		mHeight = bmp.getHeight();

		mPow2Height = pow2(mHeight);
		mPow2Width = pow2(mWidth);
		
		Bitmap bitmap = Bitmap.createBitmap(mPow2Width, mPow2Height, bmp
				.hasAlpha() ? Bitmap.Config.ARGB_8888 : Bitmap.Config.RGB_565);
		Canvas canvas = new Canvas(bitmap);
		canvas.drawBitmap(bmp, 0, 0, null);
		bmp.recycle();
		
		maxU = mWidth / (float) mPow2Width;
		maxV = mHeight / (float) mPow2Height;
		
		float coordinates[] = {// s sequence
				0, 0, 
				maxU, 0, 
				0, maxV, 
				maxU, maxV
				};

		texBuf2D.put(coordinates);
		texBuf2D.position(0);

	//	maxU = mWidth / (float) mPow2Width;
	//	maxV = mHeight / (float) mPow2Height;
		
		return bitmap;
	}
	
    
}