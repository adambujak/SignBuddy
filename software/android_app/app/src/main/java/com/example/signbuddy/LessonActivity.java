package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

public class LessonActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alesson);
        ImageView image = (ImageView) findViewById(R.id.signImage);


        Intent i = getIntent();
        String lessonNumber = i.getStringExtra("LessonNumber");
        int alphanum = i.getIntExtra("AlphaNum", -1);
        String alphaNumber = String.valueOf(alphanum);

        String fnm = "alpha_0"+lessonNumber+"_0"+alphaNumber; //  this is image file name
        String PACKAGE_NAME = getApplicationContext().getPackageName();
        int imgId = getResources().getIdentifier(PACKAGE_NAME+":drawable/"+fnm , null, null);
        image.setImageResource(imgId);
    }
}