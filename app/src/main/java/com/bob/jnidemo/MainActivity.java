package com.bob.jnidemo;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("mylib");
    }

    private ListView lv_content;
    private ArrayList<String> files;
    private TextView tv_time;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lv_content = (ListView) findViewById(R.id.lv_content);

        tv_time = (TextView) findViewById(R.id.tv_time);

        new AsyncTask<Void,Void,ArrayList>(){

            private long start_time;

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                start_time = new Date().getTime();
            }

            @Override
            protected ArrayList doInBackground(Void... params) {
                return getFiles(Environment.getExternalStorageDirectory().getPath());
            }

            @Override
            protected void onPostExecute(ArrayList arrayList) {
                if (arrayList!=null&&arrayList.size()>0){
                    tv_time.setText((new Date().getTime() - start_time) +"");
                    files = arrayList;
                    lv_content.setAdapter(new SearchAdapter());
                }
            }
        }.execute();
    }


    class SearchAdapter extends BaseAdapter{
        @Override
        public int getCount() {
            return files.size();
        }

        @Override
        public Object getItem(int position) {
            return files.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            ViewHoler holer;
            if (convertView == null){
                convertView = View.inflate(getApplicationContext(), R.layout.file_item_layout, null);
                holer = new ViewHoler();
                holer.tv_file_path = (TextView) convertView.findViewById(R.id.tv_file_path);
                convertView.setTag(holer);
            }

            holer = (ViewHoler) convertView.getTag();

            holer.tv_file_path.setText(files.get(position));
            return convertView;
        }
    }

    class ViewHoler{
        TextView tv_file_path;
    }

    public native void scanDir(String dirPath);

    public native String getStringFromC();

    public native int add(int x, int y);

    public native String getString(String str);

    public native ArrayList<String> getFiles(String path);


    public native String getStringFromJni();

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_scrolling, menu);
        return true;
    }
}
