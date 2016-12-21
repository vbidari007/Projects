package com.mad.project.team3.places_near_me;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Login_Page extends AppCompatActivity {

    EditText uname;
    EditText pwd;
   // Button signin;
    //Button changepwd;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login__page);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();

            }
        });
    }

    public void changepwd(View view)
    {
        Intent i = new Intent(Login_Page.this, Changepassword_screen.class);
        startActivity(i);
    }

    public void map_module(View view){

        // this gets called after signin button is pressed.
        uname=(EditText)findViewById(R.id.editText12);
        pwd=(EditText)findViewById(R.id.editText13);
        String username  = uname.getText().toString();
        String password=pwd.getText().toString();

        if(null == username || username.length() == 0)
        {
            Toast.makeText(this,"Please Enter UserName Properly",Toast.LENGTH_LONG).show();

        }
        else if(null == password || password.length() == 0)
        {
            Toast.makeText(this,"Please Enter Password Properly",Toast.LENGTH_LONG).show();

        }
        else if (username=="admin" && password=="admin")
        {
            Intent i = new Intent(Login_Page.this, Admin_Console.class);
            startActivity(i);
            // intent to admin console
        }


        // db validation and then intent to map module






        /*Intent i = new Intent(Login_Page.this, Home_Activity.class);
        startActivity(i);*/
    }
    }


