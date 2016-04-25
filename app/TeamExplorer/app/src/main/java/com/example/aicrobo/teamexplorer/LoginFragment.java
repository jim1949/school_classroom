//author : rescuer liao
//NWPU TEAM EXPLORER
//BSD LISENCE

package com.example.aicrobo.teamexplorer;


import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;


/**
 * A simple {@link Fragment} subclass.
 */
public class LoginFragment extends Fragment {

    private boolean isLogin_ ;
    private EditText editText_user_name_ ;
    private EditText editText_password_ ;
    private Button button_login_ ;

    public  interface login_check
    {
        void loginCheck(String user_name , String password) ;
    }

    public LoginFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View login_view = inflater.inflate(R.layout.fragment_login, container, false) ;
        editText_user_name_ = (EditText)login_view.findViewById(R.id.EditText_username) ;
        editText_password_ = (EditText)login_view.findViewById(R.id.EditText_password) ;
        button_login_ = (Button)login_view.findViewById(R.id.button_login) ;
        button_login_.setOnClickListener(OnLoginClicked);
        return login_view ;
    }


    public void setCurrentLoginState(boolean state)
    {
        isLogin_ = state ;
    }

    public boolean getCurrentLoginState()
    {
        return isLogin_ ;
    }

    private View.OnClickListener OnLoginClicked  = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if(getActivity() instanceof login_check)
            {
                String user_name = editText_user_name_.getText().toString() ;
                String password = editText_password_.getText().toString() ;
                ((login_check) getActivity()).loginCheck(user_name,password);
            }
        }
    } ;
}
