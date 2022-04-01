package com.example.signbuddy;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class QuizAlphabetActivity extends AppCompatActivity {

    ListView listViewAlpha;
    private Button continueButton;
    String[] alphaLetters;
    //String[] LessonStatus;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alphabet);

        Intent i = getIntent();
        int quizNumber = i.getIntExtra("QuizNumber", -1) + 1;
        String st = String.valueOf(quizNumber);
        ((TextView)findViewById(R.id.textView3)).setText("You have selected Quiz "+ st + ". This quiz contains the following letters:");


        int alpha = i.getIntExtra("alpha", -1) + 1;
        switch(quizNumber){
            case 1:
                alphaLetters = new String[]{"A", "B", "C", "D", "E"};
                break;
            case 2:
                alphaLetters = new String[]{"F", "G", "H", "I", "J"};
                break;
            case 3:
                alphaLetters = new String[]{"K", "L", "M", "N", "O"};
                break;
            case 4:
                alphaLetters = new String[]{"P", "Q", "R", "S", "T"};
                break;
            case 5:
                alphaLetters = new String[]{"U", "V", "W", "X", "Y", "Z"};
                break;
        }

        listViewAlpha = findViewById(R.id.listViewAlpha);

        //array adapter
        //populate data in listView
        //takes context of R.layout, layout/arrangement of data, data to be displayed
        ArrayAdapter adapterAlpha = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, alphaLetters);

        listViewAlpha.setAdapter(adapterAlpha); //link adapter to listView

        //event handler for clickEvents
        continueButton = findViewById(R.id.continueButton);
        continueButton.setText("Continue to Quiz");
        continueButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {

                Intent i = new Intent(QuizAlphabetActivity.this, QuizActivity.class);
                Bundle mbundle = new Bundle();
                mbundle.putInt("alpha", alpha);
                mbundle.putString("QuizNumber", st);
                i.putExtras(mbundle);
                startActivity(i);
            }
        });
    }
}