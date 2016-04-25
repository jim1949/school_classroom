//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE

package com.example.aicrobo.teamexplorer;


import android.os.Bundle;
import android.support.v4.app.Fragment ;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import com.jeremyfeinstein.slidingmenu.lib.app.SlidingFragmentActivity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Objects;


/**
 * A simple {@link Fragment} subclass.
 */

public class LeftMenuFragment extends Fragment {

    private int current_list_item_id_  = 0 ;
    private String[] building_name = {
            "教西A",
            "教西B",
            "教西C",
            "教西D"
    } ;
    public interface currentBuildingChange
    {
        void changeCurrentClassRoom(long current_build_id) ;
    }

    private ListView building_list_view ;
    private SimpleAdapter simpleAdapter ;

    public LeftMenuFragment() {
        // Required empty public constructor
    }

    public int getCurrentListItemId()
    {
        return current_list_item_id_ ;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View fragment_view = inflater.inflate(R.layout.fragment_left_menu, container, false);
        building_list_view = (ListView)fragment_view.findViewById(R.id.listView) ;
        simpleAdapter = new SimpleAdapter(getActivity(),
                getListData(),
                R.layout.left_list_item_layout,
                new String[]{"building_name","building_pic"},
                new int[]{R.id.textview_building,R.id.imageview_buiding}) ;
        building_list_view.setAdapter(simpleAdapter);
        building_list_view.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
               if (getActivity() instanceof currentBuildingChange)
               {
                   current_list_item_id_ = (int)id ;
                   changeCurrentClassRoomFragment(current_list_item_id_) ;
               }
            }
        });
        return fragment_view ;
    }


    private List<HashMap<String,Object>> getListData()
    {
        List<HashMap<String,Object>> building_list = new ArrayList<HashMap<String, Object>>() ;
        for(int i = 0 ; i < building_name.length ; i++)
        {
            HashMap<String , Object> building_info = new HashMap<String, Object>() ;
            building_info.put("building_name" , building_name[i]) ;
            building_info.put("building_pic",R.drawable.buildings) ;
            building_list.add(building_info) ;
        }
        return building_list ;
    }

    public boolean changeCurrentClassRoomFragment(int id)
    {
        if (getActivity() instanceof currentBuildingChange)
        {
            current_list_item_id_ = (int)id ;
            ((currentBuildingChange) getActivity()).changeCurrentClassRoom(id);
            return true ;
        }
        return false ;
    }
}
