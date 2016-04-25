//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE

//this file did not use , only a test
package com.example.aicrobo.teamexplorer;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by aicrobo on 15-11-15.
 */
public class RoomListAdapter extends SimpleAdapter {

    public class ViewHolder
    {
        ImageView image ;
        TextView class_name ;
        TextView class_people_num ;
    }

    private int[] current_class_type ;
    private LayoutInflater mInflater;

    public RoomListAdapter(Context context, List<HashMap<String,Object>> items, int resource, String[] from, int[] to , int[] current_people_num) {
        super(context,items,resource,from,to);
        current_class_type = current_people_num  ;
        this.mInflater = LayoutInflater.from(context);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder ;
        if(convertView==null)
        {
            convertView = mInflater.inflate(R.layout.list_item_layout , null) ;
            holder = new ViewHolder() ;
            holder.image = (ImageView)convertView.findViewById(R.id.view_classroom) ;
            holder.class_name = (TextView)convertView.findViewById(R.id.text_classroom_name) ;
            holder.class_people_num = (TextView)convertView.findViewById(R.id.text_classroom_num) ;
            convertView.setTag(holder);
        }
        else
        {
            holder = (ViewHolder)convertView.getTag() ;
        }
        if(current_class_type[position]<10)
        {
            holder.class_people_num.setTextColor(R.color.green);
        }
        else if(current_class_type[position]<20)
        {
            holder.class_people_num.setTextColor(R.color.yellow);
        }
        else
        {
            holder.class_people_num.setTextColor(R.color.red);
        }

        return convertView ;
    }
    }

