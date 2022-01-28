package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button buttonLesson = (Button) findViewById(R.id.buttonLesson);
        buttonLesson.setOnClickListener(v -> openLessonActivity());

        Button buttonConnect = (Button) findViewById(R.id.buttonConnect);
        buttonConnect.setOnClickListener(this::openBLEMenu);

    }

    public void openLessonActivity() {
        Intent intent = new Intent(this, LessonActivity.class);
        startActivity(intent);
    }

    public void openQuizMenu(View v) {
        Intent intent = new Intent(this, QuizActivity.class);
        startActivity(intent);

    }

    public void openBLEMenu(View v) {
        Intent intent = new Intent(this, BleActivity.class);
        startActivity(intent);
    }
}
