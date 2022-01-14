package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

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

}