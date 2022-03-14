package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

public class AlphabetActivity extends AppCompatActivity {

    ListView listViewAlpha;
    private Button continueButton;
    String[] alphaLetters;
    //String[] LessonStatus;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alphabet);

        Intent i = getIntent();
        int lessonNumber = i.getIntExtra("LessonNumber", -1) + 1;
        String st = String.valueOf(lessonNumber);
        ((TextView)findViewById(R.id.textView3)).setText("You have selected Lesson "+ st + ". This lesson contains the following letters:");

        int alpha = i.getIntExtra("alpha", -1) + 1;
        switch(lessonNumber){
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
        continueButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                Intent i = new Intent(AlphabetActivity.this, LessonActivity.class);
                Bundle mbundle = new Bundle();
                mbundle.putInt("alpha", alpha);
                mbundle.putString("LessonNumber", st);
                i.putExtras(mbundle);
                startActivity(i);
            }
        });
    }
}