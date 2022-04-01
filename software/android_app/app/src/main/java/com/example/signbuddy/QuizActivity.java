package com.example.signbuddy;

import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;


public class QuizActivity extends AppCompatActivity {
    private static final String FILE_NAME = "lessonData.txt";
    private Button correctButton;
    private Button incorrectButton;
    private ProgressBar pgsBar;
    private TextView countText;
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
        String quizNumber = i.getStringExtra("QuizNumber");
        lessonnum = Integer.valueOf(quizNumber);
        alpha = i.getIntExtra("alpha", 0);

        //set Progress Bar
        pgsBar = (ProgressBar) findViewById(R.id.progressBar);
        pgsBar.setProgress(alpha);

        //setting current letter
        if (alpha ==5){
            finish();
            Toast.makeText(QuizActivity.this, "You have completed Quiz " + lessonnum, Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(QuizActivity.this, QuizMenuActivity.class);
            startActivity(intent);

        }

        //setting current image
        alphanum  = (lessonnum-1)*5+alpha;
        String alphaNumber = String.valueOf(alphanum);
        String fnm = "alpha_"+alphaNumber; //  image file name
        String PACKAGE_NAME = getApplicationContext().getPackageName();
        int imgId = getResources().getIdentifier(PACKAGE_NAME+":drawable/"+fnm , null, null);
        image.setImageResource(imgId);

        //start countdown
        new CountDownTimer(7500, 1500) {
            public void onTick(long millisUntilFinished) {
                countText = findViewById(R.id.countText);
                countText.setText("Perform Sign in: " + ((millisUntilFinished / 1500)+1));
            }
            public void onFinish() {
                countText.setText("GO!");
            }
        }.start();

        correctButton = findViewById(R.id.correctButton);
        correctButton.setOnClickListener(new View.OnClickListener() {
                                             public void onClick(View v) {
                                                 Toast.makeText(QuizActivity.this, "Correct!", Toast.LENGTH_SHORT).show();
                                                 alpha++;
                                                 finish();
                                                 i.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                                                 Bundle mbundle = new Bundle();
                                                 mbundle.putInt("alpha", alpha);
                                                 mbundle.putString("LessonNumber", quizNumber);
                                                 i.putExtras(mbundle);

                                                 startActivity(i);
                                                 overridePendingTransition(0,0);
                                             }
                                         }
        );

        incorrectButton = findViewById(R.id.incorrectButton);
        incorrectButton.setOnClickListener(new View.OnClickListener() {
                                             public void onClick(View v) {
                                                 Toast.makeText(QuizActivity.this, "Incorrect", Toast.LENGTH_SHORT).show();
                                                 alpha++;
                                                 finish();
                                                 i.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                                                 Bundle mbundle = new Bundle();
                                                 mbundle.putInt("alpha", alpha);
                                                 mbundle.putString("LessonNumber", quizNumber);
                                                 i.putExtras(mbundle);

                                                 startActivity(i);
                                                 overridePendingTransition(0,0);
                                             }
                                         }
        );
    }


}