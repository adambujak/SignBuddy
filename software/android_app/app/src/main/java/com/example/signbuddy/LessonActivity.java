package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Toast;


public class LessonActivity extends AppCompatActivity {
    private static final String FILE_NAME = "lessonData.txt";
    private Button correctButton;
    private Button incorrectButton;
    private ProgressBar pgsBar;
    int alpha;
    int lessonnum;
    int alphanum;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alesson);
        ImageView image = (ImageView) findViewById(R.id.signImage);

        //setting gesture image
        Intent i = getIntent();
        String lessonNumber = i.getStringExtra("LessonNumber");
        lessonnum = Integer.valueOf(lessonNumber);
        alpha = i.getIntExtra("alpha", 0);

        //set Progress Bar
        pgsBar = (ProgressBar) findViewById(R.id.progressBar);
        pgsBar.setProgress(alpha);

        //setting current letter
        if (alpha ==5){
            finish();
            Toast.makeText(LessonActivity.this, "You have completed Lesson " + lessonnum, Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(LessonActivity.this, LessonMenuActivity.class);
            startActivity(intent);

        }

        alphanum  = (lessonnum-1)*5+alpha;
        String alphaNumber = String.valueOf(alphanum);
        String fnm = "alpha_"+alphaNumber; //  image file name
        String PACKAGE_NAME = getApplicationContext().getPackageName();
        int imgId = getResources().getIdentifier(PACKAGE_NAME+":drawable/"+fnm , null, null);
        image.setImageResource(imgId);



        correctButton = findViewById(R.id.correctButton);
        correctButton.setOnClickListener(new View.OnClickListener() {
                                             public void onClick(View v) {
                                                 Toast.makeText(LessonActivity.this, "Correct!", Toast.LENGTH_SHORT).show();
                                                 alpha++;
                                                 finish();
                                                 i.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                                                 Bundle mbundle = new Bundle();
                                                 mbundle.putInt("alpha", alpha);
                                                 mbundle.putString("LessonNumber", lessonNumber);
                                                 i.putExtras(mbundle);

                                                 startActivity(i);
                                                 overridePendingTransition(0,0);
                                             }
                                         }
        );

        incorrectButton = findViewById(R.id.incorrectButton);
        incorrectButton.setOnClickListener(new View.OnClickListener() {
                                             public void onClick(View v) {
                                                 Toast.makeText(LessonActivity.this, "Incorrect, Please try again!", Toast.LENGTH_SHORT).show();
                                             }
                                         }
        );
    }


}