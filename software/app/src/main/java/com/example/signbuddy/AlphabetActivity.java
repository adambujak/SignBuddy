package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class AlphabetActivity extends AppCompatActivity {

    ListView listViewAlpha;

    //data for list
    String[] alphaLetters= {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
            "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alphabet);

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
                if(position == 0) {
                    //clicked A
                    //go from alphabet activity to A Lesson activity
                    startActivity(new Intent(AlphabetActivity.this, ALessonActivity.class));
                } else {
                }
            }
        });
    }
}