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
<<<<<<< Updated upstream
        int lessonnum = Integer.valueOf(lessonNumber);
        int alphanum = i.getIntExtra("AlphaNum", -1);
        alphanum  = (lessonnum-1)*5+alphanum;
=======
        lessonnum = Integer.valueOf(lessonNumber);
        alpha = i.getIntExtra("alpha", 0);

        //set Progress Bar
        pgsBar = (ProgressBar) findViewById(R.id.progressBar);
        pgsBar.setProgress(alpha);

        //setting current letter
        if (alpha ==5){
            finish();
            Toast.makeText(LessonActivity.this, "You have completed Lesson " + lessonnum, Toast.LENGTH_LONG).show();
            Intent intent = new Intent(LessonActivity.this, LessonMenuActivity.class);
            startActivity(intent);

        }
        //start countdown
        startCountdown();

        //setting current image
        alphanum  = (lessonnum-1)*5+alpha;
>>>>>>> Stashed changes
        String alphaNumber = String.valueOf(alphanum);

        String fnm = "alpha_"+alphaNumber; //  this is image file name
        String PACKAGE_NAME = getApplicationContext().getPackageName();
        int imgId = getResources().getIdentifier(PACKAGE_NAME+":drawable/"+fnm , null, null);
        image.setImageResource(imgId);
<<<<<<< Updated upstream
    }
=======



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
                                                 //restart countdown
                                                 startCountdown();
                                             }
                                         }
        );
    }

    //start countdown
    public void startCountdown(){
        new CountDownTimer(7500, 1500) {
            public void onTick(long millisUntilFinished) {
                countText = findViewById(R.id.countText);
                countText.setText("Perform Sign in: " + ((millisUntilFinished / 1500) + 1));
            }

            public void onFinish() {
                countText.setText("GO!");
            }
        }.start();
    }


>>>>>>> Stashed changes
}