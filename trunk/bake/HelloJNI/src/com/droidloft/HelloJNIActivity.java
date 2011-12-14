package com.droidloft;

import android.app.Activity;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

public class HelloJNIActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        LinearLayout layout = (LinearLayout)findViewById(R.id.mainLayout);
        TextView textView = new TextView(this);
        layout.addView(textView);
        JNI hi=new JNI();
        textView.setText(hi.sayHello()); 
    }
    
    static {
        System.loadLibrary("hijni");
    } 
}
