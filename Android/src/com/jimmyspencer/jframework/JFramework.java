package com.jimmyspencer.jframework;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class JFramework extends Activity 
{
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jframework);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) 
    {
        getMenuInflater().inflate(R.menu.activity_jframework, menu);
        return true;
    }
}
