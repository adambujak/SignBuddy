package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private Button buttonLesson;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonLesson = (Button) findViewById(R.id.buttonLesson);

        buttonLesson.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openLessonActivity();
            }
        });

    }

    public void openLessonActivity(){
        Intent intent = new Intent(this, LessonActivity.class);
        startActivity(intent);
    }

    public void openQuizMenu(View v){
        Intent intent = new Intent(this, QuizActivity.class);
        startActivity(intent);

    }
    public void launchBLE(View v){
        Context context = getApplicationContext();
        startNewActivity(context,"com.adafruit.bluefruit.le.connect");
    }

    public void startNewActivity(Context context, String packageName) {
        Intent intent = context.getPackageManager().getLaunchIntentForPackage(packageName);
        if (intent == null) {
            // Bring user to the market or let them choose an app?
            intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse("market://details?id=" + packageName));
        }
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }
}


