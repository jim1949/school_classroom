
//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE


package com.example.aicrobo.teamexplorer;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.View;
import android.view.Window;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.example.aicrobo.teamexplorer.SchoolHttpClient ;

public class ActivityClassRoom extends AppCompatActivity {

    private ProgressBar image_progressBar_ ;
    private TextView classroom_name_ ;
    private TextView current_network_state_ ;
    private ImageView classroom_image_ ;
    private SchoolHttpClient image_client_ ;
    private String current_classroom_name ;
    private Bitmap current_classroom_pic_ ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = this.getIntent() ;
        current_classroom_name = intent.getStringExtra("classroom_name") ;
        setContentView(R.layout.activity_activity_class_room);
        initView();
        image_client_ = new SchoolHttpClient() ;
    }

    private void initView()
    {
        image_progressBar_ = (ProgressBar)findViewById(R.id.progress_image) ;
        classroom_name_ = (TextView)findViewById(R.id.textview_classroom) ;
        current_network_state_ = (TextView)findViewById(R.id.textview_imagenetwork) ;
        classroom_image_ = (ImageView)findViewById(R.id.imageview_classroom)  ;
        classroom_name_.setText(current_classroom_name);
        current_network_state_.setText("正在接收图片");
        image_progressBar_.setVisibility(View.VISIBLE);
        new Thread(getImage).start();
    }

    Handler imageShow_handler = new Handler()
    {
        @Override
        public void handleMessage(Message msg) {
            image_progressBar_.setVisibility(View.INVISIBLE);
            System.out.print(msg.arg1);
            if(msg.arg1==MessgeType.NETWORK_PROBLEM)
            {
                Toast.makeText(getApplicationContext(), "请检查网络连接", Toast.LENGTH_LONG).show();
            }
            else if(msg.arg1==MessgeType.GET_IAMGE)
            {
                if(image_client_.getImage().equals("False"))
                {
                    current_network_state_.setText("没有当前教室图像");
                    Toast.makeText(getApplicationContext(), "没有当前教室图像", Toast.LENGTH_LONG).show();
                }
                else
                {
                    //decode the byte data from python server
                    current_network_state_.setText("少侠请接好^_^");
                    byte[] bitmapArray = Base64.decode(image_client_.getImage(),Base64.DEFAULT) ;
                    current_classroom_pic_ = BitmapFactory.decodeByteArray(bitmapArray,0,bitmapArray.length) ;
                    classroom_image_.setImageBitmap(current_classroom_pic_);
                }
            }
            else if(msg.arg1 == MessgeType.NO_IMAGE)
            {
                current_network_state_.setText("没有当前教室图像");
                Toast.makeText(getApplicationContext(), "没有当前教室图像", Toast.LENGTH_LONG).show();
            }
            super.handleMessage(msg);
        }
    };

    Runnable getImage = new Runnable() {
        @Override
        public void run() {
            Message message = new Message() ;
            boolean result =  image_client_.getClassRoomImage(current_classroom_name) ;
            if (!image_client_.getNetworkInfomation()) message.arg1 = MessgeType.NETWORK_PROBLEM ;
            else if(result)message.arg1 = MessgeType.GET_IAMGE ;
            else  message.arg1 = MessgeType.NO_IMAGE ;
            imageShow_handler.sendMessage(message) ;
        }
    } ;
}
