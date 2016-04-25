
//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE


package com.example.aicrobo.teamexplorer;

import java.io.DataOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URI;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Objects;


public class SchoolHttpClient {
    private URL serve_url_ ;
    private String serve_url_name_;
    private HttpURLConnection http_url_conn_ ;
    private  boolean current_network_infomation_ = false ;
    private String current_data_ ;

    public SchoolHttpClient(){
        serve_url_name_ = "http://10.24.31.179:8000/";
        serve_url_ = null ;
        http_url_conn_ = null ;
    }

    public boolean isClientConnect()
    {
        if(http_url_conn_==null)
        {
            return false ;
        }
        return true ;
    }


    public boolean closeConnect()
    {
        http_url_conn_.disconnect();
        return true ;
    }

    //check the user login
    public boolean loginCheck(String user_id , String user_passwd)
    {
        String current_server_url = String.format(serve_url_name_+"?user_id=%s&user_passwd=%s",user_id,user_passwd) ;
        System.out.println(current_server_url);
        sendMessage(current_server_url) ;
        return true ;
    }

    //get the image
    public boolean getClassRoomImage(String classroom_name)
    {
        String current_server_url = String.format(serve_url_name_+"?classroom_name=%s",classroom_name) ;
        return sendMessage(current_server_url) ;
    }


    //get classroom people info
    public boolean getCurrentClassRoomInfo(long build_id)
    {
        String current_server_url = String.format(serve_url_name_+"?build_id=%d",build_id) ;
        return sendMessage(current_server_url) ;
    }


    //all request is sent by this function
    private boolean sendMessage(String current_server_name)
    {
        try {
            current_network_infomation_ = false ;
            serve_url_ = new URL(current_server_name) ;
            http_url_conn_ = (HttpURLConnection)serve_url_.openConnection();
            http_url_conn_.setConnectTimeout(3000);
            if(http_url_conn_.getResponseCode()==200) {
                InputStream in = http_url_conn_.getInputStream();
                ByteArrayOutputStream os = new ByteArrayOutputStream();
                int len = 0;
                byte buffer[] = new byte[1024];
                while ((len = in.read(buffer)) != -1) {
                    os.write(buffer, 0, len);
                }
                in.close();
                os.close();
                String result = new String(os.toByteArray());
                System.out.println("the result is " + result);
                current_data_ = result  ;
                current_network_infomation_ = true ;
            }
        }catch (Exception e)
        {
            current_network_infomation_= false ;
            System.out.println("error");
            e.printStackTrace();
            return false ;
        }
        finally {

        }
        System.out.println("message send!");
        return true ;
    }

    //change the response string data to map<String , int>
    public HashMap<String,Object> getCurrentClassRoomNumInfo()
    {
        HashMap<String,Object> classroom_info_map =new HashMap<String, Object>() ;
        String current_data = current_data_.substring(1,current_data_.length()-1) ;
        System.out.println("current_data is"+current_data);
        String[] str_data = current_data.split(",") ;
        for(int i = 0 ; i < str_data.length ; i++)
        {
            System.out.println(str_data[i]);
            String[] classroom_info = str_data[i].split(":") ;
            for(int j = 0 ; j < classroom_info.length; j++)
            {
                if(j%2==0)
                {
                    classroom_info_map.put(getClassRoomName(classroom_info[j]),classroom_info[j+1].substring(1)) ;
                   // System.out.println(getClassRoomName(classroom_info[j])+"     "+classroom_info[j+1]);
                }
            }
        }
        return classroom_info_map ;
    }

    //check wether the user log in successful from the response data
    public boolean getLoginCheck() {
        if (current_data_.equals("True")) {
            System.out.println("true!!!");
            return true;
        } else {
            return false;

        }
    }

    //return response of image (string)
    public String getImage()
    {
        System.out.println(current_data_);
        return current_data_ ;
    }

    public boolean getNetworkInfomation()
    {
        return current_network_infomation_ ;
    }

    public String getClassRoomName(String origin_string)
    {
        String class_name ;
        class_name = origin_string.replace(" ","") ;
        class_name = class_name.replace("'","") ;
        System.out.println(class_name);
        return class_name ;
    }
}