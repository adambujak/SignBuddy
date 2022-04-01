package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;


public class QuizMenuActivity extends AppCompatActivity {
    ListView listView;

    //data for list
    String[] categoryTypes = {"Quiz 1", "Quiz 2", "Quiz 3", "Quiz 4", "Quiz 5"};


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lesson);

        listView = findViewById(R.id.listView);

        //array adapter
        //populate data from array into listView
        //takes context of R.layout, layout/arrangement of data, data to be displayed
        ArrayAdapter adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, categoryTypes);

        listView.setAdapter(adapter); //link adapter to listView

        //event handler for clickEvents
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                //go from main activity to alphabet activity
                Intent i = new Intent(QuizMenuActivity.this, QuizAlphabetActivity.class);
                Bundle mbundle = new Bundle();
                mbundle.putInt("QuizNumber", position);
                mbundle.putInt("alpha", 0);
                i.putExtras(mbundle);
                startActivity(i);
            }
        });
    }
}

