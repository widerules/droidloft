package com.wiyun.engine;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.media.AudioManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.wiyun.engine.nodes.Director;
import com.wiyun.engine.nodes.Director.IDirectorLifecycleListener;
import com.wiyun.engine.opengl.WYGLSurfaceView;

public abstract class WiEngineTestActivity extends Activity implements IDirectorLifecycleListener {
	protected WYGLSurfaceView mGLView;
	
	// true表示这个demo已经被启动了
	private boolean mStarted;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        /*
         * 可以指定是否使用透明的surface view, 除此之外相应的activity必须要设置android:theme为
         * @android:style/Theme.Translucent. 如果希望通过xml指定是否透明, 则需要在attrs.xml中添加
         * transparent属性, 如下:
         * <declare-styleable name="com.wiyun.engine.opengl.WYGLSurfaceView">
         * 		<attr name="transparent" format="boolean" />
         * </declare-styleable>
         * 
         * 使用透明surface view可能带来兼容性问题, 有些机型可能对此支持不好.
         * 
         * 该功能目前仅可在Android上使用, 因此不具备可移植性
         */
        mGLView = (WYGLSurfaceView)createContentView();
        
        // add lifecycle listener
        Director.getInstance().addLifecycleListener(this);
        
        /*
         * 取消下面两行代码的注释可以打开基础大小适配模式。关于适配模式的介绍请
         * 查看"WiEngine 3.x综述"中的"屏幕适配"一节。 
         */
//        Director.getInstance().setScaleMode(Director.SCALE_MODE_BASE_SIZE_FIT_XY);
//        Director.getInstance().setBaseSize(320, 480);
        
        // demo controls music stream
        setVolumeControlStream(AudioManager.STREAM_MUSIC);
    }
    
    protected View createContentView() {
    	WYGLSurfaceView v = new WYGLSurfaceView(this, isTransparent());
        setContentView(v);
        return v;
    }

	protected abstract void runDemo();
	
	/**
	 * 检查这个demo是否需要一些前提条件
	 * 
	 * @return 如果前提条件满足, 返回null, 如果不满足, 返回一个错误信息
	 */
	protected String checkPrecondition() {
		return null;
	}
	
	protected boolean isTransparent() {
		return false;
	}
	
	@Override
    public void onPause() {
        super.onPause();

        Director.getInstance().pause();
    }

    @Override
    public void onResume() {
        super.onResume();

        Director.getInstance().resume();
    }

    @Override
    public void onDestroy() {
    	Director.getInstance().end();
    	
        super.onDestroy();
    }
    
    public void onDirectorPaused() {
    }
    
    public void onDirectorResumed() {
    }
    
    public void onDirectorEnded() {
    }
    
    public void onDirectorScreenCaptured(String path) {
    }
    
    public void onSurfaceChanged(int w, int h) {
    	/*
    	 * 把runDemo放在这里, 可以避免一些屏幕尺寸问题. 因为一开始WiEngine并不知道
    	 * SurfaceView的尺寸, 所以是假设为全屏大小. 在之后的onSurfaceChanged事件中, 
    	 * 正确的大小会被设置. 但是往往在onSurfaceChanged之前, 场景就已经被创建了, 
    	 * 所以如果因为某种原因不是全屏大小, 那么之前创建的场景位置就有误了.
    	 * 
    	 * 由于onSurfaceChanged有可能被调用多次, 用一个标志来防止多次运行.
    	 */
    	if(!mStarted) {
    		mStarted = true;
    		runDemo();
    	}
    }
    
    public void onSurfaceCreated() {
    	final String error = checkPrecondition();
    	if(!TextUtils.isEmpty(error)) {
    		runOnUiThread(new Runnable() {
	            public void run() {
	            	AlertDialog.Builder builder = new Builder(WiEngineTestActivity.this);
	            	builder.setMessage(error)
	            	.setNegativeButton("OK", new OnClickListener() {
	            		public void onClick(DialogInterface dialog, int which) {
	            			finish();
	            		}
	            	}).show();
	            }
            });
    	}
    }
    
    public void onSurfaceDestroyed() {
    }
}
