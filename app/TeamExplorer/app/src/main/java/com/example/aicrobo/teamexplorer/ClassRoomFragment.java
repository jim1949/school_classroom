//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE

package com.example.aicrobo.teamexplorer;

//don't use this file , fuck and bad code ........
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.widget.SwipeRefreshLayout;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.example.aicrobo.teamexplorer.RoomListAdapter ;


/**
 * A simple {@link Fragment} subclass.
 */
public class ClassRoomFragment extends Fragment {


    private long current_building_id = 0 ;
    private SwipeRefreshLayout refreshLayout_ ;
    private ListView listView_ ;
    private SimpleAdapter adapter_ ;

    private String[] current_show_classroom ;

    private String[] A_classroom = {
            "A101","A105","A103","A106",
            "A201","A205","A202","A206",
            "A301","A305","A302","A306"
    } ;

    private int[] A_classroom_num ;

    private String[] B_classroom = {
            "B103","B105","B102","B106",
            "B203","B205","B207","B209","B211","B202","B206","B208","B210","B212","B214","B216",
            "B303","B305","B307","B309","B311","B302","B306","B308","B310","B312","B314","B316"
    } ;


    private int[] B_classroom_num ;
    private String[] C_classroom = {
            "C203","C205","C207","C209","C211","C202","C206","C208","C210","C212","C214","C216",
            "C303","C305","C307","C309","C311","C302","C306","C308","C310","C312","C314","C316"
    };


    private int[] C_classroom_num ;
    private String[] D_classroom = {
            "D101","D105","D102","D106",
            "D201","D205","D202","D206",
            "D301","D305","D302","D306"
    } ;


    private int[] D_classroom_num ;
    private Map<String , Object> building_A_classroom_ ;
    private Map<String , Object> building_B_classroom_ ;
    private Map<String , Object> building_C_classroom_ ;
    private Map<String , Object> building_D_classroom_ ;

    public ClassRoomFragment() {
        // Required empty public constructor
    }


    public interface ClassRoomManipulation
    {
        void getRefresh() ;
        void getClassRoomImage(String current_classroom_name) ;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View class_room_view = inflater.inflate(R.layout.fragment_class_room, container, false);
        refreshLayout_ = (SwipeRefreshLayout)class_room_view.findViewById(R.id.swipe_refresh_widget) ;
        listView_ = (ListView)class_room_view.findViewById(R.id.listView_classroom) ;
        initClassRoomInfo();

        adapter_ = new SimpleAdapter(getActivity(),
                getBuildingA(),
                R.layout.list_item_layout,
                new String[]{"classroom_name","classroom_num","image"},
                new int[]{R.id.text_classroom_name,R.id.text_classroom_num,R.id.view_classroom}) ;
        listView_.setAdapter(adapter_);
        current_show_classroom = A_classroom ;
        refreshLayout_.setColorSchemeColors(R.color.red, R.color.green, R.color.blue);

        refreshLayout_.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                if (getActivity() instanceof ClassRoomManipulation) {
                    System.out.println("refreshing");
                    ((ClassRoomManipulation) getActivity()).getRefresh();
                }
                System.out.println("refreshing over");
                refreshLayout_.setRefreshing(false);
            }
        });

        listView_.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String click_classroom_name_ = current_show_classroom[(int)id] ;
                if(getActivity() instanceof ClassRoomFragment.ClassRoomManipulation)
                {
                    ((ClassRoomManipulation) getActivity()).getClassRoomImage(click_classroom_name_);
                }
            }
        });
        return class_room_view ;
    }

    public long getCurrentClassRoomId()
    {
        System.out.println("the current building id is!!! " + current_building_id);
        return current_building_id ;
    }


    public void setCurrentBuilding(long id)
    {
        current_building_id = id ;
        if(current_building_id==0) {
            System.out.println("get A building");
            adapter_ = new SimpleAdapter(getActivity(),
                    getBuildingA(),
                    R.layout.list_item_layout,
                    new String[]{"classroom_name","classroom_num","image"},
                    new int[]{R.id.text_classroom_name,R.id.text_classroom_num,R.id.view_classroom}) ;
            listView_.setAdapter(adapter_);
            current_show_classroom = A_classroom ;
        }
        else if (current_building_id==1)
        {
            System.out.println("get B building");
            adapter_ = new SimpleAdapter(getActivity(),
                    getBuildingB(),
                    R.layout.list_item_layout,
                    new String[]{"classroom_name","classroom_num","image"},
                    new int[]{R.id.text_classroom_name,R.id.text_classroom_num,R.id.view_classroom}) ;
            listView_.setAdapter(adapter_);
            current_show_classroom = B_classroom ;
        }
        else if (current_building_id==2)
        {
            System.out.println("get C building");
            adapter_ = new SimpleAdapter(getActivity(),
                    getBuildingC(),
                    R.layout.list_item_layout,
                    new String[]{"classroom_name","classroom_num","image"},
                    new int[]{R.id.text_classroom_name,R.id.text_classroom_num,R.id.view_classroom}) ;
            listView_.setAdapter(adapter_);
            current_show_classroom = C_classroom ;
        }
        else if(current_building_id==3)
        {
            System.out.println("get D building");
            adapter_ = new SimpleAdapter(getActivity(),
                    getBuildingD(),
                    R.layout.list_item_layout,
                    new String[]{"classroom_name","classroom_num","image"},
                    new int[]{R.id.text_classroom_name,R.id.text_classroom_num,R.id.view_classroom}) ;
            listView_.setAdapter(adapter_);
            current_show_classroom = D_classroom ;
        }
    }

    private List<HashMap<String,Object>> getBuildingD()
    {
        List<HashMap<String , Object>> buildingD = new ArrayList<HashMap<String, Object>>() ;
        for(int i = 0 ; i < D_classroom.length ; i++)
        {
            HashMap<String , Object> map_classroom = new HashMap<String,  Object>() ;
            map_classroom.put("classroom_name",D_classroom[i]) ;
            if(building_D_classroom_.get(D_classroom[i])!=null) {
                map_classroom.put("classroom_num", building_D_classroom_.get(D_classroom[i]) + "/80");
                int current_people = 0 ;
                current_people = Integer.parseInt(building_D_classroom_.get(D_classroom[i]).toString()) ;
                map_classroom.put("image",getClassRoomInfo(current_people)) ;
            }
            else
            {
                map_classroom.put("classroom_num", 0+ "/80");
                map_classroom.put("image",R.drawable.star_pink) ;
            }
            buildingD.add(map_classroom)  ;
        }
        return buildingD ;
    }

    private List<HashMap<String,Object>> getBuildingC()
    {
        List<HashMap<String , Object>> buildingC = new ArrayList<HashMap<String, Object>>() ;
        for(int i = 0 ; i < C_classroom.length ; i++)
        {
            HashMap<String , Object> map_classroom = new HashMap<String,  Object>() ;
            map_classroom.put("classroom_name",C_classroom[i]) ;
            if( building_C_classroom_.get(C_classroom[i])!=null) {
                map_classroom.put("classroom_num", building_C_classroom_.get(C_classroom[i]) + "/50");
                int current_people = 0 ;
                current_people = Integer.parseInt(building_C_classroom_.get(C_classroom[i]).toString()) ;
                map_classroom.put("image",getClassRoomInfo(current_people)) ;
            }
            else
            {
                map_classroom.put("classroom_num", 0 + "/50");
                map_classroom.put("image",R.drawable.star_pink) ;
            }
            buildingC.add(map_classroom)  ;
        }
        return buildingC ;
    }

    private List<HashMap<String,Object>> getBuildingB()
    {
        List<HashMap<String , Object>> buildingB = new ArrayList<HashMap<String, Object>>() ;
        for(int i = 0 ; i < B_classroom.length ; i++) {
            HashMap<String, Object> map_classroom = new HashMap<String, Object>();
            map_classroom.put("classroom_name", B_classroom[i]);
            if (building_B_classroom_.get(B_classroom[i]) != null) {
                map_classroom.put("classroom_num", building_B_classroom_.get(B_classroom[i]) + "/50");
                int current_people = 0 ;
                current_people = Integer.parseInt(building_B_classroom_.get(B_classroom[i]).toString()) ;
                map_classroom.put("image",getClassRoomInfo(current_people)) ;
            }
            else
            {
                map_classroom.put("classroom_num",0 + "/50");
                map_classroom.put("image",R.drawable.star_pink) ;
            }
            buildingB.add(map_classroom) ;
        }
        return buildingB ;
    }

    private List<HashMap<String,Object>> getBuildingA()
    {
        List<HashMap<String , Object>> buildingA = new ArrayList<HashMap<String, Object>>() ;
        for(int i = 0 ; i < A_classroom.length ; i++)
        {
            HashMap<String , Object> map_classroom = new HashMap<String,  Object>() ;
            map_classroom.put("classroom_name",A_classroom[i]);
            if(building_A_classroom_.get(A_classroom[i]) != null) {

                map_classroom.put("classroom_num", building_A_classroom_.get(A_classroom[i]) + "/50");
                int current_people = 0 ;
                current_people = Integer.parseInt(building_A_classroom_.get(A_classroom[i]).toString()) ;
                map_classroom.put("image",getClassRoomInfo(current_people)) ;
            }
            else
            {
                map_classroom.put("classroom_num", 0 + "/50");
                map_classroom.put("image",R.drawable.star_pink) ;
            }
            buildingA.add(map_classroom) ;
        }
        return buildingA ;
    }

    public boolean setBuildingAclassroom(HashMap<String , Object> classroom_info)
    {
        building_A_classroom_ = classroom_info ;
        return true ;
    }

    public boolean setBuildingBclassroom(HashMap<String , Object> classroom_info)
    {
        building_B_classroom_ = classroom_info ;
        return true ;
    }
    public boolean setBuildingCclassroom(HashMap<String , Object> classroom_info)
    {
        building_C_classroom_ = classroom_info ;
        return true ;
    }
    public boolean setBuildingDclassroom(HashMap<String , Object> classroom_info)
    {
        building_D_classroom_ = classroom_info ;
        return true ;
    }

    public void initClassRoomInfo()
    {
        building_A_classroom_ = new HashMap<String, Object>() ;
        building_B_classroom_ = new HashMap<String, Object>() ;
        building_C_classroom_ = new HashMap<String, Object>() ;
        building_D_classroom_ = new HashMap<String, Object>() ;
        A_classroom_num = new int[A_classroom.length] ;
        B_classroom_num = new int[B_classroom.length] ;
        C_classroom_num = new int[C_classroom.length] ;
        D_classroom_num = new int[D_classroom.length] ;

        for(int i = 0 ; i < A_classroom.length ; i++)
        {
            building_A_classroom_.put(A_classroom[i] , 0) ;
            A_classroom_num[i] = 0 ;
        }
        for(int i = 0 ; i < B_classroom.length ; i++)
        {
            building_B_classroom_.put(B_classroom[i] , 0 ) ;
            B_classroom_num[i] = 0 ;
        }
        for(int i = 0 ; i < C_classroom.length ; i++)
        {
            building_C_classroom_.put(C_classroom[i] , 0) ;
            C_classroom_num[i] = 0 ;
        }
        for(int i = 0 ; i < D_classroom.length ; i++)
        {
            building_D_classroom_.put(D_classroom[i],0) ;
            D_classroom_num[i] = 0 ;
        }
    }

    private int getClassRoomInfo(int current_people)
    {
        int drawable_id = 0 ;
        if(current_people<=15) drawable_id = R.drawable.star_gold ;
        else if(current_people<=30) drawable_id = R.drawable.star_blue ;
        else drawable_id = R.drawable.star_pink ;
        return drawable_id ;
    }

}
