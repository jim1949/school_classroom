#!/usr/bin/env python

#author rescuer liao

import urllib
import urlparse
from BaseHTTPServer import *
from BaseHTTPServer import BaseHTTPRequestHandler
import classroom_image
import database_operate
class SchoolClassRoom(BaseHTTPRequestHandler):
    def do_GET(self):
        self.getDataBaseInit()
        prased_path = urlparse.urlparse(self.path)
        print self.client_address
        print self.command
        print self.connection
        print prased_path.query
        current_command = prased_path.query
        message = self.praseCurrentRequest(str(current_command))
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        self.wfile.write(message)
        return


    def getDataBaseInit(self):
        self.database = database_operate.ClientDataBaseOperate()

    def getCurrentClassRoomInfo(self,building_id):
        if self.database.getCurrentDataBaseStatue() == False:
            self.database.connectDataBase()
        classroom_people_map = self.database.getClassRoomInfo(building_id)
        return classroom_people_map

    def checkUserLogin(self,user_name , user_password):
        if self.database.getCurrentDataBaseStatue() == False:
                self.database.connectDataBase()
        is_user_login = self.database.checkUserLogin(user_name,user_password)
        return is_user_login

    def praseCurrentRequest(self,current_request):
        request = current_request.split('&')
        message = ''
        if len(request)==1:
            if request[0].split('=')[0] == 'classroom_name':      #means to get the image
                image_file_read = classroom_image.ImageData()
                classroom_name = request[0].split('=')[1]
                print 'get image!'
                message = image_file_read.getImageData(classroom_name)
            else:                                                 #means to get the classroom_info
                build_id = request[0].split('=')[1]
                print build_id
                message = self.getCurrentClassRoomInfo(int(build_id))

        #response is user login
        elif len(request)==2:
            user_name = request[0].split('=')[1]
            user_password = request[1].split('=')[1]
            message = self.checkUserLogin(user_name,user_password)
        return message

if __name__=='__main__':
    sever = HTTPServer(('10.24.31.179',8000),SchoolClassRoom)
    print 'the server is start !'
    sever.serve_forever()
