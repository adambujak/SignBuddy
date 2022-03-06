package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class AlphabetActivity extends AppCompatActivity {

    ListView listViewAlpha;
    String[] alphaLetters;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alphabet);

        Intent i = getIntent();
        int lessonNumber = i.getIntExtra("LessonNumber", -1) + 1;
        String st = String.valueOf(lessonNumber);
        ((TextView)findViewById(R.id.textView3)).setText("You have selected Lesson "+ st);

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
        listViewAlpha.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                if (position == 0) {
                } else {
                }
                Intent i = new Intent(AlphabetActivity.this, LessonActivity.class);
                Bundle mBundle = new Bundle();
                mBundle.putString("LessonNumber", st);
                mBundle.putInt("AlphaNum", position+1);
                i.putExtras(mBundle);
                startActivity(i);
            }
        });
    }
}