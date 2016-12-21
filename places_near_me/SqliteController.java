
package com.mad.project.team3.places_near_me;

import android.util.Log;
import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class SqliteController extends SQLiteOpenHelper {
    private static final String LOGCAT = null;

    public SqliteController(Context applicationcontext) {
        super(applicationcontext, "signup.db", null, 1);
        Log.d(LOGCAT, "Created");
    }

    @Override
    public void onCreate(SQLiteDatabase database) {
        String query;
        query = "CREATE TABLE user_details ( firstname TEXT, lastname TEXT,dob TEXT,country TEXT,sex TEXT,email TEXT,uname TEXT,password TEXT)";
        database.execSQL(query);
        Log.d(LOGCAT, "Table Created");
    }

    @Override
    public void onUpgrade(SQLiteDatabase database, int version_old, int current_version) {
        String query;
        query = "DROP TABLE IF EXISTS user_details";
        database.execSQL(query);
        onCreate(database);
    }

    public void insertuser(String fname,String lname,String dob,String country,String Sex ,String email, String uname,String password) {
        SQLiteDatabase database = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put("firstname",fname);
        values.put("lastname", lname);
        values.put("dob", dob);
        values.put("country", country);
        values.put("sex", Sex);
        values.put("email",email);
        values.put("uname", uname);
        values.put("password", password);
        database.insert("user_details", null, values);
        database.close();

        Log.d(LOGCAT, "insertion completed");


    }
    public void cleardb()
    {   SQLiteDatabase database = this.getWritableDatabase();
        String query="delete from user_details";
        database.execSQL(query);


    }


}

