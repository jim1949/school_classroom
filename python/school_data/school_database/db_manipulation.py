#author : rescuer Liao

import MySQLdb
import sql_query

class DataBaseManipulation(object):
    def __init__(self):
        print 'is connecting to the database......'
        try:
            self.database_conn = MySQLdb.connect(host='localhost',user='root',passwd='666666'
                                                 ,db='student',port=3306)
            self.cur = self.database_conn.cursor()

            self.sql_query_factory = sql_query.SqlQuery()
            self.database_conn.commit()

            print 'the databases connect successed!!!'
        except MySQLdb.Error,e:
            print 'the connection get some error %d: %s'%(e.args[0],e.args[1])

    def __del__(self):
        self.database_conn.commit()
        self.cur.close()
        self.database_conn.close()

    def insert_data(self,table_name , **data_list):
        query = self.sql_query_factory.get_insert_query(table_name,data_list.keys())

        self.cur.execute(query,data_list.values())
        self.database_conn.commit()
        print 'data insert successed!!'


    def insert_data(self,table_name,data_dict):
        query = self.sql_query_factory.get_insert_query(table_name,data_dict.keys())
        self.cur.execute(query,data_dict.values())
        self.database_conn.commit()
        print 'data insert successed!!'




    def update_table(self,table_name,update_data_dict,limit_data_dict):
        query = self.sql_query_factory.get_update_query(table_name,update_data_dict,limit_data_dict)

        self.database_conn.commit()
        print 'data update successed!!'


    def delete_data(self,table_name,**limit_dict):
        query = self.sql_query_factory.get_delete_query(table_name,limit_dict)

        self.cur.execute(query,limit_dict.values())
        self.database_conn.commit()
        print 'delete data successed!!'

    def delete_data(self,table_name , data_dict):
        query = self.sql_query_factory.get_delete_query(table_name,data_dict)

        self.cur.execute(query,data_dict.values())
        self.database_conn.commit()


    def get_data(self,table_name,select_data,where_limit_data_dict):
        query = self.sql_query_factory.get_select_query(table_name,select_data,where_limit_data_dict)


        self.cur.execute(query,where_limit_data_dict.values())
        print 'select the data successed!!!'

