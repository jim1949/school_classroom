from bzrlib.win32utils import get_user_name

__author__ = 'rescuer liao'


import MySQLdb
import collections

class ClientDataBaseOperate(object):
    def __init__(self):
        self.database_init = False

    def connectDataBase(self):
        print 'is connecting to the database......'
        try:
            self.database_conn = MySQLdb.connect(host='localhost',user='root',passwd='666666'
                                                 ,db='student',port=3306)
            self.cur = self.database_conn.cursor()
            self.database_init = True
            print 'the databases connect successed!!!'
        except MySQLdb.Error,e:
            self.database_init = False
            print 'the connection get some error %d: %s'%(e.args[0],e.args[1])

    def getClassRoomInfo(self, building_id):
        a = 'call check_building_classroominfo(%s)' % building_id
        self.cur.execute(a)
        result = self.cur.fetchall()
        classroom_info = dict()
        # make the classroom peoplenum data be the 'json'
        for row in result:
            classroom_info[row[0]] = int(row[1])
        return classroom_info

    def getCurrentDataBaseStatue(self):
        return self.database_init

    def checkUserLogin(self,user_name , user_password):
        query = 'call student_login(%s,%s)'%(user_name,user_password)
        self.cur.execute(query) #check the user login
        result = self.cur.fetchone()
        if result[0] == 1:
            return 'True'
        else :
            return 'False'

#test
if __name__ == '__main__':
    db = ClientDataBaseOperate()
    db.connectDataBase()
    db.checkUserLogin('2013302637','12345')

