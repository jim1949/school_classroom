//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE
//bad and fuck code......don not use.
//sqlite will be added to this project
//this only the beta
package com.example.aicrobo.teamexplorer;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment ;
import android.support.v4.app.FragmentTransaction ;
import android.support.v4.app.FragmentManager ;
import android.os.Bundle ;
import android.view.View ;
import android.view.Window ;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import java.util.HashMap;

import com.example.aicrobo.teamexplorer.MessgeType ;
import com.jeremyfeinstein.slidingmenu.lib.* ;
import com.jeremyfeinstein.slidingmenu.lib.app.SlidingFragmentActivity;
import com.example.aicrobo.teamexplorer.ClassRoomFragment ;
import com.example.aicrobo.teamexplorer.SchoolHttpClient ;
import com.example.aicrobo.teamexplorer.UserInfoFragment ;
import com.example.aicrobo.teamexplorer.LoginFragment ;
import com.example.aicrobo.teamexplorer.ClassTableFragment ;

public class MainActivity extends SlidingFragmentActivity implements LeftMenuFragment.currentBuildingChange,
    ClassRoomFragment.ClassRoomManipulation,
        LoginFragment.login_check

{
    private SchoolHttpClient schoolHttpClient ;
    private ClassRoomFragment classroom_fragment_ ;
    private UserInfoFragment user_fragment_ ;
    private TextView top_text_view_ ;
    private LeftMenuFragment left_menu_fragment ;
    private ClassTableFragment class_table_fragment ;
    private LoginFragment login_fragement ;
    private boolean isLogin = false ;
    private ProgressDialog login_wait_dialog_ ;
    private ProgressBar label_progress_ ;

    private HashMap<String,Object> classroom_info_A = null ;
    private HashMap<String,Object> classroom_info_B = null ;
    private HashMap<String,Object> classroom_info_C = null ;
    private HashMap<String,Object> classroom_info_D = null ;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main) ;
        label_progress_ = (ProgressBar)findViewById(R.id.progress_label) ;
        schoolHttpClient = new SchoolHttpClient() ;
        top_text_view_ = (TextView)findViewById(R.id.label_name) ;
        initSlidingMenu();
        initClassRoom();
        initProgressDialog();
        label_progress_.setVisibility(View.INVISIBLE);
    }

    public void initProgressDialog()
    {
        login_wait_dialog_ = new ProgressDialog(MainActivity.this) ;
        login_wait_dialog_.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        login_wait_dialog_.setMessage("正在登陆.......");
        login_wait_dialog_.setIndeterminate(false);
        login_wait_dialog_.setCancelable(true);
    }

    Handler mainHandler  = new Handler()
    {
        @Override
        public void handleMessage(Message msg) {
            label_progress_.setVisibility(View.INVISIBLE);
            if(login_wait_dialog_.isShowing())
            {
                login_wait_dialog_.cancel();
            }
            if(msg.arg1==MessgeType.NETWORK_PROBLEM)
            {
                Toast.makeText(getApplicationContext(),"请检查网络连接",Toast.LENGTH_LONG).show();
            }
            else if(msg.arg1 == MessgeType.LOGIN_SUCCESSED)
            {
                System.out.println("successed");
                isLogin = true ;
                login_fragement.setCurrentLoginState(true);
                setMidFragment(1);
                Toast.makeText(getApplication() , "登陆成功",Toast.LENGTH_LONG).show();
            }
            else if(msg.arg1 ==  MessgeType.LOGIN_FAILED)
            {
                System.out.println("failed");
                isLogin = false ;
                login_fragement.setCurrentLoginState(false);
                Toast.makeText(getApplication() , "请检查用户名或密码",Toast.LENGTH_LONG).show();
            }
            else {
                setCurrentClassRoomInfo(msg.arg1, schoolHttpClient.getCurrentClassRoomNumInfo());
                left_menu_fragment.changeCurrentClassRoomFragment(msg.arg1);
            }
            super.handleMessage(msg);
        }
    } ;


    //the left menu change the building so change the classroom show in the middle fragment
    @Override
    public void changeCurrentClassRoom(long current_build_id) {
        classroom_fragment_.setCurrentBuilding(current_build_id);
    }

    //refresh the people num of each classroom showing in the middle fragment
    @Override
    public void getRefresh() {
        label_progress_.setVisibility(View.VISIBLE);
        new Thread(getCurrentRoomInfo).start();
    }

    //get image of the classroom choosed by user
    @Override
    public void getClassRoomImage(String current_classroom_name) {
        Intent intent = new Intent() ;
        intent.setClass(MainActivity.this,ActivityClassRoom.class) ;
        Bundle bundle = new Bundle() ;
        bundle.putString("classroom_name",current_classroom_name);
        intent.putExtras(bundle) ;
        this.startActivity(intent);
    }


    private void initSlidingMenu()
    {
        setBehindContentView(R.layout.left_menu_layout);
        left_menu_fragment = new LeftMenuFragment() ;
        FragmentTransaction fragmentTransaction = getSupportFragmentManager().beginTransaction() ;
        fragmentTransaction.replace(R.id.id_left_menu,left_menu_fragment) ;
        fragmentTransaction.commit() ;
        SlidingMenu main_sliding_menu = getSlidingMenu() ;
        main_sliding_menu.setMode(SlidingMenu.LEFT);
        main_sliding_menu.setShadowDrawable(R.drawable.shadow);
        main_sliding_menu.setTouchModeAbove(SlidingMenu.TOUCHMODE_FULLSCREEN);
        main_sliding_menu.setShadowWidthRes(R.dimen.shadow_width);
        main_sliding_menu.setBehindOffsetRes(R.dimen.slidingmenu_offset);
        main_sliding_menu.setFadeDegree(0.35f);
    }

    private void initClassRoom()
    {
        setMidFragment(0);
    }

    //thread of updating the people num
    Runnable getCurrentRoomInfo = new Runnable() {
        @Override
        public void run() {
            System.out.println("the current classroom id is "+classroom_fragment_.getCurrentClassRoomId()) ;
            schoolHttpClient.getCurrentClassRoomInfo(classroom_fragment_.getCurrentClassRoomId()) ;
            Message msg = new Message() ;
            if(schoolHttpClient.getNetworkInfomation())
            {
                int current_item_id = left_menu_fragment.getCurrentListItemId();
                msg.arg1 = current_item_id ;
            }
            else
            {
                msg.arg1 = MessgeType.NETWORK_PROBLEM ;
            }
            mainHandler.sendMessage(msg) ;
        }
    };


    //thread of checking login
    @Override
    public void loginCheck(String user_name, String password) {
        label_progress_.setVisibility(View.VISIBLE);
        UserLoginCheck userLoginCheck = new UserLoginCheck() ;
        userLoginCheck.setUserName(user_name);
        userLoginCheck.setUserPassWord(password);
        login_wait_dialog_.show();
        userLoginCheck.start();
    }

    //click listener of the button on the top left
    public void onButtonShowMenu(View view)
    {
        getSlidingMenu().showMenu();
    }

    //click listener of the button on bottom middle
    public void onButtonShowPersonInfo(View view)
    {
        setMidFragment(1);
        top_text_view_.setText("学霸的个人信息");
    }

    //click listener of the button on bottom right
    public void onButtonShowTable(View view)
    {
        setMidFragment(2);
        top_text_view_.setText("我的课程表");
    }

    //click listener of the button on bottom left
    public void onButtonShowClassRoom(View view)
    {
        setMidFragment(0);
        top_text_view_.setText("工大自习室");
    }

    private void setMidFragment(int id)
    {
        FragmentManager fm = getSupportFragmentManager() ;
        FragmentTransaction fragmentTransaction = fm.beginTransaction() ;
        switch (id)
        {
            case 0 :
                if(classroom_fragment_==null)classroom_fragment_ = new ClassRoomFragment() ;
                fragmentTransaction.replace(R.id.class_room_fragment,classroom_fragment_) ;
                break;
            case 1 :
                if (isLogin==false)

                {
                    if(login_fragement == null)login_fragement = new LoginFragment() ;
                    fragmentTransaction.replace(R.id.class_room_fragment,login_fragement) ;
                }
                else {
                    if (user_fragment_ == null) user_fragment_ = new UserInfoFragment();
                    fragmentTransaction.replace(R.id.class_room_fragment, user_fragment_);
                }
                break;
            case 2:
                if(class_table_fragment==null)class_table_fragment = new ClassTableFragment() ;
                fragmentTransaction.replace(R.id.class_room_fragment, class_table_fragment) ;
                break;
        }
        fragmentTransaction.commit() ;

    }

    //set people num of the classroom showing in the middle fragment
    void setCurrentClassRoomInfo(int id,HashMap<String,Object> classroom_info)
    {
        switch (id)
        {
            case 0:
                if(classroom_info_A!=classroom_info)classroom_info_A = classroom_info ;
                classroom_fragment_.setBuildingAclassroom(classroom_info) ;
                break;
            case 1:
                if(classroom_info_B!=classroom_info)classroom_info_B = classroom_info ;
                classroom_fragment_.setBuildingBclassroom(classroom_info) ;
                break;
            case 2:
                if(classroom_info_C!=classroom_info)classroom_info_C = classroom_info ;
                classroom_fragment_.setBuildingCclassroom(classroom_info) ;
                break;
            case 3:
                if(classroom_info_D !=classroom_info)classroom_info_D = classroom_info ;
                classroom_fragment_.setBuildingDclassroom(classroom_info);
                break;
        }
    }


    class UserLoginCheck extends Thread
    {
        private String user_name_ ;
        private String password_ ;
        void setUserName(String user_name) {
            this.user_name_ = user_name ;
        }
        void setUserPassWord(String password)
        {
            this.password_ = password ;
        }
        @Override
        public void run() {
            schoolHttpClient.loginCheck(this.user_name_,this.password_) ;
            Message message = new Message() ;
            if(!schoolHttpClient.getNetworkInfomation())
            {
                message.arg1 = MessgeType.NETWORK_PROBLEM ;
            }
            if(schoolHttpClient.getLoginCheck())
            {
                message.arg1 = MessgeType.LOGIN_SUCCESSED ;
            }
            else
            {
                message.arg1 = MessgeType.LOGIN_FAILED ;
            }
            mainHandler.sendMessage(message) ;
        }
    }
}
